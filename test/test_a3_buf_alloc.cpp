//#ifdef BE_TEST
//
//#include "a3_buf_alloc.hpp"
//#include <catch/catch.hpp>
//
//#define BE_CATCH_TAGS "[core][core:a3][core:a3:BufAlloc]"
//
//using namespace be;
//using namespace be::a3;
//
//TEST_CASE("be::a3::BufAlloc<8>", BE_CATCH_TAGS) {
//   constexpr std::size_t size = 100;
//   constexpr std::size_t alignment = 8;
//   BufAlloc<alignment> alloc(make_buf<char>(size));
//
//   REQUIRE(alloc.size() == size);
//
//   BufAlloc<alignment> other = std::move(alloc);
//   alloc = std::move(other);
//
//   REQUIRE(alloc.size() == size);
//
//   SECTION("allocate_all()") {
//      void* ptr = alloc.allocate_all();
//      REQUIRE(ptr != nullptr);
//      REQUIRE(reinterpret_cast<uintptr_t>(ptr) % alignment == 0);
//      memset(ptr, 0xF0, size);
//      REQUIRE(alloc.allocate_all() == nullptr);
//      REQUIRE(alloc.allocate(0) == nullptr);
//      REQUIRE(alloc.size() == size);
//
//      REQUIRE_FALSE(alloc.owns(nullptr, size));
//      REQUIRE(alloc.owns(ptr, size));
//      void* ptr5 = reinterpret_cast<char*>(ptr) + 5;
//      REQUIRE(alloc.owns(ptr5, 1));
//      REQUIRE(alloc.resolve_internal_pointer(ptr5) == ptr);
//
//      //REQUIRE(&(BufAlloc<alignment>::get_owner(ptr, 1)) == &alloc);
//
//      REQUIRE(alloc.deallocate_all());
//      REQUIRE(alloc.size() == size);
//   }
//
//   SECTION("allocate") {
//      void* ptr = alloc.allocate(1);
//      REQUIRE(ptr != nullptr);
//      REQUIRE(reinterpret_cast<uintptr_t>(ptr) % alignment == 0);
//      memset(ptr, 0xF0, 1);
//      REQUIRE(alloc.allocate_all() == nullptr);
//      REQUIRE(alloc.allocate(0) == nullptr);
//
//      SECTION("expand") {
//         REQUIRE(alloc.expand(ptr, 1, 1));
//         REQUIRE(alloc.expand(ptr, 2, size - 2));
//         memset(ptr, 0xF0, size);
//         REQUIRE_FALSE(alloc.expand(ptr, size, 1));
//         REQUIRE(alloc.deallocate(ptr, size));
//      }
//
//      SECTION("reallocate") {
//         REQUIRE(alloc.reallocate(ptr, 1, 2) == ptr);
//         REQUIRE(alloc.reallocate(ptr, 2, size) == ptr);
//         memset(ptr, 0xF0, size);
//         REQUIRE(alloc.reallocate(ptr, size, size * 2) == nullptr);
//         REQUIRE(alloc.deallocate(ptr, size));
//      }
//   }
//
//}
//
//#endif
