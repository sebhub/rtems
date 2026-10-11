/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuRiscvValResume
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

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <rtems/score/cpuimpl.h>
#include <rtems/score/riscv-utility.h>

#include "tx-register-check.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuRiscvValResume spec:/score/cpu/riscv/val/resume
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @brief Checks that the exception resume restores the registers of the
 *   exception frame.
 *
 * The handler of the first trap writes a value into every register member of
 * the frame. The second trap reports what the resume restored. The fatal
 * handler of the second trap needs the true gp and tp, so the code between the
 * two traps stores the restored values and loads the true ones. A register
 * check holds at most 64 checks, so two actions share the members of the
 * frame.
 *
 * This test case performs the following actions:
 *
 * - Trap. Let the handler write a new value into every integer register of the
 *   frame. Resume, and trap again to capture what the resume restored. Run
 *   this once for each value with the value inverted, then once unchanged.
 *
 *   - Check that the resume restored the RISC-V return address register ra,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V global pointer register gp.
 *     The code before the second trap stored it.
 *
 *   - Check that the resume restored the RISC-V thread pointer register tp.
 *     The code before the second trap stored it.
 *
 *   - Check that the resume restored the RISC-V register t0, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register t1, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register t2, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s0, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s1, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register a0, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register a1, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register a2, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register a3, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register a4, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register a5, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register a6, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register a7, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s2, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s3, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s4, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s5, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s6, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s7, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s8, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s9, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s10, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register s11, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register t3, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register t4, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register t5, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V register t6, and that the
 *     handler of the second trap saved it.
 *
 *   - Check that the handler saved the RISC-V global pointer register gp which
 *     the code before the second trap restored.
 *
 *   - Check that the handler saved the RISC-V thread pointer register tp which
 *     the code before the second trap restored.
 *
 *   - Check that the resume restored the stack pointer of the frame, and that
 *     the handler saved it.
 *
 *   - Check that the resume restored the previous interrupt enable and the
 *     previous privilege of the status register of the frame.
 *
 *   - Check that the handler saved the address of the first trapping
 *     instruction.
 *
 *   - Check that the resume continued at the program counter of the frame, so
 *     that the second trap happened at the instruction after the code which
 *     follows the first trap.
 *
 *   - Check that the second frame lies where the first one lay. The resume
 *     released the first frame.
 *
 *   - Check that the fatal code of the first trap is the address of a frame
 *     directly below the stack pointer of the trapped context.
 *
 *   - Check that both traps reached the fatal handler with the fatal source
 *     RTEMS_FATAL_SOURCE_EXCEPTION.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * - Trap. Let the handler write a new value into every floating-point register
 *   and into fcsr of the frame. Resume, and trap again to capture what the
 *   resume restored. Run this once for each value with the value inverted,
 *   then once unchanged.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft0,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft1,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft2,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft3,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft4,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft5,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft6,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft7,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs0,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs1,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fa0,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fa1,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fa2,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fa3,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fa4,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fa5,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fa6,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fa7,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs2,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs3,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs4,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs5,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs6,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs7,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs8,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs9,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs10,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register fs11,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft8,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft9,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft10,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the RISC-V floating-point register ft11,
 *     and that the handler of the second trap saved it.
 *
 *   - Check that the resume restored the rounding mode and the exception flags
 *     of the RISC-V register fcsr, and that the handler of the second trap
 *     saved them.
 *
 *   - Check that both traps reached the fatal handler.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

#define WORD ( (uint64_t) UINTPTR_MAX )

#if __riscv_flen == 64
#define FWORD UINT64_MAX
#elif __riscv_flen == 32
#define FWORD UINT64_C( 0xffffffff )
#endif

/*
 * The mask covers the rounding mode and the exception flags of fcsr.  The
 * pattern sets the rounding mode three, and the inverted pattern sets the
 * rounding mode four.  Both are valid.
 */
#define FCSR_MASK UINT64_C( 0xff )

#define FCSR_PATTERN UINT64_C( 0x75 )

#if defined( RISCV_USE_S_MODE )
#define STATUS_PIE       SSTATUS_SPIE
#define STATUS_TEST_MASK ( SSTATUS_SPIE | SSTATUS_SPP )
#else
#define STATUS_PIE       MSTATUS_MPIE
#define STATUS_TEST_MASK ( MSTATUS_MPIE | MSTATUS_MPP )
#endif

#if __riscv_xlen == 64
#define LREG     "ld"
#define SREG     "sd"
#define REGBYTES 8
#else
#define LREG     "lw"
#define SREG     "sw"
#define REGBYTES 4
#endif

typedef enum { MEMBER_WORD, MEMBER_FLOAT, MEMBER_FCSR } MemberKind;

