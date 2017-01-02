#pragma once
#ifndef BE_CORE_STACK_TRACE_EXCEPTION_HPP_
#define BE_CORE_STACK_TRACE_EXCEPTION_HPP_

#include "stack_trace.hpp"

namespace be {

template <typename E>
class StackTraceException : public E {
public:

   template <typename... Ts>
   explicit BE_NO_INLINE StackTraceException(Ts&&... args)
      : E(std::forward<Ts>(args)...),
        trace_(get_stack_trace(1)) { }

   StackTraceException(const StackTraceException<E>& other) = default;
   StackTraceException(StackTraceException<E>&& other) = default;

   const StackTrace& trace() const {
      return trace_;
   }

private:
   StackTrace trace_;
};

} // be

#endif
