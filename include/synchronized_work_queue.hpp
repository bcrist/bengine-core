#pragma once
#ifndef BE_CORE_SYNCHRONIZED_WORK_QUEUE_HPP_
#define BE_CORE_SYNCHRONIZED_WORK_QUEUE_HPP_

#include "be.hpp"
#include "work_queue_traits.hpp"
#include "time.hpp"
#include <functional>
#include <mutex>

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename WorkQueue>
class SynchronizedWorkQueue : WorkQueue {
   using traits_type = WorkQueueTraits<WorkQueue>;
   using priority_enable_type = std::enable_if_t<traits_type::supports_priorities::value, typename traits_type::priority_type>;
public:
   using priority_type = typename traits_type::priority_type;

   SynchronizedWorkQueue() = default;

   // move operations are not thread-safe!
   SynchronizedWorkQueue(SynchronizedWorkQueue&& other)
      : WorkQueue(std::move(other)),
        pull_wait_duration_(std::move(other.pull_wait_duration_)),
        shutdown_(std::move(other.shutdown_)) { }

   // move operations are not thread-safe!
   SynchronizedWorkQueue& operator=(SynchronizedWorkQueue&& other) {
      static_cast<WorkQueue&>(*this) = std::move(other);
      pull_wait_duration_ = std::move(other.pull_wait_duration_);
      shutdown_ = std::move(other.shutdown_);
      return *this;
   }

   priority_enable_type default_priority() const {
      std::lock_guard<std::mutex> guard(mutex_);
      return static_cast<const WorkQueue*>(this)->default_priority();
   }

   void default_priority(priority_enable_type priority) {
      std::lock_guard<std::mutex> guard(mutex_);
      static_cast<WorkQueue*>(this)->default_priority(priority);
   }

   void post(std::function<void()> func) {
      std::lock_guard<std::mutex> guard(mutex_);
      static_cast<WorkQueue*>(this)->post(std::move(func));
      cv_.notify_one();
   }

   void post(std::function<void()> func, priority_enable_type priority) {
      std::lock_guard<std::mutex> guard(mutex_);
      static_cast<WorkQueue*>(this)->post(std::move(func), priority);
      cv_.notify_one();
   }

   TU pull_wait_duration() const {
      std::lock_guard<std::mutex> guard(mutex_);
      return pull_wait_duration_;
   }

   void pull_wait_duration(TU duration) {
      std::lock_guard<std::mutex> guard(mutex_);
      pull_wait_duration_ = duration;
   }

   explicit operator bool() const {
      std::lock_guard<std::mutex> guard(mutex_);
      return !shutdown_ && (bool)static_cast<const WorkQueue&>(*this);
   }

   void shutdown(bool shutdown = true) {
      std::lock_guard<std::mutex> guard(mutex_);
      shutdown_ = shutdown;
      cv_.notify_all();
   }

   std::size_t size() const {
      std::lock_guard<std::mutex> guard(mutex_);
      return static_cast<const WorkQueue*>(this)->size();
   }

   bool empty() const {
      std::lock_guard<std::mutex> guard(mutex_);
      return static_cast<const WorkQueue*>(this)->empty();
   }

   std::function<void()> poll() {
      std::lock_guard<std::mutex> guard(mutex_);
      return static_cast<WorkQueue*>(this)->poll();
   }

   std::function<void()> pull() {
      std::unique_lock<std::mutex> lock(mutex_);
      if (static_cast<const WorkQueue*>(this)->empty()) {
         cv_.wait_for(lock, pull_wait_duration_);
      }
      return static_cast<WorkQueue*>(this)->pull();
   }

private:
   mutable std::mutex mutex_;
   std::condition_variable cv_;
   TU pull_wait_duration_ = std::chrono::seconds(1);
   bool shutdown_ = false;
};

} // be

#endif
