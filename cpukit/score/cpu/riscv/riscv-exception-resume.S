/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSScoreCPURISCV
 *
 * @brief This source file contains the implementation of
 *   _CPU_Exception_resume().
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

#include <rtems/asm.h>
#include <rtems/score/percpu.h>

PUBLIC(_CPU_Exception_resume)

	.section	.text, "ax", @progbits
	.align	2
	.option	arch, +zicsr

/*
 *  void _CPU_Exception_resume( const CPU_Exception_frame *frame )
 *
 *  This routine restores the register set of the frame and continues the
 *  execution at the program counter of the frame.
 *
 *  The frame stays in a0, which is therefore the last register to load.  The
 *  restore reads through a0 after it loads the stack pointer, and the frame
 *  sits below that pointer, so those reads stay inside the stack of the
 *  trapped context.
 */

TYPE_FUNC(_CPU_Exception_resume)
SYM(_CPU_Exception_resume):

	/* Restore the registers which only the exception frame holds */
	LREG	gp, RISCV_EXCEPTION_FRAME_GP(a0)
	LREG	tp, RISCV_EXCEPTION_FRAME_TP(a0)
	LREG	s2, RISCV_EXCEPTION_FRAME_S2(a0)
	LREG	s3, RISCV_EXCEPTION_FRAME_S3(a0)
	LREG	s4, RISCV_EXCEPTION_FRAME_S4(a0)
	LREG	s5, RISCV_EXCEPTION_FRAME_S5(a0)
	LREG	s6, RISCV_EXCEPTION_FRAME_S6(a0)
	LREG	s7, RISCV_EXCEPTION_FRAME_S7(a0)
	LREG	s8, RISCV_EXCEPTION_FRAME_S8(a0)
	LREG	s9, RISCV_EXCEPTION_FRAME_S9(a0)
	LREG	s10, RISCV_EXCEPTION_FRAME_S10(a0)
	LREG	s11, RISCV_EXCEPTION_FRAME_S11(a0)
#if __riscv_flen > 0
	FLREG	fs0, RISCV_EXCEPTION_FRAME_FS0(a0)
	FLREG	fs1, RISCV_EXCEPTION_FRAME_FS1(a0)
	FLREG	fs2, RISCV_EXCEPTION_FRAME_FS2(a0)
	FLREG	fs3, RISCV_EXCEPTION_FRAME_FS3(a0)
	FLREG	fs4, RISCV_EXCEPTION_FRAME_FS4(a0)
	FLREG	fs5, RISCV_EXCEPTION_FRAME_FS5(a0)
	FLREG	fs6, RISCV_EXCEPTION_FRAME_FS6(a0)
	FLREG	fs7, RISCV_EXCEPTION_FRAME_FS7(a0)
	FLREG	fs8, RISCV_EXCEPTION_FRAME_FS8(a0)
	FLREG	fs9, RISCV_EXCEPTION_FRAME_FS9(a0)
	FLREG	fs10, RISCV_EXCEPTION_FRAME_FS10(a0)
	FLREG	fs11, RISCV_EXCEPTION_FRAME_FS11(a0)
#endif

	/* The interrupt frame is the last member of the exception frame */
	addi	a0, a0, RISCV_EXCEPTION_FRAME_INTERRUPT_FRAME

	/* Restore the registers which the interrupt frame holds */
	LREG	s0, RISCV_INTERRUPT_FRAME_S0(a0)
	LREG	s1, RISCV_INTERRUPT_FRAME_S1(a0)
	LREG	ra, RISCV_INTERRUPT_FRAME_RA(a0)
	LREG	a3, RISCV_INTERRUPT_FRAME_A3(a0)
	LREG	a4, RISCV_INTERRUPT_FRAME_A4(a0)
	LREG	a5, RISCV_INTERRUPT_FRAME_A5(a0)
	LREG	a6, RISCV_INTERRUPT_FRAME_A6(a0)
	LREG	a7, RISCV_INTERRUPT_FRAME_A7(a0)
	LREG	t2, RISCV_INTERRUPT_FRAME_T2(a0)
	LREG	t3, RISCV_INTERRUPT_FRAME_T3(a0)
	LREG	t4, RISCV_INTERRUPT_FRAME_T4(a0)
	LREG	t5, RISCV_INTERRUPT_FRAME_T5(a0)
	LREG	t6, RISCV_INTERRUPT_FRAME_T6(a0)

	/*
	 * The write to a control register needs a general register, so t0 and
	 * t1 carry the value and take their own value afterwards.
	 */
	LREG	t0, RISCV_INTERRUPT_FRAME_STATUS(a0)
	LREG	t1, RISCV_INTERRUPT_FRAME_EPC(a0)

