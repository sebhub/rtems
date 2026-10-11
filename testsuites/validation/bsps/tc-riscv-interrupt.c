/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuRiscvValInterrupt
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
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <rtems/score/cpuimpl.h>
#include <rtems/score/riscv-utility.h>

#include "tx-register-check.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuRiscvValInterrupt spec:/score/cpu/riscv/val/interrupt
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @brief Checks that the interrupt processing preserves the registers of the
 *   interrupted context.
 *
 * The interrupt comes from the TM27 support of the BSP. The handler overwrites
 * the call-used registers, since a handler of an application may do the same.
 * The interrupt wrapper uses s0 and s1 for its own bookkeeping.
 *
 * This test case performs the following actions:
 *
 * - Raise an interrupt with interrupts disabled. Load a distinct pattern into
 *   every call-used register, into s0, s1 and fcsr. Enable interrupts. The
 *   handler writes a value into each call-used register and into fcsr. Trap,
 *   so the frame reports every register. Run this once for each value with the
 *   pattern of that value inverted, then once unchanged.
 *
 *   - Check that the interrupt processing preserved the RISC-V return address
 *     register ra.
 *
 *   - Check that the interrupt processing preserved the RISC-V register t0.
 *
 *   - Check that the interrupt processing preserved the RISC-V register t1.
 *
 *   - Check that the interrupt processing preserved the RISC-V register t2.
 *
 *   - Check that the interrupt processing preserved the RISC-V register s0.
 *
 *   - Check that the interrupt processing preserved the RISC-V register s1.
 *
 *   - Check that the interrupt processing preserved the RISC-V register a0.
 *
 *   - Check that the interrupt processing preserved the RISC-V register a1.
 *
 *   - Check that the interrupt processing preserved the RISC-V register a2.
 *
 *   - Check that the interrupt processing preserved the RISC-V register a3.
 *
 *   - Check that the interrupt processing preserved the RISC-V register a4.
 *
 *   - Check that the interrupt processing preserved the RISC-V register a5.
 *
 *   - Check that the interrupt processing preserved the RISC-V register a6.
 *
 *   - Check that the interrupt processing preserved the RISC-V register a7.
 *
 *   - Check that the interrupt processing preserved the RISC-V register t3.
 *
 *   - Check that the interrupt processing preserved the RISC-V register t4.
 *
 *   - Check that the interrupt processing preserved the RISC-V register t5.
 *
 *   - Check that the interrupt processing preserved the RISC-V register t6.
 *
 *   - Check that the interrupt processing preserved the RISC-V stack pointer
 *     register sp.
 *
 *   - Check that the interrupt processing restored the interrupt enable and
 *     the privilege of the status register of the interrupted context.
 *
 *   - Check that the interrupted context continued at the instruction after
 *     the interrupt enable, so that the trap happened there.
 *
 *   - Check that the interrupt happened once before the trap.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft0.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft1.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft2.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft3.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft4.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft5.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft6.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft7.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register fa0.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register fa1.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register fa2.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register fa3.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register fa4.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register fa5.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register fa6.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register fa7.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft8.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft9.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft10.
 *
 *   - Check that the interrupt processing preserved the RISC-V floating-point
 *     register ft11.
 *
 *   - Check that the interrupt processing preserved the rounding mode and the
 *     exception flags of the RISC-V register fcsr.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

#define _RTEMS_TMTEST27
#include <tm27.h>

#define WORD ( (uint64_t) UINTPTR_MAX )

#if __riscv_flen == 64
#define FWORD UINT64_MAX
#define FLOAD "fld"
#elif __riscv_flen == 32
#define FWORD UINT64_C( 0xffffffff )
#define FLOAD "flw"
#endif

/*
 * The mask covers the rounding mode and the exception flags of fcsr.  The
 * pattern sets the rounding mode three, and the inverted pattern sets the
 * rounding mode four.  Both are valid.  The handler writes zero.
 */
#define FCSR_MASK UINT64_C( 0xff )

#define FCSR_PATTERN UINT64_C( 0x75 )

