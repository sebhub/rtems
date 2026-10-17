/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuPowerpcValContext
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
 * @defgroup ScoreCpuPowerpcValContext spec:/score/cpu/powerpc/val/context
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that the context switch saves and restores every call-saved
 *   register.
 *
 * This test case performs the following actions:
 *
 * - Load a distinct pattern into every call-saved register. Call the context
 *   switch directly. The heir switches back. Trap, so the frame reports every
 *   register. Run this once for each register with the pattern of that
 *   register inverted, then once unchanged.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R14.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R15.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R16.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R17.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R18.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R19.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R20.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R21.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R22.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R23.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R24.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R25.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R26.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R27.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R28.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R29.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R30.
 *
 *   - Check that the switch restored the bottom 32 bits of the PowerPC
 *     register R31.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R14.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R15.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R16.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R17.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R18.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R19.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R20.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R21.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R22.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R23.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R24.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R25.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R26.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R27.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R28.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R29.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R30.
 *
 *   - Check that the switch saved the bottom 32 bits of the PowerPC register
 *     R31.
 *
 *   - Check that the switch back restored the fields CR2 to CR4 of CR.
 *
 *   - Check that the switch saved the fields CR2 to CR4 of CR.
 *
 *   - Check that the trap after the switch back reached the fatal handler
 *     once.
 *
 *   - Check that the switch saved the return address of its call in LR.
 *
 *   - Check that the switch saved the stack pointer R1 of its call.
 *
 *   - Check that the switch back restored the stack pointer R1.
 *
 *   - Check that the heir received the thread pointer of its context.
 *
 *   - Check that the switch back restored the thread pointer of the executing
 *     context.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

extern const char context_return_label[];

extern const char context_done_label[];

#define WORD UINT64_C( 0xffffffff )

/* The thread pointer is r2 in 32-bit code and r13 in 64-bit code. */
#if defined( __powerpc64__ )
#define CTX_TP "13"
#else
#define CTX_TP "2"
#endif

/* These are the fields CR2 to CR4, which a called function preserves. */
#define CR_MASK UINT64_C( 0x000fff00 )

typedef enum {
  SOURCE_R14,
  SOURCE_R15,
  SOURCE_R16,
  SOURCE_R17,
  SOURCE_R18,
  SOURCE_R19,
  SOURCE_R20,
  SOURCE_R21,
  SOURCE_R22,
  SOURCE_R23,
  SOURCE_R24,
  SOURCE_R25,
  SOURCE_R26,
  SOURCE_R27,
  SOURCE_R28,
  SOURCE_R29,
  SOURCE_R30,
  SOURCE_R31,
  SOURCE_CR,
  SOURCE_TRAP,
  SOURCE_HEIR_TP,
  SOURCE_LR,
  SOURCE_SP,
  SOURCE_EXEC_TP,
  SOURCE_MAX
} Source;

typedef enum {
  SLOT_FRAME_R14,
  SLOT_FRAME_R15,
  SLOT_FRAME_R16,
  SLOT_FRAME_R17,
  SLOT_FRAME_R18,
  SLOT_FRAME_R19,
  SLOT_FRAME_R20,
  SLOT_FRAME_R21,
  SLOT_FRAME_R22,
  SLOT_FRAME_R23,
  SLOT_FRAME_R24,
  SLOT_FRAME_R25,
  SLOT_FRAME_R26,
  SLOT_FRAME_R27,
  SLOT_FRAME_R28,
  SLOT_FRAME_R29,
  SLOT_FRAME_R30,
  SLOT_FRAME_R31,
  SLOT_SAVED_R14,
  SLOT_SAVED_R15,
  SLOT_SAVED_R16,
  SLOT_SAVED_R17,
  SLOT_SAVED_R18,
  SLOT_SAVED_R19,
  SLOT_SAVED_R20,
  SLOT_SAVED_R21,
  SLOT_SAVED_R22,
  SLOT_SAVED_R23,
  SLOT_SAVED_R24,
  SLOT_SAVED_R25,
  SLOT_SAVED_R26,
  SLOT_SAVED_R27,
  SLOT_SAVED_R28,
  SLOT_SAVED_R29,
  SLOT_SAVED_R30,
  SLOT_SAVED_R31,
  SLOT_FRAME_CR,
  SLOT_SAVED_CR,
  SLOT_TRAP,
  SLOT_SAVED_LR,
  SLOT_SAVED_SP,
  SLOT_FRAME_SP,
  SLOT_HEIR_TP,
  SLOT_EXEC_TP,
  SLOT_MAX
} Slot;

