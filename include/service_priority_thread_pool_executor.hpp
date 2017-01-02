#pragma once
#ifndef BE_CORE_SERVICE_PRIORITY_THREAD_POOL_EXECUTOR_HPP_
#define BE_CORE_SERVICE_PRIORITY_THREAD_POOL_EXECUTOR_HPP_

#include "service.hpp"
#include "thread_pool_executors.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct SuppressUndefinedService<PriorityThreadPoolExecutor<PriorityType, Comparator>> : True { };

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct ServiceTraits<PriorityThreadPoolExecutor<PriorityType, Comparator>> : ServiceTraits<> {
   using preemptive_default = no;
   using lazy_ids = yes;
};

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct ServiceShutdown<PriorityThreadPoolExecutor<PriorityType, Comparator>> {
   void operator()(std::unique_ptr<PriorityThreadPoolExecutor<PriorityType, Comparator>>& service) {
      if (service) {
         service->shutdown();
      }
   }
};

} // be

#endif