#if defined( RISCV_USE_S_MODE )
#define STATUS_IE        "2"
#define STATUS_TEST_MASK ( SSTATUS_SPIE | SSTATUS_SPP )
#define STATUS_REGISTER  "sstatus"
#else
#define STATUS_IE        "8"
#define STATUS_TEST_MASK ( MSTATUS_MPIE | MSTATUS_MPP )
#define STATUS_REGISTER  "mstatus"
#endif

#if __riscv_xlen == 64
#define LREG "ld"
#define SREG "sd"
#else
#define LREG "lw"
#define SREG "sw"
#endif

typedef enum {
  SOURCE_RA,
  SOURCE_T0,
  SOURCE_T1,
  SOURCE_T2,
  SOURCE_S0,
  SOURCE_S1,
  SOURCE_A0,
  SOURCE_A1,
  SOURCE_A2,
  SOURCE_A3,
  SOURCE_A4,
  SOURCE_A5,
  SOURCE_A6,
  SOURCE_A7,
  SOURCE_T3,
  SOURCE_T4,
  SOURCE_T5,
  SOURCE_T6,
  SOURCE_SP,
  SOURCE_STATUS,
  SOURCE_EPC,
  SOURCE_COUNT,
#if __riscv_flen > 0
  SOURCE_FT0,
  SOURCE_FT1,
  SOURCE_FT2,
  SOURCE_FT3,
  SOURCE_FT4,
  SOURCE_FT5,
  SOURCE_FT6,
  SOURCE_FT7,
  SOURCE_FA0,
  SOURCE_FA1,
  SOURCE_FA2,
  SOURCE_FA3,
  SOURCE_FA4,
  SOURCE_FA5,
  SOURCE_FA6,
  SOURCE_FA7,
  SOURCE_FT8,
  SOURCE_FT9,
  SOURCE_FT10,
  SOURCE_FT11,
  SOURCE_FCSR,
#endif
  SOURCE_MAX
} Source;

/* Each source feeds one slot of the same index */
typedef enum {
  SLOT_RA,
  SLOT_T0,
  SLOT_T1,
  SLOT_T2,
  SLOT_S0,
  SLOT_S1,
  SLOT_A0,
  SLOT_A1,
  SLOT_A2,
  SLOT_A3,
  SLOT_A4,
  SLOT_A5,
  SLOT_A6,
  SLOT_A7,
  SLOT_T3,
  SLOT_T4,
  SLOT_T5,
  SLOT_T6,
  SLOT_SP,
  SLOT_STATUS,
  SLOT_EPC,
  SLOT_COUNT,
#if __riscv_flen > 0
  SLOT_FT0,
  SLOT_FT1,
  SLOT_FT2,
  SLOT_FT3,
  SLOT_FT4,
  SLOT_FT5,
  SLOT_FT6,
  SLOT_FT7,
  SLOT_FA0,
  SLOT_FA1,
  SLOT_FA2,
  SLOT_FA3,
  SLOT_FA4,
  SLOT_FA5,
  SLOT_FA6,
  SLOT_FA7,
  SLOT_FT8,
  SLOT_FT9,
  SLOT_FT10,
  SLOT_FT11,
  SLOT_FCSR,
#endif
  SLOT_MAX
} Slot;

/* The runner loads each value from the byte offset eight times its source */
RTEMS_STATIC_ASSERT( SOURCE_RA == 0, source_ra );
RTEMS_STATIC_ASSERT( SOURCE_T0 == 1, source_t0 );
RTEMS_STATIC_ASSERT( SOURCE_T6 == 17, source_t6 );

#if __riscv_flen > 0
RTEMS_STATIC_ASSERT( SOURCE_FT0 == 22, source_ft0 );
RTEMS_STATIC_ASSERT( SOURCE_FCSR == 42, source_fcsr );
#endif

