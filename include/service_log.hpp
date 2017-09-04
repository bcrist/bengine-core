#pragma once
#ifndef BE_CORE_SERVICE_LOG_HPP_
#define BE_CORE_SERVICE_LOG_HPP_

#include "service.hpp"
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

} // be

#endif
