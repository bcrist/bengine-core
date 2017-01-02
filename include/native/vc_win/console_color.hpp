#pragma once
#ifndef BE_NATIVE_VC_WIN_CONSOLE_COLOR_HPP_
#define BE_NATIVE_VC_WIN_CONSOLE_COLOR_HPP_

#include <be/core/native.hpp>
#ifdef BE_NATIVE_VC_WIN

namespace be {
namespace detail {

void init_console_color_handlers();

} // be::detail
} // be

#endif
#endif
