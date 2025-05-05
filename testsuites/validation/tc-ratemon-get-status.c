/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsRatemonReqGetStatus
 */

/*
 * Copyright (C) 2021, 2025 embedded brains GmbH & Co. KG
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
 * @defgroup RtemsRatemonReqGetStatus spec:/rtems/ratemon/req/get-status
 *
 * @ingroup TestsuitesValidationNoClock0
 * @ingroup TestsuitesValidationOneCpu0
 *
 * @{
 */

typedef enum {
  RtemsRatemonReqGetStatus_Pre_Status_Valid,
  RtemsRatemonReqGetStatus_Pre_Status_Null,
  RtemsRatemonReqGetStatus_Pre_Status_NA
} RtemsRatemonReqGetStatus_Pre_Status;

typedef enum {
  RtemsRatemonReqGetStatus_Pre_Id_NoObj,
  RtemsRatemonReqGetStatus_Pre_Id_Period,
  RtemsRatemonReqGetStatus_Pre_Id_NA
} RtemsRatemonReqGetStatus_Pre_Id;

typedef enum {
  RtemsRatemonReqGetStatus_Pre_PostponedJobs_Zero,
  RtemsRatemonReqGetStatus_Pre_PostponedJobs_Positive,
  RtemsRatemonReqGetStatus_Pre_PostponedJobs_NA
} RtemsRatemonReqGetStatus_Pre_PostponedJobs;

typedef enum {
  RtemsRatemonReqGetStatus_Pre_State_Inactive,
  RtemsRatemonReqGetStatus_Pre_State_Active,
  RtemsRatemonReqGetStatus_Pre_State_Expired,
  RtemsRatemonReqGetStatus_Pre_State_NA
} RtemsRatemonReqGetStatus_Pre_State;

typedef enum {
  RtemsRatemonReqGetStatus_Post_Status_Ok,
  RtemsRatemonReqGetStatus_Post_Status_InvAddr,
  RtemsRatemonReqGetStatus_Post_Status_InvId,
  RtemsRatemonReqGetStatus_Post_Status_NA
} RtemsRatemonReqGetStatus_Post_Status;

typedef enum {
  RtemsRatemonReqGetStatus_Post_Owner_OwnerTask,
  RtemsRatemonReqGetStatus_Post_Owner_Nop,
  RtemsRatemonReqGetStatus_Post_Owner_NA
} RtemsRatemonReqGetStatus_Post_Owner;

typedef enum {
  RtemsRatemonReqGetStatus_Post_State_Inactive,
  RtemsRatemonReqGetStatus_Post_State_Active,
  RtemsRatemonReqGetStatus_Post_State_Expired,
  RtemsRatemonReqGetStatus_Post_State_Nop,
  RtemsRatemonReqGetStatus_Post_State_NA
} RtemsRatemonReqGetStatus_Post_State;

typedef enum {
  RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Zero,
  RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Elapsed,
  RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Nop,
  RtemsRatemonReqGetStatus_Post_SinceLastPeriod_NA
} RtemsRatemonReqGetStatus_Post_SinceLastPeriod;

typedef enum {
  RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Zero,
  RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Executed,
  RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Nop,
  RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_NA
} RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod;

typedef enum {
  RtemsRatemonReqGetStatus_Post_PostponedJobs_Zero,
  RtemsRatemonReqGetStatus_Post_PostponedJobs_Count,
  RtemsRatemonReqGetStatus_Post_PostponedJobs_Nop,
  RtemsRatemonReqGetStatus_Post_PostponedJobs_NA
} RtemsRatemonReqGetStatus_Post_PostponedJobs;

typedef struct {
  uint32_t Skip : 1;
  uint32_t Pre_Status_NA : 1;
  uint32_t Pre_Id_NA : 1;
  uint32_t Pre_PostponedJobs_NA : 1;
  uint32_t Pre_State_NA : 1;
  uint32_t Post_Status : 2;
  uint32_t Post_Owner : 2;
  uint32_t Post_State : 3;
  uint32_t Post_SinceLastPeriod : 2;
  uint32_t Post_ExecutedSinceLastPeriod : 2;
  uint32_t Post_PostponedJobs : 2;
} RtemsRatemonReqGetStatus_Entry;

