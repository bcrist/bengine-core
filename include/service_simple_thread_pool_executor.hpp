#pragma once
#ifndef BE_CORE_SERVICE_SIMPLE_THREAD_POOL_EXECUTOR_HPP_
#define BE_CORE_SERVICE_SIMPLE_THREAD_POOL_EXECUTOR_HPP_

#include "service.hpp"
#include "thread_pool_executors.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<SimpleThreadPoolExecutor> : True { };

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceTraits<SimpleThreadPoolExecutor> : ServiceTraits<> {
   using preemptive_default = no;
   using lazy_ids = yes;
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceName<SimpleThreadPoolExecutor> {
   const char* operator()() {
      return "SimpleSingleThreadExecutor";
   }
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceShutdown<SimpleThreadPoolExecutor> {
   void operator()(std::unique_ptr<SimpleThreadPoolExecutor>& service) {
      if (service) {
         service->shutdown();
      }
   }
};

} // be

#endif
