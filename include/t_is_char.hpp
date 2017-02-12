#pragma once
#ifndef BE_CORE_T_IS_CHAR_HPP_
#define BE_CORE_T_IS_CHAR_HPP_

#include "be.hpp"

namespace be {
namespace t {

template <typename T> struct IsChar : False { };
template <> struct IsChar<char> : True { };
template <> struct IsChar<unsigned char> : True { };

} // be::t
} // be

#endif
