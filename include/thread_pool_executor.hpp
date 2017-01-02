#pragma once
#ifndef BE_CORE_THREAD_POOL_EXECUTOR_HPP_
#define BE_CORE_THREAD_POOL_EXECUTOR_HPP_

#include "be.hpp"
#include "work_queue_traits.hpp"
#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <cassert>

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename WorkQueue>
class ThreadPoolExecutor : public WorkQueue {
   using type = ThreadPoolExecutor<WorkQueue>;
   using traits_type = WorkQueueTraits<WorkQueue>;
public:
   ThreadPoolExecutor() = default;
   ~ThreadPoolExecutor() {
      shutdown();
   }

   explicit operator bool() const {
      return threads() > 0 && (bool)static_cast<const WorkQueue&>(*this);
   }

   std::size_t active_threads() const {
      std::lock_guard<std::mutex> guard(mutex_);
      return threads_.size() - dead_threads_.size();
   }

   std::size_t threads() const {
      std::lock_guard<std::mutex> guard(mutex_);
      return n_threads_;
   }

   void shutdown() {
      threads(0);
   }

   void threads(std::size_t new_size) {
      BE_ASSERT_MAIN_THREAD();

      std::unique_lock<std::mutex> lock(mutex_);
      if (n_threads_locked_ || n_threads_ == new_size) {
         // threads(..) is not reentrant.
         return;
      }

      std::size_t old_size = n_threads_;
      n_threads_ = new_size;
      n_threads_locked_ = true;

      update_queue_shutdown_state_(typename traits_type::has_shutdown(), typename traits_type::has_unshutdown());

      if (new_size == 0) {
         join_all_(std::move(lock));
      } else if (new_size > old_size) {
         expand_(std::move(lock));
      } else { // new_size < old_size
         shrink_(std::move(lock));
      }
   }

   std::function<void()> pull() = delete;
   std::function<void()> poll() = delete;

private:
   void update_queue_shutdown_state_(False, False) { }
   void update_queue_shutdown_state_(True, False) {
      if (n_threads_ == 0) {
         static_cast<WorkQueue*>(this)->shutdown();
      }
   }
   void update_queue_shutdown_state_(False, True) {
      if (n_threads_ == 0) {
         static_cast<WorkQueue*>(this)->shutdown(true);
      } else {
         static_cast<WorkQueue*>(this)->shutdown(false);
      }
   }
   void update_queue_shutdown_state_(True, True) {
      if (n_threads_ == 0) {
         static_cast<WorkQueue*>(this)->shutdown();
      } else {
         static_cast<WorkQueue*>(this)->shutdown(false);
      }
   }

   void join_all_(std::unique_lock<std::mutex> lock) {
      lock.unlock();
      for (std::thread& thread : threads_) {
         if (thread.joinable()) {
            thread.join();
         }
      }
      lock.lock();
      assert(threads_.size() == dead_threads_.size());
      threads_.clear();
      dead_threads_.clear();
      n_threads_locked_ = false;
   }
   void shrink_(std::unique_lock<std::mutex> lock) {
      do {
         lock.unlock();
         std::this_thread::yield();
         lock.lock();
      } while (threads_.size() - dead_threads_.size() > n_threads_);
      expand_(std::move(lock));
   }
   void expand_(std::unique_lock<std::mutex> lock) {
      for (std::thread::id tid : dead_threads_) {
         threads_.erase(std::remove_if(threads_.begin(), threads_.end(), [tid](std::thread& t) {
            if (t.get_id() == tid) {
               if (t.joinable()) {
                  t.join();
               }
               return true;
            }
            return false;
         }), threads_.end());
      }
      dead_threads_.clear();

      while (threads_.size() < n_threads_) {
         threads_.push_back(create_thread_());
      }
      n_threads_locked_ = false;
   }

   bool should_thread_continue_() {
      std::lock_guard<std::mutex> guard(mutex_);
      if (threads_.size() - dead_threads_.size() > n_threads_) {
         // thread pool is too big; I will volunteer to die.
         dead_threads_.push_back(std::this_thread::get_id());
         return false;
      }
      return true;
   }

   std::thread create_thread_() {
      return std::thread([this]() {
         while (!this->empty() || this->should_thread_continue_()) {
            std::function<void()> func = static_cast<WorkQueue*>(this)->pull();
            if (func) {
               func();
               for (;;) {
                  func = static_cast<WorkQueue*>(this)->poll();
                  if (!func) {
                     break;
                  }
                  func();
               }
            }
         }
      });
   }

   mutable std::mutex mutex_;
   std::vector<std::thread> threads_;
   std::vector<std::thread::id> dead_threads_;
   std::size_t n_threads_ = 0;
   bool n_threads_locked_ = false;
};

} // be

#endif
