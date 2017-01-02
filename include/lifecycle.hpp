#pragma once
#ifndef BE_CORE_LIFECYCLE_HPP_
#define BE_CORE_LIFECYCLE_HPP_

namespace be {

enum class InitState {
   uninitialized,
   initializing,
   initialized,
   setting_up,
   ready,
   shutting_down,
   shutdown,
   deinitializing,
   deinitialized
};

InitState init_state();

void init();
void setup();
void shutdown();
void deinit();

void native_init();
void native_deinit();

struct CoreInitLifecycle {
   CoreInitLifecycle() { init(); }
   ~CoreInitLifecycle() { deinit(); }
};

struct CoreLifecycle {
   CoreLifecycle() { setup(); }
   ~CoreLifecycle() { shutdown(); }
};

} // be

#endif

