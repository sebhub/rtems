/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCQorIQ
 *
 * @brief This source file contains the BSP-specific SMP support.
 */

/*
 * Copyright (C) 2013, 2025 embedded brains GmbH & Co. KG
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

#include <rtems/dev/io.h>
#include <rtems/score/basedefs.h>
#include <rtems/score/smpimpl.h>

#include <libfdt.h>

#include <libcpu/powerpc-utility.h>

#include <bsp.h>
#include <bsp/fdt.h>
#include <bsp/mmu.h>
#include <bsp/fatal.h>
#include <bsp/qoriq.h>
#include <bsp/vectors.h>
#include <bsp/bootcard.h>
#include <bsp/irq-generic.h>
#include <bsp/linker-symbols.h>

#if QORIQ_THREAD_COUNT > 1
void _start_thread(void);
#endif

void _start_secondary_processor(void);

#ifndef QORIQ_SECONDARY_CORES_INITIALIZED_BY_BOOTLOADER
extern volatile uint32_t qoriq_boot_page[4096 / sizeof(uint32_t)];
#endif

#define IPI_INDEX 0

#if QORIQ_THREAD_COUNT > 1
static bool is_started_by_u_boot(uint32_t cpu_index)
{
  return cpu_index % QORIQ_THREAD_COUNT == 0;
}

void qoriq_start_thread(Per_CPU_Control *cpu_self)
{
  ppc_exc_initialize_interrupt_stack(
    (uintptr_t) cpu_self->interrupt_stack_low
  );

  bsp_interrupt_facility_initialize();

  _SMP_Start_multitasking_on_secondary_processor(cpu_self);
}
#endif

static void start_thread_if_necessary(uint32_t cpu_index_self)
{
#if QORIQ_THREAD_COUNT > 1
  uint32_t i;

  for (i = 1; i < QORIQ_THREAD_COUNT; ++i) {
    uint32_t cpu_index_next = cpu_index_self + i;

    if (
      is_started_by_u_boot(cpu_index_self)
        && cpu_index_next < rtems_configuration_get_maximum_processors()
        && _SMP_Should_start_processor(cpu_index_next)
    ) {
      /* Thread Initial Next Instruction Address (INIA) */
      PPC_SET_THREAD_MGMT_REGISTER(321, (uintptr_t) _start_thread);

      /* Thread Initial Machine State (IMSR) */
      PPC_SET_THREAD_MGMT_REGISTER(289, QORIQ_INITIAL_MSR);

      /* Thread Enable Set (TENS) */
      PPC_SET_SPECIAL_PURPOSE_REGISTER(FSL_EIS_TENS, 1U << i);
    }
  }
#else
  (void) cpu_index_self;
#endif
}

void bsp_start_on_secondary_processor(Per_CPU_Control *cpu_self)
{
  uint32_t cpu_index_self;

#ifndef QORIQ_SECONDARY_CORES_INITIALIZED_BY_BOOTLOADER
  /* Disable the boot page translation */
  qoriq.lcc.bptr &= ~BPTR_EN;
#endif

  bsp_interrupt_facility_initialize();

  cpu_index_self = _Per_CPU_Get_index(cpu_self);
  start_thread_if_necessary(cpu_index_self);

  _SMP_Start_multitasking_on_secondary_processor(cpu_self);
}

#ifndef QORIQ_IS_HYPERVISOR_GUEST
static void bsp_inter_processor_interrupt(void *arg)
{
  (void) arg;

  _SMP_Inter_processor_interrupt_handler(_Per_CPU_Get());
}
#endif

static void setup_boot_page(void)
{
#ifdef QORIQ_IS_HYPERVISOR_GUEST
  qoriq_mmu_adjust_and_write_to_tlb1(
    QORIQ_TLB1_ENTRY_COUNT - 1,
    0xfffff000,
    0xffffffff,
    0,
    0,
    FSL_EIS_MAS3_SR | FSL_EIS_MAS3_SW,
    0
  );
#endif
}

static uint32_t discover_processors(void)
{
#ifdef QORIQ_SECONDARY_CORES_INITIALIZED_BY_BOOTLOADER
  const void *fdt = bsp_fdt_get();
  int cpus = fdt_path_offset(fdt, "/cpus");
  int node = fdt_first_subnode(fdt, cpus);
  uint32_t cpu = 0;

  while (node >= 0 && cpu < RTEMS_ARRAY_SIZE(qoriq_start_spin_table_addr)) {
    int len;
    fdt64_t *addr_fdt = (fdt64_t *)
      fdt_getprop(fdt, node, "cpu-release-addr", &len);

    if (addr_fdt != NULL) {
      uintptr_t addr = (uintptr_t) fdt64_to_cpu(*addr_fdt);

      qoriq_start_spin_table_addr[cpu] = (qoriq_start_spin_table *) addr;
    }

    ++cpu;
    node = fdt_next_subnode(fdt, node);
  }

  return cpu * QORIQ_THREAD_COUNT;
#else
  return QORIQ_CPU_COUNT * QORIQ_THREAD_COUNT;
#endif
}

