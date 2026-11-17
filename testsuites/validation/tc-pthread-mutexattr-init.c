/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexattrInit
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
 * @defgroup CPthreadReqMutexattrInit spec:/c/pthread/req/mutexattr-init
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexattrInit_Pre_Attr_Obj,
  CPthreadReqMutexattrInit_Pre_Attr_Null,
  CPthreadReqMutexattrInit_Pre_Attr_NA
} CPthreadReqMutexattrInit_Pre_Attr;

typedef enum {
  CPthreadReqMutexattrInit_Post_Status_Ok,
  CPthreadReqMutexattrInit_Post_Status_Inval,
  CPthreadReqMutexattrInit_Post_Status_NA
} CPthreadReqMutexattrInit_Post_Status;

typedef enum {
  CPthreadReqMutexattrInit_Post_Initialized_Yes,
  CPthreadReqMutexattrInit_Post_Initialized_NA
} CPthreadReqMutexattrInit_Post_Initialized;

typedef enum {
  CPthreadReqMutexattrInit_Post_Protocol_None,
  CPthreadReqMutexattrInit_Post_Protocol_NA
} CPthreadReqMutexattrInit_Post_Protocol;

typedef enum {
  CPthreadReqMutexattrInit_Post_Pshared_Private,
  CPthreadReqMutexattrInit_Post_Pshared_NA
} CPthreadReqMutexattrInit_Post_Pshared;

typedef enum {
  CPthreadReqMutexattrInit_Post_Type_Default,
  CPthreadReqMutexattrInit_Post_Type_NA
} CPthreadReqMutexattrInit_Post_Type;

typedef enum {
  CPthreadReqMutexattrInit_Post_Ceiling_Max,
  CPthreadReqMutexattrInit_Post_Ceiling_NA
} CPthreadReqMutexattrInit_Post_Ceiling;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Attr_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Initialized : 1;
  uint16_t Post_Protocol : 1;
  uint16_t Post_Pshared : 1;
  uint16_t Post_Type : 1;
  uint16_t Post_Ceiling : 1;
} CPthreadReqMutexattrInit_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutexattr-init test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `attr` parameter value.
   */
  pthread_mutexattr_t *attr;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutexattr_init() call.
   */
  int status;

  /**
   * @brief This member contains the mutex attributes object.
   */
  pthread_mutexattr_t attr_obj;

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
    CPthreadReqMutexattrInit_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexattrInit_Context;

static CPthreadReqMutexattrInit_Context CPthreadReqMutexattrInit_Instance;

static const char *const CPthreadReqMutexattrInit_PreDesc_Attr[] =
  { "Obj", "Null", "NA" };

static const char *const *const CPthreadReqMutexattrInit_PreDesc[] = {
  CPthreadReqMutexattrInit_PreDesc_Attr,
  NULL
};

