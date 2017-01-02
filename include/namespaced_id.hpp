#pragma once
#ifndef BE_CORE_NAMESPACED_ID_HPP_
#define BE_CORE_NAMESPACED_ID_HPP_

#include "id.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Combines two Ids; the first identifies a namespace and the second
///         identifies a unique thing in that namespace.
///
/// \ingroup ids
struct NamespacedId final {
   constexpr NamespacedId() noexcept { }
   constexpr NamespacedId(Id ns, Id id) noexcept : ns(ns), id(id) { }
   NamespacedId(Id ns, const S& id) : ns(ns), id(id) { }
   NamespacedId(const S& ns, const S& id) : ns(ns), id(id) { }
   constexpr NamespacedId(const NamespacedId& other) noexcept : ns(other.ns), id(other.id) { }

   explicit operator S() const;

   constexpr bool operator==(const NamespacedId& other) const noexcept;
   constexpr bool operator!=(const NamespacedId& other) const noexcept;
   constexpr bool operator<(const NamespacedId& other) const noexcept;
   constexpr bool operator>(const NamespacedId& other) const noexcept;
   constexpr bool operator<=(const NamespacedId& other) const noexcept;
   constexpr bool operator>=(const NamespacedId& other) const noexcept;

   Id ns;   ///< Namespace identifier
   Id id;   ///< Namespace-unique identifier
};

S get_canonical_id_string(const NamespacedId& id);

// TODO PrintTraits<NamespacedId>
// TODO #ifdef BE_CORE_PRINT_TRAITS_HPP_
// TODO #include "print_namespaced_id.hpp"
// TODO #endif
//template<>
//struct PrintTraits<NamespacedId> : PrintTraits<void>
//{
//   void name(std::ostream& os, const NamespacedId&) const;
//};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Outputs a string representation of an NamespacedId object to the
///         provided stream.
///
/// \details The NamespacedId's `ns` and `id` fields are converted to string,
///         separated by a pipe ('|') character and surrounded by braces.  For
///         example: <tt>{#0000000000000000|#0000000000000000}</tt>
template <typename C, typename CTraits>
std::basic_ostream<C, CTraits>& operator<<(std::basic_ostream<C, CTraits>& os, const NamespacedId& nsid) {
   os << (S)nsid;
   return os;
}

} // be

///////////////////////////////////////////////////////////////////////////////
/// \brief  \c std::hash specialization for utilizing NamespacedId objects in
///         \c std::unordered_set and \c std::unordered_map containers.
/// \ingroup ids
template<> struct std::hash<be::NamespacedId> final {

   ////////////////////////////////////////////////////////////////////////////
   /// \brief  Calculates the hashcode of the provided NamespacedId.
   ///
   /// \param  asset_id The NamespacedId to hash.
   ///
   /// \return A hashcode suitable for use in hashtable-based data structures.
   constexpr std::size_t operator()(const be::NamespacedId& nsid) const noexcept {
      return (std::hash<be::Id>()(nsid.ns) * std::size_t(47)) ^ (std::hash<be::Id>()(nsid.id));
   }
};

#endif
