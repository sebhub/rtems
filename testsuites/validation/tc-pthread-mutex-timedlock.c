/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexTimedlock
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
#include <time.h>
#include <rtems/score/threadimpl.h>

#include "tr-mtx-seize-wait.h"
#include "tx-support.h"
#include "tx-thread-queue.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqMutexTimedlock spec:/c/pthread/req/mutex-timedlock
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexTimedlock_Pre_Protocol_None,
  CPthreadReqMutexTimedlock_Pre_Protocol_Inherit,
  CPthreadReqMutexTimedlock_Pre_Protocol_Protect,
  CPthreadReqMutexTimedlock_Pre_Protocol_NA
} CPthreadReqMutexTimedlock_Pre_Protocol;

typedef enum {
  CPthreadReqMutexTimedlock_Pre_Type_Normal,
  CPthreadReqMutexTimedlock_Pre_Type_Recursive,
  CPthreadReqMutexTimedlock_Pre_Type_Errorcheck,
  CPthreadReqMutexTimedlock_Pre_Type_Default,
  CPthreadReqMutexTimedlock_Pre_Type_NA
} CPthreadReqMutexTimedlock_Pre_Type;

typedef enum {
  CPthreadReqMutexTimedlock_Pre_Mutex_Null,
  CPthreadReqMutexTimedlock_Pre_Mutex_Invalid,
  CPthreadReqMutexTimedlock_Pre_Mutex_Static,
  CPthreadReqMutexTimedlock_Pre_Mutex_Init,
  CPthreadReqMutexTimedlock_Pre_Mutex_NA
} CPthreadReqMutexTimedlock_Pre_Mutex;

typedef enum {
  CPthreadReqMutexTimedlock_Pre_Timeout_Future,
  CPthreadReqMutexTimedlock_Pre_Timeout_Past,
  CPthreadReqMutexTimedlock_Pre_Timeout_Invalid,
  CPthreadReqMutexTimedlock_Pre_Timeout_NA
} CPthreadReqMutexTimedlock_Pre_Timeout;

typedef enum {
  CPthreadReqMutexTimedlock_Pre_Owner_None,
  CPthreadReqMutexTimedlock_Pre_Owner_Other,
  CPthreadReqMutexTimedlock_Pre_Owner_NA
} CPthreadReqMutexTimedlock_Pre_Owner;

typedef enum {
  CPthreadReqMutexTimedlock_Pre_Scheduler_Home,
  CPthreadReqMutexTimedlock_Pre_Scheduler_Other,
  CPthreadReqMutexTimedlock_Pre_Scheduler_NA
} CPthreadReqMutexTimedlock_Pre_Scheduler;

typedef enum {
  CPthreadReqMutexTimedlock_Post_Status_Ok,
  CPthreadReqMutexTimedlock_Post_Status_Inval,
  CPthreadReqMutexTimedlock_Post_Status_TimedOut,
  CPthreadReqMutexTimedlock_Post_Status_NA
} CPthreadReqMutexTimedlock_Post_Status;

typedef enum {
  CPthreadReqMutexTimedlock_Post_Owner_Caller,
  CPthreadReqMutexTimedlock_Post_Owner_Other,
  CPthreadReqMutexTimedlock_Post_Owner_None,
  CPthreadReqMutexTimedlock_Post_Owner_NA
} CPthreadReqMutexTimedlock_Post_Owner;

typedef enum {
  CPthreadReqMutexTimedlock_Post_Action_MtxSeizeWait,
  CPthreadReqMutexTimedlock_Post_Action_NA
} CPthreadReqMutexTimedlock_Post_Action;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Protocol_NA : 1;
  uint16_t Pre_Type_NA : 1;
  uint16_t Pre_Mutex_NA : 1;
  uint16_t Pre_Timeout_NA : 1;
  uint16_t Pre_Owner_NA : 1;
  uint16_t Pre_Scheduler_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Owner : 2;
  uint16_t Post_Action : 1;
} CPthreadReqMutexTimedlock_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutex-timedlock test case.
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
   * @brief This member provides the object referenced by the `abstime`
   *   parameter.
   */
  struct timespec abstime;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutex_timedlock() call.
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
   * @brief This member is true, if another thread owns the mutex.
   */
  bool owner_other;

  /**
   * @brief This member is true, if the home scheduler of the calling thread is
   *   not the scheduler of the mutex.
   */
  bool scheduler_other;

  /**
   * @brief This member contains the thread control block of the runner.
   */
  const rtems_tcb *runner;

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 6 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 6 ];

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
    CPthreadReqMutexTimedlock_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexTimedlock_Context;