uint32_t _CPU_SMP_Initialize(void)
{
  uint32_t cpu_count = 1;

  if (rtems_configuration_get_maximum_processors() > 0) {
    setup_boot_page();
    cpu_count = discover_processors();
  }

  start_thread_if_necessary(0);

  return cpu_count;
}

#ifdef QORIQ_SECONDARY_CORES_INITIALIZED_BY_BOOTLOADER
static bool release_processor(
  qoriq_start_spin_table *spin_table,
  uint32_t cpu_index
)
{
  bool spin_table_present = (spin_table != NULL);

  if (spin_table_present) {
    const Per_CPU_Control *cpu = _Per_CPU_Get_by_index(cpu_index);

    spin_table->pir = cpu_index;
    spin_table->r3 = (uintptr_t) cpu->interrupt_stack_high;
    rtems_cache_flush_multiple_data_lines(spin_table, sizeof(*spin_table));
    ppc_synchronize_data();
    spin_table->addr = (uintptr_t) _start_secondary_processor;
    rtems_cache_flush_multiple_data_lines(spin_table, sizeof(*spin_table));
  }

  return spin_table_present;
}

static qoriq_start_spin_table *get_spin_table(uint32_t cpu_index)
{
  qoriq_start_spin_table *spin_table;

  spin_table = qoriq_start_spin_table_addr[cpu_index / QORIQ_THREAD_COUNT];

  return spin_table;
}
#endif

bool _CPU_SMP_Start_processor(uint32_t cpu_index)
{
#ifndef QORIQ_SECONDARY_CORES_INITIALIZED_BY_BOOTLOADER
  volatile uint32_t *boot_status_and_control =
    &qoriq_boot_page[RTEMS_ARRAY_SIZE(qoriq_boot_page) - 64];
  uint32_t devdisr;

  /*
   * This code assumes that there are exactly two e500 cores and the current
   * core is core 0 the core to start is core 1 (second core).
   */
  RTEMS_STATIC_ASSERT(QORIQ_CPU_COUNT == 2, unexpected_number_of_cores);

  /*
   * Enable boot page translation for the page containing the start code for
   * the second core.
   */
  qoriq.lcc.bptr = BPTR_EN | BPTR_BOOT_PAGE((uintptr_t) qoriq_boot_page >> 12);

  /* Disable timer facilities of both cores */
  devdisr = qoriq.global_utilities.devdisr;
  qoriq.global_utilities.devdisr = devdisr | DEVDISR_TB0 | DEVDISR_TB1;
  ppc_msync();
  ppc_synchronize_instructions();

  /* Store current time base for second core */
  boot_status_and_control[1] = ppc_time_base();
  boot_status_and_control[2] = ppc_time_base_upper();

  /* Let both cores run */
  qoriq.ecm.eebpcr |= EEBPCR_CPU1_EN | EEBPCR_CPU2_EN;
  ppc_msync();
  ppc_synchronize_instructions();

  /* Wait for the second core */
  while (boot_status_and_control[3] == 0) {
    _IO_Relax();
  }

  /* Enable timer facilities of both cores */
  qoriq.global_utilities.devdisr = devdisr & ~(DEVDISR_TB0 | DEVDISR_TB1);

  return true;
#elif QORIQ_THREAD_COUNT > 1
  if (is_started_by_u_boot(cpu_index)) {
    qoriq_start_spin_table *spin_table = get_spin_table(cpu_index);

    return release_processor(spin_table, cpu_index);
  } else {
    return _SMP_Should_start_processor(cpu_index - 1);
  }
#else
  qoriq_start_spin_table *spin_table = get_spin_table(cpu_index);

  return release_processor(spin_table, cpu_index);
#endif
}

static rtems_interrupt_entry qoriq_ipi_entry;

void _CPU_SMP_Finalize_initialization(uint32_t cpu_count)
{
#ifndef QORIQ_IS_HYPERVISOR_GUEST
  rtems_status_code sc;

  rtems_interrupt_entry_initialize(
    &qoriq_ipi_entry,
    bsp_inter_processor_interrupt,
    NULL,
    "IPI"
  );
  sc = rtems_interrupt_entry_install(
    QORIQ_IRQ_IPI_0 + IPI_INDEX,
    RTEMS_INTERRUPT_UNIQUE,
    &qoriq_ipi_entry
  );
  if (sc != RTEMS_SUCCESSFUL) {
    bsp_fatal(QORIQ_FATAL_SMP_IPI_HANDLER_INSTALL);
  }
#endif

  (void) cpu_count;
}

void _CPU_SMP_Prepare_start_multitasking(void)
{
  /* Do nothing */
}

void _CPU_SMP_Send_interrupt(uint32_t target_processor_index)
{
#ifdef QORIQ_IS_HYPERVISOR_GUEST
  uint32_t msg;

  /* DBELL message type */
  msg = (0U << (63 - 36)) | target_processor_index;
  __asm__ volatile ("msgsnd %0" : : "r" (msg));
#else
  qoriq.pic.ipidr [IPI_INDEX].reg = 1U << target_processor_index;
#endif
}
