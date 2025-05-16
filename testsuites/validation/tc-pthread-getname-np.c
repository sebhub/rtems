/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqGetnameNp
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
 * @defgroup CPthreadReqGetnameNp spec:/c/pthread/req/getname-np
 *
 * @ingroup TestsuitesValidationAcfg1
 * @ingroup TestsuitesValidationNoClock0
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef enum {
  CPthreadReqGetnameNp_Pre_NameStorage_None,
  CPthreadReqGetnameNp_Pre_NameStorage_TerminatorOnly,
  CPthreadReqGetnameNp_Pre_NameStorage_String,
  CPthreadReqGetnameNp_Pre_NameStorage_NA
} CPthreadReqGetnameNp_Pre_NameStorage;

typedef enum {
  CPthreadReqGetnameNp_Pre_Thread_NoObj,
  CPthreadReqGetnameNp_Pre_Thread_OtherClass,
  CPthreadReqGetnameNp_Pre_Thread_Valid,
  CPthreadReqGetnameNp_Pre_Thread_NA
} CPthreadReqGetnameNp_Pre_Thread;

typedef enum {
  CPthreadReqGetnameNp_Pre_ThreadName_Empty,
  CPthreadReqGetnameNp_Pre_ThreadName_NonEmpty,
  CPthreadReqGetnameNp_Pre_ThreadName_NA
} CPthreadReqGetnameNp_Pre_ThreadName;

typedef enum {
  CPthreadReqGetnameNp_Pre_NameLen_NullZero,
  CPthreadReqGetnameNp_Pre_NameLen_ValidZero,
  CPthreadReqGetnameNp_Pre_NameLen_Valid,
  CPthreadReqGetnameNp_Pre_NameLen_NA
} CPthreadReqGetnameNp_Pre_NameLen;

typedef enum {
  CPthreadReqGetnameNp_Pre_ThreadNameLength_Fits,
  CPthreadReqGetnameNp_Pre_ThreadNameLength_TooLong,
  CPthreadReqGetnameNp_Pre_ThreadNameLength_NA
} CPthreadReqGetnameNp_Pre_ThreadNameLength;

typedef enum {
  CPthreadReqGetnameNp_Post_Status_Zero,
  CPthreadReqGetnameNp_Post_Status_ESRCH,
  CPthreadReqGetnameNp_Post_Status_ERANGE,
  CPthreadReqGetnameNp_Post_Status_NA
} CPthreadReqGetnameNp_Post_Status;

typedef enum {
  CPthreadReqGetnameNp_Post_Name_Empty,
  CPthreadReqGetnameNp_Post_Name_PartialObject,
  CPthreadReqGetnameNp_Post_Name_FullObject,
  CPthreadReqGetnameNp_Post_Name_PartialString,
  CPthreadReqGetnameNp_Post_Name_FullString,
  CPthreadReqGetnameNp_Post_Name_Unchanged,
  CPthreadReqGetnameNp_Post_Name_NA
} CPthreadReqGetnameNp_Post_Name;

typedef enum {
  CPthreadReqGetnameNp_Post_AllocatorLock_Once,
  CPthreadReqGetnameNp_Post_AllocatorLock_NA
} CPthreadReqGetnameNp_Post_AllocatorLock;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_NameStorage_NA : 1;
  uint16_t Pre_Thread_NA : 1;
  uint16_t Pre_ThreadName_NA : 1;
  uint16_t Pre_NameLen_NA : 1;
  uint16_t Pre_ThreadNameLength_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_Name : 3;
  uint16_t Post_AllocatorLock : 1;
} CPthreadReqGetnameNp_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/getname-np test case.
 */
