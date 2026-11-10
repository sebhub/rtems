/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexattrSetprioceiling
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
 * @defgroup CPthreadReqMutexattrSetprioceiling \
 *   spec:/c/pthread/req/mutexattr-setprioceiling
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexattrSetprioceiling_Pre_Attr_Obj,
  CPthreadReqMutexattrSetprioceiling_Pre_Attr_Null,
  CPthreadReqMutexattrSetprioceiling_Pre_Attr_NA
} CPthreadReqMutexattrSetprioceiling_Pre_Attr;

typedef enum {
  CPthreadReqMutexattrSetprioceiling_Pre_Initialized_Yes,
  CPthreadReqMutexattrSetprioceiling_Pre_Initialized_No,
  CPthreadReqMutexattrSetprioceiling_Pre_Initialized_NA
} CPthreadReqMutexattrSetprioceiling_Pre_Initialized;

typedef enum {
  CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_Minimum,
  CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_Maximum,
  CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_OutOfRange,
  CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_NA
} CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling;

typedef enum {
  CPthreadReqMutexattrSetprioceiling_Post_Status_Ok,
  CPthreadReqMutexattrSetprioceiling_Post_Status_Inval,
  CPthreadReqMutexattrSetprioceiling_Post_Status_NA
} CPthreadReqMutexattrSetprioceiling_Post_Status;

typedef enum {
  CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Minimum,
  CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Maximum,
  CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_OutOfRange,
  CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Nop,
  CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_NA
} CPthreadReqMutexattrSetprioceiling_Post_Prioceiling;

typedef enum {
  CPthreadReqMutexattrSetprioceiling_Post_Other_Nop,
  CPthreadReqMutexattrSetprioceiling_Post_Other_NA
} CPthreadReqMutexattrSetprioceiling_Post_Other;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Attr_NA : 1;
  uint16_t Pre_Initialized_NA : 1;
  uint16_t Pre_Prioceiling_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Prioceiling : 3;
  uint16_t Post_Other : 1;
} CPthreadReqMutexattrSetprioceiling_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutexattr-setprioceiling test
 *   case.
 */
