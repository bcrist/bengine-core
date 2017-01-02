#pragma once
#ifndef BE_CORE_THREAD_POOL_EXECUTORS_HPP_
#define BE_CORE_THREAD_POOL_EXECUTORS_HPP_

#include "thread_pool_executor.hpp"
#include "synchronized_work_queue.hpp"
#include "work_queues.hpp"

namespace be {

template <typename PriorityType = I32, typename Comparator = std::less<PriorityType>>
using PriorityThreadPoolExecutor = ThreadPoolExecutor<SynchronizedWorkQueue<PriorityWorkQueue<PriorityType, Comparator>>>;

using SimpleThreadPoolExecutor = ThreadPoolExecutor<SynchronizedWorkQueue<SimpleWorkQueue>>;

} // be

#endif
