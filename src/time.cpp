#include "pch.hpp"
#include "time.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
F64 tu_to_seconds(TU tu) noexcept {
   return (F64)tu.count() / (F64)TU::period::den;
}

///////////////////////////////////////////////////////////////////////////////
TU seconds_to_tu(F64 seconds) noexcept {
   return TU((U64)(seconds * TU::period::den));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a microsecond-resolution unix timestamp into a
///         second-resolution time_t.
///
/// \details Remaining microseconds are rounded by truncation towards zero.
///         ie. timestamps before 1970 will be rounded forward to the next
///         closest second, while times after 1970 will be rounded back to the
///         previous second.
time_t ts_to_time_t_fast(TU ts) noexcept {
   return ts.count() / TU::period::den;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Splits a microsecond-resolution unix timestamp into a
///         second-resolution time_t and the remainder of extra microseconds.
std::pair<time_t, U32> decompose_ts(TU ts) noexcept {
   std::pair<time_t, U32> p { ts_to_time_t_fast(ts), 0 };
   p.second = (U32)(ts.count() - (I64)p.first * TU::period::den);
   if (ts.count() < 0 && p.second != 0) {
      --p.first;
   }
   return p;
}

} // be
