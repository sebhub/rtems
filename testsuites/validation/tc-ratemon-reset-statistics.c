/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsRatemonReqResetStatistics
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
 * @defgroup RtemsRatemonReqResetStatistics \
 *   spec:/rtems/ratemon/req/reset-statistics
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  RtemsRatemonReqResetStatistics_Pre_Id_NoObj,
  RtemsRatemonReqResetStatistics_Pre_Id_Period,
  RtemsRatemonReqResetStatistics_Pre_Id_NA
} RtemsRatemonReqResetStatistics_Pre_Id;

typedef enum {
  RtemsRatemonReqResetStatistics_Post_Status_Ok,
  RtemsRatemonReqResetStatistics_Post_Status_InvId,
  RtemsRatemonReqResetStatistics_Post_Status_NA
} RtemsRatemonReqResetStatistics_Post_Status;

typedef enum {
  RtemsRatemonReqResetStatistics_Post_Count_Zero,
  RtemsRatemonReqResetStatistics_Post_Count_NA
} RtemsRatemonReqResetStatistics_Post_Count;

typedef enum {
  RtemsRatemonReqResetStatistics_Post_MissedCount_Zero,
  RtemsRatemonReqResetStatistics_Post_MissedCount_NA
} RtemsRatemonReqResetStatistics_Post_MissedCount;

typedef enum {
  RtemsRatemonReqResetStatistics_Post_MinCPUTime_Max,
  RtemsRatemonReqResetStatistics_Post_MinCPUTime_NA
} RtemsRatemonReqResetStatistics_Post_MinCPUTime;

typedef enum {
  RtemsRatemonReqResetStatistics_Post_MaxCPUTime_Zero,
  RtemsRatemonReqResetStatistics_Post_MaxCPUTime_NA
} RtemsRatemonReqResetStatistics_Post_MaxCPUTime;

typedef enum {
  RtemsRatemonReqResetStatistics_Post_TotalCPUTime_Zero,
  RtemsRatemonReqResetStatistics_Post_TotalCPUTime_NA
} RtemsRatemonReqResetStatistics_Post_TotalCPUTime;

typedef enum {
  RtemsRatemonReqResetStatistics_Post_MinMonotonicTime_Max,
  RtemsRatemonReqResetStatistics_Post_MinMonotonicTime_NA
} RtemsRatemonReqResetStatistics_Post_MinMonotonicTime;

typedef enum {
  RtemsRatemonReqResetStatistics_Post_MaxMonotonicTime_Zero,
  RtemsRatemonReqResetStatistics_Post_MaxMonotonicTime_NA
} RtemsRatemonReqResetStatistics_Post_MaxMonotonicTime;

typedef enum {
  RtemsRatemonReqResetStatistics_Post_TotalMonotonicTime_Zero,
  RtemsRatemonReqResetStatistics_Post_TotalMonotonicTime_NA
} RtemsRatemonReqResetStatistics_Post_TotalMonotonicTime;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Id_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Count : 1;
  uint16_t Post_MissedCount : 1;
  uint16_t Post_MinCPUTime : 1;
  uint16_t Post_MaxCPUTime : 1;
  uint16_t Post_TotalCPUTime : 1;
  uint16_t Post_MinMonotonicTime : 1;
  uint16_t Post_MaxMonotonicTime : 1;
  uint16_t Post_TotalMonotonicTime : 1;
} RtemsRatemonReqResetStatistics_Entry;

/**
 * @brief Test context for spec:/rtems/ratemon/req/reset-statistics test case.
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
   * @brief This member provides the period statistics.
   */
  rtems_rate_monotonic_period_statistics stats;

  /**
   * @brief This member provides the object identifier parameter.
   */
  rtems_id id;

  /**
   * @brief This member contains the return status.
   */
  rtems_status_code status;

  struct {
    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 1 ];

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
    RtemsRatemonReqResetStatistics_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsRatemonReqResetStatistics_Context;

