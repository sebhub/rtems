/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuPowerpcValFpuContext
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
 * @defgroup ScoreCpuPowerpcValFpuContext \
 *   spec:/score/cpu/powerpc/val/fpu-context
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that the context switch saves and restores the floating-point
 *   state of a thread.
 *
 * This test case performs the following actions:
 *
 * - Load a distinct pattern into every call-saved floating-point register and
 *   into FPSCR. Call the context switch directly. The heir records its FPSCR
 *   and switches back. Trap, so the frame reports every register. Run this
 *   once for each value with the pattern of that value inverted, then once
 *   unchanged.
 *
 *   - Check that the switch restored the PowerPC floating-point register F14.
 *
 *   - Check that the switch restored the PowerPC floating-point register F15.
 *
 *   - Check that the switch restored the PowerPC floating-point register F16.
 *
 *   - Check that the switch restored the PowerPC floating-point register F17.
 *
 *   - Check that the switch restored the PowerPC floating-point register F18.
 *
 *   - Check that the switch restored the PowerPC floating-point register F19.
 *
 *   - Check that the switch restored the PowerPC floating-point register F20.
 *
 *   - Check that the switch restored the PowerPC floating-point register F21.
 *
 *   - Check that the switch restored the PowerPC floating-point register F22.
 *
 *   - Check that the switch restored the PowerPC floating-point register F23.
 *
 *   - Check that the switch restored the PowerPC floating-point register F24.
 *
 *   - Check that the switch restored the PowerPC floating-point register F25.
 *
 *   - Check that the switch restored the PowerPC floating-point register F26.
 *
 *   - Check that the switch restored the PowerPC floating-point register F27.
 *
 *   - Check that the switch restored the PowerPC floating-point register F28.
 *
 *   - Check that the switch restored the PowerPC floating-point register F29.
 *
 *   - Check that the switch restored the PowerPC floating-point register F30.
 *
 *   - Check that the switch restored the PowerPC floating-point register F31.
 *
 *   - Check that the switch saved the PowerPC floating-point register F14.
 *
 *   - Check that the switch saved the PowerPC floating-point register F15.
 *
 *   - Check that the switch saved the PowerPC floating-point register F16.
 *
 *   - Check that the switch saved the PowerPC floating-point register F17.
 *
 *   - Check that the switch saved the PowerPC floating-point register F18.
 *
 *   - Check that the switch saved the PowerPC floating-point register F19.
 *
 *   - Check that the switch saved the PowerPC floating-point register F20.
 *
 *   - Check that the switch saved the PowerPC floating-point register F21.
 *
 *   - Check that the switch saved the PowerPC floating-point register F22.
 *
 *   - Check that the switch saved the PowerPC floating-point register F23.
 *
 *   - Check that the switch saved the PowerPC floating-point register F24.
 *
 *   - Check that the switch saved the PowerPC floating-point register F25.
 *
 *   - Check that the switch saved the PowerPC floating-point register F26.
 *
 *   - Check that the switch saved the PowerPC floating-point register F27.
 *
 *   - Check that the switch saved the PowerPC floating-point register F28.
 *
 *   - Check that the switch saved the PowerPC floating-point register F29.
 *
 *   - Check that the switch saved the PowerPC floating-point register F30.
 *
 *   - Check that the switch saved the PowerPC floating-point register F31.
 *
 *   - Check that the switch saved FPSCR.
 *
 *   - Check that the switch back restored FPSCR.
 *
 *   - Check that the heir received the FPSCR of its context.
 *
 *   - Check that the context initialization gave the context an FPSCR of zero.
 *
 *   - Check that the trap after the switch back reached the fatal handler
 *     once.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

#if defined( PPC_MULTILIB_FPU )
extern const char fpu_context_done_label[];

#define WORD UINT64_C( 0xffffffff )

/*
 * These are the sticky bits OX, UX, ZX and XX and the rounding mode RN of
 * FPSCR.  The mask covers no enable bit, so no operation traps.
 */
#define FPSCR_MASK UINT64_C( 0x1e000003 )

typedef enum {
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
  SOURCE_HEIR_FPSCR,
  SOURCE_INIT,
  SOURCE_TRAP,
  SOURCE_MAX
} Source;