/*
 * The block loads each register from the bottom word of the doubleword at
 * eight times its source.
 */
RTEMS_STATIC_ASSERT( SOURCE_R31 == 17, source_r31 );
RTEMS_STATIC_ASSERT( SOURCE_CR == 18, source_cr );

static const RegisterCheckSource context_sources[ SOURCE_MAX ] = {
  { "r14", REGISTER_CHECK_INITIAL, WORD },
  { "r15", REGISTER_CHECK_INITIAL, WORD },
  { "r16", REGISTER_CHECK_INITIAL, WORD },
  { "r17", REGISTER_CHECK_INITIAL, WORD },
  { "r18", REGISTER_CHECK_INITIAL, WORD },
  { "r19", REGISTER_CHECK_INITIAL, WORD },
  { "r20", REGISTER_CHECK_INITIAL, WORD },
  { "r21", REGISTER_CHECK_INITIAL, WORD },
  { "r22", REGISTER_CHECK_INITIAL, WORD },
  { "r23", REGISTER_CHECK_INITIAL, WORD },
  { "r24", REGISTER_CHECK_INITIAL, WORD },
  { "r25", REGISTER_CHECK_INITIAL, WORD },
  { "r26", REGISTER_CHECK_INITIAL, WORD },
  { "r27", REGISTER_CHECK_INITIAL, WORD },
  { "r28", REGISTER_CHECK_INITIAL, WORD },
  { "r29", REGISTER_CHECK_INITIAL, WORD },
  { "r30", REGISTER_CHECK_INITIAL, WORD },
  { "r31", REGISTER_CHECK_INITIAL, WORD },
  { "cr", REGISTER_CHECK_INITIAL, CR_MASK },
  { "trap", REGISTER_CHECK_EXPECTED, WORD },
  { "heir tp", REGISTER_CHECK_INITIAL, WORD },
  { "lr", REGISTER_CHECK_EXPECTED, WORD },
  { "sp", REGISTER_CHECK_EXPECTED, WORD },
  { "exec tp", REGISTER_CHECK_EXPECTED, WORD }
};

static const RegisterCheckSlot context_slots[ SLOT_MAX ] = {
  { "frame r14", SOURCE_R14, WORD },
  { "frame r15", SOURCE_R15, WORD },
  { "frame r16", SOURCE_R16, WORD },
  { "frame r17", SOURCE_R17, WORD },
  { "frame r18", SOURCE_R18, WORD },
  { "frame r19", SOURCE_R19, WORD },
  { "frame r20", SOURCE_R20, WORD },
  { "frame r21", SOURCE_R21, WORD },
  { "frame r22", SOURCE_R22, WORD },
  { "frame r23", SOURCE_R23, WORD },
  { "frame r24", SOURCE_R24, WORD },
  { "frame r25", SOURCE_R25, WORD },
  { "frame r26", SOURCE_R26, WORD },
  { "frame r27", SOURCE_R27, WORD },
  { "frame r28", SOURCE_R28, WORD },
  { "frame r29", SOURCE_R29, WORD },
  { "frame r30", SOURCE_R30, WORD },
  { "frame r31", SOURCE_R31, WORD },
  { "saved r14", SOURCE_R14, WORD },
  { "saved r15", SOURCE_R15, WORD },
  { "saved r16", SOURCE_R16, WORD },
  { "saved r17", SOURCE_R17, WORD },
  { "saved r18", SOURCE_R18, WORD },
  { "saved r19", SOURCE_R19, WORD },
  { "saved r20", SOURCE_R20, WORD },
  { "saved r21", SOURCE_R21, WORD },
  { "saved r22", SOURCE_R22, WORD },
  { "saved r23", SOURCE_R23, WORD },
  { "saved r24", SOURCE_R24, WORD },
  { "saved r25", SOURCE_R25, WORD },
  { "saved r26", SOURCE_R26, WORD },
  { "saved r27", SOURCE_R27, WORD },
  { "saved r28", SOURCE_R28, WORD },
  { "saved r29", SOURCE_R29, WORD },
  { "saved r30", SOURCE_R30, WORD },
  { "saved r31", SOURCE_R31, WORD },
  { "frame cr", SOURCE_CR, CR_MASK },
  { "saved cr", SOURCE_CR, CR_MASK },
  { "trap", SOURCE_TRAP, WORD },
  { "saved lr", SOURCE_LR, WORD },
  { "saved sp", SOURCE_SP, WORD },
  { "frame sp", SOURCE_SP, WORD },
  { "heir tp", SOURCE_HEIR_TP, WORD },
  { "exec tp", SOURCE_EXEC_TP, WORD }
};

