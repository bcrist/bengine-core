#pragma once
#ifndef BE_CORE_T_SELECT_HPP_
#define BE_CORE_T_SELECT_HPP_

#include "be.hpp"

namespace be {
namespace t {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Selects between one of two types based on the first template
///         parameter.
///
/// \details If the first template parameter is true, `type` becomes an alias
///         for the second template parameter, T.  Otheriwse it will be an
///         alias for the third template parameter, U.
template <bool, typename T, typename U>
struct Select {
   using type = U;
};

template <typename T, typename U>
struct Select<true, T, U> {
   using type = T;
};

} // be::t
} // be

#endif
