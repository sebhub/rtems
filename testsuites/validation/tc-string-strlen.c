/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CStringReqStrlen
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
 * @defgroup CStringReqStrlen spec:/c/string/req/strlen
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CStringReqStrlen_Pre_Alignment_Aligned,
  CStringReqStrlen_Pre_Alignment_Unaligned,
  CStringReqStrlen_Pre_Alignment_NA
} CStringReqStrlen_Pre_Alignment;

typedef enum {
  CStringReqStrlen_Pre_Position_Prefix,
  CStringReqStrlen_Pre_Position_FirstBlock,
  CStringReqStrlen_Pre_Position_SecondBlock,
  CStringReqStrlen_Pre_Position_NA
} CStringReqStrlen_Pre_Position;

typedef enum {
  CStringReqStrlen_Post_Result_Length,
  CStringReqStrlen_Post_Result_NA
} CStringReqStrlen_Post_Result;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Alignment_NA : 1;
  uint8_t Pre_Position_NA : 1;
  uint8_t Post_Result : 1;
} CStringReqStrlen_Entry;

/**
 * @brief Test context for spec:/c/string/req/strlen test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `s` parameter value.
   */
  const char *s;

  /**
   * @brief This member contains the return value.
   */
  size_t retval;

  /**
   * @brief This member specifies the offset of the string in the buffer.
   */
  size_t offset;

  /**
   * @brief This member specifies the position of the terminating null
   *   character in the buffer.
   */
  size_t position;

  /**
   * @brief This member provides the buffer of the string, aligned to the size
   *   of type long.
   */
  long buf[ 16 ];

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
    CStringReqStrlen_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CStringReqStrlen_Context;

static CStringReqStrlen_Context CStringReqStrlen_Instance;

static const char *const CStringReqStrlen_PreDesc_Alignment[] =
  { "Aligned", "Unaligned", "NA" };

static const char *const CStringReqStrlen_PreDesc_Position[] =
  { "Prefix", "FirstBlock", "SecondBlock", "NA" };

static const char *const *const CStringReqStrlen_PreDesc[] = {
  CStringReqStrlen_PreDesc_Alignment,
  CStringReqStrlen_PreDesc_Position,
  NULL
};

typedef CStringReqStrlen_Context Context;

static size_t AlignUp( size_t offset )
{
  return ( offset + sizeof( long ) - 1 ) & ~( sizeof( long ) - 1 );
}

static void Arrange( Context *ctx )
{
  char  *b;
  size_t i;

  b = (char *) ctx->buf;

  for ( i = 0; i < sizeof( ctx->buf ); ++i ) {
    b[ i ] = (char) ( 'A' + i % 26 );
  }

  b[ ctx->position ] = '\0';
  ctx->s = b + ctx->offset;
}

static void CStringReqStrlen_Pre_Alignment_Prepare(
  CStringReqStrlen_Context      *ctx,
  CStringReqStrlen_Pre_Alignment state
)
{
  switch ( state ) {
    case CStringReqStrlen_Pre_Alignment_Aligned: {
      /*
       * While the string referenced by the `s` parameter starts at an address
       * which is aligned to the size of type long.
       */
      ctx->offset = 0;
      break;
    }

    case CStringReqStrlen_Pre_Alignment_Unaligned: {
      /*
       * While the string referenced by the `s` parameter starts at an address
       * which is not aligned to the size of type long.
       */
      ctx->offset = 1;
      break;
    }

    case CStringReqStrlen_Pre_Alignment_NA:
      break;
  }
}

static void CStringReqStrlen_Pre_Position_Prepare(
  CStringReqStrlen_Context     *ctx,
  CStringReqStrlen_Pre_Position state
)
{
  switch ( state ) {
    case CStringReqStrlen_Pre_Position_Prefix: {
      /*
       * While the terminating null character of the string is located before
       * the first address aligned to the size of type long.
       */
      ctx->position = ctx->offset + 1;
      break;
    }

    case CStringReqStrlen_Pre_Position_FirstBlock: {
      /*
       * While the terminating null character of the string is located at the
       * start of the first block of the size of type long which starts at an
       * aligned address.
       */
      ctx->position = AlignUp( ctx->offset );
      break;
    }

    case CStringReqStrlen_Pre_Position_SecondBlock: {
      /*
       * While the terminating null character of the string is located in the
       * second block of the size of type long which starts at an aligned
       * address, but not at the start of this block.
       */
      ctx->position = AlignUp( ctx->offset ) + sizeof( long ) + 1;
      break;
    }

    case CStringReqStrlen_Pre_Position_NA:
      break;
  }
}

static void CStringReqStrlen_Post_Result_Check(
  CStringReqStrlen_Context    *ctx,
  CStringReqStrlen_Post_Result state
)
{
  switch ( state ) {
    case CStringReqStrlen_Post_Result_Length: {
      /*
       * The return value of strlen() shall be the count of characters which
       * precede the terminating null character of the string referenced by the
       * `s` parameter.
       */
      T_eq_sz( ctx->retval, ctx->position - ctx->offset );
      break;
    }

    case CStringReqStrlen_Post_Result_NA:
      break;
  }
}

static void CStringReqStrlen_Prepare( CStringReqStrlen_Context *ctx )
{
  ctx->retval = 0;
  ctx->offset = 0;
  ctx->position = 0;
}

static void CStringReqStrlen_Action( CStringReqStrlen_Context *ctx )
{
  Arrange( ctx );
  ctx->retval = strlen( ctx->s );
}

/* clang-format off */

static const CStringReqStrlen_Entry
CStringReqStrlen_Entries[] = {
  { 0, 0, 0, CStringReqStrlen_Post_Result_Length },
  { 1, 0, 0, CStringReqStrlen_Post_Result_NA }
};

static const uint8_t
CStringReqStrlen_Map[] = {
  1, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t CStringReqStrlen_Scope( void *arg, char *buf, size_t n )
{
  CStringReqStrlen_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CStringReqStrlen_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CStringReqStrlen_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CStringReqStrlen_Scope,
  .initial_context = &CStringReqStrlen_Instance
};

static inline CStringReqStrlen_Entry CStringReqStrlen_PopEntry(
  CStringReqStrlen_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CStringReqStrlen_Entries[ CStringReqStrlen_Map[ index ] ];
}

static void CStringReqStrlen_TestVariant( CStringReqStrlen_Context *ctx )
{
  CStringReqStrlen_Pre_Alignment_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CStringReqStrlen_Pre_Position_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CStringReqStrlen_Action( ctx );
  CStringReqStrlen_Post_Result_Check( ctx, ctx->Map.entry.Post_Result );
}

/**
 * @fn void T_case_body_CStringReqStrlen( void )
 */
T_TEST_CASE_FIXTURE( CStringReqStrlen, &CStringReqStrlen_Fixture )
{
  CStringReqStrlen_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = CStringReqStrlen_Pre_Alignment_Aligned;
    ctx->Map.pcs[ 0 ] < CStringReqStrlen_Pre_Alignment_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = CStringReqStrlen_Pre_Position_Prefix;
      ctx->Map.pcs[ 1 ] < CStringReqStrlen_Pre_Position_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      ctx->Map.entry = CStringReqStrlen_PopEntry( ctx );

      if ( ctx->Map.entry.Skip ) {
        continue;
      }

      CStringReqStrlen_Prepare( ctx );
      CStringReqStrlen_TestVariant( ctx );
    }
  }
}

/** @} */
