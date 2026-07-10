/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup NewlibValSysLock
 */

/*
 * Copyright (C) 2021, 2025 embedded brains GmbH & Co. KG
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
#include <string.h>
#include <sys/lock.h>

#include "tr-mtx-seize-try.h"
#include "tr-mtx-seize-wait.h"
#include "tr-mtx-surrender.h"
#include "tr-sem-seize-try.h"
#include "tr-sem-seize-wait.h"
#include "tr-sem-surrender.h"
#include "tr-tq-timeout-priority-inherit.h"
#include "tr-tq-timeout.h"
#include "tx-thread-queue.h"

#include <rtems/test.h>

/**
 * @defgroup NewlibValSysLock spec:/newlib/val/sys-lock
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @brief Tests the `<sys/lock.h>` mutex and semaphore directives.
 *
 * This test case performs the following actions:
 *
 * - Create a mutex and validate the mutex directives.
 *
 *   - Validate the _Mutex_Try_acquire() directive.
 *
 *   - Validate the _Mutex_Acquire_timed() directive for valid timeout
 *     parameters.
 *
 *   - Validate the _Mutex_Acquire_timed() directive for an invalid timeout
 *     parameter.
 *
 *   - Validate the _Mutex_Acquire_timed() timeout behaviour.
 *
 *   - Validate the _Mutex_Acquire() directive.
 *
 *   - Validate the _Mutex_Release() directive.
 *
 *   - Destroy the mutex.
 *
 * - Create a recursive mutex and validate the mutex directives.
 *
 *   - Validate the _Mutex_recursive_Try_acquire() directive.
 *
 *   - Validate the _Mutex_recursive_Acquire_timed() directive for valid
 *     timeout parameters.
 *
 *   - Validate the _Mutex_recursive_Acquire_timed() directive for an invalid
 *     timeout parameter.
 *
 *   - Validate the _Mutex_recursive_Acquire_timed() timeout behaviour.
 *
 *   - Validate the _Mutex_recursive_Acquire() directive.
 *
 *   - Validate the _Mutex_recursive_Release() directive.
 *
 *   - Destroy the mutex.
 *
 * - Create a semaphore and validate the semaphore directives.
 *
 *   - Validate the _Semaphore_Try_wait() directive.
 *
 *   - Validate the _Semaphore_Wait_timed_ticks() directive for valid timeout
 *     parameters.
 *
 *   - Validate the _Semaphore_Wait_timed_ticks() timeout behaviour.
 *
 *   - Validate the _Semaphore_Wait_timed_ticks() directive for a timeout
 *     parameter of zero.
 *
 *   - Validate the _Semaphore_Wait() directive.
 *
 *   - Validate the _Semaphore_Post() directive.
 *
 *   - Validate the _Semaphore_Post_binary() directive.
 *
 *   - Destroy the semaphore.
 *
 * @{
 */

/**
 * @brief Test context for spec:/newlib/val/sys-lock test case.
 */
typedef struct {
  /**
   * @brief This member contains the thread queue test context.
   */
  union {
    TQContext    tq_ctx;
    TQMtxContext tq_mtx_ctx;
    TQSemContext tq_sem_ctx;
  };
} NewlibValSysLock_Context;

static NewlibValSysLock_Context NewlibValSysLock_Instance;

static Status_Control MutexEnqueue( TQContext *ctx, TQWait wait )
{
  const struct timespec abstime = { .tv_sec = INT64_MAX, .tv_nsec = 0 };
  int                   eno;

  switch ( wait ) {
    case TQ_NO_WAIT:
      eno = _Mutex_Try_acquire( ctx->thread_queue_object );
      break;
    case TQ_WAIT_FOREVER:
      _Mutex_Acquire( ctx->thread_queue_object );
      eno = 0;
      break;
    case TQ_WAIT_TIMED:
      eno = _Mutex_Acquire_timed( ctx->thread_queue_object, &abstime );
      break;
    default:
      T_unreachable();
      break;
  }

  return STATUS_BUILD( 0, eno );
}

