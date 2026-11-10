/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexattrGettype
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
 * @defgroup CPthreadReqMutexattrGettype spec:/c/pthread/req/mutexattr-gettype
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexattrGettype_Pre_Attr_Obj,
  CPthreadReqMutexattrGettype_Pre_Attr_Null,
  CPthreadReqMutexattrGettype_Pre_Attr_NA
} CPthreadReqMutexattrGettype_Pre_Attr;

typedef enum {
  CPthreadReqMutexattrGettype_Pre_Initialized_Yes,
  CPthreadReqMutexattrGettype_Pre_Initialized_No,
  CPthreadReqMutexattrGettype_Pre_Initialized_NA
} CPthreadReqMutexattrGettype_Pre_Initialized;

typedef enum {
  CPthreadReqMutexattrGettype_Pre_StoredType_Normal,
  CPthreadReqMutexattrGettype_Pre_StoredType_Recursive,
  CPthreadReqMutexattrGettype_Pre_StoredType_Errorcheck,
  CPthreadReqMutexattrGettype_Pre_StoredType_Default,
  CPthreadReqMutexattrGettype_Pre_StoredType_NA
} CPthreadReqMutexattrGettype_Pre_StoredType;

typedef enum {
  CPthreadReqMutexattrGettype_Pre_Type_Obj,
  CPthreadReqMutexattrGettype_Pre_Type_Null,
  CPthreadReqMutexattrGettype_Pre_Type_NA
} CPthreadReqMutexattrGettype_Pre_Type;

typedef enum {
  CPthreadReqMutexattrGettype_Post_Status_Ok,
  CPthreadReqMutexattrGettype_Post_Status_Inval,
  CPthreadReqMutexattrGettype_Post_Status_NA
} CPthreadReqMutexattrGettype_Post_Status;

typedef enum {
  CPthreadReqMutexattrGettype_Post_Type_Normal,
  CPthreadReqMutexattrGettype_Post_Type_Recursive,
  CPthreadReqMutexattrGettype_Post_Type_Errorcheck,
  CPthreadReqMutexattrGettype_Post_Type_Default,
  CPthreadReqMutexattrGettype_Post_Type_Nop,
  CPthreadReqMutexattrGettype_Post_Type_NA
} CPthreadReqMutexattrGettype_Post_Type;

typedef enum {
  CPthreadReqMutexattrGettype_Post_Attr_Nop,
  CPthreadReqMutexattrGettype_Post_Attr_NA
} CPthreadReqMutexattrGettype_Post_Attr;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Attr_NA : 1;
  uint16_t Pre_Initialized_NA : 1;
  uint16_t Pre_StoredType_NA : 1;
  uint16_t Pre_Type_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Type : 3;
  uint16_t Post_Attr : 1;
} CPthreadReqMutexattrGettype_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutexattr-gettype test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `attr` parameter value.
   */
  const pthread_mutexattr_t *attr;

  /**
   * @brief This member specifies the `type` parameter value.
   */
  int *type;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutexattr_gettype() call.
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
   * @brief This member contains the object referenced by the `type` parameter.
   */
  int type_obj;

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
    CPthreadReqMutexattrGettype_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexattrGettype_Context;

static CPthreadReqMutexattrGettype_Context
  CPthreadReqMutexattrGettype_Instance;

static const char *const CPthreadReqMutexattrGettype_PreDesc_Attr[] =
  { "Obj", "Null", "NA" };

static const char *const CPthreadReqMutexattrGettype_PreDesc_Initialized[] =
  { "Yes", "No", "NA" };

static const char *const CPthreadReqMutexattrGettype_PreDesc_StoredType[] =
  { "Normal", "Recursive", "Errorcheck", "Default", "NA" };

static const char *const CPthreadReqMutexattrGettype_PreDesc_Type[] =
  { "Obj", "Null", "NA" };

static const char *const *const CPthreadReqMutexattrGettype_PreDesc[] = {
  CPthreadReqMutexattrGettype_PreDesc_Attr,
  CPthreadReqMutexattrGettype_PreDesc_Initialized,
  CPthreadReqMutexattrGettype_PreDesc_StoredType,
  CPthreadReqMutexattrGettype_PreDesc_Type,
  NULL
};

#define POISON -1

