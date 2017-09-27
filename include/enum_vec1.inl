#if !defined(BE_CORE_ENUM_VEC1_HPP_) && !defined(DOXYGEN)
#include "enum_vec1.hpp"
#elif !defined(BE_CORE_ENUM_VEC1_INL_)
#define BE_CORE_ENUM_VEC1_INL_

namespace be {

//////////////////////////////////////////////////////////////////////////////
#ifndef GLM_FORCE_NO_CTOR_INIT
template<typename T>
constexpr enum_vec<1, T>::enum_vec()
   : x(static_cast<T>(0)) { }
#endif

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<1, T>::enum_vec(glm::ctor) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<1, T>::enum_vec(T v)
   : x(v) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<1, T>::enum_vec(const enum_vec<2, T>& v)
   : x(v.x) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<1, T>::enum_vec(const enum_vec<3, T>& v)
   : x(v.x) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
constexpr enum_vec<1, T>::enum_vec(const enum_vec<4, T>& v)
   : x(v.x) { }

//////////////////////////////////////////////////////////////////////////////
template<typename T>
T& enum_vec<1, T>::operator[](length_type i) {
   assert(i >= 0 && i < this->length());
   return (&x)[i];
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
const T& enum_vec<1, T>::operator[](length_type i) const {
   assert(i >= 0 && i < this->length());
   return (&x)[i];
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
bool operator==(const enum_vec<1, T>& v1, const enum_vec<1, T>& v2) {
   return (v1.x == v2.x);
}

//////////////////////////////////////////////////////////////////////////////
template<typename T>
bool operator!=(const enum_vec<1, T>& v1, const enum_vec<1, T>& v2) {
   return (v1.x != v2.x);
}

} // be

#endif