static Status_Control MutexSurrender( TQContext *ctx )
{
  _Mutex_Release( ctx->thread_queue_object );

  return STATUS_SUCCESSFUL;
}

static rtems_tcb *GetOwner( TQContext *ctx )
{
  const struct _Thread_queue_Queue *tq;

  tq = ctx->thread_queue_object;

  return tq->_owner;
}

static Status_Control RecursiveEnqueue( TQContext *ctx, TQWait wait )
{
  const struct timespec abstime = { .tv_sec = INT64_MAX, .tv_nsec = 0 };
  int                   eno;

  switch ( wait ) {
    case TQ_NO_WAIT:
      eno = _Mutex_recursive_Try_acquire( ctx->thread_queue_object );
      break;
    case TQ_WAIT_FOREVER:
      _Mutex_recursive_Acquire( ctx->thread_queue_object );
      eno = 0;
      break;
    case TQ_WAIT_TIMED:
      eno = _Mutex_recursive_Acquire_timed(
        ctx->thread_queue_object,
        &abstime
      );
      break;
    default:
      T_unreachable();
      break;
  }

  return STATUS_BUILD( 0, eno );
}

static Status_Control RecursiveSurrender( TQContext *ctx )
{
  _Mutex_recursive_Release( ctx->thread_queue_object );

  return STATUS_SUCCESSFUL;
}

static Status_Control SemaphoreEnqueue( TQContext *ctx, TQWait wait )
{
  int eno;

  switch ( wait ) {
    case TQ_NO_WAIT:
      eno = _Semaphore_Try_wait( ctx->thread_queue_object );
      break;
    case TQ_WAIT_FOREVER:
      _Semaphore_Wait( ctx->thread_queue_object );
      eno = 0;
      break;
    case TQ_WAIT_TIMED:
      eno = _Semaphore_Wait_timed_ticks(
        ctx->thread_queue_object,
        UINT32_MAX
      );
      break;
    default:
      T_unreachable();
      break;
  }

  return STATUS_BUILD( 0, eno );
}

static Status_Control SemaphoreEnqueueZeroTicks( TQContext *ctx, TQWait wait )
{
  int eno;

  if ( wait == TQ_NO_WAIT ) {
    eno = _Semaphore_Try_wait( ctx->thread_queue_object );
  } else {
    eno = _Semaphore_Wait_timed_ticks( ctx->thread_queue_object, 0 );
  }

  return STATUS_BUILD( 0, eno );
}

static Status_Control SemaphorePost( TQContext *ctx )
{
  _Semaphore_Post( ctx->thread_queue_object );

  return STATUS_SUCCESSFUL;
}

static Status_Control SemaphorePostBinary( TQContext *ctx )
{
  _Semaphore_Post_binary( ctx->thread_queue_object );

  return STATUS_SUCCESSFUL;
}

static uint32_t SemaphoreGetCount( TQSemContext *ctx )
{
  const struct _Semaphore_Control *sem;

  sem = ctx->base.thread_queue_object;

  return sem->_count;
}

static void SemaphoreSetCount( TQSemContext *ctx, uint32_t count )
{
  struct _Semaphore_Control *sem;

  sem = ctx->base.thread_queue_object;
  sem->_count = count;
}

static void NewlibValSysLock_Setup( NewlibValSysLock_Context *ctx )
{
  memset( ctx, 0, sizeof( *ctx ) );
  ctx->tq_ctx.enqueue_variant = TQ_ENQUEUE_BLOCKS;
  ctx->tq_ctx.discipline = TQ_PRIORITY;
  ctx->tq_ctx.deadlock = TQ_DEADLOCK_FATAL;
  ctx->tq_ctx.convert_status = TQConvertStatusPOSIX;
  ctx->tq_ctx.enqueue_prepare = TQEnqueuePrepareDefault;
  ctx->tq_ctx.enqueue_done = TQEnqueueDoneDefault;
  ctx->tq_ctx.get_owner = GetOwner;
  TQInitialize( &ctx->tq_ctx );
}

