/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexattrSetprotocol
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
 * @defgroup CPthreadReqMutexattrSetprotocol \
 *   spec:/c/pthread/req/mutexattr-setprotocol
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexattrSetprotocol_Pre_Attr_Obj,
  CPthreadReqMutexattrSetprotocol_Pre_Attr_Null,
  CPthreadReqMutexattrSetprotocol_Pre_Attr_NA
} CPthreadReqMutexattrSetprotocol_Pre_Attr;

typedef enum {
  CPthreadReqMutexattrSetprotocol_Pre_Initialized_Yes,
  CPthreadReqMutexattrSetprotocol_Pre_Initialized_No,
  CPthreadReqMutexattrSetprotocol_Pre_Initialized_NA
} CPthreadReqMutexattrSetprotocol_Pre_Initialized;

typedef enum {
  CPthreadReqMutexattrSetprotocol_Pre_Protocol_None,
  CPthreadReqMutexattrSetprotocol_Pre_Protocol_Inherit,
  CPthreadReqMutexattrSetprotocol_Pre_Protocol_Protect,
  CPthreadReqMutexattrSetprotocol_Pre_Protocol_Invalid,
  CPthreadReqMutexattrSetprotocol_Pre_Protocol_NA
} CPthreadReqMutexattrSetprotocol_Pre_Protocol;

typedef enum {
  CPthreadReqMutexattrSetprotocol_Post_Status_Ok,
  CPthreadReqMutexattrSetprotocol_Post_Status_Inval,
  CPthreadReqMutexattrSetprotocol_Post_Status_NA
} CPthreadReqMutexattrSetprotocol_Post_Status;

typedef enum {
  CPthreadReqMutexattrSetprotocol_Post_Protocol_None,
  CPthreadReqMutexattrSetprotocol_Post_Protocol_Inherit,
  CPthreadReqMutexattrSetprotocol_Post_Protocol_Protect,
  CPthreadReqMutexattrSetprotocol_Post_Protocol_Nop,
  CPthreadReqMutexattrSetprotocol_Post_Protocol_NA
} CPthreadReqMutexattrSetprotocol_Post_Protocol;

typedef enum {
  CPthreadReqMutexattrSetprotocol_Post_Other_Nop,
  CPthreadReqMutexattrSetprotocol_Post_Other_NA
} CPthreadReqMutexattrSetprotocol_Post_Other;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Attr_NA : 1;
  uint16_t Pre_Initialized_NA : 1;
  uint16_t Pre_Protocol_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Protocol : 3;
  uint16_t Post_Other : 1;
} CPthreadReqMutexattrSetprotocol_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutexattr-setprotocol test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `attr` parameter value.
   */
  pthread_mutexattr_t *attr;

  /**
   * @brief This member specifies the `protocol` parameter value.
   */
  int protocol;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutexattr_setprotocol() call.
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
    CPthreadReqMutexattrSetprotocol_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexattrSetprotocol_Context;

static CPthreadReqMutexattrSetprotocol_Context
  CPthreadReqMutexattrSetprotocol_Instance;

static const char *const CPthreadReqMutexattrSetprotocol_PreDesc_Attr[] =
  { "Obj", "Null", "NA" };

static const char *const
  CPthreadReqMutexattrSetprotocol_PreDesc_Initialized[] =
    { "Yes", "No", "NA" };

static const char *const CPthreadReqMutexattrSetprotocol_PreDesc_Protocol[] =
  { "None", "Inherit", "Protect", "Invalid", "NA" };

static const char *const *const CPthreadReqMutexattrSetprotocol_PreDesc[] = {
  CPthreadReqMutexattrSetprotocol_PreDesc_Attr,
  CPthreadReqMutexattrSetprotocol_PreDesc_Initialized,
  CPthreadReqMutexattrSetprotocol_PreDesc_Protocol,
  NULL
};

#define PROTOCOL_INVALID 123