static const uint64_t context_patterns[ SOURCE_MAX ] = {
  UINT64_C( 0x11121314 ),
  UINT64_C( 0x12131415 ),
  UINT64_C( 0x13141516 ),
  UINT64_C( 0x14151617 ),
  UINT64_C( 0x15161718 ),
  UINT64_C( 0x16171819 ),
  UINT64_C( 0x1718191a ),
  UINT64_C( 0x18191a1b ),
  UINT64_C( 0x191a1b1c ),
  UINT64_C( 0x1a1b1c1d ),
  UINT64_C( 0x1b1c1d1e ),
  UINT64_C( 0x1c1d1e1f ),
  UINT64_C( 0x1d1e1f20 ),
  UINT64_C( 0x1e1f2021 ),
  UINT64_C( 0x1f202122 ),
  UINT64_C( 0x20212223 ),
  UINT64_C( 0x21222324 ),
  UINT64_C( 0x22232425 ),
  UINT64_C( 0x00a5a000 ),
  1,
  UINT64_C( 0x71727374 ),
  1,
  1,
  1
};

uint64_t context_initial[ SOURCE_MAX ];

static CPU_Exception_frame context_frame;

static volatile int context_trap;

Context_Control context_executing;

Context_Control context_heir;

uint32_t context_heir_tp;

static uintptr_t context_sp;

static uintptr_t context_exec_tp;

static uint64_t context_heir_stack[ 512 ] RTEMS_ALIGNED( CPU_STACK_ALIGNMENT );

/*
 * The block loads r3 with the address of the table and loads CR first, so that
 * r0 is free afterwards.
 */
#define CTX_LOAD                   \
  "lis 3, context_initial@ha\n"    \
  "addi 3, 3, context_initial@l\n" \
  "lwz 0, 148(3)\n"                \
  "mtcr 0\n"                       \
  "lwz 14, 4(3)\n"                 \
  "lwz 15, 12(3)\n"                \
  "lwz 16, 20(3)\n"                \
  "lwz 17, 28(3)\n"                \
  "lwz 18, 36(3)\n"                \
  "lwz 19, 44(3)\n"                \
  "lwz 20, 52(3)\n"                \
  "lwz 21, 60(3)\n"                \
  "lwz 22, 68(3)\n"                \
  "lwz 23, 76(3)\n"                \
  "lwz 24, 84(3)\n"                \
  "lwz 25, 92(3)\n"                \
  "lwz 26, 100(3)\n"               \
  "lwz 27, 108(3)\n"               \
  "lwz 28, 116(3)\n"               \
  "lwz 29, 124(3)\n"               \
  "lwz 30, 132(3)\n"               \
  "lwz 31, 140(3)\n"

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
  ++context_trap;
  SetFatalHandler( NULL, NULL );
  frame->EXC_SRR0 = (uintptr_t) context_done_label;
  _CPU_Exception_resume( frame );
}

/*
 * The heir records its thread pointer and switches straight back.  It is
 * assembly, so no prologue saves a register of the context under test.
 */
void ContextHeirEntry( void );

