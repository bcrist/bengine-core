#ifdef BE_TEST

#include "stack_trace.hpp"
#include "stack_trace_exception.hpp"
#include <be/core/filesystem.hpp>
#include <be/core/logging.hpp>
#include <catch.hpp>
#include <iostream>

#define BE_CATCH_TAGS "[core][core:stacktrace]"

using namespace be;

TEST_CASE("be::StackTraceException", BE_CATCH_TAGS) {
   try {
      throw StackTraceException<std::runtime_error>("asdf!");
   } catch (const StackTraceException<std::runtime_error>&) { }

   try {
      throw StackTraceException<std::runtime_error>("asdf!");
   } catch (const std::runtime_error&) { }
}

#endif
