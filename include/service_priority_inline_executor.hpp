#pragma once
#ifndef BE_CORE_SERVICE_PRIORITY_INLINE_EXECUTOR_HPP_
#define BE_CORE_SERVICE_PRIORITY_INLINE_EXECUTOR_HPP_

#include "service.hpp"
#include "inline_executors.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct SuppressUndefinedService<PriorityInlineExecutor<PriorityType, Comparator>> : True { };

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct ServiceTraits<PriorityInlineExecutor<PriorityType, Comparator>> : ServiceTraits<> {
   using preemptive_default = no;
   using lazy_ids = yes;
};

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType, typename Comparator>
struct ServiceShutdown<PriorityInlineExecutor<PriorityType, Comparator>> {
   void operator()(std::unique_ptr<PriorityInlineExecutor<PriorityType, Comparator>>& service) {
      if (service) {
         service->shutdown();
      }
   }
};

} // be

#endif
