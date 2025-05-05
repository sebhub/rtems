/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsRatemonReqGetStatistics
 */

/*
 * Copyright (C) 2025 embedded brains GmbH & Co. KG
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <string.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RtemsRatemonReqGetStatistics \
 *   spec:/rtems/ratemon/req/get-statistics
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  RtemsRatemonReqGetStatistics_Pre_Stats_Valid,
  RtemsRatemonReqGetStatistics_Pre_Stats_Null,
  RtemsRatemonReqGetStatistics_Pre_Stats_NA
} RtemsRatemonReqGetStatistics_Pre_Stats;

typedef enum {
  RtemsRatemonReqGetStatistics_Pre_Id_NoObj,
  RtemsRatemonReqGetStatistics_Pre_Id_Period,
  RtemsRatemonReqGetStatistics_Pre_Id_NA
} RtemsRatemonReqGetStatistics_Pre_Id;

typedef enum {
  RtemsRatemonReqGetStatistics_Post_Status_Ok,
  RtemsRatemonReqGetStatistics_Post_Status_InvAddr,
  RtemsRatemonReqGetStatistics_Post_Status_InvId,
  RtemsRatemonReqGetStatistics_Post_Status_NA
} RtemsRatemonReqGetStatistics_Post_Status;

typedef enum {
  RtemsRatemonReqGetStatistics_Post_Count_Value,
  RtemsRatemonReqGetStatistics_Post_Count_Nop,
  RtemsRatemonReqGetStatistics_Post_Count_NA
} RtemsRatemonReqGetStatistics_Post_Count;

typedef enum {
  RtemsRatemonReqGetStatistics_Post_MissedCount_Value,
  RtemsRatemonReqGetStatistics_Post_MissedCount_Nop,
  RtemsRatemonReqGetStatistics_Post_MissedCount_NA
} RtemsRatemonReqGetStatistics_Post_MissedCount;

typedef enum {
  RtemsRatemonReqGetStatistics_Post_MinCPUTime_Value,
  RtemsRatemonReqGetStatistics_Post_MinCPUTime_Nop,
  RtemsRatemonReqGetStatistics_Post_MinCPUTime_NA
} RtemsRatemonReqGetStatistics_Post_MinCPUTime;

typedef enum {
  RtemsRatemonReqGetStatistics_Post_MaxCPUTime_Value,
  RtemsRatemonReqGetStatistics_Post_MaxCPUTime_Nop,
  RtemsRatemonReqGetStatistics_Post_MaxCPUTime_NA
} RtemsRatemonReqGetStatistics_Post_MaxCPUTime;

typedef enum {
  RtemsRatemonReqGetStatistics_Post_TotalCPUTime_Value,
  RtemsRatemonReqGetStatistics_Post_TotalCPUTime_Nop,
  RtemsRatemonReqGetStatistics_Post_TotalCPUTime_NA
} RtemsRatemonReqGetStatistics_Post_TotalCPUTime;

typedef enum {
  RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_Value,
  RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_Nop,
  RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_NA
} RtemsRatemonReqGetStatistics_Post_MinMonotonicTime;

typedef enum {
  RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_Value,
  RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_Nop,
  RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_NA
} RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime;

typedef enum {
  RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_Value,
  RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_Nop,
  RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_NA
} RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime;

typedef struct {
  uint32_t Skip : 1;
  uint32_t Pre_Stats_NA : 1;
  uint32_t Pre_Id_NA : 1;
  uint32_t Post_Status : 2;
  uint32_t Post_Count : 2;
  uint32_t Post_MissedCount : 2;
  uint32_t Post_MinCPUTime : 2;
  uint32_t Post_MaxCPUTime : 2;
  uint32_t Post_TotalCPUTime : 2;
  uint32_t Post_MinMonotonicTime : 2;
  uint32_t Post_MaxMonotonicTime : 2;
  uint32_t Post_TotalMonotonicTime : 2;
} RtemsRatemonReqGetStatistics_Entry;

/**
 * @brief Test context for spec:/rtems/ratemon/req/get-statistics test case.
 */
