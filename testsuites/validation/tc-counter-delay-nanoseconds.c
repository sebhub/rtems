/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsCounterReqDelayNanoseconds
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
 * @defgroup RtemsCounterReqDelayNanoseconds \
 *   spec:/rtems/counter/req/delay-nanoseconds
 *
 * @ingroup TestsuitesValidation0
 *
 * The test reads the RTEMS counter before and after the call. The difference
 * of the two values is the count of counter ticks which elapsed during the
 * call, plus the time of one counter read.
 *
 * @{
 */

typedef enum {
  RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds_Zero,
  RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds_Positive,
  RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds_NA
} RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds;

typedef enum {
  RtemsCounterReqDelayNanoseconds_Post_Elapsed_AtLeast,
  RtemsCounterReqDelayNanoseconds_Post_Elapsed_NA
} RtemsCounterReqDelayNanoseconds_Post_Elapsed;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Nanoseconds_NA : 1;
  uint8_t Post_Elapsed : 1;
} RtemsCounterReqDelayNanoseconds_Entry;

/**
 * @brief Test context for spec:/rtems/counter/req/delay-nanoseconds test case.
 */
typedef struct {
  /**
   * @brief This member contains the nanoseconds parameter value.
   */
  uint32_t nanoseconds;

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
    RtemsCounterReqDelayNanoseconds_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsCounterReqDelayNanoseconds_Context;

static RtemsCounterReqDelayNanoseconds_Context
  RtemsCounterReqDelayNanoseconds_Instance;

static const char *const
  RtemsCounterReqDelayNanoseconds_PreDesc_Nanoseconds[] =
    { "Zero", "Positive", "NA" };

static const char *const *const RtemsCounterReqDelayNanoseconds_PreDesc[] = {
  RtemsCounterReqDelayNanoseconds_PreDesc_Nanoseconds,
  NULL
};

static void RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds_Prepare(
  RtemsCounterReqDelayNanoseconds_Context        *ctx,
  RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds state
)
{
  switch ( state ) {
    case RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds_Zero: {
      /*
       * While the `nanoseconds` parameter is equal to zero.
       */
      ctx->nanoseconds = 0;
      break;
    }

    case RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds_Positive: {
      /*
       * While the `nanoseconds` parameter is greater than zero.
       */
      ctx->nanoseconds = UINT32_C( 1000000 );
      break;
    }

    case RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds_NA:
      break;
  }
}

static void RtemsCounterReqDelayNanoseconds_Post_Elapsed_Check(
  RtemsCounterReqDelayNanoseconds_Context     *ctx,
  RtemsCounterReqDelayNanoseconds_Post_Elapsed state
)
{
  switch ( state ) {
    case RtemsCounterReqDelayNanoseconds_Post_Elapsed_AtLeast: {
      /*
       * The count of RTEMS counter ticks which elapse during the
       * rtems_counter_delay_nanoseconds() call shall be greater than or equal
       * to the value which the rtems_counter_nanoseconds_to_ticks() directive
       * returns for the `nanoseconds` parameter value.
       */
      T_ge_u32(
        rtems_counter_difference( ctx->counter_after, ctx->counter_before ),
        rtems_counter_nanoseconds_to_ticks( ctx->nanoseconds )
      );
      break;
    }

    case RtemsCounterReqDelayNanoseconds_Post_Elapsed_NA:
      break;
  }
}

static void RtemsCounterReqDelayNanoseconds_Action(
  RtemsCounterReqDelayNanoseconds_Context *ctx
)
{
  ctx->counter_before = rtems_counter_read();
  rtems_counter_delay_nanoseconds( ctx->nanoseconds );
  ctx->counter_after = rtems_counter_read();
}

/* clang-format off */

static const RtemsCounterReqDelayNanoseconds_Entry
RtemsCounterReqDelayNanoseconds_Entries[] = {
  { 0, 0, RtemsCounterReqDelayNanoseconds_Post_Elapsed_AtLeast }
};

static const uint8_t
RtemsCounterReqDelayNanoseconds_Map[] = {
  0, 0
};

/* clang-format on */

static size_t RtemsCounterReqDelayNanoseconds_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  RtemsCounterReqDelayNanoseconds_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsCounterReqDelayNanoseconds_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsCounterReqDelayNanoseconds_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = RtemsCounterReqDelayNanoseconds_Scope,
  .initial_context = &RtemsCounterReqDelayNanoseconds_Instance
};

static inline RtemsCounterReqDelayNanoseconds_Entry
RtemsCounterReqDelayNanoseconds_PopEntry(
  RtemsCounterReqDelayNanoseconds_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsCounterReqDelayNanoseconds_Entries
    [ RtemsCounterReqDelayNanoseconds_Map[ index ] ];
}

static void RtemsCounterReqDelayNanoseconds_TestVariant(
  RtemsCounterReqDelayNanoseconds_Context *ctx
)
{
  RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  RtemsCounterReqDelayNanoseconds_Action( ctx );
  RtemsCounterReqDelayNanoseconds_Post_Elapsed_Check(
    ctx,
    ctx->Map.entry.Post_Elapsed
  );
}

/**
 * @fn void T_case_body_RtemsCounterReqDelayNanoseconds( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsCounterReqDelayNanoseconds,
  &RtemsCounterReqDelayNanoseconds_Fixture
)
{
  RtemsCounterReqDelayNanoseconds_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds_Zero;
    ctx->Map.pcs[ 0 ] < RtemsCounterReqDelayNanoseconds_Pre_Nanoseconds_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = RtemsCounterReqDelayNanoseconds_PopEntry( ctx );
    RtemsCounterReqDelayNanoseconds_TestVariant( ctx );
  }
}

/** @} */
