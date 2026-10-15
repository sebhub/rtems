/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsCounterReqInitializeConverter
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
 * @defgroup RtemsCounterReqInitializeConverter \
 *   spec:/rtems/counter/req/initialize-converter
 *
 * @ingroup TestsuitesValidation0
 *
 * The test calls the directive with the frequency of the variant. It converts
 * one second in each direction, which needs no rounding, so a converter with
 * another frequency gives another result.
 *
 * @{
 */

typedef enum {
  RtemsCounterReqInitializeConverter_Pre_Frequency_Min,
  RtemsCounterReqInitializeConverter_Pre_Frequency_Mid,
  RtemsCounterReqInitializeConverter_Pre_Frequency_Max,
  RtemsCounterReqInitializeConverter_Pre_Frequency_NA
} RtemsCounterReqInitializeConverter_Pre_Frequency;

typedef enum {
  RtemsCounterReqInitializeConverter_Post_ConverterFrequency_Set,
  RtemsCounterReqInitializeConverter_Post_ConverterFrequency_NA
} RtemsCounterReqInitializeConverter_Post_ConverterFrequency;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Frequency_NA : 1;
  uint8_t Post_ConverterFrequency : 1;
} RtemsCounterReqInitializeConverter_Entry;

/**
 * @brief Test context for spec:/rtems/counter/req/initialize-converter test
 *   case.
 */
typedef struct {
  /**
   * @brief This member contains the frequency parameter value.
   */
  uint32_t frequency;

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
    RtemsCounterReqInitializeConverter_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsCounterReqInitializeConverter_Context;

static RtemsCounterReqInitializeConverter_Context
  RtemsCounterReqInitializeConverter_Instance;

static const char *const
  RtemsCounterReqInitializeConverter_PreDesc_Frequency[] =
    { "Min", "Mid", "Max", "NA" };

static const char *const *const RtemsCounterReqInitializeConverter_PreDesc[] =
  { RtemsCounterReqInitializeConverter_PreDesc_Frequency, NULL };

static void RtemsCounterReqInitializeConverter_Pre_Frequency_Prepare(
  RtemsCounterReqInitializeConverter_Context      *ctx,
  RtemsCounterReqInitializeConverter_Pre_Frequency state
)
{
  switch ( state ) {
    case RtemsCounterReqInitializeConverter_Pre_Frequency_Min: {
      /*
       * While the `frequency` parameter is equal to one.
       */
      ctx->frequency = 1;
      break;
    }

    case RtemsCounterReqInitializeConverter_Pre_Frequency_Mid: {
      /*
       * While the `frequency` parameter is greater than one, while the
       * `frequency` parameter is less than the maximum value of uint32_t.
       */
      ctx->frequency = UINT32_C( 123456789 );
      break;
    }

    case RtemsCounterReqInitializeConverter_Pre_Frequency_Max: {
      /*
       * While the `frequency` parameter is equal to the maximum value of
       * uint32_t.
       */
      ctx->frequency = UINT32_MAX;
      break;
    }

    case RtemsCounterReqInitializeConverter_Pre_Frequency_NA:
      break;
  }
}

static void RtemsCounterReqInitializeConverter_Post_ConverterFrequency_Check(
  RtemsCounterReqInitializeConverter_Context                *ctx,
  RtemsCounterReqInitializeConverter_Post_ConverterFrequency state
)
{
  int64_t sbt;

  switch ( state ) {
    case RtemsCounterReqInitializeConverter_Post_ConverterFrequency_Set: {
      /*
       * The converter frequency shall be equal to the `frequency` parameter
       * value.
       */
      T_eq_u64(
        rtems_counter_ticks_to_nanoseconds( ctx->frequency ),
        1000000000
      );
      T_eq_u32(
        rtems_counter_nanoseconds_to_ticks( 1000000000 ),
        ctx->frequency
      );
      sbt = rtems_counter_ticks_to_sbintime( ctx->frequency );
      T_ge_i64( sbt, INT64_C( 4294967296 ) );
      T_le_i64( sbt, INT64_C( 4294967298 ) );
      T_eq_u32(
        rtems_counter_sbintime_to_ticks( INT64_C( 4294967296 ) ),
        ctx->frequency
      );
      break;
    }

    case RtemsCounterReqInitializeConverter_Post_ConverterFrequency_NA:
      break;
  }
}

static void RtemsCounterReqInitializeConverter_Action(
  RtemsCounterReqInitializeConverter_Context *ctx
)
{
  rtems_counter_initialize_converter( ctx->frequency );
}

static void RtemsCounterReqInitializeConverter_Cleanup( void )
{
  rtems_counter_initialize_converter( rtems_counter_frequency() );
}

/* clang-format off */

static const RtemsCounterReqInitializeConverter_Entry
RtemsCounterReqInitializeConverter_Entries[] = {
  { 0, 0, RtemsCounterReqInitializeConverter_Post_ConverterFrequency_Set }
};

static const uint8_t
RtemsCounterReqInitializeConverter_Map[] = {
  0, 0, 0
};

/* clang-format on */

static size_t RtemsCounterReqInitializeConverter_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  RtemsCounterReqInitializeConverter_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsCounterReqInitializeConverter_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsCounterReqInitializeConverter_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = RtemsCounterReqInitializeConverter_Scope,
  .initial_context = &RtemsCounterReqInitializeConverter_Instance
};

static inline RtemsCounterReqInitializeConverter_Entry
RtemsCounterReqInitializeConverter_PopEntry(
  RtemsCounterReqInitializeConverter_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsCounterReqInitializeConverter_Entries
    [ RtemsCounterReqInitializeConverter_Map[ index ] ];
}

static void RtemsCounterReqInitializeConverter_TestVariant(
  RtemsCounterReqInitializeConverter_Context *ctx
)
{
  RtemsCounterReqInitializeConverter_Pre_Frequency_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  RtemsCounterReqInitializeConverter_Action( ctx );
  RtemsCounterReqInitializeConverter_Post_ConverterFrequency_Check(
    ctx,
    ctx->Map.entry.Post_ConverterFrequency
  );
}

/**
 * @fn void T_case_body_RtemsCounterReqInitializeConverter( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsCounterReqInitializeConverter,
  &RtemsCounterReqInitializeConverter_Fixture
)
{
  RtemsCounterReqInitializeConverter_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = RtemsCounterReqInitializeConverter_Pre_Frequency_Min;
    ctx->Map.pcs[ 0 ] < RtemsCounterReqInitializeConverter_Pre_Frequency_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = RtemsCounterReqInitializeConverter_PopEntry( ctx );
    RtemsCounterReqInitializeConverter_TestVariant( ctx );
    RtemsCounterReqInitializeConverter_Cleanup();
  }
}

/** @} */
