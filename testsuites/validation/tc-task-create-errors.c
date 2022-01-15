/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSTestCaseRtemsTaskReqCreateErrors
 */

/*
 * Copyright (C) 2020, 2021 embedded brains GmbH (http://www.embedded-brains.de)
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

/*
 * This file is part of the RTEMS quality process and was automatically
 * generated.  If you find something that needs to be fixed or
 * worded better please post a report or patch to an RTEMS mailing list
 * or raise a bug report:
 *
 * https://www.rtems.org/bugs.html
 *
 * For information on updating and regenerating please refer to the How-To
 * section in the Software Requirements Engineering chapter of the
 * RTEMS Software Engineering manual.  The manual is provided as a part of
 * a release.  For development sources please refer to the online
 * documentation at:
 *
 * https://docs.rtems.org
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <string.h>
#include <rtems/score/apimutex.h>
#include <rtems/score/threadimpl.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RTEMSTestCaseRtemsTaskReqCreateErrors \
 *   spec:/rtems/task/req/create-errors
 *
 * @ingroup RTEMSTestSuiteTestsuitesValidationNoClock0
 * @ingroup RTEMSTestSuiteTestsuitesValidationOneCpu0
 *
 * @{
 */

typedef enum {
  RtemsTaskReqCreateErrors_Pre_Name_Valid,
  RtemsTaskReqCreateErrors_Pre_Name_Inv,
  RtemsTaskReqCreateErrors_Pre_Name_NA
} RtemsTaskReqCreateErrors_Pre_Name;

typedef enum {
  RtemsTaskReqCreateErrors_Pre_Id_Valid,
  RtemsTaskReqCreateErrors_Pre_Id_Null,
  RtemsTaskReqCreateErrors_Pre_Id_NA
} RtemsTaskReqCreateErrors_Pre_Id;

typedef enum {
  RtemsTaskReqCreateErrors_Pre_SysTsk_Yes,
  RtemsTaskReqCreateErrors_Pre_SysTsk_No,
  RtemsTaskReqCreateErrors_Pre_SysTsk_NA
} RtemsTaskReqCreateErrors_Pre_SysTsk;

typedef enum {
  RtemsTaskReqCreateErrors_Pre_Prio_Valid,
  RtemsTaskReqCreateErrors_Pre_Prio_Zero,
  RtemsTaskReqCreateErrors_Pre_Prio_Inv,
  RtemsTaskReqCreateErrors_Pre_Prio_NA
} RtemsTaskReqCreateErrors_Pre_Prio;

typedef enum {
  RtemsTaskReqCreateErrors_Pre_Free_Yes,
  RtemsTaskReqCreateErrors_Pre_Free_No,
  RtemsTaskReqCreateErrors_Pre_Free_NA
} RtemsTaskReqCreateErrors_Pre_Free;

typedef enum {
  RtemsTaskReqCreateErrors_Pre_Stack_Normal,
  RtemsTaskReqCreateErrors_Pre_Stack_Small,
  RtemsTaskReqCreateErrors_Pre_Stack_Huge,
  RtemsTaskReqCreateErrors_Pre_Stack_NA
} RtemsTaskReqCreateErrors_Pre_Stack;

typedef enum {
  RtemsTaskReqCreateErrors_Pre_Ext_Ok,
  RtemsTaskReqCreateErrors_Pre_Ext_Err,
  RtemsTaskReqCreateErrors_Pre_Ext_NA
} RtemsTaskReqCreateErrors_Pre_Ext;

typedef enum {
  RtemsTaskReqCreateErrors_Post_Status_Ok,
  RtemsTaskReqCreateErrors_Post_Status_InvAddr,
  RtemsTaskReqCreateErrors_Post_Status_InvName,
  RtemsTaskReqCreateErrors_Post_Status_InvPrio,
  RtemsTaskReqCreateErrors_Post_Status_TooMany,
  RtemsTaskReqCreateErrors_Post_Status_Unsat,
  RtemsTaskReqCreateErrors_Post_Status_NA
} RtemsTaskReqCreateErrors_Post_Status;

