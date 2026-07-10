/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CStringsReqExplicitBzero
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

#include <string.h>

#include <rtems/test.h>

/**
 * @defgroup CStringsReqExplicitBzero spec:/c/strings/req/explicit-bzero
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CStringsReqExplicitBzero_Pre_Length_Zero,
  CStringsReqExplicitBzero_Pre_Length_Positive,
  CStringsReqExplicitBzero_Pre_Length_NA
} CStringsReqExplicitBzero_Pre_Length;

typedef enum {
  CStringsReqExplicitBzero_Post_Memory_Nop,
  CStringsReqExplicitBzero_Post_Memory_Zeroed,
  CStringsReqExplicitBzero_Post_Memory_NA
} CStringsReqExplicitBzero_Post_Memory;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Length_NA : 1;
  uint8_t Post_Memory : 2;
} CStringsReqExplicitBzero_Entry;

/**
 * @brief Test context for spec:/c/strings/req/explicit-bzero test case.
 */
typedef struct {
  /**
   * @brief This member provides the buffer.
   */
  char buf[ 16 ];

  /**
   * @brief This member specifies the `s` parameter value.
   */
  void *s;

  /**
   * @brief This member specifies the `n` parameter value.
   */
  size_t n;

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
    CStringsReqExplicitBzero_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CStringsReqExplicitBzero_Context;

static CStringsReqExplicitBzero_Context CStringsReqExplicitBzero_Instance;

static const char *const CStringsReqExplicitBzero_PreDesc_Length[] =
  { "Zero", "Positive", "NA" };

static const char *const *const CStringsReqExplicitBzero_PreDesc[] = {
  CStringsReqExplicitBzero_PreDesc_Length,
  NULL
};

static void CStringsReqExplicitBzero_Pre_Length_Prepare(
  CStringsReqExplicitBzero_Context   *ctx,
  CStringsReqExplicitBzero_Pre_Length state
)
{
  switch ( state ) {
    case CStringsReqExplicitBzero_Pre_Length_Zero: {
      /*
       * While the value specified by `n` is zero.
       */
      ctx->n = 0;
      break;
    }

    case CStringsReqExplicitBzero_Pre_Length_Positive: {
      /*
       * While the value specified by `n` is greater than zero.
       */
      ctx->n = 3;
      break;
    }

    case CStringsReqExplicitBzero_Pre_Length_NA:
      break;
  }
}

static void CStringsReqExplicitBzero_Post_Memory_Check(
  CStringsReqExplicitBzero_Context    *ctx,
  CStringsReqExplicitBzero_Post_Memory state
)
{
  switch ( state ) {
    case CStringsReqExplicitBzero_Post_Memory_Nop: {
      /*
       * The memory area referenced by `s` shall not be modified.
       */
      size_t i;

      for ( i = 0; i < sizeof( ctx->buf ); ++i ) {
        T_eq_int( (unsigned char) ctx->buf[ i ], 0xff );
      }
      break;
    }

    case CStringsReqExplicitBzero_Post_Memory_Zeroed: {
      /*
       * The memory area referenced by `s` shall be filled with zeros up to the
       * number of bytes specified by `n`.
       */
      size_t i;

      for ( i = 0; i < ctx->n; ++i ) {
        T_eq_int( ctx->buf[ i ], 0 );
      }

      for ( i = ctx->n; i < sizeof( ctx->buf ); ++i ) {
        T_eq_int( (unsigned char) ctx->buf[ i ], 0xff );
      }
      break;
    }

    case CStringsReqExplicitBzero_Post_Memory_NA:
      break;
  }
}

static void CStringsReqExplicitBzero_Prepare(
  CStringsReqExplicitBzero_Context *ctx
)
{
  memset( ctx->buf, 0xff, sizeof( ctx->buf ) );
  ctx->s = ctx->buf;
}

static void CStringsReqExplicitBzero_Action(
  CStringsReqExplicitBzero_Context *ctx
)
{
  explicit_bzero( ctx->s, ctx->n );
}

/* clang-format off */

static const CStringsReqExplicitBzero_Entry
CStringsReqExplicitBzero_Entries[] = {
  { 0, 0, CStringsReqExplicitBzero_Post_Memory_Nop },
  { 0, 0, CStringsReqExplicitBzero_Post_Memory_Zeroed }
};

static const uint8_t
CStringsReqExplicitBzero_Map[] = {
  0, 1
};

/* clang-format on */

static size_t CStringsReqExplicitBzero_Scope( void *arg, char *buf, size_t n )
{
  CStringsReqExplicitBzero_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CStringsReqExplicitBzero_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CStringsReqExplicitBzero_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CStringsReqExplicitBzero_Scope,
  .initial_context = &CStringsReqExplicitBzero_Instance
};

static inline CStringsReqExplicitBzero_Entry CStringsReqExplicitBzero_PopEntry(
  CStringsReqExplicitBzero_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CStringsReqExplicitBzero_Entries
    [ CStringsReqExplicitBzero_Map[ index ] ];
}

static void CStringsReqExplicitBzero_TestVariant(
  CStringsReqExplicitBzero_Context *ctx
)
{
  CStringsReqExplicitBzero_Pre_Length_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CStringsReqExplicitBzero_Action( ctx );
  CStringsReqExplicitBzero_Post_Memory_Check(
    ctx,
    ctx->Map.entry.Post_Memory
  );
}

/**
 * @fn void T_case_body_CStringsReqExplicitBzero( void )
 */
T_TEST_CASE_FIXTURE(
  CStringsReqExplicitBzero,
  &CStringsReqExplicitBzero_Fixture
)
{
  CStringsReqExplicitBzero_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = CStringsReqExplicitBzero_Pre_Length_Zero;
    ctx->Map.pcs[ 0 ] < CStringsReqExplicitBzero_Pre_Length_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = CStringsReqExplicitBzero_PopEntry( ctx );
    CStringsReqExplicitBzero_Prepare( ctx );
    CStringsReqExplicitBzero_TestVariant( ctx );
  }
}

/** @} */
