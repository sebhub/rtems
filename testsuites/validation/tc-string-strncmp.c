/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CStringReqStrncmp
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
 * @defgroup CStringReqStrncmp spec:/c/string/req/strncmp
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CStringReqStrncmp_Pre_N_Zero,
  CStringReqStrncmp_Pre_N_Positive,
  CStringReqStrncmp_Pre_N_NA
} CStringReqStrncmp_Pre_N;

typedef enum {
  CStringReqStrncmp_Pre_Compare_Equal,
  CStringReqStrncmp_Pre_Compare_Less,
  CStringReqStrncmp_Pre_Compare_Greater,
  CStringReqStrncmp_Pre_Compare_NA
} CStringReqStrncmp_Pre_Compare;

typedef enum {
  CStringReqStrncmp_Pre_EqualCause_Exhausted,
  CStringReqStrncmp_Pre_EqualCause_Terminated,
  CStringReqStrncmp_Pre_EqualCause_NA
} CStringReqStrncmp_Pre_EqualCause;

typedef enum {
  CStringReqStrncmp_Pre_Difference_Character,
  CStringReqStrncmp_Pre_Difference_Terminator,
  CStringReqStrncmp_Pre_Difference_NA
} CStringReqStrncmp_Pre_Difference;

typedef enum {
  CStringReqStrncmp_Pre_Position_First,
  CStringReqStrncmp_Pre_Position_Later,
  CStringReqStrncmp_Pre_Position_NA
} CStringReqStrncmp_Pre_Position;

typedef enum {
  CStringReqStrncmp_Post_Result_Zero,
  CStringReqStrncmp_Post_Result_Negative,
  CStringReqStrncmp_Post_Result_Positive,
  CStringReqStrncmp_Post_Result_NA
} CStringReqStrncmp_Post_Result;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_N_NA : 1;
  uint8_t Pre_Compare_NA : 1;
  uint8_t Pre_EqualCause_NA : 1;
  uint8_t Pre_Difference_NA : 1;
  uint8_t Pre_Position_NA : 1;
  uint8_t Post_Result : 2;
} CStringReqStrncmp_Entry;

/**
 * @brief Test context for spec:/c/string/req/strncmp test case.
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
   * @brief This member specifies the `n` parameter value.
   */
  size_t n;

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
   * @brief This member is true, if the strings differ only after the count.
   */
  bool exhausted;

  /**
   * @brief This member specifies the position of the first difference.
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
    size_t pci[ 5 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 5 ];

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
    CStringReqStrncmp_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CStringReqStrncmp_Context;

static CStringReqStrncmp_Context CStringReqStrncmp_Instance;

static const char *const CStringReqStrncmp_PreDesc_N[] =
  { "Zero", "Positive", "NA" };

static const char *const CStringReqStrncmp_PreDesc_Compare[] =
  { "Equal", "Less", "Greater", "NA" };

static const char *const CStringReqStrncmp_PreDesc_EqualCause[] =
  { "Exhausted", "Terminated", "NA" };

static const char *const CStringReqStrncmp_PreDesc_Difference[] =
  { "Character", "Terminator", "NA" };

static const char *const CStringReqStrncmp_PreDesc_Position[] =
  { "First", "Later", "NA" };

static const char *const *const CStringReqStrncmp_PreDesc[] = {
  CStringReqStrncmp_PreDesc_N,
  CStringReqStrncmp_PreDesc_Compare,
  CStringReqStrncmp_PreDesc_EqualCause,
  CStringReqStrncmp_PreDesc_Difference,
  CStringReqStrncmp_PreDesc_Position,
  NULL
};

typedef CStringReqStrncmp_Context Context;

static void Arrange( Context *ctx )
{
  size_t i;
  size_t p;

  for ( i = 0; i < sizeof( ctx->buf_1 ) - 1; ++i ) {
    ctx->buf_1[ i ] = (char) ( 'A' + i );
    ctx->buf_2[ i ] = (char) ( 'A' + i );
  }

  ctx->buf_1[ sizeof( ctx->buf_1 ) - 1 ] = '\0';
  ctx->buf_2[ sizeof( ctx->buf_2 ) - 1 ] = '\0';
  p = ctx->position;

  if ( ctx->compare == 0 ) {
    if ( ctx->exhausted ) {
      ctx->n = p + 1;
      ctx->buf_1[ ctx->n ] = (char) 0x01;
      ctx->buf_2[ ctx->n ] = (char) 0x81;
    } else {
      ctx->buf_1[ p ] = '\0';
      ctx->buf_2[ p ] = '\0';
    }
  } else if ( ctx->terminator ) {
    if ( ctx->compare < 0 ) {
      ctx->buf_1[ p ] = '\0';
    } else {
      ctx->buf_2[ p ] = '\0';
    }
  } else {
    ctx->buf_1[ p ] = (char) ( ctx->compare < 0 ? 0x01 : 0x81 );
    ctx->buf_2[ p ] = (char) ( ctx->compare < 0 ? 0x81 : 0x01 );
  }

  ctx->s1 = ctx->buf_1;
  ctx->s2 = ctx->buf_2;
}

static void CStringReqStrncmp_Pre_N_Prepare(
  CStringReqStrncmp_Context *ctx,
  CStringReqStrncmp_Pre_N    state
)
{
  switch ( state ) {
    case CStringReqStrncmp_Pre_N_Zero: {
      /*
       * While the `n` parameter is equal to zero.
       */
      ctx->n = 0;
      ctx->compare = 1;
      ctx->position = 0;
      break;
    }

    case CStringReqStrncmp_Pre_N_Positive: {
      /*
       * While the `n` parameter is greater than zero.
       */
      ctx->n = 8;
      break;
    }

    case CStringReqStrncmp_Pre_N_NA:
      break;
  }
}

