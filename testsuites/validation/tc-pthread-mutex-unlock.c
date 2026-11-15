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
#include <rtems/posix/priorityimpl.h>
#include <rtems/score/schedulerimpl.h>
#include <rtems/score/threadimpl.h>

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
  CPthreadReqMutexUnlock_Pre_Protocol_Ceiling,
  CPthreadReqMutexUnlock_Pre_Protocol_NA
} CPthreadReqMutexUnlock_Pre_Protocol;

typedef enum {
  CPthreadReqMutexUnlock_Pre_Owner_Caller,
  CPthreadReqMutexUnlock_Pre_Owner_Other,
  CPthreadReqMutexUnlock_Pre_Owner_None,
  CPthreadReqMutexUnlock_Pre_Owner_NA
} CPthreadReqMutexUnlock_Pre_Owner;

typedef enum {
  CPthreadReqMutexUnlock_Pre_Type_Normal,
  CPthreadReqMutexUnlock_Pre_Type_Recursive,
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
  CPthreadReqMutexUnlock_Post_Status_Ok,
  CPthreadReqMutexUnlock_Post_Status_Inval,
  CPthreadReqMutexUnlock_Post_Status_Perm,
  CPthreadReqMutexUnlock_Post_Status_NA
} CPthreadReqMutexUnlock_Post_Status;

typedef enum {
  CPthreadReqMutexUnlock_Post_Action_MtxNone,
  CPthreadReqMutexUnlock_Post_Action_MtxNoneRecursive,
  CPthreadReqMutexUnlock_Post_Action_MtxInherit,
  CPthreadReqMutexUnlock_Post_Action_MtxInheritRecursive,
  CPthreadReqMutexUnlock_Post_Action_MtxCeiling,
  CPthreadReqMutexUnlock_Post_Action_MtxCeilingRecursive,
  CPthreadReqMutexUnlock_Post_Action_NA
} CPthreadReqMutexUnlock_Post_Action;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Protocol_NA : 1;
  uint16_t Pre_Owner_NA : 1;
  uint16_t Pre_Type_NA : 1;
  uint16_t Pre_Mutex_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Action : 3;
} CPthreadReqMutexUnlock_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutex-unlock test case.
 */