typedef enum {
  RtemsTaskReqCreateErrors_Post_Name_Valid,
  RtemsTaskReqCreateErrors_Post_Name_Invalid,
  RtemsTaskReqCreateErrors_Post_Name_NA
} RtemsTaskReqCreateErrors_Post_Name;

typedef enum {
  RtemsTaskReqCreateErrors_Post_IdVar_Set,
  RtemsTaskReqCreateErrors_Post_IdVar_Nop,
  RtemsTaskReqCreateErrors_Post_IdVar_NA
} RtemsTaskReqCreateErrors_Post_IdVar;

typedef enum {
  RtemsTaskReqCreateErrors_Post_CreateExt_Yes,
  RtemsTaskReqCreateErrors_Post_CreateExt_No,
  RtemsTaskReqCreateErrors_Post_CreateExt_NA
} RtemsTaskReqCreateErrors_Post_CreateExt;

typedef enum {
  RtemsTaskReqCreateErrors_Post_DelExt_Yes,
  RtemsTaskReqCreateErrors_Post_DelExt_No,
  RtemsTaskReqCreateErrors_Post_DelExt_NA
} RtemsTaskReqCreateErrors_Post_DelExt;

typedef struct {
  uint32_t Skip : 1;
  uint32_t Pre_Name_NA : 1;
  uint32_t Pre_Id_NA : 1;
  uint32_t Pre_SysTsk_NA : 1;
  uint32_t Pre_Prio_NA : 1;
  uint32_t Pre_Free_NA : 1;
  uint32_t Pre_Stack_NA : 1;
  uint32_t Pre_Ext_NA : 1;
  uint32_t Post_Status : 3;
  uint32_t Post_Name : 2;
  uint32_t Post_IdVar : 2;
  uint32_t Post_CreateExt : 2;
  uint32_t Post_DelExt : 2;
} RtemsTaskReqCreateErrors_Entry;

/**
 * @brief Test context for spec:/rtems/task/req/create-errors test case.
 */
