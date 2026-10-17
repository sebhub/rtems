/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuAarch64ValInterrupt
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
 * @defgroup ScoreCpuAarch64ValInterrupt spec:/score/cpu/aarch64/val/interrupt
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
 *   - Check that the interrupt preserved the AArch64 call-used register X0.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X1.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X2.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X3.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X4.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X5.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X6.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X7.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X8.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X9.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X10.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X11.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X12.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X13.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X14.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X15.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X16.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X17.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X18.
 *
 *   - Check that the interrupt preserved the AArch64 wrapper-private register
 *     X19.
 *
 *   - Check that the interrupt preserved the AArch64 wrapper-private register
 *     X20.
 *
 *   - Check that the interrupt preserved the AArch64 wrapper-private register
 *     X21.
 *
 *   - Check that the interrupt preserved the AArch64 call-used register X30.
 *
 *   - Check that the interrupt preserved the condition flags of NZCV.
 *
 *   - Check that the handler ran once before the trap.
 *
 *   - Check that the interrupted code continued at the instruction after the
 *     unmask, so the trap reports its address.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V0.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V0.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V1.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V1.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V2.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V2.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V3.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V3.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V4.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V4.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V5.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V5.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V6.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V6.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V7.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V7.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V8.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V9.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V10.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V11.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V12.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V13.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V14.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V15.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V16.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V16.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V17.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V17.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V18.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V18.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V19.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V19.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V20.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V20.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V21.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V21.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V22.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V22.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V23.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V23.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V24.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V24.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V25.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V25.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V26.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V26.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V27.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V27.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V28.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V28.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V29.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V29.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V30.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V30.
 *
 *   - Check that the interrupt preserved the bottom 64 bits of the AArch64
 *     SIMD and floating-point register V31.
 *
 *   - Check that the interrupt preserved the top 64 bits of the AArch64 SIMD
 *     and floating-point register V31.
 *
 *   - Check that the interrupt preserved FPSR.
 *
 *   - Check that the interrupt preserved FPCR.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

extern const char interrupt_trap_label[];

extern const char interrupt_done_label[];

/* The condition flags are the bits 31 to 28 of NZCV. */
#define NZCV_MASK UINT64_C( 0xf0000000 )

/* These are QC and the cumulative exception flags of FPSR. */
#define FPSR_MASK UINT64_C( 0x0800009f )

/* These are AHP, DN, FZ and the rounding mode of FPCR. */
#define FPCR_MASK UINT64_C( 0x07c00000 )

typedef enum {
  GPR_X0,
  GPR_X1,
  GPR_X2,
  GPR_X3,
  GPR_X4,
  GPR_X5,
  GPR_X6,
  GPR_X7,
  GPR_X8,
  GPR_X9,
  GPR_X10,
  GPR_X11,
  GPR_X12,
  GPR_X13,
  GPR_X14,
  GPR_X15,
  GPR_X16,
  GPR_X17,
  GPR_X18,
  GPR_X19,
  GPR_X20,
  GPR_X21,
  GPR_X30,
  GPR_NZCV,
  GPR_COUNT,
  GPR_PC,
  GPR_MAX
} Gpr;

typedef enum {
  SIMD_V0_LO,
  SIMD_V0_HI,
  SIMD_V1_LO,
  SIMD_V1_HI,
  SIMD_V2_LO,
  SIMD_V2_HI,
  SIMD_V3_LO,
  SIMD_V3_HI,
  SIMD_V4_LO,
  SIMD_V4_HI,
  SIMD_V5_LO,
  SIMD_V5_HI,
  SIMD_V6_LO,
  SIMD_V6_HI,
  SIMD_V7_LO,
  SIMD_V7_HI,
  SIMD_V8_HI,
  SIMD_V9_HI,
  SIMD_V10_HI,
  SIMD_V11_HI,
  SIMD_V12_HI,
  SIMD_V13_HI,
  SIMD_V14_HI,
  SIMD_V15_HI,
  SIMD_V16_LO,
  SIMD_V16_HI,
  SIMD_V17_LO,
  SIMD_V17_HI,
  SIMD_V18_LO,
  SIMD_V18_HI,
  SIMD_V19_LO,
  SIMD_V19_HI,
  SIMD_V20_LO,
  SIMD_V20_HI,
  SIMD_V21_LO,
  SIMD_V21_HI,
  SIMD_V22_LO,
  SIMD_V22_HI,
  SIMD_V23_LO,
  SIMD_V23_HI,
  SIMD_V24_LO,
  SIMD_V24_HI,
  SIMD_V25_LO,
  SIMD_V25_HI,
  SIMD_V26_LO,
  SIMD_V26_HI,
  SIMD_V27_LO,
  SIMD_V27_HI,
  SIMD_V28_LO,
  SIMD_V28_HI,
  SIMD_V29_LO,
  SIMD_V29_HI,
  SIMD_V30_LO,
  SIMD_V30_HI,
  SIMD_V31_LO,
  SIMD_V31_HI,
  SIMD_FPSR,
  SIMD_FPCR,
  SIMD_MAX
} Simd;

