#pragma once
#ifndef BE_CORE_A3_MALLOC_ALLOC_HPP_
#define BE_CORE_A3_MALLOC_ALLOC_HPP_

#include "a3_base_alloc.hpp"

namespace be {
namespace a3 {

class MallocAlloc;

template <>
struct AllocTraits<MallocAlloc> : AllocTraits<void> {
   using has_reallocate = True;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Basic allocator that defers to malloc.
///
/// \details Does not support alignments larger than alignof(double).  Use
///         AlignedMallocAlloc instead if large alignments are required.
class MallocAlloc : public BaseAlloc<MallocAlloc> {
public:
   using BaseAlloc<MallocAlloc>::allocate;
   void* allocate(std::size_t size, std::size_t alignment) {
      BE_IGNORE(alignment);
      assert(alignment <= MallocAlloc::alignment);
      return malloc(size);
   }

   using BaseAlloc<MallocAlloc>::reallocate;
   void* reallocate(void* ptr, std::size_t old_size, std::size_t new_size, std::size_t alignment) {
      BE_IGNORE2(old_size, alignment);
      assert(alignment <= MallocAlloc::alignment);
      return realloc(ptr, new_size);
   }

   using BaseAlloc<MallocAlloc>::deallocate;
   bool deallocate(void* ptr, std::size_t size, std::size_t alignment) {
      BE_IGNORE2(size, alignment);
      assert(alignment <= MallocAlloc::alignment);
      free(ptr);
      return true;
   }

};

} // be::a3
} // be

#endif
