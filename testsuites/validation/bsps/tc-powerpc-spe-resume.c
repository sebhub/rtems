/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuPowerpcValSpeResume
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
 * @defgroup ScoreCpuPowerpcValSpeResume spec:/score/cpu/powerpc/val/spe-resume
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that the exception resume restores the SPE state of the
 *   exception frame.
 *
 * This test case performs the following actions:
 *
 * - Trap, let the handler write a new value into the top half of every
 *   register, into ACC and into SPEFSCR of the frame, resume, and trap again
 *   to capture what the resume restored. Run this once for each value with the
 *   value inverted, then once unchanged.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R0 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R3 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R4 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R5 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R6 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R7 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R8 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R9 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R10 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R11 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R12 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R14 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R15 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R16 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R17 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R18 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R19 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R20 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R21 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R22 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R23 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R24 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R25 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R26 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R27 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R28 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R29 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R30 from the frame.
 *
 *   - Check that the resume restored the top 32 bits of the PowerPC register
 *     R31 from the frame.
 *
 *   - Check that the resume restored ACC from the frame.
 *
 *   - Check that the resume restored SPEFSCR from the frame.
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
  SOURCE_ACC,
  SOURCE_SPEFSCR,
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
  SLOT_R15,
  SLOT_R16,
  SLOT_R17,
  SLOT_R18,
  SLOT_R19,
  SLOT_R20,
  SLOT_R21,
  SLOT_R22,
  SLOT_R23,
  SLOT_R24,
  SLOT_R25,
  SLOT_R26,
  SLOT_R27,
  SLOT_R28,
  SLOT_R29,
  SLOT_R30,
  SLOT_R31,
  SLOT_ACC,
  SLOT_SPEFSCR,
  SLOT_MAX
} Slot;

static const RegisterCheckSource spe_resume_sources[ SOURCE_MAX ] = {
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
  { "acc", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "spefscr", REGISTER_CHECK_INITIAL, SPEFSCR_MASK }
};

static const RegisterCheckSlot spe_resume_slots[ SOURCE_MAX ] = {
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
  { "r15 top", SOURCE_R15, TOP },
  { "r16 top", SOURCE_R16, TOP },
  { "r17 top", SOURCE_R17, TOP },
  { "r18 top", SOURCE_R18, TOP },
  { "r19 top", SOURCE_R19, TOP },
  { "r20 top", SOURCE_R20, TOP },
  { "r21 top", SOURCE_R21, TOP },
  { "r22 top", SOURCE_R22, TOP },
  { "r23 top", SOURCE_R23, TOP },
  { "r24 top", SOURCE_R24, TOP },
  { "r25 top", SOURCE_R25, TOP },
  { "r26 top", SOURCE_R26, TOP },
  { "r27 top", SOURCE_R27, TOP },
  { "r28 top", SOURCE_R28, TOP },
  { "r29 top", SOURCE_R29, TOP },
  { "r30 top", SOURCE_R30, TOP },
  { "r31 top", SOURCE_R31, TOP },
  { "acc", SOURCE_ACC, UINT64_MAX },
  { "spefscr", SOURCE_SPEFSCR, SPEFSCR_MASK }
};

static const uint64_t spe_resume_patterns[ SOURCE_MAX ] = {
  UINT64_C( 0xd1d2d3d441424344 ),
  UINT64_C( 0xd2d3d4d542434445 ),
  UINT64_C( 0xd3d4d5d643444546 ),
  UINT64_C( 0xd4d5d6d744454647 ),
  UINT64_C( 0xd5d6d7d845464748 ),
  UINT64_C( 0xd6d7d8d946474849 ),
  UINT64_C( 0xd7d8d9da4748494a ),
  UINT64_C( 0xd8d9dadb48494a4b ),
  UINT64_C( 0xd9dadbdc494a4b4c ),
  UINT64_C( 0xdadbdcdd4a4b4c4d ),
  UINT64_C( 0xdbdcddde4b4c4d4e ),
  UINT64_C( 0xdcdddedf4c4d4e4f ),
  UINT64_C( 0xdddedfe04d4e4f50 ),
  UINT64_C( 0xdedfe0e14e4f5051 ),
  UINT64_C( 0xdfe0e1e24f505152 ),
  UINT64_C( 0xe0e1e2e350515253 ),
  UINT64_C( 0xe1e2e3e451525354 ),
  UINT64_C( 0xe2e3e4e552535455 ),
  UINT64_C( 0xe3e4e5e653545556 ),
  UINT64_C( 0xe4e5e6e754555657 ),
  UINT64_C( 0xe5e6e7e855565758 ),
  UINT64_C( 0xe6e7e8e956575859 ),
  UINT64_C( 0xe7e8e9ea5758595a ),
  UINT64_C( 0xe8e9eaeb58595a5b ),
  UINT64_C( 0xe9eaebec595a5b5c ),
  UINT64_C( 0xeaebeced5a5b5c5d ),
  UINT64_C( 0xebecedee5b5c5d5e ),
  UINT64_C( 0xecedeeef5c5d5e5f ),
  UINT64_C( 0xedeeeff05d5e5f60 ),
  UINT64_C( 0x2f2e2d2c2b2a2928 ),
  UINT64_C( 0x00280002 )
};

