#pragma once
#ifndef BE_CORE_ENUM_VEC1_HPP_
#define BE_CORE_ENUM_VEC1_HPP_

#include "enum_vec_fwd.hpp"

namespace be {

template <typename T>
struct enum_vec<1, T> {
   typedef T value_type;
   typedef enum_vec type;
   typedef glm::length_t length_type;

#if GLM_HAS_ONLY_XYZW
   T x;
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
      T x;
      T r;
      T s;
   };
#if GLM_COMPILER & GLM_COMPILER_CLANG
#pragma clang diagnostic pop
#endif
#if GLM_COMPILER & GLM_COMPILER_GCC
#pragma GCC diagnostic pop
#endif
#else
   union {
      T x, r, s;
   };
#endif

   static length_type length() {
      return 1;
   }

   constexpr enum_vec() GLM_DEFAULT_CTOR;
   constexpr enum_vec(const enum_vec& v) = default;

   constexpr explicit enum_vec(glm::ctor);
   constexpr explicit enum_vec(T v);

   constexpr GLM_EXPLICIT enum_vec(const enum_vec<2, T>& v);
   constexpr GLM_EXPLICIT enum_vec(const enum_vec<3, T>& v);
   constexpr GLM_EXPLICIT enum_vec(const enum_vec<4, T>& v);

   enum_vec& operator=(const enum_vec& v) = default;

   T& operator[](length_type i);
   const T& operator[](length_type i) const;
};

template<typename T>
bool operator==(const enum_vec<1, T>& v1, const enum_vec<1, T>& v2);

template<typename T>
bool operator!=(const enum_vec<1, T>& v1, const enum_vec<1, T>& v2);

} // be

#include "enum_vec1.inl"

#endif
