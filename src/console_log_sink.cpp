#include "pch.hpp"
#include "console_log_sink.hpp"
#include "time.hpp"
#include "string_interpolation.hpp"
#include "filesystem.hpp"
#include <be/core/alg.hpp>
#include <map>
#include <iomanip>

namespace be {
namespace {

///////////////////////////////////////////////////////////////////////////////
S time_str(const LogRecord& rec) {
   auto decomposed = decompose_ts(rec.timestamp);

   const int buf_size = 27;
   char buf[buf_size] { };

   tm local_;
   tm* local = time_t_to_local_tm(decomposed.first, local_);
   strftime(buf, buf_size, " %Y-%m-%d  %H:%M:%S", local);

   char* i = buf;
   char* end = buf + buf_size;

   while (*i) ++i; // find first 0 char

   snprintf(i, end - i, ".%0.3u ", decomposed.second / 1000);

//#ifdef BE_DEBUG
   S retval(buf);
   retval.append(1, '+');
   U64 diff = (ts_now() - rec.timestamp).count();
   retval.append(std::to_string(diff / 1000));
   retval.append(1, ' ');
   return retval;
//#else
//   return buf;
//#endif
}

///////////////////////////////////////////////////////////////////////////////
S source_str(const LogRecord& rec) {
   if (!rec.source) {

      S file;
      if (rec.file) {
         file = fs::relative_source_file(rec.file);
      } else {
         file = "?";
      }

      std::ostringstream oss;
      oss << file << " : " << rec.line;
      return oss.str();
   }
   return rec.source;
}

///////////////////////////////////////////////////////////////////////////////
void output_log_string(std::ostream& os, LogColorState initial, LogColorState color, const S& text, std::size_t extraindent) {
   if (color.fg == LogColor::initial) {
      color.fg = initial.fg;
   }
   if (color.bg == LogColor::initial) {
      color.bg = initial.bg;
   }
   os << color;
   auto begin = text.begin(), end = text.end(), it = std::find(begin, end, '\n');
   if (it != end) {
      do {
         os << S(begin, it) << nl << S(extraindent, ' ');
         begin = it + 1;
         it = std::find(begin, end, '\n');
      } while (it != end);
      os << S(begin, end);
   } else {
      os << text;
   }
}

} // be::()

///////////////////////////////////////////////////////////////////////////////
ConsoleLogSink::ConsoleLogSink()
   : output_stream_(&std::cout),
     error_stream_(&std::cerr)
{ }

///////////////////////////////////////////////////////////////////////////////
ConsoleLogSink::ConsoleLogSink(std::ostream& output, std::ostream& error)
   : output_stream_(&output),
     error_stream_(&error)
{ }

///////////////////////////////////////////////////////////////////////////////
ConsoleLogSink::ConsoleLogSink(const ConsoleLogSink& other)
   : output_stream_(other.output_stream_),
     error_stream_(other.error_stream_)
{ }

///////////////////////////////////////////////////////////////////////////////
ConsoleLogSink& ConsoleLogSink::operator=(ConsoleLogSink other) {
   output_stream_ = std::move(other.output_stream_);
   error_stream_ = std::move(other.error_stream_);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
void ConsoleLogSink::operator()(const LogRecord& rec, Log& log) {
   std::lock_guard<std::mutex> guard(mutex_);

   std::ostream* os;
   if (rec.verbosity == v::error || rec.verbosity == v::fatal) {
      os = error_stream_;
   } else {
      os = output_stream_;
   }

   auto originalConfig = get_ostream_config(*os);
   *os << std::dec << std::setfill(' ') << std::boolalpha << std::defaultfloat << std::left;
   
   if (rec.short_form) {
      last_rec_was_short_form_ = true;

      *os << setcolor(LogColor::black, LogColor::dark_gray) << time_str(rec)
         << setcolor(LogColor::dark_gray, originalConfig.color.bg) << ' '
         << verbosity_color(rec.verbosity) << verbosity_name(rec.verbosity)
         << setcolor(LogColor::dark_gray, originalConfig.color.bg) << ':'
         << color::reset << ' ' << indent;
   } else {
      if (last_rec_was_short_form_) {
         *os << nl;
         last_rec_was_short_form_ = false;
      }

      *os << setcolor(LogColor::black, LogColor::dark_gray) << time_str(rec)
         << setcolor(LogColor::dark_gray, originalConfig.color.bg) << ' ' << source_str(rec) << indent << nl
         << verbosity_color(rec.verbosity) << verbosity_name(rec.verbosity)
         << setcolor(LogColor::dark_gray, originalConfig.color.bg) << ": " << indent;
   }

   // first pass through attribs: get map for doing interpolations, calculate max attrib length
   std::size_t longest_attr_name = 0;
   std::map<Id, const LogAttribute*> attrib_id_map;
   for (const LogAttribute& attr : rec.attributes) {
      Id attrId = attr.id;
      if ((U64)attrId) {
         // find attrib name, add to attrib map, etc.
         attrib_id_map[attrId] = &attr;

         // skip attributes that don't pass verbosity check
         if (!check_verbosity(attr.verbosity, log.verbosity_mask())) {
            continue;
         }

         S name = get_id_name(attrId);
         if (!name.empty()) {
            longest_attr_name = max(longest_attr_name, name.length());
         } else {
            name = get_canonical_id_string(attrId);
            longest_attr_name = max(longest_attr_name, name.length());
         }
      }
   }

   // second pass: print attribs, interpolating strings
   for (const LogAttribute& attr : rec.attributes) {
      // skip attributes that don't pass verbosity check
      if (!check_verbosity(attr.verbosity, log.verbosity_mask())) {
         continue;
      }

      Id attrId = attr.id;

      LogColorState attr_initial_color;

      // attribute name
      if ((U64)attrId) {
         *os << setcolor(LogColor::gray, LogColor::current) << std::right << std::setw(longest_attr_name);
         S name = get_id_name(attrId);
         if (!name.empty()) {
            *os << name;
         } else {
            *os << attrId;
         }
         attr_initial_color.fg = LogColor::gray;
         attr_initial_color.bg = originalConfig.color.bg;
         *os << setcolor(LogColor::dark_gray, originalConfig.color.bg) << ": " << attr_initial_color;

      } else {
         attr_initial_color.fg = LogColor::yellow;
         attr_initial_color.bg = originalConfig.color.bg;
         *os << attr_initial_color;
      }

      // attribute value
      for (const LogString& str : attr.contents) {
         if (str.text.find('$') != S::npos) {
            // interpolate string
            interpolate_string_ex(str.text, [&](S text) {
               output_log_string(*os, attr_initial_color, str.color, text, longest_attr_name + 2);
            }, [&](S interpolant) {
               Id interpolant_id = Id(interpolant);
               auto it = attrib_id_map.find(interpolant_id);
               if (it != attrib_id_map.end()) {
                  for (const LogString& str : (it->second)->contents) {
                     output_log_string(*os, attr_initial_color, str.color, str.text, longest_attr_name + 2);
                  }
               } else {
                  *os << '?';
               }
            });
         } else {
            output_log_string(*os, attr_initial_color, str.color, str.text, longest_attr_name + 2);
         }
      }

      if (rec.short_form) {
         S source = source_str(rec);
         *os << setcolor(LogColor::dark_gray, originalConfig.color.bg);
         if (!source.empty()) {
            *os << " (" << source_str(rec) << ')';
         }
         break;
      } else {
         *os << setcolor(LogColor::dark_gray, originalConfig.color.bg) << nl;
      }
   }

   *os << originalConfig << nl;
}

} // be