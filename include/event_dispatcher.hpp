#pragma once
#ifndef BE_CORE_EVENT_DISPATCHER_HPP_
#define BE_CORE_EVENT_DISPATCHER_HPP_

#include "id.hpp"
#include "event_type.hpp"
#include <unordered_map>
#include <vector>

namespace be {

template <typename E> struct EventTraits;

//////////////////////////////////////////////////////////////////////////////
class EventDispatcherBase {
public:
   virtual ~EventDispatcherBase() = default;
   virtual void fire_deferred() = 0;
};

//////////////////////////////////////////////////////////////////////////////
class EventDispatcher final {
public:
   template <typename E, typename F>
   void subscribe(Id callback_id, const E& event, F&& callback);

   template <typename E, typename F>
   Id subscribe(const E& event, F&& callback);

   template <typename E>
   void unsubscribe(Id callback_id, const E& event);
   template <typename E>
   void unsubscribe_all_callbacks(const E& event);
   template <typename E>
   void unsubscribe_all_sources(Id callback_id, const E& event);
   template <typename E>
   void unsubscribe_all_callbacks_and_sources(const E& event);

   void reset();

   template <typename E>
   void fire(const E& event);

   template <typename E, typename... Ts>
   void fire(const E& event, Ts&&... extra);

   template <typename E>
   void defer(E&& event);

   template <typename E, typename... Ts>
   void defer(E&& event, Ts&&... extra);

   void dispatch_deferred();

private:
   void inc_next_callback_id_();
   void add_deferred_dispatcher_(EventDispatcherBase& dispatcher);

   std::unordered_map<const detail::EventTypeBase*, std::unique_ptr<EventDispatcherBase>> dispatchers_;
   std::vector<EventDispatcherBase*> deferred_set_;
   Id next_callback_id_;
};

} // be

#include "event_dispatcher.inl"

#endif