static const RegisterCheckSource interrupt_gpr_sources[ GPR_MAX ] = {
  { "x0", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x1", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x2", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x3", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x4", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x5", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x6", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x7", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x8", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x9", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x10", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x11", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x12", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x13", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x14", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x15", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x16", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x17", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x18", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x19", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x20", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x21", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "x30", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "nzcv", REGISTER_CHECK_INITIAL, NZCV_MASK },
  { "count", REGISTER_CHECK_EXPECTED, UINT64_MAX },
  { "pc", REGISTER_CHECK_EXPECTED, UINT64_MAX }
};

static const RegisterCheckSlot interrupt_gpr_slots[ GPR_MAX ] = {
  { "x0", GPR_X0, UINT64_MAX },
  { "x1", GPR_X1, UINT64_MAX },
  { "x2", GPR_X2, UINT64_MAX },
  { "x3", GPR_X3, UINT64_MAX },
  { "x4", GPR_X4, UINT64_MAX },
  { "x5", GPR_X5, UINT64_MAX },
  { "x6", GPR_X6, UINT64_MAX },
  { "x7", GPR_X7, UINT64_MAX },
  { "x8", GPR_X8, UINT64_MAX },
  { "x9", GPR_X9, UINT64_MAX },
  { "x10", GPR_X10, UINT64_MAX },
  { "x11", GPR_X11, UINT64_MAX },
  { "x12", GPR_X12, UINT64_MAX },
  { "x13", GPR_X13, UINT64_MAX },
  { "x14", GPR_X14, UINT64_MAX },
  { "x15", GPR_X15, UINT64_MAX },
  { "x16", GPR_X16, UINT64_MAX },
  { "x17", GPR_X17, UINT64_MAX },
  { "x18", GPR_X18, UINT64_MAX },
  { "x19", GPR_X19, UINT64_MAX },
  { "x20", GPR_X20, UINT64_MAX },
  { "x21", GPR_X21, UINT64_MAX },
  { "x30", GPR_X30, UINT64_MAX },
  { "nzcv", GPR_NZCV, NZCV_MASK },
  { "count", GPR_COUNT, UINT64_MAX },
  { "pc", GPR_PC, UINT64_MAX }
};

static const RegisterCheckSource interrupt_simd_sources[ SIMD_MAX ] = {
  { "v0 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v0 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v1 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v1 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v2 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v2 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v3 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v3 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v4 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v4 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v5 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v5 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v6 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v6 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v7 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v7 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v8 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v9 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v10 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v11 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v12 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v13 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v14 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v15 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v16 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v16 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v17 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v17 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v18 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v18 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v19 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v19 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v20 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v20 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v21 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v21 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v22 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v22 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v23 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v23 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v24 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v24 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v25 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v25 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v26 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v26 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v27 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v27 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v28 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v28 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v29 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v29 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v30 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v30 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v31 lo", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v31 hi", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "fpsr", REGISTER_CHECK_INITIAL, FPSR_MASK },
  { "fpcr", REGISTER_CHECK_INITIAL, FPCR_MASK }
};

static const RegisterCheckSlot interrupt_simd_slots[ SIMD_MAX ] = {
  { "v0 lo", SIMD_V0_LO, UINT64_MAX },
  { "v0 hi", SIMD_V0_HI, UINT64_MAX },
  { "v1 lo", SIMD_V1_LO, UINT64_MAX },
  { "v1 hi", SIMD_V1_HI, UINT64_MAX },
  { "v2 lo", SIMD_V2_LO, UINT64_MAX },
  { "v2 hi", SIMD_V2_HI, UINT64_MAX },
  { "v3 lo", SIMD_V3_LO, UINT64_MAX },
  { "v3 hi", SIMD_V3_HI, UINT64_MAX },
  { "v4 lo", SIMD_V4_LO, UINT64_MAX },
  { "v4 hi", SIMD_V4_HI, UINT64_MAX },
  { "v5 lo", SIMD_V5_LO, UINT64_MAX },
  { "v5 hi", SIMD_V5_HI, UINT64_MAX },
  { "v6 lo", SIMD_V6_LO, UINT64_MAX },
  { "v6 hi", SIMD_V6_HI, UINT64_MAX },
  { "v7 lo", SIMD_V7_LO, UINT64_MAX },
  { "v7 hi", SIMD_V7_HI, UINT64_MAX },
  { "v8 hi", SIMD_V8_HI, UINT64_MAX },
  { "v9 hi", SIMD_V9_HI, UINT64_MAX },
  { "v10 hi", SIMD_V10_HI, UINT64_MAX },
  { "v11 hi", SIMD_V11_HI, UINT64_MAX },
  { "v12 hi", SIMD_V12_HI, UINT64_MAX },
  { "v13 hi", SIMD_V13_HI, UINT64_MAX },
  { "v14 hi", SIMD_V14_HI, UINT64_MAX },
  { "v15 hi", SIMD_V15_HI, UINT64_MAX },
  { "v16 lo", SIMD_V16_LO, UINT64_MAX },
  { "v16 hi", SIMD_V16_HI, UINT64_MAX },
  { "v17 lo", SIMD_V17_LO, UINT64_MAX },
  { "v17 hi", SIMD_V17_HI, UINT64_MAX },
  { "v18 lo", SIMD_V18_LO, UINT64_MAX },
  { "v18 hi", SIMD_V18_HI, UINT64_MAX },
  { "v19 lo", SIMD_V19_LO, UINT64_MAX },
  { "v19 hi", SIMD_V19_HI, UINT64_MAX },
  { "v20 lo", SIMD_V20_LO, UINT64_MAX },
  { "v20 hi", SIMD_V20_HI, UINT64_MAX },
  { "v21 lo", SIMD_V21_LO, UINT64_MAX },
  { "v21 hi", SIMD_V21_HI, UINT64_MAX },
  { "v22 lo", SIMD_V22_LO, UINT64_MAX },
  { "v22 hi", SIMD_V22_HI, UINT64_MAX },
  { "v23 lo", SIMD_V23_LO, UINT64_MAX },
  { "v23 hi", SIMD_V23_HI, UINT64_MAX },
  { "v24 lo", SIMD_V24_LO, UINT64_MAX },
  { "v24 hi", SIMD_V24_HI, UINT64_MAX },
  { "v25 lo", SIMD_V25_LO, UINT64_MAX },
  { "v25 hi", SIMD_V25_HI, UINT64_MAX },
  { "v26 lo", SIMD_V26_LO, UINT64_MAX },
  { "v26 hi", SIMD_V26_HI, UINT64_MAX },
  { "v27 lo", SIMD_V27_LO, UINT64_MAX },
  { "v27 hi", SIMD_V27_HI, UINT64_MAX },
  { "v28 lo", SIMD_V28_LO, UINT64_MAX },
  { "v28 hi", SIMD_V28_HI, UINT64_MAX },
  { "v29 lo", SIMD_V29_LO, UINT64_MAX },
  { "v29 hi", SIMD_V29_HI, UINT64_MAX },
  { "v30 lo", SIMD_V30_LO, UINT64_MAX },
  { "v30 hi", SIMD_V30_HI, UINT64_MAX },
  { "v31 lo", SIMD_V31_LO, UINT64_MAX },
  { "v31 hi", SIMD_V31_HI, UINT64_MAX },
  { "fpsr", SIMD_FPSR, FPSR_MASK },
  { "fpcr", SIMD_FPCR, FPCR_MASK }
};

static const uint64_t interrupt_gpr_patterns[ GPR_MAX ] = {
  UINT64_C( 0x0102030405060708 ),
  UINT64_C( 0x1112131415161718 ),
  UINT64_C( 0x2122232425262728 ),
  UINT64_C( 0x3132333435363738 ),
  UINT64_C( 0x4142434445464748 ),
  UINT64_C( 0x5152535455565758 ),
  UINT64_C( 0x6162636465666768 ),
  UINT64_C( 0x7172737475767778 ),
  UINT64_C( 0x8182838485868788 ),
  UINT64_C( 0x9192939495969798 ),
  UINT64_C( 0xa1a2a3a4a5a6a7a8 ),
  UINT64_C( 0xb1b2b3b4b5b6b7b8 ),
  UINT64_C( 0xc1c2c3c4c5c6c7c8 ),
  UINT64_C( 0xd1d2d3d4d5d6d7d8 ),
  UINT64_C( 0xe1e2e3e4e5e6e7e8 ),
  UINT64_C( 0xf1f2f3f4f5f6f7f8 ),
  UINT64_C( 0x0203040506070808 ),
  UINT64_C( 0x1213141516171818 ),
  UINT64_C( 0x2223242526272828 ),
  UINT64_C( 0x3233343536373838 ),
  UINT64_C( 0x4243444546474848 ),
  UINT64_C( 0x5253545556575858 ),
  UINT64_C( 0x6263646566676868 ),
  UINT64_C( 0x60000000 ),
  1,
  1
};

static const uint64_t interrupt_simd_patterns[ SIMD_MAX ] = {
  UINT64_C( 0x2122232425262728 ),
  UINT64_C( 0x2223242526272829 ),
  UINT64_C( 0x232425262728292a ),
  UINT64_C( 0x2425262728292a2b ),
  UINT64_C( 0x25262728292a2b2c ),
  UINT64_C( 0x262728292a2b2c2d ),
  UINT64_C( 0x2728292a2b2c2d2e ),
  UINT64_C( 0x28292a2b2c2d2e2f ),
  UINT64_C( 0x292a2b2c2d2e2f30 ),
  UINT64_C( 0x2a2b2c2d2e2f3031 ),
  UINT64_C( 0x2b2c2d2e2f303132 ),
  UINT64_C( 0x2c2d2e2f30313233 ),
  UINT64_C( 0x2d2e2f3031323334 ),
  UINT64_C( 0x2e2f303132333435 ),
  UINT64_C( 0x2f30313233343536 ),
  UINT64_C( 0x3031323334353637 ),
  UINT64_C( 0x3132333435363738 ),
  UINT64_C( 0x3233343536373839 ),
  UINT64_C( 0x333435363738393a ),
  UINT64_C( 0x3435363738393a3b ),
  UINT64_C( 0x35363738393a3b3c ),
  UINT64_C( 0x363738393a3b3c3d ),
  UINT64_C( 0x3738393a3b3c3d3e ),
  UINT64_C( 0x38393a3b3c3d3e3f ),
  UINT64_C( 0x393a3b3c3d3e3f40 ),
  UINT64_C( 0x3a3b3c3d3e3f4041 ),
  UINT64_C( 0x3b3c3d3e3f404142 ),
  UINT64_C( 0x3c3d3e3f40414243 ),
  UINT64_C( 0x3d3e3f4041424344 ),
  UINT64_C( 0x3e3f404142434445 ),
  UINT64_C( 0x3f40414243444546 ),
  UINT64_C( 0x4041424344454647 ),
  UINT64_C( 0x4142434445464748 ),
  UINT64_C( 0x4243444546474849 ),
  UINT64_C( 0x434445464748494a ),
  UINT64_C( 0x4445464748494a4b ),
  UINT64_C( 0x45464748494a4b4c ),
  UINT64_C( 0x464748494a4b4c4d ),
  UINT64_C( 0x4748494a4b4c4d4e ),
  UINT64_C( 0x48494a4b4c4d4e4f ),
  UINT64_C( 0x494a4b4c4d4e4f50 ),
  UINT64_C( 0x4a4b4c4d4e4f5051 ),
  UINT64_C( 0x4b4c4d4e4f505152 ),
  UINT64_C( 0x4c4d4e4f50515253 ),
  UINT64_C( 0x4d4e4f5051525354 ),
  UINT64_C( 0x4e4f505152535455 ),
  UINT64_C( 0x4f50515253545556 ),
  UINT64_C( 0x5051525354555657 ),
  UINT64_C( 0x5152535455565758 ),
  UINT64_C( 0x5253545556575859 ),
  UINT64_C( 0x535455565758595a ),
  UINT64_C( 0x5455565758595a5b ),
  UINT64_C( 0x55565758595a5b5c ),
  UINT64_C( 0x565758595a5b5c5d ),
  UINT64_C( 0x5758595a5b5c5d5e ),
  UINT64_C( 0x58595a5b5c5d5e5f ),
  UINT64_C( 0x08000015 ),
  UINT64_C( 0x01800000 )
};

uint64_t interrupt_gpr_initial[ GPR_MAX ];

uint64_t interrupt_simd_initial[ SIMD_MAX ];

/* The block loads q0 to q31 from this image of the vector registers. */
uint64_t interrupt_vector_image[ 64 ] RTEMS_ALIGNED( 16 );

uint64_t interrupt_status[ 3 ];

static CPU_Exception_frame interrupt_frame;

static volatile uint32_t interrupt_count;

static uint32_t interrupt_count_at_trap;

static CallWithinISRRequest interrupt_request;

/* The block loads x0 last, because x0 holds the address of the table. */
#define ISR_LOAD                               \
  "adrp x0, interrupt_status\n"                \
  "add x0, x0, :lo12:interrupt_status\n"       \
  "ldr x1, [x0, #0]\n"                         \
  "msr nzcv, x1\n"                             \
  "ldr x1, [x0, #8]\n"                         \
  "msr fpsr, x1\n"                             \
  "ldr x1, [x0, #16]\n"                        \
  "msr fpcr, x1\n"                             \
  "adrp x0, interrupt_vector_image\n"          \
  "add x0, x0, :lo12:interrupt_vector_image\n" \
  "ldr q0, [x0, #0]\n"                         \
  "ldr q1, [x0, #16]\n"                        \
  "ldr q2, [x0, #32]\n"                        \
  "ldr q3, [x0, #48]\n"                        \
  "ldr q4, [x0, #64]\n"                        \
  "ldr q5, [x0, #80]\n"                        \
  "ldr q6, [x0, #96]\n"                        \
  "ldr q7, [x0, #112]\n"                       \
  "ldr q8, [x0, #128]\n"                       \
  "ldr q9, [x0, #144]\n"                       \
  "ldr q10, [x0, #160]\n"                      \
  "ldr q11, [x0, #176]\n"                      \
  "ldr q12, [x0, #192]\n"                      \
  "ldr q13, [x0, #208]\n"                      \
  "ldr q14, [x0, #224]\n"                      \
  "ldr q15, [x0, #240]\n"                      \
  "ldr q16, [x0, #256]\n"                      \
  "ldr q17, [x0, #272]\n"                      \
  "ldr q18, [x0, #288]\n"                      \
  "ldr q19, [x0, #304]\n"                      \
  "ldr q20, [x0, #320]\n"                      \
  "ldr q21, [x0, #336]\n"                      \
  "ldr q22, [x0, #352]\n"                      \
  "ldr q23, [x0, #368]\n"                      \
  "ldr q24, [x0, #384]\n"                      \
  "ldr q25, [x0, #400]\n"                      \
  "ldr q26, [x0, #416]\n"                      \
  "ldr q27, [x0, #432]\n"                      \
  "ldr q28, [x0, #448]\n"                      \
  "ldr q29, [x0, #464]\n"                      \
  "ldr q30, [x0, #480]\n"                      \
  "ldr q31, [x0, #496]\n"                      \
  "adrp x0, interrupt_gpr_initial\n"           \
  "add x0, x0, :lo12:interrupt_gpr_initial\n"  \
  "ldr x30, [x0, #176]\n"                      \
  "ldr x1, [x0, #8]\n"                         \
  "ldr x2, [x0, #16]\n"                        \
  "ldr x3, [x0, #24]\n"                        \
  "ldr x4, [x0, #32]\n"                        \
  "ldr x5, [x0, #40]\n"                        \
  "ldr x6, [x0, #48]\n"                        \
  "ldr x7, [x0, #56]\n"                        \
  "ldr x8, [x0, #64]\n"                        \
  "ldr x9, [x0, #72]\n"                        \
  "ldr x10, [x0, #80]\n"                       \
  "ldr x11, [x0, #88]\n"                       \
  "ldr x12, [x0, #96]\n"                       \
  "ldr x13, [x0, #104]\n"                      \
  "ldr x14, [x0, #112]\n"                      \
  "ldr x15, [x0, #120]\n"                      \
  "ldr x16, [x0, #128]\n"                      \
  "ldr x17, [x0, #136]\n"                      \
  "ldr x18, [x0, #144]\n"                      \
  "ldr x19, [x0, #152]\n"                      \
  "ldr x20, [x0, #160]\n"                      \
  "ldr x21, [x0, #168]\n"                      \
  "ldr x0, [x0, #0]\n"

#define ISR_CLOBBER                                                           \
  "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11",   \
    "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "x20", "x21",     \
    "x30", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", \
    "v11", "v12", "v13", "v14", "v15", "v16", "v17", "v18", "v19", "v20",     \
    "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29", "v30",     \
    "v31"

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
  frame->register_fpcr = 0;
  frame->register_pc = (void *) interrupt_done_label;
  _CPU_Exception_resume( frame );
}

static void InterruptPrepare( void )
{
  const uint64_t *simd;

  simd = interrupt_simd_initial;
  memset( interrupt_vector_image, 0, sizeof( interrupt_vector_image ) );
  interrupt_vector_image[ 0 ] = simd[ SIMD_V0_LO ];
  interrupt_vector_image[ 1 ] = simd[ SIMD_V0_HI ];
  interrupt_vector_image[ 2 ] = simd[ SIMD_V1_LO ];
  interrupt_vector_image[ 3 ] = simd[ SIMD_V1_HI ];
  interrupt_vector_image[ 4 ] = simd[ SIMD_V2_LO ];
  interrupt_vector_image[ 5 ] = simd[ SIMD_V2_HI ];
  interrupt_vector_image[ 6 ] = simd[ SIMD_V3_LO ];
  interrupt_vector_image[ 7 ] = simd[ SIMD_V3_HI ];
  interrupt_vector_image[ 8 ] = simd[ SIMD_V4_LO ];
  interrupt_vector_image[ 9 ] = simd[ SIMD_V4_HI ];
  interrupt_vector_image[ 10 ] = simd[ SIMD_V5_LO ];
  interrupt_vector_image[ 11 ] = simd[ SIMD_V5_HI ];
  interrupt_vector_image[ 12 ] = simd[ SIMD_V6_LO ];
  interrupt_vector_image[ 13 ] = simd[ SIMD_V6_HI ];
  interrupt_vector_image[ 14 ] = simd[ SIMD_V7_LO ];
  interrupt_vector_image[ 15 ] = simd[ SIMD_V7_HI ];
  interrupt_vector_image[ 17 ] = simd[ SIMD_V8_HI ];
  interrupt_vector_image[ 19 ] = simd[ SIMD_V9_HI ];
  interrupt_vector_image[ 21 ] = simd[ SIMD_V10_HI ];
  interrupt_vector_image[ 23 ] = simd[ SIMD_V11_HI ];
  interrupt_vector_image[ 25 ] = simd[ SIMD_V12_HI ];
  interrupt_vector_image[ 27 ] = simd[ SIMD_V13_HI ];
  interrupt_vector_image[ 29 ] = simd[ SIMD_V14_HI ];
  interrupt_vector_image[ 31 ] = simd[ SIMD_V15_HI ];
  interrupt_vector_image[ 32 ] = simd[ SIMD_V16_LO ];
  interrupt_vector_image[ 33 ] = simd[ SIMD_V16_HI ];
  interrupt_vector_image[ 34 ] = simd[ SIMD_V17_LO ];
  interrupt_vector_image[ 35 ] = simd[ SIMD_V17_HI ];
  interrupt_vector_image[ 36 ] = simd[ SIMD_V18_LO ];
  interrupt_vector_image[ 37 ] = simd[ SIMD_V18_HI ];
  interrupt_vector_image[ 38 ] = simd[ SIMD_V19_LO ];
  interrupt_vector_image[ 39 ] = simd[ SIMD_V19_HI ];
  interrupt_vector_image[ 40 ] = simd[ SIMD_V20_LO ];
  interrupt_vector_image[ 41 ] = simd[ SIMD_V20_HI ];
  interrupt_vector_image[ 42 ] = simd[ SIMD_V21_LO ];
  interrupt_vector_image[ 43 ] = simd[ SIMD_V21_HI ];
  interrupt_vector_image[ 44 ] = simd[ SIMD_V22_LO ];
  interrupt_vector_image[ 45 ] = simd[ SIMD_V22_HI ];
  interrupt_vector_image[ 46 ] = simd[ SIMD_V23_LO ];
  interrupt_vector_image[ 47 ] = simd[ SIMD_V23_HI ];
  interrupt_vector_image[ 48 ] = simd[ SIMD_V24_LO ];
  interrupt_vector_image[ 49 ] = simd[ SIMD_V24_HI ];
  interrupt_vector_image[ 50 ] = simd[ SIMD_V25_LO ];
  interrupt_vector_image[ 51 ] = simd[ SIMD_V25_HI ];
  interrupt_vector_image[ 52 ] = simd[ SIMD_V26_LO ];
  interrupt_vector_image[ 53 ] = simd[ SIMD_V26_HI ];
  interrupt_vector_image[ 54 ] = simd[ SIMD_V27_LO ];
  interrupt_vector_image[ 55 ] = simd[ SIMD_V27_HI ];
  interrupt_vector_image[ 56 ] = simd[ SIMD_V28_LO ];
  interrupt_vector_image[ 57 ] = simd[ SIMD_V28_HI ];
  interrupt_vector_image[ 58 ] = simd[ SIMD_V29_LO ];
  interrupt_vector_image[ 59 ] = simd[ SIMD_V29_HI ];
  interrupt_vector_image[ 60 ] = simd[ SIMD_V30_LO ];
  interrupt_vector_image[ 61 ] = simd[ SIMD_V30_HI ];
  interrupt_vector_image[ 62 ] = simd[ SIMD_V31_LO ];
  interrupt_vector_image[ 63 ] = simd[ SIMD_V31_HI ];
  interrupt_status[ 0 ] = interrupt_gpr_initial[ GPR_NZCV ];
  interrupt_status[ 1 ] = simd[ SIMD_FPSR ];
  interrupt_status[ 2 ] = simd[ SIMD_FPCR ];
  memset( &interrupt_frame, 0, sizeof( interrupt_frame ) );
  interrupt_count = 0;
  interrupt_count_at_trap = 0;
}

/*
 * The interrupt is raised while it is masked, so no call happens after the
 * block loads the registers.  The block unmasks it, and the interrupt is taken
 * there.
 */
static void InterruptRaise( void )
{
  rtems_interrupt_level level;

  InterruptPrepare();
  SetFatalHandler( InterruptFatal, NULL );
  rtems_interrupt_local_disable( level );
  CallWithinISRSubmit( &interrupt_request );
  __asm__ volatile( ISR_LOAD "msr daifclr, #2\n"
                             "isb\n"
                             ".globl interrupt_trap_label\n"
                             "interrupt_trap_label:\n"
                             "brk #0\n"
                             ".globl interrupt_done_label\n"
                             "interrupt_done_label:\n"
                    :
                    :
                    : "memory", "cc", ISR_CLOBBER );
  rtems_interrupt_local_enable( level );
}

static void InterruptRunGpr( RegisterCheck *self, void *arg )
{
  (void) arg;
  InterruptRaise();
  RegisterCheckRecord(
    self,
    GPR_X0,
    interrupt_frame.registers[ 0 ],
    interrupt_gpr_patterns[ GPR_X0 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X1,
    interrupt_frame.registers[ 1 ],
    interrupt_gpr_patterns[ GPR_X1 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X2,
    interrupt_frame.registers[ 2 ],
    interrupt_gpr_patterns[ GPR_X2 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X3,
    interrupt_frame.registers[ 3 ],
    interrupt_gpr_patterns[ GPR_X3 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X4,
    interrupt_frame.registers[ 4 ],
    interrupt_gpr_patterns[ GPR_X4 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X5,
    interrupt_frame.registers[ 5 ],
    interrupt_gpr_patterns[ GPR_X5 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X6,
    interrupt_frame.registers[ 6 ],
    interrupt_gpr_patterns[ GPR_X6 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X7,
    interrupt_frame.registers[ 7 ],
    interrupt_gpr_patterns[ GPR_X7 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X8,
    interrupt_frame.registers[ 8 ],
    interrupt_gpr_patterns[ GPR_X8 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X9,
    interrupt_frame.registers[ 9 ],
    interrupt_gpr_patterns[ GPR_X9 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X10,
    interrupt_frame.registers[ 10 ],
    interrupt_gpr_patterns[ GPR_X10 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X11,
    interrupt_frame.registers[ 11 ],
    interrupt_gpr_patterns[ GPR_X11 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X12,
    interrupt_frame.registers[ 12 ],
    interrupt_gpr_patterns[ GPR_X12 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X13,
    interrupt_frame.registers[ 13 ],
    interrupt_gpr_patterns[ GPR_X13 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X14,
    interrupt_frame.registers[ 14 ],
    interrupt_gpr_patterns[ GPR_X14 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X15,
    interrupt_frame.registers[ 15 ],
    interrupt_gpr_patterns[ GPR_X15 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X16,
    interrupt_frame.registers[ 16 ],
    interrupt_gpr_patterns[ GPR_X16 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X17,
    interrupt_frame.registers[ 17 ],
    interrupt_gpr_patterns[ GPR_X17 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X18,
    interrupt_frame.registers[ 18 ],
    interrupt_gpr_patterns[ GPR_X18 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X19,
    interrupt_frame.registers[ 19 ],
    interrupt_gpr_patterns[ GPR_X19 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X20,
    interrupt_frame.registers[ 20 ],
    interrupt_gpr_patterns[ GPR_X20 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X21,
    interrupt_frame.registers[ 21 ],
    interrupt_gpr_patterns[ GPR_X21 ]
  );
  RegisterCheckRecord(
    self,
    GPR_X30,
    interrupt_frame.registers[ 30 ],
    interrupt_gpr_patterns[ GPR_X30 ]
  );
  RegisterCheckRecord(
    self,
    GPR_NZCV,
    interrupt_frame.register_cpsr & NZCV_MASK,
    interrupt_gpr_patterns[ GPR_NZCV ]
  );
  RegisterCheckRecord( self, GPR_COUNT, interrupt_count_at_trap, 1 );
  RegisterCheckRecord(
    self,
    GPR_PC,
    interrupt_frame.register_pc == interrupt_trap_label,
    1
  );
}

static void InterruptRunSimd( RegisterCheck *self, void *arg )
{
  (void) arg;
  InterruptRaise();
  RegisterCheckRecord(
    self,
    SIMD_V0_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 0 ] ),
    interrupt_simd_patterns[ SIMD_V0_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V0_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 0 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V0_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V1_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 1 ] ),
    interrupt_simd_patterns[ SIMD_V1_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V1_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 1 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V1_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V2_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 2 ] ),
    interrupt_simd_patterns[ SIMD_V2_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V2_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 2 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V2_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V3_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 3 ] ),
    interrupt_simd_patterns[ SIMD_V3_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V3_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 3 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V3_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V4_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 4 ] ),
    interrupt_simd_patterns[ SIMD_V4_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V4_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 4 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V4_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V5_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 5 ] ),
    interrupt_simd_patterns[ SIMD_V5_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V5_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 5 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V5_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V6_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 6 ] ),
    interrupt_simd_patterns[ SIMD_V6_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V6_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 6 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V6_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V7_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 7 ] ),
    interrupt_simd_patterns[ SIMD_V7_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V7_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 7 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V7_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V8_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 8 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V8_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V9_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 9 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V9_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V10_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 10 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V10_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V11_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 11 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V11_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V12_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 12 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V12_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V13_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 13 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V13_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V14_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 14 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V14_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V15_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 15 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V15_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V16_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 16 ] ),
    interrupt_simd_patterns[ SIMD_V16_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V16_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 16 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V16_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V17_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 17 ] ),
    interrupt_simd_patterns[ SIMD_V17_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V17_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 17 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V17_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V18_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 18 ] ),
    interrupt_simd_patterns[ SIMD_V18_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V18_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 18 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V18_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V19_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 19 ] ),
    interrupt_simd_patterns[ SIMD_V19_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V19_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 19 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V19_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V20_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 20 ] ),
    interrupt_simd_patterns[ SIMD_V20_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V20_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 20 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V20_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V21_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 21 ] ),
    interrupt_simd_patterns[ SIMD_V21_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V21_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 21 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V21_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V22_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 22 ] ),
    interrupt_simd_patterns[ SIMD_V22_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V22_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 22 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V22_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V23_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 23 ] ),
    interrupt_simd_patterns[ SIMD_V23_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V23_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 23 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V23_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V24_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 24 ] ),
    interrupt_simd_patterns[ SIMD_V24_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V24_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 24 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V24_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V25_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 25 ] ),
    interrupt_simd_patterns[ SIMD_V25_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V25_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 25 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V25_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V26_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 26 ] ),
    interrupt_simd_patterns[ SIMD_V26_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V26_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 26 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V26_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V27_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 27 ] ),
    interrupt_simd_patterns[ SIMD_V27_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V27_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 27 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V27_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V28_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 28 ] ),
    interrupt_simd_patterns[ SIMD_V28_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V28_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 28 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V28_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V29_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 29 ] ),
    interrupt_simd_patterns[ SIMD_V29_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V29_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 29 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V29_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V30_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 30 ] ),
    interrupt_simd_patterns[ SIMD_V30_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V30_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 30 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V30_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V31_LO,
    (uint64_t) ( interrupt_frame.vector_registers[ 31 ] ),
    interrupt_simd_patterns[ SIMD_V31_LO ]
  );
  RegisterCheckRecord(
    self,
    SIMD_V31_HI,
    (uint64_t) ( interrupt_frame.vector_registers[ 31 ] >> 64 ),
    interrupt_simd_patterns[ SIMD_V31_HI ]
  );
  RegisterCheckRecord(
    self,
    SIMD_FPSR,
    interrupt_frame.register_fpsr & FPSR_MASK,
    interrupt_simd_patterns[ SIMD_FPSR ]
  );
  RegisterCheckRecord(
    self,
    SIMD_FPCR,
    interrupt_frame.register_fpcr & FPCR_MASK,
    interrupt_simd_patterns[ SIMD_FPCR ]
  );
}

