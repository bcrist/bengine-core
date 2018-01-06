#pragma once
#ifndef BE_CORE_SERVICE_HELPERS_HPP_
#define BE_CORE_SERVICE_HELPERS_HPP_

#include "service_manager.hpp"

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
bool check_service() {
   return !!detail::get_service_manager<ServiceType>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
ServiceType& service() {
   return detail::get_service_manager<ServiceType>().get();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
ServiceType& service(Id id) {
   return detail::get_service_manager<ServiceType>().get(id);
}

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
ServiceType* optional_service() {
   return detail::get_service_manager<ServiceType>().optional();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
ServiceType* optional_service(Id id) {
   return detail::get_service_manager<ServiceType>().optional(id);
}

///////////////////////////////////////////////////////////////////////////////
template <typename ServiceType>
void shutdown_service() {
   return detail::get_service_manager<ServiceType>().shutdown();
}

} // be

#endif
