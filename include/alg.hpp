#pragma once
#ifndef BE_CORE_ALG_HPP_
#define BE_CORE_ALG_HPP_

#include <algorithm>
#include <iterator>
#include <cassert>

#ifdef max
#pragma message("Undefining max macro!")
#undef max
#endif

#ifdef min
#pragma message("Undefining min macro!")
#undef max
#endif

namespace be {

using std::min;
using std::max;

///////////////////////////////////////////////////////////////////////////////
/// \brief  Clamps a value between two other values, inclusive.
/// \details Based on the reference implementation for C++17 clamp (N4536).
///         See:
///         http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4536.html
///         https://github.com/martinmoene/clamp/blob/master/clamp.hpp

template <typename T, typename LessPred = std::less<>>
constexpr const T& clamp(const T& value, const T& low, const T& high, LessPred compare = LessPred()) {
   return assert(!compare(high, low)),
      compare(value, low) ? low : compare(high, value) ? high : value;
}


///////////////////////////////////////////////////////////////////////////////
/// \brief  Clamps each value in a sequence between two fixed values.
/// \details Based on the reference implementation for C++17 clamp (N4536).
///         See:
///         http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4536.html
///         https://github.com/martinmoene/clamp/blob/master/clamp.hpp
template <typename InputIterator, typename OutputIterator, typename LessPred = std::less<>>
OutputIterator clamp_range(InputIterator first, InputIterator last, OutputIterator out,
                           typename std::iterator_traits<InputIterator>::value_type const& low,
                           typename std::iterator_traits<InputIterator>::value_type const& high,
                           LessPred compare = LessPred()) {
   using arg_type = decltype(low);

   return std::transform(first, last, out,
      [&](arg_type value) -> arg_type {
         return clamp(value, low, high, compare);
      });
}

///////////////////////////////////////////////////////////////////////////////
template <typename CharT>
bool is_whitespace(CharT c) {
   return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

///////////////////////////////////////////////////////////////////////////////
struct IsWhitespace {
   template <typename CharT>
   bool operator()(CharT c) {
      return is_whitespace(c);
   }
};

///////////////////////////////////////////////////////////////////////////////
template <typename CharT>
CharT to_lower(CharT c) {
   if (c >= CharT('A') && c <= CharT('Z')) {
      return c - CharT('A') + CharT('a');
   }
   return c;
}

///////////////////////////////////////////////////////////////////////////////
template <typename CharT>
CharT to_upper(CharT c) {
   if (c >= CharT('a') && c <= CharT('z')) {
      return c - CharT('a') + CharT('A');
   }
   return c;
}

} // be

#endif
