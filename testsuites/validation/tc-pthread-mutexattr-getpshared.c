/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexattrGetpshared
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
 * @defgroup CPthreadReqMutexattrGetpshared \
 *   spec:/c/pthread/req/mutexattr-getpshared
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexattrGetpshared_Pre_Attr_Obj,
  CPthreadReqMutexattrGetpshared_Pre_Attr_Null,
  CPthreadReqMutexattrGetpshared_Pre_Attr_NA
} CPthreadReqMutexattrGetpshared_Pre_Attr;

typedef enum {
  CPthreadReqMutexattrGetpshared_Pre_Initialized_Yes,
  CPthreadReqMutexattrGetpshared_Pre_Initialized_No,
  CPthreadReqMutexattrGetpshared_Pre_Initialized_NA
} CPthreadReqMutexattrGetpshared_Pre_Initialized;

typedef enum {
  CPthreadReqMutexattrGetpshared_Pre_StoredPshared_Private,
  CPthreadReqMutexattrGetpshared_Pre_StoredPshared_Shared,
  CPthreadReqMutexattrGetpshared_Pre_StoredPshared_NA
} CPthreadReqMutexattrGetpshared_Pre_StoredPshared;

typedef enum {
  CPthreadReqMutexattrGetpshared_Pre_Pshared_Obj,
  CPthreadReqMutexattrGetpshared_Pre_Pshared_Null,
  CPthreadReqMutexattrGetpshared_Pre_Pshared_NA
} CPthreadReqMutexattrGetpshared_Pre_Pshared;

typedef enum {
  CPthreadReqMutexattrGetpshared_Post_Status_Ok,
  CPthreadReqMutexattrGetpshared_Post_Status_Inval,
  CPthreadReqMutexattrGetpshared_Post_Status_NA
} CPthreadReqMutexattrGetpshared_Post_Status;

typedef enum {
  CPthreadReqMutexattrGetpshared_Post_Pshared_Private,
  CPthreadReqMutexattrGetpshared_Post_Pshared_Shared,
  CPthreadReqMutexattrGetpshared_Post_Pshared_Nop,
  CPthreadReqMutexattrGetpshared_Post_Pshared_NA
} CPthreadReqMutexattrGetpshared_Post_Pshared;

typedef enum {
  CPthreadReqMutexattrGetpshared_Post_Attr_Nop,
  CPthreadReqMutexattrGetpshared_Post_Attr_NA
} CPthreadReqMutexattrGetpshared_Post_Attr;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Attr_NA : 1;
  uint16_t Pre_Initialized_NA : 1;
  uint16_t Pre_StoredPshared_NA : 1;
  uint16_t Pre_Pshared_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Pshared : 2;
  uint16_t Post_Attr : 1;
} CPthreadReqMutexattrGetpshared_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutexattr-getpshared test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `attr` parameter value.
   */
  const pthread_mutexattr_t *attr;

  /**
   * @brief This member specifies the `pshared` parameter value.
   */
  int *pshared;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutexattr_getpshared() call.
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
   * @brief This member contains the object referenced by the `pshared`
   *   parameter.
   */
  int pshared_obj;

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
    CPthreadReqMutexattrGetpshared_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexattrGetpshared_Context;

static CPthreadReqMutexattrGetpshared_Context
  CPthreadReqMutexattrGetpshared_Instance;

static const char *const CPthreadReqMutexattrGetpshared_PreDesc_Attr[] =
  { "Obj", "Null", "NA" };

static const char *const CPthreadReqMutexattrGetpshared_PreDesc_Initialized[] =
  { "Yes", "No", "NA" };

static const char *const
  CPthreadReqMutexattrGetpshared_PreDesc_StoredPshared[] =
    { "Private", "Shared", "NA" };

static const char *const CPthreadReqMutexattrGetpshared_PreDesc_Pshared[] =
  { "Obj", "Null", "NA" };

static const char *const *const CPthreadReqMutexattrGetpshared_PreDesc[] = {
  CPthreadReqMutexattrGetpshared_PreDesc_Attr,
  CPthreadReqMutexattrGetpshared_PreDesc_Initialized,
  CPthreadReqMutexattrGetpshared_PreDesc_StoredPshared,
  CPthreadReqMutexattrGetpshared_PreDesc_Pshared,
  NULL
};

#define POISON -1