__asm__( "  .globl ContextHeirEntry\n"
         "  .type ContextHeirEntry, @function\n"
         "ContextHeirEntry:\n"
         "  lis 5, context_heir_tp@ha\n"
         "  stw " CTX_TP ", context_heir_tp@l(5)\n"
         "  lis 3, context_heir@ha\n"
         "  addi 3, 3, context_heir@l\n"
         "  lis 4, context_executing@ha\n"
         "  addi 4, 4, context_executing@l\n"
         "  b _CPU_Context_switch\n" );

/*
 * The switch is called directly, so the patterns are in the registers when it
 * saves them.  Nothing spills them to a stack frame first.
 */
static void ContextRunSwitch( void )
{
  context_trap = 0;
  memset( &context_frame, 0, sizeof( context_frame ) );
  memset( &context_executing, 0, sizeof( context_executing ) );

  /*
   * The switch restores the thread pointer and never saves it, so the context
   * which resumes has to carry the one of the running thread.
   */
  ppc_get_context( &context_executing )->tp =
    ppc_get_context( &GetExecuting()->Registers )->tp;
  _CPU_Context_Initialize(
    &context_heir,
    context_heir_stack,
    sizeof( context_heir_stack ),
    0,
    ContextHeirEntry,
    false,
    NULL
  );
  ppc_get_context( &context_heir )->tp = (uintptr_t)
    context_initial[ SOURCE_HEIR_TP ];
  SetFatalHandler( ContextFatal, NULL );
  __asm__ volatile( "mr %0, 1"
                    : "=r"( context_sp ) );

  __asm__ volatile( CTX_LOAD "lis 3, context_executing@ha\n"
                             "addi 3, 3, context_executing@l\n"
                             "lis 4, context_heir@ha\n"
                             "addi 4, 4, context_heir@l\n"
                             "bl _CPU_Context_switch\n"
                             ".globl context_return_label\n"
                             "context_return_label:\n"
                             "trap\n"
                             ".globl context_done_label\n"
                             "context_done_label:\n"
                    :
                    :
                    : "memory",
                      "cr0",
                      "cr1",
                      "cr2",
                      "cr3",
                      "cr4",
                      "cr5",
                      "cr6",
                      "cr7",
                      "r0",
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
                      "r14",
                      "r15",
                      "r16",
                      "r17",
                      "r18",
                      "r19",
                      "r20",
                      "r21",
                      "r22",
                      "r23",
                      "r24",
                      "r25",
                      "r26",
                      "r27",
                      "r28",
                      "r29",
                      "r30",
                      "r31",
                      "lr",
                      "ctr" );
  __asm__ volatile( "mr %0, " CTX_TP
                    : "=r"( context_exec_tp ) );
}

