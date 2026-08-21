/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuArmValInterrupt
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
#include <stdint.h>
#include <string.h>
#include <rtems/score/cpuimpl.h>

#include "tx-register-check.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuArmValInterrupt spec:/score/cpu/arm/val/interrupt
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that interrupt processing preserves the registers of the
 *   interrupted context.
 *
 * The action needs a BSP which supplies the TM27 support. A BSP without it
 * raises no interrupt.
 *
 * This test case performs the following actions:
 *
 * - Load a distinct pattern into every register the interrupt has to preserve.
 *   Raise the interrupt. Trap, so the frame captures every register. Run this
 *   once for each register with the pattern of that register inverted, then
 *   once unchanged.
 *
 *   - Check that the interrupt preserved the ARM core register R0.
 *
 *   - Check that the interrupt preserved the ARM core register R1.
 *
 *   - Check that the interrupt preserved the ARM core register R2.
 *
 *   - Check that the interrupt preserved the ARM core register R3.
 *
 *   - Check that the interrupt preserved the ARM core register R12.
 *
 *   - Check that the interrupt preserved the wrapper-private ARM core register
 *     R7.
 *
 *   - Check that the interrupt preserved the wrapper-private ARM core register
 *     R9.
 *
 *   - Check that the interrupt happened before the trap. Without it the test
 *     proves nothing about interrupt processing.
 *
 *   - Check that the interrupt preserved the ARM FPU register D0.
 *
 *   - Check that the interrupt preserved the ARM FPU register D1.
 *
 *   - Check that the interrupt preserved the ARM FPU register D2.
 *
 *   - Check that the interrupt preserved the ARM FPU register D3.
 *
 *   - Check that the interrupt preserved the ARM FPU register D4.
 *
 *   - Check that the interrupt preserved the ARM FPU register D5.
 *
 *   - Check that the interrupt preserved the ARM FPU register D6.
 *
 *   - Check that the interrupt preserved the ARM FPU register D7.
 *
 *   - Check that the interrupt preserved the ARM FPU register D16.
 *
 *   - Check that the interrupt preserved the ARM FPU register D17.
 *
 *   - Check that the interrupt preserved the ARM FPU register D18.
 *
 *   - Check that the interrupt preserved the ARM FPU register D19.
 *
 *   - Check that the interrupt preserved the ARM FPU register D20.
 *
 *   - Check that the interrupt preserved the ARM FPU register D21.
 *
 *   - Check that the interrupt preserved the ARM FPU register D22.
 *
 *   - Check that the interrupt preserved the ARM FPU register D23.
 *
 *   - Check that the interrupt preserved the ARM FPU register D24.
 *
 *   - Check that the interrupt preserved the ARM FPU register D25.
 *
 *   - Check that the interrupt preserved the ARM FPU register D26.
 *
 *   - Check that the interrupt preserved the ARM FPU register D27.
 *
 *   - Check that the interrupt preserved the ARM FPU register D28.
 *
 *   - Check that the interrupt preserved the ARM FPU register D29.
 *
 *   - Check that the interrupt preserved the ARM FPU register D30.
 *
 *   - Check that the interrupt preserved the ARM FPU register D31.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

#define _RTEMS_TMTEST27
#include <tm27.h>

extern const char interrupt_done_label[];

typedef enum {
  SOURCE_R0,
  SOURCE_R1,
  SOURCE_R2,
  SOURCE_R3,
  SOURCE_R12,
  SOURCE_R7,
  SOURCE_R9,
#ifdef ARM_MULTILIB_VFP
  SOURCE_D0,
  SOURCE_D1,
  SOURCE_D2,
  SOURCE_D3,
  SOURCE_D4,
  SOURCE_D5,
  SOURCE_D6,
  SOURCE_D7,
#ifdef ARM_MULTILIB_VFP_D32
  SOURCE_D16,
  SOURCE_D17,
  SOURCE_D18,
  SOURCE_D19,
  SOURCE_D20,
  SOURCE_D21,
  SOURCE_D22,
  SOURCE_D23,
  SOURCE_D24,
  SOURCE_D25,
  SOURCE_D26,
  SOURCE_D27,
  SOURCE_D28,
  SOURCE_D29,
  SOURCE_D30,
  SOURCE_D31,
#endif
#endif
  SOURCE_COUNT_AT_TRAP,
  SOURCE_MAX
} Source;

