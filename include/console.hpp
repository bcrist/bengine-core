#pragma once
#ifndef BE_CORE_CONSOLE_HPP_
#define BE_CORE_CONSOLE_HPP_

#include "ostream_config.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
bool is_interactive_console(const std::ostream& os);

///////////////////////////////////////////////////////////////////////////////
bool is_interactive_console(const std::istream& is);

///////////////////////////////////////////////////////////////////////////////
U16 console_width(std::ostream& os);

///////////////////////////////////////////////////////////////////////////////
const char* preferred_line_ending();

namespace consoleinput {

// TODO consider moving this to a BE_NATIVE header

enum SpecialKey {
   no_input = -1,
   backspace = '\b',
   f1 = 0x1003b,
   f2 = 0x1003c,
   f3 = 0x1003d,
   f4 = 0x1003e,
   f5 = 0x1003f,
   f6 = 0x10040,
   f7 = 0x10041,
   f8 = 0x10042,
   f9 = 0x10043,
   f10 = 0x10044,
   f11 = 0x1e085,
   f12 = 0x1e086,
   up_arrow = 0x1e048,
   down_arrow = 0x1e050,
   left_arrow = 0x1e04b,
   right_arrow = 0x1e04d,
   insert = 0x1e052,
   del = 0x1e053,
   home = 0x1e047,
   end = 0x1e04f,
   pgup = 0x1e049,
   pgdn = 0x1e051,
};

///////////////////////////////////////////////////////////////////////////////
int get_char_nonblocking(int default_value = no_input);

} // be::consoleinput
} // be

#endif
