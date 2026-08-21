/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuArmValContext
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

#include <stdint.h>
#include <string.h>
#include <rtems/score/cpuimpl.h>
#include <rtems/score/percpu.h>
#include <rtems/score/thread.h>

#include "tx-register-check.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuArmValContext spec:/score/cpu/arm/val/context
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that the context switch saves and restores every call-saved
 *   register.
 *
 * The action calls the switch directly. A call through the operating system
 * runs a chain of functions. A function of the chain may preserve a register
 * on its own stack. This hides a defect of the switch in that register.
 *
 * This test case performs the following actions:
 *
 * - Load a distinct pattern into every call-saved register and into FPSCR.
 *   Give the heir its own thread pointer and FPSCR. Call the context switch
 *   directly. The heir switches back. Trap, so the frame reports every
 *   register. Run this once for each register with the pattern of that
 *   register inverted, then once unchanged.
 *
 *   - Check that the switch restored the ARM core register R4.
 *
 *   - Check that the switch restored the ARM core register R5.
 *
 *   - Check that the switch restored the ARM core register R6.
 *
 *   - Check that the switch restored the ARM core register R7.
 *
 *   - Check that the switch restored the ARM core register R8.
 *
 *   - Check that the switch restored the ARM core register R9.
 *
 *   - Check that the switch restored the ARM core register R10.
 *
 *   - Check that the switch restored the ARM core register R11.
 *
 *   - Check that the switch restored the ARM FPU register D8.
 *
 *   - Check that the switch restored the ARM FPU register D9.
 *
 *   - Check that the switch restored the ARM FPU register D10.
 *
 *   - Check that the switch restored the ARM FPU register D11.
 *
 *   - Check that the switch restored the ARM FPU register D12.
 *
 *   - Check that the switch restored the ARM FPU register D13.
 *
 *   - Check that the switch restored the ARM FPU register D14.
 *
 *   - Check that the switch restored the ARM FPU register D15.
 *
 *   - Check that the switch saved the ARM core register R4.
 *
 *   - Check that the switch saved the ARM core register R5.
 *
 *   - Check that the switch saved the ARM core register R6.
 *
 *   - Check that the switch saved the ARM core register R7.
 *
 *   - Check that the switch saved the ARM core register R8.
 *
 *   - Check that the switch saved the ARM core register R9.
 *
 *   - Check that the switch saved the ARM core register R10.
 *
 *   - Check that the switch saved the ARM core register R11.
 *
 *   - Check that the switch saved the ARM FPU register D8.
 *
 *   - Check that the switch saved the ARM FPU register D9.
 *
 *   - Check that the switch saved the ARM FPU register D10.
 *
 *   - Check that the switch saved the ARM FPU register D11.
 *
 *   - Check that the switch saved the ARM FPU register D12.
 *
 *   - Check that the switch saved the ARM FPU register D13.
 *
 *   - Check that the switch saved the ARM FPU register D14.
 *
 *   - Check that the switch saved the ARM FPU register D15.
 *
 *   - Check that the round trip reached the trap. Without it the test proves
 *     nothing.
 *
 *   - Check that the switch saved the return address of its call in the ARM
 *     link register LR.
 *
 *   - Check that the switch saved the stack pointer of its call.
 *
 *   - Check that the switch back restored the stack pointer.
 *
 *   - Check that the heir received the thread pointer TPIDRURO of its context.
 *
 *   - Check that the switch back restored the thread pointer TPIDRURO of the
 *     executing context.
 *
 *   - Check that the switch saved FPSCR.
 *
 *   - Check that the switch back restored FPSCR.
 *
 *   - Check that the heir received the FPSCR of its context.
 *
 *   - Check that the context initialization set FPSCR to zero.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

extern const char context_done_label[];

extern const char context_return_label[];

/*
 * These are the condition flags, AHP, DN, FZ, the rounding mode and the
 * cumulative exception flags of FPSCR.  An Advanced SIMD unit adds QC.  The
 * mask covers no trap enable, no vector length and no vector stride.
 */
#if defined( __ARM_NEON )
#define FPSCR UINT64_C( 0xffc0009f )
#else
#define FPSCR UINT64_C( 0xf7c0009f )
#endif