typedef struct {
  uint16_t offset;
  uint8_t  kind;
} Member;

typedef enum {
  MEMBER_INDEX_RA,
  MEMBER_INDEX_GP,
  MEMBER_INDEX_TP,
  MEMBER_INDEX_T0,
  MEMBER_INDEX_T1,
  MEMBER_INDEX_T2,
  MEMBER_INDEX_S0,
  MEMBER_INDEX_S1,
  MEMBER_INDEX_A0,
  MEMBER_INDEX_A1,
  MEMBER_INDEX_A2,
  MEMBER_INDEX_A3,
  MEMBER_INDEX_A4,
  MEMBER_INDEX_A5,
  MEMBER_INDEX_A6,
  MEMBER_INDEX_A7,
  MEMBER_INDEX_S2,
  MEMBER_INDEX_S3,
  MEMBER_INDEX_S4,
  MEMBER_INDEX_S5,
  MEMBER_INDEX_S6,
  MEMBER_INDEX_S7,
  MEMBER_INDEX_S8,
  MEMBER_INDEX_S9,
  MEMBER_INDEX_S10,
  MEMBER_INDEX_S11,
  MEMBER_INDEX_T3,
  MEMBER_INDEX_T4,
  MEMBER_INDEX_T5,
  MEMBER_INDEX_T6,
#if __riscv_flen > 0
  MEMBER_INDEX_FT0,
  MEMBER_INDEX_FT1,
  MEMBER_INDEX_FT2,
  MEMBER_INDEX_FT3,
  MEMBER_INDEX_FT4,
  MEMBER_INDEX_FT5,
  MEMBER_INDEX_FT6,
  MEMBER_INDEX_FT7,
  MEMBER_INDEX_FS0,
  MEMBER_INDEX_FS1,
  MEMBER_INDEX_FA0,
  MEMBER_INDEX_FA1,
  MEMBER_INDEX_FA2,
  MEMBER_INDEX_FA3,
  MEMBER_INDEX_FA4,
  MEMBER_INDEX_FA5,
  MEMBER_INDEX_FA6,
  MEMBER_INDEX_FA7,
  MEMBER_INDEX_FS2,
  MEMBER_INDEX_FS3,
  MEMBER_INDEX_FS4,
  MEMBER_INDEX_FS5,
  MEMBER_INDEX_FS6,
  MEMBER_INDEX_FS7,
  MEMBER_INDEX_FS8,
  MEMBER_INDEX_FS9,
  MEMBER_INDEX_FS10,
  MEMBER_INDEX_FS11,
  MEMBER_INDEX_FT8,
  MEMBER_INDEX_FT9,
  MEMBER_INDEX_FT10,
  MEMBER_INDEX_FT11,
  MEMBER_INDEX_FCSR,
#endif
  MEMBER_INDEX_MAX
} MemberIndex;

/* These are the frame members which the handler of the first trap writes */
static const Member resume_members[ MEMBER_INDEX_MAX ] = {
  { offsetof( CPU_Exception_frame, Interrupt_frame.ra ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, gp ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, tp ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.t0 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.t1 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.t2 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.s0 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.s1 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.a0 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.a1 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.a2 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.a3 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.a4 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.a5 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.a6 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.a7 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, s2 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, s3 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, s4 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, s5 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, s6 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, s7 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, s8 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, s9 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, s10 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, s11 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.t3 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.t4 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.t5 ), MEMBER_WORD },
  { offsetof( CPU_Exception_frame, Interrupt_frame.t6 ), MEMBER_WORD },
#if __riscv_flen > 0
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft0 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft1 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft2 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft3 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft4 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft5 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft6 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft7 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs0 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs1 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.fa0 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.fa1 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.fa2 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.fa3 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.fa4 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.fa5 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.fa6 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.fa7 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs2 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs3 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs4 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs5 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs6 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs7 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs8 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs9 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs10 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, fs11 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft8 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft9 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft10 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.ft11 ), MEMBER_FLOAT },
  { offsetof( CPU_Exception_frame, Interrupt_frame.fcsr ), MEMBER_FCSR },
#endif
};

static size_t GetMemberSize( const Member *m )
{
  switch ( m->kind ) {
    case MEMBER_WORD:
      return sizeof( uintptr_t );
#if __riscv_flen > 0
    case MEMBER_FLOAT:
      return sizeof( RISCV_Float );
#endif
    default:
      return sizeof( uint32_t );
  }
}

static uint64_t GetMember( const CPU_Exception_frame *frame, size_t i )
{
  const Member *m;
  uint64_t      value;
  uint32_t      value_32;

  m = &resume_members[ i ];

  if ( GetMemberSize( m ) == sizeof( value_32 ) ) {
    memcpy( &value_32, (const char *) frame + m->offset, sizeof( value_32 ) );
    return value_32;
  }

  memcpy( &value, (const char *) frame + m->offset, sizeof( value ) );
  return value;
}

