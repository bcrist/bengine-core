#pragma once
#ifndef BE_CORE_SERVICE_SIMPLE_INLINE_EXECUTOR_HPP_
#define BE_CORE_SERVICE_SIMPLE_INLINE_EXECUTOR_HPP_

#include "service.hpp"
#include "inline_executors.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<SimpleInlineExecutor> : True { };

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceTraits<SimpleInlineExecutor> : ServiceTraits<> {
   using preemptive_default = no;
   using lazy_ids = yes;
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceName<SimpleInlineExecutor> {
   const char* operator()() {
      return "SimpleInlineExecutor";
   }
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceShutdown<SimpleInlineExecutor> {
   void operator()(std::unique_ptr<SimpleInlineExecutor>& service) {
      if (service) {
         service->shutdown();
      }
   }
};

} // be

#endif
