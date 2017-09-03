/// \brief  Highly influenced by Andrei Alexandrescu's policy-based allocators:
///         https://www.youtube.com/watch?v=LIb3L4vKZ7U
///         https://github.com/D-Programming-Language/phobos/tree/master/std/experimental/allocator

#pragma once
#ifndef BE_CORE_ALLOCATORS_HPP_
#define BE_CORE_ALLOCATORS_HPP_

// Low-level allocators; not super useful alone
#include "a3_null_alloc.hpp"
#include "a3_malloc_alloc.hpp"
#include "a3_aligned_malloc_alloc.hpp"
#include "a3_vmm_alloc.hpp"
#include "a3_static_alloc.hpp"
#include "a3_buf_alloc.hpp"
//#include "a3_pmr_alloc.hpp" // C++17 polymorphic memory resource allocator

// Allocators which suballocate from large chunks obtained from another allocator
#include "a3_arena_alloc.hpp"
#include "a3_freelist_alloc.hpp"
#include "a3_chunklist_alloc.hpp"
#include "a3_bitmapped_chunk_alloc.hpp"

// Adapter allocators
#include "a3_cascading_alloc.hpp"            // allocates from one of a growable list of identically-typed allocators.  If the allocator implements getOwner deallocation is very efficient; otherwise it involves a linear search through the list of allocators in MRU order.  If allocation from the current head allocator fails,
#include "a3_fallback_alloc.hpp"             // allocates to another allocator if allocation in the primary allocator fails
#include "a3_size_discriminator_alloc.hpp"   // delegates to one allocator if requested size is <= some value, or another one otherwise.
#include "a3_linear_size_partitioner_alloc.hpp"  // equivalent to a series of SizeDiscriminatorAllocs where each manages an equal part of the range of supported sizes
#include "a3_exponential_size_partitioner_alloc.hpp" // same as LinearSizePartitionerAlloc but partition sizes grow exponentially instead of linearly
#include "a3_alignment_discriminator_alloc.hpp" // delegates to one allocator if requested alignment is <= some value, or another one otherwise.
#include "a3_predicate_discriminator_alloc.hpp" // delegates to one of two allocators based on the result of a predicate functor
#include "a3_affix_alloc.hpp"             // Adds bookkeeping space before and/or after each allocation
#include "a3_counting_alloc.hpp"          // implements BaseAlloc::count()
#include "a3_perf_stat_counter_alloc.hpp" // increment a PerfCounter when allocations occur and decrement it when deallocations occur
#include "a3_perf_stat_event_alloc.hpp"   // record a PerfEvent when any allocations/deallocations occur
#include "a3_logging_alloc.hpp"           // log when any allocations/deallocations/reallocations occur
#include "a3_debug_wrapper_alloc.hpp"     // place a buffer on each side of each allocated object and check for illegal modifications on dealloc

// C++17 PMR wrapper (opposite of PmrAlloc)
//#include "a3_alloc_pmr.hpp"

// STL allocator wrapper
#include "a3_non_owned.hpp"
#include "a3_owned.hpp"
#include "a3_unsynch.hpp"
#include "a3_thread.hpp"
#include "a3_shared.hpp"
#include "a3_stl_alloc.hpp"

namespace be {
namespace a3 {

using AutoMallocAlloc = AlignmentDiscriminatorAlloc<MallocAlloc::alignment, MallocAlloc, AlignedMallocAlloc>;

} // namespace be::a3
} // namespace be

#endif
