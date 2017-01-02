#pragma once
#ifndef BE_CORE_T_IS_SEED_SEQ_HPP_
#define BE_CORE_T_IS_SEED_SEQ_HPP_

#include "be.hpp"

namespace be {
namespace t {
namespace detail {

template<typename T>
struct HasSeedSeqGenerate {
   template<typename U> static constexpr typename std::is_same<decltype(std::declval<U>().generate((U32*)nullptr, (U32*)nullptr)), void>::type f(U*);
   template<typename U> static constexpr False f(...);

   using type = decltype(f<T>(0));
   static constexpr bool value = type::value;
};

template <typename T, typename = void>
struct GetResultType {
   using type = Nil;
};

template <typename T>
struct GetResultType<T, std::void_t<typename T::result_type>> {
   using type = typename T::result_type;
};

} // be::t::detail

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if a type implements the requirements for a seed
///         sequence.
/// \details see http://en.cppreference.com/w/cpp/concept/SeedSequence
template <typename T, typename S, typename E = S,
   typename D = typename std::decay<T>::type,
   bool = !std::is_convertible<D, detail::GetResultType<D>::type>::value &&
   !std::is_same<D, S>::value &&
   !std::is_same<D, E>::value &&
   detail::HasSeedSeqGenerate<D>::value>
struct IsSeedSeq : False { };

template <typename T, typename S, typename E, typename D>
struct IsSeedSeq<T, S, E, D, true> : True { };

} // be::t
} // be

#endif
