#pragma once
#ifndef BE_CORE_HANDLE_BASE_HPP_
#define BE_CORE_HANDLE_BASE_HPP_

#include "handle_manager.hpp"

namespace be {
namespace detail {

template <typename T>
class HandleBase {
public:
   explicit operator bool() const;

   friend bool operator==(const HandleBase<T>& a, const HandleBase<T>& b);
   friend bool operator!=(const HandleBase<T>& a, const HandleBase<T>& b);

protected:
   using base = HandleBase<T>;

   HandleBase();
   HandleBase(const base& other);
   HandleBase(U32 index, U32 check);
   void assign_base_(const base& other);
   void swap_base_(base& other);

   T* get_() const;

   U32 index_;  ///< An index which tells the HandleManager the location of the handled object's address
   U32 check_;  ///< A value used by the HandleManager to ensure that the object at index_ is the same as it was when this handle was created.
};

template <typename T>
std::size_t handled_object_count();

} // be::detail
} // be

#include "handle_base.inl"

#endif
