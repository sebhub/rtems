/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexUnlock
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
#include <rtems.h>
#include <string.h>

#include "tr-mtx-surrender.h"
#include "tx-support.h"
#include "tx-thread-queue.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqMutexUnlock spec:/c/pthread/req/mutex-unlock
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexUnlock_Pre_Protocol_None,
  CPthreadReqMutexUnlock_Pre_Protocol_Inherit,
  CPthreadReqMutexUnlock_Pre_Protocol_Protect,
  CPthreadReqMutexUnlock_Pre_Protocol_NA
} CPthreadReqMutexUnlock_Pre_Protocol;

typedef enum {
  CPthreadReqMutexUnlock_Pre_Type_Normal,
  CPthreadReqMutexUnlock_Pre_Type_Recursive,
  CPthreadReqMutexUnlock_Pre_Type_Errorcheck,
  CPthreadReqMutexUnlock_Pre_Type_Default,
  CPthreadReqMutexUnlock_Pre_Type_NA
} CPthreadReqMutexUnlock_Pre_Type;

typedef enum {
  CPthreadReqMutexUnlock_Pre_Mutex_Null,
  CPthreadReqMutexUnlock_Pre_Mutex_Invalid,
  CPthreadReqMutexUnlock_Pre_Mutex_Static,
  CPthreadReqMutexUnlock_Pre_Mutex_Init,
  CPthreadReqMutexUnlock_Pre_Mutex_NA
} CPthreadReqMutexUnlock_Pre_Mutex;

typedef enum {
  CPthreadReqMutexUnlock_Post_Status_Inval,
  CPthreadReqMutexUnlock_Post_Status_Perm,
  CPthreadReqMutexUnlock_Post_Status_NA
} CPthreadReqMutexUnlock_Post_Status;

typedef enum {
  CPthreadReqMutexUnlock_Post_Action_MtxSurrender,
  CPthreadReqMutexUnlock_Post_Action_NA
} CPthreadReqMutexUnlock_Post_Action;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Protocol_NA : 1;
  uint8_t Pre_Type_NA : 1;
  uint8_t Pre_Mutex_NA : 1;
  uint8_t Post_Status : 2;
  uint8_t Post_Action : 1;
} CPthreadReqMutexUnlock_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutex-unlock test case.
 */
