#pragma once
#ifndef BE_CORE_EXCEPTIONS_HPP_
#define BE_CORE_EXCEPTIONS_HPP_

#include "stack_trace_exception.hpp"

namespace be {

template <typename ErrorType = void>
class RecoverableException : public std::runtime_error {
public:
   template <typename... Ts>
   explicit RecoverableException(ErrorType error, Ts&&... args)
      : std::runtime_error(std::forward<Ts>(args)...),
        err_(std::move(error)) { }

   const ErrorType& error() const {
      return err_;
   }

private:
   ErrorType err_;
};

template <>
class RecoverableException<void> : public std::runtime_error {
public:
   template <typename... Ts>
   explicit RecoverableException(Ts&&... args)
      : std::runtime_error(std::forward<Ts>(args)...) { }
};
class FatalException : public std::runtime_error {
public:
   template <typename... Ts>
   explicit FatalException(Ts&&... args)
      : std::runtime_error(std::forward<Ts>(args)...) { }
};

template <typename ErrorType = void>
using Recoverable = StackTraceException<RecoverableException<ErrorType>>;
using Fatal = StackTraceException<FatalException>;

} // be

#endif
