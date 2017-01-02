#pragma once
#ifndef BE_CORE_LOG_EXPR_OPS_HPP_
#define BE_CORE_LOG_EXPR_OPS_HPP_

#include "log.hpp"
#include "stack_trace.hpp"

#define be_log(...)    ::be::log(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_fatal(...)  ::be::log_fatal(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_error(...)  ::be::log_error(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_warn(...)   ::be::log_warn(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_notice(...) ::be::log_notice(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_info(...)   ::be::log_info(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_verbose(...) ::be::log_verbose(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_debug(...)  if (::be::detail::DebugLog::value) ::be::log_debug(__VA_ARGS__).filename(__FILE__).line(__LINE__)

#define be_short_log(...)    ::be::log_short(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_short_fatal(...)  ::be::log_short_fatal(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_short_error(...)  ::be::log_short_error(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_short_warn(...)   ::be::log_short_warn(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_short_notice(...) ::be::log_short_notice(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_short_info(...)   ::be::log_short_info(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_short_verbose(...) ::be::log_short_verbose(__VA_ARGS__).filename(__FILE__).line(__LINE__)
#define be_short_debug(...)  if (::be::detail::DebugLog::value) ::be::log_short_debug(__VA_ARGS__).filename(__FILE__).line(__LINE__)

#define BE_LOG_INTERP(x) "$(" x ")"

namespace be {

detail::LogEntry log(v::Verbosity verbosity, const char* source = nullptr);
detail::LogEntry log_fatal(const char* source = nullptr);
detail::LogEntry log_error(const char* source = nullptr);
detail::LogEntry log_warn(const char* source = nullptr);
detail::LogEntry log_notice(const char* source = nullptr);
detail::LogEntry log_info(const char* source = nullptr);
detail::LogEntry log_verbose(const char* source = nullptr);

detail::LogEntry log_short(v::Verbosity verbosity, const char* source = nullptr);
detail::LogEntry log_short_fatal(const char* source = nullptr);
detail::LogEntry log_short_error(const char* source = nullptr);
detail::LogEntry log_short_warn(const char* source = nullptr);
detail::LogEntry log_short_notice(const char* source = nullptr);
detail::LogEntry log_short_info(const char* source = nullptr);
detail::LogEntry log_short_verbose(const char* source = nullptr);

#if defined(BE_DEBUG) || defined(BE_FORCE_DEBUG_LOG)
detail::LogEntry log_debug(const char* source = nullptr);
detail::LogEntry log_short_debug(const char* source = nullptr);
#else
detail::DisabledLogEntry log_debug(const char* source = nullptr);
detail::DisabledLogEntry log_short_debug(const char* source = nullptr);
#endif

detail::NullLogExpr log_nil();
detail::LogAttr log_more();

detail::LogAttr attr(Id id);
detail::LogAttr attr(U64 id);
detail::LogAttr attr(const S& name);
detail::LogAttr attr(S&& name);
detail::LogAttr attr(const char* name);

detail::VerboseLogAttr attr(Id id, v::Verbosity verbosity);
detail::VerboseLogAttr attr(U64 id, v::Verbosity verbosity);
detail::VerboseLogAttr attr(const S& name, v::Verbosity verbosity);
detail::VerboseLogAttr attr(S&& name, v::Verbosity verbosity);
detail::VerboseLogAttr attr(const char* name, v::Verbosity verbosity);

detail::VerboseLogAttr hidden(Id id);
detail::VerboseLogAttr hidden(U64 id);
detail::VerboseLogAttr hidden(const S& name);
detail::VerboseLogAttr hidden(S&& name);
detail::VerboseLogAttr hidden(const char* name);

detail::LogForm short_form(v::Verbosity verbosity = v::Verbosity::fatal);
detail::LogForm long_form(v::Verbosity verbosity = v::Verbosity::fatal);
detail::LogForm short_form(bool short_form, v::Verbosity verbosity = v::Verbosity::fatal);
detail::LogForm long_form(bool long_form, v::Verbosity verbosity = v::Verbosity::fatal);

detail::LogInsertion<detail::LogAttr, StackTrace> trace();

namespace detail {

#ifdef BE_FORCE_DEBUG_LOG
using DebugLog = std::true_type;
#else
using DebugLog = IsDebug;
#endif

typedef std::ostream& (*OstreamFuncPtr)(std::ostream&);
typedef std::ios& (*IosFuncPtr)(std::ios&);
typedef std::ios_base& (*IosBaseFuncPtr)(std::ios_base&);

///////////////////////////////////////////////////////////////////////////////
template <typename L, typename R>
LogInsertion<L, R> operator<<(LogExpr<L>&& expr, R object) {
   using V = std::remove_reference_t<R>;
   return LogInsertion<L, V>(std::move(expr), LogObject<V>(std::move(object)));
}

///////////////////////////////////////////////////////////////////////////////
template <typename L>
LogInsertion<L, OstreamFuncPtr> operator<<(LogExpr<L>&& expr, OstreamFuncPtr func) {
   return LogInsertion<L, OstreamFuncPtr>(std::move(expr), std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
template <typename L>
LogInsertion<L, IosFuncPtr> operator<<(LogExpr<L>&& expr, IosFuncPtr func) {
   return LogInsertion<L, IosFuncPtr>(std::move(expr), std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
template <typename L>
LogInsertion<L, IosBaseFuncPtr> operator<<(LogExpr<L>&& expr, IosBaseFuncPtr func) {
   return LogInsertion<L, IosBaseFuncPtr>(std::move(expr), std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
template <typename L, typename R>
LogList<L, R> operator&(LogExpr<L>&& l, LogExpr<R>&& r) {
   return LogList<L, R>(std::move(l), std::move(r));
}

///////////////////////////////////////////////////////////////////////////////
template <typename R>
DisabledLogEntry operator<<(const DisabledLogEntry&, R&&) {
   return DisabledLogEntry();
}

///////////////////////////////////////////////////////////////////////////////
template <typename R>
DisabledLogEntry operator&(const DisabledLogEntry&, LogExpr<R>&&) {
   return DisabledLogEntry();
}

///////////////////////////////////////////////////////////////////////////////
template <typename L>
bool operator|(LogExpr<L>&& expr, Log& log) {
   return log.post(std::move(expr));
}

///////////////////////////////////////////////////////////////////////////////
template <typename L>
bool operator||(LogExpr<L>&& expr, Log& log) {
   return log.handle(std::move(expr));
}

///////////////////////////////////////////////////////////////////////////////
template <typename L>
void operator||(LogExpr<L>&& expr, LogRecord& rec) {
   LogProcessor<L> processor(default_log(), rec);
   processor(expr);
}

bool operator|(const DisabledLogEntry&, Log&);
bool operator||(const DisabledLogEntry&, Log&);
bool operator||(const DisabledLogEntry&, LogRecord&);

} // be::detail

void operator|(LogRecord rec, Log& log);
void operator||(const LogRecord& rec, Log& log);

} // be

#endif
