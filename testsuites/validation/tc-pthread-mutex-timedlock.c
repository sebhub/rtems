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
#include <time.h>
#include <rtems/posix/priorityimpl.h>
#include <rtems/score/schedulerimpl.h>
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
  CPthreadReqMutexTimedlock_Pre_Protocol_Ceiling,
  CPthreadReqMutexTimedlock_Pre_Protocol_NA
} CPthreadReqMutexTimedlock_Pre_Protocol;

typedef enum {
  CPthreadReqMutexTimedlock_Pre_Ceiling_Satisfied,
  CPthreadReqMutexTimedlock_Pre_Ceiling_Violated,
  CPthreadReqMutexTimedlock_Pre_Ceiling_None,
  CPthreadReqMutexTimedlock_Pre_Ceiling_NA
} CPthreadReqMutexTimedlock_Pre_Ceiling;

typedef enum {
  CPthreadReqMutexTimedlock_Pre_Type_Normal,
  CPthreadReqMutexTimedlock_Pre_Type_Recursive,
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
  CPthreadReqMutexTimedlock_Post_Action_MtxNone,
  CPthreadReqMutexTimedlock_Post_Action_MtxNoneRecursive,
  CPthreadReqMutexTimedlock_Post_Action_MtxInherit,
  CPthreadReqMutexTimedlock_Post_Action_MtxInheritRecursive,
  CPthreadReqMutexTimedlock_Post_Action_MtxCeiling,
  CPthreadReqMutexTimedlock_Post_Action_MtxCeilingRecursive,
  CPthreadReqMutexTimedlock_Post_Action_MtxStatic,
  CPthreadReqMutexTimedlock_Post_Action_NA
} CPthreadReqMutexTimedlock_Post_Action;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Protocol_NA : 1;
  uint16_t Pre_Ceiling_NA : 1;
  uint16_t Pre_Type_NA : 1;
  uint16_t Pre_Mutex_NA : 1;
  uint16_t Pre_Timeout_NA : 1;
  uint16_t Pre_Owner_NA : 1;
  uint16_t Pre_Scheduler_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Action : 3;
} CPthreadReqMutexTimedlock_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutex-timedlock test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `mutex` parameter value.
   */
  pthread_mutex_t *mutex;

  /**
   * @brief This member specifies the `abstime` parameter value.
   */
  struct timespec abstime;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutex_timedlock() call.
   */
  int status;

  /**
   * @brief This member specifies the priority ceiling used for the mutex.
   */
  int prio_ceiling;

  /**
   * @brief This member provides the storage for a mutex which is not the
   *   statically allocated PTHREAD_MUTEX_INITIALIZER.
   */
  pthread_mutex_t mutex_storage;

  /**
   * @brief This member is true, if the mutex_storage member contains an
   *   initialized mutex object.
   */
  bool mutex_initialized;

  /**
   * @brief This member specifies the locking protocol used to initialize the
   *   mutex.
   */
  int protocol;

  /**
   * @brief This member contains the thread queue test context.
   */
  TQMtxContext tq_mtx_ctx;

  /**
   * @brief This member specifies the mutex type.
   */
  int type;

  /**
   * @brief This member is true, if the call is not delegated to the thread
   *   queue requirement.
   */
  bool direct;

  /**
   * @brief This member is true, if another thread owns the mutex.
   */
  bool owner_other;

  /**
   * @brief This member is true, if the home scheduler instance of the calling
   *   thread is not the scheduler instance of the mutex.
   */
  bool scheduler_other;

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 7 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 7 ];

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
  { "None", "Inherit", "Ceiling", "NA" };

static const char *const CPthreadReqMutexTimedlock_PreDesc_Ceiling[] =
  { "Satisfied", "Violated", "None", "NA" };