static void CStringReqStrncmp_Pre_Compare_Prepare(
  CStringReqStrncmp_Context    *ctx,
  CStringReqStrncmp_Pre_Compare state
)
{
  switch ( state ) {
    case CStringReqStrncmp_Pre_Compare_Equal: {
      /*
       * While the first `n` characters of the strings referenced by the `s1`
       * and `s2` parameters are equal, where no character which follows a null
       * character is compared.
       */
      ctx->compare = 0;
      break;
    }

    case CStringReqStrncmp_Pre_Compare_Less: {
      /*
       * While the first character which differs in the first `n` characters of
       * the strings referenced by the `s1` and `s2` parameters, interpreted as
       * unsigned char, is less in the string referenced by the `s1` parameter.
       */
      ctx->compare = -1;
      break;
    }

    case CStringReqStrncmp_Pre_Compare_Greater: {
      /*
       * While the first character which differs in the first `n` characters of
       * the strings referenced by the `s1` and `s2` parameters, interpreted as
       * unsigned char, is greater in the string referenced by the `s1`
       * parameter.
       */
      ctx->compare = 1;
      break;
    }

    case CStringReqStrncmp_Pre_Compare_NA:
      break;
  }
}

static void CStringReqStrncmp_Pre_EqualCause_Prepare(
  CStringReqStrncmp_Context       *ctx,
  CStringReqStrncmp_Pre_EqualCause state
)
{
  switch ( state ) {
    case CStringReqStrncmp_Pre_EqualCause_Exhausted: {
      /*
       * While the first character which differs in the strings referenced by
       * the `s1` and `s2` parameters follows the first `n` characters.
       */
      ctx->exhausted = true;
      break;
    }

    case CStringReqStrncmp_Pre_EqualCause_Terminated: {
      /*
       * While the strings referenced by the `s1` and `s2` parameters end with
       * a null character in the first `n` characters.
       */
      ctx->exhausted = false;
      break;
    }

    case CStringReqStrncmp_Pre_EqualCause_NA:
      break;
  }
}

static void CStringReqStrncmp_Pre_Difference_Prepare(
  CStringReqStrncmp_Context       *ctx,
  CStringReqStrncmp_Pre_Difference state
)
{
  switch ( state ) {
    case CStringReqStrncmp_Pre_Difference_Character: {
      /*
       * While the first character which differs in the strings is no
       * terminating null character.
       */
      ctx->terminator = false;
      break;
    }

    case CStringReqStrncmp_Pre_Difference_Terminator: {
      /*
       * While the first character which differs in the strings is the
       * terminating null character of one of the strings.
       */
      ctx->terminator = true;
      break;
    }

    case CStringReqStrncmp_Pre_Difference_NA:
      break;
  }
}

static void CStringReqStrncmp_Pre_Position_Prepare(
  CStringReqStrncmp_Context     *ctx,
  CStringReqStrncmp_Pre_Position state
)
{
  switch ( state ) {
    case CStringReqStrncmp_Pre_Position_First: {
      /*
       * While the comparison ends at the first character of the strings.
       */
      ctx->position = 0;
      break;
    }

    case CStringReqStrncmp_Pre_Position_Later: {
      /*
       * While the comparison ends after the first character of the strings.
       */
      ctx->position = 3;
      break;
    }

    case CStringReqStrncmp_Pre_Position_NA:
      break;
  }
}

static void CStringReqStrncmp_Post_Result_Check(
  CStringReqStrncmp_Context    *ctx,
  CStringReqStrncmp_Post_Result state
)
{
  switch ( state ) {
    case CStringReqStrncmp_Post_Result_Zero: {
      /*
       * The return value of strncmp() shall be equal to zero.
       */
      T_eq_int( ctx->retval, 0 );
      break;
    }

    case CStringReqStrncmp_Post_Result_Negative: {
      /*
       * The return value of strncmp() shall be less than zero.
       */
      T_lt_int( ctx->retval, 0 );
      break;
    }

    case CStringReqStrncmp_Post_Result_Positive: {
      /*
       * The return value of strncmp() shall be greater than zero.
       */
      T_gt_int( ctx->retval, 0 );
      break;
    }

    case CStringReqStrncmp_Post_Result_NA:
      break;
  }
}

