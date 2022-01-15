/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSTestCaseRtemsBarrierReqWait
 */

/*
 * Copyright (C) 2021 embedded brains GmbH (http://www.embedded-brains.de)
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

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RTEMSTestCaseRtemsBarrierReqWait spec:/rtems/barrier/req/wait
 *
 * @ingroup RTEMSTestSuiteTestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  RtemsBarrierReqWait_Pre_Id_NoObj,
  RtemsBarrierReqWait_Pre_Id_Manual,
  RtemsBarrierReqWait_Pre_Id_Auto,
  RtemsBarrierReqWait_Pre_Id_NA
} RtemsBarrierReqWait_Pre_Id;

typedef enum {
  RtemsBarrierReqWait_Pre_Timeout_Ticks,
  RtemsBarrierReqWait_Pre_Timeout_Forever,
  RtemsBarrierReqWait_Pre_Timeout_NA
} RtemsBarrierReqWait_Pre_Timeout;

typedef enum {
  RtemsBarrierReqWait_Pre_Satisfy_Never,
  RtemsBarrierReqWait_Pre_Satisfy_Wait,
  RtemsBarrierReqWait_Pre_Satisfy_Release,
  RtemsBarrierReqWait_Pre_Satisfy_Delete,
  RtemsBarrierReqWait_Pre_Satisfy_NA
} RtemsBarrierReqWait_Pre_Satisfy;

typedef enum {
  RtemsBarrierReqWait_Post_Status_Ok,
  RtemsBarrierReqWait_Post_Status_InvId,
  RtemsBarrierReqWait_Post_Status_Timeout,
  RtemsBarrierReqWait_Post_Status_ObjDel,
  RtemsBarrierReqWait_Post_Status_NoReturn,
  RtemsBarrierReqWait_Post_Status_NA
} RtemsBarrierReqWait_Post_Status;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Id_NA : 1;
  uint8_t Pre_Timeout_NA : 1;
  uint8_t Pre_Satisfy_NA : 1;
  uint8_t Post_Status : 3;
} RtemsBarrierReqWait_Entry;

/**
 * @brief Test context for spec:/rtems/barrier/req/wait test case.
 */
