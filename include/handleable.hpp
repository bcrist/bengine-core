#pragma once
#ifndef BE_CORE_HANDLEABLE_HPP_
#define BE_CORE_HANDLEABLE_HPP_

#include "source_handle.hpp"
#include "const_handle.hpp"
#include "handle.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  CRTP base class implementing the SourceHandle pattern.
///
/// \details Handleable is meant to be abstract, but exposes
///         assign_source_handle_ and swap_source_handle_ protected functions
///         to allow movable objects to easily be created such that when an
///         object is moved, existing handles will automatically refer to the
///         same logical object, which is now located elsewhere in memory.
///
/// \tparam T The type of the derived class.
///
/// \ingroup handles
template <typename T>
class Handleable : Movable {
public:
   explicit operator Handle<T>() { return handle_; }
   explicit operator ConstHandle<T>() const { return handle_; }
   
protected:
   Handleable() {
      handle_.associate(static_cast<T*>(this));
   }

   Handleable(Handleable<T>&& other)
      : handle_(std::move(other.handle_))
   {
      handle_.associate(static_cast<T*>(this));
   }

   void assign_source_handle_(Handleable<T>&& other) {
      handle_ = std::move(other.handle_);
   }

   void swap_source_handle_(Handleable<T>& other) {
      using std::swap;
      swap(handle_, other.handle_);
   }

private:
   SourceHandle<T> handle_;
};

} // be

#endif