typedef struct {
  rtems_status_code status;

  rtems_id *id;

  rtems_id id_value;

  bool create_extension_status;

  uint32_t create_extension_calls;

  uint32_t delete_extension_calls;

  rtems_name name;

  rtems_task_priority initial_priority;

  size_t stack_size;

  rtems_attribute attributes;

  rtems_id extension_id;

  void *seized_objects;

  struct {
    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 7 ];

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
    RtemsTaskReqCreateErrors_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsTaskReqCreateErrors_Context;

static RtemsTaskReqCreateErrors_Context
  RtemsTaskReqCreateErrors_Instance;

static const char * const RtemsTaskReqCreateErrors_PreDesc_Name[] = {
  "Valid",
  "Inv",
  "NA"
};

static const char * const RtemsTaskReqCreateErrors_PreDesc_Id[] = {
  "Valid",
  "Null",
  "NA"
};

static const char * const RtemsTaskReqCreateErrors_PreDesc_SysTsk[] = {
  "Yes",
  "No",
  "NA"
};

static const char * const RtemsTaskReqCreateErrors_PreDesc_Prio[] = {
  "Valid",
  "Zero",
  "Inv",
  "NA"
};

static const char * const RtemsTaskReqCreateErrors_PreDesc_Free[] = {
  "Yes",
  "No",
  "NA"
};

static const char * const RtemsTaskReqCreateErrors_PreDesc_Stack[] = {
  "Normal",
  "Small",
  "Huge",
  "NA"
};

static const char * const RtemsTaskReqCreateErrors_PreDesc_Ext[] = {
  "Ok",
  "Err",
  "NA"
};

static const char * const * const RtemsTaskReqCreateErrors_PreDesc[] = {
  RtemsTaskReqCreateErrors_PreDesc_Name,
  RtemsTaskReqCreateErrors_PreDesc_Id,
  RtemsTaskReqCreateErrors_PreDesc_SysTsk,
  RtemsTaskReqCreateErrors_PreDesc_Prio,
  RtemsTaskReqCreateErrors_PreDesc_Free,
  RtemsTaskReqCreateErrors_PreDesc_Stack,
  RtemsTaskReqCreateErrors_PreDesc_Ext,
  NULL
};

#define NAME rtems_build_name( 'T', 'E', 'S', 'T' )

typedef RtemsTaskReqCreateErrors_Context Context;

static rtems_status_code Create( void *arg, uint32_t *id )
{
  Context          *ctx;
  bool              create_extension_status;
  rtems_status_code sc;

  ctx = arg;
  create_extension_status = ctx->create_extension_status;
  ctx->create_extension_status = true;
  sc = rtems_task_create(
    rtems_build_name( 'S', 'I', 'Z', 'E' ),
    1,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    id
  );
  ctx->create_extension_status = create_extension_status;

  return sc;
}

static bool ThreadCreate( rtems_tcb *executing, rtems_tcb *created )
{
  (void) executing;
  (void) created;

  ++RtemsTaskReqCreateErrors_Instance.create_extension_calls;
  return RtemsTaskReqCreateErrors_Instance.create_extension_status;
}

static void ThreadDelete( rtems_tcb *executing, rtems_tcb *deleted )
{
  (void) executing;
  (void) deleted;

  ++RtemsTaskReqCreateErrors_Instance.delete_extension_calls;
}

static const rtems_extensions_table extensions = {
  .thread_create = ThreadCreate,
  .thread_delete = ThreadDelete
};

static void RtemsTaskReqCreateErrors_Pre_Name_Prepare(
  RtemsTaskReqCreateErrors_Context *ctx,
  RtemsTaskReqCreateErrors_Pre_Name state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Pre_Name_Valid: {
      /*
       * While the ``name`` parameter is valid.
       */
      ctx->name = NAME;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Name_Inv: {
      /*
       * While the ``name`` parameter is invalid.
       */
      ctx->name = 0;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Name_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Pre_Id_Prepare(
  RtemsTaskReqCreateErrors_Context *ctx,
  RtemsTaskReqCreateErrors_Pre_Id   state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Pre_Id_Valid: {
      /*
       * While the ``id`` parameter references an object of type rtems_id.
       */
      ctx->id = &ctx->id_value;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Id_Null: {
      /*
       * While the ``id`` parameter is NULL.
       */
      ctx->id = NULL;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Id_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Pre_SysTsk_Prepare(
  RtemsTaskReqCreateErrors_Context   *ctx,
  RtemsTaskReqCreateErrors_Pre_SysTsk state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Pre_SysTsk_Yes: {
      /*
       * While the ``attribute_set`` parameter specifies a system task.
       */
      ctx->attributes = RTEMS_SYSTEM_TASK;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_SysTsk_No: {
      /*
       * While the ``attribute_set`` parameter specifies an application task.
       */
      ctx->attributes = RTEMS_DEFAULT_ATTRIBUTES;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_SysTsk_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Pre_Prio_Prepare(
  RtemsTaskReqCreateErrors_Context *ctx,
  RtemsTaskReqCreateErrors_Pre_Prio state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Pre_Prio_Valid: {
      /*
       * While the ``initial_priority`` parameter is valid and non-zero.
       */
      ctx->initial_priority = RTEMS_MAXIMUM_PRIORITY - 1;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Prio_Zero: {
      /*
       * While the ``initial_priority`` parameter is zero.
       */
      ctx->initial_priority = 0;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Prio_Inv: {
      /*
       * While the ``initial_priority`` parameter is invalid.
       */
      ctx->initial_priority = 0xffffffff;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Prio_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Pre_Free_Prepare(
  RtemsTaskReqCreateErrors_Context *ctx,
  RtemsTaskReqCreateErrors_Pre_Free state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Pre_Free_Yes: {
      /*
       * While the system has at least one inactive task object available.
       */
      /* Nothing to do */
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Free_No: {
      /*
       * While the system has no inactive task object available.
       */
      ctx->seized_objects = T_seize_objects( Create, ctx );
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Free_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Pre_Stack_Prepare(
  RtemsTaskReqCreateErrors_Context  *ctx,
  RtemsTaskReqCreateErrors_Pre_Stack state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Pre_Stack_Normal: {
      /*
       * While the ``initial_priority`` parameter is greater than or equal to
       * the configured minimum size and less than or equal to the maximum
       * stack size which can be allocated by the system.
       */
      ctx->stack_size = RTEMS_MINIMUM_STACK_SIZE;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Stack_Small: {
      /*
       * While the ``initial_priority`` parameter is less than the configured
       * minimum size.
       */
      ctx->stack_size = 0;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Stack_Huge: {
      /*
       * While the ``initial_priority`` parameter is greater than the maximum
       * stack size which can be allocated by the system.
       */
      ctx->stack_size = SIZE_MAX;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Stack_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Pre_Ext_Prepare(
  RtemsTaskReqCreateErrors_Context *ctx,
  RtemsTaskReqCreateErrors_Pre_Ext  state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Pre_Ext_Ok: {
      /*
       * While none of the task create extensions fails.
       */
      ctx->create_extension_status = true;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Ext_Err: {
      /*
       * While at least one of the task create extensions fails.
       */
      ctx->create_extension_status = false;
      break;
    }

    case RtemsTaskReqCreateErrors_Pre_Ext_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Post_Status_Check(
  RtemsTaskReqCreateErrors_Context    *ctx,
  RtemsTaskReqCreateErrors_Post_Status state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Post_Status_Ok: {
      /*
       * The return status of rtems_task_create() shall be RTEMS_SUCCESSFUL.
       */
      T_rsc_success( ctx->status );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_Status_InvAddr: {
      /*
       * The return status of rtems_task_create() shall be
       * RTEMS_INVALID_ADDRESS.
       */
      T_rsc( ctx->status, RTEMS_INVALID_ADDRESS );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_Status_InvName: {
      /*
       * The return status of rtems_task_create() shall be RTEMS_INVALID_NAME.
       */
      T_rsc( ctx->status, RTEMS_INVALID_NAME );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_Status_InvPrio: {
      /*
       * The return status of rtems_task_create() shall be
       * RTEMS_INVALID_PRIORITY.
       */
      T_rsc( ctx->status, RTEMS_INVALID_PRIORITY );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_Status_TooMany: {
      /*
       * The return status of rtems_task_create() shall be RTEMS_TOO_MANY.
       */
      T_rsc( ctx->status, RTEMS_TOO_MANY );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_Status_Unsat: {
      /*
       * The return status of rtems_task_create() shall be RTEMS_UNSATISFIED.
       */
      T_rsc( ctx->status, RTEMS_UNSATISFIED  );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_Status_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Post_Name_Check(
  RtemsTaskReqCreateErrors_Context  *ctx,
  RtemsTaskReqCreateErrors_Post_Name state
)
{
  rtems_status_code sc;
  rtems_id          id;

  switch ( state ) {
    case RtemsTaskReqCreateErrors_Post_Name_Valid: {
      /*
       * The unique object name shall identify the task created by the
       * rtems_task_create() call.
       */
      id = 0;
      sc = rtems_task_ident( NAME, RTEMS_SEARCH_LOCAL_NODE, &id );
      T_rsc_success( sc );
      T_eq_u32( id, ctx->id_value );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_Name_Invalid: {
      /*
       * The unique object name shall not identify a task.
       */
      sc = rtems_task_ident( NAME, RTEMS_SEARCH_LOCAL_NODE, &id );
      T_rsc( sc, RTEMS_INVALID_NAME );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_Name_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Post_IdVar_Check(
  RtemsTaskReqCreateErrors_Context   *ctx,
  RtemsTaskReqCreateErrors_Post_IdVar state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Post_IdVar_Set: {
      /*
       * The value of the object referenced by the ``id`` parameter shall be
       * set to the object identifier of the created task after the return of
       * the rtems_task_create() call.
       */
      T_eq_ptr( ctx->id, &ctx->id_value );
      T_ne_u32( ctx->id_value, INVALID_ID );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_IdVar_Nop: {
      /*
       * Objects referenced by the ``id`` parameter in past calls to
       * rtems_task_create() shall not be accessed by the rtems_task_create()
       * call.
       */
      T_eq_u32( ctx->id_value, INVALID_ID );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_IdVar_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Post_CreateExt_Check(
  RtemsTaskReqCreateErrors_Context       *ctx,
  RtemsTaskReqCreateErrors_Post_CreateExt state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Post_CreateExt_Yes: {
      /*
       * The create user extensions shall be invoked during the
       * rtems_task_create() call.
       */
      T_eq_u32( ctx->create_extension_calls, 1 );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_CreateExt_No: {
      /*
       * The create user extensions shall not be invoked during the
       * rtems_task_create() call.
       */
      T_eq_u32( ctx->create_extension_calls, 0 );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_CreateExt_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Post_DelExt_Check(
  RtemsTaskReqCreateErrors_Context    *ctx,
  RtemsTaskReqCreateErrors_Post_DelExt state
)
{
  switch ( state ) {
    case RtemsTaskReqCreateErrors_Post_DelExt_Yes: {
      /*
       * The delete user extensions shall be invoked during the
       * rtems_task_create() call.
       */
      T_eq_u32( ctx->delete_extension_calls, 1 );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_DelExt_No: {
      /*
       * The delete user extensions shall not be invoked during the
       * rtems_task_create() call.
       */
      T_eq_u32( ctx->delete_extension_calls, 0 );
      break;
    }

    case RtemsTaskReqCreateErrors_Post_DelExt_NA:
      break;
  }
}

static void RtemsTaskReqCreateErrors_Setup(
  RtemsTaskReqCreateErrors_Context *ctx
)
{
  rtems_status_code sc;

  sc = rtems_extension_create(
    rtems_build_name( 'T', 'E', 'X', 'T' ),
    &extensions,
    &ctx->extension_id
  );
  T_rsc_success( sc );
}

static void RtemsTaskReqCreateErrors_Setup_Wrap( void *arg )
{
  RtemsTaskReqCreateErrors_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsTaskReqCreateErrors_Setup( ctx );
}

static void RtemsTaskReqCreateErrors_Teardown(
  RtemsTaskReqCreateErrors_Context *ctx
)
{
  rtems_status_code sc;

  sc = rtems_extension_delete( ctx->extension_id );
  T_rsc_success( sc );
}

static void RtemsTaskReqCreateErrors_Teardown_Wrap( void *arg )
{
  RtemsTaskReqCreateErrors_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsTaskReqCreateErrors_Teardown( ctx );
}

static void RtemsTaskReqCreateErrors_Prepare(
  RtemsTaskReqCreateErrors_Context *ctx
)
{
  _RTEMS_Lock_allocator();
  _Thread_Kill_zombies();
  _RTEMS_Unlock_allocator();

  ctx->id_value = INVALID_ID;
}

static void RtemsTaskReqCreateErrors_Action(
  RtemsTaskReqCreateErrors_Context *ctx
)
{
  ctx->create_extension_calls = 0;
  ctx->delete_extension_calls = 0;
  ctx->status = rtems_task_create(
    ctx->name,
    ctx->initial_priority,
    ctx->stack_size,
    RTEMS_DEFAULT_MODES,
    ctx->attributes,
    ctx->id
  );
}

static void RtemsTaskReqCreateErrors_Cleanup(
  RtemsTaskReqCreateErrors_Context *ctx
)
{
  if ( ctx->id_value != INVALID_ID ) {
    rtems_status_code sc;

    sc = rtems_task_delete( ctx->id_value );
    T_rsc_success( sc );

    ctx->id_value = INVALID_ID;
  }

  T_surrender_objects( &ctx->seized_objects, rtems_task_delete );
}

static const RtemsTaskReqCreateErrors_Entry
RtemsTaskReqCreateErrors_Entries[] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqCreateErrors_Post_Status_InvName,
    RtemsTaskReqCreateErrors_Post_Name_Invalid,
    RtemsTaskReqCreateErrors_Post_IdVar_Nop,
    RtemsTaskReqCreateErrors_Post_CreateExt_No,
    RtemsTaskReqCreateErrors_Post_DelExt_No },
  { 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqCreateErrors_Post_Status_InvAddr,
    RtemsTaskReqCreateErrors_Post_Name_Invalid,
    RtemsTaskReqCreateErrors_Post_IdVar_Nop,
    RtemsTaskReqCreateErrors_Post_CreateExt_No,
    RtemsTaskReqCreateErrors_Post_DelExt_No },
  { 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqCreateErrors_Post_Status_InvPrio,
    RtemsTaskReqCreateErrors_Post_Name_Invalid,
    RtemsTaskReqCreateErrors_Post_IdVar_Nop,
    RtemsTaskReqCreateErrors_Post_CreateExt_No,
    RtemsTaskReqCreateErrors_Post_DelExt_No },
  { 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqCreateErrors_Post_Status_TooMany,
    RtemsTaskReqCreateErrors_Post_Name_Invalid,
    RtemsTaskReqCreateErrors_Post_IdVar_Nop,
    RtemsTaskReqCreateErrors_Post_CreateExt_No,
    RtemsTaskReqCreateErrors_Post_DelExt_No },
  { 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqCreateErrors_Post_Status_Ok,
    RtemsTaskReqCreateErrors_Post_Name_Valid,
    RtemsTaskReqCreateErrors_Post_IdVar_Set,
    RtemsTaskReqCreateErrors_Post_CreateExt_Yes,
    RtemsTaskReqCreateErrors_Post_DelExt_No },
  { 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqCreateErrors_Post_Status_Unsat,
    RtemsTaskReqCreateErrors_Post_Name_Invalid,
    RtemsTaskReqCreateErrors_Post_IdVar_Nop,
    RtemsTaskReqCreateErrors_Post_CreateExt_Yes,
    RtemsTaskReqCreateErrors_Post_DelExt_Yes },
  { 0, 0, 0, 0, 0, 0, 0, 0, RtemsTaskReqCreateErrors_Post_Status_Unsat,
    RtemsTaskReqCreateErrors_Post_Name_Invalid,
    RtemsTaskReqCreateErrors_Post_IdVar_Nop,
    RtemsTaskReqCreateErrors_Post_CreateExt_No,
    RtemsTaskReqCreateErrors_Post_DelExt_No }
};

static const uint8_t
RtemsTaskReqCreateErrors_Map[] = {
  4, 5, 4, 5, 6, 6, 3, 3, 3, 3, 3, 3, 4, 5, 4, 5, 6, 6, 3, 3, 3, 3, 3, 3, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 5, 4, 5, 6, 6, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0
};

static size_t RtemsTaskReqCreateErrors_Scope( void *arg, char *buf, size_t n )
{
  RtemsTaskReqCreateErrors_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsTaskReqCreateErrors_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsTaskReqCreateErrors_Fixture = {
  .setup = RtemsTaskReqCreateErrors_Setup_Wrap,
  .stop = NULL,
  .teardown = RtemsTaskReqCreateErrors_Teardown_Wrap,
  .scope = RtemsTaskReqCreateErrors_Scope,
  .initial_context = &RtemsTaskReqCreateErrors_Instance
};

static inline RtemsTaskReqCreateErrors_Entry RtemsTaskReqCreateErrors_PopEntry(
  RtemsTaskReqCreateErrors_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsTaskReqCreateErrors_Entries[
    RtemsTaskReqCreateErrors_Map[ index ]
  ];
}

static void RtemsTaskReqCreateErrors_TestVariant(
  RtemsTaskReqCreateErrors_Context *ctx
)
{
  RtemsTaskReqCreateErrors_Pre_Name_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  RtemsTaskReqCreateErrors_Pre_Id_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  RtemsTaskReqCreateErrors_Pre_SysTsk_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  RtemsTaskReqCreateErrors_Pre_Prio_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  RtemsTaskReqCreateErrors_Pre_Free_Prepare( ctx, ctx->Map.pcs[ 4 ] );
  RtemsTaskReqCreateErrors_Pre_Stack_Prepare( ctx, ctx->Map.pcs[ 5 ] );
  RtemsTaskReqCreateErrors_Pre_Ext_Prepare( ctx, ctx->Map.pcs[ 6 ] );
  RtemsTaskReqCreateErrors_Action( ctx );
  RtemsTaskReqCreateErrors_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  RtemsTaskReqCreateErrors_Post_Name_Check( ctx, ctx->Map.entry.Post_Name );
  RtemsTaskReqCreateErrors_Post_IdVar_Check( ctx, ctx->Map.entry.Post_IdVar );
  RtemsTaskReqCreateErrors_Post_CreateExt_Check(
    ctx,
    ctx->Map.entry.Post_CreateExt
  );
  RtemsTaskReqCreateErrors_Post_DelExt_Check(
    ctx,
    ctx->Map.entry.Post_DelExt
  );
}

/**
 * @fn void T_case_body_RtemsTaskReqCreateErrors( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsTaskReqCreateErrors,
  &RtemsTaskReqCreateErrors_Fixture
)
{
  RtemsTaskReqCreateErrors_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = RtemsTaskReqCreateErrors_Pre_Name_Valid;
    ctx->Map.pcs[ 0 ] < RtemsTaskReqCreateErrors_Pre_Name_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = RtemsTaskReqCreateErrors_Pre_Id_Valid;
      ctx->Map.pcs[ 1 ] < RtemsTaskReqCreateErrors_Pre_Id_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      for (
        ctx->Map.pcs[ 2 ] = RtemsTaskReqCreateErrors_Pre_SysTsk_Yes;
        ctx->Map.pcs[ 2 ] < RtemsTaskReqCreateErrors_Pre_SysTsk_NA;
        ++ctx->Map.pcs[ 2 ]
      ) {
        for (
          ctx->Map.pcs[ 3 ] = RtemsTaskReqCreateErrors_Pre_Prio_Valid;
          ctx->Map.pcs[ 3 ] < RtemsTaskReqCreateErrors_Pre_Prio_NA;
          ++ctx->Map.pcs[ 3 ]
        ) {
          for (
            ctx->Map.pcs[ 4 ] = RtemsTaskReqCreateErrors_Pre_Free_Yes;
            ctx->Map.pcs[ 4 ] < RtemsTaskReqCreateErrors_Pre_Free_NA;
            ++ctx->Map.pcs[ 4 ]
          ) {
            for (
              ctx->Map.pcs[ 5 ] = RtemsTaskReqCreateErrors_Pre_Stack_Normal;
              ctx->Map.pcs[ 5 ] < RtemsTaskReqCreateErrors_Pre_Stack_NA;
              ++ctx->Map.pcs[ 5 ]
            ) {
              for (
                ctx->Map.pcs[ 6 ] = RtemsTaskReqCreateErrors_Pre_Ext_Ok;
                ctx->Map.pcs[ 6 ] < RtemsTaskReqCreateErrors_Pre_Ext_NA;
                ++ctx->Map.pcs[ 6 ]
              ) {
                ctx->Map.entry = RtemsTaskReqCreateErrors_PopEntry( ctx );
                RtemsTaskReqCreateErrors_Prepare( ctx );
                RtemsTaskReqCreateErrors_TestVariant( ctx );
                RtemsTaskReqCreateErrors_Cleanup( ctx );
              }
            }
          }
        }
      }
    }
  }
}

/** @} */
