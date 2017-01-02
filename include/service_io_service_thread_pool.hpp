#pragma once
#ifndef BE_CORE_SERVICE_IO_SERVICE_THREAD_POOL_HPP_
#define BE_CORE_SERVICE_IO_SERVICE_THREAD_POOL_HPP_

#include "service.hpp"
#include "io_service_thread_pool.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<IoServiceThreadPool> : True { };

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceTraits<IoServiceThreadPool> : ServiceTraits<> {
   using preemptive_default = no;
   using lazy_ids = yes;
   static const std::initializer_list<Id> preemptive_ids;
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceName<IoServiceThreadPool> {
   const char* operator()() {
      return "IoServiceThreadPool";
   }
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceShutdown<IoServiceThreadPool> {
   void operator()(std::unique_ptr<IoServiceThreadPool>& service) {
      if (service) {
         service->threads(0);
      }
   }
};

} // be

#endif
