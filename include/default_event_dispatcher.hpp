#pragma once
#ifndef BE_CORE_DEFAULT_EVENT_DISPATCHER_HPP_
#define BE_CORE_DEFAULT_EVENT_DISPATCHER_HPP_

#include "event_dispatcher.hpp"
#include "t_unordered_remove.hpp"
#include <functional>

namespace be::detail {

//////////////////////////////////////////////////////////////////////////////
template <typename E>
class DefaultEventDispatcher final : public EventDispatcherBase {
public:
   using event_source_type = typename EventTraits<E>::event_source_type;
   using callback_func = std::function<void(const E&)>;

   DefaultEventDispatcher(const EventType<E>&) { }
   virtual ~DefaultEventDispatcher() = default;

   template <typename F>
   void subscribe(Id callback_id, F&& callback, event_source_type event_source);

   void unsubscribe(Id callback_id, event_source_type event_source);
   void unsubscribe_all_callbacks(event_source_type event_source);
   void unsubscribe_all_sources(Id callback_id);
   void unsubscribe_all();
   
   void fire(const E& event);
   void defer(E&& event);

   virtual void fire_deferred() override;

private:
   void sort_handlers_();

   struct handler {
      event_source_type event_source;
      Id callback_id;
      callback_func callback;
   };

   struct full_comp {
      bool operator()(const handler& a, const handler& b) {
         return a.event_source < b.event_source ||
               a.event_source == b.event_source && a.callback_id < b.callback_id;
      }
   };

   struct source_comp {
      bool operator()(const handler& a, const handler& b) {
         return a.event_source < b.event_source;
      }
   };

   bool handlers_sorted_ = true;
   std::vector<handler> handlers_;
   std::vector<E> deferred_events_;
};

} // be::detail

#include "default_event_dispatcher.inl"

#endif
