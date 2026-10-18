/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuPowerpcValSpeInterrupt
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

#include "tx-register-check.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuPowerpcValSpeInterrupt \
 *   spec:/score/cpu/powerpc/val/spe-interrupt
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that the interrupt processing preserves the SPE state of the
 *   interrupted context.
 *
 * This test case performs the following actions:
 *
 * - Load a distinct pattern into both halves of every register the interrupt
 *   has to preserve, into ACC and into SPEFSCR. Raise the interrupt. Trap, so
 *   the frame captures every register. Run this once for each value with the
 *   pattern of that value inverted, then once unchanged.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R0.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R3.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R4.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R5.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R6.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R7.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R8.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R9.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R10.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R11.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R12.
 *
 *   - Check that the interrupt preserved the top 32 bits of the PowerPC
 *     register R14.
 *
 *   - Check that the interrupt preserved ACC.
 *
 *   - Check that the interrupt preserved SPEFSCR.
 *
 *   - Check that the handler ran once before the trap.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

#if defined( __SPE__ )
#define WORD UINT64_C( 0xffffffff )

/* These are the top 32 bits of a 64-bit general-purpose register. */
#define TOP UINT64_C( 0xffffffff00000000 )

/*
 * These are the sticky bits FINXS, FINVS, FDBZS and FUNFS and the rounding mode
 * of SPEFSCR.  The mask covers no enable bit, so no SPE operation traps.
 */
#define SPEFSCR_MASK UINT64_C( 0x003c0003 )

typedef enum {
  SOURCE_R0,
  SOURCE_R3,
  SOURCE_R4,
  SOURCE_R5,
  SOURCE_R6,
  SOURCE_R7,
  SOURCE_R8,
  SOURCE_R9,
  SOURCE_R10,
  SOURCE_R11,
  SOURCE_R12,
  SOURCE_R14,
  SOURCE_ACC,
  SOURCE_SPEFSCR,
  SOURCE_COUNT,
  SOURCE_MAX
} Source;

typedef enum {
  SLOT_R0,
  SLOT_R3,
  SLOT_R4,
  SLOT_R5,
  SLOT_R6,
  SLOT_R7,
  SLOT_R8,
  SLOT_R9,
  SLOT_R10,
  SLOT_R11,
  SLOT_R12,
  SLOT_R14,
  SLOT_ACC,
  SLOT_SPEFSCR,
  SLOT_COUNT,
  SLOT_MAX
} Slot;

static const RegisterCheckSource spe_interrupt_sources[ SOURCE_MAX ] = {
  { "r0 top", REGISTER_CHECK_INITIAL, TOP },
  { "r3 top", REGISTER_CHECK_INITIAL, TOP },
  { "r4 top", REGISTER_CHECK_INITIAL, TOP },
  { "r5 top", REGISTER_CHECK_INITIAL, TOP },
  { "r6 top", REGISTER_CHECK_INITIAL, TOP },
  { "r7 top", REGISTER_CHECK_INITIAL, TOP },
  { "r8 top", REGISTER_CHECK_INITIAL, TOP },
  { "r9 top", REGISTER_CHECK_INITIAL, TOP },
  { "r10 top", REGISTER_CHECK_INITIAL, TOP },
  { "r11 top", REGISTER_CHECK_INITIAL, TOP },
  { "r12 top", REGISTER_CHECK_INITIAL, TOP },
  { "r14 top", REGISTER_CHECK_INITIAL, TOP },
  { "acc", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "spefscr", REGISTER_CHECK_INITIAL, SPEFSCR_MASK },
  { "count", REGISTER_CHECK_EXPECTED, WORD }
};

