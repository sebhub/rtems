/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuPowerpcValSpeContext
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
#include <rtems/score/thread.h>

#include "tx-register-check.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuPowerpcValSpeContext \
 *   spec:/score/cpu/powerpc/val/spe-context
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that the context switch saves and restores the SPE state of a
 *   thread.
 *
 * This test case performs the following actions:
 *
 * - Load a distinct pattern into both halves of every call-saved register and
 *   into SPEFSCR. Call the context switch directly. The heir switches back.
 *   Trap, so the frame reports every register. Run this once for each value
 *   with the pattern of that value inverted, then once unchanged.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R14.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R15.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R16.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R17.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R18.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R19.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R20.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R21.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R22.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R23.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R24.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R25.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R26.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R27.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R28.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R29.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R30.
 *
 *   - Check that the switch restored the top 32 bits of the PowerPC register
 *     R31.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R14.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R15.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R16.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R17.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R18.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R19.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R20.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R21.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R22.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R23.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R24.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R25.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R26.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R27.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R28.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R29.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R30.
 *
 *   - Check that the switch saved the top 32 bits of the PowerPC register R31.
 *
 *   - Check that the switch saved SPEFSCR.
 *
 *   - Check that the switch back restored SPEFSCR.
 *
 *   - Check that the heir received the SPEFSCR of its context.
 *
 *   - Check that the context initialization set no exception enable of
 *     SPEFSCR.
 *
 *   - Check that the trap after the switch back reached the fatal handler
 *     once.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

#if defined( __SPE__ )
extern const char spe_context_done_label[];

#define WORD UINT64_C( 0xffffffff )

/* These are the top 32 bits of a 64-bit general-purpose register. */
#define TOP UINT64_C( 0xffffffff00000000 )

/*
 * These are the sticky bits FINXS, FINVS, FDBZS and FUNFS and the rounding mode
 * of SPEFSCR.  The mask covers no enable bit, so no SPE operation traps.
 */
#define SPEFSCR_MASK UINT64_C( 0x003c0003 )

/* These are FINXE, FINVE, FDBZE, FUNFE and FOVFE of SPEFSCR. */
#define SPEFSCR_ENABLES UINT32_C( 0x0000007c )

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
  SOURCE_SPEFSCR,
  SOURCE_HEIR_SPEFSCR,
  SOURCE_INIT,
  SOURCE_TRAP,
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
  SLOT_SAVED_SPEFSCR,
  SLOT_FRAME_SPEFSCR,
  SLOT_HEIR_SPEFSCR,
  SLOT_INIT_SPEFSCR,
  SLOT_TRAP,
  SLOT_MAX
} Slot;

RTEMS_STATIC_ASSERT( SOURCE_R31 == 17, source_r31 );
RTEMS_STATIC_ASSERT( SOURCE_SPEFSCR == 18, source_spefscr );

static const RegisterCheckSource spe_context_sources[ SOURCE_MAX ] = {
  { "r14 top", REGISTER_CHECK_INITIAL, TOP },
  { "r15 top", REGISTER_CHECK_INITIAL, TOP },
  { "r16 top", REGISTER_CHECK_INITIAL, TOP },
  { "r17 top", REGISTER_CHECK_INITIAL, TOP },
  { "r18 top", REGISTER_CHECK_INITIAL, TOP },
  { "r19 top", REGISTER_CHECK_INITIAL, TOP },
  { "r20 top", REGISTER_CHECK_INITIAL, TOP },
  { "r21 top", REGISTER_CHECK_INITIAL, TOP },
  { "r22 top", REGISTER_CHECK_INITIAL, TOP },
  { "r23 top", REGISTER_CHECK_INITIAL, TOP },
  { "r24 top", REGISTER_CHECK_INITIAL, TOP },
  { "r25 top", REGISTER_CHECK_INITIAL, TOP },
  { "r26 top", REGISTER_CHECK_INITIAL, TOP },
  { "r27 top", REGISTER_CHECK_INITIAL, TOP },
  { "r28 top", REGISTER_CHECK_INITIAL, TOP },
  { "r29 top", REGISTER_CHECK_INITIAL, TOP },
  { "r30 top", REGISTER_CHECK_INITIAL, TOP },
  { "r31 top", REGISTER_CHECK_INITIAL, TOP },
  { "spefscr", REGISTER_CHECK_INITIAL, SPEFSCR_MASK },
  { "heir spefscr", REGISTER_CHECK_INITIAL, SPEFSCR_MASK },
  { "init", REGISTER_CHECK_EXPECTED, WORD },
  { "trap", REGISTER_CHECK_EXPECTED, WORD }
};

