/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexSetprioceiling
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
#include <rtems/test-scheduler.h>
#include <rtems/posix/muteximpl.h>
#include <rtems/score/threadimpl.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqMutexSetprioceiling \
 *   spec:/c/pthread/req/mutex-setprioceiling
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexSetprioceiling_Pre_Mutex_Init,
  CPthreadReqMutexSetprioceiling_Pre_Mutex_Static,
  CPthreadReqMutexSetprioceiling_Pre_Mutex_Corrupted,
  CPthreadReqMutexSetprioceiling_Pre_Mutex_Null,
  CPthreadReqMutexSetprioceiling_Pre_Mutex_NA
} CPthreadReqMutexSetprioceiling_Pre_Mutex;

typedef enum {
  CPthreadReqMutexSetprioceiling_Pre_Protocol_None,
  CPthreadReqMutexSetprioceiling_Pre_Protocol_Inherit,
  CPthreadReqMutexSetprioceiling_Pre_Protocol_Protect,
  CPthreadReqMutexSetprioceiling_Pre_Protocol_NA
} CPthreadReqMutexSetprioceiling_Pre_Protocol;

typedef enum {
  CPthreadReqMutexSetprioceiling_Pre_Ceiling_Valid,
  CPthreadReqMutexSetprioceiling_Pre_Ceiling_Maximum,
  CPthreadReqMutexSetprioceiling_Pre_Ceiling_NA
} CPthreadReqMutexSetprioceiling_Pre_Ceiling;

typedef enum {
  CPthreadReqMutexSetprioceiling_Pre_Type_Normal,
  CPthreadReqMutexSetprioceiling_Pre_Type_Recursive,
  CPthreadReqMutexSetprioceiling_Pre_Type_Errorcheck,
  CPthreadReqMutexSetprioceiling_Pre_Type_Default,
  CPthreadReqMutexSetprioceiling_Pre_Type_NA
} CPthreadReqMutexSetprioceiling_Pre_Type;

typedef enum {
  CPthreadReqMutexSetprioceiling_Pre_New_Valid,
  CPthreadReqMutexSetprioceiling_Pre_New_High,
  CPthreadReqMutexSetprioceiling_Pre_New_Low,
  CPthreadReqMutexSetprioceiling_Pre_New_NA
} CPthreadReqMutexSetprioceiling_Pre_New;

typedef enum {
  CPthreadReqMutexSetprioceiling_Pre_Old_Obj,
  CPthreadReqMutexSetprioceiling_Pre_Old_Null,
  CPthreadReqMutexSetprioceiling_Pre_Old_NA
} CPthreadReqMutexSetprioceiling_Pre_Old;

typedef enum {
  CPthreadReqMutexSetprioceiling_Pre_Owner_None,
  CPthreadReqMutexSetprioceiling_Pre_Owner_Caller,
  CPthreadReqMutexSetprioceiling_Pre_Owner_Other,
  CPthreadReqMutexSetprioceiling_Pre_Owner_NA
} CPthreadReqMutexSetprioceiling_Pre_Owner;

typedef enum {
  CPthreadReqMutexSetprioceiling_Pre_Priority_Satisfied,
  CPthreadReqMutexSetprioceiling_Pre_Priority_Violated,
  CPthreadReqMutexSetprioceiling_Pre_Priority_NA
} CPthreadReqMutexSetprioceiling_Pre_Priority;

typedef enum {
  CPthreadReqMutexSetprioceiling_Pre_Scheduler_Home,
  CPthreadReqMutexSetprioceiling_Pre_Scheduler_Other,
  CPthreadReqMutexSetprioceiling_Pre_Scheduler_NA
} CPthreadReqMutexSetprioceiling_Pre_Scheduler;

typedef enum {
  CPthreadReqMutexSetprioceiling_Post_Status_Ok,
  CPthreadReqMutexSetprioceiling_Post_Status_Inval,
  CPthreadReqMutexSetprioceiling_Post_Status_Deadlock,
  CPthreadReqMutexSetprioceiling_Post_Status_NA
} CPthreadReqMutexSetprioceiling_Post_Status;

typedef enum {
  CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
  CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
  CPthreadReqMutexSetprioceiling_Post_OldCeiling_Zero,
  CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA
} CPthreadReqMutexSetprioceiling_Post_OldCeiling;

typedef enum {
  CPthreadReqMutexSetprioceiling_Post_Ceiling_New,
  CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
  CPthreadReqMutexSetprioceiling_Post_Ceiling_NA
} CPthreadReqMutexSetprioceiling_Post_Ceiling;