static const RegisterCheckSource interrupt_sources[ SOURCE_MAX ] = {
  { "ra", REGISTER_CHECK_INITIAL, WORD },
  { "t0", REGISTER_CHECK_INITIAL, WORD },
  { "t1", REGISTER_CHECK_INITIAL, WORD },
  { "t2", REGISTER_CHECK_INITIAL, WORD },
  { "s0", REGISTER_CHECK_INITIAL, WORD },
  { "s1", REGISTER_CHECK_INITIAL, WORD },
  { "a0", REGISTER_CHECK_INITIAL, WORD },
  { "a1", REGISTER_CHECK_INITIAL, WORD },
  { "a2", REGISTER_CHECK_INITIAL, WORD },
  { "a3", REGISTER_CHECK_INITIAL, WORD },
  { "a4", REGISTER_CHECK_INITIAL, WORD },
  { "a5", REGISTER_CHECK_INITIAL, WORD },
  { "a6", REGISTER_CHECK_INITIAL, WORD },
  { "a7", REGISTER_CHECK_INITIAL, WORD },
  { "t3", REGISTER_CHECK_INITIAL, WORD },
  { "t4", REGISTER_CHECK_INITIAL, WORD },
  { "t5", REGISTER_CHECK_INITIAL, WORD },
  { "t6", REGISTER_CHECK_INITIAL, WORD },
  { "sp", REGISTER_CHECK_EXPECTED, WORD },
  { "status", REGISTER_CHECK_EXPECTED, STATUS_TEST_MASK },
  { "epc", REGISTER_CHECK_EXPECTED, WORD },
  { "count", REGISTER_CHECK_EXPECTED, WORD },
#if __riscv_flen > 0
  { "ft0", REGISTER_CHECK_INITIAL, FWORD },
  { "ft1", REGISTER_CHECK_INITIAL, FWORD },
  { "ft2", REGISTER_CHECK_INITIAL, FWORD },
  { "ft3", REGISTER_CHECK_INITIAL, FWORD },
  { "ft4", REGISTER_CHECK_INITIAL, FWORD },
  { "ft5", REGISTER_CHECK_INITIAL, FWORD },
  { "ft6", REGISTER_CHECK_INITIAL, FWORD },
  { "ft7", REGISTER_CHECK_INITIAL, FWORD },
  { "fa0", REGISTER_CHECK_INITIAL, FWORD },
  { "fa1", REGISTER_CHECK_INITIAL, FWORD },
  { "fa2", REGISTER_CHECK_INITIAL, FWORD },
  { "fa3", REGISTER_CHECK_INITIAL, FWORD },
  { "fa4", REGISTER_CHECK_INITIAL, FWORD },
  { "fa5", REGISTER_CHECK_INITIAL, FWORD },
  { "fa6", REGISTER_CHECK_INITIAL, FWORD },
  { "fa7", REGISTER_CHECK_INITIAL, FWORD },
  { "ft8", REGISTER_CHECK_INITIAL, FWORD },
  { "ft9", REGISTER_CHECK_INITIAL, FWORD },
  { "ft10", REGISTER_CHECK_INITIAL, FWORD },
  { "ft11", REGISTER_CHECK_INITIAL, FWORD },
  { "fcsr", REGISTER_CHECK_INITIAL, FCSR_MASK },
#endif
};

