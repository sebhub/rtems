/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup qoriq
 *
 * @brief BSP start.
 */

/*
 * Copyright (C) 2010, 2025 embedded brains GmbH & Co. KG
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

#include <rtems/score/percpu.h>

#include <bsp.h>

#include <libcpu/powerpc-utility.h>

#include <bsp/vectors.h>

#if (QORIQ_INITIAL_MSR & MSR_FP) != 0
#define INITIALIZE_FPU
#endif

#define FIRST_TLB 0
#define SCRATCH_TLB QORIQ_TLB1_ENTRY_COUNT - 1
#define INITIAL_MSR r14
#define START_STACK r15
#define SAVED_LINK_REGISTER r16
#define FDT_REGISTER r17
#define CPU_SELF r18

#ifdef __powerpc64__
#define VECTOR_TABLE_ENTRY_SIZE 32
#else
#define VECTOR_TABLE_ENTRY_SIZE 16
#endif

/*
 * Each core/thread has its own 4KiB machine check area.  The usage is as
 * follows:
 *
 * 000..003: machine check stack pointer
 * 004..007: L1 instruction recoveries
 * 008..00b: L1 data recoveries
 * 00c..fff: machine check stack area
 */
.macro MACHINE_CHECK_DATA
	.long	. + 0x1000 - EXC_GENERIC_SIZE - PPC_DEFAULT_CACHE_LINE_SIZE
	.zero 0x0ffc
.endm

	.globl _start
#ifdef RTEMS_SMP
#if QORIQ_THREAD_COUNT > 1
	.globl _start_thread
#endif
	.globl _start_secondary_processor
#endif
	.globl qoriq_machine_check_data
	.globl bsp_exc_vector_base
	.globl bsp_exc_vector_cacheable_base

	.section ".bsp_start_text", "ax"

_start:
	mr	FDT_REGISTER, r3
	bl	.Linitearly

	/* Get start stack */
	LA	START_STACK, _ISR_Stack_area_begin
	LA	r3, _ISR_Stack_size
	add	START_STACK, START_STACK, r3

	bl	.Linitmore

	/* Copy fast text */
	LA	r3, bsp_section_fast_text_begin
	LA	r4, bsp_section_fast_text_load_begin
	LA	r5, bsp_section_fast_text_size
	bl	.Lcopy
	LA	r3, bsp_section_fast_text_begin
	LA	r4, bsp_section_fast_text_size
	bl	rtems_cache_flush_multiple_data_lines
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE

	/* Copy read-only data */
	LA	r3, bsp_section_rodata_begin
	LA	r4, bsp_section_rodata_load_begin
	LA	r5, bsp_section_rodata_size
	bl	.Lcopy

	/* Copy FDT into read-only data */
	mr	r3, FDT_REGISTER
	bl	bsp_fdt_copy
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE

	/* Flush read-only data */
	LA	r3, bsp_section_rodata_begin
	LA	r4, bsp_section_rodata_size
	bl	rtems_cache_flush_multiple_data_lines
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE

	/* Copy fast data */
	LA	r3, bsp_section_fast_data_begin
	LA	r4, bsp_section_fast_data_load_begin
	LA	r5, bsp_section_fast_data_size
	bl	.Lcopy

	/* Copy data */
	LA	r3, bsp_section_data_begin
	LA	r4, bsp_section_data_load_begin
	LA	r5, bsp_section_data_size
	bl	.Lcopy

	/* Initialize NULL pointer access protection area with "sc" instructions */
	LA32	r3, bsp_section_start_begin
	SHIFT_RIGHT_IMMEDIATE	r3, r3, 2
	mtctr	r3
	li	r3, -4
	LWI	r4, 0x44000002
.Lnull_area_setup_loop:
	stwu	r4, 4(r3)
	bdnz	.Lnull_area_setup_loop

	bl	.Linitmmu

	/* Clear SBSS */
	LA	r3, bsp_section_sbss_begin
	LA	r4, bsp_section_sbss_size
	bl	bsp_start_zero
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE

	/* Clear BSS */
	LA	r3, bsp_section_bss_begin
	LA	r4, bsp_section_bss_size
	bl	bsp_start_zero
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE

#ifndef __powerpc64__
	/* Set up EABI and SYSV environment */
	bl	__eabi
#endif

	/* Clear command line */
	li	r3, 0

	bl	boot_card
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE

.Lcopy:
	PPC_REG_CMP	r3, r4
	beqlr
	b	memcpy
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE

.Linitearly:
#ifdef __powerpc64__
	/* Enable 64-bit computation mode for exceptions */
	mfspr	r0, BOOKE_EPCR
	oris	r0, r0, BOOKE_EPCR_ICM >> 16
	mtspr	BOOKE_EPCR, r0

	/* Enable 64-bit computation mode */
	mfmsr	r0
	oris	r0, r0, MSR_CM >> 16
	mtmsr	r0
	isync
#endif

	/* Disable decrementer */
	mfspr	r0, BOOKE_TCR
	LWI	r4, BOOKE_TCR_DIE
	andc	r0, r0, r4
	mtspr	BOOKE_TCR, r0

#ifdef QORIQ_INITIAL_SPEFSCR
	/* SPEFSCR initialization */
	LWI	r0, QORIQ_INITIAL_SPEFSCR
	mtspr	FSL_EIS_SPEFSCR, r0
#endif

#ifdef QORIQ_INITIAL_BUCSR
	/* BUCSR initialization */
	LWI	r0, QORIQ_INITIAL_BUCSR
	mtspr	FSL_EIS_BUCSR, r0
	isync
#endif

#if defined(QORIQ_INITIAL_HID0) && !defined(QORIQ_IS_HYPERVISOR_GUEST)
	/* HID0 initialization */
	LWI	r0, QORIQ_INITIAL_HID0
	mtspr	HID0, r0
#endif

#ifdef __powerpc64__
	LA32	r2, .TOC.
#else
	/* Invalidate TLS anchor */
	li	r2, 0

	/* Set small-data anchor */
	LA	r13, _SDA_BASE_
#endif

	SET_SELF_CPU_CONTROL	CPU_SELF, r5

	blr

.Linitmore:
	mflr	SAVED_LINK_REGISTER

	/* Invalidate all TS1 MMU entries */
	li	r3, 1
	bl	qoriq_tlb1_invalidate_all_by_ts
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE

	/* Add TS1 entry for the first 4GiB of RAM */
	li	r3, SCRATCH_TLB
	li	r4, FSL_EIS_MAS1_TS
	li	r5, FSL_EIS_MAS2_M
	li	r6, FSL_EIS_MAS3_SR | FSL_EIS_MAS3_SW | FSL_EIS_MAS3_SX
	li	r7, 0
	li	r8, 0
	li	r9, FSL_EIS_MAS1_SIZE_4G
	bl	qoriq_tlb1_write
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE

	/* MSR initialization and use TS1 for address translation */
	LWI	INITIAL_MSR, QORIQ_INITIAL_MSR
	ori	r0, INITIAL_MSR, MSR_IS | MSR_DS
#ifdef QORIQ_IS_HYPERVISOR_GUEST
	oris	r0, r0, MSR_GS >> 16
#endif
	mtmsr	r0
	isync

	/*
	 * Initialize start and interrupt stack.  The stacks are statically
	 * allocated and properly aligned.
	 */
	subi	r1, START_STACK, PPC_DEFAULT_CACHE_LINE_SIZE
	li	r0, 0
	PPC_REG_STORE	r0, 0(r1)
	mtsprg	1, r1

	/*
	 * Initialize the exception vector prefix and the cache inhibited
	 * machine check vector offset.
	 */
	LA32	r3, bsp_exc_vector_base
	mtivpr	r3
	mtspr	BOOKE_IVOR1, r3

	/* Initialize the cacheable exception vector offsets */
	LA32	r3, bsp_exc_vector_cacheable_base
	mtspr	BOOKE_IVOR0, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR2, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR3, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR4, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR5, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR6, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
#ifdef __PPC_CPU_E6500__
	mtspr	BOOKE_IVOR7, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
#endif
	mtspr	BOOKE_IVOR8, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
#ifdef __PPC_CPU_E6500__
	mtspr	BOOKE_IVOR9, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
#endif
	mtspr	BOOKE_IVOR10, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR11, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR12, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR13, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR14, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR15, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR32, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR33, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
#ifndef __PPC_CPU_E6500__
	mtspr	BOOKE_IVOR34, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
#endif
	mtspr	BOOKE_IVOR35, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
#ifdef __PPC_CPU_E6500__
	mtspr	BOOKE_IVOR36, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR37, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
#ifndef QORIQ_IS_HYPERVISOR_GUEST
	mtspr	BOOKE_IVOR38, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR39, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR40, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR41, r3
	addi	r3, r3, VECTOR_TABLE_ENTRY_SIZE
	mtspr	BOOKE_IVOR42, r3
#endif
#endif

#ifdef INITIALIZE_FPU
	bl	.Linitfpu
