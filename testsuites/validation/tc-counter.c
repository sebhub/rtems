/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsCounterValCounter
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
#include <rtems/counter.h>
#include <rtems/sysinit.h>

#include <rtems/test.h>

/**
 * @defgroup RtemsCounterValCounter spec:/rtems/counter/val/counter
 *
 * @ingroup TestsuitesValidation0
 *
 * @brief Tests the RTEMS counter.
 *
 * A system initialization handler of the test runs before the system
 * initialization initializes the converter. It converts one second and ten
 * nanoseconds and stores the results for the check. It then sets the converter
 * frequency to a value other than the counter frequency, so only the system
 * initialization can set the counter frequency. The test calls
 * rtems_counter_difference() with a first value greater than the second value,
 * so the difference wraps around. It compares the return value of
 * rtems_counter_frequency() with the frequency of the CPU counter. It counts
 * the counter ticks from a change of the clock tick count to the tenth change
 * after it. The clock tick interrupt can be late by less than one clock tick
 * period at each change, so the count is between nine and eleven clock tick
 * periods at the frequency which rtems_counter_frequency() returns. The count
 * of counter ticks also bounds each wait for a clock tick, so a stopped clock
 * fails the check. It reads the counter until it holds ten values, each
 * different from the value before it. It converts one second at the counter
 * frequency, so a converter initialized with another frequency gives another
 * result.
 *
 * This test case performs the following actions:
 *
 * - Validate the RTEMS counter directives.
 *
 *   - Check that rtems_counter_difference() returns the expected value.
 *
 *   - Check that rtems_counter_frequency() returns the frequency of the CPU
 *     counter.
 *
 *   - Check that the count of counter ticks during some clock ticks matches
 *     the frequency which rtems_counter_frequency() returns.
 *
 *   - Check that rtems_counter_read() returns monotonically increasing values.
 *
 *   - Check that the conversions used a counter frequency of 100000000 Hz
 *     before the system initialization initialized the converter.
 *
 *   - Check that the system initialization set the converter frequency to the
 *     counter frequency. One second converts to and from counter ticks without
 *     a rounding.
 *
 * @{
 */

typedef struct {
  uint64_t            nanoseconds;
  rtems_counter_ticks ticks_of_nanoseconds;
  int64_t             sbintime;
  rtems_counter_ticks ticks_of_sbintime;
  rtems_counter_ticks ticks_of_ten_nanoseconds;
} ConverterDefault;

static ConverterDefault converter_default;

static uint32_t OtherFrequency( uint32_t frequency )
{
  if ( frequency <= UINT32_MAX / 2 ) {
    return 2 * frequency;
  }

  if ( frequency < UINT32_MAX ) {
    return UINT32_MAX;
  }

  return UINT32_MAX - 1;
}

#define CLOCK_TICK_COUNT 10

static rtems_interval WaitForClockTicks(
  rtems_interval      clock_ticks,
  rtems_interval      count,
  rtems_counter_ticks begin,
  uint64_t            bound
)
{
  rtems_interval now;

  do {
    now = rtems_clock_get_ticks_since_boot();
  } while (
    now - clock_ticks < count &&
    rtems_counter_difference( rtems_counter_read(), begin ) <= bound
  );

  return now;
}

static void ConvertBeforeInitialization( void )
{
  converter_default.nanoseconds = rtems_counter_ticks_to_nanoseconds(
    100000000
  );
  converter_default.ticks_of_nanoseconds = rtems_counter_nanoseconds_to_ticks(
    1000000000
  );
  converter_default.sbintime = rtems_counter_ticks_to_sbintime( 100000000 );
  converter_default.ticks_of_sbintime = rtems_counter_sbintime_to_ticks(
    INT64_C( 4294967296 )
  );
  converter_default.ticks_of_ten_nanoseconds =
    rtems_counter_nanoseconds_to_ticks( 10 );

  rtems_counter_initialize_converter(
    OtherFrequency( rtems_counter_frequency() )
  );
}

RTEMS_SYSINIT_ITEM(
  ConvertBeforeInitialization,
  RTEMS_SYSINIT_CPU_COUNTER,
  RTEMS_SYSINIT_ORDER_MIDDLE
);

