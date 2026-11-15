/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqMutexDestroy
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
#include <string.h>
#include <rtems/posix/muteximpl.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqMutexDestroy spec:/c/pthread/req/mutex-destroy
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqMutexDestroy_Pre_Mutex_Obj,
  CPthreadReqMutexDestroy_Pre_Mutex_Null,
  CPthreadReqMutexDestroy_Pre_Mutex_NA
} CPthreadReqMutexDestroy_Pre_Mutex;

typedef enum {
  CPthreadReqMutexDestroy_Pre_Kind_Init,
  CPthreadReqMutexDestroy_Pre_Kind_Static,
  CPthreadReqMutexDestroy_Pre_Kind_Destroyed,
  CPthreadReqMutexDestroy_Pre_Kind_Corrupted,
  CPthreadReqMutexDestroy_Pre_Kind_NA
} CPthreadReqMutexDestroy_Pre_Kind;

typedef enum {
  CPthreadReqMutexDestroy_Pre_Locked_Yes,
  CPthreadReqMutexDestroy_Pre_Locked_No,
  CPthreadReqMutexDestroy_Pre_Locked_NA
} CPthreadReqMutexDestroy_Pre_Locked;

typedef enum {
  CPthreadReqMutexDestroy_Post_Status_Ok,
  CPthreadReqMutexDestroy_Post_Status_Busy,
  CPthreadReqMutexDestroy_Post_Status_Inval,
  CPthreadReqMutexDestroy_Post_Status_NA
} CPthreadReqMutexDestroy_Post_Status;

typedef enum {
  CPthreadReqMutexDestroy_Post_Object_Destroyed,
  CPthreadReqMutexDestroy_Post_Object_Nop,
  CPthreadReqMutexDestroy_Post_Object_NA
} CPthreadReqMutexDestroy_Post_Object;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Mutex_NA : 1;
  uint8_t Pre_Kind_NA : 1;
  uint8_t Pre_Locked_NA : 1;
  uint8_t Post_Status : 2;
  uint8_t Post_Object : 2;
} CPthreadReqMutexDestroy_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/mutex-destroy test case.
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
   * @brief This member is true, if the test locked the mutex.
   */
  bool locked;

  /**
   * @brief This member contains the return value of the
   *   pthread_mutex_destroy() call.
   */
  int status;

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 3 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 3 ];

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
    CPthreadReqMutexDestroy_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqMutexDestroy_Context;

static CPthreadReqMutexDestroy_Context CPthreadReqMutexDestroy_Instance;

static const char *const CPthreadReqMutexDestroy_PreDesc_Mutex[] =
  { "Obj", "Null", "NA" };

static const char *const CPthreadReqMutexDestroy_PreDesc_Kind[] =
  { "Init", "Static", "Destroyed", "Corrupted", "NA" };

static const char *const CPthreadReqMutexDestroy_PreDesc_Locked[] =
  { "Yes", "No", "NA" };

static const char *const *const CPthreadReqMutexDestroy_PreDesc[] = {
  CPthreadReqMutexDestroy_PreDesc_Mutex,
  CPthreadReqMutexDestroy_PreDesc_Kind,
  CPthreadReqMutexDestroy_PreDesc_Locked,
  NULL
};

typedef CPthreadReqMutexDestroy_Context Context;

static POSIX_Mutex_Control *MutexOf( Context *ctx )
{
  return _POSIX_Mutex_Get( &ctx->mutex_obj );
}

static void Action( Context *ctx )
{
  memcpy( &ctx->mutex_before, &ctx->mutex_obj, sizeof( ctx->mutex_before ) );
  ctx->status = pthread_mutex_destroy( ctx->mutex );
}

static void CPthreadReqMutexDestroy_Pre_Mutex_Prepare(
  CPthreadReqMutexDestroy_Context  *ctx,
  CPthreadReqMutexDestroy_Pre_Mutex state
)
{
  switch ( state ) {
    case CPthreadReqMutexDestroy_Pre_Mutex_Obj: {
      /*
       * While the `mutex` parameter references an object of type
       * pthread_mutex_t.
       */
      ctx->mutex = &ctx->mutex_obj;
      break;
    }

    case CPthreadReqMutexDestroy_Pre_Mutex_Null: {
      /*
       * While the `mutex` parameter is equal to NULL.
       */
      ctx->mutex = NULL;
      break;
    }

    case CPthreadReqMutexDestroy_Pre_Mutex_NA:
      break;
  }
}

