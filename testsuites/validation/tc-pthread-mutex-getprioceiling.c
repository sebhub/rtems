/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexGetprioceiling
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
 * @defgroup CPthreadReqMutexGetprioceiling \
 *   spec:/c/pthread/req/mutex-getprioceiling
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexGetprioceiling_Pre_Mutex_Init,
  CPthreadReqMutexGetprioceiling_Pre_Mutex_Static,
  CPthreadReqMutexGetprioceiling_Pre_Mutex_Corrupted,
  CPthreadReqMutexGetprioceiling_Pre_Mutex_Null,
  CPthreadReqMutexGetprioceiling_Pre_Mutex_NA
} CPthreadReqMutexGetprioceiling_Pre_Mutex;

typedef enum {
  CPthreadReqMutexGetprioceiling_Pre_Protocol_None,
  CPthreadReqMutexGetprioceiling_Pre_Protocol_Inherit,
  CPthreadReqMutexGetprioceiling_Pre_Protocol_Protect,
  CPthreadReqMutexGetprioceiling_Pre_Protocol_NA
} CPthreadReqMutexGetprioceiling_Pre_Protocol;

typedef enum {
  CPthreadReqMutexGetprioceiling_Pre_Ceiling_Valid,
  CPthreadReqMutexGetprioceiling_Pre_Ceiling_Maximum,
  CPthreadReqMutexGetprioceiling_Pre_Ceiling_NA
} CPthreadReqMutexGetprioceiling_Pre_Ceiling;

typedef enum {
  CPthreadReqMutexGetprioceiling_Pre_Out_Obj,
  CPthreadReqMutexGetprioceiling_Pre_Out_Null,
  CPthreadReqMutexGetprioceiling_Pre_Out_NA
} CPthreadReqMutexGetprioceiling_Pre_Out;

typedef enum {
  CPthreadReqMutexGetprioceiling_Post_Status_Ok,
  CPthreadReqMutexGetprioceiling_Post_Status_Inval,
  CPthreadReqMutexGetprioceiling_Post_Status_NA
} CPthreadReqMutexGetprioceiling_Post_Status;

typedef enum {
  CPthreadReqMutexGetprioceiling_Post_Result_Zero,
  CPthreadReqMutexGetprioceiling_Post_Result_Valid,
  CPthreadReqMutexGetprioceiling_Post_Result_Maximum,
  CPthreadReqMutexGetprioceiling_Post_Result_Nop,
  CPthreadReqMutexGetprioceiling_Post_Result_NA
} CPthreadReqMutexGetprioceiling_Post_Result;

typedef enum {
  CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop,
  CPthreadReqMutexGetprioceiling_Post_MutexObj_NA
} CPthreadReqMutexGetprioceiling_Post_MutexObj;

typedef enum {
  CPthreadReqMutexGetprioceiling_Post_Lock_Released,
  CPthreadReqMutexGetprioceiling_Post_Lock_NA
} CPthreadReqMutexGetprioceiling_Post_Lock;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Mutex_NA : 1;
  uint16_t Pre_Protocol_NA : 1;
  uint16_t Pre_Ceiling_NA : 1;
  uint16_t Pre_Out_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Result : 3;
  uint16_t Post_MutexObj : 1;
  uint16_t Post_Lock : 1;
} CPthreadReqMutexGetprioceiling_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutex-getprioceiling test case.
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
  const pthread_mutex_t *mutex;

  /**
   * @brief This member specifies the `prioceiling` parameter value.
   */
  int *out;

  /**
   * @brief This member provides the object referenced by the `prioceiling`
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
   * @brief This member contains the return value of the
   *   pthread_mutex_getprioceiling() call.
   */
  int status;

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
    CPthreadReqMutexGetprioceiling_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexGetprioceiling_Context;

static CPthreadReqMutexGetprioceiling_Context
  CPthreadReqMutexGetprioceiling_Instance;

static const char *const CPthreadReqMutexGetprioceiling_PreDesc_Mutex[] =
  { "Init", "Static", "Corrupted", "Null", "NA" };

static const char *const CPthreadReqMutexGetprioceiling_PreDesc_Protocol[] =
  { "None", "Inherit", "Protect", "NA" };

