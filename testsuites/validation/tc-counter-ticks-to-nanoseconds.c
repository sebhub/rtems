/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsCounterReqTicksToNanoseconds
 */

/*
 * Copyright (C) 2026 embedded brains GmbH & Co. KG
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
 * @defgroup RtemsCounterReqTicksToNanoseconds \
 *   spec:/rtems/counter/req/ticks-to-nanoseconds
 *
 * @ingroup TestsuitesValidation0
 *
 * The test initializes the converter with the frequency of the variant and
 * converts the counter ticks value of the variant. It compares the result with
 * the time rounded down and rounded up, which it computes in 64 bits.
 *
 * @{
 */

typedef enum {
  RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_Min,
  RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_Mid,
  RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_Max,
  RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_NA
} RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency;

typedef enum {
  RtemsCounterReqTicksToNanoseconds_Pre_Ticks_Integral,
  RtemsCounterReqTicksToNanoseconds_Pre_Ticks_Fraction,
  RtemsCounterReqTicksToNanoseconds_Pre_Ticks_Max,
  RtemsCounterReqTicksToNanoseconds_Pre_Ticks_NA
} RtemsCounterReqTicksToNanoseconds_Pre_Ticks;

typedef enum {
  RtemsCounterReqTicksToNanoseconds_Post_Result_Bounded,
  RtemsCounterReqTicksToNanoseconds_Post_Result_NA
} RtemsCounterReqTicksToNanoseconds_Post_Result;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_ConverterFrequency_NA : 1;
  uint8_t Pre_Ticks_NA : 1;
  uint8_t Post_Result : 1;
} RtemsCounterReqTicksToNanoseconds_Entry;

/**
 * @brief Test context for spec:/rtems/counter/req/ticks-to-nanoseconds test
 *   case.
 */
