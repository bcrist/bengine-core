#pragma once
#ifndef BE_CORE_ENUM_VEC4_HPP_
#define BE_CORE_ENUM_VEC4_HPP_

#include "enum_vec_fwd.hpp"

namespace be {

template <typename T>
struct enum_vec<4, T> {
   typedef T value_type;
   typedef enum_vec type;
   typedef glm::length_t length_type;

#if GLM_HAS_ONLY_XYZW
   T x, y, z, w;
#elif GLM_HAS_ALIGNED_TYPE
#if GLM_COMPILER & GLM_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
#if GLM_COMPILER & GLM_COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#endif
   union {
      struct { T x, y, z, w;};
      struct { T r, g, b, a; };
      struct { T s, t, p, q; };
   };
#if GLM_COMPILER & GLM_COMPILER_CLANG
#pragma clang diagnostic pop
#endif
#if GLM_COMPILER & GLM_COMPILER_GCC
#pragma GCC diagnostic pop
#endif
#else
   union { T x, r, s; };
   union { T y, g, t; };
   union { T z, b, p; };
   union { T w, a, q; };
#endif

   static length_type length() {
      return 4;
   }

   constexpr enum_vec() GLM_DEFAULT_CTOR;
   constexpr enum_vec(const enum_vec& v) = default;

   constexpr explicit enum_vec(glm::ctor);
   constexpr explicit enum_vec(T v);
   constexpr enum_vec(T x, T y, T z, T w);

   constexpr enum_vec(const enum_vec<1, T>& x, const enum_vec<1, T>& y, const enum_vec<1, T>& z, const enum_vec<1, T>& w);
   constexpr enum_vec(const enum_vec<2, T>& xy, T z, T w);
   constexpr enum_vec(const enum_vec<2, T>& xy, const enum_vec<1, T>& z, const enum_vec<1, T>& w);
   constexpr enum_vec(T x, const enum_vec<2, T>& yz, T w);
   constexpr enum_vec(const enum_vec<1, T>& x, const enum_vec<2, T>& yz, const enum_vec<1, T>& w);
   constexpr enum_vec(T x, T y, const enum_vec<2, T>& zw);
   constexpr enum_vec(const enum_vec<1, T>& x, const enum_vec<1, T>& y, const enum_vec<2, T>& zw);
   constexpr enum_vec(const enum_vec<3, T>& xyz, T w);
   constexpr enum_vec(const enum_vec<3, T>& xyz, const enum_vec<1, T>& w);
	constexpr enum_vec(T x, const enum_vec<3, T>& yzw);
   constexpr enum_vec(const enum_vec<1, T>& x, const enum_vec<3, T>& yzw);	
   constexpr enum_vec(const enum_vec<2, T>& xy, const enum_vec<2, T>& zw);

   enum_vec& operator=(const enum_vec& v) = default;

   T& operator[](length_type i);
   const T& operator[](length_type i) const;
};

template<typename T>
bool operator==(const enum_vec<4, T>& v1, const enum_vec<4, T>& v2);

template<typename T>
bool operator!=(const enum_vec<4, T>& v1, const enum_vec<4, T>& v2);

} // be

#include "enum_vec4.inl"

#endif
