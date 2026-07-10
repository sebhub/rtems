/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CStringReqStrcmp
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
 * @defgroup CStringReqStrcmp spec:/c/string/req/strcmp
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CStringReqStrcmp_Pre_Compare_Equal,
  CStringReqStrcmp_Pre_Compare_Less,
  CStringReqStrcmp_Pre_Compare_Greater,
  CStringReqStrcmp_Pre_Compare_NA
} CStringReqStrcmp_Pre_Compare;

typedef enum {
  CStringReqStrcmp_Pre_Difference_Character,
  CStringReqStrcmp_Pre_Difference_Terminator,
  CStringReqStrcmp_Pre_Difference_NA
} CStringReqStrcmp_Pre_Difference;

typedef enum {
  CStringReqStrcmp_Pre_Position_First,
  CStringReqStrcmp_Pre_Position_Later,
  CStringReqStrcmp_Pre_Position_NA
} CStringReqStrcmp_Pre_Position;

typedef enum {
  CStringReqStrcmp_Post_Result_Zero,
  CStringReqStrcmp_Post_Result_Negative,
  CStringReqStrcmp_Post_Result_Positive,
  CStringReqStrcmp_Post_Result_NA
} CStringReqStrcmp_Post_Result;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Compare_NA : 1;
  uint8_t Pre_Difference_NA : 1;
  uint8_t Pre_Position_NA : 1;
  uint8_t Post_Result : 2;
} CStringReqStrcmp_Entry;

/**
 * @brief Test context for spec:/c/string/req/strcmp test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `s1` parameter value.
   */
  const char *s1;

  /**
   * @brief This member specifies the `s2` parameter value.
   */
  const char *s2;

  /**
   * @brief This member contains the return value.
   */
  int retval;

  /**
   * @brief This member specifies the sign of the comparison.
   */
  int compare;

  /**
   * @brief This member is true, if the end of one string is the first
   *   difference.
   */
  bool terminator;

  /**
   * @brief This member specifies the position of the character which ends the
   *   comparison.
   */
  size_t position;

  /**
   * @brief This member provides the buffer of the first string.
   */
  char buf_1[ 16 ];

  /**
   * @brief This member provides the buffer of the second string.
   */
  char buf_2[ 16 ];

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 3 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 3 ];

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
    CStringReqStrcmp_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CStringReqStrcmp_Context;

static CStringReqStrcmp_Context CStringReqStrcmp_Instance;

static const char *const CStringReqStrcmp_PreDesc_Compare[] =
  { "Equal", "Less", "Greater", "NA" };

static const char *const CStringReqStrcmp_PreDesc_Difference[] =
  { "Character", "Terminator", "NA" };

static const char *const CStringReqStrcmp_PreDesc_Position[] =
  { "First", "Later", "NA" };

static const char *const *const CStringReqStrcmp_PreDesc[] = {
  CStringReqStrcmp_PreDesc_Compare,
  CStringReqStrcmp_PreDesc_Difference,
  CStringReqStrcmp_PreDesc_Position,
  NULL
};

typedef CStringReqStrcmp_Context Context;

static void Arrange( Context *ctx )
{
  size_t i;

  for ( i = 0; i < sizeof( ctx->buf_1 ) - 1; ++i ) {
    ctx->buf_1[ i ] = (char) ( 'A' + i );
    ctx->buf_2[ i ] = (char) ( 'A' + i );
  }

  ctx->buf_1[ sizeof( ctx->buf_1 ) - 1 ] = '\0';
  ctx->buf_2[ sizeof( ctx->buf_2 ) - 1 ] = '\0';

  if ( ctx->compare == 0 ) {
    ctx->buf_1[ ctx->position ] = '\0';
    ctx->buf_2[ ctx->position ] = '\0';
  } else if ( ctx->terminator ) {
    if ( ctx->compare < 0 ) {
      ctx->buf_1[ ctx->position ] = '\0';
    } else {
      ctx->buf_2[ ctx->position ] = '\0';
    }
  } else {
    ctx->buf_1[ ctx->position ] = (char) ( ctx->compare < 0 ? 0x01 : 0x81 );
    ctx->buf_2[ ctx->position ] = (char) ( ctx->compare < 0 ? 0x81 : 0x01 );
  }

  ctx->s1 = ctx->buf_1;
  ctx->s2 = ctx->buf_2;
}

static void CStringReqStrcmp_Pre_Compare_Prepare(
  CStringReqStrcmp_Context    *ctx,
  CStringReqStrcmp_Pre_Compare state
)
{
  switch ( state ) {
    case CStringReqStrcmp_Pre_Compare_Equal: {
      /*
       * While the string referenced by the `s1` parameter is equal to the
       * string referenced by the `s2` parameter.
       */
      ctx->compare = 0;
      break;
    }

    case CStringReqStrcmp_Pre_Compare_Less: {
      /*
       * While the first character which differs in the strings referenced by
       * the `s1` and `s2` parameters, interpreted as unsigned char, is less in
       * the string referenced by the `s1` parameter.
       */
      ctx->compare = -1;
      break;
    }

    case CStringReqStrcmp_Pre_Compare_Greater: {
      /*
       * While the first character which differs in the strings referenced by
       * the `s1` and `s2` parameters, interpreted as unsigned char, is greater
       * in the string referenced by the `s1` parameter.
       */
      ctx->compare = 1;
      break;
    }

    case CStringReqStrcmp_Pre_Compare_NA:
      break;
  }
}

