/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqSetnameNp
 */

/*
 * Copyright (C) 2025, 2026 embedded brains GmbH & Co. KG
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
#include <rtems/score/thread.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqSetnameNp spec:/c/pthread/req/setname-np
 *
 * @ingroup TestsuitesValidationAcfg1
 * @ingroup TestsuitesValidationNoClock0
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqSetnameNp_Pre_NameStorage_None,
  CPthreadReqSetnameNp_Pre_NameStorage_TerminatorOnly,
  CPthreadReqSetnameNp_Pre_NameStorage_String,
  CPthreadReqSetnameNp_Pre_NameStorage_NA
} CPthreadReqSetnameNp_Pre_NameStorage;

typedef enum {
  CPthreadReqSetnameNp_Pre_Thread_NoObj,
  CPthreadReqSetnameNp_Pre_Thread_OtherClass,
  CPthreadReqSetnameNp_Pre_Thread_Valid,
  CPthreadReqSetnameNp_Pre_Thread_NA
} CPthreadReqSetnameNp_Pre_Thread;

typedef enum {
  CPthreadReqSetnameNp_Pre_Length_Fits,
  CPthreadReqSetnameNp_Pre_Length_TooLong,
  CPthreadReqSetnameNp_Pre_Length_NA
} CPthreadReqSetnameNp_Pre_Length;

typedef enum {
  CPthreadReqSetnameNp_Post_Status_Zero,
  CPthreadReqSetnameNp_Post_Status_ESRCH,
  CPthreadReqSetnameNp_Post_Status_ERANGE,
  CPthreadReqSetnameNp_Post_Status_NA
} CPthreadReqSetnameNp_Post_Status;

typedef enum {
  CPthreadReqSetnameNp_Post_Name_Full,
  CPthreadReqSetnameNp_Post_Name_Partial,
  CPthreadReqSetnameNp_Post_Name_Empty,
  CPthreadReqSetnameNp_Post_Name_NA
} CPthreadReqSetnameNp_Post_Name;

typedef enum {
  CPthreadReqSetnameNp_Post_ObjectName_Unchanged,
  CPthreadReqSetnameNp_Post_ObjectName_NA
} CPthreadReqSetnameNp_Post_ObjectName;

typedef enum {
  CPthreadReqSetnameNp_Post_AllocatorLock_Once,
  CPthreadReqSetnameNp_Post_AllocatorLock_NA
} CPthreadReqSetnameNp_Post_AllocatorLock;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_NameStorage_NA : 1;
  uint16_t Pre_Thread_NA : 1;
  uint16_t Pre_Length_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Name : 2;
  uint16_t Post_ObjectName : 1;
  uint16_t Post_AllocatorLock : 1;
} CPthreadReqSetnameNp_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/setname-np test case.
 */
