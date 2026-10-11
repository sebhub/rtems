/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsTaskReqDelete
 */

/*
 * Copyright (C) 2021, 2026 embedded brains GmbH & Co. KG
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

#include <limits.h>
#include <rtems.h>
#include <setjmp.h>
#include <rtems/bspIo.h>
#include <rtems/test-scheduler.h>
#include <rtems/score/statesimpl.h>
#include <rtems/score/threaddispatch.h>
#include <rtems/score/threadimpl.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RtemsTaskReqDelete spec:/rtems/task/req/delete
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  RtemsTaskReqDelete_Pre_Id_Executing,
  RtemsTaskReqDelete_Pre_Id_Other,
  RtemsTaskReqDelete_Pre_Id_Invalid,
  RtemsTaskReqDelete_Pre_Id_NA
} RtemsTaskReqDelete_Pre_Id;

typedef enum {
  RtemsTaskReqDelete_Pre_Context_Task,
  RtemsTaskReqDelete_Pre_Context_Interrupt,
  RtemsTaskReqDelete_Pre_Context_NA
} RtemsTaskReqDelete_Pre_Context;

typedef enum {
  RtemsTaskReqDelete_Pre_ThreadDispatch_Disabled,
  RtemsTaskReqDelete_Pre_ThreadDispatch_Enabled,
  RtemsTaskReqDelete_Pre_ThreadDispatch_NA
} RtemsTaskReqDelete_Pre_ThreadDispatch;

typedef enum {
  RtemsTaskReqDelete_Pre_CallerPriority_Vital,
  RtemsTaskReqDelete_Pre_CallerPriority_Dispensable,
  RtemsTaskReqDelete_Pre_CallerPriority_NA
} RtemsTaskReqDelete_Pre_CallerPriority;

typedef enum {
  RtemsTaskReqDelete_Pre_Dormant_No,
  RtemsTaskReqDelete_Pre_Dormant_Yes,
  RtemsTaskReqDelete_Pre_Dormant_NA
} RtemsTaskReqDelete_Pre_Dormant;

typedef enum {
  RtemsTaskReqDelete_Pre_Suspended_Yes,
  RtemsTaskReqDelete_Pre_Suspended_No,
  RtemsTaskReqDelete_Pre_Suspended_NA
} RtemsTaskReqDelete_Pre_Suspended;

typedef enum {
  RtemsTaskReqDelete_Pre_Restarting_No,
  RtemsTaskReqDelete_Pre_Restarting_Yes,
  RtemsTaskReqDelete_Pre_Restarting_NA
} RtemsTaskReqDelete_Pre_Restarting;

typedef enum {
  RtemsTaskReqDelete_Pre_Terminating_No,
  RtemsTaskReqDelete_Pre_Terminating_Yes,
  RtemsTaskReqDelete_Pre_Terminating_NA
} RtemsTaskReqDelete_Pre_Terminating;

typedef enum {
  RtemsTaskReqDelete_Pre_Protected_Yes,
  RtemsTaskReqDelete_Pre_Protected_No,
  RtemsTaskReqDelete_Pre_Protected_NA
} RtemsTaskReqDelete_Pre_Protected;

typedef enum {
  RtemsTaskReqDelete_Pre_State_Enqueued,
  RtemsTaskReqDelete_Pre_State_Ready,
  RtemsTaskReqDelete_Pre_State_Blocked,
  RtemsTaskReqDelete_Pre_State_NA
} RtemsTaskReqDelete_Pre_State;

typedef enum {
  RtemsTaskReqDelete_Pre_Timer_Inactive,
  RtemsTaskReqDelete_Pre_Timer_Active,
  RtemsTaskReqDelete_Pre_Timer_NA
} RtemsTaskReqDelete_Pre_Timer;

typedef enum {
  RtemsTaskReqDelete_Pre_UserExtIter_None,
  RtemsTaskReqDelete_Pre_UserExtIter_Begin,
  RtemsTaskReqDelete_Pre_UserExtIter_Exitted,
  RtemsTaskReqDelete_Pre_UserExtIter_Restart,
  RtemsTaskReqDelete_Pre_UserExtIter_Terminate,
  RtemsTaskReqDelete_Pre_UserExtIter_NA
} RtemsTaskReqDelete_Pre_UserExtIter;

typedef enum {
  RtemsTaskReqDelete_Post_Status_Ok,
  RtemsTaskReqDelete_Post_Status_InvId,
  RtemsTaskReqDelete_Post_Status_CalledFromISR,
  RtemsTaskReqDelete_Post_Status_NoReturn,
  RtemsTaskReqDelete_Post_Status_NA
} RtemsTaskReqDelete_Post_Status;

typedef enum {
  RtemsTaskReqDelete_Post_FatalError_Yes,
  RtemsTaskReqDelete_Post_FatalError_Nop,
  RtemsTaskReqDelete_Post_FatalError_NA
} RtemsTaskReqDelete_Post_FatalError;

typedef enum {
  RtemsTaskReqDelete_Post_Zombie_Yes,
  RtemsTaskReqDelete_Post_Zombie_No,
  RtemsTaskReqDelete_Post_Zombie_NA
} RtemsTaskReqDelete_Post_Zombie;

typedef enum {
  RtemsTaskReqDelete_Post_TaskPriority_Raise,
  RtemsTaskReqDelete_Post_TaskPriority_Nop,
  RtemsTaskReqDelete_Post_TaskPriority_NA
} RtemsTaskReqDelete_Post_TaskPriority;

typedef enum {
  RtemsTaskReqDelete_Post_RestartExtensions_Nop,
  RtemsTaskReqDelete_Post_RestartExtensions_NA
} RtemsTaskReqDelete_Post_RestartExtensions;

typedef enum {
  RtemsTaskReqDelete_Post_TerminateExtensions_Yes,
  RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
  RtemsTaskReqDelete_Post_TerminateExtensions_NA
} RtemsTaskReqDelete_Post_TerminateExtensions;

typedef enum {
  RtemsTaskReqDelete_Post_Dormant_Yes,
  RtemsTaskReqDelete_Post_Dormant_No,
  RtemsTaskReqDelete_Post_Dormant_NA
} RtemsTaskReqDelete_Post_Dormant;

typedef enum {
  RtemsTaskReqDelete_Post_Suspended_Yes,
  RtemsTaskReqDelete_Post_Suspended_No,
  RtemsTaskReqDelete_Post_Suspended_NA
} RtemsTaskReqDelete_Post_Suspended;

typedef enum {
  RtemsTaskReqDelete_Post_Restarting_Yes,
  RtemsTaskReqDelete_Post_Restarting_No,
  RtemsTaskReqDelete_Post_Restarting_NA
} RtemsTaskReqDelete_Post_Restarting;

typedef enum {
  RtemsTaskReqDelete_Post_Terminating_Yes,
  RtemsTaskReqDelete_Post_Terminating_No,
  RtemsTaskReqDelete_Post_Terminating_NA
} RtemsTaskReqDelete_Post_Terminating;

typedef enum {
  RtemsTaskReqDelete_Post_Protected_Yes,
  RtemsTaskReqDelete_Post_Protected_No,
  RtemsTaskReqDelete_Post_Protected_NA
} RtemsTaskReqDelete_Post_Protected;

typedef enum {
  RtemsTaskReqDelete_Post_State_Enqueued,
  RtemsTaskReqDelete_Post_State_Ready,
  RtemsTaskReqDelete_Post_State_Blocked,
  RtemsTaskReqDelete_Post_State_NA
} RtemsTaskReqDelete_Post_State;

typedef enum {
  RtemsTaskReqDelete_Post_Timer_Active,
  RtemsTaskReqDelete_Post_Timer_Inactive,
  RtemsTaskReqDelete_Post_Timer_NA
} RtemsTaskReqDelete_Post_Timer;

typedef enum {
  RtemsTaskReqDelete_Post_UserExtIter_End,
  RtemsTaskReqDelete_Post_UserExtIter_Nop,
  RtemsTaskReqDelete_Post_UserExtIter_NA
} RtemsTaskReqDelete_Post_UserExtIter;

typedef struct {
  uint64_t Skip : 1;
  uint64_t Pre_Id_NA : 1;
  uint64_t Pre_Context_NA : 1;
  uint64_t Pre_ThreadDispatch_NA : 1;
  uint64_t Pre_CallerPriority_NA : 1;
  uint64_t Pre_Dormant_NA : 1;
  uint64_t Pre_Suspended_NA : 1;
  uint64_t Pre_Restarting_NA : 1;
  uint64_t Pre_Terminating_NA : 1;
  uint64_t Pre_Protected_NA : 1;
  uint64_t Pre_State_NA : 1;
  uint64_t Pre_Timer_NA : 1;
  uint64_t Pre_UserExtIter_NA : 1;
  uint64_t Post_Status : 3;
  uint64_t Post_FatalError : 2;
  uint64_t Post_Zombie : 2;
  uint64_t Post_TaskPriority : 2;
  uint64_t Post_RestartExtensions : 1;
  uint64_t Post_TerminateExtensions : 2;
  uint64_t Post_Dormant : 2;
  uint64_t Post_Suspended : 2;
  uint64_t Post_Restarting : 2;
  uint64_t Post_Terminating : 2;
  uint64_t Post_Protected : 2;
  uint64_t Post_State : 2;
  uint64_t Post_Timer : 2;
  uint64_t Post_UserExtIter : 2;
} RtemsTaskReqDelete_Entry;

typedef enum {
  USER_EXT_ITER_NONE,
  USER_EXT_ITER_BEGIN,
  USER_EXT_ITER_EXITTED,
  USER_EXT_ITER_RESTART,
  USER_EXT_ITER_TERMINATE
} UserExtIter;

/**
 * @brief Test context for spec:/rtems/task/req/delete test case.
 */
typedef struct {
  /**
   * @brief This member provides the scheduler operation records.
   */
  T_scheduler_log_10 scheduler_log;

  /**
   * @brief This member provides a jump context to resume a thread dispatch.
   */
  jmp_buf thread_dispatch_context;

  /**
   * @brief This member contains the identifier of the runner scheduler.
   */
  rtems_id scheduler_id;

  /**
   * @brief This member contains the identifier of the runner task.
   */
  rtems_id runner_id;

  /**
   * @brief This member references the TCB of the runner task.
   */
  rtems_tcb *runner_tcb;

  /**
   * @brief This member contains the identifier of the mutex.
   */
  rtems_id mutex_id;

  /**
   * @brief This member provides an event set used to set up the blocking
   *   conditions of the task to delete.
   */
  rtems_event_set events;

  /**
   * @brief This member contains the identifier of the worker task.
   */
  rtems_id worker_id;

  /**
   * @brief This member references the TCB of the worker task.
   */
  rtems_tcb *worker_tcb;

  /**
   * @brief This member contains the worker state at the end of the
   *   rtems_task_delete() call.
   */
  States_Control worker_state;

  /**
   * @brief This member contains the worker timer info at the end of the
   *   rtems_task_delete() call.
   */
  TaskTimerInfo worker_timer_info;

  /**
   * @brief This member contains the worker thread queue at the end of the
   *   rtems_task_delete() call.
   */
  const Thread_queue_Queue *worker_wait_queue;

  /**
   * @brief This member contains the worker thread life state at the end of the
   *   rtems_task_delete() call.
   */
  Thread_Life_state worker_life_state;

  /**
   * @brief This member contains the worker priority at the end of the
   *   rtems_task_delete() call.
   */
  rtems_task_priority worker_priority;

  /**
   * @brief This member specifies the iteration of the user extensions in
   *   progress for the worker before the rtems_task_delete() call.
   */
  UserExtIter userext_iter;

  /**
   * @brief If this member is true, then the worker entered the iteration of
   *   the user extensions specified by `userext_iter`.
   */
  bool userext_iter_hooked;

  /**
   * @brief If this member is true, then the extension which entered the
   *   iteration returned.
   */
  bool userext_iter_returned;

  /**
   * @brief If this member is true, then a thread life change of the worker may
   *   set `userext_iter_ended`.
   */
  bool userext_iter_check_end;

  /**
   * @brief If this member is true, then a thread life change of the worker
   *   ended the iteration.
   */
  bool userext_iter_ended;

  /**
   * @brief If this member is true, then the thread life action handler
   *   protects the thread life of the worker during the iteration.
   */
  bool life_protected_by_handler;

  /**
   * @brief This member contains the identifier of the deleter task.
   */
  rtems_id deleter_id;

  /**
   * @brief This member references the TCB of the deleter task.
   */
  rtems_tcb *deleter_tcb;

  /**
   * @brief This member contains the identifier of the second deleter task.
   */
  rtems_id deleter_2_id;

  /**
   * @brief This member references the TCB of the second deleter task.
   */
  rtems_tcb *deleter_2_tcb;

  /**
   * @brief This member contains the identifier of the test user extensions.
   */
  rtems_id extension_id;

  /**
   * @brief This member contains extension calls.
   */
  ExtensionCalls calls;

  /**
   * @brief This member contains extension calls after the rtems_task_delete()
   *   call.
   */
  ExtensionCalls calls_after_restart;

  /**
   * @brief This member contains the delete counter.
   */
  uint32_t restart_counter;

  /**
   * @brief If this member is true, then the worker shall be dormant before the
   *   rtems_task_delete() call.
   */
  bool dormant;

  /**
   * @brief If this member is true, then the worker shall be suspended before
   *   the rtems_task_delete() call.
   */
  bool suspended;

  /**
   * @brief If this member is true, then the thread life of the worker shall be
   *   protected before the rtems_task_delete() call.
   */
  bool protected;

  /**
   * @brief If this member is true, then the worker shall be restarting before
   *   the rtems_task_delete() call.
   */
  bool restarting;

  /**
   * @brief If this member is true, then the worker shall be terminating before
   *   the rtems_task_delete() call.
   */
  bool terminating;

  /**
   * @brief If this member is true, then the rtems_task_delete() shall be
   *   called from within interrupt context.
   */
  bool interrupt;

  /**
   * @brief If this member is true, then the worker shall be blocked before the
   *   rtems_task_delete() call.
   */
  bool blocked;

  /**
   * @brief If this member is true, then the worker shall be enqueued on a wait
   *   queue before the rtems_task_delete() call.
   */
  bool enqueued;

  /**
   * @brief If this member is true, then the worker obtained a mutex.
   */
  bool worker_is_mutex_owner;

  /**
   * @brief If this member is true, then the timer of the worker shall be
   *   active before the rtems_task_delete() call.
   */
  bool timer_active;

  /**
   * @brief If this member is true, then the deleter shall have a vital
   *   priority for the worker.
   */
  bool vital_deleter_priority;

  /**
   * @brief If this member is true, then thread dispatching is disabled by the
   *   worker task before the rtems_task_delete() call.
   */
  bool dispatch_disabled;

  /**
   * @brief If this member is true, then it is expected to delete the worker.
   */
  bool delete_worker_expected;

  /**
   * @brief This member contains the return value of the rtems_task_delete()
   *   call.
   */
  rtems_status_code status;

  /**
   * @brief This member specifies if the `id` parameter value.
   */
  rtems_id id;

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 12 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 12 ];

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
    RtemsTaskReqDelete_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsTaskReqDelete_Context;

