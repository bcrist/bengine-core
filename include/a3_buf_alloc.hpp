#pragma once
#ifndef BE_CORE_A3_BUF_ALLOC_HPP_
#define BE_CORE_A3_BUF_ALLOC_HPP_

#include "a3_base_alloc.hpp"
#include "buf.hpp"

namespace be {
namespace a3 {

template <std::size_t Alignment>
class BufAlloc;

template <std::size_t Alignment>
struct AllocTraits<BufAlloc<Alignment>> : AllocTraits<void> {
   using has_allocate_all = True;
   using has_deallocate_all = True;
   using has_expand = True;
   using has_owns = True;
   using has_resolve_internal_pointer = True;
   using has_size = True;

   static constexpr std::size_t alignment = Alignment;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Basic allocator that always returns a pointer to the beginning of
///         a buffer.
///
/// \details Allocations will be check to ensure that only
///         one call to allocate() is made until deallocate() is called.
template <std::size_t Alignment>
class BufAlloc : public BaseAlloc<BufAlloc<Alignment>>, Movable {
   friend void swap(BufAlloc<Alignment>& a, BufAlloc<Alignment>& b) {
      if (consumed_) {
         throw std::logic_error("Can't replace buffer while in use!");
      }

      if (other.consumed_) {
         throw std::logic_error("Can't move from a BufAlloc in use!");
      }

      using std::swap;
      swap(a.buf_, b.buf_);
   }

public:
   BufAlloc(void* buf, size_t size)
      : buf_(static_cast<char*>(buf), size) { }

   template <typename T>
   explicit BufAlloc(Buf<T, true>&& other)
      : buf_(std::move(other)) {
      assert(expand_to_alignment<Alignment>(buf_.get()) == buf_.get());
   }

   template <std::size_t OtherAlignment>
   BufAlloc(BufAlloc<OtherAlignment>&& other) {
      static_assert(OtherAlignment >= Alignment, "Cannot move from an allocator with a smaller alignment to a larger alignment");

      if (other.consumed_) {
         throw std::logic_error("Can't move from a BufAlloc in use!");
      }

      buf_ = std::move(other.buf_);
   }

   template <std::size_t OtherAlignment>
   BufAlloc<Alignment>& operator=(BufAlloc<OtherAlignment>&& other) {
      static_assert(OtherAlignment >= Alignment, "Cannot move from an allocator with a smaller alignment to a larger alignment");

      if (consumed_) {
         throw std::logic_error("Can't replace buffer while in use!");
      }

      if (other.consumed_) {
         throw std::logic_error("Can't move from a BufAlloc in use!");
      }

      buf_ = std::move(other.buf_);
   }

   template <typename T>
   void set_buf(Buf<T, true>&& other) {
      if (consumed_) {
         throw std::logic_error("Can't replace buffer while in use!");
      }

      assert(expand_to_alignment<Alignment>(buf_.get()) == buf_.get());

      buf_ = std::move(other);
   }

   using base_type::allocate;
   void* allocate(std::size_t size, std::size_t alignment) {
      BE_IGNORE2(size, alignment);
      assert(alignment <= Alignment);
      assert(size <= buf_.size());
      return allocate_all();
   }

   void* allocate_all() {
      if (consumed_) {
         return nullptr;
      }
      consumed_ = true;
      return buf_.get();
   }

   bool expand(void* ptr, std::size_t old_size, std::size_t delta_size) {
      assert(ptr == buf_.get());
      assert(consumed_);
      return old_size + delta_size <= buf_.size();
   }

   using base_type::reallocate;
   void* reallocate(void* ptr, std::size_t old_size, std::size_t new_size, std::size_t alignment) {
      assert(ptr == buf_.get());
      assert(consumed_);
      if (new_size <= buf_.size()) {
         return ptr;
      }

      return nullptr;
   }

   using base_type::deallocate;
   bool deallocate(void* ptr, std::size_t size, std::size_t alignment) {
      BE_IGNORE3(ptr, size, alignment);
      assert(ptr == buf_.get());
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
      std::ptrdiff_t diff = cptr - buf_.get();
      return diff >= 0 && diff < static_cast<std::ptrdiff_t>(buf_.size());
   }

   void* resolve_internal_pointer(void* ptr) const {
      if (owns(ptr, 0)) {
         return buf_.get();
      } else {
         return nullptr;
      }
   }

   std::size_t size() const {
      return buf_.size();
   }

private:
   Buf<char> buf_;
   bool consumed_ = false;
};

} // be::a3
} // be

#endif
