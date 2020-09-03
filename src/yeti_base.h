#pragma once

#include "SqlRouter.h"
#include "hash/CdrList.h"
#include "resources/ResourceControl.h"
#include "RegistrarRedisConnection.h"
#include "cdr/ALegCdrHeaders.h"

#include "AmConfigReader.h"

#include <ctime>

#include "log.h"

#define YETI_ENABLE_PROFILING 1

#define YETI_CALL_DURATION_TIMER SBC_TIMER_ID_CALL_TIMERS_START
#define YETI_RINGING_TIMEOUT_TIMER (SBC_TIMER_ID_CALL_TIMERS_START+1)
#define YETI_RADIUS_INTERIM_TIMER (SBC_TIMER_ID_CALL_TIMERS_START+2)
#define YETI_FAKE_RINGING_TIMER (SBC_TIMER_ID_CALL_TIMERS_START+3)

#if YETI_ENABLE_PROFILING

#define PROF_START(var) timeval prof_start_ ## var; gettimeofday(&prof_start_ ## var,NULL);
#define PROF_END(var) timeval prof_end_ ## var; gettimeofday(&prof_end_ ## var,NULL);
#define PROF_DIFF(var) timeval prof_diff_ ## var; timersub(&prof_end_ ## var,&prof_start_ ## var,&prof_diff_ ## var);
#define PROF_PRINT(descr,var) PROF_DIFF(var); DBG("PROFILING: " descr " took %s",timeval2str_usec(prof_diff_ ## var).c_str());

#else

#define PROF_START(var) ;
#define PROF_END(var) ;
#define PROF_DIFF(var) (-1)
#define PROF_PRINT(descr,var) ;

#endif

struct YetiBaseParams {
    SqlRouter &router;
    CdrList &cdr_list;
    ResourceControl &rctl;

    YetiBaseParams(
        SqlRouter &router,
        CdrList &cdr_list,
        ResourceControl &rctl)
      : router(router),
        cdr_list(cdr_list),
        rctl(rctl)
    { }
};

struct YetiBase {
    YetiBase(YetiBaseParams &params)
      : router(params.router),
        cdr_list(params.cdr_list),
        rctl(params.rctl)
    { }

    SqlRouter &router;
    CdrList &cdr_list;
    ResourceControl &rctl;

    struct global_config {
        int pop_id;
        bool use_radius;
        bool early_100_trying;
        string routing_schema;
        string msg_logger_dir;
        string audio_recorder_dir;
        bool audio_recorder_compress;
        string log_dir;
        bool pcap_memory_logger;
        bool auth_feedback;
        string http_events_destination;

        bool registrar_enabled;
        string registrar_redis_host;
        int registrar_redis_port;
        int registrar_keepalive_interval;
        int registrar_expires_min;
        int registrar_expires_max;

        aleg_cdr_headers_t aleg_cdr_headers;
    } config;

    AmConfigReader cfg;
    time_t start_time;

    RegistrarRedisConnection registrar_redis;
};
