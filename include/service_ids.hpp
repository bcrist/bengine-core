/*!! include('common/ids', { symbol_prefix = 'CORE_SERVICE_', value_prefix = 'be:core:service:' }, {

   {'LOG_VOID', 'log:void'},

   {'IO_SERVICE_THREAD_POOL_LOCAL', 'io-service-thread-pool:local'},
   {'IO_SERVICE_THREAD_POOL_NETWORK', 'io-service-thread-pool:network'},

   {'SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING', 'simple-single-thread-executor:log-processing'}

}) !! 48 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
#pragma once
#ifndef BE_CORE_SERVICE_IDS_HPP_
#define BE_CORE_SERVICE_IDS_HPP_

#include <be/core/id.hpp>

#define BEIDN_CORE_SERVICE_LOG_VOID                                    "be:core:service:log:void"
#define  BEID_CORE_SERVICE_LOG_VOID                                    0xEAEC346BE6FAD584
#define BEIDN_CORE_SERVICE_IO_SERVICE_THREAD_POOL_LOCAL                "be:core:service:io-service-thread-pool:local"
#define  BEID_CORE_SERVICE_IO_SERVICE_THREAD_POOL_LOCAL                0x86241FA8A1F08109
#define BEIDN_CORE_SERVICE_IO_SERVICE_THREAD_POOL_NETWORK              "be:core:service:io-service-thread-pool:network"
#define  BEID_CORE_SERVICE_IO_SERVICE_THREAD_POOL_NETWORK              0x0C428BC90632BB50
#define BEIDN_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING "be:core:service:simple-single-thread-executor:log-processing"
#define  BEID_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING 0x3DC9CF096C8B2F00

#ifdef BE_ID_EXTERNS

namespace be {
namespace ids {

extern const Id core_service_log_void;
extern const Id core_service_io_service_thread_pool_local;
extern const Id core_service_io_service_thread_pool_network;
extern const Id core_service_simple_single_thread_executor_logprocessing;

} // be::ids
} // be

#endif
#endif
#ifdef BE_ID_STATICS

namespace be {
namespace ids {

const Id core_service_log_void = Id(BEIDN_CORE_SERVICE_LOG_VOID);
const Id core_service_io_service_thread_pool_local = Id(BEIDN_CORE_SERVICE_IO_SERVICE_THREAD_POOL_LOCAL);
const Id core_service_io_service_thread_pool_network = Id(BEIDN_CORE_SERVICE_IO_SERVICE_THREAD_POOL_NETWORK);
const Id core_service_simple_single_thread_executor_logprocessing = Id(BEIDN_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING);

} // be::ids
} // be

#endif

/* ######################### END OF GENERATED CODE ######################### */
