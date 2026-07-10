/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CStringReqStrrchr
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
 * @defgroup CStringReqStrrchr spec:/c/string/req/strrchr
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CStringReqStrrchr_Pre_C_Zero,
  CStringReqStrrchr_Pre_C_NonZero,
  CStringReqStrrchr_Pre_C_NA
} CStringReqStrrchr_Pre_C;

typedef enum {
  CStringReqStrrchr_Pre_Occurrences_None,
  CStringReqStrrchr_Pre_Occurrences_One,
  CStringReqStrrchr_Pre_Occurrences_Several,
  CStringReqStrrchr_Pre_Occurrences_NA
} CStringReqStrrchr_Pre_Occurrences;

typedef enum {
  CStringReqStrrchr_Post_Result_Last,
  CStringReqStrrchr_Post_Result_Null,
  CStringReqStrrchr_Post_Result_NA
} CStringReqStrrchr_Post_Result;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_C_NA : 1;
  uint8_t Pre_Occurrences_NA : 1;
  uint8_t Post_Result : 2;
} CStringReqStrrchr_Entry;

/**
 * @brief Test context for spec:/c/string/req/strrchr test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `s` parameter value.
   */
  const char *s;

  /**
   * @brief This member specifies the `c` parameter value.
   */
  int c;

  /**
   * @brief This member contains the return value.
   */
  char *retval;

  /**
   * @brief This member contains the expected return value.
   */
  const char *expected;

  /**
   * @brief This member specifies the count of occurrences of the character.
   */
  size_t count;

  /**
   * @brief This member provides the buffer of the string.
   */
  char buf[ 21 ];

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 2 ];

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
    CStringReqStrrchr_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CStringReqStrrchr_Context;

static CStringReqStrrchr_Context CStringReqStrrchr_Instance;

static const char *const CStringReqStrrchr_PreDesc_C[] =
  { "Zero", "NonZero", "NA" };

static const char *const CStringReqStrrchr_PreDesc_Occurrences[] =
  { "None", "One", "Several", "NA" };

static const char *const *const CStringReqStrrchr_PreDesc[] =
  { CStringReqStrrchr_PreDesc_C, CStringReqStrrchr_PreDesc_Occurrences, NULL };

typedef CStringReqStrrchr_Context Context;

static void Arrange( Context *ctx )
{
  static const size_t positions[] = { 15, 9, 3 };
  char               *b;
  size_t              i;

  b = ctx->buf;

  for ( i = 0; i < sizeof( ctx->buf ) - 1; ++i ) {
    b[ i ] = (char) ( 'A' + i % 26 );
  }

  b[ sizeof( ctx->buf ) - 1 ] = '\0';

  if ( (char) ctx->c == '\0' ) {
    ctx->expected = &b[ sizeof( ctx->buf ) - 1 ];
  } else if ( ctx->count == 1 ) {
    b[ 5 ] = (char) ctx->c;
    ctx->expected = &b[ 5 ];
  } else if ( ctx->count > 1 ) {
    for ( i = 0; i < RTEMS_ARRAY_SIZE( positions ); ++i ) {
      b[ positions[ i ] ] = (char) ctx->c;
    }

    ctx->expected = &b[ positions[ 0 ] ];
  }

  ctx->s = b;
}

static void CStringReqStrrchr_Pre_C_Prepare(
  CStringReqStrrchr_Context *ctx,
  CStringReqStrrchr_Pre_C    state
)
{
  switch ( state ) {
    case CStringReqStrrchr_Pre_C_Zero: {
      /*
       * While the `c` parameter converted to char is equal to the null
       * character.
       */
      ctx->c = 0x100;
      break;
    }

    case CStringReqStrrchr_Pre_C_NonZero: {
      /*
       * While the `c` parameter converted to char is not equal to the null
       * character.
       */
      ctx->c = 0x185;
      break;
    }

    case CStringReqStrrchr_Pre_C_NA:
      break;
  }
}

