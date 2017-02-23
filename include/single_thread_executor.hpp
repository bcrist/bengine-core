#pragma once
#ifndef BE_CORE_SINGLE_THREAD_EXECUTOR_HPP_
#define BE_CORE_SINGLE_THREAD_EXECUTOR_HPP_

#include "be.hpp"
#include "work_queue_traits.hpp"
#include <functional>
#include <thread>
#include <atomic>
#include <cassert>

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename WorkQueue, bool HasShutdown = WorkQueueTraits<WorkQueue>::has_shutdown::value>
class SingleThreadExecutor : public WorkQueue {
   using type = SingleThreadExecutor<WorkQueue, HasShutdown>;
public:
   SingleThreadExecutor() = default;
   SingleThreadExecutor(type&& other) = default;
   type& operator=(type&& other) = default;
   ~SingleThreadExecutor() {
      running(false);
   }

   bool running() const {
      return thread_.joinable();
   }

   void running(bool should_run) {
      BE_ASSERT_MAIN_THREAD();
      if (should_run != running()) {
         if (should_run) {
            thread_ = create_thread_();
         } else {
            this->shutdown();
            thread_.join();
         }
      }
   }

   std::function<void()> pull() = delete;
   std::function<void()> poll() = delete;

private:
   std::thread create_thread_() {
      return std::thread([this]() {
         while (*this || !this->empty()) {
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
            std::this_thread::yield();
         }
      });
   }

   std::thread thread_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename WorkQueue>
class SingleThreadExecutor<WorkQueue, false> : public WorkQueue {
   using type = SingleThreadExecutor<WorkQueue, false>;
public:
   SingleThreadExecutor() = default;
   ~SingleThreadExecutor() {
      running(false);
   }

   explicit operator bool() const {
      return !shutdown_ && (bool)static_cast<const WorkQueue&>(*this);
   }

   void shutdown(bool shutdown = true) {
      shutdown_ = shutdown;
   }

   bool running() const {
      return thread_.joinable();
   }

   void running(bool should_run) {
      BE_ASSERT_MAIN_THREAD();
      if (should_run != running()) {
         if (should_run) {
            thread_ = create_thread_();
         } else {
            this->shutdown();
            thread_.join();
         }
      }
   }

   std::function<void()> pull() = delete;
   std::function<void()> poll() = delete;

private:
   std::thread create_thread_() {
      return std::thread([this]() {
         while (*this || !this->empty()) {
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
            std::this_thread::yield();
         }
      });
   }

   std::thread thread_;
   std::atomic<bool> shutdown_ = false;
};

} // be

#endif