typedef struct {
  /**
   * @brief This member provides a mutex.
   */
  rtems_id mutex_id;

  /**
   * @brief This member provides a buffer for the thread name.
   */
  char name_buf[ 7 ];
  ;

  /**
   * @brief This member contains the object name of the calling thread.
   */
  char expected_name[ 5 ];
  ;

  /**
   * @brief This member provides the `thread` parameter value.
   */
  pthread_t thread;

  /**
   * @brief This member provides the `name` parameter value.
   */
  char *name;

  /**
   * @brief This member provides the `len` parameter value.
   */
  size_t len;

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
    CPthreadReqGetnameNp_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqGetnameNp_Context;

static CPthreadReqGetnameNp_Context CPthreadReqGetnameNp_Instance;

static const char *const CPthreadReqGetnameNp_PreDesc_NameStorage[] =
  { "None", "TerminatorOnly", "String", "NA" };

static const char *const CPthreadReqGetnameNp_PreDesc_Thread[] =
  { "NoObj", "OtherClass", "Valid", "NA" };

static const char *const CPthreadReqGetnameNp_PreDesc_ThreadName[] =
  { "Empty", "NonEmpty", "NA" };

static const char *const CPthreadReqGetnameNp_PreDesc_NameLen[] =
  { "NullZero", "ValidZero", "Valid", "NA" };

static const char *const CPthreadReqGetnameNp_PreDesc_ThreadNameLength[] =
  { "Fits", "TooLong", "NA" };

static const char *const *const CPthreadReqGetnameNp_PreDesc[] = {
  CPthreadReqGetnameNp_PreDesc_NameStorage,
  CPthreadReqGetnameNp_PreDesc_Thread,
  CPthreadReqGetnameNp_PreDesc_ThreadName,
  CPthreadReqGetnameNp_PreDesc_NameLen,
  CPthreadReqGetnameNp_PreDesc_ThreadNameLength,
  NULL
};

static void CPthreadReqGetnameNp_Pre_NameStorage_Prepare(
  CPthreadReqGetnameNp_Context        *ctx,
  CPthreadReqGetnameNp_Pre_NameStorage state
)
{
  switch ( state ) {
    case CPthreadReqGetnameNp_Pre_NameStorage_None: {
      /*
       * Where the value of the CONFIGURE_MAXIMUM_THREAD_NAME_SIZE application
       * configuration option is zero.
       */
      if ( _Thread_Maximum_name_size != 0 ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case CPthreadReqGetnameNp_Pre_NameStorage_TerminatorOnly: {
      /*
       * Where the value of the CONFIGURE_MAXIMUM_THREAD_NAME_SIZE application
       * configuration option is one.
       */
      if ( _Thread_Maximum_name_size != 1 ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case CPthreadReqGetnameNp_Pre_NameStorage_String: {
      /*
       * Where the value of the CONFIGURE_MAXIMUM_THREAD_NAME_SIZE application
       * configuration option is greater than or equal to two.
       */
      if ( _Thread_Maximum_name_size < 2 ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case CPthreadReqGetnameNp_Pre_NameStorage_NA:
      break;
  }
}

static void CPthreadReqGetnameNp_Pre_Thread_Prepare(
  CPthreadReqGetnameNp_Context   *ctx,
  CPthreadReqGetnameNp_Pre_Thread state
)
{
  switch ( state ) {
    case CPthreadReqGetnameNp_Pre_Thread_NoObj: {
      /*
       * While the `thread` parameter is not associated with an object.
       */
      ctx->thread = 0xffffffff;
      break;
    }

    case CPthreadReqGetnameNp_Pre_Thread_OtherClass: {
      /*
       * While the `thread` parameter is associated with an object which is not
       * a thread.
       */
      ctx->thread = ctx->mutex_id;
      break;
    }

    case CPthreadReqGetnameNp_Pre_Thread_Valid: {
      /*
       * While the `thread` parameter is associated with a thread.
       */
      ctx->thread = pthread_self();
      break;
    }

    case CPthreadReqGetnameNp_Pre_Thread_NA:
      break;
  }
}

static void CPthreadReqGetnameNp_Pre_ThreadName_Prepare(
  CPthreadReqGetnameNp_Pre_ThreadName state
)
{
  int eno;

  switch ( state ) {
    case CPthreadReqGetnameNp_Pre_ThreadName_Empty: {
      /*
       * While the thread has the empty string name.
       */
      eno = pthread_setname_np( pthread_self(), "" );
      T_eno_success( eno );
      break;
    }

    case CPthreadReqGetnameNp_Pre_ThreadName_NonEmpty: {
      /*
       * While the thread has a non-empty string name.
       */
      T_assert_ge_sz( _Thread_Maximum_name_size, sizeof( "string" ) );
      eno = pthread_setname_np( pthread_self(), "string" );
      T_eno_success( eno );
      break;
    }

    case CPthreadReqGetnameNp_Pre_ThreadName_NA:
      break;
  }
}

static void CPthreadReqGetnameNp_Pre_NameLen_Prepare(
  CPthreadReqGetnameNp_Context    *ctx,
  CPthreadReqGetnameNp_Pre_NameLen state
)
{
  switch ( state ) {
    case CPthreadReqGetnameNp_Pre_NameLen_NullZero: {
      /*
       * While the `name` parameter is NULL and the `len` parameter is zero.
       */
      ctx->name = NULL;
      ctx->len = 0;
      break;
    }

    case CPthreadReqGetnameNp_Pre_NameLen_ValidZero: {
      /*
       * While the `name` parameter references a buffer and the `len` parameter
       * is zero.
       */
      ctx->name = ctx->name_buf;
      ctx->len = 0;
      break;
    }

    case CPthreadReqGetnameNp_Pre_NameLen_Valid: {
      /*
       * While the `name` parameter references a buffer of the size specified
       * by the `len` parameter, which is greater than zero.
       */
      ctx->name = ctx->name_buf;
      ctx->len = RTEMS_ARRAY_SIZE( ctx->name_buf );
      break;
    }

    case CPthreadReqGetnameNp_Pre_NameLen_NA:
      break;
  }
}

static void CPthreadReqGetnameNp_Pre_ThreadNameLength_Prepare(
  CPthreadReqGetnameNp_Context             *ctx,
  CPthreadReqGetnameNp_Pre_ThreadNameLength state
)
{
  switch ( state ) {
    case CPthreadReqGetnameNp_Pre_ThreadNameLength_Fits: {
      /*
       * While the thread name fits into the buffer referenced by the `name`
       * parameter.
       */
      /* Already prepared */
      break;
    }

    case CPthreadReqGetnameNp_Pre_ThreadNameLength_TooLong: {
      /*
       * While the thread name does not fit into the buffer referenced by the
       * `name` parameter.
       */
      if ( ctx->len > 0 ) {
        ctx->len = 2;
      }
      break;
    }

    case CPthreadReqGetnameNp_Pre_ThreadNameLength_NA:
      break;
  }
}

static void CPthreadReqGetnameNp_Post_Status_Check(
  CPthreadReqGetnameNp_Context    *ctx,
  CPthreadReqGetnameNp_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqGetnameNp_Post_Status_Zero: {
      /*
       * The return value of pthread_getname_np() shall be equal to zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqGetnameNp_Post_Status_ESRCH: {
      /*
       * The return value of pthread_getname_np() shall be equal to ESRCH.
       */
      T_eq_int( ctx->status, ESRCH );
      break;
    }

    case CPthreadReqGetnameNp_Post_Status_ERANGE: {
      /*
       * The return value of pthread_getname_np() shall be equal to ERANGE.
       */
      T_eq_int( ctx->status, ERANGE );
      break;
    }

    case CPthreadReqGetnameNp_Post_Status_NA:
      break;
  }
}

static void CPthreadReqGetnameNp_Post_Name_Check(
  CPthreadReqGetnameNp_Context  *ctx,
  CPthreadReqGetnameNp_Post_Name state
)
{
  switch ( state ) {
    case CPthreadReqGetnameNp_Post_Name_Empty: {
      /*
       * The string referenced by the `name` parameter shall contain the empty
       * string.
       */
      T_eq_str( ctx->name_buf, "" );
      break;
    }

    case CPthreadReqGetnameNp_Post_Name_PartialObject: {
      /*
       * The string referenced by the `name` parameter shall contain the first
       * characters of the object name of the thread, up to the value of the
       * `len` parameter minus one.
       */
      T_eq_char( ctx->name_buf[ 0 ], ctx->expected_name[ 0 ] );
      T_eq_char( ctx->name_buf[ 1 ], '\0' );
      break;
    }

    case CPthreadReqGetnameNp_Post_Name_FullObject: {
      /*
       * The string referenced by the `name` parameter shall contain the object
       * name of the thread.
       */
      T_eq_str( ctx->name_buf, ctx->expected_name );
      break;
    }

    case CPthreadReqGetnameNp_Post_Name_PartialString: {
      /*
       * The string referenced by the `name` parameter shall contain the first
       * characters of the string name of the thread, up to the value of the
       * `len` parameter minus one.
       */
      T_eq_str( ctx->name_buf, "s" );
      break;
    }

    case CPthreadReqGetnameNp_Post_Name_FullString: {
      /*
       * The string referenced by the `name` parameter shall contain the string
       * name of the thread.
       */
      T_eq_str( ctx->name_buf, "string" );
      break;
    }

    case CPthreadReqGetnameNp_Post_Name_Unchanged: {
      /*
       * The buffer referenced by the `name` parameter shall be unchanged.
       */
      for ( size_t i = 0; i < sizeof( ctx->name_buf ); ++i ) {
        T_quiet_eq_char( ctx->name_buf[ i ], (char) 0xff );
      }
      break;
    }

    case CPthreadReqGetnameNp_Post_Name_NA:
      break;
  }
}

static void CPthreadReqGetnameNp_Post_AllocatorLock_Check(
  CPthreadReqGetnameNp_Context           *ctx,
  CPthreadReqGetnameNp_Post_AllocatorLock state
)
{
  switch ( state ) {
    case CPthreadReqGetnameNp_Post_AllocatorLock_Once: {
      /*
       * The pthread_getname_np() directive shall obtain the object allocator
       * mutex exactly once.
       */
      T_eq_uint( ctx->lock_count, 1 );
      break;
    }

    case CPthreadReqGetnameNp_Post_AllocatorLock_NA:
      break;
  }
}

static void CPthreadReqGetnameNp_Setup( CPthreadReqGetnameNp_Context *ctx )
{
  char *name;

  (void) pthread_setname_np( pthread_self(), "" );
  name = rtems_object_get_name(
    rtems_task_self(),
    sizeof( ctx->expected_name ),
    ctx->expected_name
  );
  T_assert_eq_ptr( name, ctx->expected_name );
  T_assert_ge_sz( strlen( ctx->expected_name ), 2 );
  ctx->mutex_id = CreateMutex();
}

static void CPthreadReqGetnameNp_Setup_Wrap( void *arg )
{
  CPthreadReqGetnameNp_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqGetnameNp_Setup( ctx );
}

static void CPthreadReqGetnameNp_Teardown( CPthreadReqGetnameNp_Context *ctx )
{
  CallCounterObserve( &AllocatorLockCounter, 0 );
  DeleteMutex( ctx->mutex_id );
  (void) pthread_setname_np( pthread_self(), "" );
}

static void CPthreadReqGetnameNp_Teardown_Wrap( void *arg )
{
  CPthreadReqGetnameNp_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  CPthreadReqGetnameNp_Teardown( ctx );
}

static void CPthreadReqGetnameNp_Prepare( CPthreadReqGetnameNp_Context *ctx )
{
  memset( ctx->name_buf, 0xff, sizeof( ctx->name_buf ) );
}

static void CPthreadReqGetnameNp_Action( CPthreadReqGetnameNp_Context *ctx )
{
  CallCounterObserve( &AllocatorLockCounter, rtems_task_self() );
  ctx->status = pthread_getname_np( ctx->thread, ctx->name, ctx->len );
  ctx->lock_count = CallCounterGet( &AllocatorLockCounter );
  CallCounterObserve( &AllocatorLockCounter, 0 );
}

/* clang-format off */

static const CPthreadReqGetnameNp_Entry
CPthreadReqGetnameNp_Entries[] = {
  { 1, 0, 0, 0, 0, 0, CPthreadReqGetnameNp_Post_Status_NA,
    CPthreadReqGetnameNp_Post_Name_NA,
    CPthreadReqGetnameNp_Post_AllocatorLock_NA },
  { 0, 0, 0, 1, 0, 1, CPthreadReqGetnameNp_Post_Status_ESRCH,
    CPthreadReqGetnameNp_Post_Name_NA,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 1, 0, 1, CPthreadReqGetnameNp_Post_Status_ESRCH,
    CPthreadReqGetnameNp_Post_Name_Unchanged,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 1, 0, 1, CPthreadReqGetnameNp_Post_Status_ESRCH,
    CPthreadReqGetnameNp_Post_Name_Empty,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, 0, 1, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_NA,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, 0, 0, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_NA,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, 0, 1, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_Unchanged,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, 0, 0, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_Unchanged,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, 0, 0, CPthreadReqGetnameNp_Post_Status_Zero,
    CPthreadReqGetnameNp_Post_Name_FullObject,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, 0, 0, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_PartialObject,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 1, 0, 1, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_NA,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 1, 0, 0, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_NA,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 1, 0, 1, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_Unchanged,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 1, 0, 0, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_Unchanged,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 1, 0, 0, CPthreadReqGetnameNp_Post_Status_Zero,
    CPthreadReqGetnameNp_Post_Name_FullObject,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 1, 0, 0, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_PartialObject,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, 0, 0, CPthreadReqGetnameNp_Post_Status_Zero,
    CPthreadReqGetnameNp_Post_Name_FullString,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once },
  { 0, 0, 0, 0, 0, 0, CPthreadReqGetnameNp_Post_Status_ERANGE,
    CPthreadReqGetnameNp_Post_Name_PartialString,
    CPthreadReqGetnameNp_Post_AllocatorLock_Once }
};

static const uint8_t
CPthreadReqGetnameNp_Map[] = {
  1, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 10,
  11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1,
  0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 1, 0, 2,
  0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 4, 5, 6, 7, 8,
  9, 4, 5, 6, 7, 16, 17
};

/* clang-format on */

static size_t CPthreadReqGetnameNp_Scope( void *arg, char *buf, size_t n )
{
  CPthreadReqGetnameNp_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CPthreadReqGetnameNp_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CPthreadReqGetnameNp_Fixture = {
  .setup = CPthreadReqGetnameNp_Setup_Wrap,
  .stop = NULL,
  .teardown = CPthreadReqGetnameNp_Teardown_Wrap,
  .scope = CPthreadReqGetnameNp_Scope,
  .initial_context = &CPthreadReqGetnameNp_Instance
};

static const uint8_t CPthreadReqGetnameNp_Weights[] = { 36, 12, 6, 2, 1 };

static void CPthreadReqGetnameNp_Skip(
  CPthreadReqGetnameNp_Context *ctx,
  size_t                        index
)
{
  switch ( index + 1 ) {
    case 1:
      ctx->Map.pci[ 1 ] = CPthreadReqGetnameNp_Pre_Thread_NA - 1;
      /* Fall through */
    case 2:
      ctx->Map.pci[ 2 ] = CPthreadReqGetnameNp_Pre_ThreadName_NA - 1;
      /* Fall through */
    case 3:
      ctx->Map.pci[ 3 ] = CPthreadReqGetnameNp_Pre_NameLen_NA - 1;
      /* Fall through */
    case 4:
      ctx->Map.pci[ 4 ] = CPthreadReqGetnameNp_Pre_ThreadNameLength_NA - 1;
      break;
  }
}

static inline CPthreadReqGetnameNp_Entry CPthreadReqGetnameNp_PopEntry(
  CPthreadReqGetnameNp_Context *ctx
)
{
  size_t index;

  if ( ctx->Map.skip ) {
    size_t i;

    ctx->Map.skip = false;
    index = 0;

    for ( i = 0; i < 5; ++i ) {
      index += CPthreadReqGetnameNp_Weights[ i ] * ctx->Map.pci[ i ];
    }
  } else {
    index = ctx->Map.index;
  }

  ctx->Map.index = index + 1;

  return CPthreadReqGetnameNp_Entries[ CPthreadReqGetnameNp_Map[ index ] ];
}

static void CPthreadReqGetnameNp_SetPreConditionStates(
  CPthreadReqGetnameNp_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];

  if ( ctx->Map.entry.Pre_ThreadName_NA ) {
    ctx->Map.pcs[ 2 ] = CPthreadReqGetnameNp_Pre_ThreadName_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];

  if ( ctx->Map.entry.Pre_ThreadNameLength_NA ) {
    ctx->Map.pcs[ 4 ] = CPthreadReqGetnameNp_Pre_ThreadNameLength_NA;
  } else {
    ctx->Map.pcs[ 4 ] = ctx->Map.pci[ 4 ];
  }
}

static void CPthreadReqGetnameNp_TestVariant(
  CPthreadReqGetnameNp_Context *ctx
)
{
  CPthreadReqGetnameNp_Pre_NameStorage_Prepare( ctx, ctx->Map.pcs[ 0 ] );

  if ( ctx->Map.skip ) {
    CPthreadReqGetnameNp_Skip( ctx, 0 );
    return;
  }

  CPthreadReqGetnameNp_Pre_Thread_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CPthreadReqGetnameNp_Pre_ThreadName_Prepare( ctx->Map.pcs[ 2 ] );
  CPthreadReqGetnameNp_Pre_NameLen_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  CPthreadReqGetnameNp_Pre_ThreadNameLength_Prepare( ctx, ctx->Map.pcs[ 4 ] );
  CPthreadReqGetnameNp_Action( ctx );
  CPthreadReqGetnameNp_Post_Status_Check( ctx, ctx->Map.entry.Post_Status );
  CPthreadReqGetnameNp_Post_Name_Check( ctx, ctx->Map.entry.Post_Name );
  CPthreadReqGetnameNp_Post_AllocatorLock_Check(
    ctx,
    ctx->Map.entry.Post_AllocatorLock
  );
}

/**
 * @fn void T_case_body_CPthreadReqGetnameNp( void )
 */
T_TEST_CASE_FIXTURE( CPthreadReqGetnameNp, &CPthreadReqGetnameNp_Fixture )
{
  CPthreadReqGetnameNp_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;
  ctx->Map.skip = false;

  for (
    ctx->Map.pci[ 0 ] = CPthreadReqGetnameNp_Pre_NameStorage_None;
    ctx->Map.pci[ 0 ] < CPthreadReqGetnameNp_Pre_NameStorage_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = CPthreadReqGetnameNp_Pre_Thread_NoObj;
      ctx->Map.pci[ 1 ] < CPthreadReqGetnameNp_Pre_Thread_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = CPthreadReqGetnameNp_Pre_ThreadName_Empty;
        ctx->Map.pci[ 2 ] < CPthreadReqGetnameNp_Pre_ThreadName_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = CPthreadReqGetnameNp_Pre_NameLen_NullZero;
          ctx->Map.pci[ 3 ] < CPthreadReqGetnameNp_Pre_NameLen_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          for (
            ctx->Map.pci[ 4 ] = CPthreadReqGetnameNp_Pre_ThreadNameLength_Fits;
            ctx->Map.pci[ 4 ] < CPthreadReqGetnameNp_Pre_ThreadNameLength_NA;
            ++ctx->Map.pci[ 4 ]
          ) {
            ctx->Map.entry = CPthreadReqGetnameNp_PopEntry( ctx );

            if ( ctx->Map.entry.Skip ) {
              continue;
            }

            CPthreadReqGetnameNp_SetPreConditionStates( ctx );
            CPthreadReqGetnameNp_Prepare( ctx );
            CPthreadReqGetnameNp_TestVariant( ctx );
          }
        }
      }
    }
  }
}

/** @} */
