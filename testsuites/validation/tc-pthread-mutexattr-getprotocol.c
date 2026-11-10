/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexattrGetprotocol
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
 * @defgroup CPthreadReqMutexattrGetprotocol \
 *   spec:/c/pthread/req/mutexattr-getprotocol
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexattrGetprotocol_Pre_Attr_Obj,
  CPthreadReqMutexattrGetprotocol_Pre_Attr_Null,
  CPthreadReqMutexattrGetprotocol_Pre_Attr_NA
} CPthreadReqMutexattrGetprotocol_Pre_Attr;

typedef enum {
  CPthreadReqMutexattrGetprotocol_Pre_Initialized_Yes,
  CPthreadReqMutexattrGetprotocol_Pre_Initialized_No,
  CPthreadReqMutexattrGetprotocol_Pre_Initialized_NA
} CPthreadReqMutexattrGetprotocol_Pre_Initialized;

typedef enum {
  CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_None,
  CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_Inherit,
  CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_Protect,
  CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_NA
} CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol;

typedef enum {
  CPthreadReqMutexattrGetprotocol_Pre_Protocol_Obj,
  CPthreadReqMutexattrGetprotocol_Pre_Protocol_Null,
  CPthreadReqMutexattrGetprotocol_Pre_Protocol_NA
} CPthreadReqMutexattrGetprotocol_Pre_Protocol;

typedef enum {
  CPthreadReqMutexattrGetprotocol_Post_Status_Ok,
  CPthreadReqMutexattrGetprotocol_Post_Status_Inval,
  CPthreadReqMutexattrGetprotocol_Post_Status_NA
} CPthreadReqMutexattrGetprotocol_Post_Status;

typedef enum {
  CPthreadReqMutexattrGetprotocol_Post_Protocol_None,
  CPthreadReqMutexattrGetprotocol_Post_Protocol_Inherit,
  CPthreadReqMutexattrGetprotocol_Post_Protocol_Protect,
  CPthreadReqMutexattrGetprotocol_Post_Protocol_Nop,
  CPthreadReqMutexattrGetprotocol_Post_Protocol_NA
} CPthreadReqMutexattrGetprotocol_Post_Protocol;

typedef enum {
  CPthreadReqMutexattrGetprotocol_Post_Attr_Nop,
  CPthreadReqMutexattrGetprotocol_Post_Attr_NA
} CPthreadReqMutexattrGetprotocol_Post_Attr;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Attr_NA : 1;
  uint16_t Pre_Initialized_NA : 1;
  uint16_t Pre_StoredProtocol_NA : 1;
  uint16_t Pre_Protocol_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Protocol : 3;
  uint16_t Post_Attr : 1;
} CPthreadReqMutexattrGetprotocol_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutexattr-getprotocol test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `attr` parameter value.
   */
  const pthread_mutexattr_t *attr;

  /**
   * @brief This member specifies the `protocol` parameter value.
   */
  int *protocol;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutexattr_getprotocol() call.
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
   * @brief This member contains the object referenced by the `protocol`
   *   parameter.
   */
  int protocol_obj;

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
    CPthreadReqMutexattrGetprotocol_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexattrGetprotocol_Context;

static CPthreadReqMutexattrGetprotocol_Context
  CPthreadReqMutexattrGetprotocol_Instance;

static const char *const CPthreadReqMutexattrGetprotocol_PreDesc_Attr[] =
  { "Obj", "Null", "NA" };

static const char *const
  CPthreadReqMutexattrGetprotocol_PreDesc_Initialized[] =
    { "Yes", "No", "NA" };

static const char *const
  CPthreadReqMutexattrGetprotocol_PreDesc_StoredProtocol[] =
    { "None", "Inherit", "Protect", "NA" };

static const char *const CPthreadReqMutexattrGetprotocol_PreDesc_Protocol[] =
  { "Obj", "Null", "NA" };

static const char *const *const CPthreadReqMutexattrGetprotocol_PreDesc[] = {
  CPthreadReqMutexattrGetprotocol_PreDesc_Attr,
  CPthreadReqMutexattrGetprotocol_PreDesc_Initialized,
  CPthreadReqMutexattrGetprotocol_PreDesc_StoredProtocol,
  CPthreadReqMutexattrGetprotocol_PreDesc_Protocol,
  NULL
};

#define POISON -1

