#ifdef BE_TEST

#include "inline_executors.hpp"
#include <catch/catch.hpp>

#define BE_CATCH_TAGS "[core][core:executor][core:executor:inline]"

using namespace be;

TEST_CASE("be::InlineExecutor<be::SimpleWorkQueue>", BE_CATCH_TAGS) {
   InlineExecutor<SimpleWorkQueue> executor;

   REQUIRE(executor.empty());
   REQUIRE(executor.size() == 0);

   int a = 0;

   executor.post([&]() {
      a = 1;
   });

   REQUIRE(!executor.empty());
   REQUIRE(executor.size() == 1);
   REQUIRE(a == 0);

   executor.post([&]() {
      a = 2;
   });

   REQUIRE(!executor.empty());
   REQUIRE(executor.size() == 2);
   REQUIRE(a == 0);

   SECTION("run_one()") {
      REQUIRE(executor.run_one());

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 1);
      REQUIRE(a == 1);

      executor.post([&]() {
         a = 13;
      });

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 2);
      REQUIRE(a == 1);

      REQUIRE(executor.run_one());

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 1);
      REQUIRE(a == 2);

      REQUIRE(executor.run_one());

      REQUIRE(executor.empty());
      REQUIRE(executor.size() == 0);
      REQUIRE(a == 13);

      REQUIRE(!executor.run_one());
   }
   
   SECTION("run()") {
      REQUIRE(executor.run(1));

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 1);
      REQUIRE(a == 1);

      executor.post([&]() {
         a = 13;
      });

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 2);
      REQUIRE(a == 1);

      REQUIRE(executor.run());

      REQUIRE(executor.empty());
      REQUIRE(executor.size() == 0);
      REQUIRE(a == 13);

      REQUIRE(!executor.run());
   }

   SECTION("Move construction") {
      InlineExecutor<SimpleWorkQueue> exec = std::move(executor);

      REQUIRE(!exec.empty());
      REQUIRE(exec.size() == 2);
      REQUIRE(a == 0);

      REQUIRE(exec.run());

      REQUIRE(exec.empty());
      REQUIRE(exec.size() == 0);
      REQUIRE(a == 2);

      SECTION("Move assignment") {
         exec.post([&]() {
            a = 1;
         });

         REQUIRE(!exec.empty());
         REQUIRE(exec.size() == 1);

         executor = std::move(exec);

         REQUIRE(!executor.empty());
         REQUIRE(executor.size() == 1);
         
         REQUIRE(executor.run());

         REQUIRE(executor.empty());
         REQUIRE(executor.size() == 0);
         REQUIRE(a == 1);
      }
   }
}

TEST_CASE("be::SimpleInlineExecutor", BE_CATCH_TAGS) {
   SimpleInlineExecutor executor;

   REQUIRE(executor.empty());
   REQUIRE(executor.size() == 0);

   int a = 0;

   executor.post([&]() {
      a = 1;
   });

   REQUIRE(!executor.empty());
   REQUIRE(executor.size() == 1);
   REQUIRE(a == 0);

   executor.post([&]() {
      a = 2;
   });

   REQUIRE(!executor.empty());
   REQUIRE(executor.size() == 2);
   REQUIRE(a == 0);

   SECTION("run_one()") {
      REQUIRE(executor.run_one());

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 1);
      REQUIRE(a == 1);

      executor.post([&]() {
         a = 13;
      });

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 2);
      REQUIRE(a == 1);

      REQUIRE(executor.run_one());

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 1);
      REQUIRE(a == 2);

      REQUIRE(executor.run_one());

      REQUIRE(executor.empty());
      REQUIRE(executor.size() == 0);
      REQUIRE(a == 13);

      REQUIRE(!executor.run_one());
   }

   SECTION("run()") {
      REQUIRE(executor.run(1));

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 1);
      REQUIRE(a == 1);

      executor.post([&]() {
         a = 13;
      });

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 2);
      REQUIRE(a == 1);

      REQUIRE(executor.run());

      REQUIRE(executor.empty());
      REQUIRE(executor.size() == 0);
      REQUIRE(a == 13);

      REQUIRE(!executor.run());
   }

   SECTION("Move construction") {
      SimpleInlineExecutor exec = std::move(executor);

      REQUIRE(!exec.empty());
      REQUIRE(exec.size() == 2);
      REQUIRE(a == 0);

      REQUIRE(exec.run());

      REQUIRE(exec.empty());
      REQUIRE(exec.size() == 0);
      REQUIRE(a == 2);

      SECTION("Move assignment") {
         exec.post([&]() {
            a = 1;
         });

         REQUIRE(!exec.empty());
         REQUIRE(exec.size() == 1);

         executor = std::move(exec);

         REQUIRE(!executor.empty());
         REQUIRE(executor.size() == 1);

         REQUIRE(executor.run());

         REQUIRE(executor.empty());
         REQUIRE(executor.size() == 0);
         REQUIRE(a == 1);
      }
   }
}

TEST_CASE("be::InlineExecutor<be::PriorityWorkQueue<>>", BE_CATCH_TAGS) {
   InlineExecutor<PriorityWorkQueue<>> executor;

   REQUIRE(executor.empty());
   REQUIRE(executor.size() == 0);

   int a = 0;

   executor.post([&]() {
      a = 1;
   }, 1);

   REQUIRE(!executor.empty());
   REQUIRE(executor.size() == 1);
   REQUIRE(a == 0);

   executor.post([&]() {
      a = 2;
   }, 2);

   REQUIRE(!executor.empty());
   REQUIRE(executor.size() == 2);
   REQUIRE(a == 0);

   SECTION("run_one()") {
      REQUIRE(executor.run_one());

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 1);
      REQUIRE(a == 2);

      executor.post([&]() {
         a = 13;
      });

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 2);
      REQUIRE(a == 2);

      REQUIRE(executor.run_one());

      REQUIRE(!executor.empty());
      REQUIRE(executor.size() == 1);
      REQUIRE(a == 1);

      REQUIRE(executor.run_one());

      REQUIRE(executor.empty());
      REQUIRE(executor.size() == 0);
      REQUIRE(a == 13);

      REQUIRE(!executor.run_one());
   }

   SECTION("Move construction") {
      InlineExecutor<PriorityWorkQueue<>> exec = std::move(executor);

      exec.default_priority(5);
      exec.post([&]() {
         a = 5;
      });

      REQUIRE(!exec.empty());
      REQUIRE(exec.size() == 3);
      REQUIRE(a == 0);

      REQUIRE(exec.run());

      REQUIRE(exec.empty());
      REQUIRE(exec.size() == 0);
      REQUIRE(a == 1);

      SECTION("Move assignment") {
         exec.post([&]() {
            a = 0;
         });

         REQUIRE(!exec.empty());
         REQUIRE(exec.size() == 1);

         executor = std::move(exec);

         REQUIRE(!executor.empty());
         REQUIRE(executor.size() == 1);

         REQUIRE(executor.run());

         REQUIRE(executor.empty());
         REQUIRE(executor.size() == 0);
         REQUIRE(a == 0);
      }
   }
}

#endif
