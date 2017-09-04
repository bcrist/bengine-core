#include "pch.hpp"
#include "void_log.hpp"
#include "service_log.hpp"
#include "service_helpers.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
bool void_log_available() {
   return check_service<Log>();
}

///////////////////////////////////////////////////////////////////////////////
Log& void_log() {
   return service<Log>(ids::core_service_log_void);
}

} // be
