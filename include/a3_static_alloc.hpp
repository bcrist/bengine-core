#pragma once
#ifndef BE_CORE_A3_STATIC_ALLOC_HPP_
#define BE_CORE_A3_STATIC_ALLOC_HPP_

#include "a3_base_alloc.hpp"

namespace be {
namespace a3 {

template <std::size_t Size, std::size_t Alignment>
class StaticAlloc;

template <std::size_t Size, std::size_t Alignment>
struct AllocTraits<StaticAlloc<Size, Alignment>> : AllocTraits<void> {
   using has_allocate_all = True;
   using has_deallocate_all = True;
   using has_expand = True;
   using has_owns = True;
   using has_get_owner = True;
   using has_resolve_internal_pointer = True;
   using has_size = True;

   static constexpr std::size_t alignment = Alignment;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Basic allocator that always returns an uninitialized static array.
///
/// \details Allocations will be check to ensure that only
///         one call to allocate() is made until deallocate() is called.
template <std::size_t Size, std::size_t Alignment>
class StaticAlloc : public BaseAlloc<StaticAlloc<Size, Alignment>>, Immovable {
public:
   static constexpr std::size_t preferred_size(std::size_t size) {
      BE_IGNORE(size);
      return Size;
   }

   using base_type::allocate;
   void* allocate(std::size_t size, std::size_t alignment) {
      BE_IGNORE2(size, alignment);
      assert(alignment <= Alignment);
      assert(size <= Size);
      return allocate_all();
   }

   void* allocate_all() {
      if (consumed_) {
         return nullptr;
      }
      consumed_ = true;
      return data_;
   }

   bool expand(void* ptr, std::size_t old_size, std::size_t delta_size) {
      BE_IGNORE(ptr);
      assert(ptr == data_);
      assert(consumed_);
      return old_size + delta_size <= Size;
   }

   using base_type::reallocate;
   void* reallocate(void* ptr, std::size_t old_size, std::size_t new_size, std::size_t alignment) {
      BE_IGNORE2(old_size, alignment);
      assert(ptr == data_);
      assert(consumed_);
      if (new_size <= Size) {
         return ptr;
      }

      return nullptr;
   }

   using base_type::deallocate;
   bool deallocate(void* ptr, std::size_t size, std::size_t alignment) {
      BE_IGNORE3(ptr, size, alignment);
      assert(ptr == data_);
      return deallocate_all();
   }

   bool deallocate_all() {
      if (consumed_) {
         consumed_ = false;
         return true;
      }
      return false;
   }

   bool owns(void* ptr, std::size_t size) const {
      BE_IGNORE(size);
      char* cptr = static_cast<char*>(ptr);
      std::ptrdiff_t diff = cptr - data_;
      return diff >= 0 && diff < static_cast<std::ptrdiff_t>(Size);
   }

   static alloc_type& get_owner(void* ptr, std::size_t size) { // ptr must be something returned by allocate() or allocate_all()
      BE_IGNORE(size);
      assert(ptr);
      return *reinterpret_cast<alloc_type*>(static_cast<char*>(ptr) - offsetof(alloc_type, data_));
   }

   void* resolve_internal_pointer(void* ptr) {
      if (owns(ptr, 0)) {
         return &data_;
      } else {
         return nullptr;
      }
   }

   std::size_t size() const {
      return Size;
   }

private:
   char alignas(Alignment) data_[Size];
   bool consumed_ = false;
};

} // be::a3
} // be

#endif