typedef enum {
  CPthreadReqMutexSetprioceiling_Post_Owner_None,
  CPthreadReqMutexSetprioceiling_Post_Owner_Caller,
  CPthreadReqMutexSetprioceiling_Post_Owner_NA
} CPthreadReqMutexSetprioceiling_Post_Owner;

typedef enum {
  CPthreadReqMutexSetprioceiling_Post_Wait_Yes,
  CPthreadReqMutexSetprioceiling_Post_Wait_No,
  CPthreadReqMutexSetprioceiling_Post_Wait_NA
} CPthreadReqMutexSetprioceiling_Post_Wait;

typedef enum {
  CPthreadReqMutexSetprioceiling_Post_Priority_Ceiling,
  CPthreadReqMutexSetprioceiling_Post_Priority_Nop,
  CPthreadReqMutexSetprioceiling_Post_Priority_NA
} CPthreadReqMutexSetprioceiling_Post_Priority;

typedef struct {
  uint32_t Skip : 1;
  uint32_t Pre_Mutex_NA : 1;
  uint32_t Pre_Protocol_NA : 1;
  uint32_t Pre_Ceiling_NA : 1;
  uint32_t Pre_Type_NA : 1;
  uint32_t Pre_New_NA : 1;
  uint32_t Pre_Old_NA : 1;
  uint32_t Pre_Owner_NA : 1;
  uint32_t Pre_Priority_NA : 1;
  uint32_t Pre_Scheduler_NA : 1;
  uint32_t Post_Status : 2;
  uint32_t Post_OldCeiling : 2;
  uint32_t Post_Ceiling : 2;
  uint32_t Post_Owner : 2;
  uint32_t Post_Wait : 2;
  uint32_t Post_Priority : 2;
} CPthreadReqMutexSetprioceiling_Entry;

typedef enum { OWNER_NONE, OWNER_CALLER, OWNER_OTHER } OwnerKind;

/**
 * @brief Test context for spec:/c/pthread/req/mutex-setprioceiling test case.
 */
typedef struct {
  /**
   * @brief This member provides the object referenced by the `mutex`
   *   parameter.
   */
  pthread_mutex_t mutex_obj;

  /**
   * @brief This member specifies the `mutex` parameter value.
   */
  pthread_mutex_t *mutex;

  /**
   * @brief This member provides the mutex attributes.
   */
  pthread_mutexattr_t attr_obj;

  /**
   * @brief This member is true, if the mutex shall be initialized with the
   *   mutex attributes.
   */
  bool init;

  /**
   * @brief This member specifies the `prioceiling` parameter value.
   */
  int new_ceiling;

  /**
   * @brief This member specifies the `old_ceiling` parameter value.
   */
  int *old;

  /**
   * @brief This member provides the object referenced by the `old_ceiling`
   *   parameter.
   */
  int old_obj;

  /**
   * @brief This member specifies the owner of the mutex before the call.
   */
  OwnerKind owner;

  /**
   * @brief This member is true, if the calling thread shall have a priority
   *   higher than the priority ceiling of the mutex.
   */
  bool priority_high;

  /**
   * @brief This member is true, if the home scheduler of the calling thread
   *   shall not be the scheduler of the mutex.
   */
  bool scheduler_other;

  /**
   * @brief This member contains the priority ceiling of the mutex before the
   *   call.
   */
  int ceiling_before;

  /**
   * @brief This member contains the current priority of the calling thread
   *   before the call.
   */
  rtems_task_priority priority_before;

  /**
   * @brief This member contains the current priority of the calling thread
   *   after the call.
   */
  rtems_task_priority priority_after;

  /**
   * @brief This member contains the owner of the mutex after the call.
   */
  const rtems_tcb *owner_after;

  /**
   * @brief This member is true, if the calling thread blocked in the call.
   */
  bool blocked;

  /**
   * @brief This member contains the thread control block of the runner.
   */
  const rtems_tcb *runner;

  /**
   * @brief This member contains the identifier of the worker.
   */
  rtems_id worker_id;

  /**
   * @brief This member provides the request which lets the worker surrender
   *   the mutex.
   */
  CallWithinISRRequest request;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutex_setprioceiling() call.
   */
  int status;

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 9 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 9 ];

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
    CPthreadReqMutexSetprioceiling_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexSetprioceiling_Context;

