/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsCounterReqNanosecondsToTicks
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
 * @defgroup RtemsCounterReqNanosecondsToTicks \
 *   spec:/rtems/counter/req/nanoseconds-to-ticks
 *
 * @ingroup TestsuitesValidation0
 *
 * The test initializes the converter with the frequency of the variant. It
 * computes the limit of the variant from the frequency, independent of the
 * converter. It converts the nanoseconds value of the variant and compares the
 * result with the count rounded down and rounded up.
 *
 * @{
 */

typedef enum {
  RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_Min,
  RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_Low,
  RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_High,
  RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_Max,
  RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_NA
} RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency;

typedef enum {
  RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Integral,
  RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Fraction,
  RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Limit,
  RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Beyond,
  RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_NA
} RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds;

typedef enum {
  RtemsCounterReqNanosecondsToTicks_Post_Result_Bounded,
  RtemsCounterReqNanosecondsToTicks_Post_Result_Saturated,
  RtemsCounterReqNanosecondsToTicks_Post_Result_NA
} RtemsCounterReqNanosecondsToTicks_Post_Result;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_ConverterFrequency_NA : 1;
  uint8_t Pre_Nanoseconds_NA : 1;
  uint8_t Post_Result : 2;
} RtemsCounterReqNanosecondsToTicks_Entry;

/**
 * @brief Test context for spec:/rtems/counter/req/nanoseconds-to-ticks test
 *   case.
 */
