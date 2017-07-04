#pragma once
#ifndef BE_CORE_T_PRED_HPP_
#define BE_CORE_T_PRED_HPP_

#include "be.hpp"

namespace be::t {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compile-time predicate type.
/// \details Extends True iff the template parameter is true.  Otherwise,
///         extends False.
template <bool> struct Pred : False { };
template <> struct Pred<true> : True { };

} // be::t

#endif
