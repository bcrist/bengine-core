#pragma once
#ifndef BE_CORE_T_IS_SIMPLE_ALLOC_HPP_
#define BE_CORE_T_IS_SIMPLE_ALLOC_HPP_

#include "t_pred.hpp"

namespace be {
namespace t {

///////////////////////////////////////////////////////////////////////////////
template <typename A>
struct IsSimpleAlloc : Pred<
   std::is_same<typename A::size_type, std::size_t>::value &&
   std::is_same<typename A::difference_type, std::ptrdiff_t>::value &&
   std::is_same<typename A::pointer, typename A::value_type*>::value &&
   std::is_same<typename A::const_pointer, const typename A::value_type*>::value &&
   std::is_same<typename A::reference, typename A::value_type&>::value &&
   std::is_same<typename A::const_reference, const typename A::value_type&>::value>
{ };

} // be::t
} // be

#endif