typedef enum {
  SLOT_FRAME_F14,
  SLOT_FRAME_F15,
  SLOT_FRAME_F16,
  SLOT_FRAME_F17,
  SLOT_FRAME_F18,
  SLOT_FRAME_F19,
  SLOT_FRAME_F20,
  SLOT_FRAME_F21,
  SLOT_FRAME_F22,
  SLOT_FRAME_F23,
  SLOT_FRAME_F24,
  SLOT_FRAME_F25,
  SLOT_FRAME_F26,
  SLOT_FRAME_F27,
  SLOT_FRAME_F28,
  SLOT_FRAME_F29,
  SLOT_FRAME_F30,
  SLOT_FRAME_F31,
  SLOT_SAVED_F14,
  SLOT_SAVED_F15,
  SLOT_SAVED_F16,
  SLOT_SAVED_F17,
  SLOT_SAVED_F18,
  SLOT_SAVED_F19,
  SLOT_SAVED_F20,
  SLOT_SAVED_F21,
  SLOT_SAVED_F22,
  SLOT_SAVED_F23,
  SLOT_SAVED_F24,
  SLOT_SAVED_F25,
  SLOT_SAVED_F26,
  SLOT_SAVED_F27,
  SLOT_SAVED_F28,
  SLOT_SAVED_F29,
  SLOT_SAVED_F30,
  SLOT_SAVED_F31,
  SLOT_SAVED_FPSCR,
  SLOT_FRAME_FPSCR,
  SLOT_HEIR_FPSCR,
  SLOT_INIT_FPSCR,
  SLOT_TRAP,
  SLOT_MAX
} Slot;

static const RegisterCheckSource fpu_context_sources[ SOURCE_MAX ] = {
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
  { "fpscr", REGISTER_CHECK_INITIAL, FPSCR_MASK },
  { "heir fpscr", REGISTER_CHECK_INITIAL, FPSCR_MASK },
  { "init", REGISTER_CHECK_EXPECTED, WORD },
  { "trap", REGISTER_CHECK_EXPECTED, WORD }
};

static const RegisterCheckSlot fpu_context_slots[ SLOT_MAX ] = {
  { "frame f14", SOURCE_F14, UINT64_MAX },
  { "frame f15", SOURCE_F15, UINT64_MAX },
  { "frame f16", SOURCE_F16, UINT64_MAX },
  { "frame f17", SOURCE_F17, UINT64_MAX },
  { "frame f18", SOURCE_F18, UINT64_MAX },
  { "frame f19", SOURCE_F19, UINT64_MAX },
  { "frame f20", SOURCE_F20, UINT64_MAX },
  { "frame f21", SOURCE_F21, UINT64_MAX },
  { "frame f22", SOURCE_F22, UINT64_MAX },
  { "frame f23", SOURCE_F23, UINT64_MAX },
  { "frame f24", SOURCE_F24, UINT64_MAX },
  { "frame f25", SOURCE_F25, UINT64_MAX },
  { "frame f26", SOURCE_F26, UINT64_MAX },
  { "frame f27", SOURCE_F27, UINT64_MAX },
  { "frame f28", SOURCE_F28, UINT64_MAX },
  { "frame f29", SOURCE_F29, UINT64_MAX },
  { "frame f30", SOURCE_F30, UINT64_MAX },
  { "frame f31", SOURCE_F31, UINT64_MAX },
  { "saved f14", SOURCE_F14, UINT64_MAX },
  { "saved f15", SOURCE_F15, UINT64_MAX },
  { "saved f16", SOURCE_F16, UINT64_MAX },
  { "saved f17", SOURCE_F17, UINT64_MAX },
  { "saved f18", SOURCE_F18, UINT64_MAX },
  { "saved f19", SOURCE_F19, UINT64_MAX },
  { "saved f20", SOURCE_F20, UINT64_MAX },
  { "saved f21", SOURCE_F21, UINT64_MAX },
  { "saved f22", SOURCE_F22, UINT64_MAX },
  { "saved f23", SOURCE_F23, UINT64_MAX },
  { "saved f24", SOURCE_F24, UINT64_MAX },
  { "saved f25", SOURCE_F25, UINT64_MAX },
  { "saved f26", SOURCE_F26, UINT64_MAX },
  { "saved f27", SOURCE_F27, UINT64_MAX },
  { "saved f28", SOURCE_F28, UINT64_MAX },
  { "saved f29", SOURCE_F29, UINT64_MAX },
  { "saved f30", SOURCE_F30, UINT64_MAX },
  { "saved f31", SOURCE_F31, UINT64_MAX },
  { "saved fpscr", SOURCE_FPSCR, FPSCR_MASK },
  { "frame fpscr", SOURCE_FPSCR, FPSCR_MASK },
  { "heir fpscr", SOURCE_HEIR_FPSCR, FPSCR_MASK },
  { "init fpscr", SOURCE_INIT, WORD },
  { "trap", SOURCE_TRAP, WORD }
};

