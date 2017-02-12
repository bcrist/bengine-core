#pragma once
#ifndef BE_CORE_EXTENTS_BO_HPP_
#define BE_CORE_EXTENTS_BO_HPP_

#include "generic_extents.hpp"
#include "byte_order.hpp"

namespace be {
namespace bo {

template <typename T, typename U>
struct Converter<generic_extents<T, U>> : ConvertBase<generic_extents<T, U>> {
   using base::in_place;

   static void in_place(type& v, Little, Big) {
      static_to_big<Little::value>(v.offset);
      static_to_big<Little::value>(v.dim);
   }

   static void in_place(type& v, Big, Little) {
      static_to_little<Big::value>(v.offset);
      static_to_little<Big::value>(v.dim);
   }

};

} // be::bo
} // be

#endif