static void CPthreadReqMutexattrGetprotocol_Pre_Attr_Prepare(
  CPthreadReqMutexattrGetprotocol_Context *ctx,
  CPthreadReqMutexattrGetprotocol_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprotocol_Pre_Attr_Obj: {
      /*
       * While the `attr` parameter references an object of type
       * pthread_mutexattr_t.
       */
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Pre_Attr_Null: {
      /*
       * While the `attr` parameter is NULL.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprotocol_Pre_Initialized_Prepare(
  CPthreadReqMutexattrGetprotocol_Context        *ctx,
  CPthreadReqMutexattrGetprotocol_Pre_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprotocol_Pre_Initialized_Yes: {
      /*
       * While the object referenced by the `attr` parameter is an initialized
       * mutex attributes object.
       */
      T_eno_success( pthread_mutexattr_init( &ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Pre_Initialized_No: {
      /*
       * While the object referenced by the `attr` parameter is not an
       * initialized mutex attributes object.
       */
      memset( &ctx->attr_obj, 0, sizeof( ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Pre_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_Prepare(
  CPthreadReqMutexattrGetprotocol_Context           *ctx,
  CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_None: {
      /*
       * While the mutex protocol attribute of the object referenced by the
       * `attr` parameter is PTHREAD_PRIO_NONE.
       */
      ctx->attr_obj.protocol = PTHREAD_PRIO_NONE;
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_Inherit: {
      /*
       * While the mutex protocol attribute of the object referenced by the
       * `attr` parameter is PTHREAD_PRIO_INHERIT.
       */
      ctx->attr_obj.protocol = PTHREAD_PRIO_INHERIT;
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_Protect: {
      /*
       * While the mutex protocol attribute of the object referenced by the
       * `attr` parameter is PTHREAD_PRIO_PROTECT.
       */
      ctx->attr_obj.protocol = PTHREAD_PRIO_PROTECT;
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprotocol_Pre_Protocol_Prepare(
  CPthreadReqMutexattrGetprotocol_Context     *ctx,
  CPthreadReqMutexattrGetprotocol_Pre_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprotocol_Pre_Protocol_Obj: {
      /*
       * While the `protocol` parameter references an object of type int.
       */
      ctx->protocol_obj = POISON;
      ctx->protocol = &ctx->protocol_obj;
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Pre_Protocol_Null: {
      /*
       * While the `protocol` parameter is NULL.
       */
      ctx->protocol = NULL;
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Pre_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprotocol_Post_Status_Check(
  CPthreadReqMutexattrGetprotocol_Context    *ctx,
  CPthreadReqMutexattrGetprotocol_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprotocol_Post_Status_Ok: {
      /*
       * The return value of pthread_mutexattr_getprotocol() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Post_Status_Inval: {
      /*
       * The return value of pthread_mutexattr_getprotocol() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprotocol_Post_Protocol_Check(
  CPthreadReqMutexattrGetprotocol_Context      *ctx,
  CPthreadReqMutexattrGetprotocol_Post_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprotocol_Post_Protocol_None: {
      /*
       * The object referenced by the `protocol` parameter shall be
       * PTHREAD_PRIO_NONE.
       */
      T_eq_int( ctx->protocol_obj, PTHREAD_PRIO_NONE );
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Post_Protocol_Inherit: {
      /*
       * The object referenced by the `protocol` parameter shall be
       * PTHREAD_PRIO_INHERIT.
       */
      T_eq_int( ctx->protocol_obj, PTHREAD_PRIO_INHERIT );
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Post_Protocol_Protect: {
      /*
       * The object referenced by the `protocol` parameter shall be
       * PTHREAD_PRIO_PROTECT.
       */
      T_eq_int( ctx->protocol_obj, PTHREAD_PRIO_PROTECT );
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Post_Protocol_Nop: {
      /*
       * The object referenced by the `protocol` parameter shall not be
       * modified.
       */
      T_eq_int( ctx->protocol_obj, POISON );
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Post_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprotocol_Post_Attr_Check(
  CPthreadReqMutexattrGetprotocol_Context  *ctx,
  CPthreadReqMutexattrGetprotocol_Post_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrGetprotocol_Post_Attr_Nop: {
      /*
       * The object referenced by the `attr` parameter shall not be modified.
       */
      T_eq_mem( &ctx->attr_obj, &ctx->attr_before, sizeof( ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexattrGetprotocol_Post_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrGetprotocol_Action(
  CPthreadReqMutexattrGetprotocol_Context *ctx
)
{
  memcpy( &ctx->attr_before, &ctx->attr_obj, sizeof( ctx->attr_before ) );
  ctx->status = pthread_mutexattr_getprotocol( ctx->attr, ctx->protocol );
}

/* clang-format off */

static const CPthreadReqMutexattrGetprotocol_Entry
CPthreadReqMutexattrGetprotocol_Entries[] = {
  { 1, 0, 0, 0, 0, CPthreadReqMutexattrGetprotocol_Post_Status_NA,
    CPthreadReqMutexattrGetprotocol_Post_Protocol_NA,
    CPthreadReqMutexattrGetprotocol_Post_Attr_NA },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetprotocol_Post_Status_Inval,
    CPthreadReqMutexattrGetprotocol_Post_Protocol_NA,
    CPthreadReqMutexattrGetprotocol_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetprotocol_Post_Status_Inval,
    CPthreadReqMutexattrGetprotocol_Post_Protocol_Nop,
    CPthreadReqMutexattrGetprotocol_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetprotocol_Post_Status_Ok,
    CPthreadReqMutexattrGetprotocol_Post_Protocol_None,
    CPthreadReqMutexattrGetprotocol_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetprotocol_Post_Status_Ok,
    CPthreadReqMutexattrGetprotocol_Post_Protocol_Inherit,
    CPthreadReqMutexattrGetprotocol_Post_Attr_Nop },
  { 0, 0, 0, 0, 0, CPthreadReqMutexattrGetprotocol_Post_Status_Ok,
    CPthreadReqMutexattrGetprotocol_Post_Protocol_Protect,
    CPthreadReqMutexattrGetprotocol_Post_Attr_Nop },
  { 0, 0, 1, 1, 0, CPthreadReqMutexattrGetprotocol_Post_Status_Inval,
    CPthreadReqMutexattrGetprotocol_Post_Protocol_Nop,
    CPthreadReqMutexattrGetprotocol_Post_Attr_NA },
  { 0, 0, 1, 1, 0, CPthreadReqMutexattrGetprotocol_Post_Status_Inval,
    CPthreadReqMutexattrGetprotocol_Post_Protocol_NA,
    CPthreadReqMutexattrGetprotocol_Post_Attr_NA }
};

static const uint8_t
CPthreadReqMutexattrGetprotocol_Map[] = {
  3, 1, 4, 1, 5, 1, 2, 1, 2, 1, 2, 1, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t CPthreadReqMutexattrGetprotocol_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexattrGetprotocol_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexattrGetprotocol_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexattrGetprotocol_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexattrGetprotocol_Scope,
  .initial_context = &CPthreadReqMutexattrGetprotocol_Instance
};

static inline CPthreadReqMutexattrGetprotocol_Entry
CPthreadReqMutexattrGetprotocol_PopEntry(
  CPthreadReqMutexattrGetprotocol_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexattrGetprotocol_Entries
    [ CPthreadReqMutexattrGetprotocol_Map[ index ] ];
}

static void CPthreadReqMutexattrGetprotocol_SetPreConditionStates(
  CPthreadReqMutexattrGetprotocol_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Initialized_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexattrGetprotocol_Pre_Initialized_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_StoredProtocol_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
}

static void CPthreadReqMutexattrGetprotocol_TestVariant(
  CPthreadReqMutexattrGetprotocol_Context *ctx
)
{
  CPthreadReqMutexattrGetprotocol_Pre_Attr_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexattrGetprotocol_Pre_Initialized_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_Prepare(
    ctx,
    ctx->Map.pcs[ 2 ]
  );
  CPthreadReqMutexattrGetprotocol_Pre_Protocol_Prepare(
    ctx,
    ctx->Map.pcs[ 3 ]
  );
  CPthreadReqMutexattrGetprotocol_Action( ctx );
  CPthreadReqMutexattrGetprotocol_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexattrGetprotocol_Post_Protocol_Check(
    ctx,
    ctx->Map.entry.Post_Protocol
  );
  CPthreadReqMutexattrGetprotocol_Post_Attr_Check(
    ctx,
    ctx->Map.entry.Post_Attr
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexattrGetprotocol( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexattrGetprotocol,
  &CPthreadReqMutexattrGetprotocol_Fixture
)
{
  CPthreadReqMutexattrGetprotocol_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexattrGetprotocol_Pre_Attr_Obj;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexattrGetprotocol_Pre_Attr_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexattrGetprotocol_Pre_Initialized_Yes;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexattrGetprotocol_Pre_Initialized_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] =
          CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_None;
        ctx->Map.pci[ 2 ] <
        CPthreadReqMutexattrGetprotocol_Pre_StoredProtocol_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexattrGetprotocol_Pre_Protocol_Obj;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexattrGetprotocol_Pre_Protocol_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = CPthreadReqMutexattrGetprotocol_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          CPthreadReqMutexattrGetprotocol_SetPreConditionStates( ctx );
          CPthreadReqMutexattrGetprotocol_TestVariant( ctx );
        }
      }
    }
  }
}

/** @} */