static const RegisterCheckSlot interrupt_slots[ SLOT_MAX ] = {
  { "ra", SOURCE_RA, WORD },
  { "t0", SOURCE_T0, WORD },
  { "t1", SOURCE_T1, WORD },
  { "t2", SOURCE_T2, WORD },
  { "s0", SOURCE_S0, WORD },
  { "s1", SOURCE_S1, WORD },
  { "a0", SOURCE_A0, WORD },
  { "a1", SOURCE_A1, WORD },
  { "a2", SOURCE_A2, WORD },
  { "a3", SOURCE_A3, WORD },
  { "a4", SOURCE_A4, WORD },
  { "a5", SOURCE_A5, WORD },
  { "a6", SOURCE_A6, WORD },
  { "a7", SOURCE_A7, WORD },
  { "t3", SOURCE_T3, WORD },
  { "t4", SOURCE_T4, WORD },
  { "t5", SOURCE_T5, WORD },
  { "t6", SOURCE_T6, WORD },
  { "sp", SOURCE_SP, WORD },
  { "status", SOURCE_STATUS, STATUS_TEST_MASK },
  { "epc", SOURCE_EPC, WORD },
  { "count", SOURCE_COUNT, WORD },
#if __riscv_flen > 0
  { "ft0", SOURCE_FT0, FWORD },
  { "ft1", SOURCE_FT1, FWORD },
  { "ft2", SOURCE_FT2, FWORD },
  { "ft3", SOURCE_FT3, FWORD },
  { "ft4", SOURCE_FT4, FWORD },
  { "ft5", SOURCE_FT5, FWORD },
  { "ft6", SOURCE_FT6, FWORD },
  { "ft7", SOURCE_FT7, FWORD },
  { "fa0", SOURCE_FA0, FWORD },
  { "fa1", SOURCE_FA1, FWORD },
  { "fa2", SOURCE_FA2, FWORD },
  { "fa3", SOURCE_FA3, FWORD },
  { "fa4", SOURCE_FA4, FWORD },
  { "fa5", SOURCE_FA5, FWORD },
  { "fa6", SOURCE_FA6, FWORD },
  { "fa7", SOURCE_FA7, FWORD },
  { "ft8", SOURCE_FT8, FWORD },
  { "ft9", SOURCE_FT9, FWORD },
  { "ft10", SOURCE_FT10, FWORD },
  { "ft11", SOURCE_FT11, FWORD },
  { "fcsr", SOURCE_FCSR, FCSR_MASK },
#endif
};

/* These are the frame members of the loaded values */
static const uint16_t interrupt_members[] = {
  offsetof( CPU_Exception_frame, Interrupt_frame.ra ),
  offsetof( CPU_Exception_frame, Interrupt_frame.t0 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.t1 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.t2 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.s0 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.s1 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.a0 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.a1 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.a2 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.a3 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.a4 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.a5 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.a6 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.a7 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.t3 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.t4 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.t5 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.t6 ),
#if __riscv_flen > 0
  offsetof( CPU_Exception_frame, Interrupt_frame.ft0 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft1 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft2 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft3 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft4 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft5 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft6 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft7 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.fa0 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.fa1 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.fa2 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.fa3 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.fa4 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.fa5 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.fa6 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.fa7 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft8 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft9 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft10 ),
  offsetof( CPU_Exception_frame, Interrupt_frame.ft11 ),
#endif
};

static uint64_t GetPattern( size_t source )
{
#if __riscv_flen > 0
  if ( source == SOURCE_FCSR ) {
    return FCSR_PATTERN;
  }

  if ( source >= SOURCE_FT0 ) {
    return ( UINT64_C( 0x8182838485868788 ) +
             UINT64_C( 0x0101010101010101 ) * source ) &
           FWORD;
  }
#endif

  return ( UINT64_C( 0x1112131415161718 ) +
           UINT64_C( 0x0101010101010101 ) * source ) &
         WORD;
}

static uint64_t GetMember( const CPU_Exception_frame *frame, size_t source )
{
  const char *member;
  uintptr_t   word;

#if __riscv_flen > 0
  if ( source >= SOURCE_FT0 ) {
#if __riscv_flen == 64
    uint64_t bits;
#else
    uint32_t bits;
#endif

    member = (const char *) frame +
             interrupt_members[ SOURCE_T6 + 1 + source - SOURCE_FT0 ];
    memcpy( &bits, member, sizeof( bits ) );
    return bits;
  }
#endif

  member = (const char *) frame + interrupt_members[ source ];
  memcpy( &word, member, sizeof( word ) );
  return word;
}

uint64_t interrupt_initial[ SOURCE_MAX ];

/* The ra, the s0, the s1 and the sp of the caller of the runner */
uint64_t interrupt_saved[ 4 ];

static CPU_Exception_frame interrupt_frame;

static volatile int interrupt_count;

static int interrupt_count_at_trap;

extern const char interrupt_trap_label[];

extern const char interrupt_done_label[];

void InterruptRun( void );

void InterruptClobber( void );

