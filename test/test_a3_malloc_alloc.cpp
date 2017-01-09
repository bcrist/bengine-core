#ifdef BE_TEST

#include "a3_malloc_alloc.hpp"
#include <catch/catch.hpp>

#define BE_CATCH_TAGS "[core][core:a3][core:a3:MallocAlloc]"

using namespace be::a3;

TEST_CASE("be::a3::MallocAlloc", BE_CATCH_TAGS) {
   MallocAlloc alloc;
   std::size_t sizes[] = { 1, 2, 3, 8, 11, 1337, 4096 };

   for (std::size_t size : sizes) {
      void* ptr = alloc.allocate(size);

      REQUIRE(ptr != nullptr);
      REQUIRE(reinterpret_cast<uintptr_t>(ptr) % MallocAlloc::alignment == 0);

      memset(ptr, 0xF0, size);

      void* ptr2 = alloc.reallocate(ptr, size, size + 1);
      REQUIRE(ptr2 != nullptr);
      memset(ptr2, 0xF0, size + 1);

      REQUIRE(alloc.deallocate(ptr2, size + 1));

      REQUIRE(alloc.deallocate(nullptr, 0));
      REQUIRE(alloc.deallocate(nullptr, size));
   }
}

#endif
