#pragma once
#ifndef BE_CORE_ENUM_TRAITS_HPP_
#define BE_CORE_ENUM_TRAITS_HPP_

#include "be.hpp"
#include <type_traits>
#include <array>
#include <iosfwd>

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename E>
struct EnumTraits {
   using type = E;
   using underlying_type = typename std::underlying_type<E>::type;

   static constexpr std::size_t count = 0;

   static bool is_valid(E value);
   static const char* name(E value);

   template <typename C = std::array<const type, count>>
   static C values();
};

} // be

#endif