typedef struct {
  rtems_id main_id;

  rtems_id high_worker_id;

  rtems_id low_worker_id;

  rtems_id manual_release_id;

  rtems_id auto_release_id;

  rtems_id id;

  rtems_interval timeout;

  rtems_status_code status;

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
    RtemsBarrierReqWait_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsBarrierReqWait_Context;

static RtemsBarrierReqWait_Context
  RtemsBarrierReqWait_Instance;

static const char * const RtemsBarrierReqWait_PreDesc_Id[] = {
  "NoObj",
  "Manual",
  "Auto",
  "NA"
};

static const char * const RtemsBarrierReqWait_PreDesc_Timeout[] = {
  "Ticks",
  "Forever",
  "NA"
};

static const char * const RtemsBarrierReqWait_PreDesc_Satisfy[] = {
  "Never",
  "Wait",
  "Release",
  "Delete",
  "NA"
};

static const char * const * const RtemsBarrierReqWait_PreDesc[] = {
  RtemsBarrierReqWait_PreDesc_Id,
  RtemsBarrierReqWait_PreDesc_Timeout,
  RtemsBarrierReqWait_PreDesc_Satisfy,
  NULL
};

#define NAME rtems_build_name( 'T', 'E', 'S', 'T' )

#define EVENT_TIMER_INACTIVE RTEMS_EVENT_0

#define EVENT_WAIT RTEMS_EVENT_1

#define EVENT_RELEASE RTEMS_EVENT_2

#define EVENT_DELETE RTEMS_EVENT_3

#define EVENT_TIMER_EXPIRE RTEMS_EVENT_4

typedef RtemsBarrierReqWait_Context Context;

static void Worker( rtems_task_argument arg )
{
  Context *ctx;

  ctx = (Context *) arg;

  while ( true ) {
    rtems_status_code sc;
    rtems_event_set   events;

    events = ReceiveAnyEvents();

    if ( ( events & EVENT_TIMER_INACTIVE ) != 0 ) {
      T_eq_int(
        T_get_thread_timer_state( ctx->main_id ),
        T_THREAD_TIMER_INACTIVE
      );
    }

    if ( ( events & EVENT_WAIT ) != 0 ) {
      sc = rtems_barrier_wait( ctx->id, RTEMS_NO_TIMEOUT );
      T_rsc_success( sc );
    }

    if ( ( events & EVENT_RELEASE ) != 0 ) {
      uint32_t released;

      sc = rtems_barrier_release( ctx->id, &released );
      T_rsc_success( sc );
    }

    if ( ( events & EVENT_DELETE ) != 0 ) {
      rtems_attribute     attribute_set;
      uint32_t            maximum_waiters;
      rtems_id           *id;
      rtems_task_priority prio;

      if ( ctx->id == ctx->manual_release_id ) {
        attribute_set = RTEMS_BARRIER_MANUAL_RELEASE;
        maximum_waiters = 0;
        id = &ctx->manual_release_id;
      } else {
        attribute_set = RTEMS_BARRIER_AUTOMATIC_RELEASE;
        maximum_waiters = 2;
        id = &ctx->auto_release_id;
      }

      prio = SetSelfPriority( PRIO_HIGH );
      T_eq_u32( prio, PRIO_LOW );

      sc = rtems_barrier_delete( ctx->id );
      T_rsc_success( sc );

      sc = rtems_barrier_create( NAME, attribute_set, maximum_waiters, id );
      T_rsc_success( sc );

      prio = SetSelfPriority( prio );
      T_eq_u32( prio, PRIO_HIGH );
    }

    if ( ( events & EVENT_TIMER_EXPIRE ) != 0 ) {
      T_eq_int(
        T_get_thread_timer_state( ctx->main_id ),
        T_THREAD_TIMER_SCHEDULED
      );
      FinalClockTick();
    }
  }
}

static void RtemsBarrierReqWait_Pre_Id_Prepare(
  RtemsBarrierReqWait_Context *ctx,
  RtemsBarrierReqWait_Pre_Id   state
)
{
  switch ( state ) {
    case RtemsBarrierReqWait_Pre_Id_NoObj: {
      /*
       * While the ``id`` parameter is not associated with a barrier.
       */
      ctx->id = 0xffffffff;
      break;
    }

    case RtemsBarrierReqWait_Pre_Id_Manual: {
      /*
       * While the ``id`` parameter is associated with a manual release
       * barrier.
       */
      ctx->id = ctx->manual_release_id;
      break;
    }

    case RtemsBarrierReqWait_Pre_Id_Auto: {
      /*
       * While the ``id`` parameter is associated with an automatic release
       * barrier.
       */
      ctx->id = ctx->auto_release_id;
      break;
    }

    case RtemsBarrierReqWait_Pre_Id_NA:
      break;
  }
}

static void RtemsBarrierReqWait_Pre_Timeout_Prepare(
  RtemsBarrierReqWait_Context    *ctx,
  RtemsBarrierReqWait_Pre_Timeout state
)
{
  switch ( state ) {
    case RtemsBarrierReqWait_Pre_Timeout_Ticks: {
      /*
       * While the ``released`` parameter is a clock tick interval.
       */
      ctx->timeout = UINT32_MAX;
      break;
    }

    case RtemsBarrierReqWait_Pre_Timeout_Forever: {
      /*
       * While the ``released`` parameter is RTEMS_NO_TIMEOUT.
       */
      ctx->timeout = RTEMS_NO_TIMEOUT;
      break;
    }

    case RtemsBarrierReqWait_Pre_Timeout_NA:
      break;
  }
}

static void RtemsBarrierReqWait_Pre_Satisfy_Prepare(
  RtemsBarrierReqWait_Context    *ctx,
  RtemsBarrierReqWait_Pre_Satisfy state
)
{
  switch ( state ) {
    case RtemsBarrierReqWait_Pre_Satisfy_Never: {
      /*
       * While the calling task waits at the barrier, while the barrier is not
       * released or deleted.
       */
      if ( ctx->timeout == RTEMS_NO_TIMEOUT ) {
        SendEvents( ctx->low_worker_id, EVENT_TIMER_INACTIVE | EVENT_RELEASE );
      } else {
        SendEvents( ctx->low_worker_id, EVENT_TIMER_EXPIRE );
      }
      break;
    }

    case RtemsBarrierReqWait_Pre_Satisfy_Wait: {
      /*
       * While calling the directive releases the barrier.
       */
      SendEvents( ctx->high_worker_id, EVENT_WAIT );
      break;
    }

    case RtemsBarrierReqWait_Pre_Satisfy_Release: {
      /*
       * While the calling task waits at the barrier, while the barrier is
       * released.
       */
      SendEvents( ctx->low_worker_id, EVENT_RELEASE );
      break;
    }

    case RtemsBarrierReqWait_Pre_Satisfy_Delete: {
      /*
       * While the calling task waits at the barrier, while the barrier is
       * deleted.
       */
      SendEvents( ctx->low_worker_id, EVENT_DELETE );
      break;
    }

    case RtemsBarrierReqWait_Pre_Satisfy_NA:
      break;
  }
}

static void RtemsBarrierReqWait_Post_Status_Check(
  RtemsBarrierReqWait_Context    *ctx,
  RtemsBarrierReqWait_Post_Status state
)
{
  switch ( state ) {
    case RtemsBarrierReqWait_Post_Status_Ok: {
      /*
       * The return status of rtems_barrier_wait() shall be RTEMS_SUCCESSFUL.
       */
      T_rsc_success( ctx->status );
      break;
    }

    case RtemsBarrierReqWait_Post_Status_InvId: {
      /*
       * The return status of rtems_barrier_wait() shall be RTEMS_INVALID_ID.
       */
      T_rsc( ctx->status, RTEMS_INVALID_ID );
      break;
    }

    case RtemsBarrierReqWait_Post_Status_Timeout: {
      /*
       * The return status of rtems_barrier_wait() shall be RTEMS_TIMEOUT.
       */
      T_rsc( ctx->status, RTEMS_TIMEOUT );
      break;
    }

    case RtemsBarrierReqWait_Post_Status_ObjDel: {
      /*
       * The return status of rtems_barrier_wait() shall be
       * RTEMS_OBJECT_WAS_DELETED.
       */
      T_rsc( ctx->status, RTEMS_OBJECT_WAS_DELETED );
      break;
    }

    case RtemsBarrierReqWait_Post_Status_NoReturn: {
      /*
       * The call to rtems_barrier_wait() shall not return to the calling task.
       */
      T_rsc_success( ctx->status );
      break;
    }

    case RtemsBarrierReqWait_Post_Status_NA:
      break;
  }
}

static void RtemsBarrierReqWait_Setup( RtemsBarrierReqWait_Context *ctx )
{
  rtems_status_code sc;

  memset( ctx, 0, sizeof( *ctx ) );
  ctx->main_id = rtems_task_self();
  SetSelfPriority( PRIO_NORMAL );
  ctx->high_worker_id = CreateTask( "WRKH", PRIO_HIGH );
  StartTask( ctx->high_worker_id, Worker, ctx );
  ctx->low_worker_id = CreateTask( "WRKL", PRIO_LOW );
  StartTask( ctx->low_worker_id, Worker, ctx );

  sc = rtems_barrier_create(
    NAME,
    RTEMS_BARRIER_MANUAL_RELEASE,
    0,
    &ctx->manual_release_id
  );
  T_assert_rsc_success( sc );

  sc = rtems_barrier_create(
    NAME,
    RTEMS_BARRIER_AUTOMATIC_RELEASE,
    2,
    &ctx->auto_release_id
  );
  T_assert_rsc_success( sc );
}

static void RtemsBarrierReqWait_Setup_Wrap( void *arg )
{
  RtemsBarrierReqWait_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsBarrierReqWait_Setup( ctx );
}

static void RtemsBarrierReqWait_Teardown( RtemsBarrierReqWait_Context *ctx )
{
  rtems_status_code sc;

  DeleteTask( ctx->high_worker_id );
  DeleteTask( ctx->low_worker_id );

  if ( ctx->manual_release_id != 0 ) {
    sc = rtems_barrier_delete( ctx->manual_release_id );
    T_rsc_success( sc );
  }

  if ( ctx->auto_release_id != 0 ) {
    sc = rtems_barrier_delete( ctx->auto_release_id );
    T_rsc_success( sc );
  }

  RestoreRunnerPriority();
}

static void RtemsBarrierReqWait_Teardown_Wrap( void *arg )
{
  RtemsBarrierReqWait_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsBarrierReqWait_Teardown( ctx );
}

static void RtemsBarrierReqWait_Action( RtemsBarrierReqWait_Context *ctx )
{
  ctx->status = rtems_barrier_wait( ctx->id, ctx->timeout );
}

static const RtemsBarrierReqWait_Entry
RtemsBarrierReqWait_Entries[] = {
  { 0, 0, 1, 1, RtemsBarrierReqWait_Post_Status_InvId },
  { 0, 0, 0, 0, RtemsBarrierReqWait_Post_Status_Ok },
  { 0, 0, 0, 0, RtemsBarrierReqWait_Post_Status_ObjDel },
  { 0, 0, 0, 0, RtemsBarrierReqWait_Post_Status_Timeout },
  { 1, 0, 0, 0, RtemsBarrierReqWait_Post_Status_NA },
  { 0, 0, 0, 0, RtemsBarrierReqWait_Post_Status_NoReturn }
};

static const uint8_t
RtemsBarrierReqWait_Map[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 1, 2, 5, 4, 1, 2, 3, 1, 1, 2, 5, 1, 1, 2
};

static size_t RtemsBarrierReqWait_Scope( void *arg, char *buf, size_t n )
{
  RtemsBarrierReqWait_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( RtemsBarrierReqWait_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture RtemsBarrierReqWait_Fixture = {
  .setup = RtemsBarrierReqWait_Setup_Wrap,
  .stop = NULL,
  .teardown = RtemsBarrierReqWait_Teardown_Wrap,
  .scope = RtemsBarrierReqWait_Scope,
  .initial_context = &RtemsBarrierReqWait_Instance
};

static inline RtemsBarrierReqWait_Entry RtemsBarrierReqWait_PopEntry(
  RtemsBarrierReqWait_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsBarrierReqWait_Entries[
    RtemsBarrierReqWait_Map[ index ]
  ];
}

static void RtemsBarrierReqWait_SetPreConditionStates(
  RtemsBarrierReqWait_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_Timeout_NA ) {
    ctx->Map.pcs[ 1 ] = RtemsBarrierReqWait_Pre_Timeout_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  if ( ctx->Map.entry.Pre_Satisfy_NA ) {
    ctx->Map.pcs[ 2 ] = RtemsBarrierReqWait_Pre_Satisfy_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }
}

static void RtemsBarrierReqWait_TestVariant( RtemsBarrierReqWait_Context *ctx )
{
  RtemsBarrierReqWait_Pre_Id_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  RtemsBarrierReqWait_Pre_Timeout_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  RtemsBarrierReqWait_Pre_Satisfy_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  RtemsBarrierReqWait_Action( ctx );
  RtemsBarrierReqWait_Post_Status_Check( ctx, ctx->Map.entry.Post_Status );
}

/**
 * @fn void T_case_body_RtemsBarrierReqWait( void )
 */
T_TEST_CASE_FIXTURE( RtemsBarrierReqWait, &RtemsBarrierReqWait_Fixture )
{
  RtemsBarrierReqWait_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = RtemsBarrierReqWait_Pre_Id_NoObj;
    ctx->Map.pci[ 0 ] < RtemsBarrierReqWait_Pre_Id_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = RtemsBarrierReqWait_Pre_Timeout_Ticks;
      ctx->Map.pci[ 1 ] < RtemsBarrierReqWait_Pre_Timeout_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = RtemsBarrierReqWait_Pre_Satisfy_Never;
        ctx->Map.pci[ 2 ] < RtemsBarrierReqWait_Pre_Satisfy_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        ctx->Map.entry = RtemsBarrierReqWait_PopEntry( ctx );

        if ( ctx->Map.entry.Skip ) {
          continue;
        }

        RtemsBarrierReqWait_SetPreConditionStates( ctx );
        RtemsBarrierReqWait_TestVariant( ctx );
      }
    }
  }
}

/** @} */