typedef enum {
  SOURCE_R4,
  SOURCE_R5,
  SOURCE_R6,
  SOURCE_R7,
  SOURCE_R8,
  SOURCE_R9,
  SOURCE_R10,
  SOURCE_R11,
#ifdef ARM_MULTILIB_VFP
  SOURCE_D8,
  SOURCE_D9,
  SOURCE_D10,
  SOURCE_D11,
  SOURCE_D12,
  SOURCE_D13,
  SOURCE_D14,
  SOURCE_D15,
#endif
  SOURCE_TRAP,
  SOURCE_LR,
  SOURCE_SP,
#ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
  SOURCE_HEIR_TP,
  SOURCE_EXEC_TP,
#endif
#ifdef ARM_MULTILIB_VFP
  SOURCE_FPSCR,
  SOURCE_HEIR_FPSCR,
  SOURCE_INIT,
#endif
  SOURCE_MAX
} Source;

typedef enum {
  SLOT_FRAME_R4,
  SLOT_FRAME_R5,
  SLOT_FRAME_R6,
  SLOT_FRAME_R7,
  SLOT_FRAME_R8,
  SLOT_FRAME_R9,
  SLOT_FRAME_R10,
  SLOT_FRAME_R11,
  SLOT_SAVED_R4,
  SLOT_SAVED_R5,
  SLOT_SAVED_R6,
  SLOT_SAVED_R7,
  SLOT_SAVED_R8,
  SLOT_SAVED_R9,
  SLOT_SAVED_R10,
  SLOT_SAVED_R11,
#ifdef ARM_MULTILIB_VFP
  SLOT_FRAME_D8,
  SLOT_FRAME_D9,
  SLOT_FRAME_D10,
  SLOT_FRAME_D11,
  SLOT_FRAME_D12,
  SLOT_FRAME_D13,
  SLOT_FRAME_D14,
  SLOT_FRAME_D15,
  SLOT_SAVED_D8,
  SLOT_SAVED_D9,
  SLOT_SAVED_D10,
  SLOT_SAVED_D11,
  SLOT_SAVED_D12,
  SLOT_SAVED_D13,
  SLOT_SAVED_D14,
  SLOT_SAVED_D15,
#endif
  SLOT_TRAP,
  SLOT_SAVED_LR,
  SLOT_SAVED_SP,
  SLOT_FRAME_SP,
#ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
  SLOT_HEIR_TP,
  SLOT_EXEC_TP,
#endif
#ifdef ARM_MULTILIB_VFP
  SLOT_SAVED_FPSCR,
  SLOT_FRAME_FPSCR,
  SLOT_HEIR_FPSCR,
  SLOT_INIT_FPSCR,
#endif
  SLOT_MAX
} Slot;

/* The block loads each register from the byte offset eight times its source. */
RTEMS_STATIC_ASSERT( SOURCE_R11 == 7, source_r11 );
#ifdef ARM_MULTILIB_VFP
RTEMS_STATIC_ASSERT( SOURCE_D8 == 8, source_d8 );
#endif

#define WORD UINT64_C( 0xffffffff )

