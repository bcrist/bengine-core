#pragma once
#ifndef BE_CORE_ID_COLLISION_HANDLER_HPP_
#define BE_CORE_ID_COLLISION_HANDLER_HPP_

#include "be.hpp"

namespace be {
namespace detail {

void handle_id_collision(U64 hash, S existing_name, S new_name);

} // be::detail
} // be

#endif
