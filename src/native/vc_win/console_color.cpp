#include "pch.hpp"
#include "native/vc_win/console_color.hpp"
#ifdef BE_NATIVE_VC_WIN
#include <be/core/console_color.hpp>
#include "native/vc_win/vc_win_win32.hpp"
#include <iostream>

namespace be {
namespace {

bool setup_initial_color();
LogColorState& initial_color();

///////////////////////////////////////////////////////////////////////////////
void set_console_color(std::ios_base&, LogColorState color, void* handle) {
	::HANDLE console = static_cast<::HANDLE>(handle);
	if (console == nullptr) {
		return;
	}

	{
		static bool setup = setup_initial_color();

		int new_fg = static_cast<int>(color.fg);
		int new_bg = static_cast<int>(color.bg);

		if (new_fg == static_cast<int>(LogColor::initial)) {
			new_fg = static_cast<int>(initial_color().fg);
			if (new_fg == static_cast<int>(LogColor::initial)) {
				new_fg = static_cast<int>(LogColor::gray);
			}
		}

		if (new_bg == static_cast<int>(LogColor::initial)) {
			new_bg = static_cast<int>(initial_color().bg);
			if (new_bg == static_cast<int>(LogColor::initial)) {
				new_bg = static_cast<int>(LogColor::black);
			}
		}

		UC c = 0;

		if (new_fg & 0x1)  c |= FOREGROUND_RED;
		if (new_fg & 0x2)  c |= FOREGROUND_GREEN;
		if (new_fg & 0x4)  c |= FOREGROUND_BLUE;
		if (new_fg & 0x8)  c |= FOREGROUND_INTENSITY;

		if (new_bg & 0x1) c |= BACKGROUND_RED;
		if (new_bg & 0x2) c |= BACKGROUND_GREEN;
		if (new_bg & 0x4) c |= BACKGROUND_BLUE;
		if (new_bg & 0x8) c |= BACKGROUND_INTENSITY;

		::SetConsoleTextAttribute(console, c);
	}
}

///////////////////////////////////////////////////////////////////////////////
LogColorState calculate_initial_color() {
   LogColorState retval;

   ::HANDLE console = ::GetStdHandle(STD_OUTPUT_HANDLE);
   if (!console) {
      console = ::GetStdHandle(STD_ERROR_HANDLE);
   }

   if (!console) {
      retval.fg = LogColor::initial;
      retval.bg = LogColor::initial;
   } else {
      ::CONSOLE_SCREEN_BUFFER_INFO csbi;
      ::GetConsoleScreenBufferInfo(console, &csbi);

      auto a = csbi.wAttributes;
      U8 fg = 0;
      U8 bg = 0;

      if (a & FOREGROUND_RED) fg |= 0x1;
      if (a & FOREGROUND_GREEN) fg |= 0x2;
      if (a & FOREGROUND_BLUE) fg |= 0x4;
      if (a & FOREGROUND_INTENSITY) fg |= 0x8;

      if (a & BACKGROUND_RED) fg |= 0x1;
      if (a & BACKGROUND_GREEN) fg |= 0x2;
      if (a & BACKGROUND_BLUE) fg |= 0x4;
      if (a & BACKGROUND_INTENSITY) fg |= 0x8;

      retval.fg = static_cast<LogColor>(fg);
      retval.bg = static_cast<LogColor>(bg);
   }

   return retval;
}

///////////////////////////////////////////////////////////////////////////////
LogColorState& initial_color() {
   static LogColorState color = calculate_initial_color();
   return color;
}

///////////////////////////////////////////////////////////////////////////////
void restore_initial_color() {
   LogColorState color = initial_color();
   if (color.fg != LogColor::initial && color.bg != LogColor::initial) {
      set_console_color(std::cout, color, ::GetStdHandle(STD_OUTPUT_HANDLE));
   }
}

///////////////////////////////////////////////////////////////////////////////
bool setup_initial_color() {
   initial_color();
   std::atexit(restore_initial_color);
   return true;
}

} // be::()
namespace detail {

///////////////////////////////////////////////////////////////////////////////
void init_console_color_handlers() {
	set_stream_color_handler(std::cout, set_console_color, ::GetStdHandle(STD_OUTPUT_HANDLE));
	set_stream_color_handler(std::cerr, set_console_color, ::GetStdHandle(STD_ERROR_HANDLE));
}

} // be::detail

///////////////////////////////////////////////////////////////////////////////
void set_console_color_at_exit(const LogColorState& color) {
   setup_initial_color();
   initial_color() = color;
}

} // be

#endif
