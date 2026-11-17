/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexTrylock
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

#include "tr-mtx-seize-try.h"
#include "tx-support.h"
#include "tx-thread-queue.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqMutexTrylock spec:/c/pthread/req/mutex-trylock
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexTrylock_Pre_Protocol_None,
  CPthreadReqMutexTrylock_Pre_Protocol_Inherit,
  CPthreadReqMutexTrylock_Pre_Protocol_Protect,
  CPthreadReqMutexTrylock_Pre_Protocol_NA
} CPthreadReqMutexTrylock_Pre_Protocol;

typedef enum {
  CPthreadReqMutexTrylock_Pre_Type_Normal,
  CPthreadReqMutexTrylock_Pre_Type_Recursive,
  CPthreadReqMutexTrylock_Pre_Type_Errorcheck,
  CPthreadReqMutexTrylock_Pre_Type_Default,
  CPthreadReqMutexTrylock_Pre_Type_NA
} CPthreadReqMutexTrylock_Pre_Type;

typedef enum {
  CPthreadReqMutexTrylock_Pre_Mutex_Null,
  CPthreadReqMutexTrylock_Pre_Mutex_Invalid,
  CPthreadReqMutexTrylock_Pre_Mutex_Static,
  CPthreadReqMutexTrylock_Pre_Mutex_Init,
  CPthreadReqMutexTrylock_Pre_Mutex_NA
} CPthreadReqMutexTrylock_Pre_Mutex;

typedef enum {
  CPthreadReqMutexTrylock_Pre_Scheduler_Home,
  CPthreadReqMutexTrylock_Pre_Scheduler_Other,
  CPthreadReqMutexTrylock_Pre_Scheduler_NA
} CPthreadReqMutexTrylock_Pre_Scheduler;

typedef enum {
  CPthreadReqMutexTrylock_Post_Status_Inval,
  CPthreadReqMutexTrylock_Post_Status_NA
} CPthreadReqMutexTrylock_Post_Status;

typedef enum {
  CPthreadReqMutexTrylock_Post_Action_MtxSeizeTry,
  CPthreadReqMutexTrylock_Post_Action_NA
} CPthreadReqMutexTrylock_Post_Action;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Protocol_NA : 1;
  uint8_t Pre_Type_NA : 1;
  uint8_t Pre_Mutex_NA : 1;
  uint8_t Pre_Scheduler_NA : 1;
  uint8_t Post_Status : 1;
  uint8_t Post_Action : 1;
} CPthreadReqMutexTrylock_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutex-trylock test case.
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
   * @brief This member contains the return value of the
   *   pthread_mutex_trylock() call.
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

  /**
   * @brief This member is true, if the home scheduler of the calling thread is
   *   not the scheduler of the mutex.
   */
  bool scheduler_other;

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
    CPthreadReqMutexTrylock_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexTrylock_Context;

static CPthreadReqMutexTrylock_Context CPthreadReqMutexTrylock_Instance;

static const char *const CPthreadReqMutexTrylock_PreDesc_Protocol[] =
  { "None", "Inherit", "Protect", "NA" };

static const char *const CPthreadReqMutexTrylock_PreDesc_Type[] =
  { "Normal", "Recursive", "Errorcheck", "Default", "NA" };

static const char *const CPthreadReqMutexTrylock_PreDesc_Mutex[] =
  { "Null", "Invalid", "Static", "Init", "NA" };

static const char *const CPthreadReqMutexTrylock_PreDesc_Scheduler[] =
  { "Home", "Other", "NA" };

static const char *const *const CPthreadReqMutexTrylock_PreDesc[] = {
  CPthreadReqMutexTrylock_PreDesc_Protocol,
  CPthreadReqMutexTrylock_PreDesc_Type,
  CPthreadReqMutexTrylock_PreDesc_Mutex,
  CPthreadReqMutexTrylock_PreDesc_Scheduler,
  NULL
};