static CPthreadReqMutexSetprioceiling_Context
  CPthreadReqMutexSetprioceiling_Instance;

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_Mutex[] =
  { "Init", "Static", "Corrupted", "Null", "NA" };

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_Protocol[] =
  { "None", "Inherit", "Protect", "NA" };

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_Ceiling[] =
  { "Valid", "Maximum", "NA" };

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_Type[] =
  { "Normal", "Recursive", "Errorcheck", "Default", "NA" };

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_New[] =
  { "Valid", "High", "Low", "NA" };

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_Old[] =
  { "Obj", "Null", "NA" };

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_Owner[] =
  { "None", "Caller", "Other", "NA" };

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_Priority[] =
  { "Satisfied", "Violated", "NA" };

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_Scheduler[] =
  { "Home", "Other", "NA" };

static const char *const *const CPthreadReqMutexSetprioceiling_PreDesc[] = {
  CPthreadReqMutexSetprioceiling_PreDesc_Mutex,
  CPthreadReqMutexSetprioceiling_PreDesc_Protocol,
  CPthreadReqMutexSetprioceiling_PreDesc_Ceiling,
  CPthreadReqMutexSetprioceiling_PreDesc_Type,
  CPthreadReqMutexSetprioceiling_PreDesc_New,
  CPthreadReqMutexSetprioceiling_PreDesc_Old,
  CPthreadReqMutexSetprioceiling_PreDesc_Owner,
  CPthreadReqMutexSetprioceiling_PreDesc_Priority,
  CPthreadReqMutexSetprioceiling_PreDesc_Scheduler,
  NULL
};

typedef CPthreadReqMutexSetprioceiling_Context Context;

#define EVENT_LOCK RTEMS_EVENT_0

#define EVENT_UNLOCK RTEMS_EVENT_1

#define EVENT_LOCKED RTEMS_EVENT_2

static rtems_task_priority GetCurrentPriority( void )
{
  rtems_status_code sc;
  rtems_id          scheduler_id;

  sc = rtems_task_get_scheduler( RTEMS_SELF, &scheduler_id );
  T_rsc_success( sc );

  return GetPriorityByScheduler( RTEMS_SELF, scheduler_id );
}

static int GetCeiling( Context *ctx )
{
  int ceiling;

  if ( pthread_mutex_getprioceiling( &ctx->mutex_obj, &ceiling ) != 0 ) {
    return -1;
  }

  return ceiling;
}

static void Worker( rtems_task_argument arg )
{
  Context *ctx;

  ctx = (Context *) arg;

  while ( true ) {
    rtems_event_set events;

    events = ReceiveAnyEvents();

    if ( ( events & EVENT_LOCK ) != 0 ) {
      T_eq_int( pthread_mutex_lock( ctx->mutex ), 0 );
      SendEvents( ctx->runner->Object.id, EVENT_LOCKED );
    }

    if ( ( events & EVENT_UNLOCK ) != 0 ) {
      T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );
    }
  }
}

static void Surrender( void *arg )
{
  Context *ctx;

  ctx = arg;
  SendEvents( ctx->worker_id, EVENT_UNLOCK );
}

static void SchedulerBlock(
  void                    *arg,
  const T_scheduler_event *event,
  T_scheduler_when         when
)
{
  Context *ctx;

  ctx = arg;

  if (
    when == T_SCHEDULER_BEFORE && event->operation == T_SCHEDULER_BLOCK &&
    event->thread == ctx->runner
  ) {
    T_scheduler_set_event_handler( NULL, NULL );
    ctx->blocked = true;
    CallWithinISRSubmit( &ctx->request );
  }
}

static void Action( Context *ctx )
{
  if ( ctx->init ) {
    T_eq_int( pthread_mutex_init( &ctx->mutex_obj, &ctx->attr_obj ), 0 );
    ctx->ceiling_before = GetCeiling( ctx );
  }

  if ( ctx->owner == OWNER_CALLER ) {
    T_eq_int( pthread_mutex_lock( ctx->mutex ), 0 );
  } else if ( ctx->owner == OWNER_OTHER ) {
    SendEvents( ctx->worker_id, EVENT_LOCK );
    T_eq_u32( ReceiveAnyEvents(), EVENT_LOCKED );
    T_scheduler_set_event_handler( SchedulerBlock, ctx );
  }

  if ( ctx->priority_high ) {
    SetSelfPriority( PRIO_ULTRA_HIGH );
  }

  if ( ctx->scheduler_other ) {
#if defined( RTEMS_SMP )
    SetSelfScheduler( SCHEDULER_B_ID, PRIO_NORMAL );
#endif
  }

  ctx->priority_before = GetCurrentPriority();
  ctx->status = pthread_mutex_setprioceiling(
    ctx->mutex,
    ctx->new_ceiling,
    ctx->old
  );
  ctx->priority_after = GetCurrentPriority();
  T_scheduler_set_event_handler( NULL, NULL );

  if ( ctx->init ) {
    ctx->owner_after = _POSIX_Mutex_Get_owner(
      _POSIX_Mutex_Get( &ctx->mutex_obj )
    );
  }
}

