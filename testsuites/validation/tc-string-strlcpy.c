/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CStringReqStrlcpy
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
 * @defgroup CStringReqStrlcpy spec:/c/string/req/strlcpy
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CStringReqStrlcpy_Pre_N_Zero,
  CStringReqStrlcpy_Pre_N_Positive,
  CStringReqStrlcpy_Pre_N_NA
} CStringReqStrlcpy_Pre_N;

typedef enum {
  CStringReqStrlcpy_Pre_Fit_Fits,
  CStringReqStrlcpy_Pre_Fit_Truncate,
  CStringReqStrlcpy_Pre_Fit_NA
} CStringReqStrlcpy_Pre_Fit;

typedef enum {
  CStringReqStrlcpy_Pre_Alignment_Aligned,
  CStringReqStrlcpy_Pre_Alignment_Unaligned,
  CStringReqStrlcpy_Pre_Alignment_Mismatch,
  CStringReqStrlcpy_Pre_Alignment_NA
} CStringReqStrlcpy_Pre_Alignment;

typedef enum {
  CStringReqStrlcpy_Pre_Length_Short,
  CStringReqStrlcpy_Pre_Length_Medium,
  CStringReqStrlcpy_Pre_Length_Long,
  CStringReqStrlcpy_Pre_Length_NA
} CStringReqStrlcpy_Pre_Length;

typedef enum {
  CStringReqStrlcpy_Post_Result_Truncated,
  CStringReqStrlcpy_Post_Result_NotTruncated,
  CStringReqStrlcpy_Post_Result_NA
} CStringReqStrlcpy_Post_Result;

typedef enum {
  CStringReqStrlcpy_Post_DstArea_Unmodified,
  CStringReqStrlcpy_Post_DstArea_CopiedFull,
  CStringReqStrlcpy_Post_DstArea_CopiedTruncated,
  CStringReqStrlcpy_Post_DstArea_NA
} CStringReqStrlcpy_Post_DstArea;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_N_NA : 1;
  uint16_t Pre_Fit_NA : 1;
  uint16_t Pre_Alignment_NA : 1;
  uint16_t Pre_Length_NA : 1;
  uint16_t Post_Result : 2;
  uint16_t Post_DstArea : 2;
} CStringReqStrlcpy_Entry;

/**
 * @brief Test context for spec:/c/string/req/strlcpy test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `dst` parameter value.
   */
  char *dst;

  /**
   * @brief This member specifies the `src` parameter value.
   */
  const char *src;

  /**
   * @brief This member specifies the `size` parameter value.
   */
  size_t size;

  /**
   * @brief This member contains the return value.
   */
  size_t retval;

  /**
   * @brief This member specifies the offset of the `dst` parameter value from
   *   the start of the destination buffer.
   */
  size_t offset;

  /**
   * @brief This member provides the destination buffer.
   */
  RTEMS_ALIGNED( 8 ) unsigned char dst_buf[ 8 * sizeof( size_t ) ];

  /**
   * @brief This member provides the source buffer.
   */
  RTEMS_ALIGNED( 8 ) char src_buf[ 8 * sizeof( size_t ) ];

  /**
   * @brief This member specifies the offset of the `src` parameter value from
   *   the start of the source buffer.
   */
  size_t src_offset;

  /**
   * @brief This member is true, if the string fits into the size.
   */
  bool fits;

  /**
   * @brief This member selects the count of characters to copy.
   */
  unsigned int length;

  /**
   * @brief This member is true, if the size is zero.
   */
  bool zero;

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
    CStringReqStrlcpy_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CStringReqStrlcpy_Context;

static CStringReqStrlcpy_Context CStringReqStrlcpy_Instance;

static const char *const CStringReqStrlcpy_PreDesc_N[] =
  { "Zero", "Positive", "NA" };

static const char *const CStringReqStrlcpy_PreDesc_Fit[] =
  { "Fits", "Truncate", "NA" };

static const char *const CStringReqStrlcpy_PreDesc_Alignment[] =
  { "Aligned", "Unaligned", "Mismatch", "NA" };

static const char *const CStringReqStrlcpy_PreDesc_Length[] =
  { "Short", "Medium", "Long", "NA" };

