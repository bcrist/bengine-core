#ifdef BE_TEST

#include "single_thread_executors.hpp"
#include <catch/catch.hpp>

#define BE_CATCH_TAGS "[core][core:executor][core:executor:single-thread]"

using namespace be;

TEST_CASE("be::SimpleSingleThreadExecutor", BE_CATCH_TAGS) {
   SimpleSingleThreadExecutor executor;

   int a = 0;

   REQUIRE(executor);
   REQUIRE(!executor.running());
   REQUIRE(executor.empty());
   REQUIRE(executor.size() == 0);

   executor.post([&]() { ++a; });
   REQUIRE(!executor.empty());
   REQUIRE(executor.size() == 1);
   REQUIRE(a == 0);

   executor.post([&]() { ++a; });
   REQUIRE(!executor.empty());
   REQUIRE(executor.size() == 2);
   REQUIRE(a == 0);

   SECTION("Running") {
      executor.running(true);

      REQUIRE(executor);
      REQUIRE(executor.running());

      executor.running(false);

      REQUIRE(!executor);
      REQUIRE(!executor.running());
      REQUIRE(executor.empty());
      REQUIRE(executor.size() == 0);
      REQUIRE(a == 2);

      SECTION("Restarting") {
         executor.shutdown(false);
         REQUIRE(executor);
         REQUIRE(!executor.running());

         executor.post([&]() { ++a; });
         REQUIRE(!executor.empty());
         REQUIRE(executor.size() == 1);
         REQUIRE(a == 2);

         executor.running(true);

         REQUIRE(executor);
         REQUIRE(executor.running());

         executor.running(false);

         REQUIRE(!executor);
         REQUIRE(!executor.running());
         REQUIRE(executor.empty());
         REQUIRE(executor.size() == 0);
         REQUIRE(a == 3);
      }
   }
}

TEST_CASE("be::PrioritySingleThreadExecutor", BE_CATCH_TAGS) {
   PrioritySingleThreadExecutor<> executor;

   int a = 0;

   REQUIRE(executor);
   REQUIRE(!executor.running());
   REQUIRE(executor.empty());
   REQUIRE(executor.size() == 0);

   executor.post([&]() { ++a; });
   REQUIRE(!executor.empty());
   REQUIRE(executor.size() == 1);
   REQUIRE(a == 0);

   executor.post([&]() { ++a; });
   REQUIRE(!executor.empty());
   REQUIRE(executor.size() == 2);
   REQUIRE(a == 0);

   SECTION("Running") {
      executor.running(true);

      REQUIRE(executor);
      REQUIRE(executor.running());

      executor.running(false);

      REQUIRE(!executor);
      REQUIRE(!executor.running());
      REQUIRE(executor.empty());
      REQUIRE(executor.size() == 0);
      REQUIRE(a == 2);

      SECTION("Restarting") {
         executor.shutdown(false);
         REQUIRE(executor);
         REQUIRE(!executor.running());

         executor.post([&]() { ++a; });
         REQUIRE(!executor.empty());
         REQUIRE(executor.size() == 1);
         REQUIRE(a == 2);

         executor.running(true);

         REQUIRE(executor);
         REQUIRE(executor.running());

         executor.running(false);

         REQUIRE(!executor);
         REQUIRE(!executor.running());
         REQUIRE(executor.empty());
         REQUIRE(executor.size() == 0);
         REQUIRE(a == 3);
      }
   }
}

#endif
