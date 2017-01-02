#pragma once
#ifndef BE_CORE_CONSOLE_INDENT_HPP_
#define BE_CORE_CONSOLE_INDENT_HPP_

#include "be.hpp"
#include <sstream>

namespace be {
namespace detail {

extern const int indent_count_idx;
extern const int indent_str_idx;

struct NlObj { };

struct IosModObj {
   enum class Op : U8 {
      set,
      offset
   } op;
   int index;
   void* p;
};

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
std::basic_ostream<C, CTraits>& operator<<(std::basic_ostream<C, CTraits>& os, const IosModObj& imo) {
   if (imo.op == IosModObj::Op::set) {
      os.pword(imo.index) = imo.p;
   } else {
      (char*&)os.pword(imo.index) += reinterpret_cast<ptrdiff_t>(imo.p);
   }
   return os;
}

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
std::basic_ostream<C, CTraits>& operator<<(std::basic_ostream<C, CTraits>& os, const NlObj&) {
   os << '\n';
   const char* ptr = (const char*)os.pword(indent_str_idx);
   if (!ptr) {
      ptr = "   ";
      os.pword(indent_str_idx) = const_cast<char*>(ptr);
   }

   for (intptr_t i = reinterpret_cast<intptr_t>(os.pword(indent_count_idx)); i > 0; --i) {
      os << ptr;
   }

   return os;
}

} // be::detail

extern const detail::NlObj nl;

detail::IosModObj setindent(int n = 0);
detail::IosModObj setindent(const char* str = "   ");
detail::IosModObj deltaindent(int n = 1);

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
std::basic_ostream<C, CTraits>& indent(std::basic_ostream<C, CTraits>& os) {
   ++(char*&)os.pword(detail::indent_count_idx);
   return os;
}

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
std::basic_ostream<C, CTraits>& unindent(std::basic_ostream<C, CTraits>& os) {
   --(char*&)os.pword(detail::indent_count_idx);
   return os;
}

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
std::pair<int, const char*> get_indent(const std::basic_ostream<C, CTraits>& os) {
   std::basic_ostringstream<C, CTraits> state;
   state.copyfmt(os);

   return std::make_pair(static_cast<int>(reinterpret_cast<intptr_t>(
      state.pword(detail::indent_count_idx))),
      (const char*)state.pword(detail::indent_str_idx));
}

} // be

#endif