static void CPthreadReqMutexattrGettype_Pre_Attr_Prepare(
  CPthreadReqMutexattrGettype_Context *ctx,
  CPthreadReqMutexattrGettype_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGettype_Pre_Attr_Obj: {
      /*
       * While the `attr` parameter references an object of type
       * pthread_mutexattr_t.
       */
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexattrGettype_Pre_Attr_Null: {
      /*
       * While the `attr` parameter is NULL.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexattrGettype_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrGettype_Pre_Initialized_Prepare(
  CPthreadReqMutexattrGettype_Context        *ctx,
  CPthreadReqMutexattrGettype_Pre_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGettype_Pre_Initialized_Yes: {
      /*
       * While the object referenced by the `attr` parameter is an initialized
       * mutex attributes object.
       */
      T_eno_success( pthread_mutexattr_init( &ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrGettype_Pre_Initialized_No: {
      /*
       * While the object referenced by the `attr` parameter is not an
       * initialized mutex attributes object.
       */
      memset( &ctx->attr_obj, 0, sizeof( ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrGettype_Pre_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrGettype_Pre_StoredType_Prepare(
  CPthreadReqMutexattrGettype_Context       *ctx,
  CPthreadReqMutexattrGettype_Pre_StoredType state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGettype_Pre_StoredType_Normal: {
      /*
       * While the mutex type attribute of the object referenced by the `attr`
       * parameter is PTHREAD_MUTEX_NORMAL.
       */
      ctx->attr_obj.type = PTHREAD_MUTEX_NORMAL;
      break;
    }

    case CPthreadReqMutexattrGettype_Pre_StoredType_Recursive: {
      /*
       * While the mutex type attribute of the object referenced by the `attr`
       * parameter is PTHREAD_MUTEX_RECURSIVE.
       */
      ctx->attr_obj.type = PTHREAD_MUTEX_RECURSIVE;
      break;
    }

    case CPthreadReqMutexattrGettype_Pre_StoredType_Errorcheck: {
      /*
       * While the mutex type attribute of the object referenced by the `attr`
       * parameter is PTHREAD_MUTEX_ERRORCHECK.
       */
      ctx->attr_obj.type = PTHREAD_MUTEX_ERRORCHECK;
      break;
    }

    case CPthreadReqMutexattrGettype_Pre_StoredType_Default: {
      /*
       * While the mutex type attribute of the object referenced by the `attr`
       * parameter is PTHREAD_MUTEX_DEFAULT.
       */
      ctx->attr_obj.type = PTHREAD_MUTEX_DEFAULT;
      break;
    }

    case CPthreadReqMutexattrGettype_Pre_StoredType_NA:
      break;
  }
}

static void CPthreadReqMutexattrGettype_Pre_Type_Prepare(
  CPthreadReqMutexattrGettype_Context *ctx,
  CPthreadReqMutexattrGettype_Pre_Type state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGettype_Pre_Type_Obj: {
      /*
       * While the `type` parameter references an object of type int.
       */
      ctx->type_obj = POISON;
      ctx->type = &ctx->type_obj;
      break;
    }

    case CPthreadReqMutexattrGettype_Pre_Type_Null: {
      /*
       * While the `type` parameter is NULL.
       */
      ctx->type = NULL;
      break;
    }

    case CPthreadReqMutexattrGettype_Pre_Type_NA:
      break;
  }
}

static void CPthreadReqMutexattrGettype_Post_Status_Check(
  CPthreadReqMutexattrGettype_Context    *ctx,
  CPthreadReqMutexattrGettype_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGettype_Post_Status_Ok: {
      /*
       * The return value of pthread_mutexattr_gettype() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexattrGettype_Post_Status_Inval: {
      /*
       * The return value of pthread_mutexattr_gettype() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexattrGettype_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexattrGettype_Post_Type_Check(
  CPthreadReqMutexattrGettype_Context  *ctx,
  CPthreadReqMutexattrGettype_Post_Type state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGettype_Post_Type_Normal: {
      /*
       * The object referenced by the `type` parameter shall be
       * PTHREAD_MUTEX_NORMAL.
       */
      T_eq_int( ctx->type_obj, PTHREAD_MUTEX_NORMAL );
      break;
    }

    case CPthreadReqMutexattrGettype_Post_Type_Recursive: {
      /*
       * The object referenced by the `type` parameter shall be
       * PTHREAD_MUTEX_RECURSIVE.
       */
      T_eq_int( ctx->type_obj, PTHREAD_MUTEX_RECURSIVE );
      break;
    }

    case CPthreadReqMutexattrGettype_Post_Type_Errorcheck: {
      /*
       * The object referenced by the `type` parameter shall be
       * PTHREAD_MUTEX_ERRORCHECK.
       */
      T_eq_int( ctx->type_obj, PTHREAD_MUTEX_ERRORCHECK );
      break;
    }

    case CPthreadReqMutexattrGettype_Post_Type_Default: {
      /*
       * The object referenced by the `type` parameter shall be
       * PTHREAD_MUTEX_DEFAULT.
       */
      T_eq_int( ctx->type_obj, PTHREAD_MUTEX_DEFAULT );
      break;
    }

    case CPthreadReqMutexattrGettype_Post_Type_Nop: {
      /*
       * The object referenced by the `type` parameter shall not be modified.
       */
      T_eq_int( ctx->type_obj, POISON );
      break;
    }

    case CPthreadReqMutexattrGettype_Post_Type_NA:
      break;
  }
}

static void CPthreadReqMutexattrGettype_Post_Attr_Check(
  CPthreadReqMutexattrGettype_Context  *ctx,
  CPthreadReqMutexattrGettype_Post_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGettype_Post_Attr_Nop: {
      /*
       * The object referenced by the `attr` parameter shall not be modified.
       */
      T_eq_mem( &ctx->attr_obj, &ctx->attr_before, sizeof( ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrGettype_Post_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrGettype_Action(
  CPthreadReqMutexattrGettype_Context *ctx
)
{
  memcpy( &ctx->attr_before, &ctx->attr_obj, sizeof( ctx->attr_before ) );
  ctx->status = pthread_mutexattr_gettype( ctx->attr, ctx->type );
}

/* clang-format off */

static const CPthreadReqMutexattrGettype_Entry
CPthreadReqMutexattrGettype_Entries[] = {
  { 1, 0, 0, 0, 0, CPthreadReqMutexattrGettype_Post_Status_NA,
    CPthreadReqMutexattrGettype_Post_Type_NA,
    CPthreadReqMutexattrGettype_Post_Attr_NA },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGettype_Post_Status_Inval,
    CPthreadReqMutexattrGettype_Post_Type_NA,
    CPthreadReqMutexattrGettype_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGettype_Post_Status_Inval,
    CPthreadReqMutexattrGettype_Post_Type_Nop,
    CPthreadReqMutexattrGettype_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGettype_Post_Status_Ok,
    CPthreadReqMutexattrGettype_Post_Type_Normal,
    CPthreadReqMutexattrGettype_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGettype_Post_Status_Ok,
    CPthreadReqMutexattrGettype_Post_Type_Recursive,
    CPthreadReqMutexattrGettype_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGettype_Post_Status_Ok,
    CPthreadReqMutexattrGettype_Post_Type_Errorcheck,
    CPthreadReqMutexattrGettype_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGettype_Post_Status_Ok,
    CPthreadReqMutexattrGettype_Post_Type_Default,
    CPthreadReqMutexattrGettype_Post_Attr_Nop },
  { 0, 0, 1, 1, 0, CPthreadReqMutexattrGettype_Post_Status_Inval,
    CPthreadReqMutexattrGettype_Post_Type_Nop,
    CPthreadReqMutexattrGettype_Post_Attr_NA },
  { 0, 0, 1, 1, 0, CPthreadReqMutexattrGettype_Post_Status_Inval,
    CPthreadReqMutexattrGettype_Post_Type_NA,
    CPthreadReqMutexattrGettype_Post_Attr_NA }
};

static const uint8_t
CPthreadReqMutexattrGettype_Map[] = {
  3, 1, 4, 1, 5, 1, 6, 1, 2, 1, 2, 1, 2, 1, 2, 1, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t CPthreadReqMutexattrGettype_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexattrGettype_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexattrGettype_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexattrGettype_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexattrGettype_Scope,
  .initial_context = &CPthreadReqMutexattrGettype_Instance
};

static inline CPthreadReqMutexattrGettype_Entry
CPthreadReqMutexattrGettype_PopEntry(
  CPthreadReqMutexattrGettype_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexattrGettype_Entries
    [ CPthreadReqMutexattrGettype_Map[ index ] ];
}

static void CPthreadReqMutexattrGettype_SetPreConditionStates(
  CPthreadReqMutexattrGettype_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Initialized_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexattrGettype_Pre_Initialized_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_StoredType_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqMutexattrGettype_Pre_StoredType_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
}

static void CPthreadReqMutexattrGettype_TestVariant(
  CPthreadReqMutexattrGettype_Context *ctx
)
{
  CPthreadReqMutexattrGettype_Pre_Attr_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexattrGettype_Pre_Initialized_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexattrGettype_Pre_StoredType_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexattrGettype_Pre_Type_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqMutexattrGettype_Action( ctx );
  CPthreadReqMutexattrGettype_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexattrGettype_Post_Type_Check( ctx, ctx->Map.entry.Post_Type );
  CPthreadReqMutexattrGettype_Post_Attr_Check( ctx, ctx->Map.entry.Post_Attr );
}

/**
 * @fn void T_case_body_CPthreadReqMutexattrGettype( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexattrGettype,
  &CPthreadReqMutexattrGettype_Fixture
)
{
  CPthreadReqMutexattrGettype_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexattrGettype_Pre_Attr_Obj;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexattrGettype_Pre_Attr_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexattrGettype_Pre_Initialized_Yes;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexattrGettype_Pre_Initialized_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexattrGettype_Pre_StoredType_Normal;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexattrGettype_Pre_StoredType_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexattrGettype_Pre_Type_Obj;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexattrGettype_Pre_Type_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = CPthreadReqMutexattrGettype_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          CPthreadReqMutexattrGettype_SetPreConditionStates( ctx );
          CPthreadReqMutexattrGettype_TestVariant( ctx );
        }
      }
    }
  }
}

/** @} */
