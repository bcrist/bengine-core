/*!! include('ids', { symbol_prefix = 'CORE_SERVICE_', value_prefix = 'be:core:service:' }, {

   {'IO_SERVICE_THREAD_POOL_LOCAL', 'io-service-thread-pool:local'},
   {'IO_SERVICE_THREAD_POOL_NETWORK', 'io-service-thread-pool:network'},

   {'SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING', 'simple_single_thread_executor:log-processing'},

   'opus:setup',
   'opus:shutdown'
   
}) !! 52 */
/* ######## !! GENERATED CODE -- DO NOT MODIFY !! ######## */
#pragma once
#ifndef BE_CORE_SERVICE_IDS_HPP_
#define BE_CORE_SERVICE_IDS_HPP_

#include <be/core/id.hpp>

#define BEIDN_CORE_SERVICE_IO_SERVICE_THREAD_POOL_LOCAL                "be:core:service:io-service-thread-pool:local"
#define  BEID_CORE_SERVICE_IO_SERVICE_THREAD_POOL_LOCAL                0x86241FA8A1F08109
#define BEIDN_CORE_SERVICE_IO_SERVICE_THREAD_POOL_NETWORK              "be:core:service:io-service-thread-pool:network"
#define  BEID_CORE_SERVICE_IO_SERVICE_THREAD_POOL_NETWORK              0x0C428BC90632BB50
#define BEIDN_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING "be:core:service:simple_single_thread_executor:log-processing"
#define  BEID_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING 0xF3D674A015459746
#define BEIDN_CORE_SERVICE_OPUS_SETUP                                  "be:core:service:opus:setup"
#define  BEID_CORE_SERVICE_OPUS_SETUP                                  0xE59EE3362A463E5E
#define BEIDN_CORE_SERVICE_OPUS_SHUTDOWN                               "be:core:service:opus:shutdown"
#define  BEID_CORE_SERVICE_OPUS_SHUTDOWN                               0x8714D8A99B6E5B53

#ifdef BE_ID_EXTERNS

namespace be {
namespace ids {

extern const Id core_service_io_service_thread_pool_local;
extern const Id core_service_io_service_thread_pool_network;
extern const Id core_service_simple_single_thread_executor_logprocessing;
extern const Id core_service_opus_setup;
extern const Id core_service_opus_shutdown;

} // be::ids
} // be

#endif
#endif
#ifdef BE_ID_STATICS

namespace be {
namespace ids {

const Id core_service_io_service_thread_pool_local = Id(BEIDN_CORE_SERVICE_IO_SERVICE_THREAD_POOL_LOCAL);
const Id core_service_io_service_thread_pool_network = Id(BEIDN_CORE_SERVICE_IO_SERVICE_THREAD_POOL_NETWORK);
const Id core_service_simple_single_thread_executor_logprocessing = Id(BEIDN_CORE_SERVICE_SIMPLE_SINGLE_THREAD_EXECUTOR_LOGPROCESSING);
const Id core_service_opus_setup = Id(BEIDN_CORE_SERVICE_OPUS_SETUP);
const Id core_service_opus_shutdown = Id(BEIDN_CORE_SERVICE_OPUS_SHUTDOWN);

} // be::ids
} // be

#endif

/* ################ END OF GENERATED CODE ################ */