static const RegisterCheckSlot spe_context_slots[ SLOT_MAX ] = {
  { "frame r14 top", SOURCE_R14, TOP },
  { "frame r15 top", SOURCE_R15, TOP },
  { "frame r16 top", SOURCE_R16, TOP },
  { "frame r17 top", SOURCE_R17, TOP },
  { "frame r18 top", SOURCE_R18, TOP },
  { "frame r19 top", SOURCE_R19, TOP },
  { "frame r20 top", SOURCE_R20, TOP },
  { "frame r21 top", SOURCE_R21, TOP },
  { "frame r22 top", SOURCE_R22, TOP },
  { "frame r23 top", SOURCE_R23, TOP },
  { "frame r24 top", SOURCE_R24, TOP },
  { "frame r25 top", SOURCE_R25, TOP },
  { "frame r26 top", SOURCE_R26, TOP },
  { "frame r27 top", SOURCE_R27, TOP },
  { "frame r28 top", SOURCE_R28, TOP },
  { "frame r29 top", SOURCE_R29, TOP },
  { "frame r30 top", SOURCE_R30, TOP },
  { "frame r31 top", SOURCE_R31, TOP },
  { "saved r14 top", SOURCE_R14, TOP },
  { "saved r15 top", SOURCE_R15, TOP },
  { "saved r16 top", SOURCE_R16, TOP },
  { "saved r17 top", SOURCE_R17, TOP },
  { "saved r18 top", SOURCE_R18, TOP },
  { "saved r19 top", SOURCE_R19, TOP },
  { "saved r20 top", SOURCE_R20, TOP },
  { "saved r21 top", SOURCE_R21, TOP },
  { "saved r22 top", SOURCE_R22, TOP },
  { "saved r23 top", SOURCE_R23, TOP },
  { "saved r24 top", SOURCE_R24, TOP },
  { "saved r25 top", SOURCE_R25, TOP },
  { "saved r26 top", SOURCE_R26, TOP },
  { "saved r27 top", SOURCE_R27, TOP },
  { "saved r28 top", SOURCE_R28, TOP },
  { "saved r29 top", SOURCE_R29, TOP },
  { "saved r30 top", SOURCE_R30, TOP },
  { "saved r31 top", SOURCE_R31, TOP },
  { "saved spefscr", SOURCE_SPEFSCR, SPEFSCR_MASK },
  { "frame spefscr", SOURCE_SPEFSCR, SPEFSCR_MASK },
  { "heir spefscr", SOURCE_HEIR_SPEFSCR, SPEFSCR_MASK },
  { "init spefscr", SOURCE_INIT, WORD },
  { "trap", SOURCE_TRAP, WORD }
};

static const uint64_t spe_context_patterns[ SOURCE_MAX ] = {
  UINT64_C( 0xa1a2a3a411121314 ),
  UINT64_C( 0xa2a3a4a512131415 ),
  UINT64_C( 0xa3a4a5a613141516 ),
  UINT64_C( 0xa4a5a6a714151617 ),
  UINT64_C( 0xa5a6a7a815161718 ),
  UINT64_C( 0xa6a7a8a916171819 ),
  UINT64_C( 0xa7a8a9aa1718191a ),
  UINT64_C( 0xa8a9aaab18191a1b ),
  UINT64_C( 0xa9aaabac191a1b1c ),
  UINT64_C( 0xaaabacad1a1b1c1d ),
  UINT64_C( 0xabacadae1b1c1d1e ),
  UINT64_C( 0xacadaeaf1c1d1e1f ),
  UINT64_C( 0xadaeafb01d1e1f20 ),
  UINT64_C( 0xaeafb0b11e1f2021 ),
  UINT64_C( 0xafb0b1b21f202122 ),
  UINT64_C( 0xb0b1b2b320212223 ),
  UINT64_C( 0xb1b2b3b421222324 ),
  UINT64_C( 0xb2b3b4b522232425 ),
  UINT64_C( 0x00280002 ),
  UINT64_C( 0x00180001 ),
  0,
  1
};

