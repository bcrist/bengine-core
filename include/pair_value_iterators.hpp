#pragma once
#ifndef BE_CORE_PAIR_VALUE_ITERATORS_HPP_
#define BE_CORE_PAIR_VALUE_ITERATORS_HPP_

#include <boost/iterator/iterator_adaptor.hpp>

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename I>
class PairFirstIterator : boost::iterator_adaptor<PairFirstIterator<I>, I, typename I::value_type::second_type> {
   friend class boost::iterator_core_access;
public:
   PairFirstIterator() { }
   explicit PairFirstIterator(I const& iter)
      : PairFirstIterator::iterator_adaptor_(iter)
   { }

private:
   typename iterator_adaptor_::reference dereference() const {
      return base_reference()->first;
   }
};

///////////////////////////////////////////////////////////////////////////////
template <typename I>
class PairSecondIterator : boost::iterator_adaptor<PairSecondIterator<I>, I, typename I::value_type::second_type> {
   friend class boost::iterator_core_access;
public:
   PairSecondIterator() { }
   explicit PairSecondIterator(I const& iter)
      : PairSecondIterator::iterator_adaptor_(iter)
   { }

private:
   typename iterator_adaptor_::reference dereference() const {
      return base_reference()->second;
   }
};

} // be

#endif
