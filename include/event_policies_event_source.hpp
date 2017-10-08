#pragma once
#ifndef BE_CORE_EVENT_POLICIES_EVENT_SOURCE_PTR_HPP_
#define BE_CORE_EVENT_POLICIES_EVENT_SOURCE_PTR_HPP_

namespace be::detail {

//////////////////////////////////////////////////////////////////////////////
template <typename E, typename T, T V>
struct EventSourceStatic final {
   using type = T;
   type operator()(const E&) {
      return V;
   }
};

//////////////////////////////////////////////////////////////////////////////
template <typename E, typename T>
struct EventSourceFromField final {
   using type = T;
   type operator()(const E& e) {
      return e.event_source;
   }
};

//////////////////////////////////////////////////////////////////////////////
template <typename E, typename T>
struct EventSourceFromFunc final {
   using type = T;
   type operator()(const E& e) {
      return e.event_source();
   }
};

//////////////////////////////////////////////////////////////////////////////
template <typename E>
struct EventSourceAutoPicker {
   template <typename C>
   static auto test(void*) -> EventSourceFromFunc<E, std::decay_t<decltype(std::declval<const C>().event_source())>>;

   template <typename C>
   static auto test(void*) -> EventSourceFromField<E, std::decay_t<decltype(std::declval<const C>().event_source)>>;

   template <typename>
   static auto test(...) -> EventSourceStatic<E, int, 0>;
};

//////////////////////////////////////////////////////////////////////////////
template <typename E>
using EventSourceAuto = decltype(EventSourceAutoPicker<E>::test<E>(nullptr));

} // be::detail

#endif
