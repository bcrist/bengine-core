#include "pch.hpp"
#include "log.hpp"
#include "service_ids.hpp"
#include "service_simple_single_thread_executor.hpp"
#include "service_helpers.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
Log::Log()
   : mask_(BE_CORE_DEFAULT_LOG_VERBOSITY_MASK)
{ }

///////////////////////////////////////////////////////////////////////////////
Log::Log(U16 mask)
   : mask_(mask)
{ }

///////////////////////////////////////////////////////////////////////////////
void Log::reset() {
   clear_sinks();
}

///////////////////////////////////////////////////////////////////////////////
void Log::clear_sinks() {
   sinks_.clear();
}

///////////////////////////////////////////////////////////////////////////////
void Log::sink(LogSink sink) {
   if (sink) {
      sinks_.push_back(std::move(sink));
   }
}

///////////////////////////////////////////////////////////////////////////////
U16 Log::verbosity_mask(U16 mask) noexcept {
   U16 old_mask = mask_;
   mask_ = mask;
   return old_mask;
}

///////////////////////////////////////////////////////////////////////////////
U16 & Log::verbosity_mask() noexcept {
   return mask_;
}

///////////////////////////////////////////////////////////////////////////////
U16 Log::verbosity_mask() const noexcept {
   return mask_;
}

namespace {

struct PostRecordFunctor {
   Log* log;
   LogRecord record;

   void operator()() {
      log->handle(record);
   }
};

} // be::()

///////////////////////////////////////////////////////////////////////////////
void Log::post(LogRecord record) {
   auto& exec = service<SimpleSingleThreadExecutor>(Id(BEID_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING));

   if (exec.running()) {
      // background service is running, handle asynchronously
      exec.post(PostRecordFunctor{ this, std::move(record) });
   } else {
      // background service not running, handle synchronously
      handle(record);
   }
}

///////////////////////////////////////////////////////////////////////////////
void Log::handle(const LogRecord & record) {
   for (LogSink& sink : sinks_) {
      sink(record, *this);
   }
}

} // be
