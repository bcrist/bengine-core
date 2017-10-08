#if !defined(BE_CORE_DEFAULT_EVENT_DISPATCHER_HPP_) && !defined(DOXYGEN)
#include "default_event_dispatcher.hpp"
#elif !defined(BE_CORE_DEFAULT_EVENT_DISPATCHER_INL_)
#define BE_CORE_DEFAULT_EVENT_DISPATCHER_INL_

namespace be::detail {

//////////////////////////////////////////////////////////////////////////////
template <typename E>
template <typename F>
void DefaultEventDispatcher<E>::subscribe(Id callback_id, F&& callback, event_source_type event_source) {
   static_assert(std::is_convertible_v<F&&, callback_func>, "Invalid callback signature; expected a functor with one parameter convertible from the event type.");
   handlers_.push_back(handler { event_source, callback_id, std::forward<F>(callback) });
   handlers_sorted_ = false;
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void DefaultEventDispatcher<E>::unsubscribe(Id callback_id, event_source_type event_source) {
   if (handlers_sorted_) {
      auto[first, last] = std::equal_range(handlers_.begin(), handlers_.end(),
         handler { event_source, callback_id }, full_comp());

      handlers_.erase(first, last);
      // handlers_ remains sorted
   } else {
      handlers_.erase(t::unordered_remove_if(handlers_.begin(), handlers_.end(),
         [=](const handler& h) {
            return h.callback_id == callback_id && h.event_source == event_source;
         }), handlers_.end());
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void DefaultEventDispatcher<E>::unsubscribe_all_callbacks(event_source_type event_source) {
   if (handlers_sorted_) {
      auto [first, last] = std::equal_range(handlers_.begin(), handlers_.end(),
         handler { event_source }, source_comp());

      handlers_.erase(first, last);
      // handlers_ remains sorted
   } else {
      handlers_.erase(t::unordered_remove_if(handlers_.begin(), handlers_.end(),
         [=](const handler& h) {
            return h.event_source == event_source;
         }), handlers_.end());
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void DefaultEventDispatcher<E>::unsubscribe_all_sources(Id callback_id) {
   if (handlers_sorted_) {
      // unfortunately being sorted doesn't help us here, since callback_id isn't the primary sort key.
      // we could use the same implementation as for unsorted and be slightly faster here, but then we'd
      // probably have to sort again later, and if we just keep sorted here we'll do less work overall.
      handlers_.erase(std::remove_if(handlers_.begin(), handlers_.end(),
         [=](const handler& h) {
            return h.callback_id == callback_id;
         }), handlers_.end());
      // handlers_ remains sorted
   } else {
      handlers_.erase(t::unordered_remove_if(handlers_.begin(), handlers_.end(),
         [=](const handler& h) {
            return h.callback_id == callback_id;
         }), handlers_.end());
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void DefaultEventDispatcher<E>::unsubscribe_all() {
   handlers_.clear();
   handlers_sorted_ = true;
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void DefaultEventDispatcher<E>::fire(const E& event) {
   sort_handlers_();

   auto [first, last] = std::equal_range(handlers_.begin(), handlers_.end(),
      handler { EventTraits<E>::event_source(event) }, source_comp());

   while (first != last) {
      first->callback(event);
      ++first;
   }
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void DefaultEventDispatcher<E>::defer(E&& event) {
   deferred_events_.push_back(std::forward<E>(event));
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void DefaultEventDispatcher<E>::fire_deferred() {
   sort_handlers_();

   for (auto& event : deferred_events_) {
      fire(event);
   }

   deferred_events_.clear();
}

//////////////////////////////////////////////////////////////////////////////
template <typename E>
void DefaultEventDispatcher<E>::sort_handlers_() {
   if (!handlers_sorted_) {
      std::sort(handlers_.begin(), handlers_.end(), full_comp());
      handlers_sorted_ = true;
   }
}

} // be::detail

#endif
