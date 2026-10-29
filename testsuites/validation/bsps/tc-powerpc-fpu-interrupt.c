/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuPowerpcValFpuInterrupt
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
 * @defgroup ScoreCpuPowerpcValFpuInterrupt \
 *   spec:/score/cpu/powerpc/val/fpu-interrupt
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that interrupt processing preserves the floating-point state
 *   of the interrupted context.
 *
 * This test case performs the following actions:
 *
 * - Load a distinct pattern into every call-used floating-point register and
 *   into FPSCR. Raise the interrupt. The handler overwrites each of these
 *   registers. Trap, so the frame captures every register. Run this once for
 *   each value with the pattern of that value inverted, then once unchanged.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F0.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F1.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F2.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F3.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F4.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F5.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F6.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F7.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F8.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F9.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F10.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F11.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F12.
 *
 *   - Check that the interrupt preserved the PowerPC floating-point register
 *     F13.
 *
 *   - Check that the interrupt preserved FPSCR.
 *
 *   - Check that the interrupt occurred exactly once before the trap.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

#if defined( PPC_MULTILIB_FPU )
#define WORD UINT64_C( 0xffffffff )

/*
 * These are the sticky bits OX, UX, ZX and XX and the rounding mode RN of
 * FPSCR.  The mask covers no enable bit, so no operation traps.
 */
#define FPSCR_MASK UINT64_C( 0x1e000003 )

typedef enum {
  SOURCE_F0,
  SOURCE_F1,
  SOURCE_F2,
  SOURCE_F3,
  SOURCE_F4,
  SOURCE_F5,
  SOURCE_F6,
  SOURCE_F7,
  SOURCE_F8,
  SOURCE_F9,
  SOURCE_F10,
  SOURCE_F11,
  SOURCE_F12,
  SOURCE_F13,
  SOURCE_FPSCR,
  SOURCE_COUNT,
  SOURCE_MAX
} Source;

typedef enum {
  SLOT_F0,
  SLOT_F1,
  SLOT_F2,
  SLOT_F3,
  SLOT_F4,
  SLOT_F5,
  SLOT_F6,
  SLOT_F7,
  SLOT_F8,
  SLOT_F9,
  SLOT_F10,
  SLOT_F11,
  SLOT_F12,
  SLOT_F13,
  SLOT_FPSCR,
  SLOT_COUNT,
  SLOT_MAX
} Slot;

