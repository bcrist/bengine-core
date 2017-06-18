#pragma once
#ifndef BE_CORE_NATIVE_VC_WIN_BUILTINS_HPP_
#define BE_CORE_NATIVE_VC_WIN_BUILTINS_HPP_

#include "vc_win.hpp"

#define BE_NO_INLINE __declspec(noinline)

#define BE_BSWAP_U16(value) (U16)_byteswap_ushort(static_cast<unsigned short>(value))
#define BE_BSWAP_U32(value) (U32)_byteswap_ulong(static_cast<unsigned long>(value))
#define BE_BSWAP_U64(value) (U64)_byteswap_uint64(static_cast<unsigned __int64>(value))

#endif
