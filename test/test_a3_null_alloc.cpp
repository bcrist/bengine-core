#ifdef BE_TEST

#include "a3_null_alloc.hpp"
#include <catch.hpp>

#define BE_CATCH_TAGS "[core][core:a3][core:a3:NullAlloc]"

using namespace be::a3;

TEST_CASE("be::a3::NullAlloc", BE_CATCH_TAGS) {
   NullAlloc alloc;

   std::size_t sizes[] = { 0, 1, 2, 3, 8, 11, 1337 };

   for (std::size_t size : sizes) {
      REQUIRE(alloc.preferred_size(size) == size);

      REQUIRE(alloc.count() == 0);
      REQUIRE(alloc.allocated() == 0);
      REQUIRE(alloc.available() == 0);
      REQUIRE(alloc.size() == 0);

      REQUIRE(alloc.allocate_all() == nullptr);

      REQUIRE(alloc.count() == 0);
      REQUIRE(alloc.allocated() == 0);
      REQUIRE(alloc.available() == 0);
      REQUIRE(alloc.size() == 0);

      REQUIRE(alloc.deallocate_all());

      REQUIRE(alloc.allocate(size) == nullptr);
      REQUIRE(alloc.allocate(size, size) == nullptr);

      REQUIRE_FALSE(alloc.expand(nullptr, size, 1));
      REQUIRE_FALSE(alloc.expand(nullptr, size, size));

      REQUIRE(alloc.reallocate(nullptr, size, size) == nullptr);
      REQUIRE(alloc.reallocate(nullptr, size, size + 1) == nullptr);
      REQUIRE(alloc.reallocate(nullptr, size, size * 2) == nullptr);
      REQUIRE(alloc.reallocate(nullptr, size, size, size) == nullptr);
      REQUIRE(alloc.reallocate(nullptr, size, size + 1, size) == nullptr);
      REQUIRE(alloc.reallocate(nullptr, size, size * 2, size) == nullptr);

      REQUIRE(alloc.count() == 0);
      REQUIRE(alloc.allocated() == 0);
      REQUIRE(alloc.available() == 0);
      REQUIRE(alloc.size() == 0);

      REQUIRE(alloc.deallocate(nullptr, size));
      REQUIRE(alloc.deallocate(nullptr, size, size));

      REQUIRE_FALSE(alloc.owns(nullptr, size));
      REQUIRE_FALSE(alloc.owns(&sizes, size));

      REQUIRE(alloc.resolve_internal_pointer(nullptr) == nullptr);
      REQUIRE(alloc.resolve_internal_pointer(&sizes) == nullptr);
   }
}

#endif
