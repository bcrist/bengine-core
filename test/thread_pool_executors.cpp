#ifdef BE_TEST

#include "thread_pool_executors.hpp"
#include <catch.hpp>

#define BE_CATCH_TAGS "[core][core:executor][core:executor:thread-pool]"

using namespace be;

TEST_CASE("be::SimpleThreadPoolExecutor", BE_CATCH_TAGS) {
   SimpleThreadPoolExecutor executor;

   int a = 0;

   REQUIRE(!executor);
   REQUIRE(executor.threads() == 0);
   REQUIRE(executor.active_threads() == 0);
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
      executor.threads(1);

      REQUIRE(executor);
      REQUIRE(executor.threads() == 1);
      REQUIRE(executor.active_threads() == 1);

      executor.shutdown();

      REQUIRE(!executor);
      REQUIRE(executor.threads() == 0);
      REQUIRE(executor.active_threads() == 0);
      REQUIRE(executor.empty());
      REQUIRE(executor.size() == 0);
      REQUIRE(a == 2);

      SECTION("Restarting") {
         executor.post([&]() { ++a; });
         REQUIRE(!executor.empty());
         REQUIRE(executor.size() == 1);
         REQUIRE(a == 2);

         executor.threads(2);

         REQUIRE(executor);
         REQUIRE(executor.threads() == 2);
         REQUIRE(executor.active_threads() == 2);

         executor.threads(0);

         REQUIRE(!executor);
         REQUIRE(executor.threads() == 0);
         REQUIRE(executor.active_threads() == 0);
         REQUIRE(executor.empty());
         REQUIRE(executor.size() == 0);
         REQUIRE(a == 3);
      }
   }
}

TEST_CASE("be::PriorityThreadPoolExecutor", BE_CATCH_TAGS) {
   PriorityThreadPoolExecutor<> executor;

   int a = 0;

   REQUIRE(!executor);
   REQUIRE(executor.threads() == 0);
   REQUIRE(executor.active_threads() == 0);
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
      executor.threads(1);

      REQUIRE(executor);
      REQUIRE(executor.threads() == 1);
      REQUIRE(executor.active_threads() == 1);

      executor.shutdown();

      REQUIRE(!executor);
      REQUIRE(executor.threads() == 0);
      REQUIRE(executor.active_threads() == 0);
      REQUIRE(executor.empty());
      REQUIRE(executor.size() == 0);
      REQUIRE(a == 2);

      SECTION("Restarting") {
         executor.post([&]() { ++a; });
         REQUIRE(!executor.empty());
         REQUIRE(executor.size() == 1);
         REQUIRE(a == 2);

         executor.threads(2);

         REQUIRE(executor);
         REQUIRE(executor.threads() == 2);
         REQUIRE(executor.active_threads() == 2);

         executor.threads(0);

         REQUIRE(!executor);
         REQUIRE(executor.threads() == 0);
         REQUIRE(executor.active_threads() == 0);
         REQUIRE(executor.empty());
         REQUIRE(executor.size() == 0);
         REQUIRE(a == 3);
      }
   }
}

#endif
