#include "pch.hpp"
#include "service_simple_single_thread_executor.hpp"
#include "service_ids.hpp"
#include "service_helpers.hpp"

namespace be {

const std::initializer_list<Id> ServiceTraits<SimpleSingleThreadExecutor>::preemptive_ids = {
   Id(BEID_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING)
};

} // be
