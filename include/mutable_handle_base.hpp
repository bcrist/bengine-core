#pragma once
#ifndef BE_CORE_MUTABLE_HANDLE_BASE_HPP_
#define BE_CORE_MUTABLE_HANDLE_BASE_HPP_

#include "handle_base.hpp"

namespace be {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename T>
class MutableHandleBase : public HandleBase<T> {
protected:
   using mutable_base = MutableHandleBase<T>;

   ///////////////////////////////////////////////////////////////////////////////
   /// \brief  Constructs a handle in an invalidated state.
   ///
   /// \details Any attempts to retrieve a pointer to the handled object will
   ///         result in `nullptr` and conversion to bool will yield `false`.
   MutableHandleBase() { }

   ///////////////////////////////////////////////////////////////////////////////
   /// \brief  Constructs a handle that references the same object as the handle
   ///         passed in.
   ///
   /// \param  other The handle to copy.
   MutableHandleBase(const MutableHandleBase<T>& other) : HandleBase<T>(other) { }

   ///////////////////////////////////////////////////////////////////////////////
   /// \brief  Constructs a handle using a specific index and check value.
   MutableHandleBase(U32 index, U32 check) : HandleBase<T>(index, check) { }

};

} // be::detail
} // be

#endif