typedef struct {
  /**
   * @brief This member contains the identifier of the worker task.
   */
  rtems_id worker_id;

  /**
   * @brief This member contains the previous get timecount handler to restore.
   */
  GetTimecountHandler previous_get_timecount;

  /**
   * @brief This member provides the period statistics object.
   */
  rtems_rate_monotonic_period_statistics stats_obj;

  /**
   * @brief This member provides the object identifier parameter.
   */
  rtems_id id;

  /**
   * @brief This member provides the period statistics parameter.
   */
  rtems_rate_monotonic_period_statistics *stats;

  /**
   * @brief This member contains the return status.
   */
  rtems_status_code status;

  struct {
    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 2 ];

    /**
     * @brief If this member is true, then the test action loop is executed.
     */
    bool in_action_loop;

    /**
     * @brief This member contains the next transition map index.
     */
    size_t index;

    /**
     * @brief This member contains the current transition map entry.
     */
    RtemsRatemonReqGetStatistics_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsRatemonReqGetStatistics_Context;

static RtemsRatemonReqGetStatistics_Context
  RtemsRatemonReqGetStatistics_Instance;

static const char *const RtemsRatemonReqGetStatistics_PreDesc_Stats[] =
  { "Valid", "Null", "NA" };

static const char *const RtemsRatemonReqGetStatistics_PreDesc_Id[] =
  { "NoObj", "Period", "NA" };

static const char *const *const RtemsRatemonReqGetStatistics_PreDesc[] = {
  RtemsRatemonReqGetStatistics_PreDesc_Stats,
  RtemsRatemonReqGetStatistics_PreDesc_Id,
  NULL
};

static uint32_t FreezeTime( void )
{
  return GetTimecountCounter() - 1;
}

static void TickWorker( rtems_task_argument arg )
{
  (void) arg;

  while ( true ) {
    TimecounterTick();
  }
}

static void TimespecEq( const struct timespec *ts, rtems_interval ticks )
{
  T_eq_ll( ts->tv_sec, 0 );
  T_eq_long(
    ( ts->tv_nsec + 499999L ) / 1000000L,
    ticks * rtems_configuration_get_milliseconds_per_tick()
  );
}

static void TimespecNop( const struct timespec *ts )
{
  T_eq_ll( ts->tv_sec, -1LL );
  T_eq_long( ts->tv_nsec, -1L );
}

static rtems_id CreatePeriod( void )
{
  rtems_id          id;
  rtems_status_code sc;

  sc = rtems_rate_monotonic_create( OBJECT_NAME, &id );
  T_rsc_success( sc );

  sc = rtems_rate_monotonic_period( id, 3 );
  T_rsc_success( sc );

  TimecounterTick();
  TimecounterTick();
  TimecounterTick();
  TimecounterTick();

  sc = rtems_rate_monotonic_period( id, 3 );
  T_rsc( sc, RTEMS_TIMEOUT );

  TimecounterTick();

  sc = rtems_rate_monotonic_period( id, 3 );
  T_rsc_success( sc );

  sc = rtems_task_wake_after( 1 );
  T_rsc_success( sc );

  TimecounterTick();

  sc = rtems_rate_monotonic_period( id, 3 );
  T_rsc_success( sc );

  return id;
}

static void RtemsRatemonReqGetStatistics_Pre_Stats_Prepare(
  RtemsRatemonReqGetStatistics_Context  *ctx,
  RtemsRatemonReqGetStatistics_Pre_Stats state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Pre_Stats_Valid: {
      /*
       * While the `statistics` parameter references an object of type
       * rtems_rate_monotonic_period_statistics.
       */
      ctx->stats = &ctx->stats_obj;
      break;
    }

    case RtemsRatemonReqGetStatistics_Pre_Stats_Null: {
      /*
       * While the `statistics` parameter is equal to NULL.
       */
      ctx->stats = NULL;
      break;
    }

    case RtemsRatemonReqGetStatistics_Pre_Stats_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Pre_Id_Prepare(
  RtemsRatemonReqGetStatistics_Context *ctx,
  RtemsRatemonReqGetStatistics_Pre_Id   state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Pre_Id_NoObj: {
      /*
       * While the `id` parameter is not associated with a period.
       */
      ctx->id = 0xffffffff;
      break;
    }

    case RtemsRatemonReqGetStatistics_Pre_Id_Period: {
      /*
       * While the `id` parameter is associated with a period.
       */
      ctx->id = CreatePeriod();
      break;
    }

    case RtemsRatemonReqGetStatistics_Pre_Id_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Post_Status_Check(
  RtemsRatemonReqGetStatistics_Context    *ctx,
  RtemsRatemonReqGetStatistics_Post_Status state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Post_Status_Ok: {
      /*
       * The return status of rtems_rate_monotonic_get_statistics() shall be
       * RTEMS_SUCCESSFUL.
       */
      T_rsc_success( ctx->status );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_Status_InvAddr: {
      /*
       * The return status of rtems_rate_monotonic_get_statistics() shall be
       * RTEMS_INVALID_ADDRESS.
       */
      T_rsc( ctx->status, RTEMS_INVALID_ADDRESS );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_Status_InvId: {
      /*
       * The return status of rtems_rate_monotonic_get_statistics() shall be
       * RTEMS_INVALID_ID.
       */
      T_rsc( ctx->status, RTEMS_INVALID_ID );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_Status_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Post_Count_Check(
  RtemsRatemonReqGetStatistics_Context   *ctx,
  RtemsRatemonReqGetStatistics_Post_Count state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Post_Count_Value: {
      /*
       * The value of the member count of the object referenced by the
       * `statistics` parameter shall be set to the count of executed periods
       * since the last reset recorded for the period associated with the
       * object identifier specified by the `id` parameter.
       */
      T_eq_u32( ctx->stats_obj.count, 3 );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_Count_Nop: {
      /*
       * The value of the member count of the object referenced by the
       * `statistics` parameter shall not be modified.
       */
      T_eq_u32( ctx->stats_obj.count, 0xffffffff );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_Count_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Post_MissedCount_Check(
  RtemsRatemonReqGetStatistics_Context         *ctx,
  RtemsRatemonReqGetStatistics_Post_MissedCount state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Post_MissedCount_Value: {
      /*
       * The value of the member missed_count of the object referenced by the
       * `statistics` parameter shall be set to the count of missed periods
       * since the last reset recorded for the period associated with the
       * object identifier specified by the `id` parameter.
       */
      T_eq_u32( ctx->stats_obj.missed_count, 1 );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_MissedCount_Nop: {
      /*
       * The value of the member missed_count of the object referenced by the
       * `statistics` parameter shall not be modified.
       */
      T_eq_u32( ctx->stats_obj.missed_count, 0xffffffff );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_MissedCount_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Post_MinCPUTime_Check(
  RtemsRatemonReqGetStatistics_Context        *ctx,
  RtemsRatemonReqGetStatistics_Post_MinCPUTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Post_MinCPUTime_Value: {
      /*
       * The value of the member min_cpu_time of the object referenced by the
       * `statistics` parameter shall be set to the least amount of processor
       * time used in a period since the last reset recorded for the period
       * associated with the object identifier specified by the `id` parameter.
       */
      TimespecEq( &ctx->stats_obj.min_cpu_time, 1 );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_MinCPUTime_Nop: {
      /*
       * The value of the member min_cpu_time of the object referenced by the
       * `statistics` parameter shall not be modified.
       */
      TimespecNop( &ctx->stats_obj.min_cpu_time );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_MinCPUTime_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Post_MaxCPUTime_Check(
  RtemsRatemonReqGetStatistics_Context        *ctx,
  RtemsRatemonReqGetStatistics_Post_MaxCPUTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Post_MaxCPUTime_Value: {
      /*
       * The value of the member max_cpu_time of the object referenced by the
       * `statistics` parameter shall be set to the highest amount of processor
       * time used in a period since the last reset recorded for the period
       * associated with the object identifier specified by the `id` parameter.
       */
      TimespecEq( &ctx->stats_obj.max_cpu_time, 4 );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_MaxCPUTime_Nop: {
      /*
       * The value of the member max_cpu_time of the object referenced by the
       * `statistics` parameter shall not be modified.
       */
      TimespecNop( &ctx->stats_obj.max_cpu_time );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_MaxCPUTime_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Post_TotalCPUTime_Check(
  RtemsRatemonReqGetStatistics_Context          *ctx,
  RtemsRatemonReqGetStatistics_Post_TotalCPUTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Post_TotalCPUTime_Value: {
      /*
       * The value of the member total_cpu_time of the object referenced by the
       * `statistics` parameter shall be set to the total amount of processor
       * time used in all periods since the last reset recorded for the period
       * associated with the object identifier specified by the `id` parameter.
       */
      TimespecEq( &ctx->stats_obj.total_cpu_time, 6 );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_TotalCPUTime_Nop: {
      /*
       * The value of the member total_cpu_time of the object referenced by the
       * `statistics` parameter shall not be modified.
       */
      TimespecNop( &ctx->stats_obj.total_cpu_time );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_TotalCPUTime_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_Check(
  RtemsRatemonReqGetStatistics_Context              *ctx,
  RtemsRatemonReqGetStatistics_Post_MinMonotonicTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_Value: {
      /*
       * The value of the member min_wall_time of the object referenced by the
       * `statistics` parameter shall be set to the least amount of
       * CLOCK_MONOTONIC time used in a period since the last reset recorded
       * for the period associated with the object identifier specified by the
       * `id` parameter.
       */
      TimespecEq( &ctx->stats_obj.min_wall_time, 1 );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_Nop: {
      /*
       * The value of the member min_wall_time of the object referenced by the
       * `statistics` parameter shall not be modified.
       */
      TimespecNop( &ctx->stats_obj.min_wall_time );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_Check(
  RtemsRatemonReqGetStatistics_Context              *ctx,
  RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_Value: {
      /*
       * The value of the member max_wall_time of the object referenced by the
       * `statistics` parameter shall be set to the highest amount of
       * CLOCK_MONOTONIC time used in a period since the last reset recorded
       * for the period associated with the object identifier specified by the
       * `id` parameter.
       */
      TimespecEq( &ctx->stats_obj.max_wall_time, 4 );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_Nop: {
      /*
       * The value of the member max_wall_time of the object referenced by the
       * `statistics` parameter shall not be modified.
       */
      TimespecNop( &ctx->stats_obj.max_wall_time );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_Check(
  RtemsRatemonReqGetStatistics_Context                *ctx,
  RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_Value: {
      /*
       * The value of the member total_wall_time of the object referenced by
       * the `statistics` parameter shall be set to the total amount of
       * CLOCK_MONOTONIC time used in a period since the last reset recorded
       * for the period associated with the object identifier specified by the
       * `id` parameter.
       */
      TimespecEq( &ctx->stats_obj.total_wall_time, 7 );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_Nop: {
      /*
       * The value of the member total_wall_time of the object referenced by
       * the `statistics` parameter shall not be modified.
       */
      TimespecNop( &ctx->stats_obj.total_wall_time );
      break;
    }

    case RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatistics_Setup(
  RtemsRatemonReqGetStatistics_Context *ctx
)
{
  ctx->previous_get_timecount = SetGetTimecountHandler( FreezeTime );
  ctx->worker_id = CreateTask( "WORK", GetSelfPriority() + 1 );
  StartTask( ctx->worker_id, TickWorker, NULL );
  TimecounterTick();
}

static void RtemsRatemonReqGetStatistics_Setup_Wrap( void *arg )
{
  RtemsRatemonReqGetStatistics_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsRatemonReqGetStatistics_Setup( ctx );
}

static void RtemsRatemonReqGetStatistics_Teardown(
  RtemsRatemonReqGetStatistics_Context *ctx
)
{
  DeleteTask( ctx->worker_id );
  SetGetTimecountHandler( ctx->previous_get_timecount );
}

static void RtemsRatemonReqGetStatistics_Teardown_Wrap( void *arg )
{
  RtemsRatemonReqGetStatistics_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsRatemonReqGetStatistics_Teardown( ctx );
}

static void RtemsRatemonReqGetStatistics_Prepare(
  RtemsRatemonReqGetStatistics_Context *ctx
)
{
  memset( &ctx->stats_obj, 0xff, sizeof( ctx->stats_obj ) );
}

static void RtemsRatemonReqGetStatistics_Action(
  RtemsRatemonReqGetStatistics_Context *ctx
)
{
  ctx->status = rtems_rate_monotonic_get_statistics( ctx->id, ctx->stats );
}

static void RtemsRatemonReqGetStatistics_Cleanup(
  RtemsRatemonReqGetStatistics_Context *ctx
)
{
  if ( ctx->id != 0xffffffff ) {
    rtems_status_code sc;

    sc = rtems_rate_monotonic_delete( ctx->id );
    T_rsc_success( sc );
  }
}

/* clang-format off */

static const RtemsRatemonReqGetStatistics_Entry
RtemsRatemonReqGetStatistics_Entries[] = {
  { 0, 0, 0, RtemsRatemonReqGetStatistics_Post_Status_InvAddr,
    RtemsRatemonReqGetStatistics_Post_Count_NA,
    RtemsRatemonReqGetStatistics_Post_MissedCount_NA,
    RtemsRatemonReqGetStatistics_Post_MinCPUTime_NA,
    RtemsRatemonReqGetStatistics_Post_MaxCPUTime_NA,
    RtemsRatemonReqGetStatistics_Post_TotalCPUTime_NA,
    RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_NA,
    RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_NA,
    RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_NA },
  { 0, 0, 0, RtemsRatemonReqGetStatistics_Post_Status_InvId,
    RtemsRatemonReqGetStatistics_Post_Count_Nop,
    RtemsRatemonReqGetStatistics_Post_MissedCount_Nop,
    RtemsRatemonReqGetStatistics_Post_MinCPUTime_Nop,
    RtemsRatemonReqGetStatistics_Post_MaxCPUTime_Nop,
    RtemsRatemonReqGetStatistics_Post_TotalCPUTime_Nop,
    RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_Nop,
    RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_Nop,
    RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_Nop },
  { 0, 0, 0, RtemsRatemonReqGetStatistics_Post_Status_Ok,
    RtemsRatemonReqGetStatistics_Post_Count_Value,
    RtemsRatemonReqGetStatistics_Post_MissedCount_Value,
    RtemsRatemonReqGetStatistics_Post_MinCPUTime_Value,
    RtemsRatemonReqGetStatistics_Post_MaxCPUTime_Value,
    RtemsRatemonReqGetStatistics_Post_TotalCPUTime_Value,
    RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_Value,
    RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_Value,
    RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_Value }
};

static const uint8_t
RtemsRatemonReqGetStatistics_Map[] = {
  1, 2, 0, 0
};

/* clang-format on */

static size_t RtemsRatemonReqGetStatistics_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  RtemsRatemonReqGetStatistics_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsRatemonReqGetStatistics_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsRatemonReqGetStatistics_Fixture = {
  .setup = RtemsRatemonReqGetStatistics_Setup_Wrap,
  .stop = NULL,
  .teardown = RtemsRatemonReqGetStatistics_Teardown_Wrap,
  .scope = RtemsRatemonReqGetStatistics_Scope,
  .initial_context = &RtemsRatemonReqGetStatistics_Instance
};

static inline RtemsRatemonReqGetStatistics_Entry
RtemsRatemonReqGetStatistics_PopEntry(
  RtemsRatemonReqGetStatistics_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsRatemonReqGetStatistics_Entries
    [ RtemsRatemonReqGetStatistics_Map[ index ] ];
}

static void RtemsRatemonReqGetStatistics_TestVariant(
  RtemsRatemonReqGetStatistics_Context *ctx
)
{
  RtemsRatemonReqGetStatistics_Pre_Stats_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  RtemsRatemonReqGetStatistics_Pre_Id_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  RtemsRatemonReqGetStatistics_Action( ctx );
  RtemsRatemonReqGetStatistics_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  RtemsRatemonReqGetStatistics_Post_Count_Check(
    ctx,
    ctx->Map.entry.Post_Count
  );
  RtemsRatemonReqGetStatistics_Post_MissedCount_Check(
    ctx,
    ctx->Map.entry.Post_MissedCount
  );
  RtemsRatemonReqGetStatistics_Post_MinCPUTime_Check(
    ctx,
    ctx->Map.entry.Post_MinCPUTime
  );
  RtemsRatemonReqGetStatistics_Post_MaxCPUTime_Check(
    ctx,
    ctx->Map.entry.Post_MaxCPUTime
  );
  RtemsRatemonReqGetStatistics_Post_TotalCPUTime_Check(
    ctx,
    ctx->Map.entry.Post_TotalCPUTime
  );
  RtemsRatemonReqGetStatistics_Post_MinMonotonicTime_Check(
    ctx,
    ctx->Map.entry.Post_MinMonotonicTime
  );
  RtemsRatemonReqGetStatistics_Post_MaxMonotonicTime_Check(
    ctx,
    ctx->Map.entry.Post_MaxMonotonicTime
  );
  RtemsRatemonReqGetStatistics_Post_TotalMonotonicTime_Check(
    ctx,
    ctx->Map.entry.Post_TotalMonotonicTime
  );
}

/**
 * @fn void T_case_body_RtemsRatemonReqGetStatistics( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsRatemonReqGetStatistics,
  &RtemsRatemonReqGetStatistics_Fixture
)
{
  RtemsRatemonReqGetStatistics_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = RtemsRatemonReqGetStatistics_Pre_Stats_Valid;
    ctx->Map.pcs[ 0 ] < RtemsRatemonReqGetStatistics_Pre_Stats_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = RtemsRatemonReqGetStatistics_Pre_Id_NoObj;
      ctx->Map.pcs[ 1 ] < RtemsRatemonReqGetStatistics_Pre_Id_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      ctx->Map.entry = RtemsRatemonReqGetStatistics_PopEntry( ctx );
      RtemsRatemonReqGetStatistics_Prepare( ctx );
      RtemsRatemonReqGetStatistics_TestVariant( ctx );
      RtemsRatemonReqGetStatistics_Cleanup( ctx );
    }
  }
}

/** @} */
