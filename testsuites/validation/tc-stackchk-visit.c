/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsStackchkReqVisit
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
#include <rtems/score/percpu.h>
#include <rtems/score/threadimpl.h>
#include <rtems/score/userextdata.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RtemsStackchkReqVisit spec:/rtems/stackchk/req/visit
 *
 * @ingroup TestsuitesValidation0
 * @ingroup TestsuitesValidationAcfg1
 *
 * The configuration of the test suite decides whether it enables the stack
 * checker, so the test skips the variants of the other state.
 *
 * The processor of some architectures serves interrupts on the stack of the
 * interrupted thread and never executes on its interrupt stack. The test
 * therefore establishes the execution on the interrupt stack of processor 0
 * itself. It writes zero to the word of the interrupt stack which the
 * processor uses first, if this word still holds the fill pattern of the stack
 * checker.
 *
 * The member of the stack pointer in a saved context depends on the
 * architecture and on some architectures also on the address of the context.
 * The test locates the member in the context of the test task.
 *
 * @{
 */

typedef enum {
  RtemsStackchkReqVisit_Pre_Enabled_Yes,
  RtemsStackchkReqVisit_Pre_Enabled_No,
  RtemsStackchkReqVisit_Pre_Enabled_NA
} RtemsStackchkReqVisit_Pre_Enabled;

typedef enum {
  RtemsStackchkReqVisit_Pre_Kind_Thread,
  RtemsStackchkReqVisit_Pre_Kind_ISR,
  RtemsStackchkReqVisit_Pre_Kind_NA
} RtemsStackchkReqVisit_Pre_Kind;

typedef enum {
  RtemsStackchkReqVisit_Pre_HasExecuted_Yes,
  RtemsStackchkReqVisit_Pre_HasExecuted_No,
  RtemsStackchkReqVisit_Pre_HasExecuted_NA
} RtemsStackchkReqVisit_Pre_HasExecuted;

typedef enum {
  RtemsStackchkReqVisit_Pre_StackPointer_Below,
  RtemsStackchkReqVisit_Pre_StackPointer_Inside,
  RtemsStackchkReqVisit_Pre_StackPointer_Above,
  RtemsStackchkReqVisit_Pre_StackPointer_NA
} RtemsStackchkReqVisit_Pre_StackPointer;

typedef enum {
  RtemsStackchkReqVisit_Post_Id_Thread,
  RtemsStackchkReqVisit_Post_Id_Processor,
  RtemsStackchkReqVisit_Post_Id_NA
} RtemsStackchkReqVisit_Post_Id;

typedef enum {
  RtemsStackchkReqVisit_Post_Name_Thread,
  RtemsStackchkReqVisit_Post_Name_Interrupt,
  RtemsStackchkReqVisit_Post_Name_NA
} RtemsStackchkReqVisit_Post_Name;

typedef enum {
  RtemsStackchkReqVisit_Post_Area_Allocated,
  RtemsStackchkReqVisit_Post_Area_NA
} RtemsStackchkReqVisit_Post_Area;

typedef enum {
  RtemsStackchkReqVisit_Post_StackPointer_Context,
  RtemsStackchkReqVisit_Post_StackPointer_Null,
  RtemsStackchkReqVisit_Post_StackPointer_NA
} RtemsStackchkReqVisit_Post_StackPointer;

typedef enum {
  RtemsStackchkReqVisit_Post_Used_Zero,
  RtemsStackchkReqVisit_Post_Used_InRange,
  RtemsStackchkReqVisit_Post_Used_Unknown,
  RtemsStackchkReqVisit_Post_Used_NA
} RtemsStackchkReqVisit_Post_Used;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Enabled_NA : 1;
  uint16_t Pre_Kind_NA : 1;
  uint16_t Pre_HasExecuted_NA : 1;
  uint16_t Pre_StackPointer_NA : 1;
  uint16_t Post_Id : 2;
  uint16_t Post_Name : 2;
  uint16_t Post_Area : 1;
  uint16_t Post_StackPointer : 2;
  uint16_t Post_Used : 2;
} RtemsStackchkReqVisit_Entry;