static const char *const *const CStringReqStrlcpy_PreDesc[] = {
  CStringReqStrlcpy_PreDesc_N,
  CStringReqStrlcpy_PreDesc_Fit,
  CStringReqStrlcpy_PreDesc_Alignment,
  CStringReqStrlcpy_PreDesc_Length,
  NULL
};

typedef CStringReqStrlcpy_Context Context;

static void Arrange( Context *ctx )
{
  size_t len;
  size_t i;
  char  *src;

  if ( ctx->zero ) {
    len = 3;
    ctx->size = 0;
  } else if ( ctx->fits ) {
    static const size_t lengths[] =
      { 1, sizeof( size_t ) - 1, 2 * sizeof( size_t ) + sizeof( size_t ) / 2 };

    len = lengths[ ctx->length ];
    ctx->size = 4 * sizeof( size_t );
  } else {
    len = 4 * sizeof( size_t );
    static const size_t sizes[] =
      { 2, sizeof( size_t ), 2 * sizeof( size_t ) + 1 };

    ctx->size = sizes[ ctx->length ];
  }

  src = ctx->src_buf + ctx->src_offset;

  for ( i = 0; i < len; ++i ) {
    src[ i ] = (char) ( 'A' + i % 26 );
  }

  src[ len ] = '\0';
  ctx->src = src;
  ctx->dst = (char *) ( ctx->dst_buf + ctx->offset );
}

static void CStringReqStrlcpy_Pre_N_Prepare(
  CStringReqStrlcpy_Context *ctx,
  CStringReqStrlcpy_Pre_N    state
)
{
  switch ( state ) {
    case CStringReqStrlcpy_Pre_N_Zero: {
      /*
       * While the size specified by `size` is equal to zero.
       */
      ctx->zero = true;
      break;
    }

    case CStringReqStrlcpy_Pre_N_Positive: {
      /*
       * While the size specified by `size` is greater than zero.
       */
      /* The pre-conditions Fit and Length set the size. */
      break;
    }

    case CStringReqStrlcpy_Pre_N_NA:
      break;
  }
}

static void CStringReqStrlcpy_Pre_Fit_Prepare(
  CStringReqStrlcpy_Context *ctx,
  CStringReqStrlcpy_Pre_Fit  state
)
{
  switch ( state ) {
    case CStringReqStrlcpy_Pre_Fit_Fits: {
      /*
       * While the length of the string referenced by `src` is less than the
       * size specified by `size`.
       */
      ctx->fits = true;
      break;
    }

    case CStringReqStrlcpy_Pre_Fit_Truncate: {
      /*
       * While the length of the string referenced by `src` is greater than or
       * equal to the size specified by `size`.
       */
      ctx->fits = false;
      break;
    }

    case CStringReqStrlcpy_Pre_Fit_NA:
      break;
  }
}

static void CStringReqStrlcpy_Pre_Alignment_Prepare(
  CStringReqStrlcpy_Context      *ctx,
  CStringReqStrlcpy_Pre_Alignment state
)
{
  switch ( state ) {
    case CStringReqStrlcpy_Pre_Alignment_Aligned: {
      /*
       * While the memory areas referenced by the `dst` and `src` parameters
       * start at addresses which are aligned to the size of type size_t.
       */
      ctx->src_offset = 0;
      ctx->offset = 0;
      break;
    }

    case CStringReqStrlcpy_Pre_Alignment_Unaligned: {
      /*
       * While the memory areas referenced by the `dst` and `src` parameters
       * start at addresses which are not aligned to the size of type size_t
       * and which have the same remainder of the division by the size of type
       * size_t.
       */
      ctx->src_offset = 1;
      ctx->offset = 1;
      break;
    }

    case CStringReqStrlcpy_Pre_Alignment_Mismatch: {
      /*
       * While the addresses of the memory areas referenced by the `dst` and
       * `src` parameters have different remainders of the division by the size
       * of type size_t.
       */
      ctx->src_offset = 1;
      ctx->offset = 2;
      break;
    }

    case CStringReqStrlcpy_Pre_Alignment_NA:
      break;
  }
}