static const RegisterCheckSource context_sources[ SOURCE_MAX ] = {
  { "r4", REGISTER_CHECK_INITIAL, WORD },
  { "r5", REGISTER_CHECK_INITIAL, WORD },
  { "r6", REGISTER_CHECK_INITIAL, WORD },
  { "r7", REGISTER_CHECK_INITIAL, WORD },
  { "r8", REGISTER_CHECK_INITIAL, WORD },
  { "r9", REGISTER_CHECK_INITIAL, WORD },
  { "r10", REGISTER_CHECK_INITIAL, WORD },
  { "r11", REGISTER_CHECK_INITIAL, WORD },
#ifdef ARM_MULTILIB_VFP
  { "d8", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d9", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d10", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d11", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d12", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d13", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d14", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d15", REGISTER_CHECK_INITIAL, UINT64_MAX },
#endif
  { "trap", REGISTER_CHECK_EXPECTED, WORD },
  { "lr", REGISTER_CHECK_EXPECTED, WORD },
  { "sp", REGISTER_CHECK_EXPECTED, WORD },
#ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
  { "heir tp", REGISTER_CHECK_INITIAL, WORD },
  { "exec tp", REGISTER_CHECK_EXPECTED, WORD },
#endif
#ifdef ARM_MULTILIB_VFP
  { "fpscr", REGISTER_CHECK_INITIAL, FPSCR },
  { "heir fpscr", REGISTER_CHECK_INITIAL, FPSCR },
  { "init", REGISTER_CHECK_EXPECTED, WORD },
#endif
};

static const RegisterCheckSlot context_slots[ SLOT_MAX ] = {
  { "frame r4", SOURCE_R4, WORD },
  { "frame r5", SOURCE_R5, WORD },
  { "frame r6", SOURCE_R6, WORD },
  { "frame r7", SOURCE_R7, WORD },
  { "frame r8", SOURCE_R8, WORD },
  { "frame r9", SOURCE_R9, WORD },
  { "frame r10", SOURCE_R10, WORD },
  { "frame r11", SOURCE_R11, WORD },
  { "saved r4", SOURCE_R4, WORD },
  { "saved r5", SOURCE_R5, WORD },
  { "saved r6", SOURCE_R6, WORD },
  { "saved r7", SOURCE_R7, WORD },
  { "saved r8", SOURCE_R8, WORD },
  { "saved r9", SOURCE_R9, WORD },
  { "saved r10", SOURCE_R10, WORD },
  { "saved r11", SOURCE_R11, WORD },
#ifdef ARM_MULTILIB_VFP
  { "frame d8", SOURCE_D8, UINT64_MAX },
  { "frame d9", SOURCE_D9, UINT64_MAX },
  { "frame d10", SOURCE_D10, UINT64_MAX },
  { "frame d11", SOURCE_D11, UINT64_MAX },
  { "frame d12", SOURCE_D12, UINT64_MAX },
  { "frame d13", SOURCE_D13, UINT64_MAX },
  { "frame d14", SOURCE_D14, UINT64_MAX },
  { "frame d15", SOURCE_D15, UINT64_MAX },
  { "saved d8", SOURCE_D8, UINT64_MAX },
  { "saved d9", SOURCE_D9, UINT64_MAX },
  { "saved d10", SOURCE_D10, UINT64_MAX },
  { "saved d11", SOURCE_D11, UINT64_MAX },
  { "saved d12", SOURCE_D12, UINT64_MAX },
  { "saved d13", SOURCE_D13, UINT64_MAX },
  { "saved d14", SOURCE_D14, UINT64_MAX },
  { "saved d15", SOURCE_D15, UINT64_MAX },
#endif
  { "trap", SOURCE_TRAP, WORD },
  { "saved lr", SOURCE_LR, WORD },
  { "saved sp", SOURCE_SP, WORD },
  { "frame sp", SOURCE_SP, WORD },
#ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
  { "heir tp", SOURCE_HEIR_TP, WORD },
  { "exec tp", SOURCE_EXEC_TP, WORD },
#endif
#ifdef ARM_MULTILIB_VFP
  { "saved fpscr", SOURCE_FPSCR, FPSCR },
  { "frame fpscr", SOURCE_FPSCR, FPSCR },
  { "heir fpscr", SOURCE_HEIR_FPSCR, FPSCR },
  { "init fpscr", SOURCE_INIT, WORD },
#endif
};

static const uint64_t context_patterns[ SOURCE_MAX ] = {
  0x11121314,
  0x12131415,
  0x13141516,
  0x14151617,
  0x15161718,
  0x16171819,
  0x1718191a,
  0x18191a1b,
#ifdef ARM_MULTILIB_VFP
  0x3132333435363738,
  0x3233343536373839,
  0x333435363738393a,
  0x3435363738393a3b,
  0x35363738393a3b3c,
  0x363738393a3b3c3d,
  0x3738393a3b3c3d3e,
  0x38393a3b3c3d3e3f,
#endif
  0,
  0,
  0,
#ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
  0x71727374,
  0,
#endif
#ifdef ARM_MULTILIB_VFP
  0x90400011,
  0x40800006,
  0,
#endif
};

uint64_t context_initial[ SOURCE_MAX ];

static CPU_Exception_frame context_frame;

#ifdef ARM_MULTILIB_VFP
static ARM_VFP_context context_frame_vfp;
#endif

static volatile int context_trap;

static uintptr_t context_sp;

#ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
uint32_t context_heir_tp;

static uint32_t context_exec_tp;
#endif

#ifdef ARM_MULTILIB_VFP
uint32_t context_heir_fpscr;

static uint32_t context_fpscr;

static uint32_t context_init_fpscr;
#endif

Context_Control context_executing;

Context_Control context_heir;

static long context_heir_stack[ 256 ];

/* The block loads r0 last, because r0 holds the address of the table. */
#define CTX_LOAD               \
  "ldr r0, =context_initial\n" \
  "ldr r4, [r0, #0]\n"         \
  "ldr r5, [r0, #8]\n"         \
  "ldr r6, [r0, #16]\n"        \
  "ldr r7, [r0, #24]\n"        \
  "ldr r8, [r0, #32]\n"        \
  "ldr r9, [r0, #40]\n"        \
  "ldr r10, [r0, #48]\n"       \
  "ldr r11, [r0, #56]\n"

#ifdef ARM_MULTILIB_VFP
#define CTX_LOAD_VFP          \
  "vldr.64 d8, [r0, #64]\n"   \
  "vldr.64 d9, [r0, #72]\n"   \
  "vldr.64 d10, [r0, #80]\n"  \
  "vldr.64 d11, [r0, #88]\n"  \
  "vldr.64 d12, [r0, #96]\n"  \
  "vldr.64 d13, [r0, #104]\n" \
  "vldr.64 d14, [r0, #112]\n" \
  "vldr.64 d15, [r0, #120]\n"

#else
#define CTX_LOAD_VFP
#endif

static void ContextFatal(
  rtems_fatal_source source,
  rtems_fatal_code   code,
  void              *arg
)
{
  CPU_Exception_frame *frame;

  (void) arg;

  if ( source != RTEMS_FATAL_SOURCE_EXCEPTION ) {
    return;
  }

  frame = (CPU_Exception_frame *) code;
  context_frame = *frame;
#ifdef ARM_MULTILIB_VFP
  if ( frame->vfp_context != NULL ) {
    context_frame_vfp = *frame->vfp_context;
    ( (ARM_VFP_context *) frame->vfp_context )->register_fpscr = context_fpscr;
  }
#endif
  ++context_trap;
  SetFatalHandler( NULL, NULL );
  frame->register_pc = (void *) context_done_label;
  _CPU_Exception_resume( frame );
}

/*
 * The heir switches straight back.  It is assembly, so no prologue saves a
 * register of the context under test.
 */
void ContextHeirEntry( void );

__asm__( "  .globl ContextHeirEntry\n"
         "  .type ContextHeirEntry, %function\n"
         "ContextHeirEntry:\n"
#ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
         "  mrc p15, 0, r2, c13, c0, 3\n"
         "  ldr r3, =context_heir_tp\n"
         "  str r2, [r3]\n"
#endif
#ifdef ARM_MULTILIB_VFP
         "  vmrs r2, FPSCR\n"
         "  ldr r3, =context_heir_fpscr\n"
         "  str r2, [r3]\n"
#endif
         "  ldr r0, =context_heir\n"
         "  ldr r1, =context_executing\n"
         "  b _CPU_Context_switch\n" );

/*
 * The switch is called directly, so the patterns are in the registers when it
 * saves them.  Nothing spills them to a stack frame first.
 */
static void ContextRunSwitch( void )
{
  context_trap = 0;
  memset( &context_frame, 0, sizeof( context_frame ) );
#ifdef ARM_MULTILIB_VFP
  memset( &context_frame_vfp, 0, sizeof( context_frame_vfp ) );
#endif
  memset( &context_executing, 0, sizeof( context_executing ) );

  /*
   * The switch restores the thread identifier and never saves it, so the
   * context which resumes has to carry the one of the running thread.
   */
  context_executing.thread_id = GetExecuting()->Registers.thread_id;
  _CPU_Context_Initialize(
    &context_heir,
    context_heir_stack,
    sizeof( context_heir_stack ),
    0,
    ContextHeirEntry,
    true,
    NULL
  );
#ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
  context_heir.thread_id = (uint32_t) context_initial[ SOURCE_HEIR_TP ];
#endif
#ifdef ARM_MULTILIB_VFP
  context_init_fpscr = context_heir.register_fpscr;
  context_heir.register_fpscr = (uint32_t)
    context_initial[ SOURCE_HEIR_FPSCR ];
  __asm__ volatile( "vmrs %0, FPSCR"
                    : "=r"( context_fpscr ) );
  __asm__ volatile( "vmsr FPSCR, %0"
                    :
                    : "r"( (uint32_t) context_initial[SOURCE_FPSCR] ) );
#endif
  __asm__ volatile( "mov %0, sp"
                    : "=r"( context_sp ) );
  SetFatalHandler( ContextFatal, NULL );

  __asm__ volatile( CTX_LOAD CTX_LOAD_VFP "ldr r0, =context_executing\n"
                                          "ldr r1, =context_heir\n"
                                          "bl _CPU_Context_switch\n"
                                          ".globl context_return_label\n"
                                          "context_return_label:\n"
                                          "udf #0\n"
                                          ".globl context_done_label\n"
                                          "context_done_label:\n"
                    :
                    :
                    :
#ifdef ARM_MULTILIB_VFP
                    "d8",
                    "d9",
                    "d10",
                    "d11",
                    "d12",
                    "d13",
                    "d14",
                    "d15",
#endif
                    "memory",
                    "cc",
                    "r0",
                    "r1",
                    "r2",
                    "r3",
                    "r4",
                    "r5",
                    "r6",
                    "r7",
                    "r8",
                    "r9",
                    "r10",
                    "r11",
                    "r12",
                    "r14" );
#ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
  __asm__ volatile( "mrc p15, 0, %0, c13, c0, 3"
                    : "=r"( context_exec_tp ) );
#endif
}

static void ContextRun( RegisterCheck *self, void *arg )
{
  const uint64_t *patterns;

  (void) arg;
  ContextRunSwitch();
  patterns = context_patterns;

  RegisterCheckRecord(
    self,
    SLOT_FRAME_R4,
    context_frame.register_r4,
    patterns[ SOURCE_R4 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R5,
    context_frame.register_r5,
    patterns[ SOURCE_R5 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R6,
    context_frame.register_r6,
    patterns[ SOURCE_R6 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R7,
    context_frame.register_r7,
    patterns[ SOURCE_R7 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R8,
    context_frame.register_r8,
    patterns[ SOURCE_R8 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R9,
    context_frame.register_r9,
    patterns[ SOURCE_R9 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R10,
    context_frame.register_r10,
    patterns[ SOURCE_R10 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R11,
    context_frame.register_r11,
    patterns[ SOURCE_R11 ]
  );

  RegisterCheckRecord(
    self,
    SLOT_SAVED_R4,
    context_executing.register_r4,
    patterns[ SOURCE_R4 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R5,
    context_executing.register_r5,
    patterns[ SOURCE_R5 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R6,
    context_executing.register_r6,
    patterns[ SOURCE_R6 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R7,
    context_executing.register_r7,
    patterns[ SOURCE_R7 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R8,
    context_executing.register_r8,
    patterns[ SOURCE_R8 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R9,
    context_executing.register_r9,
    patterns[ SOURCE_R9 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R10,
    context_executing.register_r10,
    patterns[ SOURCE_R10 ]
  );
#if defined( ARM_MULTILIB_ARCH_V4 )
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R11,
    context_executing.register_fp,
    patterns[ SOURCE_R11 ]
  );
#else
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R11,
    context_executing.register_r11,
    patterns[ SOURCE_R11 ]
  );
#endif

#ifdef ARM_MULTILIB_VFP
  {
    const uint64_t *frame_d;
    const uint64_t *saved_d;
    size_t          i;

    frame_d = &context_frame_vfp.register_d8;
    saved_d = &context_executing.register_d8;

    for ( i = 0; i < 8; ++i ) {
      RegisterCheckRecord(
        self,
        SLOT_FRAME_D8 + i,
        frame_d[ i ],
        patterns[ SOURCE_D8 + i ]
      );
      RegisterCheckRecord(
        self,
        SLOT_SAVED_D8 + i,
        saved_d[ i ],
        patterns[ SOURCE_D8 + i ]
      );
    }
  }
#endif

  RegisterCheckRecord( self, SLOT_TRAP, (uint64_t) context_trap, 1 );

  /* The two addresses may differ in the Thumb bit. */
  RegisterCheckRecord(
    self,
    SLOT_SAVED_LR,
    ( (uintptr_t) context_executing.register_lr & ~(uintptr_t) 1 ) ==
      ( (uintptr_t) context_return_label & ~(uintptr_t) 1 ),
    1
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_SP,
    (uintptr_t) context_executing.register_sp == context_sp,
    1
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_SP,
    (uintptr_t) context_frame.register_sp == context_sp,
    1
  );
#ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
  RegisterCheckRecord(
    self,
    SLOT_HEIR_TP,
    context_heir_tp,
    patterns[ SOURCE_HEIR_TP ]
  );
  RegisterCheckRecord(
    self,
    SLOT_EXEC_TP,
    context_exec_tp == context_executing.thread_id,
    1
  );
#endif
#ifdef ARM_MULTILIB_VFP
  RegisterCheckRecord(
    self,
    SLOT_SAVED_FPSCR,
    context_executing.register_fpscr,
    patterns[ SOURCE_FPSCR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_FPSCR,
    context_frame_vfp.register_fpscr,
    patterns[ SOURCE_FPSCR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_HEIR_FPSCR,
    context_heir_fpscr,
    patterns[ SOURCE_HEIR_FPSCR ]
  );
  RegisterCheckRecord( self, SLOT_INIT_FPSCR, context_init_fpscr, 0 );
#endif
}

static RegisterCheck context_check = {
  .sources = context_sources,
  .source_count = SOURCE_MAX,
  .slots = context_slots,
  .slot_count = SLOT_MAX,
  .initial = context_initial,
  .run = ContextRun
};

static void CheckSlot( Slot slot )
{
  RegisterCheckVerify( &context_check, slot );
}

/**
 * @brief Load a distinct pattern into every call-saved register and into
 *   FPSCR. Give the heir its own thread pointer and FPSCR. Call the context
 *   switch directly. The heir switches back. Trap, so the frame reports every
 *   register. Run this once for each register with the pattern of that
 *   register inverted, then once unchanged.
 */
static void ScoreCpuArmValContext_Action_0( void )
{
  memcpy( context_initial, context_patterns, sizeof( context_initial ) );
  RegisterCheckRun( &context_check );

  /*
   * Check that the switch restored the ARM core register R4.
   */
  CheckSlot( SLOT_FRAME_R4 );

  /*
   * Check that the switch restored the ARM core register R5.
   */
  CheckSlot( SLOT_FRAME_R5 );

  /*
   * Check that the switch restored the ARM core register R6.
   */
  CheckSlot( SLOT_FRAME_R6 );

  /*
   * Check that the switch restored the ARM core register R7.
   */
  CheckSlot( SLOT_FRAME_R7 );

  /*
   * Check that the switch restored the ARM core register R8.
   */
  CheckSlot( SLOT_FRAME_R8 );

  /*
   * Check that the switch restored the ARM core register R9.
   */
  CheckSlot( SLOT_FRAME_R9 );

  /*
   * Check that the switch restored the ARM core register R10.
   */
  CheckSlot( SLOT_FRAME_R10 );

  /*
   * Check that the switch restored the ARM core register R11.
   */
  CheckSlot( SLOT_FRAME_R11 );

  /*
   * Check that the switch restored the ARM FPU register D8.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_FRAME_D8 );
  #endif

  /*
   * Check that the switch restored the ARM FPU register D9.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_FRAME_D9 );
  #endif

  /*
   * Check that the switch restored the ARM FPU register D10.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_FRAME_D10 );
  #endif

  /*
   * Check that the switch restored the ARM FPU register D11.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_FRAME_D11 );
  #endif

  /*
   * Check that the switch restored the ARM FPU register D12.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_FRAME_D12 );
  #endif

  /*
   * Check that the switch restored the ARM FPU register D13.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_FRAME_D13 );
  #endif

  /*
   * Check that the switch restored the ARM FPU register D14.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_FRAME_D14 );
  #endif

  /*
   * Check that the switch restored the ARM FPU register D15.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_FRAME_D15 );
  #endif

  /*
   * Check that the switch saved the ARM core register R4.
   */
  CheckSlot( SLOT_SAVED_R4 );

  /*
   * Check that the switch saved the ARM core register R5.
   */
  CheckSlot( SLOT_SAVED_R5 );

  /*
   * Check that the switch saved the ARM core register R6.
   */
  CheckSlot( SLOT_SAVED_R6 );

  /*
   * Check that the switch saved the ARM core register R7.
   */
  CheckSlot( SLOT_SAVED_R7 );

  /*
   * Check that the switch saved the ARM core register R8.
   */
  CheckSlot( SLOT_SAVED_R8 );

  /*
   * Check that the switch saved the ARM core register R9.
   */
  CheckSlot( SLOT_SAVED_R9 );

  /*
   * Check that the switch saved the ARM core register R10.
   */
  CheckSlot( SLOT_SAVED_R10 );

  /*
   * Check that the switch saved the ARM core register R11.
   */
  CheckSlot( SLOT_SAVED_R11 );

  /*
   * Check that the switch saved the ARM FPU register D8.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_SAVED_D8 );
  #endif

  /*
   * Check that the switch saved the ARM FPU register D9.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_SAVED_D9 );
  #endif

  /*
   * Check that the switch saved the ARM FPU register D10.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_SAVED_D10 );
  #endif

  /*
   * Check that the switch saved the ARM FPU register D11.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_SAVED_D11 );
  #endif

  /*
   * Check that the switch saved the ARM FPU register D12.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_SAVED_D12 );
  #endif

  /*
   * Check that the switch saved the ARM FPU register D13.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_SAVED_D13 );
  #endif

  /*
   * Check that the switch saved the ARM FPU register D14.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_SAVED_D14 );
  #endif

  /*
   * Check that the switch saved the ARM FPU register D15.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_SAVED_D15 );
  #endif

  /*
   * Check that the round trip reached the trap. Without it the test proves
   * nothing.
   */
  CheckSlot( SLOT_TRAP );

  /*
   * Check that the switch saved the return address of its call in the ARM link
   * register LR.
   */
  CheckSlot( SLOT_SAVED_LR );

  /*
   * Check that the switch saved the stack pointer of its call.
   */
  CheckSlot( SLOT_SAVED_SP );

  /*
   * Check that the switch back restored the stack pointer.
   */
  CheckSlot( SLOT_FRAME_SP );

  /*
   * Check that the heir received the thread pointer TPIDRURO of its context.
   */
  #ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
  CheckSlot( SLOT_HEIR_TP );
  #endif

  /*
   * Check that the switch back restored the thread pointer TPIDRURO of the
   * executing context.
   */
  #ifdef ARM_MULTILIB_HAS_THREAD_ID_REGISTER
  CheckSlot( SLOT_EXEC_TP );
  #endif

  /*
   * Check that the switch saved FPSCR.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_SAVED_FPSCR );
  #endif

  /*
   * Check that the switch back restored FPSCR.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_FRAME_FPSCR );
  #endif

  /*
   * Check that the heir received the FPSCR of its context.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_HEIR_FPSCR );
  #endif

  /*
   * Check that the context initialization set FPSCR to zero.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_INIT_FPSCR );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  RegisterCheckReport( &context_check );
}

/**
 * @fn void T_case_body_ScoreCpuArmValContext( void )
 */
T_TEST_CASE( ScoreCpuArmValContext )
{
  ScoreCpuArmValContext_Action_0();
}

/** @} */
