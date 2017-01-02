#pragma once
#ifndef BE_CORE_SERVICE_MANAGER_HPP_
#define BE_CORE_SERVICE_MANAGER_HPP_

#include "log_attrib_ids.hpp"
#include "log_expr_ops.hpp"
#include "log_handle.hpp"
#include "service_log.hpp"
#include <unordered_map>
#include <vector>
#include <cassert>

namespace be {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Provides access to one or more services of the templated type, and
///         provides basic automatic lifetime management for them.
///
/// \details Multiple service objects can be retrieved by passing an Id to
///         get(Id).  Services are automatically constructed when necessary by
///         a call to a default-constructed template functor ServiceFactory.
///         The Id of the requested service is passed into the factory.
///
///         get() retrieves a default service.  The default service reports
///         Id(0) to the ServiceFactory on construction, but get() and
///         get(Id()) may not refer to the same service.
///
///         If ServiceTraits::PreempiveDefault is True, the default
///         service will be constructed automatically when the ServiceManager
///         is constructed.  This guarantees that calling
///         `service<ServiceType>()` from multiple threads will not cause a
///         race condition.  Named service objects can similarly be
///         preemptively constructed by placing their Ids in an iterable
///         object at ServiceTraits::preemptive_ids.
///
///         ServiceTraits::LazyDefault and ServiceTraits::LazyIds determine
///         if additional service objects can be constructed after the manager
///         has been created.
///
///         ServiceManagers can be setup to be preconstructed when the
///         Engine object is created if ServiceTraits::PreemptiveManager is
///         True.
///
///         ServiceManagers are never destroyed, but they are "cleaned up"
///         when the Engine object is destroyed.  By default, service
///         objects are destroyed when cleaning up.  The ServiceCleanup
///         functor can be specialized to altar this behavior.  It receives
///         a reference to the unique_ptr that owns the service object and
///         the service Id.
///
///         Thread local service types will only preemptively initialize
///         their manager on the main thread.  Similarly, the cleanup and
///         shutdown customization points may not be called until the
///         thread is stopping for threads other than the main thread.
template <typename ServiceType>
class ServiceManager final {
   using traits = ServiceTraits<ServiceType>;
   using init_deps = ServiceInitDependencies<ServiceType>;
   using name_func = ServiceName<ServiceType>;
   using factory_func = ServiceFactory<ServiceType>;
   using shutdown_func = ServiceShutdown<ServiceType>;
   using cleanup_func = ServiceCleanup<ServiceType>;