static RtemsRatemonReqResetStatistics_Context
  RtemsRatemonReqResetStatistics_Instance;

static const char *const RtemsRatemonReqResetStatistics_PreDesc_Id[] =
  { "NoObj", "Period", "NA" };

static const char *const *const RtemsRatemonReqResetStatistics_PreDesc[] = {
  RtemsRatemonReqResetStatistics_PreDesc_Id,
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

static void TimespecZero( const struct timespec *ts )
{
  T_eq_ll( ts->tv_sec, 0 );
  T_eq_long( ts->tv_nsec, 0 );
}

static void TimespecMax( const struct timespec *ts )
{
  T_eq_ll( ts->tv_sec, 0x7fffffffLL );
  T_eq_long( ts->tv_nsec, 999999999L );
}

static void TimespecEq( const struct timespec *ts, rtems_interval ticks )
{
  T_eq_ll( ts->tv_sec, 0 );
  T_eq_long(
    ( ts->tv_nsec + 499999L ) / 1000000L,
    ticks * rtems_configuration_get_milliseconds_per_tick()
  );
}

static rtems_id CreatePeriod( void )
{
  rtems_id                               id;
  rtems_rate_monotonic_period_statistics stats;
  rtems_status_code                      sc;

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

  sc = rtems_rate_monotonic_get_statistics( id, &stats );
  T_rsc_success( sc );

  T_eq_u32( stats.count, 3 );
  T_eq_u32( stats.missed_count, 1 );
  TimespecEq( &stats.min_cpu_time, 1 );
  TimespecEq( &stats.max_cpu_time, 4 );
  TimespecEq( &stats.total_cpu_time, 6 );
  TimespecEq( &stats.min_wall_time, 1 );
  TimespecEq( &stats.max_wall_time, 4 );
  TimespecEq( &stats.total_wall_time, 7 );

  return id;
}

static void RtemsRatemonReqResetStatistics_Pre_Id_Prepare(
  RtemsRatemonReqResetStatistics_Context *ctx,
  RtemsRatemonReqResetStatistics_Pre_Id   state
)
{
  switch ( state ) {
    case RtemsRatemonReqResetStatistics_Pre_Id_NoObj: {
      /*
       * While the `id` parameter is not associated with a period.
       */
      ctx->id = 0xffffffff;
      break;
    }

    case RtemsRatemonReqResetStatistics_Pre_Id_Period: {
      /*
       * While the `id` parameter is associated with a period.
       */
      ctx->id = CreatePeriod();
      break;
    }

    case RtemsRatemonReqResetStatistics_Pre_Id_NA:
      break;
  }
}

static void RtemsRatemonReqResetStatistics_Post_Status_Check(
  RtemsRatemonReqResetStatistics_Context    *ctx,
  RtemsRatemonReqResetStatistics_Post_Status state
)
{
  switch ( state ) {
    case RtemsRatemonReqResetStatistics_Post_Status_Ok: {
      /*
       * The return status of rtems_rate_monotonic_reset_statistics() shall be
       * RTEMS_SUCCESSFUL.
       */
      T_rsc_success( ctx->status );
      break;
    }

    case RtemsRatemonReqResetStatistics_Post_Status_InvId: {
      /*
       * The return status of rtems_rate_monotonic_reset_statistics() shall be
       * RTEMS_INVALID_ID.
       */
      T_rsc( ctx->status, RTEMS_INVALID_ID );
      break;
    }

    case RtemsRatemonReqResetStatistics_Post_Status_NA:
      break;
  }
}

static void RtemsRatemonReqResetStatistics_Post_Count_Check(
  RtemsRatemonReqResetStatistics_Context   *ctx,
  RtemsRatemonReqResetStatistics_Post_Count state
)
{
  switch ( state ) {
    case RtemsRatemonReqResetStatistics_Post_Count_Zero: {
      /*
       * The count of executed periods recorded for the period associated with
       * the object identifier specified by the `id` parameter shall be set to
       * zero.
       */
      T_eq_u32( ctx->stats.count, 0 );
      break;
    }

    case RtemsRatemonReqResetStatistics_Post_Count_NA:
      break;
  }
}

static void RtemsRatemonReqResetStatistics_Post_MissedCount_Check(
  RtemsRatemonReqResetStatistics_Context         *ctx,
  RtemsRatemonReqResetStatistics_Post_MissedCount state
)
{
  switch ( state ) {
    case RtemsRatemonReqResetStatistics_Post_MissedCount_Zero: {
      /*
       * The count of missed periods recorded for the period associated with
       * the object identifier specified by the `id` parameter shall be set to
       * zero.
       */
      T_eq_u32( ctx->stats.missed_count, 0 );
      break;
    }

    case RtemsRatemonReqResetStatistics_Post_MissedCount_NA:
      break;
  }
}

static void RtemsRatemonReqResetStatistics_Post_MinCPUTime_Check(
  RtemsRatemonReqResetStatistics_Context        *ctx,
  RtemsRatemonReqResetStatistics_Post_MinCPUTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqResetStatistics_Post_MinCPUTime_Max: {
      /*
       * The least amount of processor time used in a period recorded for the
       * period associated with the object identifier specified by the `id`
       * parameter shall be set to 2147483647 seconds and 999999999
       * nanoseconds.
       */
      TimespecMax( &ctx->stats.min_cpu_time );
      break;
    }

    case RtemsRatemonReqResetStatistics_Post_MinCPUTime_NA:
      break;
  }
}