#endif

	mtlr	SAVED_LINK_REGISTER
	blr

.Linitmmu:
	mflr	SAVED_LINK_REGISTER

	/* Configure MMU */
#ifdef QORIQ_USE_FIXED_MMU_CONFIG
	bl	qoriq_mmu_fixed_config
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#else
	li	r4, FIRST_TLB
	li	r5, SCRATCH_TLB
	bl	qoriq_mmu_config
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	mtmsr	INITIAL_MSR
	isync
	li	r3, SCRATCH_TLB
	bl	qoriq_tlb1_invalidate
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#endif

	mtlr	SAVED_LINK_REGISTER
	blr

#ifdef INITIALIZE_FPU
	/*
	 * Write a value to the FPRs to initialize the hidden tag bits.  See
	 * also "Core Software Initialization Requirements" of the e500mc
	 * reference manual for example.
	 */
.Linitfpu:
	li	r0, 0
	stw	r0, 0(r1)
	stw	r0, 4(r1)
	lfd	f0, 0(r1)
	fmr	f1, f0
	fmr	f2, f0
	fmr	f3, f0
	fmr	f4, f0
	fmr	f5, f0
	fmr	f6, f0
	fmr	f7, f0
	fmr	f8, f0
	fmr	f9, f0
	fmr	f10, f0
	fmr	f11, f0
	fmr	f12, f0
	fmr	f13, f0
	fmr	f14, f0
	fmr	f15, f0
	fmr	f16, f0
	fmr	f17, f0
	fmr	f18, f0
	fmr	f19, f0
	fmr	f20, f0
	fmr	f21, f0
	fmr	f22, f0
	fmr	f23, f0
	fmr	f24, f0
	fmr	f25, f0
	fmr	f26, f0
	fmr	f27, f0
	fmr	f28, f0
	fmr	f29, f0
	fmr	f30, f0
	fmr	f31, f0
	blr
#endif

#ifdef RTEMS_SMP
#if QORIQ_THREAD_COUNT > 1
_start_thread:
	/* Adjust PIR */
	mfspr	r0, BOOKE_PIR
	srawi	r0, r0, 2
	ori	r0, r0, 1
	mtspr	BOOKE_PIR, r0

	bl	.Linitearly

	/* Initialize start stack */
	PPC_REG_LOAD	r3, PER_CPU_INTERRUPT_STACK_HIGH(CPU_SELF)
	subi	r1, r3, PPC_MINIMUM_STACK_FRAME_SIZE
	CLEAR_RIGHT_IMMEDIATE	r1, r1, PPC_STACK_ALIGN_POWER
	li	r0, 0
	PPC_REG_STORE	r0, 0(r1)

#ifdef INITIALIZE_FPU
	bl	.Linitfpu
#endif

	mr	r3, CPU_SELF
	b	qoriq_start_thread
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#endif
_start_secondary_processor:
	bl	.Linitearly

	/* Get start stack */
	mr	START_STACK, r3

	bl	.Linitmore
	bl	.Linitmmu
	mr	r3, CPU_SELF
	b	bsp_start_on_secondary_processor
	PPC64_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#endif /* RTEMS_SMP */

#ifdef __powerpc64__
#define START_NOP_FOR_LINKER_TOC_POINTER_RESTORE nop; nop; nop; nop
#else
#define START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#endif

	/* Exception handling and boot area */

	.section ".bsp_exceptions.vector", "ax"

	.balign 0x10000

bsp_exc_vector_base:

	/* Machine check (IVOR1) */

	/* Use SPRG2 to save the stack pointer */
	mtsprg	2, r1

	/*
	 * Get the machine check stack pointer for this processor.  Each
	 * processor has a 4KiB read-write, cache inhibited data area for the
	 * machine check handling.
	 */
	mfspr	r1, BOOKE_PIR
	slwi	r1, r1, 12
	addis	r1, r1, qoriq_machine_check_data@h
	PPC_REG_LOAD	r1, 0(r1)
	PPC_REG_STORE	r3, GPR3_OFFSET(r1)
	mfspr	r3, BOOKE_PIR
	PPC_REG_STORE	r4, GPR4_OFFSET(r1)
	mfspr	r4, BOOKE_MCSR
	PPC_REG_STORE	r5, GPR5_OFFSET(r1)
	mfcr	r5
	PPC_REG_STORE	r6, GPR6_OFFSET(r1)
	PPC_REG_STORE	r5, EXC_CR_OFFSET(r1)
	slwi	r3, r3, 12
	addis	r3, r3, qoriq_machine_check_data@h

	/* Is ICPERR? */
	andis.	r5, r4, 0x4000
	beq	.Lmachine_check_no_icperr

	/*
	 * Perform an L1 instruction cache parity error recovery.  Get the
	 * machine check data area of this processor.  Invalidate the cache
	 * line indicated by MCSRR0.  Increment the L1 instruction recovery
	 * counter.  Return from the machine check interrupt.
	 */
	mfmcsrr0	r4
	lwz	r5, 4(r3)
	icbi	r0, r4
	msync
	addi	r5, r5, 1
	stw	r5, 4(r3)
	b	.Lmachine_check_return