static void CStringReqStrlcpy_Pre_Length_Prepare(
  CStringReqStrlcpy_Context   *ctx,
  CStringReqStrlcpy_Pre_Length state
)
{
  switch ( state ) {
    case CStringReqStrlcpy_Pre_Length_Short: {
      /*
       * While the count of characters to copy is less than the size of type
       * size_t minus one.
       */
      ctx->length = 0;
      break;
    }

    case CStringReqStrlcpy_Pre_Length_Medium: {
      /*
       * While the count of characters to copy is equal to the size of type
       * size_t minus one.
       */
      ctx->length = 1;
      break;
    }

    case CStringReqStrlcpy_Pre_Length_Long: {
      /*
       * While the count of characters to copy is greater than or equal to two
       * times the size of type size_t.
       */
      ctx->length = 2;
      break;
    }

    case CStringReqStrlcpy_Pre_Length_NA:
      break;
  }
}

static void CStringReqStrlcpy_Post_Result_Check(
  CStringReqStrlcpy_Context    *ctx,
  CStringReqStrlcpy_Post_Result state
)
{
  switch ( state ) {
    case CStringReqStrlcpy_Post_Result_Truncated: {
      /*
       * The return value of strlcpy() shall be greater than or equal to the
       * size specified by `size`.
       */
      T_ge_uint( ctx->retval, ctx->size );
      T_eq_uint( ctx->retval, strlen( ctx->src ) );
      break;
    }

    case CStringReqStrlcpy_Post_Result_NotTruncated: {
      /*
       * The return value of strlcpy() shall be less than the size specified by
       * `size`.
       */
      T_lt_uint( ctx->retval, ctx->size );
      T_eq_uint( ctx->retval, strlen( ctx->src ) );
      break;
    }

    case CStringReqStrlcpy_Post_Result_NA:
      break;
  }
}

static void CStringReqStrlcpy_Post_DstArea_Check(
  CStringReqStrlcpy_Context     *ctx,
  CStringReqStrlcpy_Post_DstArea state
)
{
  switch ( state ) {
    case CStringReqStrlcpy_Post_DstArea_Unmodified: {
      /*
       * The memory area referenced by `dst` shall not be modified.
       */
      for ( size_t i = 0; i < sizeof( ctx->dst_buf ); ++i ) {
        T_quiet_eq_uint( ctx->dst_buf[ i ], 0xaa );
      }
      break;
    }

    case CStringReqStrlcpy_Post_DstArea_CopiedFull: {
      /*
       * Only the string referenced by `src`, including its terminating null
       * character, shall be copied to the memory area referenced by `dst`.
       */
      size_t len = strlen( ctx->src );

      T_eq_nstr( ctx->dst, ctx->src, len + 1 );

      for ( size_t i = 0; i < ctx->offset; ++i ) {
        T_quiet_eq_uint( ctx->dst_buf[ i ], 0xaa );
      }

      for (
        size_t i = ctx->offset + len + 1; i < sizeof( ctx->dst_buf ); ++i
      ) {
        T_quiet_eq_uint( ctx->dst_buf[ i ], 0xaa );
      }
      break;
    }

    case CStringReqStrlcpy_Post_DstArea_CopiedTruncated: {
      /*
       * Only the first characters of the string referenced by `src`, up to one
       * less than the size specified by `size`, followed by a terminating null
       * character shall be copied to the memory area referenced by `dst`.
       */
      T_eq_nstr( ctx->dst, ctx->src, ctx->size - 1 );
      T_eq_uint( ctx->dst[ ctx->size - 1 ], 0 );

      for ( size_t i = 0; i < ctx->offset; ++i ) {
        T_quiet_eq_uint( ctx->dst_buf[ i ], 0xaa );
      }

      for (
        size_t i = ctx->offset + ctx->size; i < sizeof( ctx->dst_buf ); ++i
      ) {
        T_quiet_eq_uint( ctx->dst_buf[ i ], 0xaa );
      }
      break;
    }

    case CStringReqStrlcpy_Post_DstArea_NA:
      break;
  }
}

static void CStringReqStrlcpy_Prepare( CStringReqStrlcpy_Context *ctx )
{
  memset( ctx->dst_buf, 0xaa, sizeof( ctx->dst_buf ) );
  memset( ctx->src_buf, 0, sizeof( ctx->src_buf ) );
  ctx->offset = 0;
  ctx->src_offset = 0;
  ctx->zero = false;
  ctx->fits = false;
  ctx->length = 0;
  ctx->dst = (char *) ctx->dst_buf;
  ctx->src = ctx->src_buf;
  ctx->retval = 0;
  ctx->size = 0;
}

