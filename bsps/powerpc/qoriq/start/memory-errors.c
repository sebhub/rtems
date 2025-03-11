/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCQorIQ
 *
 * @brief This source file contains the implementation of the memory error
 *   handling support.
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
#include <bsp/qoriq.h>

#include <string.h>

#if !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT)

extern uint32_t qoriq_machine_check_data[QORIQ_CPU_COUNT][4096 / sizeof(uint32_t)];

static uint32_t l2_instruction_recoveries;

static uint32_t l2_single_bit_error_count;

static uint32_t l2_multiple_bit_error_count;

static uint32_t l2_tag_parity_error_count;

static uint32_t ddr_single_bit_error_count;

static uint32_t ddr_address_parity_error_count;

static uint32_t ddr_automatic_calibration_error_count;

static uint32_t ddr_multiple_bit_error_count;

static uint32_t ddr_memory_select_error_count;

void qoriq_get_memory_error_stats(qoriq_memory_error_stats *stats)
{
	size_t i;
	uint32_t reg;

	stats = memset(stats, 0, sizeof(*stats));

	for (i = 0; i < QORIQ_CPU_COUNT; ++i) {
		uint32_t *machine_check_data;

		machine_check_data = &qoriq_machine_check_data[i][0];
		stats->l1_stats[i].instruction_recoveries = machine_check_data[1];
		stats->l1_stats[i].data_recoveries = machine_check_data[2];
	}

	reg = qoriq.l2_cache.l2errctl;
	stats->l2_single_bit_error_threshold = (reg >> 16) & 0xff;
	stats->l2_single_bit_error_count =
		l2_single_bit_error_count + (reg & 0xff);

	reg = qoriq.ddr_controller.ddr_err_sbe;
	stats->ddr_single_bit_error_threshold = (reg >> 16) & 0xff;
	stats->ddr_single_bit_error_count =
		ddr_single_bit_error_count + (reg & 0xff);

	stats->l2_instruction_recoveries = l2_instruction_recoveries;
	stats->l2_multiple_bit_error_count = l2_multiple_bit_error_count;
	stats->l2_tag_parity_error_count = l2_tag_parity_error_count;
	stats->ddr_address_parity_error_count = ddr_address_parity_error_count;
	stats->ddr_automatic_calibration_error_count = ddr_automatic_calibration_error_count;
	stats->ddr_multiple_bit_error_count = ddr_multiple_bit_error_count;
	stats->ddr_memory_select_error_count = ddr_memory_select_error_count;
}

void qoriq_set_l2_single_bit_error_threshold(uint32_t threshold)
{
	if (threshold > 0xff) {
		threshold = 0xff;
	}

	qoriq.l2_cache.l2errctl = threshold << 16;
}

void qoriq_set_ddr_single_bit_error_threshold(uint32_t threshold)
{
	if (threshold > 0xff) {
		threshold = 0xff;
	}

	qoriq.ddr_controller.ddr_err_sbe = threshold << 16;
}

void qoriq_increment_l2_instruction_recoveries(void)
{
	++l2_instruction_recoveries;
}

void qoriq_increment_l2_single_bit_error_count(void)
{
	l2_single_bit_error_count +=
		(qoriq.l2_cache.l2errctl >> 16) & 0xff;
}

void qoriq_increment_l2_multiple_bit_error_count(void)
{
	++l2_multiple_bit_error_count;
}

void qoriq_increment_l2_tag_parity_error_count(void)
{
	++l2_tag_parity_error_count;
}

void qoriq_increment_ddr_address_parity_error_count(void)
{
	++ddr_address_parity_error_count;
}

void qoriq_increment_ddr_automatic_calibration_error_count(void)
{
	++ddr_automatic_calibration_error_count;
}

void qoriq_increment_ddr_single_bit_error_count(void)
{
	ddr_single_bit_error_count +=
		(qoriq.ddr_controller.ddr_err_sbe >> 16) & 0xff;
}

void qoriq_increment_ddr_multiple_bit_error_count(void)
{
	++ddr_multiple_bit_error_count;
}

void qoriq_increment_ddr_memory_select_error_count(void)
{
	++ddr_memory_select_error_count;
}

#endif /* !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT) */
