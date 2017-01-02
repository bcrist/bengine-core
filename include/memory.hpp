#pragma once
#ifndef BE_CORE_MEMORY_HPP_
#define BE_CORE_MEMORY_HPP_
#ifndef BE_CORE_BE_HPP_
#include "be.hpp"
#endif
#include BE_NATIVE_CORE(memory.hpp)
#include <memory>

namespace be {
namespace immovable_detail {  // protection from unintended ADL

class Immovable {
protected:
   constexpr Immovable() = default;
   ~Immovable() = default;
   Immovable(const Immovable&) = delete;
   Immovable& operator=(const Immovable&) = delete;
};

class Movable {
protected:
   constexpr Movable() = default;
   ~Movable() = default;
   Movable(const Movable&) = delete;
   Movable(Movable&&) = default;
   Movable& operator=(const Movable&) = delete;
   Movable& operator=(Movable&&) = default;
};

} // be::immovable_detail

using Immovable = immovable_detail::Immovable;
using Movable = immovable_detail::Movable;

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

using UC = unsigned char;
using SC = signed char;

using UI = unsigned int;
using SI = signed int;

template <typename T>
using SizeTag = Tag<std::size_t, sizeof(T)>;

template <typename T>
using AlignTag = Tag<std::size_t, alignof(T)>;

template <typename T, std::size_t ExpectedSize, std::size_t ActualSize = sizeof(T)>
struct SizeIs : False {};
template <typename T, std::size_t Size>
struct SizeIs<T, Size, Size> : True {};

namespace detail {

template <std::size_t N>
struct IntegerType { };
template <> struct IntegerType<sizeof(I8)> { using type = I8; };
template <> struct IntegerType<sizeof(I16)> { using type = I16; };
template <> struct IntegerType<sizeof(I32)> { using type = I32; };
template <> struct IntegerType<sizeof(I64)> { using type = I64; };

template <std::size_t N>
struct UnsignedType { };
template <> struct UnsignedType<sizeof(U8)> { using type = U8; };
template <> struct UnsignedType<sizeof(U16)> { using type = U16; };
template <> struct UnsignedType<sizeof(U32)> { using type = U32; };
template <> struct UnsignedType<sizeof(U64)> { using type = U64; };

} // be::detail

template <std::size_t N>
using Integer = typename detail::IntegerType<N>::type;
template <std::size_t N>
using Unsigned = typename detail::UnsignedType<N>::type;

template <typename T>
S type_name() {
   S name = typeid(T).name();
   S test = "class ";
   if (std::equal(test.begin(), test.end(), name.begin())) {
      name.erase(0, test.length());
   } else {
      test = "struct ";
      if (std::equal(test.begin(), test.end(), name.begin())) {
         name.erase(0, test.length());
      }
   }
   return name;
}

//namespace detail {
//
//// TODO replace with a3 allocators
//template <typename T, std::size_t N, std::size_t A = alignof(T)> class NodeAllocator;
//template <typename T, std::size_t N, std::size_t A = alignof(T)> class ThreadNodeAllocator;
//template <typename T, std::size_t N, std::size_t A = alignof(T)> class SharedNodeAllocator;
//
//} // be::detail
//
//// Don't forget to include "be/detail/node_allocator.h", "be/detail/thread_node_allocator.h" or "be/detail/shared_node_allocator.h" when using these!
//template <typename T, std::size_t A = 0> using NodeAllocator = detail::NodeAllocator<T, 64, A>;
//template <typename T, std::size_t A = 0> using NodeAllocatorLarge = detail::NodeAllocator<T, 1024, A>;
//template <typename T, std::size_t A = 0> using ThreadNodeAllocator = detail::ThreadNodeAllocator<T, 64, A>;
//template <typename T, std::size_t A = 0> using ThreadNodeAllocatorLarge = detail::ThreadNodeAllocator<T, 1024, A>;
//template <typename T, std::size_t A = 0> using SharedNodeAllocator = detail::SharedNodeAllocator<T, 64, A>;
//template <typename T, std::size_t A = 0> using SharedNodeAllocatorLarge = detail::SharedNodeAllocator<T, 1024, A>;
//
//#define BE_ALLOC_SET(Type, Allocator)                       std::set<Type, std::less<Type>, Allocator<Type>>
//#define BE_ALLOC_MAP(KeyType, ValueType, Allocator)         std::map<KeyType, ValueType, std::less<KeyType>, Allocator<std::pair<KeyType const, ValueType>>>
//#define BE_ALLOC_MULTISET(Type, Allocator)                  std::multiset<Type, std::less<Type>, Allocator<Type>>
//#define BE_ALLOC_MULTIMAP(KeyType, ValueType, Allocator)    std::multimap<KeyType, ValueType, std::less<KeyType>, Allocator<std::pair<KeyType const, ValueType>>>
//
//#define BE_ALLOC_UO_SET(Type, Allocator)                    std::unordered_set<Type, std::hash<Type>, std::equal_to<Type>, Allocator<Type>>
//#define BE_ALLOC_UO_MAP(KeyType, ValueType, Allocator)      std::unordered_map<KeyType, ValueType, std::hash<KeyType>, std::equal_to<KeyType>, Allocator<std::pair<KeyType const, ValueType>>>
//#define BE_ALLOC_UO_MULTISET(Type, Allocator)               std::unordered_multiset<Type, std::hash<Type>, std::equal_to<Type>, Allocator<Type>>
//#define BE_ALLOC_UO_MULTIMAP(KeyType, ValueType, Allocator) std::unordered_multimap<KeyType, ValueType, std::hash<KeyType>, std::equal_to<KeyType>, Allocator<std::pair<KeyType const, ValueType>>>

} // be

#endif
