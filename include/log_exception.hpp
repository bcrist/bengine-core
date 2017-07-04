#pragma once
#ifndef BE_CORE_LOG_EXCEPTION_HPP_
#define BE_CORE_LOG_EXCEPTION_HPP_

#include "exceptions.hpp"
#include "log.hpp"
#include "filesystem.hpp"

namespace be {

void log_exception(const RecoverableTrace& e, Log& log, v::Verbosity verbosity = v::error, const char* source = nullptr);
void log_exception(const FatalTrace& e, Log& log, v::Verbosity verbosity = v::error, const char* source = nullptr);
void log_exception(const fs::filesystem_error& e, Log& log, v::Verbosity verbosity = v::error, const char* source = nullptr);
void log_exception(const std::system_error& e, Log& log, v::Verbosity verbosity = v::error, const char* source = nullptr);
void log_exception(const std::exception& e, Log& log, v::Verbosity verbosity = v::error, const char* source = nullptr);

} // be

#endif
