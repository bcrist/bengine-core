#pragma once
#ifndef BE_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_HPP_
#define BE_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_HPP_

#include "service.hpp"
#include "single_thread_executors.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<SimpleSingleThreadExecutor> : True { };

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceTraits<SimpleSingleThreadExecutor> : ServiceTraits<> {
   using preemptive_default = no;
   using lazy_ids = yes;
   static const std::initializer_list<Id> preemptive_ids;
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceName<SimpleSingleThreadExecutor> {
   const char* operator()() {
      return "SimpleSingleThreadExecutor";
   }
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceShutdown<SimpleSingleThreadExecutor> {
   void operator()(std::unique_ptr<SimpleSingleThreadExecutor>& service) {
      if (service) {
         service->shutdown();
      }
   }
};

} // be

#endif
