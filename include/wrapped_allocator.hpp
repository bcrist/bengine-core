#pragma once
#ifndef BE_DETAIL_WRAPPED_ALLOCATOR_HPP_
#define BE_DETAIL_WRAPPED_ALLOCATOR_HPP_

#include "be.hpp"

namespace be {

template <typename A>
struct WrappedAllocator : A {
private:
   using allocator_ = A;
   using traits_ = std::allocator_traits<A>;

public:
   using value_type = typename traits_::value_type;
   using pointer = typename traits_::pointer;
   using const_pointer = typename traits_::const_pointer;
   using void_pointer = typename traits_::void_pointer;
   using const_void_pointer = typename traits_::const_void_pointer;
   using reference = typename t::Select<::std::is_void<value_type>::value, int, value_type>::type&;
   using const_reference = typename t::Select<::std::is_void<const value_type>::value, const int, const value_type>::type&;
   using size_type = typename traits_::size_type;
   using difference_type = typename traits_::difference_type;
   using propagate_on_container_copy_assignment = typename traits_::propagate_on_container_copy_assignment;
   using propagate_on_container_move_assignment = typename traits_::propagate_on_container_move_assignment;
   using propagate_on_container_swap = typename traits_::propagate_on_container_swap;

   WrappedAllocator<A> select_on_container_copy_construction(Nil = Nil()) const {
      return traits_::select_on_container_copy_construction(*this);
   }

   template <typename O>
   struct rebind {
   private:
      using other_alloc = typename traits_::template rebind_alloc<O>;
   public:
      using other = WrappedAllocator<other_alloc>;
   };

   pointer address(reference val) const {
      return std::addressof(val);
   }

   const_pointer address(const_reference val) const {
      return std::addressof(val);
   }

   WrappedAllocator() noexcept : allocator_() { }
   WrappedAllocator(const allocator_& other) noexcept : allocator_(other) { }
   WrappedAllocator(const WrappedAllocator<A>& other) noexcept : allocator_(other) { }

   template<typename O>
   WrappedAllocator(const O& other) noexcept
      : allocator_(other)
   { }

   template<typename O>
   WrappedAllocator(O& other) noexcept
      : allocator_(other)
   { }

   WrappedAllocator<A>& operator=(const allocator_& other) {
      allocator_::operator=(other);
      return *this;
   }

   WrappedAllocator<A>& operator=(const WrappedAllocator<A>& other) {
      allocator_::operator=(other);
      return *this;
   }

   template<typename Other>
   WrappedAllocator<A>& operator=(const Other& other) {
      allocator_::operator=(other);
      return *this;
   }

   BE_ALLOCATOR_DECL pointer allocate(size_type count) {
      return allocator_::allocate(count);
   }

   BE_ALLOCATOR_DECL pointer allocate(size_type count, const_void_pointer hint, Nil = Nil()) {
      return traits_::allocate(*this, count, hint);
   }

   void deallocate(pointer ptr, size_type count) {
      allocator_::deallocate(ptr, count);
   }

   template <typename T, typename... Args>
   void construct(T* ptr, Args&&... args) {
      traits_::construct(*this, ptr, std::forward<Args>(args)...);
   }

   template <typename T>
   void destroy(T* ptr) {
      traits_::destroy(*this, ptr);
   }

   size_type max_size(Nil = Nil()) const noexcept {
      return traits_::max_size(*this);
   }
};

template <typename T, typename O>
bool operator==(const WrappedAllocator<T>& left, const WrappedAllocator<O>& right) noexcept {
   return static_cast<const T&>(left) == static_cast<const O&>(right);
}

template <typename T, typename O>
bool operator!=(const WrappedAllocator<T>& left, const WrappedAllocator<O>& right) noexcept {
   return !(left == right);
}

} // be

#endif
