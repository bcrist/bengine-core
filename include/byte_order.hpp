#pragma once
#ifndef BE_CORE_BYTE_ORDER_HPP_
#define BE_CORE_BYTE_ORDER_HPP_

#include "builtins.hpp"
#include "memory.hpp"
#include <boost/detail/endian.hpp>

#ifdef BOOST_BIG_ENDIAN
#define BE_BIG_ENDIAN
#elif defined(BOOST_LITTLE_ENDIAN)
#define BE_LITTLE_ENDIAN
#endif

namespace be {

enum class ByteOrderType {
   unknown,
   little_endian,
   big_endian
};

template <ByteOrderType Value>
using ByteOrderTag = Tag<ByteOrderType, Value>;

namespace bo {

using Little = ByteOrderTag<ByteOrderType::little_endian>;
using Big = ByteOrderTag<ByteOrderType::big_endian>;
using Net = Big;
#ifdef BE_BIG_ENDIAN
using Native = ByteOrderTag<ByteOrderType::big_endian>;
#elif defined(BE_LITTLE_ENDIAN)
using Native = ByteOrderTag<ByteOrderType::little_endian>;
#else
using Native = ByteOrderTag<ByteOrderType::unknown>;
#endif
static_assert(Native::value != ByteOrderType::unknown, "Architecture endianness could not be determined!");
using Host = Native;

template <typename T, std::size_t N, bool, bool>
struct Converter { };

template <ByteOrderType To, ByteOrderType From = Host::value, typename T = void> T to(T v) { return Converter<T>::convert<From, To>(std::move(v)); }
template <ByteOrderType From = Host::value, typename T = void> T to_little(T v) { return Converter<T>::convert<From, Little::value>(std::move(v)); }
template <ByteOrderType From = Host::value, typename T = void> T to_big(T v)    { return Converter<T>::convert<From, Big::value>(std::move(v)); }
template <ByteOrderType From = Host::value, typename T = void> T to_net(T v)    { return Converter<T>::convert<From, Net::value>(std::move(v)); }
template <ByteOrderType From = Net::value, typename T = void>  T to_host(T v)   { return Converter<T>::convert<From, Host::value>(std::move(v)); }

template <ByteOrderType To, ByteOrderType From = Host::value, typename T = void> void static_to(T& v) { Converter<T>::in_place<From, To>(v); }
template <ByteOrderType From = Host::value, typename T = void> void static_to_little(T& v) { Converter<T>::in_place<From, Little::value>(v); }
template <ByteOrderType From = Host::value, typename T = void> void static_to_big(T& v)    { Converter<T>::in_place<From, Big::value>(v); }
template <ByteOrderType From = Host::value, typename T = void> void static_to_net(T& v)    { Converter<T>::in_place<From, Net::value>(v); }
template <ByteOrderType From = Net::value, typename T = void> void static_to_host(T& v)    { Converter<T>::in_place<From, Host::value>(v); }

template <typename T>
struct ConvertBase {
   using type = T;

   template <ByteOrderType From, ByteOrderType To>
   static T convert(T v) {
      return Converter<T>::convert(std::move(v), ByteOrderTag<From>(), ByteOrderTag<To>());
   }

   template <ByteOrderType From, ByteOrderType To>
   static void in_place(T& v) {
      Converter<T>::in_place(v, ByteOrderTag<From>(), ByteOrderTag<To>());
   }

   // Non conversion is nop
   template <typename Tag>
   static T convert(T v, Tag, Tag) { return v; }

   // Non conversion is nop
   template <typename Tag>
   static void in_place(T&, Tag, Tag) { }

   // Allow implementors the option of only implementing in_place
   template <typename FromTag, typename ToTag>
   static T convert(T v, FromTag, ToTag) {
      T result { std::move(v) };
      Converter<T>::in_place<FromTag::value, ToTag::value>(result);
      return result;
   }

   // Allow implementors the option of only implementing convert
   template <typename FromTag, typename ToTag>
   static void in_place(T& v, FromTag, ToTag) {
      v = Converter<T>::convert<FromTag::value, ToTag::value>(v);
   }

protected:
   using base = ConvertBase<T>;
};

// 1-byte objects have no byte order by definition
template <typename T, bool I, bool F>
struct Converter<T, 1, I, F> : ConvertBase<T> {
   template <ByteOrderType From, ByteOrderType To>
   static T convert(T v) { return v; }

   template <ByteOrderType From, ByteOrderType To>
   static void in_place(T& v) { }
};

// integral types:

template <typename T>
struct Converter<T, 2, true, false> : ConvertBase<T> {
   using base::convert;
   static T convert(T v, Little, Big) { return static_cast<T>(BE_BSWAP_U16(v)); }
   static T convert(T v, Big, Little) { return static_cast<T>(BE_BSWAP_U16(v)); }
};

template <typename T>
struct Converter<T, 4, true, false> : ConvertBase<T> {
   using base::convert;
   static T convert(T v, Little, Big) { return static_cast<T>(BE_BSWAP_U32(v)); }
   static T convert(T v, Big, Little) { return static_cast<T>(BE_BSWAP_U32(v)); }
};

template <typename T>
struct Converter<T, 8, true, false> : ConvertBase<T> {
   using base::convert;
   static T convert(T v, Little, Big) { return static_cast<T>(BE_BSWAP_U64(v)); }
   static T convert(T v, Big, Little) { return static_cast<T>(BE_BSWAP_U64(v)); }
};

// floating point types:

template <>
struct Converter<F32, 4, false, true> : ConvertBase<F32> {
   using base::in_place;
   static void in_place(type& v, Little, Big) { reverse(v); }
   static void in_place(type& v, Big, Little) { reverse(v); }
   static void reverse(type& v) {
      U32 tmp;
      memcpy(&tmp, &v, sizeof(tmp));
      tmp = BE_BSWAP_U32(tmp);
      memcpy(&v, &tmp, sizeof(tmp));
   }
};

template <>
struct Converter<F64, 8, false, true> : ConvertBase<F64> {
   using base::in_place;
   static void in_place(type& v, Little, Big) { reverse(v); }
   static void in_place(type& v, Big, Little) { reverse(v); }
   static void reverse(type& v) {
      U64 tmp;
      memcpy(&tmp, &v, sizeof(tmp));
      tmp = BE_BSWAP_U64(tmp);
      memcpy(&v, &tmp, sizeof(tmp));
   }
};

// raw arrays:

template <typename T, std::size_t N, std::size_t S>
struct Converter<T[N], S, false, false> {
   using type = T[N];

   template <ByteOrderType From, ByteOrderType To>
   static void in_place(type& v) {
      Converter<type>::in_place(v, ByteOrderTag<From>(), ByteOrderTag<To>());
   }

   static void in_place(type& v, Little, Big) {
      for (std::size_t i = 0; i < N; ++i) {
         Converter<T>::in_place(v[i], Little(), Big());
      }
   }

   static void in_place(type& v, Big, Little) {
      for (std::size_t i = 0; i < N; ++i) {
         Converter<T>::in_place(v[i], Little(), Big());
      }
   }

   // Non conversion is nop
   template <typename Tag>
   static void in_place(T&, Tag, Tag) { }
};

} // be::bo
} // be

#endif
