#if !defined(BE_CORE_SOURCE_HANDLE_HPP_) && !defined(DOXYGEN)
#include "source_handle.hpp"
#elif !defined(BE_CORE_SOURCE_HANDLE_INL_)
#define BE_CORE_SOURCE_HANDLE_INL_

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a SourceHandle.
/// \details The handle can be copied and converted to other types of handle,
///         but the handle and all handles derived from it will remain invalid
///         until associate() is called to specify the target of the handle.
///         After that the handles will remain valid until the SourceHandle is
///         destroyed or moved.
template <typename T>
SourceHandle<T>::SourceHandle() {
   detail::HandleManager<T>& mgr = detail::handle_manager<T>();
   mgr.add(index_, check_, nullptr);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Move-constructs an existing SourceHandle.
/// \details Handles which previously referred to the object associate()ed with
///         "other" will now refer to the object associate()ed with this
///         SourceHandle (after associate() is called).  From this point on,
///         "other" will be an invalidated handle.
/// \param  other The SourceHandle to move.
template <typename T>
SourceHandle<T>::SourceHandle(SourceHandle<T>&& other)
   : mutable_base(other)
{
   detail::HandleManager<T>& mgr = detail::handle_manager<T>();
   mgr.update(index_, nullptr);

   other.check_ = 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Move-assigns another SourceHandle.
/// \details Handles which previously referred to the object associate()ed with
///         "other" will now refer to the object associate()ed with this
///         SourceHandle, and vice-versa.
/// \param  other The SourceHandle to swap with this one.
/// \return This SourceHandle.
template <typename T>
SourceHandle<T>& SourceHandle<T>::operator=(SourceHandle<T>&& other) {
   swap(*this, other);

   return *this;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys a SourceHandle.
/// \details If the handle is valid, all handles referring to the object 
///         associate()ed with this SourceHandle will now be invalidated.
template <typename T>
SourceHandle<T>::~SourceHandle() {
   if (check_) {
      detail::HandleManager<T>& mgr = detail::handle_manager<T>();
      mgr.invalidate(index_);
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Associates a target object with this handle.
/// \details This association needs to be done only once, and is not changed
///         when a SourceHandle is move-assigned.
/// \param  target The object to associate with this SourceHandle.
template <typename T>
void SourceHandle<T>::associate(T* target) {
   detail::HandleManager<T>& mgr = detail::handle_manager<T>();
   mgr.update(index_, target);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
void SourceHandle<T>::swap_(SourceHandle& other) {
   using std::swap;

   T* target = get_();
   T* other_target = other.get_();

   swap(index_, other.index_);
   swap(check_, other.check_);

   detail::HandleManager<T>& mgr = detail::handle_manager<T>();

   if (check_) {
      mgr.update(index_, target);
   }
   
   if (other.check_) {
      mgr.update(other.index_, other_target);
   }
}

} // be

#endif