static const uint64_t fpu_context_patterns[ SOURCE_MAX ] = {
  UINT64_C( 0xc1c2c3c431323334 ),
  UINT64_C( 0xc2c3c4c532333435 ),
  UINT64_C( 0xc3c4c5c633343536 ),
  UINT64_C( 0xc4c5c6c734353637 ),
  UINT64_C( 0xc5c6c7c835363738 ),
  UINT64_C( 0xc6c7c8c936373839 ),
  UINT64_C( 0xc7c8c9ca3738393a ),
  UINT64_C( 0xc8c9cacb38393a3b ),
  UINT64_C( 0xc9cacbcc393a3b3c ),
  UINT64_C( 0xcacbcccd3a3b3c3d ),
  UINT64_C( 0xcbcccdce3b3c3d3e ),
  UINT64_C( 0xcccdcecf3c3d3e3f ),
  UINT64_C( 0xcdcecfd03d3e3f40 ),
  UINT64_C( 0xcecfd0d13e3f4041 ),
  UINT64_C( 0xcfd0d1d23f404142 ),
  UINT64_C( 0xd0d1d2d340414243 ),
  UINT64_C( 0xd1d2d3d441424344 ),
  UINT64_C( 0xd2d3d4d542434445 ),
  UINT64_C( 0x0a000002 ),
  UINT64_C( 0x06000001 ),
  0,
  1
};

RTEMS_STATIC_ASSERT( SOURCE_F31 == 17, source_f31 );
RTEMS_STATIC_ASSERT( SOURCE_FPSCR == 18, source_fpscr );

uint64_t fpu_context_initial[ SOURCE_MAX ] RTEMS_ALIGNED( 8 );

static CPU_Exception_frame fpu_context_frame;

static volatile int fpu_context_trap;

Context_Control fpu_context_executing;

Context_Control fpu_context_heir;

uint64_t fpu_context_heir_fpscr;

static uint64_t fpu_context_init_fpscr;

static uint64_t fpu_context_fpscr;

static uint64_t fpu_context_heir_stack[ 512 ] RTEMS_ALIGNED(
  CPU_STACK_ALIGNMENT
);

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

/*
 * The block loads r3 with the address of the table, sets FPSCR through f0, and
 * loads each call-saved floating-point register.
 */
#define CTX_LOAD                       \
  "lis 3, fpu_context_initial@ha\n"    \
  "addi 3, 3, fpu_context_initial@l\n" \
  "lfd 0, 144(3)\n"                    \
  "mtfsf 255, 0\n"                     \
  "lfd 14, 0(3)\n"                     \
  "lfd 15, 8(3)\n"                     \
  "lfd 16, 16(3)\n"                    \
  "lfd 17, 24(3)\n"                    \
  "lfd 18, 32(3)\n"                    \
  "lfd 19, 40(3)\n"                    \
  "lfd 20, 48(3)\n"                    \
  "lfd 21, 56(3)\n"                    \
  "lfd 22, 64(3)\n"                    \
  "lfd 23, 72(3)\n"                    \
  "lfd 24, 80(3)\n"                    \
  "lfd 25, 88(3)\n"                    \
  "lfd 26, 96(3)\n"                    \
  "lfd 27, 104(3)\n"                   \
  "lfd 28, 112(3)\n"                   \
  "lfd 29, 120(3)\n"                   \
  "lfd 30, 128(3)\n"                   \
  "lfd 31, 136(3)\n"

static void FpuContextFatal(
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
  fpu_context_frame = *frame;
  ++fpu_context_trap;
  SetFatalHandler( NULL, NULL );
  frame->FPSCR = fpu_context_fpscr;
  frame->EXC_SRR0 = (uintptr_t) fpu_context_done_label;
  _CPU_Exception_resume( frame );
}

/*
 * The heir records its FPSCR and switches straight back.  It is assembly, so
 * no prologue saves a register of the context under test.
 */
void FpuContextHeirEntry( void );

__asm__( "  .globl FpuContextHeirEntry\n"
         "  .type FpuContextHeirEntry, @function\n"
         "FpuContextHeirEntry:\n"
         "  mffs 0\n"
         "  lis 5, fpu_context_heir_fpscr@ha\n"
         "  stfd 0, fpu_context_heir_fpscr@l(5)\n"
         "  lis 3, fpu_context_heir@ha\n"
         "  addi 3, 3, fpu_context_heir@l\n"
         "  lis 4, fpu_context_executing@ha\n"
         "  addi 4, 4, fpu_context_executing@l\n"
         "  b _CPU_Context_switch\n" );

