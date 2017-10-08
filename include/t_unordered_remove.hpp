#pragma once
#ifndef BE_CORE_T_UNORDERED_REMOVE_HPP_
#define BE_CORE_T_UNORDERED_REMOVE_HPP_

#include "be.hpp"
#include <algorithm>

namespace be::t {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Similar to std::remove, except that non-removed elements are not
///         guaranteed to retain their original relative ordering.
///
/// \details Instead of shifting all remaining elements after the first one
///         removed, each removed element simply swaps places with the last
///         element.  This means that while both std::remove and this algorithm
///         are O(N) in the number of elements visited, std::remove is also
///         O(N) in the number of moves performed, while this algorithm is
///         O(R), where R is the number of removed elements.  This makes it
///         much faster when moves aren't cheap and only a few elements are
///         removed.
///
/// \note   While std::remove works on FowardIterators, this requires
///         BidirectionalIterators
template <typename I, typename T>
I unordered_remove(I first, I last, const T& value) {
   first = std::find(first, last, value);
   I end = last;
   if (first != last) {
      for (;;) {
         if (*first == value) {
            --end;
            if (first == end) {
               break;
            } else {
               *first = std::move(*end);
            }
         } else {
            ++first;
            if (first == end) {
               break;
            }
         }
      }
   }
   return end;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Similar to std::remove_if, except that non-removed elements are not
///         guaranteed to retain their original relative ordering.
///
/// \details Instead of shifting all remaining elements after the first one
///         removed, each removed element simply swaps places with the last
///         element.  This means that while both std::remove_if and this
///         algorithm are O(N) in the number of elements visited,
///         std::remove_if is also O(N) in the number of moves performed, while
///         this algorithm is O(R) where R is the number of removed elements.
///         This makes it much faster when moves aren't cheap and only a few
///         elements are removed.
///
///         This algorithm is also similar to std::partition, where the
///         predicate to this function would be negated to be used with
///         std::partition.  Additionally, std::partition uses iter_swap, while
///         this algorithm simply overwrites removed items with std::move().
///
/// \note   While std::remove_if works on FowardIterators, this requires
///         BidirectionalIterators
template <typename I, typename P>
I unordered_remove_if(I first, I last, P pred) {
   first = std::find_if(first, last, pred);
   I end = last;
   if (first != last) {
      for (;;) {
         if (pred(*first)) {
            --end;
            if (first == end) {
               break;
            } else {
               *first = std::move(*end);
            }
         } else {
            ++first;
            if (first == end) {
               break;
            }
         }
      }
   }
   return end;
}

} // be::t

#endif
