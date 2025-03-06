/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCQorIQ
 *
 * @brief BSP startup.
 */

/*
 * Copyright (C) 2010, 2017 embedded brains GmbH & Co. KG
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

#include <libfdt.h>

#include <rtems.h>
#include <rtems/config.h>
#include <rtems/counter.h>
#include <rtems/sysinit.h>

#include <libcpu/powerpc-utility.h>

#include <bsp.h>
#include <bsp/bootcard.h>
#include <bsp/fatal.h>
#include <bsp/fdt.h>
#include <bsp/irq-generic.h>
#include <bsp/linker-symbols.h>
#include <bsp/mmu.h>
#include <bsp/qoriq.h>

#ifdef RTEMS_MULTIPROCESSING
#include <bsp/intercom.h>
#endif

LINKER_SYMBOL(bsp_exc_vector_base);
LINKER_SYMBOL(bsp_exc_vector_cacheable_base);

qoriq_start_spin_table *
qoriq_start_spin_table_addr[QORIQ_CPU_COUNT / QORIQ_THREAD_COUNT];

/* Configuration parameters for console driver, ... */
unsigned int BSP_bus_frequency;

/* Configuration parameter for clock driver, ... */
uint32_t bsp_time_base_frequency;

uint32_t qoriq_clock_frequency;

uint32_t _CPU_Counter_frequency(void)
{
#ifdef __PPC_CPU_E6500__
  return qoriq_clock_frequency;
#else
  return bsp_time_base_frequency;
#endif
}

static void initialize_frequency_parameters(void)
{
  const void *fdt = bsp_fdt_get();
  int node;
  int len;
  fdt32_t *val_fdt;

  node = fdt_node_offset_by_prop_value(fdt, -1, "device_type", "cpu", 4);

  val_fdt = (fdt32_t *) fdt_getprop(fdt, node, "bus-frequency", &len);
  if (val_fdt == NULL || len != 4) {
    bsp_fatal(QORIQ_FATAL_FDT_NO_BUS_FREQUENCY);
  }
  BSP_bus_frequency = fdt32_to_cpu(*val_fdt) / QORIQ_BUS_CLOCK_DIVIDER;

  val_fdt = (fdt32_t *) fdt_getprop(fdt, node, "timebase-frequency", &len);
  if (val_fdt == NULL || len != 4) {
    bsp_fatal(QORIQ_FATAL_FDT_NO_BUS_FREQUENCY);
  }
  bsp_time_base_frequency = fdt32_to_cpu(*val_fdt);

  #ifdef __PPC_CPU_E6500__
    val_fdt = (fdt32_t *) fdt_getprop(fdt, node, "clock-frequency", &len);
    if (val_fdt == NULL || len != 4) {
      bsp_fatal(QORIQ_FATAL_FDT_NO_CLOCK_FREQUENCY);
    }
    qoriq_clock_frequency = fdt32_to_cpu(*val_fdt);
  #endif
}

void bsp_start(void)
{
  /*
   * Get CPU identification dynamically. Note that the get_ppc_cpu_type() function
   * store the result in global variables so that it can be used latter...
   */
  get_ppc_cpu_type();
  get_ppc_cpu_revision();

  initialize_frequency_parameters();
  bsp_interrupt_initialize();

  rtems_cache_coherent_add_area(
    bsp_section_nocacheheap_begin,
    (uintptr_t) bsp_section_nocacheheap_size
  );

#ifndef QORIQ_IS_HYPERVISOR_GUEST
  /* Disable boot page translation */
#if QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT)
  qoriq.lcc.bstar &= ~LCC_BSTAR_EN;
#else
  qoriq.lcc.bptr &= ~BPTR_EN;
#endif
#endif
}

#ifdef RTEMS_MULTIPROCESSING
RTEMS_SYSINIT_ITEM(
  qoriq_intercom_init,
  RTEMS_SYSINIT_BSP_PRE_DRIVERS,
  RTEMS_SYSINIT_ORDER_MIDDLE
);
#endif
