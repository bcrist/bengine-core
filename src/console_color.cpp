#include "pch.hpp"
#include "console_color.hpp"

namespace be {
namespace color {

const LogColorState black          = setcolor(LogColor::black, LogColor::black);
const LogColorState red            = setcolor(LogColor::red, LogColor::black);
const LogColorState green          = setcolor(LogColor::green, LogColor::black);
const LogColorState yellow         = setcolor(LogColor::yellow, LogColor::black);
const LogColorState blue           = setcolor(LogColor::blue, LogColor::black);
const LogColorState purple         = setcolor(LogColor::purple, LogColor::black);
const LogColorState cyan           = setcolor(LogColor::cyan, LogColor::black);
const LogColorState gray           = setcolor(LogColor::gray, LogColor::black);
const LogColorState dark_gray      = setcolor(LogColor::dark_gray, LogColor::black);
const LogColorState bright_red     = setcolor(LogColor::bright_red, LogColor::black);
const LogColorState bright_green   = setcolor(LogColor::bright_green, LogColor::black);
const LogColorState bright_yellow  = setcolor(LogColor::bright_yellow, LogColor::black);
const LogColorState bright_blue    = setcolor(LogColor::bright_blue, LogColor::black);
const LogColorState bright_purple  = setcolor(LogColor::bright_purple, LogColor::black);
const LogColorState bright_cyan    = setcolor(LogColor::bright_cyan, LogColor::black);
const LogColorState white          = setcolor(LogColor::white, LogColor::black);

const LogColorState reset    = setcolor(LogColor::initial, LogColor::initial);
const LogColorState invert   = setcolor(LogColor::other, LogColor::other);

///////////////////////////////////////////////////////////////////////////////

const LogColorState fg_black          = setcolor(LogColor::black);
const LogColorState fg_red            = setcolor(LogColor::red);
const LogColorState fg_green          = setcolor(LogColor::green);
const LogColorState fg_yellow         = setcolor(LogColor::yellow);
const LogColorState fg_blue           = setcolor(LogColor::blue);
const LogColorState fg_purple         = setcolor(LogColor::purple);
const LogColorState fg_cyan           = setcolor(LogColor::cyan);
const LogColorState fg_gray           = setcolor(LogColor::gray);
const LogColorState fg_dark_gray      = setcolor(LogColor::dark_gray);
const LogColorState fg_bright_red     = setcolor(LogColor::bright_red);
const LogColorState fg_bright_green   = setcolor(LogColor::bright_green);
const LogColorState fg_bright_yellow  = setcolor(LogColor::bright_yellow);
const LogColorState fg_bright_blue    = setcolor(LogColor::bright_blue);
const LogColorState fg_bright_purple  = setcolor(LogColor::bright_purple);
const LogColorState fg_bright_cyan    = setcolor(LogColor::bright_cyan);
const LogColorState fg_white          = setcolor(LogColor::white);

const LogColorState fg_reset    = setcolor(LogColor::initial);

///////////////////////////////////////////////////////////////////////////////

const LogColorState bg_black          = setcolor(LogColor::current, LogColor::black);
const LogColorState bg_red            = setcolor(LogColor::current, LogColor::red);
const LogColorState bg_green          = setcolor(LogColor::current, LogColor::green);
const LogColorState bg_yellow         = setcolor(LogColor::current, LogColor::yellow);
const LogColorState bg_blue           = setcolor(LogColor::current, LogColor::blue);
const LogColorState bg_purple         = setcolor(LogColor::current, LogColor::purple);
const LogColorState bg_cyan           = setcolor(LogColor::current, LogColor::cyan);
const LogColorState bg_gray           = setcolor(LogColor::current, LogColor::gray);
const LogColorState bg_dark_gray      = setcolor(LogColor::current, LogColor::dark_gray);
const LogColorState bg_bright_red     = setcolor(LogColor::current, LogColor::bright_red);
const LogColorState bg_bright_green   = setcolor(LogColor::current, LogColor::bright_green);
const LogColorState bg_bright_yellow  = setcolor(LogColor::current, LogColor::bright_yellow);
const LogColorState bg_bright_blue    = setcolor(LogColor::current, LogColor::bright_blue);
const LogColorState bg_bright_purple  = setcolor(LogColor::current, LogColor::bright_purple);
const LogColorState bg_bright_cyan    = setcolor(LogColor::current, LogColor::bright_cyan);
const LogColorState bg_white          = setcolor(LogColor::current, LogColor::white);

const LogColorState bg_reset    = setcolor(LogColor::current, LogColor::initial);

} // be::color
namespace {

const int color_idx = std::ios_base::xalloc();
const int color_handler_idx = std::ios_base::xalloc();
const int color_ud_idx = std::ios_base::xalloc();

LogColor get_fg(long color) {
	const constexpr long initial = static_cast<long>(LogColor::initial);
	return static_cast<LogColor>((color & 0x1F) ^ initial);
}

LogColor get_bg(long color) {
	return get_fg(color >> 8);
}

long get_long_fg(LogColor color) {
	const constexpr long initial = static_cast<long>(LogColor::initial);
	return (static_cast<long>(color) & 0x1F) ^ initial;
}

long get_long_bg(LogColor color) {
	return get_long_fg(color) << 8;
}

} // be::()

///////////////////////////////////////////////////////////////////////////////
void set_stream_color_handler(std::ios_base& s, StreamColorHandler handler, void* ud) {
	s.pword(color_handler_idx) = handler;
	s.pword(color_ud_idx) = ud;
}

///////////////////////////////////////////////////////////////////////////////
bool set_stream_color(std::ios_base& s, LogColorState color) {
	if (color.fg == LogColor::current && color.bg == LogColor::current) {
		return false;
	}

	long& c = s.iword(color_idx);
	long old = c;

	switch (color.fg) {
		case LogColor::current:
			color.fg = get_fg(c);
			break;
		case LogColor::other:
			color.fg = get_bg(c);
			break;
		default:
			break;
	}

	switch (color.bg) {
		case LogColor::current:
			color.bg = get_bg(c);
			break;
		case LogColor::other:
			color.bg = get_fg(c);
			break;
		default:
			break;
	}

	c = get_long_fg(color.fg) | get_long_bg(color.bg);

	if ((c & 0x1F1F) != (old & 0x1F1F)) {
		StreamColorHandler handler = static_cast<StreamColorHandler>(s.pword(color_handler_idx));
		if (handler) {
			handler(s, color, s.pword(color_ud_idx));
		}
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
LogColorState get_stream_color(std::ios_base& s) {
	long c = s.iword(color_idx);
	return { get_fg(c), get_bg(c) };
}

///////////////////////////////////////////////////////////////////////////////
LogColorState verbosity_color(v::Verbosity level) {
   if (check_verbosity(level, v::fatal))   return setcolor(LogColor::black, LogColor::bright_red);
   if (check_verbosity(level, v::error))   return setcolor(LogColor::red, LogColor::current);
   if (check_verbosity(level, v::warning)) return setcolor(LogColor::yellow, LogColor::current);
   if (check_verbosity(level, v::notice))  return setcolor(LogColor::cyan, LogColor::current);
   if (check_verbosity(level, v::info))    return setcolor(LogColor::blue, LogColor::current);
   if (check_verbosity(level, v::verbose)) return setcolor(LogColor::gray, LogColor::current);
   if (check_verbosity(level, v::debug))   return setcolor(LogColor::green, LogColor::current);
   return setcolor(LogColor::current, LogColor::current);
}

///////////////////////////////////////////////////////////////////////////////
LogColorState setcolor(LogColor color) {
	return { color, LogColor::current };
}

///////////////////////////////////////////////////////////////////////////////
LogColorState setcolor(LogColor color, LogColor background) {
	return { color, background };
}

} // be
