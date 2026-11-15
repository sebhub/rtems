/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexattrSetpshared
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

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqMutexattrSetpshared \
 *   spec:/c/pthread/req/mutexattr-setpshared
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexattrSetpshared_Pre_Attr_Obj,
  CPthreadReqMutexattrSetpshared_Pre_Attr_Null,
  CPthreadReqMutexattrSetpshared_Pre_Attr_NA
} CPthreadReqMutexattrSetpshared_Pre_Attr;

typedef enum {
  CPthreadReqMutexattrSetpshared_Pre_Initialized_Yes,
  CPthreadReqMutexattrSetpshared_Pre_Initialized_No,
  CPthreadReqMutexattrSetpshared_Pre_Initialized_NA
} CPthreadReqMutexattrSetpshared_Pre_Initialized;

typedef enum {
  CPthreadReqMutexattrSetpshared_Pre_Value_Private,
  CPthreadReqMutexattrSetpshared_Pre_Value_Shared,
  CPthreadReqMutexattrSetpshared_Pre_Value_Invalid,
  CPthreadReqMutexattrSetpshared_Pre_Value_NA
} CPthreadReqMutexattrSetpshared_Pre_Value;

typedef enum {
  CPthreadReqMutexattrSetpshared_Post_Status_Ok,
  CPthreadReqMutexattrSetpshared_Post_Status_Inval,
  CPthreadReqMutexattrSetpshared_Post_Status_NA
} CPthreadReqMutexattrSetpshared_Post_Status;

typedef enum {
  CPthreadReqMutexattrSetpshared_Post_Value_Private,
  CPthreadReqMutexattrSetpshared_Post_Value_Shared,
  CPthreadReqMutexattrSetpshared_Post_Value_Nop,
  CPthreadReqMutexattrSetpshared_Post_Value_NA
} CPthreadReqMutexattrSetpshared_Post_Value;

