#pragma once
#ifndef BE_CORE_VERBOSITY_HPP_
#define BE_CORE_VERBOSITY_HPP_

namespace be {
namespace v {

enum Verbosity : U16 {
   hidden = 0,
   fatal = 0x1,
   error = 0x2,
   warning = 0x4,
   notice = 0x8,
   info = 0x10,
   verbose = 0x20,
   debug = 0x40
};

enum VerbosityMask : U16 {
   nothing = 0,
   errors_or_worse = 0x3,
   warnings_or_worse = 0x7,
   notices_or_worse = 0xF,
   info_or_worse = 0x1F,
   verbose_or_worse = 0x3F,
   debug_or_worse = 0x7F,
   everything = 0xFFFF
};

} // be::v

inline constexpr bool check_verbosity(U16 verbosity, U16 mask) {
   return !!(verbosity & mask);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Gets a string representation of a verbosity level.
///
/// \param  level the verbosity level we are interested in.
///
/// \return A C-string describing the type of message represented by the given
///         \ref Verbosity level, or "Unknown" if the verbosity level is not
///         recognized.
inline const char* verbosity_name(v::Verbosity verbosity) {
   if (check_verbosity(verbosity, v::fatal)) return " ! Fatal ! ";
   if (check_verbosity(verbosity, v::error)) return "Error";
   if (check_verbosity(verbosity, v::warning)) return "Warning";
   if (check_verbosity(verbosity, v::notice)) return "Notice";
   if (check_verbosity(verbosity, v::info)) return "Info";
   if (check_verbosity(verbosity, v::verbose)) return "Info";
   if (check_verbosity(verbosity, v::debug)) return "Debug";
   return "Unknown";
}

} // be

#endif
