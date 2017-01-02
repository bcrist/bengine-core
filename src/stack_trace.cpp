#include "pch.hpp"
#include "stack_trace.hpp"
#include "console.hpp"
#include "filesystem.hpp"
#include <ostream>
#include <iomanip>

namespace be {

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const StackTrace& trace) {
   auto osc = get_ostream_config(os);
   
   for (auto frame_ptr : trace.frames) {
      auto info = get_stack_frame_symbol_info(frame_ptr);
      
      os << nl << color::dark_gray << std::right << std::hex << std::setw(16) << std::setfill('0') << info.address << ' ';
      
      if (!info.module.empty()) {
         os << color::gray << info.module << color::dark_gray << '!';
      }

      os << color::purple << info.symbol;

      if (!info.file.empty() || info.line != 0) {
         os << color::dark_gray << " " << fs::relative_source_file(info.file) << " : " << std::dec << info.line;

         if (info.line_displacement > 0) {
            os << " +" << info.line_displacement << 'B';
         } else if (info.line_displacement < 0) {
            os << " -" << -info.line_displacement << 'B';
         }
      }
   }

   return os << osc;
}

} // be
