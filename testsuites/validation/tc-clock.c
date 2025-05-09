/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsClockValClock
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

#include "ts-config.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RtemsClockValClock spec:/rtems/clock/val/clock
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @brief Tests some @ref RTEMSAPIClassicClock directives.
 *
 * This test case performs the following actions:
 *
 * - Use the rtems_clock_get_ticks_since_boot() directive before and after
 *   exactly one clock tick.
 *
 *   - Check that clock tick gets incremented.
 *
 * - Use the rtems_clock_get_ticks_since_boot() directive before and after
 *   exactly one clock tick.
 *
 *   - Check that clock tick gets incremented.
 *
 * - Use the rtems_clock_get_ticks_per_second() directive.
 *
 *   - Check that rtems_clock_get_ticks_per_second() actually returns 1us /
 *     CONFIGURE_MICROSECONDS_PER_TICK.
 *
 * - Use the rtems_clock_get_ticks_per_second() directive.
 *
 *   - Check that rtems_clock_get_ticks_per_second() actually returns 1us /
 *     CONFIGURE_MICROSECONDS_PER_TICK.
 *
 * - Use the rtems_clock_get_uptime_nanoseconds() directive before and after
 *   exactly one timecounter tick. Between the two calls, advance
 *   CLOCK_REALTIME by twelve years.
 *
 *   - Check that the rtems_clock_set() call for 1988-01-01 was successful.
 *
 *   - Check that the rtems_clock_set() call for 2000-01-01 was successful.
 *
 *   - Check that the elapsed time in milliseconds is the length of one clock
 *     tick. This shows that the change of CLOCK_REALTIME has no effect on the
 *     result.
 *
 * @{
 */

/**
 * @brief Use the rtems_clock_get_ticks_since_boot() directive before and after
 *   exactly one clock tick.
 */
static void RtemsClockValClock_Action_0( void )
{
  rtems_interval result_0;
  rtems_interval result_1;

  result_0 = rtems_clock_get_ticks_since_boot();
  ClockTick();
  result_1 = rtems_clock_get_ticks_since_boot();

  /*
   * Check that clock tick gets incremented.
   */
  T_step_eq_u32( 0, result_1 - result_0, 1 );
}

/**
 * @brief Use the rtems_clock_get_ticks_since_boot() directive before and after
 *   exactly one clock tick.
 */
static void RtemsClockValClock_Action_1( void )
{
  rtems_interval result_0;
  rtems_interval result_1;

  #undef rtems_clock_get_ticks_since_boot

  result_0 = rtems_clock_get_ticks_since_boot();
  ClockTick();
  result_1 = rtems_clock_get_ticks_since_boot();

  /*
   * Check that clock tick gets incremented.
   */
  T_step_eq_u32( 1, result_1 - result_0, 1 );
}

/**
 * @brief Use the rtems_clock_get_ticks_per_second() directive.
 */
static void RtemsClockValClock_Action_2( void )
{
  rtems_interval result;

  result = rtems_clock_get_ticks_per_second();

  /*
   * Check that rtems_clock_get_ticks_per_second() actually returns 1us /
   * CONFIGURE_MICROSECONDS_PER_TICK.
   */
  T_step_eq_u32( 2, result, 1000000UL / TEST_MICROSECONDS_PER_TICK );
}

/**
 * @brief Use the rtems_clock_get_ticks_per_second() directive.
 */
static void RtemsClockValClock_Action_3( void )
{
  rtems_interval result;

  #undef rtems_clock_get_ticks_per_second

  result = rtems_clock_get_ticks_per_second();

  /*
   * Check that rtems_clock_get_ticks_per_second() actually returns 1us /
   * CONFIGURE_MICROSECONDS_PER_TICK.
   */
  T_step_eq_u32( 3, result, 1000000UL / TEST_MICROSECONDS_PER_TICK );
}

/**
 * @brief Use the rtems_clock_get_uptime_nanoseconds() directive before and
 *   after exactly one timecounter tick. Between the two calls, advance
 *   CLOCK_REALTIME by twelve years.
 */
static void RtemsClockValClock_Action_4( void )
{
  rtems_time_of_day tod_0 = { 1988, 1, 1, 0, 0, 0, 0 };
  rtems_time_of_day tod_1 = { 2000, 1, 1, 0, 0, 0, 0 };
  rtems_status_code sc_0;
  rtems_status_code sc_1;
  uint64_t          result_0;
  uint64_t          result_1;

  sc_0 = rtems_clock_set( &tod_0 );
  result_0 = rtems_clock_get_uptime_nanoseconds();
  sc_1 = rtems_clock_set( &tod_1 );
  TimecounterTick();
  result_1 = rtems_clock_get_uptime_nanoseconds();

  /*
   * Check that the rtems_clock_set() call for 1988-01-01 was successful.
   */
  T_step_rsc_success( 4, sc_0 );

  /*
   * Check that the rtems_clock_set() call for 2000-01-01 was successful.
   */
  T_step_rsc_success( 5, sc_1 );

  /*
   * Check that the elapsed time in milliseconds is the length of one clock
   * tick. This shows that the change of CLOCK_REALTIME has no effect on the
   * result.
   */
  T_step_eq_u32(
    6,
    ( result_1 - result_0 + 499999 ) / 1000000,
    rtems_configuration_get_milliseconds_per_tick()
  );
}

/**
 * @fn void T_case_body_RtemsClockValClock( void )
 */
T_TEST_CASE( RtemsClockValClock )
{
  T_plan( 7 );

  RtemsClockValClock_Action_0();
  RtemsClockValClock_Action_1();
  RtemsClockValClock_Action_2();
  RtemsClockValClock_Action_3();
  RtemsClockValClock_Action_4();
}

/** @} */
