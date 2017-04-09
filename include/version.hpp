#pragma once
#ifndef BE_CORE_VERSION_HPP_
#define BE_CORE_VERSION_HPP_

#include "macros.hpp"

#define BE_CORE_VERSION_MAJOR 0
#define BE_CORE_VERSION_MINOR 1
#define BE_CORE_VERSION_REV 0

/*!! include('common/version', 'BE_CORE', 'bengine') !! 6 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
#define BE_CORE_VERSION (BE_CORE_VERSION_MAJOR * 100000 + BE_CORE_VERSION_MINOR * 1000 + BE_CORE_VERSION_REV)
#define BE_CORE_VERSION_STRING "bengine " BE_STRINGIFY(BE_CORE_VERSION_MAJOR) "." BE_STRINGIFY(BE_CORE_VERSION_MINOR) "." BE_STRINGIFY(BE_CORE_VERSION_REV)

/* ######################### END OF GENERATED CODE ######################### */

#define BE_COPYRIGHT "Copyright (C) 2012-2017 B. Crist"
#define BE_LICENSE "Distributed under the MIT license as part of Bengine"
#define BE_URL "https://github.com/magicmoremagic/bengine"

#endif