static void CStringReqStrncmp_Prepare( CStringReqStrncmp_Context *ctx )
{
  ctx->retval = 0;
  ctx->compare = 0;
  ctx->terminator = false;
  ctx->exhausted = false;
  ctx->position = 3;
}

static void CStringReqStrncmp_Action( CStringReqStrncmp_Context *ctx )
{
  Arrange( ctx );
  ctx->retval = strncmp( ctx->s1, ctx->s2, ctx->n );
}

/* clang-format off */

static const CStringReqStrncmp_Entry
CStringReqStrncmp_Entries[] = {
  { 1, 0, 0, 0, 0, 0, CStringReqStrncmp_Post_Result_NA },
  { 0, 0, 0, 0, 1, 0, CStringReqStrncmp_Post_Result_Zero },
  { 0, 0, 0, 1, 0, 0, CStringReqStrncmp_Post_Result_Negative },
  { 0, 0, 0, 1, 0, 0, CStringReqStrncmp_Post_Result_Positive },
  { 0, 0, 1, 1, 1, 1, CStringReqStrncmp_Post_Result_Zero }
};

static const uint8_t
CStringReqStrncmp_Map[] = {
  4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
  0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0
};

/* clang-format on */

static size_t CStringReqStrncmp_Scope( void *arg, char *buf, size_t n )
{
  CStringReqStrncmp_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CStringReqStrncmp_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CStringReqStrncmp_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CStringReqStrncmp_Scope,
  .initial_context = &CStringReqStrncmp_Instance
};

static inline CStringReqStrncmp_Entry CStringReqStrncmp_PopEntry(
  CStringReqStrncmp_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CStringReqStrncmp_Entries[ CStringReqStrncmp_Map[ index ] ];
}

static void CStringReqStrncmp_SetPreConditionStates(
  CStringReqStrncmp_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Compare_NA ) {
    ctx->Map.pcs[ 1 ] = CStringReqStrncmp_Pre_Compare_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_EqualCause_NA ) {
    ctx->Map.pcs[ 2 ] = CStringReqStrncmp_Pre_EqualCause_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  if ( ctx->Map.entry.Pre_Difference_NA ) {
    ctx->Map.pcs[ 3 ] = CStringReqStrncmp_Pre_Difference_NA;
  } else {
    ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  }

  if ( ctx->Map.entry.Pre_Position_NA ) {
    ctx->Map.pcs[ 4 ] = CStringReqStrncmp_Pre_Position_NA;
  } else {
    ctx->Map.pcs[ 4 ] = ctx->Map.pci[ 4 ];
  }
}

static void CStringReqStrncmp_TestVariant( CStringReqStrncmp_Context *ctx )
{
  CStringReqStrncmp_Pre_N_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CStringReqStrncmp_Pre_Compare_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CStringReqStrncmp_Pre_EqualCause_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CStringReqStrncmp_Pre_Difference_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CStringReqStrncmp_Pre_Position_Prepare( ctx, ctx->Map.pcs[ 4 ] );
  CStringReqStrncmp_Action( ctx );
  CStringReqStrncmp_Post_Result_Check( ctx, ctx->Map.entry.Post_Result );
}

/**
 * @fn void T_case_body_CStringReqStrncmp( void )
 */
T_TEST_CASE_FIXTURE( CStringReqStrncmp, &CStringReqStrncmp_Fixture )
{
  CStringReqStrncmp_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CStringReqStrncmp_Pre_N_Zero;
    ctx->Map.pci[ 0 ] < CStringReqStrncmp_Pre_N_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CStringReqStrncmp_Pre_Compare_Equal;
      ctx->Map.pci[ 1 ] < CStringReqStrncmp_Pre_Compare_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CStringReqStrncmp_Pre_EqualCause_Exhausted;
        ctx->Map.pci[ 2 ] < CStringReqStrncmp_Pre_EqualCause_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CStringReqStrncmp_Pre_Difference_Character;
          ctx->Map.pci[ 3 ] < CStringReqStrncmp_Pre_Difference_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          for (
            ctx->Map.pci[ 4 ] = CStringReqStrncmp_Pre_Position_First;
            ctx->Map.pci[ 4 ] < CStringReqStrncmp_Pre_Position_NA;
            ++ctx->Map.pci[ 4 ]
          ) {
            ctx->Map.entry = CStringReqStrncmp_PopEntry( ctx );

            if ( ctx->Map.entry.Skip ) {
              continue;
            }

            CStringReqStrncmp_SetPreConditionStates( ctx );
            CStringReqStrncmp_Prepare( ctx );
            CStringReqStrncmp_TestVariant( ctx );
          }
        }
      }
    }
  }
}

/** @} */