static void SetMember( CPU_Exception_frame *frame, size_t i, uint64_t value )
{
  const Member *m;
  uint32_t      value_32;

  m = &resume_members[ i ];

  if ( GetMemberSize( m ) == sizeof( value_32 ) ) {
    value_32 = (uint32_t) value;
    memcpy( (char *) frame + m->offset, &value_32, sizeof( value_32 ) );
  } else {
    memcpy( (char *) frame + m->offset, &value, sizeof( value ) );
  }
}

static uint64_t GetPattern( size_t i )
{
  const Member *m;

  m = &resume_members[ i ];

  switch ( m->kind ) {
    case MEMBER_WORD:
      return ( UINT64_C( 0x1112131415161718 ) +
               UINT64_C( 0x0101010101010101 ) * i ) &
             WORD;
#if __riscv_flen > 0
    case MEMBER_FLOAT:
      return ( UINT64_C( 0x8182838485868788 ) +
               UINT64_C( 0x0101010101010101 ) * i ) &
             FWORD;
#endif
    default:
      return FCSR_PATTERN;
  }
}

/*
 * A run of a register check changes the value of one member of its group.
 * The handler of the first trap writes the value of the table of initial
 * values for each member of the current group and the pattern for each
 * other member.
 */
typedef struct {
  size_t    begin;
  size_t    end;
  uint64_t *initial;
} ResumeGroup;

static const ResumeGroup *resume_group;

static CPU_Exception_frame resume_before;

static CPU_Exception_frame resume_after;

static const CPU_Exception_frame *resume_frame[ 2 ];

static int resume_trap;

static uintptr_t resume_gp;

static uintptr_t resume_tp;

/* The gp and the tp which the resume restored */
static uintptr_t resume_seen[ 2 ];

extern const char resume_trap1_label[];

extern const char resume_trap2_label[];

extern const char resume_trap3_label[];

extern const char resume_done_label[];

void ResumeRun( void );

/*
 * The first trap captures the register set, and its handler writes a value
 * into every member of the frame.  The resume continues at the second label.
 * The fatal handler of the second trap runs C code, which needs the true gp
 * and tp.  The code at the second label therefore stores the gp and the tp
 * which the resume restored into the stack frame, and loads the true values.
 * The third label traps again to capture what the resume restored.
 */
/* clang-format off */
__asm__ (
  "  .globl ResumeRun\n"
  "ResumeRun:\n"
  "  addi sp, sp, -48\n"
  "  " SREG " ra, 4 * " RTEMS_XSTRING( REGBYTES ) "(sp)\n"
  "  " SREG " gp, 2 * " RTEMS_XSTRING( REGBYTES ) "(sp)\n"
  "  " SREG " tp, 3 * " RTEMS_XSTRING( REGBYTES ) "(sp)\n"
  "  .globl resume_trap1_label\n"
  "resume_trap1_label:\n"
  "  unimp\n"
  "  .globl resume_trap2_label\n"
  "resume_trap2_label:\n"
  "  " SREG " gp, 0(sp)\n"
  "  " SREG " tp, " RTEMS_XSTRING( REGBYTES ) "(sp)\n"
  "  " LREG " gp, 2 * " RTEMS_XSTRING( REGBYTES ) "(sp)\n"
  "  " LREG " tp, 3 * " RTEMS_XSTRING( REGBYTES ) "(sp)\n"
  "  .globl resume_trap3_label\n"
  "resume_trap3_label:\n"
  "  unimp\n"
  "  .globl resume_done_label\n"
  "resume_done_label:\n"
  "  " LREG " ra, 4 * " RTEMS_XSTRING( REGBYTES ) "(sp)\n"
  "  addi sp, sp, 48\n"
  "  ret\n"
);
/* clang-format on */

static void ResumeFatal(
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

  if ( resume_trap < 2 ) {
    resume_frame[ resume_trap ] = frame;
  }

  ++resume_trap;

  if ( resume_trap == 1 ) {
    size_t i;

    resume_before = *frame;

    for ( i = 0; i < MEMBER_INDEX_MAX; ++i ) {
      uint64_t value;

      if ( i >= resume_group->begin && i < resume_group->end ) {
        value = resume_group->initial[ i - resume_group->begin ];
      } else {
        value = GetPattern( i );
      }

      SetMember( frame, i, value );
    }

    /*
     * The gp and the tp hold values of the test until the code at the second
     * label loads the true values, so no interrupt may arrive in between.
     */
    frame->Interrupt_frame.status &= ~STATUS_PIE;
    frame->Interrupt_frame.epc = (uintptr_t) resume_trap2_label;
  } else {
    const uintptr_t *seen;

    /*
     * The code at the second label stored the gp and the tp in the stack
     * frame of ResumeRun(), which is live until the resume returns.
     */
    seen = (const uintptr_t *) resume_before.sp;
    resume_seen[ 0 ] = seen[ 0 ];
    resume_seen[ 1 ] = seen[ 1 ];
    resume_after = *frame;
    SetFatalHandler( NULL, NULL );
    *frame = resume_before;
    frame->Interrupt_frame.epc = (uintptr_t) resume_done_label;
  }

  _CPU_Exception_resume( frame );
}

