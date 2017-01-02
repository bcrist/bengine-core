#pragma once
#ifndef BE_CORE_TIME_HPP_
#define BE_CORE_TIME_HPP_

#include "be.hpp"
#include <chrono>
#include <ctime>

namespace be {

using TU = std::chrono::microseconds;

// in native time.cpp
TU ts_now() noexcept;
U64 perf_now() noexcept;
TU perf_delta_to_tu(U64 delta) noexcept;
F64 perf_delta_to_seconds(U64 delta) noexcept;
tm* time_t_to_local_tm(time_t t, tm& in) noexcept;

// in core time.cpp
F64 tu_to_seconds(TU tu) noexcept;
TU seconds_to_tu(F64 seconds) noexcept;
time_t ts_to_time_t_fast(TU ts) noexcept;
std::pair<time_t, U32> decompose_ts(TU ts) noexcept;

} // be

#endif
