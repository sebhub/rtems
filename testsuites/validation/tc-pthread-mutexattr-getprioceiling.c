/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexattrGetprioceiling
 */

/*
 * Copyright (C) 2026 OHB System AG
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

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqMutexattrGetprioceiling \
 *   spec:/c/pthread/req/mutexattr-getprioceiling
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexattrGetprioceiling_Pre_Attr_Obj,
  CPthreadReqMutexattrGetprioceiling_Pre_Attr_Null,
  CPthreadReqMutexattrGetprioceiling_Pre_Attr_NA
} CPthreadReqMutexattrGetprioceiling_Pre_Attr;

typedef enum {
  CPthreadReqMutexattrGetprioceiling_Pre_Initialized_Yes,
  CPthreadReqMutexattrGetprioceiling_Pre_Initialized_No,
  CPthreadReqMutexattrGetprioceiling_Pre_Initialized_NA
} CPthreadReqMutexattrGetprioceiling_Pre_Initialized;

typedef enum {
  CPthreadReqMutexattrGetprioceiling_Pre_Stored_Min,
  CPthreadReqMutexattrGetprioceiling_Pre_Stored_Valid,
  CPthreadReqMutexattrGetprioceiling_Pre_Stored_Max,
  CPthreadReqMutexattrGetprioceiling_Pre_Stored_NA
} CPthreadReqMutexattrGetprioceiling_Pre_Stored;

typedef enum {
  CPthreadReqMutexattrGetprioceiling_Pre_Out_Obj,
  CPthreadReqMutexattrGetprioceiling_Pre_Out_Null,
  CPthreadReqMutexattrGetprioceiling_Pre_Out_NA
} CPthreadReqMutexattrGetprioceiling_Pre_Out;

typedef enum {
  CPthreadReqMutexattrGetprioceiling_Post_Status_Ok,
  CPthreadReqMutexattrGetprioceiling_Post_Status_Inval,
  CPthreadReqMutexattrGetprioceiling_Post_Status_NA
} CPthreadReqMutexattrGetprioceiling_Post_Status;

typedef enum {
  CPthreadReqMutexattrGetprioceiling_Post_Result_Stored,
  CPthreadReqMutexattrGetprioceiling_Post_Result_Nop,
  CPthreadReqMutexattrGetprioceiling_Post_Result_NA
} CPthreadReqMutexattrGetprioceiling_Post_Result;

typedef enum {
  CPthreadReqMutexattrGetprioceiling_Post_Other_Nop,
  CPthreadReqMutexattrGetprioceiling_Post_Other_NA
} CPthreadReqMutexattrGetprioceiling_Post_Other;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Attr_NA : 1;
  uint16_t Pre_Initialized_NA : 1;
  uint16_t Pre_Stored_NA : 1;
  uint16_t Pre_Out_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Result : 2;
  uint16_t Post_Other : 1;
} CPthreadReqMutexattrGetprioceiling_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutexattr-getprioceiling test
 *   case.
 */
