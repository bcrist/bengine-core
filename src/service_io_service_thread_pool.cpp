#include "pch.hpp"
#include "service_io_service_thread_pool.hpp"
#include "service_ids.hpp"
#include "service_helpers.hpp"

namespace be {

const std::initializer_list<Id> ServiceTraits<IoServiceThreadPool>::preemptive_ids = {
   Id(BEID_CORE_SERVICE_IO_SERVICE_THREAD_POOL_LOCAL),
   Id(BEID_CORE_SERVICE_IO_SERVICE_THREAD_POOL_NETWORK)
};

} // be
