#pragma once
#ifndef BE_CORE_SMALL_TRIPLET_HPP_
#define BE_CORE_SMALL_TRIPLET_HPP_

#include "be.hpp"

namespace be {

template <typename T, typename U, typename V,
   bool = std::is_empty<T>::value && !std::is_final<T>::value,
   bool = std::is_empty<U>::value && !std::is_final<U>::value>
class SmallTriplet final : private T, private U {
public:
   template <typename... Q>
   SmallTriplet(NoArgsTag, NoArgsTag, Q&&... args)
      : T(),
        U(),
        third_(std::forward<Q>(args)...)
   { }

   template <typename P, typename... Q>
   SmallTriplet(NoArgsTag, OneArgTag, P&& p, Q&&... args)
      : T(),
        U(std::forward<P>(p)),
        third_(std::forward<Q>(args)...)
   { }

   template <typename O, typename... Q>
   SmallTriplet(OneArgTag, NoArgsTag, O&& o, Q&&... args)
      : T(std::forward<O>(o)),
        U(),
        third_(std::forward<Q>(args)...)
   { }

   template <typename O, typename P, typename... Q>
   SmallTriplet(OneArgTag, OneArgTag, O&& o, P&& p, Q&&... args)
      : T(std::forward<O>(o)),
        U(std::forward<P>(p)),
        third_(std::forward<Q>(args)...)
   { }

   T& first() noexcept              { return *this; }
   const T& first() const noexcept  { return *this; }

   U& second() noexcept             { return *this; }
   const U& second() const noexcept { return *this; }

   V& third() noexcept              { return third_; }
   const V& third() const noexcept  { return third_; }

private:
   V third_;
};

template <typename T, typename U, typename V>
class SmallTriplet<T, U, V, true, false> final : private T {
public:
   template <typename... Q>
   SmallTriplet(NoArgsTag, NoArgsTag, Q&&... args)
      : T(),
        second_(),
        third_(std::forward<Q>(args)...)
   { }

   template <typename P, typename... Q>
   SmallTriplet(NoArgsTag, OneArgTag, P&& p, Q&&... args)
      : T(),
        second_(std::forward<P>(p)),
        third_(std::forward<Q>(args)...)
   { }

   template <typename O, typename... Q>
   SmallTriplet(OneArgTag, NoArgsTag, O&& o, Q&&... args)
      : T(std::forward<O>(o)),
        second_(),
        third_(std::forward<Q>(args)...)
   { }

   template <typename O, typename P, typename... Q>
   SmallTriplet(OneArgTag, OneArgTag, O&& o, P&& p, Q&&... args)
      : T(std::forward<O>(o)),
        second_(std::forward<P>(p)),
        third_(std::forward<Q>(args)...)
   { }

   T& first() noexcept              { return *this; }
   const T& first() const noexcept  { return *this; }

   U& second() noexcept             { return second_; }
   const U& second() const noexcept { return second_; }

   V& third() noexcept              { return third_; }
   const V& third() const noexcept  { return third_; }

private:
   U second_;
   V third_;
};

template <typename T, typename U, typename V>
class SmallTriplet<T, U, V, false, false> final : private T {
public:
   template <typename... Q>
   SmallTriplet(NoArgsTag, NoArgsTag, Q&&... args)
      : first_(),
        second_(),
        third_(std::forward<Q>(args)...)
   { }

   template <typename P, typename... Q>
   SmallTriplet(NoArgsTag, OneArgTag, P&& p, Q&&... args)
      : first_(),
        second_(std::forward<P>(p)),
        third_(std::forward<Q>(args)...)
   { }

   template <typename O, typename... Q>
   SmallTriplet(OneArgTag, NoArgsTag, O&& o, Q&&... args)
      : first_(std::forward<O>(o)),
        second_(),
        third_(std::forward<Q>(args)...)
   { }

   template <typename O, typename P, typename... Q>
   SmallTriplet(OneArgTag, OneArgTag, O&& o, P&& p, Q&&... args)
      : first_(std::forward<O>(o)),
        second_(std::forward<P>(p)),
        third_(std::forward<Q>(args)...)
   { }

   T& first() noexcept              { return first_; }
   const T& first() const noexcept  { return first_; }

   U& second() noexcept             { return second_; }
   const U& second() const noexcept { return second_; }

   V& third() noexcept              { return third_; }
   const V& third() const noexcept  { return third_; }

private:
   T first_;
   U second_;
   V third_;
};

} // be

#endif
