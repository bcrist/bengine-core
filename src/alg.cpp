#include "pch.hpp"
#include "alg.hpp"
#include "be.hpp"

namespace be {
namespace {

///////////////////////////////////////////////////////////////////////////////
template <std::size_t N = sizeof(std::size_t)>
struct raw_hash_constants { };

///////////////////////////////////////////////////////////////////////////////
template <>
struct raw_hash_constants<sizeof(U32)> {
   static constexpr std::size_t basis = 2166136261u;
   static constexpr std::size_t mult = 16777619u;
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct raw_hash_constants<sizeof(U64)> {
   static constexpr std::size_t basis = 14695981039346656037ull;
   static constexpr std::size_t mult = 1099511628211ull;
};

} // be::()

///////////////////////////////////////////////////////////////////////////////
std::size_t std_hash_raw(const void* data, std::size_t size) {
   return std_hash_raw(raw_hash_constants<>::basis, data, size);
}

///////////////////////////////////////////////////////////////////////////////
std::size_t std_hash_raw(std::size_t in, const void* data, std::size_t size) {
   const char* cdata = static_cast<const char*>(data);
   for (std::size_t i = 0; i < size; ++i) {
      in ^= (std::size_t)cdata[i];
      in *= raw_hash_constants<>::mult;
   }
   return in;
}

} // be
