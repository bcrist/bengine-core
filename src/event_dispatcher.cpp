#include "pch.hpp"
#include "event_dispatcher.hpp"
#include "logging.hpp"
#include <algorithm>

namespace be {

//////////////////////////////////////////////////////////////////////////////
void EventDispatcher::reset() {
   deferred_set_.clear();
   dispatchers_.clear();
   next_callback_id_ = Id();
}

//////////////////////////////////////////////////////////////////////////////
void EventDispatcher::dispatch_deferred() {
   for (auto ptr : deferred_set_) {
      ptr->fire_deferred();
   }
   deferred_set_.clear();
}

//////////////////////////////////////////////////////////////////////////////
void EventDispatcher::inc_next_callback_id_() {
   next_callback_id_ = Id((U64)next_callback_id_ + 1);
   if ((U64) next_callback_id_ == 0x0) {
      be_warn() << "EventDispatcher automatic callback ID overflow!"
         | default_log();
   }
}

//////////////////////////////////////////////////////////////////////////////
void EventDispatcher::add_deferred_dispatcher_(EventDispatcherBase& dispatcher) {
   auto ptr = &dispatcher;
   auto it = std::lower_bound(deferred_set_.begin(), deferred_set_.end(), ptr);
   if (it == deferred_set_.end() || ptr != *it) {
      deferred_set_.insert(it, ptr);
   }
}

} // be
