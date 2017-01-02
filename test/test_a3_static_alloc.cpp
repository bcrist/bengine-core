#ifdef BE_TEST

#include "a3_static_alloc.hpp"
#include <catch.hpp>

#define BE_CATCH_TAGS "[core][core:a3][core:a3:StaticAlloc]"

using namespace be::a3;

TEST_CASE("be::a3::StaticAlloc<100, 1>", BE_CATCH_TAGS) {
   constexpr std::size_t size = 100;
   StaticAlloc<size, 1> alloc;

   REQUIRE(alloc.preferred_size(0) == size);
   REQUIRE(alloc.preferred_size(1) == size);
   REQUIRE(alloc.preferred_size(55) == size);
   REQUIRE(alloc.preferred_size(100) == size);

   REQUIRE(alloc.size() == size);

   SECTION("allocate_all()") {
      void* ptr = alloc.allocate_all();
      REQUIRE(ptr != nullptr);
      memset(ptr, 0xF0, size);
      REQUIRE(alloc.allocate_all() == nullptr);
      REQUIRE(alloc.allocate(0) == nullptr);
      REQUIRE(alloc.size() == size);

      REQUIRE_FALSE(alloc.owns(nullptr, size));
      REQUIRE(alloc.owns(ptr, size));
      void* ptr5 = reinterpret_cast<char*>(ptr) + 5;
      REQUIRE(alloc.owns(ptr5, 1));
      REQUIRE(alloc.resolve_internal_pointer(ptr5) == ptr);

      REQUIRE(&(StaticAlloc<size, 1>::get_owner(ptr, 1)) == &alloc);

      REQUIRE(alloc.deallocate_all());
      REQUIRE(alloc.size() == size);
   }

   SECTION("allocate") {
      void* ptr = alloc.allocate(1);
      REQUIRE(ptr != nullptr);
      memset(ptr, 0xF0, 1);
      REQUIRE(alloc.allocate_all() == nullptr);
      REQUIRE(alloc.allocate(0) == nullptr);

      SECTION("expand") {
         REQUIRE(alloc.expand(ptr, 1, 1));
         REQUIRE(alloc.expand(ptr, 2, size - 2));
         memset(ptr, 0xF0, size);
         REQUIRE_FALSE(alloc.expand(ptr, size, 1));
         REQUIRE(alloc.deallocate(ptr, size));
      }

      SECTION("reallocate") {
         REQUIRE(alloc.reallocate(ptr, 1, 2) == ptr);
         REQUIRE(alloc.reallocate(ptr, 2, size) == ptr);
         memset(ptr, 0xF0, size);
         REQUIRE(alloc.reallocate(ptr, size, size * 2) == nullptr);
         REQUIRE(alloc.deallocate(ptr, size));
      }
   }

   REQUIRE(alloc.size() == size);
}

TEST_CASE("be::a3::StaticAlloc<10000, 32>", BE_CATCH_TAGS) {
   constexpr std::size_t size = 10000;
   constexpr std::size_t alignment = 32;
   StaticAlloc<size, alignment> alloc;

   REQUIRE(alloc.preferred_size(0) == size);
   REQUIRE(alloc.preferred_size(1) == size);
   REQUIRE(alloc.preferred_size(55) == size);
   REQUIRE(alloc.preferred_size(100) == size);

   REQUIRE(alloc.size() == size);

   SECTION("allocate_all()") {
      void* ptr = alloc.allocate_all();
      REQUIRE(ptr != nullptr);
      REQUIRE(reinterpret_cast<uintptr_t>(ptr) % alignment == 0);
      memset(ptr, 0xF0, size);
      REQUIRE(alloc.allocate_all() == nullptr);
      REQUIRE(alloc.allocate(0) == nullptr);
      REQUIRE(alloc.size() == size);

      REQUIRE_FALSE(alloc.owns(nullptr, size));
      REQUIRE(alloc.owns(ptr, size));
      void* ptr5 = reinterpret_cast<char*>(ptr) + 5;
      REQUIRE(alloc.owns(ptr5, 1));
      REQUIRE(alloc.resolve_internal_pointer(ptr5) == ptr);

      REQUIRE(&(StaticAlloc<size, alignment>::get_owner(ptr, 1)) == &alloc);

      REQUIRE(alloc.deallocate_all());
      REQUIRE(alloc.size() == size);
   }

   SECTION("allocate") {
      void* ptr = alloc.allocate(1);
      REQUIRE(ptr != nullptr);
      REQUIRE(reinterpret_cast<uintptr_t>(ptr) % alignment == 0);
      memset(ptr, 0xF0, 1);
      REQUIRE(alloc.allocate_all() == nullptr);
      REQUIRE(alloc.allocate(0) == nullptr);

      SECTION("expand") {
         REQUIRE(alloc.expand(ptr, 1, 1));
         REQUIRE(alloc.expand(ptr, 2, size - 2));
         memset(ptr, 0xF0, size);
         REQUIRE_FALSE(alloc.expand(ptr, size, 1));
         REQUIRE(alloc.deallocate(ptr, size));
      }

      SECTION("reallocate") {
         REQUIRE(alloc.reallocate(ptr, 1, 2) == ptr);
         REQUIRE(alloc.reallocate(ptr, 2, size) == ptr);
         memset(ptr, 0xF0, size);
         REQUIRE(alloc.reallocate(ptr, size, size * 2) == nullptr);
         REQUIRE(alloc.deallocate(ptr, size));
      }
   }

   REQUIRE(alloc.size() == size);
}

#endif
