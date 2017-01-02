#if !defined(BE_CORE_HANDLE_BASE_HPP_) && !defined(DOXYGEN)
#include "handle_base.hpp"
#elif !defined(BE_CORE_HANDLE_BASE_INL_)
#define BE_CORE_HANDLE_BASE_INL_

namespace be {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if this handle refers to a valid object.
///
/// \return `true` if get_() would return a non-null pointer.
template <typename T>
HandleBase<T>::operator bool() const {
   if (check_ == 0) {   // technically any check_ where check_ & 0x7FFFFFFF == true is invalid, but
      return false;     // those values are never stored in ConstHandle, only HandleManager
   }

   HandleManager<T>& mgr = handle_manager<T>();

   if (index_ >= mgr.entries.size()) {
      return false;
   }

   if (mgr.entries[index_].check != check_) {
      return false;
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this handle with another to see if they reference the same
///         target.
///
/// \details Two handles which once referenced different objects but are both
///         now invalidated will be considered equal.
///
/// \return true if get() == other.get().
template <typename T>
bool operator==(const HandleBase<T>& a, const HandleBase<T>& b) {
   return a.index_ == b.index_ && a.check_ == b.check_ || a.get_() == b.get_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this handle with another to see if they reference
///         different targets.
///
/// \details Two handles which once referenced different objects but are both
///         now invalidated will not be considered different.
///
/// \return true if get() != other.get().
template <typename T>
bool operator!=(const HandleBase<T>& a, const HandleBase<T>& b) {
   return !(a == b);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a handle in an invalidated state.
///
/// \details Any attempts to retrieve a pointer to the handled object will
///         result in `nullptr` and conversion to bool will yield `false`.
template <typename T>
HandleBase<T>::HandleBase()
   : index_(0),
     check_(0)
{ }

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a handle that references the same object as the handle
///         passed in.
///
/// \param  other The handle to copy.
template <typename T>
HandleBase<T>::HandleBase(const base& other)
   : index_(other.index_),
     check_(other.check_)
{ }

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a handle using a specific index and check value.
template <typename T>
HandleBase<T>::HandleBase(U32 index, U32 check)
   : index_(index),
     check_(check)
{ }

///////////////////////////////////////////////////////////////////////////////
/// \brief  Assigns this handle to reference the same object referenced by
///         another handle.
///
/// \param  other The handle to copy.
template <typename T>
void HandleBase<T>::assign_base_(const base& other) {
   index_ = other.index_;
   check_ = other.check_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Assigns this handle to reference the same object referenced by
///         another handle.
///
/// \param  other The handle to copy.
template <typename T>
void HandleBase<T>::swap_base_(base& other) {
   using std::swap;
   swap(index_, other.index_);
   swap(check_, other.check_);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a pointer to the referenced object.
///
/// \details A handle that isn't referencing anything (for instance, one
///         created with the default constructor) will yield a `nullptr`.  A
///         handle which refers to an object that has been destroyed will also
///         return `nullptr`.
///
/// \return A pointer to the referenced object, or `nullptr` if the handle is
///         invalid.
template <typename T>
T* HandleBase<T>::get_() const {
   if (check_ == 0) {
      return nullptr;
   }

   HandleManager<T>& mgr = handle_manager<T>();

   if (index_ >= mgr.entries.size()) {
      return nullptr;
   }

   const HandleEntry<T>& entry = mgr.entries[index_];

   if (entry.check != check_) {
      return nullptr;
   }

   return entry.target;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the current number of handled objects of the template type.
///
/// \details Essentially, this is the number of SourceHandle<T> objects that
///         exist at the moment.
///
/// \return The number of unique valid handled objects of type T.
///
/// \ingroup handles
template <typename T>
std::size_t handled_object_count() {
   HandleManager<T>& mgr = handle_manager<T>();
   return mgr.size;
}

} // be::detail
} // be

#endif
