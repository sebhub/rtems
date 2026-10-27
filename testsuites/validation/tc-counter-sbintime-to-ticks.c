/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsCounterReqSbintimeToTicks
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
 * @defgroup RtemsCounterReqSbintimeToTicks \
 *   spec:/rtems/counter/req/sbintime-to-ticks
 *
 * @ingroup TestsuitesValidation0
 *
 * The test initializes the converter with the frequency of the variant. It
 * computes the limit of the variant from the frequency, independent of the
 * converter. It converts the signed binary time value of the variant and
 * compares the result with the count rounded down, which the test computes in
 * 64 bits.
 *
 * @{
 */

typedef enum {
  RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_Min,
  RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_Low,
  RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_High,
  RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_Max,
  RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_NA
} RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency;

typedef enum {
  RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Negative,
  RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Integral,
  RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Fraction,
  RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Limit,
  RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Beyond,
  RtemsCounterReqSbintimeToTicks_Pre_Sbintime_NA
} RtemsCounterReqSbintimeToTicks_Pre_Sbintime;

typedef enum {
  RtemsCounterReqSbintimeToTicks_Post_Result_Bounded,
  RtemsCounterReqSbintimeToTicks_Post_Result_Saturated,
  RtemsCounterReqSbintimeToTicks_Post_Result_Zero,
  RtemsCounterReqSbintimeToTicks_Post_Result_NA
} RtemsCounterReqSbintimeToTicks_Post_Result;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_ConverterFrequency_NA : 1;
  uint8_t Pre_Sbintime_NA : 1;
  uint8_t Post_Result : 2;
} RtemsCounterReqSbintimeToTicks_Entry;

/**
 * @brief Test context for spec:/rtems/counter/req/sbintime-to-ticks test case.
 */