static void CStringReqStrlcpy_Action( CStringReqStrlcpy_Context *ctx )
{
  Arrange( ctx );
  ctx->retval = strlcpy( ctx->dst, ctx->src, ctx->size );
}

/* clang-format off */

static const CStringReqStrlcpy_Entry
CStringReqStrlcpy_Entries[] = {
  { 1, 0, 0, 0, 0, CStringReqStrlcpy_Post_Result_NA,
    CStringReqStrlcpy_Post_DstArea_NA },
  { 0, 0, 0, 0, 0, CStringReqStrlcpy_Post_Result_NotTruncated,
    CStringReqStrlcpy_Post_DstArea_CopiedFull },
  { 0, 0, 0, 0, 0, CStringReqStrlcpy_Post_Result_Truncated,
    CStringReqStrlcpy_Post_DstArea_CopiedTruncated },
  { 0, 0, 1, 1, 1, CStringReqStrlcpy_Post_Result_Truncated,
    CStringReqStrlcpy_Post_DstArea_Unmodified }
};

static const uint8_t
CStringReqStrlcpy_Map[] = {
  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

/* clang-format on */

static size_t CStringReqStrlcpy_Scope( void *arg, char *buf, size_t n )
{
  CStringReqStrlcpy_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CStringReqStrlcpy_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CStringReqStrlcpy_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CStringReqStrlcpy_Scope,
  .initial_context = &CStringReqStrlcpy_Instance
};

static inline CStringReqStrlcpy_Entry CStringReqStrlcpy_PopEntry(
  CStringReqStrlcpy_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CStringReqStrlcpy_Entries[ CStringReqStrlcpy_Map[ index ] ];
}

static void CStringReqStrlcpy_SetPreConditionStates(
  CStringReqStrlcpy_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Fit_NA ) {
    ctx->Map.pcs[ 1 ] = CStringReqStrlcpy_Pre_Fit_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_Alignment_NA ) {
    ctx->Map.pcs[ 2 ] = CStringReqStrlcpy_Pre_Alignment_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  if ( ctx->Map.entry.Pre_Length_NA ) {
    ctx->Map.pcs[ 3 ] = CStringReqStrlcpy_Pre_Length_NA;
  } else {
    ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  }
}

static void CStringReqStrlcpy_TestVariant( CStringReqStrlcpy_Context *ctx )
{
  CStringReqStrlcpy_Pre_N_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CStringReqStrlcpy_Pre_Fit_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CStringReqStrlcpy_Pre_Alignment_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CStringReqStrlcpy_Pre_Length_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CStringReqStrlcpy_Action( ctx );
  CStringReqStrlcpy_Post_Result_Check( ctx, ctx->Map.entry.Post_Result );
  CStringReqStrlcpy_Post_DstArea_Check( ctx, ctx->Map.entry.Post_DstArea );
}

/**
 * @fn void T_case_body_CStringReqStrlcpy( void )
 */
T_TEST_CASE_FIXTURE( CStringReqStrlcpy, &CStringReqStrlcpy_Fixture )
{
  CStringReqStrlcpy_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CStringReqStrlcpy_Pre_N_Zero;
    ctx->Map.pci[ 0 ] < CStringReqStrlcpy_Pre_N_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CStringReqStrlcpy_Pre_Fit_Fits;
      ctx->Map.pci[ 1 ] < CStringReqStrlcpy_Pre_Fit_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CStringReqStrlcpy_Pre_Alignment_Aligned;
        ctx->Map.pci[ 2 ] < CStringReqStrlcpy_Pre_Alignment_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CStringReqStrlcpy_Pre_Length_Short;
          ctx->Map.pci[ 3 ] < CStringReqStrlcpy_Pre_Length_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = CStringReqStrlcpy_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          CStringReqStrlcpy_SetPreConditionStates( ctx );
          CStringReqStrlcpy_Prepare( ctx );
          CStringReqStrlcpy_TestVariant( ctx );
        }
      }
    }
  }
}

/** @} */
