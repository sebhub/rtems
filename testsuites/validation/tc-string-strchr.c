/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CStringReqStrchr
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
 * @defgroup CStringReqStrchr spec:/c/string/req/strchr
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CStringReqStrchr_Pre_C_Zero,
  CStringReqStrchr_Pre_C_NonZero,
  CStringReqStrchr_Pre_C_NA
} CStringReqStrchr_Pre_C;

typedef enum {
  CStringReqStrchr_Pre_Alignment_Aligned,
  CStringReqStrchr_Pre_Alignment_Unaligned,
  CStringReqStrchr_Pre_Alignment_NA
} CStringReqStrchr_Pre_Alignment;

typedef enum {
  CStringReqStrchr_Pre_Match_Found,
  CStringReqStrchr_Pre_Match_NotFound,
  CStringReqStrchr_Pre_Match_NA
} CStringReqStrchr_Pre_Match;

typedef enum {
  CStringReqStrchr_Pre_Position_Prefix,
  CStringReqStrchr_Pre_Position_FirstBlock,
  CStringReqStrchr_Pre_Position_SecondBlock,
  CStringReqStrchr_Pre_Position_NA
} CStringReqStrchr_Pre_Position;

typedef enum {
  CStringReqStrchr_Post_Result_Found,
  CStringReqStrchr_Post_Result_Null,
  CStringReqStrchr_Post_Result_NA
} CStringReqStrchr_Post_Result;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_C_NA : 1;
  uint8_t Pre_Alignment_NA : 1;
  uint8_t Pre_Match_NA : 1;
  uint8_t Pre_Position_NA : 1;
  uint8_t Post_Result : 2;
} CStringReqStrchr_Entry;

/**
 * @brief Test context for spec:/c/string/req/strchr test case.
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
   * @brief This member contains the expected return value for a found
   *   character.
   */
  char *expected;

  /**
   * @brief This member specifies the offset of the string in the buffer.
   */
  size_t offset;

  /**
   * @brief This member specifies the position of the character which ends the
   *   search in the buffer.
   */
  size_t position;

  /**
   * @brief If this member is true, then the string contains the searched
   *   character.
   */
  bool found;

  /**
   * @brief This member provides the buffer of the string, aligned to the size
   *   of type size_t.
   */
  size_t buf[ 16 ];

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 4 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 4 ];

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
    CStringReqStrchr_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CStringReqStrchr_Context;

static CStringReqStrchr_Context CStringReqStrchr_Instance;

static const char *const CStringReqStrchr_PreDesc_C[] =
  { "Zero", "NonZero", "NA" };

static const char *const CStringReqStrchr_PreDesc_Alignment[] =
  { "Aligned", "Unaligned", "NA" };

static const char *const CStringReqStrchr_PreDesc_Match[] =
  { "Found", "NotFound", "NA" };

static const char *const CStringReqStrchr_PreDesc_Position[] =
  { "Prefix", "FirstBlock", "SecondBlock", "NA" };

static const char *const *const CStringReqStrchr_PreDesc[] = {
  CStringReqStrchr_PreDesc_C,
  CStringReqStrchr_PreDesc_Alignment,
  CStringReqStrchr_PreDesc_Match,
  CStringReqStrchr_PreDesc_Position,
  NULL
};

typedef CStringReqStrchr_Context Context;

static size_t AlignUp( size_t offset )
{
  return ( offset + sizeof( size_t ) - 1 ) & ~( sizeof( size_t ) - 1 );
}

static void Arrange( Context *ctx )
{
  char  *b;
  size_t i;

  b = (char *) ctx->buf;

  for ( i = 0; i < sizeof( ctx->buf ); ++i ) {
    b[ i ] = (char) ( 'A' + i % 26 );
  }

  ctx->s = b + ctx->offset;

  if ( (char) ctx->c == '\0' || !ctx->found ) {
    b[ ctx->position ] = '\0';
  } else {
    b[ ctx->position ] = (char) ctx->c;
    b[ ctx->position + 2 * sizeof( size_t ) ] = '\0';
  }

  ctx->expected = b + ctx->position;
}

