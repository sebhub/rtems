/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsStackchkReqIsBlown
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
#include <setjmp.h>
#include <string.h>
#include <rtems/stackchk.h>
#include <rtems/score/threaddispatch.h>
#include <rtems/score/threadimpl.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RtemsStackchkReqIsBlown spec:/rtems/stackchk/req/is-blown
 *
 * @ingroup TestsuitesValidation0
 *
 * State Below of a stack which grows down uses a buffer in the stack frame of
 * the action. State Above of a stack which grows up uses the same buffer. The
 * action passes the buffer to a function which it calls. A local variable of
 * that function is therefore beyond the buffer in the direction of stack
 * growth. The case takes the address of this variable as the stack pointer of
 * the caller.
 *
 * @{
 */

typedef enum {
  RtemsStackchkReqIsBlown_Pre_StackPointer_Below,
  RtemsStackchkReqIsBlown_Pre_StackPointer_Inside,
  RtemsStackchkReqIsBlown_Pre_StackPointer_Above,
  RtemsStackchkReqIsBlown_Pre_StackPointer_NA
} RtemsStackchkReqIsBlown_Pre_StackPointer;

typedef enum {
  RtemsStackchkReqIsBlown_Pre_Pattern_Intact,
  RtemsStackchkReqIsBlown_Pre_Pattern_Damaged,
  RtemsStackchkReqIsBlown_Pre_Pattern_NA
} RtemsStackchkReqIsBlown_Pre_Pattern;

typedef enum {
  RtemsStackchkReqIsBlown_Pre_InterruptStackPattern_Intact,
  RtemsStackchkReqIsBlown_Pre_InterruptStackPattern_Damaged,
  RtemsStackchkReqIsBlown_Pre_InterruptStackPattern_NA
} RtemsStackchkReqIsBlown_Pre_InterruptStackPattern;

typedef enum {
  RtemsStackchkReqIsBlown_Post_Result_False,
  RtemsStackchkReqIsBlown_Post_Result_NoReturn,
  RtemsStackchkReqIsBlown_Post_Result_NA
} RtemsStackchkReqIsBlown_Post_Result;

typedef enum {
  RtemsStackchkReqIsBlown_Post_Fatal_Nop,
  RtemsStackchkReqIsBlown_Post_Fatal_StackChecker,
  RtemsStackchkReqIsBlown_Post_Fatal_StackCheckerInterrupt,
  RtemsStackchkReqIsBlown_Post_Fatal_NA
} RtemsStackchkReqIsBlown_Post_Fatal;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_StackPointer_NA : 1;
  uint8_t Pre_Pattern_NA : 1;
  uint8_t Pre_InterruptStackPattern_NA : 1;
  uint8_t Post_Result : 2;
  uint8_t Post_Fatal : 2;
} RtemsStackchkReqIsBlown_Entry;

typedef enum { POSITION_BELOW, POSITION_INSIDE, POSITION_ABOVE } Position;

/**
 * @brief Test context for spec:/rtems/stackchk/req/is-blown test case.
 */
