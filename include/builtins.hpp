#pragma once
#ifndef BE_CORE_BUILTINS_HPP_
#define BE_CORE_BUILTINS_HPP_

#ifdef DOXYGEN
///////////////////////////////////////////////////////////////////////////////
/// \brief  Cross-platform macro for swapping the MSB and LSB bytes in a 16-bit
///         word.
///
/// \details Most architectures/compilers support this through an intrinsic.
#define BE_BSWAP_U16(value)

///////////////////////////////////////////////////////////////////////////////
/// \brief  Cross-platform macro for swapping the MSB and LSB bytes in a 32-bit
///         word, as well as swapping the two "inner" bytes.
///
/// \details Most architectures/compilers support this through an intrinsic.
#define BE_BSWAP_U32(value)

///////////////////////////////////////////////////////////////////////////////
/// \brief  Cross-platform macro for reversing the order of the bytes in a
///         64-bit word.
///
/// \details Most architectures/compilers support this through an intrinsic.
#define BE_BSWAP_U64(value)

#endif

#include BE_NATIVE_CORE(builtins.hpp)

#endif