static void CStringReqStrchr_Pre_C_Prepare(
  CStringReqStrchr_Context *ctx,
  CStringReqStrchr_Pre_C    state
)
{
  switch ( state ) {
    case CStringReqStrchr_Pre_C_Zero: {
      /*
       * While the value of the `c` parameter converted to char is the null
       * character.
       */
      ctx->c = 0x100;
      break;
    }

    case CStringReqStrchr_Pre_C_NonZero: {
      /*
       * While the value of the `c` parameter converted to char is not the null
       * character.
       */
      ctx->c = 0x185;
      break;
    }

    case CStringReqStrchr_Pre_C_NA:
      break;
  }
}

static void CStringReqStrchr_Pre_Alignment_Prepare(
  CStringReqStrchr_Context      *ctx,
  CStringReqStrchr_Pre_Alignment state
)
{
  switch ( state ) {
    case CStringReqStrchr_Pre_Alignment_Aligned: {
      /*
       * While the string referenced by the `s` parameter starts at an address
       * which is aligned to the size of type size_t.
       */
      ctx->offset = 0;
      break;
    }

    case CStringReqStrchr_Pre_Alignment_Unaligned: {
      /*
       * While the string referenced by the `s` parameter starts at an address
       * which is not aligned to the size of type size_t.
       */
      ctx->offset = 1;
      break;
    }

    case CStringReqStrchr_Pre_Alignment_NA:
      break;
  }
}

static void CStringReqStrchr_Pre_Match_Prepare(
  CStringReqStrchr_Context  *ctx,
  CStringReqStrchr_Pre_Match state
)
{
  switch ( state ) {
    case CStringReqStrchr_Pre_Match_Found: {
      /*
       * While the string referenced by the `s` parameter contains a character
       * which is equal to the value of the `c` parameter converted to char.
       */
      ctx->found = true;
      break;
    }

    case CStringReqStrchr_Pre_Match_NotFound: {
      /*
       * While the string referenced by the `s` parameter contains no character
       * which is equal to the value of the `c` parameter converted to char.
       */
      ctx->found = false;
      break;
    }

    case CStringReqStrchr_Pre_Match_NA:
      break;
  }
}

static void CStringReqStrchr_Pre_Position_Prepare(
  CStringReqStrchr_Context     *ctx,
  CStringReqStrchr_Pre_Position state
)
{
  switch ( state ) {
    case CStringReqStrchr_Pre_Position_Prefix: {
      /*
       * While the first character of the string which is equal to the value of
       * the `c` parameter converted to char, or else its terminating null
       * character, is located before the first address aligned to the size of
       * type size_t.
       */
      ctx->position = ctx->offset;
      break;
    }

    case CStringReqStrchr_Pre_Position_FirstBlock: {
      /*
       * While the first character of the string which is equal to the value of
       * the `c` parameter converted to char, or else its terminating null
       * character, is located in the first block of the size of type size_t
       * which starts at an aligned address.
       */
      ctx->position = AlignUp( ctx->offset );
      break;
    }

    case CStringReqStrchr_Pre_Position_SecondBlock: {
      /*
       * While the first character of the string which is equal to the value of
       * the `c` parameter converted to char, or else its terminating null
       * character, is located in the second block of the size of type size_t
       * which starts at an aligned address.
       */
      ctx->position = AlignUp( ctx->offset ) + sizeof( size_t ) + 1;
      break;
    }

    case CStringReqStrchr_Pre_Position_NA:
      break;
  }
}

