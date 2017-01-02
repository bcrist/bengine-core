#pragma once
#ifndef BE_CORE_SINGLE_THREAD_EXECUTORS_HPP_
#define BE_CORE_SINGLE_THREAD_EXECUTORS_HPP_

#include "single_thread_executor.hpp"
#include "synchronized_work_queue.hpp"
#include "work_queues.hpp"

namespace be {

template <typename PriorityType = I32, typename Comparator = std::less<PriorityType>>
using PrioritySingleThreadExecutor = SingleThreadExecutor<SynchronizedWorkQueue<PriorityWorkQueue<PriorityType, Comparator>>>;

using SimpleSingleThreadExecutor = SingleThreadExecutor<SynchronizedWorkQueue<SimpleWorkQueue>>;

} // be

#endif
