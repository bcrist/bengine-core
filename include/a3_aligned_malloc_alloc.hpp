#pragma once
#ifndef BE_CORE_A3_ALIGNED_MALLOC_ALLOC_HPP_
#define BE_CORE_A3_ALIGNED_MALLOC_ALLOC_HPP_

#include "a3_base_alloc.hpp"

namespace be {
namespace a3 {

class AlignedMallocAlloc;

template <>
struct AllocTraits<AlignedMallocAlloc> : AllocTraits<void> {
   using has_reallocate = True;
};

///////////////////////////////////////////////////////////////////////////////
class AlignedMallocAlloc : public BaseAlloc<AlignedMallocAlloc> {
public:
   using BaseAlloc<AlignedMallocAlloc>::allocate;
   void* allocate(std::size_t size, std::size_t alignment) {
      return BE_ALIGNED_ALLOC(alignment, size);
   }

   using BaseAlloc<AlignedMallocAlloc>::reallocate;
   void* reallocate(void* ptr, std::size_t old_size, std::size_t new_size, std::size_t alignment) {
      BE_IGNORE(old_size);
      return BE_ALIGNED_REALLOC(alignment, ptr, new_size);
   }

   using BaseAlloc<AlignedMallocAlloc>::deallocate;
   bool deallocate(void* ptr, std::size_t size, std::size_t alignment) {
      BE_IGNORE2(size, alignment);
      BE_ALIGNED_FREE(ptr);
      return true;
   }

};

} // be::a3
} // be

#endif
