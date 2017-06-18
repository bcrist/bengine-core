#include <be/core/native.hpp>
#ifdef BE_NATIVE_VC_WIN

#include <be/core/memory.hpp>

#include "native/vc_win/vc_win_win32.hpp"

namespace be {
namespace detail {

namespace {

::SYSTEM_INFO system_info() {
   ::SYSTEM_INFO info;
   ::GetSystemInfo(&info);
   return info;
}

} // be::detail::()

std::size_t mem_page_size() {
   static std::size_t pageSize = system_info().dwPageSize;
   return pageSize;
}

std::size_t virtual_alloc_granularity() {
   static std::size_t granularity = system_info().dwAllocationGranularity;
   return granularity;
}

void* virtual_alloc(std::size_t size) {
   return ::VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
}

bool virtual_free(void* ptr) {
   return ::VirtualFree(ptr, 0, MEM_RELEASE) != 0;
}

} // be::detail
} // be

#endif
