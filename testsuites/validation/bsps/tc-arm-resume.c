/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuArmValResume
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
 * @defgroup ScoreCpuArmValResume spec:/score/cpu/arm/val/resume
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that the exception resume restores the core registers of the
 *   exception frame.
 *
 * The test changes each field of the status register which holds a result of
 * the interrupted context. These are the condition flags, the Q flag, the GE
 * flags, the E bit and the IT state. On an A or R profile processor it also
 * changes the I bit, since a trap with interrupts disabled resumes a context
 * with interrupts disabled. It does not change the mode bits, the T bit, the J
 * bit, the A bit and the F bit. These bits select the state in which the
 * resumed code runs. A set J bit with a set T bit selects the ThumbEE state. A
 * clear A bit lets a pending asynchronous abort occur. A clear F bit lets a
 * pending FIQ occur.
 *
 * This test case performs the following actions:
 *
 * - Trap, let the handler write a new value into every core register of the
 *   frame and into each status field which the interrupted context owns,
 *   resume, and trap again to capture what the resume restored. Run this once
 *   for each value with the value inverted, then once unchanged.
 *
 *   - Check that the resume restored the ARM core register R0.
 *
 *   - Check that the resume restored the ARM core register R1.
 *
 *   - Check that the resume restored the ARM core register R2.
 *
 *   - Check that the resume restored the ARM core register R3.
 *
 *   - Check that the resume restored the ARM core register R4.
 *
 *   - Check that the resume restored the ARM core register R5.
 *
 *   - Check that the resume restored the ARM core register R6.
 *
 *   - Check that the resume restored the ARM core register R7.
 *
 *   - Check that the resume restored the ARM core register R8.
 *
 *   - Check that the resume restored the ARM core register R9.
 *
 *   - Check that the resume restored the ARM core register R10.
 *
 *   - Check that the resume restored the ARM core register R11.
 *
 *   - Check that the resume restored the ARM core register R12.
 *
 *   - Check that the resume restored the stack pointer.
 *
 *   - Check that the resume restored the link register.
 *
 *   - Check that the resume restored the ARM FPU register D0.
 *
 *   - Check that the resume restored the ARM FPU register D1.
 *
 *   - Check that the resume restored the ARM FPU register D2.
 *
 *   - Check that the resume restored the ARM FPU register D3.
 *
 *   - Check that the resume restored the ARM FPU register D4.
 *
 *   - Check that the resume restored the ARM FPU register D5.
 *
 *   - Check that the resume restored the ARM FPU register D6.
 *
 *   - Check that the resume restored the ARM FPU register D7.
 *
 *   - Check that the resume restored the ARM FPU register D8.
 *
 *   - Check that the resume restored the ARM FPU register D9.
 *
 *   - Check that the resume restored the ARM FPU register D10.
 *
 *   - Check that the resume restored the ARM FPU register D11.
 *
 *   - Check that the resume restored the ARM FPU register D12.
 *
 *   - Check that the resume restored the ARM FPU register D13.
 *
 *   - Check that the resume restored the ARM FPU register D14.
 *
 *   - Check that the resume restored the ARM FPU register D15.
 *
 *   - Check that the resume restored the ARM FPU register D16.
 *
 *   - Check that the resume restored the ARM FPU register D17.
 *
 *   - Check that the resume restored the ARM FPU register D18.
 *
 *   - Check that the resume restored the ARM FPU register D19.
 *
 *   - Check that the resume restored the ARM FPU register D20.
 *
 *   - Check that the resume restored the ARM FPU register D21.
 *
 *   - Check that the resume restored the ARM FPU register D22.
 *
 *   - Check that the resume restored the ARM FPU register D23.
 *
 *   - Check that the resume restored the ARM FPU register D24.
 *
 *   - Check that the resume restored the ARM FPU register D25.
 *
 *   - Check that the resume restored the ARM FPU register D26.
 *
 *   - Check that the resume restored the ARM FPU register D27.
 *
 *   - Check that the resume restored the ARM FPU register D28.
 *
 *   - Check that the resume restored the ARM FPU register D29.
 *
 *   - Check that the resume restored the ARM FPU register D30.
 *
 *   - Check that the resume restored the ARM FPU register D31.
 *
 *   - Check that the resume restored FPSCR.
 *
 *   - Check that the resume released the exception frame. The second trap
 *     places its frame where the first trap placed it.
 *
 *   - Check that the resume restored the condition flags.
 *
 *   - Check that the resume restored the Q flag on a processor which has it.
 *
 *   - Check that the resume restored the GE flags on a processor which has
 *     them.
 *
 *   - Check that the resume restored the E bit on an A or R profile processor
 *     of ARMv6 and later.
 *
 *   - Check that the resume restored the I bit on an A or R profile processor.
 *
 *   - Check that the default exception handler saved BASEPRI to the exception
 *     frame on ARMv7-M.
 *
 *   - Check that the default exception handler saved the EXC_RETURN value to
 *     the exception frame on ARMv7-M. The trap returns to Thread mode on PSP.
 *
 *   - Check that the resume restored BASEPRI from the exception frame on
 *     ARMv7-M.
 *
 *   - Check that the resume returned through the EXC_RETURN value of the
 *     exception frame on ARMv7-M. The second trap reports the same mode, stack
 *     and frame type.
 *
 *   - Check that the resume ended the active state of the exception on
 *     ARMv7-M. The code after the second resume runs in Thread mode.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * - Trap in the Thumb state, let the handler resume at a block of four
 *   conditional instructions with the IT state of an ITETE EQ instruction, and
 *   trap again after the block. Run this once with the Z flag set and once
 *   with the Z flag clear.
 *
 *   - Check that the instructions of the block executed as the IT state and
 *     the Z flag of the frame select. With the Z flag set, the first and the
 *     third instruction executed. With the Z flag clear, the second and the
 *     fourth instruction executed.
 *
 *   - Check that the block ended before the second trap and that the Z flag
 *     kept the value of the frame.
 *
 * @{
 */

/* A processor with the Q flag holds it in bit 27 of the CPSR or xPSR. */
#if defined( __ARM_FEATURE_QBIT )
#define RESUME_HAS_Q
#endif

/*
 * A processor with the SIMD instructions of ARMv6 holds the GE flags in bits
 * 19 to 16 of the CPSR or xPSR.
 */
#if defined( __ARM_FEATURE_SIMD32 )
#define RESUME_HAS_GE
#endif

/*
 * An A or R profile processor of ARMv6 and later holds the E bit in bit 9 of
 * the CPSR.  Bit 9 of the xPSR of ARMv7-M is the stack alignment flag.
 */
#if defined( ARM_MULTILIB_ARCH_V4 ) && __ARM_ARCH >= 6
#define RESUME_HAS_E
#endif

/*
 * A processor with the Thumb-2 instructions holds the IT state in bits 26 to
 * 25 and 15 to 10 of the CPSR or xPSR.
 */
#if defined( __thumb2__ )
#define RESUME_HAS_IT
#endif

extern const char resume_trap2_label[];

extern const char resume_done_label[];

#define RESUME_STACK_WORDS 128

static uint64_t resume_stack[ RESUME_STACK_WORDS ];

static CPU_Exception_frame resume_before;

static CPU_Exception_frame resume_after;

#if defined( ARM_MULTILIB_ARCH_V7M )
#define RESUME_BASEPRI_TRAP_1 0xe0

#define RESUME_BASEPRI_TRAP_2 0xc0

static uint32_t resume_basepri_done;

static uint32_t resume_ipsr_done;
#endif

#ifdef ARM_MULTILIB_VFP
#ifdef ARM_MULTILIB_VFP_D32
#define ARM_VFP_D_COUNT 32
#else
#define ARM_VFP_D_COUNT 16
#endif

static ARM_VFP_context resume_before_vfp;

static ARM_VFP_context resume_after_vfp;
#endif

static int resume_trap;

static const CPU_Exception_frame *resume_frame[ 2 ];

typedef enum {
  SOURCE_R0,
  SOURCE_R1,
  SOURCE_R2,
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
  SOURCE_LR,
  SOURCE_FLAGS,
#ifdef RESUME_HAS_Q
  SOURCE_Q,
#endif
#ifdef RESUME_HAS_GE
  SOURCE_GE,
#endif
#ifdef RESUME_HAS_E
  SOURCE_E,
#endif
#if defined( ARM_MULTILIB_ARCH_V4 )
  SOURCE_I,
#endif
#ifdef ARM_MULTILIB_VFP
  SOURCE_D0,
  SOURCE_D1,
  SOURCE_D2,
  SOURCE_D3,
  SOURCE_D4,
  SOURCE_D5,
  SOURCE_D6,
  SOURCE_D7,
  SOURCE_D8,
  SOURCE_D9,
  SOURCE_D10,
  SOURCE_D11,
  SOURCE_D12,
  SOURCE_D13,
  SOURCE_D14,
  SOURCE_D15,
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
#ifdef ARM_MULTILIB_VFP
  SOURCE_FPSCR,
#endif
#if defined( ARM_MULTILIB_ARCH_V7M )
  SOURCE_BASEPRI,
#endif
  SOURCE_SP,
  SOURCE_FRAME,
#if defined( ARM_MULTILIB_ARCH_V7M )
  SOURCE_BASEPRI_SAVED,
  SOURCE_EXC_RETURN_SAVED,
  SOURCE_BASEPRI_DONE,
  SOURCE_EXC_RETURN,
  SOURCE_IPSR_DONE,
#endif
  SOURCE_MAX
} Source;

typedef enum {
  SLOT_R0,
  SLOT_R1,
  SLOT_R2,
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
  SLOT_LR,
  SLOT_FLAGS,
#ifdef RESUME_HAS_Q
  SLOT_Q,
#endif
#ifdef RESUME_HAS_GE
  SLOT_GE,
#endif
#ifdef RESUME_HAS_E
  SLOT_E,
#endif
#if defined( ARM_MULTILIB_ARCH_V4 )
  SLOT_I,
#endif
#ifdef ARM_MULTILIB_VFP
  SLOT_D0,
  SLOT_D1,
  SLOT_D2,
  SLOT_D3,
  SLOT_D4,
  SLOT_D5,
  SLOT_D6,
  SLOT_D7,
  SLOT_D8,
  SLOT_D9,
  SLOT_D10,
  SLOT_D11,
  SLOT_D12,
  SLOT_D13,
  SLOT_D14,
  SLOT_D15,
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
#ifdef ARM_MULTILIB_VFP
  SLOT_FPSCR,
#endif
#if defined( ARM_MULTILIB_ARCH_V7M )
  SLOT_BASEPRI,
#endif
  SLOT_SP,
  SLOT_FRAME,
#if defined( ARM_MULTILIB_ARCH_V7M )
  SLOT_BASEPRI_SAVED,
  SLOT_EXC_RETURN_SAVED,
  SLOT_BASEPRI_DONE,
  SLOT_EXC_RETURN,
  SLOT_IPSR_DONE,
#endif
  SLOT_MAX
} Slot;

#define WORD UINT64_C( 0xffffffff )

#define FLAGS UINT64_C( 0xf0000000 )

#define Q UINT64_C( 0x08000000 )

#define GE UINT64_C( 0x000f0000 )

#define E UINT64_C( 0x00000200 )

#define PSR_I UINT64_C( 0x00000080 )

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

/* The bits of BASEPRI which a device with three priority bits implements. */
#define BASEPRI UINT64_C( 0xe0 )

#define IPSR UINT64_C( 0x1ff )

static const RegisterCheckSource resume_sources[ SOURCE_MAX ] = {
  { "r0", REGISTER_CHECK_INITIAL, WORD },
  { "r1", REGISTER_CHECK_INITIAL, WORD },
  { "r2", REGISTER_CHECK_INITIAL, WORD },
  { "r3", REGISTER_CHECK_INITIAL, WORD },
  { "r4", REGISTER_CHECK_INITIAL, WORD },
  { "r5", REGISTER_CHECK_INITIAL, WORD },
  { "r6", REGISTER_CHECK_INITIAL, WORD },
  { "r7", REGISTER_CHECK_INITIAL, WORD },
  { "r8", REGISTER_CHECK_INITIAL, WORD },
  { "r9", REGISTER_CHECK_INITIAL, WORD },
  { "r10", REGISTER_CHECK_INITIAL, WORD },
  { "r11", REGISTER_CHECK_INITIAL, WORD },
  { "r12", REGISTER_CHECK_INITIAL, WORD },
  { "lr", REGISTER_CHECK_INITIAL, WORD },
  { "flags", REGISTER_CHECK_INITIAL, FLAGS },
#ifdef RESUME_HAS_Q
  { "q", REGISTER_CHECK_INITIAL, Q },
#endif
#ifdef RESUME_HAS_GE
  { "ge", REGISTER_CHECK_INITIAL, GE },
#endif
#ifdef RESUME_HAS_E
  { "e", REGISTER_CHECK_INITIAL, E },
#endif
#if defined( ARM_MULTILIB_ARCH_V4 )
  { "i", REGISTER_CHECK_INITIAL, PSR_I },
#endif
#ifdef ARM_MULTILIB_VFP
  { "d0", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d1", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d2", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d3", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d4", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d5", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d6", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d7", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d8", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d9", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d10", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d11", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d12", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d13", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d14", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d15", REGISTER_CHECK_INITIAL, UINT64_MAX },
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
#ifdef ARM_MULTILIB_VFP
  { "fpscr", REGISTER_CHECK_INITIAL, FPSCR },
#endif
#if defined( ARM_MULTILIB_ARCH_V7M )
  { "basepri", REGISTER_CHECK_INITIAL, BASEPRI },
#endif
  { "sp", REGISTER_CHECK_EXPECTED, WORD },
  { "frame", REGISTER_CHECK_EXPECTED, WORD },
#if defined( ARM_MULTILIB_ARCH_V7M )
  { "basepri saved", REGISTER_CHECK_EXPECTED, BASEPRI },
  { "exc_return saved", REGISTER_CHECK_EXPECTED, WORD },
  { "basepri done", REGISTER_CHECK_EXPECTED, BASEPRI },
  { "exc_return", REGISTER_CHECK_EXPECTED, WORD },
  { "ipsr done", REGISTER_CHECK_EXPECTED, IPSR },
#endif
};

static const RegisterCheckSlot resume_slots[ SLOT_MAX ] = {
  { "r0", SOURCE_R0, WORD },
  { "r1", SOURCE_R1, WORD },
  { "r2", SOURCE_R2, WORD },
  { "r3", SOURCE_R3, WORD },
  { "r4", SOURCE_R4, WORD },
  { "r5", SOURCE_R5, WORD },
  { "r6", SOURCE_R6, WORD },
  { "r7", SOURCE_R7, WORD },
  { "r8", SOURCE_R8, WORD },
  { "r9", SOURCE_R9, WORD },
  { "r10", SOURCE_R10, WORD },
  { "r11", SOURCE_R11, WORD },
  { "r12", SOURCE_R12, WORD },
  { "lr", SOURCE_LR, WORD },
  { "flags", SOURCE_FLAGS, FLAGS },
#ifdef RESUME_HAS_Q
  { "q", SOURCE_Q, Q },
#endif
#ifdef RESUME_HAS_GE
  { "ge", SOURCE_GE, GE },
#endif
#ifdef RESUME_HAS_E
  { "e", SOURCE_E, E },
#endif
#if defined( ARM_MULTILIB_ARCH_V4 )
  { "i", SOURCE_I, PSR_I },
#endif
#ifdef ARM_MULTILIB_VFP
  { "d0", SOURCE_D0, UINT64_MAX },
  { "d1", SOURCE_D1, UINT64_MAX },
  { "d2", SOURCE_D2, UINT64_MAX },
  { "d3", SOURCE_D3, UINT64_MAX },
  { "d4", SOURCE_D4, UINT64_MAX },
  { "d5", SOURCE_D5, UINT64_MAX },
  { "d6", SOURCE_D6, UINT64_MAX },
  { "d7", SOURCE_D7, UINT64_MAX },
  { "d8", SOURCE_D8, UINT64_MAX },
  { "d9", SOURCE_D9, UINT64_MAX },
  { "d10", SOURCE_D10, UINT64_MAX },
  { "d11", SOURCE_D11, UINT64_MAX },
  { "d12", SOURCE_D12, UINT64_MAX },
  { "d13", SOURCE_D13, UINT64_MAX },
  { "d14", SOURCE_D14, UINT64_MAX },
  { "d15", SOURCE_D15, UINT64_MAX },
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
#ifdef ARM_MULTILIB_VFP
  { "fpscr", SOURCE_FPSCR, FPSCR },
#endif
#if defined( ARM_MULTILIB_ARCH_V7M )
  { "basepri", SOURCE_BASEPRI, BASEPRI },
#endif
  { "sp", SOURCE_SP, WORD },
  { "frame", SOURCE_FRAME, WORD },
#if defined( ARM_MULTILIB_ARCH_V7M )
  { "basepri saved", SOURCE_BASEPRI_SAVED, BASEPRI },
  { "exc_return saved", SOURCE_EXC_RETURN_SAVED, WORD },
  { "basepri done", SOURCE_BASEPRI_DONE, BASEPRI },
  { "exc_return", SOURCE_EXC_RETURN, WORD },
  { "ipsr done", SOURCE_IPSR_DONE, IPSR },
#endif
};

static const uint64_t resume_patterns[ SOURCE_MAX ] = {
  0x51525354,
  0x52535455,
  0x53545556,
  0x54555657,
  0x55565758,
  0x56575859,
  0x5758595a,
  0x58595a5b,
  0x595a5b5c,
  0x5a5b5c5d,
  0x5b5c5d5e,
  0x5c5d5e5f,
  0x5d5e5f60,
  0x5e5f6061,
  0x50000000,
#ifdef RESUME_HAS_Q
  0x00000000,
#endif
#ifdef RESUME_HAS_GE
  0x00050000,
#endif
#ifdef RESUME_HAS_E
  0x00000000,
#endif
#if defined( ARM_MULTILIB_ARCH_V4 )
  0x00000000,
#endif
#ifdef ARM_MULTILIB_VFP
  0x6162636465666768,
  0x6263646566676869,
  0x636465666768696a,
  0x6465666768696a6b,
  0x65666768696a6b6c,
  0x666768696a6b6c6d,
  0x6768696a6b6c6d6e,
  0x68696a6b6c6d6e6f,
  0x696a6b6c6d6e6f70,
  0x6a6b6c6d6e6f7071,
  0x6b6c6d6e6f707172,
  0x6c6d6e6f70717273,
  0x6d6e6f7071727374,
  0x6e6f707172737475,
  0x6f70717273747576,
  0x7071727374757677,
#ifdef ARM_MULTILIB_VFP_D32
  0x7172737475767778,
  0x7273747576777879,
  0x737475767778797a,
  0x7475767778797a7b,
  0x75767778797a7b7c,
  0x767778797a7b7c7d,
  0x7778797a7b7c7d7e,
  0x78797a7b7c7d7e7f,
  0x797a7b7c7d7e7f80,
  0x7a7b7c7d7e7f8081,
  0x7b7c7d7e7f808182,
  0x7c7d7e7f80818283,
  0x7d7e7f8081828384,
  0x7e7f808182838485,
  0x7f80818283848586,
  0x8081828384858687,
#endif
#endif
#ifdef ARM_MULTILIB_VFP
  0x90400011,
#endif
#if defined( ARM_MULTILIB_ARCH_V7M )
  RESUME_BASEPRI_TRAP_2,
#endif
};

static uint64_t resume_initial[ SOURCE_MAX ];

static uint32_t *ResumePsr( CPU_Exception_frame *frame )
{
#if defined( ARM_MULTILIB_ARCH_V4 )
  return &frame->register_cpsr;
#else
  return &frame->register_xpsr;
#endif
}

static uint32_t ResumeSetBits( uint32_t psr, uint64_t mask, uint64_t value )
{
  return ( psr & ~(uint32_t) mask ) | ( (uint32_t) value & (uint32_t) mask );
}

/*
 * The handler writes the initial values into the frame, then resumes.  The
 * second trap captures what the resume restored.
 */
static void ResumeFatal(
  rtems_fatal_source source,
  rtems_fatal_code   code,
  void              *arg
)
{
  CPU_Exception_frame *frame;
  const uint64_t      *initial;
  uint32_t            *psr;
  int                  i;

  (void) arg;

  if ( source != RTEMS_FATAL_SOURCE_EXCEPTION ) {
    return;
  }

  frame = (CPU_Exception_frame *) code;
  initial = resume_initial;
  ++resume_trap;

  if ( resume_trap <= 2 ) {
    resume_frame[ resume_trap - 1 ] = frame;
  }

  if ( resume_trap == 1 ) {
    resume_before = *frame;
#ifdef ARM_MULTILIB_VFP
    if ( frame->vfp_context != NULL ) {
      resume_before_vfp = *frame->vfp_context;
      resume_before.vfp_context = &resume_before_vfp;
    }
#endif

    for ( i = 0; i < 13; ++i ) {
      frame->registers[ i ] = (uint32_t) initial[ SOURCE_R0 + i ];
    }

    /*
     * The stack pointer takes the trampoline stack.  A Cortex-M writes the
     * frame of the second trap to the active stack, so the value has to be a
     * stack and not a pattern.
     */
    frame->register_sp = (uintptr_t) &resume_stack[ RESUME_STACK_WORDS ];

    /* Nothing branches through the link register before the second trap. */
    frame->register_lr = (void *) (uintptr_t) initial[ SOURCE_LR ];

    psr = ResumePsr( frame );
    *psr = ResumeSetBits( *psr, FLAGS, initial[ SOURCE_FLAGS ] );
#if defined( RESUME_HAS_Q )
    *psr = ResumeSetBits( *psr, Q, initial[ SOURCE_Q ] );
#endif
#if defined( RESUME_HAS_GE )
    *psr = ResumeSetBits( *psr, GE, initial[ SOURCE_GE ] );
#endif
#if defined( RESUME_HAS_E )
    *psr = ResumeSetBits( *psr, E, initial[ SOURCE_E ] );
#endif
#if defined( ARM_MULTILIB_ARCH_V4 )
    *psr = ResumeSetBits( *psr, PSR_I, initial[ SOURCE_I ] );
#endif

#ifdef ARM_MULTILIB_VFP
    if ( frame->vfp_context != NULL ) {
      ARM_VFP_context *vfp;
      int              n;

      vfp = (ARM_VFP_context *) frame->vfp_context;

      for ( n = 0; n < ARM_VFP_D_COUNT; ++n ) {
        ( &vfp->register_d0 )[ n ] = initial[ SOURCE_D0 + n ];
      }

      vfp->register_fpscr = ResumeSetBits(
        vfp->register_fpscr,
        FPSCR,
        initial[ SOURCE_FPSCR ]
      );
    }
#endif

#if defined( ARM_MULTILIB_ARCH_V7M )
    frame->register_basepri = (uint32_t) initial[ SOURCE_BASEPRI ];
#endif

    /*
     * The resume goes to a label and not to an address computed from the
     * trap.  A Thumb trap is two bytes and an ARM trap is four.
     */
    frame->register_pc = (void *) resume_trap2_label;
  } else {
    resume_after = *frame;
#ifdef ARM_MULTILIB_VFP
    if ( frame->vfp_context != NULL ) {
      resume_after_vfp = *frame->vfp_context;
      resume_after.vfp_context = &resume_after_vfp;
    }
#endif
    SetFatalHandler( NULL, NULL );

    /*
     * The action continues in the block with whatever the resume restored.
     * Put the whole context back, so the compiler finds the registers it
     * expects.  Only the program counter moves on.
     */
    *frame = resume_before;
    frame->register_pc = (void *) resume_done_label;
  }

  _CPU_Exception_resume( frame );
}

static void ResumeRunOnce( void )
{
#if defined( ARM_MULTILIB_ARCH_V7M )
  uint32_t basepri;

  __asm__ volatile( "mrs %0, basepri"
                    : "=r"( basepri ) );
  __asm__ volatile( "msr basepri, %0"
                    :
                    : "r"( RESUME_BASEPRI_TRAP_1 )
                    : "memory" );
#endif

  resume_trap = 0;
  resume_frame[ 0 ] = NULL;
  resume_frame[ 1 ] = NULL;
  memset( &resume_before, 0, sizeof( resume_before ) );
  memset( &resume_after, 0, sizeof( resume_after ) );
#ifdef ARM_MULTILIB_VFP
  memset( &resume_before_vfp, 0, sizeof( resume_before_vfp ) );
  memset( &resume_after_vfp, 0, sizeof( resume_after_vfp ) );
#endif
  SetFatalHandler( ResumeFatal, NULL );
  __asm__ volatile( "udf #0\n"
                    ".globl resume_trap2_label\n"
                    "resume_trap2_label:\n"
                    "udf #0\n"
                    ".globl resume_done_label\n"
                    "resume_done_label:\n"
                    :
                    :
                    : "memory" );

#if defined( ARM_MULTILIB_ARCH_V7M )
  __asm__ volatile( "mrs %0, basepri"
                    : "=r"( resume_basepri_done ) );
  __asm__ volatile( "mrs %0, ipsr"
                    : "=r"( resume_ipsr_done ) );
  __asm__ volatile( "msr basepri, %0"
                    :
                    : "r"( basepri )
                    : "memory" );
#endif
}

static void ResumeRun( RegisterCheck *self, void *arg )
{
  const CPU_Exception_frame *after;
  const uint64_t            *patterns;
  uint32_t                   psr;
  int                        i;

  (void) arg;
  ResumeRunOnce();
  after = &resume_after;
  patterns = resume_patterns;

  for ( i = 0; i < 13; ++i ) {
    RegisterCheckRecord(
      self,
      (size_t) ( SLOT_R0 + i ),
      after->registers[ i ],
      patterns[ SOURCE_R0 + i ]
    );
  }

  RegisterCheckRecord(
    self,
    SLOT_LR,
    (uintptr_t) after->register_lr,
    patterns[ SOURCE_LR ]
  );
#if defined( ARM_MULTILIB_ARCH_V4 )
  psr = after->register_cpsr;
#else
  psr = after->register_xpsr;
#endif
  RegisterCheckRecord( self, SLOT_FLAGS, psr, patterns[ SOURCE_FLAGS ] );
#if defined( RESUME_HAS_Q )
  RegisterCheckRecord( self, SLOT_Q, psr, patterns[ SOURCE_Q ] );
#endif
#if defined( RESUME_HAS_GE )
  RegisterCheckRecord( self, SLOT_GE, psr, patterns[ SOURCE_GE ] );
#endif
#if defined( RESUME_HAS_E )
  RegisterCheckRecord( self, SLOT_E, psr, patterns[ SOURCE_E ] );
#endif
#if defined( ARM_MULTILIB_ARCH_V4 )
  RegisterCheckRecord( self, SLOT_I, psr, patterns[ SOURCE_I ] );
#endif

#ifdef ARM_MULTILIB_VFP
  for ( i = 0; i < ARM_VFP_D_COUNT; ++i ) {
    RegisterCheckRecord(
      self,
      (size_t) ( SLOT_D0 + i ),
      ( &resume_after_vfp.register_d0 )[ i ],
      patterns[ SOURCE_D0 + i ]
    );
  }

  RegisterCheckRecord(
    self,
    SLOT_FPSCR,
    resume_after_vfp.register_fpscr,
    patterns[ SOURCE_FPSCR ]
  );
#endif

  RegisterCheckRecord(
    self,
    SLOT_SP,
    (uintptr_t) after->register_sp,
    (uintptr_t) &resume_stack[ RESUME_STACK_WORDS ]
  );
  RegisterCheckRecord(
    self,
    SLOT_FRAME,
    (uintptr_t) resume_frame[ 1 ],
    (uintptr_t) resume_frame[ 0 ]
  );

#if defined( ARM_MULTILIB_ARCH_V7M )
  RegisterCheckRecord(
    self,
    SLOT_BASEPRI,
    after->register_basepri,
    patterns[ SOURCE_BASEPRI ]
  );
  RegisterCheckRecord(
    self,
    SLOT_BASEPRI_SAVED,
    resume_before.register_basepri,
    RESUME_BASEPRI_TRAP_1
  );
  RegisterCheckRecord(
    self,
    SLOT_EXC_RETURN_SAVED,
    resume_before.exc_return | 0x10,
    0xfffffffd
  );
  RegisterCheckRecord(
    self,
    SLOT_BASEPRI_DONE,
    resume_basepri_done,
    RESUME_BASEPRI_TRAP_1
  );
  RegisterCheckRecord(
    self,
    SLOT_EXC_RETURN,
    after->exc_return,
    resume_before.exc_return
  );
  RegisterCheckRecord( self, SLOT_IPSR_DONE, resume_ipsr_done, 0 );
#endif
}

static RegisterCheck resume_check = {
  .sources = resume_sources,
  .source_count = SOURCE_MAX,
  .slots = resume_slots,
  .slot_count = SLOT_MAX,
  .initial = resume_initial,
  .run = ResumeRun
};

static void CheckSlot( Slot slot )
{
  RegisterCheckVerify( &resume_check, slot );
}

#if defined( RESUME_HAS_IT )
/*
 * This is the IT state of an ITETE EQ instruction.  The first and the third
 * instruction of the block run if the Z flag is set.  The second and the
 * fourth instruction run if the Z flag is clear.
 */
#define RESUME_IT_STATE 0x0bU

#define RESUME_IT_PSR \
  ( ( ( RESUME_IT_STATE & 0x3U ) << 25 ) | ( ( RESUME_IT_STATE >> 2 ) << 10 ) )

#define RESUME_IT_MASK UINT32_C( 0x0600fc00 )

#define RESUME_Z UINT32_C( 0x40000000 )

#define RESUME_IT_INITIAL( reg ) ( UINT32_C( 0x5a5a5a50 ) + ( reg ) )

typedef struct {
  uint32_t z;
  int      trap;
  uint32_t r[ 4 ];
  uint32_t psr;
} ResumeIt;

extern const char resume_it_label[];

extern const char resume_it_done_label[];

static ResumeIt resume_it_set;

static ResumeIt resume_it_clear;

static ResumeIt *resume_it;

static CPU_Exception_frame resume_it_before;

#ifdef ARM_MULTILIB_VFP
static ARM_VFP_context resume_it_before_vfp;
#endif

/*
 * The handler resumes the first trap at the block with the IT state and the Z
 * flag of the run.  The second trap captures R1 to R4 and the status
 * register.
 */
static void ResumeItFatal(
  rtems_fatal_source source,
  rtems_fatal_code   code,
  void              *arg
)
{
  CPU_Exception_frame *frame;
  ResumeIt            *it;
  uint32_t            *psr;
  int                  i;

  (void) arg;

  if ( source != RTEMS_FATAL_SOURCE_EXCEPTION ) {
    return;
  }

  frame = (CPU_Exception_frame *) code;
  it = resume_it;
  psr = ResumePsr( frame );
  ++it->trap;

  if ( it->trap == 1 ) {
    resume_it_before = *frame;
#ifdef ARM_MULTILIB_VFP
    if ( frame->vfp_context != NULL ) {
      resume_it_before_vfp = *frame->vfp_context;
      resume_it_before.vfp_context = &resume_it_before_vfp;
    }
#endif

    for ( i = 1; i <= 4; ++i ) {
      frame->registers[ i ] = RESUME_IT_INITIAL( i );
    }

    *psr = ( *psr & ~( RESUME_IT_MASK | RESUME_Z ) ) | RESUME_IT_PSR | it->z;
    frame->register_pc = (void *) resume_it_label;
  } else {
    for ( i = 1; i <= 4; ++i ) {
      it->r[ i - 1 ] = frame->registers[ i ];
    }

    it->psr = *psr;
    SetFatalHandler( NULL, NULL );
    *frame = resume_it_before;
    frame->register_pc = (void *) resume_it_done_label;
  }

  _CPU_Exception_resume( frame );
}

/*
 * The block holds the encodings of MOVS R1, #1 to MOVS R4, #4.  Inside an IT
 * block, these encodings are conditional MOV instructions which leave the
 * flags unchanged.  The labels exist once, so the function is not inlined.
 */
static RTEMS_NO_INLINE void ResumeItRun( ResumeIt *it, uint32_t z )
{
  memset( it, 0, sizeof( *it ) );
  it->z = z;
  resume_it = it;
  SetFatalHandler( ResumeItFatal, NULL );
  __asm__ volatile( "udf #1\n"
                    ".globl resume_it_label\n"
                    "resume_it_label:\n"
                    ".inst.n 0x2101\n"
                    ".inst.n 0x2202\n"
                    ".inst.n 0x2303\n"
                    ".inst.n 0x2404\n"
                    "udf #2\n"
                    ".globl resume_it_done_label\n"
                    "resume_it_done_label:\n"
                    :
                    :
                    : "r1", "r2", "r3", "r4", "cc", "memory" );
}
#endif

/**
 * @brief Trap, let the handler write a new value into every core register of
 *   the frame and into each status field which the interrupted context owns,
 *   resume, and trap again to capture what the resume restored. Run this once
 *   for each value with the value inverted, then once unchanged.
 */
static void ScoreCpuArmValResume_Action_0( void )
{
  memcpy( resume_initial, resume_patterns, sizeof( resume_initial ) );
  RegisterCheckRun( &resume_check );

  /*
   * Check that the resume restored the ARM core register R0.
   */
  CheckSlot( SLOT_R0 );

  /*
   * Check that the resume restored the ARM core register R1.
   */
  CheckSlot( SLOT_R1 );

  /*
   * Check that the resume restored the ARM core register R2.
   */
  CheckSlot( SLOT_R2 );

  /*
   * Check that the resume restored the ARM core register R3.
   */
  CheckSlot( SLOT_R3 );

  /*
   * Check that the resume restored the ARM core register R4.
   */
  CheckSlot( SLOT_R4 );

  /*
   * Check that the resume restored the ARM core register R5.
   */
  CheckSlot( SLOT_R5 );

  /*
   * Check that the resume restored the ARM core register R6.
   */
  CheckSlot( SLOT_R6 );

  /*
   * Check that the resume restored the ARM core register R7.
   */
  CheckSlot( SLOT_R7 );

  /*
   * Check that the resume restored the ARM core register R8.
   */
  CheckSlot( SLOT_R8 );

  /*
   * Check that the resume restored the ARM core register R9.
   */
  CheckSlot( SLOT_R9 );

  /*
   * Check that the resume restored the ARM core register R10.
   */
  CheckSlot( SLOT_R10 );

  /*
   * Check that the resume restored the ARM core register R11.
   */
  CheckSlot( SLOT_R11 );

  /*
   * Check that the resume restored the ARM core register R12.
   */
  CheckSlot( SLOT_R12 );

  /*
   * Check that the resume restored the stack pointer.
   */
  CheckSlot( SLOT_SP );

  /*
   * Check that the resume restored the link register.
   */
  CheckSlot( SLOT_LR );

  /*
   * Check that the resume restored the ARM FPU register D0.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D0 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D1.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D1 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D2.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D2 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D3.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D3 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D4.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D4 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D5.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D5 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D6.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D6 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D7.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D7 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D8.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D8 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D9.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D9 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D10.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D10 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D11.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D11 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D12.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D12 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D13.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D13 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D14.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D14 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D15.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D15 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D16.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D16 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D17.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D17 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D18.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D18 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D19.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D19 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D20.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D20 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D21.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D21 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D22.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D22 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D23.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D23 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D24.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D24 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D25.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D25 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D26.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D26 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D27.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D27 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D28.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D28 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D29.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D29 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D30.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D30 );
  #endif

  /*
   * Check that the resume restored the ARM FPU register D31.
   */
  #ifdef ARM_MULTILIB_VFP_D32
  CheckSlot( SLOT_D31 );
  #endif

  /*
   * Check that the resume restored FPSCR.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_FPSCR );
  #endif

  /*
   * Check that the resume released the exception frame. The second trap places
   * its frame where the first trap placed it.
   */
  CheckSlot( SLOT_FRAME );

  /*
   * Check that the resume restored the condition flags.
   */
  CheckSlot( SLOT_FLAGS );

  /*
   * Check that the resume restored the Q flag on a processor which has it.
   */
  #if defined( RESUME_HAS_Q )
  CheckSlot( SLOT_Q );
  #endif

  /*
   * Check that the resume restored the GE flags on a processor which has them.
   */
  #if defined( RESUME_HAS_GE )
  CheckSlot( SLOT_GE );
  #endif

  /*
   * Check that the resume restored the E bit on an A or R profile processor of
   * ARMv6 and later.
   */
  #if defined( RESUME_HAS_E )
  CheckSlot( SLOT_E );
  #endif

  /*
   * Check that the resume restored the I bit on an A or R profile processor.
   */
  #if defined( ARM_MULTILIB_ARCH_V4 )
  CheckSlot( SLOT_I );
  #endif

  /*
   * Check that the default exception handler saved BASEPRI to the exception
   * frame on ARMv7-M.
   */
  #if defined( ARM_MULTILIB_ARCH_V7M )
  CheckSlot( SLOT_BASEPRI_SAVED );
  #endif

  /*
   * Check that the default exception handler saved the EXC_RETURN value to the
   * exception frame on ARMv7-M. The trap returns to Thread mode on PSP.
   */
  #if defined( ARM_MULTILIB_ARCH_V7M )
  CheckSlot( SLOT_EXC_RETURN_SAVED );
  #endif

  /*
   * Check that the resume restored BASEPRI from the exception frame on
   * ARMv7-M.
   */
  #if defined( ARM_MULTILIB_ARCH_V7M )
  CheckSlot( SLOT_BASEPRI );
  CheckSlot( SLOT_BASEPRI_DONE );
  #endif

  /*
   * Check that the resume returned through the EXC_RETURN value of the
   * exception frame on ARMv7-M. The second trap reports the same mode, stack
   * and frame type.
   */
  #if defined( ARM_MULTILIB_ARCH_V7M )
  CheckSlot( SLOT_EXC_RETURN );
  #endif

  /*
   * Check that the resume ended the active state of the exception on ARMv7-M.
   * The code after the second resume runs in Thread mode.
   */
  #if defined( ARM_MULTILIB_ARCH_V7M )
  CheckSlot( SLOT_IPSR_DONE );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  RegisterCheckReport( &resume_check );
}

/**
 * @brief Trap in the Thumb state, let the handler resume at a block of four
 *   conditional instructions with the IT state of an ITETE EQ instruction, and
 *   trap again after the block. Run this once with the Z flag set and once
 *   with the Z flag clear.
 */
static void ScoreCpuArmValResume_Action_1( void )
{
  #if defined( RESUME_HAS_IT )
  ResumeItRun( &resume_it_set, RESUME_Z );
  ResumeItRun( &resume_it_clear, 0 );
  #endif

  /*
   * Check that the instructions of the block executed as the IT state and the
   * Z flag of the frame select. With the Z flag set, the first and the third
   * instruction executed. With the Z flag clear, the second and the fourth
   * instruction executed.
   */
  #if defined( RESUME_HAS_IT )
  T_eq_int( resume_it_set.trap, 2 );
  T_eq_u32( resume_it_set.r[ 0 ], 1 );
  T_eq_u32( resume_it_set.r[ 1 ], RESUME_IT_INITIAL( 2 ) );
  T_eq_u32( resume_it_set.r[ 2 ], 3 );
  T_eq_u32( resume_it_set.r[ 3 ], RESUME_IT_INITIAL( 4 ) );
  T_eq_int( resume_it_clear.trap, 2 );
  T_eq_u32( resume_it_clear.r[ 0 ], RESUME_IT_INITIAL( 1 ) );
  T_eq_u32( resume_it_clear.r[ 1 ], 2 );
  T_eq_u32( resume_it_clear.r[ 2 ], RESUME_IT_INITIAL( 3 ) );
  T_eq_u32( resume_it_clear.r[ 3 ], 4 );
  #endif

  /*
   * Check that the block ended before the second trap and that the Z flag kept
   * the value of the frame.
   */
  #if defined( RESUME_HAS_IT )
  T_eq_u32( resume_it_set.psr & ( RESUME_IT_MASK | RESUME_Z ), RESUME_Z );
  T_eq_u32( resume_it_clear.psr & ( RESUME_IT_MASK | RESUME_Z ), 0 );
  #endif
}

/**
 * @fn void T_case_body_ScoreCpuArmValResume( void )
 */
T_TEST_CASE( ScoreCpuArmValResume )
{
  ScoreCpuArmValResume_Action_0();
  ScoreCpuArmValResume_Action_1();
}

/** @} */
