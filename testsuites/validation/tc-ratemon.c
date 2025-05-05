/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsRatemonValRatemon
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
 * @defgroup RtemsRatemonValRatemon spec:/rtems/ratemon/val/ratemon
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @brief This test case collection provides validation test cases for
 *   requirements of the @ref RTEMSAPIClassicRatemon.
 *
 * This test case performs the following actions:
 *
 * - Create a rate-monotonic object. Reset all rate-monotonic statistics. Get
 *   the statistics of the rate-monotonic object.
 *
 *   - Check that the statistics are in the reset state.
 *
 *   - Delete the rate-monotonic object.
 *
 * @{
 */

/**
 * @brief Test context for spec:/rtems/ratemon/val/ratemon test case.
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
} RtemsRatemonValRatemon_Context;

static RtemsRatemonValRatemon_Context RtemsRatemonValRatemon_Instance;

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

static void TimespecEq( const struct timespec *ts, long ms )
{
  T_eq_ll( ts->tv_sec, 0 );
  T_eq_long( ( ts->tv_nsec + 499999L ) / 1000000L, ms );
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

static void RtemsRatemonValRatemon_Setup( RtemsRatemonValRatemon_Context *ctx )
{
  ctx->previous_get_timecount = SetGetTimecountHandler( FreezeTime );
  ctx->worker_id = CreateTask( "WORK", GetSelfPriority() + 1 );
  StartTask( ctx->worker_id, TickWorker, NULL );
  TimecounterTick();
}

static void RtemsRatemonValRatemon_Setup_Wrap( void *arg )
{
  RtemsRatemonValRatemon_Context *ctx;

  ctx = arg;
  RtemsRatemonValRatemon_Setup( ctx );
}

static void RtemsRatemonValRatemon_Teardown(
  RtemsRatemonValRatemon_Context *ctx
)
{
  DeleteTask( ctx->worker_id );
  SetGetTimecountHandler( ctx->previous_get_timecount );
}

static void RtemsRatemonValRatemon_Teardown_Wrap( void *arg )
{
  RtemsRatemonValRatemon_Context *ctx;

  ctx = arg;
  RtemsRatemonValRatemon_Teardown( ctx );
}

static T_fixture RtemsRatemonValRatemon_Fixture = {
  .setup = RtemsRatemonValRatemon_Setup_Wrap,
  .stop = NULL,
  .teardown = RtemsRatemonValRatemon_Teardown_Wrap,
  .scope = NULL,
  .initial_context = &RtemsRatemonValRatemon_Instance
};

/**
 * @brief Create a rate-monotonic object. Reset all rate-monotonic statistics.
 *   Get the statistics of the rate-monotonic object.
 */
static void RtemsRatemonValRatemon_Action_0( void )
{
  rtems_id                               id;
  rtems_rate_monotonic_period_statistics stats;
  rtems_status_code                      sc;

  id = CreatePeriod();
  rtems_rate_monotonic_reset_all_statistics();

  sc = rtems_rate_monotonic_get_statistics( id, &stats );
  T_rsc_success( sc );

  /*
   * Check that the statistics are in the reset state.
   */
  T_eq_u32( stats.count, 0 );
  T_eq_u32( stats.missed_count, 0 );
  TimespecMax( &stats.min_cpu_time );
  TimespecZero( &stats.max_cpu_time );
  TimespecZero( &stats.total_cpu_time );
  TimespecMax( &stats.min_wall_time );
  TimespecZero( &stats.max_wall_time );
  TimespecZero( &stats.total_wall_time );

  /*
   * Delete the rate-monotonic object.
   */
  sc = rtems_rate_monotonic_delete( id );
  T_rsc_success( sc );
}

/**
 * @fn void T_case_body_RtemsRatemonValRatemon( void )
 */
T_TEST_CASE_FIXTURE( RtemsRatemonValRatemon, &RtemsRatemonValRatemon_Fixture )
{
  RtemsRatemonValRatemon_Action_0();
}

/** @} */
