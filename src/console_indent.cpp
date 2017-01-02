#include "pch.hpp"
#include "console_indent.hpp"

namespace be {
namespace detail {

const int indent_count_idx = std::ios_base::xalloc();
const int indent_str_idx = std::ios_base::xalloc();

} // be::detail

const detail::NlObj nl;

///////////////////////////////////////////////////////////////////////////////
detail::IosModObj setindent(int n) {
   detail::IosModObj imo;
   imo.op = detail::IosModObj::Op::set;
   imo.index = detail::indent_count_idx;
   (char*&)imo.p = reinterpret_cast<char*>((intptr_t)n);
   return imo;
}

///////////////////////////////////////////////////////////////////////////////
detail::IosModObj setindent(const char* str) {
   detail::IosModObj imo;
   imo.op = detail::IosModObj::Op::set;
   imo.index = detail::indent_str_idx;
   imo.p = const_cast<char*>(str);
   return imo;
}

///////////////////////////////////////////////////////////////////////////////
detail::IosModObj deltaindent(int n) {
   detail::IosModObj imo;
   imo.op = detail::IosModObj::Op::offset;
   imo.index = detail::indent_count_idx;
   (char*&)imo.p = reinterpret_cast<char*>((intptr_t)n);
   return imo;
}

} // be
