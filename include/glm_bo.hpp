#pragma once
#ifndef BE_CORE_GLM_BO_HPP_
#define BE_CORE_GLM_BO_HPP_

#include "glm.hpp"
#include "byte_order.hpp"

namespace be {
namespace bo {

template <glm::length_t L, typename T, glm::precision P>
struct Converter<glm::vec<L, T, P>> : ConvertBase<glm::vec<L, T, P>> {
   using base::in_place;

   static void in_place(type& v, Little, Big) {
      for (glm::length_t i = 0; i < L; ++i) {
         static_to_big<Little::value>(v[i]);
      }
   }

   static void in_place(type& v, Big, Little) {
      for (glm::length_t i = 0; i < L; ++i) {
         static_to_little<Big::value>(v[i]);
      }
   }

};

template <glm::length_t C, glm::length_t R, typename T, glm::precision P>
struct Converter<glm::mat<C, R, T, P>> : ConvertBase<glm::mat<C, R, T, P>> {
   using base::in_place;

   static void in_place(type& v, Little, Big) {
      for (glm::length_t i = 0; i < C; ++i) {
         for (glm::length_t j = 0; j < R; ++j) {
            static_to_big<Little::value>(v[i][j]);
         }
      }
   }

   static void in_place(type& v, Big, Little) {
      for (glm::length_t i = 0; i < C; ++i) {
         for (glm::length_t j = 0; j < R; ++j) {
            static_to_little<Big::value>(v[i][j]);
         }
      }
   }

};

template <typename T, glm::precision P>
struct Converter<glm::tquat<T, P>> : ConvertBase<glm::tquat<T, P>> {
   using base::in_place;

   static void in_place(type& v, Little, Big) {
      for (glm::length_t i = 0; i < 4; ++i) {
         static_to_big<Little::value>(v[i]);
      }
   }

   static void in_place(type& v, Big, Little) {
      for (glm::length_t i = 0; i < 4; ++i) {
         static_to_little<Big::value>(v[i]);
      }
   }

};

template <typename T, glm::precision P>
struct Converter<glm::tdualquat<T, P>> : ConvertBase<glm::tdualquat<T, P>> {
   using base::in_place;

   static void in_place(type& v, Little, Big) {
      for (glm::length_t i = 0; i < 4; ++i) {
         static_to_big<Little::value>(v.real[i]);
      }
      for (glm::length_t i = 0; i < 4; ++i) {
         static_to_big<Little::value>(v.dual[i]);
      }
   }

   static void in_place(type& v, Big, Little) {
      for (glm::length_t i = 0; i < 4; ++i) {
         static_to_little<Big::value>(v.real[i]);
      }
      for (glm::length_t i = 0; i < 4; ++i) {
         static_to_little<Big::value>(v.dual[i]);
      }
   }

};

} // be::bo
} // be

#endif