uint64_t spe_context_initial[ SOURCE_MAX ] RTEMS_ALIGNED( 8 );

static CPU_Exception_frame spe_context_frame;

static volatile int spe_context_trap;

Context_Control spe_context_executing;

Context_Control spe_context_heir;

uint32_t spe_context_heir_spefscr;

static uint32_t spe_context_init_spefscr;

static uint32_t spe_context_spefscr;

static uint64_t spe_context_heir_stack[ 512 ] RTEMS_ALIGNED(
  CPU_STACK_ALIGNMENT
);

/*
 * The block loads r3 with the address of the table, sets SPEFSCR through r0,
 * and loads both halves of each register with evldd.
 */
#define CTX_LOAD                       \
  "lis 3, spe_context_initial@ha\n"    \
  "addi 3, 3, spe_context_initial@l\n" \
  "lwz 0, 148(3)\n"                    \
  "mtspr 512, 0\n"                     \
  "evldd 14, 0(3)\n"                   \
  "evldd 15, 8(3)\n"                   \
  "evldd 16, 16(3)\n"                  \
  "evldd 17, 24(3)\n"                  \
  "evldd 18, 32(3)\n"                  \
  "evldd 19, 40(3)\n"                  \
  "evldd 20, 48(3)\n"                  \
  "evldd 21, 56(3)\n"                  \
  "evldd 22, 64(3)\n"                  \
  "evldd 23, 72(3)\n"                  \
  "evldd 24, 80(3)\n"                  \
  "evldd 25, 88(3)\n"                  \
  "evldd 26, 96(3)\n"                  \
  "evldd 27, 104(3)\n"                 \
  "evldd 28, 112(3)\n"                 \
  "evldd 29, 120(3)\n"                 \
  "evldd 30, 128(3)\n"                 \
  "evldd 31, 136(3)\n"

static void SpeContextFatal(
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
  spe_context_frame = *frame;
  ++spe_context_trap;
  SetFatalHandler( NULL, NULL );
  frame->EXC_SPEFSCR = spe_context_spefscr;
  frame->EXC_SRR0 = (uintptr_t) spe_context_done_label;
  _CPU_Exception_resume( frame );
}

/*
 * The heir records its SPEFSCR and switches straight back.  It is assembly, so
 * no prologue saves a register of the context under test.
 */
void SpeContextHeirEntry( void );

__asm__( "  .globl SpeContextHeirEntry\n"
         "  .type SpeContextHeirEntry, @function\n"
         "SpeContextHeirEntry:\n"
         "  mfspr 6, 512\n"
         "  lis 5, spe_context_heir_spefscr@ha\n"
         "  stw 6, spe_context_heir_spefscr@l(5)\n"
         "  lis 3, spe_context_heir@ha\n"
         "  addi 3, 3, spe_context_heir@l\n"
         "  lis 4, spe_context_executing@ha\n"
         "  addi 4, 4, spe_context_executing@l\n"
         "  b _CPU_Context_switch\n" );

