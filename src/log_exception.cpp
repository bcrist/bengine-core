#include "pch.hpp"
#include "log_exception.hpp"
#include <be/core/logging.hpp>

namespace be {

///////////////////////////////////////////////////////////////////////////////
void log_exception(const RecoverableTrace& e, Log& log, v::Verbosity verbosity, const char* source) {
   be_log(verbosity, source) << S(e.what())
      & attr(ids::log_attr_category) << e.code().category().name()
      & attr(ids::log_attr_error_code) << e.code().value()
      & attr(ids::log_attr_description) << e.code().message()
      | log;
}

///////////////////////////////////////////////////////////////////////////////
void log_exception(const FatalTrace& e, Log& log, v::Verbosity verbosity, const char* source) {
   be_log(verbosity, source) << S(e.what())
      & attr(ids::log_attr_category) << e.code().category().name()
      & attr(ids::log_attr_error_code) << e.code().value()
      & attr(ids::log_attr_description) << e.code().message()
      & attr(ids::log_attr_trace) << StackTrace(e.trace())
      | log;
}

///////////////////////////////////////////////////////////////////////////////
void log_exception(const fs::filesystem_error& e, Log& log, v::Verbosity verbosity, const char* source) {
   be_log(verbosity, source) << S(e.what())
      & attr(ids::log_attr_category) << e.code().category().name()
      & attr(ids::log_attr_error_code) << e.code().value()
      & attr(ids::log_attr_description) << e.code().message()
      & attr(ids::log_attr_path) << e.path1().string()
      & attr(ids::log_attr_path) << e.path2().string()
      | log;
}

///////////////////////////////////////////////////////////////////////////////
void log_exception(const std::system_error& e, Log& log, v::Verbosity verbosity, const char* source) {
   be_log(verbosity, source) << S(e.what())
      & attr(ids::log_attr_category) << e.code().category().name()
      & attr(ids::log_attr_error_code) << e.code().value()
      & attr(ids::log_attr_description) << e.code().message()
      | log;
}

///////////////////////////////////////////////////////////////////////////////
void log_exception(const std::exception& e, Log& log, v::Verbosity verbosity, const char* source) {
   be_log(verbosity, source) << S(e.what()) | log;
}

} // be
