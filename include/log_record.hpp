#pragma once
#ifndef BE_CORE_LOG_RECORD_HPP_
#define BE_CORE_LOG_RECORD_HPP_

#include "id.hpp"
#include "time.hpp"
#include "verbosity.hpp"
#include "console_color.hpp"
#include <vector>

namespace be {

struct LogString {
	S text;
	LogColorState color;
};

struct LogAttribute {
   Id id;
   v::Verbosity verbosity = static_cast<v::Verbosity>(~0);
   std::vector<LogString> contents;
};

struct LogRecord {
   v::Verbosity verbosity;
   bool short_form;
   U32 line;
   TU timestamp;
   const char* source;
   const char* file;
   std::vector<LogAttribute> attributes;
};

} // be

#endif
