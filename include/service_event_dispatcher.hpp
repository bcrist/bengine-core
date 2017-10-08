#pragma once
#ifndef BE_CORE_SERVICE_EVENT_DISPATCHER_HPP_
#define BE_CORE_SERVICE_EVENT_DISPATCHER_HPP_

#include "service.hpp"
#include "event_dispatcher.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<EventDispatcher> : True { };

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceInitDependencies<EventDispatcher> {
   void operator()();
};

} // be

#endif
