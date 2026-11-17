/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexTimedlockTimeout
 */

/*
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

#include <pthread.h>
#include <string.h>

#include "tr-tq-timeout-priority-inherit.h"
#include "tr-tq-timeout.h"
#include "tx-support.h"
#include "tx-thread-queue.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqMutexTimedlockTimeout \
 *   spec:/c/pthread/req/mutex-timedlock-timeout
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexTimedlockTimeout_Pre_Protocol_None,
  CPthreadReqMutexTimedlockTimeout_Pre_Protocol_Inherit,
  CPthreadReqMutexTimedlockTimeout_Pre_Protocol_Protect,
  CPthreadReqMutexTimedlockTimeout_Pre_Protocol_NA
} CPthreadReqMutexTimedlockTimeout_Pre_Protocol;

typedef enum {
  CPthreadReqMutexTimedlockTimeout_Post_Action_Timeout,
  CPthreadReqMutexTimedlockTimeout_Post_Action_TimeoutPriorityInherit,
  CPthreadReqMutexTimedlockTimeout_Post_Action_NA
} CPthreadReqMutexTimedlockTimeout_Post_Action;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Protocol_NA : 1;
  uint8_t Post_Action : 2;
} CPthreadReqMutexTimedlockTimeout_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutex-timedlock-timeout test
 *   case.
 */
