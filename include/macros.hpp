#pragma once
#ifndef BE_CORE_MACROS_HPP_
#define BE_CORE_MACROS_HPP_

#define BE_STRINGIFY_HELPER(x) #x
#define BE_STRINGIFY(x) BE_STRINGIFY_HELPER(x)

#define BE_VERSION_MAJOR 0
#define BE_VERSION_MINOR 1
#define BE_VERSION_REV 0
#define BE_VERSION (BE_VERSION_MAJOR * 100000 + BE_VERSION_MINOR * 1000 + BE_VERSION_REV)
#define BE_VERSION_STRING "Bengine " BE_STRINGIFY(BE_VERSION_MAJOR) "." BE_STRINGIFY(BE_VERSION_MINOR) "." BE_STRINGIFY(BE_VERSION_REV)
#define BE_COPYRIGHT BE_VERSION_STRING "  Copyright (C) 2012-2016 B. Crist"
#define BE_LICENSE "Distributed under the MIT license as part of Bengine"

#define BE_CONCAT_HELPER(x, y) x##y
#define BE_CONCAT(x, y) BE_CONCAT_HELPER(x, y)

#define BE_IGNORE(x) (void)(x)
#define BE_IGNORE2(x,y) BE_IGNORE(x),BE_IGNORE(y)
#define BE_IGNORE3(x,y,z) BE_IGNORE(x),BE_IGNORE(y),BE_IGNORE(z)
#define BE_IGNORE4(x,y,z,w) BE_IGNORE(x),BE_IGNORE(y),BE_IGNORE(z),BE_IGNORE(w)

#if defined(BE_DEBUG) || defined(DEBUG) || defined(_DEBUG)
#  ifndef BE_DEBUG
#     define BE_DEBUG
#  endif
#  ifndef DEBUG
#     define DEBUG
#  endif
#  ifdef NDEBUG
#     undef NDEBUG
#  endif
#else
#  ifndef NDEBUG
#     define NDEBUG
#  endif
#endif

#endif
