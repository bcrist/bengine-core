#pragma once
#ifndef BE_CORE_FILESYSTEM_HPP_
#define BE_CORE_FILESYSTEM_HPP_

#include "t_is_container.hpp"
#include BE_NATIVE_CORE(filesystem.hpp)

namespace be {

using Path = fs::path;

S relative_source_file(Path source_file_path);

namespace t {

template <>
struct IsContainer<Path> : False { };

} // be::t
} // be

#endif