static void SpeContextRunSwitch( void )
{
  spe_context_trap = 0;
  memset( &spe_context_frame, 0, sizeof( spe_context_frame ) );
  memset( &spe_context_executing, 0, sizeof( spe_context_executing ) );
  ppc_get_context( &spe_context_executing )->tp =
    ppc_get_context( &GetExecuting()->Registers )->tp;
  _CPU_Context_Initialize(
    &spe_context_heir,
    spe_context_heir_stack,
    sizeof( spe_context_heir_stack ),
    0,
    SpeContextHeirEntry,
    true,
    NULL
  );
  spe_context_init_spefscr = ppc_get_context( &spe_context_heir )->spefscr;
  ppc_get_context( &spe_context_heir )->spefscr = (uint32_t)
    spe_context_initial[ SOURCE_HEIR_SPEFSCR ];
  __asm__ volatile( "mfspr %0, 512"
                    : "=r"( spe_context_spefscr ) );
  SetFatalHandler( SpeContextFatal, NULL );

  __asm__ volatile( CTX_LOAD "lis 3, spe_context_executing@ha\n"
                             "addi 3, 3, spe_context_executing@l\n"
                             "lis 4, spe_context_heir@ha\n"
                             "addi 4, 4, spe_context_heir@l\n"
                             "bl _CPU_Context_switch\n"
                             "trap\n"
                             ".globl spe_context_done_label\n"
                             "spe_context_done_label:\n"
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
}

static void SpeContextRun( RegisterCheck *self, void *arg )
{
  const ppc_context *ctx;

  (void) arg;
  SpeContextRunSwitch();
  ctx = ppc_get_context( &spe_context_executing );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R14,
    spe_context_frame.registers[ 14 ] & TOP,
    spe_context_patterns[ SOURCE_R14 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R15,
    spe_context_frame.registers[ 15 ] & TOP,
    spe_context_patterns[ SOURCE_R15 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R16,
    spe_context_frame.registers[ 16 ] & TOP,
    spe_context_patterns[ SOURCE_R16 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R17,
    spe_context_frame.registers[ 17 ] & TOP,
    spe_context_patterns[ SOURCE_R17 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R18,
    spe_context_frame.registers[ 18 ] & TOP,
    spe_context_patterns[ SOURCE_R18 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R19,
    spe_context_frame.registers[ 19 ] & TOP,
    spe_context_patterns[ SOURCE_R19 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R20,
    spe_context_frame.registers[ 20 ] & TOP,
    spe_context_patterns[ SOURCE_R20 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R21,
    spe_context_frame.registers[ 21 ] & TOP,
    spe_context_patterns[ SOURCE_R21 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R22,
    spe_context_frame.registers[ 22 ] & TOP,
    spe_context_patterns[ SOURCE_R22 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R23,
    spe_context_frame.registers[ 23 ] & TOP,
    spe_context_patterns[ SOURCE_R23 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R24,
    spe_context_frame.registers[ 24 ] & TOP,
    spe_context_patterns[ SOURCE_R24 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R25,
    spe_context_frame.registers[ 25 ] & TOP,
    spe_context_patterns[ SOURCE_R25 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R26,
    spe_context_frame.registers[ 26 ] & TOP,
    spe_context_patterns[ SOURCE_R26 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R27,
    spe_context_frame.registers[ 27 ] & TOP,
    spe_context_patterns[ SOURCE_R27 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R28,
    spe_context_frame.registers[ 28 ] & TOP,
    spe_context_patterns[ SOURCE_R28 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R29,
    spe_context_frame.registers[ 29 ] & TOP,
    spe_context_patterns[ SOURCE_R29 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R30,
    spe_context_frame.registers[ 30 ] & TOP,
    spe_context_patterns[ SOURCE_R30 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_R31,
    spe_context_frame.registers[ 31 ] & TOP,
    spe_context_patterns[ SOURCE_R31 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R14,
    ctx->gpr14 & TOP,
    spe_context_patterns[ SOURCE_R14 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R15,
    ctx->gpr15 & TOP,
    spe_context_patterns[ SOURCE_R15 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R16,
    ctx->gpr16 & TOP,
    spe_context_patterns[ SOURCE_R16 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R17,
    ctx->gpr17 & TOP,
    spe_context_patterns[ SOURCE_R17 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R18,
    ctx->gpr18 & TOP,
    spe_context_patterns[ SOURCE_R18 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R19,
    ctx->gpr19 & TOP,
    spe_context_patterns[ SOURCE_R19 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R20,
    ctx->gpr20 & TOP,
    spe_context_patterns[ SOURCE_R20 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R21,
    ctx->gpr21 & TOP,
    spe_context_patterns[ SOURCE_R21 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R22,
    ctx->gpr22 & TOP,
    spe_context_patterns[ SOURCE_R22 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R23,
    ctx->gpr23 & TOP,
    spe_context_patterns[ SOURCE_R23 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R24,
    ctx->gpr24 & TOP,
    spe_context_patterns[ SOURCE_R24 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R25,
    ctx->gpr25 & TOP,
    spe_context_patterns[ SOURCE_R25 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R26,
    ctx->gpr26 & TOP,
    spe_context_patterns[ SOURCE_R26 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R27,
    ctx->gpr27 & TOP,
    spe_context_patterns[ SOURCE_R27 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R28,
    ctx->gpr28 & TOP,
    spe_context_patterns[ SOURCE_R28 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R29,
    ctx->gpr29 & TOP,
    spe_context_patterns[ SOURCE_R29 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R30,
    ctx->gpr30 & TOP,
    spe_context_patterns[ SOURCE_R30 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_R31,
    ctx->gpr31 & TOP,
    spe_context_patterns[ SOURCE_R31 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_SPEFSCR,
    ctx->spefscr & SPEFSCR_MASK,
    spe_context_patterns[ SOURCE_SPEFSCR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_SPEFSCR,
    spe_context_frame.EXC_SPEFSCR & SPEFSCR_MASK,
    spe_context_patterns[ SOURCE_SPEFSCR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_HEIR_SPEFSCR,
    spe_context_heir_spefscr & SPEFSCR_MASK,
    spe_context_patterns[ SOURCE_HEIR_SPEFSCR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_INIT_SPEFSCR,
    spe_context_init_spefscr & SPEFSCR_ENABLES,
    0
  );
  RegisterCheckRecord( self, SLOT_TRAP, (uint64_t) spe_context_trap, 1 );
}

static RegisterCheck spe_context_check = {
  .sources = spe_context_sources,
  .source_count = SOURCE_MAX,
  .slots = spe_context_slots,
  .slot_count = SLOT_MAX,
  .initial = spe_context_initial,
  .run = SpeContextRun
};

static void CheckSlot( Slot slot )
{
  RegisterCheckVerify( &spe_context_check, slot );
}
#endif

/**
 * @brief Load a distinct pattern into both halves of every call-saved register
 *   and into SPEFSCR. Call the context switch directly. The heir switches
 *   back. Trap, so the frame reports every register. Run this once for each
 *   value with the pattern of that value inverted, then once unchanged.
 */
static void ScoreCpuPowerpcValSpeContext_Action_0( void )
{
  #if defined( __SPE__ )
  memcpy(
    spe_context_initial,
    spe_context_patterns,
    sizeof( spe_context_initial )
  );
  RegisterCheckRun( &spe_context_check );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R14.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R14 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R15.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R15 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R16.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R16 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R17.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R17 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R18.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R18 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R19.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R19 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R20.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R20 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R21.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R21 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R22.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R22 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R23.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R23 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R24.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R24 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R25.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R25 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R26.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R26 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R27.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R27 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R28.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R28 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R29.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R29 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R30.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R30 );
  #endif

  /*
   * Check that the switch restored the top 32 bits of the PowerPC register
   * R31.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_R31 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R14.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R14 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R15.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R15 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R16.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R16 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R17.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R17 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R18.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R18 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R19.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R19 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R20.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R20 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R21.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R21 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R22.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R22 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R23.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R23 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R24.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R24 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R25.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R25 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R26.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R26 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R27.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R27 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R28.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R28 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R29.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R29 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R30.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R30 );
  #endif

  /*
   * Check that the switch saved the top 32 bits of the PowerPC register R31.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_R31 );
  #endif

  /*
   * Check that the switch saved SPEFSCR.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SAVED_SPEFSCR );
  #endif

  /*
   * Check that the switch back restored SPEFSCR.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_FRAME_SPEFSCR );
  #endif

  /*
   * Check that the heir received the SPEFSCR of its context.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_HEIR_SPEFSCR );
  #endif

  /*
   * Check that the context initialization set no exception enable of SPEFSCR.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_INIT_SPEFSCR );
  #endif

  /*
   * Check that the trap after the switch back reached the fatal handler once.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_TRAP );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  #if defined( __SPE__ )
  RegisterCheckReport( &spe_context_check );
  #endif
}

/**
 * @fn void T_case_body_ScoreCpuPowerpcValSpeContext( void )
 */
T_TEST_CASE( ScoreCpuPowerpcValSpeContext )
{
  ScoreCpuPowerpcValSpeContext_Action_0();
}

/** @} */