static void ResumeRunTraps( const ResumeGroup *group )
{
  resume_group = group;
  resume_trap = 0;
  resume_frame[ 0 ] = NULL;
  resume_frame[ 1 ] = NULL;
  memset( &resume_before, 0, sizeof( resume_before ) );
  memset( &resume_after, 0, sizeof( resume_after ) );
  resume_seen[ 0 ] = 0;
  resume_seen[ 1 ] = 0;
  SetFatalHandler( ResumeFatal, NULL );
  ResumeRun();
  SetFatalHandler( NULL, NULL );
}

typedef enum {
  RESUME_A_RA,
  RESUME_A_GP,
  RESUME_A_TP,
  RESUME_A_T0,
  RESUME_A_T1,
  RESUME_A_T2,
  RESUME_A_S0,
  RESUME_A_S1,
  RESUME_A_A0,
  RESUME_A_A1,
  RESUME_A_A2,
  RESUME_A_A3,
  RESUME_A_A4,
  RESUME_A_A5,
  RESUME_A_A6,
  RESUME_A_A7,
  RESUME_A_S2,
  RESUME_A_S3,
  RESUME_A_S4,
  RESUME_A_S5,
  RESUME_A_S6,
  RESUME_A_S7,
  RESUME_A_S8,
  RESUME_A_S9,
  RESUME_A_S10,
  RESUME_A_S11,
  RESUME_A_T3,
  RESUME_A_T4,
  RESUME_A_T5,
  RESUME_A_T6,
  RESUME_A_FRAME_GP,
  RESUME_A_FRAME_TP,
  RESUME_A_SP,
  RESUME_A_STATUS,
  RESUME_A_TRAP1_PC,
  RESUME_A_TRAP3_PC,
  RESUME_A_FRAME,
  RESUME_A_CODE,
  RESUME_A_TRAPS,
  RESUME_A_MAX
} ResumeAIndex;

RTEMS_STATIC_ASSERT( RESUME_A_RA == 0, resume_a_first );

RTEMS_STATIC_ASSERT(
  (int) RESUME_A_T6 == (int) MEMBER_INDEX_T6,
  resume_a_members
);

static const RegisterCheckSource resume_a_sources[ RESUME_A_MAX ] = {
  { "ra", REGISTER_CHECK_INITIAL, WORD },
  { "gp", REGISTER_CHECK_INITIAL, WORD },
  { "tp", REGISTER_CHECK_INITIAL, WORD },
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
  { "s2", REGISTER_CHECK_INITIAL, WORD },
  { "s3", REGISTER_CHECK_INITIAL, WORD },
  { "s4", REGISTER_CHECK_INITIAL, WORD },
  { "s5", REGISTER_CHECK_INITIAL, WORD },
  { "s6", REGISTER_CHECK_INITIAL, WORD },
  { "s7", REGISTER_CHECK_INITIAL, WORD },
  { "s8", REGISTER_CHECK_INITIAL, WORD },
  { "s9", REGISTER_CHECK_INITIAL, WORD },
  { "s10", REGISTER_CHECK_INITIAL, WORD },
  { "s11", REGISTER_CHECK_INITIAL, WORD },
  { "t3", REGISTER_CHECK_INITIAL, WORD },
  { "t4", REGISTER_CHECK_INITIAL, WORD },
  { "t5", REGISTER_CHECK_INITIAL, WORD },
  { "t6", REGISTER_CHECK_INITIAL, WORD },
  { "frame gp", REGISTER_CHECK_EXPECTED, WORD },
  { "frame tp", REGISTER_CHECK_EXPECTED, WORD },
  { "sp", REGISTER_CHECK_EXPECTED, WORD },
  { "status", REGISTER_CHECK_EXPECTED, STATUS_TEST_MASK },
  { "trap 1 pc", REGISTER_CHECK_EXPECTED, WORD },
  { "trap 3 pc", REGISTER_CHECK_EXPECTED, WORD },
  { "frame", REGISTER_CHECK_EXPECTED, WORD },
  { "code", REGISTER_CHECK_EXPECTED, WORD },
  { "traps", REGISTER_CHECK_EXPECTED, WORD },
};