static void CPthreadReqMutexattrInit_Pre_Attr_Prepare(
  CPthreadReqMutexattrInit_Context *ctx,
  CPthreadReqMutexattrInit_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrInit_Pre_Attr_Obj: {
      /*
       * While the `attr` parameter references an object of type
       * pthread_mutexattr_t.
       */
      ctx->attr = &ctx->attr_obj;
      memset( &ctx->attr_obj, 0xff, sizeof( ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrInit_Pre_Attr_Null: {
      /*
       * While the `attr` parameter is equal to NULL.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexattrInit_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrInit_Post_Status_Check(
  CPthreadReqMutexattrInit_Context    *ctx,
  CPthreadReqMutexattrInit_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrInit_Post_Status_Ok: {
      /*
       * The return value of pthread_mutexattr_init() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexattrInit_Post_Status_Inval: {
      /*
       * The return value of pthread_mutexattr_init() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexattrInit_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexattrInit_Post_Initialized_Check(
  CPthreadReqMutexattrInit_Context         *ctx,
  CPthreadReqMutexattrInit_Post_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrInit_Post_Initialized_Yes: {
      /*
       * The mutex attributes object referenced by the `attr` parameter shall
       * be an initialized mutex attributes object.
       */
      T_true( ctx->attr_obj.is_initialized );
      break;
    }

    case CPthreadReqMutexattrInit_Post_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrInit_Post_Protocol_Check(
  CPthreadReqMutexattrInit_Context      *ctx,
  CPthreadReqMutexattrInit_Post_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrInit_Post_Protocol_None: {
      /*
       * The mutex protocol attribute of the mutex attributes object referenced
       * by the `attr` parameter shall be PTHREAD_PRIO_NONE.
       */
      T_eq_int( ctx->attr_obj.protocol, PTHREAD_PRIO_NONE );
      break;
    }

    case CPthreadReqMutexattrInit_Post_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexattrInit_Post_Pshared_Check(
  CPthreadReqMutexattrInit_Context     *ctx,
  CPthreadReqMutexattrInit_Post_Pshared state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrInit_Post_Pshared_Private: {
      /*
       * The process-shared attribute of the mutex attributes object referenced
       * by the `attr` parameter shall be PTHREAD_PROCESS_PRIVATE.
       */
      T_eq_int( ctx->attr_obj.process_shared, PTHREAD_PROCESS_PRIVATE );
      break;
    }

    case CPthreadReqMutexattrInit_Post_Pshared_NA:
      break;
  }
}

static void CPthreadReqMutexattrInit_Post_Type_Check(
  CPthreadReqMutexattrInit_Context  *ctx,
  CPthreadReqMutexattrInit_Post_Type state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrInit_Post_Type_Default: {
      /*
       * The mutex type attribute of the mutex attributes object referenced by
       * the `attr` parameter shall be PTHREAD_MUTEX_DEFAULT.
       */
      T_eq_int( ctx->attr_obj.type, PTHREAD_MUTEX_DEFAULT );
      break;
    }

    case CPthreadReqMutexattrInit_Post_Type_NA:
      break;
  }
}

static void CPthreadReqMutexattrInit_Post_Ceiling_Check(
  CPthreadReqMutexattrInit_Context     *ctx,
  CPthreadReqMutexattrInit_Post_Ceiling state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrInit_Post_Ceiling_Max: {
      /*
       * The mutex priority ceiling attribute of the mutex attributes object
       * referenced by the `attr` parameter shall be the maximum of the type
       * int.
       */
      T_eq_int( ctx->attr_obj.prio_ceiling, INT_MAX );
      break;
    }

    case CPthreadReqMutexattrInit_Post_Ceiling_NA:
      break;
  }
}

static void CPthreadReqMutexattrInit_Action(
  CPthreadReqMutexattrInit_Context *ctx
)
{
  ctx->status = pthread_mutexattr_init( ctx->attr );
}

/* clang-format off */

static const CPthreadReqMutexattrInit_Entry
CPthreadReqMutexattrInit_Entries[] = {
  { 0, 0, CPthreadReqMutexattrInit_Post_Status_Ok,
    CPthreadReqMutexattrInit_Post_Initialized_Yes,
    CPthreadReqMutexattrInit_Post_Protocol_None,
    CPthreadReqMutexattrInit_Post_Pshared_Private,
    CPthreadReqMutexattrInit_Post_Type_Default,
    CPthreadReqMutexattrInit_Post_Ceiling_Max },
  { 0, 0, CPthreadReqMutexattrInit_Post_Status_Inval,
    CPthreadReqMutexattrInit_Post_Initialized_NA,
    CPthreadReqMutexattrInit_Post_Protocol_NA,
    CPthreadReqMutexattrInit_Post_Pshared_NA,
    CPthreadReqMutexattrInit_Post_Type_NA,
    CPthreadReqMutexattrInit_Post_Ceiling_NA }
};

static const uint8_t
CPthreadReqMutexattrInit_Map[] = {
  0, 1
};

/* clang-format on */

static size_t CPthreadReqMutexattrInit_Scope( void *arg, char *buf, size_t n )
{
  CPthreadReqMutexattrInit_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexattrInit_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexattrInit_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexattrInit_Scope,
  .initial_context = &CPthreadReqMutexattrInit_Instance
};

static inline CPthreadReqMutexattrInit_Entry CPthreadReqMutexattrInit_PopEntry(
  CPthreadReqMutexattrInit_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexattrInit_Entries
    [ CPthreadReqMutexattrInit_Map[ index ] ];
}

static void CPthreadReqMutexattrInit_TestVariant(
  CPthreadReqMutexattrInit_Context *ctx
)
{
  CPthreadReqMutexattrInit_Pre_Attr_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexattrInit_Action( ctx );
  CPthreadReqMutexattrInit_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexattrInit_Post_Initialized_Check(
    ctx,
    ctx->Map.entry.Post_Initialized
  );
  CPthreadReqMutexattrInit_Post_Protocol_Check(
    ctx,
    ctx->Map.entry.Post_Protocol
  );
  CPthreadReqMutexattrInit_Post_Pshared_Check(
    ctx,
    ctx->Map.entry.Post_Pshared
  );
  CPthreadReqMutexattrInit_Post_Type_Check( ctx, ctx->Map.entry.Post_Type );
  CPthreadReqMutexattrInit_Post_Ceiling_Check(
    ctx,
    ctx->Map.entry.Post_Ceiling
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexattrInit( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexattrInit,
  &CPthreadReqMutexattrInit_Fixture
)
{
  CPthreadReqMutexattrInit_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = CPthreadReqMutexattrInit_Pre_Attr_Obj;
    ctx->Map.pcs[ 0 ] < CPthreadReqMutexattrInit_Pre_Attr_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = CPthreadReqMutexattrInit_PopEntry( ctx );
    CPthreadReqMutexattrInit_TestVariant( ctx );
  }
}

/** @} */
