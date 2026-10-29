/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuPowerpcValFpuExceptionFrame
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
 * @defgroup ScoreCpuPowerpcValFpuExceptionFrame \
 *   spec:/score/cpu/powerpc/val/fpu-exception-frame
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that the default exception handler saves the floating-point
 *   state to the exception frame.
 *
 * This test case performs the following actions:
 *
 * - Load a distinct pattern into every floating-point register and into FPSCR.
 *   Trap. Run this once for each value with the pattern of that value
 *   inverted, then once unchanged.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F0.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F1.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F2.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F3.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F4.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F5.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F6.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F7.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F8.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F9.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F10.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F11.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F12.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F13.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F14.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F15.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F16.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F17.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F18.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F19.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F20.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F21.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F22.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F23.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F24.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F25.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F26.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F27.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F28.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F29.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F30.
 *
 *   - Check that the exception frame holds the PowerPC floating-point register
 *     F31.
 *
 *   - Check that the exception frame holds FPSCR.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

#if defined( PPC_MULTILIB_FPU )
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
  SOURCE_F14,
  SOURCE_F15,
  SOURCE_F16,
  SOURCE_F17,
  SOURCE_F18,
  SOURCE_F19,
  SOURCE_F20,
  SOURCE_F21,
  SOURCE_F22,
  SOURCE_F23,
  SOURCE_F24,
  SOURCE_F25,
  SOURCE_F26,
  SOURCE_F27,
  SOURCE_F28,
  SOURCE_F29,
  SOURCE_F30,
  SOURCE_F31,
  SOURCE_FPSCR,
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
  SLOT_F14,
  SLOT_F15,
  SLOT_F16,
  SLOT_F17,
  SLOT_F18,
  SLOT_F19,
  SLOT_F20,
  SLOT_F21,
  SLOT_F22,
  SLOT_F23,
  SLOT_F24,
  SLOT_F25,
  SLOT_F26,
  SLOT_F27,
  SLOT_F28,
  SLOT_F29,
  SLOT_F30,
  SLOT_F31,
  SLOT_FPSCR,
  SLOT_MAX
} Slot;

static const RegisterCheckSource fpu_exception_sources[ SOURCE_MAX ] = {
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
  { "f14", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f15", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f16", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f17", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f18", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f19", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f20", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f21", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f22", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f23", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f24", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f25", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f26", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f27", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f28", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f29", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f30", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "f31", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "fpscr", REGISTER_CHECK_INITIAL, FPSCR_MASK }
};

static const RegisterCheckSlot fpu_exception_slots[ SLOT_MAX ] = {
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
  { "f14", SOURCE_F14, UINT64_MAX },
  { "f15", SOURCE_F15, UINT64_MAX },
  { "f16", SOURCE_F16, UINT64_MAX },
  { "f17", SOURCE_F17, UINT64_MAX },
  { "f18", SOURCE_F18, UINT64_MAX },
  { "f19", SOURCE_F19, UINT64_MAX },
  { "f20", SOURCE_F20, UINT64_MAX },
  { "f21", SOURCE_F21, UINT64_MAX },
  { "f22", SOURCE_F22, UINT64_MAX },
  { "f23", SOURCE_F23, UINT64_MAX },
  { "f24", SOURCE_F24, UINT64_MAX },
  { "f25", SOURCE_F25, UINT64_MAX },
  { "f26", SOURCE_F26, UINT64_MAX },
  { "f27", SOURCE_F27, UINT64_MAX },
  { "f28", SOURCE_F28, UINT64_MAX },
  { "f29", SOURCE_F29, UINT64_MAX },
  { "f30", SOURCE_F30, UINT64_MAX },
  { "f31", SOURCE_F31, UINT64_MAX },
  { "fpscr", SOURCE_FPSCR, FPSCR_MASK }
};

