#pragma once
#ifndef BE_CORE_INLINE_EXECUTOR_HPP_
#define BE_CORE_INLINE_EXECUTOR_HPP_

#include "be.hpp"
#include <functional>
#include <limits>

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename WorkQueue>
class InlineExecutor : public WorkQueue {
public:
   bool run(std::size_t num = std::numeric_limits<std::size_t>::max()) {
      bool ran_one_or_more = false;
      if (num == std::numeric_limits<std::size_t>::max()) {
         num = this->size();
      }

      while (num > 0 && run_one()) {
         ran_one_or_more = true;
         --num;
      }

      return ran_one_or_more;
   }

   bool run_one() {
      std::function<void()> func = static_cast<WorkQueue*>(this)->poll();
      if (func) {
         func();
         return true;
      }
      return false;
   }

   std::function<void()> pull() = delete;
   std::function<void()> poll() = delete;

};

} // be

#endif