static const RegisterCheckSlot spe_interrupt_slots[ SOURCE_MAX ] = {
  { "r0 top", SOURCE_R0, TOP },
  { "r3 top", SOURCE_R3, TOP },
  { "r4 top", SOURCE_R4, TOP },
  { "r5 top", SOURCE_R5, TOP },
  { "r6 top", SOURCE_R6, TOP },
  { "r7 top", SOURCE_R7, TOP },
  { "r8 top", SOURCE_R8, TOP },
  { "r9 top", SOURCE_R9, TOP },
  { "r10 top", SOURCE_R10, TOP },
  { "r11 top", SOURCE_R11, TOP },
  { "r12 top", SOURCE_R12, TOP },
  { "r14 top", SOURCE_R14, TOP },
  { "acc", SOURCE_ACC, UINT64_MAX },
  { "spefscr", SOURCE_SPEFSCR, SPEFSCR_MASK },
  { "count", SOURCE_COUNT, WORD }
};

static const uint64_t spe_interrupt_patterns[ SOURCE_MAX ] = {
  UINT64_C( 0xb1b2b3b421222324 ),
  UINT64_C( 0xb2b3b4b522232425 ),
  UINT64_C( 0xb3b4b5b623242526 ),
  UINT64_C( 0xb4b5b6b724252627 ),
  UINT64_C( 0xb5b6b7b825262728 ),
  UINT64_C( 0xb6b7b8b926272829 ),
  UINT64_C( 0xb7b8b9ba2728292a ),
  UINT64_C( 0xb8b9babb28292a2b ),
  UINT64_C( 0xb9babbbc292a2b2c ),
  UINT64_C( 0xbabbbcbd2a2b2c2d ),
  UINT64_C( 0xbbbcbdbe2b2c2d2e ),
  UINT64_C( 0xbcbdbebf2c2d2e2f ),
  UINT64_C( 0x0f0e0d0c0b0a0908 ),
  UINT64_C( 0x00280002 ),
  1
};

uint64_t spe_interrupt_initial[ SOURCE_MAX ] RTEMS_ALIGNED( 8 );

static CPU_Exception_frame spe_interrupt_frame;

static volatile uint32_t spe_interrupt_count;

static uint32_t spe_interrupt_count_at_trap;

static uint32_t spe_interrupt_spefscr;

static CallWithinISRRequest spe_interrupt_request;

extern const char spe_interrupt_done_label[];

#define ISR_LOAD                           \
  "lis 12, spe_interrupt_initial@ha\n"     \
  "addi 12, 12, spe_interrupt_initial@l\n" \
  "lwz 0, 108(12)\n"                       \
  "mtspr 512, 0\n"                         \
  "evldd 0, 96(12)\n"                      \
  "evmra 0, 0\n"                           \
  "evldd 0, 0(12)\n"                       \
  "evldd 3, 8(12)\n"                       \
  "evldd 4, 16(12)\n"                      \
  "evldd 5, 24(12)\n"                      \
  "evldd 6, 32(12)\n"                      \
  "evldd 7, 40(12)\n"                      \
  "evldd 8, 48(12)\n"                      \
  "evldd 9, 56(12)\n"                      \
  "evldd 10, 64(12)\n"                     \
  "evldd 11, 72(12)\n"                     \
  "evldd 14, 88(12)\n"                     \
  "evldd 12, 80(12)\n"

static void SpeInterruptHandler( void *arg )
{
  (void) arg;
  ++spe_interrupt_count;
}

static void SpeInterruptFatal(
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
  spe_interrupt_frame = *frame;
  spe_interrupt_count_at_trap = spe_interrupt_count;
  SetFatalHandler( NULL, NULL );
  frame->EXC_SPEFSCR = spe_interrupt_spefscr;
  frame->EXC_SRR0 = (uintptr_t) spe_interrupt_done_label;
  _CPU_Exception_resume( frame );
}

/*
 * The interrupt is raised while it is masked, so no call happens after the
 * block loads the registers.  The block unmasks it.  The interrupt controller
 * delivers an IPI a few cycles later, so a run of no-operations follows.
 */