static const uint64_t fpu_exception_patterns[ SOURCE_MAX ] = {
  UINT64_C( 0xe1e2e3e431323334 ),
  UINT64_C( 0xe2e3e4e532333435 ),
  UINT64_C( 0xe3e4e5e633343536 ),
  UINT64_C( 0xe4e5e6e734353637 ),
  UINT64_C( 0xe5e6e7e835363738 ),
  UINT64_C( 0xe6e7e8e936373839 ),
  UINT64_C( 0xe7e8e9ea3738393a ),
  UINT64_C( 0xe8e9eaeb38393a3b ),
  UINT64_C( 0xe9eaebec393a3b3c ),
  UINT64_C( 0xeaebeced3a3b3c3d ),
  UINT64_C( 0xebecedee3b3c3d3e ),
  UINT64_C( 0xecedeeef3c3d3e3f ),
  UINT64_C( 0xedeeeff03d3e3f40 ),
  UINT64_C( 0xeeeff0f13e3f4041 ),
  UINT64_C( 0xeff0f1f23f404142 ),
  UINT64_C( 0xf0f1f2f340414243 ),
  UINT64_C( 0xf1f2f3f441424344 ),
  UINT64_C( 0xf2f3f4f542434445 ),
  UINT64_C( 0xf3f4f5f643444546 ),
  UINT64_C( 0xf4f5f6f744454647 ),
  UINT64_C( 0xf5f6f7f845464748 ),
  UINT64_C( 0xf6f7f8f946474849 ),
  UINT64_C( 0xf7f8f9fa4748494a ),
  UINT64_C( 0xf8f9fafb48494a4b ),
  UINT64_C( 0xf9fafbfc494a4b4c ),
  UINT64_C( 0xfafbfcfd4a4b4c4d ),
  UINT64_C( 0xfbfcfdfe4b4c4d4e ),
  UINT64_C( 0xfcfdfeff4c4d4e4f ),
  UINT64_C( 0xfdff00004d4e4f50 ),
  UINT64_C( 0xff0001014e4f5051 ),
  UINT64_C( 0x000102024f505152 ),
  UINT64_C( 0x0102030350515253 ),
  UINT64_C( 0x0a000002 )
};

RTEMS_STATIC_ASSERT( SOURCE_FPSCR == 32, source_fpscr );

uint64_t fpu_exception_initial[ SOURCE_MAX ] RTEMS_ALIGNED( 8 );

static CPU_Exception_frame fpu_exception_frame;

static uint64_t fpu_exception_fpscr;

extern const char fpu_exception_done_label[];

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

#define EXC_LOAD                         \
  "lis 3, fpu_exception_initial@ha\n"    \
  "addi 3, 3, fpu_exception_initial@l\n" \
  "lfd 0, 256(3)\n"                      \
  "mtfsf 255, 0\n"                       \
  "lfd 0, 0(3)\n"                        \
  "lfd 1, 8(3)\n"                        \
  "lfd 2, 16(3)\n"                       \
  "lfd 3, 24(3)\n"                       \
  "lfd 4, 32(3)\n"                       \
  "lfd 5, 40(3)\n"                       \
  "lfd 6, 48(3)\n"                       \
  "lfd 7, 56(3)\n"                       \
  "lfd 8, 64(3)\n"                       \
  "lfd 9, 72(3)\n"                       \
  "lfd 10, 80(3)\n"                      \
  "lfd 11, 88(3)\n"                      \
  "lfd 12, 96(3)\n"                      \
  "lfd 13, 104(3)\n"                     \
  "lfd 14, 112(3)\n"                     \
  "lfd 15, 120(3)\n"                     \
  "lfd 16, 128(3)\n"                     \
  "lfd 17, 136(3)\n"                     \
  "lfd 18, 144(3)\n"                     \
  "lfd 19, 152(3)\n"                     \
  "lfd 20, 160(3)\n"                     \
  "lfd 21, 168(3)\n"                     \
  "lfd 22, 176(3)\n"                     \
  "lfd 23, 184(3)\n"                     \
  "lfd 24, 192(3)\n"                     \
  "lfd 25, 200(3)\n"                     \
  "lfd 26, 208(3)\n"                     \
  "lfd 27, 216(3)\n"                     \
  "lfd 28, 224(3)\n"                     \
  "lfd 29, 232(3)\n"                     \
  "lfd 30, 240(3)\n"                     \
  "lfd 31, 248(3)\n"