/*
 * The handler writes a value into each call-used register and into fcsr.  An
 * interrupt wrapper which fails to restore one of them then shows that value.
 */
/* clang-format off */
__asm__ (
  "  .globl InterruptClobber\n"
  "InterruptClobber:\n"
  "  li t0, -1\n"
  "  li t1, -1\n"
  "  li t2, -1\n"
  "  li t3, -1\n"
  "  li t4, -1\n"
  "  li t5, -1\n"
  "  li t6, -1\n"
  "  li a0, -1\n"
  "  li a1, -1\n"
  "  li a2, -1\n"
  "  li a3, -1\n"
  "  li a4, -1\n"
  "  li a5, -1\n"
  "  li a6, -1\n"
  "  li a7, -1\n"
#if __riscv_flen > 0

  "  fmv.w.x ft0, zero\n"
  "  fmv.w.x ft1, zero\n"
  "  fmv.w.x ft2, zero\n"
  "  fmv.w.x ft3, zero\n"
  "  fmv.w.x ft4, zero\n"
  "  fmv.w.x ft5, zero\n"
  "  fmv.w.x ft6, zero\n"
  "  fmv.w.x ft7, zero\n"
  "  fmv.w.x fa0, zero\n"
  "  fmv.w.x fa1, zero\n"
  "  fmv.w.x fa2, zero\n"
  "  fmv.w.x fa3, zero\n"
  "  fmv.w.x fa4, zero\n"
  "  fmv.w.x fa5, zero\n"
  "  fmv.w.x fa6, zero\n"
  "  fmv.w.x fa7, zero\n"
  "  fmv.w.x ft8, zero\n"
  "  fmv.w.x ft9, zero\n"
  "  fmv.w.x ft10, zero\n"
  "  fmv.w.x ft11, zero\n"
  "  fscsr zero\n"
#endif
  "  ret\n"
);
/* clang-format on */

/*
 * The block saves the registers of its caller which it changes.  It loads
 * each value from the table of initial values.  It loads t0 last, because t0
 * holds the address of the table.  The interrupt is pending, so it arrives
 * after the instruction which enables interrupts.  The trap which follows
 * captures every register.
 */
/* clang-format off */
__asm__ (
  "  .globl InterruptRun\n"
  "InterruptRun:\n"
  "  la t0, interrupt_saved\n"
  "  " SREG " ra, 0(t0)\n"
  "  " SREG " s0, 8(t0)\n"
  "  " SREG " s1, 16(t0)\n"
  "  " SREG " sp, 24(t0)\n"
  "  la t0, interrupt_initial\n"
#if __riscv_flen > 0
  "  " FLOAD " ft0, 176(t0)\n"
  "  " FLOAD " ft1, 184(t0)\n"
  "  " FLOAD " ft2, 192(t0)\n"
  "  " FLOAD " ft3, 200(t0)\n"
  "  " FLOAD " ft4, 208(t0)\n"
  "  " FLOAD " ft5, 216(t0)\n"
  "  " FLOAD " ft6, 224(t0)\n"
  "  " FLOAD " ft7, 232(t0)\n"
  "  " FLOAD " fa0, 240(t0)\n"
  "  " FLOAD " fa1, 248(t0)\n"
  "  " FLOAD " fa2, 256(t0)\n"
  "  " FLOAD " fa3, 264(t0)\n"
  "  " FLOAD " fa4, 272(t0)\n"
  "  " FLOAD " fa5, 280(t0)\n"
  "  " FLOAD " fa6, 288(t0)\n"
  "  " FLOAD " fa7, 296(t0)\n"
  "  " FLOAD " ft8, 304(t0)\n"
  "  " FLOAD " ft9, 312(t0)\n"
  "  " FLOAD " ft10, 320(t0)\n"
  "  " FLOAD " ft11, 328(t0)\n"
  "  lw t1, 336(t0)\n"
  "  fscsr t1\n"
#endif
  "  " LREG " ra, 0(t0)\n"
  "  " LREG " t1, 16(t0)\n"
  "  " LREG " t2, 24(t0)\n"
  "  " LREG " s0, 32(t0)\n"
  "  " LREG " s1, 40(t0)\n"
  "  " LREG " a0, 48(t0)\n"
  "  " LREG " a1, 56(t0)\n"
  "  " LREG " a2, 64(t0)\n"
  "  " LREG " a3, 72(t0)\n"
  "  " LREG " a4, 80(t0)\n"
  "  " LREG " a5, 88(t0)\n"
  "  " LREG " a6, 96(t0)\n"
  "  " LREG " a7, 104(t0)\n"
  "  " LREG " t3, 112(t0)\n"
  "  " LREG " t4, 120(t0)\n"
  "  " LREG " t5, 128(t0)\n"
  "  " LREG " t6, 136(t0)\n"
  "  " LREG " t0, 8(t0)\n"
  "  .option push\n"
  "  .option arch, +zicsr\n"
  "  csrsi " STATUS_REGISTER ", " STATUS_IE "\n"
  "  .option pop\n"
  "  .globl interrupt_trap_label\n"
  "interrupt_trap_label:\n"
  "  unimp\n"
  "  .globl interrupt_done_label\n"
  "interrupt_done_label:\n"
  "  la t0, interrupt_saved\n"
  "  " LREG " ra, 0(t0)\n"
  "  " LREG " s0, 8(t0)\n"
  "  " LREG " s1, 16(t0)\n"
  "  ret\n"
);
/* clang-format on */