static CPthreadReqMutexTimedlock_Context CPthreadReqMutexTimedlock_Instance;

static const char *const CPthreadReqMutexTimedlock_PreDesc_Protocol[] =
  { "None", "Inherit", "Protect", "NA" };

static const char *const CPthreadReqMutexTimedlock_PreDesc_Type[] =
  { "Normal", "Recursive", "Errorcheck", "Default", "NA" };

static const char *const CPthreadReqMutexTimedlock_PreDesc_Mutex[] =
  { "Null", "Invalid", "Static", "Init", "NA" };

static const char *const CPthreadReqMutexTimedlock_PreDesc_Timeout[] =
  { "Future", "Past", "Invalid", "NA" };

static const char *const CPthreadReqMutexTimedlock_PreDesc_Owner[] =
  { "None", "Other", "NA" };

static const char *const CPthreadReqMutexTimedlock_PreDesc_Scheduler[] =
  { "Home", "Other", "NA" };

static const char *const *const CPthreadReqMutexTimedlock_PreDesc[] = {
  CPthreadReqMutexTimedlock_PreDesc_Protocol,
  CPthreadReqMutexTimedlock_PreDesc_Type,
  CPthreadReqMutexTimedlock_PreDesc_Mutex,
  CPthreadReqMutexTimedlock_PreDesc_Timeout,
  CPthreadReqMutexTimedlock_PreDesc_Owner,
  CPthreadReqMutexTimedlock_PreDesc_Scheduler,
  NULL
};