static uint64_t spe_resume_initial[ SOURCE_MAX ];

static CPU_Exception_frame spe_resume_frame;

static int spe_resume_trap;

static uint32_t spe_resume_spefscr;

extern const char spe_resume_second_trap_label[];

extern const char spe_resume_done_label[];

static void SpeResumeFatal(
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

  if ( spe_resume_trap == 0 ) {
    ++spe_resume_trap;
    frame->registers[ 0 ] = ( spe_resume_initial[ SOURCE_R0 ] & TOP ) |
                            ( frame->registers[ 0 ] & WORD );
    frame->registers[ 3 ] = ( spe_resume_initial[ SOURCE_R3 ] & TOP ) |
                            ( frame->registers[ 3 ] & WORD );
    frame->registers[ 4 ] = ( spe_resume_initial[ SOURCE_R4 ] & TOP ) |
                            ( frame->registers[ 4 ] & WORD );
    frame->registers[ 5 ] = ( spe_resume_initial[ SOURCE_R5 ] & TOP ) |
                            ( frame->registers[ 5 ] & WORD );
    frame->registers[ 6 ] = ( spe_resume_initial[ SOURCE_R6 ] & TOP ) |
                            ( frame->registers[ 6 ] & WORD );
    frame->registers[ 7 ] = ( spe_resume_initial[ SOURCE_R7 ] & TOP ) |
                            ( frame->registers[ 7 ] & WORD );
    frame->registers[ 8 ] = ( spe_resume_initial[ SOURCE_R8 ] & TOP ) |
                            ( frame->registers[ 8 ] & WORD );
    frame->registers[ 9 ] = ( spe_resume_initial[ SOURCE_R9 ] & TOP ) |
                            ( frame->registers[ 9 ] & WORD );
    frame->registers[ 10 ] = ( spe_resume_initial[ SOURCE_R10 ] & TOP ) |
                             ( frame->registers[ 10 ] & WORD );
    frame->registers[ 11 ] = ( spe_resume_initial[ SOURCE_R11 ] & TOP ) |
                             ( frame->registers[ 11 ] & WORD );
    frame->registers[ 12 ] = ( spe_resume_initial[ SOURCE_R12 ] & TOP ) |
                             ( frame->registers[ 12 ] & WORD );
    frame->registers[ 14 ] = ( spe_resume_initial[ SOURCE_R14 ] & TOP ) |
                             ( frame->registers[ 14 ] & WORD );
    frame->registers[ 15 ] = ( spe_resume_initial[ SOURCE_R15 ] & TOP ) |
                             ( frame->registers[ 15 ] & WORD );
    frame->registers[ 16 ] = ( spe_resume_initial[ SOURCE_R16 ] & TOP ) |
                             ( frame->registers[ 16 ] & WORD );
    frame->registers[ 17 ] = ( spe_resume_initial[ SOURCE_R17 ] & TOP ) |
                             ( frame->registers[ 17 ] & WORD );
    frame->registers[ 18 ] = ( spe_resume_initial[ SOURCE_R18 ] & TOP ) |
                             ( frame->registers[ 18 ] & WORD );
    frame->registers[ 19 ] = ( spe_resume_initial[ SOURCE_R19 ] & TOP ) |
                             ( frame->registers[ 19 ] & WORD );
    frame->registers[ 20 ] = ( spe_resume_initial[ SOURCE_R20 ] & TOP ) |
                             ( frame->registers[ 20 ] & WORD );
    frame->registers[ 21 ] = ( spe_resume_initial[ SOURCE_R21 ] & TOP ) |
                             ( frame->registers[ 21 ] & WORD );
    frame->registers[ 22 ] = ( spe_resume_initial[ SOURCE_R22 ] & TOP ) |
                             ( frame->registers[ 22 ] & WORD );
    frame->registers[ 23 ] = ( spe_resume_initial[ SOURCE_R23 ] & TOP ) |
                             ( frame->registers[ 23 ] & WORD );
    frame->registers[ 24 ] = ( spe_resume_initial[ SOURCE_R24 ] & TOP ) |
                             ( frame->registers[ 24 ] & WORD );
    frame->registers[ 25 ] = ( spe_resume_initial[ SOURCE_R25 ] & TOP ) |
                             ( frame->registers[ 25 ] & WORD );
    frame->registers[ 26 ] = ( spe_resume_initial[ SOURCE_R26 ] & TOP ) |
                             ( frame->registers[ 26 ] & WORD );
    frame->registers[ 27 ] = ( spe_resume_initial[ SOURCE_R27 ] & TOP ) |
                             ( frame->registers[ 27 ] & WORD );
    frame->registers[ 28 ] = ( spe_resume_initial[ SOURCE_R28 ] & TOP ) |
                             ( frame->registers[ 28 ] & WORD );
    frame->registers[ 29 ] = ( spe_resume_initial[ SOURCE_R29 ] & TOP ) |
                             ( frame->registers[ 29 ] & WORD );
    frame->registers[ 30 ] = ( spe_resume_initial[ SOURCE_R30 ] & TOP ) |
                             ( frame->registers[ 30 ] & WORD );
    frame->registers[ 31 ] = ( spe_resume_initial[ SOURCE_R31 ] & TOP ) |
                             ( frame->registers[ 31 ] & WORD );
    frame->EXC_ACC = spe_resume_initial[ SOURCE_ACC ];
    frame->EXC_SPEFSCR = (uint32_t) spe_resume_initial[ SOURCE_SPEFSCR ];
    frame->EXC_SRR0 = (uintptr_t) spe_resume_second_trap_label;
  } else {
    ++spe_resume_trap;
    spe_resume_frame = *frame;
    SetFatalHandler( NULL, NULL );
    frame->EXC_SPEFSCR = spe_resume_spefscr;
    frame->EXC_SRR0 = (uintptr_t) spe_resume_done_label;
  }

  _CPU_Exception_resume( frame );
}

