#pragma once
#ifndef BE_CORE_SERVICE_PRIORITY_SINGLE_THREAD_EXECUTOR_HPP_
#define BE_CORE_SERVICE_PRIORITY_SINGLE_THREAD_EXECUTOR_HPP_

#include "service.hpp"
#include "single_thread_executors.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct SuppressUndefinedService<PrioritySingleThreadExecutor<PriorityType, Comparator>> : True { };

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct ServiceTraits<PrioritySingleThreadExecutor<PriorityType, Comparator>> : ServiceTraits<> {
   using preemptive_default = no;
   using lazy_ids = yes;
};

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct ServiceShutdown<PrioritySingleThreadExecutor<PriorityType, Comparator>> {
   void operator()(std::unique_ptr<PrioritySingleThreadExecutor<PriorityType, Comparator>>& service) {
      if (service) {
         service->shutdown();
      }
   }
};

} // be

#endif