static const RegisterCheckSource fpu_interrupt_sources[ SOURCE_MAX ] = {
  { "f0", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f1", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f2", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f3", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f4", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f5", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f6", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f7", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f8", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f9", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f10", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f11", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f12", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f13", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "fpscr", REGISTER_CHECK_INITIAL, FPSCR_MASK },
  { "count", REGISTER_CHECK_EXPECTED, WORD }
};

static const RegisterCheckSlot fpu_interrupt_slots[ SLOT_MAX ] = {
  { "f0", SOURCE_F0, UINT64_MAX },
  { "f1", SOURCE_F1, UINT64_MAX },
  { "f2", SOURCE_F2, UINT64_MAX },
  { "f3", SOURCE_F3, UINT64_MAX },
  { "f4", SOURCE_F4, UINT64_MAX },
  { "f5", SOURCE_F5, UINT64_MAX },
  { "f6", SOURCE_F6, UINT64_MAX },
  { "f7", SOURCE_F7, UINT64_MAX },
  { "f8", SOURCE_F8, UINT64_MAX },
  { "f9", SOURCE_F9, UINT64_MAX },
  { "f10", SOURCE_F10, UINT64_MAX },
  { "f11", SOURCE_F11, UINT64_MAX },
  { "f12", SOURCE_F12, UINT64_MAX },
  { "f13", SOURCE_F13, UINT64_MAX },
  { "fpscr", SOURCE_FPSCR, FPSCR_MASK },
  { "count", SOURCE_COUNT, WORD }
};

static const uint64_t fpu_interrupt_patterns[ SOURCE_MAX ] = {
  UINT64_C( 0xd1d2d3d431323334 ),
  UINT64_C( 0xd2d3d4d532333435 ),
  UINT64_C( 0xd3d4d5d633343536 ),
  UINT64_C( 0xd4d5d6d734353637 ),
  UINT64_C( 0xd5d6d7d835363738 ),
  UINT64_C( 0xd6d7d8d936373839 ),
  UINT64_C( 0xd7d8d9da3738393a ),
  UINT64_C( 0xd8d9dadb38393a3b ),
  UINT64_C( 0xd9dadbdc393a3b3c ),
  UINT64_C( 0xdadbdcdd3a3b3c3d ),
  UINT64_C( 0xdbdcddde3b3c3d3e ),
  UINT64_C( 0xdcdddedf3c3d3e3f ),
  UINT64_C( 0xdddedfe03d3e3f40 ),
  UINT64_C( 0xdedfe0e13e3f4041 ),
  UINT64_C( 0x0a000002 ),
  1
};

RTEMS_STATIC_ASSERT( SOURCE_F13 == 13, source_f13 );
RTEMS_STATIC_ASSERT( SOURCE_FPSCR == 14, source_fpscr );

uint64_t fpu_interrupt_initial[ SOURCE_MAX ] RTEMS_ALIGNED( 8 );

/*
 * The handler loads these values.  Each one differs from the pattern and from
 * the inverted pattern of its register.
 */
uint64_t fpu_interrupt_handler_values[ SOURCE_COUNT ] RTEMS_ALIGNED( 8 ) = {
  UINT64_C( 0x5152535431323334 ),
  UINT64_C( 0x5253545532333435 ),
  UINT64_C( 0x5354555633343536 ),
  UINT64_C( 0x5455565734353637 ),
  UINT64_C( 0x5556575835363738 ),
  UINT64_C( 0x5657585936373839 ),
  UINT64_C( 0x5758595a3738393a ),
  UINT64_C( 0x58595a5b38393a3b ),
  UINT64_C( 0x595a5b5c393a3b3c ),
  UINT64_C( 0x5a5b5c5d3a3b3c3d ),
  UINT64_C( 0x5b5c5d5e3b3c3d3e ),
  UINT64_C( 0x5c5d5e5f3c3d3e3f ),
  UINT64_C( 0x5d5e5f603d3e3f40 ),
  UINT64_C( 0x5e5f60613e3f4041 ),
  UINT64_C( 0x10000001 )
};

static CPU_Exception_frame fpu_interrupt_frame;

static volatile uint32_t fpu_interrupt_count;

static uint32_t fpu_interrupt_count_at_trap;

static uint64_t fpu_interrupt_fpscr;

static CallWithinISRRequest fpu_interrupt_request;

extern const char fpu_interrupt_done_label[];

static uint64_t Bits( double value )
{
  uint64_t bits;

  memcpy( &bits, &value, sizeof( bits ) );
  return bits;
}

static uint64_t GetFPSCR( void )
{
  double value;

  __asm__ volatile( "mffs %0"
                    : "=f"( value ) );
  return Bits( value );
}

#define ISR_LOAD                           \
  "lis 12, fpu_interrupt_initial@ha\n"     \
  "addi 12, 12, fpu_interrupt_initial@l\n" \
  "lfd 0, 112(12)\n"                       \
  "mtfsf 255, 0\n"                         \
  "lfd 0, 0(12)\n"                         \
  "lfd 1, 8(12)\n"                         \
  "lfd 2, 16(12)\n"                        \
  "lfd 3, 24(12)\n"                        \
  "lfd 4, 32(12)\n"                        \
  "lfd 5, 40(12)\n"                        \
  "lfd 6, 48(12)\n"                        \
  "lfd 7, 56(12)\n"                        \
  "lfd 8, 64(12)\n"                        \
  "lfd 9, 72(12)\n"                        \
  "lfd 10, 80(12)\n"                       \
  "lfd 11, 88(12)\n"                       \
  "lfd 12, 96(12)\n"                       \
  "lfd 13, 104(12)\n"

#define HANDLER_LOAD                              \
  "lis 12, fpu_interrupt_handler_values@ha\n"     \
  "addi 12, 12, fpu_interrupt_handler_values@l\n" \
  "lfd 0, 112(12)\n"                              \
  "mtfsf 255, 0\n"                                \
  "lfd 0, 0(12)\n"                                \
  "lfd 1, 8(12)\n"                                \
  "lfd 2, 16(12)\n"                               \
  "lfd 3, 24(12)\n"                               \
  "lfd 4, 32(12)\n"                               \
  "lfd 5, 40(12)\n"                               \
  "lfd 6, 48(12)\n"                               \
  "lfd 7, 56(12)\n"                               \
  "lfd 8, 64(12)\n"                               \
  "lfd 9, 72(12)\n"                               \
  "lfd 10, 80(12)\n"                              \
  "lfd 11, 88(12)\n"                              \
  "lfd 12, 96(12)\n"                              \
  "lfd 13, 104(12)\n"

/*
 * The handler overwrites each call-used floating-point register and FPSCR, so
 * an interrupt which restores none of them fails the check.
 */
static void FpuInterruptHandler( void *arg )
{
  (void) arg;
  ++fpu_interrupt_count;
  __asm__ volatile( HANDLER_LOAD
                    :
                    :
                    : "memory",
                      "r12",
                      "fr0",
                      "fr1",
                      "fr2",
                      "fr3",
                      "fr4",
                      "fr5",
                      "fr6",
                      "fr7",
                      "fr8",
                      "fr9",
                      "fr10",
                      "fr11",
                      "fr12",
                      "fr13" );
}

static void FpuInterruptFatal(
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
  fpu_interrupt_frame = *frame;
  fpu_interrupt_count_at_trap = fpu_interrupt_count;
  SetFatalHandler( NULL, NULL );
  frame->FPSCR = fpu_interrupt_fpscr;
  frame->EXC_SRR0 = (uintptr_t) fpu_interrupt_done_label;
  _CPU_Exception_resume( frame );
}

/*
 * The interrupt is raised while it is masked, so no call happens after the
 * block loads the registers.  The block unmasks it.  The interrupt controller
 * delivers an IPI a few cycles later, so a run of no-operations follows.
 */
static void FpuInterruptRun( RegisterCheck *self, void *arg )
{
  rtems_interrupt_level level;

  (void) arg;
  memset( &fpu_interrupt_frame, 0, sizeof( fpu_interrupt_frame ) );
  fpu_interrupt_count = 0;
  fpu_interrupt_count_at_trap = 0;
  fpu_interrupt_fpscr = GetFPSCR();
  SetFatalHandler( FpuInterruptFatal, NULL );
  rtems_interrupt_local_disable( level );
  CallWithinISRSubmit( &fpu_interrupt_request );
  __asm__ volatile( ISR_LOAD "wrteei 1\n"
                             ".rept 256\n"
                             "nop\n"
                             ".endr\n"
                             "trap\n"
                             ".globl fpu_interrupt_done_label\n"
                             "fpu_interrupt_done_label:\n"
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
                      "xer",
                      "fr0",
                      "fr1",
                      "fr2",
                      "fr3",
                      "fr4",
                      "fr5",
                      "fr6",
                      "fr7",
                      "fr8",
                      "fr9",
                      "fr10",
                      "fr11",
                      "fr12",
                      "fr13" );
  rtems_interrupt_local_enable( level );
  RegisterCheckRecord(
    self,
    SLOT_F0,
    Bits( fpu_interrupt_frame.F0 ),
    fpu_interrupt_patterns[ SOURCE_F0 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F1,
    Bits( fpu_interrupt_frame.F1 ),
    fpu_interrupt_patterns[ SOURCE_F1 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F2,
    Bits( fpu_interrupt_frame.F2 ),
    fpu_interrupt_patterns[ SOURCE_F2 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F3,
    Bits( fpu_interrupt_frame.F3 ),
    fpu_interrupt_patterns[ SOURCE_F3 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F4,
    Bits( fpu_interrupt_frame.F4 ),
    fpu_interrupt_patterns[ SOURCE_F4 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F5,
    Bits( fpu_interrupt_frame.F5 ),
    fpu_interrupt_patterns[ SOURCE_F5 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F6,
    Bits( fpu_interrupt_frame.F6 ),
    fpu_interrupt_patterns[ SOURCE_F6 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F7,
    Bits( fpu_interrupt_frame.F7 ),
    fpu_interrupt_patterns[ SOURCE_F7 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F8,
    Bits( fpu_interrupt_frame.F8 ),
    fpu_interrupt_patterns[ SOURCE_F8 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F9,
    Bits( fpu_interrupt_frame.F9 ),
    fpu_interrupt_patterns[ SOURCE_F9 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F10,
    Bits( fpu_interrupt_frame.F10 ),
    fpu_interrupt_patterns[ SOURCE_F10 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F11,
    Bits( fpu_interrupt_frame.F11 ),
    fpu_interrupt_patterns[ SOURCE_F11 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F12,
    Bits( fpu_interrupt_frame.F12 ),
    fpu_interrupt_patterns[ SOURCE_F12 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F13,
    Bits( fpu_interrupt_frame.F13 ),
    fpu_interrupt_patterns[ SOURCE_F13 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FPSCR,
    fpu_interrupt_frame.FPSCR & FPSCR_MASK,
    fpu_interrupt_patterns[ SOURCE_FPSCR ]
  );
  RegisterCheckRecord( self, SLOT_COUNT, fpu_interrupt_count_at_trap, 1 );
}

static RegisterCheck fpu_interrupt_check = {
  .sources = fpu_interrupt_sources,
  .source_count = SOURCE_MAX,
  .slots = fpu_interrupt_slots,
  .slot_count = SLOT_MAX,
  .initial = fpu_interrupt_initial,
  .run = FpuInterruptRun
};

static void CheckSlot( Slot slot )
{
  RegisterCheckVerify( &fpu_interrupt_check, slot );
}
#endif

/**
 * @brief Load a distinct pattern into every call-used floating-point register
 *   and into FPSCR. Raise the interrupt. The handler overwrites each of these
 *   registers. Trap, so the frame captures every register. Run this once for
 *   each value with the pattern of that value inverted, then once unchanged.
 */
static void ScoreCpuPowerpcValFpuInterrupt_Action_0( void )
{
  #if defined( PPC_MULTILIB_FPU )
  memcpy(
    fpu_interrupt_initial,
    fpu_interrupt_patterns,
    sizeof( fpu_interrupt_initial )
  );
  fpu_interrupt_request.handler = FpuInterruptHandler;
  fpu_interrupt_request.arg = NULL;
  RegisterCheckRun( &fpu_interrupt_check );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register F0.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F0 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register F1.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F1 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register F2.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F2 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register F3.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F3 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register F4.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F4 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register F5.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F5 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register F6.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F6 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register F7.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F7 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register F8.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F8 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register F9.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F9 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register
   * F10.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F10 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register
   * F11.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F11 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register
   * F12.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F12 );
  #endif

  /*
   * Check that the interrupt preserved the PowerPC floating-point register
   * F13.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F13 );
  #endif

  /*
   * Check that the interrupt preserved FPSCR.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FPSCR );
  #endif

  /*
   * Check that the interrupt occurred exactly once before the trap.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_COUNT );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  #if defined( PPC_MULTILIB_FPU )
  RegisterCheckReport( &fpu_interrupt_check );
  #endif
}

/**
 * @fn void T_case_body_ScoreCpuPowerpcValFpuInterrupt( void )
 */
T_TEST_CASE( ScoreCpuPowerpcValFpuInterrupt )
{
  ScoreCpuPowerpcValFpuInterrupt_Action_0();
}

/** @} */
