#ifdef BE_TEST_PERF

#include <catch/catch.hpp>
#include "ensure_init.hpp"
#include "blackhole_log.hpp"
#include "time.hpp"

#define BE_CATCH_TAGS "[core][core:logging]"

using namespace be;

TEST_CASE("Complex Log::handle performance", BE_CATCH_TAGS) {
   ensure_init();
   Log& log = blackhole_log();

   F64 avg = 0;

   for (int i = 0; i < 1000; ++i) {
      U64 begin = perf_now();

      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         || log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         || log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         || log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         || log;
      be_verbose() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         || log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         || log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         || log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         || log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         || log;
      be_verbose() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         || log;

      U64 end = perf_now();

      F64 elapsed = perf_delta_to_seconds(end - begin);
      avg += elapsed;
   }

   avg *= 100;

   be_notice() << "Complex Log::handle performance"
      & attr("Time per log()") << avg << " us" | default_log();
}

TEST_CASE("Simple Log::handle performance", BE_CATCH_TAGS) {
   ensure_init();
   Log& log = blackhole_log();

   F64 avg = 0;

   for (int i = 0; i < 1000; ++i) {
      U64 begin = perf_now();

      be_info() << "asdf" || log;
      be_info() << "asdf" || log;
      be_info() << "asdf" || log;
      be_info() << "asdf" || log;
      be_verbose() << "asdf" || log;
      be_info() << "asdf" || log;
      be_info() << "asdf" || log;
      be_info() << "asdf" || log;
      be_info() << "asdf" || log;
      be_verbose() << "asdf" || log;

      U64 end = perf_now();

      F64 elapsed = perf_delta_to_seconds(end - begin);
      avg += elapsed;
   }

   avg *= 100;

   be_notice() << "Simple Log::handle performance"
      & attr("Time per log()") << avg << " us" | default_log();
}

TEST_CASE("Complex Log::post performance", BE_CATCH_TAGS) {
   ensure_init();
   Log& log = blackhole_log();

   F64 avg = 0;

   for (int i = 0; i < 1000; ++i) {
      U64 begin = perf_now();

      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         | log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         | log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         | log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         | log;
      be_verbose() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         | log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         | log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         | log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         | log;
      be_info() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         | log;
      be_verbose() << "asdf"
         & attr("asdffff") << 123
         & attr(1) << color::blue << "asdf " << color::red << "red"
         & attr("hello") << "world"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         & attr("aaaa") << "balskdgjalskgj;alskdjf;alskfj"
         | log;

      U64 end = perf_now();

      F64 elapsed = perf_delta_to_seconds(end - begin);
      avg += elapsed;
   }

   avg *= 100;

   be_notice() << "Complex Log::post performance"
      & attr("Time per log()") << avg << " us" | default_log();
}

TEST_CASE("Simple Log::post performance", BE_CATCH_TAGS) {
   ensure_init();
   Log& log = blackhole_log();

   F64 avg = 0;

   for (int i = 0; i < 1000; ++i) {
      U64 begin = perf_now();

      be_info() << "asdf" | log;
      be_info() << "asdf" | log;
      be_info() << "asdf" | log;
      be_info() << "asdf" | log;
      be_verbose() << "asdf" | log;
      be_info() << "asdf" | log;
      be_info() << "asdf" | log;
      be_info() << "asdf" | log;
      be_info() << "asdf" | log;
      be_verbose() << "asdf" | log;

      U64 end = perf_now();

      F64 elapsed = perf_delta_to_seconds(end - begin);
      avg += elapsed;
   }

   avg *= 100;

   be_notice() << "Simple Log::post performance"
      & attr("Time per log()") << avg << " us" | default_log();
}


TEST_CASE("Shutdown log processing thread", BE_CATCH_TAGS) {
   service<SimpleSingleThreadExecutor>(ids::core_service_simple_single_thread_executor_logprocessing).running(false);
}

#endif
