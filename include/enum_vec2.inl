#if !defined(BE_CORE_ENUM_VEC2_HPP_) && !defined(DOXYGEN)
#include "enum_vec2.hpp"
#elif !defined(BE_CORE_ENUM_VEC2_INL_)
#define BE_CORE_ENUM_VEC2_INL_

namespace be {

//////////////////////////////////////////////////////////////////////////////
#ifndef GLM_FORCE_NO_CTOR_INIT
template<typename T>
constexpr enum_vec<2, T>::enum_vec()
   : x(static_cast<T>(0)),
     y(static_cast<T>(0)) { }
#endif

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<2, T>::enum_vec(glm::ctor) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<2, T>::enum_vec(T v)
   : x(v),
     y(v) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<2, T>::enum_vec(T x, T y)
   : x(x),
     y(y) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<2, T>::enum_vec(const enum_vec<1, T>& x, const enum_vec<1, T>& y)
   : x(x.x),
     y(y.x) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<2, T>::enum_vec(const enum_vec<3, T>& v)
   : x(v.x),
     y(v.y) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<2, T>::enum_vec(const enum_vec<4, T>& v)
   : x(v.x),
     y(v.y) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
T& enum_vec<2, T>::operator[](length_type i) {
   assert(i >= 0 && i < this->length());
   return (&x)[i];
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
const T& enum_vec<2, T>::operator[](length_type i) const {
   assert(i >= 0 && i < this->length());
   return (&x)[i];
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
bool operator==(const enum_vec<2, T>& v1, const enum_vec<2, T>& v2) {
   return (v1.x == v2.x) && (v1.y == v2.y);
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
bool operator!=(const enum_vec<2, T>& v1, const enum_vec<2, T>& v2) {
   return (v1.x != v2.x) || (v1.y != v2.y);
}

} // be

#endif