static void InterruptHandler( void *arg )
{
  (void) arg;
  Clear_tm27_intr();
  InterruptClobber();
  ++interrupt_count;
}

/*
 * The trap after the interrupt captures every register.  The resume continues
 * at the code which loads the registers of the caller of the runner.
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
  interrupt_count_at_trap = interrupt_count;
  SetFatalHandler( NULL, NULL );
  frame->Interrupt_frame.epc = (uintptr_t) interrupt_done_label;
  _CPU_Exception_resume( frame );
}

static void InterruptRunCheck( RegisterCheck *self, void *arg )
{
  rtems_interrupt_level level;
  size_t                i;

  (void) arg;
  memset( &interrupt_frame, 0, sizeof( interrupt_frame ) );
  interrupt_count = 0;
  interrupt_count_at_trap = 0;
  SetFatalHandler( InterruptFatal, NULL );
  rtems_interrupt_local_disable( level );
  Cause_tm27_intr();
  InterruptRun();
  rtems_interrupt_local_enable( level );
  SetFatalHandler( NULL, NULL );

  for ( i = SOURCE_RA; i <= SOURCE_T6; ++i ) {
    RegisterCheckRecord(
      self,
      i,
      GetMember( &interrupt_frame, i ),
      GetPattern( i )
    );
  }

  RegisterCheckRecord(
    self,
    SLOT_SP,
    interrupt_frame.sp,
    interrupt_saved[ 3 ]
  );
  RegisterCheckRecord(
    self,
    SLOT_STATUS,
    interrupt_frame.Interrupt_frame.status,
    STATUS_TEST_MASK
  );
  RegisterCheckRecord(
    self,
    SLOT_EPC,
    interrupt_frame.Interrupt_frame.epc,
    (uintptr_t) interrupt_trap_label
  );
  RegisterCheckRecord(
    self,
    SLOT_COUNT,
    (uint64_t) interrupt_count_at_trap,
    1
  );

#if __riscv_flen > 0
  for ( i = SOURCE_FT0; i < SOURCE_FCSR; ++i ) {
    RegisterCheckRecord(
      self,
      i,
      GetMember( &interrupt_frame, i ),
      GetPattern( i )
    );
  }

  RegisterCheckRecord(
    self,
    SLOT_FCSR,
    interrupt_frame.Interrupt_frame.fcsr,
    FCSR_PATTERN
  );
#endif
}

static RegisterCheck interrupt_check = {
  .sources = interrupt_sources,
  .source_count = SOURCE_MAX,
  .slots = interrupt_slots,
  .slot_count = SLOT_MAX,
  .initial = interrupt_initial,
  .run = InterruptRunCheck
};

static void InterruptPrepare( void )
{
  size_t i;

  for ( i = 0; i < SOURCE_MAX; ++i ) {
    interrupt_initial[ i ] = GetPattern( i );
  }

  Install_tm27_vector( InterruptHandler );
}

/**
 * @brief Raise an interrupt with interrupts disabled. Load a distinct pattern
 *   into every call-used register, into s0, s1 and fcsr. Enable interrupts.
 *   The handler writes a value into each call-used register and into fcsr.
 *   Trap, so the frame reports every register. Run this once for each value
 *   with the pattern of that value inverted, then once unchanged.
 */
