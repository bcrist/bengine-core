#pragma once
#ifndef BE_CORE_EVENT_TYPE_HPP_
#define BE_CORE_EVENT_TYPE_HPP_

#include "be.hpp"

namespace be {
namespace detail {

//////////////////////////////////////////////////////////////////////////////
class EventTypeBase : Immovable {
public:
   virtual ~EventTypeBase() noexcept = default;
   virtual SV name() const noexcept = 0;

   bool operator==(const EventTypeBase& other) const noexcept {
      return this == &other;
   }

   bool operator!=(const EventTypeBase& other) const noexcept {
      return this != &other;
   }

   bool operator<(const EventTypeBase& other) const noexcept {
      return this < &other;
   }

   bool operator<=(const EventTypeBase& other) const noexcept {
      return this <= &other;
   }

   bool operator>(const EventTypeBase& other) const noexcept {
      return this > &other;
   }

   bool operator>=(const EventTypeBase& other) const noexcept {
      return this >= &other;
   }
};

} // be::detail

//////////////////////////////////////////////////////////////////////////////
template <typename E>
class EventType final : public detail::EventTypeBase {
public:
   EventType() noexcept
      : name_(type_name<E>()) { }

   EventType(SV name) noexcept
      : name_(name) { }

   virtual SV name() const noexcept override {
      return name_;
   }

private:
   SV name_;
};

} // be

#endif
