#ifdef BE_TEST_PERF

#include <catch/catch.hpp>
#include "version.hpp"
#include <be/testing/version.hpp>
#include <be/perf/version.hpp>

TEST_CASE("Version", "[version]") {
   REQUIRE(BE_CORE_VERSION == BE_CORE_VERSION);
   REQUIRE(BE_TESTING_VERSION == BE_TESTING_VERSION);
   REQUIRE(BE_PERF_VERSION == BE_PERF_VERSION);
}

#endif
