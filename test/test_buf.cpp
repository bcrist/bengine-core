#ifdef BE_TEST

#include "buf.hpp"
#include <catch/catch.hpp>

#define BE_CATCH_TAGS "[core][core:buf]"

using namespace be;

TEST_CASE("be::Buf empty buffer", BE_CATCH_TAGS) {
   SECTION("default constructed") {
      Buf<UC> buf;
      REQUIRE_FALSE(buf.is_owner());
      REQUIRE(buf.size() == 0);
      REQUIRE_FALSE(buf);
      REQUIRE((UC*)buf == nullptr);
      REQUIRE((const UC*)buf == nullptr);
      REQUIRE(buf.get() == nullptr);
   }

   SECTION("nullptr with nonzero size") {
      Buf<UC> buf(nullptr, 1);
      REQUIRE_FALSE(buf.is_owner());
      REQUIRE(buf.size() == 0);
      REQUIRE_FALSE(buf);
      REQUIRE((UC*)buf == nullptr);
      REQUIRE((const UC*)buf == nullptr);
      REQUIRE(buf.get() == nullptr);
   }
   
   SECTION("nonnull pointer with zero size") {
      UC x;
      Buf<UC> buf(&x, 0);
      REQUIRE_FALSE(buf.is_owner());
      REQUIRE(buf.size() == 0);
      REQUIRE_FALSE(buf);
      REQUIRE((UC*)buf == &x);
      REQUIRE((const UC*)buf == &x);
      REQUIRE(buf.get() == &x);
   }

   SECTION("make_buf(0)") {
      Buf<UC> buf = make_buf<UC>(0);
      REQUIRE_FALSE(buf.is_owner());
      REQUIRE(buf.size() == 0);
      REQUIRE_FALSE(buf);
      REQUIRE((UC*)buf == nullptr);
      REQUIRE((const UC*)buf == nullptr);
      REQUIRE(buf.get() == nullptr);
   }
}

TEST_CASE("be::Buf make_buf(size_t)", BE_CATCH_TAGS) {
   SECTION("int") {
      Buf<int> buf = make_buf<int>(5);
      REQUIRE(buf.is_owner());
      REQUIRE(buf.size() == 5);
      REQUIRE(buf);
      REQUIRE(buf.get() != nullptr);
      REQUIRE((int*)buf == buf.get());
      REQUIRE((int*)buf != nullptr);
      REQUIRE((const int*)buf == buf.get());
      REQUIRE((const int*)buf != nullptr);

      buf[0] = 1337;
      buf[1] = 9001;
      REQUIRE(&(buf[0]) == buf.get());
      REQUIRE(*buf == 1337);
      REQUIRE(*(buf.get()) == 1337);
      REQUIRE(buf[0] == 1337);
      REQUIRE(buf[1] == 9001);
      REQUIRE(*(buf.get() + 1) == 9001);
   }

   SECTION("struct, operator->") {
      struct test {
         int a, b;
      };

      Buf<test> buf = make_buf<test>(2);
      REQUIRE(buf.is_owner());
      REQUIRE(buf.size() == 2);
      REQUIRE(buf);
      REQUIRE(buf.get() != nullptr);

      buf->a = 7;
      buf->b = 13;
      REQUIRE(buf[0].a == 7);
      REQUIRE(buf[0].b == 13);
      buf[1].a = 256;
      buf[1].b = 1024;
      REQUIRE(buf[1].a == 256);
      REQUIRE(buf[1].b == 1024);
      buf[1] = *buf;
      REQUIRE(buf[1].a == 7);
      REQUIRE(buf[1].b == 13);
   }
}

TEST_CASE("be::Buf conversion", BE_CATCH_TAGS) {
   Buf<int> buf = make_buf<int>(2);
   REQUIRE(buf.is_owner());
   REQUIRE(buf.size() == 2);
   REQUIRE(buf);
   REQUIRE(buf.get() != nullptr);

   void* ptr = buf.get();

   Buf<const int> buf2 = std::move(buf);
   REQUIRE(buf2.is_owner());
   REQUIRE(buf2.size() == 2);
   REQUIRE(buf2);
   REQUIRE(buf2.get() == ptr);
   REQUIRE_FALSE(buf.is_owner());

   Buf<const char> buf3 = std::move(buf2);
   REQUIRE(buf3.is_owner());
   REQUIRE(buf3.size() == 2 * sizeof(int));
   REQUIRE(buf3);
   REQUIRE(buf3.get() == ptr);
   REQUIRE_FALSE(buf2.is_owner());

   Buf<const UC> buf4 = std::move(buf3);
   REQUIRE(buf4.is_owner());
   REQUIRE(buf4.size() == 2 * sizeof(int));
   REQUIRE(buf4);
   REQUIRE(buf4.get() == ptr);
   REQUIRE_FALSE(buf3.is_owner());
}