static void SpeInterruptRun( RegisterCheck *self, void *arg )
{
  rtems_interrupt_level level;

  (void) arg;
  memset( &spe_interrupt_frame, 0, sizeof( spe_interrupt_frame ) );
  spe_interrupt_count = 0;
  spe_interrupt_count_at_trap = 0;
  __asm__ volatile( "mfspr %0, 512"
                    : "=r"( spe_interrupt_spefscr ) );
  SetFatalHandler( SpeInterruptFatal, NULL );
  rtems_interrupt_local_disable( level );
  CallWithinISRSubmit( &spe_interrupt_request );
  __asm__ volatile( ISR_LOAD "wrteei 1\n"
                             ".rept 256\n"
                             "nop\n"
                             ".endr\n"
                             "trap\n"
                             ".globl spe_interrupt_done_label\n"
                             "spe_interrupt_done_label:\n"
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
                      "ctr",
                      "xer" );
  rtems_interrupt_local_enable( level );
  RegisterCheckRecord(
    self,
    SLOT_R0,
    spe_interrupt_frame.registers[ 0 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R0 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R3,
    spe_interrupt_frame.registers[ 3 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R3 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R4,
    spe_interrupt_frame.registers[ 4 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R4 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R5,
    spe_interrupt_frame.registers[ 5 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R5 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R6,
    spe_interrupt_frame.registers[ 6 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R6 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R7,
    spe_interrupt_frame.registers[ 7 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R7 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R8,
    spe_interrupt_frame.registers[ 8 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R8 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R9,
    spe_interrupt_frame.registers[ 9 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R9 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R10,
    spe_interrupt_frame.registers[ 10 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R10 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R11,
    spe_interrupt_frame.registers[ 11 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R11 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R12,
    spe_interrupt_frame.registers[ 12 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R12 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R14,
    spe_interrupt_frame.registers[ 14 ] & TOP,
    spe_interrupt_patterns[ SOURCE_R14 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_ACC,
    spe_interrupt_frame.EXC_ACC,
    spe_interrupt_patterns[ SOURCE_ACC ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SPEFSCR,
    spe_interrupt_frame.EXC_SPEFSCR & SPEFSCR_MASK,
    spe_interrupt_patterns[ SOURCE_SPEFSCR ]
  );
  RegisterCheckRecord( self, SLOT_COUNT, spe_interrupt_count_at_trap, 1 );
}

static RegisterCheck spe_interrupt_check = {
  .sources = spe_interrupt_sources,
  .source_count = SOURCE_MAX,
  .slots = spe_interrupt_slots,
  .slot_count = SOURCE_MAX,
  .initial = spe_interrupt_initial,
  .run = SpeInterruptRun
};

static void CheckSlot( Slot slot )
{
  RegisterCheckVerify( &spe_interrupt_check, slot );
}
#endif

/**
 * @brief Load a distinct pattern into both halves of every register the
 *   interrupt has to preserve, into ACC and into SPEFSCR. Raise the interrupt.
 *   Trap, so the frame captures every register. Run this once for each value
 *   with the pattern of that value inverted, then once unchanged.
 */
static void ScoreCpuPowerpcValSpeInterrupt_Action_0( void )
{
  #if defined( __SPE__ )
  memcpy(
    spe_interrupt_initial,
    spe_interrupt_patterns,
    sizeof( spe_interrupt_initial )
  );
  spe_interrupt_request.handler = SpeInterruptHandler;
  spe_interrupt_request.arg = NULL;
  RegisterCheckRun( &spe_interrupt_check );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R0.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R0 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R3.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R3 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R4.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R4 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R5.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R5 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R6.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R6 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R7.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R7 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R8.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R8 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R9.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R9 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R10.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R10 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R11.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R11 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R12.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R12 );
  #endif

  /*
   * Check that the interrupt preserved the top 32 bits of the PowerPC register
   * R14.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R14 );
  #endif

  /*
   * Check that the interrupt preserved ACC.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_ACC );
  #endif

  /*
   * Check that the interrupt preserved SPEFSCR.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SPEFSCR );
  #endif

  /*
   * Check that the handler ran once before the trap.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_COUNT );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  #if defined( __SPE__ )
  RegisterCheckReport( &spe_interrupt_check );
  #endif
}

/**
 * @fn void T_case_body_ScoreCpuPowerpcValSpeInterrupt( void )
 */
T_TEST_CASE( ScoreCpuPowerpcValSpeInterrupt )
{
  ScoreCpuPowerpcValSpeInterrupt_Action_0();
}

/** @} */