static RtemsTaskReqDelete_Context RtemsTaskReqDelete_Instance;

static const char *const RtemsTaskReqDelete_PreDesc_Id[] =
  { "Executing", "Other", "Invalid", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_Context[] =
  { "Task", "Interrupt", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_ThreadDispatch[] =
  { "Disabled", "Enabled", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_CallerPriority[] =
  { "Vital", "Dispensable", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_Dormant[] =
  { "No", "Yes", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_Suspended[] =
  { "Yes", "No", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_Restarting[] =
  { "No", "Yes", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_Terminating[] =
  { "No", "Yes", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_Protected[] =
  { "Yes", "No", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_State[] =
  { "Enqueued", "Ready", "Blocked", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_Timer[] =
  { "Inactive", "Active", "NA" };

static const char *const RtemsTaskReqDelete_PreDesc_UserExtIter[] =
  { "None", "Begin", "Exitted", "Restart", "Terminate", "NA" };

static const char *const *const RtemsTaskReqDelete_PreDesc[] = {
  RtemsTaskReqDelete_PreDesc_Id,
  RtemsTaskReqDelete_PreDesc_Context,
  RtemsTaskReqDelete_PreDesc_ThreadDispatch,
  RtemsTaskReqDelete_PreDesc_CallerPriority,
  RtemsTaskReqDelete_PreDesc_Dormant,
  RtemsTaskReqDelete_PreDesc_Suspended,
  RtemsTaskReqDelete_PreDesc_Restarting,
  RtemsTaskReqDelete_PreDesc_Terminating,
  RtemsTaskReqDelete_PreDesc_Protected,
  RtemsTaskReqDelete_PreDesc_State,
  RtemsTaskReqDelete_PreDesc_Timer,
  RtemsTaskReqDelete_PreDesc_UserExtIter,
  NULL
};

typedef RtemsTaskReqDelete_Context Context;

static void CaptureWorkerState( Context *ctx )
{
  T_scheduler_log *log;

  log = T_scheduler_record( NULL );

  if ( log != NULL ) {
    T_eq_ptr( &log->header, &ctx->scheduler_log.header );

    ctx->worker_wait_queue = ctx->worker_tcb->Wait.queue;
    ctx->worker_state = ctx->worker_tcb->current_state;
    ctx->worker_life_state = ctx->worker_tcb->Life.state;
    ctx->worker_priority = SCHEDULER_PRIORITY_UNMAP(
      _Thread_Get_priority( ctx->worker_tcb )
    );
    CopyExtensionCalls( &ctx->calls, &ctx->calls_after_restart );
    GetTaskTimerInfoByThread( ctx->worker_tcb, &ctx->worker_timer_info );
  }
}

static void TaskSwitch( rtems_tcb *executing, rtems_tcb *heir )
{
  (void) executing;
  (void) heir;
  CaptureWorkerState( T_fixture_context() );
}

static void VerifyTaskPreparation( const Context *ctx )
{
  if ( ctx->id != INVALID_ID ) {
    States_Control    state;
    Thread_Life_state life_state;

    state = STATES_READY;
    life_state = ctx->worker_tcb->Life.state;

    if ( ctx->suspended ) {
      state |= STATES_SUSPENDED;
    }

    if ( ctx->dormant ) {
      T_eq_int( life_state, 0 );
      state |= STATES_DORMANT;
    } else {
      T_eq( ctx->protected, ( life_state & THREAD_LIFE_PROTECTED ) != 0 );
      T_eq( ctx->restarting, ( life_state & THREAD_LIFE_RESTARTING ) != 0 );
      T_eq( ctx->terminating, ( life_state & THREAD_LIFE_TERMINATING ) != 0 );

      if ( ctx->blocked ) {
        if ( ctx->enqueued ) {
          state |= STATES_WAITING_FOR_MUTEX;
        } else {
          state |= STATES_WAITING_FOR_EVENT;
        }
      }
    }

    T_eq_u32( ctx->worker_tcb->current_state, state );
  }
}

static void Fatal(
  rtems_fatal_source source,
  rtems_fatal_code   code,
  void              *arg
)
{
  Context *ctx;

  T_eq_int( source, INTERNAL_ERROR_CORE );
  T_eq_ulong( code, INTERNAL_ERROR_BAD_THREAD_DISPATCH_DISABLE_LEVEL );

  ctx = arg;
  ++ctx->calls.fatal;
  T_assert_eq_int( ctx->calls.fatal, 1 );
  _ISR_Set_level( 0 );
  longjmp( ctx->thread_dispatch_context, 1 );
}

static void ResumeThreadDispatch(
  rtems_fatal_source source,
  rtems_fatal_code   code,
  void              *arg
)
{
  Context *ctx;

  T_eq_int( source, INTERNAL_ERROR_CORE );
  T_eq_ulong( code, INTERNAL_ERROR_BAD_THREAD_DISPATCH_DISABLE_LEVEL );

  ctx = arg;
  SetFatalHandler( Fatal, ctx );
  _ISR_Set_level( 0 );
  longjmp( ctx->thread_dispatch_context, 1 );
}

static void DeleteAndJumpBack( Context *const ctx )
{
  if ( setjmp( ctx->thread_dispatch_context ) == 0 ) {
    ctx->status = rtems_task_delete( ctx->id );
  } else {
    _Thread_Dispatch_unnest( _Per_CPU_Get() );
  }
}

static void Delete( void *arg )
{
  Context         *ctx;
  T_scheduler_log *log;

  ctx = arg;

  if ( ctx->suspended && ctx->id != INVALID_ID ) {
    if ( ctx->id != RTEMS_SELF || ctx->interrupt ) {
      SuspendTask( ctx->worker_id );
    } else {
      Per_CPU_Control *cpu_self;

      /*
       * Where the system was built with SMP support enabled, a suspended
       * executing thread during the rtems_task_delete() call can happen
       * if the thread was suspended by another processor and the
       * inter-processor interrupt did not yet arrive.  Where the system was
       * built with SMP support disabled, this state cannot happen with the
       * current implementation.  However, we still specify and validate this
       * behaviour unconditionally since there exist alternative
       * implementations which would lead to such a state if the executing
       * thread is suspended by an ISR.
       */
      cpu_self = _Thread_Dispatch_disable();
      SuspendSelf();
      cpu_self->dispatch_necessary = false;
      _Thread_Dispatch_enable( cpu_self );
    }
  }

  if ( ctx->dispatch_disabled ) {
    _Thread_Dispatch_disable();
  }

  VerifyTaskPreparation( ctx );
  ClearExtensionCalls( &ctx->calls );

  log = T_scheduler_record_10( &ctx->scheduler_log );
  T_null( log );

  DeleteAndJumpBack( ctx );
  CaptureWorkerState( ctx );

  if ( ctx->status == RTEMS_CALLED_FROM_ISR ) {
    /* A thread life change after a call without effect does not count */
    ctx->userext_iter_check_end = false;
  }

  if ( ctx->dispatch_disabled ) {
    _Thread_Dispatch_enable( _Per_CPU_Get() );
  }
}

static void Block( Context *ctx )
{
  rtems_interval ticks;

  if ( ctx->timer_active ) {
    ticks = UINT32_MAX;
  } else {
    ticks = RTEMS_NO_TIMEOUT;
  }

  if ( ctx->enqueued ) {
    ObtainMutexTimed( ctx->mutex_id, ticks );
    ctx->worker_is_mutex_owner = true;
  } else {
    /*
     * Do not use a stack variable for the event set, since we may jump out
     * of the directive call.
     */
    (void) rtems_event_receive(
      RTEMS_ALL_EVENTS,
      RTEMS_EVENT_ANY | RTEMS_WAIT,
      ticks,
      &ctx->events
    );
  }
}

static void BlockDone( Context *ctx )
{
  if ( ctx->enqueued ) {
    ReleaseMutex( ctx->mutex_id );
  }
}

static void BlockAndJumpBack(
  Context *const         ctx,
  Per_CPU_Control *const cpu_self
)
{
  if ( setjmp( ctx->thread_dispatch_context ) == 0 ) {
    Block( ctx );
  } else {
    _Thread_Dispatch_unnest( cpu_self );
  }
}

static void Signal( rtems_signal_set signals )
{
  Context *ctx;

  (void) signals;
  ctx = T_fixture_context();

  if ( ctx->id == RTEMS_SELF ) {
    SetPriority( ctx->runner_id, PRIO_LOW );

    if ( ctx->interrupt ) {
      if ( ctx->blocked ) {
        Per_CPU_Control *cpu_self;

        SetFatalHandler( ResumeThreadDispatch, ctx );
        cpu_self = _Thread_Dispatch_disable();

        BlockAndJumpBack( ctx, cpu_self );
        CallWithinISR( Delete, ctx );

        _Thread_Dispatch_direct( cpu_self );
        BlockDone( ctx );
      } else {
        CallWithinISR( Delete, ctx );
      }
    } else {
      Delete( ctx );
    }
  } else {
    if ( ctx->blocked ) {
      Block( ctx );
      BlockDone( ctx );
    } else {
      SetPriority( ctx->runner_id, PRIO_HIGH );
    }
  }

  if ( ctx->protected && !ctx->life_protected_by_handler ) {
    _Thread_Set_life_protection( 0 );
  }
}

static void Deleter( rtems_task_argument arg )
{
  Context *ctx;

  ctx = (Context *) arg;

  if ( ctx != NULL ) {
    /* We have to prevent the priority boost in the task delete below */
    SetPriority( ctx->runner_id, PRIO_LOW );
    SetSelfPriorityNoYield( PRIO_NORMAL );

    DeleteTask( ctx->worker_id );
  }

  SuspendSelf();
}

static void SecondDeleter( rtems_task_argument arg )
{
  Context *ctx;

  ctx = (Context *) arg;

  if ( ctx != NULL ) {
    if ( !ctx->vital_deleter_priority ) {
      SetPriority( ctx->runner_id, PRIO_LOW );
      SetSelfPriorityNoYield( PRIO_NORMAL );
    }

    Delete( ctx );
  }

  SuspendSelf();
}

static void WorkerSetup( Context *ctx )
{
  rtems_status_code sc;

  sc = rtems_signal_catch( Signal, RTEMS_NO_ASR );
  T_rsc_success( sc );

  if ( ctx->protected ) {
    _Thread_Set_life_protection( THREAD_LIFE_PROTECTED );
  }

  if ( ctx->userext_iter == USER_EXT_ITER_TERMINATE ) {
    /* The deleter raised the priority of the worker */
    (void) ReceiveAnyEvents();
  } else {
    Yield();
  }
}

static void UserExtIterCheckEnd( Context *ctx, const rtems_tcb *thread )
{
  if (
    thread == ctx->worker_tcb && ctx->userext_iter_check_end &&
    ctx->userext_iter_hooked && !ctx->userext_iter_returned
  ) {
    ctx->userext_iter_ended = true;
  }
}

static bool UserExtIterHook(
  Context         *ctx,
  UserExtIter      kind,
  const rtems_tcb *thread
)
{
  if (
    thread != ctx->worker_tcb || ctx->userext_iter != kind ||
    ctx->userext_iter_hooked
  ) {
    return false;
  }

  ctx->userext_iter_hooked = true;
  WorkerSetup( ctx );
  ctx->userext_iter_returned = true;
  return true;
}

static void ThreadBegin( rtems_tcb *executing )
{
  (void)
    UserExtIterHook( T_fixture_context(), USER_EXT_ITER_BEGIN, executing );
}

static void ThreadExitted( rtems_tcb *executing )
{
  if (
    UserExtIterHook( T_fixture_context(), USER_EXT_ITER_EXITTED, executing )
  ) {
    rtems_task_exit();
  }
}

static void Worker( rtems_task_argument arg )
{
  Context *ctx;

  ctx = T_fixture_context();

  if ( ctx->userext_iter == USER_EXT_ITER_NONE ) {
    WorkerSetup( ctx );
  } else if (
    ctx->userext_iter == USER_EXT_ITER_RESTART && !ctx->userext_iter_hooked
  ) {
    (void) rtems_task_restart( RTEMS_SELF, arg );
  } else if (
    ctx->userext_iter == USER_EXT_ITER_EXITTED && !ctx->userext_iter_hooked
  ) {
    return;
  }

  if ( IsMutexOwner( ctx->mutex_id ) ) {
    ReleaseMutex( ctx->mutex_id );
  }

  rtems_task_exit();
}

static void ThreadDelete( rtems_tcb *executing, rtems_tcb *deleted )
{
  Context *ctx;

  ctx = T_fixture_context();
  ++ctx->calls.thread_delete;

  T_eq_u32( executing->Object.id, ctx->runner_id );

  if ( ctx->delete_worker_expected ) {
    T_eq_u32( deleted->Object.id, ctx->worker_id );
  }
}

static void ThreadRestart( rtems_tcb *executing, rtems_tcb *restarted )
{
  Context *ctx;

  (void) executing;

  ctx = T_fixture_context();
  ++ctx->calls.thread_restart;

  UserExtIterCheckEnd( ctx, restarted );
  (void) UserExtIterHook( ctx, USER_EXT_ITER_RESTART, restarted );
}

static void ThreadTerminate( rtems_tcb *executing )
{
  Context *ctx;

  ctx = T_fixture_context();
  ++ctx->calls.thread_terminate;

  T_eq_u32( executing->Object.id, ctx->worker_id );

  if ( IsMutexOwner( ctx->mutex_id ) ) {
    ReleaseMutex( ctx->mutex_id );
  }

  UserExtIterCheckEnd( ctx, executing );
  (void) UserExtIterHook( ctx, USER_EXT_ITER_TERMINATE, executing );
}

static void Cleanup( Context *ctx )
{
  SetSelfPriority( PRIO_VERY_LOW );

  if ( ( ctx->id == RTEMS_SELF || ctx->interrupt ) && ctx->suspended ) {
    ResumeTask( ctx->worker_id );
  }

  if ( ctx->protected && ctx->blocked ) {
    if ( ctx->enqueued ) {
      ReleaseMutex( ctx->mutex_id );
      ObtainMutex( ctx->mutex_id );
    } else {
      SendEvents( ctx->worker_id, RTEMS_EVENT_0 );
    }
  }

  /* A deletion by the cleanup does not count */
  ctx->userext_iter_check_end = false;

  if (
    ctx->id == INVALID_ID ||
    ( ctx->calls.thread_terminate == 0 &&
      ctx->userext_iter != USER_EXT_ITER_TERMINATE &&
      !( ctx->dormant && ctx->status != RTEMS_CALLED_FROM_ISR ) )
  ) {
    DeleteTask( ctx->worker_id );
  }

  SetSelfPriority( PRIO_NORMAL );
}

static const rtems_extensions_table extensions = {
  .thread_begin = ThreadBegin,
  .thread_delete = ThreadDelete,
  .thread_exitted = ThreadExitted,
  .thread_restart = ThreadRestart,
  .thread_terminate = ThreadTerminate
};

static void RtemsTaskReqDelete_Pre_Id_Prepare(
  RtemsTaskReqDelete_Context *ctx,
  RtemsTaskReqDelete_Pre_Id   state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_Id_Executing: {
      /*
       * While the `id` parameter is associated with the calling task.
       */
      ctx->id = RTEMS_SELF;
      break;
    }

    case RtemsTaskReqDelete_Pre_Id_Other: {
      /*
       * While the `id` parameter is associated with a task other than the
       * calling task.
       */
      ctx->id = ctx->worker_id;
      break;
    }

    case RtemsTaskReqDelete_Pre_Id_Invalid: {
      /*
       * While the `id` parameter is not associated with a task.
       */
      ctx->id = INVALID_ID;
      break;
    }

    case RtemsTaskReqDelete_Pre_Id_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_Context_Prepare(
  RtemsTaskReqDelete_Context    *ctx,
  RtemsTaskReqDelete_Pre_Context state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_Context_Task: {
      /*
       * While the rtems_task_delete() directive is called from within task
       * context.
       */
      ctx->interrupt = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_Context_Interrupt: {
      /*
       * While the rtems_task_delete() directive is called from within
       * interrupt context.
       */
      ctx->interrupt = true;
      break;
    }

    case RtemsTaskReqDelete_Pre_Context_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_ThreadDispatch_Prepare(
  RtemsTaskReqDelete_Context           *ctx,
  RtemsTaskReqDelete_Pre_ThreadDispatch state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_ThreadDispatch_Disabled: {
      /*
       * While thread dispatching is disabled for the calling task.
       */
      ctx->dispatch_disabled = true;
      break;
    }

    case RtemsTaskReqDelete_Pre_ThreadDispatch_Enabled: {
      /*
       * While thread dispatching is enabled for the calling task.
       */
      ctx->dispatch_disabled = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_ThreadDispatch_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_CallerPriority_Prepare(
  RtemsTaskReqDelete_Context           *ctx,
  RtemsTaskReqDelete_Pre_CallerPriority state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_CallerPriority_Vital: {
      /*
       * While at least one priority of the calling task is higher than the
       * highest priority of the task specified by the `id` parameter.
       */
      ctx->vital_deleter_priority = true;
      break;
    }

    case RtemsTaskReqDelete_Pre_CallerPriority_Dispensable: {
      /*
       * While all priorities of the calling task are lower than or equal to
       * the highest priority of the task specified by the `id` parameter.
       */
      ctx->vital_deleter_priority = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_CallerPriority_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_Dormant_Prepare(
  RtemsTaskReqDelete_Context    *ctx,
  RtemsTaskReqDelete_Pre_Dormant state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_Dormant_No: {
      /*
       * While the task specified by the `id` parameter is not dormant.
       */
      ctx->dormant = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_Dormant_Yes: {
      /*
       * While the task specified by the `id` parameter is dormant.
       */
      ctx->dormant = true;
      break;
    }

    case RtemsTaskReqDelete_Pre_Dormant_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_Suspended_Prepare(
  RtemsTaskReqDelete_Context      *ctx,
  RtemsTaskReqDelete_Pre_Suspended state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_Suspended_Yes: {
      /*
       * While the task specified by the `id` parameter is suspended.
       */
      ctx->suspended = true;
      break;
    }

    case RtemsTaskReqDelete_Pre_Suspended_No: {
      /*
       * While the task specified by the `id` parameter is not suspended.
       */
      ctx->suspended = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_Suspended_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_Restarting_Prepare(
  RtemsTaskReqDelete_Context       *ctx,
  RtemsTaskReqDelete_Pre_Restarting state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_Restarting_No: {
      /*
       * While the task specified by the `id` parameter is not restarting.
       */
      ctx->restarting = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_Restarting_Yes: {
      /*
       * While the task specified by the `id` parameter is restarting.
       */
      ctx->restarting = true;
      break;
    }

    case RtemsTaskReqDelete_Pre_Restarting_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_Terminating_Prepare(
  RtemsTaskReqDelete_Context        *ctx,
  RtemsTaskReqDelete_Pre_Terminating state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_Terminating_No: {
      /*
       * While the task specified by the `id` parameter is not terminating.
       */
      ctx->terminating = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_Terminating_Yes: {
      /*
       * While the task specified by the `id` parameter is terminating.
       */
      ctx->terminating = true;
      break;
    }

    case RtemsTaskReqDelete_Pre_Terminating_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_Protected_Prepare(
  RtemsTaskReqDelete_Context      *ctx,
  RtemsTaskReqDelete_Pre_Protected state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_Protected_Yes: {
      /*
       * While thread life of the task specified by the `id` parameter is
       * protected.
       */
      ctx->protected = true;
      break;
    }

    case RtemsTaskReqDelete_Pre_Protected_No: {
      /*
       * While thread life of the task specified by the `id` parameter is not
       * protected.
       */
      ctx->protected = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_Protected_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_State_Prepare(
  RtemsTaskReqDelete_Context  *ctx,
  RtemsTaskReqDelete_Pre_State state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_State_Enqueued: {
      /*
       * While the task specified by the `id` parameter is enqueued on a wait
       * queue.
       */
      ctx->blocked = true;
      ctx->enqueued = true;
      break;
    }

    case RtemsTaskReqDelete_Pre_State_Ready: {
      /*
       * While the task specified by the `id` parameter is a ready task or a
       * scheduled task.
       */
      ctx->blocked = false;
      ctx->enqueued = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_State_Blocked: {
      /*
       * While the task specified by the `id` parameter is blocked.
       */
      ctx->blocked = true;
      ctx->enqueued = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_State_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_Timer_Prepare(
  RtemsTaskReqDelete_Context  *ctx,
  RtemsTaskReqDelete_Pre_Timer state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_Timer_Inactive: {
      /*
       * While timer of the task specified by the `id` parameter is inactive.
       */
      ctx->timer_active = false;
      break;
    }

    case RtemsTaskReqDelete_Pre_Timer_Active: {
      /*
       * While timer of the task specified by the `id` parameter is active.
       */
      ctx->timer_active = true;
      break;
    }

    case RtemsTaskReqDelete_Pre_Timer_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Pre_UserExtIter_Prepare(
  RtemsTaskReqDelete_Context        *ctx,
  RtemsTaskReqDelete_Pre_UserExtIter state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Pre_UserExtIter_None: {
      /*
       * While no iteration of the user extensions is in progress for the task
       * specified by the `id` parameter.
       */
      ctx->userext_iter = USER_EXT_ITER_NONE;
      break;
    }

    case RtemsTaskReqDelete_Pre_UserExtIter_Begin: {
      /*
       * While the user extensions are iterated to invoke the thread begin
       * extensions for the task specified by the `id` parameter.
       */
      ctx->userext_iter = USER_EXT_ITER_BEGIN;
      break;
    }

    case RtemsTaskReqDelete_Pre_UserExtIter_Exitted: {
      /*
       * While the user extensions are iterated to invoke the thread exitted
       * extensions for the task specified by the `id` parameter.
       */
      ctx->userext_iter = USER_EXT_ITER_EXITTED;
      break;
    }

    case RtemsTaskReqDelete_Pre_UserExtIter_Restart: {
      /*
       * While the user extensions are iterated to invoke the thread restart
       * extensions for the task specified by the `id` parameter.
       */
      ctx->userext_iter = USER_EXT_ITER_RESTART;
      break;
    }

    case RtemsTaskReqDelete_Pre_UserExtIter_Terminate: {
      /*
       * While the user extensions are iterated to invoke the thread terminate
       * extensions for the task specified by the `id` parameter.
       */
      ctx->userext_iter = USER_EXT_ITER_TERMINATE;
      break;
    }

    case RtemsTaskReqDelete_Pre_UserExtIter_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_Status_Check(
  RtemsTaskReqDelete_Context    *ctx,
  RtemsTaskReqDelete_Post_Status state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_Status_Ok: {
      /*
       * The return status of rtems_task_delete() shall be RTEMS_SUCCESSFUL.
       */
      T_rsc_success( ctx->status );
      break;
    }

    case RtemsTaskReqDelete_Post_Status_InvId: {
      /*
       * The return status of rtems_task_delete() shall be RTEMS_INVALID_ID.
       */
      T_rsc( ctx->status, RTEMS_INVALID_ID );
      break;
    }

    case RtemsTaskReqDelete_Post_Status_CalledFromISR: {
      /*
       * The return status of rtems_task_delete() shall be
       * RTEMS_CALLED_FROM_ISR.
       */
      T_rsc( ctx->status, RTEMS_CALLED_FROM_ISR );
      break;
    }

    case RtemsTaskReqDelete_Post_Status_NoReturn: {
      /*
       * The rtems_task_delete() call shall not return.
       */
      T_rsc( ctx->status, RTEMS_NOT_IMPLEMENTED );
      break;
    }

    case RtemsTaskReqDelete_Post_Status_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_FatalError_Check(
  RtemsTaskReqDelete_Context        *ctx,
  RtemsTaskReqDelete_Post_FatalError state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_FatalError_Yes: {
      /*
       * The fatal error with a fatal source of INTERNAL_ERROR_CORE and a fatal
       * code of INTERNAL_ERROR_BAD_THREAD_DISPATCH_DISABLE_LEVEL shall occur
       * through the rtems_task_delete() call.
       */
      T_eq_u32( ctx->calls.fatal, 1 );
      break;
    }

    case RtemsTaskReqDelete_Post_FatalError_Nop: {
      /*
       * No fatal error shall occur through the rtems_task_delete() call.
       */
      T_eq_u32( ctx->calls.fatal, 0 );
      break;
    }

    case RtemsTaskReqDelete_Post_FatalError_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_Zombie_Check(
  RtemsTaskReqDelete_Context    *ctx,
  RtemsTaskReqDelete_Post_Zombie state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_Zombie_Yes: {
      /*
       * The task specified by the `id` parameter shall be in the zombie state
       * after the rtems_task_delete() call.
       */
      T_eq_u32( ctx->worker_state & STATES_ZOMBIE, STATES_ZOMBIE ) break;
    }

    case RtemsTaskReqDelete_Post_Zombie_No: {
      /*
       * The task specified by the `id` parameter shall not be in the zombie
       * state after the rtems_task_delete() call.
       */
      T_eq_u32( ctx->worker_state & STATES_ZOMBIE, 0 ) break;
    }

    case RtemsTaskReqDelete_Post_Zombie_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_TaskPriority_Check(
  RtemsTaskReqDelete_Context          *ctx,
  RtemsTaskReqDelete_Post_TaskPriority state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_TaskPriority_Raise: {
      /*
       * Each priority of the calling task which is higher than the highest
       * priority of the task specified by the `id` parameter shall be made the
       * highest priority of the task.
       */
      T_eq_u32( ctx->worker_priority, PRIO_ULTRA_HIGH );
      break;
    }

    case RtemsTaskReqDelete_Post_TaskPriority_Nop: {
      /*
       * The priorities of the task specified by the `id` parameter shall not
       * be changed by the rtems_task_delete() call.
       */
      T_eq_u32( ctx->worker_priority, PRIO_NORMAL );
      break;
    }

    case RtemsTaskReqDelete_Post_TaskPriority_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_RestartExtensions_Check(
  RtemsTaskReqDelete_Context               *ctx,
  RtemsTaskReqDelete_Post_RestartExtensions state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_RestartExtensions_Nop: {
      /*
       * The thread delete user extensions shall not be invoked by the
       * rtems_task_delete() call.
       */
      T_eq_u32( ctx->calls_after_restart.thread_restart, 0 );
      break;
    }

    case RtemsTaskReqDelete_Post_RestartExtensions_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_TerminateExtensions_Check(
  RtemsTaskReqDelete_Context                 *ctx,
  RtemsTaskReqDelete_Post_TerminateExtensions state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_TerminateExtensions_Yes: {
      /*
       * The thread terminate user extensions shall be invoked by the
       * rtems_task_delete() call.
       */
      T_eq_u32( ctx->calls_after_restart.thread_terminate, 1 );
      break;
    }

    case RtemsTaskReqDelete_Post_TerminateExtensions_Nop: {
      /*
       * The thread terminate user extensions shall not be invoked by the
       * rtems_task_delete() call.
       */
      T_eq_u32( ctx->calls_after_restart.thread_terminate, 0 );
      break;
    }

    case RtemsTaskReqDelete_Post_TerminateExtensions_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_Dormant_Check(
  RtemsTaskReqDelete_Context     *ctx,
  RtemsTaskReqDelete_Post_Dormant state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_Dormant_Yes: {
      /*
       * The task specified by the `id` parameter shall be dormant after the
       * rtems_task_delete() call.
       */
      T_eq_u32( ctx->worker_state & STATES_DORMANT, STATES_DORMANT ) break;
    }

    case RtemsTaskReqDelete_Post_Dormant_No: {
      /*
       * The task specified by the `id` parameter shall not be dormant after
       * the rtems_task_delete() call.
       */
      T_eq_u32( ctx->worker_state & STATES_DORMANT, 0 ) break;
    }

    case RtemsTaskReqDelete_Post_Dormant_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_Suspended_Check(
  RtemsTaskReqDelete_Context       *ctx,
  RtemsTaskReqDelete_Post_Suspended state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_Suspended_Yes: {
      /*
       * The task specified by the `id` parameter shall be suspended after the
       * rtems_task_delete() call.
       */
      T_eq_u32( ctx->worker_state & STATES_SUSPENDED, STATES_SUSPENDED ) break;
    }

    case RtemsTaskReqDelete_Post_Suspended_No: {
      /*
       * The task specified by the `id` parameter shall not be suspended after
       * the rtems_task_delete() call.
       */
      T_eq_u32( ctx->worker_state & STATES_SUSPENDED, 0 ) break;
    }

    case RtemsTaskReqDelete_Post_Suspended_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_Restarting_Check(
  RtemsTaskReqDelete_Context        *ctx,
  RtemsTaskReqDelete_Post_Restarting state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_Restarting_Yes: {
      /*
       * The task specified by the `id` parameter shall be restarting after the
       * rtems_task_delete() call.
       */
      T_ne_int( ctx->worker_life_state & THREAD_LIFE_RESTARTING, 0 );
      break;
    }

    case RtemsTaskReqDelete_Post_Restarting_No: {
      /*
       * The task specified by the `id` parameter shall not be restarting after
       * the rtems_task_delete() call.
       */
      T_eq_int( ctx->worker_life_state & THREAD_LIFE_RESTARTING, 0 );
      break;
    }

    case RtemsTaskReqDelete_Post_Restarting_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_Terminating_Check(
  RtemsTaskReqDelete_Context         *ctx,
  RtemsTaskReqDelete_Post_Terminating state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_Terminating_Yes: {
      /*
       * The task specified by the `id` parameter shall be terminating after
       * the rtems_task_delete() call.
       */
      T_ne_int( ctx->worker_life_state & THREAD_LIFE_TERMINATING, 0 );
      break;
    }

    case RtemsTaskReqDelete_Post_Terminating_No: {
      /*
       * The task specified by the `id` parameter shall not be terminating
       * after the rtems_task_delete() call.
       */
      T_eq_int( ctx->worker_life_state & THREAD_LIFE_TERMINATING, 0 );
      break;
    }

    case RtemsTaskReqDelete_Post_Terminating_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_Protected_Check(
  RtemsTaskReqDelete_Context       *ctx,
  RtemsTaskReqDelete_Post_Protected state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_Protected_Yes: {
      /*
       * The thread life of the task specified by the `id` parameter be
       * protected after the rtems_task_delete() call.
       */
      T_ne_int( ctx->worker_life_state & THREAD_LIFE_PROTECTED, 0 );
      break;
    }

    case RtemsTaskReqDelete_Post_Protected_No: {
      /*
       * The thread life of the task specified by the `id` parameter shall not
       * be protected after the rtems_task_delete() call.
       */
      T_eq_int( ctx->worker_life_state & THREAD_LIFE_PROTECTED, 0 );
      break;
    }

    case RtemsTaskReqDelete_Post_Protected_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_State_Check(
  RtemsTaskReqDelete_Context   *ctx,
  RtemsTaskReqDelete_Post_State state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_State_Enqueued: {
      /*
       * The task specified by the `id` parameter shall be enqueued on a wait
       * queue and blocked.
       */
      T_ne_u32( ctx->worker_state & STATES_BLOCKED, 0 )
        T_not_null( ctx->worker_wait_queue );
      break;
    }

    case RtemsTaskReqDelete_Post_State_Ready: {
      /*
       * The task specified by the `id` parameter shall not be enqueued on a
       * wait queue and not blocked.
       */
      T_eq_u32( ctx->worker_state & STATES_BLOCKED, 0 )
        T_null( ctx->worker_wait_queue );
      break;
    }

    case RtemsTaskReqDelete_Post_State_Blocked: {
      /*
       * The task specified by the `id` parameter shall be not enqueued on a
       * wait queue and blocked.
       */
      T_ne_u32( ctx->worker_state & STATES_BLOCKED, 0 )
        T_null( ctx->worker_wait_queue );
      break;
    }

    case RtemsTaskReqDelete_Post_State_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_Timer_Check(
  RtemsTaskReqDelete_Context   *ctx,
  RtemsTaskReqDelete_Post_Timer state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_Timer_Active: {
      /*
       * The timer of the task specified by the `id` parameter shall be active
       * after the rtems_task_delete() call.
       */
      T_eq_int( ctx->worker_timer_info.state, TASK_TIMER_TICKS );
      break;
    }

    case RtemsTaskReqDelete_Post_Timer_Inactive: {
      /*
       * The timer of the task specified by the `id` parameter shall be
       * inactive after the rtems_task_delete() call.
       */
      T_eq_int( ctx->worker_timer_info.state, TASK_TIMER_INACTIVE );
      break;
    }

    case RtemsTaskReqDelete_Post_Timer_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Post_UserExtIter_Check(
  RtemsTaskReqDelete_Context         *ctx,
  RtemsTaskReqDelete_Post_UserExtIter state
)
{
  switch ( state ) {
    case RtemsTaskReqDelete_Post_UserExtIter_End: {
      /*
       * The rtems_task_delete() call shall end the iteration of the user
       * extensions in progress for the task specified by the `id` parameter.
       */
      T_true( ctx->userext_iter_hooked );
      T_true( ctx->userext_iter_ended );
      break;
    }

    case RtemsTaskReqDelete_Post_UserExtIter_Nop: {
      /*
       * The rtems_task_delete() call shall not end the iteration of the user
       * extensions in progress for the task specified by the `id` parameter.
       */
      T_true( ctx->userext_iter_hooked );
      T_false( ctx->userext_iter_ended );
      break;
    }

    case RtemsTaskReqDelete_Post_UserExtIter_NA:
      break;
  }
}

static void RtemsTaskReqDelete_Setup( RtemsTaskReqDelete_Context *ctx )
{
  rtems_status_code sc;

  ctx->runner_id = rtems_task_self();
  ctx->runner_tcb = GetThread( ctx->runner_id );
  ctx->scheduler_id = GetSelfScheduler();
  ctx->mutex_id = CreateMutexNoProtocol();
  ObtainMutex( ctx->mutex_id );

  sc = rtems_extension_create(
    rtems_build_name( 'T', 'E', 'S', 'T' ),
    &extensions,
    &ctx->extension_id
  );
  T_rsc_success( sc );

  SetFatalHandler( Fatal, ctx );
  SetTaskSwitchExtension( TaskSwitch );
  SetSelfPriority( PRIO_NORMAL );

  ctx->deleter_id = CreateTask( "DELE", PRIO_HIGH );
  ctx->deleter_tcb = GetThread( ctx->deleter_id );
  StartTask( ctx->deleter_id, Deleter, NULL );

  ctx->deleter_2_id = CreateTask( "DEL2", PRIO_ULTRA_HIGH );
  ctx->deleter_2_tcb = GetThread( ctx->deleter_2_id );
  StartTask( ctx->deleter_2_id, SecondDeleter, NULL );
}

static void RtemsTaskReqDelete_Setup_Wrap( void *arg )
{
  RtemsTaskReqDelete_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsTaskReqDelete_Setup( ctx );
}

static void RtemsTaskReqDelete_Teardown( RtemsTaskReqDelete_Context *ctx )
{
  rtems_status_code sc;

  sc = rtems_extension_delete( ctx->extension_id );
  T_rsc_success( sc );

  SetFatalHandler( NULL, NULL );
  SetTaskSwitchExtension( NULL );
  DeleteTask( ctx->deleter_id );
  DeleteTask( ctx->deleter_2_id );
  ReleaseMutex( ctx->mutex_id );
  DeleteMutex( ctx->mutex_id );
  RestoreRunnerASR();
  RestoreRunnerPriority();
}

static void RtemsTaskReqDelete_Teardown_Wrap( void *arg )
{
  RtemsTaskReqDelete_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsTaskReqDelete_Teardown( ctx );
}

static void RtemsTaskReqDelete_Prepare( RtemsTaskReqDelete_Context *ctx )
{
  ctx->status = RTEMS_NOT_IMPLEMENTED;
  ctx->userext_iter_hooked = false;
  ctx->userext_iter_returned = false;
  ctx->userext_iter_ended = false;
  ctx->userext_iter_check_end = true;
  ctx->restart_counter = 0;

  ctx->delete_worker_expected = false;
  ctx->worker_id = CreateTask( "WORK", PRIO_NORMAL );
  ctx->delete_worker_expected = true;

  ctx->worker_tcb = GetThread( ctx->worker_id );
  ctx->worker_state = UINT32_MAX;
  ctx->worker_life_state = INT_MAX;
  ctx->worker_priority = UINT32_MAX;
}

static void RtemsTaskReqDelete_Action( RtemsTaskReqDelete_Context *ctx )
{
  rtems_status_code sc;

  ctx->life_protected_by_handler =
    ( ctx->userext_iter == USER_EXT_ITER_RESTART ||
      ctx->userext_iter == USER_EXT_ITER_TERMINATE );

  if ( ctx->id != INVALID_ID && !ctx->dormant ) {
    ctx->worker_is_mutex_owner = false;
    StartTask( ctx->worker_id, Worker, ctx );

    if ( ctx->userext_iter == USER_EXT_ITER_TERMINATE ) {
      /* Let the deleter terminate the worker */
      sc = rtems_task_restart( ctx->deleter_id, (rtems_task_argument) ctx );
      T_rsc_success( sc );
    } else {
      /* Let the worker catch signals and set the thread life protection state */
      Yield();

      if ( ctx->userext_iter == USER_EXT_ITER_RESTART ) {
        /* The self restart of the worker moved it behind the runner */
        Yield();
      }
    }

    sc = rtems_signal_send( ctx->worker_id, RTEMS_SIGNAL_0 );
    T_rsc_success( sc );

    if ( ctx->restarting ) {
      sc = rtems_task_restart( ctx->worker_id, 0 );
      T_rsc_success( sc );
    }

    if ( ctx->userext_iter == USER_EXT_ITER_TERMINATE ) {
      SendEvents( ctx->worker_id, RTEMS_EVENT_0 );
    } else if ( ctx->terminating ) {
      sc = rtems_task_restart( ctx->deleter_id, (rtems_task_argument) ctx );
      T_rsc_success( sc );
    } else {
      Yield();
    }
  }

  if ( ctx->id != RTEMS_SELF ) {
    if ( ctx->interrupt ) {
      CallWithinISR( Delete, ctx );
    } else {
      sc = rtems_task_restart( ctx->deleter_2_id, (rtems_task_argument) ctx );
      T_rsc_success( sc );
    }
  }

  Cleanup( ctx );
}

/* clang-format off */

static const RtemsTaskReqDelete_Entry
RtemsTaskReqDelete_Entries[] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_NA,
    RtemsTaskReqDelete_Post_FatalError_NA, RtemsTaskReqDelete_Post_Zombie_NA,
    RtemsTaskReqDelete_Post_TaskPriority_NA,
    RtemsTaskReqDelete_Post_RestartExtensions_NA,
    RtemsTaskReqDelete_Post_TerminateExtensions_NA,
    RtemsTaskReqDelete_Post_Dormant_NA, RtemsTaskReqDelete_Post_Suspended_NA,
    RtemsTaskReqDelete_Post_Restarting_NA,
    RtemsTaskReqDelete_Post_Terminating_NA,
    RtemsTaskReqDelete_Post_Protected_NA, RtemsTaskReqDelete_Post_State_NA,
    RtemsTaskReqDelete_Post_Timer_NA, RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_NA,
    RtemsTaskReqDelete_Post_FatalError_NA, RtemsTaskReqDelete_Post_Zombie_NA,
    RtemsTaskReqDelete_Post_TaskPriority_NA,
    RtemsTaskReqDelete_Post_RestartExtensions_NA,
    RtemsTaskReqDelete_Post_TerminateExtensions_NA,
    RtemsTaskReqDelete_Post_Dormant_NA, RtemsTaskReqDelete_Post_Suspended_NA,
    RtemsTaskReqDelete_Post_Restarting_NA,
    RtemsTaskReqDelete_Post_Terminating_NA,
    RtemsTaskReqDelete_Post_Protected_NA, RtemsTaskReqDelete_Post_State_NA,
    RtemsTaskReqDelete_Post_Timer_NA, RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_NA,
    RtemsTaskReqDelete_Post_FatalError_NA, RtemsTaskReqDelete_Post_Zombie_NA,
    RtemsTaskReqDelete_Post_TaskPriority_NA,
    RtemsTaskReqDelete_Post_RestartExtensions_NA,
    RtemsTaskReqDelete_Post_TerminateExtensions_NA,
    RtemsTaskReqDelete_Post_Dormant_NA, RtemsTaskReqDelete_Post_Suspended_NA,
    RtemsTaskReqDelete_Post_Restarting_NA,
    RtemsTaskReqDelete_Post_Terminating_NA,
    RtemsTaskReqDelete_Post_Protected_NA, RtemsTaskReqDelete_Post_State_NA,
    RtemsTaskReqDelete_Post_Timer_NA, RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_NA,
    RtemsTaskReqDelete_Post_FatalError_NA, RtemsTaskReqDelete_Post_Zombie_NA,
    RtemsTaskReqDelete_Post_TaskPriority_NA,
    RtemsTaskReqDelete_Post_RestartExtensions_NA,
    RtemsTaskReqDelete_Post_TerminateExtensions_NA,
    RtemsTaskReqDelete_Post_Dormant_NA, RtemsTaskReqDelete_Post_Suspended_NA,
    RtemsTaskReqDelete_Post_Restarting_NA,
    RtemsTaskReqDelete_Post_Terminating_NA,
    RtemsTaskReqDelete_Post_Protected_NA, RtemsTaskReqDelete_Post_State_NA,
    RtemsTaskReqDelete_Post_Timer_NA, RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_NA,
    RtemsTaskReqDelete_Post_FatalError_NA, RtemsTaskReqDelete_Post_Zombie_NA,
    RtemsTaskReqDelete_Post_TaskPriority_NA,
    RtemsTaskReqDelete_Post_RestartExtensions_NA,
    RtemsTaskReqDelete_Post_TerminateExtensions_NA,
    RtemsTaskReqDelete_Post_Dormant_NA, RtemsTaskReqDelete_Post_Suspended_NA,
    RtemsTaskReqDelete_Post_Restarting_NA,
    RtemsTaskReqDelete_Post_Terminating_NA,
    RtemsTaskReqDelete_Post_Protected_NA, RtemsTaskReqDelete_Post_State_NA,
    RtemsTaskReqDelete_Post_Timer_NA, RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_NA,
    RtemsTaskReqDelete_Post_FatalError_NA, RtemsTaskReqDelete_Post_Zombie_NA,
    RtemsTaskReqDelete_Post_TaskPriority_NA,
    RtemsTaskReqDelete_Post_RestartExtensions_NA,
    RtemsTaskReqDelete_Post_TerminateExtensions_NA,
    RtemsTaskReqDelete_Post_Dormant_NA, RtemsTaskReqDelete_Post_Suspended_NA,
    RtemsTaskReqDelete_Post_Restarting_NA,
    RtemsTaskReqDelete_Post_Terminating_NA,
    RtemsTaskReqDelete_Post_Protected_NA, RtemsTaskReqDelete_Post_State_NA,
    RtemsTaskReqDelete_Post_Timer_NA, RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_Yes,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Yes,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_Yes,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Yes,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_End },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_Yes,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Yes,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_Yes,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Yes,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_Yes,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_Yes, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_Yes,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_Yes, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    RtemsTaskReqDelete_Post_Status_InvId,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_NA,
    RtemsTaskReqDelete_Post_TaskPriority_NA,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_NA, RtemsTaskReqDelete_Post_Suspended_NA,
    RtemsTaskReqDelete_Post_Restarting_NA,
    RtemsTaskReqDelete_Post_Terminating_NA,
    RtemsTaskReqDelete_Post_Protected_NA, RtemsTaskReqDelete_Post_State_NA,
    RtemsTaskReqDelete_Post_Timer_NA, RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_NoReturn,
    RtemsTaskReqDelete_Post_FatalError_Yes, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Raise,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqDelete_Post_Status_Ok,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_Yes, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_Yes, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_Yes,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_No,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_No,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_Yes,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Enqueued,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No, RtemsTaskReqDelete_Post_State_Ready,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Inactive,
    RtemsTaskReqDelete_Post_UserExtIter_NA },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    RtemsTaskReqDelete_Post_Status_CalledFromISR,
    RtemsTaskReqDelete_Post_FatalError_Nop, RtemsTaskReqDelete_Post_Zombie_No,
    RtemsTaskReqDelete_Post_TaskPriority_Nop,
    RtemsTaskReqDelete_Post_RestartExtensions_Nop,
    RtemsTaskReqDelete_Post_TerminateExtensions_Nop,
    RtemsTaskReqDelete_Post_Dormant_No, RtemsTaskReqDelete_Post_Suspended_No,
    RtemsTaskReqDelete_Post_Restarting_Yes,
    RtemsTaskReqDelete_Post_Terminating_Yes,
    RtemsTaskReqDelete_Post_Protected_No,
    RtemsTaskReqDelete_Post_State_Blocked,
    RtemsTaskReqDelete_Post_Timer_Active,
    RtemsTaskReqDelete_Post_UserExtIter_NA }
};

static const uint16_t
RtemsTaskReqDelete_Map[] = {
  0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 251, 117, 117, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3,
  4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 252, 120, 120, 3, 4, 5, 5, 5,
  3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 251, 117,
  117, 3, 117, 5, 5, 5, 3, 5, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 4, 0,
  0, 0, 3, 4, 252, 120, 120, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4,
  0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 253, 74, 74, 74, 4, 5, 5, 5, 5, 4, 0, 0, 0, 0,
  4, 0, 0, 0, 0, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 254, 121, 121, 3, 4, 5, 5, 5,
  3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 253, 74,
  74, 74, 74, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0,
  0, 3, 4, 254, 121, 121, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0,
  0, 0, 3, 4, 0, 0, 0, 3, 4, 255, 118, 118, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4,
  0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 256, 122, 122, 3, 4, 5, 5, 5, 3,
  4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 255, 118, 118,
  3, 118, 5, 5, 5, 3, 5, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 4, 0, 0, 0,
  3, 4, 256, 122, 122, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0,
  0, 0, 4, 0, 0, 0, 0, 4, 257, 75, 75, 75, 4, 5, 5, 5, 5, 4, 0, 0, 0, 0, 4, 0,
  0, 0, 0, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 258, 123, 123, 3, 4, 5, 5, 5, 3, 4,
  0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 257, 75, 75, 75,
  75, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0, 0, 3,
  4, 258, 123, 123, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5,
  5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0,
  3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 0, 0, 0, 0, 3,
  0, 0, 0, 0, 3, 0, 5, 5, 5, 3, 5, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 4,
  0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0,
  0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 5, 5, 5, 5, 4, 0, 0, 0, 0, 4, 0, 0,
  0, 0, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0,
  3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5,
  5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4,
  5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0,
  0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0,
  0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0,
  3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 5, 5, 5, 3, 5, 0, 0, 0, 3, 0, 0, 0, 0, 3,
  0, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4,
  0, 0, 0, 3, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 5, 5, 5, 5, 4, 0,
  0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5,
  5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0, 0, 3,
  4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 259, 119, 119, 3, 4, 5, 5,
  5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 260,
  124, 124, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 0,
  0, 0, 0, 3, 0, 259, 119, 119, 3, 119, 5, 5, 5, 3, 5, 0, 0, 0, 3, 0, 0, 0, 0,
  3, 0, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 260, 124, 124, 3, 4, 5, 5, 5, 3, 4, 0, 0,
  0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 261, 76, 76, 76, 4, 5,
  5, 5, 5, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 262,
  125, 125, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 261, 76, 76, 76, 76, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 262, 125, 125, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0,
  3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 126, 23, 23, 3, 4, 5, 5,
  5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 126, 23,
  23, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 0, 0, 0,
  0, 3, 0, 126, 23, 23, 3, 23, 5, 5, 5, 3, 5, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0,
  0, 0, 3, 4, 0, 0, 0, 3, 4, 126, 23, 23, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4,
  0, 0, 0, 3, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 127, 22, 22, 22, 4, 5, 5, 5, 5,
  4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 127, 22, 22,
  3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 127, 22, 22, 22, 22, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  3, 4, 0, 0, 0, 3, 4, 127, 22, 22, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0,
  0, 3, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0,
  0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0,
  0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0,
  3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 5, 5, 5, 3, 5, 0, 0, 0, 3, 0, 0, 0, 0, 3,
  0, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4,
  0, 0, 0, 3, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 5, 5, 5, 5, 4, 0,
  0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5,
  5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0, 0, 3,
  4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4,
  0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0,
  0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0,
  0, 3, 4, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 5, 5, 5, 3, 5, 0, 0, 0,
  3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3,
  4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4,
  5, 5, 5, 5, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0,
  0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3,
  4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 415, 263, 263, 3, 4, 416, 264, 264, 3,
  4, 417, 265, 265, 3, 4, 5, 5, 5, 3, 4, 418, 266, 266, 3, 4, 419, 267, 267, 3,
  4, 420, 268, 268, 3, 4, 421, 269, 269, 3, 4, 422, 270, 270, 3, 4, 5, 5, 5, 3,
  4, 423, 271, 271, 3, 4, 424, 272, 272, 3, 4, 425, 230, 230, 3, 230, 426, 231,
  231, 3, 231, 427, 232, 232, 3, 232, 5, 5, 5, 3, 5, 428, 233, 233, 3, 233,
  429, 234, 234, 3, 234, 430, 273, 273, 3, 4, 431, 274, 274, 3, 4, 432, 275,
  275, 3, 4, 5, 5, 5, 3, 4, 433, 276, 276, 3, 4, 434, 277, 277, 3, 4, 435, 235,
  235, 235, 4, 436, 236, 236, 236, 4, 437, 237, 237, 237, 4, 5, 5, 5, 5, 4,
  438, 238, 238, 238, 4, 439, 239, 239, 239, 4, 440, 278, 278, 3, 4, 441, 279,
  279, 3, 4, 442, 280, 280, 3, 4, 5, 5, 5, 3, 4, 443, 281, 281, 3, 4, 444, 282,
  282, 3, 4, 445, 128, 128, 128, 128, 446, 129, 129, 129, 129, 447, 130, 130,
  130, 130, 5, 5, 5, 5, 5, 448, 131, 131, 131, 131, 449, 132, 132, 132, 132,
  450, 283, 283, 3, 4, 451, 284, 284, 3, 4, 452, 285, 285, 3, 4, 5, 5, 5, 3, 4,
  453, 286, 286, 3, 4, 454, 287, 287, 3, 4, 455, 288, 288, 3, 4, 456, 289, 289,
  3, 4, 457, 290, 290, 3, 4, 5, 5, 5, 3, 4, 458, 291, 291, 3, 4, 459, 292, 292,
  3, 4, 460, 293, 293, 3, 4, 461, 294, 294, 3, 4, 462, 295, 295, 3, 4, 5, 5, 5,
  3, 4, 463, 296, 296, 3, 4, 464, 297, 297, 3, 4, 465, 240, 240, 3, 240, 466,
  241, 241, 3, 241, 467, 242, 242, 3, 242, 5, 5, 5, 3, 5, 468, 243, 243, 3,
  243, 469, 244, 244, 3, 244, 470, 298, 298, 3, 4, 471, 299, 299, 3, 4, 472,
  300, 300, 3, 4, 5, 5, 5, 3, 4, 473, 301, 301, 3, 4, 474, 302, 302, 3, 4, 475,
  245, 245, 245, 4, 476, 246, 246, 246, 4, 477, 247, 247, 247, 4, 5, 5, 5, 5,
  4, 478, 248, 248, 248, 4, 479, 249, 249, 249, 4, 480, 303, 303, 3, 4, 481,
  304, 304, 3, 4, 482, 305, 305, 3, 4, 5, 5, 5, 3, 4, 483, 306, 306, 3, 4, 484,
  307, 307, 3, 4, 485, 133, 133, 133, 133, 486, 134, 134, 134, 134, 487, 135,
  135, 135, 135, 5, 5, 5, 5, 5, 488, 136, 136, 136, 136, 489, 137, 137, 137,
  137, 490, 308, 308, 3, 4, 491, 309, 309, 3, 4, 492, 310, 310, 3, 4, 5, 5, 5,
  3, 4, 493, 311, 311, 3, 4, 494, 312, 312, 3, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3,
  4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4,
  0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 5,
  5, 5, 3, 5, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0,
  0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 4, 0, 0, 0,
  0, 4, 0, 0, 0, 0, 4, 5, 5, 5, 5, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 3,
  4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0,
  0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5,
  3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3,
  4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0,
  0, 0, 0, 3, 0, 5, 5, 5, 3, 5, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 4, 0,
  0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0,
  0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 5, 5, 5, 5, 4, 0, 0, 0, 0, 4, 0, 0, 0,
  0, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5, 5, 5, 3, 4, 0, 0, 0, 3,
  4, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 5,
  5, 5, 3, 4, 0, 0, 0, 3, 4, 0, 0, 0, 3, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1,
  1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1,
  3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3,
  4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4,
  1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3,
  4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1,
  1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1,
  4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3,
  4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1,
  1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1,
  1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1,
  1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3,
  1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1, 4, 1,
  1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1,
  1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 138, 24,
  24, 3, 4, 139, 25, 25, 3, 4, 140, 26, 26, 3, 4, 5, 5, 5, 3, 4, 141, 27, 27,
  3, 4, 142, 28, 28, 3, 4, 14, 6, 6, 3, 4, 14, 6, 6, 3, 4, 14, 6, 6, 3, 4, 5,
  5, 5, 3, 4, 14, 6, 6, 3, 4, 14, 6, 6, 3, 4, 138, 24, 24, 3, 313, 139, 25, 25,
  3, 314, 140, 26, 26, 3, 315, 5, 5, 5, 3, 5, 141, 27, 27, 3, 316, 142, 28, 28,
  3, 317, 14, 6, 6, 3, 4, 14, 6, 6, 3, 4, 14, 6, 6, 3, 4, 5, 5, 5, 3, 4, 14, 6,
  6, 3, 4, 14, 6, 6, 3, 4, 143, 29, 29, 77, 4, 144, 30, 30, 78, 4, 145, 31, 31,
  79, 4, 5, 5, 5, 5, 4, 146, 32, 32, 80, 4, 147, 33, 33, 81, 4, 15, 7, 7, 3, 4,
  15, 7, 7, 3, 4, 15, 7, 7, 3, 4, 5, 5, 5, 3, 4, 15, 7, 7, 3, 4, 15, 7, 7, 3,
  4, 143, 29, 29, 77, 77, 144, 30, 30, 78, 78, 145, 31, 31, 79, 79, 5, 5, 5, 5,
  5, 146, 32, 32, 80, 80, 147, 33, 33, 81, 81, 15, 7, 7, 3, 4, 15, 7, 7, 3, 4,
  15, 7, 7, 3, 4, 5, 5, 5, 3, 4, 15, 7, 7, 3, 4, 15, 7, 7, 3, 4, 138, 24, 24,
  3, 4, 139, 25, 25, 3, 4, 140, 26, 26, 3, 4, 5, 5, 5, 3, 4, 141, 27, 27, 3, 4,
  142, 28, 28, 3, 4, 14, 6, 6, 3, 4, 14, 6, 6, 3, 4, 14, 6, 6, 3, 4, 5, 5, 5,
  3, 4, 14, 6, 6, 3, 4, 14, 6, 6, 3, 4, 138, 24, 24, 3, 313, 139, 25, 25, 3,
  314, 140, 26, 26, 3, 315, 5, 5, 5, 3, 5, 141, 27, 27, 3, 316, 142, 28, 28, 3,
  317, 14, 6, 6, 3, 4, 14, 6, 6, 3, 4, 14, 6, 6, 3, 4, 5, 5, 5, 3, 4, 14, 6, 6,
  3, 4, 14, 6, 6, 3, 4, 143, 29, 29, 77, 4, 144, 30, 30, 78, 4, 145, 31, 31,
  79, 4, 5, 5, 5, 5, 4, 146, 32, 32, 80, 4, 147, 33, 33, 81, 4, 15, 7, 7, 3, 4,
  15, 7, 7, 3, 4, 15, 7, 7, 3, 4, 5, 5, 5, 3, 4, 15, 7, 7, 3, 4, 15, 7, 7, 3,
  4, 143, 29, 29, 77, 77, 144, 30, 30, 78, 78, 145, 31, 31, 79, 79, 5, 5, 5, 5,
  5, 146, 32, 32, 80, 80, 147, 33, 33, 81, 81, 15, 7, 7, 3, 4, 15, 7, 7, 3, 4,
  15, 7, 7, 3, 4, 5, 5, 5, 3, 4, 15, 7, 7, 3, 4, 15, 7, 7, 3, 4, 148, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 149, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 34, 34, 3, 4, 151, 35, 35, 3, 4, 152, 36, 36,
  3, 4, 5, 5, 5, 3, 4, 153, 37, 37, 3, 4, 154, 38, 38, 3, 4, 16, 8, 8, 3, 4,
  16, 8, 8, 3, 4, 16, 8, 8, 3, 4, 5, 5, 5, 3, 4, 16, 8, 8, 3, 4, 16, 8, 8, 3,
  4, 150, 34, 34, 3, 318, 151, 35, 35, 3, 319, 152, 36, 36, 3, 320, 5, 5, 5, 3,
  5, 153, 37, 37, 3, 321, 154, 38, 38, 3, 322, 16, 8, 8, 3, 4, 16, 8, 8, 3, 4,
  16, 8, 8, 3, 4, 5, 5, 5, 3, 4, 16, 8, 8, 3, 4, 16, 8, 8, 3, 4, 155, 39, 39,
  82, 4, 156, 40, 40, 83, 4, 157, 41, 41, 84, 4, 5, 5, 5, 5, 4, 158, 42, 42,
  85, 4, 159, 43, 43, 86, 4, 17, 9, 9, 3, 4, 17, 9, 9, 3, 4, 17, 9, 9, 3, 4, 5,
  5, 5, 3, 4, 17, 9, 9, 3, 4, 17, 9, 9, 3, 4, 155, 39, 39, 82, 82, 156, 40, 40,
  83, 83, 157, 41, 41, 84, 84, 5, 5, 5, 5, 5, 158, 42, 42, 85, 85, 159, 43, 43,
  86, 86, 17, 9, 9, 3, 4, 17, 9, 9, 3, 4, 17, 9, 9, 3, 4, 5, 5, 5, 3, 4, 17, 9,
  9, 3, 4, 17, 9, 9, 3, 4, 150, 34, 34, 3, 4, 151, 35, 35, 3, 4, 152, 36, 36,
  3, 4, 5, 5, 5, 3, 4, 153, 37, 37, 3, 4, 154, 38, 38, 3, 4, 16, 8, 8, 3, 4,
  16, 8, 8, 3, 4, 16, 8, 8, 3, 4, 5, 5, 5, 3, 4, 16, 8, 8, 3, 4, 16, 8, 8, 3,
  4, 150, 34, 34, 3, 318, 151, 35, 35, 3, 319, 152, 36, 36, 3, 320, 5, 5, 5, 3,
  5, 153, 37, 37, 3, 321, 154, 38, 38, 3, 322, 16, 8, 8, 3, 4, 16, 8, 8, 3, 4,
  16, 8, 8, 3, 4, 5, 5, 5, 3, 4, 16, 8, 8, 3, 4, 16, 8, 8, 3, 4, 155, 39, 39,
  82, 4, 156, 40, 40, 83, 4, 157, 41, 41, 84, 4, 5, 5, 5, 5, 4, 158, 42, 42,
  85, 4, 159, 43, 43, 86, 4, 17, 9, 9, 3, 4, 17, 9, 9, 3, 4, 17, 9, 9, 3, 4, 5,
  5, 5, 3, 4, 17, 9, 9, 3, 4, 17, 9, 9, 3, 4, 155, 39, 39, 82, 82, 156, 40, 40,
  83, 83, 157, 41, 41, 84, 84, 5, 5, 5, 5, 5, 158, 42, 42, 85, 85, 159, 43, 43,
  86, 86, 17, 9, 9, 3, 4, 17, 9, 9, 3, 4, 17, 9, 9, 3, 4, 5, 5, 5, 3, 4, 17, 9,
  9, 3, 4, 17, 9, 9, 3, 4, 148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 149, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 160, 44,
  44, 3, 4, 161, 45, 45, 3, 4, 162, 46, 46, 3, 4, 5, 5, 5, 3, 4, 163, 47, 47,
  3, 4, 164, 48, 48, 3, 4, 18, 10, 10, 3, 4, 18, 10, 10, 3, 4, 18, 10, 10, 3,
  4, 5, 5, 5, 3, 4, 18, 10, 10, 3, 4, 18, 10, 10, 3, 4, 160, 44, 44, 3, 323,
  161, 45, 45, 3, 324, 162, 46, 46, 3, 325, 5, 5, 5, 3, 5, 163, 47, 47, 3, 326,
  164, 48, 48, 3, 327, 18, 10, 10, 3, 4, 18, 10, 10, 3, 4, 18, 10, 10, 3, 4, 5,
  5, 5, 3, 4, 18, 10, 10, 3, 4, 18, 10, 10, 3, 4, 165, 49, 49, 87, 4, 166, 50,
  50, 88, 4, 167, 51, 51, 89, 4, 5, 5, 5, 5, 4, 168, 52, 52, 90, 4, 169, 53,
  53, 91, 4, 19, 11, 11, 3, 4, 19, 11, 11, 3, 4, 19, 11, 11, 3, 4, 5, 5, 5, 3,
  4, 19, 11, 11, 3, 4, 19, 11, 11, 3, 4, 165, 49, 49, 87, 87, 166, 50, 50, 88,
  88, 167, 51, 51, 89, 89, 5, 5, 5, 5, 5, 168, 52, 52, 90, 90, 169, 53, 53, 91,
  91, 19, 11, 11, 3, 4, 19, 11, 11, 3, 4, 19, 11, 11, 3, 4, 5, 5, 5, 3, 4, 19,
  11, 11, 3, 4, 19, 11, 11, 3, 4, 160, 44, 44, 3, 4, 161, 45, 45, 3, 4, 162,
  46, 46, 3, 4, 5, 5, 5, 3, 4, 163, 47, 47, 3, 4, 164, 48, 48, 3, 4, 18, 10,
  10, 3, 4, 18, 10, 10, 3, 4, 18, 10, 10, 3, 4, 5, 5, 5, 3, 4, 18, 10, 10, 3,
  4, 18, 10, 10, 3, 4, 160, 44, 44, 3, 323, 161, 45, 45, 3, 324, 162, 46, 46,
  3, 325, 5, 5, 5, 3, 5, 163, 47, 47, 3, 326, 164, 48, 48, 3, 327, 18, 10, 10,
  3, 4, 18, 10, 10, 3, 4, 18, 10, 10, 3, 4, 5, 5, 5, 3, 4, 18, 10, 10, 3, 4,
  18, 10, 10, 3, 4, 165, 49, 49, 87, 4, 166, 50, 50, 88, 4, 167, 51, 51, 89, 4,
  5, 5, 5, 5, 4, 168, 52, 52, 90, 4, 169, 53, 53, 91, 4, 19, 11, 11, 3, 4, 19,
  11, 11, 3, 4, 19, 11, 11, 3, 4, 5, 5, 5, 3, 4, 19, 11, 11, 3, 4, 19, 11, 11,
  3, 4, 165, 49, 49, 87, 87, 166, 50, 50, 88, 88, 167, 51, 51, 89, 89, 5, 5, 5,
  5, 5, 168, 52, 52, 90, 90, 169, 53, 53, 91, 91, 19, 11, 11, 3, 4, 19, 11, 11,
  3, 4, 19, 11, 11, 3, 4, 5, 5, 5, 3, 4, 19, 11, 11, 3, 4, 19, 11, 11, 3, 4,
  148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 149, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 54, 54, 3, 4, 171, 55, 55, 3,
  4, 172, 56, 56, 3, 4, 5, 5, 5, 3, 4, 173, 57, 57, 3, 4, 174, 58, 58, 3, 4,
  20, 12, 12, 3, 4, 20, 12, 12, 3, 4, 20, 12, 12, 3, 4, 5, 5, 5, 3, 4, 20, 12,
  12, 3, 4, 20, 12, 12, 3, 4, 170, 54, 54, 3, 328, 171, 55, 55, 3, 329, 172,
  56, 56, 3, 330, 5, 5, 5, 3, 5, 173, 57, 57, 3, 331, 174, 58, 58, 3, 332, 20,
  12, 12, 3, 4, 20, 12, 12, 3, 4, 20, 12, 12, 3, 4, 5, 5, 5, 3, 4, 20, 12, 12,
  3, 4, 20, 12, 12, 3, 4, 175, 59, 59, 92, 4, 176, 60, 60, 93, 4, 177, 61, 61,
  94, 4, 5, 5, 5, 5, 4, 178, 62, 62, 95, 4, 179, 63, 63, 96, 4, 21, 13, 13, 3,
  4, 21, 13, 13, 3, 4, 21, 13, 13, 3, 4, 5, 5, 5, 3, 4, 21, 13, 13, 3, 4, 21,
  13, 13, 3, 4, 175, 59, 59, 92, 92, 176, 60, 60, 93, 93, 177, 61, 61, 94, 94,
  5, 5, 5, 5, 5, 178, 62, 62, 95, 95, 179, 63, 63, 96, 96, 21, 13, 13, 3, 4,
  21, 13, 13, 3, 4, 21, 13, 13, 3, 4, 5, 5, 5, 3, 4, 21, 13, 13, 3, 4, 21, 13,
  13, 3, 4, 170, 54, 54, 3, 4, 171, 55, 55, 3, 4, 172, 56, 56, 3, 4, 5, 5, 5,
  3, 4, 173, 57, 57, 3, 4, 174, 58, 58, 3, 4, 20, 12, 12, 3, 4, 20, 12, 12, 3,
  4, 20, 12, 12, 3, 4, 5, 5, 5, 3, 4, 20, 12, 12, 3, 4, 20, 12, 12, 3, 4, 170,
  54, 54, 3, 328, 171, 55, 55, 3, 329, 172, 56, 56, 3, 330, 5, 5, 5, 3, 5, 173,
  57, 57, 3, 331, 174, 58, 58, 3, 332, 20, 12, 12, 3, 4, 20, 12, 12, 3, 4, 20,
  12, 12, 3, 4, 5, 5, 5, 3, 4, 20, 12, 12, 3, 4, 20, 12, 12, 3, 4, 175, 59, 59,
  92, 4, 176, 60, 60, 93, 4, 177, 61, 61, 94, 4, 5, 5, 5, 5, 4, 178, 62, 62,
  95, 4, 179, 63, 63, 96, 4, 21, 13, 13, 3, 4, 21, 13, 13, 3, 4, 21, 13, 13, 3,
  4, 5, 5, 5, 3, 4, 21, 13, 13, 3, 4, 21, 13, 13, 3, 4, 175, 59, 59, 92, 92,
  176, 60, 60, 93, 93, 177, 61, 61, 94, 94, 5, 5, 5, 5, 5, 178, 62, 62, 95, 95,
  179, 63, 63, 96, 96, 21, 13, 13, 3, 4, 21, 13, 13, 3, 4, 21, 13, 13, 3, 4, 5,
  5, 5, 3, 4, 21, 13, 13, 3, 4, 21, 13, 13, 3, 4, 148, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 149, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 333, 180, 180, 3, 4, 334, 181, 181, 3, 4, 335, 182, 182, 3, 4, 5,
  5, 5, 3, 4, 336, 183, 183, 3, 4, 337, 184, 184, 3, 4, 338, 185, 185, 3, 4,
  339, 186, 186, 3, 4, 340, 187, 187, 3, 4, 5, 5, 5, 3, 4, 341, 188, 188, 3, 4,
  342, 189, 189, 3, 4, 343, 97, 97, 3, 97, 344, 98, 98, 3, 98, 345, 99, 99, 3,
  99, 5, 5, 5, 3, 5, 346, 100, 100, 3, 100, 347, 101, 101, 3, 101, 348, 190,
  190, 3, 4, 349, 191, 191, 3, 4, 350, 192, 192, 3, 4, 5, 5, 5, 3, 4, 351, 193,
  193, 3, 4, 352, 194, 194, 3, 4, 353, 102, 102, 102, 4, 354, 103, 103, 103, 4,
  355, 104, 104, 104, 4, 5, 5, 5, 5, 4, 356, 105, 105, 105, 4, 357, 106, 106,
  106, 4, 358, 195, 195, 3, 4, 359, 196, 196, 3, 4, 360, 197, 197, 3, 4, 5, 5,
  5, 3, 4, 361, 198, 198, 3, 4, 362, 199, 199, 3, 4, 363, 64, 64, 64, 64, 364,
  65, 65, 65, 65, 365, 66, 66, 66, 66, 5, 5, 5, 5, 5, 366, 67, 67, 67, 67, 367,
  68, 68, 68, 68, 368, 200, 200, 3, 4, 369, 201, 201, 3, 4, 370, 202, 202, 3,
  4, 5, 5, 5, 3, 4, 371, 203, 203, 3, 4, 372, 204, 204, 3, 4, 373, 205, 205, 3,
  4, 374, 206, 206, 3, 4, 375, 207, 207, 3, 4, 5, 5, 5, 3, 4, 376, 208, 208, 3,
  4, 377, 209, 209, 3, 4, 378, 210, 210, 3, 4, 379, 211, 211, 3, 4, 380, 212,
  212, 3, 4, 5, 5, 5, 3, 4, 381, 213, 213, 3, 4, 382, 214, 214, 3, 4, 383, 107,
  107, 3, 107, 384, 108, 108, 3, 108, 385, 109, 109, 3, 109, 5, 5, 5, 3, 5,
  386, 110, 110, 3, 110, 387, 111, 111, 3, 111, 388, 215, 215, 3, 4, 389, 216,
  216, 3, 4, 390, 217, 217, 3, 4, 5, 5, 5, 3, 4, 391, 218, 218, 3, 4, 392, 219,
  219, 3, 4, 393, 112, 112, 112, 4, 394, 113, 113, 113, 4, 395, 114, 114, 114,
  4, 5, 5, 5, 5, 4, 396, 115, 115, 115, 4, 397, 116, 116, 116, 4, 398, 220,
  220, 3, 4, 399, 221, 221, 3, 4, 400, 222, 222, 3, 4, 5, 5, 5, 3, 4, 401, 223,
  223, 3, 4, 402, 224, 224, 3, 4, 403, 69, 69, 69, 69, 404, 70, 70, 70, 70,
  405, 71, 71, 71, 71, 5, 5, 5, 5, 5, 406, 72, 72, 72, 72, 407, 73, 73, 73, 73,
  408, 225, 225, 3, 4, 409, 226, 226, 3, 4, 410, 227, 227, 3, 4, 5, 5, 5, 3, 4,
  411, 228, 228, 3, 4, 412, 229, 229, 3, 4, 413, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 414, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 333, 180, 180, 3, 4, 334, 181, 181, 3, 4, 335, 182, 182, 3, 4, 5, 5, 5,
  3, 4, 336, 183, 183, 3, 4, 337, 184, 184, 3, 4, 338, 185, 185, 3, 4, 339,
  186, 186, 3, 4, 340, 187, 187, 3, 4, 5, 5, 5, 3, 4, 341, 188, 188, 3, 4, 342,
  189, 189, 3, 4, 343, 97, 97, 3, 97, 344, 98, 98, 3, 98, 345, 99, 99, 3, 99,
  5, 5, 5, 3, 5, 346, 100, 100, 3, 100, 347, 101, 101, 3, 101, 348, 190, 190,
  3, 4, 349, 191, 191, 3, 4, 350, 192, 192, 3, 4, 5, 5, 5, 3, 4, 351, 193, 193,
  3, 4, 352, 194, 194, 3, 4, 353, 102, 102, 102, 4, 354, 103, 103, 103, 4, 355,
  104, 104, 104, 4, 5, 5, 5, 5, 4, 356, 105, 105, 105, 4, 357, 106, 106, 106,
  4, 358, 195, 195, 3, 4, 359, 196, 196, 3, 4, 360, 197, 197, 3, 4, 5, 5, 5, 3,
  4, 361, 198, 198, 3, 4, 362, 199, 199, 3, 4, 363, 64, 64, 64, 64, 364, 65,
  65, 65, 65, 365, 66, 66, 66, 66, 5, 5, 5, 5, 5, 366, 67, 67, 67, 67, 367, 68,
  68, 68, 68, 368, 200, 200, 3, 4, 369, 201, 201, 3, 4, 370, 202, 202, 3, 4, 5,
  5, 5, 3, 4, 371, 203, 203, 3, 4, 372, 204, 204, 3, 4, 373, 205, 205, 3, 4,
  374, 206, 206, 3, 4, 375, 207, 207, 3, 4, 5, 5, 5, 3, 4, 376, 208, 208, 3, 4,
  377, 209, 209, 3, 4, 378, 210, 210, 3, 4, 379, 211, 211, 3, 4, 380, 212, 212,
  3, 4, 5, 5, 5, 3, 4, 381, 213, 213, 3, 4, 382, 214, 214, 3, 4, 383, 107, 107,
  3, 107, 384, 108, 108, 3, 108, 385, 109, 109, 3, 109, 5, 5, 5, 3, 5, 386,
  110, 110, 3, 110, 387, 111, 111, 3, 111, 388, 215, 215, 3, 4, 389, 216, 216,
  3, 4, 390, 217, 217, 3, 4, 5, 5, 5, 3, 4, 391, 218, 218, 3, 4, 392, 219, 219,
  3, 4, 393, 112, 112, 112, 4, 394, 113, 113, 113, 4, 395, 114, 114, 114, 4, 5,
  5, 5, 5, 4, 396, 115, 115, 115, 4, 397, 116, 116, 116, 4, 398, 220, 220, 3,
  4, 399, 221, 221, 3, 4, 400, 222, 222, 3, 4, 5, 5, 5, 3, 4, 401, 223, 223, 3,
  4, 402, 224, 224, 3, 4, 403, 69, 69, 69, 69, 404, 70, 70, 70, 70, 405, 71,
  71, 71, 71, 5, 5, 5, 5, 5, 406, 72, 72, 72, 72, 407, 73, 73, 73, 73, 408,
  225, 225, 3, 4, 409, 226, 226, 3, 4, 410, 227, 227, 3, 4, 5, 5, 5, 3, 4, 411,
  228, 228, 3, 4, 412, 229, 229, 3, 4, 413, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 414, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1,
  1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1,
  3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3,
  4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4,
  1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3,
  4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1,
  1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1,
  4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3,
  4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1,
  1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1,
  1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1,
  1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1,
  1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3,
  1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1, 4, 1,
  1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1,
  1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1,
  3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 3, 4,
  1, 1, 1, 3, 4, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 250, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 250, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 250, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1
};

/* clang-format on */

static size_t RtemsTaskReqDelete_Scope( void *arg, char *buf, size_t n )
{
  RtemsTaskReqDelete_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( RtemsTaskReqDelete_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture RtemsTaskReqDelete_Fixture = {
  .setup = RtemsTaskReqDelete_Setup_Wrap,
  .stop = NULL,
  .teardown = RtemsTaskReqDelete_Teardown_Wrap,
  .scope = RtemsTaskReqDelete_Scope,
  .initial_context = &RtemsTaskReqDelete_Instance
};

static inline RtemsTaskReqDelete_Entry RtemsTaskReqDelete_PopEntry(
  RtemsTaskReqDelete_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsTaskReqDelete_Entries[ RtemsTaskReqDelete_Map[ index ] ];
}

static void RtemsTaskReqDelete_SetPreConditionStates(
  RtemsTaskReqDelete_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];

  if ( ctx->Map.entry.Pre_CallerPriority_NA ) {
    ctx->Map.pcs[ 3 ] = RtemsTaskReqDelete_Pre_CallerPriority_NA;
  } else {
    ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  }

  if ( ctx->Map.entry.Pre_Dormant_NA ) {
    ctx->Map.pcs[ 4 ] = RtemsTaskReqDelete_Pre_Dormant_NA;
  } else {
    ctx->Map.pcs[ 4 ] = ctx->Map.pci[ 4 ];
  }

  if ( ctx->Map.entry.Pre_Suspended_NA ) {
    ctx->Map.pcs[ 5 ] = RtemsTaskReqDelete_Pre_Suspended_NA;
  } else {
    ctx->Map.pcs[ 5 ] = ctx->Map.pci[ 5 ];
  }

  if ( ctx->Map.entry.Pre_Restarting_NA ) {
    ctx->Map.pcs[ 6 ] = RtemsTaskReqDelete_Pre_Restarting_NA;
  } else {
    ctx->Map.pcs[ 6 ] = ctx->Map.pci[ 6 ];
  }

  if ( ctx->Map.entry.Pre_Terminating_NA ) {
    ctx->Map.pcs[ 7 ] = RtemsTaskReqDelete_Pre_Terminating_NA;
  } else {
    ctx->Map.pcs[ 7 ] = ctx->Map.pci[ 7 ];
  }

  if ( ctx->Map.entry.Pre_Protected_NA ) {
    ctx->Map.pcs[ 8 ] = RtemsTaskReqDelete_Pre_Protected_NA;
  } else {
    ctx->Map.pcs[ 8 ] = ctx->Map.pci[ 8 ];
  }

  if ( ctx->Map.entry.Pre_State_NA ) {
    ctx->Map.pcs[ 9 ] = RtemsTaskReqDelete_Pre_State_NA;
  } else {
    ctx->Map.pcs[ 9 ] = ctx->Map.pci[ 9 ];
  }

  if ( ctx->Map.entry.Pre_Timer_NA ) {
    ctx->Map.pcs[ 10 ] = RtemsTaskReqDelete_Pre_Timer_NA;
  } else {
    ctx->Map.pcs[ 10 ] = ctx->Map.pci[ 10 ];
  }

  if ( ctx->Map.entry.Pre_UserExtIter_NA ) {
    ctx->Map.pcs[ 11 ] = RtemsTaskReqDelete_Pre_UserExtIter_NA;
  } else {
    ctx->Map.pcs[ 11 ] = ctx->Map.pci[ 11 ];
  }
}

static void RtemsTaskReqDelete_TestVariant( RtemsTaskReqDelete_Context *ctx )
{
  RtemsTaskReqDelete_Pre_Id_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  RtemsTaskReqDelete_Pre_Context_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  RtemsTaskReqDelete_Pre_ThreadDispatch_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  RtemsTaskReqDelete_Pre_CallerPriority_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  RtemsTaskReqDelete_Pre_Dormant_Prepare( ctx, ctx->Map.pcs[ 4 ] );
  RtemsTaskReqDelete_Pre_Suspended_Prepare( ctx, ctx->Map.pcs[ 5 ] );
  RtemsTaskReqDelete_Pre_Restarting_Prepare( ctx, ctx->Map.pcs[ 6 ] );
  RtemsTaskReqDelete_Pre_Terminating_Prepare( ctx, ctx->Map.pcs[ 7 ] );
  RtemsTaskReqDelete_Pre_Protected_Prepare( ctx, ctx->Map.pcs[ 8 ] );
  RtemsTaskReqDelete_Pre_State_Prepare( ctx, ctx->Map.pcs[ 9 ] );
  RtemsTaskReqDelete_Pre_Timer_Prepare( ctx, ctx->Map.pcs[ 10 ] );
  RtemsTaskReqDelete_Pre_UserExtIter_Prepare( ctx, ctx->Map.pcs[ 11 ] );
  RtemsTaskReqDelete_Action( ctx );
  RtemsTaskReqDelete_Post_Status_Check( ctx, ctx->Map.entry.Post_Status );
  RtemsTaskReqDelete_Post_FatalError_Check(
    ctx,
    ctx->Map.entry.Post_FatalError
  );
  RtemsTaskReqDelete_Post_Zombie_Check( ctx, ctx->Map.entry.Post_Zombie );
  RtemsTaskReqDelete_Post_TaskPriority_Check(
    ctx,
    ctx->Map.entry.Post_TaskPriority
  );
  RtemsTaskReqDelete_Post_RestartExtensions_Check(
    ctx,
    ctx->Map.entry.Post_RestartExtensions
  );
  RtemsTaskReqDelete_Post_TerminateExtensions_Check(
    ctx,
    ctx->Map.entry.Post_TerminateExtensions
  );
  RtemsTaskReqDelete_Post_Dormant_Check( ctx, ctx->Map.entry.Post_Dormant );
  RtemsTaskReqDelete_Post_Suspended_Check(
    ctx,
    ctx->Map.entry.Post_Suspended
  );
  RtemsTaskReqDelete_Post_Restarting_Check(
    ctx,
    ctx->Map.entry.Post_Restarting
  );
  RtemsTaskReqDelete_Post_Terminating_Check(
    ctx,
    ctx->Map.entry.Post_Terminating
  );
  RtemsTaskReqDelete_Post_Protected_Check(
    ctx,
    ctx->Map.entry.Post_Protected
  );
  RtemsTaskReqDelete_Post_State_Check( ctx, ctx->Map.entry.Post_State );
  RtemsTaskReqDelete_Post_Timer_Check( ctx, ctx->Map.entry.Post_Timer );
  RtemsTaskReqDelete_Post_UserExtIter_Check(
    ctx,
    ctx->Map.entry.Post_UserExtIter
  );
}

/**
 * @fn void T_case_body_RtemsTaskReqDelete( void )
 */
T_TEST_CASE_FIXTURE( RtemsTaskReqDelete, &RtemsTaskReqDelete_Fixture )
{
  RtemsTaskReqDelete_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = RtemsTaskReqDelete_Pre_Id_Executing;
    ctx->Map.pci[ 0 ] < RtemsTaskReqDelete_Pre_Id_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = RtemsTaskReqDelete_Pre_Context_Task;
      ctx->Map.pci[ 1 ] < RtemsTaskReqDelete_Pre_Context_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = RtemsTaskReqDelete_Pre_ThreadDispatch_Disabled;
        ctx->Map.pci[ 2 ] < RtemsTaskReqDelete_Pre_ThreadDispatch_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = RtemsTaskReqDelete_Pre_CallerPriority_Vital;
          ctx->Map.pci[ 3 ] < RtemsTaskReqDelete_Pre_CallerPriority_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          for (
            ctx->Map.pci[ 4 ] = RtemsTaskReqDelete_Pre_Dormant_No;
            ctx->Map.pci[ 4 ] < RtemsTaskReqDelete_Pre_Dormant_NA;
            ++ctx->Map.pci[ 4 ]
          ) {
            for (
              ctx->Map.pci[ 5 ] = RtemsTaskReqDelete_Pre_Suspended_Yes;
              ctx->Map.pci[ 5 ] < RtemsTaskReqDelete_Pre_Suspended_NA;
              ++ctx->Map.pci[ 5 ]
            ) {
              for (
                ctx->Map.pci[ 6 ] = RtemsTaskReqDelete_Pre_Restarting_No;
                ctx->Map.pci[ 6 ] < RtemsTaskReqDelete_Pre_Restarting_NA;
                ++ctx->Map.pci[ 6 ]
              ) {
                for (
                  ctx->Map.pci[ 7 ] = RtemsTaskReqDelete_Pre_Terminating_No;
                  ctx->Map.pci[ 7 ] < RtemsTaskReqDelete_Pre_Terminating_NA;
                  ++ctx->Map.pci[ 7 ]
                ) {
                  for (
                    ctx->Map.pci[ 8 ] = RtemsTaskReqDelete_Pre_Protected_Yes;
                    ctx->Map.pci[ 8 ] < RtemsTaskReqDelete_Pre_Protected_NA;
                    ++ctx->Map.pci[ 8 ]
                  ) {
                    for (
                      ctx->Map.pci[ 9 ] =
                        RtemsTaskReqDelete_Pre_State_Enqueued;
                      ctx->Map.pci[ 9 ] < RtemsTaskReqDelete_Pre_State_NA;
                      ++ctx->Map.pci[ 9 ]
                    ) {
                      for (
                        ctx->Map.pci[ 10 ] =
                          RtemsTaskReqDelete_Pre_Timer_Inactive;
                        ctx->Map.pci[ 10 ] < RtemsTaskReqDelete_Pre_Timer_NA;
                        ++ctx->Map.pci[ 10 ]
                      ) {
                        for (
                          ctx->Map.pci[ 11 ] =
                            RtemsTaskReqDelete_Pre_UserExtIter_None;
                          ctx->Map.pci[ 11 ] <
                          RtemsTaskReqDelete_Pre_UserExtIter_NA;
                          ++ctx->Map.pci[ 11 ]
                        ) {
                          ctx->Map.entry = RtemsTaskReqDelete_PopEntry( ctx );

                          if ( ctx->Map.entry.Skip ) {
                            continue;
                          }

                          RtemsTaskReqDelete_SetPreConditionStates( ctx );
                          RtemsTaskReqDelete_Prepare( ctx );
                          RtemsTaskReqDelete_TestVariant( ctx );
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

/** @} */
