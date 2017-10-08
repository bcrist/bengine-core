#include "pch.hpp"
#include "service_event_dispatcher.hpp"
#include "logging.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
void ServiceInitDependencies<EventDispatcher>::operator()() {
   default_log();
}

} // be