static const char *const CPthreadReqMutexTimedlock_PreDesc_Type[] =
  { "Normal", "Recursive", "NA" };

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
  CPthreadReqMutexTimedlock_PreDesc_Ceiling,
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
       * While the mutex uses no locking protocol.
       */
      ctx->protocol = PTHREAD_PRIO_NONE;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_NO_PROTOCOL;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_FIFO;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Protocol_Inherit: {
      /*
       * While the mutex uses the priority inheritance locking protocol.
       */
      ctx->protocol = PTHREAD_PRIO_INHERIT;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_PRIORITY_INHERIT;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_PRIORITY;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Protocol_Ceiling: {
      /*
       * While the mutex uses the priority ceiling locking protocol.
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

static void CPthreadReqMutexTimedlock_Pre_Ceiling_Prepare(
  CPthreadReqMutexTimedlock_Context    *ctx,
  CPthreadReqMutexTimedlock_Pre_Ceiling state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Pre_Ceiling_Satisfied: {
      /*
       * While the mutex uses the priority ceiling locking protocol, while the
       * calling thread's current priority does not violate the priority
       * ceiling of the mutex.
       */
      if ( ctx->protocol != PTHREAD_PRIO_PROTECT ) {
        ctx->Map.skip = true;
      }

      {
        const Scheduler_Control *scheduler;

        scheduler = _Thread_Scheduler_get_home( _Thread_Get_executing() );
        ctx->prio_ceiling = _POSIX_Priority_From_core(
          scheduler,
          _Scheduler_Map_priority( scheduler, PRIO_VERY_HIGH )
        );
      }
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Ceiling_Violated: {
      /*
       * While the mutex uses the priority ceiling locking protocol, while the
       * calling thread's current priority violates the priority ceiling of the
       * mutex.
       */
      if ( ctx->protocol != PTHREAD_PRIO_PROTECT ) {
        ctx->Map.skip = true;
      }

      ctx->prio_ceiling = GetPOSIXPriorityMinimum();
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Ceiling_None: {
      /*
       * Where the mutex does not use the priority ceiling locking protocol.
       */
      if ( ctx->protocol == PTHREAD_PRIO_PROTECT ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Ceiling_NA:
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
       * While the type of the mutex is not recursive.
       */
      ctx->type = PTHREAD_MUTEX_DEFAULT;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Type_Recursive: {
      /*
       * While the type of the mutex is recursive.
       */
      ctx->type = PTHREAD_MUTEX_RECURSIVE;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_ALLOWED;
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
      ctx->mutex_initialized = false;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Mutex_Invalid: {
      /*
       * While the mutex referenced by the `mutex` parameter is invalid.
       */
      T_eq_int( pthread_mutex_init( &ctx->mutex_storage, NULL ), 0 );
      T_eq_int( pthread_mutex_destroy( &ctx->mutex_storage ), 0 );
      ctx->mutex = &ctx->mutex_storage;
      ctx->mutex_initialized = false;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Mutex_Static: {
      /*
       * While the object referenced by the `mutex` parameter is a statically
       * initialized mutex.
       */
      ctx->mutex_storage = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
      ctx->mutex = &ctx->mutex_storage;
      ctx->mutex_initialized = true;

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

      if ( ctx->protocol == PTHREAD_PRIO_PROTECT ) {
        T_eq_int(
          pthread_mutexattr_setprioceiling( &attr, ctx->prio_ceiling ),
          0
        );
      }

      T_eq_int( pthread_mutex_init( &ctx->mutex_storage, &attr ), 0 );
      T_eq_int( pthread_mutexattr_destroy( &attr ), 0 );

      ctx->mutex = &ctx->mutex_storage;
      ctx->mutex_initialized = true;
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
       * While the `abstime` parameter specifies a valid time point which is
       * later than the current time of CLOCK_REALTIME.
       */
      T_eq_int( clock_gettime( CLOCK_REALTIME, &ctx->abstime ), 0 );
      ctx->abstime.tv_sec += 3600;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Timeout_Past: {
      /*
       * While the `abstime` parameter specifies a valid time point which is
       * not later than the current time of CLOCK_REALTIME.
       */
      T_eq_int( clock_gettime( CLOCK_REALTIME, &ctx->abstime ), 0 );
      ctx->abstime.tv_sec -= 1;
      ctx->direct = true;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Timeout_Invalid: {
      /*
       * While the `abstime` parameter does not specify a valid time point.
       */
      ctx->abstime.tv_sec = 0;
      ctx->abstime.tv_nsec = -1;
      ctx->direct = true;
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
       * While the mutex is owned by another thread.
       */
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.base.wait = TQ_WAIT_FOREVER;
      ctx->owner_other = true;
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
       * While the scheduler instance of the mutex is the home scheduler of the
       * calling thread.
       */
      ctx->scheduler_other = false;
      break;
    }

    case CPthreadReqMutexTimedlock_Pre_Scheduler_Other: {
      /*
       * While the scheduler instance of the mutex is not the home scheduler of
       * the calling thread.
       */
      #if defined( RTEMS_SMP )
      SetSelfScheduler( SCHEDULER_B_ID, PRIO_VERY_LOW );
      ctx->scheduler_other = true;
      #else
      ctx->Map.skip = true;
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

static void CPthreadReqMutexTimedlock_Post_Action_Check(
  CPthreadReqMutexTimedlock_Context    *ctx,
  CPthreadReqMutexTimedlock_Post_Action state
)
{
  switch ( state ) {
    case CPthreadReqMutexTimedlock_Post_Action_MtxNone: {
      /*
       * The calling thread shall attempt to seize the mutex referenced by the
       * `mutex` parameter as specified by spec:/score/mtx/req/seize-wait where
       * the mutex does not use a locking protocol, the thread queue of the
       * mutex uses the FIFO discipline, a detected deadlock results in a
       * return with a status code, and a recursive seize of the mutex results
       * in a deadlock.
       */
      T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.base.wait = TQ_WAIT_TIMED;
      ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Action_MtxNoneRecursive: {
      /*
       * The calling thread shall attempt to seize the mutex referenced by the
       * `mutex` parameter as specified by spec:/score/mtx/req/seize-wait where
       * the mutex does not use a locking protocol, the thread queue of the
       * mutex uses the FIFO discipline, a detected deadlock results in a
       * return with a status code, and a recursive seize of the mutex is
       * allowed.
       */
      T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.base.wait = TQ_WAIT_TIMED;
      ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Action_MtxInherit: {
      /*
       * The calling thread shall attempt to seize the mutex referenced by the
       * `mutex` parameter as specified by spec:/score/mtx/req/seize-wait where
       * the mutex uses the priority inheritance locking protocol, the thread
       * queue of the mutex uses the priority discipline, a detected deadlock
       * results in a return with a status code, and a recursive seize of the
       * mutex results in a deadlock.
       */
      T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.base.wait = TQ_WAIT_TIMED;
      ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Action_MtxInheritRecursive: {
      /*
       * The calling thread shall attempt to seize the mutex referenced by the
       * `mutex` parameter as specified by spec:/score/mtx/req/seize-wait where
       * the mutex uses the priority inheritance locking protocol, the thread
       * queue of the mutex uses the priority discipline, a detected deadlock
       * results in a return with a status code, and a recursive seize of the
       * mutex is allowed.
       */
      T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.base.wait = TQ_WAIT_TIMED;
      ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Action_MtxCeiling: {
      /*
       * The calling thread shall attempt to seize the mutex referenced by the
       * `mutex` parameter as specified by spec:/score/mtx/req/seize-wait where
       * the mutex uses the priority ceiling locking protocol, the thread queue
       * of the mutex uses the priority discipline, a detected deadlock results
       * in a return with a status code, and a recursive seize of the mutex
       * results in a deadlock.
       */
      T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.base.wait = TQ_WAIT_TIMED;
      ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Action_MtxCeilingRecursive: {
      /*
       * The calling thread shall attempt to seize the mutex referenced by the
       * `mutex` parameter as specified by spec:/score/mtx/req/seize-wait where
       * the mutex uses the priority ceiling locking protocol, the thread queue
       * of the mutex uses the priority discipline, a detected deadlock results
       * in a return with a status code, and a recursive seize of the mutex is
       * allowed.
       */
      T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.base.wait = TQ_WAIT_TIMED;
      ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexTimedlock_Post_Action_MtxStatic: {
      /*
       * The calling thread shall attempt to seize the mutex referenced by the
       * `mutex` parameter as specified by spec:/score/mtx/req/seize-wait where
       * the mutex does not use a locking protocol, the thread queue of the
       * mutex uses the FIFO discipline, a detected deadlock results in a
       * return with a status code, and a recursive seize of the mutex results
       * in a deadlock.
       */
      T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
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
  memset( ctx, 0, sizeof( *ctx ) );
  ctx->tq_mtx_ctx.base.enqueue_prepare = TQEnqueuePrepareDefault;
  ctx->tq_mtx_ctx.base.enqueue_done = TQEnqueueDoneDefault;
  ctx->tq_mtx_ctx.base.enqueue = TQEnqueuePOSIXMutex;
  ctx->tq_mtx_ctx.base.surrender = TQSurrenderPOSIXMutex;
  ctx->tq_mtx_ctx.base.get_owner = TQGetOwnerPOSIXMutex;
  ctx->tq_mtx_ctx.base.convert_status = TQConvertStatusPOSIX;
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
  ctx->mutex_initialized = false;
  ctx->type = PTHREAD_MUTEX_DEFAULT;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
  ctx->direct = false;
  ctx->owner_other = false;
  ctx->scheduler_other = false;
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
    SetSelfScheduler( SCHEDULER_A_ID, PRIO_VERY_LOW );
  }

  if ( ctx->mutex_initialized ) {
    if ( ctx->owner_other ) {
      TQSend( &ctx->tq_mtx_ctx.base, TQ_BLOCKER_B, TQ_EVENT_SURRENDER );
    } else if ( ctx->direct && ctx->status == 0 ) {
      T_eq_int( pthread_mutex_unlock( &ctx->mutex_storage ), 0 );
    }

    T_eq_int( pthread_mutex_destroy( &ctx->mutex_storage ), 0 );
  }

  ctx->mutex_initialized = false;
}

/* clang-format off */

static const CPthreadReqMutexTimedlock_Entry
CPthreadReqMutexTimedlock_Entries[] = {
  { 0, 0, 0, 1, 0, 1, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Inval,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 1, 0, 1, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxNone },
  { 0, 0, 0, 1, 0, 1, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxInherit },
  { 0, 0, 0, 1, 0, 1, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxCeiling },
  { 0, 0, 0, 0, 0, 0, 0, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, 1, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, 0, 0, 1, CPthreadReqMutexTimedlock_Post_Status_TimedOut,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, 0, 0, 1, CPthreadReqMutexTimedlock_Post_Status_Inval,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, 1, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Inval,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, 0, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxNone },
  { 0, 0, 0, 0, 0, 0, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxNoneRecursive },
  { 0, 0, 0, 0, 0, 0, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxInherit },
  { 0, 0, 0, 0, 0, 0, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxInheritRecursive },
  { 0, 0, 0, 0, 0, 0, 1, 0, CPthreadReqMutexTimedlock_Post_Status_Inval,
    CPthreadReqMutexTimedlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, 0, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxCeiling },
  { 0, 0, 0, 0, 0, 0, 1, 1, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxCeilingRecursive },
  { 0, 0, 0, 0, 0, 0, 1, 0, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxCeiling },
  { 0, 0, 0, 0, 0, 0, 1, 0, CPthreadReqMutexTimedlock_Post_Status_Ok,
    CPthreadReqMutexTimedlock_Post_Action_MtxCeilingRecursive }
};

static const uint8_t
CPthreadReqMutexTimedlock_Map[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 4, 4, 6, 6, 4, 4, 7, 7, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 10, 10, 10, 10, 4, 4, 6, 6, 4, 4, 7, 7, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9,
  4, 4, 6, 6, 4, 4, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 10, 10, 10, 10, 4, 4,
  6, 6, 4, 4, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 11, 11, 11, 11, 4, 4, 6, 6,
  4, 4, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 12, 12, 12, 12, 4, 4, 6, 6, 4, 4,
  7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 11, 11, 11, 11, 4, 4, 6, 6, 4, 4, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 12, 12, 12, 12, 4, 4, 6, 6, 4, 4, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  16, 13, 16, 13, 4, 4, 6, 6, 4, 4, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 17,
  13, 17, 13, 4, 4, 6, 6, 4, 4, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 14, 14, 14, 14, 4, 4, 6, 6, 4, 4,
  7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 15, 15, 15, 15, 4, 4, 6, 6, 4, 4, 7, 7
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

static const uint16_t CPthreadReqMutexTimedlock_Weights[] =
  { 288, 96, 48, 12, 4, 2, 1 };

static void CPthreadReqMutexTimedlock_Skip(
  CPthreadReqMutexTimedlock_Context *ctx,
  size_t                             index
)
{
  switch ( index + 1 ) {
    case 1:
      ctx->Map.pci[ 1 ] = CPthreadReqMutexTimedlock_Pre_Ceiling_NA - 1;
      /* Fall through */
    case 2:
      ctx->Map.pci[ 2 ] = CPthreadReqMutexTimedlock_Pre_Type_NA - 1;
      /* Fall through */
    case 3:
      ctx->Map.pci[ 3 ] = CPthreadReqMutexTimedlock_Pre_Mutex_NA - 1;
      /* Fall through */
    case 4:
      ctx->Map.pci[ 4 ] = CPthreadReqMutexTimedlock_Pre_Timeout_NA - 1;
      /* Fall through */
    case 5:
      ctx->Map.pci[ 5 ] = CPthreadReqMutexTimedlock_Pre_Owner_NA - 1;
      /* Fall through */
    case 6:
      ctx->Map.pci[ 6 ] = CPthreadReqMutexTimedlock_Pre_Scheduler_NA - 1;
      break;
  }
}

static inline CPthreadReqMutexTimedlock_Entry
CPthreadReqMutexTimedlock_PopEntry( CPthreadReqMutexTimedlock_Context *ctx )
{
  size_t index;

  if ( ctx->Map.skip ) {
    size_t i;

    ctx->Map.skip = false;
    index = 0;

    for ( i = 0; i < 7; ++i ) {
      index += CPthreadReqMutexTimedlock_Weights[ i ] * ctx->Map.pci[ i ];
    }
  } else {
    index = ctx->Map.index;
  }

  ctx->Map.index = index + 1;

  return CPthreadReqMutexTimedlock_Entries
    [ CPthreadReqMutexTimedlock_Map[ index ] ];
}

static void CPthreadReqMutexTimedlock_SetPreConditionStates(
  CPthreadReqMutexTimedlock_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];

  if ( ctx->Map.entry.Pre_Type_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqMutexTimedlock_Pre_Type_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];

  if ( ctx->Map.entry.Pre_Timeout_NA ) {
    ctx->Map.pcs[ 4 ] = CPthreadReqMutexTimedlock_Pre_Timeout_NA;
  } else {
    ctx->Map.pcs[ 4 ] = ctx->Map.pci[ 4 ];
  }

  if ( ctx->Map.entry.Pre_Owner_NA ) {
    ctx->Map.pcs[ 5 ] = CPthreadReqMutexTimedlock_Pre_Owner_NA;
  } else {
    ctx->Map.pcs[ 5 ] = ctx->Map.pci[ 5 ];
  }

  if ( ctx->Map.entry.Pre_Scheduler_NA ) {
    ctx->Map.pcs[ 6 ] = CPthreadReqMutexTimedlock_Pre_Scheduler_NA;
  } else {
    ctx->Map.pcs[ 6 ] = ctx->Map.pci[ 6 ];
  }
}

static void CPthreadReqMutexTimedlock_TestVariant(
  CPthreadReqMutexTimedlock_Context *ctx
)
{
  CPthreadReqMutexTimedlock_Pre_Protocol_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexTimedlock_Pre_Ceiling_Prepare( ctx, ctx->Map.pcs[ 1 ] );

  if ( ctx->Map.skip ) {
    CPthreadReqMutexTimedlock_Skip( ctx, 1 );
    return;
  }

  CPthreadReqMutexTimedlock_Pre_Type_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexTimedlock_Pre_Mutex_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqMutexTimedlock_Pre_Timeout_Prepare( ctx, ctx->Map.pcs[ 4 ] );
  CPthreadReqMutexTimedlock_Pre_Owner_Prepare( ctx, ctx->Map.pcs[ 5 ] );
  CPthreadReqMutexTimedlock_Pre_Scheduler_Prepare( ctx, ctx->Map.pcs[ 6 ] );

  if ( ctx->Map.skip ) {
    CPthreadReqMutexTimedlock_Skip( ctx, 6 );
    return;
  }

  CPthreadReqMutexTimedlock_Action( ctx );
  CPthreadReqMutexTimedlock_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
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
  ctx->Map.skip = false;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexTimedlock_Pre_Protocol_None;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexTimedlock_Pre_Protocol_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexTimedlock_Pre_Ceiling_Satisfied;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexTimedlock_Pre_Ceiling_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexTimedlock_Pre_Type_Normal;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexTimedlock_Pre_Type_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexTimedlock_Pre_Mutex_Null;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexTimedlock_Pre_Mutex_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          for (
            ctx->Map.pci[ 4 ] = CPthreadReqMutexTimedlock_Pre_Timeout_Future;
            ctx->Map.pci[ 4 ] < CPthreadReqMutexTimedlock_Pre_Timeout_NA;
            ++ctx->Map.pci[ 4 ]
          ) {
            for (
              ctx->Map.pci[ 5 ] = CPthreadReqMutexTimedlock_Pre_Owner_None;
              ctx->Map.pci[ 5 ] < CPthreadReqMutexTimedlock_Pre_Owner_NA;
              ++ctx->Map.pci[ 5 ]
            ) {
              for (
                ctx->Map.pci[ 6 ] =
                  CPthreadReqMutexTimedlock_Pre_Scheduler_Home;
                ctx->Map.pci[ 6 ] < CPthreadReqMutexTimedlock_Pre_Scheduler_NA;
                ++ctx->Map.pci[ 6 ]
              ) {
                ctx->Map.entry = CPthreadReqMutexTimedlock_PopEntry( ctx );
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
}

/** @} */