static void NewlibValSysLock_Setup_Wrap( void *arg )
{
  NewlibValSysLock_Context *ctx;

  ctx = arg;
  NewlibValSysLock_Setup( ctx );
}

static void NewlibValSysLock_Teardown( NewlibValSysLock_Context *ctx )
{
  TQDestroy( &ctx->tq_ctx );
  RestoreRunnerPriority();
}

static void NewlibValSysLock_Teardown_Wrap( void *arg )
{
  NewlibValSysLock_Context *ctx;

  ctx = arg;
  NewlibValSysLock_Teardown( ctx );
}

static T_fixture NewlibValSysLock_Fixture = {
  .setup = NewlibValSysLock_Setup_Wrap,
  .stop = NULL,
  .teardown = NewlibValSysLock_Teardown_Wrap,
  .scope = NULL,
  .initial_context = &NewlibValSysLock_Instance
};

/**
 * @brief Create a mutex and validate the mutex directives.
 */
static void NewlibValSysLock_Action_0( NewlibValSysLock_Context *ctx )
{
  const struct timespec invalid_abstime = { .tv_sec = -1, .tv_nsec = -1 };
  int                   eno;
  struct _Mutex_Control mutex;

  _Mutex_Initialize( &mutex );

  ctx->tq_mtx_ctx.base.thread_queue_object = &mutex;
  ctx->tq_mtx_ctx.base.enqueue = MutexEnqueue;
  ctx->tq_mtx_ctx.base.surrender = MutexSurrender;
  ctx->tq_mtx_ctx.protocol = TQ_MTX_PRIORITY_INHERIT;
  ctx->tq_mtx_ctx.owner_check = TQ_MTX_NO_OWNER_CHECK;
  ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;

  /*
   * Validate the _Mutex_Try_acquire() directive.
   */
  ctx->tq_mtx_ctx.base.wait = TQ_NO_WAIT;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_UNAVAILABLE;
  ScoreMtxReqSeizeTry_Run( &ctx->tq_mtx_ctx );

  /*
   * Validate the _Mutex_Acquire_timed() directive for valid timeout
   * parameters.
   */
  ctx->tq_mtx_ctx.base.wait = TQ_WAIT_TIMED;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
  ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );

  /*
   * Validate the _Mutex_Acquire_timed() directive for an invalid timeout
   * parameter.
   */
  TQSetScheduler(
    &ctx->tq_mtx_ctx.base,
    TQ_HELPER_A,
    SCHEDULER_A_ID,
    PRIO_HIGH
  );
  TQSend( &ctx->tq_mtx_ctx.base, TQ_HELPER_A, TQ_EVENT_ENQUEUE );
  eno = _Mutex_Acquire_timed( &mutex, &invalid_abstime );
  T_eq_int( eno, EINVAL );
  TQSend( &ctx->tq_mtx_ctx.base, TQ_HELPER_A, TQ_EVENT_SURRENDER );

  /*
   * Validate the _Mutex_Acquire_timed() timeout behaviour.
   */
  ctx->tq_mtx_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
  ScoreTqReqTimeoutPriorityInherit_Run( &ctx->tq_mtx_ctx.base );

  /*
   * Validate the _Mutex_Acquire() directive.
   */
  ctx->tq_mtx_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
  ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );

  /*
   * Validate the _Mutex_Release() directive.
   */
  ctx->tq_mtx_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_DEADLOCK;
  ScoreMtxReqSurrender_Run( &ctx->tq_mtx_ctx );

  /*
   * Destroy the mutex.
   */
  _Mutex_Destroy( &mutex );
}