static void FpuContextRunSwitch( void )
{
  uint64_t     heir_fpscr;
  ppc_context *heir;

  fpu_context_trap = 0;
  memset( &fpu_context_frame, 0, sizeof( fpu_context_frame ) );
  memset( &fpu_context_executing, 0, sizeof( fpu_context_executing ) );
  ppc_get_context( &fpu_context_executing )->tp =
    ppc_get_context( &GetExecuting()->Registers )->tp;
  _CPU_Context_Initialize(
    &fpu_context_heir,
    fpu_context_heir_stack,
    sizeof( fpu_context_heir_stack ),
    0,
    FpuContextHeirEntry,
    true,
    NULL
  );
  heir = ppc_get_context( &fpu_context_heir );
  fpu_context_init_fpscr = Bits( heir->fpscr );
  heir_fpscr = fpu_context_initial[ SOURCE_HEIR_FPSCR ];
  memcpy( &heir->fpscr, &heir_fpscr, sizeof( heir->fpscr ) );
  fpu_context_fpscr = GetFPSCR();
  SetFatalHandler( FpuContextFatal, NULL );

  __asm__ volatile( CTX_LOAD "lis 3, fpu_context_executing@ha\n"
                             "addi 3, 3, fpu_context_executing@l\n"
                             "lis 4, fpu_context_heir@ha\n"
                             "addi 4, 4, fpu_context_heir@l\n"
                             "bl _CPU_Context_switch\n"
                             "trap\n"
                             ".globl fpu_context_done_label\n"
                             "fpu_context_done_label:\n"
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
}

static void FpuContextRun( RegisterCheck *self, void *arg )
{
  const ppc_context *ctx;

  (void) arg;
  FpuContextRunSwitch();
  ctx = ppc_get_context( &fpu_context_executing );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F14,
    Bits( fpu_context_frame.F14 ),
    fpu_context_patterns[ SOURCE_F14 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F15,
    Bits( fpu_context_frame.F15 ),
    fpu_context_patterns[ SOURCE_F15 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F16,
    Bits( fpu_context_frame.F16 ),
    fpu_context_patterns[ SOURCE_F16 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F17,
    Bits( fpu_context_frame.F17 ),
    fpu_context_patterns[ SOURCE_F17 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F18,
    Bits( fpu_context_frame.F18 ),
    fpu_context_patterns[ SOURCE_F18 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F19,
    Bits( fpu_context_frame.F19 ),
    fpu_context_patterns[ SOURCE_F19 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F20,
    Bits( fpu_context_frame.F20 ),
    fpu_context_patterns[ SOURCE_F20 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F21,
    Bits( fpu_context_frame.F21 ),
    fpu_context_patterns[ SOURCE_F21 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F22,
    Bits( fpu_context_frame.F22 ),
    fpu_context_patterns[ SOURCE_F22 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F23,
    Bits( fpu_context_frame.F23 ),
    fpu_context_patterns[ SOURCE_F23 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F24,
    Bits( fpu_context_frame.F24 ),
    fpu_context_patterns[ SOURCE_F24 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F25,
    Bits( fpu_context_frame.F25 ),
    fpu_context_patterns[ SOURCE_F25 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F26,
    Bits( fpu_context_frame.F26 ),
    fpu_context_patterns[ SOURCE_F26 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F27,
    Bits( fpu_context_frame.F27 ),
    fpu_context_patterns[ SOURCE_F27 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F28,
    Bits( fpu_context_frame.F28 ),
    fpu_context_patterns[ SOURCE_F28 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F29,
    Bits( fpu_context_frame.F29 ),
    fpu_context_patterns[ SOURCE_F29 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F30,
    Bits( fpu_context_frame.F30 ),
    fpu_context_patterns[ SOURCE_F30 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_F31,
    Bits( fpu_context_frame.F31 ),
    fpu_context_patterns[ SOURCE_F31 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F14,
    Bits( ctx->f14 ),
    fpu_context_patterns[ SOURCE_F14 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F15,
    Bits( ctx->f15 ),
    fpu_context_patterns[ SOURCE_F15 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F16,
    Bits( ctx->f16 ),
    fpu_context_patterns[ SOURCE_F16 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F17,
    Bits( ctx->f17 ),
    fpu_context_patterns[ SOURCE_F17 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F18,
    Bits( ctx->f18 ),
    fpu_context_patterns[ SOURCE_F18 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F19,
    Bits( ctx->f19 ),
    fpu_context_patterns[ SOURCE_F19 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F20,
    Bits( ctx->f20 ),
    fpu_context_patterns[ SOURCE_F20 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F21,
    Bits( ctx->f21 ),
    fpu_context_patterns[ SOURCE_F21 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F22,
    Bits( ctx->f22 ),
    fpu_context_patterns[ SOURCE_F22 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F23,
    Bits( ctx->f23 ),
    fpu_context_patterns[ SOURCE_F23 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F24,
    Bits( ctx->f24 ),
    fpu_context_patterns[ SOURCE_F24 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F25,
    Bits( ctx->f25 ),
    fpu_context_patterns[ SOURCE_F25 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F26,
    Bits( ctx->f26 ),
    fpu_context_patterns[ SOURCE_F26 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F27,
    Bits( ctx->f27 ),
    fpu_context_patterns[ SOURCE_F27 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F28,
    Bits( ctx->f28 ),
    fpu_context_patterns[ SOURCE_F28 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F29,
    Bits( ctx->f29 ),
    fpu_context_patterns[ SOURCE_F29 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F30,
    Bits( ctx->f30 ),
    fpu_context_patterns[ SOURCE_F30 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_F31,
    Bits( ctx->f31 ),
    fpu_context_patterns[ SOURCE_F31 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SAVED_FPSCR,
    Bits( ctx->fpscr ) & FPSCR_MASK,
    fpu_context_patterns[ SOURCE_FPSCR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME_FPSCR,
    fpu_context_frame.FPSCR & FPSCR_MASK,
    fpu_context_patterns[ SOURCE_FPSCR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_HEIR_FPSCR,
    fpu_context_heir_fpscr & FPSCR_MASK,
    fpu_context_patterns[ SOURCE_HEIR_FPSCR ]
  );
  RegisterCheckRecord(
    self,
    SLOT_INIT_FPSCR,
    fpu_context_init_fpscr & WORD,
    0
  );
  RegisterCheckRecord( self, SLOT_TRAP, (uint64_t) fpu_context_trap, 1 );
}

static RegisterCheck fpu_context_check = {
  .sources = fpu_context_sources,
  .source_count = SOURCE_MAX,
  .slots = fpu_context_slots,
  .slot_count = SLOT_MAX,
  .initial = fpu_context_initial,
  .run = FpuContextRun
};

static void CheckSlot( Slot slot )
{
  RegisterCheckVerify( &fpu_context_check, slot );
}
#endif

/**
 * @brief Load a distinct pattern into every call-saved floating-point register
 *   and into FPSCR. Call the context switch directly. The heir records its
 *   FPSCR and switches back. Trap, so the frame reports every register. Run
 *   this once for each value with the pattern of that value inverted, then
 *   once unchanged.
 */
static void ScoreCpuPowerpcValFpuContext_Action_0( void )
{
  #if defined( PPC_MULTILIB_FPU )
  memcpy(
    fpu_context_initial,
    fpu_context_patterns,
    sizeof( fpu_context_initial )
  );
  RegisterCheckRun( &fpu_context_check );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F14.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F14 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F15.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F15 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F16.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F16 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F17.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F17 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F18.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F18 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F19.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F19 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F20.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F20 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F21.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F21 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F22.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F22 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F23.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F23 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F24.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F24 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F25.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F25 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F26.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F26 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F27.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F27 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F28.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F28 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F29.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F29 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F30.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F30 );
  #endif

  /*
   * Check that the switch restored the PowerPC floating-point register F31.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_F31 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F14.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F14 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F15.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F15 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F16.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F16 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F17.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F17 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F18.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F18 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F19.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F19 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F20.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F20 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F21.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F21 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F22.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F22 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F23.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F23 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F24.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F24 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F25.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F25 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F26.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F26 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F27.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F27 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F28.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F28 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F29.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F29 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F30.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F30 );
  #endif

  /*
   * Check that the switch saved the PowerPC floating-point register F31.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_F31 );
  #endif

  /*
   * Check that the switch saved FPSCR.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_SAVED_FPSCR );
  #endif

  /*
   * Check that the switch back restored FPSCR.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_FRAME_FPSCR );
  #endif

  /*
   * Check that the heir received the FPSCR of its context.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_HEIR_FPSCR );
  #endif

  /*
   * Check that the context initialization gave the context an FPSCR of zero.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_INIT_FPSCR );
  #endif

  /*
   * Check that the trap after the switch back reached the fatal handler once.
   */
  #if defined( PPC_MULTILIB_FPU )
  CheckSlot( SLOT_TRAP );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  #if defined( PPC_MULTILIB_FPU )
  RegisterCheckReport( &fpu_context_check );
  #endif
}

/**
 * @fn void T_case_body_ScoreCpuPowerpcValFpuContext( void )
 */
T_TEST_CASE( ScoreCpuPowerpcValFpuContext )
{
  ScoreCpuPowerpcValFpuContext_Action_0();
}

/** @} */