.Lmachine_check_no_icperr:

	/* Is DCP_PERR? */
	andis.	r5, r4, 0x2000
	beq	.Lmachine_check_no_dcp_perr

	/*
	 * A Data Cache Push Parity Error (DCP_PERR) is not recoverable.  If it
	 * happens, proceed to the fatal machine check handling.
	 */
	b	.Lmachine_check_fatal_error

.Lmachine_check_no_dcp_perr:

	/* Is DCPERR? */
	andis.	r5, r4, 0x1000
	beq	.Lmachine_check_fatal_error

	/* This is a nested DCPERR, just return */
	addi	r4, r3, 0x1000 - EXC_GENERIC_SIZE - PPC_DEFAULT_CACHE_LINE_SIZE
	cmpw	r1, r4
	bne	.Lmachine_check_return

	/*
	 * A Data Cache Parity Error (DCPERR) is recoverable if the error is on
	 * a non-modified line.  To check this, the machine check handling has
	 * to be re-entrant to a 2-deep level.
	 */

	/* Save machine check context */
	mfsprg	r4, 2
	mfmcsrr0	r5
	mfmcsrr1	r6
	PPC_REG_STORE	r4, GPR1_OFFSET(r1)
	mfctr	r4
	PPC_REG_STORE	r5, SRR0_FRAME_OFFSET(r1)
	PPC_REG_STORE	r6, SRR1_FRAME_OFFSET(r1)
	PPC_REG_STORE	r4, EXC_CTR_OFFSET(r1)
	subi	r1, r1, EXC_GENERIC_SIZE
	PPC_REG_STORE	r1, 0(r3)

	/* Enable data cache flush assist HID0[DCFA] */
	mfspr	r4, HID0
	ori	r4, r4, 0x40
	mtspr	HID0, r4

	/* Enable MSR[ME] */
	mfmsr	r4
	ori	r5, r4, MSR_ME
	mtmsr	r5
	isync

	/* Load the L1 data cache */
	li	r5, 8 * 128
	mtctr	r5
	LA	r5, bsp_exc_vector_cacheable_base

.Lmachine_check_l1_load:

	lwz	r6, 0(r5)
	addi	r5, r5, 32

	bdnz	.Lmachine_check_l1_load

	/* Flash invalidate the L1 data cache */

	mfspr	r5, FSL_EIS_L1CSR0
	ori	r5, r5, FSL_EIS_L1CSR0_CFI
	msync
	isync
	mtspr	FSL_EIS_L1CSR0, r5
	isync

.Lmachine_check_wait_for_cfi_cleared:

	mfspr	r5, FSL_EIS_L1CSR0
	andi.	r5, r5, FSL_EIS_L1CSR0_CFI
	bne	.Lmachine_check_wait_for_cfi_cleared

	/* Disable MSR[ME] */
	mtmsr	r4
	isync

	/* Disable data cache flush assist HID0[DCFA] */
	mfspr	r4, HID0
	rlwinm  r4, r4, 0, 26, 24
	mtspr	HID0, r4

	/* Restore machine check context */
	addi	r1, r1, EXC_GENERIC_SIZE
	PPC_REG_STORE	r1, 0(r3)
	PPC_REG_LOAD	r4, GPR1_OFFSET(r1)
	PPC_REG_LOAD	r5, SRR0_FRAME_OFFSET(r1)
	PPC_REG_LOAD	r6, SRR1_FRAME_OFFSET(r1)
	mtsprg	2, r4
	PPC_REG_LOAD	r4, EXC_CTR_OFFSET(r1)
	mtmcsrr0	r5
	lwz	r5, 8(r3)
	mtmcsrr1	r6
	mtctr	r4

	/* Increment the L1 data recovery counter */
	addi	r5, r5, 1
	stw	r5, 8(r3)
	b	.Lmachine_check_return