static const char *const CPthreadReqMutexGetprioceiling_PreDesc_Ceiling[] =
  { "Valid", "Maximum", "NA" };

static const char *const CPthreadReqMutexGetprioceiling_PreDesc_Out[] =
  { "Obj", "Null", "NA" };

static const char *const *const CPthreadReqMutexGetprioceiling_PreDesc[] = {
  CPthreadReqMutexGetprioceiling_PreDesc_Mutex,
  CPthreadReqMutexGetprioceiling_PreDesc_Protocol,
  CPthreadReqMutexGetprioceiling_PreDesc_Ceiling,
  CPthreadReqMutexGetprioceiling_PreDesc_Out,
  NULL
};

typedef CPthreadReqMutexGetprioceiling_Context Context;

static int CeilingValue( void )
{
  return GetPOSIXPriorityMaximum() - 10;
}

#if defined( RTEMS_SMP )
static POSIX_Mutex_Control *MutexOf( Context *ctx )
{
  return _POSIX_Mutex_Get( &ctx->mutex_obj );
}

static const SMP_ticket_lock_Control *MutexTicketLock( Context *ctx )
{
  return &MutexOf( ctx )->Recursive.Mutex.Queue.Queue.Lock;
}
#endif

static void Action( Context *ctx )
{
  if ( ctx->init ) {
    T_eq_int( pthread_mutex_init( &ctx->mutex_obj, &ctx->attr_obj ), 0 );
  }

  memcpy( &ctx->mutex_before, &ctx->mutex_obj, sizeof( ctx->mutex_before ) );
  ctx->status = pthread_mutex_getprioceiling( ctx->mutex, ctx->out );
}

