#include "pch.hpp"
#include "namespaced_id.hpp"
#include <ostream>
#include <sstream>
#include <string>

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a string representation of this NamespacedId.
///
/// \details Uses the same format as `operator<<`.
///
/// \return A string representation of the NamespacedId.
NamespacedId::operator S() const {
#ifdef BE_ID_NAMES_ENABLED
   S name;
   name.reserve(64);
   name.append(1, '{');
   name.append((S)ns);
   name.append(1, '|');
   name.append((S)id);
   name.append(1, '}');
   return name;
#else
   return get_canonical_id_string(*this);
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a cononical string representation of this NamespacedId.
///
/// \details In addition to uniquely describing the Id, any other Ids which
///         compare equal to this one will produce the same string, regardless
///         of whether or not #BE_ID_NAMES_ENABLED is defined.  Uppercase
///         hex digits are always used, and names are not appended even if
///         they are available.
///
///         Additionally this method may be significantly more efficient than
///         operator S().
S get_canonical_id_string(const NamespacedId& nsid) {
   S str(37, '0');
   U64 nsVal = (U64)nsid.ns;
   U64 idVal = (U64)nsid.id;
   auto it = str.rbegin(); // fill in reverse order
   *(it++) = '}';
   if (idVal) {      
      for (int i = 0; i < 16; ++i) {
         *it = "0123456789ABCDEF"[idVal & 0xF];
         idVal >>= 4;
         ++it;
      }
   } else {
      it += 16; // skip "0000000000000000"
   }
   *(it++) = '#';
   *(it++) = '|';
   if (nsVal) {
      for (int i = 0; i < 16; ++i) {
         *it = "0123456789ABCDEF"[nsVal & 0xF];
         nsVal >>= 4;
         ++it;
      }
   } else {
      it += 16; // skip "0000000000000000"
   }
   *(it++) = '#';
   *it = '{';
   return str;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this NamespacedId with another to see if they reference
///         the same thing.
///
/// \details The ns and id fields must both match to be considered equal.
constexpr bool NamespacedId::operator==(const NamespacedId& other) const noexcept {
   return ns == other.ns && id == other.id;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this NamespacedId with another to see if they have
///         reference different things.
///
/// \details At least one of either the ns or id fields must differ for 
///         the NamespacedIds to be considered not equal.
constexpr bool NamespacedId::operator!=(const NamespacedId& other) const noexcept {
   return !(*this == other);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this NamespacedId with another to see if it has a smaller
///         value than the other.
///
/// \return `true` if this NamespacedId's ns is 'less than' other's, or both
///         nss are the same but this NamespacedId's id field is 'less than'
///         other's.
constexpr bool NamespacedId::operator<(const NamespacedId& other) const noexcept {
   return ns < other.ns || ns == other.ns && id < other.id;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this NamespacedId with another to see if it has a larger
///         value than the other.
///
/// \return `true` if this NamespacedId's ns is 'greater than' other's, or both
///         nss are the same but this NamespacedId's id field is 'greater than'
///         other's.
constexpr bool NamespacedId::operator>(const NamespacedId& other) const noexcept {
   return ns > other.ns || ns == other.ns && id > other.id;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this NamespacedId with another to see if it has a not larger
///         value than the other.
///
/// \return `true` if this NamespacedId's ns is not 'greater than' other's, or
///         both nss are the same but this NamespacedId's id field is not
///         'greater than' other's.
constexpr bool NamespacedId::operator<=(const NamespacedId& other) const noexcept {
   return !(*this > other);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this NamespacedId with another to see if it has a not smaller
///         value than the other.
///
/// \return `true` if this NamespacedId's ns is not 'less than' other's, or both
///         nss are the same but this NamespacedId's id field is not 'less than'
///         other's.
constexpr bool NamespacedId::operator>=(const NamespacedId& other) const noexcept {
   return !(*this < other);
}

///////////////////////////////////////////////////////////////////////////////

// TODO PrintTraits<NamespacedId>
//void PrintTraits<NamespacedId>::name(std::ostream& os, const NamespacedId&) const {
//   os << "NamespacedId";
//}

} // be