static void CStringReqStrrchr_Pre_Occurrences_Prepare(
  CStringReqStrrchr_Context        *ctx,
  CStringReqStrrchr_Pre_Occurrences state
)
{
  switch ( state ) {
    case CStringReqStrrchr_Pre_Occurrences_None: {
      /*
       * While the string referenced by the `s` parameter contains no character
       * which is equal to the `c` parameter converted to char.
       */
      ctx->count = 0;
      break;
    }

    case CStringReqStrrchr_Pre_Occurrences_One: {
      /*
       * While the string referenced by the `s` parameter contains exactly one
       * character which is equal to the `c` parameter converted to char.
       */
      ctx->count = 1;
      break;
    }

    case CStringReqStrrchr_Pre_Occurrences_Several: {
      /*
       * While the string referenced by the `s` parameter contains more than
       * one character which is equal to the `c` parameter converted to char.
       */
      ctx->count = 3;
      break;
    }

    case CStringReqStrrchr_Pre_Occurrences_NA:
      break;
  }
}

static void CStringReqStrrchr_Post_Result_Check(
  CStringReqStrrchr_Context    *ctx,
  CStringReqStrrchr_Post_Result state
)
{
  switch ( state ) {
    case CStringReqStrrchr_Post_Result_Last: {
      /*
       * The return value of strrchr() shall be the address of the last
       * character in the string referenced by the `s` parameter which is equal
       * to the `c` parameter converted to char.
       */
      T_eq_ptr( ctx->retval, ctx->expected );
      T_not_null( ctx->retval );
      break;
    }

    case CStringReqStrrchr_Post_Result_Null: {
      /*
       * The return value of strrchr() shall be NULL.
       */
      T_null( ctx->retval );
      break;
    }

    case CStringReqStrrchr_Post_Result_NA:
      break;
  }
}

static void CStringReqStrrchr_Prepare( CStringReqStrrchr_Context *ctx )
{
  ctx->retval = NULL;
  ctx->expected = NULL;
  ctx->count = 0;
}

static void CStringReqStrrchr_Action( CStringReqStrrchr_Context *ctx )
{
  Arrange( ctx );
  ctx->retval = strrchr( ctx->s, ctx->c );
}

/* clang-format off */

static const CStringReqStrrchr_Entry
CStringReqStrrchr_Entries[] = {
  { 1, 0, 0, CStringReqStrrchr_Post_Result_NA },
  { 0, 0, 0, CStringReqStrrchr_Post_Result_Last },
  { 0, 0, 1, CStringReqStrrchr_Post_Result_Last },
  { 0, 0, 0, CStringReqStrrchr_Post_Result_Null }
};

static const uint8_t
CStringReqStrrchr_Map[] = {
  2, 0, 0, 3, 1, 1
};

/* clang-format on */

static size_t CStringReqStrrchr_Scope( void *arg, char *buf, size_t n )
{
  CStringReqStrrchr_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CStringReqStrrchr_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CStringReqStrrchr_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CStringReqStrrchr_Scope,
  .initial_context = &CStringReqStrrchr_Instance
};

static inline CStringReqStrrchr_Entry CStringReqStrrchr_PopEntry(
  CStringReqStrrchr_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CStringReqStrrchr_Entries[ CStringReqStrrchr_Map[ index ] ];
}

static void CStringReqStrrchr_SetPreConditionStates(
  CStringReqStrrchr_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Occurrences_NA ) {
    ctx->Map.pcs[ 1 ] = CStringReqStrrchr_Pre_Occurrences_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }
}

static void CStringReqStrrchr_TestVariant( CStringReqStrrchr_Context *ctx )
{
  CStringReqStrrchr_Pre_C_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CStringReqStrrchr_Pre_Occurrences_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CStringReqStrrchr_Action( ctx );
  CStringReqStrrchr_Post_Result_Check( ctx, ctx->Map.entry.Post_Result );
}

/**
 * @fn void T_case_body_CStringReqStrrchr( void )
 */
T_TEST_CASE_FIXTURE( CStringReqStrrchr, &CStringReqStrrchr_Fixture )
{
  CStringReqStrrchr_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CStringReqStrrchr_Pre_C_Zero;
    ctx->Map.pci[ 0 ] < CStringReqStrrchr_Pre_C_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CStringReqStrrchr_Pre_Occurrences_None;
      ctx->Map.pci[ 1 ] < CStringReqStrrchr_Pre_Occurrences_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      ctx->Map.entry = CStringReqStrrchr_PopEntry( ctx );

      if ( ctx->Map.entry.Skip ) {
        continue;
      }

      CStringReqStrrchr_SetPreConditionStates( ctx );
      CStringReqStrrchr_Prepare( ctx );
      CStringReqStrrchr_TestVariant( ctx );
    }
  }
}

/** @} */