static void CPthreadReqMutexattrSetprotocol_Pre_Attr_Prepare(
  CPthreadReqMutexattrSetprotocol_Context *ctx,
  CPthreadReqMutexattrSetprotocol_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprotocol_Pre_Attr_Obj: {
      /*
       * While the `attr` parameter references an object of type
       * pthread_mutexattr_t.
       */
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Pre_Attr_Null: {
      /*
       * While the `attr` parameter is NULL.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprotocol_Pre_Initialized_Prepare(
  CPthreadReqMutexattrSetprotocol_Context        *ctx,
  CPthreadReqMutexattrSetprotocol_Pre_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprotocol_Pre_Initialized_Yes: {
      /*
       * While the object referenced by the `attr` parameter is an initialized
       * mutex attributes object.
       */
      T_eno_success( pthread_mutexattr_init( &ctx->attr_obj ) );
      ctx->attr_obj.protocol = -1;
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Pre_Initialized_No: {
      /*
       * While the object referenced by the `attr` parameter is not an
       * initialized mutex attributes object.
       */
      memset( &ctx->attr_obj, 0, sizeof( ctx->attr_obj ) );
      ctx->attr_obj.protocol = -1;
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Pre_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprotocol_Pre_Protocol_Prepare(
  CPthreadReqMutexattrSetprotocol_Context     *ctx,
  CPthreadReqMutexattrSetprotocol_Pre_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprotocol_Pre_Protocol_None: {
      /*
       * While the `protocol` parameter is equal to PTHREAD_PRIO_NONE.
       */
      ctx->protocol = PTHREAD_PRIO_NONE;
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Pre_Protocol_Inherit: {
      /*
       * While the `protocol` parameter is equal to PTHREAD_PRIO_INHERIT.
       */
      ctx->protocol = PTHREAD_PRIO_INHERIT;
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Pre_Protocol_Protect: {
      /*
       * While the `protocol` parameter is equal to PTHREAD_PRIO_PROTECT.
       */
      ctx->protocol = PTHREAD_PRIO_PROTECT;
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Pre_Protocol_Invalid: {
      /*
       * While the `protocol` parameter is not a valid mutex protocol
       * attribute.
       */
      ctx->protocol = PROTOCOL_INVALID;
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Pre_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprotocol_Post_Status_Check(
  CPthreadReqMutexattrSetprotocol_Context    *ctx,
  CPthreadReqMutexattrSetprotocol_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprotocol_Post_Status_Ok: {
      /*
       * The return value of pthread_mutexattr_setprotocol() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Post_Status_Inval: {
      /*
       * The return value of pthread_mutexattr_setprotocol() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprotocol_Post_Protocol_Check(
  CPthreadReqMutexattrSetprotocol_Context      *ctx,
  CPthreadReqMutexattrSetprotocol_Post_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprotocol_Post_Protocol_None: {
      /*
       * The mutex protocol attribute of the mutex attributes object referenced
       * by the `attr` parameter shall be PTHREAD_PRIO_NONE.
       */
      T_eq_int( ctx->attr_obj.protocol, PTHREAD_PRIO_NONE );
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Post_Protocol_Inherit: {
      /*
       * The mutex protocol attribute of the mutex attributes object referenced
       * by the `attr` parameter shall be PTHREAD_PRIO_INHERIT.
       */
      T_eq_int( ctx->attr_obj.protocol, PTHREAD_PRIO_INHERIT );
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Post_Protocol_Protect: {
      /*
       * The mutex protocol attribute of the mutex attributes object referenced
       * by the `attr` parameter shall be PTHREAD_PRIO_PROTECT.
       */
      T_eq_int( ctx->attr_obj.protocol, PTHREAD_PRIO_PROTECT );
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Post_Protocol_Nop: {
      /*
       * The mutex protocol attribute of the mutex attributes object referenced
       * by the `attr` parameter shall not be modified.
       */
      T_eq_int( ctx->attr_obj.protocol, ctx->attr_before.protocol );
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Post_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprotocol_Post_Other_Check(
  CPthreadReqMutexattrSetprotocol_Context   *ctx,
  CPthreadReqMutexattrSetprotocol_Post_Other state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprotocol_Post_Other_Nop: {
      /*
       * The content of the mutex attributes object referenced by the `attr`
       * parameter, other than its mutex protocol attribute, shall not be
       * modified.
       */
      pthread_mutexattr_t expected;

      memcpy( &expected, &ctx->attr_before, sizeof( expected ) );
      expected.protocol = ctx->attr_obj.protocol;
      T_eq_mem( &ctx->attr_obj, &expected, sizeof( expected ) );
      break;
    }

    case CPthreadReqMutexattrSetprotocol_Post_Other_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprotocol_Action(
  CPthreadReqMutexattrSetprotocol_Context *ctx
)
{
  memcpy( &ctx->attr_before, &ctx->attr_obj, sizeof( ctx->attr_before ) );
  ctx->status = pthread_mutexattr_setprotocol( ctx->attr, ctx->protocol );
}

/* clang-format off */

static const CPthreadReqMutexattrSetprotocol_Entry
CPthreadReqMutexattrSetprotocol_Entries[] = {
  { 0, 0, 0, 0, CPthreadReqMutexattrSetprotocol_Post_Status_Inval,
    CPthreadReqMutexattrSetprotocol_Post_Protocol_Nop,
    CPthreadReqMutexattrSetprotocol_Post_Other_Nop },
  { 0, 0, 1, 0, CPthreadReqMutexattrSetprotocol_Post_Status_Inval,
    CPthreadReqMutexattrSetprotocol_Post_Protocol_NA,
    CPthreadReqMutexattrSetprotocol_Post_Other_NA },
  { 1, 0, 0, 0, CPthreadReqMutexattrSetprotocol_Post_Status_NA,
    CPthreadReqMutexattrSetprotocol_Post_Protocol_NA,
    CPthreadReqMutexattrSetprotocol_Post_Other_NA },
  { 0, 0, 0, 0, CPthreadReqMutexattrSetprotocol_Post_Status_Ok,
    CPthreadReqMutexattrSetprotocol_Post_Protocol_None,
    CPthreadReqMutexattrSetprotocol_Post_Other_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexattrSetprotocol_Post_Status_Ok,
    CPthreadReqMutexattrSetprotocol_Post_Protocol_Inherit,
    CPthreadReqMutexattrSetprotocol_Post_Other_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexattrSetprotocol_Post_Status_Ok,
    CPthreadReqMutexattrSetprotocol_Post_Protocol_Protect,
    CPthreadReqMutexattrSetprotocol_Post_Other_Nop }
};

static const uint8_t
CPthreadReqMutexattrSetprotocol_Map[] = {
  3, 4, 5, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2
};

/* clang-format on */

static size_t CPthreadReqMutexattrSetprotocol_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexattrSetprotocol_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexattrSetprotocol_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexattrSetprotocol_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexattrSetprotocol_Scope,
  .initial_context = &CPthreadReqMutexattrSetprotocol_Instance
};

static inline CPthreadReqMutexattrSetprotocol_Entry
CPthreadReqMutexattrSetprotocol_PopEntry(
  CPthreadReqMutexattrSetprotocol_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexattrSetprotocol_Entries
    [ CPthreadReqMutexattrSetprotocol_Map[ index ] ];
}

static void CPthreadReqMutexattrSetprotocol_SetPreConditionStates(
  CPthreadReqMutexattrSetprotocol_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Initialized_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexattrSetprotocol_Pre_Initialized_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
}

static void CPthreadReqMutexattrSetprotocol_TestVariant(
  CPthreadReqMutexattrSetprotocol_Context *ctx
)
{
  CPthreadReqMutexattrSetprotocol_Pre_Attr_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexattrSetprotocol_Pre_Initialized_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexattrSetprotocol_Pre_Protocol_Prepare(
    ctx,
    ctx->Map.pcs[ 2 ]
  );
  CPthreadReqMutexattrSetprotocol_Action( ctx );
  CPthreadReqMutexattrSetprotocol_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexattrSetprotocol_Post_Protocol_Check(
    ctx,
    ctx->Map.entry.Post_Protocol
  );
  CPthreadReqMutexattrSetprotocol_Post_Other_Check(
    ctx,
    ctx->Map.entry.Post_Other
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexattrSetprotocol( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexattrSetprotocol,
  &CPthreadReqMutexattrSetprotocol_Fixture
)
{
  CPthreadReqMutexattrSetprotocol_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexattrSetprotocol_Pre_Attr_Obj;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexattrSetprotocol_Pre_Attr_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexattrSetprotocol_Pre_Initialized_Yes;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexattrSetprotocol_Pre_Initialized_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexattrSetprotocol_Pre_Protocol_None;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexattrSetprotocol_Pre_Protocol_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        ctx->Map.entry = CPthreadReqMutexattrSetprotocol_PopEntry( ctx );

        if ( ctx->Map.entry.Skip ) {
          continue;
        }

        CPthreadReqMutexattrSetprotocol_SetPreConditionStates( ctx );
        CPthreadReqMutexattrSetprotocol_TestVariant( ctx );
      }
    }
  }
}

/** @} */
