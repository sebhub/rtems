/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexattrDestroy
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
#include <pthread.h>
#include <string.h>

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqMutexattrDestroy spec:/c/pthread/req/mutexattr-destroy
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexattrDestroy_Pre_Attr_Obj,
  CPthreadReqMutexattrDestroy_Pre_Attr_Null,
  CPthreadReqMutexattrDestroy_Pre_Attr_NA
} CPthreadReqMutexattrDestroy_Pre_Attr;

typedef enum {
  CPthreadReqMutexattrDestroy_Pre_Initialized_Yes,
  CPthreadReqMutexattrDestroy_Pre_Initialized_No,
  CPthreadReqMutexattrDestroy_Pre_Initialized_NA
} CPthreadReqMutexattrDestroy_Pre_Initialized;

typedef enum {
  CPthreadReqMutexattrDestroy_Post_Status_Ok,
  CPthreadReqMutexattrDestroy_Post_Status_Inval,
  CPthreadReqMutexattrDestroy_Post_Status_NA
} CPthreadReqMutexattrDestroy_Post_Status;

typedef enum {
  CPthreadReqMutexattrDestroy_Post_Initialized_Cleared,
  CPthreadReqMutexattrDestroy_Post_Initialized_Nop,
  CPthreadReqMutexattrDestroy_Post_Initialized_NA
} CPthreadReqMutexattrDestroy_Post_Initialized;

typedef enum {
  CPthreadReqMutexattrDestroy_Post_Other_Nop,
  CPthreadReqMutexattrDestroy_Post_Other_NA
} CPthreadReqMutexattrDestroy_Post_Other;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Attr_NA : 1;
  uint8_t Pre_Initialized_NA : 1;
  uint8_t Post_Status : 2;
  uint8_t Post_Initialized : 2;
  uint8_t Post_Other : 1;
} CPthreadReqMutexattrDestroy_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutexattr-destroy test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `attr` parameter value.
   */
  pthread_mutexattr_t *attr;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutexattr_destroy() call.
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
    CPthreadReqMutexattrDestroy_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexattrDestroy_Context;

static CPthreadReqMutexattrDestroy_Context
  CPthreadReqMutexattrDestroy_Instance;

static const char *const CPthreadReqMutexattrDestroy_PreDesc_Attr[] =
  { "Obj", "Null", "NA" };

static const char *const CPthreadReqMutexattrDestroy_PreDesc_Initialized[] =
  { "Yes", "No", "NA" };

static const char *const *const CPthreadReqMutexattrDestroy_PreDesc[] = {
  CPthreadReqMutexattrDestroy_PreDesc_Attr,
  CPthreadReqMutexattrDestroy_PreDesc_Initialized,
  NULL
};