typedef struct {
  /**
   * @brief This member provides a mutex.
   */
  rtems_id mutex_id;

  /**
   * @brief This member contains the object name of the calling thread.
   */
  char object_name[ 5 ];
  ;

  /**
   * @brief This member provides the string for the `name` parameter.
   */
  char name_param[ 64 ];
  ;

  /**
   * @brief This member provides a buffer for the thread name.
   */
  char name_buf[ 64 ];
  ;

  /**
   * @brief This member provides the `thread` parameter value.
   */
  pthread_t thread;

  /**
   * @brief This member provides the `name` parameter value.
   */
  const char *name;

  /**
   * @brief This member contains the returned status.
   */
  int status;

  /**
   * @brief This member contains the count of the obtains of the object
   *   allocator mutex by the call.
   */
  unsigned int lock_count;

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
    CPthreadReqSetnameNp_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqSetnameNp_Context;

static CPthreadReqSetnameNp_Context CPthreadReqSetnameNp_Instance;

static const char *const CPthreadReqSetnameNp_PreDesc_NameStorage[] =
  { "None", "TerminatorOnly", "String", "NA" };

static const char *const CPthreadReqSetnameNp_PreDesc_Thread[] =
  { "NoObj", "OtherClass", "Valid", "NA" };

static const char *const CPthreadReqSetnameNp_PreDesc_Length[] =
  { "Fits", "TooLong", "NA" };

static const char *const *const CPthreadReqSetnameNp_PreDesc[] = {
  CPthreadReqSetnameNp_PreDesc_NameStorage,
  CPthreadReqSetnameNp_PreDesc_Thread,
  CPthreadReqSetnameNp_PreDesc_Length,
  NULL
};

static void CPthreadReqSetnameNp_Pre_NameStorage_Prepare(
  CPthreadReqSetnameNp_Context        *ctx,
  CPthreadReqSetnameNp_Pre_NameStorage state
)
{
  switch ( state ) {
    case CPthreadReqSetnameNp_Pre_NameStorage_None: {
      /*
       * Where the value of the CONFIGURE_MAXIMUM_THREAD_NAME_SIZE application
       * configuration option is zero.
       */
      if ( _Thread_Maximum_name_size != 0 ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case CPthreadReqSetnameNp_Pre_NameStorage_TerminatorOnly: {
      /*
       * Where the value of the CONFIGURE_MAXIMUM_THREAD_NAME_SIZE application
       * configuration option is one.
       */
      if ( _Thread_Maximum_name_size != 1 ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case CPthreadReqSetnameNp_Pre_NameStorage_String: {
      /*
       * Where the value of the CONFIGURE_MAXIMUM_THREAD_NAME_SIZE application
       * configuration option is greater than or equal to two.
       */
      if ( _Thread_Maximum_name_size < 2 ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case CPthreadReqSetnameNp_Pre_NameStorage_NA:
      break;
  }
}

static void CPthreadReqSetnameNp_Pre_Thread_Prepare(
  CPthreadReqSetnameNp_Context   *ctx,
  CPthreadReqSetnameNp_Pre_Thread state
)
{
  switch ( state ) {
    case CPthreadReqSetnameNp_Pre_Thread_NoObj: {
      /*
       * While the `thread` parameter is not associated with an object.
       */
      ctx->thread = 0xffffffff;
      break;
    }

    case CPthreadReqSetnameNp_Pre_Thread_OtherClass: {
      /*
       * While the `thread` parameter is associated with an object which is not
       * a thread.
       */
      ctx->thread = ctx->mutex_id;
      break;
    }

    case CPthreadReqSetnameNp_Pre_Thread_Valid: {
      /*
       * While the `thread` parameter is associated with a thread.
       */
      ctx->thread = pthread_self();
      break;
    }

    case CPthreadReqSetnameNp_Pre_Thread_NA:
      break;
  }
}

static void CPthreadReqSetnameNp_Pre_Length_Prepare(
  CPthreadReqSetnameNp_Context   *ctx,
  CPthreadReqSetnameNp_Pre_Length state
)
{
  switch ( state ) {
    case CPthreadReqSetnameNp_Pre_Length_Fits: {
      /*
       * While the length of the string referenced by the `name` parameter is
       * less than the maximum thread name size.
       */
      T_assert_gt_sz( _Thread_Maximum_name_size, 0 );
      memset( ctx->name_param, 'n', _Thread_Maximum_name_size - 1 );
      ctx->name_param[ _Thread_Maximum_name_size - 1 ] = '\0';
      break;
    }

    case CPthreadReqSetnameNp_Pre_Length_TooLong: {
      /*
       * While the length of the string referenced by the `name` parameter is
       * greater than or equal to the maximum thread name size.
       */
      memset( ctx->name_param, 'n', _Thread_Maximum_name_size );
      ctx->name_param[ _Thread_Maximum_name_size ] = '\0';
      break;
    }

    case CPthreadReqSetnameNp_Pre_Length_NA:
      break;
  }
}

static void CPthreadReqSetnameNp_Post_Status_Check(
  CPthreadReqSetnameNp_Context    *ctx,
  CPthreadReqSetnameNp_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqSetnameNp_Post_Status_Zero: {
      /*
       * The return value of pthread_setname_np() shall be equal to zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqSetnameNp_Post_Status_ESRCH: {
      /*
       * The return value of pthread_setname_np() shall be equal to ESRCH.
       */
      T_eq_int( ctx->status, ESRCH );
      break;
    }

    case CPthreadReqSetnameNp_Post_Status_ERANGE: {
      /*
       * The return value of pthread_setname_np() shall be equal to ERANGE.
       */
      T_eq_int( ctx->status, ERANGE );
      break;
    }

    case CPthreadReqSetnameNp_Post_Status_NA:
      break;
  }
}

static void CPthreadReqSetnameNp_Post_Name_Check(
  CPthreadReqSetnameNp_Context  *ctx,
  CPthreadReqSetnameNp_Post_Name state
)
{
  int eno;

  eno = pthread_getname_np(
    pthread_self(),
    ctx->name_buf,
    sizeof( ctx->name_buf )
  );
  T_eno_success( eno );

  switch ( state ) {
    case CPthreadReqSetnameNp_Post_Name_Full: {
      /*
       * The string name of the thread shall be the string referenced by the
       * `name` parameter.
       */
      T_eq_str( ctx->name_buf, ctx->name );
      break;
    }

    case CPthreadReqSetnameNp_Post_Name_Partial: {
      /*
       * The string name of the thread shall be the first characters of the
       * string referenced by the `name` parameter up to the maximum thread
       * name size minus one.
       */
      T_eq_sz( strlen( ctx->name_buf ), _Thread_Maximum_name_size - 1 );
      T_eq_int(
        memcmp( ctx->name_buf, ctx->name, _Thread_Maximum_name_size - 1 ),
        0
      );
      break;
    }

    case CPthreadReqSetnameNp_Post_Name_Empty: {
      /*
       * The string name of the thread shall be the empty string.
       */
      T_eq_str( ctx->name_buf, ctx->object_name );
      break;
    }

    case CPthreadReqSetnameNp_Post_Name_NA:
      break;
  }

  (void) pthread_setname_np( pthread_self(), "" );
}

static void CPthreadReqSetnameNp_Post_ObjectName_Check(
  CPthreadReqSetnameNp_Context        *ctx,
  CPthreadReqSetnameNp_Post_ObjectName state
)
{
  switch ( state ) {
    case CPthreadReqSetnameNp_Post_ObjectName_Unchanged: {
      /*
       * The object name of the thread shall be unchanged.
       */
      char  buf[ sizeof( ctx->object_name ) ];
      char *name;

      name = rtems_object_get_name( rtems_task_self(), sizeof( buf ), buf );
      T_eq_ptr( name, buf );
      T_eq_str( buf, ctx->object_name );
      break;
    }

    case CPthreadReqSetnameNp_Post_ObjectName_NA:
      break;
  }
}

static void CPthreadReqSetnameNp_Post_AllocatorLock_Check(
  CPthreadReqSetnameNp_Context           *ctx,
  CPthreadReqSetnameNp_Post_AllocatorLock state
)
{
  switch ( state ) {
    case CPthreadReqSetnameNp_Post_AllocatorLock_Once: {
      /*
       * The pthread_setname_np() directive shall obtain the object allocator
       * mutex exactly once.
       */
      T_eq_uint( ctx->lock_count, 1 );
      break;
    }

    case CPthreadReqSetnameNp_Post_AllocatorLock_NA:
      break;
  }
}

static void CPthreadReqSetnameNp_Setup( CPthreadReqSetnameNp_Context *ctx )
{
  char *name;

  T_assert_lt_sz( _Thread_Maximum_name_size, sizeof( ctx->name_param ) );
  (void) pthread_setname_np( pthread_self(), "" );
  name = rtems_object_get_name(
    rtems_task_self(),
    sizeof( ctx->object_name ),
    ctx->object_name
  );
  T_assert_eq_ptr( name, ctx->object_name );
  ctx->mutex_id = CreateMutex();
}

static void CPthreadReqSetnameNp_Setup_Wrap( void *arg )
{
  CPthreadReqSetnameNp_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqSetnameNp_Setup( ctx );
}

static void CPthreadReqSetnameNp_Teardown( CPthreadReqSetnameNp_Context *ctx )
{
  CallCounterObserve( &AllocatorLockCounter, 0 );
  DeleteMutex( ctx->mutex_id );
  (void) pthread_setname_np( pthread_self(), "" );
}

static void CPthreadReqSetnameNp_Teardown_Wrap( void *arg )
{
  CPthreadReqSetnameNp_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqSetnameNp_Teardown( ctx );
}

static void CPthreadReqSetnameNp_Prepare( CPthreadReqSetnameNp_Context *ctx )
{
  (void) pthread_setname_np( pthread_self(), "" );
  ctx->name_param[ 0 ] = '\0';
  ctx->name = ctx->name_param;
}

static void CPthreadReqSetnameNp_Action( CPthreadReqSetnameNp_Context *ctx )
{
  CallCounterObserve( &AllocatorLockCounter, rtems_task_self() );
  ctx->status = pthread_setname_np( ctx->thread, ctx->name );
  ctx->lock_count = CallCounterGet( &AllocatorLockCounter );
  CallCounterObserve( &AllocatorLockCounter, 0 );
}

/* clang-format off */

static const CPthreadReqSetnameNp_Entry
CPthreadReqSetnameNp_Entries[] = {
  { 1, 0, 0, 0, CPthreadReqSetnameNp_Post_Status_NA,
    CPthreadReqSetnameNp_Post_Name_NA, CPthreadReqSetnameNp_Post_ObjectName_NA,
    CPthreadReqSetnameNp_Post_AllocatorLock_NA },
  { 0, 0, 0, 1, CPthreadReqSetnameNp_Post_Status_ESRCH,
    CPthreadReqSetnameNp_Post_Name_NA,
    CPthreadReqSetnameNp_Post_ObjectName_Unchanged,
    CPthreadReqSetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, CPthreadReqSetnameNp_Post_Status_ERANGE,
    CPthreadReqSetnameNp_Post_Name_Empty,
    CPthreadReqSetnameNp_Post_ObjectName_Unchanged,
    CPthreadReqSetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, CPthreadReqSetnameNp_Post_Status_Zero,
    CPthreadReqSetnameNp_Post_Name_Empty,
    CPthreadReqSetnameNp_Post_ObjectName_Unchanged,
    CPthreadReqSetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, CPthreadReqSetnameNp_Post_Status_Zero,
    CPthreadReqSetnameNp_Post_Name_Full,
    CPthreadReqSetnameNp_Post_ObjectName_Unchanged,
    CPthreadReqSetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, CPthreadReqSetnameNp_Post_Status_ERANGE,
    CPthreadReqSetnameNp_Post_Name_Partial,
    CPthreadReqSetnameNp_Post_ObjectName_Unchanged,
    CPthreadReqSetnameNp_Post_AllocatorLock_Once }
};

static const uint8_t
CPthreadReqSetnameNp_Map[] = {
  1, 0, 1, 0, 0, 2, 1, 0, 1, 0, 3, 2, 1, 0, 1, 0, 4, 5
};

/* clang-format on */

static size_t CPthreadReqSetnameNp_Scope( void *arg, char *buf, size_t n )
{
  CPthreadReqSetnameNp_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CPthreadReqSetnameNp_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CPthreadReqSetnameNp_Fixture = {
  .setup = CPthreadReqSetnameNp_Setup_Wrap,
  .stop = NULL,
  .teardown = CPthreadReqSetnameNp_Teardown_Wrap,
  .scope = CPthreadReqSetnameNp_Scope,
  .initial_context = &CPthreadReqSetnameNp_Instance
};

static const uint8_t CPthreadReqSetnameNp_Weights[] = { 6, 2, 1 };

static void CPthreadReqSetnameNp_Skip(
  CPthreadReqSetnameNp_Context *ctx,
  size_t                        index
)
{
  switch ( index + 1 ) {
    case 1:
      ctx->Map.pci[ 1 ] = CPthreadReqSetnameNp_Pre_Thread_NA - 1;
      /* Fall through */
    case 2:
      ctx->Map.pci[ 2 ] = CPthreadReqSetnameNp_Pre_Length_NA - 1;
      break;
  }
}

static inline CPthreadReqSetnameNp_Entry CPthreadReqSetnameNp_PopEntry(
  CPthreadReqSetnameNp_Context *ctx
)
{
  size_t index;

  if ( ctx->Map.skip ) {
    size_t i;

    ctx->Map.skip = false;
    index = 0;

    for ( i = 0; i < 3; ++i ) {
      index += CPthreadReqSetnameNp_Weights[ i ] * ctx->Map.pci[ i ];
    }
  } else {
    index = ctx->Map.index;
  }

  ctx->Map.index = index + 1;

  return CPthreadReqSetnameNp_Entries[ CPthreadReqSetnameNp_Map[ index ] ];
}

static void CPthreadReqSetnameNp_SetPreConditionStates(
  CPthreadReqSetnameNp_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];

  if ( ctx->Map.entry.Pre_Length_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqSetnameNp_Pre_Length_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }
}

static void CPthreadReqSetnameNp_TestVariant(
  CPthreadReqSetnameNp_Context *ctx
)
{
  CPthreadReqSetnameNp_Pre_NameStorage_Prepare( ctx, ctx->Map.pcs[ 0 ] );

  if ( ctx->Map.skip ) {
    CPthreadReqSetnameNp_Skip( ctx, 0 );
    return;
  }

  CPthreadReqSetnameNp_Pre_Thread_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CPthreadReqSetnameNp_Pre_Length_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CPthreadReqSetnameNp_Action( ctx );
  CPthreadReqSetnameNp_Post_Status_Check( ctx, ctx->Map.entry.Post_Status );
  CPthreadReqSetnameNp_Post_Name_Check( ctx, ctx->Map.entry.Post_Name );
  CPthreadReqSetnameNp_Post_ObjectName_Check(
    ctx,
    ctx->Map.entry.Post_ObjectName
  );
  CPthreadReqSetnameNp_Post_AllocatorLock_Check(
    ctx,
    ctx->Map.entry.Post_AllocatorLock
  );
}

/**
 * @fn void T_case_body_CPthreadReqSetnameNp( void )
 */
T_TEST_CASE_FIXTURE( CPthreadReqSetnameNp, &CPthreadReqSetnameNp_Fixture )
{
  CPthreadReqSetnameNp_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;
  ctx->Map.skip = false;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqSetnameNp_Pre_NameStorage_None;
    ctx->Map.pci[ 0 ] < CPthreadReqSetnameNp_Pre_NameStorage_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqSetnameNp_Pre_Thread_NoObj;
      ctx->Map.pci[ 1 ] < CPthreadReqSetnameNp_Pre_Thread_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqSetnameNp_Pre_Length_Fits;
        ctx->Map.pci[ 2 ] < CPthreadReqSetnameNp_Pre_Length_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        ctx->Map.entry = CPthreadReqSetnameNp_PopEntry( ctx );

        if ( ctx->Map.entry.Skip ) {
          continue;
        }

        CPthreadReqSetnameNp_SetPreConditionStates( ctx );
        CPthreadReqSetnameNp_Prepare( ctx );
        CPthreadReqSetnameNp_TestVariant( ctx );
      }
    }
  }
}

/** @} */
