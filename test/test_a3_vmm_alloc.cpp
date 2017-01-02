#ifdef BE_TEST

#include "a3_vmm_alloc.hpp"
#include <catch.hpp>

#define BE_CATCH_TAGS "[core][core:a3][core:a3:VmmAlloc]"

using namespace be::a3;

TEST_CASE("be::a3::VmmAlloc", BE_CATCH_TAGS) {
   REQUIRE(VmmAlloc::granularity >= VmmAlloc::alignment);
   REQUIRE(VmmAlloc::granularity % VmmAlloc::alignment == 0);

   VmmAlloc alloc;
   std::size_t sizes[] = { 1, 2, 3, 8, 11, 1337, 4096 };

   for (std::size_t size : sizes) {
      REQUIRE(alloc.preferred_size(size) == VmmAlloc::alignment);

      void* ptr = alloc.allocate(size);

      REQUIRE(ptr != nullptr);
      REQUIRE(reinterpret_cast<uintptr_t>(ptr) % VmmAlloc::granularity == 0);

      memset(ptr, 0xF0, size);

      SECTION("expand") {
         REQUIRE_FALSE(alloc.expand(nullptr, 0, size));
         REQUIRE_FALSE(alloc.expand(nullptr, size, size));
         REQUIRE_FALSE(alloc.expand(nullptr, size, size + 1));
         REQUIRE_FALSE(alloc.expand(nullptr, size, size * 2));

         REQUIRE(alloc.expand(ptr, size, 0));
         REQUIRE(alloc.expand(ptr, size, 1));
         memset(ptr, 0xF0, size + 1);

         REQUIRE(alloc.expand(ptr, size + 1, size));
         memset(ptr, 0xF0, size * 2 + 1);

         REQUIRE_FALSE(alloc.expand(ptr, size * 2 + 1, VmmAlloc::granularity));

         REQUIRE(alloc.deallocate(ptr, size * 2 + 1));
      }

      SECTION("reallocate") {
         REQUIRE(alloc.reallocate(ptr, size, size) == ptr);
         REQUIRE(alloc.reallocate(ptr, size, size + 1) == ptr);
         memset(ptr, 0xF0, size + 1);

         REQUIRE(alloc.reallocate(ptr, size + 1, size * 2) == ptr);
         memset(ptr, 0xF0, size * 2);

         void* ptr2 = alloc.reallocate(ptr, size * 2, size + VmmAlloc::granularity);

         CHECK(ptr != ptr2);
         REQUIRE(ptr2 != nullptr);

         REQUIRE(alloc.deallocate(ptr2, size + VmmAlloc::granularity));
      }

      REQUIRE(alloc.deallocate(nullptr, 0));
      REQUIRE(alloc.deallocate(nullptr, size));
   }
}

#endif
