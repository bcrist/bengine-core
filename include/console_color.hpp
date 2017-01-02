#pragma once
#ifndef BE_CORE_CONSOLE_COLOR_HPP_
#define BE_CORE_CONSOLE_COLOR_HPP_

#include "be.hpp"
#include "verbosity.hpp"
#include <sstream>

namespace be {

enum class LogColor : U8 {
   black = 0,
   red,
   green,
   yellow,
   blue,
   purple,
   cyan,
   gray,
   dark_gray,
   bright_red,
   bright_green,
   bright_yellow,
   bright_blue,
   bright_purple,
   bright_cyan,
   white,

   initial = 0x10,
   current = 0x20,
   other = 0x30,
};

struct LogColorState {
   LogColor fg;
   LogColor bg;
};

namespace color {

extern const LogColorState black;
extern const LogColorState red;
extern const LogColorState green;
extern const LogColorState yellow;
extern const LogColorState blue;
extern const LogColorState purple;
extern const LogColorState cyan;
extern const LogColorState gray;
extern const LogColorState dark_gray;
extern const LogColorState bright_red;
extern const LogColorState bright_green;
extern const LogColorState bright_yellow;
extern const LogColorState bright_blue;
extern const LogColorState bright_purple;
extern const LogColorState bright_cyan;
extern const LogColorState white;

extern const LogColorState reset;
extern const LogColorState invert;

///////////////////////////////////////////////////////////////////////////////

extern const LogColorState fg_black;
extern const LogColorState fg_red;
extern const LogColorState fg_green;
extern const LogColorState fg_yellow;
extern const LogColorState fg_blue;
extern const LogColorState fg_purple;
extern const LogColorState fg_cyan;
extern const LogColorState fg_gray;
extern const LogColorState fg_dark_gray;
extern const LogColorState fg_bright_red;
extern const LogColorState fg_bright_green;
extern const LogColorState fg_bright_yellow;
extern const LogColorState fg_bright_blue;
extern const LogColorState fg_bright_purple;
extern const LogColorState fg_bright_cyan;
extern const LogColorState fg_white;

extern const LogColorState fg_reset;

///////////////////////////////////////////////////////////////////////////////

extern const LogColorState bg_black;
extern const LogColorState bg_red;
extern const LogColorState bg_green;
extern const LogColorState bg_yellow;
extern const LogColorState bg_blue;
extern const LogColorState bg_purple;
extern const LogColorState bg_cyan;
extern const LogColorState bg_gray;
extern const LogColorState bg_dark_gray;
extern const LogColorState bg_bright_red;
extern const LogColorState bg_bright_green;
extern const LogColorState bg_bright_yellow;
extern const LogColorState bg_bright_blue;
extern const LogColorState bg_bright_purple;
extern const LogColorState bg_bright_cyan;
extern const LogColorState bg_white;

extern const LogColorState bg_reset;

} // be::color

using StreamColorHandler = void (*)(std::ios_base&, LogColorState, void*);

void set_stream_color_handler(std::ios_base& s, StreamColorHandler handler, void* ud = nullptr);
bool set_stream_color(std::ios_base& s, LogColorState color);
LogColorState get_stream_color(std::ios_base& s);
LogColorState verbosity_color(v::Verbosity level);

///////////////////////////////////////////////////////////////////////////////
/// \details Native-implemented functions
void set_console_color_at_exit(const LogColorState& color);

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
std::basic_ostream<C, CTraits>& operator<<(std::basic_ostream<C, CTraits>& os, LogColorState color) {
   set_stream_color(os, color);
   return os;
}

LogColorState setcolor(LogColor color);
LogColorState setcolor(LogColor color, LogColor background);

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
LogColorState get_color(const std::basic_ostream<C, CTraits>& os) {
   std::basic_ostringstream<C, CTraits> state;
   state.copyfmt(os);
   return get_stream_color(state);
}

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
LogColor get_foreground_color(const std::basic_ostream<C, CTraits>& os) {
   LogColorState state = get_color(os);
   return state.fg;
}

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename CTraits>
LogColor get_background_color(const std::basic_ostream<C, CTraits>& os) {
   LogColorState state = get_color(os);
   return state.bg;
}

} // be

#endif
