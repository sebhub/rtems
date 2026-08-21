/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuArmValExceptionFrame
 */

/*
 * Copyright (C) 2024 embedded brains GmbH & Co. KG
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

#ifndef _TR_ARM_EXCEPTION_FRAME_H
#define _TR_ARM_EXCEPTION_FRAME_H

#include <rtems.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup ScoreCpuArmValExceptionFrame
 *
 * @{
 */

#if defined( ARM_MULTILIB_ARCH_V7M )
/*
 * The test bits are the flags N, C and Q and the Thumb bit.  The mask
 * excludes bit 9, which records the stack alignment, and the GE bits.
 */
#define XPSR_TEST_MASK 0xff00fdff
#define XPSR_TEST_BITS 0xa9000000
#define CPSR_SET_MASK  0xf8000000
#define CPSR_SET_BITS  0xa8000000
#elif __ARM_ARCH >= 6
#define CPSR_SET_MASK  0xf8000000
#define CPSR_SET_BITS  0xa8000000
#define CPSR_TEST_MASK 0xf800009f
#define CPSR_TEST_BITS 0xa8000013
#else
#define CPSR_SET_MASK  0xf0000000
#define CPSR_SET_BITS  0xa0000000
#define CPSR_TEST_MASK 0xf000009f
#define CPSR_TEST_BITS 0xa0000013
#endif

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

#ifdef ARM_MULTILIB_VFP
#ifdef ARM_MULTILIB_VFP_D32
#define ASM_CLOBBER_VFP                                                       \
  , "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "d10", "d11", \
    "d12", "d13", "d14", "d15", "d16", "d17", "d18", "d19", "d20", "d21",     \
    "d22", "d23", "d24", "d25", "d26", "d27", "d28", "d29", "d30", "d31"
#else
#define ASM_CLOBBER_VFP                                                       \
  , "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "d10", "d11", \
    "d12", "d13", "d14", "d15"
#endif
#else
#define ASM_CLOBBER_VFP
#endif

#define ASM_CLOBBER                                                     \
  "memory", "cc", "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r8", "r9", \
    "r10", "r11", "r12", "r14" ASM_CLOBBER_VFP

#ifdef ARM_MULTILIB_VFP
#ifdef ARM_MULTILIB_VFP_D32
#define ASM_PREPARE_VFP_D32   \
  "vldr.64 d16, [r0, #256]\n" \
  "vldr.64 d17, [r0, #264]\n" \
  "vldr.64 d18, [r0, #272]\n" \
  "vldr.64 d19, [r0, #280]\n" \
  "vldr.64 d20, [r0, #288]\n" \
  "vldr.64 d21, [r0, #296]\n" \
  "vldr.64 d22, [r0, #304]\n" \
  "vldr.64 d23, [r0, #312]\n" \
  "vldr.64 d24, [r0, #320]\n" \
  "vldr.64 d25, [r0, #328]\n" \
  "vldr.64 d26, [r0, #336]\n" \
  "vldr.64 d27, [r0, #344]\n" \
  "vldr.64 d28, [r0, #352]\n" \
  "vldr.64 d29, [r0, #360]\n" \
  "vldr.64 d30, [r0, #368]\n" \
  "vldr.64 d31, [r0, #376]\n"
#define ASM_PREPARE_FPSCR \
  "ldr r1, [r0, #384]\n"  \
  "vmsr FPSCR, r1\n"
#else
#define ASM_PREPARE_VFP_D32
#define ASM_PREPARE_FPSCR \
  "ldr r1, [r0, #264]\n"  \
  "vmsr FPSCR, r1\n"
#endif
#define ASM_PREPARE_VFP       \
  "vldr.64 d0, [r0, #128]\n"  \
  "vldr.64 d1, [r0, #136]\n"  \
  "vldr.64 d2, [r0, #144]\n"  \
  "vldr.64 d3, [r0, #152]\n"  \
  "vldr.64 d4, [r0, #160]\n"  \
  "vldr.64 d5, [r0, #168]\n"  \
  "vldr.64 d6, [r0, #176]\n"  \
  "vldr.64 d7, [r0, #184]\n"  \
  "vldr.64 d8, [r0, #192]\n"  \
  "vldr.64 d9, [r0, #200]\n"  \
  "vldr.64 d10, [r0, #208]\n" \
  "vldr.64 d11, [r0, #216]\n" \
  "vldr.64 d12, [r0, #224]\n" \
  "vldr.64 d13, [r0, #232]\n" \
  "vldr.64 d14, [r0, #240]\n" \
  "vldr.64 d15, [r0, #248]\n" ASM_PREPARE_VFP_D32 ASM_PREPARE_FPSCR
