/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexattrSettype
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
 * @defgroup CPthreadReqMutexattrSettype spec:/c/pthread/req/mutexattr-settype
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexattrSettype_Pre_Attr_Obj,
  CPthreadReqMutexattrSettype_Pre_Attr_Null,
  CPthreadReqMutexattrSettype_Pre_Attr_NA
} CPthreadReqMutexattrSettype_Pre_Attr;

typedef enum {
  CPthreadReqMutexattrSettype_Pre_Initialized_Yes,
  CPthreadReqMutexattrSettype_Pre_Initialized_No,
  CPthreadReqMutexattrSettype_Pre_Initialized_NA
} CPthreadReqMutexattrSettype_Pre_Initialized;

typedef enum {
  CPthreadReqMutexattrSettype_Pre_Type_Normal,
  CPthreadReqMutexattrSettype_Pre_Type_Recursive,
  CPthreadReqMutexattrSettype_Pre_Type_Errorcheck,
  CPthreadReqMutexattrSettype_Pre_Type_Default,
  CPthreadReqMutexattrSettype_Pre_Type_Invalid,
  CPthreadReqMutexattrSettype_Pre_Type_NA
} CPthreadReqMutexattrSettype_Pre_Type;

typedef enum {
  CPthreadReqMutexattrSettype_Post_Status_Ok,
  CPthreadReqMutexattrSettype_Post_Status_Inval,
  CPthreadReqMutexattrSettype_Post_Status_NA
} CPthreadReqMutexattrSettype_Post_Status;

typedef enum {
  CPthreadReqMutexattrSettype_Post_Type_Normal,
  CPthreadReqMutexattrSettype_Post_Type_Recursive,
  CPthreadReqMutexattrSettype_Post_Type_Errorcheck,
  CPthreadReqMutexattrSettype_Post_Type_Default,
  CPthreadReqMutexattrSettype_Post_Type_Nop,
  CPthreadReqMutexattrSettype_Post_Type_NA
} CPthreadReqMutexattrSettype_Post_Type;

typedef enum {
  CPthreadReqMutexattrSettype_Post_Other_Nop,
  CPthreadReqMutexattrSettype_Post_Other_NA
} CPthreadReqMutexattrSettype_Post_Other;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Attr_NA : 1;
  uint16_t Pre_Initialized_NA : 1;
  uint16_t Pre_Type_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Type : 3;
  uint16_t Post_Other : 1;
} CPthreadReqMutexattrSettype_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutexattr-settype test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `attr` parameter value.
   */
  pthread_mutexattr_t *attr;

  /**
   * @brief This member specifies the `type` parameter value.
   */
  int type;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutexattr_settype() call.
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
    CPthreadReqMutexattrSettype_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexattrSettype_Context;

static CPthreadReqMutexattrSettype_Context
  CPthreadReqMutexattrSettype_Instance;

static const char *const CPthreadReqMutexattrSettype_PreDesc_Attr[] =
  { "Obj", "Null", "NA" };

static const char *const CPthreadReqMutexattrSettype_PreDesc_Initialized[] =
  { "Yes", "No", "NA" };

static const char *const CPthreadReqMutexattrSettype_PreDesc_Type[] =
  { "Normal", "Recursive", "Errorcheck", "Default", "Invalid", "NA" };

static const char *const *const CPthreadReqMutexattrSettype_PreDesc[] = {
  CPthreadReqMutexattrSettype_PreDesc_Attr,
  CPthreadReqMutexattrSettype_PreDesc_Initialized,
  CPthreadReqMutexattrSettype_PreDesc_Type,
  NULL
};

#define TYPE_INVALID 123