/**
 * @brief Create a recursive mutex and validate the mutex directives.
 */
static void NewlibValSysLock_Action_1( NewlibValSysLock_Context *ctx )
{
  const struct timespec invalid_abstime = { .tv_sec = -1, .tv_nsec = -1 };
  int                   eno;
  struct _Mutex_recursive_Control mutex;

  _Mutex_recursive_Initialize( &mutex );

  ctx->tq_mtx_ctx.base.thread_queue_object = &mutex;
  ctx->tq_mtx_ctx.base.enqueue = RecursiveEnqueue;
  ctx->tq_mtx_ctx.base.surrender = RecursiveSurrender;
  ctx->tq_mtx_ctx.protocol = TQ_MTX_PRIORITY_INHERIT;
  ctx->tq_mtx_ctx.owner_check = TQ_MTX_NO_OWNER_CHECK;
  ctx->tq_mtx_ctx.priority_ceiling = PRIO_INVALID;

  /*
   * Validate the _Mutex_recursive_Try_acquire() directive.
   */
  ctx->tq_mtx_ctx.base.wait = TQ_NO_WAIT;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_ALLOWED;
  ScoreMtxReqSeizeTry_Run( &ctx->tq_mtx_ctx );

  /*
   * Validate the _Mutex_recursive_Acquire_timed() directive for valid timeout
   * parameters.
   */
  ctx->tq_mtx_ctx.base.wait = TQ_WAIT_TIMED;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_ALLOWED;
  ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );

  /*
   * Validate the _Mutex_recursive_Acquire_timed() directive for an invalid
   * timeout parameter.
   */
  TQSetScheduler(
    &ctx->tq_mtx_ctx.base,
    TQ_HELPER_A,
    SCHEDULER_A_ID,
    PRIO_HIGH
  );
  TQSend( &ctx->tq_mtx_ctx.base, TQ_HELPER_A, TQ_EVENT_ENQUEUE );
  eno = _Mutex_recursive_Acquire_timed( &mutex, &invalid_abstime );
  T_eq_int( eno, EINVAL );
  TQSend( &ctx->tq_mtx_ctx.base, TQ_HELPER_A, TQ_EVENT_SURRENDER );

  /*
   * Validate the _Mutex_recursive_Acquire_timed() timeout behaviour.
   */
  ctx->tq_mtx_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_ALLOWED;
  ScoreTqReqTimeoutPriorityInherit_Run( &ctx->tq_mtx_ctx.base );

  /*
   * Validate the _Mutex_recursive_Acquire() directive.
   */
  ctx->tq_mtx_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_ALLOWED;
  ScoreMtxReqSeizeWait_Run( &ctx->tq_mtx_ctx );

  /*
   * Validate the _Mutex_recursive_Release() directive.
   */
  ctx->tq_mtx_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_mtx_ctx.recursive = TQ_MTX_RECURSIVE_ALLOWED;
  ScoreMtxReqSurrender_Run( &ctx->tq_mtx_ctx );

  /*
   * Destroy the mutex.
   */
  _Mutex_recursive_Destroy( &mutex );
}

/**
 * @brief Create a semaphore and validate the semaphore directives.
 */