static void CPthreadReqMutexSetprioceiling_Pre_Mutex_Prepare(
  CPthreadReqMutexSetprioceiling_Context  *ctx,
  CPthreadReqMutexSetprioceiling_Pre_Mutex state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Pre_Mutex_Init: {
      /*
       * While the `mutex` parameter references an initialized mutex.
       */
      ctx->mutex = &ctx->mutex_obj;
      ctx->init = true;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Mutex_Static: {
      /*
       * While the object referenced by the `mutex` parameter is equal to
       * PTHREAD_MUTEX_INITIALIZER.
       */
      static const pthread_mutex_t initializer = PTHREAD_MUTEX_INITIALIZER;

      ctx->mutex_obj = initializer;
      ctx->mutex = &ctx->mutex_obj;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Mutex_Corrupted: {
      /*
       * While the `mutex` parameter references an object which is not a mutex.
       */
      memset( &ctx->mutex_obj, 0xff, sizeof( ctx->mutex_obj ) );
      ctx->mutex = &ctx->mutex_obj;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Mutex_Null: {
      /*
       * While the `mutex` parameter is equal to NULL.
       */
      ctx->mutex = NULL;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Mutex_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Pre_Protocol_Prepare(
  CPthreadReqMutexSetprioceiling_Context     *ctx,
  CPthreadReqMutexSetprioceiling_Pre_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Pre_Protocol_None: {
      /*
       * While the mutex protocol attribute of the mutex is PTHREAD_PRIO_NONE.
       */
      T_eq_int(
        pthread_mutexattr_setprotocol( &ctx->attr_obj, PTHREAD_PRIO_NONE ),
        0
      );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Protocol_Inherit: {
      /*
       * While the mutex protocol attribute of the mutex is
       * PTHREAD_PRIO_INHERIT.
       */
      T_eq_int(
        pthread_mutexattr_setprotocol( &ctx->attr_obj, PTHREAD_PRIO_INHERIT ),
        0
      );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Protocol_Protect: {
      /*
       * While the mutex protocol attribute of the mutex is
       * PTHREAD_PRIO_PROTECT.
       */
      T_eq_int(
        pthread_mutexattr_setprotocol( &ctx->attr_obj, PTHREAD_PRIO_PROTECT ),
        0
      );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Pre_Ceiling_Prepare(
  CPthreadReqMutexSetprioceiling_Context    *ctx,
  CPthreadReqMutexSetprioceiling_Pre_Ceiling state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Pre_Ceiling_Valid: {
      /*
       * While the mutex priority ceiling attribute of the mutex is a thread
       * priority which is less than the maximum thread priority.
       */
      T_eq_int(
        pthread_mutexattr_setprioceiling(
          &ctx->attr_obj,
          ToPOSIXPriority( PRIO_VERY_HIGH )
        ),
        0
      );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Ceiling_Maximum: {
      /*
       * While the mutex priority ceiling attribute of the mutex is the maximum
       * thread priority.
       */
      T_eq_int(
        pthread_mutexattr_setprioceiling( &ctx->attr_obj, INT_MAX ),
        0
      );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Ceiling_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Pre_Type_Prepare(
  CPthreadReqMutexSetprioceiling_Context *ctx,
  CPthreadReqMutexSetprioceiling_Pre_Type state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Pre_Type_Normal: {
      /*
       * While the mutex type attribute of the mutex is PTHREAD_MUTEX_NORMAL.
       */
      T_eq_int(
        pthread_mutexattr_settype( &ctx->attr_obj, PTHREAD_MUTEX_NORMAL ),
        0
      );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Type_Recursive: {
      /*
       * While the mutex type attribute of the mutex is
       * PTHREAD_MUTEX_RECURSIVE.
       */
      T_eq_int(
        pthread_mutexattr_settype( &ctx->attr_obj, PTHREAD_MUTEX_RECURSIVE ),
        0
      );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Type_Errorcheck: {
      /*
       * While the mutex type attribute of the mutex is
       * PTHREAD_MUTEX_ERRORCHECK.
       */
      T_eq_int(
        pthread_mutexattr_settype( &ctx->attr_obj, PTHREAD_MUTEX_ERRORCHECK ),
        0
      );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Type_Default: {
      /*
       * While the mutex type attribute of the mutex is PTHREAD_MUTEX_DEFAULT.
       */
      T_eq_int(
        pthread_mutexattr_settype( &ctx->attr_obj, PTHREAD_MUTEX_DEFAULT ),
        0
      );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Type_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Pre_New_Prepare(
  CPthreadReqMutexSetprioceiling_Context *ctx,
  CPthreadReqMutexSetprioceiling_Pre_New  state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Pre_New_Valid: {
      /*
       * While the `prioceiling` parameter is a valid thread priority which
       * differs from the mutex priority ceiling attribute of the mutex.
       */
      ctx->new_ceiling = ToPOSIXPriority( PRIO_ULTRA_HIGH );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_New_High: {
      /*
       * While the `prioceiling` parameter is greater than the maximum thread
       * priority.
       */
      ctx->new_ceiling = GetPOSIXPriorityMaximum() + 1;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_New_Low: {
      /*
       * While the `prioceiling` parameter is less than the minimum thread
       * priority.
       */
      ctx->new_ceiling = GetPOSIXPriorityMinimum() - 1;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_New_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Pre_Old_Prepare(
  CPthreadReqMutexSetprioceiling_Context *ctx,
  CPthreadReqMutexSetprioceiling_Pre_Old  state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Pre_Old_Obj: {
      /*
       * While the `old_ceiling` parameter references an object of type int.
       */
      ctx->old = &ctx->old_obj;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Old_Null: {
      /*
       * While the `old_ceiling` parameter is equal to NULL.
       */
      ctx->old = NULL;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Old_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Pre_Owner_Prepare(
  CPthreadReqMutexSetprioceiling_Context  *ctx,
  CPthreadReqMutexSetprioceiling_Pre_Owner state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Pre_Owner_None: {
      /*
       * While the mutex has no owner.
       */
      ctx->owner = OWNER_NONE;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Owner_Caller: {
      /*
       * While the owner of the mutex is the calling thread.
       */
      ctx->owner = OWNER_CALLER;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Owner_Other: {
      /*
       * While the owner of the mutex is a thread other than the calling
       * thread.
       */
      ctx->owner = OWNER_OTHER;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Owner_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Pre_Priority_Prepare(
  CPthreadReqMutexSetprioceiling_Context     *ctx,
  CPthreadReqMutexSetprioceiling_Pre_Priority state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Pre_Priority_Satisfied: {
      /*
       * While the current priority of the calling thread is not higher than
       * the priority which corresponds to the mutex priority ceiling attribute
       * of the mutex.
       */
      ctx->priority_high = false;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Priority_Violated: {
      /*
       * While the current priority of the calling thread is higher than the
       * priority which corresponds to the mutex priority ceiling attribute of
       * the mutex.
       */
      ctx->priority_high = true;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Priority_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Pre_Scheduler_Prepare(
  CPthreadReqMutexSetprioceiling_Context      *ctx,
  CPthreadReqMutexSetprioceiling_Pre_Scheduler state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Pre_Scheduler_Home: {
      /*
       * While the home scheduler of the calling thread is the scheduler
       * instance of the mutex.
       */
      ctx->scheduler_other = false;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Scheduler_Other: {
      /*
       * While the home scheduler of the calling thread is not the scheduler
       * instance of the mutex.
       */
      ctx->scheduler_other = true;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Scheduler_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Post_Status_Check(
  CPthreadReqMutexSetprioceiling_Context    *ctx,
  CPthreadReqMutexSetprioceiling_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Post_Status_Ok: {
      /*
       * The return value of pthread_mutex_setprioceiling() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Status_Inval: {
      /*
       * The return value of pthread_mutex_setprioceiling() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Status_Deadlock: {
      /*
       * The return value of pthread_mutex_setprioceiling() shall be EDEADLK.
       */
      T_eq_int( ctx->status, EDEADLK );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Post_OldCeiling_Check(
  CPthreadReqMutexSetprioceiling_Context        *ctx,
  CPthreadReqMutexSetprioceiling_Post_OldCeiling state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid: {
      /*
       * The value of the object referenced by the `old_ceiling` parameter
       * shall be the mutex priority ceiling attribute of the mutex before the
       * call.
       */
      T_eq_int( ctx->old_obj, ToPOSIXPriority( PRIO_VERY_HIGH ) );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum: {
      /*
       * The value of the object referenced by the `old_ceiling` parameter
       * shall be the maximum thread priority.
       */
      T_eq_int( ctx->old_obj, GetPOSIXPriorityMaximum() );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_OldCeiling_Zero: {
      /*
       * The value of the object referenced by the `old_ceiling` parameter
       * shall be zero.
       */
      T_eq_int( ctx->old_obj, 0 );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Post_Ceiling_Check(
  CPthreadReqMutexSetprioceiling_Context     *ctx,
  CPthreadReqMutexSetprioceiling_Post_Ceiling state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Post_Ceiling_New: {
      /*
       * The mutex priority ceiling attribute of the mutex shall be the value
       * of the `prioceiling` parameter.
       */
      T_eq_int( GetCeiling( ctx ), ctx->new_ceiling );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop: {
      /*
       * The mutex priority ceiling attribute of the mutex shall not be
       * modified.
       */
      T_eq_int( GetCeiling( ctx ), ctx->ceiling_before );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Ceiling_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Post_Owner_Check(
  CPthreadReqMutexSetprioceiling_Context   *ctx,
  CPthreadReqMutexSetprioceiling_Post_Owner state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Post_Owner_None: {
      /*
       * The mutex shall have no owner.
       */
      T_null( ctx->owner_after );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Owner_Caller: {
      /*
       * The owner of the mutex shall be the calling thread.
       */
      T_eq_ptr( ctx->owner_after, ctx->runner );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Owner_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Post_Wait_Check(
  CPthreadReqMutexSetprioceiling_Context  *ctx,
  CPthreadReqMutexSetprioceiling_Post_Wait state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Post_Wait_Yes: {
      /*
       * The calling thread shall wait until the other thread surrenders the
       * mutex.
       */
      T_true( ctx->blocked );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Wait_No: {
      /*
       * The calling thread shall not wait.
       */
      T_false( ctx->blocked );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Wait_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Post_Priority_Check(
  CPthreadReqMutexSetprioceiling_Context      *ctx,
  CPthreadReqMutexSetprioceiling_Post_Priority state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Post_Priority_Ceiling: {
      /*
       * The current priority of the calling thread shall be the priority which
       * corresponds to the value of the `prioceiling` parameter.
       */
      T_eq_u32( ctx->priority_after, PRIO_ULTRA_HIGH );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Priority_Nop: {
      /*
       * The current priority of the calling thread shall not be modified.
       */
      T_eq_u32( ctx->priority_after, ctx->priority_before );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Priority_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Setup(
  CPthreadReqMutexSetprioceiling_Context *ctx
)
{
  SetSelfPriority( PRIO_NORMAL );
  ctx->runner = _Thread_Get_executing();
  ctx->request.handler = Surrender;
  ctx->request.arg = ctx;
  ctx->worker_id = CreateTask( "WORK", PRIO_HIGH );
  StartTask( ctx->worker_id, Worker, ctx );
}

static void CPthreadReqMutexSetprioceiling_Setup_Wrap( void *arg )
{
  CPthreadReqMutexSetprioceiling_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexSetprioceiling_Setup( ctx );
}

static void CPthreadReqMutexSetprioceiling_Teardown(
  CPthreadReqMutexSetprioceiling_Context *ctx
)
{
  DeleteTask( ctx->worker_id );
  RestoreRunnerPriority();
}

static void CPthreadReqMutexSetprioceiling_Teardown_Wrap( void *arg )
{
  CPthreadReqMutexSetprioceiling_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexSetprioceiling_Teardown( ctx );
}

static void CPthreadReqMutexSetprioceiling_Prepare(
  CPthreadReqMutexSetprioceiling_Context *ctx
)
{
  ctx->init = false;
  ctx->new_ceiling = 0;
  ctx->old_obj = -1;
  ctx->owner = OWNER_NONE;
  ctx->priority_high = false;
  ctx->scheduler_other = false;
  ctx->ceiling_before = -1;
  ctx->owner_after = NULL;
  ctx->blocked = false;
  ctx->status = -1;
  T_eq_int( pthread_mutexattr_init( &ctx->attr_obj ), 0 );
}

static void CPthreadReqMutexSetprioceiling_Action(
  CPthreadReqMutexSetprioceiling_Context *ctx
)
{
  Action( ctx );
}

static void CPthreadReqMutexSetprioceiling_Cleanup(
  CPthreadReqMutexSetprioceiling_Context *ctx
)
{
  if ( ctx->scheduler_other ) {
    SetSelfScheduler( SCHEDULER_A_ID, PRIO_NORMAL );
  }

  if ( ctx->owner == OWNER_CALLER ) {
    T_eq_int( pthread_mutex_unlock( ctx->mutex ), 0 );
  }

  if ( ctx->priority_high ) {
    SetSelfPriority( PRIO_NORMAL );
  }

  if ( ctx->init ) {
    T_eq_int( pthread_mutex_destroy( &ctx->mutex_obj ), 0 );
  }
}

/* clang-format off */

static const CPthreadReqMutexSetprioceiling_Entry
CPthreadReqMutexSetprioceiling_Entries[] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    CPthreadReqMutexSetprioceiling_Post_Status_NA,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Owner_NA,
    CPthreadReqMutexSetprioceiling_Post_Wait_NA,
    CPthreadReqMutexSetprioceiling_Post_Priority_NA },
  { 0, 0, 1, 1, 1, 0, 0, 1, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Zero,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Owner_NA,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 1, 1, 1, 0, 0, 1, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Owner_NA,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 1, 1, 0, 0, 1, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Zero,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 1, 1, 0, 0, 1, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 1, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Deadlock,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_Caller,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Deadlock,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_Caller,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 1, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 1, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_New,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
#if defined(RTEMS_SMP)
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
#else
  { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    CPthreadReqMutexSetprioceiling_Post_Status_NA,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Owner_NA,
    CPthreadReqMutexSetprioceiling_Post_Wait_NA,
    CPthreadReqMutexSetprioceiling_Post_Priority_NA },
#endif
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 0, 1, 0, 0, 0, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_New,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_Yes,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_New,
    CPthreadReqMutexSetprioceiling_Post_Owner_Caller,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Ceiling },
  { 0, 0, 0, 0, 1, 0, 0, 0, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_New,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
#if defined(RTEMS_SMP)
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
#else
  { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    CPthreadReqMutexSetprioceiling_Post_Status_NA,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Owner_NA,
    CPthreadReqMutexSetprioceiling_Post_Wait_NA,
    CPthreadReqMutexSetprioceiling_Post_Priority_NA },
#endif
  { 0, 0, 0, 0, 1, 0, 0, 0, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_New,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Wait_Yes,
    CPthreadReqMutexSetprioceiling_Post_Priority_Nop },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_New,
    CPthreadReqMutexSetprioceiling_Post_Owner_Caller,
    CPthreadReqMutexSetprioceiling_Post_Wait_No,
    CPthreadReqMutexSetprioceiling_Post_Priority_Ceiling }
};

static const uint8_t
CPthreadReqMutexSetprioceiling_Map[] = {
  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
  0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
  0, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 0, 6, 0, 0, 0, 13, 0, 0, 0, 5, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 16, 0, 0, 7, 0, 0, 0, 17, 0, 0, 0,
  5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
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
  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0,
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
  0
};

/* clang-format on */

static size_t CPthreadReqMutexSetprioceiling_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexSetprioceiling_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexSetprioceiling_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexSetprioceiling_Fixture = {
  .setup = CPthreadReqMutexSetprioceiling_Setup_Wrap,
  .stop = NULL,
  .teardown = CPthreadReqMutexSetprioceiling_Teardown_Wrap,
  .scope = CPthreadReqMutexSetprioceiling_Scope,
  .initial_context = &CPthreadReqMutexSetprioceiling_Instance
};

static inline CPthreadReqMutexSetprioceiling_Entry
CPthreadReqMutexSetprioceiling_PopEntry(
  CPthreadReqMutexSetprioceiling_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexSetprioceiling_Entries
    [ CPthreadReqMutexSetprioceiling_Map[ index ] ];
}

static void CPthreadReqMutexSetprioceiling_SetPreConditionStates(
  CPthreadReqMutexSetprioceiling_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Protocol_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexSetprioceiling_Pre_Protocol_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_Ceiling_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqMutexSetprioceiling_Pre_Ceiling_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  if ( ctx->Map.entry.Pre_Type_NA ) {
    ctx->Map.pcs[ 3 ] = CPthreadReqMutexSetprioceiling_Pre_Type_NA;
  } else {
    ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  }

  ctx->Map.pcs[ 4 ] = ctx->Map.pci[ 4 ];
  ctx->Map.pcs[ 5 ] = ctx->Map.pci[ 5 ];

  if ( ctx->Map.entry.Pre_Owner_NA ) {
    ctx->Map.pcs[ 6 ] = CPthreadReqMutexSetprioceiling_Pre_Owner_NA;
  } else {
    ctx->Map.pcs[ 6 ] = ctx->Map.pci[ 6 ];
  }

  if ( ctx->Map.entry.Pre_Priority_NA ) {
    ctx->Map.pcs[ 7 ] = CPthreadReqMutexSetprioceiling_Pre_Priority_NA;
  } else {
    ctx->Map.pcs[ 7 ] = ctx->Map.pci[ 7 ];
  }

  if ( ctx->Map.entry.Pre_Scheduler_NA ) {
    ctx->Map.pcs[ 8 ] = CPthreadReqMutexSetprioceiling_Pre_Scheduler_NA;
  } else {
    ctx->Map.pcs[ 8 ] = ctx->Map.pci[ 8 ];
  }
}

static void CPthreadReqMutexSetprioceiling_TestVariant(
  CPthreadReqMutexSetprioceiling_Context *ctx
)
{
  CPthreadReqMutexSetprioceiling_Pre_Mutex_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexSetprioceiling_Pre_Protocol_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexSetprioceiling_Pre_Ceiling_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexSetprioceiling_Pre_Type_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqMutexSetprioceiling_Pre_New_Prepare( ctx, ctx->Map.pcs[ 4 ] );
  CPthreadReqMutexSetprioceiling_Pre_Old_Prepare( ctx, ctx->Map.pcs[ 5 ] );
  CPthreadReqMutexSetprioceiling_Pre_Owner_Prepare( ctx, ctx->Map.pcs[ 6 ] );
  CPthreadReqMutexSetprioceiling_Pre_Priority_Prepare(
    ctx,
    ctx->Map.pcs[ 7 ]
  );
  CPthreadReqMutexSetprioceiling_Pre_Scheduler_Prepare(
    ctx,
    ctx->Map.pcs[ 8 ]
  );
  CPthreadReqMutexSetprioceiling_Action( ctx );
  CPthreadReqMutexSetprioceiling_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexSetprioceiling_Post_OldCeiling_Check(
    ctx,
    ctx->Map.entry.Post_OldCeiling
  );
  CPthreadReqMutexSetprioceiling_Post_Ceiling_Check(
    ctx,
    ctx->Map.entry.Post_Ceiling
  );
  CPthreadReqMutexSetprioceiling_Post_Owner_Check(
    ctx,
    ctx->Map.entry.Post_Owner
  );
  CPthreadReqMutexSetprioceiling_Post_Wait_Check(
    ctx,
    ctx->Map.entry.Post_Wait
  );
  CPthreadReqMutexSetprioceiling_Post_Priority_Check(
    ctx,
    ctx->Map.entry.Post_Priority
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexSetprioceiling( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexSetprioceiling,
  &CPthreadReqMutexSetprioceiling_Fixture
)
{
  CPthreadReqMutexSetprioceiling_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexSetprioceiling_Pre_Mutex_Init;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexSetprioceiling_Pre_Mutex_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexSetprioceiling_Pre_Protocol_None;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexSetprioceiling_Pre_Protocol_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexSetprioceiling_Pre_Ceiling_Valid;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexSetprioceiling_Pre_Ceiling_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexSetprioceiling_Pre_Type_Normal;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexSetprioceiling_Pre_Type_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          for (
            ctx->Map.pci[ 4 ] = CPthreadReqMutexSetprioceiling_Pre_New_Valid;
            ctx->Map.pci[ 4 ] < CPthreadReqMutexSetprioceiling_Pre_New_NA;
            ++ctx->Map.pci[ 4 ]
          ) {
            for (
              ctx->Map.pci[ 5 ] = CPthreadReqMutexSetprioceiling_Pre_Old_Obj;
              ctx->Map.pci[ 5 ] < CPthreadReqMutexSetprioceiling_Pre_Old_NA;
              ++ctx->Map.pci[ 5 ]
            ) {
              for (
                ctx->Map.pci[ 6 ] =
                  CPthreadReqMutexSetprioceiling_Pre_Owner_None;
                ctx->Map.pci[ 6 ] <
                CPthreadReqMutexSetprioceiling_Pre_Owner_NA;
                ++ctx->Map.pci[ 6 ]
              ) {
                for (
                  ctx->Map.pci[ 7 ] =
                    CPthreadReqMutexSetprioceiling_Pre_Priority_Satisfied;
                  ctx->Map.pci[ 7 ] <
                  CPthreadReqMutexSetprioceiling_Pre_Priority_NA;
                  ++ctx->Map.pci[ 7 ]
                ) {
                  for (
                    ctx->Map.pci[ 8 ] =
                      CPthreadReqMutexSetprioceiling_Pre_Scheduler_Home;
                    ctx->Map.pci[ 8 ] <
                    CPthreadReqMutexSetprioceiling_Pre_Scheduler_NA;
                    ++ctx->Map.pci[ 8 ]
                  ) {
                    ctx->Map.entry = CPthreadReqMutexSetprioceiling_PopEntry(
                      ctx
                    );

                    if ( ctx->Map.entry.Skip ) {
                      continue;
                    }

                    CPthreadReqMutexSetprioceiling_SetPreConditionStates(
                      ctx
                    );
                    CPthreadReqMutexSetprioceiling_Prepare( ctx );
                    CPthreadReqMutexSetprioceiling_TestVariant( ctx );
                    CPthreadReqMutexSetprioceiling_Cleanup( ctx );
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