static void RtemsRatemonReqResetStatistics_Post_MaxCPUTime_Check(
  RtemsRatemonReqResetStatistics_Context        *ctx,
  RtemsRatemonReqResetStatistics_Post_MaxCPUTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqResetStatistics_Post_MaxCPUTime_Zero: {
      /*
       * The highest amount of processor time used in a period recorded for the
       * period associated with the object identifier specified by the `id`
       * parameter shall be set to zero.
       */
      TimespecZero( &ctx->stats.max_cpu_time );
      break;
    }

    case RtemsRatemonReqResetStatistics_Post_MaxCPUTime_NA:
      break;
  }
}

static void RtemsRatemonReqResetStatistics_Post_TotalCPUTime_Check(
  RtemsRatemonReqResetStatistics_Context          *ctx,
  RtemsRatemonReqResetStatistics_Post_TotalCPUTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqResetStatistics_Post_TotalCPUTime_Zero: {
      /*
       * The total amount of processor time used in all periods recorded for
       * the period associated with the object identifier specified by the `id`
       * parameter shall be set to zero.
       */
      TimespecZero( &ctx->stats.total_cpu_time );
      break;
    }

    case RtemsRatemonReqResetStatistics_Post_TotalCPUTime_NA:
      break;
  }
}

static void RtemsRatemonReqResetStatistics_Post_MinMonotonicTime_Check(
  RtemsRatemonReqResetStatistics_Context              *ctx,
  RtemsRatemonReqResetStatistics_Post_MinMonotonicTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqResetStatistics_Post_MinMonotonicTime_Max: {
      /*
       * The least amount of CLOCK_MONOTONIC time used in a period recorded for
       * the period associated with the object identifier specified by the `id`
       * parameter shall be set to 2147483647 seconds and 999999999
       * nanoseconds.
       */
      TimespecMax( &ctx->stats.min_wall_time );
      break;
    }

    case RtemsRatemonReqResetStatistics_Post_MinMonotonicTime_NA:
      break;
  }
}

