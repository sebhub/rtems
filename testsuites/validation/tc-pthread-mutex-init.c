/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexInit
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
#include <rtems.h>
#include <string.h>
#include <rtems/posix/muteximpl.h>
#include <rtems/score/schedulerimpl.h>
#include <rtems/score/threadimpl.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqMutexInit spec:/c/pthread/req/mutex-init
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexInit_Pre_Pshared_Private,
  CPthreadReqMutexInit_Pre_Pshared_Shared,
  CPthreadReqMutexInit_Pre_Pshared_Invalid,
  CPthreadReqMutexInit_Pre_Pshared_NA
} CPthreadReqMutexInit_Pre_Pshared;

typedef enum {
  CPthreadReqMutexInit_Pre_Protocol_None,
  CPthreadReqMutexInit_Pre_Protocol_Inherit,
  CPthreadReqMutexInit_Pre_Protocol_Protect,
  CPthreadReqMutexInit_Pre_Protocol_Invalid,
  CPthreadReqMutexInit_Pre_Protocol_NA
} CPthreadReqMutexInit_Pre_Protocol;

typedef enum {
  CPthreadReqMutexInit_Pre_Type_Normal,
  CPthreadReqMutexInit_Pre_Type_Recursive,
  CPthreadReqMutexInit_Pre_Type_Errorcheck,
  CPthreadReqMutexInit_Pre_Type_Default,
  CPthreadReqMutexInit_Pre_Type_Invalid,
  CPthreadReqMutexInit_Pre_Type_NA
} CPthreadReqMutexInit_Pre_Type;

typedef enum {
  CPthreadReqMutexInit_Pre_Ceiling_Default,
  CPthreadReqMutexInit_Pre_Ceiling_Valid,
  CPthreadReqMutexInit_Pre_Ceiling_TooLow,
  CPthreadReqMutexInit_Pre_Ceiling_TooHigh,
  CPthreadReqMutexInit_Pre_Ceiling_NA
} CPthreadReqMutexInit_Pre_Ceiling;

typedef enum {
  CPthreadReqMutexInit_Pre_Attr_Default,
  CPthreadReqMutexInit_Pre_Attr_Init,
  CPthreadReqMutexInit_Pre_Attr_Uninit,
  CPthreadReqMutexInit_Pre_Attr_NA
} CPthreadReqMutexInit_Pre_Attr;

typedef enum {
  CPthreadReqMutexInit_Pre_Mutex_Valid,
  CPthreadReqMutexInit_Pre_Mutex_Null,
  CPthreadReqMutexInit_Pre_Mutex_NA
} CPthreadReqMutexInit_Pre_Mutex;

typedef enum {
  CPthreadReqMutexInit_Post_Status_Ok,
  CPthreadReqMutexInit_Post_Status_Inval,
  CPthreadReqMutexInit_Post_Status_NA
} CPthreadReqMutexInit_Post_Status;

typedef enum {
  CPthreadReqMutexInit_Post_MutexObj_Initialized,
  CPthreadReqMutexInit_Post_MutexObj_Nop,
  CPthreadReqMutexInit_Post_MutexObj_NA
} CPthreadReqMutexInit_Post_MutexObj;

typedef enum {
  CPthreadReqMutexInit_Post_Protocol_None,
  CPthreadReqMutexInit_Post_Protocol_Inherit,
  CPthreadReqMutexInit_Post_Protocol_Protect,
  CPthreadReqMutexInit_Post_Protocol_NA
} CPthreadReqMutexInit_Post_Protocol;

typedef enum {
  CPthreadReqMutexInit_Post_Recursive_Yes,
  CPthreadReqMutexInit_Post_Recursive_No,
  CPthreadReqMutexInit_Post_Recursive_NA
} CPthreadReqMutexInit_Post_Recursive;

typedef enum {
  CPthreadReqMutexInit_Post_Owner_None,
  CPthreadReqMutexInit_Post_Owner_NA
} CPthreadReqMutexInit_Post_Owner;

typedef enum {
  CPthreadReqMutexInit_Post_Ceiling_Maximum,
  CPthreadReqMutexInit_Post_Ceiling_Valid,
  CPthreadReqMutexInit_Post_Ceiling_NA
} CPthreadReqMutexInit_Post_Ceiling;

typedef enum {
  CPthreadReqMutexInit_Post_Scheduler_Home,
  CPthreadReqMutexInit_Post_Scheduler_NA
} CPthreadReqMutexInit_Post_Scheduler;

typedef enum {
  CPthreadReqMutexInit_Post_AttrObj_Nop,
  CPthreadReqMutexInit_Post_AttrObj_NA
} CPthreadReqMutexInit_Post_AttrObj;