namespace {
int deletion_count;
}

TEST_CASE("be::Buf deleter", BE_CATCH_TAGS) {
   deletion_count = 0;
   REQUIRE(deletion_count == 0);
   SECTION("destruction") {
      {
         Buf<const int> buf(new int[5], 5, [](void* ptr, std::size_t size) {
            ++deletion_count;
            delete[] ptr;
         });
         REQUIRE(buf.is_owner());
         REQUIRE(deletion_count == 0);

         Buf<const int> buf2 = std::move(buf);
         REQUIRE(buf2.is_owner());
         REQUIRE(deletion_count == 0);
      }
      
      REQUIRE(deletion_count == 1);
   }
   
   SECTION("release") {
      int* ptr = new int[5];
      {
         Buf<const int> buf(ptr, 5, [](void* ptr, std::size_t size) {
            ++deletion_count;
            delete[] ptr;
         });
         REQUIRE(buf.is_owner());
         REQUIRE(deletion_count == 0);

         buf.release();

         REQUIRE_FALSE(buf.is_owner());
         REQUIRE(deletion_count == 0);
         REQUIRE(buf.get() == ptr);
         REQUIRE(buf.size() == 5);
      }
      
      REQUIRE(deletion_count == 0);
      delete[] ptr;
   }
}

TEST_CASE("be::Buf swap", BE_CATCH_TAGS) {
   Buf<int> a = make_buf<int>(5);
   Buf<int> b = make_buf<int>(1);

   REQUIRE(a.size() == 5);
   REQUIRE(b.size() == 1);

   int* pa = a.get();
   int* pb = b.get();

   using std::swap;
   swap(a, b);

   REQUIRE(a.size() == 1);
   REQUIRE(b.size() == 5);

   REQUIRE(a.get() == pb);
   REQUIRE(b.get() == pa);

   // TODO test deleter swapping
}

TEST_CASE("be::Buf make_buf(T*, size_t)", BE_CATCH_TAGS) {
   SECTION("nullptr deleter") {
      int arr[5];
      Buf<int> buf = make_buf<int>(arr, 5);

      REQUIRE_FALSE(buf.is_owner());
      REQUIRE(buf.get() == arr);
      REQUIRE(buf.size() == 5);

      memset(arr, 0x5A, sizeof(arr));
      for (std::size_t i = 0; i < buf.size(); ++i) {
         REQUIRE(arr[i] == buf[i]);
         REQUIRE(&(arr[i]) == &(buf[i]));
      }
   }

   SECTION("detail::delete_array") {
      Buf<int> buf = make_buf<int>(new int[5], 5, detail::delete_array);
      REQUIRE(buf.is_owner());
      REQUIRE(buf.get() != nullptr);
      REQUIRE(buf.size() == 5);
   }
}

TEST_CASE("be::Buf tmp_buf(T*, size_t)", BE_CATCH_TAGS) {
   int arr[5];
   Buf<int> buf = tmp_buf<int>(arr, 5);

   REQUIRE_FALSE(buf.is_owner());
   REQUIRE(buf.get() == arr);
   REQUIRE(buf.size() == 5);

   memset(arr, 0x5A, sizeof(arr));
   for (std::size_t i = 0; i < buf.size(); ++i) {
      REQUIRE(arr[i] == buf[i]);
      REQUIRE(&(arr[i]) == &(buf[i]));
   }
}

TEST_CASE("be::Buf tmp_buf(S)", BE_CATCH_TAGS) {
   SECTION("non const") {
      S str = "asdf";
      Buf<char> buf = tmp_buf(str);
      REQUIRE_FALSE(buf.is_owner());
      REQUIRE(buf.size() == 4);
      buf[0] = 'b';
      REQUIRE(str == "bsdf");
   }

   SECTION("const") {
      REQUIRE_FALSE(tmp_buf(S("asdf")).is_owner());
      REQUIRE(tmp_buf(S("asdf")).size() == 4);
   }
}

