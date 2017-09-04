#include "pch.hpp"
#include "default_log.hpp"
#include "service_log.hpp"
#include "service_helpers.hpp"

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
