#if !defined(BE_CORE_EVENT_DISPATCHER_HPP_) && !defined(DOXYGEN)
#include "event_dispatcher.hpp"
#elif !defined(BE_CORE_EVENT_DISPATCHER_INL_)
#define BE_CORE_EVENT_DISPATCHER_INL_

namespace be {

//////////////////////////////////////////////////////////////////////////////
template <typename E, typename F>
void EventDispatcher::subscribe(Id callback_id, const E& event, F&& callback) {
   using dispatcher_type = typename EventTraits<E>::dispatcher_type;
   const EventType<E>& event_type = EventTraits<E>::event_type(event);

   auto it = dispatchers_.find(&event_type);
   if (it == dispatchers_.end()) {
      auto ptr = new dispatcher_type(event_type);
      auto uptr = std::unique_ptr<EventDispatcherBase>(ptr);
      std::tie(it, std::ignore) = dispatchers_.insert_or_assign(&event_type, std::move(uptr));
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*ptr);
      dispatcher.subscribe(callback_id, std::forward<F>(callback), EventTraits<E>::event_source(event));
   } else {
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*(it->second));
      dispatcher.subscribe(callback_id, std::forward<F>(callback), EventTraits<E>::event_source(event));
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E, typename F>
Id EventDispatcher::subscribe(const E& event, F&& callback) {
   Id id = next_callback_id_;
   inc_next_callback_id_();
   subscribe<E, F>(id, event, std::forward<F>(callback));
   return id;
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void EventDispatcher::unsubscribe(Id callback_id, const E& event) {
   using dispatcher_type = typename EventTraits<E>::dispatcher_type;
   const EventType<E>& event_type = EventTraits<E>::event_type(event);

   auto it = dispatchers_.find(&event_type);
   if (it != dispatchers_.end()) {
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*(it->second));
      dispatcher.unsubscribe(callback_id, EventTraits<E>::event_source(event));
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void EventDispatcher::unsubscribe_all_callbacks(const E& event) {
   using dispatcher_type = typename EventTraits<E>::dispatcher_type;
   const EventType<E>& event_type = EventTraits<E>::event_type(event);

   auto it = dispatchers_.find(&event_type);
   if (it != dispatchers_.end()) {
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*(it->second));
      dispatcher.unsubscribe_all_callbacks(EventTraits<E>::event_source(event));
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void EventDispatcher::unsubscribe_all_sources(Id callback_id, const E& event) {
   using dispatcher_type = typename EventTraits<E>::dispatcher_type;
   const EventType<E>& event_type = EventTraits<E>::event_type(event);

   auto it = dispatchers_.find(&event_type);
   if (it != dispatchers_.end()) {
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*(it->second));
      dispatcher.unsubscribe_all_sources(callback_id);
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void EventDispatcher::unsubscribe_all_callbacks_and_sources(const E& event) {
   using dispatcher_type = typename EventTraits<E>::dispatcher_type;
   const EventType<E>& event_type = EventTraits<E>::event_type(event);

   auto it = dispatchers_.find(&event_type);
   if (it != dispatchers_.end()) {
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*(it->second));
      dispatcher.unsubscribe_all();
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void EventDispatcher::fire(const E& event) {
   using dispatcher_type = typename EventTraits<E>::dispatcher_type;
   const EventType<E>& event_type = EventTraits<E>::event_type(event);

   auto it = dispatchers_.find(&event_type);
   if (it != dispatchers_.end()) {
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*(it->second));
      dispatcher.fire(event);
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E, typename... Ts>
void EventDispatcher::fire(const E& event, Ts&&... extra) {
   using dispatcher_type = typename EventTraits<E>::dispatcher_type;
   const EventType<E>& event_type = EventTraits<E>::event_type(event);

   auto it = dispatchers_.find(&event_type);
   if (it != dispatchers_.end()) {
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*(it->second));
      dispatcher.fire(event, std::forward<Ts>(extra)...);
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void EventDispatcher::defer(E&& event) {
   using dispatcher_type = typename EventTraits<E>::dispatcher_type;
   const EventType<E>& event_type = EventTraits<E>::event_type(event);

   auto it = dispatchers_.find(&event_type);
   if (it == dispatchers_.end()) {
      auto ptr = new dispatcher_type(event_type);
      auto uptr = std::unique_ptr<EventDispatcherBase>(ptr);
      std::tie(it, std::ignore) = dispatchers_.insert_or_assign(&event_type, std::move(uptr));
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*ptr);
      dispatcher.defer(std::forward<E>(event));
      add_deferred_dispatcher_(dispatcher);
   } else {
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*(it->second));
      dispatcher.defer(std::forward<E>(event));
      add_deferred_dispatcher_(dispatcher);
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E, typename ...Ts>
void EventDispatcher::defer(E&& event, Ts&&... extra) {
   using dispatcher_type = typename EventTraits<E>::dispatcher_type;
   const EventType<E>& event_type = EventTraits<E>::event_type(event);

   auto it = dispatchers_.find(&event_type);
   if (it == dispatchers_.end()) {
      auto ptr = new dispatcher_type(event_type);
      auto uptr = std::unique_ptr<EventDispatcherBase>(ptr);
      std::tie(it, std::ignore) = dispatchers_.insert_or_assign(&event_type, std::move(uptr));
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*ptr);
      dispatcher.defer(std::forward<E>(event), std::forward<Ts>(extra)...);
      add_deferred_dispatcher_(dispatcher);
   } else {
      dispatcher_type& dispatcher = static_cast<dispatcher_type&>(*(it->second));
      dispatcher.defer(std::forward<E>(event), std::forward<Ts>(extra)...);
      add_deferred_dispatcher_(dispatcher);
   }
}

} // be

#endif