static const RegisterCheckSlot resume_a_slots[ RESUME_A_MAX ] = {
  { "ra", RESUME_A_RA, WORD },
  { "gp", RESUME_A_GP, WORD },
  { "tp", RESUME_A_TP, WORD },
  { "t0", RESUME_A_T0, WORD },
  { "t1", RESUME_A_T1, WORD },
  { "t2", RESUME_A_T2, WORD },
  { "s0", RESUME_A_S0, WORD },
  { "s1", RESUME_A_S1, WORD },
  { "a0", RESUME_A_A0, WORD },
  { "a1", RESUME_A_A1, WORD },
  { "a2", RESUME_A_A2, WORD },
  { "a3", RESUME_A_A3, WORD },
  { "a4", RESUME_A_A4, WORD },
  { "a5", RESUME_A_A5, WORD },
  { "a6", RESUME_A_A6, WORD },
  { "a7", RESUME_A_A7, WORD },
  { "s2", RESUME_A_S2, WORD },
  { "s3", RESUME_A_S3, WORD },
  { "s4", RESUME_A_S4, WORD },
  { "s5", RESUME_A_S5, WORD },
  { "s6", RESUME_A_S6, WORD },
  { "s7", RESUME_A_S7, WORD },
  { "s8", RESUME_A_S8, WORD },
  { "s9", RESUME_A_S9, WORD },
  { "s10", RESUME_A_S10, WORD },
  { "s11", RESUME_A_S11, WORD },
  { "t3", RESUME_A_T3, WORD },
  { "t4", RESUME_A_T4, WORD },
  { "t5", RESUME_A_T5, WORD },
  { "t6", RESUME_A_T6, WORD },
  { "frame gp", RESUME_A_FRAME_GP, WORD },
  { "frame tp", RESUME_A_FRAME_TP, WORD },
  { "sp", RESUME_A_SP, WORD },
  { "status", RESUME_A_STATUS, STATUS_TEST_MASK },
  { "trap 1 pc", RESUME_A_TRAP1_PC, WORD },
  { "trap 3 pc", RESUME_A_TRAP3_PC, WORD },
  { "frame", RESUME_A_FRAME, WORD },
  { "code", RESUME_A_CODE, WORD },
  { "traps", RESUME_A_TRAPS, WORD },
};

static uint64_t resume_a_initial[ RESUME_A_MAX ];

static const ResumeGroup resume_a_group = {
  .begin = MEMBER_INDEX_RA,
  .end = MEMBER_INDEX_T6 + 1,
  .initial = resume_a_initial
};

static void ResumeARun( RegisterCheck *self, void *arg )
{
  size_t i;

  (void) arg;
  ResumeRunTraps( &resume_a_group );

  for ( i = MEMBER_INDEX_RA; i <= MEMBER_INDEX_T6; ++i ) {
    uint64_t actual;

    if ( i == MEMBER_INDEX_GP ) {
      actual = resume_seen[ 0 ];
    } else if ( i == MEMBER_INDEX_TP ) {
      actual = resume_seen[ 1 ];
    } else {
      actual = GetMember( &resume_after, i );
    }

    RegisterCheckRecord( self, i, actual, GetPattern( i ) );
  }

  RegisterCheckRecord( self, RESUME_A_FRAME_GP, resume_after.gp, resume_gp );
  RegisterCheckRecord( self, RESUME_A_FRAME_TP, resume_after.tp, resume_tp );
  RegisterCheckRecord( self, RESUME_A_SP, resume_after.sp, resume_before.sp );
  RegisterCheckRecord(
    self,
    RESUME_A_STATUS,
    resume_after.Interrupt_frame.status,
    resume_before.Interrupt_frame.status & ~STATUS_PIE
  );
  RegisterCheckRecord(
    self,
    RESUME_A_TRAP1_PC,
    resume_before.Interrupt_frame.epc,
    (uintptr_t) resume_trap1_label
  );
  RegisterCheckRecord(
    self,
    RESUME_A_TRAP3_PC,
    resume_after.Interrupt_frame.epc,
    (uintptr_t) resume_trap3_label
  );
  RegisterCheckRecord(
    self,
    RESUME_A_FRAME,
    (uintptr_t) resume_frame[ 1 ],
    (uintptr_t) resume_frame[ 0 ]
  );
  RegisterCheckRecord(
    self,
    RESUME_A_CODE,
    (uintptr_t) resume_frame[ 0 ],
    resume_before.sp - sizeof( CPU_Exception_frame )
  );
  RegisterCheckRecord( self, RESUME_A_TRAPS, (uint64_t) resume_trap, 2 );
}

static RegisterCheck resume_a_check = {
  .sources = resume_a_sources,
  .source_count = RESUME_A_MAX,
  .slots = resume_a_slots,
  .slot_count = RESUME_A_MAX,
  .initial = resume_a_initial,
  .run = ResumeARun
};