typedef struct {
  /**
   * @brief This member contains the converter frequency.
   */
  uint32_t frequency;

  /**
   * @brief This member contains the signed binary time parameter value.
   */
  int64_t sbintime;

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
    RtemsCounterReqSbintimeToTicks_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsCounterReqSbintimeToTicks_Context;

static RtemsCounterReqSbintimeToTicks_Context
  RtemsCounterReqSbintimeToTicks_Instance;

static const char *const
  RtemsCounterReqSbintimeToTicks_PreDesc_ConverterFrequency[] =
    { "Min", "Low", "High", "Max", "NA" };

static const char *const RtemsCounterReqSbintimeToTicks_PreDesc_Sbintime[] =
  { "Negative", "Integral", "Fraction", "Limit", "Beyond", "NA" };

static const char *const *const RtemsCounterReqSbintimeToTicks_PreDesc[] = {
  RtemsCounterReqSbintimeToTicks_PreDesc_ConverterFrequency,
  RtemsCounterReqSbintimeToTicks_PreDesc_Sbintime,
  NULL
};

static int64_t Limit( uint32_t frequency )
{
  uint64_t limit;

  limit = UINT64_MAX / frequency;

  if ( limit > INT64_MAX ) {
    limit = INT64_MAX;
  }

  return (int64_t) limit;
}

static void RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_Prepare(
  RtemsCounterReqSbintimeToTicks_Context               *ctx,
  RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency state
)
{
  switch ( state ) {
    case RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_Min: {
      /*
       * While the converter frequency is equal to one.
       */
      ctx->frequency = 1;
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_Low: {
      /*
       * While the converter frequency is equal to two.
       */
      ctx->frequency = 2;
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_High: {
      /*
       * While the converter frequency is greater than two, while the converter
       * frequency is less than the maximum value of uint32_t.
       */
      ctx->frequency = UINT32_C( 123456789 );
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_Max: {
      /*
       * While the converter frequency is equal to the maximum value of
       * uint32_t.
       */
      ctx->frequency = UINT32_MAX;
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_NA:
      break;
  }
}

static void RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Prepare(
  RtemsCounterReqSbintimeToTicks_Context     *ctx,
  RtemsCounterReqSbintimeToTicks_Pre_Sbintime state
)
{
  switch ( state ) {
    case RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Negative: {
      /*
       * While the `sbt` parameter is less than zero.
       */
      ctx->sbintime = -1;
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Integral: {
      /*
       * While the `sbt` parameter is greater than or equal to zero, while the
       * `sbt` parameter is less than the largest value of int64_t which
       * specifies a time shorter than 4294967296 counter ticks at the
       * converter frequency, while the `sbt` parameter specifies the time of a
       * whole number of counter ticks at the converter frequency.
       */
      ctx->sbintime = INT64_C( 4294967296 );
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Fraction: {
      /*
       * While the `sbt` parameter is greater than or equal to zero, while the
       * `sbt` parameter is less than the largest value of int64_t which
       * specifies a time shorter than 4294967296 counter ticks at the
       * converter frequency, while the `sbt` parameter specifies no time of a
       * whole number of counter ticks at the converter frequency.
       */
      ctx->sbintime = 1;
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Limit: {
      /*
       * While the `sbt` parameter is equal to the largest value of int64_t
       * which specifies a time shorter than 4294967296 counter ticks at the
       * converter frequency.
       */
      ctx->sbintime = Limit( ctx->frequency );
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Beyond: {
      /*
       * While the `sbt` parameter is greater than the largest value of int64_t
       * which specifies a time shorter than 4294967296 counter ticks at the
       * converter frequency.
       */
      ctx->sbintime = Limit( ctx->frequency ) + 1;
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Pre_Sbintime_NA:
      break;
  }
}

static void RtemsCounterReqSbintimeToTicks_Post_Result_Check(
  RtemsCounterReqSbintimeToTicks_Context    *ctx,
  RtemsCounterReqSbintimeToTicks_Post_Result state
)
{
  switch ( state ) {
    case RtemsCounterReqSbintimeToTicks_Post_Result_Bounded: {
      /*
       * The rtems_counter_sbintime_to_ticks() directive shall return the count
       * of counter ticks in the time which the `sbt` parameter specifies in
       * signed binary time at the converter frequency, rounded down.
       */
      T_eq_u64(
        ctx->result,
        ( (uint64_t) ctx->sbintime * ctx->frequency ) >> 32
      );
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Post_Result_Saturated: {
      /*
       * The rtems_counter_sbintime_to_ticks() directive shall return
       * UINT32_MAX.
       */
      T_eq_u32( ctx->result, UINT32_MAX );
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Post_Result_Zero: {
      /*
       * The rtems_counter_sbintime_to_ticks() directive shall return zero.
       */
      T_eq_u32( ctx->result, 0 );
      break;
    }

    case RtemsCounterReqSbintimeToTicks_Post_Result_NA:
      break;
  }
}

static void RtemsCounterReqSbintimeToTicks_Action(
  RtemsCounterReqSbintimeToTicks_Context *ctx
)
{
  ctx->result = rtems_counter_sbintime_to_ticks( ctx->sbintime );
}

static void RtemsCounterReqSbintimeToTicks_Cleanup( void )
{
  rtems_counter_initialize_converter( rtems_counter_frequency() );
}

/* clang-format off */

static const RtemsCounterReqSbintimeToTicks_Entry
RtemsCounterReqSbintimeToTicks_Entries[] = {
  { 0, 0, 0, RtemsCounterReqSbintimeToTicks_Post_Result_Bounded },
  { 0, 0, 0, RtemsCounterReqSbintimeToTicks_Post_Result_Zero },
  { 1, 0, 0, RtemsCounterReqSbintimeToTicks_Post_Result_NA },
  { 0, 0, 0, RtemsCounterReqSbintimeToTicks_Post_Result_Saturated }
};

static const uint8_t
RtemsCounterReqSbintimeToTicks_Map[] = {
  1, 0, 0, 0, 2, 1, 0, 0, 0, 2, 1, 0, 0, 0, 3, 1, 0, 0, 0, 3
};

/* clang-format on */

static size_t RtemsCounterReqSbintimeToTicks_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  RtemsCounterReqSbintimeToTicks_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsCounterReqSbintimeToTicks_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsCounterReqSbintimeToTicks_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = RtemsCounterReqSbintimeToTicks_Scope,
  .initial_context = &RtemsCounterReqSbintimeToTicks_Instance
};

static inline RtemsCounterReqSbintimeToTicks_Entry
RtemsCounterReqSbintimeToTicks_PopEntry(
  RtemsCounterReqSbintimeToTicks_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsCounterReqSbintimeToTicks_Entries
    [ RtemsCounterReqSbintimeToTicks_Map[ index ] ];
}

static void RtemsCounterReqSbintimeToTicks_TestVariant(
  RtemsCounterReqSbintimeToTicks_Context *ctx
)
{
  RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  RtemsCounterReqSbintimeToTicks_Action( ctx );
  RtemsCounterReqSbintimeToTicks_Post_Result_Check(
    ctx,
    ctx->Map.entry.Post_Result
  );
}

/**
 * @fn void T_case_body_RtemsCounterReqSbintimeToTicks( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsCounterReqSbintimeToTicks,
  &RtemsCounterReqSbintimeToTicks_Fixture
)
{
  RtemsCounterReqSbintimeToTicks_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] =
      RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_Min;
    ctx->Map.pcs[ 0 ] <
    RtemsCounterReqSbintimeToTicks_Pre_ConverterFrequency_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = RtemsCounterReqSbintimeToTicks_Pre_Sbintime_Negative;
      ctx->Map.pcs[ 1 ] < RtemsCounterReqSbintimeToTicks_Pre_Sbintime_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      ctx->Map.entry = RtemsCounterReqSbintimeToTicks_PopEntry( ctx );

      if ( ctx->Map.entry.Skip ) {
        continue;
      }

      RtemsCounterReqSbintimeToTicks_TestVariant( ctx );
      RtemsCounterReqSbintimeToTicks_Cleanup();
    }
  }
}

/** @} */