#ifdef RISCV_USE_S_MODE
	/* Clear the SDT bit before the write to the sstatus register */
	li	a1, SSTATUS_SDT
	not	a1, a1
	and	t0, t0, a1
	csrw	sstatus, t0
	csrw	sepc, t1
#else
	/*
	 * Clear the MDT bit before the write to the mstatus register.  This is
	 * only necessary if the Smdbltrp extension is implemented.  In that
	 * case a write to mstatus with MDT set clears the MIE bit, whatever
	 * the MIE value written.  On RV32 the MDT bit sits in mstatush, which
	 * the frame does not hold.
	 */
#if __riscv_xlen == 64
	li	a1, MSTATUS_MDT
	not	a1, a1
	and	t0, t0, a1
#endif
	csrw	mstatus, t0
	csrw	mepc, t1
#endif /* RISCV_USE_S_MODE */

#if __riscv_flen > 0
	lw	t0, RISCV_INTERRUPT_FRAME_FCSR(a0)
	FLREG	ft0, RISCV_INTERRUPT_FRAME_FT0(a0)
	FLREG	ft1, RISCV_INTERRUPT_FRAME_FT1(a0)
	FLREG	ft2, RISCV_INTERRUPT_FRAME_FT2(a0)
	FLREG	ft3, RISCV_INTERRUPT_FRAME_FT3(a0)
	FLREG	ft4, RISCV_INTERRUPT_FRAME_FT4(a0)
	FLREG	ft5, RISCV_INTERRUPT_FRAME_FT5(a0)
	FLREG	ft6, RISCV_INTERRUPT_FRAME_FT6(a0)
	FLREG	ft7, RISCV_INTERRUPT_FRAME_FT7(a0)
	FLREG	ft8, RISCV_INTERRUPT_FRAME_FT8(a0)
	FLREG	ft9, RISCV_INTERRUPT_FRAME_FT9(a0)
	FLREG	ft10, RISCV_INTERRUPT_FRAME_FT10(a0)
	FLREG	ft11, RISCV_INTERRUPT_FRAME_FT11(a0)
	FLREG	fa0, RISCV_INTERRUPT_FRAME_FA0(a0)
	FLREG	fa1, RISCV_INTERRUPT_FRAME_FA1(a0)
	FLREG	fa2, RISCV_INTERRUPT_FRAME_FA2(a0)
	FLREG	fa3, RISCV_INTERRUPT_FRAME_FA3(a0)
	FLREG	fa4, RISCV_INTERRUPT_FRAME_FA4(a0)
	FLREG	fa5, RISCV_INTERRUPT_FRAME_FA5(a0)
	FLREG	fa6, RISCV_INTERRUPT_FRAME_FA6(a0)
	FLREG	fa7, RISCV_INTERRUPT_FRAME_FA7(a0)
	fscsr	t0
#endif

	LREG	t0, RISCV_INTERRUPT_FRAME_T0(a0)
	LREG	t1, RISCV_INTERRUPT_FRAME_T1(a0)
	LREG	a1, RISCV_INTERRUPT_FRAME_A1(a0)
	LREG	a2, RISCV_INTERRUPT_FRAME_A2(a0)
	LREG	sp, RISCV_EXCEPTION_FRAME_SP - RISCV_EXCEPTION_FRAME_INTERRUPT_FRAME(a0)
	LREG	a0, RISCV_INTERRUPT_FRAME_A0(a0)

#ifdef RISCV_USE_S_MODE
	sret
#else
	mret
#endif

/* end of file */
