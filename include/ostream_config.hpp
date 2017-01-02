#pragma once
#ifndef BE_CORE_OSTREAM_CONFIG_HPP_
#define BE_CORE_OSTREAM_CONFIG_HPP_

#include "console_color.hpp"
#include "console_indent.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
struct OStreamConfig {
   std::ios_base::fmtflags flags = std::ios_base::dec | std::ios_base::left;
   std::streamsize precision = 6;
   char fill = ' ';
   int indent = 0;
   const char* indent_str = "   ";
   LogColorState color = color::reset;
};

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
OStreamConfig get_ostream_config(const std::basic_ostream<C, CTraits>& os) {
   OStreamConfig config;
   config.flags = os.flags();
   config.precision = os.precision();
   config.fill = os.fill();
   std::tie(config.indent, config.indent_str) = get_indent(os);
   config.color = get_color(os);
   return config;
}

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
OStreamConfig set_ostream_config(std::basic_ostream<C, CTraits>& os, OStreamConfig config) {
   os.flags(config.flags);
   os.precision(config.precision);
   os.fill(config.fill);
   os << setindent(config.indent);
   os << setindent(config.indent_str);
   os << config.color;
   return config;
}

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
std::basic_ostream<C, CTraits>& operator<<(std::basic_ostream<C, CTraits>& os, const OStreamConfig& config) {
   os.flags(config.flags);
   os.precision(config.precision);
   os.fill(config.fill);
   os << setindent(config.indent);
   os << setindent(config.indent_str);
   os << config.color;
   return os;
}

} // be

#endif
