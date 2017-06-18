#include <be/core/native.hpp>
#ifdef BE_NATIVE_VC_WIN
#include <be/core/console.hpp>
#include "native/vc_win/vc_win_win32.hpp"
#include <mutex>
#include <iostream>
#include <conio.h>
#include <io.h>

namespace be {
namespace {

///////////////////////////////////////////////////////////////////////////////
bool is_stdout_interactive() {
   static bool interactive = 0 != _isatty(_fileno(stdout));
   return interactive;
}

///////////////////////////////////////////////////////////////////////////////
bool is_stderr_interactive() {
   static bool interactive = 0 != _isatty(_fileno(stderr));
   return interactive;
}

///////////////////////////////////////////////////////////////////////////////
bool is_stdin_interactive() {
   static bool interactive = 0 != _isatty(_fileno(stdin));
   return interactive;
}

} // be::()

///////////////////////////////////////////////////////////////////////////////
bool is_interactive_console(const std::ostream& os) {
   if (&os == &std::cout) {
      return is_stdout_interactive();
   } else if (&os == &std::cerr) {
      return is_stderr_interactive();
   }
   return false;
}

///////////////////////////////////////////////////////////////////////////////
bool is_interactive_console(const std::istream& is) {
   if (&is == &std::cin) {
      return is_stdin_interactive();
   }
   return false;
}

///////////////////////////////////////////////////////////////////////////////
U16 console_width(std::ostream& os) {
   ::HANDLE console;
   if (&os == &std::cerr) {
      console = ::GetStdHandle(STD_ERROR_HANDLE);
   } else if (&os == &std::cout) {
      console = ::GetStdHandle(STD_OUTPUT_HANDLE);
   } else {
      return 80;
   }

   ::CONSOLE_SCREEN_BUFFER_INFO info;
   ::GetConsoleScreenBufferInfo(console, &info);

   return static_cast<U16>(info.dwSize.X);
}

///////////////////////////////////////////////////////////////////////////////
const char* preferred_line_ending() {
   return "\r\n";
}

namespace consoleinput {

///////////////////////////////////////////////////////////////////////////////
int get_char_nonblocking(int default_value) {
   static std::mutex m;
   int ch = default_value;
   std::lock_guard<std::mutex> guard(m);
   if (::_kbhit()) {
      ch = ::_getch_nolock();
      if (ch == 0 || ch == 0xE0) {
         ch |= 0x100;
         ch <<= 8;
         ch |= ::_getch_nolock();
      }
   }
   return ch;
}

} // be::consoleinput
} // be

#endif
