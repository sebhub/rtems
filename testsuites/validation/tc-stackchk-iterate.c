/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsStackchkReqIterate
 */

/*
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

#include <rtems.h>
#include <string.h>
#include <rtems/stackchk.h>
#include <rtems/stackchkimpl.h>
#include <rtems/sysinit.h>
#include <rtems/score/apimutex.h>
#include <rtems/score/userextdata.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RtemsStackchkReqIterate spec:/rtems/stackchk/req/iterate
 *
 * @ingroup TestsuitesValidation0
 * @ingroup TestsuitesValidationAcfg1
 *
 * The configuration of the test suite decides whether it enables the stack
 * checker, so the test skips the variants of the other state. A handler of the
 * system initialization calls the directive in the device driver
 * initialization step. The variants in device driver initialization context
 * check the record of this call. The processor owned by a scheduler is
 * processor 0. A processor on which a thread began leaves its scheduler
 * through rtems_scheduler_remove_processor(). A processor on which no thread
 * began is a processor which the application configuration assigns to no
 * scheduler. The test skips a variant where no such processor exists.
 *
 * @{
 */

typedef enum {
  RtemsStackchkReqIterate_Pre_Enabled_Yes,
  RtemsStackchkReqIterate_Pre_Enabled_No,
  RtemsStackchkReqIterate_Pre_Enabled_NA
} RtemsStackchkReqIterate_Pre_Enabled;

typedef enum {
  RtemsStackchkReqIterate_Pre_Phase_DeviceDriverInit,
  RtemsStackchkReqIterate_Pre_Phase_Task,
  RtemsStackchkReqIterate_Pre_Phase_NA
} RtemsStackchkReqIterate_Pre_Phase;

typedef enum {
  RtemsStackchkReqIterate_Pre_Started_Yes,
  RtemsStackchkReqIterate_Pre_Started_No,
  RtemsStackchkReqIterate_Pre_Started_NA
} RtemsStackchkReqIterate_Pre_Started;

typedef enum {
  RtemsStackchkReqIterate_Pre_Owned_Yes,
  RtemsStackchkReqIterate_Pre_Owned_No,
  RtemsStackchkReqIterate_Pre_Owned_NA
} RtemsStackchkReqIterate_Pre_Owned;

typedef enum {
  RtemsStackchkReqIterate_Post_ThreadStacks_Each,
  RtemsStackchkReqIterate_Post_ThreadStacks_NA
} RtemsStackchkReqIterate_Post_ThreadStacks;

typedef enum {
  RtemsStackchkReqIterate_Post_InterruptStack_Once,
  RtemsStackchkReqIterate_Post_InterruptStack_Never,
  RtemsStackchkReqIterate_Post_InterruptStack_NA
} RtemsStackchkReqIterate_Post_InterruptStack;

typedef enum {
  RtemsStackchkReqIterate_Post_Order_ThreadsFirst,
  RtemsStackchkReqIterate_Post_Order_NA
} RtemsStackchkReqIterate_Post_Order;

typedef enum {
  RtemsStackchkReqIterate_Post_Argument_Arg,
  RtemsStackchkReqIterate_Post_Argument_NA
} RtemsStackchkReqIterate_Post_Argument;

typedef enum {
  RtemsStackchkReqIterate_Post_Lock_ThreadsLocked,
  RtemsStackchkReqIterate_Post_Lock_NA
} RtemsStackchkReqIterate_Post_Lock;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Enabled_NA : 1;
  uint16_t Pre_Phase_NA : 1;
  uint16_t Pre_Started_NA : 1;
  uint16_t Pre_Owned_NA : 1;
  uint16_t Post_ThreadStacks : 1;
  uint16_t Post_InterruptStack : 2;
  uint16_t Post_Order : 1;
  uint16_t Post_Argument : 1;
  uint16_t Post_Lock : 1;
} RtemsStackchkReqIterate_Entry;

