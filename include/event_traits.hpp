#pragma once
#ifndef BE_CORE_EVENT_TRAITS_HPP_
#define BE_CORE_EVENT_TRAITS_HPP_

#include "event_policies_event_type.hpp"
#include "event_policies_event_source.hpp"
#include "default_event_dispatcher.hpp"

namespace be {
namespace detail {

template <typename E> class DefaultEventDispatcher;

//////////////////////////////////////////////////////////////////////////////
template <typename E,
   typename EventSourcePolicy = EventSourceAuto<E>,
   typename EventTypePolicy = EventTypeAuto<E>,
   typename DispatcherTypePolicy = DefaultEventDispatcher<E>>

struct PolicyEventTraits {
   using dispatcher_type = DispatcherTypePolicy;
   using event_source_type = typename EventSourcePolicy::type;

   static event_source_type event_source(const E& e) {
      return EventSourcePolicy()(e);
   }

   static const EventType<E>& event_type(const E& e) {
      return EventTypePolicy()(e);
   }
};

} // be::detail

//////////////////////////////////////////////////////////////////////////////
template <typename E>
struct EventTraits : public detail::PolicyEventTraits<E> { };

} // be

#endif
