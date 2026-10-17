/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuPowerpcValInterrupt
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
 * @defgroup ScoreCpuPowerpcValInterrupt spec:/score/cpu/powerpc/val/interrupt
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that the interrupt processing preserves every register of the
 *   interrupted context.
 *
 * This test case performs the following actions:
 *
 * - Load a distinct pattern into every register the interrupt has to preserve.
 *   Raise the interrupt. Trap, so the frame captures every register. Run this
 *   once for each register with the pattern of that register inverted, then
 *   once unchanged.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R0.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R3.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R4.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R5.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R6.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R7.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R8.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R9.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R10.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R11.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     wrapper-private register R14.
 *
 *   - Check that the interrupt preserved the bottom 32 bits of the PowerPC
 *     call-used register R12.
 *
 *   - Check that the interrupt preserved CR.
 *
 *   - Check that the interrupt preserved XER.
 *
 *   - Check that the interrupt preserved CTR.
 *
 *   - Check that the interrupt preserved LR.
 *
 *   - Check that the handler ran once before the trap.
 *
 *   - Check that the interrupted code continued at its next instruction, so
 *     the trap reports its address.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

extern const char interrupt_trap_label[];

extern const char interrupt_done_label[];

#define WORD UINT64_C( 0xffffffff )

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
  SOURCE_R14,
  SOURCE_R12,
  SOURCE_CR,
  SOURCE_XER,
  SOURCE_CTR,
  SOURCE_LR,
  SOURCE_COUNT,
  SOURCE_PC,
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
  SLOT_R14,
  SLOT_R12,
  SLOT_CR,
  SLOT_XER,
  SLOT_CTR,
  SLOT_LR,
  SLOT_COUNT,
  SLOT_PC,
  SLOT_MAX
} Slot;

static const RegisterCheckSource interrupt_sources[ SOURCE_MAX ] = {
  { "r0", REGISTER_CHECK_INITIAL, WORD },
  { "r3", REGISTER_CHECK_INITIAL, WORD },
  { "r4", REGISTER_CHECK_INITIAL, WORD },
  { "r5", REGISTER_CHECK_INITIAL, WORD },
  { "r6", REGISTER_CHECK_INITIAL, WORD },
  { "r7", REGISTER_CHECK_INITIAL, WORD },
  { "r8", REGISTER_CHECK_INITIAL, WORD },
  { "r9", REGISTER_CHECK_INITIAL, WORD },
  { "r10", REGISTER_CHECK_INITIAL, WORD },
  { "r11", REGISTER_CHECK_INITIAL, WORD },
  { "r14", REGISTER_CHECK_INITIAL, WORD },
  { "r12", REGISTER_CHECK_INITIAL, WORD },
  { "cr", REGISTER_CHECK_INITIAL, UINT64_C( 0xffffffff ) },
  { "xer", REGISTER_CHECK_INITIAL, UINT64_C( 0xe000007f ) },
  { "ctr", REGISTER_CHECK_INITIAL, WORD },
  { "lr", REGISTER_CHECK_INITIAL, WORD },
  { "count", REGISTER_CHECK_EXPECTED, WORD },
  { "pc", REGISTER_CHECK_EXPECTED, WORD }
};

static const RegisterCheckSlot interrupt_slots[ SOURCE_MAX ] = {
  { "r0", SOURCE_R0, WORD },
  { "r3", SOURCE_R3, WORD },
  { "r4", SOURCE_R4, WORD },
  { "r5", SOURCE_R5, WORD },
  { "r6", SOURCE_R6, WORD },
  { "r7", SOURCE_R7, WORD },
  { "r8", SOURCE_R8, WORD },
  { "r9", SOURCE_R9, WORD },
  { "r10", SOURCE_R10, WORD },
  { "r11", SOURCE_R11, WORD },
  { "r14", SOURCE_R14, WORD },
  { "r12", SOURCE_R12, WORD },
  { "cr", SOURCE_CR, UINT64_C( 0xffffffff ) },
  { "xer", SOURCE_XER, UINT64_C( 0xe000007f ) },
  { "ctr", SOURCE_CTR, WORD },
  { "lr", SOURCE_LR, WORD },
  { "count", SOURCE_COUNT, WORD },
  { "pc", SOURCE_PC, WORD }
};

