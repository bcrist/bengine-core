#pragma once
#ifndef BE_CORE_SERVICE_LOG_HPP_
#define BE_CORE_SERVICE_LOG_HPP_

#include "service.hpp"
#include "service_ids.hpp"
#include "console_log_sink.hpp"
#include "log.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<Log> : True { };

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceTraits<Log> : ServiceTraits<> {
   using lazy_ids = yes;
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceName<Log> {
   const char* operator()() {
      return "Log";
   }
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceFactory<Log> {
   std::unique_ptr<Log> operator()(Id id) {
      std::unique_ptr<Log> ptr = std::make_unique<Log>();
      if (id == Id()) {
         ptr->sink(ConsoleLogSink());
      } else if (id == ids::core_service_log_void) {
         ptr->verbosity_mask(0);
      }
      return ptr;
   }
};


} // be

#endif
