#pragma once
#ifndef BE_CORE_SERVICE_THREAD_LOCAL_PRIORITY_INLINE_EXECUTOR_HPP_
#define BE_CORE_SERVICE_THREAD_LOCAL_PRIORITY_INLINE_EXECUTOR_HPP_

#include "service.hpp"
#include "inline_executor.hpp"
#include "work_queues.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct SuppressUndefinedService<InlineExecutor<PriorityWorkQueue<PriorityType, Comparator>>> : True { };

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct ServiceTraits<InlineExecutor<PriorityWorkQueue<PriorityType, Comparator>>> : ServiceTraits<> {
   using thread_local_manager = yes;
   using preemptive_default = no;
   using lazy_ids = yes;
};

} // be

#endif
