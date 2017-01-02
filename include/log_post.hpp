#pragma once
#ifndef BE_CORE_LOG_POST_HPP_
#define BE_CORE_LOG_POST_HPP_

#include "service_ids.hpp"
#include "log_processor.hpp"
#include "service_simple_single_thread_executor.hpp"
#include "service.hpp"
#include "service_manager.hpp"

namespace be {

template <typename L>
bool Log::post(detail::LogExpr<L>&& log_expr) {
   if (check_verbosity(static_cast<L&>(log_expr).verbosity(), verbosity_mask())) {
      auto& exec = service<SimpleSingleThreadExecutor>(Id(BEID_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING));

      if (exec.running()) {
         // background service is running, handle asynchronously
         exec.post(detail::LogProcessorTask<L>(*this, std::move(log_expr)));
      } else {
         // background service not running, handle synchronously
         LogRecord rec;
         detail::LogProcessor<L> processor(*this, rec);
         processor(log_expr);
         handle(rec);
      }

      return true;
   }
   return false;
}

} // be

#endif