static void CPthreadReqMutexattrGetpshared_Pre_Attr_Prepare(
  CPthreadReqMutexattrGetpshared_Context *ctx,
  CPthreadReqMutexattrGetpshared_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetpshared_Pre_Attr_Obj: {
      /*
       * While the `attr` parameter references an object of type
       * pthread_mutexattr_t.
       */
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexattrGetpshared_Pre_Attr_Null: {
      /*
       * While the `attr` parameter is NULL.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexattrGetpshared_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetpshared_Pre_Initialized_Prepare(
  CPthreadReqMutexattrGetpshared_Context        *ctx,
  CPthreadReqMutexattrGetpshared_Pre_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetpshared_Pre_Initialized_Yes: {
      /*
       * While the object referenced by the `attr` parameter is an initialized
       * mutex attributes object.
       */
      T_eno_success( pthread_mutexattr_init( &ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrGetpshared_Pre_Initialized_No: {
      /*
       * While the object referenced by the `attr` parameter is not an
       * initialized mutex attributes object.
       */
      memset( &ctx->attr_obj, 0, sizeof( ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrGetpshared_Pre_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetpshared_Pre_StoredPshared_Prepare(
  CPthreadReqMutexattrGetpshared_Context          *ctx,
  CPthreadReqMutexattrGetpshared_Pre_StoredPshared state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetpshared_Pre_StoredPshared_Private: {
      /*
       * While the process-shared attribute of the object referenced by the
       * `attr` parameter is PTHREAD_PROCESS_PRIVATE.
       */
      ctx->attr_obj.process_shared = PTHREAD_PROCESS_PRIVATE;
      break;
    }

    case CPthreadReqMutexattrGetpshared_Pre_StoredPshared_Shared: {
      /*
       * While the process-shared attribute of the object referenced by the
       * `attr` parameter is PTHREAD_PROCESS_SHARED.
       */
      ctx->attr_obj.process_shared = PTHREAD_PROCESS_SHARED;
      break;
    }

    case CPthreadReqMutexattrGetpshared_Pre_StoredPshared_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetpshared_Pre_Pshared_Prepare(
  CPthreadReqMutexattrGetpshared_Context    *ctx,
  CPthreadReqMutexattrGetpshared_Pre_Pshared state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetpshared_Pre_Pshared_Obj: {
      /*
       * While the `pshared` parameter references an object of type int.
       */
      ctx->pshared_obj = POISON;
      ctx->pshared = &ctx->pshared_obj;
      break;
    }

    case CPthreadReqMutexattrGetpshared_Pre_Pshared_Null: {
      /*
       * While the `pshared` parameter is NULL.
       */
      ctx->pshared = NULL;
      break;
    }

    case CPthreadReqMutexattrGetpshared_Pre_Pshared_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetpshared_Post_Status_Check(
  CPthreadReqMutexattrGetpshared_Context    *ctx,
  CPthreadReqMutexattrGetpshared_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetpshared_Post_Status_Ok: {
      /*
       * The return value of pthread_mutexattr_getpshared() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexattrGetpshared_Post_Status_Inval: {
      /*
       * The return value of pthread_mutexattr_getpshared() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexattrGetpshared_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetpshared_Post_Pshared_Check(
  CPthreadReqMutexattrGetpshared_Context     *ctx,
  CPthreadReqMutexattrGetpshared_Post_Pshared state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetpshared_Post_Pshared_Private: {
      /*
       * The object referenced by the `pshared` parameter shall be
       * PTHREAD_PROCESS_PRIVATE.
       */
      T_eq_int( ctx->pshared_obj, PTHREAD_PROCESS_PRIVATE );
      break;
    }

    case CPthreadReqMutexattrGetpshared_Post_Pshared_Shared: {
      /*
       * The object referenced by the `pshared` parameter shall be
       * PTHREAD_PROCESS_SHARED.
       */
      T_eq_int( ctx->pshared_obj, PTHREAD_PROCESS_SHARED );
      break;
    }

    case CPthreadReqMutexattrGetpshared_Post_Pshared_Nop: {
      /*
       * The object referenced by the `pshared` parameter shall not be
       * modified.
       */
      T_eq_int( ctx->pshared_obj, POISON );
      break;
    }

    case CPthreadReqMutexattrGetpshared_Post_Pshared_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetpshared_Post_Attr_Check(
  CPthreadReqMutexattrGetpshared_Context  *ctx,
  CPthreadReqMutexattrGetpshared_Post_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetpshared_Post_Attr_Nop: {
      /*
       * The object referenced by the `attr` parameter shall not be modified.
       */
      T_eq_mem( &ctx->attr_obj, &ctx->attr_before, sizeof( ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrGetpshared_Post_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetpshared_Action(
  CPthreadReqMutexattrGetpshared_Context *ctx
)
{
  memcpy( &ctx->attr_before, &ctx->attr_obj, sizeof( ctx->attr_before ) );
  ctx->status = pthread_mutexattr_getpshared( ctx->attr, ctx->pshared );
}

/* clang-format off */

static const CPthreadReqMutexattrGetpshared_Entry
CPthreadReqMutexattrGetpshared_Entries[] = {
  { 1, 0, 0, 0, 0, CPthreadReqMutexattrGetpshared_Post_Status_NA,
    CPthreadReqMutexattrGetpshared_Post_Pshared_NA,
    CPthreadReqMutexattrGetpshared_Post_Attr_NA },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetpshared_Post_Status_Inval,
    CPthreadReqMutexattrGetpshared_Post_Pshared_NA,
    CPthreadReqMutexattrGetpshared_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetpshared_Post_Status_Inval,
    CPthreadReqMutexattrGetpshared_Post_Pshared_Nop,
    CPthreadReqMutexattrGetpshared_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetpshared_Post_Status_Ok,
    CPthreadReqMutexattrGetpshared_Post_Pshared_Private,
    CPthreadReqMutexattrGetpshared_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetpshared_Post_Status_Ok,
    CPthreadReqMutexattrGetpshared_Post_Pshared_Shared,
    CPthreadReqMutexattrGetpshared_Post_Attr_Nop },
  { 0, 0, 1, 1, 0, CPthreadReqMutexattrGetpshared_Post_Status_Inval,
    CPthreadReqMutexattrGetpshared_Post_Pshared_Nop,
    CPthreadReqMutexattrGetpshared_Post_Attr_NA },
  { 0, 0, 1, 1, 0, CPthreadReqMutexattrGetpshared_Post_Status_Inval,
    CPthreadReqMutexattrGetpshared_Post_Pshared_NA,
    CPthreadReqMutexattrGetpshared_Post_Attr_NA }
};

static const uint8_t
CPthreadReqMutexattrGetpshared_Map[] = {
  3, 1, 4, 1, 2, 1, 2, 1, 5, 6, 0, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t CPthreadReqMutexattrGetpshared_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexattrGetpshared_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexattrGetpshared_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexattrGetpshared_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexattrGetpshared_Scope,
  .initial_context = &CPthreadReqMutexattrGetpshared_Instance
};

static inline CPthreadReqMutexattrGetpshared_Entry
CPthreadReqMutexattrGetpshared_PopEntry(
  CPthreadReqMutexattrGetpshared_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexattrGetpshared_Entries
    [ CPthreadReqMutexattrGetpshared_Map[ index ] ];
}

static void CPthreadReqMutexattrGetpshared_SetPreConditionStates(
  CPthreadReqMutexattrGetpshared_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Initialized_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexattrGetpshared_Pre_Initialized_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_StoredPshared_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqMutexattrGetpshared_Pre_StoredPshared_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
}

static void CPthreadReqMutexattrGetpshared_TestVariant(
  CPthreadReqMutexattrGetpshared_Context *ctx
)
{
  CPthreadReqMutexattrGetpshared_Pre_Attr_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexattrGetpshared_Pre_Initialized_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexattrGetpshared_Pre_StoredPshared_Prepare(
    ctx,
    ctx->Map.pcs[ 2 ]
  );
  CPthreadReqMutexattrGetpshared_Pre_Pshared_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqMutexattrGetpshared_Action( ctx );
  CPthreadReqMutexattrGetpshared_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexattrGetpshared_Post_Pshared_Check(
    ctx,
    ctx->Map.entry.Post_Pshared
  );
  CPthreadReqMutexattrGetpshared_Post_Attr_Check(
    ctx,
    ctx->Map.entry.Post_Attr
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexattrGetpshared( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexattrGetpshared,
  &CPthreadReqMutexattrGetpshared_Fixture
)
{
  CPthreadReqMutexattrGetpshared_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexattrGetpshared_Pre_Attr_Obj;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexattrGetpshared_Pre_Attr_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexattrGetpshared_Pre_Initialized_Yes;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexattrGetpshared_Pre_Initialized_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] =
          CPthreadReqMutexattrGetpshared_Pre_StoredPshared_Private;
        ctx->Map.pci[ 2 ] <
        CPthreadReqMutexattrGetpshared_Pre_StoredPshared_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexattrGetpshared_Pre_Pshared_Obj;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexattrGetpshared_Pre_Pshared_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = CPthreadReqMutexattrGetpshared_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          CPthreadReqMutexattrGetpshared_SetPreConditionStates( ctx );
          CPthreadReqMutexattrGetpshared_TestVariant( ctx );
        }
      }
    }
  }
}

/** @} */