static void CPthreadReqMutexDestroy_Pre_Kind_Prepare(
  CPthreadReqMutexDestroy_Context *ctx,
  CPthreadReqMutexDestroy_Pre_Kind state
)
{
  switch ( state ) {
    case CPthreadReqMutexDestroy_Pre_Kind_Init: {
      /*
       * While the object referenced by the `mutex` parameter is an initialized
       * mutex.
       */
      T_eq_int( pthread_mutex_init( &ctx->mutex_obj, NULL ), 0 );
      break;
    }

    case CPthreadReqMutexDestroy_Pre_Kind_Static: {
      /*
       * While the object referenced by the `mutex` parameter is a statically
       * initialized mutex.
       */
      ctx->mutex_obj = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
      break;
    }

    case CPthreadReqMutexDestroy_Pre_Kind_Destroyed: {
      /*
       * While the object referenced by the `mutex` parameter is a destroyed
       * mutex.
       */
      T_eq_int( pthread_mutex_init( &ctx->mutex_obj, NULL ), 0 );
      T_eq_int( pthread_mutex_destroy( &ctx->mutex_obj ), 0 );
      break;
    }

    case CPthreadReqMutexDestroy_Pre_Kind_Corrupted: {
      /*
       * While the object referenced by the `mutex` parameter is not a mutex.
       */
      memset( &ctx->mutex_obj, 0xff, sizeof( ctx->mutex_obj ) );
      break;
    }

    case CPthreadReqMutexDestroy_Pre_Kind_NA:
      break;
  }
}

static void CPthreadReqMutexDestroy_Pre_Locked_Prepare(
  CPthreadReqMutexDestroy_Context   *ctx,
  CPthreadReqMutexDestroy_Pre_Locked state
)
{
  switch ( state ) {
    case CPthreadReqMutexDestroy_Pre_Locked_Yes: {
      /*
       * While the mutex is locked by the calling thread.
       */
      T_eq_int( pthread_mutex_lock( &ctx->mutex_obj ), 0 );
      ctx->locked = true;
      break;
    }

    case CPthreadReqMutexDestroy_Pre_Locked_No: {
      /*
       * While the mutex is not locked.
       */
      ctx->locked = false;
      break;
    }

    case CPthreadReqMutexDestroy_Pre_Locked_NA:
      break;
  }
}

static void CPthreadReqMutexDestroy_Post_Status_Check(
  CPthreadReqMutexDestroy_Context    *ctx,
  CPthreadReqMutexDestroy_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqMutexDestroy_Post_Status_Ok: {
      /*
       * The return value of pthread_mutex_destroy() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqMutexDestroy_Post_Status_Busy: {
      /*
       * The return value of pthread_mutex_destroy() shall be EBUSY.
       */
      T_eq_int( ctx->status, EBUSY );
      break;
    }

    case CPthreadReqMutexDestroy_Post_Status_Inval: {
      /*
       * The return value of pthread_mutex_destroy() shall be EINVAL.
       */
      T_eq_int( ctx->status, EINVAL );
      break;
    }

    case CPthreadReqMutexDestroy_Post_Status_NA:
      break;
  }
}

