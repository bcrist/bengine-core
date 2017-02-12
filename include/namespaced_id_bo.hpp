#pragma once
#ifndef BE_CORE_NAMESPACED_ID_BO_HPP_
#define BE_CORE_NAMESPACED_ID_BO_HPP_

#include "namespaced_id.hpp"
#include "id_bo.hpp"

namespace be {
namespace bo {

template<>
struct Converter<NamespacedId> : ConvertBase<NamespacedId> {
   using base::in_place;

   static void in_place(type& v, Little, Big) {
      static_to_big<Little::value>(v.ns);
      static_to_big<Little::value>(v.id);
   }

   static void in_place(type& v, Big, Little) {
      static_to_little<Big::value>(v.ns);
      static_to_little<Big::value>(v.id);
   }

};

} // be::bo
} // be

#endif