typedef enum {
  CPthreadReqMutexattrSetpshared_Post_Other_Nop,
  CPthreadReqMutexattrSetpshared_Post_Other_NA
} CPthreadReqMutexattrSetpshared_Post_Other;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Attr_NA : 1;
  uint16_t Pre_Initialized_NA : 1;
  uint16_t Pre_Value_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Value : 2;
  uint16_t Post_Other : 1;
} CPthreadReqMutexattrSetpshared_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutexattr-setpshared test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `attr` parameter value.
   */
  pthread_mutexattr_t *attr;

  /**
   * @brief This member specifies the `pshared` parameter value.
   */
  int value;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutexattr_setpshared() call.
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

  struct {
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
    CPthreadReqMutexattrSetpshared_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexattrSetpshared_Context;

static CPthreadReqMutexattrSetpshared_Context
  CPthreadReqMutexattrSetpshared_Instance;

static const char *const CPthreadReqMutexattrSetpshared_PreDesc_Attr[] =
  { "Obj", "Null", "NA" };

static const char *const CPthreadReqMutexattrSetpshared_PreDesc_Initialized[] =
  { "Yes", "No", "NA" };

static const char *const CPthreadReqMutexattrSetpshared_PreDesc_Value[] =
  { "Private", "Shared", "Invalid", "NA" };

static const char *const *const CPthreadReqMutexattrSetpshared_PreDesc[] = {
  CPthreadReqMutexattrSetpshared_PreDesc_Attr,
  CPthreadReqMutexattrSetpshared_PreDesc_Initialized,
  CPthreadReqMutexattrSetpshared_PreDesc_Value,
  NULL
};

static void CPthreadReqMutexattrSetpshared_Pre_Attr_Prepare(
  CPthreadReqMutexattrSetpshared_Context *ctx,
  CPthreadReqMutexattrSetpshared_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetpshared_Pre_Attr_Obj: {
      /*
       * While the `attr` parameter references an object of type
       * pthread_mutexattr_t.
       */
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexattrSetpshared_Pre_Attr_Null: {
      /*
       * While the `attr` parameter is equal to NULL.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexattrSetpshared_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetpshared_Pre_Initialized_Prepare(
  CPthreadReqMutexattrSetpshared_Context        *ctx,
  CPthreadReqMutexattrSetpshared_Pre_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetpshared_Pre_Initialized_Yes: {
      /*
       * While the object referenced by the `attr` parameter is an initialized
       * mutex attributes object.
       */
      T_eq_int( pthread_mutexattr_init( &ctx->attr_obj ), 0 );
      ctx->attr_obj.process_shared = -1;
      break;
    }

    case CPthreadReqMutexattrSetpshared_Pre_Initialized_No: {
      /*
       * While the object referenced by the `attr` parameter is not an
       * initialized mutex attributes object.
       */
      memset( &ctx->attr_obj, 0xff, sizeof( ctx->attr_obj ) );
      ctx->attr_obj.is_initialized = 0;
      ctx->attr_obj.process_shared = -1;
      break;
    }

    case CPthreadReqMutexattrSetpshared_Pre_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetpshared_Pre_Value_Prepare(
  CPthreadReqMutexattrSetpshared_Context  *ctx,
  CPthreadReqMutexattrSetpshared_Pre_Value state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetpshared_Pre_Value_Private: {
      /*
       * While the `pshared` parameter is equal to PTHREAD_PROCESS_PRIVATE.
       */
      ctx->value = PTHREAD_PROCESS_PRIVATE;
      break;
    }

    case CPthreadReqMutexattrSetpshared_Pre_Value_Shared: {
      /*
       * While the `pshared` parameter is equal to PTHREAD_PROCESS_SHARED.
       */
      ctx->value = PTHREAD_PROCESS_SHARED;
      break;
    }

    case CPthreadReqMutexattrSetpshared_Pre_Value_Invalid: {
      /*
       * While the `pshared` parameter is not a valid process-shared attribute
       * value.
       */
      ctx->value = 0x7fffffff;
      break;
    }

    case CPthreadReqMutexattrSetpshared_Pre_Value_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetpshared_Post_Status_Check(
  CPthreadReqMutexattrSetpshared_Context    *ctx,
  CPthreadReqMutexattrSetpshared_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetpshared_Post_Status_Ok: {
      /*
       * The return value of pthread_mutexattr_setpshared() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexattrSetpshared_Post_Status_Inval: {
      /*
       * The return value of pthread_mutexattr_setpshared() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexattrSetpshared_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetpshared_Post_Value_Check(
  CPthreadReqMutexattrSetpshared_Context   *ctx,
  CPthreadReqMutexattrSetpshared_Post_Value state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetpshared_Post_Value_Private: {
      /*
       * The process-shared attribute of the mutex attributes object referenced
       * by the `attr` parameter shall be PTHREAD_PROCESS_PRIVATE.
       */
      T_eq_int( ctx->attr_obj.process_shared, PTHREAD_PROCESS_PRIVATE );
      break;
    }

    case CPthreadReqMutexattrSetpshared_Post_Value_Shared: {
      /*
       * The process-shared attribute of the mutex attributes object referenced
       * by the `attr` parameter shall be PTHREAD_PROCESS_SHARED.
       */
      T_eq_int( ctx->attr_obj.process_shared, PTHREAD_PROCESS_SHARED );
      break;
    }

    case CPthreadReqMutexattrSetpshared_Post_Value_Nop: {
      /*
       * The process-shared attribute of the mutex attributes object referenced
       * by the `attr` parameter shall not be modified.
       */
      T_eq_int(
        ctx->attr_obj.process_shared,
        ctx->attr_before.process_shared
      );
      break;
    }

    case CPthreadReqMutexattrSetpshared_Post_Value_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetpshared_Post_Other_Check(
  CPthreadReqMutexattrSetpshared_Context   *ctx,
  CPthreadReqMutexattrSetpshared_Post_Other state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetpshared_Post_Other_Nop: {
      /*
       * The content of the mutex attributes object referenced by the `attr`
       * parameter, other than its process-shared attribute, shall not be
       * modified.
       */
      pthread_mutexattr_t expected;

      memcpy( &expected, &ctx->attr_before, sizeof( expected ) );
      expected.process_shared = ctx->attr_obj.process_shared;
      T_eq_mem( &ctx->attr_obj, &expected, sizeof( expected ) );
      break;
    }

    case CPthreadReqMutexattrSetpshared_Post_Other_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetpshared_Action(
  CPthreadReqMutexattrSetpshared_Context *ctx
)
{
  memcpy( &ctx->attr_before, &ctx->attr_obj, sizeof( ctx->attr_before ) );
  ctx->status = pthread_mutexattr_setpshared( ctx->attr, ctx->value );
}

/* clang-format off */

static const CPthreadReqMutexattrSetpshared_Entry
CPthreadReqMutexattrSetpshared_Entries[] = {
  { 0, 0, 0, 0, CPthreadReqMutexattrSetpshared_Post_Status_Inval,
    CPthreadReqMutexattrSetpshared_Post_Value_NA,
    CPthreadReqMutexattrSetpshared_Post_Other_NA },
  { 0, 0, 0, 0, CPthreadReqMutexattrSetpshared_Post_Status_Inval,
    CPthreadReqMutexattrSetpshared_Post_Value_Nop,
    CPthreadReqMutexattrSetpshared_Post_Other_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexattrSetpshared_Post_Status_Ok,
    CPthreadReqMutexattrSetpshared_Post_Value_Private,
    CPthreadReqMutexattrSetpshared_Post_Other_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexattrSetpshared_Post_Status_Ok,
    CPthreadReqMutexattrSetpshared_Post_Value_Shared,
    CPthreadReqMutexattrSetpshared_Post_Other_Nop }
};

static const uint8_t
CPthreadReqMutexattrSetpshared_Map[] = {
  2, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t CPthreadReqMutexattrSetpshared_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexattrSetpshared_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexattrSetpshared_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexattrSetpshared_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexattrSetpshared_Scope,
  .initial_context = &CPthreadReqMutexattrSetpshared_Instance
};

static inline CPthreadReqMutexattrSetpshared_Entry
CPthreadReqMutexattrSetpshared_PopEntry(
  CPthreadReqMutexattrSetpshared_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexattrSetpshared_Entries
    [ CPthreadReqMutexattrSetpshared_Map[ index ] ];
}

static void CPthreadReqMutexattrSetpshared_TestVariant(
  CPthreadReqMutexattrSetpshared_Context *ctx
)
{
  CPthreadReqMutexattrSetpshared_Pre_Attr_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexattrSetpshared_Pre_Initialized_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexattrSetpshared_Pre_Value_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexattrSetpshared_Action( ctx );
  CPthreadReqMutexattrSetpshared_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexattrSetpshared_Post_Value_Check(
    ctx,
    ctx->Map.entry.Post_Value
  );
  CPthreadReqMutexattrSetpshared_Post_Other_Check(
    ctx,
    ctx->Map.entry.Post_Other
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexattrSetpshared( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexattrSetpshared,
  &CPthreadReqMutexattrSetpshared_Fixture
)
{
  CPthreadReqMutexattrSetpshared_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = CPthreadReqMutexattrSetpshared_Pre_Attr_Obj;
    ctx->Map.pcs[ 0 ] < CPthreadReqMutexattrSetpshared_Pre_Attr_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = CPthreadReqMutexattrSetpshared_Pre_Initialized_Yes;
      ctx->Map.pcs[ 1 ] < CPthreadReqMutexattrSetpshared_Pre_Initialized_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      for (
        ctx->Map.pcs[ 2 ] = CPthreadReqMutexattrSetpshared_Pre_Value_Private;
        ctx->Map.pcs[ 2 ] < CPthreadReqMutexattrSetpshared_Pre_Value_NA;
        ++ctx->Map.pcs[ 2 ]
      ) {
        ctx->Map.entry = CPthreadReqMutexattrSetpshared_PopEntry( ctx );
        CPthreadReqMutexattrSetpshared_TestVariant( ctx );
      }
    }
  }
}

/** @} */
