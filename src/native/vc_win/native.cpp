#include <be/core/native.hpp>
#ifdef BE_NATIVE_VC_WIN

#include <boost/locale/localization_backend.hpp>
#include <boost/locale/generator.hpp>

#include "native/vc_win/vc_win_win32.hpp"
#include <codecvt>
#include <sstream>

namespace be {

#pragma warning (push)
#pragma warning (disable : 4996)
///////////////////////////////////////////////////////////////////////////////
S os_version() {
   ::OSVERSIONINFO version_info;
   version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   if (::GetVersionEx(&version_info)) {
      std::ostringstream oss;
      oss << version_info.dwMajorVersion << '.' << version_info.dwMinorVersion << " Windows";
      return oss.str();
   }
   return "Unknown";
}

///////////////////////////////////////////////////////////////////////////////
std::locale default_locale() {
   auto locmgr = boost::locale::localization_backend_manager::global();
   locmgr.select("std");
   boost::locale::localization_backend_manager::global(locmgr);
   std::locale base_locale = boost::locale::generator()("en_US.UTF-8");
   return std::locale(base_locale, new std::codecvt_utf8_utf16<wchar_t>);
}
#pragma warning (pop)

} // be

#endif