static void CPthreadReqMutexattrSettype_Pre_Attr_Prepare(
  CPthreadReqMutexattrSettype_Context *ctx,
  CPthreadReqMutexattrSettype_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSettype_Pre_Attr_Obj: {
      /*
       * While the `attr` parameter references an object of type
       * pthread_mutexattr_t.
       */
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexattrSettype_Pre_Attr_Null: {
      /*
       * While the `attr` parameter is NULL.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexattrSettype_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrSettype_Pre_Initialized_Prepare(
  CPthreadReqMutexattrSettype_Context        *ctx,
  CPthreadReqMutexattrSettype_Pre_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSettype_Pre_Initialized_Yes: {
      /*
       * While the object referenced by the `attr` parameter is an initialized
       * mutex attributes object.
       */
      T_eno_success( pthread_mutexattr_init( &ctx->attr_obj ) );
      ctx->attr_obj.type = -1;
      break;
    }

    case CPthreadReqMutexattrSettype_Pre_Initialized_No: {
      /*
       * While the object referenced by the `attr` parameter is not an
       * initialized mutex attributes object.
       */
      memset( &ctx->attr_obj, 0, sizeof( ctx->attr_obj ) );
      ctx->attr_obj.type = -1;
      break;
    }

    case CPthreadReqMutexattrSettype_Pre_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrSettype_Pre_Type_Prepare(
  CPthreadReqMutexattrSettype_Context *ctx,
  CPthreadReqMutexattrSettype_Pre_Type state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSettype_Pre_Type_Normal: {
      /*
       * While the `type` parameter is equal to PTHREAD_MUTEX_NORMAL.
       */
      ctx->type = PTHREAD_MUTEX_NORMAL;
      break;
    }

    case CPthreadReqMutexattrSettype_Pre_Type_Recursive: {
      /*
       * While the `type` parameter is equal to PTHREAD_MUTEX_RECURSIVE.
       */
      ctx->type = PTHREAD_MUTEX_RECURSIVE;
      break;
    }

    case CPthreadReqMutexattrSettype_Pre_Type_Errorcheck: {
      /*
       * While the `type` parameter is equal to PTHREAD_MUTEX_ERRORCHECK.
       */
      ctx->type = PTHREAD_MUTEX_ERRORCHECK;
      break;
    }

    case CPthreadReqMutexattrSettype_Pre_Type_Default: {
      /*
       * While the `type` parameter is equal to PTHREAD_MUTEX_DEFAULT.
       */
      ctx->type = PTHREAD_MUTEX_DEFAULT;
      break;
    }

    case CPthreadReqMutexattrSettype_Pre_Type_Invalid: {
      /*
       * While the `type` parameter is not a valid mutex type attribute.
       */
      ctx->type = TYPE_INVALID;
      break;
    }

    case CPthreadReqMutexattrSettype_Pre_Type_NA:
      break;
  }
}

static void CPthreadReqMutexattrSettype_Post_Status_Check(
  CPthreadReqMutexattrSettype_Context    *ctx,
  CPthreadReqMutexattrSettype_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSettype_Post_Status_Ok: {
      /*
       * The return value of pthread_mutexattr_settype() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexattrSettype_Post_Status_Inval: {
      /*
       * The return value of pthread_mutexattr_settype() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexattrSettype_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexattrSettype_Post_Type_Check(
  CPthreadReqMutexattrSettype_Context  *ctx,
  CPthreadReqMutexattrSettype_Post_Type state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSettype_Post_Type_Normal: {
      /*
       * The mutex type attribute of the mutex attributes object referenced by
       * the `attr` parameter shall be PTHREAD_MUTEX_NORMAL.
       */
      T_eq_int( ctx->attr_obj.type, PTHREAD_MUTEX_NORMAL );
      break;
    }

    case CPthreadReqMutexattrSettype_Post_Type_Recursive: {
      /*
       * The mutex type attribute of the mutex attributes object referenced by
       * the `attr` parameter shall be PTHREAD_MUTEX_RECURSIVE.
       */
      T_eq_int( ctx->attr_obj.type, PTHREAD_MUTEX_RECURSIVE );
      break;
    }

    case CPthreadReqMutexattrSettype_Post_Type_Errorcheck: {
      /*
       * The mutex type attribute of the mutex attributes object referenced by
       * the `attr` parameter shall be PTHREAD_MUTEX_ERRORCHECK.
       */
      T_eq_int( ctx->attr_obj.type, PTHREAD_MUTEX_ERRORCHECK );
      break;
    }

    case CPthreadReqMutexattrSettype_Post_Type_Default: {
      /*
       * The mutex type attribute of the mutex attributes object referenced by
       * the `attr` parameter shall be PTHREAD_MUTEX_DEFAULT.
       */
      T_eq_int( ctx->attr_obj.type, PTHREAD_MUTEX_DEFAULT );
      break;
    }

    case CPthreadReqMutexattrSettype_Post_Type_Nop: {
      /*
       * The mutex type attribute of the mutex attributes object referenced by
       * the `attr` parameter shall not be modified.
       */
      T_eq_int( ctx->attr_obj.type, ctx->attr_before.type );
      break;
    }

    case CPthreadReqMutexattrSettype_Post_Type_NA:
      break;
  }
}

