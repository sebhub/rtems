/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCQorIQ
 *
 * @brief Global BSP definitions.
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

#ifndef LIBBSP_POWERPC_QORIQ_BSP_H
#define LIBBSP_POWERPC_QORIQ_BSP_H

/**
 * @defgroup RTEMSBSPsPowerPCQorIQ NXP QorIQ
 *
 * @ingroup RTEMSBSPsPowerPC
 *
 * @brief NXP QorIQ Board Support Package.
 *
 * @{
 */

#include <bspopts.h>

#ifdef QORIQ_IS_HYPERVISOR_GUEST
#define QORIQ_THREAD_COUNT 1
#else
#define QORIQ_THREAD_COUNT QORIQ_PHYSICAL_THREAD_COUNT
#endif

#ifndef ASM

#include <rtems.h>

#include <bsp/default-initial-extension.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BSP_FEATURE_IRQ_EXTENSION

#define BSP_FDT_IS_SUPPORTED

#define QORIQ_CHIP(alpha, num) ((alpha) * 10000 + (num))

#define QORIQ_CHIP_P1020 QORIQ_CHIP('P', 1020)

#define QORIQ_CHIP_T2080 QORIQ_CHIP('T', 2080)

#define QORIQ_CHIP_T4240 QORIQ_CHIP('T', 4240)

#define QORIQ_CHIP_VARIANT QORIQ_CHIP(QORIQ_CHIP_SERIES, QORIQ_CHIP_NUMBER)

#define QORIQ_CHIP_IS_T_VARIANT(variant) ((variant) / 10000 == 'T')

extern unsigned BSP_bus_frequency;

struct rtems_bsdnet_ifconfig;

int BSP_tsec_attach(
  struct rtems_bsdnet_ifconfig *config,
  int attaching
);

int qoriq_if_intercom_attach_detach(
  struct rtems_bsdnet_ifconfig *config,
  int attaching
);

#if defined(HAS_UBOOT)
  /* Routine to obtain U-Boot environment variables */
  const char *bsp_uboot_getenv(
    const char *name
  );
#endif

RTEMS_NO_RETURN void bsp_restart(void *addr);

void *bsp_idle_thread( uintptr_t ignored );
#define BSP_IDLE_TASK_BODY bsp_idle_thread

#define RTEMS_BSP_NETWORK_DRIVER_ATTACH BSP_tsec_attach
#define RTEMS_BSP_NETWORK_DRIVER_ATTACH4 qoriq_if_intercom_attach_detach

#define RTEMS_BSP_NETWORK_DRIVER_NAME "tsec1"
#define RTEMS_BSP_NETWORK_DRIVER_NAME2 "tsec2"
#define RTEMS_BSP_NETWORK_DRIVER_NAME3 "tsec3"
#define RTEMS_BSP_NETWORK_DRIVER_NAME4 "intercom1"

/* Internal data and functions */

typedef struct {
  uint64_t addr;
  uint64_t r3;
  uint32_t reserved_0;
  uint32_t pir;
  uint64_t r6;
  uint32_t reserved_1[8];
} qoriq_start_spin_table;

extern qoriq_start_spin_table *
qoriq_start_spin_table_addr[QORIQ_CPU_COUNT / QORIQ_THREAD_COUNT];

struct Per_CPU_Control;

void qoriq_start_thread(struct Per_CPU_Control *);

RTEMS_NO_RETURN void qoriq_restart_secondary_processor(
  const qoriq_start_spin_table *spin_table
);

void qoriq_decrementer_dispatch(void);

extern uint32_t bsp_time_base_frequency;

extern uint32_t qoriq_clock_frequency;

/**
 * @brief This structure contains the L1 data and instruction recovery counters
 *   of a core.
 */
typedef struct {
  /**
   * @brief This member contains the count of L1 data cache recovery counties
   *   carried out by the machine check handler.
   */
  uint32_t data_recoveries;

  /**
   * @brief This member contains the count of L1 data instruction recoveries
   *   carried out by the machine check handler.
   */
  uint32_t instruction_recoveries;
} qoriq_l1_error_stats;

/**
 * @brief This structure contains the memory error statistics.
 *
 * Use qoriq_get_memory_error_stats() to obtain a snapshot of the memory statistics.
 */
