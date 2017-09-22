#pragma once
#ifndef BE_CORE_ID_HPP_
#define BE_CORE_ID_HPP_

#include "be.hpp"
#include "t_size.hpp"

#ifdef DOXYGEN

///////////////////////////////////////////////////////////////////////////////
/// \brief  Indicates that Id names should be remembered and hash collision
///         checking should be enabled.
/// \details If #BE_ID_NAMES_ENABLED and #BE_ID_NAMES_DISABLED are both defined
///         then #BE_ID_NAMES_ENABLED takes precedence.  If neither name is
///         explicitly defined, #BE_ID_NAMES_ENABLED will be defined only if
///         #DEBUG or #BE_BEDITOR is defined.
/// \ingroup ids
#define BE_ID_NAMES_ENABLED

///////////////////////////////////////////////////////////////////////////////
/// \brief  Indicates that Id names should not be remembered and hash collision
///         checking should be disabled.
/// \details Disabling these features will result in a slight performance
///         boost.
/// \ingroup ids
#define BE_ID_NAMES_DISABLED

#endif

#if (defined BE_BEDITOR || defined(DEBUG)) && !defined(BE_ID_NAMES_ENABLED) && !defined(BE_ID_NAMES_DISABLED)
#define BE_ID_NAMES_ENABLED
#endif

#if defined(BE_ID_NAMES_DISABLED) && defined(BE_ID_NAMES_ENABLED)
#undef BE_ID_NAMES_DISABLED
#endif

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Identifier object.
/// \details Id objects are used to efficiently represent game resources and
///         extensible engine types.  They can be used for map keys, event
///         types, etc.
///
///         Each ID consists of a 64-bit integer.  IDs with the same integer
///         value are considered equal.  Id objects can be constructed directly
///         from a numeric value or by hashing a string.  When hashing strings,
///         it is possible for collisions to occur (though it is extremely rare
///         due to the use of 64-bits of state), so the use of hashed strings
///         is best reserved for situations where the string value is known
///         prior to runtime (compile-time constants, asset names, etc.).
///
///         If #BE_ID_NAMES_ENABLED is defined, the string parameters used to
///         construct objects are remembered.  This makes debugging easier,
///         since string representations of the Id can include the original
///         text as well as the hashed value.  In addition, a warning is
///         emitted any time a hash collision occurs (two different strings
///         yield the same hash).  By default, #BE_ID_NAMES_ENABLED will be
///         defined if either #DEBUG or #BE_BEDITOR is defined and not
///         defined otherwise, but this behavior can be overridden by
///         explicitly defining either #BE_ID_NAMES_ENABLED or
///         #BE_ID_NAMES_DISABLED.
/// \ingroup ids
class Id final {
public:
   constexpr Id() noexcept : id_(0) { }
   explicit constexpr Id(U64 id) noexcept : id_(id) { }
   explicit Id(const S& name);
   constexpr Id(const Id& other) noexcept : id_(other.id_) { }

   Id& operator=(Id other) noexcept {
      id_ = other.id_;
      return *this;
   }

   explicit operator S() const;

   explicit constexpr operator U64() const noexcept {
      return id_;
   }

private:
   U64 id_;
};

S get_id_name(Id id);
S get_canonical_id_string(Id id);

constexpr bool operator==(Id a, Id b) noexcept { return (U64)a == (U64)b; }
constexpr bool operator!=(Id a, Id b) noexcept { return (U64)a != (U64)b; }
constexpr bool operator< (Id a, Id b) noexcept { return (U64)a <  (U64)b; }
constexpr bool operator> (Id a, Id b) noexcept { return (U64)a >  (U64)b; }
constexpr bool operator<=(Id a, Id b) noexcept { return (U64)a <= (U64)b; }
constexpr bool operator>=(Id a, Id b) noexcept { return (U64)a >= (U64)b; }

constexpr bool operator==(Id a, U64 b) noexcept { return (U64)a == b; }
constexpr bool operator!=(Id a, U64 b) noexcept { return (U64)a != b; }
constexpr bool operator< (Id a, U64 b) noexcept { return (U64)a <  b; }
constexpr bool operator> (Id a, U64 b) noexcept { return (U64)a >  b; }
constexpr bool operator<=(Id a, U64 b) noexcept { return (U64)a <= b; }
constexpr bool operator>=(Id a, U64 b) noexcept { return (U64)a >= b; }

constexpr bool operator==(U64 a, Id b) noexcept { return a == (U64)b; }
constexpr bool operator!=(U64 a, Id b) noexcept { return a != (U64)b; }
constexpr bool operator< (U64 a, Id b) noexcept { return a <  (U64)b; }
constexpr bool operator> (U64 a, Id b) noexcept { return a >  (U64)b; }
constexpr bool operator<=(U64 a, Id b) noexcept { return a <= (U64)b; }
constexpr bool operator>=(U64 a, Id b) noexcept { return a >= (U64)b; }

// TODO PrintTraits<Id>
//template<>
//struct PrintTraits<Id> : PrintTraits<void>
//{
//   void name(std::ostream& os, const Id&) const;
//};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Outputs a string representation of an Id object to the provided
///         stream.
///
/// \details The format used is the same as that used by Id::string().
///
/// \param  os The \c std::ostream to output to.
/// \param  id The Id to output.
/// \return The same \c std::ostream passed in (allows for operator chaining).
template <typename C, typename CTraits>
std::basic_ostream<C, CTraits>& operator<<(std::basic_ostream<C, CTraits>& os, Id id) {
   os << (S)id;
   return os;
}

} // be

///////////////////////////////////////////////////////////////////////////////
/// \brief  \c std::hash specialization for utilizing Id objects in
///         \c std::unordered_set and \c std::unordered_map containers.
/// \ingroup ids
template<> struct std::hash<be::Id> final {

   ////////////////////////////////////////////////////////////////////////////
   /// \brief  Calculates the hashcode of the provided Id.
   /// \details In 64-bit builds, the Id's value is simply used directly as the
   ///         hashcode.  In 32-bit builds, the upper half of the value is
   ///         XORed with the lower 32 bits to yield a 32-bit \c std::size_t result.
   /// \param id The Id to hash.
   /// \return A hashcode suitable for use in hashtable-based data structures.
   constexpr std::size_t operator()(const be::Id& id) const noexcept {
      return hash_(id, be::t::SizeTag<std::size_t>());
   }

private:
   constexpr std::size_t hash_(const be::Id& id, be::t::SizeTag<be::U32>) const noexcept {
      return static_cast<std::size_t>(be::U64(id) ^ (be::U64(id) >> 32));
   }

   constexpr std::size_t hash_(const be::Id& id, be::t::SizeTag<be::U64>) const noexcept {
      return static_cast<std::size_t>(be::U64(id));
   }

};

#endif
