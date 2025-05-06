/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCQorIQ
 *
 * @brief This source file contains the MSI interrupt support.
 */

/*
 * Copyright (C) 2022 embedded brains GmbH & Co. KG
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

#include <sys/bitset.h>

#include <bsp.h>
#include <bsp/irq-generic.h>
#include <bsp/qoriq.h>

#ifndef QORIQ_IS_HYPERVISOR_GUEST

typedef __BITSET_DEFINE(pic_msi_bitset, QORIQ_IRQ_MSI_COUNT) pic_msi_bitset;

static pic_msi_bitset pic_msi_available =
  __BITSET_T_INITIALIZER(__BITSET_FSET(__bitset_words(QORIQ_IRQ_MSI_COUNT)));


static uint32_t pic_msi_bitset_to_uint32_t(
	const pic_msi_bitset *bitset,
	uint32_t              index
)
{
	long bits = bitset->__bits[index / _BITSET_BITS];

	return (uint32_t) (bits >> (32 * ((index % _BITSET_BITS) / 32)));
}

static void pic_msi_dispatch(void *arg)
{
	uintptr_t reg = (uintptr_t) arg;
	uint32_t msir = qoriq.pic.msir[reg].reg;

	while (msir != 0) {
		uint32_t index = 31 - __builtin_clz(msir);
		const rtems_interrupt_entry *entry;

		msir &= ~(UINT32_C(1) << index);
		entry = bsp_interrupt_entry_load_first(
			QORIQ_IRQ_MSI_VECTOR(32 * reg + index)
		);

		if (entry != NULL) {
			bsp_interrupt_dispatch_entries(entry);
		}
	}
}

static rtems_status_code pic_msi_allocate(rtems_vector_number *vector)
{
	pic_msi_bitset *available = &pic_msi_available;
	long found = __BIT_FFS(QORIQ_IRQ_MSI_COUNT, available);
	rtems_vector_number index;
	uint32_t subset;

	if (found == 0) {
		return RTEMS_UNSATISFIED;
	}

	index = (rtems_vector_number) found - 1;
	subset = pic_msi_bitset_to_uint32_t(available, index);

	if (subset == 0xffffffff) {
		uintptr_t reg = index / 32;
		rtems_status_code sc;

		sc = rtems_interrupt_handler_install(
			QORIQ_IRQ_MSI_0 + reg,
			"MSI",
			RTEMS_INTERRUPT_UNIQUE,
			pic_msi_dispatch,
			(void *) reg
		);

		if (sc != RTEMS_SUCCESSFUL) {
			return sc;
		}
	}

	__BIT_CLR(QORIQ_IRQ_MSI_COUNT, index, available);
	*vector = QORIQ_IRQ_MSI_VECTOR(index);
	return RTEMS_SUCCESSFUL;
}

static rtems_status_code pic_msi_free(rtems_vector_number vector)
{
	pic_msi_bitset *available = &pic_msi_available;
	rtems_vector_number index = QORIQ_IRQ_MSI_INDEX(vector);
	uint32_t subset;

	if (__BIT_ISSET(QORIQ_IRQ_MSI_COUNT, index, available)) {
		return RTEMS_NOT_DEFINED;
	}

	__BIT_SET(QORIQ_IRQ_MSI_COUNT, index, available);
	subset = pic_msi_bitset_to_uint32_t(available, index);

	if (subset == 0xffffffff) {
		uintptr_t reg = index / 32;

		return rtems_interrupt_handler_remove(
			QORIQ_IRQ_MSI_0 + reg,
			pic_msi_dispatch,
			(void *) reg
		);
	}

	return RTEMS_SUCCESSFUL;
}

rtems_status_code qoriq_pic_msi_allocate(rtems_vector_number *vector)
{
	rtems_status_code sc;

	if (!bsp_interrupt_is_initialized()) {
		return RTEMS_INCORRECT_STATE;
	}

	if (vector == NULL) {
		return RTEMS_INVALID_ADDRESS;
	}

	if (rtems_interrupt_is_in_progress()) {
		return RTEMS_CALLED_FROM_ISR;
	}

	bsp_interrupt_lock();
	sc = pic_msi_allocate(vector);
	bsp_interrupt_unlock();
	return sc;
}

rtems_status_code qoriq_pic_msi_free(rtems_vector_number vector)
{
	rtems_status_code sc;

	if (!bsp_interrupt_is_initialized()) {
		return RTEMS_INCORRECT_STATE;
	}

	if (!QORIQ_IRQ_IS_MSI(vector) ) {
		return RTEMS_INVALID_ID;
	}

	if (rtems_interrupt_is_in_progress()) {
		return RTEMS_CALLED_FROM_ISR;
	}

	bsp_interrupt_lock();
	sc = pic_msi_free(vector);
	bsp_interrupt_unlock();
	return sc;
}

rtems_status_code qoriq_pic_msi_map(
	rtems_vector_number vector,
	uint64_t *addr,
	uint32_t *data
)
{
	if (addr == NULL) {
		return RTEMS_INVALID_ADDRESS;
	}

	if (data == NULL) {
		return RTEMS_INVALID_ADDRESS;
	}

	if (!QORIQ_IRQ_IS_MSI(vector) ) {
		return RTEMS_INVALID_ID;
	}

	*addr = (uint64_t)(uintptr_t) &qoriq.pic.msiir;
	*data = QORIQ_IRQ_MSI_INDEX(vector) << 24;
	return RTEMS_SUCCESSFUL;
}

#endif /* QORIQ_IS_HYPERVISOR_GUEST */