static void RtemsRatemonReqResetStatistics_Post_MaxMonotonicTime_Check(
  RtemsRatemonReqResetStatistics_Context              *ctx,
  RtemsRatemonReqResetStatistics_Post_MaxMonotonicTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqResetStatistics_Post_MaxMonotonicTime_Zero: {
      /*
       * The highest amount of CLOCK_MONOTONIC time used in a period recorded
       * for the period associated with the object identifier specified by the
       * `id` parameter shall be set to zero.
       */
      TimespecZero( &ctx->stats.max_wall_time );
      break;
    }

    case RtemsRatemonReqResetStatistics_Post_MaxMonotonicTime_NA:
      break;
  }
}

static void RtemsRatemonReqResetStatistics_Post_TotalMonotonicTime_Check(
  RtemsRatemonReqResetStatistics_Context                *ctx,
  RtemsRatemonReqResetStatistics_Post_TotalMonotonicTime state
)
{
  switch ( state ) {
    case RtemsRatemonReqResetStatistics_Post_TotalMonotonicTime_Zero: {
      /*
       * The total amount of CLOCK_MONOTONIC time used in all periods recorded
       * for the period associated with the object identifier specified by the
       * `id` parameter shall be set to zero.
       */
      TimespecZero( &ctx->stats.total_wall_time );
      break;
    }

    case RtemsRatemonReqResetStatistics_Post_TotalMonotonicTime_NA:
      break;
  }
}

static void RtemsRatemonReqResetStatistics_Setup(
  RtemsRatemonReqResetStatistics_Context *ctx
)
{
  ctx->previous_get_timecount = SetGetTimecountHandler( FreezeTime );
  ctx->worker_id = CreateTask( "WORK", GetSelfPriority() + 1 );
  StartTask( ctx->worker_id, TickWorker, NULL );
  TimecounterTick();
}

static void RtemsRatemonReqResetStatistics_Setup_Wrap( void *arg )
{
  RtemsRatemonReqResetStatistics_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsRatemonReqResetStatistics_Setup( ctx );
}

static void RtemsRatemonReqResetStatistics_Teardown(
  RtemsRatemonReqResetStatistics_Context *ctx
)
{
  DeleteTask( ctx->worker_id );
  SetGetTimecountHandler( ctx->previous_get_timecount );
}

static void RtemsRatemonReqResetStatistics_Teardown_Wrap( void *arg )
{
  RtemsRatemonReqResetStatistics_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsRatemonReqResetStatistics_Teardown( ctx );
}

static void RtemsRatemonReqResetStatistics_Action(
  RtemsRatemonReqResetStatistics_Context *ctx
)
{
  ctx->status = rtems_rate_monotonic_reset_statistics( ctx->id );
  memset( &ctx->stats, 0xff, sizeof( ctx->stats ) );
  (void) rtems_rate_monotonic_get_statistics( ctx->id, &ctx->stats );
}

static void RtemsRatemonReqResetStatistics_Cleanup(
  RtemsRatemonReqResetStatistics_Context *ctx
)
{
  if ( ctx->id != 0xffffffff ) {
    rtems_status_code sc;

    sc = rtems_rate_monotonic_delete( ctx->id );
    T_rsc_success( sc );
  }
}

/* clang-format off */

static const RtemsRatemonReqResetStatistics_Entry
RtemsRatemonReqResetStatistics_Entries[] = {
  { 0, 0, RtemsRatemonReqResetStatistics_Post_Status_InvId,
    RtemsRatemonReqResetStatistics_Post_Count_NA,
    RtemsRatemonReqResetStatistics_Post_MissedCount_NA,
    RtemsRatemonReqResetStatistics_Post_MinCPUTime_NA,
    RtemsRatemonReqResetStatistics_Post_MaxCPUTime_NA,
    RtemsRatemonReqResetStatistics_Post_TotalCPUTime_NA,
    RtemsRatemonReqResetStatistics_Post_MinMonotonicTime_NA,
    RtemsRatemonReqResetStatistics_Post_MaxMonotonicTime_NA,
    RtemsRatemonReqResetStatistics_Post_TotalMonotonicTime_NA },
  { 0, 0, RtemsRatemonReqResetStatistics_Post_Status_Ok,
    RtemsRatemonReqResetStatistics_Post_Count_Zero,
    RtemsRatemonReqResetStatistics_Post_MissedCount_Zero,
    RtemsRatemonReqResetStatistics_Post_MinCPUTime_Max,
    RtemsRatemonReqResetStatistics_Post_MaxCPUTime_Zero,
    RtemsRatemonReqResetStatistics_Post_TotalCPUTime_Zero,
    RtemsRatemonReqResetStatistics_Post_MinMonotonicTime_Max,
    RtemsRatemonReqResetStatistics_Post_MaxMonotonicTime_Zero,
    RtemsRatemonReqResetStatistics_Post_TotalMonotonicTime_Zero }
};

