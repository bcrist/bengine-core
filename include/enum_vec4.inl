#if !defined(BE_CORE_ENUM_VEC4_HPP_) && !defined(DOXYGEN)
#include "enum_vec4.hpp"
#elif !defined(BE_CORE_ENUM_VEC4_INL_)
#define BE_CORE_ENUM_VEC4_INL_

namespace be {

//////////////////////////////////////////////////////////////////////////////
#ifndef GLM_FORCE_NO_CTOR_INIT
template<typename T>
constexpr enum_vec<4, T>::enum_vec()
   : x(static_cast<T>(0)),
     y(static_cast<T>(0)),
     z(static_cast<T>(0)),
     w(static_cast<T>(0)) { }
#endif

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(glm::ctor) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(T v)
   : x(v),
     y(v),
     z(v),
     w(v) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(T x, T y, T z, T w)
   : x(x),
     y(y),
     z(z),
     w(w) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(const enum_vec<1, T>& x, const enum_vec<1, T>& y, const enum_vec<1, T>& z, const enum_vec<1, T>& w)
   : x(x.x),
     y(y.x),
     z(z.x),
     w(w.x){ }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(const enum_vec<2, T>& xy, T z, T w)
   : x(xy.x),
     y(xy.y),
     z(z),
     w(w) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(const enum_vec<2, T>& xy, const enum_vec<1, T>& z, const enum_vec<1, T>& w)
   : x(xy.x),
     y(xy.y),
     z(z.x),
     w(w.x) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(T x, const enum_vec<2, T>& yz, T w)
   : x(x),
     y(yz.x),
     z(yz.y),
     w(w) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(const enum_vec<1, T>& x, const enum_vec<2, T>& yz, const enum_vec<1, T>& w)
   : x(x.x),
     y(yz.x),
     z(yz.y),
     w(w.x) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(T x, T y, const enum_vec<2, T>& zw)
   : x(x),
     y(y),
     z(zw.x),
     w(zw.y) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(const enum_vec<1, T>& x, const enum_vec<1, T>& y, const enum_vec<2, T>& zw)
   : x(x.x),
     y(y.x),
     z(zw.x),
     w(zw.y) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(const enum_vec<3, T>& xyz, T w)
   : x(xyz.x),
     y(xyz.y),
     z(xyz.z),
     w(w) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(const enum_vec<3, T>& xyz, const enum_vec<1, T>& w)
   : x(xyz.x),
     y(xyz.y),
     z(xyz.z),
     w(w.x) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(T x, const enum_vec<3, T>& yzw)
   : x(x),
     y(yzw.x),
     z(yzw.y),
     w(yzw.z) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(const enum_vec<1, T>& x, const enum_vec<3, T>& yzw)
   : x(x.x),
     y(yzw.x),
     z(yzw.y),
     w(yzw.z) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<4, T>::enum_vec(const enum_vec<2, T>& xy, const enum_vec<2, T>& zw)
   : x(xy.x),
     y(xy.y),
     z(zw.x),
     w(zw.y) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
T& enum_vec<4, T>::operator[](length_type i) {
   assert(i >= 0 && i < this->length());
   return (&x)[i];
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
const T& enum_vec<4, T>::operator[](length_type i) const {
   assert(i >= 0 && i < this->length());
   return (&x)[i];
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
bool operator==(const enum_vec<4, T>& v1, const enum_vec<4, T>& v2) {
   return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w);
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
bool operator!=(const enum_vec<4, T>& v1, const enum_vec<4, T>& v2) {
   return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z) || (v1.w != v2.w);
}

} // be

#endif
