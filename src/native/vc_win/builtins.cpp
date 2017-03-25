#include "pch.hpp"
#include <be/core/native.hpp>
#ifdef BE_NATIVE_VC_WIN
#include <be/core/builtins.hpp>
#include "native/vc_win/vc_win_win32.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
std::pair<U8, bool> bitscan_fwd(U32 data) {
   ::DWORD dest;
   ::BOOLEAN result = _BitScanForward(&dest, static_cast<::DWORD>(data));
   return std::make_pair(static_cast<U8>(dest), result != 0);
}

///////////////////////////////////////////////////////////////////////////////
std::pair<U8, bool> bitscan_fwd(U64 data) {
   ::DWORD dest;
   ::BOOLEAN result = _BitScanForward64(&dest, static_cast<::DWORD64>(data));
   return std::make_pair(static_cast<U8>(dest), result != 0);
}

///////////////////////////////////////////////////////////////////////////////
std::pair<U8, bool> bitscan_rev(U32 data) {
   ::DWORD dest;
   ::BOOLEAN result = _BitScanReverse(&dest, static_cast<::DWORD>(data));
   return std::make_pair(static_cast<U8>(dest), result != 0);
}

///////////////////////////////////////////////////////////////////////////////
std::pair<U8, bool> bitscan_rev(U64 data) {
   ::DWORD dest;
   ::BOOLEAN result = _BitScanReverse64(&dest, static_cast<::DWORD64>(data));
   return std::make_pair(static_cast<U8>(dest), result != 0);
}

} // be

#endif
