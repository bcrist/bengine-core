#pragma once
#ifndef BE_CORE_A3_BASE_ALLOC_HPP_
#define BE_CORE_A3_BASE_ALLOC_HPP_

#include "be.hpp"
#include "alg.hpp"

#include <limits>
#include <algorithm>
#include <cassert>

namespace be {
namespace a3 {

inline void assert_if(std::false_type) { }
inline void assert_if(std::true_type) { assert(false); }

inline void assert_if_not(std::false_type) { assert(false); }
inline void assert_if_not(std::true_type) { }

inline void assert_alignment(std::size_t alignment) {
   BE_IGNORE(alignment);
   assert((alignment & (alignment - 1)) == 0);
}

inline std::size_t expand_to_alignment(std::size_t size, std::size_t alignment) {
   assert_alignment(alignment);
   std::size_t am1 = alignment - 1;
   return (size + am1) & ~am1;
}

template <typename T>
T* expand_to_alignment(T* ptr, std::size_t alignment) {
   static_assert(sizeof(T) == 1, "expand_to_alignment(T*): sizeof(T) != 1");
   assert_alignment(alignment);
   std::uintptr_t am1 = static_cast<std::uintptr_t>(alignment) - 1;
   return reinterpret_cast<T*>((reinterpret_cast<std::uintptr_t>(ptr) + am1) & ~am1);
}

template <std::size_t Alignment>
struct AssertAlignment {
   static_assert((Alignment & (Alignment - 1)) == 0, "Alignment must be a power of two!");
   static constexpr std::size_t value = Alignment;

   static constexpr std::size_t expand_to_alignment(std::size_t size) {
      return (size + value - 1) & ~(value - 1);
   }

   template <typename T>
   static constexpr T* expand_to_alignment(T* ptr) {
      static_assert(sizeof(T) == 1, "expand_to_alignment(T*): sizeof(T) != 1");
      return reinterpret_cast<T*>((reinterpret_cast<std::uintptr_t>(ptr) +
                                  static_cast<std::uintptr_t>(value) - 1) &
                                  ~(static_cast<std::uintptr_t>(value) - 1));
   }
};

template <std::size_t Alignment>
constexpr std::size_t expand_to_alignment(std::size_t size) {
   return AssertAlignment<Alignment>::expand_to_alignment(size);
}

template <std::size_t Alignment, typename T>
constexpr T* expand_to_alignment(T* ptr) {
   return AssertAlignment<Alignment>::expand_to_alignment(ptr);
}

template <typename Alloc>
struct AllocTraits {
   using has_allocate_all = False;
   using has_deallocate_all = False;
   using has_expand = False;
   using has_reallocate = False;
   using has_owns = False;
   using has_get_owner = False;
   using has_resolve_internal_pointer = False;
   using has_count = False;
   using has_allocated = False;
   using has_available = False;
   using has_size = False;

   static constexpr std::size_t alignment = alignof(double);
};

///////////////////////////////////////////////////////////////////////////////
template <typename Alloc>
class BaseAlloc {
protected:
   using base_type = BaseAlloc<Alloc>;
   using alloc_type = Alloc;
public:
   using traits = AllocTraits<Alloc>;

   static constexpr typename traits::has_allocate_all has_allocate_all = typename traits::has_allocate_all();
   static constexpr typename traits::has_deallocate_all has_deallocate_all = typename traits::has_deallocate_all();
   static constexpr typename traits::has_expand has_expand = typename traits::has_expand();
   static constexpr typename traits::has_reallocate has_reallocate = typename traits::has_reallocate();
   static constexpr typename traits::has_owns has_owns = typename traits::has_owns();
   static constexpr typename traits::has_get_owner has_get_owner = typename traits::has_get_owner();
   static constexpr typename traits::has_resolve_internal_pointer has_resolve_internal_pointer = typename traits::has_resolve_internal_pointer();
   static constexpr typename traits::has_count has_count = typename traits::has_count();
   static constexpr typename traits::has_allocated has_allocated = typename traits::has_allocated();
   static constexpr typename traits::has_available has_available = typename traits::has_available();
   static constexpr typename traits::has_size has_size = typename traits::has_size();

   static constexpr std::size_t alignment = traits::alignment;
   static_assert(AssertAlignment<alignment>::value, "Alignment must be >= 1");

   static constexpr std::size_t preferred_size(std::size_t size) {
      return expand_to_alignment<alignment>(size);
   }

   void* allocate(std::size_t size)  {
      return static_cast<Alloc*>(this)->allocate(size, Alloc::alignment);
   }

//   void* allocate(std::size_t size, std::size_t alignment) = 0;

   void* allocate_all() {
      assert_if_not(has_allocate_all);
      return nullptr;
   }

   bool expand(void* ptr, std::size_t old_size, std::size_t delta_size) {
      assert_if_not(has_expand);
      BE_IGNORE3(ptr, old_size, delta_size);
      return false;
   }

   void* reallocate(void* ptr, std::size_t old_size, std::size_t new_size) {
      return static_cast<Alloc*>(this)->reallocate(ptr, old_size, new_size, Alloc::alignment);
   }

   void* reallocate(void* ptr, std::size_t old_size, std::size_t new_size, std::size_t alignment) {
      assert_if_not(has_reallocate);
      if (new_size > old_size) {
         void* new_ptr = static_cast<Alloc*>(this)->allocate(new_size, alignment);
         if (ptr && new_ptr) {
            memcpy(new_ptr, ptr, min(old_size, new_size));
            static_cast<Alloc*>(this)->deallocate(ptr, old_size, alignment);
         }
         ptr = new_ptr;
      }
      return ptr;
   }

   bool deallocate(void* ptr, std::size_t size) {
      return static_cast<Alloc*>(this)->deallocate(ptr, size, Alloc::alignment);
   }

//   bool deallocate(void* ptr, std::size_t size, unsigned alignment) = 0;

   bool deallocate_all() {
      assert_if_not(has_deallocate_all);
      return false;
   }

   // determines if this allocator controls the specified block.  Not guaranteed to be accurate unless resolve_internal_pointer(ptr) == ptr
   bool owns(void* ptr, std::size_t size) const {
      assert_if_not(has_owns);
      BE_IGNORE2(ptr, size);
      return true;
   }

//  static SomeAlloc& getOwner(void* ptr, std::size_t size) const;

   // retrieves the beginning of the block containing an arbitrary pointer.  Given any arbitrary address, owns(resolve_internal_pointer(ptr)) == true iff it points to some part of a block managed by this allocator
   void* resolve_internal_pointer(void* ptr) {
      assert_if_not(has_resolve_internal_pointer);
      BE_IGNORE(ptr);
      return nullptr;
   }

   // returns the number calls to allocate whose resulting buffers have not yet been deallocated.
   std::size_t count() const {
      assert_if_not(has_count);
      return 0;
   }

   // returns the total number of bytes allocated but not released for reuse.  If size() and available()
   // exist, allocated() should be equivalent to size() - available().  This may include book-keeping information.
   std::size_t allocated() const {
      assert_if_not(has_allocated);
      return 0;
   }

   // returns the expected number of bytes that this allocator will be able to provide.  This should not include book-keeping information
   std::size_t available() const {
      assert_if_not(has_available);
      return std::numeric_limits<std::size_t>::max();
   }

   // returns the total number of bytes managed by this allocator.  This may include book-keeping information.
   std::size_t size() const {
      assert_if_not(has_size);
      return std::numeric_limits<std::size_t>::max();
   }

};

} // be::a3
} // be

#endif
