#include "pch.hpp"
#include "log_processor.hpp"
#include "log.hpp"

namespace be {
namespace detail {

void handle_log_record(const LogRecord& record, Log& log) {
   log.handle(record);
}

} // be::detail
} // be
