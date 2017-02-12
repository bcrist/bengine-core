#pragma once
#ifndef BE_CORE_GLM_HELPERS_HPP_
#define BE_CORE_GLM_HELPERS_HPP_

#include "glm.hpp"

namespace std {

template <glm::length_t L, typename T, glm::precision P>
struct make_unsigned<glm::vec<L, T, P>> {
   typedef typename glm::vec<L, typename std::make_unsigned<T>::type, P> type;
};

template <glm::length_t C, glm::length_t R, typename T, glm::precision P>
struct make_unsigned<glm::mat<C, R, T, P>> {
   typedef typename glm::mat<C, R, typename std::make_unsigned<T>::type, P> type;
};

template <glm::length_t L, typename T, glm::precision P>
struct make_signed<glm::vec<L, T, P>> {
   typedef typename glm::vec<L, typename std::make_signed<T>::type, P> type;
};

template <glm::length_t C, glm::length_t R, typename T, glm::precision P>
struct make_signed<glm::mat<C, R, T, P>> {
   typedef typename glm::mat<C, R, typename std::make_signed<T>::type, P> type;
};


template <glm::length_t L, typename T, glm::precision P>
T* begin(glm::vec<L, T, P>& v) {
   return &(v.x);
}

template <glm::length_t L, typename T, glm::precision P>
T* end(glm::vec<L, T, P>& v) {
   return &(v.x) + L;
}

template <glm::length_t L, typename T, glm::precision P>
const T* begin(const glm::vec<L, T, P>& v) {
   return &(v.x);
}

template <glm::length_t L, typename T, glm::precision P>
const T* end(const glm::vec<L, T, P>& v) {
   return &(v.x) + L;
}


template <glm::length_t C, glm::length_t R, typename T, glm::precision P>
T* begin(glm::mat<C, R, T, P>& v) {
   return &(v[0].x);
}

template <glm::length_t C, glm::length_t R, typename T, glm::precision P>
T* end(glm::mat<C, R, T, P>& v) {
   return &(v[0].x) + C * R;
}

template <glm::length_t C, glm::length_t R, typename T, glm::precision P>
const T* begin(const glm::mat<C, R, T, P>& v) {
   return &(v[0].x);
}

template <glm::length_t C, glm::length_t R, typename T, glm::precision P>
const T* end(const glm::mat<C, R, T, P>& v) {
   return &(v[0].x) + C * R;
}


template <typename T, glm::precision P>
T* begin(glm::tquat<T, P>& v) {
   return &(v.x);
}

template <typename T, glm::precision P>
T* end(glm::tquat<T, P>& v) {
   return &(v.x) + 4;
}

template <typename T, glm::precision P>
const T* begin(const glm::tquat<T, P>& v) {
   return &(v.x);
}

template <typename T, glm::precision P>
const T* end(const glm::tquat<T, P>& v) {
   return &(v.x) + 4;
}


template <typename T, glm::precision P>
T* begin(glm::tdualquat<T, P>& v) {
   return &(v.real.x);
}

template <typename T, glm::precision P>
T* end(glm::tdualquat<T, P>& v) {
   return &(v.real.x) + 8;
}

template <typename T, glm::precision P>
const T* begin(const glm::tdualquat<T, P>& v) {
   return &(v.real.x);
}

template <typename T, glm::precision P>
const T* end(const glm::tdualquat<T, P>& v) {
   return &(v.real.x) + 8;
}

} // std

namespace be {
namespace t {

template <typename T>
struct is_container;

template <glm::length_t L, typename T, glm::precision P>
struct is_container<glm::vec<L, T, P>> : public True { };

template <glm::length_t C, glm::length_t R, typename T, glm::precision P>
struct is_container<glm::mat<C, R, T, P>> : public True { };

template <typename T, glm::precision P>
struct is_container<glm::tquat<T, P>> : public True { };

template <typename T, glm::precision P>
struct is_container<glm::tdualquat<T, P>> : public True { };

} // be::t
} // be

#endif