typedef struct {
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
   * @brief This member is true, if the calling thread owns the mutex before
   *   the directive call.
   */
  bool owner_caller;

  /**
   * @brief This member is true, if a thread other than the calling thread owns
   *   the mutex before the directive call.
   */
  bool owner_other;

  /**
   * @brief This member contains the thread queue test context.
   */
  TQMtxContext tq_mtx_ctx;

  /**
   * @brief This member specifies the mutex type.
   */
  int type;

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
  { "None", "Inherit", "Ceiling", "NA" };

static const char *const CPthreadReqMutexUnlock_PreDesc_Owner[] =
  { "Caller", "Other", "None", "NA" };

static const char *const CPthreadReqMutexUnlock_PreDesc_Type[] =
  { "Normal", "Recursive", "NA" };

static const char *const CPthreadReqMutexUnlock_PreDesc_Mutex[] =
  { "Null", "Invalid", "Static", "Init", "NA" };

static const char *const *const CPthreadReqMutexUnlock_PreDesc[] = {
  CPthreadReqMutexUnlock_PreDesc_Protocol,
  CPthreadReqMutexUnlock_PreDesc_Owner,
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
       * While the mutex uses no locking protocol.
       */
      ctx->protocol = PTHREAD_PRIO_NONE;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_NO_PROTOCOL;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_FIFO;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Protocol_Inherit: {
      /*
       * While the mutex uses the priority inheritance locking protocol.
       */
      ctx->protocol = PTHREAD_PRIO_INHERIT;
      ctx->tq_mtx_ctx.protocol = TQ_MTX_PRIORITY_INHERIT;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_PRIORITY;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Protocol_Ceiling: {
      /*
       * While the mutex uses the priority ceiling locking protocol.
       */
      {
        const Scheduler_Control *scheduler;

        scheduler = _Thread_Scheduler_get_home( _Thread_Get_executing() );
        ctx->prio_ceiling = _POSIX_Priority_From_core(
          scheduler,
          _Scheduler_Map_priority( scheduler, PRIO_VERY_HIGH )
        );
      }

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

static void CPthreadReqMutexUnlock_Pre_Owner_Prepare(
  CPthreadReqMutexUnlock_Context  *ctx,
  CPthreadReqMutexUnlock_Pre_Owner state
)
{
  switch ( state ) {
    case CPthreadReqMutexUnlock_Pre_Owner_Caller: {
      /*
       * While the owner of the mutex is the calling thread.
       */
      ctx->owner_caller = true;
      ctx->owner_other = false;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Owner_Other: {
      /*
       * While the owner of the mutex is a thread other than the calling
       * thread.
       */
      ctx->owner_caller = false;
      ctx->owner_other = true;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Owner_None: {
      /*
       * While the mutex has no owner.
       */
      ctx->owner_caller = false;
      ctx->owner_other = false;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Owner_NA:
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
       * While the type of the mutex is not recursive.
       */
      ctx->type = PTHREAD_MUTEX_DEFAULT;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Type_Recursive: {
      /*
       * While the type of the mutex is recursive.
       */
      ctx->type = PTHREAD_MUTEX_RECURSIVE;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_ALLOWED;
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
      ctx->mutex_initialized = false;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Mutex_Invalid: {
      /*
       * While the mutex referenced by the `mutex` parameter is invalid.
       */
      T_eq_int( pthread_mutex_init( &ctx->mutex_storage, NULL ), 0 );
      T_eq_int( pthread_mutex_destroy( &ctx->mutex_storage ), 0 );
      ctx->mutex = &ctx->mutex_storage;
      ctx->mutex_initialized = false;
      break;
    }

    case CPthreadReqMutexUnlock_Pre_Mutex_Static: {
      /*
       * While the object referenced by the `mutex` parameter is a statically
       * initialized mutex.
       */
      ctx->mutex_storage = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
      ctx->mutex = &ctx->mutex_storage;
      ctx->mutex_initialized = true;
      ctx->owner_caller = false;
      ctx->owner_other = false;

      ctx->tq_mtx_ctx.protocol = TQ_MTX_NO_PROTOCOL;
      ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
      ctx->tq_mtx_ctx.owner_check = TQ_MTX_CHECKS_OWNER;
      ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;
      ctx->tq_mtx_ctx.base.discipline = TQ_FIFO;
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

      ctx->tq_mtx_ctx.owner_check = TQ_MTX_CHECKS_OWNER;
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ctx->tq_mtx_ctx.base.wait = TQ_WAIT_FOREVER;

      if ( ctx->owner_caller ) {
        T_eq_int( pthread_mutex_lock( ctx->mutex ), 0 );
      } else if ( ctx->owner_other ) {
        TQSend( &ctx->tq_mtx_ctx.base, TQ_BLOCKER_B, TQ_EVENT_ENQUEUE );
      }
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
    case CPthreadReqMutexUnlock_Post_Status_Ok: {
      /*
       * The return value of pthread_mutex_unlock() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

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
    case CPthreadReqMutexUnlock_Post_Action_MtxNone: {
      /*
       * The calling thread shall surrender the mutex referenced by the `mutex`
       * parameter as specified by spec:/score/mtx/req/surrender where the
       * mutex does not use a locking protocol, the thread queue of the mutex
       * uses the FIFO discipline, a recursive seize of the mutex is not
       * allowed, and the surrender checks that the mutex owner is the calling
       * thread.
       */
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ScoreMtxReqSurrender_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexUnlock_Post_Action_MtxNoneRecursive: {
      /*
       * The calling thread shall surrender the mutex referenced by the `mutex`
       * parameter as specified by spec:/score/mtx/req/surrender where the
       * mutex does not use a locking protocol, the thread queue of the mutex
       * uses the FIFO discipline, a recursive seize of the mutex is allowed,
       * and the surrender checks that the mutex owner is the calling thread.
       */
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ScoreMtxReqSurrender_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexUnlock_Post_Action_MtxInherit: {
      /*
       * The calling thread shall surrender the mutex referenced by the `mutex`
       * parameter as specified by spec:/score/mtx/req/surrender where the
       * mutex uses the priority inheritance locking protocol, the thread queue
       * of the mutex uses the priority discipline, a recursive seize of the
       * mutex is not allowed, and the surrender checks that the mutex owner is
       * the calling thread.
       */
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ScoreMtxReqSurrender_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexUnlock_Post_Action_MtxInheritRecursive: {
      /*
       * The calling thread shall surrender the mutex referenced by the `mutex`
       * parameter as specified by spec:/score/mtx/req/surrender where the
       * mutex uses the priority inheritance locking protocol, the thread queue
       * of the mutex uses the priority discipline, a recursive seize of the
       * mutex is allowed, and the surrender checks that the mutex owner is the
       * calling thread.
       */
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ScoreMtxReqSurrender_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexUnlock_Post_Action_MtxCeiling: {
      /*
       * The calling thread shall surrender the mutex referenced by the `mutex`
       * parameter as specified by spec:/score/mtx/req/surrender where the
       * mutex uses the priority ceiling locking protocol, the thread queue of
       * the mutex uses the priority discipline, a recursive seize of the mutex
       * is not allowed, and the surrender checks that the mutex owner is the
       * calling thread.
       */
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ScoreMtxReqSurrender_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexUnlock_Post_Action_MtxCeilingRecursive: {
      /*
       * The calling thread shall surrender the mutex referenced by the `mutex`
       * parameter as specified by spec:/score/mtx/req/surrender where the
       * mutex uses the priority ceiling locking protocol, the thread queue of
       * the mutex uses the priority discipline, a recursive seize of the mutex
       * is allowed, and the surrender checks that the mutex owner is the
       * calling thread.
       */
      ctx->tq_mtx_ctx.base.thread_queue_object = ctx->mutex;
      ScoreMtxReqSurrender_Run( &ctx->tq_mtx_ctx );
      break;
    }

    case CPthreadReqMutexUnlock_Post_Action_NA:
      break;
  }
}

static void CPthreadReqMutexUnlock_Setup( CPthreadReqMutexUnlock_Context *ctx )
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
  ctx->mutex_initialized = false;
  ctx->owner_caller = false;
  ctx->owner_other = false;
  ctx->type = PTHREAD_MUTEX_DEFAULT;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
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
    if ( ctx->owner_other ) {
      TQSend( &ctx->tq_mtx_ctx.base, TQ_BLOCKER_B, TQ_EVENT_SURRENDER );
    }

    T_eq_int( pthread_mutex_destroy( &ctx->mutex_storage ), 0 );
  }
}

/* clang-format off */

static const CPthreadReqMutexUnlock_Entry
CPthreadReqMutexUnlock_Entries[] = {
  { 1, 0, 0, 0, 0, CPthreadReqMutexUnlock_Post_Status_NA,
    CPthreadReqMutexUnlock_Post_Action_NA },
  { 0, 0, 0, 1, 0, CPthreadReqMutexUnlock_Post_Status_Inval,
    CPthreadReqMutexUnlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, CPthreadReqMutexUnlock_Post_Status_Perm,
    CPthreadReqMutexUnlock_Post_Action_NA },
  { 0, 0, 0, 1, 0, CPthreadReqMutexUnlock_Post_Status_Perm,
    CPthreadReqMutexUnlock_Post_Action_NA },
  { 0, 0, 0, 0, 0, CPthreadReqMutexUnlock_Post_Status_Ok,
    CPthreadReqMutexUnlock_Post_Action_MtxNone },
  { 0, 0, 0, 0, 0, CPthreadReqMutexUnlock_Post_Status_Ok,
    CPthreadReqMutexUnlock_Post_Action_MtxNoneRecursive },
  { 0, 0, 0, 0, 0, CPthreadReqMutexUnlock_Post_Status_Ok,
    CPthreadReqMutexUnlock_Post_Action_MtxInherit },
  { 0, 0, 0, 0, 0, CPthreadReqMutexUnlock_Post_Status_Ok,
    CPthreadReqMutexUnlock_Post_Action_MtxInheritRecursive },
  { 0, 0, 0, 0, 0, CPthreadReqMutexUnlock_Post_Status_Ok,
    CPthreadReqMutexUnlock_Post_Action_MtxCeiling },
  { 0, 0, 0, 0, 0, CPthreadReqMutexUnlock_Post_Status_Ok,
    CPthreadReqMutexUnlock_Post_Action_MtxCeilingRecursive }
};

static const uint8_t
CPthreadReqMutexUnlock_Map[] = {
  1, 1, 3, 4, 0, 0, 0, 5, 1, 1, 3, 2, 0, 0, 0, 2, 1, 1, 3, 2, 0, 0, 0, 2, 1, 1,
  3, 6, 0, 0, 0, 7, 1, 1, 3, 2, 0, 0, 0, 2, 1, 1, 3, 2, 0, 0, 0, 2, 1, 1, 3, 8,
  0, 0, 0, 9, 1, 1, 3, 2, 0, 0, 0, 2, 1, 1, 3, 2, 0, 0, 0, 2
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
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];

  if ( ctx->Map.entry.Pre_Type_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqMutexUnlock_Pre_Type_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
}

static void CPthreadReqMutexUnlock_TestVariant(
  CPthreadReqMutexUnlock_Context *ctx
)
{
  CPthreadReqMutexUnlock_Pre_Protocol_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexUnlock_Pre_Owner_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CPthreadReqMutexUnlock_Pre_Type_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexUnlock_Pre_Mutex_Prepare( ctx, ctx->Map.pcs[ 3 ] );
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
      ctx->Map.pci[ 1 ] = CPthreadReqMutexUnlock_Pre_Owner_Caller;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexUnlock_Pre_Owner_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexUnlock_Pre_Type_Normal;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexUnlock_Pre_Type_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexUnlock_Pre_Mutex_Null;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexUnlock_Pre_Mutex_NA;
          ++ctx->Map.pci[ 3 ]
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
}

/** @} */
