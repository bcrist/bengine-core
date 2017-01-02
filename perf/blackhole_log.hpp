#pragma once
#ifndef BE_CORE_PERF_BLACKHOLE_LOG_HPP_
#define BE_CORE_PERF_BLACKHOLE_LOG_HPP_

#include "service_simple_single_thread_executor.hpp"
#include "services.hpp"
#include "logging.hpp"
#include "console_log_sink.hpp"
#include <sstream>

namespace be {

inline std::unique_ptr<Log> create_blackhole_log() {
   auto log = std::make_unique<Log>();
   log->clear_sinks();
   return log;
}

///////////////////////////////////////////////////////////////////////////////
inline Log& blackhole_log() {
   static std::unique_ptr<Log> log = create_blackhole_log();
   service<SimpleSingleThreadExecutor>(ids::core_service_simple_single_thread_executor_logprocessing).running(true);

   return *log;
}

} // be

#endif