static void CPthreadReqMutexTrylock_Pre_Protocol_Prepare(
  CPthreadReqMutexTrylock_Context     *ctx,
  CPthreadReqMutexTrylock_Pre_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexTrylock_Pre_Protocol_None: {
      /*
       * While the mutex protocol attribute of the mutex is PTHREAD_PRIO_NONE.
       */
      ctx->protocol = PTHREAD_PRIO_NONE;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_NO_PROTOCOL;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_FIFO;
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Protocol_Inherit: {
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

    case CPthreadReqMutexTrylock_Pre_Protocol_Protect: {
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

    case CPthreadReqMutexTrylock_Pre_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexTrylock_Pre_Type_Prepare(
  CPthreadReqMutexTrylock_Context *ctx,
  CPthreadReqMutexTrylock_Pre_Type state
)
{
  switch ( state ) {
    case CPthreadReqMutexTrylock_Pre_Type_Normal: {
      /*
       * While the mutex type attribute of the mutex is PTHREAD_MUTEX_NORMAL.
       */
      ctx->type = PTHREAD_MUTEX_NORMAL;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_UNAVAILABLE;
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Type_Recursive: {
      /*
       * While the mutex type attribute of the mutex is
       * PTHREAD_MUTEX_RECURSIVE.
       */
      ctx->type = PTHREAD_MUTEX_RECURSIVE;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_ALLOWED;
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Type_Errorcheck: {
      /*
       * While the mutex type attribute of the mutex is
       * PTHREAD_MUTEX_ERRORCHECK.
       */
      ctx->type = PTHREAD_MUTEX_ERRORCHECK;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_UNAVAILABLE;
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Type_Default: {
      /*
       * While the mutex type attribute of the mutex is PTHREAD_MUTEX_DEFAULT.
       */
      ctx->type = PTHREAD_MUTEX_DEFAULT;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_UNAVAILABLE;
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Type_NA:
      break;
  }
}

static void CPthreadReqMutexTrylock_Pre_Mutex_Prepare(
  CPthreadReqMutexTrylock_Context  *ctx,
  CPthreadReqMutexTrylock_Pre_Mutex state
)
{
  switch ( state ) {
    case CPthreadReqMutexTrylock_Pre_Mutex_Null: {
      /*
       * While the `mutex` parameter is NULL.
       */
      ctx->mutex = NULL;
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Mutex_Invalid: {
      /*
       * While the object referenced by the `mutex` parameter is not a valid
       * mutex.
       */
      T_eq_int( pthread_mutex_init( &ctx->mutex_obj, NULL ), 0 );
      T_eq_int( pthread_mutex_destroy( &ctx->mutex_obj ), 0 );
      ctx->mutex = &ctx->mutex_obj;
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Mutex_Static: {
      /*
       * While the object referenced by the `mutex` parameter is equal to
       * PTHREAD_MUTEX_INITIALIZER.
       */
      static const pthread_mutex_t initializer = PTHREAD_MUTEX_INITIALIZER;

      /*
       * The first lock initializes the mutex with the default attributes.
       * The Protocol and Type pre-conditions are not applicable, so the
       * thread queue test context is set here.
       */
      ctx->mutex_obj = initializer;
      ctx->mutex = &ctx->mutex_obj;
      ctx->mutex_initialized = true;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_NO_PROTOCOL;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_UNAVAILABLE;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_FIFO;
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Mutex_Init: {
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
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Mutex_NA:
      break;
  }
}

static void CPthreadReqMutexTrylock_Pre_Scheduler_Prepare(
  CPthreadReqMutexTrylock_Context      *ctx,
  CPthreadReqMutexTrylock_Pre_Scheduler state
)
{
  switch ( state ) {
    case CPthreadReqMutexTrylock_Pre_Scheduler_Home: {
      /*
       * While the home scheduler of the calling thread is the scheduler
       * instance of the mutex.
       */
      ctx->scheduler_other = false;
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Scheduler_Other: {
      /*
       * While the home scheduler of the calling thread is not the scheduler
       * instance of the mutex.
       */
      ctx->scheduler_other = true;

      #if defined( RTEMS_SMP )
      SetSelfScheduler( SCHEDULER_B_ID, PRIO_NORMAL );
      #endif
      break;
    }

    case CPthreadReqMutexTrylock_Pre_Scheduler_NA:
      break;
  }
}

static void CPthreadReqMutexTrylock_Post_Status_Check(
  CPthreadReqMutexTrylock_Context    *ctx,
  CPthreadReqMutexTrylock_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexTrylock_Post_Status_Inval: {
      /*
       * The return value of pthread_mutex_trylock() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexTrylock_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexTrylock_Post_Action_Check(
  CPthreadReqMutexTrylock_Context    *ctx,
  CPthreadReqMutexTrylock_Post_Action state
)
{
  switch ( state ) {
    case CPthreadReqMutexTrylock_Post_Action_MtxSeizeTry: {
      /*
       * The calling thread shall try to seize the mutex as specified by
       * spec:/score/mtx/req/seize-try.
       */
      /*
       * The function call left the mutex owned by the calling thread, and a
       * priority ceiling raised its priority.  Release it so that the
       * delegated test starts with an unowned mutex and the calling thread at
       * its normal priority.
       */
      T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );

      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.base.wait = TQ_NO_WAIT;
      ScoreMtxReqSeizeTry_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexTrylock_Post_Action_NA:
      break;
  }
}

static void CPthreadReqMutexTrylock_Setup(
  CPthreadReqMutexTrylock_Context *ctx
)
{
  SetSelfPriority( PRIO_NORMAL );
  memset( ctx, 0, sizeof( *ctx ) );
  ctx->tq_mtx_ctx.base.enqueue_variant = TQ_ENQUEUE_BLOCKS;
  ctx->tq_mtx_ctx.base.deadlock = TQ_DEADLOCK_STATUS;
  ctx->tq_mtx_ctx.base.convert_status = TQConvertStatusPOSIX;
  ctx->tq_mtx_ctx.base.enqueue_prepare = TQEnqueuePrepareDefault;
  ctx->tq_mtx_ctx.base.enqueue_done = TQEnqueueDoneDefault;
  ctx->tq_mtx_ctx.base.enqueue = TQEnqueuePOSIXMutex;
  ctx->tq_mtx_ctx.base.surrender = TQSurrenderPOSIXMutex;
  ctx->tq_mtx_ctx.base.get_owner = TQGetOwnerPOSIXMutex;
  TQInitialize( &ctx->tq_mtx_ctx.base );
}

static void CPthreadReqMutexTrylock_Setup_Wrap( void *arg )
{
  CPthreadReqMutexTrylock_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexTrylock_Setup( ctx );
}

static void CPthreadReqMutexTrylock_Teardown(
  CPthreadReqMutexTrylock_Context *ctx
)
{
  TQDestroy( &ctx->tq_mtx_ctx.base );
  RestoreRunnerPriority();
}

static void CPthreadReqMutexTrylock_Teardown_Wrap( void *arg )
{
  CPthreadReqMutexTrylock_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexTrylock_Teardown( ctx );
}

static void CPthreadReqMutexTrylock_Prepare(
  CPthreadReqMutexTrylock_Context *ctx
)
{
  ctx->protocol = PTHREAD_PRIO_NONE;
  ctx->type = PTHREAD_MUTEX_DEFAULT;
  ctx->mutex_initialized = false;
  ctx->scheduler_other = false;
}

static void CPthreadReqMutexTrylock_Action(
  CPthreadReqMutexTrylock_Context *ctx
)
{
  ctx->status = pthread_mutex_trylock( ctx->mutex );
}

static void CPthreadReqMutexTrylock_Cleanup(
  CPthreadReqMutexTrylock_Context *ctx
)
{
  if ( ctx->scheduler_other ) {
    SetSelfScheduler( SCHEDULER_A_ID, PRIO_NORMAL );
  }

  if ( ctx->mutex_initialized ) {
    ctx->mutex_initialized = false;

    /*
     * The mutex may be still owned by the calling thread, possibly with
     * recursive locks.
     */
    while ( pthread_mutex_unlock( ctx->mutex ) == 0 ) {
      /* Release the mutex including all recursive locks */
    }

    T_eq_int( pthread_mutex_destroy( &ctx->mutex_obj ), 0 );
  }
}

/* clang-format off */

static const CPthreadReqMutexTrylock_Entry
CPthreadReqMutexTrylock_Entries[] = {
  { 1, 0, 0, 0, 0, CPthreadReqMutexTrylock_Post_Status_NA,
    CPthreadReqMutexTrylock_Post_Action_NA },
  { 0, 0, 0, 0, 1, CPthreadReqMutexTrylock_Post_Status_NA,
    CPthreadReqMutexTrylock_Post_Action_MtxSeizeTry },
  { 0, 0, 0, 0, 0, CPthreadReqMutexTrylock_Post_Status_NA,
    CPthreadReqMutexTrylock_Post_Action_MtxSeizeTry },
#if defined(RTEMS_SMP)
  { 0, 0, 0, 0, 0, CPthreadReqMutexTrylock_Post_Status_Inval,
    CPthreadReqMutexTrylock_Post_Action_NA },
#else
  { 1, 0, 0, 0, 0, CPthreadReqMutexTrylock_Post_Status_NA,
    CPthreadReqMutexTrylock_Post_Action_NA },
#endif
  { 0, 1, 1, 0, 1, CPthreadReqMutexTrylock_Post_Status_Inval,
    CPthreadReqMutexTrylock_Post_Action_NA },
  { 0, 1, 1, 0, 1, CPthreadReqMutexTrylock_Post_Status_NA,
    CPthreadReqMutexTrylock_Post_Action_MtxSeizeTry }
};

static const uint8_t
CPthreadReqMutexTrylock_Map[] = {
  4, 0, 4, 0, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
  0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0,
  2, 3, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 2, 3
};

/* clang-format on */

static size_t CPthreadReqMutexTrylock_Scope( void *arg, char *buf, size_t n )
{
  CPthreadReqMutexTrylock_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexTrylock_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexTrylock_Fixture = {
  .setup = CPthreadReqMutexTrylock_Setup_Wrap,
  .stop = NULL,
  .teardown = CPthreadReqMutexTrylock_Teardown_Wrap,
  .scope = CPthreadReqMutexTrylock_Scope,
  .initial_context = &CPthreadReqMutexTrylock_Instance
};

static inline CPthreadReqMutexTrylock_Entry CPthreadReqMutexTrylock_PopEntry(
  CPthreadReqMutexTrylock_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexTrylock_Entries
    [ CPthreadReqMutexTrylock_Map[ index ] ];
}

static void CPthreadReqMutexTrylock_SetPreConditionStates(
  CPthreadReqMutexTrylock_Context *ctx
)
{
  if ( ctx->Map.entry.Pre_Protocol_NA ) {
    ctx->Map.pcs[ 0 ] = CPthreadReqMutexTrylock_Pre_Protocol_NA;
  } else {
    ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  }

  if ( ctx->Map.entry.Pre_Type_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexTrylock_Pre_Type_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];

  if ( ctx->Map.entry.Pre_Scheduler_NA ) {
    ctx->Map.pcs[ 3 ] = CPthreadReqMutexTrylock_Pre_Scheduler_NA;
  } else {
    ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  }
}

static void CPthreadReqMutexTrylock_TestVariant(
  CPthreadReqMutexTrylock_Context *ctx
)
{
  CPthreadReqMutexTrylock_Pre_Protocol_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexTrylock_Pre_Type_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CPthreadReqMutexTrylock_Pre_Mutex_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexTrylock_Pre_Scheduler_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqMutexTrylock_Action( ctx );
  CPthreadReqMutexTrylock_Post_Status_Check( ctx, ctx->Map.entry.Post_Status );
  CPthreadReqMutexTrylock_Post_Action_Check( ctx, ctx->Map.entry.Post_Action );
}

/**
 * @fn void T_case_body_CPthreadReqMutexTrylock( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexTrylock,
  &CPthreadReqMutexTrylock_Fixture
)
{
  CPthreadReqMutexTrylock_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexTrylock_Pre_Protocol_None;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexTrylock_Pre_Protocol_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexTrylock_Pre_Type_Normal;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexTrylock_Pre_Type_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexTrylock_Pre_Mutex_Null;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexTrylock_Pre_Mutex_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexTrylock_Pre_Scheduler_Home;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexTrylock_Pre_Scheduler_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = CPthreadReqMutexTrylock_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          CPthreadReqMutexTrylock_SetPreConditionStates( ctx );
          CPthreadReqMutexTrylock_Prepare( ctx );
          CPthreadReqMutexTrylock_TestVariant( ctx );
          CPthreadReqMutexTrylock_Cleanup( ctx );
        }
      }
    }
  }
}

/** @} */
