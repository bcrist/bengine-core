#include "pch.hpp"
#include "service_log.hpp"
#include "service_helpers.hpp"
#include "service_io_service_thread_pool.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
bool default_log_available() {
   return check_service<Log>();
}

///////////////////////////////////////////////////////////////////////////////
Log& default_log() {
   return service<Log>();
}

} // be