static void CPthreadReqMutexattrDestroy_Pre_Attr_Prepare(
  CPthreadReqMutexattrDestroy_Context *ctx,
  CPthreadReqMutexattrDestroy_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrDestroy_Pre_Attr_Obj: {
      /*
       * While the `attr` parameter references an object of type
       * pthread_mutexattr_t.
       */
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexattrDestroy_Pre_Attr_Null: {
      /*
       * While the `attr` parameter is equal to NULL.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexattrDestroy_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrDestroy_Pre_Initialized_Prepare(
  CPthreadReqMutexattrDestroy_Context        *ctx,
  CPthreadReqMutexattrDestroy_Pre_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrDestroy_Pre_Initialized_Yes: {
      /*
       * While the object referenced by the `attr` parameter is an initialized
       * mutex attributes object.
       */
      T_eq_int( pthread_mutexattr_init( &ctx->attr_obj ), 0 );
      break;
    }

    case CPthreadReqMutexattrDestroy_Pre_Initialized_No: {
      /*
       * While the object referenced by the `attr` parameter is not an
       * initialized mutex attributes object.
       */
      memset( &ctx->attr_obj, 0xff, sizeof( ctx->attr_obj ) );
      ctx->attr_obj.is_initialized = 0;
      break;
    }

    case CPthreadReqMutexattrDestroy_Pre_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrDestroy_Post_Status_Check(
  CPthreadReqMutexattrDestroy_Context    *ctx,
  CPthreadReqMutexattrDestroy_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrDestroy_Post_Status_Ok: {
      /*
       * The return value of pthread_mutexattr_destroy() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexattrDestroy_Post_Status_Inval: {
      /*
       * The return value of pthread_mutexattr_destroy() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexattrDestroy_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexattrDestroy_Post_Initialized_Check(
  CPthreadReqMutexattrDestroy_Context         *ctx,
  CPthreadReqMutexattrDestroy_Post_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrDestroy_Post_Initialized_Cleared: {
      /*
       * The mutex attributes object referenced by the `attr` parameter shall
       * not be an initialized mutex attributes object.
       */
      T_false( ctx->attr_obj.is_initialized );
      break;
    }

    case CPthreadReqMutexattrDestroy_Post_Initialized_Nop: {
      /*
       * The initialization state of the mutex attributes object referenced by
       * the `attr` parameter shall not be modified.
       */
      T_eq_int(
        ctx->attr_obj.is_initialized,
        ctx->attr_before.is_initialized
      );
      break;
    }

    case CPthreadReqMutexattrDestroy_Post_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrDestroy_Post_Other_Check(
  CPthreadReqMutexattrDestroy_Context   *ctx,
  CPthreadReqMutexattrDestroy_Post_Other state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrDestroy_Post_Other_Nop: {
      /*
       * The content of the mutex attributes object referenced by the `attr`
       * parameter, other than its initialization state, shall not be modified.
       */
      pthread_mutexattr_t expected;

      memcpy( &expected, &ctx->attr_before, sizeof( expected ) );
      expected.is_initialized = ctx->attr_obj.is_initialized;
      T_eq_mem( &ctx->attr_obj, &expected, sizeof( expected ) );
      break;
    }

    case CPthreadReqMutexattrDestroy_Post_Other_NA:
      break;
  }
}

static void CPthreadReqMutexattrDestroy_Action(
  CPthreadReqMutexattrDestroy_Context *ctx
)
{
  memcpy( &ctx->attr_before, &ctx->attr_obj, sizeof( ctx->attr_before ) );
  ctx->status = pthread_mutexattr_destroy( ctx->attr );
}

/* clang-format off */

static const CPthreadReqMutexattrDestroy_Entry
CPthreadReqMutexattrDestroy_Entries[] = {
  { 0, 0, 0, CPthreadReqMutexattrDestroy_Post_Status_Inval,
    CPthreadReqMutexattrDestroy_Post_Initialized_NA,
    CPthreadReqMutexattrDestroy_Post_Other_NA },
  { 0, 0, 0, CPthreadReqMutexattrDestroy_Post_Status_Ok,
    CPthreadReqMutexattrDestroy_Post_Initialized_Cleared,
    CPthreadReqMutexattrDestroy_Post_Other_Nop },
  { 0, 0, 0, CPthreadReqMutexattrDestroy_Post_Status_Inval,
    CPthreadReqMutexattrDestroy_Post_Initialized_Nop,
    CPthreadReqMutexattrDestroy_Post_Other_Nop }
};

static const uint8_t
CPthreadReqMutexattrDestroy_Map[] = {
  1, 2, 0, 0
};

/* clang-format on */

static size_t CPthreadReqMutexattrDestroy_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexattrDestroy_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexattrDestroy_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexattrDestroy_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexattrDestroy_Scope,
  .initial_context = &CPthreadReqMutexattrDestroy_Instance
};

static inline CPthreadReqMutexattrDestroy_Entry
CPthreadReqMutexattrDestroy_PopEntry(
  CPthreadReqMutexattrDestroy_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexattrDestroy_Entries
    [ CPthreadReqMutexattrDestroy_Map[ index ] ];
}

static void CPthreadReqMutexattrDestroy_TestVariant(
  CPthreadReqMutexattrDestroy_Context *ctx
)
{
  CPthreadReqMutexattrDestroy_Pre_Attr_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexattrDestroy_Pre_Initialized_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexattrDestroy_Action( ctx );
  CPthreadReqMutexattrDestroy_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexattrDestroy_Post_Initialized_Check(
    ctx,
    ctx->Map.entry.Post_Initialized
  );
  CPthreadReqMutexattrDestroy_Post_Other_Check(
    ctx,
    ctx->Map.entry.Post_Other
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexattrDestroy( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexattrDestroy,
  &CPthreadReqMutexattrDestroy_Fixture
)
{
  CPthreadReqMutexattrDestroy_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = CPthreadReqMutexattrDestroy_Pre_Attr_Obj;
    ctx->Map.pcs[ 0 ] < CPthreadReqMutexattrDestroy_Pre_Attr_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = CPthreadReqMutexattrDestroy_Pre_Initialized_Yes;
      ctx->Map.pcs[ 1 ] < CPthreadReqMutexattrDestroy_Pre_Initialized_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      ctx->Map.entry = CPthreadReqMutexattrDestroy_PopEntry( ctx );
      CPthreadReqMutexattrDestroy_TestVariant( ctx );
    }
  }
}

/** @} */
