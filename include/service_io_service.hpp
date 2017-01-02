#pragma once
#ifndef BE_CORE_SERVICE_IO_SERVICE_HPP_
#define BE_CORE_SERVICE_IO_SERVICE_HPP_

#include "service.hpp"
#include "io_service.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<IoService> : True { };

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceName<IoService> {
   const char* operator()() {
      return "asio::io_service";
   }
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceShutdown<IoService> {
   void operator()(std::unique_ptr<IoService>& service) {
      if (!service->stopped()) {
         service->stop();
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceCleanup<IoService> {
   void operator()(std::unique_ptr<IoService>& service) {
      service.reset();
   }
};

///////////////////////////////////////////////////////////////////////////////
IoService& foreground_io_service();

} // be

#endif

