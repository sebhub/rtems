/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCQorIQMMU
 *
 * @brief This source file contains the implementation of the fixed QorIQ MMU
 *   configuration.
 */

/*
 * Copyright (C) 2025 embedded brains GmbH & Co. KG
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

#include <bsp.h>
#include <bsp/bootcard.h>
#include <bsp/linker-symbols.h>
#include <bsp/mmu.h>

#ifdef QORIQ_USE_FIXED_MMU_CONFIG

#define TEXT __attribute__((section(".bsp_start_text")))
#define DATA __attribute__((section(".bsp_start_data")))

#define ENTRY_R(b, ts) { \
	.begin = (uintptr_t) b, \
	.tsize = ts, \
	.mas2 = 0, \
	.mas3 = FSL_EIS_MAS3_SR \
}

#define ENTRY_IO(b, ts) { \
	.begin = (uintptr_t) b, \
	.tsize = ts, \
	.mas2 = FSL_EIS_MAS2_I | FSL_EIS_MAS2_G, \
	.mas3 = FSL_EIS_MAS3_SR | FSL_EIS_MAS3_SW \
}

#ifdef RTEMS_SMP
  #define ENTRY_RW_MAS2 FSL_EIS_MAS2_M
#else
  #define ENTRY_RW_MAS2 0
#endif

/* Fixed MMU configuration for a 1GiB DDR RAM */

qoriq_tlb1_entry qoriq_tlb1_config[QORIQ_TLB1_ENTRY_COUNT] = {
	ENTRY_R(0x00300000, FSL_EIS_MAS1_SIZE_1M),
	ENTRY_R(0x00400000, FSL_EIS_MAS1_SIZE_4M),
	ENTRY_R(0x00800000, FSL_EIS_MAS1_SIZE_4M),
	ENTRY_R(0x00c00000, FSL_EIS_MAS1_SIZE_4M),
	ENTRY_R(0x01000000, FSL_EIS_MAS1_SIZE_16M),
	ENTRY_R(0x02000000, FSL_EIS_MAS1_SIZE_16M),
	ENTRY_R(0x03000000, FSL_EIS_MAS1_SIZE_16M),
	ENTRY_R(0x04000000, FSL_EIS_MAS1_SIZE_64M),
	ENTRY_R(0x08000000, FSL_EIS_MAS1_SIZE_64M),
	ENTRY_R(0x0c000000, FSL_EIS_MAS1_SIZE_64M),
	ENTRY_R(0x10000000, FSL_EIS_MAS1_SIZE_256M),
	ENTRY_R(0x20000000, FSL_EIS_MAS1_SIZE_256M),
	ENTRY_R(0x30000000, FSL_EIS_MAS1_SIZE_256M),
	ENTRY_IO(0x40000000, FSL_EIS_MAS1_SIZE_1G),
	ENTRY_IO(0x80000000, FSL_EIS_MAS1_SIZE_1G),
	ENTRY_IO(0xc0000000, FSL_EIS_MAS1_SIZE_1G)
};

static const qoriq_tlb1_adjustment tlb1_adjustments[] = {
	{
		.begin = (uintptr_t) bsp_section_start_begin,
		.end = (uintptr_t) bsp_section_fast_text_end,
		.mas2 = 0,
		.mas3 = FSL_EIS_MAS3_SX
	}, {
		.begin = (uintptr_t) bsp_section_fast_data_begin,
		.end = (uintptr_t) bsp_section_stack_end,
		.mas2 = ENTRY_RW_MAS2,
		.mas3 = FSL_EIS_MAS3_SW
	}
};

void TEXT qoriq_tlb1_apply_adjustments(
	const qoriq_tlb1_adjustment *adjustments,
	size_t count
)
{
	size_t i;

	for (i = 0; i < count; ++i) {
		const qoriq_tlb1_adjustment *a;
		size_t j;

		a = &adjustments[i];

		for (j = 0; j < QORIQ_TLB1_ENTRY_COUNT; ++j) {
			qoriq_tlb1_entry *e;
			uintptr_t begin;
			uintptr_t last;

			e = &qoriq_tlb1_config[j];
			begin = e->begin;
			last = begin + (UINT32_C(1) << (2 * e->tsize + 10)) - 1;

			if (last < a->begin) {
				continue;
			}

			if (begin >= a->end) {
				break;
			}

			e->mas2 |= a->mas2;
			e->mas3 |= a->mas3;
		}
	}
}

static void TEXT qoriq_mmu_config_tlb0(void)
{
	uintptr_t begin;
	size_t i;

	/* The TLB0 can cover 2MiB, it is not fully used below */
	begin = 0x00100000;

	/* First 32KiB for cache inhibited exception entries */
	for (i = 0; i < 8; ++i) {
		qoriq_tlb0_write(
			0,
			0,
			FSL_EIS_MAS2_I,
			FSL_EIS_MAS3_SX,
			0,
			begin
		);
		begin += 4096;
	}

	/* Second 32KiB for cacheable exception entries */
	for (i = 0; i < 8; ++i) {
		qoriq_tlb0_write(
			0,
			0,
			0,
			FSL_EIS_MAS3_SR | FSL_EIS_MAS3_SX,
			0,
			begin
		);
		begin += 4096;
	}

	/* Data for machine check handling (cache inhibited) */
	for (i = 0; i < QORIQ_CPU_COUNT; ++i) {
		qoriq_tlb0_write(
			0,
			0,
			FSL_EIS_MAS2_I,
			FSL_EIS_MAS3_SR | FSL_EIS_MAS3_SW,
			0,
			begin
		);
		begin += 4096;
	}

#ifndef QORIQ_SECONDARY_CORES_INITIALIZED_BY_BOOTLOADER
	/* Boot page */
	qoriq_tlb0_write(
		0,
		0,
		FSL_EIS_MAS2_I,
		FSL_EIS_MAS3_SR | FSL_EIS_MAS3_SW,
		0,
		begin
	);
#endif
}

void TEXT qoriq_mmu_fixed_config(void)
{
	qoriq_tlb1_entry *e;
	size_t i;

	qoriq_mmu_config_tlb0();
	qoriq_tlb1_apply_adjustments(
		tlb1_adjustments,
		RTEMS_ARRAY_SIZE(tlb1_adjustments)
	);

	for (i = 0; i < QORIQ_TLB1_ENTRY_COUNT - 1; ++i) {
		e = &qoriq_tlb1_config[i];
		qoriq_tlb1_write(
			i,
			0,
			e->mas2,
			e->mas3,
			0,
			e->begin,
			e->tsize
		);
	}

	ppc_set_machine_state_register(QORIQ_INITIAL_MSR);
	ppc_synchronize_instructions();

	e = &qoriq_tlb1_config[i];
	qoriq_tlb1_write(
		i,
		0,
		e->mas2,
		e->mas3,
		0,
		e->begin,
		e->tsize
	);
}

static Memory_Area _Memory_Areas[] = {
	MEMORY_INITIALIZER(bsp_section_work_begin, bsp_section_work_end)
};

static const Memory_Information _Memory_Information =
	MEMORY_INFORMATION_INITIALIZER(_Memory_Areas);

const Memory_Information *_Memory_Get(void)
{
	return &_Memory_Information;
}

#endif /* QORIQ_USE_FIXED_MMU_CONFIG */
