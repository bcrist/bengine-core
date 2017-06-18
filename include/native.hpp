#pragma once
#ifndef BE_CORE_NATIVE_HPP_
#define BE_CORE_NATIVE_HPP_

#include "macros.hpp"
#include <string>
#include <locale>

#if defined(_WIN32) && defined(_MSC_VER)
#  define BE_NATIVE_VC_WIN
#  define BE_NATIVE(lib, x) <BE_CONCAT(be/,BE_CONCAT(lib,BE_CONCAT(/native/vc_win/,x)))>
#  define BE_NATIVE_CORE(x) <BE_CONCAT(be/core/native/vc_win/,x)>
#else
#  error Unsupported compiler/platform!
#endif

namespace be {

std::string os_version();
std::locale default_locale();

} // be

#endif
