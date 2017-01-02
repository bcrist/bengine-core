#pragma once
#ifndef BE_CORE_SMALL_PAIR_HPP_
#define BE_CORE_SMALL_PAIR_HPP_

#include "be.hpp"

namespace be {

template <typename T, typename U, bool = std::is_empty<T>::value && !std::is_final<T>::value>
class SmallPair final : private T {
public:
   template <typename... P>
   explicit SmallPair(NoArgsTag, P&&... args)
      : T(),
        second_(std::forward<P>(args)...)
   { }

   template <typename O, typename... P>
   SmallPair(OneArgTag, O&& o, P&&... args)
      : T(std::forward<O>(o)),
        second_(std::forward<P>(args)...)
   { }

   T& first() noexcept              { return *this; }
   const T& first() const noexcept  { return *this; }

   U& second() noexcept             { return second_; }
   const U& second() const noexcept { return second_; }

private:
   U second_;
};

template <typename T, typename U>
class SmallPair<T, U, false> final {
public:
   template <typename... P>
   explicit SmallPair(NoArgsTag, P&&... args)
      : first_(),
        second_(std::forward<P>(args)...)
   { }

   template <typename O, typename... P>
   SmallPair(OneArgTag, O&& o, P&&... args)
      : first_(std::forward<O>(o)),
        second_(std::forward<P>(args)...)
   { }

   T& first() noexcept              { return first_; }
   const T& first() const noexcept  { return first_; }

   U& second() noexcept             { return second_; }
   const U& second() const noexcept { return second_; }

private:
   T first_;
   U second_;
};

} // be

#endif