   using unique_service = std::unique_ptr<ServiceType>;
   using service_map = std::unordered_map<Id, unique_service>;
   using construction_order_list = std::vector<std::pair<Id, unique_service*>>;

public:
   ////////////////////////////////////////////////////////////////////////////
   ServiceManager() {
      static_assert(SuppressUndefinedService<ServiceType>::value, "Service not properly configured!");

      init_deps init_func;
      init_func();

      Log* log = log_();
      if (log) {
         be_short_debug() << "Initializing " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service manager..."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            || *log;
      }

      construct_(default_service_, typename traits::preemptive_default());

      for (Id id : traits::preemptive_ids) {
         construct_(named_services_[id], id);
      }
      
      if (log) {
         be_short_debug() << BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service manager initialized."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            || *log;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   ~ServiceManager() {
      if (!is_shutdown_) {
         shutdown();
         if (!is_cleaned_up_) {
            cleanup();
         }
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   explicit operator bool() {
      return !is_shutdown_ && !is_cleaned_up_;
   }

   ////////////////////////////////////////////////////////////////////////////
   ServiceType& get() {
      return get_(typename traits::lazy_default());
   }

   ////////////////////////////////////////////////////////////////////////////
   ServiceType& get(Id id) {
      return get_(id, typename traits::lazy_ids());
   }

   ////////////////////////////////////////////////////////////////////////////
   ServiceType* optional() {
      return default_service_.get();
   }

   ////////////////////////////////////////////////////////////////////////////
   ServiceType* optional(Id id) {
      auto it = named_services_.find(id);
      return it == named_services_.end() ? nullptr : it->second.get();
   }

   ////////////////////////////////////////////////////////////////////////////
   void shutdown() {
      assert(!is_shutdown_);
      is_shutdown_ = true;
      
      Log* log = log_();
      if (log) {
         be_short_debug() << "Shutting down " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service manager..."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            || *log;
      }
      
      for (auto it = construction_order_.rbegin(); it != construction_order_.rend(); ++it) {
         unique_service& service = *(it->second);
         if (service) {
            if (&service == &default_service_) {
               shutdown_();
            } else {
               shutdown_(service, it->first);
            }
         }
      }

      if (log) {
         be_short_debug() << BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service manager shut down."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            || *log;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   void cleanup() {
      assert(is_shutdown_);
      assert(!is_cleaned_up_);
      is_cleaned_up_ = true;

      Log* log = log_();
      if (log) {
         be_short_debug() << "Cleaning up " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service manager..."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            || *log;
      }

      for (auto it = construction_order_.rbegin(); it != construction_order_.rend(); ++it) {
         unique_service& service = *(it->second);
         if (service) {
            if (&service == &default_service_) {
               cleanup_();
            } else {
               cleanup_(service, it->first);
            }
         }
      }

      if (log) {
         be_short_debug() << BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service manager cleaned up."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            || *log;
      }
   }

private:
   ////////////////////////////////////////////////////////////////////////////
   ServiceType& get_(False) {
      return *default_service_;
   }

   ////////////////////////////////////////////////////////////////////////////
   ServiceType& get_(True) {
      if (!default_service_) {
         construct_(default_service_, True());
      }
      return *default_service_;
   }

   ////////////////////////////////////////////////////////////////////////////
   ServiceType& get_(Id id, False) {
      return *(named_services_.find(id)->second);
   }

   ////////////////////////////////////////////////////////////////////////////
   ServiceType& get_(Id id, True) {
      unique_service& service = named_services_[id];
      if (!service) {
         construct_(service, id);
      }
      return *service;
   }

   ////////////////////////////////////////////////////////////////////////////
   void construct_(unique_service&, False) { }
   void construct_(unique_service& ptr, True) {
      assert(!is_shutdown_);
      Log* log = log_();
      if (log) {
         be_short_debug() << "Constructing default " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service..."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            || *log;
      }
      factory_func factory;
      ptr = factory(Id());
      construction_order_.emplace_back(Id(), &ptr);
      if (log) {
         be_short_debug() << "Default " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service initialized."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            || *log;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   void construct_(unique_service& ptr, Id id) {
      assert(!is_shutdown_);
      Log* log = log_();
      if (log) {
         be_debug() << "Constructing " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service..."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            & attr(BEIDN_LOG_ATTR_SERVICE_ID) << id
            || *log;
      }
      factory_func factory;
      ptr = factory(id);
      construction_order_.emplace_back(id, &ptr);
      if (log) {
         be_debug() << BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service initialized."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            & attr(BEIDN_LOG_ATTR_SERVICE_ID) << id
            || *log;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   void shutdown_() {
      Log* log = log_();
      if (log) {
         be_short_debug() << "Shutting down default " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service..."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            || *log;
      }
      shutdown_func shutdown;
      shutdown(default_service_);
      if (log) {
         be_debug() << "Default " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service shut down."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            & attr(BEIDN_LOG_ATTR_DESTROYED) << !default_service_
            || *log;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   void shutdown_(unique_service& service, Id id) {
      Log* log = log_();
      if (log) {
         be_debug() << "Shutting down " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service..."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            & attr(BEIDN_LOG_ATTR_SERVICE_ID) << id
            || *log;
      }
      shutdown_func shutdown;
      shutdown(service);
      if (log) {
         be_debug() << BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service shut down."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            & attr(BEIDN_LOG_ATTR_SERVICE_ID) << id
            & attr(BEIDN_LOG_ATTR_DESTROYED) << !service
            || *log;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   void cleanup_() {
      Log* log = log_();
      if (log) {
         be_short_debug() << "Cleaning up default " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service..."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            || *log;
      }
      cleanup_func cleanup;
      cleanup(default_service_);
      if (log) {
         be_debug() << "Default " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service cleaned up."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            & attr(BEIDN_LOG_ATTR_DESTROYED) << !default_service_
            || *log;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   void cleanup_(unique_service& service, Id id) {
      Log* log = log_();
      if (log) {
         be_debug() << "Cleaning up " BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service..."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            & attr(BEIDN_LOG_ATTR_SERVICE_ID) << id
            || *log;
      }
      cleanup_func cleanup;
      cleanup(service);
      if (log) {
         be_debug() << BE_LOG_INTERP(BEIDN_LOG_ATTR_SERVICE_TYPE) " service cleaned up."
            & hidden(BEIDN_LOG_ATTR_SERVICE_TYPE) << name_func()()
            & attr(BEIDN_LOG_ATTR_SERVICE_ID) << id
            & attr(BEIDN_LOG_ATTR_DESTROYED) << !service
            || *log;
      }

   }

   Log* log_() const {
      return log_(typename std::is_same<ServiceType, Log>::type());
   }

   Log* log_(True) const {
      return nullptr;
   }

   Log* log_(False) const {
      return &service<Log>();
   }

   unique_service default_service_;
   service_map named_services_;
   construction_order_list construction_order_;
   bool is_shutdown_ = false;
   bool is_cleaned_up_ = false;
};

template <typename ServiceType>
ServiceManager<ServiceType>& get_service_manager(False) {
   static ServiceManager<ServiceType> manager;
   return manager;
}

template <typename ServiceType>
ServiceManager<ServiceType>& get_service_manager(True) {
   static thread_local ServiceManager<ServiceType> manager;
   return manager;
}

template <typename ServiceType>
ServiceManager<ServiceType>& get_service_manager() {
   return get_service_manager<ServiceType>(typename ServiceTraits<ServiceType>::thread_local_manager());
}

} // be::detail
} // be

#endif
