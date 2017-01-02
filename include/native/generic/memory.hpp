#pragma once
#ifndef BE_NATIVE_GENERIC_MEMORY_HPP_
#define BE_NATIVE_GENERIC_MEMORY_HPP_

// TODO allocate larger buffers and do alignment manually.
#define BE_ALIGNED_ALLOC(alignment, size) malloc(size)
#define BE_ALIGNED_REALLOC(alignment, size) relloc(size)
#define BE_ALIGNED_FREE(ptr) free(ptr)
#define BE_VMM_ALLOC(size) malloc(size)
#define BE_VMM_FREE(ptr) free(ptr)
#define BE_VMM_PAGE_SIZE() 4096;

#define BE_ALLOCATOR_DECL

namespace be {

using C16 = char16_t;
using C32 = char32_t;

using I8 = int8_t;
using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;
using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

using F32 = float;
using F64 = double;

using S = std::string;

} // be

#endif
