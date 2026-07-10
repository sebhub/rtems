/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CStringsReqFlsl
 */

/*
 * Copyright (C) 2021 embedded brains GmbH & Co. KG
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

#include <limits.h>
#include <strings.h>

#include <rtems/test.h>

/**
 * @defgroup CStringsReqFlsl spec:/c/strings/req/flsl
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  CStringsReqFlsl_Pre_Value_Zero,
  CStringsReqFlsl_Pre_Value_NonZero,
  CStringsReqFlsl_Pre_Value_NA
} CStringsReqFlsl_Pre_Value;

typedef enum {
  CStringsReqFlsl_Post_Result_Zero,
  CStringsReqFlsl_Post_Result_LastBitSet,
  CStringsReqFlsl_Post_Result_NA
} CStringsReqFlsl_Post_Result;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Value_NA : 1;
  uint8_t Post_Result : 2;
} CStringsReqFlsl_Entry;

/**
 * @brief Test context for spec:/c/strings/req/flsl test case.
 */
typedef struct {
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
    CStringsReqFlsl_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CStringsReqFlsl_Context;

static CStringsReqFlsl_Context CStringsReqFlsl_Instance;

static const char *const CStringsReqFlsl_PreDesc_Value[] =
  { "Zero", "NonZero", "NA" };

static const char *const *const CStringsReqFlsl_PreDesc[] = {
  CStringsReqFlsl_PreDesc_Value,
  NULL
};

static void CStringsReqFlsl_Pre_Value_Prepare(
  CStringsReqFlsl_Pre_Value state
)
{
  switch ( state ) {
    case CStringsReqFlsl_Pre_Value_Zero: {
      /*
       * While the parameter value is equal to zero.
       */
      /* Nothing to prepare */
      break;
    }

    case CStringsReqFlsl_Pre_Value_NonZero: {
      /*
       * While the parameter value is not equal to zero.
       */
      /* Nothing to prepare */
      break;
    }

    case CStringsReqFlsl_Pre_Value_NA:
      break;
  }
}

static void CStringsReqFlsl_Post_Result_Check(
  CStringsReqFlsl_Post_Result state
)
{
  int    expected_result;
  long   value;
  size_t i;

  switch ( state ) {
    case CStringsReqFlsl_Post_Result_Zero: {
      /*
       * The return value of flsl() shall be equal to zero.
       */
      T_eq_int( flsl( 0 ), 0 );
      break;
    }

    case CStringsReqFlsl_Post_Result_LastBitSet: {
      /*
       * The return value of flsl() shall be equal to the index of the
       * most-significant bit set in the parameter value.
       */
      expected_result = 1;
      value = 1;

      for ( i = 0; i < sizeof( long ) * CHAR_BIT; ++i ) {
        T_eq_int( flsl( value ), expected_result );
        ++expected_result;
        value <<= 1;
      }
      break;
    }

    case CStringsReqFlsl_Post_Result_NA:
      break;
  }
}

static void CStringsReqFlsl_Action( void )
{
  /* The action is performed in the post-condition states */
}

/* clang-format off */

static const CStringsReqFlsl_Entry
CStringsReqFlsl_Entries[] = {
  { 0, 0, CStringsReqFlsl_Post_Result_Zero },
  { 0, 0, CStringsReqFlsl_Post_Result_LastBitSet }
};

static const uint8_t
CStringsReqFlsl_Map[] = {
  0, 1
};

/* clang-format on */

static size_t CStringsReqFlsl_Scope( void *arg, char *buf, size_t n )
{
  CStringsReqFlsl_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CStringsReqFlsl_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CStringsReqFlsl_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CStringsReqFlsl_Scope,
  .initial_context = &CStringsReqFlsl_Instance
};

static inline CStringsReqFlsl_Entry CStringsReqFlsl_PopEntry(
  CStringsReqFlsl_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CStringsReqFlsl_Entries[ CStringsReqFlsl_Map[ index ] ];
}

static void CStringsReqFlsl_TestVariant( CStringsReqFlsl_Context *ctx )
{
  CStringsReqFlsl_Pre_Value_Prepare( ctx->Map.pcs[ 0 ] );
  CStringsReqFlsl_Action();
  CStringsReqFlsl_Post_Result_Check( ctx->Map.entry.Post_Result );
}

/**
 * @fn void T_case_body_CStringsReqFlsl( void )
 */
T_TEST_CASE_FIXTURE( CStringsReqFlsl, &CStringsReqFlsl_Fixture )
{
  CStringsReqFlsl_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = CStringsReqFlsl_Pre_Value_Zero;
    ctx->Map.pcs[ 0 ] < CStringsReqFlsl_Pre_Value_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = CStringsReqFlsl_PopEntry( ctx );
    CStringsReqFlsl_TestVariant( ctx );
  }
}

/** @} */
