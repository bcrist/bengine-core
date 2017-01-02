#pragma once
#ifndef BE_CORE_LOG_PROCESSOR_HPP_
#define BE_CORE_LOG_PROCESSOR_HPP_

#include "log_record.hpp"
#include "log_expr.hpp"
#include "ostream_config.hpp"

namespace be { 

class Log;

namespace detail {

///////////////////////////////////////////////////////////////////////////////
void handle_log_record(const LogRecord& record, Log& log);

///////////////////////////////////////////////////////////////////////////////
template <typename E>
class LogProcessor {
public:
   LogProcessor(Log& log, LogRecord& rec)
      : log_(log),
        rec_(rec),
        color_(color::reset)
   {
      OStreamConfig config;
      os_ << config;

      set_stream_color_handler(os_, stream_color_handler, this);
   }

   void operator()(const E& log_expr) {
      process_(log_expr);
      finish_();
   }

private:
   static void stream_color_handler(std::ios_base&, LogColorState color, void* ud) {
      auto& self = *static_cast<LogProcessor<E>*>(ud);
      self.finish_();
      self.color_ = color;
   }

   void finish_() {
      S str = os_.str();
      if (!str.empty()) {
         rec_.attributes.back().contents.push_back({ str, color_ });
         os_.str(S());
      }
      os_.clear();
   }

   LogAttribute& push_attrib_() {
      finish_();
      OStreamConfig config;
      os_ << config;
      rec_.attributes.emplace_back();
      return rec_.attributes.back();
   }

   void process_(const detail::NullLogExpr&) { }

   void process_(const detail::LogEntry& entry) {
      rec_.verbosity = entry.verbosity();
      rec_.short_form = entry.short_form();
      rec_.line = entry.line();
      rec_.timestamp = entry.timestamp();
      rec_.source = entry.source();
      rec_.file = entry.filename();
      push_attrib_().verbosity = static_cast<v::Verbosity>(~0);
   }

   void process_(const detail::LogAttr& attr) {
      LogAttribute& attrib = push_attrib_();
      attrib.id = attr.id();
      attrib.verbosity = static_cast<v::Verbosity>(~0);
   }

   void process_(const detail::VerboseLogAttr& attr) {
      LogAttribute& attrib = push_attrib_();
      attrib.id = attr.id();
      attrib.verbosity = attr.verbosity();
   }

   void process_(const detail::LogForm& form) {
      if (check_verbosity(form.verbosity(), log_.verbosity_mask())) {
         rec_.short_form = form.short_form();
      }
   }

   template <typename L, typename R>
   void process_(const detail::LogList<L, R>& list) {
      process_(list.left());
      process_(list.right());
   }

   template <typename L, typename R>
   void process_(const detail::LogInsertion<L, R>& insertion) {
      validate_insertion_(TNil<R>());
      process_(insertion.left());
      os_ << insertion.right();
   }

   template <typename R>
   void validate_insertion_(TNil<R>) const { /* OK */ }
   template <typename E>
   void validate_insertion_(TNil<detail::LogExpr<E>>) const     { static_assert(false, "Can't insert that log expression into a log entry!  Perhaps you meant to use operator& instead?"); }
   void validate_insertion_(TNil<detail::LogAttr>) const        { static_assert(false, "Can't insert a log attribute into a log entry!  Remember to use '... & attr(...) << ...' not ' ... << attr(...) << ...'."); }
   void validate_insertion_(TNil<detail::VerboseLogAttr>) const { static_assert(false, "Can't insert a log attribute into a log entry!  Remember to use '... & attr(...) << ...' not ' ... << attr(...) << ...'."); }
   void validate_insertion_(TNil<detail::LogForm>) const        { static_assert(false, "Can't insert a log short/long form specifier into a log entry!  Remember to use '... & short_form(...) << ...' not ' ... << short_form(...) << ...' (or the long_form equivalents)."); }

   Log& log_;
   LogRecord& rec_;
   LogColorState color_;
   std::ostringstream os_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename E>
struct LogProcessorTask {
   LogProcessorTask(Log& log, E log_expr)
      : log(log),
        log_expr(std::move(log_expr)) { }

   void operator()() {
      LogRecord rec;
      LogProcessor<E>(log, rec)(log_expr);
      handle_log_record(rec, log);
   }

   Log& log;
   const E log_expr;
};

} // be::detail
} // be

#endif
