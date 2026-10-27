/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplCounter
 *
 * @brief This source file contains the implementation of
 *   rtems_counter_delay_nanoseconds().
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/counter.h>

/*
 * The conversion of this count of nanoseconds gives at most 429496729 ticks,
 * so the conversion does not saturate.
 */
#define COUNTER_DELAY_CHUNK UINT32_C( 100000000 )

/*
 * The conversion rounds down or rounds up.  The count of ticks which it gives
 * can span up to one counter period less than the step.
 */
#define COUNTER_DELAY_ROUNDING_MARGIN 1

/*
 * The first step can start just before a change of the counter, so its busy
 * wait can be up to one counter period shorter than its count of ticks.  Each
 * further step starts after the change at which the step before it returned.
 */
#define COUNTER_DELAY_PARTIAL_PERIOD_MARGIN 1

static void _Counter_Delay_nanoseconds(
  uint32_t            nanoseconds,
  rtems_counter_ticks margin
)
{
  rtems_counter_ticks ticks;

  ticks = rtems_counter_nanoseconds_to_ticks( nanoseconds );
  rtems_counter_delay_ticks( ticks + margin );
}

void rtems_counter_delay_nanoseconds( uint32_t nanoseconds )
{
  while ( nanoseconds > COUNTER_DELAY_CHUNK ) {
    _Counter_Delay_nanoseconds(
      COUNTER_DELAY_CHUNK,
      COUNTER_DELAY_ROUNDING_MARGIN
    );
    nanoseconds -= COUNTER_DELAY_CHUNK;
  }

  _Counter_Delay_nanoseconds(
    nanoseconds,
    COUNTER_DELAY_ROUNDING_MARGIN + COUNTER_DELAY_PARTIAL_PERIOD_MARGIN
  );
}