static void FpuExceptionFatal(
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
  fpu_exception_frame = *frame;
  SetFatalHandler( NULL, NULL );
  frame->FPSCR = fpu_exception_fpscr;
  frame->EXC_SRR0 = (uintptr_t) fpu_exception_done_label;
  _CPU_Exception_resume( frame );
}

static void FpuExceptionRun( RegisterCheck *self, void *arg )
{
  (void) arg;
  memset( &fpu_exception_frame, 0, sizeof( fpu_exception_frame ) );
  fpu_exception_fpscr = GetFPSCR();
  SetFatalHandler( FpuExceptionFatal, NULL );
  __asm__ volatile( EXC_LOAD "trap\n"
                             ".globl fpu_exception_done_label\n"
                             "fpu_exception_done_label:\n"
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
                      "fr13",
                      "fr14",
                      "fr15",
                      "fr16",
                      "fr17",
                      "fr18",
                      "fr19",
                      "fr20",
                      "fr21",
                      "fr22",
                      "fr23",
                      "fr24",
                      "fr25",
                      "fr26",
                      "fr27",
                      "fr28",
                      "fr29",
                      "fr30",
                      "fr31" );
  RegisterCheckRecord(
    self,
    SLOT_F0,
    Bits( fpu_exception_frame.F0 ),
    fpu_exception_patterns[ SOURCE_F0 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F1,
    Bits( fpu_exception_frame.F1 ),
    fpu_exception_patterns[ SOURCE_F1 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F2,
    Bits( fpu_exception_frame.F2 ),
    fpu_exception_patterns[ SOURCE_F2 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F3,
    Bits( fpu_exception_frame.F3 ),
    fpu_exception_patterns[ SOURCE_F3 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F4,
    Bits( fpu_exception_frame.F4 ),
    fpu_exception_patterns[ SOURCE_F4 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F5,
    Bits( fpu_exception_frame.F5 ),
    fpu_exception_patterns[ SOURCE_F5 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F6,
    Bits( fpu_exception_frame.F6 ),
    fpu_exception_patterns[ SOURCE_F6 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F7,
    Bits( fpu_exception_frame.F7 ),
    fpu_exception_patterns[ SOURCE_F7 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F8,
    Bits( fpu_exception_frame.F8 ),
    fpu_exception_patterns[ SOURCE_F8 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F9,
    Bits( fpu_exception_frame.F9 ),
    fpu_exception_patterns[ SOURCE_F9 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F10,
    Bits( fpu_exception_frame.F10 ),
    fpu_exception_patterns[ SOURCE_F10 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F11,
    Bits( fpu_exception_frame.F11 ),
    fpu_exception_patterns[ SOURCE_F11 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F12,
    Bits( fpu_exception_frame.F12 ),
    fpu_exception_patterns[ SOURCE_F12 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F13,
    Bits( fpu_exception_frame.F13 ),
    fpu_exception_patterns[ SOURCE_F13 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F14,
    Bits( fpu_exception_frame.F14 ),
    fpu_exception_patterns[ SOURCE_F14 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F15,
    Bits( fpu_exception_frame.F15 ),
    fpu_exception_patterns[ SOURCE_F15 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F16,
    Bits( fpu_exception_frame.F16 ),
    fpu_exception_patterns[ SOURCE_F16 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F17,
    Bits( fpu_exception_frame.F17 ),
    fpu_exception_patterns[ SOURCE_F17 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F18,
    Bits( fpu_exception_frame.F18 ),
    fpu_exception_patterns[ SOURCE_F18 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F19,
    Bits( fpu_exception_frame.F19 ),
    fpu_exception_patterns[ SOURCE_F19 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F20,
    Bits( fpu_exception_frame.F20 ),
    fpu_exception_patterns[ SOURCE_F20 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F21,
    Bits( fpu_exception_frame.F21 ),
    fpu_exception_patterns[ SOURCE_F21 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F22,
    Bits( fpu_exception_frame.F22 ),
    fpu_exception_patterns[ SOURCE_F22 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F23,
    Bits( fpu_exception_frame.F23 ),
    fpu_exception_patterns[ SOURCE_F23 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F24,
    Bits( fpu_exception_frame.F24 ),
    fpu_exception_patterns[ SOURCE_F24 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F25,
    Bits( fpu_exception_frame.F25 ),
    fpu_exception_patterns[ SOURCE_F25 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F26,
    Bits( fpu_exception_frame.F26 ),
    fpu_exception_patterns[ SOURCE_F26 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F27,
    Bits( fpu_exception_frame.F27 ),
    fpu_exception_patterns[ SOURCE_F27 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F28,
    Bits( fpu_exception_frame.F28 ),
    fpu_exception_patterns[ SOURCE_F28 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F29,
    Bits( fpu_exception_frame.F29 ),
    fpu_exception_patterns[ SOURCE_F29 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F30,
    Bits( fpu_exception_frame.F30 ),
    fpu_exception_patterns[ SOURCE_F30 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_F31,
    Bits( fpu_exception_frame.F31 ),
    fpu_exception_patterns[ SOURCE_F31 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FPSCR,
    fpu_exception_frame.FPSCR & FPSCR_MASK,
    fpu_exception_patterns[ SOURCE_FPSCR ]
  );
}

static RegisterCheck fpu_exception_check = {
  .sources = fpu_exception_sources,
  .source_count = SOURCE_MAX,
  .slots = fpu_exception_slots,
  .slot_count = SLOT_MAX,
  .initial = fpu_exception_initial,
  .run = FpuExceptionRun
};

static void CheckSlot( Slot slot )
{
  RegisterCheckVerify( &fpu_exception_check, slot );
}
#endif

/**
 * @brief Load a distinct pattern into every floating-point register and into
 *   FPSCR. Trap. Run this once for each value with the pattern of that value
 *   inverted, then once unchanged.
 */
static void ScoreCpuPowerpcValFpuExceptionFrame_Action_0( void )
{
  #if defined( PPC_MULTILIB_FPU )
  memcpy(
    fpu_exception_initial,
    fpu_exception_patterns,
    sizeof( fpu_exception_initial )
  );
  RegisterCheckRun( &fpu_exception_check );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F0.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F0 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F1.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F1 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F2.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F2 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F3.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F3 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F4.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F4 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F5.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F5 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F6.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F6 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F7.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F7 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F8.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F8 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F9.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F9 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F10.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F10 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F11.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F11 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F12.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F12 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F13.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F13 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F14.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F14 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F15.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F15 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F16.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F16 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F17.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F17 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F18.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F18 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F19.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F19 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F20.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F20 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F21.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F21 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F22.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F22 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F23.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F23 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F24.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F24 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F25.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F25 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F26.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F26 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F27.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F27 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F28.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F28 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F29.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F29 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F30.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F30 );
  #endif

  /*
   * Check that the exception frame holds the PowerPC floating-point register
   * F31.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_F31 );
  #endif

  /*
   * Check that the exception frame holds FPSCR.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FPSCR );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  #if defined( PPC_MULTILIB_FPU )
  RegisterCheckReport( &fpu_exception_check );
  #endif
}

/**
 * @fn void T_case_body_ScoreCpuPowerpcValFpuExceptionFrame( void )
 */
T_TEST_CASE( ScoreCpuPowerpcValFpuExceptionFrame )
{
  ScoreCpuPowerpcValFpuExceptionFrame_Action_0();
}

/** @} */
