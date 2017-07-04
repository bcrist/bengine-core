#pragma once
#ifndef BE_CORE_T_IS_ITERATOR_HPP_
#define BE_CORE_T_IS_ITERATOR_HPP_

#include "be.hpp"

namespace be::t {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if a type is an iterator.
///
/// \details For the purposes of this utility, an iterator is any type T for
///         for which std::iterator_traits<T>::iterator_category exists.
template <typename T, typename = void> struct IsIterator : False { };
template <typename T> struct IsIterator<T, std::void_t<typename std::iterator_traits<T>::iterator_category>> : True { };

} // be::t

#endif