static const uint8_t
RtemsRatemonReqResetStatistics_Map[] = {
  0, 1
};

/* clang-format on */

static size_t RtemsRatemonReqResetStatistics_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  RtemsRatemonReqResetStatistics_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsRatemonReqResetStatistics_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsRatemonReqResetStatistics_Fixture = {
  .setup = RtemsRatemonReqResetStatistics_Setup_Wrap,
  .stop = NULL,
  .teardown = RtemsRatemonReqResetStatistics_Teardown_Wrap,
  .scope = RtemsRatemonReqResetStatistics_Scope,
  .initial_context = &RtemsRatemonReqResetStatistics_Instance
};

static inline RtemsRatemonReqResetStatistics_Entry
RtemsRatemonReqResetStatistics_PopEntry(
  RtemsRatemonReqResetStatistics_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsRatemonReqResetStatistics_Entries
    [ RtemsRatemonReqResetStatistics_Map[ index ] ];
}

static void RtemsRatemonReqResetStatistics_TestVariant(
  RtemsRatemonReqResetStatistics_Context *ctx
)
{
  RtemsRatemonReqResetStatistics_Pre_Id_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  RtemsRatemonReqResetStatistics_Action( ctx );
  RtemsRatemonReqResetStatistics_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  RtemsRatemonReqResetStatistics_Post_Count_Check(
    ctx,
    ctx->Map.entry.Post_Count
  );
  RtemsRatemonReqResetStatistics_Post_MissedCount_Check(
    ctx,
    ctx->Map.entry.Post_MissedCount
  );
  RtemsRatemonReqResetStatistics_Post_MinCPUTime_Check(
    ctx,
    ctx->Map.entry.Post_MinCPUTime
  );
  RtemsRatemonReqResetStatistics_Post_MaxCPUTime_Check(
    ctx,
    ctx->Map.entry.Post_MaxCPUTime
  );
  RtemsRatemonReqResetStatistics_Post_TotalCPUTime_Check(
    ctx,
    ctx->Map.entry.Post_TotalCPUTime
  );
  RtemsRatemonReqResetStatistics_Post_MinMonotonicTime_Check(
    ctx,
    ctx->Map.entry.Post_MinMonotonicTime
  );
  RtemsRatemonReqResetStatistics_Post_MaxMonotonicTime_Check(
    ctx,
    ctx->Map.entry.Post_MaxMonotonicTime
  );
  RtemsRatemonReqResetStatistics_Post_TotalMonotonicTime_Check(
    ctx,
    ctx->Map.entry.Post_TotalMonotonicTime
  );
}

/**
 * @fn void T_case_body_RtemsRatemonReqResetStatistics( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsRatemonReqResetStatistics,
  &RtemsRatemonReqResetStatistics_Fixture
)
{
  RtemsRatemonReqResetStatistics_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = RtemsRatemonReqResetStatistics_Pre_Id_NoObj;
    ctx->Map.pcs[ 0 ] < RtemsRatemonReqResetStatistics_Pre_Id_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = RtemsRatemonReqResetStatistics_PopEntry( ctx );
    RtemsRatemonReqResetStatistics_TestVariant( ctx );
    RtemsRatemonReqResetStatistics_Cleanup( ctx );
  }
}

/** @} */
