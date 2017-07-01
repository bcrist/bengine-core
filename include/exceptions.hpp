#pragma once
#ifndef BE_CORE_EXCEPTIONS_HPP_
#define BE_CORE_EXCEPTIONS_HPP_

#include "stack_trace_exception.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
class RecoverableError : public std::system_error {
public:
   explicit RecoverableError(std::error_code ec);
   RecoverableError(std::error_code ec, const S& msg);
   RecoverableError(std::error_code ec, const char* msg);
};

///////////////////////////////////////////////////////////////////////////////
class FatalError : public std::system_error {
public:
   explicit FatalError(std::error_code ec);
   FatalError(std::error_code ec, const S& msg);
   FatalError(std::error_code ec, const char* msg);
};

///////////////////////////////////////////////////////////////////////////////
using RecoverableTrace = StackTraceException<RecoverableError>;
using FatalTrace = StackTraceException<FatalError>;

} // be

#endif
