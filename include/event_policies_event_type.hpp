#pragma once
#ifndef BE_CORE_EVENT_POLICIES_EVENT_TYPE_HPP_
#define BE_CORE_EVENT_POLICIES_EVENT_TYPE_HPP_

#include "event_type.hpp"

namespace be::detail {

//////////////////////////////////////////////////////////////////////////////
template <typename E>
struct EventTypeDefault final {
   const EventType<E>& operator()(const E&) {
      static EventType<E> type;
      return type;
   }
};

//////////////////////////////////////////////////////////////////////////////
template <typename E>
struct EventTypeFromRefField final {
   const EventType<E>& operator()(const E& e) {
      return e.event_type;
   }
};

//////////////////////////////////////////////////////////////////////////////
template <typename E>
struct EventTypeFromRefFunc final {
   const EventType<E>& operator()(const E& e) {
      return e.event_type();
   }
};

//////////////////////////////////////////////////////////////////////////////
template <typename E>
struct EventTypeFromPtrField final {
   const EventType<E>& operator()(const E& e) {
      auto ptr = e.event_type;
      if (ptr) {
         return *ptr;
      } else {
         return EventTypeDefault<E>()(e);
      }
   }
};

//////////////////////////////////////////////////////////////////////////////
template <typename E>
struct EventTypeFromPtrFunc final {
   const EventType<E>& operator()(const E& e) {
      auto ptr = e.event_type();
      if (ptr) {
         return *ptr;
      } else {
         return EventTypeDefault<E>()(e);
      }
   }
};

//////////////////////////////////////////////////////////////////////////////
template <typename E>
struct EventTypePicker {
   template <typename C>
   static auto test(void*) -> std::enable_if_t<std::is_convertible_v<decltype(std::declval<const C>().event_type()), const EventType<E>&>, EventTypeFromRefFunc<E>>;

   template <typename C>
   static auto test(void*) -> std::enable_if_t<std::is_convertible_v<decltype(std::declval<const C>().event_type), const EventType<E>&>, EventTypeFromRefField<E>>;

   template <typename C>
   static auto test(void*) -> std::enable_if_t<std::is_convertible_v<decltype(std::declval<const C>().event_type()), const EventType<E>*>, EventTypeFromPtrFunc<E>>;

   template <typename C>
   static auto test(void*) -> std::enable_if_t<std::is_convertible_v<decltype(std::declval<const C>().event_type), const EventType<E>*>, EventTypeFromPtrField<E>>;

   template <typename>
   static auto test(...) -> EventTypeDefault<E>;
};

//////////////////////////////////////////////////////////////////////////////
template <typename E>
using EventTypeAuto = decltype(EventTypePicker<E>::test<E>(nullptr));

#pragma endregion

} // be::detail

#endif
