#pragma once
#ifndef BE_CORE_A3_VMM_ALLOC_HPP_
#define BE_CORE_A3_VMM_ALLOC_HPP_

#include "a3_base_alloc.hpp"

namespace be {
namespace a3 {

class VmmAlloc;

template <>
struct AllocTraits<VmmAlloc> : AllocTraits<void> {
   using has_expand = True;
   using has_reallocate = True;

   static constexpr std::size_t alignment = 4096;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Basic allocator that defers directly to the virtual memory manager.
class VmmAlloc : public BaseAlloc<VmmAlloc> {
public:
   static const std::size_t granularity;

   using BaseAlloc<VmmAlloc>::allocate;
   void* allocate(std::size_t size, std::size_t alignment) {
      BE_IGNORE(alignment);
      assert(alignment <= granularity);
      return BE_VMM_ALLOC(size);
   }

   bool expand(void* ptr, std::size_t old_size, std::size_t delta_size) {
      return ptr && expand_to_alignment(old_size, granularity) >= old_size + delta_size;
   }

   using BaseAlloc<VmmAlloc>::reallocate;
   void* reallocate(void* ptr, std::size_t old_size, std::size_t new_size, std::size_t alignment) {
      if (ptr && expand_to_alignment(old_size, granularity) >= new_size) {
         return ptr;
      }

      if (new_size > old_size) {
         void* new_ptr = allocate(new_size, alignment);
         if (ptr && new_ptr) {
            memcpy(new_ptr, ptr, min(old_size, new_size));
            deallocate(ptr, old_size, alignment);
         }
         ptr = new_ptr;
      }
      return ptr;
   }

   using BaseAlloc<VmmAlloc>::deallocate;
   bool deallocate(void* ptr, std::size_t size, std::size_t alignment) {
      BE_IGNORE2(size, alignment);
      return !ptr || BE_VMM_FREE(ptr);
   }

};

} // be::a3
} // be

#endif
