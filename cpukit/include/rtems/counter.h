/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSAPICounter
 *
 * @brief This header file provides the free-running counter and busy wait
 *   delay interfaces.
 */

/*
 * Copyright (C) 2014, 2026 embedded brains GmbH & Co. KG
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

/*
 * The spec:/... comments allow you to identify content which is
 * duplicated in the rtems and rtems-docs repositories.
 */

/* Find related documentation with spec:/rtems/counter/if/header */

#ifndef _RTEMS_COUNTER_H
#define _RTEMS_COUNTER_H

#include <stdint.h>
#include <rtems/score/cpu.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/rtems/counter/if/group */

/**
 * @defgroup RTEMSAPICounter Free-Running Counter and Busy Wait Delay
 *
 * @ingroup RTEMSAPIClassic
 *
 * @brief This group contains free-running counter and busy wait delay
 *   functions.
 *
 * The RTEMS counter is some free-running counter. It ticks usually with a
 * frequency close to the processor or system bus clock. The counter can be
 * used in case the overhead of the rtems_clock_get_uptime_nanoseconds() is too
 * high. The step from counter ticks to/from nanoseconds is explicit in this
 * API unlike to rtems_clock_get_uptime_nanoseconds() which performs the
 * conversion on each invocation. This counter works without a clock driver and
 * during system initialization. The counter can be used to profile low-level
 * operations like SMP locks or interrupt disabled critical sections. The
 * counter can act also as an entropy source for a random number generator. The
 * period of the counter depends on the actual hardware.
 */

/* Find related documentation with spec:/rtems/counter/if/initialize-converter */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief Initializes the counter ticks to and from nanoseconds and signed
 *   binary time converter functions.
 *
 * @param[in] frequency is the current counter frequency in Hz. The frequency
 *   shall be greater than zero.
 *
 * The function sets the converter frequency to the frequency. The
 * rtems_counter_ticks_to_nanoseconds(), rtems_counter_nanoseconds_to_ticks(),
 * rtems_counter_ticks_to_sbintime() and rtems_counter_sbintime_to_ticks()
 * functions convert at the converter frequency.
 *
 * @par Notes
 * The system initialization calls the function with the value which
 * rtems_counter_frequency() returns. Before the first call, the converter
 * frequency is 100000000 Hz.
 */
void rtems_counter_initialize_converter( uint32_t frequency );

/* Find related documentation with spec:/rtems/counter/if/ticks */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief This unsigned integer type represents counter values.
 */
typedef CPU_Counter_ticks rtems_counter_ticks;

/* Find related documentation with spec:/rtems/counter/if/frequency */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief Returns the current counter frequency in Hz.
 *
 * @return Returns the counter frequency in Hz.
 */
static inline uint32_t rtems_counter_frequency( void )
{
  return _CPU_Counter_frequency();
}

/* Find related documentation with spec:/rtems/counter/if/read */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief Reads the current counter value.
 *
 * @return Returns the current counter value.
 */
static inline rtems_counter_ticks rtems_counter_read( void )
{
  return _CPU_Counter_read();
}

/* Find related documentation with spec:/rtems/counter/if/difference */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief Returns the difference between the second and first counter value.
 *
 * @param second is the second counter value.
 *
 * @param first is the first counter value.
 *
 * @return Returns `second` minus `first` modulo the range of
 *   ::rtems_counter_ticks.
 *
 * @par Notes
 * The function exists for backward compatibility. Code can use `second -
 * first` directly.
 */
static inline rtems_counter_ticks rtems_counter_difference(
  rtems_counter_ticks second,
  rtems_counter_ticks first
)
{
  return second - first;
}

/* Find related documentation with spec:/rtems/counter/if/ticks-to-nanoseconds */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief Converts the counter ticks into nanoseconds.
 *
 * @param ticks is the counter ticks value to convert.
 *
 * The function converts at the converter frequency.
 *
 * @return Returns the nanoseconds value corresponding to the counter ticks.
 *   The value is the exact value rounded down or rounded up.
 */
uint64_t rtems_counter_ticks_to_nanoseconds( rtems_counter_ticks ticks );

/* Find related documentation with spec:/rtems/counter/if/nanoseconds-to-ticks */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief Converts the nanoseconds into counter ticks.
 *
 * @param nanoseconds is the nanoseconds value to convert.
 *
 * The function converts at the converter frequency.
 *
 * @return Returns the counter ticks value corresponding to the nanoseconds
 *   value. The value is the exact value rounded down or rounded up. Where the
 *   exact value rounded down is greater than UINT32_MAX, the value is
 *   UINT32_MAX.
 */
rtems_counter_ticks rtems_counter_nanoseconds_to_ticks( uint32_t nanoseconds );

/* Find related documentation with spec:/rtems/counter/if/ticks-to-sbintime */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief Converts the counter ticks into signed binary time (sbintime_t).
 *
 * @param ticks is the counter ticks value to convert.
 *
 * The function converts at the converter frequency.
 *
 * @return Returns the signed binary time value corresponding to the counter
 *   ticks value. The value is greater than or equal to the exact value rounded
 *   down. The value is less than or equal to the exact value rounded down plus
 *   two. Where the exact value is greater than the maximum value of int64_t,
 *   the value is the maximum value of int64_t.
 */
int64_t rtems_counter_ticks_to_sbintime( rtems_counter_ticks ticks );

/* Find related documentation with spec:/rtems/counter/if/sbintime-to-ticks */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief Converts the signed binary time (sbintime_t) into counter ticks.
 *
 * @param sbt is the signed binary time value to convert.
 *
 * The function converts at the converter frequency.
 *
 * @return Returns the counter ticks value corresponding to the signed binary
 *   time value. The value is the exact value rounded down. Where the exact
 *   value rounded down is greater than UINT32_MAX, the value is UINT32_MAX.
 *   Where the signed binary time value is negative, the value is zero.
 */
rtems_counter_ticks rtems_counter_sbintime_to_ticks( int64_t sbt );

/* Find related documentation with spec:/rtems/counter/if/delay-ticks */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief Busy wait for some counter ticks.
 *
 * @param ticks is the minimum busy wait time in counter ticks.
 *
 * @par Notes
 * This function does not disable interrupts. Task switches and interrupts can
 * prolong the delay. This function busy waits at least the specified time. The
 * overhead of the function can make the delay longer.
 */
void rtems_counter_delay_ticks( rtems_counter_ticks ticks );

/* Find related documentation with spec:/rtems/counter/if/delay-nanoseconds */

/**
 * @ingroup RTEMSAPICounter
 *
 * @brief Busy wait for some nanoseconds.
 *
 * @param nanoseconds is the busy wait time in nanoseconds.
 *
 * @par Notes
 * This function does not disable interrupts. Task switches and interrupts can
 * prolong the delay. This function converts the nanoseconds into counter ticks
 * with rtems_counter_nanoseconds_to_ticks(). The conversion rounds down or
 * rounds up, so the busy wait time can be up to one counter tick shorter than
 * the specified time. The overhead of the function can make the delay longer.
 */
void rtems_counter_delay_nanoseconds( uint32_t nanoseconds );

#ifdef __cplusplus
}
#endif

#endif /* _RTEMS_COUNTER_H */
