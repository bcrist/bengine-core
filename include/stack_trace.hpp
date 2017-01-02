#pragma once
#ifndef BE_CORE_STACK_TRACE_HPP_
#define BE_CORE_STACK_TRACE_HPP_

#include "be.hpp"
#include BE_NATIVE_CORE(stack_trace.hpp)
#include <vector>

namespace be {

struct StackTrace {
   std::vector<std::uintptr_t> frames;
};

struct StackFrameSymbolInfo {
   std::uintptr_t address;
   S symbol;
   S module;
   S file;
   U32 line;
   I32 line_displacement;
};

StackTrace get_stack_trace(U32 frames_to_skip = 0);
StackFrameSymbolInfo get_stack_frame_symbol_info(std::uintptr_t address);
std::ostream& operator<<(std::ostream& os, const StackTrace& trace);

} // be

#endif
