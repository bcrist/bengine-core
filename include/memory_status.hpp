#pragma once
#ifndef BE_CORE_MEMORY_STATUS_HPP_
#define BE_CORE_MEMORY_STATUS_HPP_

#include "be.hpp"

namespace be {

struct MemoryStatus {
   U64 available;
   U64 total;
};

struct DeviceMemoryStatus {
   MemoryStatus physical;
   MemoryStatus vmm;
};

DeviceMemoryStatus system_memory_status();
U64 largest_available_system_memory_block();

} // be

#endif
