#pragma once
#ifndef BE_CORE_SERVICE_MUTEX_HPP_
#define BE_CORE_SERVICE_MUTEX_HPP_

#include "service.hpp"
#include <mutex>

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<std::mutex> : True { };

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceTraits<std::mutex> : ServiceTraits<> {
   using preemptive_default = no;
   using lazy_ids = yes;
};

///////////////////////////////////////////////////////////////////////////////
template <>
struct ServiceName<std::mutex> {
   const char* operator()() {
      return "mutex";
   }
};

} // be

#endif