/**
 * @brief Test context for spec:/rtems/ratemon/req/get-status test case.
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
   * @brief This member provides the expected since last period.
   */
  rtems_interval expected_since_last_period;

  /**
   * @brief This member provides the expected executed since last period.
   */
  rtems_interval expected_executed_since_last_period;

  /**
   * @brief This member provides the expected postponed job count.
   */
  uint32_t expected_postponed_jobs_count;

  /**
   * @brief This member provides the expected period status.
   */
  rtems_rate_monotonic_period_status status_obj;

  /**
   * @brief This member provides the object identifier parameter.
   */
  rtems_id id;

  /**
   * @brief This member provides the period status parameter.
   */
  rtems_rate_monotonic_period_status *period_status;

  /**
   * @brief This member contains the return status.
   */
  rtems_status_code status;

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 4 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 4 ];

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
    RtemsRatemonReqGetStatus_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsRatemonReqGetStatus_Context;

static RtemsRatemonReqGetStatus_Context RtemsRatemonReqGetStatus_Instance;

static const char *const RtemsRatemonReqGetStatus_PreDesc_Status[] =
  { "Valid", "Null", "NA" };

static const char *const RtemsRatemonReqGetStatus_PreDesc_Id[] =
  { "NoObj", "Period", "NA" };

static const char *const RtemsRatemonReqGetStatus_PreDesc_PostponedJobs[] =
  { "Zero", "Positive", "NA" };

static const char *const RtemsRatemonReqGetStatus_PreDesc_State[] =
  { "Inactive", "Active", "Expired", "NA" };

static const char *const *const RtemsRatemonReqGetStatus_PreDesc[] = {
  RtemsRatemonReqGetStatus_PreDesc_Status,
  RtemsRatemonReqGetStatus_PreDesc_Id,
  RtemsRatemonReqGetStatus_PreDesc_PostponedJobs,
  RtemsRatemonReqGetStatus_PreDesc_State,
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

  return id;
}

