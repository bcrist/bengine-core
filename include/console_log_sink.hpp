#pragma once
#ifndef BE_CORE_CONSOLE_LOG_SINK_HPP_
#define BE_CORE_CONSOLE_LOG_SINK_HPP_

#include "log.hpp"
#include "console.hpp"
#include <iostream>
#include <algorithm>
#include <mutex>

namespace be {

///////////////////////////////////////////////////////////////////////////////
class ConsoleLogSink final {
public:
   ConsoleLogSink();
   ConsoleLogSink(std::ostream& output, std::ostream& error);
   ConsoleLogSink(const ConsoleLogSink& other);
   ConsoleLogSink& operator=(ConsoleLogSink other);

   void operator()(const LogRecord& rec, Log&);

private:
   std::mutex mutex_;
   std::ostream* output_stream_;
   std::ostream* error_stream_;
   bool last_rec_was_short_form_ = false;
};

} // be

#endif
