#pragma once
#ifndef BE_CORE_GENERIC_EXTENTS_HPP_
#define BE_CORE_GENERIC_EXTENTS_HPP_

#include "alg.hpp"
#include "glm.hpp"

namespace be {

template <typename T, typename U = T>
struct generic_extents {
   typedef generic_extents<T, U> type;
   typedef T offset_type;
   typedef U dim_type;

   offset_type offset;
   dim_type dim;

   T left() const    { return offset; }
   T right() const   { return offset + T(dim); }
   T begin() const   { return offset; }
   T end() const     { return offset + T(dim); }
   U width() const   { return dim; }

   T center() const  { return offset + T(dim) / T(2); }

   bool operator==(const type& other) const {
      return offset == other.offset && dim == other.dim;
   }

   bool operator!=(const type& other) const {
      return !(*this == other);
   }

   bool zilch() const {
      return dim == 0;
   }

   bool empty() const {
      return dim == 0;
   }

   U size() const {
      return dim;
   }

   type union_bounds(const type& other) {
      type retval;
      T min_value = min(min(left(), right()), min(other.left(), other.right()));
      T max_value = max(max(left(), right()), max(other.left(), other.right()));

      retval.offset = min_value;
      retval.dim = U(max_value - min_value);
      return retval;
   }

   type union_bounds(offset_type point) {
      type retval;
      T min_value = min(min(left(), right()), point);
      T max_value = max(max(left(), right()), point);

      retval.offset = min_value;
      retval.dim = U(max_value - min_value);
      return retval;
   }

   type intersection(const type& other) {
      type retval;
      T new_left = max(offset, other.offset);
      T new_right = min(right(), other.right());

      if (new_left > new_right) {
         retval.offset = 0;
         retval.dim = 0;
      } else {
         retval.offset = new_left;
         retval.dim = U(new_right - new_left);
      }
      return retval;
   }

   bool intersects(const type& other) const {
      T new_left = max(offset, other.offset);
      T new_right = min(right(), other.right());

      return new_left <= new_right;
   }

   bool contains(offset_type point) const {
      return point >= offset && point < right();
   }

   bool contains(const type& other) const {
      return contains(other.offset) && contains(other.offset + other.dim);
   }
};

template <typename C, typename D, glm::precision P>
struct generic_extents<glm::tvec2<C, P>, glm::tvec2<D, P>> {
   typedef generic_extents<glm::tvec2<C, P>, glm::tvec2<D, P>> type;
   typedef glm::tvec2<C, P> offset_type;
   typedef glm::tvec2<D, P> dim_type;

   offset_type offset;
   dim_type dim;

   C left() const    { return offset.x; }
   C right() const   { return offset.x + C(dim.x); }
   C bottom() const  { return offset.y; }
   C top() const     { return offset.y + C(dim.y); }
   D width() const   { return dim.x; }
   D height() const  { return dim.y; }

   offset_type top_left() const      { return offset_type(left(), top()); }
   offset_type top_right() const     { return offset_type(right(), top()); }
   offset_type bottom_left() const   { return offset_type(left(), bottom()); }
   offset_type bottom_right() const  { return offset_type(right(), bottom()); }

   offset_type center() const {
      return offset_type(offset.x + C(dim.x) / C(2),
                         offset.y + C(dim.y) / C(2));
   }

   bool operator==(const type& other) const {
      return offset == other.offset && dim == other.dim;
   }

   bool operator!=(const type& other) const {
      return offset != other.offset || dim != other.dim;
   }

   bool zilch() const {
      return dim.x == 0 && dim.y == 0;
   }

   bool empty() const {
      return dim.x == 0 || dim.y == 0;
   }

   D size() const {
      return dim.x * dim.y;
   }

   type union_bounds(const type& other) {
      type retval;
      for (glm::length_t i = 0; i < 2; ++i) {
         C min_value = min(min(offset[i], offset[i] + C(dim[i])), min(other.offset[i], other.offset[i] + C(dim[i])));
         C max_value = max(max(offset[i], offset[i] + C(dim[i])), max(other.offset[i], other.offset[i] + C(dim[i])));

         retval.offset[i] = min_value;
         retval.dim[i] = D(max_value - min_value);
      }
      return retval;
   }

   type union_bounds(const offset_type& point) {
      type retval;
      for (glm::length_t i = 0; i < 2; ++i) {
         C min_value = min(min(offset[i], offset[i] + C(dim[i])), point[i]);
         C max_value = max(max(offset[i], offset[i] + C(dim[i])), point[i]);

         retval.offset[i] = min_value;
         retval.dim[i] = D(max_value - min_value);
      }
      return retval;
   }

   type intersection(const type& other) {
      type retval;
      for (glm::length_t i = 0; i < 2; ++i) {
         C new_a = max(offset[i], other.offset[i]);
         C new_b = min(offset[i] + C(dim[i]), other.offset[i] + C(other.dim[i]));

         if (new_a > new_b) {
            retval.offset[i] = 0;
            retval.dim[i] = 0;
         } else {
            retval.offset[i] = new_a;
            retval.dim[i] = D(new_b - new_a);
         }
      }
      return retval;
   }

