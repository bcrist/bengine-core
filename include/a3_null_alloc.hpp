#pragma once
#ifndef BE_CORE_A3_NULL_ALLOC_HPP_
#define BE_CORE_A3_NULL_ALLOC_HPP_

#include "a3_base_alloc.hpp"

namespace be {
namespace a3 {

class NullAlloc;

template <>
struct AllocTraits<NullAlloc> {
   using has_allocate_all = True;
   using has_deallocate_all = True;
   using has_expand = True;
   using has_reallocate = True;
   using has_owns = True;
   using has_get_owner = True;
   using has_resolve_internal_pointer = True;
   using has_count = True;
   using has_allocated = True;
   using has_available = True;
   using has_size = True;

   static constexpr std::size_t alignment = std::numeric_limits<std::size_t>::max() ^ (std::numeric_limits<std::size_t>::max() >> 1);
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  An allocator that never allocates any memory.
///
/// \details Calls to allocation functions always return null.  Passing any
///         value except nullptr to a deallocation function causes an assertion
///         failure.
///
class NullAlloc : public BaseAlloc<NullAlloc> {
public:
   static constexpr std::size_t preferred_size(std::size_t size) {
      return size;
   }

   using BaseAlloc<NullAlloc>::allocate;
   void* allocate(std::size_t size, std::size_t alignment) {
      BE_IGNORE2(size, alignment);
      return nullptr;
   }

   using BaseAlloc<NullAlloc>::reallocate;
   void* reallocate(void* ptr, std::size_t old_size, std::size_t new_size, std::size_t alignment) {
      BE_IGNORE2(ptr, alignment);
      BE_IGNORE2(old_size, new_size);
      assert(!ptr);
      return nullptr;
   }

   using BaseAlloc<NullAlloc>::deallocate;
   bool deallocate(void* ptr, std::size_t size, std::size_t alignment) {
      BE_IGNORE3(ptr, size, alignment);
      assert(!ptr);
      return true;
   }

   bool deallocate_all() {
      return true;
   }

   bool owns(void* ptr, std::size_t size) const {
      BE_IGNORE2(ptr, size);
      return false;
   }

   std::size_t available() const {
      return 0;
   }

   std::size_t size() const {
      return 0;
   }

};

} // be::a3
} // be

#endif
