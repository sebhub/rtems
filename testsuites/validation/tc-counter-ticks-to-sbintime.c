/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsCounterReqTicksToSbintime
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
 * @defgroup RtemsCounterReqTicksToSbintime \
 *   spec:/rtems/counter/req/ticks-to-sbintime
 *
 * @ingroup TestsuitesValidation0
 *
 * The test initializes the converter with the frequency of the variant. It
 * computes the limit of the variant from the frequency, independent of the
 * converter. It converts the counter ticks value of the variant and compares
 * the result with the time rounded down and with this value plus two.
 *
 * @{
 */

typedef enum {
  RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_Min,
  RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_Mid,
  RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_Max,
  RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_NA
} RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency;

typedef enum {
  RtemsCounterReqTicksToSbintime_Pre_Ticks_Integral,
  RtemsCounterReqTicksToSbintime_Pre_Ticks_Fraction,
  RtemsCounterReqTicksToSbintime_Pre_Ticks_Limit,
  RtemsCounterReqTicksToSbintime_Pre_Ticks_Beyond,
  RtemsCounterReqTicksToSbintime_Pre_Ticks_NA
} RtemsCounterReqTicksToSbintime_Pre_Ticks;

typedef enum {
  RtemsCounterReqTicksToSbintime_Post_Result_Bounded,
  RtemsCounterReqTicksToSbintime_Post_Result_Saturated,
  RtemsCounterReqTicksToSbintime_Post_Result_NA
} RtemsCounterReqTicksToSbintime_Post_Result;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_ConverterFrequency_NA : 1;
  uint8_t Pre_Ticks_NA : 1;
  uint8_t Post_Result : 2;
} RtemsCounterReqTicksToSbintime_Entry;

/**
 * @brief Test context for spec:/rtems/counter/req/ticks-to-sbintime test case.
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
  int64_t result;

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
    RtemsCounterReqTicksToSbintime_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsCounterReqTicksToSbintime_Context;

static RtemsCounterReqTicksToSbintime_Context
  RtemsCounterReqTicksToSbintime_Instance;

static const char *const
  RtemsCounterReqTicksToSbintime_PreDesc_ConverterFrequency[] =
    { "Min", "Mid", "Max", "NA" };

static const char *const RtemsCounterReqTicksToSbintime_PreDesc_Ticks[] =
  { "Integral", "Fraction", "Limit", "Beyond", "NA" };

static const char *const *const RtemsCounterReqTicksToSbintime_PreDesc[] = {
  RtemsCounterReqTicksToSbintime_PreDesc_ConverterFrequency,
  RtemsCounterReqTicksToSbintime_PreDesc_Ticks,
  NULL
};

static rtems_counter_ticks Limit( uint32_t frequency )
{
  uint64_t limit;

  limit = ( (uint64_t) frequency << 31 ) - 1;

  if ( limit > UINT32_MAX ) {
    limit = UINT32_MAX;
  }

  return (rtems_counter_ticks) limit;
}

static void RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_Prepare(
  RtemsCounterReqTicksToSbintime_Context               *ctx,
  RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency state
)
{
  switch ( state ) {
    case RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_Min: {
      /*
       * While the converter frequency is equal to one.
       */
      ctx->frequency = 1;
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_Mid: {
      /*
       * While the converter frequency is greater than one, while the converter
       * frequency is less than the maximum value of uint32_t.
       */
      ctx->frequency = UINT32_C( 123456789 );
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_Max: {
      /*
       * While the converter frequency is equal to the maximum value of
       * uint32_t.
       */
      ctx->frequency = UINT32_MAX;
      rtems_counter_initialize_converter( ctx->frequency );
      break;
    }

    case RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_NA:
      break;
  }
}

static void RtemsCounterReqTicksToSbintime_Pre_Ticks_Prepare(
  RtemsCounterReqTicksToSbintime_Context  *ctx,
  RtemsCounterReqTicksToSbintime_Pre_Ticks state
)
{
  switch ( state ) {
    case RtemsCounterReqTicksToSbintime_Pre_Ticks_Integral: {
      /*
       * While the `ticks` parameter is less than the largest value of
       * rtems_counter_ticks which represents a time shorter than 2147483648
       * seconds at the converter frequency, while the `ticks` parameter
       * represents a whole number of seconds at the converter frequency.
       */
      ctx->ticks = Limit( ctx->frequency ) > ctx->frequency ? ctx->frequency
                                                            : 0;
      break;
    }

    case RtemsCounterReqTicksToSbintime_Pre_Ticks_Fraction: {
      /*
       * While the `ticks` parameter is less than the largest value of
       * rtems_counter_ticks which represents a time shorter than 2147483648
       * seconds at the converter frequency, while the `ticks` parameter
       * represents no whole number of seconds at the converter frequency.
       */
      ctx->ticks = 1;
      break;
    }

    case RtemsCounterReqTicksToSbintime_Pre_Ticks_Limit: {
      /*
       * While the `ticks` parameter is equal to the largest value of
       * rtems_counter_ticks which represents a time shorter than 2147483648
       * seconds at the converter frequency.
       */
      ctx->ticks = Limit( ctx->frequency );
      break;
    }

    case RtemsCounterReqTicksToSbintime_Pre_Ticks_Beyond: {
      /*
       * While the `ticks` parameter is greater than the largest value of
       * rtems_counter_ticks which represents a time shorter than 2147483648
       * seconds at the converter frequency.
       */
      ctx->ticks = Limit( ctx->frequency ) + 1;
      break;
    }

    case RtemsCounterReqTicksToSbintime_Pre_Ticks_NA:
      break;
  }
}