static const uint64_t interrupt_patterns[ SOURCE_MAX ] = {
  UINT64_C( 0x01020304 ),
  UINT64_C( 0x11121314 ),
  UINT64_C( 0x21222324 ),
  UINT64_C( 0x31323334 ),
  UINT64_C( 0x41424344 ),
  UINT64_C( 0x51525354 ),
  UINT64_C( 0x61626364 ),
  UINT64_C( 0x71727374 ),
  UINT64_C( 0x81828384 ),
  UINT64_C( 0x91929394 ),
  UINT64_C( 0xa1a2a3a4 ),
  UINT64_C( 0xb1b2b3b4 ),
  UINT64_C( 0x5a5a5a5a ),
  UINT64_C( 0x40000015 ),
  UINT64_C( 0x31323334 ),
  UINT64_C( 0x41424344 ),
  1,
  1
};

uint64_t interrupt_initial[ SOURCE_MAX ];

static CPU_Exception_frame interrupt_frame;

static volatile uint32_t interrupt_count;

static uint32_t interrupt_count_at_trap;

static CallWithinISRRequest interrupt_request;

/*
 * The block uses r12 as the address of the table and loads r12 last.  It
 * loads CR, XER, CTR and LR through r0 first.
 */
#define ISR_LOAD                       \
  "lis 12, interrupt_initial@ha\n"     \
  "addi 12, 12, interrupt_initial@l\n" \
  "lwz 0, 100(12)\n"                   \
  "mtcr 0\n"                           \
  "lwz 0, 108(12)\n"                   \
  "mtxer 0\n"                          \
  "lwz 0, 116(12)\n"                   \
  "mtctr 0\n"                          \
  "lwz 0, 124(12)\n"                   \
  "mtlr 0\n"                           \
  "lwz 0, 4(12)\n"                     \
  "lwz 3, 12(12)\n"                    \
  "lwz 4, 20(12)\n"                    \
  "lwz 5, 28(12)\n"                    \
  "lwz 6, 36(12)\n"                    \
  "lwz 7, 44(12)\n"                    \
  "lwz 8, 52(12)\n"                    \
  "lwz 9, 60(12)\n"                    \
  "lwz 10, 68(12)\n"                   \
  "lwz 11, 76(12)\n"                   \
  "lwz 14, 84(12)\n"                   \
  "lwz 12, 92(12)\n"

static void InterruptHandler( void *arg )
{
  (void) arg;
  ++interrupt_count;
}

static void InterruptFatal(
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
  interrupt_frame = *frame;
  interrupt_count_at_trap = interrupt_count;
  SetFatalHandler( NULL, NULL );
  frame->EXC_SRR0 = (uintptr_t) interrupt_done_label;
  _CPU_Exception_resume( frame );
}

/*
 * The interrupt is raised while it is masked, so no call happens after the
 * block loads the registers.  The block unmasks it.  The interrupt controller
 * delivers an IPI a few cycles later, so a run of no-operations follows.
 */
static void InterruptRaise( void )
{
  rtems_interrupt_level level;

  memset( &interrupt_frame, 0, sizeof( interrupt_frame ) );
  interrupt_count = 0;
  interrupt_count_at_trap = 0;
  SetFatalHandler( InterruptFatal, NULL );
  rtems_interrupt_local_disable( level );
  CallWithinISRSubmit( &interrupt_request );
  __asm__ volatile( "mfmsr 15\n"
                    "ori 15, 15, 0x8000\n" ISR_LOAD "mtmsr 15\n"
                    ".rept 256\n"
                    "nop\n"
                    ".endr\n"
                    ".globl interrupt_trap_label\n"
                    "interrupt_trap_label:\n"
                    "trap\n"
                    ".globl interrupt_done_label\n"
                    "interrupt_done_label:\n"
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
                      "lr",
                      "ctr",
                      "xer" );
  rtems_interrupt_local_enable( level );
}

