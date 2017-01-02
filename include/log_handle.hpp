#pragma once
#ifndef BE_CORE_LOG_HANDLE_IMMEDIATE_HPP_
#define BE_CORE_LOG_HANDLE_IMMEDIATE_HPP_

#include "log_processor.hpp"
#include "log.hpp"

namespace be {

template <typename L>
bool Log::handle(detail::LogExpr<L>&& log_expr) {
   if (check_verbosity(static_cast<L&>(log_expr).verbosity(), verbosity_mask())) {
      LogRecord rec;
      detail::LogProcessor<L>(*this, rec)(std::move(log_expr));
      handle(rec);
      return true;
   }
   return false;
}

} // be

#endif