static void NewlibValSysLock_Action_2( NewlibValSysLock_Context *ctx )
{
  struct _Semaphore_Control sem;

  _Semaphore_Initialize( &sem, 0 );

  ctx->tq_sem_ctx.base.thread_queue_object = &sem;
  ctx->tq_sem_ctx.base.enqueue = SemaphoreEnqueue;
  ctx->tq_sem_ctx.get_count = SemaphoreGetCount;
  ctx->tq_sem_ctx.set_count = SemaphoreSetCount;

  /*
   * Validate the _Semaphore_Try_wait() directive.
   */
  ctx->tq_sem_ctx.base.surrender = SemaphorePostBinary;
  ctx->tq_sem_ctx.base.wait = TQ_NO_WAIT;
  ctx->tq_sem_ctx.variant = TQ_SEM_BINARY;
  SemaphoreSetCount( &ctx->tq_sem_ctx, 1 );
  ScoreSemReqSeizeTry_Run( &ctx->tq_sem_ctx );

  /*
   * Validate the _Semaphore_Wait_timed_ticks() directive for valid timeout
   * parameters.
   */
  ctx->tq_sem_ctx.base.surrender = SemaphorePostBinary;
  ctx->tq_sem_ctx.base.wait = TQ_WAIT_TIMED;
  ctx->tq_sem_ctx.variant = TQ_SEM_BINARY;
  SemaphoreSetCount( &ctx->tq_sem_ctx, 1 );
  ScoreSemReqSeizeWait_Run( &ctx->tq_sem_ctx );

  /*
   * Validate the _Semaphore_Wait_timed_ticks() timeout behaviour.
   */
  ctx->tq_sem_ctx.base.surrender = SemaphorePostBinary;
  ctx->tq_sem_ctx.base.wait = TQ_WAIT_TIMED;
  ctx->tq_sem_ctx.variant = TQ_SEM_BINARY;
  SemaphoreSetCount( &ctx->tq_sem_ctx, 1 );
  ScoreTqReqTimeout_Run( &ctx->tq_sem_ctx.base );

  /*
   * Validate the _Semaphore_Wait_timed_ticks() directive for a timeout
   * parameter of zero.
   */
  ctx->tq_sem_ctx.base.enqueue = SemaphoreEnqueueZeroTicks;
  ctx->tq_sem_ctx.base.surrender = SemaphorePostBinary;
  ctx->tq_sem_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_sem_ctx.variant = TQ_SEM_BINARY;
  SemaphoreSetCount( &ctx->tq_sem_ctx, 1 );
  ScoreSemReqSeizeWait_Run( &ctx->tq_sem_ctx );
  ctx->tq_sem_ctx.base.enqueue = SemaphoreEnqueue;

  /*
   * Validate the _Semaphore_Wait() directive.
   */
  ctx->tq_sem_ctx.base.surrender = SemaphorePostBinary;
  ctx->tq_sem_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_sem_ctx.variant = TQ_SEM_BINARY;
  SemaphoreSetCount( &ctx->tq_sem_ctx, 1 );
  ScoreSemReqSeizeWait_Run( &ctx->tq_sem_ctx );

  /*
   * Validate the _Semaphore_Post() directive.
   */
  ctx->tq_sem_ctx.base.surrender = SemaphorePost;
  ctx->tq_sem_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_sem_ctx.variant = TQ_SEM_COUNTING_MODULO;
  SemaphoreSetCount( &ctx->tq_sem_ctx, 1 );
  ScoreSemReqSurrender_Run( &ctx->tq_sem_ctx );

  /*
   * Validate the _Semaphore_Post_binary() directive.
   */
  ctx->tq_sem_ctx.base.surrender = SemaphorePostBinary;
  ctx->tq_sem_ctx.base.wait = TQ_WAIT_FOREVER;
  ctx->tq_sem_ctx.variant = TQ_SEM_BINARY;
  SemaphoreSetCount( &ctx->tq_sem_ctx, 1 );
  ScoreSemReqSurrender_Run( &ctx->tq_sem_ctx );

  /*
   * Destroy the semaphore.
   */
  _Semaphore_Destroy( &sem );
}

/**
 * @fn void T_case_body_NewlibValSysLock( void )
 */
T_TEST_CASE_FIXTURE( NewlibValSysLock, &NewlibValSysLock_Fixture )
{
  NewlibValSysLock_Context *ctx;

  ctx = T_fixture_context();

  NewlibValSysLock_Action_0( ctx );
  NewlibValSysLock_Action_1( ctx );
  NewlibValSysLock_Action_2( ctx );
}

/** @} */
