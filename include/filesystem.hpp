#pragma once
#ifndef BE_CORE_FILESYSTEM_HPP_
#define BE_CORE_FILESYSTEM_HPP_

#include "t_is_container.hpp"
#include <filesystem>

namespace be {
namespace fs {

using namespace std::experimental::filesystem;

// try to turn absolute paths to source files into relative paths - native implementation
S relative_source_file(path source_file_path);

} // be::fs

using Path = fs::path;

namespace t {

template <>
struct IsContainer<Path> : False { };

} // be::t
} // be

#endif