static RegisterCheck interrupt_gpr_check = {
  .sources = interrupt_gpr_sources,
  .source_count = GPR_MAX,
  .slots = interrupt_gpr_slots,
  .slot_count = GPR_MAX,
  .initial = interrupt_gpr_initial,
  .run = InterruptRunGpr
};

static RegisterCheck interrupt_simd_check = {
  .sources = interrupt_simd_sources,
  .source_count = SIMD_MAX,
  .slots = interrupt_simd_slots,
  .slot_count = SIMD_MAX,
  .initial = interrupt_simd_initial,
  .run = InterruptRunSimd
};

static void CheckGpr( Gpr slot )
{
  RegisterCheckVerify( &interrupt_gpr_check, slot );
}

static void CheckSimd( Simd slot )
{
  RegisterCheckVerify( &interrupt_simd_check, slot );
}

/**
 * @brief Load a distinct pattern into every register the interrupt has to
 *   preserve. Raise the interrupt. Trap, so the frame captures every register.
 *   Run this once for each register with the pattern of that register
 *   inverted, then once unchanged.
 */
static void ScoreCpuAarch64ValInterrupt_Action_0( void )
{
  memcpy(
    interrupt_gpr_initial,
    interrupt_gpr_patterns,
    sizeof( interrupt_gpr_initial )
  );
  memcpy(
    interrupt_simd_initial,
    interrupt_simd_patterns,
    sizeof( interrupt_simd_initial )
  );
  interrupt_request.handler = InterruptHandler;
  interrupt_request.arg = NULL;
  RegisterCheckRun( &interrupt_gpr_check );
  RegisterCheckRun( &interrupt_simd_check );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X0.
   */
  CheckGpr( GPR_X0 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X1.
   */
  CheckGpr( GPR_X1 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X2.
   */
  CheckGpr( GPR_X2 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X3.
   */
  CheckGpr( GPR_X3 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X4.
   */
  CheckGpr( GPR_X4 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X5.
   */
  CheckGpr( GPR_X5 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X6.
   */
  CheckGpr( GPR_X6 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X7.
   */
  CheckGpr( GPR_X7 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X8.
   */
  CheckGpr( GPR_X8 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X9.
   */
  CheckGpr( GPR_X9 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X10.
   */
  CheckGpr( GPR_X10 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X11.
   */
  CheckGpr( GPR_X11 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X12.
   */
  CheckGpr( GPR_X12 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X13.
   */
  CheckGpr( GPR_X13 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X14.
   */
  CheckGpr( GPR_X14 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X15.
   */
  CheckGpr( GPR_X15 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X16.
   */
  CheckGpr( GPR_X16 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X17.
   */
  CheckGpr( GPR_X17 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X18.
   */
  CheckGpr( GPR_X18 );

  /*
   * Check that the interrupt preserved the AArch64 wrapper-private register
   * X19.
   */
  CheckGpr( GPR_X19 );

  /*
   * Check that the interrupt preserved the AArch64 wrapper-private register
   * X20.
   */
  CheckGpr( GPR_X20 );

  /*
   * Check that the interrupt preserved the AArch64 wrapper-private register
   * X21.
   */
  CheckGpr( GPR_X21 );

  /*
   * Check that the interrupt preserved the AArch64 call-used register X30.
   */
  CheckGpr( GPR_X30 );

  /*
   * Check that the interrupt preserved the condition flags of NZCV.
   */
  CheckGpr( GPR_NZCV );

  /*
   * Check that the handler ran once before the trap.
   */
  CheckGpr( GPR_COUNT );

  /*
   * Check that the interrupted code continued at the instruction after the
   * unmask, so the trap reports its address.
   */
  CheckGpr( GPR_PC );

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  RegisterCheckReport( &interrupt_gpr_check );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V0.
   */
  CheckSimd( SIMD_V0_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V0.
   */
  CheckSimd( SIMD_V0_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V1.
   */
  CheckSimd( SIMD_V1_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V1.
   */
  CheckSimd( SIMD_V1_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V2.
   */
  CheckSimd( SIMD_V2_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V2.
   */
  CheckSimd( SIMD_V2_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V3.
   */
  CheckSimd( SIMD_V3_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V3.
   */
  CheckSimd( SIMD_V3_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V4.
   */
  CheckSimd( SIMD_V4_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V4.
   */
  CheckSimd( SIMD_V4_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V5.
   */
  CheckSimd( SIMD_V5_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V5.
   */
  CheckSimd( SIMD_V5_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V6.
   */
  CheckSimd( SIMD_V6_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V6.
   */
  CheckSimd( SIMD_V6_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V7.
   */
  CheckSimd( SIMD_V7_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V7.
   */
  CheckSimd( SIMD_V7_HI );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V8.
   */
  CheckSimd( SIMD_V8_HI );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V9.
   */
  CheckSimd( SIMD_V9_HI );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V10.
   */
  CheckSimd( SIMD_V10_HI );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V11.
   */
  CheckSimd( SIMD_V11_HI );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V12.
   */
  CheckSimd( SIMD_V12_HI );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V13.
   */
  CheckSimd( SIMD_V13_HI );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V14.
   */
  CheckSimd( SIMD_V14_HI );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V15.
   */
  CheckSimd( SIMD_V15_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V16.
   */
  CheckSimd( SIMD_V16_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V16.
   */
  CheckSimd( SIMD_V16_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V17.
   */
  CheckSimd( SIMD_V17_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V17.
   */
  CheckSimd( SIMD_V17_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V18.
   */
  CheckSimd( SIMD_V18_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V18.
   */
  CheckSimd( SIMD_V18_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V19.
   */
  CheckSimd( SIMD_V19_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V19.
   */
  CheckSimd( SIMD_V19_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V20.
   */
  CheckSimd( SIMD_V20_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V20.
   */
  CheckSimd( SIMD_V20_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V21.
   */
  CheckSimd( SIMD_V21_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V21.
   */
  CheckSimd( SIMD_V21_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V22.
   */
  CheckSimd( SIMD_V22_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V22.
   */
  CheckSimd( SIMD_V22_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V23.
   */
  CheckSimd( SIMD_V23_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V23.
   */
  CheckSimd( SIMD_V23_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V24.
   */
  CheckSimd( SIMD_V24_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V24.
   */
  CheckSimd( SIMD_V24_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V25.
   */
  CheckSimd( SIMD_V25_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V25.
   */
  CheckSimd( SIMD_V25_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V26.
   */
  CheckSimd( SIMD_V26_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V26.
   */
  CheckSimd( SIMD_V26_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V27.
   */
  CheckSimd( SIMD_V27_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V27.
   */
  CheckSimd( SIMD_V27_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V28.
   */
  CheckSimd( SIMD_V28_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V28.
   */
  CheckSimd( SIMD_V28_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V29.
   */
  CheckSimd( SIMD_V29_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V29.
   */
  CheckSimd( SIMD_V29_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V30.
   */
  CheckSimd( SIMD_V30_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V30.
   */
  CheckSimd( SIMD_V30_HI );

  /*
   * Check that the interrupt preserved the bottom 64 bits of the AArch64 SIMD
   * and floating-point register V31.
   */
  CheckSimd( SIMD_V31_LO );

  /*
   * Check that the interrupt preserved the top 64 bits of the AArch64 SIMD and
   * floating-point register V31.
   */
  CheckSimd( SIMD_V31_HI );

  /*
   * Check that the interrupt preserved FPSR.
   */
  CheckSimd( SIMD_FPSR );

  /*
   * Check that the interrupt preserved FPCR.
   */
  CheckSimd( SIMD_FPCR );

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  RegisterCheckReport( &interrupt_simd_check );
}

/**
 * @fn void T_case_body_ScoreCpuAarch64ValInterrupt( void )
 */
T_TEST_CASE( ScoreCpuAarch64ValInterrupt )
{
  ScoreCpuAarch64ValInterrupt_Action_0();
}

/** @} */
