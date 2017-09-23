#pragma once
#ifndef BE_CORE_NATIVE_VC_WIN_MEMORY_HPP_
#define BE_CORE_NATIVE_VC_WIN_MEMORY_HPP_

#include "vc_win.hpp"

#ifdef BE_DEBUG
#  ifndef BE_NO_LEAKCHECK
#    include <vld.h>
#  endif
#  define BE_ALIGNED_ALLOC(alignment, size) _aligned_malloc_dbg(size, alignment, __FILE__, __LINE__)
#  define BE_ALIGNED_REALLOC(alignment, ptr, new_size) _aligned_realloc_dbg(ptr, new_size, alignment, __FILE__, __LINE__)
#  define BE_ALIGNED_FREE(ptr) _aligned_free_dbg(ptr)
#else
#  define BE_ALIGNED_ALLOC(alignment, size) _aligned_malloc(size, alignment)
#  define BE_ALIGNED_REALLOC(alignment, ptr, new_size) _aligned_realloc(ptr, new_size, alignment)
#  define BE_ALIGNED_FREE(ptr) _aligned_free(ptr)
#endif

#define BE_VMM_ALLOC(size) ::be::detail::virtual_alloc(size)
#define BE_VMM_FREE(ptr) ::be::detail::virtual_free(ptr)
#define BE_VMM_ALLOC_GRANULARITY() ::be::detail::virtual_alloc_granularity();
#define BE_VMM_PAGE_SIZE() ::be::detail::mem_page_size();

#if _MSC_VER >= 1900 && !defined(__EDG__)
#  define BE_ALLOCATOR_DECL __declspec(allocator)
#else
#  define BE_ALLOCATOR_DECL
#endif

namespace be {
namespace detail {

std::size_t mem_page_size();
std::size_t virtual_alloc_granularity();
void* virtual_alloc(std::size_t size);
bool virtual_free(void* ptr);

} // be::detail

using C16 = char16_t;
using C32 = char32_t;

using I8 = signed __int8;
using I16 = signed __int16;
using I32 = signed __int32;
using I64 = signed __int64;
using U8 = unsigned __int8;
using U16 = unsigned __int16;
using U32 = unsigned __int32;
using U64 = unsigned __int64;

using F32 = float;
using F64 = double;

using S = std::string;
using SV = std::string_view;

} // be

#endif