static void ContextRun( RegisterCheck *self, void *arg )
{
  const ppc_context *ctx;

  (void) arg;
  ContextRunSwitch();
  ctx = ppc_get_context( &context_executing );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R14,
    (uint32_t) context_frame.registers[ 14 ],
    context_patterns[ SOURCE_R14 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R15,
    (uint32_t) context_frame.registers[ 15 ],
    context_patterns[ SOURCE_R15 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R16,
    (uint32_t) context_frame.registers[ 16 ],
    context_patterns[ SOURCE_R16 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R17,
    (uint32_t) context_frame.registers[ 17 ],
    context_patterns[ SOURCE_R17 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R18,
    (uint32_t) context_frame.registers[ 18 ],
    context_patterns[ SOURCE_R18 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R19,
    (uint32_t) context_frame.registers[ 19 ],
    context_patterns[ SOURCE_R19 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R20,
    (uint32_t) context_frame.registers[ 20 ],
    context_patterns[ SOURCE_R20 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R21,
    (uint32_t) context_frame.registers[ 21 ],
    context_patterns[ SOURCE_R21 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R22,
    (uint32_t) context_frame.registers[ 22 ],
    context_patterns[ SOURCE_R22 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R23,
    (uint32_t) context_frame.registers[ 23 ],
    context_patterns[ SOURCE_R23 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R24,
    (uint32_t) context_frame.registers[ 24 ],
    context_patterns[ SOURCE_R24 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R25,
    (uint32_t) context_frame.registers[ 25 ],
    context_patterns[ SOURCE_R25 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R26,
    (uint32_t) context_frame.registers[ 26 ],
    context_patterns[ SOURCE_R26 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R27,
    (uint32_t) context_frame.registers[ 27 ],
    context_patterns[ SOURCE_R27 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R28,
    (uint32_t) context_frame.registers[ 28 ],
    context_patterns[ SOURCE_R28 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R29,
    (uint32_t) context_frame.registers[ 29 ],
    context_patterns[ SOURCE_R29 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R30,
    (uint32_t) context_frame.registers[ 30 ],
    context_patterns[ SOURCE_R30 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R31,
    (uint32_t) context_frame.registers[ 31 ],
    context_patterns[ SOURCE_R31 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R14,
    (uint32_t) ctx->gpr14,
    context_patterns[ SOURCE_R14 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R15,
    (uint32_t) ctx->gpr15,
    context_patterns[ SOURCE_R15 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R16,
    (uint32_t) ctx->gpr16,
    context_patterns[ SOURCE_R16 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R17,
    (uint32_t) ctx->gpr17,
    context_patterns[ SOURCE_R17 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R18,
    (uint32_t) ctx->gpr18,
    context_patterns[ SOURCE_R18 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R19,
    (uint32_t) ctx->gpr19,
    context_patterns[ SOURCE_R19 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R20,
    (uint32_t) ctx->gpr20,
    context_patterns[ SOURCE_R20 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R21,
    (uint32_t) ctx->gpr21,
    context_patterns[ SOURCE_R21 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R22,
    (uint32_t) ctx->gpr22,
    context_patterns[ SOURCE_R22 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R23,
    (uint32_t) ctx->gpr23,
    context_patterns[ SOURCE_R23 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R24,
    (uint32_t) ctx->gpr24,
    context_patterns[ SOURCE_R24 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R25,
    (uint32_t) ctx->gpr25,
    context_patterns[ SOURCE_R25 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R26,
    (uint32_t) ctx->gpr26,
    context_patterns[ SOURCE_R26 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R27,
    (uint32_t) ctx->gpr27,
    context_patterns[ SOURCE_R27 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R28,
    (uint32_t) ctx->gpr28,
    context_patterns[ SOURCE_R28 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R29,
    (uint32_t) ctx->gpr29,
    context_patterns[ SOURCE_R29 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R30,
    (uint32_t) ctx->gpr30,
    context_patterns[ SOURCE_R30 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R31,
    (uint32_t) ctx->gpr31,
    context_patterns[ SOURCE_R31 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_CR,
    context_frame.EXC_CR & CR_MASK,
    context_patterns[ SOURCE_CR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_CR,
    ctx->cr & CR_MASK,
    context_patterns[ SOURCE_CR ]
  );
  RegisterCheckRecord( self, SLOT_TRAP, (uint64_t) context_trap, 1 );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_LR,
    ctx->lr == (uintptr_t) context_return_label,
    1
  );
  RegisterCheckRecord( self, SLOT_SAVED_SP, ctx->gpr1 == context_sp, 1 );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_SP,
    (uintptr_t) context_frame.GPR1 == context_sp,
    1
  );
  RegisterCheckRecord(
    self,
    SLOT_HEIR_TP,
    context_heir_tp,
    context_patterns[ SOURCE_HEIR_TP ]
  );
  RegisterCheckRecord( self, SLOT_EXEC_TP, context_exec_tp == ctx->tp, 1 );
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
 * @brief Load a distinct pattern into every call-saved register. Call the
 *   context switch directly. The heir switches back. Trap, so the frame
 *   reports every register. Run this once for each register with the pattern
 *   of that register inverted, then once unchanged.
 */
static void ScoreCpuPowerpcValContext_Action_0( void )
{
  memcpy( context_initial, context_patterns, sizeof( context_initial ) );
  RegisterCheckRun( &context_check );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R14.
   */
  CheckSlot( SLOT_FRAME_R14 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R15.
   */
  CheckSlot( SLOT_FRAME_R15 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R16.
   */
  CheckSlot( SLOT_FRAME_R16 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R17.
   */
  CheckSlot( SLOT_FRAME_R17 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R18.
   */
  CheckSlot( SLOT_FRAME_R18 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R19.
   */
  CheckSlot( SLOT_FRAME_R19 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R20.
   */
  CheckSlot( SLOT_FRAME_R20 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R21.
   */
  CheckSlot( SLOT_FRAME_R21 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R22.
   */
  CheckSlot( SLOT_FRAME_R22 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R23.
   */
  CheckSlot( SLOT_FRAME_R23 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R24.
   */
  CheckSlot( SLOT_FRAME_R24 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R25.
   */
  CheckSlot( SLOT_FRAME_R25 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R26.
   */
  CheckSlot( SLOT_FRAME_R26 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R27.
   */
  CheckSlot( SLOT_FRAME_R27 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R28.
   */
  CheckSlot( SLOT_FRAME_R28 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R29.
   */
  CheckSlot( SLOT_FRAME_R29 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R30.
   */
  CheckSlot( SLOT_FRAME_R30 );

  /*
   * Check that the switch restored the bottom 32 bits of the PowerPC register
   * R31.
   */
  CheckSlot( SLOT_FRAME_R31 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R14.
   */
  CheckSlot( SLOT_SAVED_R14 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R15.
   */
  CheckSlot( SLOT_SAVED_R15 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R16.
   */
  CheckSlot( SLOT_SAVED_R16 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R17.
   */
  CheckSlot( SLOT_SAVED_R17 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R18.
   */
  CheckSlot( SLOT_SAVED_R18 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R19.
   */
  CheckSlot( SLOT_SAVED_R19 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R20.
   */
  CheckSlot( SLOT_SAVED_R20 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R21.
   */
  CheckSlot( SLOT_SAVED_R21 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R22.
   */
  CheckSlot( SLOT_SAVED_R22 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R23.
   */
  CheckSlot( SLOT_SAVED_R23 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R24.
   */
  CheckSlot( SLOT_SAVED_R24 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R25.
   */
  CheckSlot( SLOT_SAVED_R25 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R26.
   */
  CheckSlot( SLOT_SAVED_R26 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R27.
   */
  CheckSlot( SLOT_SAVED_R27 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R28.
   */
  CheckSlot( SLOT_SAVED_R28 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R29.
   */
  CheckSlot( SLOT_SAVED_R29 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R30.
   */
  CheckSlot( SLOT_SAVED_R30 );

  /*
   * Check that the switch saved the bottom 32 bits of the PowerPC register
   * R31.
   */
  CheckSlot( SLOT_SAVED_R31 );

  /*
   * Check that the switch back restored the fields CR2 to CR4 of CR.
   */
  CheckSlot( SLOT_FRAME_CR );

  /*
   * Check that the switch saved the fields CR2 to CR4 of CR.
   */
  CheckSlot( SLOT_SAVED_CR );

  /*
   * Check that the trap after the switch back reached the fatal handler once.
   */
  CheckSlot( SLOT_TRAP );

  /*
   * Check that the switch saved the return address of its call in LR.
   */
  CheckSlot( SLOT_SAVED_LR );

  /*
   * Check that the switch saved the stack pointer R1 of its call.
   */
  CheckSlot( SLOT_SAVED_SP );

  /*
   * Check that the switch back restored the stack pointer R1.
   */
  CheckSlot( SLOT_FRAME_SP );

  /*
   * Check that the heir received the thread pointer of its context.
   */
  CheckSlot( SLOT_HEIR_TP );

  /*
   * Check that the switch back restored the thread pointer of the executing
   * context.
   */
  CheckSlot( SLOT_EXEC_TP );

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  RegisterCheckReport( &context_check );
}

/**
 * @fn void T_case_body_ScoreCpuPowerpcValContext( void )
 */
T_TEST_CASE( ScoreCpuPowerpcValContext )
{
  ScoreCpuPowerpcValContext_Action_0();
}

/** @} */
