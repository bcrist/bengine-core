#include "pch.hpp"
#include "id_collision_handler.hpp"
#include "logging.hpp"
#include <iomanip>

namespace be {
namespace detail {

void handle_id_collision(U64 hash, S existing_name, S new_name) {
   // Hash collision: Print a warning
   be_warn() << "Id hash collision detected!"
      & attr("Hash") << std::hex << std::setfill('0') << std::setw(16) << hash
      & attr("Existing") << std::move(existing_name)
      & attr("Current") << std::move(new_name)
      | default_log();
}

} // be::detail
} // be
