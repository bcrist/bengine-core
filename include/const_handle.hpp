#pragma once
#ifndef BE_CORE_CONST_HANDLE_HPP_
#define BE_CORE_CONST_HANDLE_HPP_

#include "handle_base.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  A pointer-like object which remains usable when its target is
///         destroyed.
/// \details Handles can be used to uniquely identify an object in memory.
///         Unlike plain pointers or references, if the handle's target is
///         destroyed or invalidated, the handle remains usable (if the object
///         is no longer accessible, it will resolve to null, rather than
///         causing illegal access errors).  Note that this means handles
///         should not be used as keys in map or set data structures which
///         assume key objects remain constant as long as they exist in the
///         container.  If a handle were used as a key (using a comparator
///         based on the value returned by get()), and the handled object were
///         destroyed, it would likely corrupt the rest of the container.
///
///         A ConstHandle behaves similarly to a pointer to const data.  Handle
///         objects can be converted into ConstHandle objects implicitly (but
///         the reverse is not possible).  Neither Handle nor ConstHandle
///         provides any facility for setting or changing the target of a
///         handle.  For that, a SourceHandle is required.
///
/// \note   Due to the C++ static deinitialization order fiasco, static objects
///         should not access any handles during deinitialization, though the
///         destruction of handle objects themselves (other than SourceHandles)
///         will not cause problems at static deinitialization time.
/// \tparam T The type of the object to be handled.
/// \ingroup handles
template <typename T>
class ConstHandle final : public detail::HandleBase<T> {
   friend void swap(ConstHandle<T>& a, ConstHandle<T>& b) { a.swap_(b); }
public:
   ConstHandle() { }
   ConstHandle(const base& other) : base(other) { }
   ConstHandle<T>& operator=(const base& other) {
      assign_base_(other);
      return *this;
   }

   const T* get() const { return get_(); }
   const T* operator->() const { return get_(); }
   const T& operator*() const { return *get_(); }
   
private:
   void swap_(ConstHandle<T>& other) { swap_base_(other); }
};

} // be

#endif