static void InterruptRun( RegisterCheck *self, void *arg )
{
  (void) arg;
  InterruptRaise();
  RegisterCheckRecord(
    self,
    SLOT_R0,
    (uint32_t) interrupt_frame.registers[ 0 ],
    interrupt_patterns[ SOURCE_R0 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R3,
    (uint32_t) interrupt_frame.registers[ 3 ],
    interrupt_patterns[ SOURCE_R3 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R4,
    (uint32_t) interrupt_frame.registers[ 4 ],
    interrupt_patterns[ SOURCE_R4 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R5,
    (uint32_t) interrupt_frame.registers[ 5 ],
    interrupt_patterns[ SOURCE_R5 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R6,
    (uint32_t) interrupt_frame.registers[ 6 ],
    interrupt_patterns[ SOURCE_R6 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R7,
    (uint32_t) interrupt_frame.registers[ 7 ],
    interrupt_patterns[ SOURCE_R7 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R8,
    (uint32_t) interrupt_frame.registers[ 8 ],
    interrupt_patterns[ SOURCE_R8 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R9,
    (uint32_t) interrupt_frame.registers[ 9 ],
    interrupt_patterns[ SOURCE_R9 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R10,
    (uint32_t) interrupt_frame.registers[ 10 ],
    interrupt_patterns[ SOURCE_R10 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R11,
    (uint32_t) interrupt_frame.registers[ 11 ],
    interrupt_patterns[ SOURCE_R11 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R14,
    (uint32_t) interrupt_frame.registers[ 14 ],
    interrupt_patterns[ SOURCE_R14 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R12,
    (uint32_t) interrupt_frame.registers[ 12 ],
    interrupt_patterns[ SOURCE_R12 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_CR,
    interrupt_frame.EXC_CR,
    interrupt_patterns[ SOURCE_CR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_XER,
    interrupt_frame.EXC_XER & UINT32_C( 0xe000007f ),
    interrupt_patterns[ SOURCE_XER ]
  );
  RegisterCheckRecord(
    self,
    SLOT_CTR,
    interrupt_frame.EXC_CTR,
    interrupt_patterns[ SOURCE_CTR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_LR,
    interrupt_frame.EXC_LR,
    interrupt_patterns[ SOURCE_LR ]
  );
  RegisterCheckRecord( self, SLOT_COUNT, interrupt_count_at_trap, 1 );
  RegisterCheckRecord(
    self,
    SLOT_PC,
    interrupt_frame.EXC_SRR0 == (uintptr_t) interrupt_trap_label,
    1
  );
}

static RegisterCheck interrupt_check = {
  .sources = interrupt_sources,
  .source_count = SOURCE_MAX,
  .slots = interrupt_slots,
  .slot_count = SOURCE_MAX,
  .initial = interrupt_initial,
  .run = InterruptRun
};

static void CheckSlot( Slot slot )
{
  RegisterCheckVerify( &interrupt_check, slot );
}

/**
 * @brief Load a distinct pattern into every register the interrupt has to
 *   preserve. Raise the interrupt. Trap, so the frame captures every register.
 *   Run this once for each register with the pattern of that register
 *   inverted, then once unchanged.
 */
static void ScoreCpuPowerpcValInterrupt_Action_0( void )
{
  memcpy( interrupt_initial, interrupt_patterns, sizeof( interrupt_initial ) );
  interrupt_request.handler = InterruptHandler;
  interrupt_request.arg = NULL;
  RegisterCheckRun( &interrupt_check );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R0.
   */
  CheckSlot( SLOT_R0 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R3.
   */
  CheckSlot( SLOT_R3 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R4.
   */
  CheckSlot( SLOT_R4 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R5.
   */
  CheckSlot( SLOT_R5 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R6.
   */
  CheckSlot( SLOT_R6 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R7.
   */
  CheckSlot( SLOT_R7 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R8.
   */
  CheckSlot( SLOT_R8 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R9.
   */
  CheckSlot( SLOT_R9 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R10.
   */
  CheckSlot( SLOT_R10 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R11.
   */
  CheckSlot( SLOT_R11 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * wrapper-private register R14.
   */
  CheckSlot( SLOT_R14 );

  /*
   * Check that the interrupt preserved the bottom 32 bits of the PowerPC
   * call-used register R12.
   */
  CheckSlot( SLOT_R12 );

  /*
   * Check that the interrupt preserved CR.
   */
  CheckSlot( SLOT_CR );

  /*
   * Check that the interrupt preserved XER.
   */
  CheckSlot( SLOT_XER );

  /*
   * Check that the interrupt preserved CTR.
   */
  CheckSlot( SLOT_CTR );

  /*
   * Check that the interrupt preserved LR.
   */
  CheckSlot( SLOT_LR );

  /*
   * Check that the handler ran once before the trap.
   */
  CheckSlot( SLOT_COUNT );

  /*
   * Check that the interrupted code continued at its next instruction, so the
   * trap reports its address.
   */
  CheckSlot( SLOT_PC );

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  RegisterCheckReport( &interrupt_check );
}

/**
 * @fn void T_case_body_ScoreCpuPowerpcValInterrupt( void )
 */
T_TEST_CASE( ScoreCpuPowerpcValInterrupt )
{
  ScoreCpuPowerpcValInterrupt_Action_0();
}

/** @} */
