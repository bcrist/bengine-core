#pragma once
#ifndef BE_CORE_PERF_ENSURE_INIT_HPP_
#define BE_CORE_PERF_ENSURE_INIT_HPP_

#include <be/core/lifecycle.hpp>

namespace be {

///////////////////////////////////////////////////////////////////////////////
inline void ensure_init() {
   static CoreInitLifecycle lifecycle;
}

} // be

#endif
