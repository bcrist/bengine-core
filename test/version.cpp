#ifdef BE_TEST

#include <catch/catch.hpp>
#include "version.hpp"
#include <be/testing/version.hpp>

TEST_CASE("Version", "[version]") {
   REQUIRE(BE_CORE_VERSION == BE_CORE_VERSION);
   REQUIRE(BE_TESTING_VERSION == BE_TESTING_VERSION);
}

#endif
