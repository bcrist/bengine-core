#pragma once
#ifndef BE_CORE_LOG_EXCEPTION_HPP_
#define BE_CORE_LOG_EXCEPTION_HPP_

#include "exceptions.hpp"
#include "default_log.hpp"
#include "filesystem.hpp"

namespace be {

void log_exception(const RecoverableTrace& e, v::Verbosity verbosity = v::error, Log& log = default_log(), const char* source = nullptr);
void log_exception(const FatalTrace& e, v::Verbosity verbosity = v::error, Log& log = default_log(), const char* source = nullptr);
void log_exception(const fs::filesystem_error& e, v::Verbosity verbosity = v::error, Log& log = default_log(), const char* source = nullptr);
void log_exception(const std::system_error& e, v::Verbosity verbosity = v::error, Log& log = default_log(), const char* source = nullptr);
void log_exception(const std::exception& e, v::Verbosity verbosity = v::error, Log& log = default_log(), const char* source = nullptr);

} // be

#endif