static void CPthreadReqMutexGetprioceiling_Pre_Mutex_Prepare(
  CPthreadReqMutexGetprioceiling_Context  *ctx,
  CPthreadReqMutexGetprioceiling_Pre_Mutex state
)
{
  switch ( state ) {
    case CPthreadReqMutexGetprioceiling_Pre_Mutex_Init: {
      /*
       * While the `mutex` parameter references an initialized mutex.
       */
      ctx->mutex = &ctx->mutex_obj;
      ctx->init = true;
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Mutex_Static: {
      /*
       * While the `mutex` parameter references a statically initialized mutex.
       */
      memset( &ctx->mutex_obj, 0, sizeof( ctx->mutex_obj ) );
      ctx->mutex = &ctx->mutex_obj;
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Mutex_Corrupted: {
      /*
       * While the `mutex` parameter references an object which is not a mutex.
       */
      memset( &ctx->mutex_obj, 0xff, sizeof( ctx->mutex_obj ) );
      ctx->mutex = &ctx->mutex_obj;
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Mutex_Null: {
      /*
       * While the `mutex` parameter is equal to NULL.
       */
      ctx->mutex = NULL;
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Mutex_NA:
      break;
  }
}

static void CPthreadReqMutexGetprioceiling_Pre_Protocol_Prepare(
  CPthreadReqMutexGetprioceiling_Context     *ctx,
  CPthreadReqMutexGetprioceiling_Pre_Protocol state
)
{
  switch ( state ) {
    case CPthreadReqMutexGetprioceiling_Pre_Protocol_None: {
      /*
       * While the mutex uses no priority protocol.
       */
      ctx->attr_obj.protocol = PTHREAD_PRIO_NONE;
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Protocol_Inherit: {
      /*
       * While the mutex uses the priority inheritance protocol.
       */
      ctx->attr_obj.protocol = PTHREAD_PRIO_INHERIT;
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Protocol_Protect: {
      /*
       * While the mutex uses the priority ceiling protocol.
       */
      ctx->attr_obj.protocol = PTHREAD_PRIO_PROTECT;
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Protocol_NA:
      break;
  }
}

static void CPthreadReqMutexGetprioceiling_Pre_Ceiling_Prepare(
  CPthreadReqMutexGetprioceiling_Context    *ctx,
  CPthreadReqMutexGetprioceiling_Pre_Ceiling state
)
{
  switch ( state ) {
    case CPthreadReqMutexGetprioceiling_Pre_Ceiling_Valid: {
      /*
       * While the priority ceiling of the mutex is a valid priority which is
       * not the maximum priority.
       */
      ctx->attr_obj.prio_ceiling = CeilingValue();
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Ceiling_Maximum: {
      /*
       * While the priority ceiling of the mutex is the maximum priority.
       */
      ctx->attr_obj.prio_ceiling = INT_MAX;
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Ceiling_NA:
      break;
  }
}

static void CPthreadReqMutexGetprioceiling_Pre_Out_Prepare(
  CPthreadReqMutexGetprioceiling_Context *ctx,
  CPthreadReqMutexGetprioceiling_Pre_Out  state
)
{
  switch ( state ) {
    case CPthreadReqMutexGetprioceiling_Pre_Out_Obj: {
      /*
       * While the `prioceiling` parameter references an object of type int.
       */
      ctx->out = &ctx->out_obj;
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Out_Null: {
      /*
       * While the `prioceiling` parameter is equal to NULL.
       */
      ctx->out = NULL;
      break;
    }

    case CPthreadReqMutexGetprioceiling_Pre_Out_NA:
      break;
  }
}

static void CPthreadReqMutexGetprioceiling_Post_Status_Check(
  CPthreadReqMutexGetprioceiling_Context    *ctx,
  CPthreadReqMutexGetprioceiling_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexGetprioceiling_Post_Status_Ok: {
      /*
       * The return value of pthread_mutex_getprioceiling() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexGetprioceiling_Post_Status_Inval: {
      /*
       * The return value of pthread_mutex_getprioceiling() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexGetprioceiling_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexGetprioceiling_Post_Result_Check(
  CPthreadReqMutexGetprioceiling_Context    *ctx,
  CPthreadReqMutexGetprioceiling_Post_Result state
)
{
  switch ( state ) {
    case CPthreadReqMutexGetprioceiling_Post_Result_Zero: {
      /*
       * The value of the object referenced by the `prioceiling` parameter
       * shall be zero.
       */
      T_eq_int( ctx->out_obj, 0 );
      break;
    }

    case CPthreadReqMutexGetprioceiling_Post_Result_Valid: {
      /*
       * The value of the object referenced by the `prioceiling` parameter
       * shall be the priority ceiling of the mutex.
       */
      T_eq_int( ctx->out_obj, CeilingValue() );
      break;
    }

    case CPthreadReqMutexGetprioceiling_Post_Result_Maximum: {
      /*
       * The value of the object referenced by the `prioceiling` parameter
       * shall be the maximum priority.
       */
      T_eq_int( ctx->out_obj, GetPOSIXPriorityMaximum() );
      break;
    }

    case CPthreadReqMutexGetprioceiling_Post_Result_Nop: {
      /*
       * The value of the object referenced by the `prioceiling` parameter
       * shall not be modified.
       */
      T_eq_int( ctx->out_obj, -1 );
      break;
    }

    case CPthreadReqMutexGetprioceiling_Post_Result_NA:
      break;
  }
}

static void CPthreadReqMutexGetprioceiling_Post_MutexObj_Check(
  CPthreadReqMutexGetprioceiling_Context      *ctx,
  CPthreadReqMutexGetprioceiling_Post_MutexObj state
)
{
  switch ( state ) {
    case CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop: {
      /*
       * The object referenced by the `mutex` parameter, other than its SMP
       * lock, shall not be modified.
       */
      pthread_mutex_t expected;

      memcpy( &expected, &ctx->mutex_before, sizeof( expected ) );
      #if defined( RTEMS_SMP )
      memcpy(
        &_POSIX_Mutex_Get( &expected )->Recursive.Mutex.Queue.Queue.Lock,
        &MutexOf( ctx )->Recursive.Mutex.Queue.Queue.Lock,
        sizeof( MutexOf( ctx )->Recursive.Mutex.Queue.Queue.Lock )
      );
      #endif
      T_eq_mem( &ctx->mutex_obj, &expected, sizeof( expected ) );
      break;
    }

    case CPthreadReqMutexGetprioceiling_Post_MutexObj_NA:
      break;
  }
}

static void CPthreadReqMutexGetprioceiling_Post_Lock_Check(
  CPthreadReqMutexGetprioceiling_Context  *ctx,
  CPthreadReqMutexGetprioceiling_Post_Lock state
)
{
  switch ( state ) {
    case CPthreadReqMutexGetprioceiling_Post_Lock_Released: {
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

    case CPthreadReqMutexGetprioceiling_Post_Lock_NA:
      break;
  }
}

static void CPthreadReqMutexGetprioceiling_Prepare(
  CPthreadReqMutexGetprioceiling_Context *ctx
)
{
  ctx->init = false;
  ctx->out_obj = -1;
  ctx->out = &ctx->out_obj;
  T_eq_int( pthread_mutexattr_init( &ctx->attr_obj ), 0 );
}

static void CPthreadReqMutexGetprioceiling_Action(
  CPthreadReqMutexGetprioceiling_Context *ctx
)
{
  Action( ctx );
}

static void CPthreadReqMutexGetprioceiling_Cleanup(
  CPthreadReqMutexGetprioceiling_Context *ctx
)
{
  if ( ctx->init ) {
    (void) pthread_mutex_destroy( &ctx->mutex_obj );
  }
}

/* clang-format off */

static const CPthreadReqMutexGetprioceiling_Entry
CPthreadReqMutexGetprioceiling_Entries[] = {
  { 1, 0, 0, 0, 0, CPthreadReqMutexGetprioceiling_Post_Status_NA,
    CPthreadReqMutexGetprioceiling_Post_Result_NA,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_NA,
    CPthreadReqMutexGetprioceiling_Post_Lock_NA },
  { 0, 0, 0, 0, 0, CPthreadReqMutexGetprioceiling_Post_Status_Inval,
    CPthreadReqMutexGetprioceiling_Post_Result_NA,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop,
    CPthreadReqMutexGetprioceiling_Post_Lock_NA },
#if defined(RTEMS_SMP)
  { 0, 0, 0, 1, 0, CPthreadReqMutexGetprioceiling_Post_Status_Ok,
    CPthreadReqMutexGetprioceiling_Post_Result_Zero,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop,
    CPthreadReqMutexGetprioceiling_Post_Lock_Released },
#else
  { 0, 0, 0, 1, 0, CPthreadReqMutexGetprioceiling_Post_Status_Ok,
    CPthreadReqMutexGetprioceiling_Post_Result_Zero,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop,
    CPthreadReqMutexGetprioceiling_Post_Lock_NA },
#endif
  { 0, 0, 1, 1, 0, CPthreadReqMutexGetprioceiling_Post_Status_Inval,
    CPthreadReqMutexGetprioceiling_Post_Result_NA,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop,
    CPthreadReqMutexGetprioceiling_Post_Lock_NA },
#if defined(RTEMS_SMP)
  { 0, 0, 0, 0, 0, CPthreadReqMutexGetprioceiling_Post_Status_Ok,
    CPthreadReqMutexGetprioceiling_Post_Result_Valid,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop,
    CPthreadReqMutexGetprioceiling_Post_Lock_Released },
#else
  { 0, 0, 0, 0, 0, CPthreadReqMutexGetprioceiling_Post_Status_Ok,
    CPthreadReqMutexGetprioceiling_Post_Result_Valid,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop,
    CPthreadReqMutexGetprioceiling_Post_Lock_NA },
#endif
#if defined(RTEMS_SMP)
  { 0, 0, 0, 0, 0, CPthreadReqMutexGetprioceiling_Post_Status_Ok,
    CPthreadReqMutexGetprioceiling_Post_Result_Maximum,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop,
    CPthreadReqMutexGetprioceiling_Post_Lock_Released },
#else
  { 0, 0, 0, 0, 0, CPthreadReqMutexGetprioceiling_Post_Status_Ok,
    CPthreadReqMutexGetprioceiling_Post_Result_Maximum,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop,
    CPthreadReqMutexGetprioceiling_Post_Lock_NA },
#endif
#if defined(RTEMS_SMP)
  { 0, 0, 1, 1, 0, CPthreadReqMutexGetprioceiling_Post_Status_Ok,
    CPthreadReqMutexGetprioceiling_Post_Result_Zero,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_NA,
    CPthreadReqMutexGetprioceiling_Post_Lock_Released },
#else
  { 0, 0, 1, 1, 0, CPthreadReqMutexGetprioceiling_Post_Status_Ok,
    CPthreadReqMutexGetprioceiling_Post_Result_Zero,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_NA,
    CPthreadReqMutexGetprioceiling_Post_Lock_NA },
#endif
  { 0, 0, 1, 1, 0, CPthreadReqMutexGetprioceiling_Post_Status_Inval,
    CPthreadReqMutexGetprioceiling_Post_Result_Nop,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_Nop,
    CPthreadReqMutexGetprioceiling_Post_Lock_NA },
  { 0, 0, 1, 1, 0, CPthreadReqMutexGetprioceiling_Post_Status_Inval,
    CPthreadReqMutexGetprioceiling_Post_Result_Nop,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_NA,
    CPthreadReqMutexGetprioceiling_Post_Lock_NA },
  { 0, 0, 1, 1, 0, CPthreadReqMutexGetprioceiling_Post_Status_Inval,
    CPthreadReqMutexGetprioceiling_Post_Result_NA,
    CPthreadReqMutexGetprioceiling_Post_MutexObj_NA,
    CPthreadReqMutexGetprioceiling_Post_Lock_NA }
};

static const uint8_t
CPthreadReqMutexGetprioceiling_Map[] = {
  2, 1, 0, 1, 2, 1, 0, 1, 4, 1, 5, 1, 6, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 3,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t CPthreadReqMutexGetprioceiling_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqMutexGetprioceiling_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexGetprioceiling_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexGetprioceiling_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexGetprioceiling_Scope,
  .initial_context = &CPthreadReqMutexGetprioceiling_Instance
};

static inline CPthreadReqMutexGetprioceiling_Entry
CPthreadReqMutexGetprioceiling_PopEntry(
  CPthreadReqMutexGetprioceiling_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexGetprioceiling_Entries
    [ CPthreadReqMutexGetprioceiling_Map[ index ] ];
}

static void CPthreadReqMutexGetprioceiling_SetPreConditionStates(
  CPthreadReqMutexGetprioceiling_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Protocol_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexGetprioceiling_Pre_Protocol_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_Ceiling_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqMutexGetprioceiling_Pre_Ceiling_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
}

static void CPthreadReqMutexGetprioceiling_TestVariant(
  CPthreadReqMutexGetprioceiling_Context *ctx
)
{
  CPthreadReqMutexGetprioceiling_Pre_Mutex_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexGetprioceiling_Pre_Protocol_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  CPthreadReqMutexGetprioceiling_Pre_Ceiling_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexGetprioceiling_Pre_Out_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqMutexGetprioceiling_Action( ctx );
  CPthreadReqMutexGetprioceiling_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  CPthreadReqMutexGetprioceiling_Post_Result_Check(
    ctx,
    ctx->Map.entry.Post_Result
  );
  CPthreadReqMutexGetprioceiling_Post_MutexObj_Check(
    ctx,
    ctx->Map.entry.Post_MutexObj
  );
  CPthreadReqMutexGetprioceiling_Post_Lock_Check(
    ctx,
    ctx->Map.entry.Post_Lock
  );
}

/**
 * @fn void T_case_body_CPthreadReqMutexGetprioceiling( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexGetprioceiling,
  &CPthreadReqMutexGetprioceiling_Fixture
)
{
  CPthreadReqMutexGetprioceiling_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexGetprioceiling_Pre_Mutex_Init;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexGetprioceiling_Pre_Mutex_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexGetprioceiling_Pre_Protocol_None;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexGetprioceiling_Pre_Protocol_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexGetprioceiling_Pre_Ceiling_Valid;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexGetprioceiling_Pre_Ceiling_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqMutexGetprioceiling_Pre_Out_Obj;
          ctx->Map.pci[ 3 ] < CPthreadReqMutexGetprioceiling_Pre_Out_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = CPthreadReqMutexGetprioceiling_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          CPthreadReqMutexGetprioceiling_SetPreConditionStates( ctx );
          CPthreadReqMutexGetprioceiling_Prepare( ctx );
          CPthreadReqMutexGetprioceiling_TestVariant( ctx );
          CPthreadReqMutexGetprioceiling_Cleanup( ctx );
        }
      }
    }
  }
}

/** @} */