typedef enum {
  SLOT_R0,
  SLOT_R1,
  SLOT_R2,
  SLOT_R3,
  SLOT_R12,
  SLOT_R7,
  SLOT_R9,
#ifdef ARM_MULTILIB_VFP
  SLOT_D0,
  SLOT_D1,
  SLOT_D2,
  SLOT_D3,
  SLOT_D4,
  SLOT_D5,
  SLOT_D6,
  SLOT_D7,
#ifdef ARM_MULTILIB_VFP_D32
  SLOT_D16,
  SLOT_D17,
  SLOT_D18,
  SLOT_D19,
  SLOT_D20,
  SLOT_D21,
  SLOT_D22,
  SLOT_D23,
  SLOT_D24,
  SLOT_D25,
  SLOT_D26,
  SLOT_D27,
  SLOT_D28,
  SLOT_D29,
  SLOT_D30,
  SLOT_D31,
#endif
#endif
  SLOT_COUNT_AT_TRAP,
  SLOT_MAX
} Slot;

/* The block loads each register from the byte offset eight times its source. */
RTEMS_STATIC_ASSERT( SOURCE_R9 == 6, source_r9 );
#ifdef ARM_MULTILIB_VFP
RTEMS_STATIC_ASSERT( SOURCE_D0 == 7, source_d0 );
#ifdef ARM_MULTILIB_VFP_D32
RTEMS_STATIC_ASSERT( SOURCE_D16 == 15, source_d16 );
#endif
#endif

#define WORD UINT64_C( 0xffffffff )