static void ResumeAPrepare( void )
{
  size_t i;

  /* clang-format off */
  __asm__ volatile ( "mv %0, gp" : "=r" ( resume_gp ) );
  __asm__ volatile ( "mv %0, tp" : "=r" ( resume_tp ) );
  /* clang-format on */

  for ( i = MEMBER_INDEX_RA; i <= MEMBER_INDEX_T6; ++i ) {
    resume_a_initial[ i ] = GetPattern( i );
  }
}

#if __riscv_flen > 0
typedef enum {
  RESUME_B_FT0,
  RESUME_B_FT1,
  RESUME_B_FT2,
  RESUME_B_FT3,
  RESUME_B_FT4,
  RESUME_B_FT5,
  RESUME_B_FT6,
  RESUME_B_FT7,
  RESUME_B_FS0,
  RESUME_B_FS1,
  RESUME_B_FA0,
  RESUME_B_FA1,
  RESUME_B_FA2,
  RESUME_B_FA3,
  RESUME_B_FA4,
  RESUME_B_FA5,
  RESUME_B_FA6,
  RESUME_B_FA7,
  RESUME_B_FS2,
  RESUME_B_FS3,
  RESUME_B_FS4,
  RESUME_B_FS5,
  RESUME_B_FS6,
  RESUME_B_FS7,
  RESUME_B_FS8,
  RESUME_B_FS9,
  RESUME_B_FS10,
  RESUME_B_FS11,
  RESUME_B_FT8,
  RESUME_B_FT9,
  RESUME_B_FT10,
  RESUME_B_FT11,
  RESUME_B_FCSR,
  RESUME_B_TRAPS,
  RESUME_B_MAX
} ResumeBIndex;

RTEMS_STATIC_ASSERT( RESUME_B_FT0 == 0, resume_b_first );

static const RegisterCheckSource resume_b_sources[ RESUME_B_MAX ] = {
  { "ft0", REGISTER_CHECK_INITIAL, FWORD },
  { "ft1", REGISTER_CHECK_INITIAL, FWORD },
  { "ft2", REGISTER_CHECK_INITIAL, FWORD },
  { "ft3", REGISTER_CHECK_INITIAL, FWORD },
  { "ft4", REGISTER_CHECK_INITIAL, FWORD },
  { "ft5", REGISTER_CHECK_INITIAL, FWORD },
  { "ft6", REGISTER_CHECK_INITIAL, FWORD },
  { "ft7", REGISTER_CHECK_INITIAL, FWORD },
  { "fs0", REGISTER_CHECK_INITIAL, FWORD },
  { "fs1", REGISTER_CHECK_INITIAL, FWORD },
  { "fa0", REGISTER_CHECK_INITIAL, FWORD },
  { "fa1", REGISTER_CHECK_INITIAL, FWORD },
  { "fa2", REGISTER_CHECK_INITIAL, FWORD },
  { "fa3", REGISTER_CHECK_INITIAL, FWORD },
  { "fa4", REGISTER_CHECK_INITIAL, FWORD },
  { "fa5", REGISTER_CHECK_INITIAL, FWORD },
  { "fa6", REGISTER_CHECK_INITIAL, FWORD },
  { "fa7", REGISTER_CHECK_INITIAL, FWORD },
  { "fs2", REGISTER_CHECK_INITIAL, FWORD },
  { "fs3", REGISTER_CHECK_INITIAL, FWORD },
  { "fs4", REGISTER_CHECK_INITIAL, FWORD },
  { "fs5", REGISTER_CHECK_INITIAL, FWORD },
  { "fs6", REGISTER_CHECK_INITIAL, FWORD },
  { "fs7", REGISTER_CHECK_INITIAL, FWORD },
  { "fs8", REGISTER_CHECK_INITIAL, FWORD },
  { "fs9", REGISTER_CHECK_INITIAL, FWORD },
  { "fs10", REGISTER_CHECK_INITIAL, FWORD },
  { "fs11", REGISTER_CHECK_INITIAL, FWORD },
  { "ft8", REGISTER_CHECK_INITIAL, FWORD },
  { "ft9", REGISTER_CHECK_INITIAL, FWORD },
  { "ft10", REGISTER_CHECK_INITIAL, FWORD },
  { "ft11", REGISTER_CHECK_INITIAL, FWORD },
  { "fcsr", REGISTER_CHECK_INITIAL, FCSR_MASK },
  { "traps", REGISTER_CHECK_EXPECTED, WORD },
};

