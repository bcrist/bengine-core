#pragma once
#ifndef BE_CORE_T_COVERING_UNSIGNED_HPP_
#define BE_CORE_T_COVERING_UNSIGNED_HPP_

#include "be.hpp"

namespace be::t {
namespace detail {

template <std::size_t N, bool = (N < (1ull << 8)), bool = (N < (1ull << 16)), bool = (N < 1ull << 32)>
struct CoveringUnsigned { };

template <std::size_t N>
struct CoveringUnsigned<N, true, true, true> {
   using type = U8;
};

template <std::size_t N>
struct CoveringUnsigned<N, false, true, true> {
   using type = U16;
};

template <std::size_t N>
struct CoveringUnsigned<N, false, false, true> {
   using type = U32;
};

template <std::size_t N>
struct CoveringUnsigned<N, false, false, false> {
   using type = U64;
};

} // be::t::detail

template <std::size_t N>
using CoveringUnsigned = typename detail::CoveringUnsigned<N>::type;

} // be::t

#endif