static void ScoreCpuRiscvValInterrupt_Action_0( void )
{
  InterruptPrepare();
  RegisterCheckRun( &interrupt_check );

  /*
   * Check that the interrupt processing preserved the RISC-V return address
   * register ra.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_RA );

  /*
   * Check that the interrupt processing preserved the RISC-V register t0.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_T0 );

  /*
   * Check that the interrupt processing preserved the RISC-V register t1.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_T1 );

  /*
   * Check that the interrupt processing preserved the RISC-V register t2.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_T2 );

  /*
   * Check that the interrupt processing preserved the RISC-V register s0.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_S0 );

  /*
   * Check that the interrupt processing preserved the RISC-V register s1.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_S1 );

  /*
   * Check that the interrupt processing preserved the RISC-V register a0.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_A0 );

  /*
   * Check that the interrupt processing preserved the RISC-V register a1.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_A1 );

  /*
   * Check that the interrupt processing preserved the RISC-V register a2.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_A2 );

  /*
   * Check that the interrupt processing preserved the RISC-V register a3.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_A3 );

  /*
   * Check that the interrupt processing preserved the RISC-V register a4.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_A4 );

  /*
   * Check that the interrupt processing preserved the RISC-V register a5.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_A5 );

  /*
   * Check that the interrupt processing preserved the RISC-V register a6.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_A6 );

  /*
   * Check that the interrupt processing preserved the RISC-V register a7.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_A7 );

  /*
   * Check that the interrupt processing preserved the RISC-V register t3.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_T3 );

  /*
   * Check that the interrupt processing preserved the RISC-V register t4.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_T4 );

  /*
   * Check that the interrupt processing preserved the RISC-V register t5.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_T5 );

  /*
   * Check that the interrupt processing preserved the RISC-V register t6.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_T6 );

  /*
   * Check that the interrupt processing preserved the RISC-V stack pointer
   * register sp.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_SP );

  /*
   * Check that the interrupt processing restored the interrupt enable and the
   * privilege of the status register of the interrupted context.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_STATUS );

  /*
   * Check that the interrupted context continued at the instruction after the
   * interrupt enable, so that the trap happened there.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_EPC );

  /*
   * Check that the interrupt happened once before the trap.
   */
  RegisterCheckVerify( &interrupt_check, SLOT_COUNT );

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft0.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT0 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft1.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT1 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft2.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT2 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft3.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT3 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft4.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT4 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft5.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT5 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft6.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT6 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft7.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT7 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register fa0.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FA0 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register fa1.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FA1 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register fa2.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FA2 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register fa3.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FA3 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register fa4.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FA4 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register fa5.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FA5 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register fa6.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FA6 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register fa7.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FA7 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft8.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT8 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft9.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT9 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft10.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT10 );
  #endif

  /*
   * Check that the interrupt processing preserved the RISC-V floating-point
   * register ft11.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FT11 );
  #endif

  /*
   * Check that the interrupt processing preserved the rounding mode and the
   * exception flags of the RISC-V register fcsr.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &interrupt_check, SLOT_FCSR );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  RegisterCheckReport( &interrupt_check );
}

/**
 * @fn void T_case_body_ScoreCpuRiscvValInterrupt( void )
 */
T_TEST_CASE( ScoreCpuRiscvValInterrupt )
{
  ScoreCpuRiscvValInterrupt_Action_0();
}

/** @} */
