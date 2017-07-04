#pragma once
#ifndef BE_CONTAINER_TYPES_HPP_
#define BE_CONTAINER_TYPES_HPP_

#include "t_select.hpp"
#include "t_is_simple_alloc.hpp"
#include "wrapped_allocator.hpp"

namespace be::t {

template <typename T, typename S, typename D, typename P, typename CP, typename R, typename CR>
struct IteratorTypes {
   using value_type = T;
   using size_type = S;
   using difference_type = D;
   using pointer = P;
   using const_pointer = CP;
   using reference = R;
   using const_reference = CR;
};

template<typename T>
struct SimpleIteratorTypes {
   using value_type = T;
   using size_type = std::size_t;
   using difference_type = std::ptrdiff_t;
   using pointer = value_type*;
   using const_pointer = const value_type*;
   using reference = value_type&;
   using const_reference = const value_type&;
};

template<typename T, typename A>
struct ContainerTypes {
   using base_types = ContainerTypes<T, A>;
   using raw_alloc = A;
   using wrapped_alloc = WrappedAllocator<raw_alloc>;
   using allocator = typename wrapped_alloc::template rebind<T>::other;

   using types = typename t::Select<t::IsSimpleAlloc<allocator>::value,
      SimpleIteratorTypes<typename allocator::value_type>,
      IteratorTypes<typename allocator::value_type, typename allocator::size_type, typename allocator::difference_type,
         typename allocator::pointer, typename allocator::const_pointer,
         typename allocator::reference, typename allocator::const_reference>
      >::type;
};

} // be::t

#endif
