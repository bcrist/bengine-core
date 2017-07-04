#pragma once
#ifndef BE_CORE_T_INTEGER_HPP_
#define BE_CORE_T_INTEGER_HPP_

#include "be.hpp"

namespace be::t {


template <typename T> struct IsChar : False { };
template <> struct IsChar<char> : True { };
template <> struct IsChar<unsigned char> : True { };

} // be::t

#endif
