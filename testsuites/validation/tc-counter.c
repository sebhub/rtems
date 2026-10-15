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
 * nanoseconds and stores the results for the check. The test calls
 * rtems_counter_difference() with a first value greater than the second value,
 * so the difference wraps around. It compares the return value of
 * rtems_counter_frequency() with the frequency of the CPU counter. It reads
 * the counter until it holds ten values, each different from the value before
 * it. It converts one second of the counter frequency, which the converter
 * represents without a rounding, so a converter initialized with another
 * frequency gives another result.
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