static void CStringReqStrchr_Post_Result_Check(
  CStringReqStrchr_Context    *ctx,
  CStringReqStrchr_Post_Result state
)
{
  switch ( state ) {
    case CStringReqStrchr_Post_Result_Found: {
      /*
       * The return value of strchr() shall be the address of the first
       * character of the string referenced by `s` which is equal to the value
       * of `c` converted to char.
       */
      T_eq_ptr( ctx->retval, ctx->expected );
      T_not_null( ctx->retval );
      break;
    }

    case CStringReqStrchr_Post_Result_Null: {
      /*
       * The return value of strchr() shall be NULL.
       */
      T_null( ctx->retval );
      break;
    }

    case CStringReqStrchr_Post_Result_NA:
      break;
  }
}

static void CStringReqStrchr_Prepare( CStringReqStrchr_Context *ctx )
{
  ctx->found = false;
  ctx->offset = 0;
  ctx->position = 5;
  ctx->retval = NULL;
  ctx->expected = NULL;
}

static void CStringReqStrchr_Action( CStringReqStrchr_Context *ctx )
{
  Arrange( ctx );
  ctx->retval = strchr( ctx->s, ctx->c );
}

/* clang-format off */

static const CStringReqStrchr_Entry
CStringReqStrchr_Entries[] = {
  { 1, 0, 0, 0, 0, CStringReqStrchr_Post_Result_NA },
  { 0, 0, 0, 0, 0, CStringReqStrchr_Post_Result_Found },
  { 0, 0, 0, 0, 0, CStringReqStrchr_Post_Result_Null },
  { 0, 0, 1, 1, 1, CStringReqStrchr_Post_Result_Found }
};

static const uint8_t
CStringReqStrchr_Map[] = {
  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 2, 2, 1, 1, 1, 2, 2, 2
};

/* clang-format on */

static size_t CStringReqStrchr_Scope( void *arg, char *buf, size_t n )
{
  CStringReqStrchr_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CStringReqStrchr_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CStringReqStrchr_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CStringReqStrchr_Scope,
  .initial_context = &CStringReqStrchr_Instance
};

static inline CStringReqStrchr_Entry CStringReqStrchr_PopEntry(
  CStringReqStrchr_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CStringReqStrchr_Entries[ CStringReqStrchr_Map[ index ] ];
}

static void CStringReqStrchr_SetPreConditionStates(
  CStringReqStrchr_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Alignment_NA ) {
    ctx->Map.pcs[ 1 ] = CStringReqStrchr_Pre_Alignment_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_Match_NA ) {
    ctx->Map.pcs[ 2 ] = CStringReqStrchr_Pre_Match_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  if ( ctx->Map.entry.Pre_Position_NA ) {
    ctx->Map.pcs[ 3 ] = CStringReqStrchr_Pre_Position_NA;
  } else {
    ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  }
}

static void CStringReqStrchr_TestVariant( CStringReqStrchr_Context *ctx )
{
  CStringReqStrchr_Pre_C_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CStringReqStrchr_Pre_Alignment_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CStringReqStrchr_Pre_Match_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CStringReqStrchr_Pre_Position_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CStringReqStrchr_Action( ctx );
  CStringReqStrchr_Post_Result_Check( ctx, ctx->Map.entry.Post_Result );
}

/**
 * @fn void T_case_body_CStringReqStrchr( void )
 */
T_TEST_CASE_FIXTURE( CStringReqStrchr, &CStringReqStrchr_Fixture )
{
  CStringReqStrchr_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CStringReqStrchr_Pre_C_Zero;
    ctx->Map.pci[ 0 ] < CStringReqStrchr_Pre_C_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CStringReqStrchr_Pre_Alignment_Aligned;
      ctx->Map.pci[ 1 ] < CStringReqStrchr_Pre_Alignment_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CStringReqStrchr_Pre_Match_Found;
        ctx->Map.pci[ 2 ] < CStringReqStrchr_Pre_Match_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CStringReqStrchr_Pre_Position_Prefix;
          ctx->Map.pci[ 3 ] < CStringReqStrchr_Pre_Position_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = CStringReqStrchr_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          CStringReqStrchr_SetPreConditionStates( ctx );
          CStringReqStrchr_Prepare( ctx );
          CStringReqStrchr_TestVariant( ctx );
        }
      }
    }
  }
}

/** @} */