.Lmachine_check_fatal_error:

	li	r3, 1
	PPC_REG_LOAD	r6, EXC_CR_OFFSET(r1)
	PPC_REG_LOAD	r4, GPR4_OFFSET(r1)
	PPC_REG_LOAD	r5, GPR5_OFFSET(r1)
	mtcr	r6
	PPC_REG_LOAD	r6, GPR6_OFFSET(r1)
	b	ppc_exc_fatal_machine_check
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE

.Lmachine_check_return:

	PPC_REG_LOAD	r6, EXC_CR_OFFSET(r1)
	PPC_REG_LOAD	r3, GPR3_OFFSET(r1)
	PPC_REG_LOAD	r4, GPR4_OFFSET(r1)
	PPC_REG_LOAD	r5, GPR5_OFFSET(r1)
	mtcr	r6
	PPC_REG_LOAD	r6, GPR6_OFFSET(r1)
	mfsprg	r1, 2
	rfmci

	.balign 0x8000

bsp_exc_vector_cacheable_base:

	/* Critical input (IVOR0) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 0
	b	ppc_exc_fatal_critical
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Data storage (IVOR2) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 2
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Instruction storage (IVOR3) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 3
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* External input (IVOR4) */
	PPC_REG_STORE_UPDATE	r1, -PPC_EXC_INTERRUPT_FRAME_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 4
	b	ppc_exc_interrupt
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Alignment (IVOR5) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 5
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Program (IVOR6) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 6
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#ifdef __PPC_CPU_E6500__
	/* Floating-point unavailable (IVOR7) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 7
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#endif
	/* System call (IVOR8) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 8
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#ifdef __PPC_CPU_E6500__
	/* APU unavailable (IVOR9) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 9
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#endif
	/* Decrementer (IVOR10) */
#ifdef QORIQ_IS_HYPERVISOR_GUEST
	PPC_REG_STORE_UPDATE	r1, -PPC_EXC_INTERRUPT_FRAME_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 10
	b	ppc_exc_interrupt
#else
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 10
	b	ppc_exc_fatal_normal
#endif
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Fixed-interval timer interrupt (IVOR11) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 11
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Watchdog timer interrupt (IVOR12) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 12
	b	ppc_exc_fatal_critical
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Data TLB error (IVOR13) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 13
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Instruction TLB error (IVOR14) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 14
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Debug (IVOR15) */
	.balign VECTOR_TABLE_ENTRY_SIZE
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 15
	b	ppc_exc_fatal_debug
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* SPE APU unavailable or AltiVec unavailable (IVOR32) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 32
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* SPE floating-point data exception or AltiVec assist (IVOR33) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 33
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#ifndef __PPC_CPU_E6500__
	/* SPE floating-point round exception (IVOR34) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 34
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#endif
	/* Performance monitor (IVOR35) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 35
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#ifdef __PPC_CPU_E6500__
	/* Processor doorbell interrupt (IVOR36) */
#if defined(QORIQ_IS_HYPERVISOR_GUEST) && defined(RTEMS_SMP)
	PPC_REG_STORE_UPDATE	r1, -PPC_EXC_INTERRUPT_FRAME_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 36
	b	ppc_exc_interrupt
#else
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 36
	b	ppc_exc_fatal_normal
#endif
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Processor doorbell critical interrupt (IVOR37) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 37
	b	ppc_exc_fatal_critical
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Guest processor doorbell (IVOR38) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 38
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Guest processor doorbell critical and machine check (IVOR39) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 39
	b	ppc_exc_fatal_critical
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Hypervisor system call (IVOR40) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 40
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* Hypervisor privilege (IVOR41) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 41
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
	/* LRAT error (IVOR42) */
	PPC_REG_STORE_UPDATE	r1, -EXC_GENERIC_SIZE(r1)
	PPC_REG_STORE	r3, PPC_EXC_GPR3_PROLOGUE_OFFSET(r1)
	li	r3, 42
	b	ppc_exc_fatal_normal
	START_NOP_FOR_LINKER_TOC_POINTER_RESTORE
#endif

/* Symbol provided for debugging and tracing */
bsp_exc_vector_end:

	.section ".bsp_exceptions.machine_check", "aw"

	.balign 0x10000

qoriq_machine_check_data:

	MACHINE_CHECK_DATA
#if QORIQ_CPU_COUNT > 1
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 2
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 3
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 4
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 5
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 6
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 7
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 8
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 9
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 10
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 11
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 12
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 13
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 14
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 15
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 16
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 17
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 18
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 19
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 20
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 21
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 22
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 23
	MACHINE_CHECK_DATA
#endif
#if QORIQ_CPU_COUNT > 24
#error "unexpected QORIQ_CPU_COUNT"
#endif
