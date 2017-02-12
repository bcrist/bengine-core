#pragma once
#ifndef BE_CORE_BE_HPP_
#define BE_CORE_BE_HPP_

#include "native.hpp"
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <utility>
#include <gsl/string_span>

namespace be {

template <typename T, T Value>
using Tag = std::integral_constant<T, Value>;

#ifdef BE_DEBUG
using IsDebug = std::true_type;
#else
using IsDebug = std::false_type;
#endif

using True = std::true_type;
using False = std::false_type;

struct NoArgsTag { };
struct OneArgTag { };
struct Nil { };
template <typename>
struct TNil { };

bool is_main_thread();
#ifdef BE_ENABLE_MAIN_THREAD_ASSERTIONS
#define BE_ASSERT_MAIN_THREAD() assert(::be::is_main_thread())
#else
#define BE_ASSERT_MAIN_THREAD()
#endif

} // be

#include "core_autolink.hpp"
#include "memory.hpp"

#endif