typedef struct {
  /**
   * @brief This member contains the L1 cache recovery counts for each core.
   */
  qoriq_l1_error_stats l1_stats[QORIQ_CPU_COUNT];

  /**
   * @brief This member contains the count of L2 instruction recoveries carried
   *   out by the L2 error handler.
   */
  uint32_t l2_instruction_recoveries;

  /**
   * @brief This member contains the L2 single bit error threshold set in the
   *   L2 cache controller.
   */
  uint32_t l2_single_bit_error_threshold;

  /**
   * @brief This member contains the count of L2 single bit errors present in
   *   the L2 cache controller and recognized by the L2 error handler.
   */
  uint32_t l2_single_bit_error_count;

  /**
   * @brief This member contains the count of L2 multiple bit errors recognized
   *   by the L2 error handler.
   */
  uint32_t l2_multiple_bit_error_count;

  /**
   * @brief This member contains the count of L2 tag parity errors recognized
   *   by the L2 error handler.
   */
  uint32_t l2_tag_parity_error_count;

  /**
   * @brief This member contains the DDR memory single bit error threshold set
   *   in the DDR memory controller.
   */
  uint32_t ddr_single_bit_error_threshold;

  /**
   * @brief This member contains the count of DDR memory single bit errors
   *   present in the DDR memory controller and recognized by the DDR memory
   *   error handler.
   */
  uint32_t ddr_single_bit_error_count;

  /**
   * @brief This member contains the count of DDR memory address parity errors
   *   recognized by the DDR memory error handler.
   */
  uint32_t ddr_address_parity_error_count;

  /**
   * @brief This member contains the count of DDR memory automatic calibration
   *   errors recognized by the DDR memory error handler.
   */
  uint32_t ddr_automatic_calibration_error_count;

  /**
   * @brief This member contains the count of DDR memory multiple bit errors
   *   recognized by the DDR memory error handler.
   */
  uint32_t ddr_multiple_bit_error_count;

  /**
   * @brief This member contains the count of DDR memory select errors
   *   recognized by the DDR memory error handler.
   */
  uint32_t ddr_memory_select_error_count;
} qoriq_memory_error_stats;

/**
 * @brief Gets a snapshot of the memory error statistics.
 *
 * @param[out] stats is a reference to the memory error statistics block.
 */
void qoriq_get_memory_error_stats(qoriq_memory_error_stats *stats);

/**
 * @brief Sets the L2 cache single bit error threshold.
 *
 * @param[in] threshold is the new threshold.
 */
void qoriq_set_l2_single_bit_error_threshold(uint32_t threshold);

/**
 * @brief Sets the DDR single bit error threshold.
 *
 * @param[in] threshold is the new threshold.
 */
void qoriq_set_ddr_single_bit_error_threshold(uint32_t threshold);

/**
 * @brief Increments the L2 cache instruction recovery counter.
 *
 * Use it in the L2 cache error handler to update the statistics.
 */
void qoriq_increment_l2_instruction_recoveries(void);

/**
 * @brief Increments the L2 cache single bit error counter.
 *
 * Use it in the L2 cache error handler to update the statistics.
 */
void qoriq_increment_l2_single_bit_error_count(void);

/**
 * @brief Increments the L2 cache multiple bit error counter.
 *
 * Use it in the L2 cache error handler to update the statistics.
 */
void qoriq_increment_l2_multiple_bit_error_count(void);

/**
 * @brief Increments the L2 cache tag parity bit error counter.
 *
 * Use it in the L2 cache error handler to update the statistics.
 */
void qoriq_increment_l2_tag_parity_error_count(void);

/**
 * @brief Increments the DDR memory address parity error counter.
 *
 * Use it in the DDR memory error handler to update the statistics.
 */
void qoriq_increment_ddr_address_parity_error_count(void);

/**
 * @brief Increments the DDR memory automatic calibration error counter.
 *
 * Use it in the DDR memory error handler to update the statistics.
 */
void qoriq_increment_ddr_automatic_calibration_error_count(void);

/**
 * @brief Increments the DDR memory single bit error counter.
 *
 * Use it in the DDR memory error handler to update the statistics.
 */
void qoriq_increment_ddr_single_bit_error_count(void);

/**
 * @brief Increments the DDR memory multiple bit error counter.
 *
 * Use it in the DDR memory error handler to update the statistics.
 */
void qoriq_increment_ddr_multiple_bit_error_count(void);

/**
 * @brief Increments the DDR memory memory select error counter.
 *
 * Use it in the DDR memory error handler to update the statistics.
 */
void qoriq_increment_ddr_memory_select_error_count(void);

#ifdef QORIQ_INITIALIZE_CACHES
void qoriq_initialize_l1_cache_config(void *fdt, uint32_t *config);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ASM */

/** @} */

#endif /* LIBBSP_POWERPC_QORIQ_BSP_H */