typedef struct {
  /**
   * @brief This member contains the thread queue test context.
   */
  TQContext tq_ctx;

  /**
   * @brief This member contains the mutex.
   */
  pthread_mutex_t mutex;

  /**
   * @brief This member specifies the protocol attribute used to initialize the
   *   mutex.
   */
  int protocol;

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
    CPthreadReqMutexTimedlockTimeout_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexTimedlockTimeout_Context;

static CPthreadReqMutexTimedlockTimeout_Context
  CPthreadReqMutexTimedlockTimeout_Instance;

static const char *const CPthreadReqMutexTimedlockTimeout_PreDesc_Protocol[] =
  { "None", "Inherit", "Protect", "NA" };

static const char *const *const CPthreadReqMutexTimedlockTimeout_PreDesc[] = {
  CPthreadReqMutexTimedlockTimeout_PreDesc_Protocol,
  NULL
};

static void CPthreadReqMutexTimedlockTimeout_Pre_Protocol_Prepare(
  CPthreadReqMutexTimedlockTimeout_Context     *ctx,
  CPthreadReqMutexTimedlockTimeout_Pre_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlockTimeout_Pre_Protocol_None: {
      /*
       * While the mutex protocol attribute of the mutex is PTHREAD_PRIO_NONE.
       */
      ctx->protocol = PTHREAD_PRIO_NONE;
      ctx->tq_ctx.discipline = TQ_FIFO;
      break;
    }

    case CPthreadReqMutexTimedlockTimeout_Pre_Protocol_Inherit: {
      /*
       * While the mutex protocol attribute of the mutex is
       * PTHREAD_PRIO_INHERIT.
       */
      ctx->protocol = PTHREAD_PRIO_INHERIT;
      ctx->tq_ctx.discipline = TQ_PRIORITY;
      break;
    }

    case CPthreadReqMutexTimedlockTimeout_Pre_Protocol_Protect: {
      /*
       * While the mutex protocol attribute of the mutex is
       * PTHREAD_PRIO_PROTECT.
       */
      ctx->protocol = PTHREAD_PRIO_PROTECT;
      ctx->tq_ctx.discipline = TQ_PRIORITY;
      break;
    }

    case CPthreadReqMutexTimedlockTimeout_Pre_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlockTimeout_Post_Action_Check(
  CPthreadReqMutexTimedlockTimeout_Context    *ctx,
  CPthreadReqMutexTimedlockTimeout_Post_Action state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlockTimeout_Post_Action_Timeout: {
      /*
       * The timeout actions shall be done as specified by
       * spec:/score/tq/req/timeout.
       */
      ctx->tq_ctx.wait = TQ_WAIT_TIMED;
      ScoreTqReqTimeout_Run( &ctx->tq_ctx );
      break;
    }

    case CPthreadReqMutexTimedlockTimeout_Post_Action_TimeoutPriorityInherit: {
      /*
       * The timeout actions shall be done as specified by
       * spec:/score/tq/req/timeout-priority-inherit.
       */
      ctx->tq_ctx.wait = TQ_WAIT_FOREVER;
      ScoreTqReqTimeoutPriorityInherit_Run( &ctx->tq_ctx );
      break;
    }

    case CPthreadReqMutexTimedlockTimeout_Post_Action_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlockTimeout_Setup(
  CPthreadReqMutexTimedlockTimeout_Context *ctx
)
{
  memset( ctx, 0, sizeof( *ctx ) );
  ctx->tq_ctx.enqueue_variant = TQ_ENQUEUE_BLOCKS;
  ctx->tq_ctx.enqueue_prepare = TQEnqueuePrepareDefault;
  ctx->tq_ctx.enqueue_done = TQEnqueueDoneDefault;
  ctx->tq_ctx.enqueue = TQEnqueuePOSIXMutex;
  ctx->tq_ctx.surrender = TQSurrenderPOSIXMutex;
  ctx->tq_ctx.get_owner = TQGetOwnerPOSIXMutex;
  ctx->tq_ctx.convert_status = TQConvertStatusPOSIX;
  TQInitialize( &ctx->tq_ctx );
}

static void CPthreadReqMutexTimedlockTimeout_Setup_Wrap( void *arg )
{
  CPthreadReqMutexTimedlockTimeout_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexTimedlockTimeout_Setup( ctx );
}

static void CPthreadReqMutexTimedlockTimeout_Teardown(
  CPthreadReqMutexTimedlockTimeout_Context *ctx
)
{
  TQDestroy( &ctx->tq_ctx );
}

static void CPthreadReqMutexTimedlockTimeout_Teardown_Wrap( void *arg )
{
  CPthreadReqMutexTimedlockTimeout_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexTimedlockTimeout_Teardown( ctx );
}

static void CPthreadReqMutexTimedlockTimeout_Action(
  CPthreadReqMutexTimedlockTimeout_Context *ctx
)
{
  pthread_mutexattr_t attr;

  T_eq_int( pthread_mutexattr_init( &attr ), 0 );
  T_eq_int( pthread_mutexattr_setprotocol( &attr, ctx->protocol ), 0 );
  T_eq_int(
    pthread_mutexattr_setprioceiling( &attr, ToPOSIXPriority( PRIO_HIGH ) ),
    0
  );
  T_eq_int( pthread_mutex_init( &ctx->mutex, &attr ), 0 );
  T_eq_int( pthread_mutexattr_destroy( &attr ), 0 );
  ctx->tq_ctx.thread_queue_object = &ctx->mutex;
}

static void CPthreadReqMutexTimedlockTimeout_Cleanup(
  CPthreadReqMutexTimedlockTimeout_Context *ctx
)
{
  T_eq_int( pthread_mutex_destroy( &ctx->mutex ), 0 );
}

/* clang-format off */

static const CPthreadReqMutexTimedlockTimeout_Entry
CPthreadReqMutexTimedlockTimeout_Entries[] = {
  { 0, 0, CPthreadReqMutexTimedlockTimeout_Post_Action_Timeout },
  { 0, 0, CPthreadReqMutexTimedlockTimeout_Post_Action_TimeoutPriorityInherit }
};

static const uint8_t
CPthreadReqMutexTimedlockTimeout_Map[] = {
  0, 1, 0
};

/* clang-format on */

static size_t CPthreadReqMutexTimedlockTimeout_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexTimedlockTimeout_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexTimedlockTimeout_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexTimedlockTimeout_Fixture = {
  .setup = CPthreadReqMutexTimedlockTimeout_Setup_Wrap,
  .stop = NULL,
  .teardown = CPthreadReqMutexTimedlockTimeout_Teardown_Wrap,
  .scope = CPthreadReqMutexTimedlockTimeout_Scope,
  .initial_context = &CPthreadReqMutexTimedlockTimeout_Instance
};

static inline CPthreadReqMutexTimedlockTimeout_Entry
CPthreadReqMutexTimedlockTimeout_PopEntry(
  CPthreadReqMutexTimedlockTimeout_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexTimedlockTimeout_Entries
    [ CPthreadReqMutexTimedlockTimeout_Map[ index ] ];
}

static void CPthreadReqMutexTimedlockTimeout_TestVariant(
  CPthreadReqMutexTimedlockTimeout_Context *ctx
)
{
  CPthreadReqMutexTimedlockTimeout_Pre_Protocol_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  CPthreadReqMutexTimedlockTimeout_Action( ctx );
  CPthreadReqMutexTimedlockTimeout_Post_Action_Check(
    ctx,
    ctx->Map.entry.Post_Action
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexTimedlockTimeout( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexTimedlockTimeout,
  &CPthreadReqMutexTimedlockTimeout_Fixture
)
{
  CPthreadReqMutexTimedlockTimeout_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = CPthreadReqMutexTimedlockTimeout_Pre_Protocol_None;
    ctx->Map.pcs[ 0 ] < CPthreadReqMutexTimedlockTimeout_Pre_Protocol_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = CPthreadReqMutexTimedlockTimeout_PopEntry( ctx );
    CPthreadReqMutexTimedlockTimeout_TestVariant( ctx );
    CPthreadReqMutexTimedlockTimeout_Cleanup( ctx );
  }
}

/** @} */
