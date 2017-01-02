#include "pch.hpp"
#include "lifecycle.hpp"
#include "id_collision_handler.hpp"
#include "service_ids.hpp"
#include "service_helpers.hpp"
#include "service_io_service_thread_pool.hpp"
#include "service_simple_single_thread_executor.hpp"
#include "stack_trace.hpp"
#include <iostream>

namespace be {
namespace {

// Initialize this regardless of BE_ENABLE_MAIN_THREAD_ASSERTIONS so that other modules can define it and still work.
std::thread::id main_thread_id;

BE_NO_INLINE void on_terminate() {
   if (std::uncaught_exceptions() <= 1) {
      if (check_service<SimpleSingleThreadExecutor>()) {
         service<SimpleSingleThreadExecutor>(ids::core_service_simple_single_thread_executor_logprocessing).running(false);
      }

      std::cerr << get_stack_trace(1);
   }
   std::abort();
}

InitState& state() {
   static InitState s = InitState::uninitialized;
   return s;
}

} // be::()

InitState init_state() {
   return state();
}

void init() {
   assert(state() == InitState::uninitialized);
   state() = InitState::initializing;
   native_init();
   main_thread_id = std::this_thread::get_id();
   std::set_terminate(on_terminate);
   std::locale::global(default_locale());
   state() = InitState::initialized;
}

void setup() {
   assert(state() == InitState::initialized);
   BE_ASSERT_MAIN_THREAD();
   state() = InitState::setting_up;
   service<SimpleSingleThreadExecutor>(ids::core_service_simple_single_thread_executor_logprocessing).running(true);
   state() = InitState::ready;
}

void shutdown() {
   assert(state() == InitState::ready);
   BE_ASSERT_MAIN_THREAD();
   state() = InitState::shutting_down;
   service<SimpleSingleThreadExecutor>(ids::core_service_simple_single_thread_executor_logprocessing).running(false);
   state() = InitState::shutdown;
}

void deinit() {
   assert(state() == InitState::shutdown || state() == InitState::initialized);
   BE_ASSERT_MAIN_THREAD();
   state() = InitState::deinitializing;
   std::locale::global(std::locale::classic());
   native_deinit();
   state() = InitState::deinitialized;
}

bool is_main_thread() {
   return std::this_thread::get_id() == main_thread_id || main_thread_id == std::thread::id();
}

} // be
