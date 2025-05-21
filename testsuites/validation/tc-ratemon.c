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

#include <inttypes.h>
#include <rtems.h>
#include <stdarg.h>
#include <string.h>
#include <rtems/bspIo.h>
#include <rtems/printer.h>
#include <rtems/dev/io.h>

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
 * - Reset the rate-monotonic statistics. Create an active period object.
 *   Create an inactive period object. Build the expected report from the
 *   statistics of both periods.
 *
 *   - Call rtems_rate_monotonic_report_statistics_with_plugin() with a printer
 *     which captures the output. Check that the output starts with the header
 *     lines.
 *
 *   - Check that the period lines follow the header lines.
 *
 *   - Call rtems_rate_monotonic_report_statistics(). Check that it prints the
 *     output of rtems_rate_monotonic_report_statistics_with_plugin() through
 *     the printk printer.
 *
 *   - Delete the rate-monotonic objects.
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

static void TimespecEq( const struct timespec *ts, rtems_interval ticks )
{
  T_eq_ll( ts->tv_sec, 0 );
  T_eq_long(
    ( ts->tv_nsec + 499999L ) / 1000000L,
    ticks * rtems_configuration_get_milliseconds_per_tick()
  );
}

static rtems_id CreatePeriod( rtems_name name, uint32_t index )
{
  while ( true ) {
    rtems_id          id;
    rtems_status_code sc;

    sc = rtems_rate_monotonic_create( name, &id );
    T_rsc_success( sc );

    if ( rtems_object_id_get_index( id ) == index ) {
      return id;
    }

    sc = rtems_rate_monotonic_delete( id );
    T_rsc_success( sc );
  }
}

static void InitializePeriod( rtems_id id )
{
  rtems_rate_monotonic_period_statistics stats;
  rtems_status_code                      sc;

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
}

#define HEADER                                                                  \
  "Period information by period\n"                                              \
  "--- CPU times are in seconds ---\n"                                          \
  "--- Wall times are in seconds ---\n"                                         \
  "   ID     OWNER COUNT MISSED          CPU TIME                  WALL TIME\n" \
  "                                    MIN/MAX/AVG                MIN/MAX/AVG\n"

typedef struct {
  char   buffer[ 1024 ];
  size_t size;
} Output;

static Output output;

static Output expected;

static Output printk_output;

static BSP_output_char_function_type output_char;

static void ResetOutput( Output *out )
{
  memset( out->buffer, 0, sizeof( out->buffer ) );
  out->size = 0;
}

static void PutChar( Output *out, char c )
{
  if ( out->size < sizeof( out->buffer ) - 1 ) {
    out->buffer[ out->size ] = c;
    ++out->size;
  }
}

static void PutCharOfPrinter( int c, void *arg )
{
  PutChar( arg, (char) c );
}

static int Capture( void *arg, const char *format, va_list ap )
{
  return _IO_Vprintf( PutCharOfPrinter, arg, format, ap );
}

static void OutputChar( char c )
{
  /* The printk printer outputs a carriage return before each newline */
  if ( c != '\r' ) {
    PutChar( &printk_output, c );
  }
}

static void Append( Output *out, const char *format, ... )
{
  va_list ap;

  va_start( ap, format );
  (void) _IO_Vprintf( PutCharOfPrinter, out, format, ap );
  va_end( ap );
  T_quiet_lt_sz( out->size, sizeof( out->buffer ) - 1 );
}

static void AppendTime( Output *out, const struct timespec *ts )
{
  Append( out, "%lld.%06ld", (long long) ts->tv_sec, ts->tv_nsec / 1000 );
}

static void AppendTimes(
  Output                *out,
  const struct timespec *min,
  const struct timespec *max,
  const struct timespec *total,
  uint32_t               count
)
{
  struct timespec avg;
  int64_t         ns;

  ns = (int64_t) total->tv_sec * 1000000000 + total->tv_nsec;
  ns /= count;
  avg.tv_sec = (time_t) ( ns / 1000000000 );
  avg.tv_nsec = (long) ( ns % 1000000000 );
  AppendTime( out, min );
  Append( out, "/" );
  AppendTime( out, max );
  Append( out, "/" );
  AppendTime( out, &avg );
}