typedef struct {
  /**
   * @brief This member contains the thread queue test context.
   */
  TQMtxContext tq_mtx_ctx;

  /**
   * @brief This member specifies the `mutex` parameter value.
   */
  pthread_mutex_t *mutex;

  /**
   * @brief This member contains the return value of the pthread_mutex_unlock()
   *   call.
   */
  int status;

  /**
   * @brief This member contains the mutex object.
   */
  pthread_mutex_t mutex_obj;

  /**
   * @brief This member specifies the protocol attribute used to initialize the
   *   mutex object.
   */
  int protocol;

  /**
   * @brief This member specifies the type attribute used to initialize the
   *   mutex object.
   */
  int type;

  /**
   * @brief This member is true, if the mutex object shall be destroyed by the
   *   cleanup step.
   */
  bool mutex_initialized;

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
    CPthreadReqMutexUnlock_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexUnlock_Context;

static CPthreadReqMutexUnlock_Context CPthreadReqMutexUnlock_Instance;

static const char *const CPthreadReqMutexUnlock_PreDesc_Protocol[] =
  { "None", "Inherit", "Protect", "NA" };

static const char *const CPthreadReqMutexUnlock_PreDesc_Type[] =
  { "Normal", "Recursive", "Errorcheck", "Default", "NA" };

static const char *const CPthreadReqMutexUnlock_PreDesc_Mutex[] =
  { "Null", "Invalid", "Static", "Init", "NA" };

static const char *const *const CPthreadReqMutexUnlock_PreDesc[] = {
  CPthreadReqMutexUnlock_PreDesc_Protocol,
  CPthreadReqMutexUnlock_PreDesc_Type,
  CPthreadReqMutexUnlock_PreDesc_Mutex,
  NULL
};

static void CPthreadReqMutexUnlock_Pre_Protocol_Prepare(
  CPthreadReqMutexUnlock_Context     *ctx,
  CPthreadReqMutexUnlock_Pre_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexUnlock_Pre_Protocol_None: {
      /*
       * While the mutex protocol attribute of the mutex is PTHREAD_PRIO_NONE.
       */
      ctx->protocol = PTHREAD_PRIO_NONE;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_NO_PROTOCOL;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_FIFO;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Protocol_Inherit: {
      /*
       * While the mutex protocol attribute of the mutex is
       * PTHREAD_PRIO_INHERIT.
       */
      ctx->protocol = PTHREAD_PRIO_INHERIT;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_PRIORITY_INHERIT;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_PRIORITY;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Protocol_Protect: {
      /*
       * While the mutex protocol attribute of the mutex is
       * PTHREAD_PRIO_PROTECT.
       */
      ctx->protocol = PTHREAD_PRIO_PROTECT;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_PRIORITY_CEILING;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_VERY_HIGH;
      ctx->tq_mtx_ctx.base.discipline = TQ_PRIORITY;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexUnlock_Pre_Type_Prepare(
  CPthreadReqMutexUnlock_Context *ctx,
  CPthreadReqMutexUnlock_Pre_Type state
)
{
  switch ( state ) {
    case CPthreadReqMutexUnlock_Pre_Type_Normal: {
      /*
       * While the mutex type attribute of the mutex is PTHREAD_MUTEX_NORMAL.
       */
      ctx->type = PTHREAD_MUTEX_NORMAL;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Type_Recursive: {
      /*
       * While the mutex type attribute of the mutex is
       * PTHREAD_MUTEX_RECURSIVE.
       */
      ctx->type = PTHREAD_MUTEX_RECURSIVE;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_ALLOWED;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Type_Errorcheck: {
      /*
       * While the mutex type attribute of the mutex is
       * PTHREAD_MUTEX_ERRORCHECK.
       */
      ctx->type = PTHREAD_MUTEX_ERRORCHECK;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Type_Default: {
      /*
       * While the mutex type attribute of the mutex is PTHREAD_MUTEX_DEFAULT.
       */
      ctx->type = PTHREAD_MUTEX_DEFAULT;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Type_NA:
      break;
  }
}

static void CPthreadReqMutexUnlock_Pre_Mutex_Prepare(
  CPthreadReqMutexUnlock_Context  *ctx,
  CPthreadReqMutexUnlock_Pre_Mutex state
)
{
  switch ( state ) {
    case CPthreadReqMutexUnlock_Pre_Mutex_Null: {
      /*
       * While the `mutex` parameter is NULL.
       */
      ctx->mutex = NULL;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Mutex_Invalid: {
      /*
       * While the object referenced by the `mutex` parameter is not a valid
       * mutex.
       */
      T_eq_int( pthread_mutex_init( &ctx->mutex_obj, NULL ), 0 );
      T_eq_int( pthread_mutex_destroy( &ctx->mutex_obj ), 0 );
      ctx->mutex = &ctx->mutex_obj;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Mutex_Static: {
      /*
       * While the object referenced by the `mutex` parameter is equal to
       * PTHREAD_MUTEX_INITIALIZER.
       */
      static const pthread_mutex_t initializer = PTHREAD_MUTEX_INITIALIZER;

      ctx->mutex_obj = initializer;
      ctx->mutex = &ctx->mutex_obj;
      ctx->mutex_initialized = true;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Mutex_Init: {
      /*
       * While the object referenced by the `mutex` parameter is an initialized
       * mutex.
       */
      pthread_mutexattr_t attr;

      T_eq_int( pthread_mutexattr_init( &attr ), 0 );
      T_eq_int( pthread_mutexattr_settype( &attr, ctx->type ), 0 );
      T_eq_int( pthread_mutexattr_setprotocol( &attr, ctx->protocol ), 0 );
      T_eq_int(
        pthread_mutexattr_setprioceiling(
          &attr,
          ToPOSIXPriority( PRIO_VERY_HIGH )
        ),
        0
      );
      T_eq_int( pthread_mutex_init( &ctx->mutex_obj, &attr ), 0 );
      T_eq_int( pthread_mutexattr_destroy( &attr ), 0 );
      ctx->mutex = &ctx->mutex_obj;
      ctx->mutex_initialized = true;
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      T_eq_int( pthread_mutex_lock( ctx->mutex ), 0 );
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Mutex_NA:
      break;
  }
}

static void CPthreadReqMutexUnlock_Post_Status_Check(
  CPthreadReqMutexUnlock_Context    *ctx,
  CPthreadReqMutexUnlock_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexUnlock_Post_Status_Inval: {
      /*
       * The return value of pthread_mutex_unlock() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexUnlock_Post_Status_Perm: {
      /*
       * The return value of pthread_mutex_unlock() shall be EPERM.
       */
      T_eq_int( ctx->status, EPERM );
      break;
    }

    case CPthreadReqMutexUnlock_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexUnlock_Post_Action_Check(
  CPthreadReqMutexUnlock_Context    *ctx,
  CPthreadReqMutexUnlock_Post_Action state
)
{
  switch ( state ) {
    case CPthreadReqMutexUnlock_Post_Action_MtxSurrender: {
      /*
       * The calling thread shall surrender the mutex as specified by
       * spec:/score/mtx/req/surrender.
       */
      /*
       * The function call released the mutex which the preparation locked.
       * The delegated test starts with an unowned mutex.
       */
      T_eq_int( ctx->status, 0 );
      T_null( TQGetOwnerPOSIXMutex( &ctx->tq_mtx_ctx.base ) );
      ScoreMtxReqSurrender_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexUnlock_Post_Action_NA:
      break;
  }
}

static void CPthreadReqMutexUnlock_Setup( CPthreadReqMutexUnlock_Context *ctx )
{
  SetSelfPriority( PRIO_NORMAL );
  memset( ctx, 0, sizeof( *ctx ) );
  ctx->tq_mtx_ctx.owner_check = TQ_MTX_CHECKS_OWNER;
  ctx->tq_mtx_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_mtx_ctx.base.enqueue_variant = TQ_ENQUEUE_BLOCKS;
  ctx->tq_mtx_ctx.base.convert_status = TQConvertStatusPOSIX;
  ctx->tq_mtx_ctx.base.enqueue_prepare = TQEnqueuePrepareDefault;
  ctx->tq_mtx_ctx.base.enqueue_done = TQEnqueueDoneDefault;
  ctx->tq_mtx_ctx.base.enqueue = TQEnqueuePOSIXMutex;
  ctx->tq_mtx_ctx.base.surrender = TQSurrenderPOSIXMutex;
  ctx->tq_mtx_ctx.base.get_owner = TQGetOwnerPOSIXMutex;
  TQInitialize( &ctx->tq_mtx_ctx.base );
}

static void CPthreadReqMutexUnlock_Setup_Wrap( void *arg )
{
  CPthreadReqMutexUnlock_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexUnlock_Setup( ctx );
}

static void CPthreadReqMutexUnlock_Teardown(
  CPthreadReqMutexUnlock_Context *ctx
)
{
  TQDestroy( &ctx->tq_mtx_ctx.base );
  RestoreRunnerPriority();
}

static void CPthreadReqMutexUnlock_Teardown_Wrap( void *arg )
{
  CPthreadReqMutexUnlock_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexUnlock_Teardown( ctx );
}

static void CPthreadReqMutexUnlock_Prepare(
  CPthreadReqMutexUnlock_Context *ctx
)
{
  ctx->protocol = PTHREAD_PRIO_NONE;
  ctx->type = PTHREAD_MUTEX_DEFAULT;
  ctx->mutex_initialized = false;
  ctx->status = -1;
}

static void CPthreadReqMutexUnlock_Action(
  CPthreadReqMutexUnlock_Context *ctx
)
{
  ctx->status = pthread_mutex_unlock( ctx->mutex );
}

static void CPthreadReqMutexUnlock_Cleanup(
  CPthreadReqMutexUnlock_Context *ctx
)
{
  if ( ctx->mutex_initialized ) {
    ctx->mutex_initialized = false;

    while ( pthread_mutex_unlock( ctx->mutex ) == 0 ) {
      /* Release the mutex including all recursive locks */
    }

    T_eq_int( pthread_mutex_destroy( &ctx->mutex_obj ), 0 );
  }
}

/* clang-format off */

static const CPthreadReqMutexUnlock_Entry
CPthreadReqMutexUnlock_Entries[] = {
  { 1, 0, 0, 0, CPthreadReqMutexUnlock_Post_Status_NA,
    CPthreadReqMutexUnlock_Post_Action_NA },
  { 0, 0, 0, 0, CPthreadReqMutexUnlock_Post_Status_NA,
    CPthreadReqMutexUnlock_Post_Action_MtxSurrender },
  { 0, 1, 1, 0, CPthreadReqMutexUnlock_Post_Status_Inval,
    CPthreadReqMutexUnlock_Post_Action_NA },
  { 0, 1, 1, 0, CPthreadReqMutexUnlock_Post_Status_Perm,
    CPthreadReqMutexUnlock_Post_Action_NA }
};

static const uint8_t
CPthreadReqMutexUnlock_Map[] = {
  2, 2, 3, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
  0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1
};

/* clang-format on */

static size_t CPthreadReqMutexUnlock_Scope( void *arg, char *buf, size_t n )
{
  CPthreadReqMutexUnlock_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CPthreadReqMutexUnlock_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CPthreadReqMutexUnlock_Fixture = {
  .setup = CPthreadReqMutexUnlock_Setup_Wrap,
  .stop = NULL,
  .teardown = CPthreadReqMutexUnlock_Teardown_Wrap,
  .scope = CPthreadReqMutexUnlock_Scope,
  .initial_context = &CPthreadReqMutexUnlock_Instance
};

static inline CPthreadReqMutexUnlock_Entry CPthreadReqMutexUnlock_PopEntry(
  CPthreadReqMutexUnlock_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexUnlock_Entries[ CPthreadReqMutexUnlock_Map[ index ] ];
}

static void CPthreadReqMutexUnlock_SetPreConditionStates(
  CPthreadReqMutexUnlock_Context *ctx
)
{
  if ( ctx->Map.entry.Pre_Protocol_NA ) {
    ctx->Map.pcs[ 0 ] = CPthreadReqMutexUnlock_Pre_Protocol_NA;
  } else {
    ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  }

  if ( ctx->Map.entry.Pre_Type_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexUnlock_Pre_Type_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
}

static void CPthreadReqMutexUnlock_TestVariant(
  CPthreadReqMutexUnlock_Context *ctx
)
{
  CPthreadReqMutexUnlock_Pre_Protocol_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexUnlock_Pre_Type_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CPthreadReqMutexUnlock_Pre_Mutex_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexUnlock_Action( ctx );
  CPthreadReqMutexUnlock_Post_Status_Check( ctx, ctx->Map.entry.Post_Status );
  CPthreadReqMutexUnlock_Post_Action_Check( ctx, ctx->Map.entry.Post_Action );
}

/**
 * @fn void T_case_body_CPthreadReqMutexUnlock( void )
 */
T_TEST_CASE_FIXTURE( CPthreadReqMutexUnlock, &CPthreadReqMutexUnlock_Fixture )
{
  CPthreadReqMutexUnlock_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexUnlock_Pre_Protocol_None;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexUnlock_Pre_Protocol_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexUnlock_Pre_Type_Normal;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexUnlock_Pre_Type_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexUnlock_Pre_Mutex_Null;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexUnlock_Pre_Mutex_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        ctx->Map.entry = CPthreadReqMutexUnlock_PopEntry( ctx );

        if ( ctx->Map.entry.Skip ) {
          continue;
        }

        CPthreadReqMutexUnlock_SetPreConditionStates( ctx );
        CPthreadReqMutexUnlock_Prepare( ctx );
        CPthreadReqMutexUnlock_TestVariant( ctx );
        CPthreadReqMutexUnlock_Cleanup( ctx );
      }
    }
  }
}

/** @} */