TEST_CASE("be::Buf tmp_buf(vector)", BE_CATCH_TAGS) {
   SECTION("non const") {
      std::vector<int> v { 0, 0 };
      Buf<int> buf = tmp_buf(v);
      REQUIRE_FALSE(buf.is_owner());
      REQUIRE(buf.size() == 2);
      buf[0] = 1;
      REQUIRE(v[0] == 1);
   }

   SECTION("const") {
      std::vector<int> v { 0, 0 };
      const std::vector<int>& cv = v;
      Buf<const int> buf = tmp_buf(cv);
      REQUIRE_FALSE(buf.is_owner());
      REQUIRE(buf.size() == 2);
   }
}

TEST_CASE("be::Buf tmp_buf(array)", BE_CATCH_TAGS) {
   SECTION("non const") {
      std::array<int, 2> v { 0, 0 };
      Buf<int> buf = tmp_buf(v);
      REQUIRE_FALSE(buf.is_owner());
      REQUIRE(buf.size() == 2);
      buf[0] = 1;
      REQUIRE(v[0] == 1);
   }

   SECTION("const") {
      std::array<int, 2> v { 0, 0 };
      const std::array<int, 2>& cv = v;
      Buf<const int> buf = tmp_buf(cv);
      REQUIRE_FALSE(buf.is_owner());
      REQUIRE(buf.size() == 2);
   }
}

TEST_CASE("be::Buf sub_buf", BE_CATCH_TAGS) {
   int arr[5] = { };
   Buf<int> buf = tmp_buf(arr);
   REQUIRE_FALSE(buf.is_owner());
   REQUIRE(buf.size() == 5);

   SECTION("suffix") {
      Buf<int> sb = sub_buf(buf, 2);
      REQUIRE_FALSE(sb.is_owner());
      REQUIRE(sb.size() == 3);

      *sb = 1337;

      REQUIRE(buf[2] == 1337);
      REQUIRE(&(buf[3]) == &(sb[1]));
   }

   SECTION("interval") {
      Buf<int> sb = sub_buf(buf, 2, 2);
      REQUIRE_FALSE(sb.is_owner());
      REQUIRE(sb.size() == 2);

      *sb = 1337;

      REQUIRE(buf[2] == 1337);
      REQUIRE(&(buf[3]) == &(sb[1]));
   }

   SECTION("out of range interval bounds") {
      REQUIRE_FALSE(sub_buf(buf, 5));
      REQUIRE_FALSE(sub_buf(buf, 100));
      REQUIRE_FALSE(sub_buf(buf, 100, 4));
      REQUIRE(sub_buf(buf, 3, 100).size() == 2);
   }
}

TEST_CASE("be::Buf copy_buf", BE_CATCH_TAGS) {
   int arr[5] = { };
   Buf<I32> buf = tmp_buf(arr);
   REQUIRE_FALSE(buf.is_owner());
   REQUIRE(buf.size() == 5);

   SECTION("same type") {
      Buf<I32> sb = copy_buf(buf);
      REQUIRE(sb.is_owner());
      REQUIRE(sb.size() == 5);
      REQUIRE(buf.get() != sb.get());
   }

   SECTION("to char") {
      Buf<char> sb = copy_buf(buf);
      REQUIRE(sb.is_owner());
      REQUIRE(sb.size() == 5 * sizeof(I32));
      REQUIRE(static_cast<void*>(buf.get()) != sb.get());
      REQUIRE(sb[0] == 0);
   }

   SECTION("different types") {
      Buf<F32> sb = copy_buf<F32>(buf);
      REQUIRE(sb.is_owner());
      REQUIRE(sb.size() == 5);
      REQUIRE(sb[0] == 0.f);
      REQUIRE(static_cast<void*>(buf.get()) != sb.get());
   }

   SECTION("empty") {
      Buf<int> sb = copy_buf(Buf<int>());
      REQUIRE_FALSE(sb.is_owner());
      REQUIRE(sb.size() == 0);
      REQUIRE(sb.get() == nullptr);
   }
}

TEST_CASE("buf_to_string", BE_CATCH_TAGS) {
   REQUIRE("asdf" == buf_to_string(tmp_buf(S("asdf"))));
}

TEST_CASE("buf_to_vector", BE_CATCH_TAGS) {
   std::vector<int> vec;
   vec.push_back(1);
   vec.push_back(7);
   vec.push_back(12);
   REQUIRE(vec == buf_to_vector<int>(tmp_buf(vec)));
}

#endif