static void RtemsRatemonReqGetStatus_Pre_Status_Prepare(
  RtemsRatemonReqGetStatus_Context   *ctx,
  RtemsRatemonReqGetStatus_Pre_Status state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatus_Pre_Status_Valid: {
      /*
       * While the `status` parameter references an object of type
       * rtems_rate_monotonic_period_status.
       */
      ctx->period_status = &ctx->status_obj;
      break;
    }

    case RtemsRatemonReqGetStatus_Pre_Status_Null: {
      /*
       * While the `status` parameter is equal to NULL.
       */
      ctx->period_status = NULL;
      break;
    }

    case RtemsRatemonReqGetStatus_Pre_Status_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatus_Pre_Id_Prepare(
  RtemsRatemonReqGetStatus_Context *ctx,
  RtemsRatemonReqGetStatus_Pre_Id   state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatus_Pre_Id_NoObj: {
      /*
       * While the `id` parameter is not associated with a period.
       */
      ctx->id = 0xffffffff;
      break;
    }

    case RtemsRatemonReqGetStatus_Pre_Id_Period: {
      /*
       * While the `id` parameter is associated with a period.
       */
      ctx->id = CreatePeriod();
      break;
    }

    case RtemsRatemonReqGetStatus_Pre_Id_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatus_Pre_PostponedJobs_Prepare(
  RtemsRatemonReqGetStatus_Context          *ctx,
  RtemsRatemonReqGetStatus_Pre_PostponedJobs state
)
{
  rtems_status_code sc;

  switch ( state ) {
    case RtemsRatemonReqGetStatus_Pre_PostponedJobs_Zero: {
      /*
       * While the count of postponed jobs of the period associated with the
       * object identifier specified by the `id` parameter is zero.
       */
      /* Nothing to prepare */
      break;
    }

    case RtemsRatemonReqGetStatus_Pre_PostponedJobs_Positive: {
      /*
       * While the count of postponed jobs of the period associated with the
       * object identifier specified by the `id` parameter is greater than
       * zero.
       */
      sc = rtems_rate_monotonic_period( ctx->id, 3 );
      T_rsc_success( sc );

      TimecounterTick();
      TimecounterTick();
      TimecounterTick();

      sc = rtems_task_wake_after( 1 );
      T_rsc_success( sc );

      TimecounterTick();
      TimecounterTick();

      ctx->expected_postponed_jobs_count = 2;
      break;
    }

    case RtemsRatemonReqGetStatus_Pre_PostponedJobs_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatus_Pre_State_Prepare(
  RtemsRatemonReqGetStatus_Context  *ctx,
  RtemsRatemonReqGetStatus_Pre_State state
)
{
  rtems_status_code sc;

  switch ( state ) {
    case RtemsRatemonReqGetStatus_Pre_State_Inactive: {
      /*
       * While the state of the period associated with the object identifier
       * specified by the `id` parameter is inactive.
       */
      /* Nothing to prepare */
      break;
    }

    case RtemsRatemonReqGetStatus_Pre_State_Active: {
      /*
       * While the state of the period associated with the object identifier
       * specified by the `id` parameter is active.
       */
      sc = rtems_rate_monotonic_period( ctx->id, 3 );

      if ( ctx->expected_postponed_jobs_count > 0 ) {
        T_rsc( sc, RTEMS_TIMEOUT );
      } else {
        T_rsc_success( sc );
      }

      sc = rtems_task_wake_after( 1 );
      T_rsc_success( sc );

      TimecounterTick();

      if ( ctx->expected_postponed_jobs_count > 0 ) {
        --ctx->expected_postponed_jobs_count;
      }

      ctx->expected_since_last_period = 2;
      ctx->expected_executed_since_last_period = 1;
      break;
    }

    case RtemsRatemonReqGetStatus_Pre_State_Expired: {
      /*
       * While the state of the period associated with the object identifier
       * specified by the `id` parameter is expired.
       */
      if ( ctx->expected_postponed_jobs_count == 0 ) {
        sc = rtems_rate_monotonic_period( ctx->id, 3 );
        T_rsc_success( sc );

        sc = rtems_task_wake_after( 1 );
        T_rsc_success( sc );

        TimecounterTick();
        TimecounterTick();

        ctx->expected_since_last_period = 3;
        ctx->expected_executed_since_last_period = 2;
        ctx->expected_postponed_jobs_count = 1;
      } else {
        ctx->expected_since_last_period = 6;
        ctx->expected_executed_since_last_period = 5;
      }
      break;
    }

    case RtemsRatemonReqGetStatus_Pre_State_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatus_Post_Status_Check(
  RtemsRatemonReqGetStatus_Context    *ctx,
  RtemsRatemonReqGetStatus_Post_Status state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatus_Post_Status_Ok: {
      /*
       * The return status of rtems_rate_monotonic_get_status() shall be
       * RTEMS_SUCCESSFUL.
       */
      T_rsc_success( ctx->status );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_Status_InvAddr: {
      /*
       * The return status of rtems_rate_monotonic_get_status() shall be
       * RTEMS_INVALID_ADDRESS.
       */
      T_rsc( ctx->status, RTEMS_INVALID_ADDRESS );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_Status_InvId: {
      /*
       * The return status of rtems_rate_monotonic_get_status() shall be
       * RTEMS_INVALID_ID.
       */
      T_rsc( ctx->status, RTEMS_INVALID_ID );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_Status_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatus_Post_Owner_Check(
  RtemsRatemonReqGetStatus_Context   *ctx,
  RtemsRatemonReqGetStatus_Post_Owner state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatus_Post_Owner_OwnerTask: {
      /*
       * The value of the member owner of the object referenced by the `status`
       * parameter shall be set to the object identifier of the owner task of
       * the period associated with the object identifier specified by the `id`
       * parameter.
       */
      T_eq_u32( ctx->status_obj.owner, rtems_task_self() );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_Owner_Nop: {
      /*
       * The value of the member owner of the object referenced by the `status`
       * parameter shall not be modified.
       */
      T_eq_u32( ctx->status_obj.owner, 0xffffffff );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_Owner_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatus_Post_State_Check(
  RtemsRatemonReqGetStatus_Context   *ctx,
  RtemsRatemonReqGetStatus_Post_State state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatus_Post_State_Inactive: {
      /*
       * The value of the member state of the object referenced by the `status`
       * parameter shall be set to RATE_MONOTONIC_INACTIVE.
       */
      T_eq_int( ctx->status_obj.state, RATE_MONOTONIC_INACTIVE );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_State_Active: {
      /*
       * The value of the member state of the object referenced by the `status`
       * parameter shall be set to RATE_MONOTONIC_ACTIVE.
       */
      T_eq_int( ctx->status_obj.state, RATE_MONOTONIC_ACTIVE );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_State_Expired: {
      /*
       * The value of the member state of the object referenced by the `status`
       * parameter shall be set to RATE_MONOTONIC_EXPIRED.
       */
      T_eq_int( ctx->status_obj.state, RATE_MONOTONIC_EXPIRED );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_State_Nop: {
      /*
       * The value of the member state of the object referenced by the `status`
       * parameter shall not be modified.
       */
      T_eq_int( ctx->status_obj.state, -1 );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_State_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Check(
  RtemsRatemonReqGetStatus_Context             *ctx,
  RtemsRatemonReqGetStatus_Post_SinceLastPeriod state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Zero: {
      /*
       * The value of the member since_last_period of the object referenced by
       * the `status` parameter shall be set to zero.
       */
      TimespecZero( &ctx->status_obj.since_last_period );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Elapsed: {
      /*
       * The value of the member since_last_period of the object referenced by
       * the `status` parameter shall be set to the CLOCK_MONOTONIC time
       * elapsed since the last invocation of rtems_rate_monotonic_period() for
       * the period associated with the object identifier specified by the `id`
       * parameter.
       */
      TimespecEq(
        &ctx->status_obj.since_last_period,
        ctx->expected_since_last_period
      );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Nop: {
      /*
       * The value of the member since_last_period of the object referenced by
       * the `status` parameter shall not be modified.
       */
      TimespecNop( &ctx->status_obj.since_last_period );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_SinceLastPeriod_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Check(
  RtemsRatemonReqGetStatus_Context                     *ctx,
  RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Zero: {
      /*
       * The value of the member executed_since_last_period of the object
       * referenced by the `status` parameter shall be set to zero.
       */
      TimespecZero( &ctx->status_obj.executed_since_last_period );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Executed: {
      /*
       * The value of the member executed_since_last_period of the object
       * referenced by the `status` parameter shall be set to the processor
       * time consumed by the owner task since the last invocation of
       * rtems_rate_monotonic_period() for the period associated with the
       * object identifier specified by the `id` parameter.
       */
      TimespecEq(
        &ctx->status_obj.executed_since_last_period,
        ctx->expected_executed_since_last_period
      );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Nop: {
      /*
       * The value of the member executed_since_last_period of the object
       * referenced by the `status` parameter shall not be modified.
       */
      TimespecNop( &ctx->status_obj.executed_since_last_period );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatus_Post_PostponedJobs_Check(
  RtemsRatemonReqGetStatus_Context           *ctx,
  RtemsRatemonReqGetStatus_Post_PostponedJobs state
)
{
  switch ( state ) {
    case RtemsRatemonReqGetStatus_Post_PostponedJobs_Zero: {
      /*
       * The value of the member postponed_jobs_count of the object referenced
       * by the `status` parameter shall be set to zero.
       */
      T_eq_u32( ctx->status_obj.postponed_jobs_count, 0 );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_PostponedJobs_Count: {
      /*
       * The value of the member postponed_jobs_count of the object referenced
       * by the `status` parameter shall be set to the count of postponed jobs
       * of the period associated with the object identifier specified by the
       * `id` parameter.
       */
      T_eq_u32(
        ctx->status_obj.postponed_jobs_count,
        ctx->expected_postponed_jobs_count
      );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_PostponedJobs_Nop: {
      /*
       * The value of the member postponed_jobs_count of the object referenced
       * by the `status` parameter shall not be modified.
       */
      T_eq_u32( ctx->status_obj.postponed_jobs_count, 0xffffffff );
      break;
    }

    case RtemsRatemonReqGetStatus_Post_PostponedJobs_NA:
      break;
  }
}

static void RtemsRatemonReqGetStatus_Setup(
  RtemsRatemonReqGetStatus_Context *ctx
)
{
  ctx->previous_get_timecount = SetGetTimecountHandler( FreezeTime );
  ctx->worker_id = CreateTask( "WORK", GetSelfPriority() + 1 );
  StartTask( ctx->worker_id, TickWorker, NULL );
  TimecounterTick();
}

static void RtemsRatemonReqGetStatus_Setup_Wrap( void *arg )
{
  RtemsRatemonReqGetStatus_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsRatemonReqGetStatus_Setup( ctx );
}

static void RtemsRatemonReqGetStatus_Teardown(
  RtemsRatemonReqGetStatus_Context *ctx
)
{
  DeleteTask( ctx->worker_id );
  SetGetTimecountHandler( ctx->previous_get_timecount );
}

static void RtemsRatemonReqGetStatus_Teardown_Wrap( void *arg )
{
  RtemsRatemonReqGetStatus_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsRatemonReqGetStatus_Teardown( ctx );
}

static void RtemsRatemonReqGetStatus_Prepare(
  RtemsRatemonReqGetStatus_Context *ctx
)
{
  ctx->expected_since_last_period = 0;
  ctx->expected_executed_since_last_period = 0;
  ctx->expected_postponed_jobs_count = 0;
  memset( &ctx->status_obj, 0xff, sizeof( ctx->status_obj ) );
}

static void RtemsRatemonReqGetStatus_Action(
  RtemsRatemonReqGetStatus_Context *ctx
)
{
  ctx->status = rtems_rate_monotonic_get_status( ctx->id, ctx->period_status );
}

static void RtemsRatemonReqGetStatus_Cleanup(
  RtemsRatemonReqGetStatus_Context *ctx
)
{
  if ( ctx->id != 0xffffffff ) {
    rtems_status_code sc;

    sc = rtems_rate_monotonic_delete( ctx->id );
    T_rsc_success( sc );
  }
}

/* clang-format off */

static const RtemsRatemonReqGetStatus_Entry
RtemsRatemonReqGetStatus_Entries[] = {
  { 1, 0, 0, 0, 0, RtemsRatemonReqGetStatus_Post_Status_NA,
    RtemsRatemonReqGetStatus_Post_Owner_NA,
    RtemsRatemonReqGetStatus_Post_State_NA,
    RtemsRatemonReqGetStatus_Post_SinceLastPeriod_NA,
    RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_NA,
    RtemsRatemonReqGetStatus_Post_PostponedJobs_NA },
  { 0, 0, 0, 0, 0, RtemsRatemonReqGetStatus_Post_Status_InvAddr,
    RtemsRatemonReqGetStatus_Post_Owner_NA,
    RtemsRatemonReqGetStatus_Post_State_NA,
    RtemsRatemonReqGetStatus_Post_SinceLastPeriod_NA,
    RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_NA,
    RtemsRatemonReqGetStatus_Post_PostponedJobs_NA },
  { 0, 0, 0, 0, 0, RtemsRatemonReqGetStatus_Post_Status_Ok,
    RtemsRatemonReqGetStatus_Post_Owner_OwnerTask,
    RtemsRatemonReqGetStatus_Post_State_Active,
    RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Elapsed,
    RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Executed,
    RtemsRatemonReqGetStatus_Post_PostponedJobs_Count },
  { 0, 0, 0, 0, 0, RtemsRatemonReqGetStatus_Post_Status_Ok,
    RtemsRatemonReqGetStatus_Post_Owner_OwnerTask,
    RtemsRatemonReqGetStatus_Post_State_Expired,
    RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Elapsed,
    RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Executed,
    RtemsRatemonReqGetStatus_Post_PostponedJobs_Count },
  { 0, 0, 0, 1, 1, RtemsRatemonReqGetStatus_Post_Status_InvId,
    RtemsRatemonReqGetStatus_Post_Owner_Nop,
    RtemsRatemonReqGetStatus_Post_State_Nop,
    RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Nop,
    RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Nop,
    RtemsRatemonReqGetStatus_Post_PostponedJobs_Nop },
  { 0, 0, 0, 1, 0, RtemsRatemonReqGetStatus_Post_Status_Ok,
    RtemsRatemonReqGetStatus_Post_Owner_OwnerTask,
    RtemsRatemonReqGetStatus_Post_State_Inactive,
    RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Zero,
    RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Zero,
    RtemsRatemonReqGetStatus_Post_PostponedJobs_Zero },
  { 0, 0, 0, 1, 1, RtemsRatemonReqGetStatus_Post_Status_InvAddr,
    RtemsRatemonReqGetStatus_Post_Owner_NA,
    RtemsRatemonReqGetStatus_Post_State_NA,
    RtemsRatemonReqGetStatus_Post_SinceLastPeriod_NA,
    RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_NA,
    RtemsRatemonReqGetStatus_Post_PostponedJobs_NA },
  { 0, 0, 0, 1, 0, RtemsRatemonReqGetStatus_Post_Status_InvAddr,
    RtemsRatemonReqGetStatus_Post_Owner_NA,
    RtemsRatemonReqGetStatus_Post_State_NA,
    RtemsRatemonReqGetStatus_Post_SinceLastPeriod_NA,
    RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_NA,
    RtemsRatemonReqGetStatus_Post_PostponedJobs_NA }
};

static const uint8_t
RtemsRatemonReqGetStatus_Map[] = {
  4, 0, 0, 0, 0, 0, 5, 2, 3, 0, 2, 3, 6, 0, 0, 0, 0, 0, 7, 1, 1, 0, 1, 1
};

/* clang-format on */

static size_t RtemsRatemonReqGetStatus_Scope( void *arg, char *buf, size_t n )
{
  RtemsRatemonReqGetStatus_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsRatemonReqGetStatus_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsRatemonReqGetStatus_Fixture = {
  .setup = RtemsRatemonReqGetStatus_Setup_Wrap,
  .stop = NULL,
  .teardown = RtemsRatemonReqGetStatus_Teardown_Wrap,
  .scope = RtemsRatemonReqGetStatus_Scope,
  .initial_context = &RtemsRatemonReqGetStatus_Instance
};

static inline RtemsRatemonReqGetStatus_Entry RtemsRatemonReqGetStatus_PopEntry(
  RtemsRatemonReqGetStatus_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsRatemonReqGetStatus_Entries
    [ RtemsRatemonReqGetStatus_Map[ index ] ];
}

static void RtemsRatemonReqGetStatus_SetPreConditionStates(
  RtemsRatemonReqGetStatus_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];

  if ( ctx->Map.entry.Pre_PostponedJobs_NA ) {
    ctx->Map.pcs[ 2 ] = RtemsRatemonReqGetStatus_Pre_PostponedJobs_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  if ( ctx->Map.entry.Pre_State_NA ) {
    ctx->Map.pcs[ 3 ] = RtemsRatemonReqGetStatus_Pre_State_NA;
  } else {
    ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  }
}

static void RtemsRatemonReqGetStatus_TestVariant(
  RtemsRatemonReqGetStatus_Context *ctx
)
{
  RtemsRatemonReqGetStatus_Pre_Status_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  RtemsRatemonReqGetStatus_Pre_Id_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  RtemsRatemonReqGetStatus_Pre_PostponedJobs_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  RtemsRatemonReqGetStatus_Pre_State_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  RtemsRatemonReqGetStatus_Action( ctx );
  RtemsRatemonReqGetStatus_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  RtemsRatemonReqGetStatus_Post_Owner_Check( ctx, ctx->Map.entry.Post_Owner );
  RtemsRatemonReqGetStatus_Post_State_Check( ctx, ctx->Map.entry.Post_State );
  RtemsRatemonReqGetStatus_Post_SinceLastPeriod_Check(
    ctx,
    ctx->Map.entry.Post_SinceLastPeriod
  );
  RtemsRatemonReqGetStatus_Post_ExecutedSinceLastPeriod_Check(
    ctx,
    ctx->Map.entry.Post_ExecutedSinceLastPeriod
  );
  RtemsRatemonReqGetStatus_Post_PostponedJobs_Check(
    ctx,
    ctx->Map.entry.Post_PostponedJobs
  );
}

/**
 * @fn void T_case_body_RtemsRatemonReqGetStatus( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsRatemonReqGetStatus,
  &RtemsRatemonReqGetStatus_Fixture
)
{
  RtemsRatemonReqGetStatus_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = RtemsRatemonReqGetStatus_Pre_Status_Valid;
    ctx->Map.pci[ 0 ] < RtemsRatemonReqGetStatus_Pre_Status_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = RtemsRatemonReqGetStatus_Pre_Id_NoObj;
      ctx->Map.pci[ 1 ] < RtemsRatemonReqGetStatus_Pre_Id_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = RtemsRatemonReqGetStatus_Pre_PostponedJobs_Zero;
        ctx->Map.pci[ 2 ] < RtemsRatemonReqGetStatus_Pre_PostponedJobs_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = RtemsRatemonReqGetStatus_Pre_State_Inactive;
          ctx->Map.pci[ 3 ] < RtemsRatemonReqGetStatus_Pre_State_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = RtemsRatemonReqGetStatus_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          RtemsRatemonReqGetStatus_SetPreConditionStates( ctx );
          RtemsRatemonReqGetStatus_Prepare( ctx );
          RtemsRatemonReqGetStatus_TestVariant( ctx );
          RtemsRatemonReqGetStatus_Cleanup( ctx );
        }
      }
    }
  }
}

/** @} */
