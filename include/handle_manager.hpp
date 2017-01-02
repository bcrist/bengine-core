#pragma once
#ifndef BE_CORE_HANDLE_MANAGER_HPP_
#define BE_CORE_HANDLE_MANAGER_HPP_

#include "log_attrib_ids.hpp"
#include "log_expr_ops.hpp"
#include "log_handle.hpp"
#include "service_log.hpp"
#include <cassert>

namespace be {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Describes an object in a HandleManager which can be referenced by
///         handles.
/// \tparam T The type of object that is being handled.
template <typename T>
struct HandleEntry {
   ////////////////////////////////////////////////////////////////////////////
   /// \brief  Constructs a HandleEntry using the provided check value and
   ///         target pointer.
   HandleEntry(U32 check, T* target)
      : check(check), target(target) { }

   /// \brief  Value used to determine if a handle is valid.
   /// \details When the MSB of check is set, it means this entry refers to a
   ///         valid object somewhere.  When the MSB is not set, it means
   ///         the entry is currently invalid; it can be reused for another
   ///         handle.
   ///
   ///         Each time an entry is used for a new handle, the lower 31 bits
   ///         are incremented once.  A handle's check value must match the
   ///         check value in the HandleEntry it's index refers to,
   ///         otherwise the handle doesn't refer to the same object.
   U32 check;      
   union {
      T* target;     ///< If a valid entry, points to the handled object.
      U32 next_free; ///< If this is an unused entry, this is the index of the next unused HandleEntry index.
   };
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  A HandleManager is a static object which is used to convert a
///         handle into a pointer to the object it points to.
/// \details A single HandleManager exists for each type of object that can
///         have handles, i.e. each specialization of SourceHandle, Handle
///         or ConstHandle.
/// \tparam T the type of object this manager deals with handles to.
template <typename T>
struct HandleManager {
   HandleManager();
   ~HandleManager();

   void add(U32& index, U32& check, T* target);
   void update(U32 index, T* target);
   void invalidate(U32 index);

   std::vector<HandleEntry<T>> entries;///< Holds the address of all currently valid objects which can have handles to them.
   U32 first_free;                     ///< Index in entries of the first unused (invalid) entry.  When an entry is added, it is located at this index.  Then first_free becomes the old entry's next_free index.
   U32 last_free;                      ///< Index in entries of the last unused (invalid) entry.  When an entry is invalidated, this element's next_free is set to the newly invalidated entry and last_free is set to the new entry's index.
   U32 size;                           ///< Holds the number of valid entries present in entries.  Not necessarily equal to entries.size().
};

template <typename T>
HandleManager<T>& handle_manager();

} // be::detail
} // be

#include "handle_manager.inl"

#endif