static const RegisterCheckSlot resume_b_slots[ RESUME_B_MAX ] = {
  { "ft0", RESUME_B_FT0, FWORD },
  { "ft1", RESUME_B_FT1, FWORD },
  { "ft2", RESUME_B_FT2, FWORD },
  { "ft3", RESUME_B_FT3, FWORD },
  { "ft4", RESUME_B_FT4, FWORD },
  { "ft5", RESUME_B_FT5, FWORD },
  { "ft6", RESUME_B_FT6, FWORD },
  { "ft7", RESUME_B_FT7, FWORD },
  { "fs0", RESUME_B_FS0, FWORD },
  { "fs1", RESUME_B_FS1, FWORD },
  { "fa0", RESUME_B_FA0, FWORD },
  { "fa1", RESUME_B_FA1, FWORD },
  { "fa2", RESUME_B_FA2, FWORD },
  { "fa3", RESUME_B_FA3, FWORD },
  { "fa4", RESUME_B_FA4, FWORD },
  { "fa5", RESUME_B_FA5, FWORD },
  { "fa6", RESUME_B_FA6, FWORD },
  { "fa7", RESUME_B_FA7, FWORD },
  { "fs2", RESUME_B_FS2, FWORD },
  { "fs3", RESUME_B_FS3, FWORD },
  { "fs4", RESUME_B_FS4, FWORD },
  { "fs5", RESUME_B_FS5, FWORD },
  { "fs6", RESUME_B_FS6, FWORD },
  { "fs7", RESUME_B_FS7, FWORD },
  { "fs8", RESUME_B_FS8, FWORD },
  { "fs9", RESUME_B_FS9, FWORD },
  { "fs10", RESUME_B_FS10, FWORD },
  { "fs11", RESUME_B_FS11, FWORD },
  { "ft8", RESUME_B_FT8, FWORD },
  { "ft9", RESUME_B_FT9, FWORD },
  { "ft10", RESUME_B_FT10, FWORD },
  { "ft11", RESUME_B_FT11, FWORD },
  { "fcsr", RESUME_B_FCSR, FCSR_MASK },
  { "traps", RESUME_B_TRAPS, WORD },
};

static uint64_t resume_b_initial[ RESUME_B_MAX ];

static const ResumeGroup resume_b_group = {
  .begin = MEMBER_INDEX_FT0,
  .end = MEMBER_INDEX_FCSR + 1,
  .initial = resume_b_initial
};

static void ResumeBRun( RegisterCheck *self, void *arg )
{
  size_t i;

  (void) arg;
  ResumeRunTraps( &resume_b_group );

  for ( i = MEMBER_INDEX_FT0; i <= MEMBER_INDEX_FCSR; ++i ) {
    RegisterCheckRecord(
      self,
      i - MEMBER_INDEX_FT0,
      GetMember( &resume_after, i ),
      GetPattern( i )
    );
  }

  RegisterCheckRecord( self, RESUME_B_TRAPS, (uint64_t) resume_trap, 2 );
}

static RegisterCheck resume_b_check = {
  .sources = resume_b_sources,
  .source_count = RESUME_B_MAX,
  .slots = resume_b_slots,
  .slot_count = RESUME_B_MAX,
  .initial = resume_b_initial,
  .run = ResumeBRun
};

static void ResumeBPrepare( void )
{
  size_t i;

  for ( i = MEMBER_INDEX_FT0; i <= MEMBER_INDEX_FCSR; ++i ) {
    resume_b_initial[ i - MEMBER_INDEX_FT0 ] = GetPattern( i );
  }
}
#endif

/**
 * @brief Trap. Let the handler write a new value into every integer register
 *   of the frame. Resume, and trap again to capture what the resume restored.
 *   Run this once for each value with the value inverted, then once unchanged.
 */
