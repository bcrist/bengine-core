#include "pch.hpp"
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

// http://stackoverflow.com/a/4679992/1571944
U64 largest_available_system_memory_block() {
   ::MEMORY_BASIC_INFORMATION mbi;
   ::SIZE_T start = 0;
   bool recording = false;
   ::SIZE_T freeStart = 0, largestFreeStart = 0;
   U64 free = 0, largestFree = 0;

   for (;;) {
      ::SIZE_T s = ::VirtualQuery(reinterpret_cast<::LPCVOID>(start), &mbi, sizeof(mbi));
      if (s != sizeof(mbi)) {
         break;
      }

      if (mbi.State == MEM_FREE) {
         if (!recording) freeStart = start;

         free += static_cast<U64>(mbi.RegionSize);
         recording = true;
      } else {
         if (recording) {
            if (free > largestFree) {
               largestFree = free;
               largestFreeStart = freeStart;
            }
         }
         free = 0u;
         recording = false;
      }
      start += mbi.RegionSize;
   }

   return largestFree;
}

DeviceMemoryStatus system_memory_status() {
   DeviceMemoryStatus status;

   ::MEMORYSTATUSEX msx;
   msx.dwLength = sizeof(msx);
   ::GlobalMemoryStatusEx(&msx);

   status.physical.available = msx.ullAvailPhys;
   status.physical.total = msx.ullTotalPhys;
   status.vmm.available = msx.ullAvailVirtual;
   status.vmm.total = msx.ullTotalVirtual;

   return status;
}

} // be

#endif
