#pragma once
#ifndef BE_CORE_ZLIB_HPP_
#define BE_CORE_ZLIB_HPP_

#define BE_AUTOLINK_LIB "zlib-static"
#include "autolink.hpp"

#include "buf.hpp"

namespace be {

Buf<UC> deflate_text(const S& text, bool encode_length = true, I8 level = 9);
Buf<UC> deflate_blob(const Buf<const UC>& data, bool encode_length = true, I8 level = 9);

S inflate_text(const Buf<const UC>& compressed);
S inflate_text(const Buf<const UC>& compressed, std::size_t uncomressed_length);
Buf<UC> inflate_blob(const Buf<const UC>& compressed);
Buf<UC> inflate_blob(const Buf<const UC>& compressed, std::size_t uncomressed_length);

} // be

#endif