static void ScoreCpuRiscvValResume_Action_0( void )
{
  ResumeAPrepare();
  RegisterCheckRun( &resume_a_check );

  /*
   * Check that the resume restored the RISC-V return address register ra, and
   * that the handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_RA );

  /*
   * Check that the resume restored the RISC-V global pointer register gp. The
   * code before the second trap stored it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_GP );

  /*
   * Check that the resume restored the RISC-V thread pointer register tp. The
   * code before the second trap stored it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_TP );

  /*
   * Check that the resume restored the RISC-V register t0, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_T0 );

  /*
   * Check that the resume restored the RISC-V register t1, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_T1 );

  /*
   * Check that the resume restored the RISC-V register t2, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_T2 );

  /*
   * Check that the resume restored the RISC-V register s0, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S0 );

  /*
   * Check that the resume restored the RISC-V register s1, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S1 );

  /*
   * Check that the resume restored the RISC-V register a0, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_A0 );

  /*
   * Check that the resume restored the RISC-V register a1, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_A1 );

  /*
   * Check that the resume restored the RISC-V register a2, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_A2 );

  /*
   * Check that the resume restored the RISC-V register a3, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_A3 );

  /*
   * Check that the resume restored the RISC-V register a4, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_A4 );

  /*
   * Check that the resume restored the RISC-V register a5, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_A5 );

  /*
   * Check that the resume restored the RISC-V register a6, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_A6 );

  /*
   * Check that the resume restored the RISC-V register a7, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_A7 );

  /*
   * Check that the resume restored the RISC-V register s2, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S2 );

  /*
   * Check that the resume restored the RISC-V register s3, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S3 );

  /*
   * Check that the resume restored the RISC-V register s4, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S4 );

  /*
   * Check that the resume restored the RISC-V register s5, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S5 );

  /*
   * Check that the resume restored the RISC-V register s6, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S6 );

  /*
   * Check that the resume restored the RISC-V register s7, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S7 );

  /*
   * Check that the resume restored the RISC-V register s8, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S8 );

  /*
   * Check that the resume restored the RISC-V register s9, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S9 );

  /*
   * Check that the resume restored the RISC-V register s10, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S10 );

  /*
   * Check that the resume restored the RISC-V register s11, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_S11 );

  /*
   * Check that the resume restored the RISC-V register t3, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_T3 );

  /*
   * Check that the resume restored the RISC-V register t4, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_T4 );

  /*
   * Check that the resume restored the RISC-V register t5, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_T5 );

  /*
   * Check that the resume restored the RISC-V register t6, and that the
   * handler of the second trap saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_T6 );

  /*
   * Check that the handler saved the RISC-V global pointer register gp which
   * the code before the second trap restored.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_FRAME_GP );

  /*
   * Check that the handler saved the RISC-V thread pointer register tp which
   * the code before the second trap restored.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_FRAME_TP );

  /*
   * Check that the resume restored the stack pointer of the frame, and that
   * the handler saved it.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_SP );

  /*
   * Check that the resume restored the previous interrupt enable and the
   * previous privilege of the status register of the frame.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_STATUS );

  /*
   * Check that the handler saved the address of the first trapping
   * instruction.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_TRAP1_PC );

  /*
   * Check that the resume continued at the program counter of the frame, so
   * that the second trap happened at the instruction after the code which
   * follows the first trap.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_TRAP3_PC );

  /*
   * Check that the second frame lies where the first one lay. The resume
   * released the first frame.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_FRAME );

  /*
   * Check that the fatal code of the first trap is the address of a frame
   * directly below the stack pointer of the trapped context.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_CODE );

  /*
   * Check that both traps reached the fatal handler with the fatal source
   * RTEMS_FATAL_SOURCE_EXCEPTION.
   */
  RegisterCheckVerify( &resume_a_check, RESUME_A_TRAPS );

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  RegisterCheckReport( &resume_a_check );
}

/**
 * @brief Trap. Let the handler write a new value into every floating-point
 *   register and into fcsr of the frame. Resume, and trap again to capture
 *   what the resume restored. Run this once for each value with the value
 *   inverted, then once unchanged.
 */
static void ScoreCpuRiscvValResume_Action_1( void )
{
  #if __riscv_flen > 0
  ResumeBPrepare();
  RegisterCheckRun( &resume_b_check );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft0, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT0 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft1, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT1 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft2, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT2 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft3, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT3 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft4, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT4 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft5, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT5 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft6, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT6 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft7, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT7 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs0, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS0 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs1, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS1 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fa0, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FA0 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fa1, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FA1 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fa2, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FA2 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fa3, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FA3 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fa4, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FA4 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fa5, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FA5 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fa6, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FA6 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fa7, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FA7 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs2, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS2 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs3, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS3 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs4, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS4 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs5, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS5 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs6, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS6 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs7, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS7 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs8, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS8 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs9, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS9 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs10,
   * and that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS10 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register fs11,
   * and that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FS11 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft8, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT8 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft9, and
   * that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT9 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft10,
   * and that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT10 );
  #endif

  /*
   * Check that the resume restored the RISC-V floating-point register ft11,
   * and that the handler of the second trap saved it.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FT11 );
  #endif

  /*
   * Check that the resume restored the rounding mode and the exception flags
   * of the RISC-V register fcsr, and that the handler of the second trap saved
   * them.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_FCSR );
  #endif

  /*
   * Check that both traps reached the fatal handler.
   */
  #if __riscv_flen > 0
  RegisterCheckVerify( &resume_b_check, RESUME_B_TRAPS );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  #if __riscv_flen > 0
  RegisterCheckReport( &resume_b_check );
  #endif
}

/**
 * @fn void T_case_body_ScoreCpuRiscvValResume( void )
 */
T_TEST_CASE( ScoreCpuRiscvValResume )
{
  ScoreCpuRiscvValResume_Action_0();
  ScoreCpuRiscvValResume_Action_1();
}

/** @} */
