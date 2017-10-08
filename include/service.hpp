#pragma once
#ifndef BE_CORE_SERVICE_HPP_
#define BE_CORE_SERVICE_HPP_

#include "id.hpp"
#include <array>

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
struct SuppressUndefinedService : False { };

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType = void>
struct ServiceTraits {
   using yes = True;
   using no = False;

   // DO NOT CHANGE THESE HERE; SPECIALIZE ServiceTraits FOR YOUR TYPE!
   using thread_local_manager = no; ///< if yes, all services of this will be thread local
   using preemptive_default = yes; ///< if yes, default service will be constructed when manager is constructed.
   using lazy_default = no; ///< if yes, default service will be constructed when it is first requested.  If preemptive_default is yes, lazy_default should be no.
   using lazy_ids = no; ///< if yes, arbitrary services can be constructed when they are requested.  Otherwise only those specified by preemptive_ids will be constructed when the manager is initialized, and no further services can be created after the fact.
   static const std::array<Id, 0> preemptive_ids;
};

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
const std::array<Id, 0> ServiceTraits<ServiceType>::preemptive_ids;

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
struct ServiceName {
   SV operator()() {
     return type_name<ServiceType>();
   }
};

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
struct ServiceInitDependencies {
   void operator()() { }
};

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
struct ServiceFactory {
   std::unique_ptr<ServiceType> operator()(Id) {
      return std::make_unique<ServiceType>();
   }
};

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
struct ServiceShutdown {
   void operator()(std::unique_ptr<ServiceType>&) { }
};

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
struct ServiceCleanup {
   void operator()(std::unique_ptr<ServiceType>& service) {
      service.reset();
   }
};

} // be

#endif