/**
 * @brief Validate the RTEMS counter directives.
 */
static void RtemsCounterValCounter_Action_0( void )
{
  rtems_counter_ticks ticks[ 10 ];
  rtems_counter_ticks max;
  uint32_t            frequency;
  size_t              i;
  rtems_interval      clock_ticks;
  rtems_interval      clock_ticks_end;
  rtems_counter_ticks begin;
  rtems_counter_ticks end;
  uint64_t            lower;
  uint64_t            upper;

  max = (rtems_counter_ticks) -1;
  frequency = rtems_counter_frequency();

  /*
   * Check that rtems_counter_difference() returns the expected value.
   */
  T_eq_ulong( rtems_counter_difference( 2, 1 ), 1 );
  T_eq_ulong( rtems_counter_difference( 0, 1 ), max );

  /*
   * Check that rtems_counter_frequency() returns the frequency of the CPU
   * counter.
   */
  T_eq_u32( frequency, _CPU_Counter_frequency() );

  /*
   * Check that the count of counter ticks during some clock ticks matches the
   * frequency which rtems_counter_frequency() returns.
   */
  lower = (uint64_t) ( CLOCK_TICK_COUNT - 1 ) * frequency /
          rtems_clock_get_ticks_per_second();
  upper = ( (uint64_t) ( CLOCK_TICK_COUNT + 1 ) * frequency +
            rtems_clock_get_ticks_per_second() - 1 ) /
          rtems_clock_get_ticks_per_second();
  clock_ticks = WaitForClockTicks(
    rtems_clock_get_ticks_since_boot(),
    1,
    rtems_counter_read(),
    upper
  );
  begin = rtems_counter_read();
  clock_ticks_end = WaitForClockTicks(
    clock_ticks,
    CLOCK_TICK_COUNT,
    begin,
    upper
  );
  end = rtems_counter_read();
  T_ge_u32( clock_ticks_end - clock_ticks, CLOCK_TICK_COUNT );
  T_ge_u64( rtems_counter_difference( end, begin ), lower );
  T_le_u64( rtems_counter_difference( end, begin ), upper );

  /*
   * Check that rtems_counter_read() returns monotonically increasing values.
   */
  ticks[ 0 ] = rtems_counter_read();

  for ( i = 1; i < RTEMS_ARRAY_SIZE( ticks ); ++i ) {
    rtems_counter_ticks delta;

    do {
      ticks[ i ] = rtems_counter_read();
    } while ( ticks[ i - 1 ] == ticks[ i ] );

    delta = ticks[ i ] - ticks[ i - 1 ];
    T_lt_ulong( delta, max / 2 );
  }

  /*
   * Check that the conversions used a counter frequency of 100000000 Hz before
   * the system initialization initialized the converter.
   */
  T_eq_u64( converter_default.nanoseconds, 1000000000 );
  T_eq_ulong( converter_default.ticks_of_nanoseconds, 100000000 );
  T_ge_i64( converter_default.sbintime, INT64_C( 4294967296 ) );
  T_le_i64( converter_default.sbintime, INT64_C( 4294967298 ) );
  T_eq_ulong( converter_default.ticks_of_sbintime, 100000000 );
  T_eq_ulong( converter_default.ticks_of_ten_nanoseconds, 1 );

  /*
   * Check that the system initialization set the converter frequency to the
   * counter frequency. One second converts to and from counter ticks without a
   * rounding.
   */
  T_eq_u64( rtems_counter_ticks_to_nanoseconds( frequency ), 1000000000 );
  T_eq_ulong( rtems_counter_nanoseconds_to_ticks( 1000000000 ), frequency );
  T_eq_ulong(
    rtems_counter_sbintime_to_ticks( INT64_C( 4294967296 ) ),
    frequency
  );
}

/**
 * @fn void T_case_body_RtemsCounterValCounter( void )
 */
T_TEST_CASE( RtemsCounterValCounter )
{
  RtemsCounterValCounter_Action_0();
}

/** @} */
