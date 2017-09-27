#pragma once
#ifndef BE_CORE_ENUM_VEC_FWD_HPP_
#define BE_CORE_ENUM_VEC_FWD_HPP_

#include "be.hpp"
#include <glm/detail/setup.hpp>

namespace be {

template <glm::length_t L, typename T>
struct enum_vec;

template <typename T>
using evec1 = enum_vec<1, T>;

template <typename T>
using evec2 = enum_vec<2, T>;

template <typename T>
using evec3 = enum_vec<3, T>;

template <typename T>
using evec4 = enum_vec<4, T>;

} // be

#endif
