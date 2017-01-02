#pragma once
#ifndef BE_CORE_STRING_INTERPOLATION_HPP_
#define BE_CORE_STRING_INTERPOLATION_HPP_

#include "be.hpp"
#include <regex>

namespace be {

template <typename F, char Sigil = '$'>
S interpolate_string(const S& source, F func = F());

template <typename G, typename F, char Sigil = '$'>
void interpolate_string_ex(const S& source, G noninterp_func = G(), F interp_func = F());

} // be

#include "string_interpolation.inl"

#endif
