/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsCounterReqDelayTicks
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

#include <rtems/test.h>

/**
 * @defgroup RtemsCounterReqDelayTicks spec:/rtems/counter/req/delay-ticks
 *
 * @ingroup TestsuitesValidation0
 *
 * The test reads the RTEMS counter before and after the call. The difference
 * of the two values is the count of counter ticks which elapsed during the
 * call, plus the time of one counter read. The measured interval includes the
 * overhead of the call. The test therefore detects a shortfall of the busy
 * wait only where the shortfall exceeds this overhead. For the state Zero, the
 * return of the call is the result.
 *
 * @{
 */

typedef enum {
  RtemsCounterReqDelayTicks_Pre_Ticks_Zero,
  RtemsCounterReqDelayTicks_Pre_Ticks_Positive,
  RtemsCounterReqDelayTicks_Pre_Ticks_NA
} RtemsCounterReqDelayTicks_Pre_Ticks;

typedef enum {
  RtemsCounterReqDelayTicks_Post_Elapsed_AtLeast,
  RtemsCounterReqDelayTicks_Post_Elapsed_NA
} RtemsCounterReqDelayTicks_Post_Elapsed;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Ticks_NA : 1;
  uint8_t Post_Elapsed : 1;
} RtemsCounterReqDelayTicks_Entry;

/**
 * @brief Test context for spec:/rtems/counter/req/delay-ticks test case.
 */
typedef struct {
  /**
   * @brief This member contains the counter ticks parameter value.
   */
  rtems_counter_ticks ticks;

  /**
   * @brief This member contains the counter value before the call.
   */
  rtems_counter_ticks counter_before;

  /**
   * @brief This member contains the counter value after the call.
   */
  rtems_counter_ticks counter_after;

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
    RtemsCounterReqDelayTicks_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsCounterReqDelayTicks_Context;

static RtemsCounterReqDelayTicks_Context RtemsCounterReqDelayTicks_Instance;

static const char *const RtemsCounterReqDelayTicks_PreDesc_Ticks[] =
  { "Zero", "Positive", "NA" };

static const char *const *const RtemsCounterReqDelayTicks_PreDesc[] = {
  RtemsCounterReqDelayTicks_PreDesc_Ticks,
  NULL
};

static void RtemsCounterReqDelayTicks_Pre_Ticks_Prepare(
  RtemsCounterReqDelayTicks_Context  *ctx,
  RtemsCounterReqDelayTicks_Pre_Ticks state
)
{
  switch ( state ) {
    case RtemsCounterReqDelayTicks_Pre_Ticks_Zero: {
      /*
       * While the `ticks` parameter is equal to zero.
       */
      ctx->ticks = 0;
      break;
    }

    case RtemsCounterReqDelayTicks_Pre_Ticks_Positive: {
      /*
       * While the `ticks` parameter is greater than zero.
       */
      ctx->ticks = rtems_counter_frequency() / 1000 + 1;
      break;
    }

    case RtemsCounterReqDelayTicks_Pre_Ticks_NA:
      break;
  }
}

static void RtemsCounterReqDelayTicks_Post_Elapsed_Check(
  RtemsCounterReqDelayTicks_Context     *ctx,
  RtemsCounterReqDelayTicks_Post_Elapsed state
)
{
  switch ( state ) {
    case RtemsCounterReqDelayTicks_Post_Elapsed_AtLeast: {
      /*
       * The rtems_counter_delay_ticks() directive shall return after the count
       * of counter ticks which elapse during the call is greater than or equal
       * to the `ticks` parameter value.
       */
      if ( ctx->ticks > 0 ) {
        T_ge_u32(
          rtems_counter_difference( ctx->counter_after, ctx->counter_before ),
          ctx->ticks
        );
      }
      break;
    }

    case RtemsCounterReqDelayTicks_Post_Elapsed_NA:
      break;
  }
}

static void RtemsCounterReqDelayTicks_Action(
  RtemsCounterReqDelayTicks_Context *ctx
)
{
  ctx->counter_before = rtems_counter_read();
  rtems_counter_delay_ticks( ctx->ticks );
  ctx->counter_after = rtems_counter_read();
}

/* clang-format off */

static const RtemsCounterReqDelayTicks_Entry
RtemsCounterReqDelayTicks_Entries[] = {
  { 0, 0, RtemsCounterReqDelayTicks_Post_Elapsed_AtLeast }
};

static const uint8_t
RtemsCounterReqDelayTicks_Map[] = {
  0, 0
};

/* clang-format on */

static size_t RtemsCounterReqDelayTicks_Scope( void *arg, char *buf, size_t n )
{
  RtemsCounterReqDelayTicks_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsCounterReqDelayTicks_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsCounterReqDelayTicks_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = RtemsCounterReqDelayTicks_Scope,
  .initial_context = &RtemsCounterReqDelayTicks_Instance
};

static inline RtemsCounterReqDelayTicks_Entry
RtemsCounterReqDelayTicks_PopEntry( RtemsCounterReqDelayTicks_Context *ctx )
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsCounterReqDelayTicks_Entries
    [ RtemsCounterReqDelayTicks_Map[ index ] ];
}

static void RtemsCounterReqDelayTicks_TestVariant(
  RtemsCounterReqDelayTicks_Context *ctx
)
{
  RtemsCounterReqDelayTicks_Pre_Ticks_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  RtemsCounterReqDelayTicks_Action( ctx );
  RtemsCounterReqDelayTicks_Post_Elapsed_Check(
    ctx,
    ctx->Map.entry.Post_Elapsed
  );
}

/**
 * @fn void T_case_body_RtemsCounterReqDelayTicks( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsCounterReqDelayTicks,
  &RtemsCounterReqDelayTicks_Fixture
)
{
  RtemsCounterReqDelayTicks_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = RtemsCounterReqDelayTicks_Pre_Ticks_Zero;
    ctx->Map.pcs[ 0 ] < RtemsCounterReqDelayTicks_Pre_Ticks_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = RtemsCounterReqDelayTicks_PopEntry( ctx );
    RtemsCounterReqDelayTicks_TestVariant( ctx );
  }
}

/** @} */