typedef struct {
  uint32_t thread_count;
  uint32_t thread_visits;
  uint32_t thread_visits_twice;
  uint32_t thread_visits_unlocked;
  uint32_t interrupt_visits;
  uint32_t interrupt_visits_by_index[ CPU_MAXIMUM_PROCESSORS ];
  uint32_t thread_after_interrupt;
  uint32_t bad_arg;
  rtems_id last_thread_id;
} Record;

/**
 * @brief Test context for spec:/rtems/stackchk/req/iterate test case.
 */
typedef struct {
  /**
   * @brief This member is true, if the directive shall be called from within
   *   task context.
   */
  bool multitasking;

  /**
   * @brief This member contains the record of the visits.
   */
  Record record;

  /**
   * @brief This member is true, if a thread began on the processor.
   */
  bool thread_began;

  /**
   * @brief This member contains the index of the processor.
   */
  uint32_t cpu_index;

  /**
   * @brief This member contains the identifier of the scheduler which owned
   *   the removed processor, otherwise it is zero.
   */
  rtems_id removed_from;

  struct {
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
    RtemsStackchkReqIterate_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsStackchkReqIterate_Context;

static RtemsStackchkReqIterate_Context RtemsStackchkReqIterate_Instance;

static const char *const RtemsStackchkReqIterate_PreDesc_Enabled[] =
  { "Yes", "No", "NA" };

static const char *const RtemsStackchkReqIterate_PreDesc_Phase[] =
  { "DeviceDriverInit", "Task", "NA" };

static const char *const RtemsStackchkReqIterate_PreDesc_Started[] =
  { "Yes", "No", "NA" };

static const char *const RtemsStackchkReqIterate_PreDesc_Owned[] =
  { "Yes", "No", "NA" };

static const char *const *const RtemsStackchkReqIterate_PreDesc[] = {
  RtemsStackchkReqIterate_PreDesc_Enabled,
  RtemsStackchkReqIterate_PreDesc_Phase,
  RtemsStackchkReqIterate_PreDesc_Started,
  RtemsStackchkReqIterate_PreDesc_Owned,
  NULL
};

typedef RtemsStackchkReqIterate_Context Context;

static Record initialization_record;

static bool IsStackCheckerEnabled( void )
{
  size_t i;

  for ( i = 0; i < _User_extensions_Initial_count; ++i ) {
    if (
      _User_extensions_Initial_extensions[ i ].thread_create ==
      rtems_stack_checker_create_extension
    ) {
      return true;
    }
  }

  return false;
}

static bool RemoveAnyProcessor( Context *ctx )
{
  uint32_t cpu_index;

  for (
    cpu_index = 1; cpu_index < rtems_scheduler_get_processor_maximum();
    ++cpu_index
  ) {
    rtems_status_code sc;
    rtems_id          id;

    sc = rtems_scheduler_ident_by_processor( cpu_index, &id );

    if ( sc != RTEMS_SUCCESSFUL ) {
      continue;
    }

    sc = rtems_scheduler_remove_processor( id, cpu_index );

    if ( sc == RTEMS_SUCCESSFUL ) {
      ctx->cpu_index = cpu_index;
      ctx->removed_from = id;
      return true;
    }
  }

  return false;
}

static bool FindProcessorWithoutScheduler( Context *ctx )
{
  uint32_t cpu_index;

  for (
    cpu_index = 0; cpu_index < rtems_scheduler_get_processor_maximum();
    ++cpu_index
  ) {
    rtems_status_code sc;
    rtems_id          id;

    sc = rtems_scheduler_ident_by_processor( cpu_index, &id );

    if ( sc == RTEMS_INCORRECT_STATE ) {
      ctx->cpu_index = cpu_index;
      return true;
    }
  }

  return false;
}

static Record *expected_record;

static bool CountThread( rtems_tcb *tcb, void *arg )
{
  uint32_t *count;

  (void) tcb;
  count = arg;
  ++( *count );

  return false;
}

static void Visit( const rtems_stack_checker_info *info, void *arg )
{
  Record *record;

  record = expected_record;

  if ( arg != record ) {
    ++record->bad_arg;
  }

  /* The identifier of an interrupt stack is a processor index */
  if ( rtems_object_id_get_api( info->id ) == 0 ) {
    ++record->interrupt_visits;

    if ( info->id < CPU_MAXIMUM_PROCESSORS ) {
      ++record->interrupt_visits_by_index[ info->id ];
    }

    return;
  }

  ++record->thread_visits;

  if ( info->id == record->last_thread_id ) {
    ++record->thread_visits_twice;
  }

  record->last_thread_id = info->id;

  if ( record->interrupt_visits != 0 ) {
    ++record->thread_after_interrupt;
  }

  if ( !_RTEMS_Allocator_is_owner() ) {
    ++record->thread_visits_unlocked;
  }
}

static void Iterate( Record *record )
{
  memset( record, 0, sizeof( *record ) );
  expected_record = record;
  rtems_stack_checker_iterate( Visit, record );
  (void) rtems_task_iterate( CountThread, &record->thread_count );
}

static void IterateDuringInitialization( void )
{
  Iterate( &initialization_record );
}

RTEMS_SYSINIT_ITEM(
  IterateDuringInitialization,
  RTEMS_SYSINIT_DEVICE_DRIVERS,
  RTEMS_SYSINIT_ORDER_LAST
);

static void RtemsStackchkReqIterate_Pre_Enabled_Prepare(
  RtemsStackchkReqIterate_Context    *ctx,
  RtemsStackchkReqIterate_Pre_Enabled state
)
{
  switch ( state ) {
    case RtemsStackchkReqIterate_Pre_Enabled_Yes: {
      /*
       * Where the application configuration enables the stack checker.
       */
      if ( !IsStackCheckerEnabled() ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case RtemsStackchkReqIterate_Pre_Enabled_No: {
      /*
       * Where the application configuration does not enable the stack checker.
       */
      if ( IsStackCheckerEnabled() ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case RtemsStackchkReqIterate_Pre_Enabled_NA:
      break;
  }
}

static void RtemsStackchkReqIterate_Pre_Phase_Prepare(
  RtemsStackchkReqIterate_Context  *ctx,
  RtemsStackchkReqIterate_Pre_Phase state
)
{
  switch ( state ) {
    case RtemsStackchkReqIterate_Pre_Phase_DeviceDriverInit: {
      /*
       * While the rtems_stack_checker_iterate() directive is called from
       * within device driver initialization context.
       */
      ctx->multitasking = false;
      break;
    }

    case RtemsStackchkReqIterate_Pre_Phase_Task: {
      /*
       * While the rtems_stack_checker_iterate() directive is called from
       * within task context.
       */
      ctx->multitasking = true;
      break;
    }

    case RtemsStackchkReqIterate_Pre_Phase_NA:
      break;
  }
}

static void RtemsStackchkReqIterate_Pre_Started_Prepare(
  RtemsStackchkReqIterate_Context    *ctx,
  RtemsStackchkReqIterate_Pre_Started state
)
{
  switch ( state ) {
    case RtemsStackchkReqIterate_Pre_Started_Yes: {
      /*
       * While a thread began on the processor.
       */
      ctx->thread_began = true;
      break;
    }

    case RtemsStackchkReqIterate_Pre_Started_No: {
      /*
       * While no thread began on the processor.
       */
      ctx->thread_began = false;
      break;
    }

    case RtemsStackchkReqIterate_Pre_Started_NA:
      break;
  }
}

static void RtemsStackchkReqIterate_Pre_Owned_Prepare(
  RtemsStackchkReqIterate_Context  *ctx,
  RtemsStackchkReqIterate_Pre_Owned state
)
{
  switch ( state ) {
    case RtemsStackchkReqIterate_Pre_Owned_Yes: {
      /*
       * While the processor is owned by a scheduler.
       */
      ctx->cpu_index = 0;
      break;
    }

    case RtemsStackchkReqIterate_Pre_Owned_No: {
      /*
       * While the processor is not owned by a scheduler.
       */
      if ( ctx->thread_began ) {
        if ( !RemoveAnyProcessor( ctx ) ) {
          ctx->Map.skip = true;
        }
      } else if ( !FindProcessorWithoutScheduler( ctx ) ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case RtemsStackchkReqIterate_Pre_Owned_NA:
      break;
  }
}

static void RtemsStackchkReqIterate_Post_ThreadStacks_Check(
  RtemsStackchkReqIterate_Context          *ctx,
  RtemsStackchkReqIterate_Post_ThreadStacks state
)
{
  switch ( state ) {
    case RtemsStackchkReqIterate_Post_ThreadStacks_Each: {
      /*
       * The rtems_stack_checker_iterate() call shall invoke the visitor
       * routine specified by the `visit` parameter exactly once for the stack
       * of each thread.
       */
      T_eq_u32( ctx->record.thread_visits, ctx->record.thread_count );
      T_eq_u32( ctx->record.thread_visits_twice, 0 );
      break;
    }

    case RtemsStackchkReqIterate_Post_ThreadStacks_NA:
      break;
  }
}

static void RtemsStackchkReqIterate_Post_InterruptStack_Check(
  RtemsStackchkReqIterate_Context            *ctx,
  RtemsStackchkReqIterate_Post_InterruptStack state
)
{
  switch ( state ) {
    case RtemsStackchkReqIterate_Post_InterruptStack_Once: {
      /*
       * The rtems_stack_checker_iterate() call shall invoke the visitor
       * routine specified by the `visit` parameter exactly once for the
       * interrupt stack of the processor.
       */
      T_eq_u32( ctx->record.interrupt_visits_by_index[ ctx->cpu_index ], 1 );
      break;
    }

    case RtemsStackchkReqIterate_Post_InterruptStack_Never: {
      /*
       * The rtems_stack_checker_iterate() call shall invoke the visitor
       * routine specified by the `visit` parameter for no interrupt stack of
       * the processor.
       */
      T_eq_u32( ctx->record.interrupt_visits_by_index[ ctx->cpu_index ], 0 );
      break;
    }

    case RtemsStackchkReqIterate_Post_InterruptStack_NA:
      break;
  }
}

static void RtemsStackchkReqIterate_Post_Order_Check(
  RtemsStackchkReqIterate_Context   *ctx,
  RtemsStackchkReqIterate_Post_Order state
)
{
  switch ( state ) {
    case RtemsStackchkReqIterate_Post_Order_ThreadsFirst: {
      /*
       * The rtems_stack_checker_iterate() call shall invoke the visitor
       * routine specified by the `visit` parameter for the stacks of the
       * threads before it invokes this visitor routine for the interrupt stack
       * of a processor.
       */
      T_eq_u32( ctx->record.thread_after_interrupt, 0 );
      break;
    }

    case RtemsStackchkReqIterate_Post_Order_NA:
      break;
  }
}

static void RtemsStackchkReqIterate_Post_Argument_Check(
  RtemsStackchkReqIterate_Context      *ctx,
  RtemsStackchkReqIterate_Post_Argument state
)
{
  switch ( state ) {
    case RtemsStackchkReqIterate_Post_Argument_Arg: {
      /*
       * The second parameter of each invocation of the visitor routine
       * specified by the `visit` parameter shall be the `arg` parameter of the
       * rtems_stack_checker_iterate() call.
       */
      T_eq_u32( ctx->record.bad_arg, 0 );
      break;
    }

    case RtemsStackchkReqIterate_Post_Argument_NA:
      break;
  }
}

static void RtemsStackchkReqIterate_Post_Lock_Check(
  RtemsStackchkReqIterate_Context  *ctx,
  RtemsStackchkReqIterate_Post_Lock state
)
{
  switch ( state ) {
    case RtemsStackchkReqIterate_Post_Lock_ThreadsLocked: {
      /*
       * The rtems_stack_checker_iterate() call shall invoke the visitor
       * routine specified by the `visit` parameter for the stack of a thread
       * as the owner of the object allocator lock.
       */
      T_eq_u32( ctx->record.thread_visits_unlocked, 0 );
      break;
    }

    case RtemsStackchkReqIterate_Post_Lock_NA:
      break;
  }
}

static void RtemsStackchkReqIterate_Prepare(
  RtemsStackchkReqIterate_Context *ctx
)
{
  ctx->removed_from = 0;
}

static void RtemsStackchkReqIterate_Action(
  RtemsStackchkReqIterate_Context *ctx
)
{
  if ( ctx->multitasking ) {
    Iterate( &ctx->record );
  } else {
    ctx->record = initialization_record;
  }
}

static void RtemsStackchkReqIterate_Cleanup(
  RtemsStackchkReqIterate_Context *ctx
)
{
  if ( ctx->removed_from != 0 ) {
    rtems_status_code sc;

    sc = rtems_scheduler_add_processor( ctx->removed_from, ctx->cpu_index );
    T_rsc_success( sc );
  }
}

/* clang-format off */

static const RtemsStackchkReqIterate_Entry
RtemsStackchkReqIterate_Entries[] = {
  { 0, 0, 0, 0, 0, RtemsStackchkReqIterate_Post_ThreadStacks_Each,
    RtemsStackchkReqIterate_Post_InterruptStack_Never,
    RtemsStackchkReqIterate_Post_Order_ThreadsFirst,
    RtemsStackchkReqIterate_Post_Argument_Arg,
    RtemsStackchkReqIterate_Post_Lock_ThreadsLocked },
  { 1, 0, 0, 0, 0, RtemsStackchkReqIterate_Post_ThreadStacks_NA,
    RtemsStackchkReqIterate_Post_InterruptStack_NA,
    RtemsStackchkReqIterate_Post_Order_NA,
    RtemsStackchkReqIterate_Post_Argument_NA,
    RtemsStackchkReqIterate_Post_Lock_NA },
  { 0, 0, 0, 0, 0, RtemsStackchkReqIterate_Post_ThreadStacks_Each,
    RtemsStackchkReqIterate_Post_InterruptStack_Once,
    RtemsStackchkReqIterate_Post_Order_ThreadsFirst,
    RtemsStackchkReqIterate_Post_Argument_Arg,
    RtemsStackchkReqIterate_Post_Lock_ThreadsLocked },
  { 1, 0, 0, 0, 0, RtemsStackchkReqIterate_Post_ThreadStacks_NA,
    RtemsStackchkReqIterate_Post_InterruptStack_NA,
    RtemsStackchkReqIterate_Post_Order_NA,
    RtemsStackchkReqIterate_Post_Argument_NA,
    RtemsStackchkReqIterate_Post_Lock_NA }
};

static const uint8_t
RtemsStackchkReqIterate_Map[] = {
  1, 1, 0, 0, 2, 2, 3, 0, 1, 1, 0, 0, 0, 0, 3, 0
};

/* clang-format on */

static size_t RtemsStackchkReqIterate_Scope( void *arg, char *buf, size_t n )
{
  RtemsStackchkReqIterate_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsStackchkReqIterate_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsStackchkReqIterate_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = RtemsStackchkReqIterate_Scope,
  .initial_context = &RtemsStackchkReqIterate_Instance
};

static const uint8_t RtemsStackchkReqIterate_Weights[] = { 8, 4, 2, 1 };

static void RtemsStackchkReqIterate_Skip(
  RtemsStackchkReqIterate_Context *ctx,
  size_t                           index
)
{
  switch ( index + 1 ) {
    case 1:
      ctx->Map.pcs[ 1 ] = RtemsStackchkReqIterate_Pre_Phase_NA - 1;
      /* Fall through */
    case 2:
      ctx->Map.pcs[ 2 ] = RtemsStackchkReqIterate_Pre_Started_NA - 1;
      /* Fall through */
    case 3:
      ctx->Map.pcs[ 3 ] = RtemsStackchkReqIterate_Pre_Owned_NA - 1;
      break;
  }
}

static inline RtemsStackchkReqIterate_Entry RtemsStackchkReqIterate_PopEntry(
  RtemsStackchkReqIterate_Context *ctx
)
{
  size_t index;

  if ( ctx->Map.skip ) {
    size_t i;

    ctx->Map.skip = false;
    index = 0;

    for ( i = 0; i < 4; ++i ) {
      index += RtemsStackchkReqIterate_Weights[ i ] * ctx->Map.pcs[ i ];
    }
  } else {
    index = ctx->Map.index;
  }

  ctx->Map.index = index + 1;

  return RtemsStackchkReqIterate_Entries
    [ RtemsStackchkReqIterate_Map[ index ] ];
}

static void RtemsStackchkReqIterate_TestVariant(
  RtemsStackchkReqIterate_Context *ctx
)
{
  RtemsStackchkReqIterate_Pre_Enabled_Prepare( ctx, ctx->Map.pcs[ 0 ] );

  if ( ctx->Map.skip ) {
    RtemsStackchkReqIterate_Skip( ctx, 0 );
    return;
  }

  RtemsStackchkReqIterate_Pre_Phase_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  RtemsStackchkReqIterate_Pre_Started_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  RtemsStackchkReqIterate_Pre_Owned_Prepare( ctx, ctx->Map.pcs[ 3 ] );

  if ( ctx->Map.skip ) {
    RtemsStackchkReqIterate_Skip( ctx, 3 );
    return;
  }

  RtemsStackchkReqIterate_Action( ctx );
  RtemsStackchkReqIterate_Post_ThreadStacks_Check(
    ctx,
    ctx->Map.entry.Post_ThreadStacks
  );
  RtemsStackchkReqIterate_Post_InterruptStack_Check(
    ctx,
    ctx->Map.entry.Post_InterruptStack
  );
  RtemsStackchkReqIterate_Post_Order_Check( ctx, ctx->Map.entry.Post_Order );
  RtemsStackchkReqIterate_Post_Argument_Check(
    ctx,
    ctx->Map.entry.Post_Argument
  );
  RtemsStackchkReqIterate_Post_Lock_Check( ctx, ctx->Map.entry.Post_Lock );
}

/**
 * @fn void T_case_body_RtemsStackchkReqIterate( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsStackchkReqIterate,
  &RtemsStackchkReqIterate_Fixture
)
{
  RtemsStackchkReqIterate_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;
  ctx->Map.skip = false;

  for (
    ctx->Map.pcs[ 0 ] = RtemsStackchkReqIterate_Pre_Enabled_Yes;
    ctx->Map.pcs[ 0 ] < RtemsStackchkReqIterate_Pre_Enabled_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = RtemsStackchkReqIterate_Pre_Phase_DeviceDriverInit;
      ctx->Map.pcs[ 1 ] < RtemsStackchkReqIterate_Pre_Phase_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      for (
        ctx->Map.pcs[ 2 ] = RtemsStackchkReqIterate_Pre_Started_Yes;
        ctx->Map.pcs[ 2 ] < RtemsStackchkReqIterate_Pre_Started_NA;
        ++ctx->Map.pcs[ 2 ]
      ) {
        for (
          ctx->Map.pcs[ 3 ] = RtemsStackchkReqIterate_Pre_Owned_Yes;
          ctx->Map.pcs[ 3 ] < RtemsStackchkReqIterate_Pre_Owned_NA;
          ++ctx->Map.pcs[ 3 ]
        ) {
          ctx->Map.entry = RtemsStackchkReqIterate_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          RtemsStackchkReqIterate_Prepare( ctx );
          RtemsStackchkReqIterate_TestVariant( ctx );
          RtemsStackchkReqIterate_Cleanup( ctx );
        }
      }
    }
  }
}

/** @} */
