#include "pch.hpp"
#include <be/core/native.hpp>
#ifdef BE_NATIVE_GENERIC

#include <be/core/utf16_widen_narrow.hpp>
#include <locale>
#include <codecvt>

namespace be {

///////////////////////////////////////////////////////////////////////////////
std::u16string widen(std::u16string source) {
   return source;
}

///////////////////////////////////////////////////////////////////////////////
std::u16string widen(const S& source) {
   return widen(source.c_str(), source.length());
}

///////////////////////////////////////////////////////////////////////////////
std::u16string widen(const char* source) {
   return widen(source, strlen(source));
}

///////////////////////////////////////////////////////////////////////////////
std::u16string widen(const char* source, std::size_t length) {
   std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
   std::u16string dest = convert.from_bytes(source, source + length);
   return dest;
}

///////////////////////////////////////////////////////////////////////////////
S narrow(S source) {
   return source;
}

///////////////////////////////////////////////////////////////////////////////
S narrow(const std::u16string& source) {
   return narrow(source.c_str(), source.length());
}

///////////////////////////////////////////////////////////////////////////////
S narrow(const char16_t* source) {
   const char16_t* end = source;
   for (; *end; ++end);
   return narrow(source, end - source);
}

///////////////////////////////////////////////////////////////////////////////
S narrow(const char16_t* source, std::size_t length) {
   std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
   S dest = convert.to_bytes(source, source + length);
   return dest;
}

} // be

#endif