   bool intersects(const type& other) const {
      for (glm::length_t i = 0; i < 2; ++i) {
         C new_a = max(offset[i], other.offset[i]);
         C new_b = min(offset[i] + C(dim[i]), other.offset[i] + C(other.dim[i]));

         if (new_a > new_b)
            return false;
      }
      return true;
   }

   bool contains(const offset_type& point) const {
      return point.x >= offset.x && point.x < right() &&
         point.y >= offset.y && point.y < top();
   }

   bool contains(const type& other) const {
      return contains(other.offset) && contains(other.offset + offset_type(other.dim));
   }
};

template <typename C, typename D, glm::precision P>
struct generic_extents<glm::tvec3<C, P>, glm::tvec3<D, P>> {
   typedef generic_extents<glm::tvec3<C, P>, glm::tvec3<D, P>> type;
   typedef glm::tvec3<C, P> offset_type;
   typedef glm::tvec3<D, P> dim_type;

   offset_type offset;
   dim_type dim;

   C left() const    { return offset.x; }
   C right() const   { return offset.x + C(dim.x); }
   C bottom() const  { return offset.y; }
   C top() const     { return offset.y + C(dim.y); }

   // OpenGL uses RH coordinate systems, so smaller Z coordinates usually denote
   // greater depth, hence offset.z is "back"
   C back() const    { return offset.z; }
   C front() const   { return offset.z + C(dim.z); }

   D width() const   { return dim.x; }
   D height() const  { return dim.y; }
   D depth() const   { return dim.z; }

   offset_type back_top_left() const        { return offset_type(left(), top(), back()); }
   offset_type back_top_right() const       { return offset_type(right(), top(), back()); }
   offset_type back_bottom_left() const     { return offset_type(left(), bottom(), back()); }
   offset_type back_bottom_right() const    { return offset_type(right(), bottom(), back()); }
   offset_type front_top_left() const       { return offset_type(left(), top(), front()); }
   offset_type front_top_right() const      { return offset_type(right(), top(), front()); }
   offset_type front_bottom_left() const    { return offset_type(left(), bottom(), front()); }
   offset_type front_bottom_right() const   { return offset_type(right(), bottom(), front()); }

   offset_type center() const {
      return offset_type(offset.x + C(dim.x) / C(2),
                         offset.y + C(dim.y) / C(2),
                         offset.z + C(dim.z) / C(2));
   }

   bool operator==(const type& other) const {
      return offset == other.offset && dim == other.dim;
   }

   bool operator!=(const type& other) const {
      return !(*this == other);
   }

   bool zilch() const {
      return dim.x == 0 && dim.y == 0 && dim.z == 0;
   }

   bool empty() const {
      return dim.x == 0 || dim.y == 0 || dim.z == 0;
   }

   D size() const {
      return dim.x * dim.y * dim.z;
   }

   type union_bounds(const type& other) {
      type retval;
      for (glm::length_t i = 0; i < 3; ++i) {
         C min_value = min(min(offset[i], offset[i] + C(dim[i])), min(other.offset[i], other.offset[i] + C(dim[i])));
         C max_value = max(max(offset[i], offset[i] + C(dim[i])), max(other.offset[i], other.offset[i] + C(dim[i])));

         retval.offset[i] = min_value;
         retval.dim[i] = D(max_value - min_value);
      }
      return retval;
   }

   type union_bounds(const offset_type& point) {
      type retval;
      for (glm::length_t i = 0; i < 3; ++i) {
         C min_value = min(min(offset[i], offset[i] + C(dim[i])), point[i]);
         C max_value = max(max(offset[i], offset[i] + C(dim[i])), point[i]);

         retval.offset[i] = min_value;
         retval.dim[i] = D(max_value - min_value);
      }
      return retval;
   }

   type intersection(const type& other) {
      type retval;
      for (glm::length_t i = 0; i < 3; ++i) {
         C new_a = max(offset[i], other.offset[i]);
         C new_b = min(offset[i] + C(dim[i]), other.offset[i] + C(other.dim[i]));

         if (new_a > new_b) {
            retval.offset[i] = 0;
            retval.dim[i] = 0;
         } else {
            retval.offset[i] = new_a;
            retval.dim[i] = D(new_b - new_a);
         }
      }
      return retval;
   }

   bool intersects(const type& other) const {
      for (glm::length_t i = 0; i < 3; ++i) {
         C new_a = max(offset[i], other.offset[i]);
         C new_b = min(offset[i] + C(dim[i]), other.offset[i] + C(other.dim[i]));

         if (new_a > new_b)
            return false;
      }
      return true;
   }

   bool contains(const offset_type& point) const {
      return point.x >= offset.x && point.x < right() &&
         point.y >= offset.y && point.y < top() &&
         point.z >= offset.z && point.z < front();
   }

   bool contains(const type& other) const {
      return contains(other.offset) && contains(other.offset + offset_type(other.dim));
   }
};

template <typename T, typename U>
generic_extents<T, U> make_extents(T t, U u) {
   return generic_extents<T, U> { t, u };
}

template <typename T, typename U = T>
generic_extents<T, U> make_extents_from_min_max(T t, T u) {
   return generic_extents<T, U> { t, static_cast<U>(u) - static_cast<U>(t) };
}

} // be

#endif
