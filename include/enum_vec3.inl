#if !defined(BE_CORE_ENUM_VEC3_HPP_) && !defined(DOXYGEN)
#include "enum_vec3.hpp"
#elif !defined(BE_CORE_ENUM_VEC3_INL_)
#define BE_CORE_ENUM_VEC3_INL_

namespace be {

//////////////////////////////////////////////////////////////////////////////
#ifndef GLM_FORCE_NO_CTOR_INIT
template<typename T>
constexpr enum_vec<3, T>::enum_vec()
   : x(static_cast<T>(0)),
     y(static_cast<T>(0)),
     z(static_cast<T>(0)) { }
#endif

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<3, T>::enum_vec(glm::ctor) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<3, T>::enum_vec(T v)
   : x(v),
     y(v),
     z(v) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<3, T>::enum_vec(T x, T y, T z)
   : x(x),
     y(y),
     z(z) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<3, T>::enum_vec(const enum_vec<1, T>& x, const enum_vec<1, T>& y, const enum_vec<1, T>& z)
   : x(x.x),
     y(y.x),
     z(z.x) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<3, T>::enum_vec(const enum_vec<2, T>& xy, T z)
   : x(xy.x),
     y(xy.y),
     z(z) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<3, T>::enum_vec(const enum_vec<2, T>& xy, const enum_vec<1, T>& z)
   : x(xy.x),
     y(xy.y),
     z(z.x) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<3, T>::enum_vec(T x, const enum_vec<2, T>& yz)
   : x(x),
     y(yz.x),
     z(yz.y) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<3, T>::enum_vec(const enum_vec<1, T>& x, const enum_vec<2, T>& yz)
   : x(x.x),
     y(yz.x),
     z(yz.y) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<3, T>::enum_vec(const enum_vec<4, T>& v)
   : x(v.x),
     y(v.y),
     z(v.z) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
T& enum_vec<3, T>::operator[](length_type i) {
   assert(i >= 0 && i < this->length());
   return (&x)[i];
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
const T& enum_vec<3, T>::operator[](length_type i) const {
   assert(i >= 0 && i < this->length());
   return (&x)[i];
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
bool operator==(const enum_vec<3, T>& v1, const enum_vec<3, T>& v2) {
   return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
bool operator!=(const enum_vec<3, T>& v1, const enum_vec<3, T>& v2) {
   return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z);
}

} // be

#endif