static const RegisterCheckSource interrupt_sources[ SOURCE_MAX ] = {
  { "r0", REGISTER_CHECK_INITIAL, WORD },
  { "r1", REGISTER_CHECK_INITIAL, WORD },
  { "r2", REGISTER_CHECK_INITIAL, WORD },
  { "r3", REGISTER_CHECK_INITIAL, WORD },
  { "r12", REGISTER_CHECK_INITIAL, WORD },
  { "r7", REGISTER_CHECK_INITIAL, WORD },
  { "r9", REGISTER_CHECK_INITIAL, WORD },
#ifdef ARM_MULTILIB_VFP
  { "d0", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d1", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d2", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d3", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d4", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d5", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d6", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d7", REGISTER_CHECK_INITIAL, UINT64_MAX },
#ifdef ARM_MULTILIB_VFP_D32
  { "d16", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d17", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d18", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d19", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d20", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d21", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d22", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d23", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d24", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d25", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d26", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d27", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d28", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d29", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d30", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d31", REGISTER_CHECK_INITIAL, UINT64_MAX },
#endif
#endif
  { "count at trap", REGISTER_CHECK_EXPECTED, WORD }
};

static const RegisterCheckSlot interrupt_slots[ SLOT_MAX ] = {
  { "r0", SOURCE_R0, WORD },
  { "r1", SOURCE_R1, WORD },
  { "r2", SOURCE_R2, WORD },
  { "r3", SOURCE_R3, WORD },
  { "r12", SOURCE_R12, WORD },
  { "r7", SOURCE_R7, WORD },
  { "r9", SOURCE_R9, WORD },
#ifdef ARM_MULTILIB_VFP
  { "d0", SOURCE_D0, UINT64_MAX },
  { "d1", SOURCE_D1, UINT64_MAX },
  { "d2", SOURCE_D2, UINT64_MAX },
  { "d3", SOURCE_D3, UINT64_MAX },
  { "d4", SOURCE_D4, UINT64_MAX },
  { "d5", SOURCE_D5, UINT64_MAX },
  { "d6", SOURCE_D6, UINT64_MAX },
  { "d7", SOURCE_D7, UINT64_MAX },
#ifdef ARM_MULTILIB_VFP_D32
  { "d16", SOURCE_D16, UINT64_MAX },
  { "d17", SOURCE_D17, UINT64_MAX },
  { "d18", SOURCE_D18, UINT64_MAX },
  { "d19", SOURCE_D19, UINT64_MAX },
  { "d20", SOURCE_D20, UINT64_MAX },
  { "d21", SOURCE_D21, UINT64_MAX },
  { "d22", SOURCE_D22, UINT64_MAX },
  { "d23", SOURCE_D23, UINT64_MAX },
  { "d24", SOURCE_D24, UINT64_MAX },
  { "d25", SOURCE_D25, UINT64_MAX },
  { "d26", SOURCE_D26, UINT64_MAX },
  { "d27", SOURCE_D27, UINT64_MAX },
  { "d28", SOURCE_D28, UINT64_MAX },
  { "d29", SOURCE_D29, UINT64_MAX },
  { "d30", SOURCE_D30, UINT64_MAX },
  { "d31", SOURCE_D31, UINT64_MAX },
#endif
#endif
  { "count at trap", SOURCE_COUNT_AT_TRAP, WORD }
};

static const uint64_t interrupt_patterns[ SOURCE_MAX ] = {
  0x71727374,
  0x72737475,
  0x73747576,
  0x74757677,
  0x75767778,
  0x81828384,
  0x82838485,
#ifdef ARM_MULTILIB_VFP
  0x9192939495969798,
  0x9293949596979899,
  0x939495969798999a,
  0x9495969798999a9b,
  0x95969798999a9b9c,
  0x969798999a9b9c9d,
  0x9798999a9b9c9d9e,
  0x98999a9b9c9d9e9f,
#ifdef ARM_MULTILIB_VFP_D32
  0xa1a2a3a4a5a6a7a8,
  0xa2a3a4a5a6a7a8a9,
  0xa3a4a5a6a7a8a9aa,
  0xa4a5a6a7a8a9aaab,
  0xa5a6a7a8a9aaabac,
  0xa6a7a8a9aaabacad,
  0xa7a8a9aaabacadae,
  0xa8a9aaabacadaeaf,
  0xa9aaabacadaeafb0,
  0xaaabacadaeafb0b1,
  0xabacadaeafb0b1b2,
  0xacadaeafb0b1b2b3,
  0xadaeafb0b1b2b3b4,
  0xaeafb0b1b2b3b4b5,
  0xafb0b1b2b3b4b5b6,
  0xb0b1b2b3b4b5b6b7,
#endif
#endif
};

uint64_t interrupt_initial[ SOURCE_MAX ];

static CPU_Exception_frame interrupt_frame;

#ifdef ARM_MULTILIB_VFP
static ARM_VFP_context interrupt_vfp_context;
#endif

static volatile int interrupt_count;

static int interrupt_count_at_trap;

volatile uint32_t interrupt_restore_r7;

volatile uint32_t interrupt_restore_r9;

static CallWithinISRRequest interrupt_request;

static void InterruptHandler( void *arg )
{
  (void) arg;
  ++interrupt_count;
}

#ifdef ARM_MULTILIB_VFP_D32
#define ISR_LOAD_VFP_D32      \
  "vldr.64 d16, [r0, #120]\n" \
  "vldr.64 d17, [r0, #128]\n" \
  "vldr.64 d18, [r0, #136]\n" \
  "vldr.64 d19, [r0, #144]\n" \
  "vldr.64 d20, [r0, #152]\n" \
  "vldr.64 d21, [r0, #160]\n" \
  "vldr.64 d22, [r0, #168]\n" \
  "vldr.64 d23, [r0, #176]\n" \
  "vldr.64 d24, [r0, #184]\n" \
  "vldr.64 d25, [r0, #192]\n" \
  "vldr.64 d26, [r0, #200]\n" \
  "vldr.64 d27, [r0, #208]\n" \
  "vldr.64 d28, [r0, #216]\n" \
  "vldr.64 d29, [r0, #224]\n" \
  "vldr.64 d30, [r0, #232]\n" \
  "vldr.64 d31, [r0, #240]\n"

#else
#define ISR_LOAD_VFP_D32
#endif

#ifdef ARM_MULTILIB_VFP
#define ISR_LOAD_VFP         \
  "vldr.64 d0, [r0, #56]\n"  \
  "vldr.64 d1, [r0, #64]\n"  \
  "vldr.64 d2, [r0, #72]\n"  \
  "vldr.64 d3, [r0, #80]\n"  \
  "vldr.64 d4, [r0, #88]\n"  \
  "vldr.64 d5, [r0, #96]\n"  \
  "vldr.64 d6, [r0, #104]\n" \
  "vldr.64 d7, [r0, #112]\n" ISR_LOAD_VFP_D32
#else
#define ISR_LOAD_VFP
#endif

#ifdef ARM_MULTILIB_VFP
#ifdef ARM_MULTILIB_VFP_D32
#define ISR_CLOBBER_VFP                                                   \
  , "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d16", "d17", "d18",  \
    "d19", "d20", "d21", "d22", "d23", "d24", "d25", "d26", "d27", "d28", \
    "d29", "d30", "d31"
#else
#define ISR_CLOBBER_VFP , "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7"
#endif
#else
#define ISR_CLOBBER_VFP
#endif

/*
 * The resume keeps the patterns in the registers.  The fatal handler puts
 * back r7 and r9, because r7 may be the frame pointer.
 */
#define ISR_CLOBBER "r0", "r1", "r2", "r3", "r10", "r12" ISR_CLOBBER_VFP

#if defined( ARM_MULTILIB_ARCH_V7M )
/*
 * The interrupt level of rtems_interrupt_local_disable() is a BASEPRI
 * value.  It masks the interrupt of the test.
 */
#define ISR_UNMASK_LEVEL \
  "mov r10, #0\n"        \
  "msr basepri, r10\n"
#else
#define ISR_UNMASK_LEVEL "cpsie i\n"
#endif

/* The synchronization takes the pending interrupt before the trap. */
#ifdef ARM_MULTILIB_HAS_BARRIER_INSTRUCTIONS
#define ISR_UNMASK ISR_UNMASK_LEVEL "isb\n"
#else
#define ISR_UNMASK ISR_UNMASK_LEVEL
#endif

#define ISR_SAVE                    \
  "ldr r0, =interrupt_restore_r7\n" \
  "str r7, [r0]\n"                  \
  "ldr r0, =interrupt_restore_r9\n" \
  "str r9, [r0]\n"

/* The block loads r0 last, because r0 holds the address of the table. */
#define ISR_LOAD                                                   \
  ISR_SAVE                                                         \
  "ldr r0, =interrupt_initial\n" ISR_LOAD_VFP "ldr r1, [r0, #8]\n" \
  "ldr r2, [r0, #16]\n"                                            \
  "ldr r3, [r0, #24]\n"                                            \
  "ldr r12, [r0, #32]\n"                                           \
  "ldr r7, [r0, #40]\n"                                            \
  "ldr r9, [r0, #48]\n"                                            \
  "ldr r0, [r0, #0]\n"

/*
 * The trap after the interrupt captures every register.  The frame is the
 * store-out, so no register under test addresses it.
 */
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
#ifdef ARM_MULTILIB_VFP
  interrupt_vfp_context = *frame->vfp_context;
#endif
  interrupt_count_at_trap = interrupt_count;
  SetFatalHandler( NULL, NULL );
  frame->register_r7 = interrupt_restore_r7;
  frame->register_r9 = interrupt_restore_r9;
  frame->register_pc = (void *) interrupt_done_label;
  _CPU_Exception_resume( frame );
}

static void InterruptRun( RegisterCheck *self, void *arg )
{
  rtems_interrupt_level level;
  const uint64_t       *patterns;

  (void) arg;
  memset( &interrupt_frame, 0, sizeof( interrupt_frame ) );
#ifdef ARM_MULTILIB_VFP
  memset( &interrupt_vfp_context, 0, sizeof( interrupt_vfp_context ) );
#endif
  interrupt_count = 0;
  interrupt_count_at_trap = 0;
  SetFatalHandler( InterruptFatal, NULL );

  /*
   * The interrupt is raised while it is masked, so no call happens after
   * the block loads the registers.  The block unmasks it, and the
   * interrupt is taken there.
   */
  rtems_interrupt_local_disable( level );
  CallWithinISRSubmit( &interrupt_request );
  __asm__ volatile( ISR_LOAD ISR_UNMASK "udf #0\n"
                                        ".globl interrupt_done_label\n"
                                        "interrupt_done_label:\n"
                    :
                    :
                    : "memory", ISR_CLOBBER );
  rtems_interrupt_local_enable( level );

  patterns = interrupt_patterns;
  RegisterCheckRecord(
    self,
    SLOT_R0,
    interrupt_frame.register_r0,
    patterns[ SOURCE_R0 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R1,
    interrupt_frame.register_r1,
    patterns[ SOURCE_R1 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R2,
    interrupt_frame.register_r2,
    patterns[ SOURCE_R2 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R3,
    interrupt_frame.register_r3,
    patterns[ SOURCE_R3 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R12,
    interrupt_frame.register_r12,
    patterns[ SOURCE_R12 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R7,
    interrupt_frame.register_r7,
    patterns[ SOURCE_R7 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_R9,
    interrupt_frame.register_r9,
    patterns[ SOURCE_R9 ]
  );

#ifdef ARM_MULTILIB_VFP
  {
    const uint64_t *d;
    size_t          i;

    d = &interrupt_vfp_context.register_d0;

    for ( i = 0; i < 8; ++i ) {
      RegisterCheckRecord(
        self,
        SLOT_D0 + i,
        d[ i ],
        patterns[ SOURCE_D0 + i ]
      );
    }

#ifdef ARM_MULTILIB_VFP_D32
    d = &interrupt_vfp_context.register_d16;

    for ( i = 0; i < 16; ++i ) {
      RegisterCheckRecord(
        self,
        SLOT_D16 + i,
        d[ i ],
        patterns[ SOURCE_D16 + i ]
      );
    }
#endif
  }
#endif

  RegisterCheckRecord(
    self,
    SLOT_COUNT_AT_TRAP,
    (uint64_t) interrupt_count_at_trap,
    1
  );
}

static RegisterCheck interrupt_check = {
  .sources = interrupt_sources,
  .source_count = SOURCE_MAX,
  .slots = interrupt_slots,
  .slot_count = SLOT_MAX,
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
static void ScoreCpuArmValInterrupt_Action_0( void )
{
  memcpy( interrupt_initial, interrupt_patterns, sizeof( interrupt_initial ) );
  interrupt_request.handler = InterruptHandler;
  interrupt_request.arg = NULL;

  #ifdef TM27_INTERRUPT_VECTOR_DEFAULT
  RegisterCheckRun( &interrupt_check );
  #endif

  /*
   * Check that the interrupt preserved the ARM core register R0.
   */
  CheckSlot( SLOT_R0 );

  /*
   * Check that the interrupt preserved the ARM core register R1.
   */
  CheckSlot( SLOT_R1 );

  /*
   * Check that the interrupt preserved the ARM core register R2.
   */
  CheckSlot( SLOT_R2 );

  /*
   * Check that the interrupt preserved the ARM core register R3.
   */
  CheckSlot( SLOT_R3 );

  /*
   * Check that the interrupt preserved the ARM core register R12.
   */
  CheckSlot( SLOT_R12 );

  /*
   * Check that the interrupt preserved the wrapper-private ARM core register
   * R7.
   */
  CheckSlot( SLOT_R7 );

  /*
   * Check that the interrupt preserved the wrapper-private ARM core register
   * R9.
   */
  CheckSlot( SLOT_R9 );

  /*
   * Check that the interrupt happened before the trap. Without it the test
   * proves nothing about interrupt processing.
   */
  CheckSlot( SLOT_COUNT_AT_TRAP );

  /*
   * Check that the interrupt preserved the ARM FPU register D0.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D0 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D1.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D1 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D2.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D2 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D3.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D3 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D4.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D4 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D5.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D5 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D6.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D6 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D7.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D7 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D16.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D16 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D17.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D17 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D18.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D18 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D19.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D19 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D20.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D20 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D21.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D21 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D22.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D22 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D23.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D23 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D24.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D24 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D25.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D25 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D26.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D26 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D27.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D27 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D28.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D28 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D29.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D29 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D30.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D30 );
  #endif

  /*
   * Check that the interrupt preserved the ARM FPU register D31.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D31 );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  RegisterCheckReport( &interrupt_check );
}

/**
 * @fn void T_case_body_ScoreCpuArmValInterrupt( void )
 */
T_TEST_CASE( ScoreCpuArmValInterrupt )
{
  ScoreCpuArmValInterrupt_Action_0();
}

/** @} */