#else
#define ASM_PREPARE_VFP
#endif

#if defined( ARM_MULTILIB_ARCH_V7M )
#define ASM_PREPARE_STATUS_READ  "mrs r1, apsr\n"
#define ASM_PREPARE_STATUS_WRITE "msr APSR_nzcvq, r1\n"

/*
 * The processor stacks the hardware frame of the exception on the stack
 * of the interrupted context, so the stack pointer keeps its value.
 */
#define ASM_PREPARE_SP
#else
#define ASM_PREPARE_STATUS_READ  "mrs r1, cpsr\n"
#define ASM_PREPARE_STATUS_WRITE "msr cpsr, r1\n"
#define ASM_PREPARE_SP           "ldr r13, [r0, #104]\n"
#endif

/*
 * The block loads each value from the table of initial values at the byte
 * offset eight times its source.  It loads r0 last, because r0 holds the
 * address of the table.
 */
#define ASM_PREPARE                                                   \
  "ldr r0, =arm_exception_initial\n" ASM_PREPARE_VFP                  \
  "ldr r1, =exception_restore_r7\n"                                   \
  "str r7, [r1]\n"                                                    \
  "ldr r1, =exception_restore_sp\n"                                   \
  "str sp, [r1]\n" ASM_PREPARE_STATUS_READ "ldr r2, =" RTEMS_XSTRING( \
    CPSR_SET_MASK                                                     \
  ) "\n"                                                              \
    "bic r1, r1, r2\n"                                                \
    "ldr r3, [r0, #120]\n"                                            \
    "and r3, r3, r2\n"                                                \
    "orr r1, r1, r3\n" ASM_PREPARE_STATUS_WRITE "ldr r1, [r0, #8]\n"  \
    "ldr r2, [r0, #16]\n"                                             \
    "ldr r3, [r0, #24]\n"                                             \
    "ldr r4, [r0, #32]\n"                                             \
    "ldr r5, [r0, #40]\n"                                             \
    "ldr r6, [r0, #48]\n"                                             \
    "ldr r7, [r0, #56]\n"                                             \
    "ldr r8, [r0, #64]\n"                                             \
    "ldr r9, [r0, #72]\n"                                             \
    "ldr r10, [r0, #80]\n"                                            \
    "ldr r11, [r0, #88]\n"                                            \
    "ldr r12, [r0, #96]\n" ASM_PREPARE_SP "ldr r14, [r0, #112]\n"     \
    "ldr r0, [r0]\n"

extern uint64_t arm_exception_initial[];

/*
 * The scenario writes the expected value of each register which it sets
 * after the prepare block, and of the program counter.
 */
extern uint32_t exception_expected_registers[ 15 ];

extern const void *exception_expected_pc;

extern char exception_resume_prefetch_abort_label[];

extern volatile uint32_t exception_restore_r7;

extern volatile uint32_t exception_restore_sp;

/*
 * A non-NULL value is the address at which the exception resume
 * continues on ARMv7-M.
 */
extern const void *volatile exception_resume_pc;

extern const CPU_Exception_frame *volatile exception_frame_address;

CPU_Exception_frame *ExceptionFrameInitialize( void );

const CPU_Exception_frame *ExceptionFrameGet( void );

void ExceptionFrameCheckVector( void );

#if defined( ARM_MULTILIB_ARCH_V7M )
void ExceptionFrameCheckStack( void );
#endif

/**
 * @brief Runs the parameterized test case.
 *
 * @param scenario is the scenario. It raises the exception once.
 *
 * @param overrides is the set of the core registers R0 to R14 which the
 *   scenario sets after the prepare block. Bit n stands for register Rn.
 *
 * @param vector is the expected vector number of the exception.
 */
void ScoreCpuArmValExceptionFrame_Run(
  void ( *scenario )( void ),
  uint32_t overrides,
  uint32_t vector
);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _TR_ARM_EXCEPTION_FRAME_H */