typedef struct {
  /**
   * @brief This member specifies the position of the stack pointer of the
   *   caller relative to the allocated stack area of the executing thread.
   */
  Position position;

  /**
   * @brief This member is true, if the pattern area of the stack of the
   *   executing thread shall be damaged.
   */
  bool damage;

  /**
   * @brief This member is true, if the pattern area of the interrupt stack of
   *   the current processor shall be damaged.
   */
  bool damage_interrupt_stack;

  /**
   * @brief This member contains the processor of the thread dispatch disable.
   */
  Per_CPU_Control *cpu_self;

  /**
   * @brief This member provides the context to return from the fatal error.
   */
  jmp_buf before;

  /**
   * @brief This member is true, if the rtems_stack_checker_is_blown() call
   *   returned.
   */
  bool returned;

  /**
   * @brief This member contains the return value of the
   *   rtems_stack_checker_is_blown() call.
   */
  bool result;

  /**
   * @brief This member contains the count of fatal errors.
   */
  uint32_t fatal_counter;

  /**
   * @brief This member contains the fatal source of the last fatal error.
   */
  rtems_fatal_source fatal_source;

  /**
   * @brief This member contains the fatal code of the last fatal error.
   */
  rtems_fatal_code fatal_code;

  /**
   * @brief This member contains the object name of the executing thread.
   */
  rtems_name name;

  struct {
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
    RtemsStackchkReqIsBlown_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsStackchkReqIsBlown_Context;

static RtemsStackchkReqIsBlown_Context RtemsStackchkReqIsBlown_Instance;

static const char *const RtemsStackchkReqIsBlown_PreDesc_StackPointer[] =
  { "Below", "Inside", "Above", "NA" };

static const char *const RtemsStackchkReqIsBlown_PreDesc_Pattern[] =
  { "Intact", "Damaged", "NA" };

static const char *const
  RtemsStackchkReqIsBlown_PreDesc_InterruptStackPattern[] =
    { "Intact", "Damaged", "NA" };

static const char *const *const RtemsStackchkReqIsBlown_PreDesc[] = {
  RtemsStackchkReqIsBlown_PreDesc_StackPointer,
  RtemsStackchkReqIsBlown_PreDesc_Pattern,
  RtemsStackchkReqIsBlown_PreDesc_InterruptStackPattern,
  NULL
};

typedef RtemsStackchkReqIsBlown_Context Context;

#define BUFFER_SIZE 512

/*
 * The pattern area sits at the end of the allocated stack area in the
 * direction of stack growth.  Each arrangement keeps the content of that end.
 */
static RTEMS_NO_INLINE void ArrangeStackPointer(
  Context       *ctx,
  Stack_Control *stack,
  uint8_t       *buffer,
  uintptr_t      stack_pointer
)
{
  void     *area;
  uintptr_t size;
  bool      copy;

  area = stack->area;
  size = stack->size;
  copy = false;

  switch ( ctx->position ) {
    case POSITION_BELOW:
#if CPU_STACK_GROWS_UP == TRUE
      area = (char *) stack->area + stack->size - BUFFER_SIZE;
#else
      area = buffer;
      copy = true;
#endif
      size = BUFFER_SIZE;
      T_assert_lt_uptr( stack_pointer, (uintptr_t) area );
      break;
    case POSITION_ABOVE:
#if CPU_STACK_GROWS_UP == TRUE
      area = buffer;
      copy = true;
#endif
      size = BUFFER_SIZE;
      T_assert_gt_uptr( stack_pointer, (uintptr_t) area + size );
      break;
    default:
      T_assert_ge_uptr( stack_pointer, (uintptr_t) area );
      T_assert_le_uptr( stack_pointer, (uintptr_t) area + size );
      break;
  }

  if ( copy ) {
#if CPU_STACK_GROWS_UP == TRUE
    memcpy(
      buffer,
      (const char *) stack->area + stack->size - BUFFER_SIZE,
      BUFFER_SIZE
    );
#else
    memcpy( buffer, stack->area, BUFFER_SIZE );
#endif
  }

  stack->area = area;
  stack->size = size;
}

static void Fatal(
  rtems_fatal_source source,
  rtems_fatal_code   code,
  void              *arg
)
{
  Context *ctx;

  ctx = arg;
  ctx->fatal_source = source;
  ctx->fatal_code = code;
  ++ctx->fatal_counter;
  _ISR_Set_level( 0 );
  longjmp( ctx->before, 1 );
}

static RTEMS_NO_INLINE void Act( Context *ctx, uint8_t *buffer )
{
  Thread_Control *executing;
  Stack_Control  *stack;
  Stack_Control   saved;
  uint8_t        *pattern;
  uint8_t        *interrupt_pattern;
  uintptr_t       stack_pointer;

  stack_pointer = (uintptr_t) &stack_pointer;
  executing = _Thread_Get_executing();
  stack = &executing->Start.Initial_stack;
  saved = *stack;

  ArrangeStackPointer( ctx, stack, buffer, stack_pointer );

  #if CPU_STACK_GROWS_UP == TRUE
  pattern = (uint8_t *) stack->area + stack->size - 1;
  #else
  pattern = stack->area;
  #endif

  if ( ctx->damage ) {
    *pattern = (uint8_t) ~*pattern;
  }

  ctx->returned = false;
  ctx->result = true;
  ctx->fatal_counter = 0;
  ctx->cpu_self = _Thread_Dispatch_disable();

  #if CPU_STACK_GROWS_UP == TRUE
  interrupt_pattern = (uint8_t *) ctx->cpu_self->interrupt_stack_high - 1;
  #else
  interrupt_pattern = ctx->cpu_self->interrupt_stack_low;
  #endif

  if ( ctx->damage_interrupt_stack ) {
    *interrupt_pattern = (uint8_t) ~*interrupt_pattern;
  }

  SetFatalHandler( Fatal, ctx );

  if ( setjmp( ctx->before ) == 0 ) {
    ctx->result = rtems_stack_checker_is_blown();
    ctx->returned = true;
  }

  SetFatalHandler( NULL, NULL );

  if ( ctx->damage_interrupt_stack ) {
    *interrupt_pattern = (uint8_t) ~*interrupt_pattern;
  }

  _Thread_Dispatch_enable( ctx->cpu_self );

  if ( ctx->damage ) {
    *pattern = (uint8_t) ~*pattern;
  }

  *stack = saved;
}

static void RtemsStackchkReqIsBlown_Pre_StackPointer_Prepare(
  RtemsStackchkReqIsBlown_Context         *ctx,
  RtemsStackchkReqIsBlown_Pre_StackPointer state
)
{
  switch ( state ) {
    case RtemsStackchkReqIsBlown_Pre_StackPointer_Below: {
      /*
       * While the stack pointer of the caller is below the begin address of
       * the allocated stack area of the executing thread.
       */
      ctx->position = POSITION_BELOW;
      break;
    }

    case RtemsStackchkReqIsBlown_Pre_StackPointer_Inside: {
      /*
       * While the stack pointer of the caller is inside the allocated stack
       * area of the executing thread.
       */
      ctx->position = POSITION_INSIDE;
      break;
    }

    case RtemsStackchkReqIsBlown_Pre_StackPointer_Above: {
      /*
       * While the stack pointer of the caller is above the end address of the
       * allocated stack area of the executing thread.
       */
      ctx->position = POSITION_ABOVE;
      break;
    }

    case RtemsStackchkReqIsBlown_Pre_StackPointer_NA:
      break;
  }
}

static void RtemsStackchkReqIsBlown_Pre_Pattern_Prepare(
  RtemsStackchkReqIsBlown_Context    *ctx,
  RtemsStackchkReqIsBlown_Pre_Pattern state
)
{
  switch ( state ) {
    case RtemsStackchkReqIsBlown_Pre_Pattern_Intact: {
      /*
       * While the stack checker pattern area of the stack of the executing
       * thread holds the pattern.
       */
      ctx->damage = false;
      break;
    }

    case RtemsStackchkReqIsBlown_Pre_Pattern_Damaged: {
      /*
       * While the stack checker pattern area of the stack of the executing
       * thread does not hold the pattern.
       */
      ctx->damage = true;
      break;
    }

    case RtemsStackchkReqIsBlown_Pre_Pattern_NA:
      break;
  }
}

static void RtemsStackchkReqIsBlown_Pre_InterruptStackPattern_Prepare(
  RtemsStackchkReqIsBlown_Context                  *ctx,
  RtemsStackchkReqIsBlown_Pre_InterruptStackPattern state
)
{
  switch ( state ) {
    case RtemsStackchkReqIsBlown_Pre_InterruptStackPattern_Intact: {
      /*
       * While the stack checker pattern area of the interrupt stack of the
       * current processor holds the pattern.
       */
      ctx->damage_interrupt_stack = false;
      break;
    }

    case RtemsStackchkReqIsBlown_Pre_InterruptStackPattern_Damaged: {
      /*
       * While the stack checker pattern area of the interrupt stack of the
       * current processor does not hold the pattern.
       */
      ctx->damage_interrupt_stack = true;
      break;
    }

    case RtemsStackchkReqIsBlown_Pre_InterruptStackPattern_NA:
      break;
  }
}

static void RtemsStackchkReqIsBlown_Post_Result_Check(
  RtemsStackchkReqIsBlown_Context    *ctx,
  RtemsStackchkReqIsBlown_Post_Result state
)
{
  switch ( state ) {
    case RtemsStackchkReqIsBlown_Post_Result_False: {
      /*
       * The return value of rtems_stack_checker_is_blown() shall be false.
       */
      T_true( ctx->returned );
      T_false( ctx->result );
      break;
    }

    case RtemsStackchkReqIsBlown_Post_Result_NoReturn: {
      /*
       * The rtems_stack_checker_is_blown() call shall not return to the
       * caller.
       */
      T_false( ctx->returned );
      break;
    }

    case RtemsStackchkReqIsBlown_Post_Result_NA:
      break;
  }
}

static void RtemsStackchkReqIsBlown_Post_Fatal_Check(
  RtemsStackchkReqIsBlown_Context   *ctx,
  RtemsStackchkReqIsBlown_Post_Fatal state
)
{
  switch ( state ) {
    case RtemsStackchkReqIsBlown_Post_Fatal_Nop: {
      /*
       * The rtems_stack_checker_is_blown() call shall raise no fatal error.
       */
      T_eq_u32( ctx->fatal_counter, 0 );
      break;
    }

    case RtemsStackchkReqIsBlown_Post_Fatal_StackChecker: {
      /*
       * The rtems_stack_checker_is_blown() call shall raise the fatal error
       * with the fatal source RTEMS_FATAL_SOURCE_STACK_CHECKER and the object
       * name of the executing thread as the fatal code.
       */
      T_eq_u32( ctx->fatal_counter, 1 );
      T_eq_int( ctx->fatal_source, RTEMS_FATAL_SOURCE_STACK_CHECKER );
      T_eq_ulong( ctx->fatal_code, ctx->name );
      break;
    }

    case RtemsStackchkReqIsBlown_Post_Fatal_StackCheckerInterrupt: {
      /*
       * The rtems_stack_checker_is_blown() call shall raise the fatal error
       * with the fatal source RTEMS_FATAL_SOURCE_STACK_CHECKER and the object
       * name built from the characters `INTR` as the fatal code.
       */
      T_eq_u32( ctx->fatal_counter, 1 );
      T_eq_int( ctx->fatal_source, RTEMS_FATAL_SOURCE_STACK_CHECKER );
      T_eq_ulong( ctx->fatal_code, rtems_build_name( 'I', 'N', 'T', 'R' ) );
      break;
    }

    case RtemsStackchkReqIsBlown_Post_Fatal_NA:
      break;
  }
}

static void RtemsStackchkReqIsBlown_Setup(
  RtemsStackchkReqIsBlown_Context *ctx
)
{
  ctx->name = _Thread_Get_executing()->Object.name.name_u32;
}

static void RtemsStackchkReqIsBlown_Setup_Wrap( void *arg )
{
  RtemsStackchkReqIsBlown_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsStackchkReqIsBlown_Setup( ctx );
}

static void RtemsStackchkReqIsBlown_Action(
  RtemsStackchkReqIsBlown_Context *ctx
)
{
  uint8_t buffer[ BUFFER_SIZE ] RTEMS_ALIGNED( CPU_STACK_ALIGNMENT );

  Act( ctx, buffer );
}

/* clang-format off */

static const RtemsStackchkReqIsBlown_Entry
RtemsStackchkReqIsBlown_Entries[] = {
  { 0, 0, 0, 0, RtemsStackchkReqIsBlown_Post_Result_NoReturn,
    RtemsStackchkReqIsBlown_Post_Fatal_StackChecker },
  { 0, 0, 0, 0, RtemsStackchkReqIsBlown_Post_Result_False,
    RtemsStackchkReqIsBlown_Post_Fatal_Nop },
  { 0, 0, 0, 0, RtemsStackchkReqIsBlown_Post_Result_NoReturn,
    RtemsStackchkReqIsBlown_Post_Fatal_StackCheckerInterrupt }
};

static const uint8_t
RtemsStackchkReqIsBlown_Map[] = {
  0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0
};

/* clang-format on */

static size_t RtemsStackchkReqIsBlown_Scope( void *arg, char *buf, size_t n )
{
  RtemsStackchkReqIsBlown_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      RtemsStackchkReqIsBlown_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture RtemsStackchkReqIsBlown_Fixture = {
  .setup = RtemsStackchkReqIsBlown_Setup_Wrap,
  .stop = NULL,
  .teardown = NULL,
  .scope = RtemsStackchkReqIsBlown_Scope,
  .initial_context = &RtemsStackchkReqIsBlown_Instance
};

static inline RtemsStackchkReqIsBlown_Entry RtemsStackchkReqIsBlown_PopEntry(
  RtemsStackchkReqIsBlown_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsStackchkReqIsBlown_Entries
    [ RtemsStackchkReqIsBlown_Map[ index ] ];
}

static void RtemsStackchkReqIsBlown_TestVariant(
  RtemsStackchkReqIsBlown_Context *ctx
)
{
  RtemsStackchkReqIsBlown_Pre_StackPointer_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  RtemsStackchkReqIsBlown_Pre_Pattern_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  RtemsStackchkReqIsBlown_Pre_InterruptStackPattern_Prepare(
    ctx,
    ctx->Map.pcs[ 2 ]
  );
  RtemsStackchkReqIsBlown_Action( ctx );
  RtemsStackchkReqIsBlown_Post_Result_Check( ctx, ctx->Map.entry.Post_Result );
  RtemsStackchkReqIsBlown_Post_Fatal_Check( ctx, ctx->Map.entry.Post_Fatal );
}

/**
 * @fn void T_case_body_RtemsStackchkReqIsBlown( void )
 */
T_TEST_CASE_FIXTURE(
  RtemsStackchkReqIsBlown,
  &RtemsStackchkReqIsBlown_Fixture
)
{
  RtemsStackchkReqIsBlown_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = RtemsStackchkReqIsBlown_Pre_StackPointer_Below;
    ctx->Map.pcs[ 0 ] < RtemsStackchkReqIsBlown_Pre_StackPointer_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = RtemsStackchkReqIsBlown_Pre_Pattern_Intact;
      ctx->Map.pcs[ 1 ] < RtemsStackchkReqIsBlown_Pre_Pattern_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      for (
        ctx->Map.pcs[ 2 ] =
          RtemsStackchkReqIsBlown_Pre_InterruptStackPattern_Intact;
        ctx->Map.pcs[ 2 ] <
        RtemsStackchkReqIsBlown_Pre_InterruptStackPattern_NA;
        ++ctx->Map.pcs[ 2 ]
      ) {
        ctx->Map.entry = RtemsStackchkReqIsBlown_PopEntry( ctx );
        RtemsStackchkReqIsBlown_TestVariant( ctx );
      }
    }
  }
}

/** @} */
