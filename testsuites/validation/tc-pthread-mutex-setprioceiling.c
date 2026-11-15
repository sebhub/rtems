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
#include <rtems/posix/muteximpl.h>
#include <rtems/posix/priorityimpl.h>

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
  CPthreadReqMutexSetprioceiling_Post_Status_Ok,
  CPthreadReqMutexSetprioceiling_Post_Status_Inval,
  CPthreadReqMutexSetprioceiling_Post_Status_NA
} CPthreadReqMutexSetprioceiling_Post_Status;

typedef enum {
  CPthreadReqMutexSetprioceiling_Post_OldCeiling_Zero,
  CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
  CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
  CPthreadReqMutexSetprioceiling_Post_OldCeiling_Nop,
  CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA
} CPthreadReqMutexSetprioceiling_Post_OldCeiling;

typedef enum {
  CPthreadReqMutexSetprioceiling_Post_Ceiling_Valid,
  CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
  CPthreadReqMutexSetprioceiling_Post_Ceiling_NA
} CPthreadReqMutexSetprioceiling_Post_Ceiling;

typedef enum {
  CPthreadReqMutexSetprioceiling_Post_Owner_None,
  CPthreadReqMutexSetprioceiling_Post_Owner_NA
} CPthreadReqMutexSetprioceiling_Post_Owner;

typedef enum {
  CPthreadReqMutexSetprioceiling_Post_Lock_Released,
  CPthreadReqMutexSetprioceiling_Post_Lock_NA
} CPthreadReqMutexSetprioceiling_Post_Lock;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Mutex_NA : 1;
  uint16_t Pre_Protocol_NA : 1;
  uint16_t Pre_Ceiling_NA : 1;
  uint16_t Pre_New_NA : 1;
  uint16_t Pre_Old_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_OldCeiling : 3;
  uint16_t Post_Ceiling : 2;
  uint16_t Post_Owner : 1;
  uint16_t Post_Lock : 1;
} CPthreadReqMutexSetprioceiling_Entry;

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
   * @brief This member contains a copy of the mutex object before the call.
   */
  pthread_mutex_t mutex_before;

  /**
   * @brief This member specifies the `mutex` parameter value.
   */
  pthread_mutex_t *mutex;

  /**
   * @brief This member specifies the `prioceiling` parameter value.
   */
  int value;

  /**
   * @brief This member specifies the `old_ceiling` parameter value.
   */
  int *out;

  /**
   * @brief This member provides the object referenced by the `old_ceiling`
   *   parameter.
   */
  int out_obj;

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
   * @brief This member contains the core priority of the priority ceiling
   *   before the call.
   */
  Priority_Control priority_before;

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
    size_t pci[ 5 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 5 ];

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

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_New[] =
  { "Valid", "High", "Low", "NA" };

static const char *const CPthreadReqMutexSetprioceiling_PreDesc_Old[] =
  { "Obj", "Null", "NA" };

static const char *const *const CPthreadReqMutexSetprioceiling_PreDesc[] = {
  CPthreadReqMutexSetprioceiling_PreDesc_Mutex,
  CPthreadReqMutexSetprioceiling_PreDesc_Protocol,
  CPthreadReqMutexSetprioceiling_PreDesc_Ceiling,
  CPthreadReqMutexSetprioceiling_PreDesc_New,
  CPthreadReqMutexSetprioceiling_PreDesc_Old,
  NULL
};

typedef CPthreadReqMutexSetprioceiling_Context Context;

static int CeilingValue( void )
{
  return GetPOSIXPriorityMaximum() - 10;
}

static POSIX_Mutex_Control *MutexOf( Context *ctx )
{
  return _POSIX_Mutex_Get( &ctx->mutex_obj );
}

#if defined( RTEMS_SMP )
static const SMP_ticket_lock_Control *MutexTicketLock( Context *ctx )
{
  return &MutexOf( ctx )->Recursive.Mutex.Queue.Queue.Lock;
}
#endif

static int NewValue( void )
{
  return GetPOSIXPriorityMaximum() - 20;
}

static Priority_Control CoreOf( Context *ctx, int priority )
{
  bool             valid;
  Priority_Control core;

  core = _POSIX_Priority_To_core(
    _POSIX_Mutex_Get_scheduler( MutexOf( ctx ) ),
    priority,
    &valid
  );
  T_true( valid );
  return core;
}