static void AppendPeriodLine( Output *out, rtems_id id )
{
  rtems_rate_monotonic_period_statistics stats;
  rtems_rate_monotonic_period_status     status;
  rtems_status_code                      sc;
  char                                   name[ 5 ];

  sc = rtems_rate_monotonic_get_statistics( id, &stats );
  T_quiet_rsc_success( sc );
  sc = rtems_rate_monotonic_get_status( id, &status );
  T_quiet_rsc_success( sc );
  (void) rtems_object_get_name( status.owner, sizeof( name ), name );
  Append(
    out,
    "0x%08" PRIx32 " %4s %5" PRIu32 " %6" PRIu32 " ",
    id,
    name,
    stats.count,
    stats.missed_count
  );

  if ( stats.count != 0 ) {
    AppendTimes(
      out,
      &stats.min_cpu_time,
      &stats.max_cpu_time,
      &stats.total_cpu_time,
      stats.count
    );
    Append( out, " " );
    AppendTimes(
      out,
      &stats.min_wall_time,
      &stats.max_wall_time,
      &stats.total_wall_time,
      stats.count
    );
  }

  Append( out, "\n" );
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

  id = CreatePeriod( OBJECT_NAME, 2 );
  InitializePeriod( id );
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
 * @brief Reset the rate-monotonic statistics. Create an active period object.
 *   Create an inactive period object. Build the expected report from the
 *   statistics of both periods.
 */
static void RtemsRatemonValRatemon_Action_1( void )
{
  rtems_id          active;
  rtems_id          inactive;
  rtems_status_code sc;
  rtems_printer     printer;

  rtems_rate_monotonic_reset_all_statistics();
  active = CreatePeriod( rtems_build_name( 'A', 'C', 'T', 'V' ), 2 );
  InitializePeriod( active );
  inactive = CreatePeriod( rtems_build_name( 'I', 'A', 'C', 'T' ), 3 );
  ResetOutput( &expected );
  Append( &expected, "%s", HEADER );
  AppendPeriodLine( &expected, active );
  AppendPeriodLine( &expected, inactive );

  /*
   * Call rtems_rate_monotonic_report_statistics_with_plugin() with a printer
   * which captures the output. Check that the output starts with the header
   * lines.
   */
  ResetOutput( &output );
  printer.context = &output;
  printer.printer = Capture;
  rtems_rate_monotonic_report_statistics_with_plugin( &printer );
  T_eq_nstr( output.buffer, HEADER, sizeof( HEADER ) - 1 );

  /*
   * Check that the period lines follow the header lines.
   */
  T_eq_str( output.buffer, expected.buffer );

  /*
   * Call rtems_rate_monotonic_report_statistics(). Check that it prints the
   * output of rtems_rate_monotonic_report_statistics_with_plugin() through the
   * printk printer.
   */
  ResetOutput( &printk_output );
  output_char = BSP_output_char;
  BSP_output_char = OutputChar;
  rtems_rate_monotonic_report_statistics();
  BSP_output_char = output_char;
  T_eq_str( printk_output.buffer, expected.buffer );

  /*
   * Delete the rate-monotonic objects.
   */
  sc = rtems_rate_monotonic_delete( active );
  T_rsc_success( sc );
  sc = rtems_rate_monotonic_delete( inactive );
  T_rsc_success( sc );
}

/**
 * @fn void T_case_body_RtemsRatemonValRatemon( void )
 */
T_TEST_CASE_FIXTURE( RtemsRatemonValRatemon, &RtemsRatemonValRatemon_Fixture )
{
  RtemsRatemonValRatemon_Action_0();
  RtemsRatemonValRatemon_Action_1();
}

/** @} */