static void CStringReqStrcmp_Pre_Difference_Prepare(
  CStringReqStrcmp_Context       *ctx,
  CStringReqStrcmp_Pre_Difference state
)
{
  switch ( state ) {
    case CStringReqStrcmp_Pre_Difference_Character: {
      /*
       * While the first character which differs in the strings is no
       * terminating null character.
       */
      ctx->terminator = false;
      break;
    }

    case CStringReqStrcmp_Pre_Difference_Terminator: {
      /*
       * While the first character which differs in the strings is the
       * terminating null character of one of the strings.
       */
      ctx->terminator = true;
      break;
    }

    case CStringReqStrcmp_Pre_Difference_NA:
      break;
  }
}

static void CStringReqStrcmp_Pre_Position_Prepare(
  CStringReqStrcmp_Context     *ctx,
  CStringReqStrcmp_Pre_Position state
)
{
  switch ( state ) {
    case CStringReqStrcmp_Pre_Position_First: {
      /*
       * While the comparison ends at the first character of the strings.
       */
      ctx->position = 0;
      break;
    }

    case CStringReqStrcmp_Pre_Position_Later: {
      /*
       * While the comparison ends after the first character of the strings.
       */
      ctx->position = 3;
      break;
    }

    case CStringReqStrcmp_Pre_Position_NA:
      break;
  }
}

static void CStringReqStrcmp_Post_Result_Check(
  CStringReqStrcmp_Context    *ctx,
  CStringReqStrcmp_Post_Result state
)
{
  switch ( state ) {
    case CStringReqStrcmp_Post_Result_Zero: {
      /*
       * The return value of strcmp() shall be equal to zero.
       */
      T_eq_int( ctx->retval, 0 );
      break;
    }

    case CStringReqStrcmp_Post_Result_Negative: {
      /*
       * The return value of strcmp() shall be less than zero.
       */
      T_lt_int( ctx->retval, 0 );
      break;
    }

    case CStringReqStrcmp_Post_Result_Positive: {
      /*
       * The return value of strcmp() shall be greater than zero.
       */
      T_gt_int( ctx->retval, 0 );
      break;
    }

    case CStringReqStrcmp_Post_Result_NA:
      break;
  }
}

static void CStringReqStrcmp_Prepare( CStringReqStrcmp_Context *ctx )
{
  ctx->retval = 0;
  ctx->terminator = false;
  ctx->position = 3;
}

static void CStringReqStrcmp_Action( CStringReqStrcmp_Context *ctx )
{
  Arrange( ctx );
  ctx->retval = strcmp( ctx->s1, ctx->s2 );
}

/* clang-format off */

static const CStringReqStrcmp_Entry
CStringReqStrcmp_Entries[] = {
  { 0, 0, 0, 0, CStringReqStrcmp_Post_Result_Negative },
  { 0, 0, 0, 0, CStringReqStrcmp_Post_Result_Positive },
  { 0, 0, 1, 0, CStringReqStrcmp_Post_Result_Zero },
  { 1, 0, 0, 0, CStringReqStrcmp_Post_Result_NA }
};

static const uint8_t
CStringReqStrcmp_Map[] = {
  2, 2, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1
};

/* clang-format on */

static size_t CStringReqStrcmp_Scope( void *arg, char *buf, size_t n )
{
  CStringReqStrcmp_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CStringReqStrcmp_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CStringReqStrcmp_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CStringReqStrcmp_Scope,
  .initial_context = &CStringReqStrcmp_Instance
};

static inline CStringReqStrcmp_Entry CStringReqStrcmp_PopEntry(
  CStringReqStrcmp_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CStringReqStrcmp_Entries[ CStringReqStrcmp_Map[ index ] ];
}

static void CStringReqStrcmp_SetPreConditionStates(
  CStringReqStrcmp_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Difference_NA ) {
    ctx->Map.pcs[ 1 ] = CStringReqStrcmp_Pre_Difference_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
}

static void CStringReqStrcmp_TestVariant( CStringReqStrcmp_Context *ctx )
{
  CStringReqStrcmp_Pre_Compare_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CStringReqStrcmp_Pre_Difference_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CStringReqStrcmp_Pre_Position_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CStringReqStrcmp_Action( ctx );
  CStringReqStrcmp_Post_Result_Check( ctx, ctx->Map.entry.Post_Result );
}

/**
 * @fn void T_case_body_CStringReqStrcmp( void )
 */
T_TEST_CASE_FIXTURE( CStringReqStrcmp, &CStringReqStrcmp_Fixture )
{
  CStringReqStrcmp_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CStringReqStrcmp_Pre_Compare_Equal;
    ctx->Map.pci[ 0 ] < CStringReqStrcmp_Pre_Compare_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CStringReqStrcmp_Pre_Difference_Character;
      ctx->Map.pci[ 1 ] < CStringReqStrcmp_Pre_Difference_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CStringReqStrcmp_Pre_Position_First;
        ctx->Map.pci[ 2 ] < CStringReqStrcmp_Pre_Position_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        ctx->Map.entry = CStringReqStrcmp_PopEntry( ctx );

        if ( ctx->Map.entry.Skip ) {
          continue;
        }

        CStringReqStrcmp_SetPreConditionStates( ctx );
        CStringReqStrcmp_Prepare( ctx );
        CStringReqStrcmp_TestVariant( ctx );
      }
    }
  }
}

/** @} */
