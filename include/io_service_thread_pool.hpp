#pragma once
#ifndef BE_CORE_IO_SERVICE_THREAD_POOL_HPP_
#define BE_CORE_IO_SERVICE_THREAD_POOL_HPP_

#include "be.hpp"
#include "io_service.hpp"
#include <vector>
#include <thread>
#include <mutex>

namespace be {

struct IoServiceThreadPoolMain;

////////////////////////////////////////////////////////////////////////////
class IoServiceThreadPool final : Immovable {
   friend struct IoServiceThreadPoolMain;
public:
   IoServiceThreadPool();
   ~IoServiceThreadPool();

   std::size_t active_threads() const;
   std::size_t threads() const;
   void threads(std::size_t new_size);

   IoService io_service;
   
private:
   void shutdown_(std::unique_lock<std::mutex> lock);
   void shrink_(std::unique_lock<std::mutex> lock);
   void expand_(std::unique_lock<std::mutex> lock);

   mutable std::mutex mutex_;
   std::vector<std::thread> threads_;
   std::vector<std::thread::id> dead_threads_;
   std::size_t n_threads_;
   bool n_threads_locked_;
   std::unique_ptr<IoService::work> work_;
};

} // be

#endif