typedef struct {
  /**
   * @brief This member contains the converter frequency.
   */
  uint32_t frequency;

  /**
   * @brief This member contains the counter ticks parameter value.
   */
  rtems_counter_ticks ticks;

  /**
   * @brief This member contains the return value.
   */
  uint64_t result;

  struct {
    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 2 ];

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
    RtemsCounterReqTicksToNanoseconds_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsCounterReqTicksToNanoseconds_Context;

static RtemsCounterReqTicksToNanoseconds_Context
  RtemsCounterReqTicksToNanoseconds_Instance;

static const char *const
  RtemsCounterReqTicksToNanoseconds_PreDesc_ConverterFrequency[] =
    { "Min", "Mid", "Max", "NA" };

static const char *const RtemsCounterReqTicksToNanoseconds_PreDesc_Ticks[] =
  { "Integral", "Fraction", "Max", "NA" };

static const char *const *const RtemsCounterReqTicksToNanoseconds_PreDesc[] = {
  RtemsCounterReqTicksToNanoseconds_PreDesc_ConverterFrequency,
  RtemsCounterReqTicksToNanoseconds_PreDesc_Ticks,
  NULL
};

static void RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_Prepare(
  RtemsCounterReqTicksToNanoseconds_Context               *ctx,
  RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency state
)
{
  switch ( state ) {
    case RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_Min: {
      /*
       * While the converter frequency is equal to one.
       */
      ctx->frequency = 1;
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_Mid: {
      /*
       * While the converter frequency is greater than one, while the converter
       * frequency is less than the maximum value of uint32_t.
       */
      ctx->frequency = UINT32_C( 123456789 );
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_Max: {
      /*
       * While the converter frequency is equal to the maximum value of
       * uint32_t.
       */
      ctx->frequency = UINT32_MAX;
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_NA:
      break;
  }
}

static void RtemsCounterReqTicksToNanoseconds_Pre_Ticks_Prepare(
  RtemsCounterReqTicksToNanoseconds_Context  *ctx,
  RtemsCounterReqTicksToNanoseconds_Pre_Ticks state
)
{
  switch ( state ) {
    case RtemsCounterReqTicksToNanoseconds_Pre_Ticks_Integral: {
      /*
       * While the `ticks` parameter is less than the maximum value of
       * rtems_counter_ticks, while the `ticks` parameter represents a whole
       * number of seconds at the converter frequency.
       */
      ctx->ticks = ctx->frequency < UINT32_MAX ? ctx->frequency : 0;
      break;
    }

    case RtemsCounterReqTicksToNanoseconds_Pre_Ticks_Fraction: {
      /*
       * While the `ticks` parameter is less than the maximum value of
       * rtems_counter_ticks, while the `ticks` parameter represents no whole
       * number of seconds at the converter frequency.
       */
      ctx->ticks = 1;
      break;
    }

    case RtemsCounterReqTicksToNanoseconds_Pre_Ticks_Max: {
      /*
       * While the `ticks` parameter is equal to the maximum value of
       * rtems_counter_ticks.
       */
      ctx->ticks = UINT32_MAX;
      break;
    }

    case RtemsCounterReqTicksToNanoseconds_Pre_Ticks_NA:
      break;
  }
}

static void RtemsCounterReqTicksToNanoseconds_Post_Result_Check(
  RtemsCounterReqTicksToNanoseconds_Context    *ctx,
  RtemsCounterReqTicksToNanoseconds_Post_Result state
)
{
  uint64_t exact;

  switch ( state ) {
    case RtemsCounterReqTicksToNanoseconds_Post_Result_Bounded: {
      /*
       * The rtems_counter_ticks_to_nanoseconds() directive shall return a
       * value which is greater than the time in nanoseconds which the `ticks`
       * parameter represents at the converter frequency minus one and less
       * than this time plus one.
       */
      exact = (uint64_t) ctx->ticks * UINT64_C( 1000000000 );
      T_ge_u64( ctx->result, exact / ctx->frequency );
      T_le_u64( ctx->result, ( exact + ctx->frequency - 1 ) / ctx->frequency );
      break;
    }

    case RtemsCounterReqTicksToNanoseconds_Post_Result_NA:
      break;
  }
}

static void RtemsCounterReqTicksToNanoseconds_Action(
  RtemsCounterReqTicksToNanoseconds_Context *ctx
)
{
  ctx->result = rtems_counter_ticks_to_nanoseconds( ctx->ticks );
}

static void RtemsCounterReqTicksToNanoseconds_Cleanup( void )
{
  rtems_counter_initialize_converter( rtems_counter_frequency() );
}

/* clang-format off */

static const RtemsCounterReqTicksToNanoseconds_Entry
RtemsCounterReqTicksToNanoseconds_Entries[] = {
  { 0, 0, 0, RtemsCounterReqTicksToNanoseconds_Post_Result_Bounded },
  { 1, 0, 0, RtemsCounterReqTicksToNanoseconds_Post_Result_NA }
};

static const uint8_t
RtemsCounterReqTicksToNanoseconds_Map[] = {
  0, 1, 0, 0, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t RtemsCounterReqTicksToNanoseconds_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  RtemsCounterReqTicksToNanoseconds_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsCounterReqTicksToNanoseconds_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsCounterReqTicksToNanoseconds_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = RtemsCounterReqTicksToNanoseconds_Scope,
  .initial_context = &RtemsCounterReqTicksToNanoseconds_Instance
};

static inline RtemsCounterReqTicksToNanoseconds_Entry
RtemsCounterReqTicksToNanoseconds_PopEntry(
  RtemsCounterReqTicksToNanoseconds_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsCounterReqTicksToNanoseconds_Entries
    [ RtemsCounterReqTicksToNanoseconds_Map[ index ] ];
}

static void RtemsCounterReqTicksToNanoseconds_TestVariant(
  RtemsCounterReqTicksToNanoseconds_Context *ctx
)
{
  RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  RtemsCounterReqTicksToNanoseconds_Pre_Ticks_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  RtemsCounterReqTicksToNanoseconds_Action( ctx );
  RtemsCounterReqTicksToNanoseconds_Post_Result_Check(
    ctx,
    ctx->Map.entry.Post_Result
  );
}

/**
 * @fn void T_case_body_RtemsCounterReqTicksToNanoseconds( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsCounterReqTicksToNanoseconds,
  &RtemsCounterReqTicksToNanoseconds_Fixture
)
{
  RtemsCounterReqTicksToNanoseconds_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] =
      RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_Min;
    ctx->Map.pcs[ 0 ] <
    RtemsCounterReqTicksToNanoseconds_Pre_ConverterFrequency_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = RtemsCounterReqTicksToNanoseconds_Pre_Ticks_Integral;
      ctx->Map.pcs[ 1 ] < RtemsCounterReqTicksToNanoseconds_Pre_Ticks_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      ctx->Map.entry = RtemsCounterReqTicksToNanoseconds_PopEntry( ctx );

      if ( ctx->Map.entry.Skip ) {
        continue;
      }

      RtemsCounterReqTicksToNanoseconds_TestVariant( ctx );
      RtemsCounterReqTicksToNanoseconds_Cleanup();
    }
  }
}

/** @} */