static void SpeResumeRun( RegisterCheck *self, void *arg )
{
  (void) arg;
  spe_resume_trap = 0;
  memset( &spe_resume_frame, 0, sizeof( spe_resume_frame ) );
  __asm__ volatile( "mfspr %0, 512"
                    : "=r"( spe_resume_spefscr ) );
  SetFatalHandler( SpeResumeFatal, NULL );

  /*
   * The block holds both traps, so the compiler keeps no value of its own in a
   * register which the first trap changes.
   */
  __asm__ volatile( "trap\n"
                    ".globl spe_resume_second_trap_label\n"
                    "spe_resume_second_trap_label:\n"
                    "trap\n"
                    ".globl spe_resume_done_label\n"
                    "spe_resume_done_label:\n"
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
  RegisterCheckRecord(
    self,
    SLOT_R0,
    spe_resume_frame.registers[ 0 ] & TOP,
    spe_resume_patterns[ SOURCE_R0 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R3,
    spe_resume_frame.registers[ 3 ] & TOP,
    spe_resume_patterns[ SOURCE_R3 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R4,
    spe_resume_frame.registers[ 4 ] & TOP,
    spe_resume_patterns[ SOURCE_R4 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R5,
    spe_resume_frame.registers[ 5 ] & TOP,
    spe_resume_patterns[ SOURCE_R5 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R6,
    spe_resume_frame.registers[ 6 ] & TOP,
    spe_resume_patterns[ SOURCE_R6 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R7,
    spe_resume_frame.registers[ 7 ] & TOP,
    spe_resume_patterns[ SOURCE_R7 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R8,
    spe_resume_frame.registers[ 8 ] & TOP,
    spe_resume_patterns[ SOURCE_R8 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R9,
    spe_resume_frame.registers[ 9 ] & TOP,
    spe_resume_patterns[ SOURCE_R9 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R10,
    spe_resume_frame.registers[ 10 ] & TOP,
    spe_resume_patterns[ SOURCE_R10 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R11,
    spe_resume_frame.registers[ 11 ] & TOP,
    spe_resume_patterns[ SOURCE_R11 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R12,
    spe_resume_frame.registers[ 12 ] & TOP,
    spe_resume_patterns[ SOURCE_R12 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R14,
    spe_resume_frame.registers[ 14 ] & TOP,
    spe_resume_patterns[ SOURCE_R14 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R15,
    spe_resume_frame.registers[ 15 ] & TOP,
    spe_resume_patterns[ SOURCE_R15 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R16,
    spe_resume_frame.registers[ 16 ] & TOP,
    spe_resume_patterns[ SOURCE_R16 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R17,
    spe_resume_frame.registers[ 17 ] & TOP,
    spe_resume_patterns[ SOURCE_R17 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R18,
    spe_resume_frame.registers[ 18 ] & TOP,
    spe_resume_patterns[ SOURCE_R18 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R19,
    spe_resume_frame.registers[ 19 ] & TOP,
    spe_resume_patterns[ SOURCE_R19 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R20,
    spe_resume_frame.registers[ 20 ] & TOP,
    spe_resume_patterns[ SOURCE_R20 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R21,
    spe_resume_frame.registers[ 21 ] & TOP,
    spe_resume_patterns[ SOURCE_R21 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R22,
    spe_resume_frame.registers[ 22 ] & TOP,
    spe_resume_patterns[ SOURCE_R22 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R23,
    spe_resume_frame.registers[ 23 ] & TOP,
    spe_resume_patterns[ SOURCE_R23 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R24,
    spe_resume_frame.registers[ 24 ] & TOP,
    spe_resume_patterns[ SOURCE_R24 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R25,
    spe_resume_frame.registers[ 25 ] & TOP,
    spe_resume_patterns[ SOURCE_R25 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R26,
    spe_resume_frame.registers[ 26 ] & TOP,
    spe_resume_patterns[ SOURCE_R26 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R27,
    spe_resume_frame.registers[ 27 ] & TOP,
    spe_resume_patterns[ SOURCE_R27 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R28,
    spe_resume_frame.registers[ 28 ] & TOP,
    spe_resume_patterns[ SOURCE_R28 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R29,
    spe_resume_frame.registers[ 29 ] & TOP,
    spe_resume_patterns[ SOURCE_R29 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R30,
    spe_resume_frame.registers[ 30 ] & TOP,
    spe_resume_patterns[ SOURCE_R30 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_R31,
    spe_resume_frame.registers[ 31 ] & TOP,
    spe_resume_patterns[ SOURCE_R31 ] & TOP
  );
  RegisterCheckRecord(
    self,
    SLOT_ACC,
    spe_resume_frame.EXC_ACC,
    spe_resume_patterns[ SOURCE_ACC ]
  );
  RegisterCheckRecord(
    self,
    SLOT_SPEFSCR,
    spe_resume_frame.EXC_SPEFSCR & SPEFSCR_MASK,
    spe_resume_patterns[ SOURCE_SPEFSCR ]
  );
}

static RegisterCheck spe_resume_check = {
  .sources = spe_resume_sources,
  .source_count = SOURCE_MAX,
  .slots = spe_resume_slots,
  .slot_count = SOURCE_MAX,
  .initial = spe_resume_initial,
  .run = SpeResumeRun
};

static void CheckSlot( Slot slot )
{
  RegisterCheckVerify( &spe_resume_check, slot );
}
#endif

/**
 * @brief Trap, let the handler write a new value into the top half of every
 *   register, into ACC and into SPEFSCR of the frame, resume, and trap again
 *   to capture what the resume restored. Run this once for each value with the
 *   value inverted, then once unchanged.
 */
static void ScoreCpuPowerpcValSpeResume_Action_0( void )
{
  #if defined( __SPE__ )
  memcpy(
    spe_resume_initial,
    spe_resume_patterns,
    sizeof( spe_resume_initial )
  );
  RegisterCheckRun( &spe_resume_check );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R0
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R0 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R3
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R3 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R4
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R4 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R5
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R5 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R6
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R6 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R7
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R7 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R8
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R8 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R9
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R9 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R10
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R10 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R11
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R11 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R12
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R12 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R14
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R14 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R15
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R15 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R16
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R16 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R17
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R17 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R18
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R18 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R19
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R19 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R20
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R20 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R21
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R21 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R22
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R22 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R23
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R23 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R24
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R24 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R25
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R25 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R26
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R26 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R27
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R27 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R28
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R28 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R29
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R29 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R30
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R30 );
  #endif

  /*
   * Check that the resume restored the top 32 bits of the PowerPC register R31
   * from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_R31 );
  #endif

  /*
   * Check that the resume restored ACC from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_ACC );
  #endif

  /*
   * Check that the resume restored SPEFSCR from the frame.
   */
  #if defined( __SPE__ )
  CheckSlot( SLOT_SPEFSCR );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  #if defined( __SPE__ )
  RegisterCheckReport( &spe_resume_check );
  #endif
}

/**
 * @fn void T_case_body_ScoreCpuPowerpcValSpeResume( void )
 */
T_TEST_CASE( ScoreCpuPowerpcValSpeResume )
{
  ScoreCpuPowerpcValSpeResume_Action_0();
}

/** @} */