static void RtemsCounterReqTicksToSbintime_Post_Result_Check(
  RtemsCounterReqTicksToSbintime_Context    *ctx,
  RtemsCounterReqTicksToSbintime_Post_Result state
)
{
  uint64_t lower;

  switch ( state ) {
    case RtemsCounterReqTicksToSbintime_Post_Result_Bounded: {
      /*
       * The rtems_counter_ticks_to_sbintime() directive shall return a value
       * which is greater than the time in signed binary time which the `ticks`
       * parameter represents at the converter frequency minus one and less
       * than or equal to this time plus two.
       */
      lower = ( (uint64_t) ctx->ticks << 32 ) / ctx->frequency;
      T_ge_u64( (uint64_t) ctx->result, lower );
      T_le_u64( (uint64_t) ctx->result, lower + 2 );
      break;
    }

    case RtemsCounterReqTicksToSbintime_Post_Result_Saturated: {
      /*
       * The rtems_counter_ticks_to_sbintime() directive shall return the
       * maximum value of int64_t.
       */
      T_eq_i64( ctx->result, INT64_MAX );
      break;
    }

    case RtemsCounterReqTicksToSbintime_Post_Result_NA:
      break;
  }
}

static void RtemsCounterReqTicksToSbintime_Action(
  RtemsCounterReqTicksToSbintime_Context *ctx
)
{
  ctx->result = rtems_counter_ticks_to_sbintime( ctx->ticks );
}

static void RtemsCounterReqTicksToSbintime_Cleanup( void )
{
  rtems_counter_initialize_converter( rtems_counter_frequency() );
}

/* clang-format off */

static const RtemsCounterReqTicksToSbintime_Entry
RtemsCounterReqTicksToSbintime_Entries[] = {
  { 0, 0, 0, RtemsCounterReqTicksToSbintime_Post_Result_Bounded },
  { 1, 0, 0, RtemsCounterReqTicksToSbintime_Post_Result_NA },
  { 1, 0, 0, RtemsCounterReqTicksToSbintime_Post_Result_NA },
  { 0, 0, 0, RtemsCounterReqTicksToSbintime_Post_Result_Saturated }
};

static const uint8_t
RtemsCounterReqTicksToSbintime_Map[] = {
  0, 2, 0, 3, 0, 0, 0, 1, 0, 0, 0, 1
};

/* clang-format on */

static size_t RtemsCounterReqTicksToSbintime_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  RtemsCounterReqTicksToSbintime_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsCounterReqTicksToSbintime_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsCounterReqTicksToSbintime_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = RtemsCounterReqTicksToSbintime_Scope,
  .initial_context = &RtemsCounterReqTicksToSbintime_Instance
};

static inline RtemsCounterReqTicksToSbintime_Entry
RtemsCounterReqTicksToSbintime_PopEntry(
  RtemsCounterReqTicksToSbintime_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsCounterReqTicksToSbintime_Entries
    [ RtemsCounterReqTicksToSbintime_Map[ index ] ];
}

static void RtemsCounterReqTicksToSbintime_TestVariant(
  RtemsCounterReqTicksToSbintime_Context *ctx
)
{
  RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  RtemsCounterReqTicksToSbintime_Pre_Ticks_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  RtemsCounterReqTicksToSbintime_Action( ctx );
  RtemsCounterReqTicksToSbintime_Post_Result_Check(
    ctx,
    ctx->Map.entry.Post_Result
  );
}

/**
 * @fn void T_case_body_RtemsCounterReqTicksToSbintime( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsCounterReqTicksToSbintime,
  &RtemsCounterReqTicksToSbintime_Fixture
)
{
  RtemsCounterReqTicksToSbintime_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] =
      RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_Min;
    ctx->Map.pcs[ 0 ] <
    RtemsCounterReqTicksToSbintime_Pre_ConverterFrequency_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = RtemsCounterReqTicksToSbintime_Pre_Ticks_Integral;
      ctx->Map.pcs[ 1 ] < RtemsCounterReqTicksToSbintime_Pre_Ticks_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      ctx->Map.entry = RtemsCounterReqTicksToSbintime_PopEntry( ctx );

      if ( ctx->Map.entry.Skip ) {
        continue;
      }

      RtemsCounterReqTicksToSbintime_TestVariant( ctx );
      RtemsCounterReqTicksToSbintime_Cleanup();
    }
  }
}

/** @} */