static void CPthreadReqMutexTimedlock_Pre_Protocol_Prepare(
  CPthreadReqMutexTimedlock_Context     *ctx,
  CPthreadReqMutexTimedlock_Pre_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Pre_Protocol_None: {
      /*
       * While the mutex protocol attribute of the mutex is PTHREAD_PRIO_NONE.
       */
      ctx->protocol = PTHREAD_PRIO_NONE;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_NO_PROTOCOL;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_FIFO;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Protocol_Inherit: {
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

    case CPthreadReqMutexTimedlock_Pre_Protocol_Protect: {
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

    case CPthreadReqMutexTimedlock_Pre_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlock_Pre_Type_Prepare(
  CPthreadReqMutexTimedlock_Context *ctx,
  CPthreadReqMutexTimedlock_Pre_Type state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Pre_Type_Normal: {
      /*
       * While the mutex type attribute of the mutex is PTHREAD_MUTEX_NORMAL.
       */
      ctx->type = PTHREAD_MUTEX_NORMAL;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Type_Recursive: {
      /*
       * While the mutex type attribute of the mutex is
       * PTHREAD_MUTEX_RECURSIVE.
       */
      ctx->type = PTHREAD_MUTEX_RECURSIVE;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_ALLOWED;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Type_Errorcheck: {
      /*
       * While the mutex type attribute of the mutex is
       * PTHREAD_MUTEX_ERRORCHECK.
       */
      ctx->type = PTHREAD_MUTEX_ERRORCHECK;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Type_Default: {
      /*
       * While the mutex type attribute of the mutex is PTHREAD_MUTEX_DEFAULT.
       */
      ctx->type = PTHREAD_MUTEX_DEFAULT;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Type_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlock_Pre_Mutex_Prepare(
  CPthreadReqMutexTimedlock_Context  *ctx,
  CPthreadReqMutexTimedlock_Pre_Mutex state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Pre_Mutex_Null: {
      /*
       * While the `mutex` parameter is NULL.
       */
      ctx->mutex = NULL;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Mutex_Invalid: {
      /*
       * While the object referenced by the `mutex` parameter is not a valid
       * mutex.
       */
      T_eq_int( pthread_mutex_init( &ctx->mutex_obj, NULL ), 0 );
      T_eq_int( pthread_mutex_destroy( &ctx->mutex_obj ), 0 );
      ctx->mutex = &ctx->mutex_obj;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Mutex_Static: {
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
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_NO_PROTOCOL;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_FIFO;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Mutex_Init: {
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
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Mutex_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlock_Pre_Timeout_Prepare(
  CPthreadReqMutexTimedlock_Context    *ctx,
  CPthreadReqMutexTimedlock_Pre_Timeout state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Pre_Timeout_Future: {
      /*
       * While the `abstime` parameter references a valid time point which is
       * later than the current time of CLOCK_REALTIME.
       */
      T_eq_int( clock_gettime( CLOCK_REALTIME, &ctx->abstime ), 0 );
      ctx->abstime.tv_sec += 3600;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Timeout_Past: {
      /*
       * While the `abstime` parameter references a valid time point which is
       * not later than the current time of CLOCK_REALTIME.
       */
      T_eq_int( clock_gettime( CLOCK_REALTIME, &ctx->abstime ), 0 );
      ctx->abstime.tv_sec -= 1;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Timeout_Invalid: {
      /*
       * While the `abstime` parameter references an object which is not a
       * valid time point.
       */
      ctx->abstime.tv_sec = 0;
      ctx->abstime.tv_nsec = -1;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Timeout_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlock_Pre_Owner_Prepare(
  CPthreadReqMutexTimedlock_Context  *ctx,
  CPthreadReqMutexTimedlock_Pre_Owner state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Pre_Owner_None: {
      /*
       * While the mutex has no owner.
       */
      ctx->owner_other = false;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Owner_Other: {
      /*
       * While the owner of the mutex is a thread other than the calling
       * thread.
       */
      ctx->owner_other = true;
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.base.wait = TQ_WAIT_FOREVER;
      TQSend( &ctx->tq_mtx_ctx.base, TQ_BLOCKER_B, TQ_EVENT_ENQUEUE );
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Owner_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlock_Pre_Scheduler_Prepare(
  CPthreadReqMutexTimedlock_Context      *ctx,
  CPthreadReqMutexTimedlock_Pre_Scheduler state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Pre_Scheduler_Home: {
      /*
       * While the home scheduler of the calling thread is the scheduler
       * instance of the mutex.
       */
      ctx->scheduler_other = false;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Scheduler_Other: {
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

    case CPthreadReqMutexTimedlock_Pre_Scheduler_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlock_Post_Status_Check(
  CPthreadReqMutexTimedlock_Context    *ctx,
  CPthreadReqMutexTimedlock_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Post_Status_Ok: {
      /*
       * The return value of pthread_mutex_timedlock() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Status_Inval: {
      /*
       * The return value of pthread_mutex_timedlock() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Status_TimedOut: {
      /*
       * The return value of pthread_mutex_timedlock() shall be ETIMEDOUT.
       */
      T_eq_int( ctx->status, ETIMEDOUT );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlock_Post_Owner_Check(
  CPthreadReqMutexTimedlock_Context   *ctx,
  CPthreadReqMutexTimedlock_Post_Owner state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Post_Owner_Caller: {
      /*
       * The owner of the mutex shall be the calling thread.
       */
      T_eq_ptr( TQGetOwnerPOSIXMutex( &ctx->tq_mtx_ctx.base ), ctx->runner );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Owner_Other: {
      /*
       * The owner of the mutex shall be the other thread.
       */
      T_eq_ptr(
        TQGetOwnerPOSIXMutex( &ctx->tq_mtx_ctx.base ),
        ctx->tq_mtx_ctx.base.worker_tcb[ TQ_BLOCKER_B ]
      );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Owner_None: {
      /*
       * The mutex shall have no owner.
       */
      T_null( TQGetOwnerPOSIXMutex( &ctx->tq_mtx_ctx.base ) );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Owner_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlock_Post_Action_Check(
  CPthreadReqMutexTimedlock_Context    *ctx,
  CPthreadReqMutexTimedlock_Post_Action state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Post_Action_MtxSeizeWait: {
      /*
       * The calling thread shall wait to seize the mutex as specified by
       * spec:/score/mtx/req/seize-wait.
       */
      /*
       * The function call left the mutex owned by the calling thread, and a
       * priority ceiling raised its priority.  Release it so that the
       * delegated test starts with an unowned mutex and the calling thread at
       * its normal priority.
       */
      T_eq_int( ctx->status, 0 );
      T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );

      ctx->tq_mtx_ctx.base.wait = TQ_WAIT_TIMED;
      ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Action_NA:
      break;
  }
}

static void CPthreadReqMutexTimedlock_Setup(
  CPthreadReqMutexTimedlock_Context *ctx
)
{
  SetSelfPriority( PRIO_NORMAL );
  memset( ctx, 0, sizeof( *ctx ) );
  ctx->runner = _Thread_Get_executing();
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

static void CPthreadReqMutexTimedlock_Setup_Wrap( void *arg )
{
  CPthreadReqMutexTimedlock_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexTimedlock_Setup( ctx );
}

static void CPthreadReqMutexTimedlock_Teardown(
  CPthreadReqMutexTimedlock_Context *ctx
)
{
  TQDestroy( &ctx->tq_mtx_ctx.base );
  RestoreRunnerPriority();
}

static void CPthreadReqMutexTimedlock_Teardown_Wrap( void *arg )
{
  CPthreadReqMutexTimedlock_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexTimedlock_Teardown( ctx );
}

static void CPthreadReqMutexTimedlock_Prepare(
  CPthreadReqMutexTimedlock_Context *ctx
)
{
  ctx->protocol = PTHREAD_PRIO_NONE;
  ctx->type = PTHREAD_MUTEX_DEFAULT;
  ctx->mutex_initialized = false;
  ctx->owner_other = false;
  ctx->scheduler_other = false;
  ctx->status = -1;
  T_eq_int( clock_gettime( CLOCK_REALTIME, &ctx->abstime ), 0 );
  ctx->abstime.tv_sec += 3600;
}

static void CPthreadReqMutexTimedlock_Action(
  CPthreadReqMutexTimedlock_Context *ctx
)
{
  ctx->status = pthread_mutex_timedlock( ctx->mutex, &ctx->abstime );
}

static void CPthreadReqMutexTimedlock_Cleanup(
  CPthreadReqMutexTimedlock_Context *ctx
)
{
  if ( ctx->scheduler_other ) {
    SetSelfScheduler( SCHEDULER_A_ID, PRIO_NORMAL );
  }

  if ( ctx->owner_other ) {
    TQSend( &ctx->tq_mtx_ctx.base, TQ_BLOCKER_B, TQ_EVENT_SURRENDER );
  }

  if ( ctx->mutex_initialized ) {
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

static const CPthreadReqMutexTimedlock_Entry
CPthreadReqMutexTimedlock_Entries[] = {
  { 1, 0, 0, 0, 0, 0, 0, CPthreadReqMutexTimedlock_Post_Status_NA,
    CPthreadReqMutexTimedlock_Post_Owner_NA,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, 0, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Owner_Caller,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, 0, 1, CPthreadReqMutexTimedlock_Post_Status_TimedOut,
    CPthreadReqMutexTimedlock_Post_Owner_Other,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, 0, 1, CPthreadReqMutexTimedlock_Post_Status_Inval,
    CPthreadReqMutexTimedlock_Post_Owner_Other,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, 1, 1, CPthreadReqMutexTimedlock_Post_Status_NA,
    CPthreadReqMutexTimedlock_Post_Owner_NA,
    CPthreadReqMutexTimedlock_Post_Action_MtxSeizeWait },
  { 0, 0, 0, 0, 0, 1, 0, CPthreadReqMutexTimedlock_Post_Status_NA,
    CPthreadReqMutexTimedlock_Post_Owner_NA,
    CPthreadReqMutexTimedlock_Post_Action_MtxSeizeWait },
#if defined(RTEMS_SMP)
  { 0, 0, 0, 0, 0, 1, 0, CPthreadReqMutexTimedlock_Post_Status_Inval,
    CPthreadReqMutexTimedlock_Post_Owner_None,
    CPthreadReqMutexTimedlock_Post_Action_NA },
#else
  { 1, 0, 0, 0, 0, 1, 0, CPthreadReqMutexTimedlock_Post_Status_NA,
    CPthreadReqMutexTimedlock_Post_Owner_NA,
    CPthreadReqMutexTimedlock_Post_Action_NA },
#endif
  { 0, 1, 1, 0, 1, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Inval,
    CPthreadReqMutexTimedlock_Post_Owner_NA,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 1, 1, 0, 0, 0, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Owner_Caller,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 1, 1, 0, 0, 1, 1, CPthreadReqMutexTimedlock_Post_Status_NA,
    CPthreadReqMutexTimedlock_Post_Owner_NA,
    CPthreadReqMutexTimedlock_Post_Action_MtxSeizeWait },
  { 0, 1, 1, 0, 0, 0, 1, CPthreadReqMutexTimedlock_Post_Status_TimedOut,
    CPthreadReqMutexTimedlock_Post_Owner_Other,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 1, 1, 0, 0, 0, 1, CPthreadReqMutexTimedlock_Post_Status_Inval,
    CPthreadReqMutexTimedlock_Post_Owner_Other,
    CPthreadReqMutexTimedlock_Post_Action_NA }
};

static const uint8_t
CPthreadReqMutexTimedlock_Map[] = {
  7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0,
  0, 0, 8, 0, 10, 0, 8, 0, 11, 0, 4, 0, 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 4, 0, 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
  0, 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1,
  0, 2, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 2, 0, 1,
  0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 5, 6, 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 5, 6, 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0,
  0, 1, 0, 2, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 0, 1, 0, 2,
  0, 1, 0, 3, 0
};

/* clang-format on */

static size_t CPthreadReqMutexTimedlock_Scope( void *arg, char *buf, size_t n )
{
  CPthreadReqMutexTimedlock_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexTimedlock_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexTimedlock_Fixture = {
  .setup = CPthreadReqMutexTimedlock_Setup_Wrap,
  .stop = NULL,
  .teardown = CPthreadReqMutexTimedlock_Teardown_Wrap,
  .scope = CPthreadReqMutexTimedlock_Scope,
  .initial_context = &CPthreadReqMutexTimedlock_Instance
};

static inline CPthreadReqMutexTimedlock_Entry
CPthreadReqMutexTimedlock_PopEntry( CPthreadReqMutexTimedlock_Context *ctx )
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexTimedlock_Entries
    [ CPthreadReqMutexTimedlock_Map[ index ] ];
}

static void CPthreadReqMutexTimedlock_SetPreConditionStates(
  CPthreadReqMutexTimedlock_Context *ctx
)
{
  if ( ctx->Map.entry.Pre_Protocol_NA ) {
    ctx->Map.pcs[ 0 ] = CPthreadReqMutexTimedlock_Pre_Protocol_NA;
  } else {
    ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  }

  if ( ctx->Map.entry.Pre_Type_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexTimedlock_Pre_Type_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];

  if ( ctx->Map.entry.Pre_Timeout_NA ) {
    ctx->Map.pcs[ 3 ] = CPthreadReqMutexTimedlock_Pre_Timeout_NA;
  } else {
    ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  }

  if ( ctx->Map.entry.Pre_Owner_NA ) {
    ctx->Map.pcs[ 4 ] = CPthreadReqMutexTimedlock_Pre_Owner_NA;
  } else {
    ctx->Map.pcs[ 4 ] = ctx->Map.pci[ 4 ];
  }

  if ( ctx->Map.entry.Pre_Scheduler_NA ) {
    ctx->Map.pcs[ 5 ] = CPthreadReqMutexTimedlock_Pre_Scheduler_NA;
  } else {
    ctx->Map.pcs[ 5 ] = ctx->Map.pci[ 5 ];
  }
}

static void CPthreadReqMutexTimedlock_TestVariant(
  CPthreadReqMutexTimedlock_Context *ctx
)
{
  CPthreadReqMutexTimedlock_Pre_Protocol_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexTimedlock_Pre_Type_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CPthreadReqMutexTimedlock_Pre_Mutex_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexTimedlock_Pre_Timeout_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqMutexTimedlock_Pre_Owner_Prepare( ctx, ctx->Map.pcs[ 4 ] );
  CPthreadReqMutexTimedlock_Pre_Scheduler_Prepare( ctx, ctx->Map.pcs[ 5 ] );
  CPthreadReqMutexTimedlock_Action( ctx );
  CPthreadReqMutexTimedlock_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexTimedlock_Post_Owner_Check( ctx, ctx->Map.entry.Post_Owner );
  CPthreadReqMutexTimedlock_Post_Action_Check(
    ctx,
    ctx->Map.entry.Post_Action
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexTimedlock( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexTimedlock,
  &CPthreadReqMutexTimedlock_Fixture
)
{
  CPthreadReqMutexTimedlock_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexTimedlock_Pre_Protocol_None;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexTimedlock_Pre_Protocol_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexTimedlock_Pre_Type_Normal;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexTimedlock_Pre_Type_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexTimedlock_Pre_Mutex_Null;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexTimedlock_Pre_Mutex_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexTimedlock_Pre_Timeout_Future;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexTimedlock_Pre_Timeout_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          for (
            ctx->Map.pci[ 4 ] = CPthreadReqMutexTimedlock_Pre_Owner_None;
            ctx->Map.pci[ 4 ] < CPthreadReqMutexTimedlock_Pre_Owner_NA;
            ++ctx->Map.pci[ 4 ]
          ) {
            for (
              ctx->Map.pci[ 5 ] = CPthreadReqMutexTimedlock_Pre_Scheduler_Home;
              ctx->Map.pci[ 5 ] < CPthreadReqMutexTimedlock_Pre_Scheduler_NA;
              ++ctx->Map.pci[ 5 ]
            ) {
              ctx->Map.entry = CPthreadReqMutexTimedlock_PopEntry( ctx );

              if ( ctx->Map.entry.Skip ) {
                continue;
              }

              CPthreadReqMutexTimedlock_SetPreConditionStates( ctx );
              CPthreadReqMutexTimedlock_Prepare( ctx );
              CPthreadReqMutexTimedlock_TestVariant( ctx );
              CPthreadReqMutexTimedlock_Cleanup( ctx );
            }
          }
        }
      }
    }
  }
}

/** @} */
