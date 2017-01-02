#include "pch.hpp"
#include "io_service_thread_pool.hpp"

namespace be {

////////////////////////////////////////////////////////////////////////////
struct IoServiceThreadPoolMain final {

   void operator()() {
      while (!executor.io_service.stopped()) {
         executor.io_service.run();

         {
            std::lock_guard<std::mutex> guard(executor.mutex_);
            if (executor.threads_.size() - executor.dead_threads_.size() > executor.n_threads_) {
               // thread pool is too big; I will volunteer to die.
               executor.dead_threads_.push_back(std::this_thread::get_id());
               return;
            }
         }

         std::this_thread::yield();
      }
   }

   IoServiceThreadPool& executor;
};

////////////////////////////////////////////////////////////////////////////
IoServiceThreadPool::IoServiceThreadPool()
   : n_threads_(0),
     n_threads_locked_(false)
{ }

////////////////////////////////////////////////////////////////////////////
IoServiceThreadPool::~IoServiceThreadPool() {
   threads(0);
}

////////////////////////////////////////////////////////////////////////////
std::size_t IoServiceThreadPool::active_threads() const {
   std::lock_guard<std::mutex> guard(mutex_);
   return threads_.size() - dead_threads_.size();
}

////////////////////////////////////////////////////////////////////////////
std::size_t IoServiceThreadPool::threads() const {
   std::lock_guard<std::mutex> guard(mutex_);
   return n_threads_;
}

////////////////////////////////////////////////////////////////////////////
void IoServiceThreadPool::threads(std::size_t new_size) {
   BE_ASSERT_MAIN_THREAD();

   std::unique_lock<std::mutex> lock(mutex_);
   if (n_threads_locked_ || n_threads_ == new_size) {
      // threads(..) is not reentrant.
      return;
   }

   std::size_t old_size = n_threads_;
   n_threads_ = new_size;
   n_threads_locked_ = true;

   if (new_size == 0) {
      shutdown_(std::move(lock));
   } else if (new_size > old_size) {
      expand_(std::move(lock));
   } else { // new_size < old_size
      shrink_(std::move(lock));
   }
}

////////////////////////////////////////////////////////////////////////////
void IoServiceThreadPool::shutdown_(std::unique_lock<std::mutex> lock) {
   work_.reset();
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

////////////////////////////////////////////////////////////////////////////
void IoServiceThreadPool::shrink_(std::unique_lock<std::mutex> lock) {
   work_.reset();
   do {
      lock.unlock();
      std::this_thread::yield();
      lock.lock();
   } while (threads_.size() - dead_threads_.size() > n_threads_);
   expand_(std::move(lock));
}

////////////////////////////////////////////////////////////////////////////
void IoServiceThreadPool::expand_(std::unique_lock<std::mutex> lock) {
   if (!work_) {
      work_ = std::make_unique<IoService::work>(io_service);
   }

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
      threads_.push_back(std::thread(IoServiceThreadPoolMain { *this }));
   }
   n_threads_locked_ = false;
}

} // be