typedef struct {
  /**
   * @brief This member contains the converter frequency.
   */
  uint32_t frequency;

  /**
   * @brief This member contains the nanoseconds parameter value.
   */
  uint32_t nanoseconds;

  /**
   * @brief This member contains the return value.
   */
  rtems_counter_ticks result;

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
    RtemsCounterReqNanosecondsToTicks_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsCounterReqNanosecondsToTicks_Context;

static RtemsCounterReqNanosecondsToTicks_Context
  RtemsCounterReqNanosecondsToTicks_Instance;

static const char *const
  RtemsCounterReqNanosecondsToTicks_PreDesc_ConverterFrequency[] =
    { "Min", "Low", "High", "Max", "NA" };

static const char *const
  RtemsCounterReqNanosecondsToTicks_PreDesc_Nanoseconds[] =
    { "Integral", "Fraction", "Limit", "Beyond", "NA" };

static const char *const *const RtemsCounterReqNanosecondsToTicks_PreDesc[] = {
  RtemsCounterReqNanosecondsToTicks_PreDesc_ConverterFrequency,
  RtemsCounterReqNanosecondsToTicks_PreDesc_Nanoseconds,
  NULL
};

static uint32_t Limit( uint32_t frequency )
{
  uint64_t limit;

  limit = ( UINT64_C( 4294967296 ) * UINT64_C( 1000000000 ) - 1 ) / frequency;

  if ( limit > UINT32_MAX ) {
    limit = UINT32_MAX;
  }

  return (uint32_t) limit;
}

static void RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_Prepare(
  RtemsCounterReqNanosecondsToTicks_Context               *ctx,
  RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency state
)
{
  switch ( state ) {
    case RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_Min: {
      /*
       * While the converter frequency is equal to one.
       */
      ctx->frequency = 1;
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_Low: {
      /*
       * While the converter frequency is greater than one, while the converter
       * frequency is less than or equal to 1000000000.
       */
      ctx->frequency = UINT32_C( 123456789 );
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_High: {
      /*
       * While the converter frequency is greater than 1000000000, while the
       * converter frequency is less than the maximum value of uint32_t.
       */
      ctx->frequency = UINT32_C( 2000000000 );
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_Max: {
      /*
       * While the converter frequency is equal to the maximum value of
       * uint32_t.
       */
      ctx->frequency = UINT32_MAX;
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_NA:
      break;
  }
}

static void RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Prepare(
  RtemsCounterReqNanosecondsToTicks_Context        *ctx,
  RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds state
)
{
  switch ( state ) {
    case RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Integral: {
      /*
       * While the `nanoseconds` parameter is less than the largest value of
       * uint32_t which specifies a time shorter than 4294967296 counter ticks
       * at the converter frequency, while the `nanoseconds` parameter
       * specifies the time of a whole number of counter ticks at the converter
       * frequency.
       */
      ctx->nanoseconds = Limit( ctx->frequency ) > UINT32_C( 1000000000 )
                           ? UINT32_C( 1000000000 )
                           : 0;
      break;
    }

    case RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Fraction: {
      /*
       * While the `nanoseconds` parameter is less than the largest value of
       * uint32_t which specifies a time shorter than 4294967296 counter ticks
       * at the converter frequency, while the `nanoseconds` parameter
       * specifies no time of a whole number of counter ticks at the converter
       * frequency.
       */
      ctx->nanoseconds = 1;
      break;
    }

    case RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Limit: {
      /*
       * While the `nanoseconds` parameter is equal to the largest value of
       * uint32_t which specifies a time shorter than 4294967296 counter ticks
       * at the converter frequency.
       */
      ctx->nanoseconds = Limit( ctx->frequency );
      break;
    }

    case RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Beyond: {
      /*
       * While the `nanoseconds` parameter is greater than the largest value of
       * uint32_t which specifies a time shorter than 4294967296 counter ticks
       * at the converter frequency.
       */
      ctx->nanoseconds = Limit( ctx->frequency ) + 1;
      break;
    }

    case RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_NA:
      break;
  }
}

static void RtemsCounterReqNanosecondsToTicks_Post_Result_Check(
  RtemsCounterReqNanosecondsToTicks_Context    *ctx,
  RtemsCounterReqNanosecondsToTicks_Post_Result state
)
{
  uint64_t exact;

  switch ( state ) {
    case RtemsCounterReqNanosecondsToTicks_Post_Result_Bounded: {
      /*
       * The rtems_counter_nanoseconds_to_ticks() directive shall return the
       * count of counter ticks in the time which the `nanoseconds` parameter
       * specifies in nanoseconds at the converter frequency, rounded down or
       * rounded up.
       */
      exact = (uint64_t) ctx->nanoseconds * ctx->frequency;
      T_ge_u64( ctx->result, exact / UINT64_C( 1000000000 ) );
      T_le_u64(
        ctx->result,
        ( exact + UINT64_C( 999999999 ) ) / UINT64_C( 1000000000 )
      );
      break;
    }

    case RtemsCounterReqNanosecondsToTicks_Post_Result_Saturated: {
      /*
       * The rtems_counter_nanoseconds_to_ticks() directive shall return
       * UINT32_MAX.
       */
      T_eq_u32( ctx->result, UINT32_MAX );
      break;
    }

    case RtemsCounterReqNanosecondsToTicks_Post_Result_NA:
      break;
  }
}

static void RtemsCounterReqNanosecondsToTicks_Action(
  RtemsCounterReqNanosecondsToTicks_Context *ctx
)
{
  ctx->result = rtems_counter_nanoseconds_to_ticks( ctx->nanoseconds );
}

static void RtemsCounterReqNanosecondsToTicks_Cleanup( void )
{
  rtems_counter_initialize_converter( rtems_counter_frequency() );
}

/* clang-format off */

static const RtemsCounterReqNanosecondsToTicks_Entry
RtemsCounterReqNanosecondsToTicks_Entries[] = {
  { 0, 0, 0, RtemsCounterReqNanosecondsToTicks_Post_Result_Bounded },
  { 1, 0, 0, RtemsCounterReqNanosecondsToTicks_Post_Result_NA },
  { 0, 0, 0, RtemsCounterReqNanosecondsToTicks_Post_Result_Saturated }
};

static const uint8_t
RtemsCounterReqNanosecondsToTicks_Map[] = {
  0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 2
};

/* clang-format on */

static size_t RtemsCounterReqNanosecondsToTicks_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  RtemsCounterReqNanosecondsToTicks_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsCounterReqNanosecondsToTicks_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsCounterReqNanosecondsToTicks_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = RtemsCounterReqNanosecondsToTicks_Scope,
  .initial_context = &RtemsCounterReqNanosecondsToTicks_Instance
};

static inline RtemsCounterReqNanosecondsToTicks_Entry
RtemsCounterReqNanosecondsToTicks_PopEntry(
  RtemsCounterReqNanosecondsToTicks_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsCounterReqNanosecondsToTicks_Entries
    [ RtemsCounterReqNanosecondsToTicks_Map[ index ] ];
}

static void RtemsCounterReqNanosecondsToTicks_TestVariant(
  RtemsCounterReqNanosecondsToTicks_Context *ctx
)
{
  RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  RtemsCounterReqNanosecondsToTicks_Action( ctx );
  RtemsCounterReqNanosecondsToTicks_Post_Result_Check(
    ctx,
    ctx->Map.entry.Post_Result
  );
}

/**
 * @fn void T_case_body_RtemsCounterReqNanosecondsToTicks( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsCounterReqNanosecondsToTicks,
  &RtemsCounterReqNanosecondsToTicks_Fixture
)
{
  RtemsCounterReqNanosecondsToTicks_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] =
      RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_Min;
    ctx->Map.pcs[ 0 ] <
    RtemsCounterReqNanosecondsToTicks_Pre_ConverterFrequency_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] =
        RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_Integral;
      ctx->Map.pcs[ 1 ] < RtemsCounterReqNanosecondsToTicks_Pre_Nanoseconds_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      ctx->Map.entry = RtemsCounterReqNanosecondsToTicks_PopEntry( ctx );

      if ( ctx->Map.entry.Skip ) {
        continue;
      }

      RtemsCounterReqNanosecondsToTicks_TestVariant( ctx );
      RtemsCounterReqNanosecondsToTicks_Cleanup();
    }
  }
}

/** @} */
