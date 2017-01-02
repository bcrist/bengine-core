#pragma once
#ifndef BE_CORE_SERVICE_ATOMIC_HPP_
#define BE_CORE_SERVICE_ATOMIC_HPP_

#include "service.hpp"
#include <atomic>

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct SuppressUndefinedService<std::atomic<T>> : True { };

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct ServiceTraits<std::atomic<T>> : ServiceTraits<> {
   using preemptive_default = no;
   using lazy_ids = yes;
};

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct ServiceName<std::atomic<T>> {
   S operator()() {
      return "atomic<" + type_name<T>() + ">";
   }
};

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct ServiceFactory<std::atomic<T>> {
   std::unique_ptr<std::atomic<T>> operator()(Id) {
      return std::make_unique<std::atomic<T>>(T());
   }
};

} // be

#endif
