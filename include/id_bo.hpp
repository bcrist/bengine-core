#pragma once
#ifndef BE_CORE_ID_BO_HPP_
#define BE_CORE_ID_BO_HPP_

#include "id.hpp"
#include "byte_order.hpp"

namespace be {
namespace bo {

template<>
struct Converter<Id> : ConvertBase<Id> {
   using base::convert;

   static Id convert(Id v, Little, Big) {
      return Id(to_big<Little::value>((U64)v));
   }

   static Id convert(Id v, Big, Little) {
      return Id(to_little<Big::value>((U64)v));
   }

};

} // be::bo
} // be

#endif