typedef struct {
  uint32_t Skip : 1;
  uint32_t Pre_Pshared_NA : 1;
  uint32_t Pre_Protocol_NA : 1;
  uint32_t Pre_Type_NA : 1;
  uint32_t Pre_Ceiling_NA : 1;
  uint32_t Pre_Attr_NA : 1;
  uint32_t Pre_Mutex_NA : 1;
  uint32_t Post_Status : 2;
  uint32_t Post_MutexObj : 2;
  uint32_t Post_Protocol : 2;
  uint32_t Post_Recursive : 2;
  uint32_t Post_Owner : 1;
  uint32_t Post_Ceiling : 2;
  uint32_t Post_Scheduler : 1;
  uint32_t Post_AttrObj : 1;
} CPthreadReqMutexInit_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutex-init test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `mutex` parameter value.
   */
  pthread_mutex_t *mutex;

  /**
   * @brief This member specifies the `attr` parameter value.
   */
  const pthread_mutexattr_t *attr;

  /**
   * @brief This member contains the return value of the pthread_mutex_init()
   *   call.
   */
  int status;

  /**
   * @brief This member contains the mutex object.
   */
  pthread_mutex_t mutex_obj;

  /**
   * @brief This member contains the initial content of the mutex object.
   */
  pthread_mutex_t mutex_prefill;

  /**
   * @brief This member contains the mutex attributes object.
   */
  pthread_mutexattr_t attr_obj;

  /**
   * @brief This member contains a copy of the mutex attributes object before
   *   the call.
   */
  pthread_mutexattr_t attr_before;

  /**
   * @brief This member contains the home scheduler of the calling task.
   */
  const Scheduler_Control *scheduler;

  /**
   * @brief This member is true, if the mutex attributes object shall be
   *   destroyed by the cleanup step.
   */
  bool attr_valid;

  /**
   * @brief This member specifies the process-shared attribute of the mutex
   *   attributes object.
   */
  int pshared;

  /**
   * @brief This member specifies the mutex protocol attribute of the mutex
   *   attributes object.
   */
  int protocol;

  /**
   * @brief This member specifies the mutex type attribute of the mutex
   *   attributes object.
   */
  int type;

  /**
   * @brief This member specifies the mutex priority ceiling attribute of the
   *   mutex attributes object.
   */
  int prio_ceiling;

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
    CPthreadReqMutexInit_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexInit_Context;

static CPthreadReqMutexInit_Context CPthreadReqMutexInit_Instance;

static const char *const CPthreadReqMutexInit_PreDesc_Pshared[] =
  { "Private", "Shared", "Invalid", "NA" };

static const char *const CPthreadReqMutexInit_PreDesc_Protocol[] =
  { "None", "Inherit", "Protect", "Invalid", "NA" };

static const char *const CPthreadReqMutexInit_PreDesc_Type[] =
  { "Normal", "Recursive", "Errorcheck", "Default", "Invalid", "NA" };

static const char *const CPthreadReqMutexInit_PreDesc_Ceiling[] =
  { "Default", "Valid", "TooLow", "TooHigh", "NA" };

static const char *const CPthreadReqMutexInit_PreDesc_Attr[] =
  { "Default", "Init", "Uninit", "NA" };

static const char *const CPthreadReqMutexInit_PreDesc_Mutex[] =
  { "Valid", "Null", "NA" };

static const char *const *const CPthreadReqMutexInit_PreDesc[] = {
  CPthreadReqMutexInit_PreDesc_Pshared,
  CPthreadReqMutexInit_PreDesc_Protocol,
  CPthreadReqMutexInit_PreDesc_Type,
  CPthreadReqMutexInit_PreDesc_Ceiling,
  CPthreadReqMutexInit_PreDesc_Attr,
  CPthreadReqMutexInit_PreDesc_Mutex,
  NULL
};