typedef struct {
  /**
   * @brief This member specifies the `attr` parameter value.
   */
  const pthread_mutexattr_t *attr;

  /**
   * @brief This member specifies the `prioceiling` parameter value.
   */
  int *out;

  /**
   * @brief This member provides the object referenced by the `prioceiling`
   *   parameter.
   */
  int out_obj;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutexattr_getprioceiling() call.
   */
  int status;

  /**
   * @brief This member contains the mutex attributes object.
   */
  pthread_mutexattr_t attr_obj;

  /**
   * @brief This member contains a copy of the mutex attributes object before
   *   the call.
   */
  pthread_mutexattr_t attr_before;

  /**
   * @brief This member contains the value of the mutex priority ceiling
   *   attribute which the preparation stores in the mutex attributes object.
   */
  int stored;

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
    CPthreadReqMutexattrGetprioceiling_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexattrGetprioceiling_Context;

static CPthreadReqMutexattrGetprioceiling_Context
  CPthreadReqMutexattrGetprioceiling_Instance;

static const char *const CPthreadReqMutexattrGetprioceiling_PreDesc_Attr[] =
  { "Obj", "Null", "NA" };

static const char *const
  CPthreadReqMutexattrGetprioceiling_PreDesc_Initialized[] =
    { "Yes", "No", "NA" };

static const char *const CPthreadReqMutexattrGetprioceiling_PreDesc_Stored[] =
  { "Min", "Valid", "Max", "NA" };

static const char *const CPthreadReqMutexattrGetprioceiling_PreDesc_Out[] =
  { "Obj", "Null", "NA" };

static const char *const *const CPthreadReqMutexattrGetprioceiling_PreDesc[] =
  { CPthreadReqMutexattrGetprioceiling_PreDesc_Attr,
    CPthreadReqMutexattrGetprioceiling_PreDesc_Initialized,
    CPthreadReqMutexattrGetprioceiling_PreDesc_Stored,
    CPthreadReqMutexattrGetprioceiling_PreDesc_Out,
    NULL };

static void CPthreadReqMutexattrGetprioceiling_Pre_Attr_Prepare(
  CPthreadReqMutexattrGetprioceiling_Context *ctx,
  CPthreadReqMutexattrGetprioceiling_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprioceiling_Pre_Attr_Obj: {
      /*
       * While the `attr` parameter references an object of type
       * pthread_mutexattr_t.
       */
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Pre_Attr_Null: {
      /*
       * While the `attr` parameter is equal to NULL.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprioceiling_Pre_Initialized_Prepare(
  CPthreadReqMutexattrGetprioceiling_Context        *ctx,
  CPthreadReqMutexattrGetprioceiling_Pre_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprioceiling_Pre_Initialized_Yes: {
      /*
       * While the object referenced by the `attr` parameter is an initialized
       * mutex attributes object.
       */
      T_eq_int( pthread_mutexattr_init( &ctx->attr_obj ), 0 );
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Pre_Initialized_No: {
      /*
       * While the object referenced by the `attr` parameter is not an
       * initialized mutex attributes object.
       */
      memset( &ctx->attr_obj, 0xff, sizeof( ctx->attr_obj ) );
      ctx->attr_obj.is_initialized = 0;
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Pre_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprioceiling_Pre_Stored_Prepare(
  CPthreadReqMutexattrGetprioceiling_Context   *ctx,
  CPthreadReqMutexattrGetprioceiling_Pre_Stored state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprioceiling_Pre_Stored_Min: {
      /*
       * While the mutex priority ceiling attribute of the object referenced by
       * the `attr` parameter is the minimum of the type int.
       */
      ctx->stored = INT_MIN;
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Pre_Stored_Valid: {
      /*
       * While the mutex priority ceiling attribute of the object referenced by
       * the `attr` parameter is a valid thread priority.
       */
      ctx->stored = ToPOSIXPriority( PRIO_HIGH );
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Pre_Stored_Max: {
      /*
       * While the mutex priority ceiling attribute of the object referenced by
       * the `attr` parameter is the maximum of the type int.
       */
      ctx->stored = INT_MAX;
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Pre_Stored_NA:
      break;
  }

  ctx->attr_obj.prio_ceiling = ctx->stored;
}

static void CPthreadReqMutexattrGetprioceiling_Pre_Out_Prepare(
  CPthreadReqMutexattrGetprioceiling_Context *ctx,
  CPthreadReqMutexattrGetprioceiling_Pre_Out  state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprioceiling_Pre_Out_Obj: {
      /*
       * While the `prioceiling` parameter references an object of type int.
       */
      ctx->out = &ctx->out_obj;
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Pre_Out_Null: {
      /*
       * While the `prioceiling` parameter is equal to NULL.
       */
      ctx->out = NULL;
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Pre_Out_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprioceiling_Post_Status_Check(
  CPthreadReqMutexattrGetprioceiling_Context    *ctx,
  CPthreadReqMutexattrGetprioceiling_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprioceiling_Post_Status_Ok: {
      /*
       * The return value of pthread_mutexattr_getprioceiling() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Post_Status_Inval: {
      /*
       * The return value of pthread_mutexattr_getprioceiling() shall be
       * EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprioceiling_Post_Result_Check(
  CPthreadReqMutexattrGetprioceiling_Context    *ctx,
  CPthreadReqMutexattrGetprioceiling_Post_Result state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprioceiling_Post_Result_Stored: {
      /*
       * The value of the object referenced by the `prioceiling` parameter
       * shall be the mutex priority ceiling attribute of the mutex attributes
       * object.
       */
      T_eq_int( ctx->out_obj, ctx->stored );
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Post_Result_Nop: {
      /*
       * The value of the object referenced by the `prioceiling` parameter
       * shall not be modified.
       */
      T_eq_int( ctx->out_obj, -1 );
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Post_Result_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprioceiling_Post_Other_Check(
  CPthreadReqMutexattrGetprioceiling_Context   *ctx,
  CPthreadReqMutexattrGetprioceiling_Post_Other state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprioceiling_Post_Other_Nop: {
      /*
       * The mutex attributes object referenced by the `attr` parameter shall
       * not be modified.
       */
      T_eq_mem( &ctx->attr_obj, &ctx->attr_before, sizeof( ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrGetprioceiling_Post_Other_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprioceiling_Prepare(
  CPthreadReqMutexattrGetprioceiling_Context *ctx
)
{
  ctx->out_obj = -1;
}

static void CPthreadReqMutexattrGetprioceiling_Action(
  CPthreadReqMutexattrGetprioceiling_Context *ctx
)
{
  memcpy( &ctx->attr_before, &ctx->attr_obj, sizeof( ctx->attr_before ) );
  ctx->status = pthread_mutexattr_getprioceiling( ctx->attr, ctx->out );
}

/* clang-format off */

static const CPthreadReqMutexattrGetprioceiling_Entry
CPthreadReqMutexattrGetprioceiling_Entries[] = {
  { 1, 0, 0, 0, 0, CPthreadReqMutexattrGetprioceiling_Post_Status_NA,
    CPthreadReqMutexattrGetprioceiling_Post_Result_NA,
    CPthreadReqMutexattrGetprioceiling_Post_Other_NA },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetprioceiling_Post_Status_Inval,
    CPthreadReqMutexattrGetprioceiling_Post_Result_NA,
    CPthreadReqMutexattrGetprioceiling_Post_Other_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetprioceiling_Post_Status_Ok,
    CPthreadReqMutexattrGetprioceiling_Post_Result_Stored,
    CPthreadReqMutexattrGetprioceiling_Post_Other_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetprioceiling_Post_Status_Inval,
    CPthreadReqMutexattrGetprioceiling_Post_Result_Nop,
    CPthreadReqMutexattrGetprioceiling_Post_Other_Nop },
  { 0, 0, 1, 1, 0, CPthreadReqMutexattrGetprioceiling_Post_Status_Inval,
    CPthreadReqMutexattrGetprioceiling_Post_Result_Nop,
    CPthreadReqMutexattrGetprioceiling_Post_Other_NA },
  { 0, 0, 1, 1, 0, CPthreadReqMutexattrGetprioceiling_Post_Status_Inval,
    CPthreadReqMutexattrGetprioceiling_Post_Result_NA,
    CPthreadReqMutexattrGetprioceiling_Post_Other_NA }
};

static const uint8_t
CPthreadReqMutexattrGetprioceiling_Map[] = {
  2, 1, 2, 1, 2, 1, 3, 1, 3, 1, 3, 1, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t CPthreadReqMutexattrGetprioceiling_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexattrGetprioceiling_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexattrGetprioceiling_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexattrGetprioceiling_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexattrGetprioceiling_Scope,
  .initial_context = &CPthreadReqMutexattrGetprioceiling_Instance
};

static inline CPthreadReqMutexattrGetprioceiling_Entry
CPthreadReqMutexattrGetprioceiling_PopEntry(
  CPthreadReqMutexattrGetprioceiling_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexattrGetprioceiling_Entries
    [ CPthreadReqMutexattrGetprioceiling_Map[ index ] ];
}

static void CPthreadReqMutexattrGetprioceiling_SetPreConditionStates(
  CPthreadReqMutexattrGetprioceiling_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Initialized_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexattrGetprioceiling_Pre_Initialized_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_Stored_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqMutexattrGetprioceiling_Pre_Stored_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
}

static void CPthreadReqMutexattrGetprioceiling_TestVariant(
  CPthreadReqMutexattrGetprioceiling_Context *ctx
)
{
  CPthreadReqMutexattrGetprioceiling_Pre_Attr_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  CPthreadReqMutexattrGetprioceiling_Pre_Initialized_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexattrGetprioceiling_Pre_Stored_Prepare(
    ctx,
    ctx->Map.pcs[ 2 ]
  );
  CPthreadReqMutexattrGetprioceiling_Pre_Out_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqMutexattrGetprioceiling_Action( ctx );
  CPthreadReqMutexattrGetprioceiling_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexattrGetprioceiling_Post_Result_Check(
    ctx,
    ctx->Map.entry.Post_Result
  );
  CPthreadReqMutexattrGetprioceiling_Post_Other_Check(
    ctx,
    ctx->Map.entry.Post_Other
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexattrGetprioceiling( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexattrGetprioceiling,
  &CPthreadReqMutexattrGetprioceiling_Fixture
)
{
  CPthreadReqMutexattrGetprioceiling_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexattrGetprioceiling_Pre_Attr_Obj;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexattrGetprioceiling_Pre_Attr_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] =
        CPthreadReqMutexattrGetprioceiling_Pre_Initialized_Yes;
      ctx->Map.pci[ 1 ] <
      CPthreadReqMutexattrGetprioceiling_Pre_Initialized_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexattrGetprioceiling_Pre_Stored_Min;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexattrGetprioceiling_Pre_Stored_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexattrGetprioceiling_Pre_Out_Obj;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexattrGetprioceiling_Pre_Out_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = CPthreadReqMutexattrGetprioceiling_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          CPthreadReqMutexattrGetprioceiling_SetPreConditionStates( ctx );
          CPthreadReqMutexattrGetprioceiling_Prepare( ctx );
          CPthreadReqMutexattrGetprioceiling_TestVariant( ctx );
        }
      }
    }
  }
}

/** @} */
