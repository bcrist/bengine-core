#pragma once
#ifndef BE_CORE_T_IS_CONTAINER_HPP_
#define BE_CORE_T_IS_CONTAINER_HPP_

#include "t_select.hpp"

namespace be::t {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if a type is a container.
///
/// \details For the purposes of this utility, a container is any type T
///         which defines a member type `const_iterator`.  Additionally,
///         statically-sized arrays are considered containers, while
///         std::basic_string specializations and Path are not, despite having
///         a `const_iterator` member type.
template <typename T, typename = void>
struct IsContainer : False { };

template <typename T>
struct IsContainer<T, std::void_t<typename T::const_iterator>> : True { };

template <typename T, std::size_t N>
struct IsContainer<T[N]> : True { };

template <typename C, typename T, typename A>
struct IsContainer<std::basic_string<C, T, A>> : False { };

template <typename T, typename = void>
struct IsAssociativeContainer : False { };

template <typename T>
struct IsAssociativeContainer<T, std::void_t<typename T::key_type>> : IsContainer<T> { };

namespace detail {

template <typename T>
struct IsSizedContainerHelper {
   template <typename U> static constexpr typename IsContainer<U>::type f(decltype(std::declval<U>().size())*);
   template <typename U> static constexpr False f(...);
   using type = decltype(f<T>(nullptr));
};

template <typename T>
struct IsPushBackContainerHelper {
   template <typename U> static constexpr typename IsContainer<U>::type f(decltype(std::declval<U>().push_back(std::declval<typename U::value_type>()))*);
   template <typename U> static constexpr False f(...);
   using type = decltype(f<T>(nullptr));
};

template <typename T>
struct IsInsertContainerHelper {
   template <typename U> static constexpr typename IsContainer<U>::type f(decltype(std::declval<U>().insert(std::declval<typename U::value_type>()))*);
   template <typename U> static constexpr False f(...);
   using type = decltype(f<T>(nullptr));
};

} // be::t::detail

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if a type is a sized container.
///
/// \details In addition to the requirements for IsContainer being satisfied,
///         IsSizedContainer also requires the type to have a member function
///         with signature matching `* size()`.  The return type, constness,
///         exception specification, etc. of size() do not matter.
template <typename T>
struct IsSizedContainer : detail::IsSizedContainerHelper<T>::type { };

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct IsPushBackContainer : detail::IsPushBackContainerHelper<T>::type { };

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct IsInsertContainer : detail::IsInsertContainerHelper<T>::type { };

} // be::t

#endif
