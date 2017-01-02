#ifndef BE_NO_AUTOLINK

#ifndef BE_AUTOLINK_LIB
#   pragma message("autolink.hpp was included without first defining BE_AUTOLINK_LIB!")
#elif !defined(BE_STATIC_LIB)

#   ifndef BE_CORE_NATIVE_HPP_
#      include "native.hpp"
#   endif

#   ifndef BE_AUTOLINK_DEBUG_SUFFIX
#      define BE_AUTOLINK_DEBUG_SUFFIX "-debug"
#   endif

#   ifndef BE_AUTOLINK_RELEASE_SUFFIX
#      define BE_AUTOLINK_RELEASE_SUFFIX
#   endif

#   ifndef BE_AUTOLINK_SUFFIX
#      ifdef BE_DEBUG
#         define BE_AUTOLINK_SUFFIX BE_AUTOLINK_DEBUG_SUFFIX
#      else
#         define BE_AUTOLINK_SUFFIX BE_AUTOLINK_RELEASE_SUFFIX
#      endif
#   endif

#   ifdef BE_NATIVE_VC_WIN
#      pragma comment(lib, BE_AUTOLINK_LIB BE_AUTOLINK_SUFFIX ".lib")
#      ifdef BE_LIB_DIAGNOSTIC
#         pragma message("Linking to lib file: " BE_AUTOLINK_LIB BE_AUTOLINK_SUFFIX ".lib")
#      endif
#   endif

#endif

#ifdef BE_AUTOLINK_LIB
#   undef BE_AUTOLINK_LIB
#endif

#ifdef BE_AUTOLINK_SUFFIX
#   undef BE_AUTOLINK_SUFFIX
#endif

#ifdef BE_AUTOLINK_DEBUG_SUFFIX
#   undef BE_AUTOLINK_DEBUG_SUFFIX
#endif

#ifdef BE_AUTOLINK_RELEASE_SUFFIX
#   undef BE_AUTOLINK_RELEASE_SUFFIX
#endif

#endif