typedef struct {
  /**
   * @brief This member specifies the `attr` parameter value.
   */
  pthread_mutexattr_t *attr;

  /**
   * @brief This member specifies the `prioceiling` parameter value.
   */
  int prio_ceiling;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutexattr_setprioceiling() call.
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
    CPthreadReqMutexattrSetprioceiling_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexattrSetprioceiling_Context;

static CPthreadReqMutexattrSetprioceiling_Context
  CPthreadReqMutexattrSetprioceiling_Instance;

static const char *const CPthreadReqMutexattrSetprioceiling_PreDesc_Attr[] =
  { "Obj", "Null", "NA" };

static const char *const
  CPthreadReqMutexattrSetprioceiling_PreDesc_Initialized[] =
    { "Yes", "No", "NA" };

static const char *const
  CPthreadReqMutexattrSetprioceiling_PreDesc_Prioceiling[] =
    { "Minimum", "Maximum", "OutOfRange", "NA" };

static const char *const *const CPthreadReqMutexattrSetprioceiling_PreDesc[] =
  { CPthreadReqMutexattrSetprioceiling_PreDesc_Attr,
    CPthreadReqMutexattrSetprioceiling_PreDesc_Initialized,
    CPthreadReqMutexattrSetprioceiling_PreDesc_Prioceiling,
    NULL };

static void CPthreadReqMutexattrSetprioceiling_Pre_Attr_Prepare(
  CPthreadReqMutexattrSetprioceiling_Context *ctx,
  CPthreadReqMutexattrSetprioceiling_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprioceiling_Pre_Attr_Obj: {
      /*
       * While the `attr` parameter references an object of type
       * pthread_mutexattr_t.
       */
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Pre_Attr_Null: {
      /*
       * While the `attr` parameter is NULL.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprioceiling_Pre_Initialized_Prepare(
  CPthreadReqMutexattrSetprioceiling_Context        *ctx,
  CPthreadReqMutexattrSetprioceiling_Pre_Initialized state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprioceiling_Pre_Initialized_Yes: {
      /*
       * While the object referenced by the `attr` parameter is an initialized
       * mutex attributes object.
       */
      T_eno_success( pthread_mutexattr_init( &ctx->attr_obj ) );
      ctx->attr_obj.prio_ceiling = INT_MIN;
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Pre_Initialized_No: {
      /*
       * While the object referenced by the `attr` parameter is not an
       * initialized mutex attributes object.
       */
      memset( &ctx->attr_obj, 0, sizeof( ctx->attr_obj ) );
      ctx->attr_obj.prio_ceiling = INT_MIN;
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Pre_Initialized_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_Prepare(
  CPthreadReqMutexattrSetprioceiling_Context        *ctx,
  CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_Minimum: {
      /*
       * While the `prioceiling` parameter is equal to the minimum thread
       * priority of the SCHED_FIFO scheduling policy.
       */
      ctx->prio_ceiling = GetPOSIXPriorityMinimum();
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_Maximum: {
      /*
       * While the `prioceiling` parameter is equal to the maximum thread
       * priority of the SCHED_FIFO scheduling policy.
       */
      ctx->prio_ceiling = GetPOSIXPriorityMaximum();
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_OutOfRange: {
      /*
       * While the `prioceiling` parameter is equal to the maximum thread
       * priority of the SCHED_FIFO scheduling policy plus one.
       */
      ctx->prio_ceiling = GetPOSIXPriorityMaximum() + 1;
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprioceiling_Post_Status_Check(
  CPthreadReqMutexattrSetprioceiling_Context    *ctx,
  CPthreadReqMutexattrSetprioceiling_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprioceiling_Post_Status_Ok: {
      /*
       * The return value of pthread_mutexattr_setprioceiling() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Post_Status_Inval: {
      /*
       * The return value of pthread_mutexattr_setprioceiling() shall be
       * EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Check(
  CPthreadReqMutexattrSetprioceiling_Context         *ctx,
  CPthreadReqMutexattrSetprioceiling_Post_Prioceiling state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Minimum: {
      /*
       * The mutex priority ceiling attribute of the mutex attributes object
       * referenced by the `attr` parameter shall be the minimum thread
       * priority of the SCHED_FIFO scheduling policy.
       */
      T_eq_int( ctx->attr_obj.prio_ceiling, GetPOSIXPriorityMinimum() );
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Maximum: {
      /*
       * The mutex priority ceiling attribute of the mutex attributes object
       * referenced by the `attr` parameter shall be the maximum thread
       * priority of the SCHED_FIFO scheduling policy.
       */
      T_eq_int( ctx->attr_obj.prio_ceiling, GetPOSIXPriorityMaximum() );
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_OutOfRange: {
      /*
       * The mutex priority ceiling attribute of the mutex attributes object
       * referenced by the `attr` parameter shall be the maximum thread
       * priority of the SCHED_FIFO scheduling policy plus one.
       */
      T_eq_int( ctx->attr_obj.prio_ceiling, GetPOSIXPriorityMaximum() + 1 );
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Nop: {
      /*
       * The mutex priority ceiling attribute of the mutex attributes object
       * referenced by the `attr` parameter shall not be modified.
       */
      T_eq_int( ctx->attr_obj.prio_ceiling, ctx->attr_before.prio_ceiling );
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprioceiling_Post_Other_Check(
  CPthreadReqMutexattrSetprioceiling_Context   *ctx,
  CPthreadReqMutexattrSetprioceiling_Post_Other state
)
{
  switch ( state ) {
    case CPthreadReqMutexattrSetprioceiling_Post_Other_Nop: {
      /*
       * The content of the mutex attributes object referenced by the `attr`
       * parameter, other than its mutex priority ceiling attribute, shall not
       * be modified.
       */
      pthread_mutexattr_t expected;

      memcpy( &expected, &ctx->attr_before, sizeof( expected ) );
      expected.prio_ceiling = ctx->attr_obj.prio_ceiling;
      T_eq_mem( &ctx->attr_obj, &expected, sizeof( expected ) );
      break;
    }

    case CPthreadReqMutexattrSetprioceiling_Post_Other_NA:
      break;
  }
}

static void CPthreadReqMutexattrSetprioceiling_Action(
  CPthreadReqMutexattrSetprioceiling_Context *ctx
)
{
  memcpy( &ctx->attr_before, &ctx->attr_obj, sizeof( ctx->attr_before ) );
  ctx->status = pthread_mutexattr_setprioceiling(
    ctx->attr,
    ctx->prio_ceiling
  );
}

/* clang-format off */

static const CPthreadReqMutexattrSetprioceiling_Entry
CPthreadReqMutexattrSetprioceiling_Entries[] = {
  { 0, 0, 0, 0, CPthreadReqMutexattrSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Nop,
    CPthreadReqMutexattrSetprioceiling_Post_Other_Nop },
  { 0, 0, 1, 0, CPthreadReqMutexattrSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_NA,
    CPthreadReqMutexattrSetprioceiling_Post_Other_NA },
  { 1, 0, 0, 0, CPthreadReqMutexattrSetprioceiling_Post_Status_NA,
    CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_NA,
    CPthreadReqMutexattrSetprioceiling_Post_Other_NA },
  { 0, 0, 0, 0, CPthreadReqMutexattrSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Minimum,
    CPthreadReqMutexattrSetprioceiling_Post_Other_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexattrSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Maximum,
    CPthreadReqMutexattrSetprioceiling_Post_Other_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexattrSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_OutOfRange,
    CPthreadReqMutexattrSetprioceiling_Post_Other_Nop }
};

static const uint8_t
CPthreadReqMutexattrSetprioceiling_Map[] = {
  3, 4, 5, 0, 0, 0, 1, 1, 1, 2, 2, 2
};

/* clang-format on */

static size_t CPthreadReqMutexattrSetprioceiling_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexattrSetprioceiling_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexattrSetprioceiling_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexattrSetprioceiling_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexattrSetprioceiling_Scope,
  .initial_context = &CPthreadReqMutexattrSetprioceiling_Instance
};

static inline CPthreadReqMutexattrSetprioceiling_Entry
CPthreadReqMutexattrSetprioceiling_PopEntry(
  CPthreadReqMutexattrSetprioceiling_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexattrSetprioceiling_Entries
    [ CPthreadReqMutexattrSetprioceiling_Map[ index ] ];
}

static void CPthreadReqMutexattrSetprioceiling_SetPreConditionStates(
  CPthreadReqMutexattrSetprioceiling_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Initialized_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexattrSetprioceiling_Pre_Initialized_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
}

static void CPthreadReqMutexattrSetprioceiling_TestVariant(
  CPthreadReqMutexattrSetprioceiling_Context *ctx
)
{
  CPthreadReqMutexattrSetprioceiling_Pre_Attr_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  CPthreadReqMutexattrSetprioceiling_Pre_Initialized_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_Prepare(
    ctx,
    ctx->Map.pcs[ 2 ]
  );
  CPthreadReqMutexattrSetprioceiling_Action( ctx );
  CPthreadReqMutexattrSetprioceiling_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexattrSetprioceiling_Post_Prioceiling_Check(
    ctx,
    ctx->Map.entry.Post_Prioceiling
  );
  CPthreadReqMutexattrSetprioceiling_Post_Other_Check(
    ctx,
    ctx->Map.entry.Post_Other
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexattrSetprioceiling( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexattrSetprioceiling,
  &CPthreadReqMutexattrSetprioceiling_Fixture
)
{
  CPthreadReqMutexattrSetprioceiling_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexattrSetprioceiling_Pre_Attr_Obj;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexattrSetprioceiling_Pre_Attr_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] =
        CPthreadReqMutexattrSetprioceiling_Pre_Initialized_Yes;
      ctx->Map.pci[ 1 ] <
      CPthreadReqMutexattrSetprioceiling_Pre_Initialized_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] =
          CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_Minimum;
        ctx->Map.pci[ 2 ] <
        CPthreadReqMutexattrSetprioceiling_Pre_Prioceiling_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        ctx->Map.entry = CPthreadReqMutexattrSetprioceiling_PopEntry( ctx );

        if ( ctx->Map.entry.Skip ) {
          continue;
        }

        CPthreadReqMutexattrSetprioceiling_SetPreConditionStates( ctx );
        CPthreadReqMutexattrSetprioceiling_TestVariant( ctx );
      }
    }
  }
}

/** @} */
