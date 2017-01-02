#pragma once
#ifndef BE_CORE_T_IS_SAME_SIZE_HPP_
#define BE_CORE_T_IS_SAME_SIZE_HPP_

#include "be.hpp"

namespace be {
namespace t {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if two types have the same size, as reported by
///         sizeof(T).
template <typename T, typename U, bool = sizeof(T) == sizeof(U)>
struct IsSameSize : False { };

template <typename T, typename U>
struct IsSameSize<T, U, true> : True { };

} // be::t
} // be

#endif
