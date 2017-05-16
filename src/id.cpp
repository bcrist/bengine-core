#include "id.hpp"
#include "id_collision_handler.hpp"
#include <unordered_map>
#include <mutex>

///////////////////////////////////////////////////////////////////////////////
/// \brief  The initial state used in the 64-bit FNV-1a hash algorithm.
/// \ingroup ids
#define BE_CORE_ID_FNV_OFFSET_BASIS 14695981039346656037ULL

///////////////////////////////////////////////////////////////////////////////
/// \brief  A large prime number used by the 64-bit FNV-1a hash algorithm.
/// \ingroup ids
#define BE_CORE_ID_FNV_PRIME 1099511628211ULL

namespace be {
namespace {

#ifdef BE_ID_NAMES_ENABLED

using IdNamesMap = std::unordered_map<U64, S>;

// Retrieves a map of Id values to strings used to generate them.
IdNamesMap& get_names() {
   static IdNamesMap names;
   return names;
}

// Retrieves a mutex that protects the map above.
std::mutex& get_names_mutex() {
   static std::mutex mutex;
   return mutex;
}

#endif

// Calculates 64-bit FNV-1a hash of provided name.
U64 hash(const S& name) {
   U64 hash = BE_CORE_ID_FNV_OFFSET_BASIS;
   for (auto i(name.begin()), end(name.end()); i != end; ++i) {
      hash = (hash ^ static_cast<U8>(*i)) * BE_CORE_ID_FNV_PRIME;
   }

#ifdef BE_ID_NAMES_ENABLED

   IdNamesMap& names = get_names();
   std::mutex& mutex = get_names_mutex();
   std::lock_guard<std::mutex> lock(mutex);
   
   auto i(names.find(hash));
   if (i != names.end()) {
      // Check for collision with any previously hashed name
      if (i->second != name) {
         detail::handle_id_collision(hash, i->second, name);
      }
   } else {
      // Save this name/hash for future checks
      names[hash] = name;          
   }

#endif

   return hash;
}

} // be::()

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new Id object by hashing a string value.
///
/// \details The hash function used is a Fowler-Noll-Vo variant.
///         (64-bit FNV-1a)
///
/// \param  name The string to hash to generate the numeric value for the Id.
Id::Id(const S& name)
   : id_(hash(name))
{ }

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a string representation of this Id.
///
/// \details The string will be at least seventeen characters long, beginning
///         with a '#' and followed by the value of the Id in 16 digits of
///         zero-padded hexadecimal.  The case of hexadecimal digits A-F should
///         not be relied upon.
///
///         If #BE_ID_NAMES_ENABLED is defined and an Id with this value has
///         been constructed by hashing a string, the hex value is followed by
///         a ':' and the string that was hashed like so:
///         <tt>#8ae793af1a1dc82a:Aduka!</tt>.
///
/// \note   Even if an object was created directly using a numeric value, it
///         may still display a textual name as well if another object was
///         created from a string that hashes to the same value.  For instance:
///
/// \code
///    be::Id id1(0x90285684421F9857);
///    std::cout << id1 << std::endl;  // outputs "#90285684421f9857"
///
///    be::Id id2("asdf");
///    std::cout << id1 << std::endl;  // outputs "#90285684421f9857:asdf"
///    std::cout << id2 << std::endl;  // outputs "#90285684421f9857:asdf"
/// \endcode
///
/// \return A string representation of the Id.
Id::operator S() const {
#ifdef BE_ID_NAMES_ENABLED
   S name = get_canonical_id_string(*this);
   S extra = get_id_name(*this);
   if (!extra.empty()) {
      name.reserve(name.length() + extra.length() + 1);
      name.append(1, ':');
      name.append(extra);
   }
   return name;
#else
   return get_canonical_id_string(*this);
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a canonical string representation of this Id.
///
/// \details In addition to uniquely describing the Id, any other Ids which
///         compare equal to this one will produce the same string, regardless
///         of whether or not #BE_ID_NAMES_ENABLED is defined.  Uppercase
///         hex digits are always used, and names are not appended even if
///         they are available.
///
///         Additionally this method may be significantly more efficient than
///         operator S().
S get_canonical_id_string(Id id) {
   S str(17, '0');
   U64 val = (U64)id;
   if (val) {
      auto it = str.rbegin(); // fill in reverse order
      for (int i = 0; i < 16; ++i) {
         *it = "0123456789ABCDEF"[val & 0xF];
         val >>= 4;
         ++it;
      }
   }
   str[0] = '#';
   return str;
}

///////////////////////////////////////////////////////////////////////////////

// TODO PrintTraits<Id>
//void PrintTraits<Id>::name(std::ostream& os, const Id&) const {
//   os << "Id";
//}

#if defined(BE_ID_NAMES_ENABLED) || defined(DOXYGEN)

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the string that was used to generate the provided Id.
///
/// \details The string returned can be used to recreate the Id by passing it
///         to the Id constructor.  If the Id has never been generated by
///         hashing a string (by using the U64 constructor) then an empty
///         string will be returned.  In this case `Id(getName(id)) == id` is
///         not guaranteed.
///
///         This function is only available if #BE_ID_NAMES_ENABLED is defined
///         (such as in a debug build).  Otherwise it will always return an
///         empty string.
S get_id_name(Id id) {
   IdNamesMap& names = get_names();
   std::mutex& mutex = get_names_mutex();
   std::lock_guard<std::mutex> lock(mutex);

   auto i(names.find((U64)id));
   if (i != names.end())
      return i->second;

   return S();
}
#else
S get_id_name(Id) {
   return S();
}
#endif

} // be