static void CPthreadReqMutexDestroy_Post_Object_Check(
  CPthreadReqMutexDestroy_Context    *ctx,
  CPthreadReqMutexDestroy_Post_Object state
)
{
  switch ( state ) {
    case CPthreadReqMutexDestroy_Post_Object_Destroyed: {
      /*
       * The object referenced by the `mutex` parameter shall no longer be an
       * initialized mutex.
       */
      T_ne_ulong(
        MutexOf( ctx )->flags & ~POSIX_MUTEX_FLAGS_MASK,
        ( (uintptr_t) &ctx->mutex_obj ^ POSIX_MUTEX_MAGIC ) &
          ~POSIX_MUTEX_FLAGS_MASK
      );
      break;
    }

    case CPthreadReqMutexDestroy_Post_Object_Nop: {
      /*
       * The mutex referenced by the `mutex` parameter shall not be modified.
       */
      pthread_mutex_t expected;

      memcpy( &expected, &ctx->mutex_before, sizeof( expected ) );

      /* The call acquires and releases the lock of the thread queue. */
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

    case CPthreadReqMutexDestroy_Post_Object_NA:
      break;
  }
}

static void CPthreadReqMutexDestroy_Prepare(
  CPthreadReqMutexDestroy_Context *ctx
)
{
  ctx->locked = false;
}

static void CPthreadReqMutexDestroy_Action(
  CPthreadReqMutexDestroy_Context *ctx
)
{
  Action( ctx );
}

static void CPthreadReqMutexDestroy_Cleanup(
  CPthreadReqMutexDestroy_Context *ctx
)
{
  if ( ctx->locked ) {
    (void) pthread_mutex_unlock( &ctx->mutex_obj );
    (void) pthread_mutex_destroy( &ctx->mutex_obj );
  }
}

/* clang-format off */

static const CPthreadReqMutexDestroy_Entry
CPthreadReqMutexDestroy_Entries[] = {
  { 1, 0, 0, 0, CPthreadReqMutexDestroy_Post_Status_NA,
    CPthreadReqMutexDestroy_Post_Object_NA },
  { 0, 0, 0, 1, CPthreadReqMutexDestroy_Post_Status_Inval,
    CPthreadReqMutexDestroy_Post_Object_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexDestroy_Post_Status_Busy,
    CPthreadReqMutexDestroy_Post_Object_Nop },
  { 0, 0, 0, 0, CPthreadReqMutexDestroy_Post_Status_Ok,
    CPthreadReqMutexDestroy_Post_Object_Destroyed },
  { 0, 0, 0, 1, CPthreadReqMutexDestroy_Post_Status_Ok,
    CPthreadReqMutexDestroy_Post_Object_Destroyed },
  { 0, 0, 1, 1, CPthreadReqMutexDestroy_Post_Status_Inval,
    CPthreadReqMutexDestroy_Post_Object_NA }
};

static const uint8_t
CPthreadReqMutexDestroy_Map[] = {
  2, 3, 4, 0, 1, 0, 1, 0, 5, 0, 0, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t CPthreadReqMutexDestroy_Scope( void *arg, char *buf, size_t n )
{
  CPthreadReqMutexDestroy_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqMutexDestroy_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqMutexDestroy_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqMutexDestroy_Scope,
  .initial_context = &CPthreadReqMutexDestroy_Instance
};

static inline CPthreadReqMutexDestroy_Entry CPthreadReqMutexDestroy_PopEntry(
  CPthreadReqMutexDestroy_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqMutexDestroy_Entries
    [ CPthreadReqMutexDestroy_Map[ index ] ];
}

static void CPthreadReqMutexDestroy_SetPreConditionStates(
  CPthreadReqMutexDestroy_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Kind_NA ) {
    ctx->Map.pcs[ 1 ] = CPthreadReqMutexDestroy_Pre_Kind_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_Locked_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqMutexDestroy_Pre_Locked_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }
}

static void CPthreadReqMutexDestroy_TestVariant(
  CPthreadReqMutexDestroy_Context *ctx
)
{
  CPthreadReqMutexDestroy_Pre_Mutex_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqMutexDestroy_Pre_Kind_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CPthreadReqMutexDestroy_Pre_Locked_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqMutexDestroy_Action( ctx );
  CPthreadReqMutexDestroy_Post_Status_Check( ctx, ctx->Map.entry.Post_Status );
  CPthreadReqMutexDestroy_Post_Object_Check( ctx, ctx->Map.entry.Post_Object );
}

/**
 * @fn void T_case_body_CPthreadReqMutexDestroy( void )
 */
T_TEST_CASE_FIXTURE(
  CPthreadReqMutexDestroy,
  &CPthreadReqMutexDestroy_Fixture
)
{
  CPthreadReqMutexDestroy_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqMutexDestroy_Pre_Mutex_Obj;
    ctx->Map.pci[ 0 ] < CPthreadReqMutexDestroy_Pre_Mutex_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqMutexDestroy_Pre_Kind_Init;
      ctx->Map.pci[ 1 ] < CPthreadReqMutexDestroy_Pre_Kind_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqMutexDestroy_Pre_Locked_Yes;
        ctx->Map.pci[ 2 ] < CPthreadReqMutexDestroy_Pre_Locked_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        ctx->Map.entry = CPthreadReqMutexDestroy_PopEntry( ctx );

        if ( ctx->Map.entry.Skip ) {
          continue;
        }

        CPthreadReqMutexDestroy_SetPreConditionStates( ctx );
        CPthreadReqMutexDestroy_Prepare( ctx );
        CPthreadReqMutexDestroy_TestVariant( ctx );
        CPthreadReqMutexDestroy_Cleanup( ctx );
      }
    }
  }
}

/** @} */
