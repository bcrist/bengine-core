#pragma once
#ifndef BE_CORE_INLINE_EXECUTORS_HPP_
#define BE_CORE_INLINE_EXECUTORS_HPP_

#include "inline_executor.hpp"
#include "synchronized_work_queue.hpp"
#include "work_queues.hpp"

namespace be {

template <typename PriorityType = I32, typename Comparator = std::less<PriorityType>>
using PriorityInlineExecutor = InlineExecutor<SynchronizedWorkQueue<PriorityWorkQueue<PriorityType, Comparator>>>;

using SimpleInlineExecutor = InlineExecutor<SynchronizedWorkQueue<SimpleWorkQueue>>;

} // be

#endif
