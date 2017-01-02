#pragma once
#ifndef BE_CORE_SERVICE_THREAD_LOCAL_SIMPLE_INLINE_EXECUTOR_HPP_
#define BE_CORE_SERVICE_THREAD_LOCAL_SIMPLE_INLINE_EXECUTOR_HPP_

#include "service.hpp"
#include "inline_executor.hpp"
#include "work_queues.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<InlineExecutor<SimpleWorkQueue>> : True { };

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceTraits<InlineExecutor<SimpleWorkQueue>> : ServiceTraits<> {
   using thread_local_manager = yes;
   using preemptive_default = no;
   using lazy_ids = yes;
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceName<InlineExecutor<SimpleWorkQueue>> {
   const char* operator()() {
      return "InlineExecutor<SimpleWorkQueue>";
   }
};

} // be

#endif
