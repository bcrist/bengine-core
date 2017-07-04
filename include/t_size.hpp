#pragma once
#ifndef BE_CORE_T_IS_SAME_SIZE_HPP_
#define BE_CORE_T_IS_SAME_SIZE_HPP_

#include "be.hpp"
#include <type_traits>

namespace be::t {
namespace detail {

template <std::size_t N>
struct IntegerType { };
template <> struct IntegerType<sizeof(I8)> { using type = I8; };
template <> struct IntegerType<sizeof(I16)> { using type = I16; };
template <> struct IntegerType<sizeof(I32)> { using type = I32; };
template <> struct IntegerType<sizeof(I64)> { using type = I64; };

template <std::size_t N>
struct UnsignedType { };
template <> struct UnsignedType<sizeof(U8)> { using type = U8; };
template <> struct UnsignedType<sizeof(U16)> { using type = U16; };
template <> struct UnsignedType<sizeof(U32)> { using type = U32; };
template <> struct UnsignedType<sizeof(U64)> { using type = U64; };

template <typename SizeTag, bool Unsigned>
struct PossiblyUnsignedIntegerType { };
template <typename SizeTag> struct PossiblyUnsignedIntegerType<SizeTag, false> : IntegerType<SizeTag::value> { };
template <typename SizeTag> struct PossiblyUnsignedIntegerType<SizeTag, true> : UnsignedType<SizeTag::value> { };

} // be::detail

template <std::size_t N>
using Integer = typename ::be::t::detail::IntegerType<N>::type;
template <std::size_t N>
using Unsigned = typename ::be::t::detail::UnsignedType<N>::type;

template <typename T, std::size_t N>
using ResizeInteger = typename ::be::t::detail::PossiblyUnsignedIntegerType<Tag<std::size_t, N>, std::is_unsigned_v<T>>::type;

template <typename T>
using SizeTag = Tag<std::size_t, sizeof(T)>;

template <typename T>
using AlignTag = Tag<std::size_t, alignof(T)>;

template <typename T, std::size_t ExpectedSize, std::size_t ActualSize = sizeof(T)>
struct SizeIs : False {};
template <typename T, std::size_t Size>
struct SizeIs<T, Size, Size> : True {};

template <typename T, typename U, bool = sizeof(T) == sizeof(U)>
struct IsSameSize : False { };
template <typename T, typename U>
struct IsSameSize<T, U, true> : True { };

} // be::t

#endif