static void Action( Context *ctx )
{
  if ( ctx->init ) {
    T_eq_int( pthread_mutex_init( &ctx->mutex_obj, &ctx->attr_obj ), 0 );
  }

  if ( ctx->init ) {
    ctx->priority_before = _POSIX_Mutex_Get_priority( MutexOf( ctx ) );
  }

  memcpy( &ctx->mutex_before, &ctx->mutex_obj, sizeof( ctx->mutex_before ) );
  ctx->status = pthread_mutex_setprioceiling(
    ctx->mutex,
    ctx->value,
    ctx->out
  );
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
       * While the `mutex` parameter references a statically initialized mutex.
       */
      memset( &ctx->mutex_obj, 0, sizeof( ctx->mutex_obj ) );
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
       * While the mutex uses no priority protocol.
       */
      ctx->attr_obj.protocol = PTHREAD_PRIO_NONE;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Protocol_Inherit: {
      /*
       * While the mutex uses the priority inheritance protocol.
       */
      ctx->attr_obj.protocol = PTHREAD_PRIO_INHERIT;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Protocol_Protect: {
      /*
       * While the mutex uses the priority ceiling protocol.
       */
      ctx->attr_obj.protocol = PTHREAD_PRIO_PROTECT;
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
       * While the priority ceiling of the mutex is a valid priority which is
       * not the maximum priority.
       */
      ctx->attr_obj.prio_ceiling = CeilingValue();
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Ceiling_Maximum: {
      /*
       * While the priority ceiling of the mutex is the maximum priority.
       */
      ctx->attr_obj.prio_ceiling = INT_MAX;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Ceiling_NA:
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
       * While the `prioceiling` parameter is a valid priority which differs
       * from the priority ceiling of the mutex.
       */
      ctx->value = NewValue();
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_New_High: {
      /*
       * While the `prioceiling` parameter is greater than the maximum
       * priority.
       */
      ctx->value = 0x7fffffff;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_New_Low: {
      /*
       * While the `prioceiling` parameter is less than the minimum priority.
       */
      ctx->value = GetPOSIXPriorityMinimum() - 1;
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
      ctx->out = &ctx->out_obj;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Old_Null: {
      /*
       * While the `old_ceiling` parameter is equal to NULL.
       */
      ctx->out = NULL;
      break;
    }

    case CPthreadReqMutexSetprioceiling_Pre_Old_NA:
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
    case CPthreadReqMutexSetprioceiling_Post_OldCeiling_Zero: {
      /*
       * The value of the object referenced by the `old_ceiling` parameter
       * shall be zero.
       */
      T_eq_int( ctx->out_obj, 0 );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid: {
      /*
       * The value of the object referenced by the `old_ceiling` parameter
       * shall be the priority ceiling of the mutex.
       */
      T_eq_int( ctx->out_obj, CeilingValue() );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum: {
      /*
       * The value of the object referenced by the `old_ceiling` parameter
       * shall be the maximum priority.
       */
      T_eq_int( ctx->out_obj, GetPOSIXPriorityMaximum() );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_OldCeiling_Nop: {
      /*
       * The value of the object referenced by the `old_ceiling` parameter
       * shall not be modified.
       */
      T_eq_int( ctx->out_obj, -1 );
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
    case CPthreadReqMutexSetprioceiling_Post_Ceiling_Valid: {
      /*
       * The priority ceiling of the mutex referenced by the `mutex` parameter
       * shall be the value of the `prioceiling` parameter.
       */
      T_eq_u64(
        _POSIX_Mutex_Get_priority( MutexOf( ctx ) ),
        CoreOf( ctx, NewValue() )
      );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop: {
      /*
       * The priority ceiling of the mutex referenced by the `mutex` parameter
       * shall not be modified.
       */
      T_eq_u64(
        _POSIX_Mutex_Get_priority( MutexOf( ctx ) ),
        ctx->priority_before
      );
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
       * The mutex referenced by the `mutex` parameter shall have no owner.
       */
      T_null( _POSIX_Mutex_Get_owner( MutexOf( ctx ) ) );
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Owner_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Post_Lock_Check(
  CPthreadReqMutexSetprioceiling_Context  *ctx,
  CPthreadReqMutexSetprioceiling_Post_Lock state
)
{
  switch ( state ) {
    case CPthreadReqMutexSetprioceiling_Post_Lock_Released: {
      /*
       * The SMP lock of the mutex referenced by the `mutex` parameter shall be
       * released.
       */
      #if defined( RTEMS_SMP )
      T_true( TicketLockIsAvailable( MutexTicketLock( ctx ) ) );
      #else
      (void) ctx;
      #endif
      break;
    }

    case CPthreadReqMutexSetprioceiling_Post_Lock_NA:
      break;
  }
}

static void CPthreadReqMutexSetprioceiling_Setup( void )
{
  SetSelfPriority( PRIO_NEARLY_IDLE );
}

static void CPthreadReqMutexSetprioceiling_Setup_Wrap( void *arg )
{
  CPthreadReqMutexSetprioceiling_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexSetprioceiling_Setup();
}

static void CPthreadReqMutexSetprioceiling_Teardown( void )
{
  RestoreRunnerPriority();
}

static void CPthreadReqMutexSetprioceiling_Teardown_Wrap( void *arg )
{
  CPthreadReqMutexSetprioceiling_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexSetprioceiling_Teardown();
}

static void CPthreadReqMutexSetprioceiling_Prepare(
  CPthreadReqMutexSetprioceiling_Context *ctx
)
{
  ctx->priority_before = 0;
  ctx->init = false;
  ctx->out_obj = -1;
  ctx->out = &ctx->out_obj;
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
  if ( ctx->init ) {
    (void) pthread_mutex_destroy( &ctx->mutex_obj );
  }
}

/* clang-format off */

static const CPthreadReqMutexSetprioceiling_Entry
CPthreadReqMutexSetprioceiling_Entries[] = {
  { 1, 0, 0, 0, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_NA,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Owner_NA,
    CPthreadReqMutexSetprioceiling_Post_Lock_NA },
  { 0, 0, 0, 0, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_NA },
  { 0, 0, 1, 1, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Owner_NA,
    CPthreadReqMutexSetprioceiling_Post_Lock_NA },
#if defined(RTEMS_SMP)
  { 0, 0, 0, 1, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Zero,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_Released },
#else
  { 0, 0, 0, 1, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Zero,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_NA },
#endif
  { 0, 0, 1, 1, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_NA,
    CPthreadReqMutexSetprioceiling_Post_Owner_NA,
    CPthreadReqMutexSetprioceiling_Post_Lock_NA },
#if defined(RTEMS_SMP)
  { 0, 0, 1, 1, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Zero,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_Released },
#else
  { 0, 0, 1, 1, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Zero,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_NA },
#endif
  { 0, 0, 0, 0, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_NA },
  { 0, 0, 0, 0, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Inval,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Nop,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_NA },
#if defined(RTEMS_SMP)
  { 0, 0, 0, 0, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_Released },
#else
  { 0, 0, 0, 0, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_NA },
#endif
#if defined(RTEMS_SMP)
  { 0, 0, 0, 0, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_Released }
#else
  { 0, 0, 0, 0, 0, 0, CPthreadReqMutexSetprioceiling_Post_Status_Ok,
    CPthreadReqMutexSetprioceiling_Post_OldCeiling_Maximum,
    CPthreadReqMutexSetprioceiling_Post_Ceiling_Valid,
    CPthreadReqMutexSetprioceiling_Post_Owner_None,
    CPthreadReqMutexSetprioceiling_Post_Lock_NA }
#endif
};

static const uint8_t
CPthreadReqMutexSetprioceiling_Map[] = {
  3, 1, 3, 1, 3, 1, 0, 1, 0, 1, 0, 1, 3, 1, 3, 1, 3, 1, 0, 1, 0, 1, 0, 1, 8, 1,
  6, 1, 6, 1, 9, 1, 7, 1, 7, 1, 5, 2, 5, 2, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 4, 2, 4, 2,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 4, 2, 4, 2, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  ctx->Map.pcs[ 4 ] = ctx->Map.pci[ 4 ];
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
  CPthreadReqMutexSetprioceiling_Pre_New_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqMutexSetprioceiling_Pre_Old_Prepare( ctx, ctx->Map.pcs[ 4 ] );
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
  CPthreadReqMutexSetprioceiling_Post_Lock_Check(
    ctx,
    ctx->Map.entry.Post_Lock
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
          ctx->Map.pci[ 3 ] = CPthreadReqMutexSetprioceiling_Pre_New_Valid;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexSetprioceiling_Pre_New_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          for (
            ctx->Map.pci[ 4 ] = CPthreadReqMutexSetprioceiling_Pre_Old_Obj;
            ctx->Map.pci[ 4 ] < CPthreadReqMutexSetprioceiling_Pre_Old_NA;
            ++ctx->Map.pci[ 4 ]
          ) {
            ctx->Map.entry = CPthreadReqMutexSetprioceiling_PopEntry( ctx );

            if ( ctx->Map.entry.Skip ) {
              continue;
            }

            CPthreadReqMutexSetprioceiling_SetPreConditionStates( ctx );
            CPthreadReqMutexSetprioceiling_Prepare( ctx );
            CPthreadReqMutexSetprioceiling_TestVariant( ctx );
            CPthreadReqMutexSetprioceiling_Cleanup( ctx );
          }
        }
      }
    }
  }
}

/** @} */
