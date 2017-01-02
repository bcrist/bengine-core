#pragma once
#ifndef BE_CORE_LOG_EXPR_HPP_
#define BE_CORE_LOG_EXPR_HPP_

#include "id.hpp"
#include "time.hpp"
#include "verbosity.hpp"

namespace be {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename E>
class LogExpr {
public:
   operator E&() { return static_cast<E&>(*this); }
   operator const E&() const { return static_cast<const E&>(*this); }
};

///////////////////////////////////////////////////////////////////////////////
class NullLogExpr : public LogExpr<NullLogExpr> {
public:
   NullLogExpr() noexcept { }
};

///////////////////////////////////////////////////////////////////////////////
struct DisabledLogEntry {
   DisabledLogEntry line(int) { return *this; }
   DisabledLogEntry timestamp(TU) { return *this; }
   DisabledLogEntry source(const char*) { return *this; }
   DisabledLogEntry filename(const char*) { return *this; }
};

///////////////////////////////////////////////////////////////////////////////
class LogEntry : public LogExpr<LogEntry> {
public:
   LogEntry(v::Verbosity verbosity, bool short_form) noexcept
      : verbosity_(verbosity),
        short_form_(short_form),
        timestamp_(ts_now())
   { }

   LogEntry(v::Verbosity verbosity, bool short_form, const char* source) noexcept
      : verbosity_(verbosity),
        short_form_(short_form),
        timestamp_(ts_now()),
        source_(source)
   { }

   LogEntry line(int line) noexcept { line_ = line; return *this; }
   LogEntry timestamp(TU ts) noexcept { timestamp_ = ts; return *this; }
   LogEntry source(const char* src) noexcept { file_ = src; return *this; }
   LogEntry filename(const char* file) noexcept { file_ = file; return *this; }

   v::Verbosity verbosity() const noexcept { return verbosity_; }
   bool short_form() const noexcept { return short_form_; }
   int line() const noexcept { return line_; }
   TU timestamp() const noexcept { return timestamp_; }
   const char* source() const noexcept { return source_; }
   const char* filename() const noexcept { return file_; }

private:
   const v::Verbosity verbosity_;
   bool short_form_;
   U32 line_ = 0;
   TU timestamp_;
   const char* source_ = nullptr;
   const char* file_ = nullptr;
};

///////////////////////////////////////////////////////////////////////////////
class LogAttr final : public LogExpr<LogAttr> {
public:
   constexpr LogAttr(Id id) noexcept : id_(id) { }
   constexpr Id id() const noexcept { return id_; }

private:
   const Id id_;
};

///////////////////////////////////////////////////////////////////////////////
class VerboseLogAttr final : public LogExpr<VerboseLogAttr> {
public:
   constexpr VerboseLogAttr(Id id, v::Verbosity verbosity) noexcept : id_(id), verbosity_(verbosity) { }
   constexpr Id id() const noexcept { return id_; }
   constexpr v::Verbosity verbosity() const noexcept { return verbosity_; }

private:
   const Id id_;
   const v::Verbosity verbosity_;
};

///////////////////////////////////////////////////////////////////////////////
class LogForm final : public LogExpr<LogForm> {
public:
   constexpr LogForm(bool short_form, v::Verbosity verbosity) noexcept : short_form_(short_form), verbosity_(verbosity) { }
   constexpr bool short_form() const noexcept { return short_form_; }
   constexpr v::Verbosity verbosity() const noexcept { return verbosity_; }

private:
   const bool short_form_;
   const v::Verbosity verbosity_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename T>
class LogObject final : public LogExpr<LogObject<T>> {
public:
   LogObject(T&& object) : object_(std::forward<T>(object)) { }
   const T& get() const noexcept { return object_; }

private:
   const T object_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename L, typename R>
class LogInsertion final : public LogExpr<LogInsertion<L, R>> {
public:
   LogInsertion(LogExpr<L>&& l, LogObject<R>&& r)
      : l_(std::move(l)),
        r_(std::move(r))
   { }

   v::Verbosity verbosity() const noexcept {
      return l_.verbosity();
   }

   const L& left() const noexcept {
      return l_;
   }

   const R& right() const noexcept {
      return r_.get();
   }

private:
   const L l_;
   const LogObject<R> r_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename L, typename R>
class LogList final : public LogExpr<LogList<L, R>> {
public:
   LogList(LogExpr<L>&& l, LogExpr<R>&& r)
      : l_(std::move(l)),
        r_(std::move(r))
   { }

   v::Verbosity verbosity() const noexcept {
      return l_.verbosity();
   }

   const L& left() const noexcept {
      return l_;
   }

   const R& right() const noexcept {
      return r_;
   }

private:
   const L l_;
   const R r_;
};

} // be::detail
} // be

#endif