typedef enum { KIND_THREAD, KIND_ISR } Kind;

typedef enum { POSITION_BELOW, POSITION_INSIDE, POSITION_ABOVE } Position;

/**
 * @brief Test context for spec:/rtems/stackchk/req/visit test case.
 */
typedef struct {
  /**
   * @brief This member specifies the kind of the visited stack.
   */
  Kind kind;

  /**
   * @brief This member is true, if the test task shall execute.
   */
  bool execute;

  /**
   * @brief This member specifies the position of the stack pointer of the
   *   saved context of the test task.
   */
  Position position;

  /**
   * @brief This member is true, if the test task executed.
   */
  volatile bool executed;

  /**
   * @brief This member contains the identifier of the test task.
   */
  rtems_id task_id;

  /**
   * @brief This member contains the begin address of the allocated stack area
   *   of the visited stack.
   */
  uintptr_t area_begin;

  /**
   * @brief This member contains the end address of the allocated stack area of
   *   the visited stack.
   */
  uintptr_t area_end;

  /**
   * @brief This member contains the count of visits of the selected stack.
   */
  uint32_t visits;

  /**
   * @brief This member contains the stack information of the selected stack.
   */
  rtems_stack_checker_info info;

  /**
   * @brief This member contains a copy of the name in the stack information.
   */
  char name[ 16 ];

  /**
   * @brief This member contains the stack pointer of the saved context of the
   *   test task during the call.
   */
  uintptr_t stack_pointer;

  /**
   * @brief This member contains the stack pointer of the saved context of the
   *   test task before the arrangement.
   */
  uintptr_t saved_stack_pointer;

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
    RtemsStackchkReqVisit_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsStackchkReqVisit_Context;

static RtemsStackchkReqVisit_Context RtemsStackchkReqVisit_Instance;

static const char *const RtemsStackchkReqVisit_PreDesc_Enabled[] =
  { "Yes", "No", "NA" };

static const char *const RtemsStackchkReqVisit_PreDesc_Kind[] =
  { "Thread", "ISR", "NA" };

static const char *const RtemsStackchkReqVisit_PreDesc_HasExecuted[] =
  { "Yes", "No", "NA" };

static const char *const RtemsStackchkReqVisit_PreDesc_StackPointer[] =
  { "Below", "Inside", "Above", "NA" };

static const char *const *const RtemsStackchkReqVisit_PreDesc[] = {
  RtemsStackchkReqVisit_PreDesc_Enabled,
  RtemsStackchkReqVisit_PreDesc_Kind,
  RtemsStackchkReqVisit_PreDesc_HasExecuted,
  RtemsStackchkReqVisit_PreDesc_StackPointer,
  NULL
};

typedef RtemsStackchkReqVisit_Context Context;

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

static void Worker( rtems_task_argument arg )
{
  Context *ctx;

  ctx = (Context *) arg;
  ctx->executed = true;

  while ( true ) {
    (void) ReceiveAnyEvents();
  }
}

static void SetSavedStackPointer( Context *ctx, uintptr_t stack_pointer )
{
  Context_Control *context;
  Context_Control  saved;
  size_t           i;

  context = &GetThread( ctx->task_id )->Registers;
  saved = *context;

  /*
   * The member of the stack pointer in the context depends on the
   * architecture and on some architectures also on the address of the
   * context.  Each byte of the context which _CPU_Context_Get_SP() reads
   * receives the corresponding byte of the stack pointer.
   */
  for ( i = 0; i < sizeof( *context ); ++i ) {
    uintptr_t value;
    size_t    shift;

    memset( context, 0, sizeof( *context ) );
    ( (uint8_t *) context )[ i ] = 0xff;
    value = _CPU_Context_Get_SP( context );

    for ( shift = 0; shift < 8 * sizeof( value ); shift += 8 ) {
      if ( ( ( value >> shift ) & 0xff ) == 0xff ) {
        ( (uint8_t *) &saved )[ i ] = (uint8_t) ( stack_pointer >> shift );
        break;
      }
    }
  }

  *context = saved;
}

/* The stack checker fills each stack with this value */
#define STACK_FILL_PATTERN 0xa5a5a5a5U

static void ArrangeInterruptStackUse( void )
{
  Per_CPU_Control      *cpu;
  uint32_t             *word;
  rtems_interrupt_level level;

  cpu = _Per_CPU_Get_by_index( 0 );
#if CPU_STACK_GROWS_UP == TRUE
  word = (uint32_t *) cpu->interrupt_stack_low + 1;
#else
  word = (uint32_t *) cpu->interrupt_stack_high - 1;
#endif

  rtems_interrupt_local_disable( level );

  if ( *word == STACK_FILL_PATTERN ) {
    *word = 0;
  }

  rtems_interrupt_local_enable( level );
}

/*
 * The test task does not execute until the action restores the stack pointer
 * of its saved context.
 */
static void ArrangeStackPointer( Context *ctx )
{
  uintptr_t stack_pointer;

  ctx->saved_stack_pointer = (uintptr_t) _CPU_Context_Get_SP(
    &GetThread( ctx->task_id )->Registers
  );

  switch ( ctx->position ) {
    case POSITION_BELOW:
      stack_pointer = ctx->area_begin - CPU_STACK_ALIGNMENT;
      break;
    case POSITION_ABOVE:
      stack_pointer = ctx->area_end + CPU_STACK_ALIGNMENT;
      break;
    default:
      stack_pointer = RTEMS_ALIGN_DOWN(
        ctx->area_begin + ( ctx->area_end - ctx->area_begin ) / 2,
        CPU_STACK_ALIGNMENT
      );
      break;
  }

  SetSavedStackPointer( ctx, stack_pointer );
  ctx->stack_pointer = stack_pointer;
}

static void Visit( const rtems_stack_checker_info *info, void *arg )
{
  Context *ctx;
  bool     selected;

  ctx = arg;

  if ( ctx->kind == KIND_THREAD ) {
    selected = info->id == ctx->task_id;
  } else {
    /* The identifier of an interrupt stack is a processor index */
    selected = rtems_object_id_get_api( info->id ) == 0 && info->id == 0;
  }

  if ( selected ) {
    ++ctx->visits;
    ctx->info = *info;
    strlcpy( ctx->name, info->name, sizeof( ctx->name ) );
  }
}

static void RtemsStackchkReqVisit_Pre_Enabled_Prepare(
  RtemsStackchkReqVisit_Context    *ctx,
  RtemsStackchkReqVisit_Pre_Enabled state
)
{
  switch ( state ) {
    case RtemsStackchkReqVisit_Pre_Enabled_Yes: {
      /*
       * Where the application configuration enables the stack checker.
       */
      if ( !IsStackCheckerEnabled() ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case RtemsStackchkReqVisit_Pre_Enabled_No: {
      /*
       * Where the application configuration does not enable the stack checker.
       */
      if ( IsStackCheckerEnabled() ) {
        ctx->Map.skip = true;
      }
      break;
    }

    case RtemsStackchkReqVisit_Pre_Enabled_NA:
      break;
  }
}

static void RtemsStackchkReqVisit_Pre_Kind_Prepare(
  RtemsStackchkReqVisit_Context *ctx,
  RtemsStackchkReqVisit_Pre_Kind state
)
{
  switch ( state ) {
    case RtemsStackchkReqVisit_Pre_Kind_Thread: {
      /*
       * While the stack is the stack of a thread.
       */
      ctx->kind = KIND_THREAD;
      break;
    }

    case RtemsStackchkReqVisit_Pre_Kind_ISR: {
      /*
       * While the stack is the interrupt stack of a processor.
       */
      ctx->kind = KIND_ISR;
      break;
    }

    case RtemsStackchkReqVisit_Pre_Kind_NA:
      break;
  }
}

static void RtemsStackchkReqVisit_Pre_HasExecuted_Prepare(
  RtemsStackchkReqVisit_Context        *ctx,
  RtemsStackchkReqVisit_Pre_HasExecuted state
)
{
  switch ( state ) {
    case RtemsStackchkReqVisit_Pre_HasExecuted_Yes: {
      /*
       * While the owner of the stack executed on the stack.
       */
      ctx->execute = true;
      break;
    }

    case RtemsStackchkReqVisit_Pre_HasExecuted_No: {
      /*
       * While the owner of the stack never executed on the stack.
       */
      ctx->execute = false;
      break;
    }

    case RtemsStackchkReqVisit_Pre_HasExecuted_NA:
      break;
  }
}

static void RtemsStackchkReqVisit_Pre_StackPointer_Prepare(
  RtemsStackchkReqVisit_Context         *ctx,
  RtemsStackchkReqVisit_Pre_StackPointer state
)
{
  switch ( state ) {
    case RtemsStackchkReqVisit_Pre_StackPointer_Below: {
      /*
       * While the stack pointer of the saved context of the thread is below
       * the begin address of the allocated stack area of the thread.
       */
      ctx->position = POSITION_BELOW;
      break;
    }

    case RtemsStackchkReqVisit_Pre_StackPointer_Inside: {
      /*
       * While the stack pointer of the saved context of the thread is inside
       * the allocated stack area of the thread.
       */
      ctx->position = POSITION_INSIDE;
      break;
    }

    case RtemsStackchkReqVisit_Pre_StackPointer_Above: {
      /*
       * While the stack pointer of the saved context of the thread is above
       * the end address of the allocated stack area of the thread.
       */
      ctx->position = POSITION_ABOVE;
      break;
    }

    case RtemsStackchkReqVisit_Pre_StackPointer_NA:
      break;
  }
}

static void RtemsStackchkReqVisit_Post_Id_Check(
  RtemsStackchkReqVisit_Context *ctx,
  RtemsStackchkReqVisit_Post_Id  state
)
{
  switch ( state ) {
    case RtemsStackchkReqVisit_Post_Id_Thread: {
      /*
       * The id member of the stack information shall be the object identifier
       * of the thread.
       */
      T_eq_u32( ctx->info.id, ctx->task_id );
      break;
    }

    case RtemsStackchkReqVisit_Post_Id_Processor: {
      /*
       * The id member of the stack information shall be the index of the
       * processor of the interrupt stack.
       */
      T_eq_u32( ctx->info.id, 0 );
      break;
    }

    case RtemsStackchkReqVisit_Post_Id_NA:
      break;
  }
}

static void RtemsStackchkReqVisit_Post_Name_Check(
  RtemsStackchkReqVisit_Context  *ctx,
  RtemsStackchkReqVisit_Post_Name state
)
{
  switch ( state ) {
    case RtemsStackchkReqVisit_Post_Name_Thread: {
      /*
       * The name member of the stack information shall reference the name of
       * the thread.
       */
      T_eq_str( ctx->name, "STCK" );
      break;
    }

    case RtemsStackchkReqVisit_Post_Name_Interrupt: {
      /*
       * The name member of the stack information shall reference the string
       * `Interrupt Stack`.
       */
      T_eq_str( ctx->name, "Interrupt Stack" );
      break;
    }

    case RtemsStackchkReqVisit_Post_Name_NA:
      break;
  }
}

static void RtemsStackchkReqVisit_Post_Area_Check(
  RtemsStackchkReqVisit_Context  *ctx,
  RtemsStackchkReqVisit_Post_Area state
)
{
  switch ( state ) {
    case RtemsStackchkReqVisit_Post_Area_Allocated: {
      /*
       * The begin and size members of the stack information shall reference
       * the allocated stack area of the stack without the stack checker
       * pattern area.
       */
      uintptr_t begin;
      uintptr_t end;

      begin = (uintptr_t) ctx->info.begin;
      end = begin + ctx->info.size;
      #if CPU_STACK_GROWS_UP == TRUE
      T_eq_uptr( begin, ctx->area_begin );
      T_lt_uptr( end, ctx->area_end );
      #else
      T_gt_uptr( begin, ctx->area_begin );
      T_eq_uptr( end, ctx->area_end );
      #endif
      break;
    }

    case RtemsStackchkReqVisit_Post_Area_NA:
      break;
  }
}

static void RtemsStackchkReqVisit_Post_StackPointer_Check(
  RtemsStackchkReqVisit_Context          *ctx,
  RtemsStackchkReqVisit_Post_StackPointer state
)
{
  switch ( state ) {
    case RtemsStackchkReqVisit_Post_StackPointer_Context: {
      /*
       * The current member of the stack information shall be the stack pointer
       * of the saved context of the thread.
       */
      T_eq_uptr( (uintptr_t) ctx->info.current, ctx->stack_pointer );
      break;
    }

    case RtemsStackchkReqVisit_Post_StackPointer_Null: {
      /*
       * The current member of the stack information shall be NULL.
       */
      T_null( ctx->info.current );
      break;
    }

    case RtemsStackchkReqVisit_Post_StackPointer_NA:
      break;
  }
}

static void RtemsStackchkReqVisit_Post_Used_Check(
  RtemsStackchkReqVisit_Context  *ctx,
  RtemsStackchkReqVisit_Post_Used state
)
{
  switch ( state ) {
    case RtemsStackchkReqVisit_Post_Used_Zero: {
      /*
       * The used member of the stack information shall be zero.
       */
      T_eq_uptr( ctx->info.used, 0 );
      break;
    }

    case RtemsStackchkReqVisit_Post_Used_InRange: {
      /*
       * The used member of the stack information shall be greater than zero
       * and less than or equal to the size member.
       */
      T_gt_uptr( ctx->info.used, 0 );
      T_le_uptr( ctx->info.used, ctx->info.size );
      break;
    }

    case RtemsStackchkReqVisit_Post_Used_Unknown: {
      /*
       * The used member of the stack information shall be UINTPTR_MAX.
       */
      T_eq_uptr( ctx->info.used, UINTPTR_MAX );
      break;
    }

    case RtemsStackchkReqVisit_Post_Used_NA:
      break;
  }
}

static void RtemsStackchkReqVisit_Setup( void )
{
  SetSelfPriority( PRIO_NORMAL );
}

static void RtemsStackchkReqVisit_Setup_Wrap( void *arg )
{
  RtemsStackchkReqVisit_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsStackchkReqVisit_Setup();
}

static void RtemsStackchkReqVisit_Teardown( void )
{
  RestoreRunnerPriority();
}

static void RtemsStackchkReqVisit_Teardown_Wrap( void *arg )
{
  RtemsStackchkReqVisit_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsStackchkReqVisit_Teardown();
}

static void RtemsStackchkReqVisit_Prepare( RtemsStackchkReqVisit_Context *ctx )
{
  ctx->task_id = 0;
}

static void RtemsStackchkReqVisit_Action( RtemsStackchkReqVisit_Context *ctx )
{
  ctx->executed = false;
  ctx->task_id = CreateTask( "STCK", PRIO_HIGH );

  if ( ctx->kind == KIND_THREAD ) {
    const Stack_Control *stack;

    stack = &GetThread( ctx->task_id )->Start.Initial_stack;
    ctx->area_begin = (uintptr_t) stack->area;
    ctx->area_end = ctx->area_begin + stack->size;

    if ( ctx->execute ) {
      StartTask( ctx->task_id, Worker, ctx );

      while ( !ctx->executed ) {
        /* Wait */
      }
    }

    ArrangeStackPointer( ctx );
  } else {
    const Per_CPU_Control *cpu;

    cpu = _Per_CPU_Get_by_index( 0 );
    ctx->area_begin = (uintptr_t) cpu->interrupt_stack_low;
    ctx->area_end = (uintptr_t) cpu->interrupt_stack_high;

    if ( ctx->execute ) {
      ArrangeInterruptStackUse();
    }
  }

  ctx->visits = 0;
  rtems_stack_checker_iterate( Visit, ctx );
  T_eq_u32( ctx->visits, 1 );

  if ( ctx->kind == KIND_THREAD ) {
    SetSavedStackPointer( ctx, ctx->saved_stack_pointer );
  }
}

static void RtemsStackchkReqVisit_Cleanup( RtemsStackchkReqVisit_Context *ctx )
{
  DeleteTask( ctx->task_id );
}

/* clang-format off */

static const RtemsStackchkReqVisit_Entry
RtemsStackchkReqVisit_Entries[] = {
  { 0, 0, 0, 0, 0, RtemsStackchkReqVisit_Post_Id_Thread,
    RtemsStackchkReqVisit_Post_Name_Thread,
    RtemsStackchkReqVisit_Post_Area_Allocated,
    RtemsStackchkReqVisit_Post_StackPointer_Context,
    RtemsStackchkReqVisit_Post_Used_Unknown },
  { 1, 0, 0, 0, 1, RtemsStackchkReqVisit_Post_Id_NA,
    RtemsStackchkReqVisit_Post_Name_NA, RtemsStackchkReqVisit_Post_Area_NA,
    RtemsStackchkReqVisit_Post_StackPointer_NA,
    RtemsStackchkReqVisit_Post_Used_NA },
  { 0, 0, 0, 0, 0, RtemsStackchkReqVisit_Post_Id_Thread,
    RtemsStackchkReqVisit_Post_Name_Thread,
    RtemsStackchkReqVisit_Post_Area_Allocated,
    RtemsStackchkReqVisit_Post_StackPointer_Context,
    RtemsStackchkReqVisit_Post_Used_InRange },
  { 0, 0, 0, 0, 0, RtemsStackchkReqVisit_Post_Id_Thread,
    RtemsStackchkReqVisit_Post_Name_Thread,
    RtemsStackchkReqVisit_Post_Area_Allocated,
    RtemsStackchkReqVisit_Post_StackPointer_Context,
    RtemsStackchkReqVisit_Post_Used_Zero },
  { 1, 0, 0, 0, 1, RtemsStackchkReqVisit_Post_Id_NA,
    RtemsStackchkReqVisit_Post_Name_NA, RtemsStackchkReqVisit_Post_Area_NA,
    RtemsStackchkReqVisit_Post_StackPointer_NA,
    RtemsStackchkReqVisit_Post_Used_NA },
  { 1, 0, 0, 0, 0, RtemsStackchkReqVisit_Post_Id_NA,
    RtemsStackchkReqVisit_Post_Name_NA, RtemsStackchkReqVisit_Post_Area_NA,
    RtemsStackchkReqVisit_Post_StackPointer_NA,
    RtemsStackchkReqVisit_Post_Used_NA },
  { 0, 0, 0, 0, 1, RtemsStackchkReqVisit_Post_Id_Processor,
    RtemsStackchkReqVisit_Post_Name_Interrupt,
    RtemsStackchkReqVisit_Post_Area_Allocated,
    RtemsStackchkReqVisit_Post_StackPointer_Null,
    RtemsStackchkReqVisit_Post_Used_InRange }
};

static const uint8_t
RtemsStackchkReqVisit_Map[] = {
  2, 2, 2, 3, 3, 3, 6, 5, 5, 4, 4, 4, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1
};

/* clang-format on */

static size_t RtemsStackchkReqVisit_Scope( void *arg, char *buf, size_t n )
{
  RtemsStackchkReqVisit_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( RtemsStackchkReqVisit_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture RtemsStackchkReqVisit_Fixture = {
  .setup = RtemsStackchkReqVisit_Setup_Wrap,
  .stop = NULL,
  .teardown = RtemsStackchkReqVisit_Teardown_Wrap,
  .scope = RtemsStackchkReqVisit_Scope,
  .initial_context = &RtemsStackchkReqVisit_Instance
};

static const uint8_t RtemsStackchkReqVisit_Weights[] = { 12, 6, 3, 1 };

static void RtemsStackchkReqVisit_Skip(
  RtemsStackchkReqVisit_Context *ctx,
  size_t                         index
)
{
  switch ( index + 1 ) {
    case 1:
      ctx->Map.pci[ 1 ] = RtemsStackchkReqVisit_Pre_Kind_NA - 1;
      /* Fall through */
    case 2:
      ctx->Map.pci[ 2 ] = RtemsStackchkReqVisit_Pre_HasExecuted_NA - 1;
      /* Fall through */
    case 3:
      ctx->Map.pci[ 3 ] = RtemsStackchkReqVisit_Pre_StackPointer_NA - 1;
      break;
  }
}

static inline RtemsStackchkReqVisit_Entry RtemsStackchkReqVisit_PopEntry(
  RtemsStackchkReqVisit_Context *ctx
)
{
  size_t index;

  if ( ctx->Map.skip ) {
    size_t i;

    ctx->Map.skip = false;
    index = 0;

    for ( i = 0; i < 4; ++i ) {
      index += RtemsStackchkReqVisit_Weights[ i ] * ctx->Map.pci[ i ];
    }
  } else {
    index = ctx->Map.index;
  }

  ctx->Map.index = index + 1;

  return RtemsStackchkReqVisit_Entries[ RtemsStackchkReqVisit_Map[ index ] ];
}

static void RtemsStackchkReqVisit_SetPreConditionStates(
  RtemsStackchkReqVisit_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];

  if ( ctx->Map.entry.Pre_StackPointer_NA ) {
    ctx->Map.pcs[ 3 ] = RtemsStackchkReqVisit_Pre_StackPointer_NA;
  } else {
    ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
  }
}

static void RtemsStackchkReqVisit_TestVariant(
  RtemsStackchkReqVisit_Context *ctx
)
{
  RtemsStackchkReqVisit_Pre_Enabled_Prepare( ctx, ctx->Map.pcs[ 0 ] );

  if ( ctx->Map.skip ) {
    RtemsStackchkReqVisit_Skip( ctx, 0 );
    return;
  }

  RtemsStackchkReqVisit_Pre_Kind_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  RtemsStackchkReqVisit_Pre_HasExecuted_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  RtemsStackchkReqVisit_Pre_StackPointer_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  RtemsStackchkReqVisit_Action( ctx );
  RtemsStackchkReqVisit_Post_Id_Check( ctx, ctx->Map.entry.Post_Id );
  RtemsStackchkReqVisit_Post_Name_Check( ctx, ctx->Map.entry.Post_Name );
  RtemsStackchkReqVisit_Post_Area_Check( ctx, ctx->Map.entry.Post_Area );
  RtemsStackchkReqVisit_Post_StackPointer_Check(
    ctx,
    ctx->Map.entry.Post_StackPointer
  );
  RtemsStackchkReqVisit_Post_Used_Check( ctx, ctx->Map.entry.Post_Used );
}

/**
 * @fn void T_case_body_RtemsStackchkReqVisit( void )
 */
T_TEST_CASE_FIXTURE( RtemsStackchkReqVisit, &RtemsStackchkReqVisit_Fixture )
{
  RtemsStackchkReqVisit_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;
  ctx->Map.skip = false;

  for (
    ctx->Map.pci[ 0 ] = RtemsStackchkReqVisit_Pre_Enabled_Yes;
    ctx->Map.pci[ 0 ] < RtemsStackchkReqVisit_Pre_Enabled_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = RtemsStackchkReqVisit_Pre_Kind_Thread;
      ctx->Map.pci[ 1 ] < RtemsStackchkReqVisit_Pre_Kind_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = RtemsStackchkReqVisit_Pre_HasExecuted_Yes;
        ctx->Map.pci[ 2 ] < RtemsStackchkReqVisit_Pre_HasExecuted_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = RtemsStackchkReqVisit_Pre_StackPointer_Below;
          ctx->Map.pci[ 3 ] < RtemsStackchkReqVisit_Pre_StackPointer_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = RtemsStackchkReqVisit_PopEntry( ctx );

          if ( ctx->Map.entry.Skip ) {
            continue;
          }

          RtemsStackchkReqVisit_SetPreConditionStates( ctx );
          RtemsStackchkReqVisit_Prepare( ctx );
          RtemsStackchkReqVisit_TestVariant( ctx );
          RtemsStackchkReqVisit_Cleanup( ctx );
        }
      }
    }
  }
}

/** @} */
