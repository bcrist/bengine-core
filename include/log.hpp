#pragma once
#ifndef BE_CORE_LOG_HPP_
#define BE_CORE_LOG_HPP_

#include "log_expr.hpp"
#include "log_record.hpp"
#include <functional>

#ifndef BE_CORE_DEFAULT_LOG_VERBOSITY_MASK
//#define BE_CORE_DEFAULT_LOG_VERBOSITY_MASK v::everything
#define BE_CORE_DEFAULT_LOG_VERBOSITY_MASK v::info_or_worse
#endif

namespace be {

class Log;
using LogSink = std::function<void(const LogRecord&, Log&)>;

///////////////////////////////////////////////////////////////////////////////
class Log final : Immovable {
public:
   Log();
   explicit Log(U16 mask);
   Log(U16 mask, LogSink sink_func) : mask_(mask) { sink(std::move(sink_func)); }

   void reset();

   void clear_sinks();
   void sink(LogSink sink);

   U16 verbosity_mask(U16 mask) noexcept;
   U16& verbosity_mask() noexcept;
   U16 verbosity_mask() const noexcept;

   template <typename L>
   bool post(detail::LogExpr<L>&& log_expr);
   void post(LogRecord record);

   template <typename L>
   bool handle(detail::LogExpr<L>&& log_expr);
   void handle(const LogRecord& record);

private:
   U16 mask_;
   std::vector<LogSink> sinks_;
};

} // be

#endif
