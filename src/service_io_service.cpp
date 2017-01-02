#include "pch.hpp"
#include "service_io_service.hpp"
#include "service_helpers.hpp"

namespace be {

IoService& foreground_io_service() {
   return service<IoService>();
}

} // be