static void CPthreadReqMutexattrSettype_Post_Other_Check(
  CPthreadReqMutexattrSettype_Context   *ctx,
  CPthreadReqMutexattrSettype_Post_Other state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSettype_Post_Other_Nop: {
      /*
       * The content of the mutex attributes object referenced by the `attr`
       * parameter, other than its mutex type attribute, shall not be modified.
       */
      pthread_mutexattr_t expected;

      memcpy( &expected, &ctx->attr_before, sizeof( expected ) );
      expected.type = ctx->attr_obj.type;
      T_eq_mem( &ctx->attr_obj, &expected, sizeof( expected ) );
      break;
    }

    case CPthreadReqMutexattrSettype_Post_Other_NA:
      break;
  }
}

static void CPthreadReqMutexattrSettype_Action(
  CPthreadReqMutexattrSettype_Context *ctx
)
{
  memcpy( &ctx->attr_before, &ctx->attr_obj, sizeof( ctx->attr_before ) );
  ctx->status = pthread_mutexattr_settype( ctx->attr, ctx->type );
}

/* clang-format off */

static const CPthreadReqMutexattrSettype_Entry
CPthreadReqMutexattrSettype_Entries[] = {
  { 0, 0, 0, 0, CPthreadReqMutexattrSettype_Post_Status_Inval,
    CPthreadReqMutexattrSettype_Post_Type_Nop,
    CPthreadReqMutexattrSettype_Post_Other_Nop },
  { 0, 0, 1, 0, CPthreadReqMutexattrSettype_Post_Status_Inval,
    CPthreadReqMutexattrSettype_Post_Type_NA,
    CPthreadReqMutexattrSettype_Post_Other_NA },
  { 1, 0, 0, 0, CPthreadReqMutexattrSettype_Post_Status_NA,
    CPthreadReqMutexattrSettype_Post_Type_NA,
    CPthreadReqMutexattrSettype_Post_Other_NA },
  { 0, 0, 0, 0, CPthreadReqMutexattrSettype_Post_Status_Ok,
    CPthreadReqMutexattrSettype_Post_Type_Normal,
    CPthreadReqMutexattrSettype_Post_Other_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexattrSettype_Post_Status_Ok,
    CPthreadReqMutexattrSettype_Post_Type_Recursive,
    CPthreadReqMutexattrSettype_Post_Other_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexattrSettype_Post_Status_Ok,
    CPthreadReqMutexattrSettype_Post_Type_Errorcheck,
    CPthreadReqMutexattrSettype_Post_Other_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexattrSettype_Post_Status_Ok,
    CPthreadReqMutexattrSettype_Post_Type_Default,
    CPthreadReqMutexattrSettype_Post_Other_Nop }
};

static const uint8_t
CPthreadReqMutexattrSettype_Map[] = {
  3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2
};

/* clang-format on */

static size_t CPthreadReqMutexattrSettype_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexattrSettype_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexattrSettype_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexattrSettype_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexattrSettype_Scope,
  .initial_context = &CPthreadReqMutexattrSettype_Instance
};

static inline CPthreadReqMutexattrSettype_Entry
CPthreadReqMutexattrSettype_PopEntry(
  CPthreadReqMutexattrSettype_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexattrSettype_Entries
    [ CPthreadReqMutexattrSettype_Map[ index ] ];
}

static void CPthreadReqMutexattrSettype_SetPreConditionStates(
  CPthreadReqMutexattrSettype_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Initialized_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexattrSettype_Pre_Initialized_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
}

static void CPthreadReqMutexattrSettype_TestVariant(
  CPthreadReqMutexattrSettype_Context *ctx
)
{
  CPthreadReqMutexattrSettype_Pre_Attr_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexattrSettype_Pre_Initialized_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexattrSettype_Pre_Type_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexattrSettype_Action( ctx );
  CPthreadReqMutexattrSettype_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexattrSettype_Post_Type_Check( ctx, ctx->Map.entry.Post_Type );
  CPthreadReqMutexattrSettype_Post_Other_Check(
    ctx,
    ctx->Map.entry.Post_Other
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexattrSettype( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexattrSettype,
  &CPthreadReqMutexattrSettype_Fixture
)
{
  CPthreadReqMutexattrSettype_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexattrSettype_Pre_Attr_Obj;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexattrSettype_Pre_Attr_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexattrSettype_Pre_Initialized_Yes;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexattrSettype_Pre_Initialized_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexattrSettype_Pre_Type_Normal;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexattrSettype_Pre_Type_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        ctx->Map.entry = CPthreadReqMutexattrSettype_PopEntry( ctx );

        if ( ctx->Map.entry.Skip ) {
          continue;
        }

        CPthreadReqMutexattrSettype_SetPreConditionStates( ctx );
        CPthreadReqMutexattrSettype_TestVariant( ctx );
      }
    }
  }
}

/** @} */
