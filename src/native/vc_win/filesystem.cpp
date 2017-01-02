#include "pch.hpp"
#include "filesystem.hpp"

namespace be {
namespace fs {

///////////////////////////////////////////////////////////////////////////////
S relative_source_file(path source_file_path) {
   path::iterator begin = source_file_path.begin();
   for (auto it = begin, end = source_file_path.end(); it != end; ++it) {
      S gp = it->generic_string();
      std::transform(gp.begin(), gp.end(), gp.begin(), tolower);

      if (gp == "bengine" || gp == "bengine-vc") {
         begin = it;
         ++begin;
      } else if (gp == "modules" || gp == "tools" || gp == "demos" || gp == "deps" || gp == "include" || gp == "vc_include" || gp == "vc_lib_build") {
         begin = it;
         ++begin;
         break;
      }
   }

   if (begin == source_file_path.begin()) {
      return source_file_path.generic_string();
   } else {
      path rel;
      for (auto it = begin, end = source_file_path.end(); it != end; ++it) {
         rel /= *it;
      }
      return rel.generic_string();
   }
}

} // be::fs
} // be