static void CPthreadReqMutexInit_Pre_Pshared_Prepare(
  CPthreadReqMutexInit_Context    *ctx,
  CPthreadReqMutexInit_Pre_Pshared state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Pre_Pshared_Private: {
      /*
       * While the process-shared attribute of the mutex attributes object is
       * PTHREAD_PROCESS_PRIVATE.
       */
      ctx->pshared = PTHREAD_PROCESS_PRIVATE;
      break;
    }

    case CPthreadReqMutexInit_Pre_Pshared_Shared: {
      /*
       * While the process-shared attribute of the mutex attributes object is
       * PTHREAD_PROCESS_SHARED.
       */
      ctx->pshared = PTHREAD_PROCESS_SHARED;
      break;
    }

    case CPthreadReqMutexInit_Pre_Pshared_Invalid: {
      /*
       * While the process-shared attribute of the mutex attributes object is
       * invalid.
       */
      ctx->pshared = INT_MAX;
      break;
    }

    case CPthreadReqMutexInit_Pre_Pshared_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Pre_Protocol_Prepare(
  CPthreadReqMutexInit_Context     *ctx,
  CPthreadReqMutexInit_Pre_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Pre_Protocol_None: {
      /*
       * While the mutex protocol attribute of the mutex attributes object is
       * PTHREAD_PRIO_NONE.
       */
      ctx->protocol = PTHREAD_PRIO_NONE;
      break;
    }

    case CPthreadReqMutexInit_Pre_Protocol_Inherit: {
      /*
       * While the mutex protocol attribute of the mutex attributes object is
       * PTHREAD_PRIO_INHERIT.
       */
      ctx->protocol = PTHREAD_PRIO_INHERIT;
      break;
    }

    case CPthreadReqMutexInit_Pre_Protocol_Protect: {
      /*
       * While the mutex protocol attribute of the mutex attributes object is
       * PTHREAD_PRIO_PROTECT.
       */
      ctx->protocol = PTHREAD_PRIO_PROTECT;
      break;
    }

    case CPthreadReqMutexInit_Pre_Protocol_Invalid: {
      /*
       * While the mutex protocol attribute of the mutex attributes object is
       * invalid.
       */
      ctx->protocol = INT_MAX;
      break;
    }

    case CPthreadReqMutexInit_Pre_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Pre_Type_Prepare(
  CPthreadReqMutexInit_Context *ctx,
  CPthreadReqMutexInit_Pre_Type state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Pre_Type_Normal: {
      /*
       * While the mutex type attribute of the mutex attributes object is
       * PTHREAD_MUTEX_NORMAL.
       */
      ctx->type = PTHREAD_MUTEX_NORMAL;
      break;
    }

    case CPthreadReqMutexInit_Pre_Type_Recursive: {
      /*
       * While the mutex type attribute of the mutex attributes object is
       * PTHREAD_MUTEX_RECURSIVE.
       */
      ctx->type = PTHREAD_MUTEX_RECURSIVE;
      break;
    }

    case CPthreadReqMutexInit_Pre_Type_Errorcheck: {
      /*
       * While the mutex type attribute of the mutex attributes object is
       * PTHREAD_MUTEX_ERRORCHECK.
       */
      ctx->type = PTHREAD_MUTEX_ERRORCHECK;
      break;
    }

    case CPthreadReqMutexInit_Pre_Type_Default: {
      /*
       * While the mutex type attribute of the mutex attributes object is
       * PTHREAD_MUTEX_DEFAULT.
       */
      ctx->type = PTHREAD_MUTEX_DEFAULT;
      break;
    }

    case CPthreadReqMutexInit_Pre_Type_Invalid: {
      /*
       * While the mutex type attribute of the mutex attributes object is
       * invalid.
       */
      ctx->type = INT_MAX;
      break;
    }

    case CPthreadReqMutexInit_Pre_Type_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Pre_Ceiling_Prepare(
  CPthreadReqMutexInit_Context    *ctx,
  CPthreadReqMutexInit_Pre_Ceiling state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Pre_Ceiling_Default: {
      /*
       * While the mutex priority ceiling attribute of the mutex attributes
       * object has the default value.
       */
      ctx->prio_ceiling = INT_MAX;
      break;
    }

    case CPthreadReqMutexInit_Pre_Ceiling_Valid: {
      /*
       * While the mutex priority ceiling attribute of the mutex attributes
       * object is a valid thread priority.
       */
      ctx->prio_ceiling = ToPOSIXPriority( PRIO_HIGH );
      break;
    }

    case CPthreadReqMutexInit_Pre_Ceiling_TooLow: {
      /*
       * While the mutex priority ceiling attribute of the mutex attributes
       * object is lower than the minimum thread priority.
       */
      ctx->prio_ceiling = GetPOSIXPriorityMinimum() - 1;
      break;
    }

    case CPthreadReqMutexInit_Pre_Ceiling_TooHigh: {
      /*
       * While the mutex priority ceiling attribute of the mutex attributes
       * object is higher than the maximum thread priority.
       */
      ctx->prio_ceiling = GetPOSIXPriorityMaximum() + 1;
      break;
    }

    case CPthreadReqMutexInit_Pre_Ceiling_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Pre_Attr_Prepare(
  CPthreadReqMutexInit_Context *ctx,
  CPthreadReqMutexInit_Pre_Attr state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Pre_Attr_Default: {
      /*
       * While the `attr` parameter is NULL so that the default attributes are
       * used.
       */
      ctx->attr = NULL;
      break;
    }

    case CPthreadReqMutexInit_Pre_Attr_Init: {
      /*
       * While the `attr` parameter references an initialized mutex attributes
       * object.
       */
      T_eq_int( pthread_mutexattr_init( &ctx->attr_obj ), 0 );
      ctx->attr_valid = true;

      /*
       * The pthread_mutexattr_setpshared(), pthread_mutexattr_setprotocol() and
       * pthread_mutexattr_settype() functions reject invalid values. The
       * attribute values are written directly into the members of the mutex
       * attributes object, so that the checks of pthread_mutex_init() can be
       * reached. The priority ceiling is written the same way for consistency.
       */
      ctx->attr_obj.process_shared = ctx->pshared;
      ctx->attr_obj.protocol = ctx->protocol;
      ctx->attr_obj.type = ctx->type;
      ctx->attr_obj.prio_ceiling = ctx->prio_ceiling;
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexInit_Pre_Attr_Uninit: {
      /*
       * While the `attr` parameter references an mutex attributes object which
       * is not initialized.
       */
      memset( &ctx->attr_obj, 0x0, sizeof( ctx->attr_obj ) );
      ctx->attr = &ctx->attr_obj;
      break;
    }

    case CPthreadReqMutexInit_Pre_Attr_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Pre_Mutex_Prepare(
  CPthreadReqMutexInit_Context  *ctx,
  CPthreadReqMutexInit_Pre_Mutex state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Pre_Mutex_Valid: {
      /*
       * While the `mutex` parameter references an object of type
       * pthread_mutex_t.
       */
      ctx->mutex = &ctx->mutex_obj;
      break;
    }

    case CPthreadReqMutexInit_Pre_Mutex_Null: {
      /*
       * While the `mutex` parameter is NULL.
       */
      ctx->mutex = NULL;
      break;
    }

    case CPthreadReqMutexInit_Pre_Mutex_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Post_Status_Check(
  CPthreadReqMutexInit_Context    *ctx,
  CPthreadReqMutexInit_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Post_Status_Ok: {
      /*
       * The return value of pthread_mutex_init() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexInit_Post_Status_Inval: {
      /*
       * The return value of pthread_mutex_init() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexInit_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Post_MutexObj_Check(
  CPthreadReqMutexInit_Context      *ctx,
  CPthreadReqMutexInit_Post_MutexObj state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Post_MutexObj_Initialized: {
      /*
       * The object referenced by the `mutex` parameter shall be an initialized
       * mutex.
       */
      T_eq_ulong(
        ctx->mutex_obj._flags & ~POSIX_MUTEX_FLAGS_MASK,
        ( (uintptr_t) &ctx->mutex_obj ^ POSIX_MUTEX_MAGIC ) &
          ~POSIX_MUTEX_FLAGS_MASK
      );
      break;
    }

    case CPthreadReqMutexInit_Post_MutexObj_Nop: {
      /*
       * The object referenced by the `mutex` parameter shall not be modified.
       */
      T_eq_mem(
        &ctx->mutex_obj,
        &ctx->mutex_prefill,
        sizeof( ctx->mutex_obj )
      );
      break;
    }

    case CPthreadReqMutexInit_Post_MutexObj_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Post_Protocol_Check(
  CPthreadReqMutexInit_Context      *ctx,
  CPthreadReqMutexInit_Post_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Post_Protocol_None: {
      /*
       * The mutex protocol attribute of the mutex initialized by the call
       * shall be PTHREAD_PRIO_NONE.
       */
      T_eq_int(
        _POSIX_Mutex_Get_protocol( ctx->mutex_obj._flags ),
        POSIX_MUTEX_NO_PROTOCOL
      );
      break;
    }

    case CPthreadReqMutexInit_Post_Protocol_Inherit: {
      /*
       * The mutex protocol attribute of the mutex initialized by the call
       * shall be PTHREAD_PRIO_INHERIT.
       */
      T_eq_int(
        _POSIX_Mutex_Get_protocol( ctx->mutex_obj._flags ),
        POSIX_MUTEX_PRIORITY_INHERIT
      );
      break;
    }

    case CPthreadReqMutexInit_Post_Protocol_Protect: {
      /*
       * The mutex protocol attribute of the mutex initialized by the call
       * shall be PTHREAD_PRIO_PROTECT.
       */
      T_eq_int(
        _POSIX_Mutex_Get_protocol( ctx->mutex_obj._flags ),
        POSIX_MUTEX_PRIORITY_CEILING
      );
      break;
    }

    case CPthreadReqMutexInit_Post_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Post_Recursive_Check(
  CPthreadReqMutexInit_Context       *ctx,
  CPthreadReqMutexInit_Post_Recursive state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Post_Recursive_Yes: {
      /*
       * The mutex initialized by the call shall permit a recursive lock by its
       * owner.
       */
      T_true( _POSIX_Mutex_Is_recursive( ctx->mutex_obj._flags ) );
      break;
    }

    case CPthreadReqMutexInit_Post_Recursive_No: {
      /*
       * The mutex initialized by the call shall not permit a recursive lock by
       * its owner.
       */
      T_false( _POSIX_Mutex_Is_recursive( ctx->mutex_obj._flags ) );
      break;
    }

    case CPthreadReqMutexInit_Post_Recursive_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Post_Owner_Check(
  CPthreadReqMutexInit_Context   *ctx,
  CPthreadReqMutexInit_Post_Owner state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Post_Owner_None: {
      /*
       * The mutex initialized by the call shall have no owner.
       */
      T_null( _POSIX_Mutex_Get_owner( _POSIX_Mutex_Get( &ctx->mutex_obj ) ) );
      T_eq_uint(
        _POSIX_Mutex_Get( &ctx->mutex_obj )->Recursive.nest_level,
        0
      );
      break;
    }

    case CPthreadReqMutexInit_Post_Owner_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Post_Ceiling_Check(
  CPthreadReqMutexInit_Context     *ctx,
  CPthreadReqMutexInit_Post_Ceiling state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Post_Ceiling_Maximum: {
      /*
       * The mutex priority ceiling attribute of the mutex initialized by the
       * call shall be the maximum thread priority.
       */
      T_eq_u64(
        _POSIX_Mutex_Get_priority( _POSIX_Mutex_Get( &ctx->mutex_obj ) ),
        _Scheduler_Map_priority( ctx->scheduler, PRIO_VERY_ULTRA_HIGH )
      );
      break;
    }

    case CPthreadReqMutexInit_Post_Ceiling_Valid: {
      /*
       * The mutex priority ceiling attribute of the mutex initialized by the
       * call shall be the mutex priority ceiling attribute of the mutex
       * attributes object.
       */
      T_eq_u64(
        _POSIX_Mutex_Get_priority( _POSIX_Mutex_Get( &ctx->mutex_obj ) ),
        _Scheduler_Map_priority( ctx->scheduler, PRIO_HIGH )
      );
      break;
    }

    case CPthreadReqMutexInit_Post_Ceiling_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Post_Scheduler_Check(
  CPthreadReqMutexInit_Context       *ctx,
  CPthreadReqMutexInit_Post_Scheduler state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Post_Scheduler_Home: {
      /*
       * The scheduler of the mutex initialized by the call shall be the home
       * scheduler of the calling task.
       */
      T_eq_ptr(
        _POSIX_Mutex_Get( &ctx->mutex_obj )->scheduler,
        ctx->scheduler
      );
      break;
    }

    case CPthreadReqMutexInit_Post_Scheduler_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Post_AttrObj_Check(
  CPthreadReqMutexInit_Context     *ctx,
  CPthreadReqMutexInit_Post_AttrObj state
)
{
  switch ( state ) {
    case CPthreadReqMutexInit_Post_AttrObj_Nop: {
      /*
       * The object referenced by the `attr` parameter shall not be modified.
       */
      T_eq_mem( &ctx->attr_obj, &ctx->attr_before, sizeof( ctx->attr_obj ) );
      break;
    }

    case CPthreadReqMutexInit_Post_AttrObj_NA:
      break;
  }
}

static void CPthreadReqMutexInit_Setup( CPthreadReqMutexInit_Context *ctx )
{
  /*
   * The priority of the runner task must not be higher than the priority
   * ceiling of the mutexes under test, otherwise the mutexes could not be
   * locked by the runner task.
   */
  SetSelfPriority( PRIO_NORMAL );
  ctx->scheduler = _Thread_Scheduler_get_home( _Thread_Get_executing() );
}

static void CPthreadReqMutexInit_Setup_Wrap( void *arg )
{
  CPthreadReqMutexInit_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexInit_Setup( ctx );
}

static void CPthreadReqMutexInit_Teardown( void )
{
  RestoreRunnerPriority();
}

static void CPthreadReqMutexInit_Teardown_Wrap( void *arg )
{
  CPthreadReqMutexInit_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqMutexInit_Teardown();
}

static void CPthreadReqMutexInit_Prepare( CPthreadReqMutexInit_Context *ctx )
{
  /*
   * The object is filled with a non-zero pattern.  A mutex object which
   * consists of zero bytes only would be automatically initialized by the
   * functions which use a mutex.
   */
  memset( &ctx->mutex_obj, 0xa5, sizeof( ctx->mutex_obj ) );
  memset( &ctx->mutex_prefill, 0xa5, sizeof( ctx->mutex_prefill ) );

  ctx->attr_valid = false;
  ctx->pshared = PTHREAD_PROCESS_PRIVATE;
  ctx->protocol = PTHREAD_PRIO_NONE;
  ctx->type = PTHREAD_MUTEX_DEFAULT;
  ctx->prio_ceiling = INT_MAX;
}

static void CPthreadReqMutexInit_Action( CPthreadReqMutexInit_Context *ctx )
{
  memcpy( &ctx->attr_before, &ctx->attr_obj, sizeof( ctx->attr_before ) );
  ctx->status = pthread_mutex_init( ctx->mutex, ctx->attr );
}

static void CPthreadReqMutexInit_Cleanup( CPthreadReqMutexInit_Context *ctx )
{
  if ( ctx->attr_valid ) {
    T_eq_int( pthread_mutexattr_destroy( &ctx->attr_obj ), 0 );
  }

  if ( ctx->status == 0 ) {
    T_eq_int( pthread_mutex_destroy( &ctx->mutex_obj ), 0 );
  }
}

/* clang-format off */

static const CPthreadReqMutexInit_Entry
CPthreadReqMutexInit_Entries[] = {
  { 1, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_NA,
    CPthreadReqMutexInit_Post_MutexObj_NA,
    CPthreadReqMutexInit_Post_Protocol_NA,
    CPthreadReqMutexInit_Post_Recursive_NA, CPthreadReqMutexInit_Post_Owner_NA,
    CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_NA },
  { 0, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_Inval,
    CPthreadReqMutexInit_Post_MutexObj_NA,
    CPthreadReqMutexInit_Post_Protocol_NA,
    CPthreadReqMutexInit_Post_Recursive_NA, CPthreadReqMutexInit_Post_Owner_NA,
    CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_Inval,
    CPthreadReqMutexInit_Post_MutexObj_Nop,
    CPthreadReqMutexInit_Post_Protocol_NA,
    CPthreadReqMutexInit_Post_Recursive_NA, CPthreadReqMutexInit_Post_Owner_NA,
    CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_Ok,
    CPthreadReqMutexInit_Post_MutexObj_Initialized,
    CPthreadReqMutexInit_Post_Protocol_None,
    CPthreadReqMutexInit_Post_Recursive_No,
    CPthreadReqMutexInit_Post_Owner_None, CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_Ok,
    CPthreadReqMutexInit_Post_MutexObj_Initialized,
    CPthreadReqMutexInit_Post_Protocol_Inherit,
    CPthreadReqMutexInit_Post_Recursive_No,
    CPthreadReqMutexInit_Post_Owner_None, CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 1, 0, 0, CPthreadReqMutexInit_Post_Status_Inval,
    CPthreadReqMutexInit_Post_MutexObj_Nop,
    CPthreadReqMutexInit_Post_Protocol_NA,
    CPthreadReqMutexInit_Post_Recursive_NA, CPthreadReqMutexInit_Post_Owner_NA,
    CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 1, 0, 0, CPthreadReqMutexInit_Post_Status_Inval,
    CPthreadReqMutexInit_Post_MutexObj_NA,
    CPthreadReqMutexInit_Post_Protocol_NA,
    CPthreadReqMutexInit_Post_Recursive_NA, CPthreadReqMutexInit_Post_Owner_NA,
    CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_Ok,
    CPthreadReqMutexInit_Post_MutexObj_Initialized,
    CPthreadReqMutexInit_Post_Protocol_None,
    CPthreadReqMutexInit_Post_Recursive_Yes,
    CPthreadReqMutexInit_Post_Owner_None, CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_Ok,
    CPthreadReqMutexInit_Post_MutexObj_Initialized,
    CPthreadReqMutexInit_Post_Protocol_Inherit,
    CPthreadReqMutexInit_Post_Recursive_Yes,
    CPthreadReqMutexInit_Post_Owner_None, CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_Ok,
    CPthreadReqMutexInit_Post_MutexObj_Initialized,
    CPthreadReqMutexInit_Post_Protocol_Protect,
    CPthreadReqMutexInit_Post_Recursive_No,
    CPthreadReqMutexInit_Post_Owner_None,
    CPthreadReqMutexInit_Post_Ceiling_Maximum,
    CPthreadReqMutexInit_Post_Scheduler_Home,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_Ok,
    CPthreadReqMutexInit_Post_MutexObj_Initialized,
    CPthreadReqMutexInit_Post_Protocol_Protect,
    CPthreadReqMutexInit_Post_Recursive_No,
    CPthreadReqMutexInit_Post_Owner_None,
    CPthreadReqMutexInit_Post_Ceiling_Valid,
    CPthreadReqMutexInit_Post_Scheduler_Home,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_Ok,
    CPthreadReqMutexInit_Post_MutexObj_Initialized,
    CPthreadReqMutexInit_Post_Protocol_Protect,
    CPthreadReqMutexInit_Post_Recursive_Yes,
    CPthreadReqMutexInit_Post_Owner_None,
    CPthreadReqMutexInit_Post_Ceiling_Maximum,
    CPthreadReqMutexInit_Post_Scheduler_Home,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 0, 0, 0, 0, 0, 0, CPthreadReqMutexInit_Post_Status_Ok,
    CPthreadReqMutexInit_Post_MutexObj_Initialized,
    CPthreadReqMutexInit_Post_Protocol_Protect,
    CPthreadReqMutexInit_Post_Recursive_Yes,
    CPthreadReqMutexInit_Post_Owner_None,
    CPthreadReqMutexInit_Post_Ceiling_Valid,
    CPthreadReqMutexInit_Post_Scheduler_Home,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 1, 1, 1, 1, 0, 0, CPthreadReqMutexInit_Post_Status_Ok,
    CPthreadReqMutexInit_Post_MutexObj_Initialized,
    CPthreadReqMutexInit_Post_Protocol_None,
    CPthreadReqMutexInit_Post_Recursive_No,
    CPthreadReqMutexInit_Post_Owner_None, CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_NA },
  { 0, 1, 1, 1, 1, 0, 0, CPthreadReqMutexInit_Post_Status_Inval,
    CPthreadReqMutexInit_Post_MutexObj_NA,
    CPthreadReqMutexInit_Post_Protocol_NA,
    CPthreadReqMutexInit_Post_Recursive_NA, CPthreadReqMutexInit_Post_Owner_NA,
    CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_NA },
  { 0, 1, 1, 1, 1, 0, 0, CPthreadReqMutexInit_Post_Status_Inval,
    CPthreadReqMutexInit_Post_MutexObj_Nop,
    CPthreadReqMutexInit_Post_Protocol_NA,
    CPthreadReqMutexInit_Post_Recursive_NA, CPthreadReqMutexInit_Post_Owner_NA,
    CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_Nop },
  { 0, 1, 1, 1, 1, 0, 0, CPthreadReqMutexInit_Post_Status_Inval,
    CPthreadReqMutexInit_Post_MutexObj_NA,
    CPthreadReqMutexInit_Post_Protocol_NA,
    CPthreadReqMutexInit_Post_Recursive_NA, CPthreadReqMutexInit_Post_Owner_NA,
    CPthreadReqMutexInit_Post_Ceiling_NA,
    CPthreadReqMutexInit_Post_Scheduler_NA,
    CPthreadReqMutexInit_Post_AttrObj_Nop }
};

static const uint8_t
CPthreadReqMutexInit_Map[] = {
  13, 14, 3, 1, 15, 16, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0,
  0, 0, 7, 1, 0, 0, 0, 0, 7, 1, 0, 0, 0, 0, 7, 1, 0, 0, 0, 0, 7, 1, 0, 0, 0, 0,
  3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1,
  0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 2, 1, 0, 0,
  0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0,
  4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 8, 1,
  0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0,
  0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0,
  4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1,
  0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 9, 1, 0, 0, 0, 0, 10, 1, 0, 0, 0, 0, 2, 1, 0,
  0, 0, 0, 2, 1, 0, 0, 0, 0, 11, 1, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 2, 1, 0, 0,
  0, 0, 2, 1, 0, 0, 0, 0, 9, 1, 0, 0, 0, 0, 10, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0,
  0, 2, 1, 0, 0, 0, 0, 9, 1, 0, 0, 0, 0, 10, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0,
  2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1,
  0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0,
  3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 7, 1, 0, 0, 0, 0, 7, 1,
  0, 0, 0, 0, 7, 1, 0, 0, 0, 0, 7, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0,
  0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0,
  3, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1,
  0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0,
  0, 0, 4, 1, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0,
  8, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1,
  0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 4, 1, 0, 0,
  0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0,
  9, 1, 0, 0, 0, 0, 10, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 11,
  1, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 9, 1,
  0, 0, 0, 0, 10, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 9, 1, 0,
  0, 0, 0, 10, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0,
  0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0,
  0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0,
  2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1,
  0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0,
  0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0,
  2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1,
  0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0,
  0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0,
  2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1,
  0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0,
  0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0,
  2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1,
  0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0,
  0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0,
  2, 1, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t CPthreadReqMutexInit_Scope( void *arg, char *buf, size_t n )
{
  CPthreadReqMutexInit_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CPthreadReqMutexInit_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CPthreadReqMutexInit_Fixture = {
  .setup = CPthreadReqMutexInit_Setup_Wrap,
  .stop = NULL,
  .teardown = CPthreadReqMutexInit_Teardown_Wrap,
  .scope = CPthreadReqMutexInit_Scope,
  .initial_context = &CPthreadReqMutexInit_Instance
};

static inline CPthreadReqMutexInit_Entry CPthreadReqMutexInit_PopEntry(
  CPthreadReqMutexInit_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexInit_Entries[ CPthreadReqMutexInit_Map[ index ] ];
}

static void CPthreadReqMutexInit_SetPreConditionStates(
  CPthreadReqMutexInit_Context *ctx
)
{
  if ( ctx->Map.entry.Pre_Pshared_NA ) {
    ctx->Map.pcs[ 0 ] = CPthreadReqMutexInit_Pre_Pshared_NA;
  } else {
    ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  }

  if ( ctx->Map.entry.Pre_Protocol_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexInit_Pre_Protocol_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_Type_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqMutexInit_Pre_Type_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  if ( ctx->Map.entry.Pre_Ceiling_NA ) {
    ctx->Map.pcs[ 3 ] = CPthreadReqMutexInit_Pre_Ceiling_NA;
  } else {
    ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  }

  ctx->Map.pcs[ 4 ] = ctx->Map.pci[ 4 ];
  ctx->Map.pcs[ 5 ] = ctx->Map.pci[ 5 ];
}

static void CPthreadReqMutexInit_TestVariant(
  CPthreadReqMutexInit_Context *ctx
)
{
  CPthreadReqMutexInit_Pre_Pshared_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexInit_Pre_Protocol_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CPthreadReqMutexInit_Pre_Type_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexInit_Pre_Ceiling_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqMutexInit_Pre_Attr_Prepare( ctx, ctx->Map.pcs[ 4 ] );
  CPthreadReqMutexInit_Pre_Mutex_Prepare( ctx, ctx->Map.pcs[ 5 ] );
  CPthreadReqMutexInit_Action( ctx );
  CPthreadReqMutexInit_Post_Status_Check( ctx, ctx->Map.entry.Post_Status );
  CPthreadReqMutexInit_Post_MutexObj_Check(
    ctx,
    ctx->Map.entry.Post_MutexObj
  );
  CPthreadReqMutexInit_Post_Protocol_Check(
    ctx,
    ctx->Map.entry.Post_Protocol
  );
  CPthreadReqMutexInit_Post_Recursive_Check(
    ctx,
    ctx->Map.entry.Post_Recursive
  );
  CPthreadReqMutexInit_Post_Owner_Check( ctx, ctx->Map.entry.Post_Owner );
  CPthreadReqMutexInit_Post_Ceiling_Check( ctx, ctx->Map.entry.Post_Ceiling );
  CPthreadReqMutexInit_Post_Scheduler_Check(
    ctx,
    ctx->Map.entry.Post_Scheduler
  );
  CPthreadReqMutexInit_Post_AttrObj_Check( ctx, ctx->Map.entry.Post_AttrObj );
}

/**
 * @fn void T_case_body_CPthreadReqMutexInit( void )
 */
T_TEST_CASE_FIXTURE( CPthreadReqMutexInit, &CPthreadReqMutexInit_Fixture )
{
  CPthreadReqMutexInit_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexInit_Pre_Pshared_Private;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexInit_Pre_Pshared_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexInit_Pre_Protocol_None;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexInit_Pre_Protocol_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexInit_Pre_Type_Normal;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexInit_Pre_Type_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexInit_Pre_Ceiling_Default;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexInit_Pre_Ceiling_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          for (
            ctx->Map.pci[ 4 ] = CPthreadReqMutexInit_Pre_Attr_Default;
            ctx->Map.pci[ 4 ] < CPthreadReqMutexInit_Pre_Attr_NA;
            ++ctx->Map.pci[ 4 ]
          ) {
            for (
              ctx->Map.pci[ 5 ] = CPthreadReqMutexInit_Pre_Mutex_Valid;
              ctx->Map.pci[ 5 ] < CPthreadReqMutexInit_Pre_Mutex_NA;
              ++ctx->Map.pci[ 5 ]
            ) {
              ctx->Map.entry = CPthreadReqMutexInit_PopEntry( ctx );

              if ( ctx->Map.entry.Skip ) {
                continue;
              }

              CPthreadReqMutexInit_SetPreConditionStates( ctx );
              CPthreadReqMutexInit_Prepare( ctx );
              CPthreadReqMutexInit_TestVariant( ctx );
              CPthreadReqMutexInit_Cleanup( ctx );
            }
          }
        }
      }
    }
  }
}

/** @} */
