#if !defined(BE_CORE_HANDLE_MANAGER_HPP_) && !defined(DOXYGEN)
#include "handle_manager.hpp"
#elif !defined(BE_CORE_HANDLE_MANAGER_INL_)
#define BE_CORE_HANDLE_MANAGER_INL_

namespace be {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename T>
HandleManager<T>::HandleManager()
   : first_free(0),
     last_free(0)
{
   BE_ASSERT_MAIN_THREAD();
   entries.reserve(64);

   be_short_debug() << BE_LOG_INTERP(BEIDN_LOG_ATTR_HANDLE_TYPE) " handle manager constructed."
      & hidden(BEID_LOG_ATTR_HANDLE_TYPE) << type_name<T>()
      || default_log();
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
HandleManager<T>::~HandleManager() {
   be_debug() << BE_LOG_INTERP(BEIDN_LOG_ATTR_HANDLE_TYPE) " handle manager destroyed."
      & hidden(BEID_LOG_ATTR_HANDLE_TYPE) << type_name<T>()
      & attr(BEID_LOG_ATTR_REMAINING_HANDLES) << size
      || default_log();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Adds a new entry to this handle manager and sets the handle
///         variables provided to refer to the new handle.
/// \param  index A reference to the \ref be::SourceHandle::index_ "index"
///         member of the SourceHandle associated with the object.
/// \param  check A reference to the \ref be::SourceHandle::check_ "check"
///         member of the SourceHandle associated with the object.
/// \param  target The address of the object that the SourceHandle is
///         associated with.
template <typename T>
void HandleManager<T>::add(U32& index, U32& check, T* target) {
   BE_ASSERT_MAIN_THREAD();

   auto nEntries = entries.size();
   U32 nEntries32 = (U32)nEntries;
   assert(nEntries < std::numeric_limits<U32>::max());
   if (first_free >= nEntries32) {
      index = nEntries32;
      entries.push_back(HandleEntry<T>(0x80000000, target));
      last_free = first_free = (U32)entries.size();
   } else {
      // first_free is inside vector
      index = first_free;
      first_free = entries[first_free].next_free;
      if (last_free == index) {
         last_free = first_free;
      }
      entries[index].target = target;
      entries[index].check |= 0x80000000;
      entries[index].check += 1;
   }

   check = entries[index].check;
   ++size;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Updates an existing handle to point to a new target.  Existing
///         handles will remain valid, but now point to the new target.
/// \param  index Determines the HandleEntry to update.
/// \param  target The new address of the handled object.
template <typename T>
void HandleManager<T>::update(U32 index, T* target) {
   BE_ASSERT_MAIN_THREAD();
   entries[index].target = target;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Invalidates an existing handle, freeing that index for use by new
///         handles and invalidating old handles.
/// \details After an index has been used 2^31 (approximately 2 billion) times,
///         there are no more usable check values, so the index will not be
///         added to the free list and will not be used again until the process
///         ends.
/// \param  index Determines the HandleEntry to invalidate.
template <typename T>
void HandleManager<T>::invalidate(U32 index) {
   BE_ASSERT_MAIN_THREAD();
   entries[index].check &= 0x7FFFFFFF;
   if (entries[index].check != 0x7FFFFFFF) {
      // only add this index to the free list if there is at least 1 usable check value left.

      entries[index].next_free = (U32)entries.size();

      if (last_free >= entries.size()) {
         first_free = index;
      } else {
         entries[last_free].next_free = index;
      }
      last_free = index;
   }
   --size;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a handle manager for a specific type.
template <typename T>
HandleManager<T>& handle_manager() {
   static HandleManager<T> manager;
   return manager;
}

} // be::detail
} // be

#endif
