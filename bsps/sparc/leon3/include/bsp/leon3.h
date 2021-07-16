/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsSPARCLEON3
 *
 * @brief This header file provides interfaces used by the BSP implementation.
 */

/*
 * Copyright (C) 2014, 2021 embedded brains GmbH & Co. KG
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

#ifndef LIBBSP_SPARC_LEON3_BSP_LEON3_H
#define LIBBSP_SPARC_LEON3_BSP_LEON3_H

#include <grlib/apbuart-regs.h>
#include <grlib/gptimer-regs.h>

#include <bspopts.h>
#include <bsp/irqimpl.h>

#if !defined(LEON3_PLB_FREQUENCY_DEFINED_BY_GPTIMER)
#include <grlib/ambapp.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup RTEMSBSPsSPARCLEON3
 *
 * @{
 */

/**
 * @brief This constant represents the flush instruction cache flag of the LEON
 *   cache control register.
 */
#define LEON3_REG_CACHE_CTRL_FI 0x00200000U

/**
 * @brief This constant represents the data cache snooping enable flag of the
 *   LEON cache control register.
 */
#define LEON3_REG_CACHE_CTRL_DS 0x00800000U

/**
 * @brief Sets the ASI 0x2 system register value.
 *
 * @param addr is the address of the ASI 0x2 system register.
 *
 * @param val is the value to set.
 */
static inline void leon3_set_system_register( uint32_t addr, uint32_t val )
{
  __asm__ volatile(
    "sta %1, [%0] 2"
    :
    : "r" ( addr ), "r" ( val )
  );
}

/**
 * @brief Gets the ASI 0x2 system register value.
 *
 * @param addr is the address of the ASI 0x2 system register.
 *
 * @return Returns the register value.
 */
static inline uint32_t leon3_get_system_register( uint32_t addr )
{
  uint32_t val;

  __asm__ volatile(
    "lda [%1] 2, %0"
    : "=r" ( val )
    : "r" ( addr )
  );

  return val;
}

/**
 * @brief Sets the LEON cache control register value.
 *
 * @param val is the value to set.
 */
static inline void leon3_set_cache_control_register( uint32_t val )
{
  leon3_set_system_register( 0x0, val );
}

/**
 * @brief Gets the LEON cache control register value.
 *
 * @return Returns the register value.
 */
static inline uint32_t leon3_get_cache_control_register( void )
{
  return leon3_get_system_register( 0x0 );
}

/**
 * @brief Checks if the data cache snooping is enabled.
 *
 * @return Returns true, if the data cache snooping is enabled, otherwise
 *   false.
 */
static inline bool leon3_data_cache_snooping_enabled( void )
{
  return ( leon3_get_cache_control_register() & LEON3_REG_CACHE_CTRL_DS ) != 0;
}

/**
 * @brief Gets the LEON instruction cache configuration register value.
 *
 * @return Returns the register value.
 */
static inline uint32_t leon3_get_inst_cache_config_register( void )
{
  return leon3_get_system_register( 0x8 );
}

/**
 * @brief Gets the LEON data cache configuration register value.
 *
 * @return Returns the register value.
 */
static inline uint32_t leon3_get_data_cache_config_register( void )
{
  return leon3_get_system_register( 0xc );
}

/**
 * @brief This constant defines the index of the GPTIMER timer used by the
 *   clock driver.
 */
#if defined(RTEMS_MULTIPROCESSING)
#define LEON3_CLOCK_INDEX \
  ( rtems_configuration_get_user_multiprocessing_table() ? LEON3_Cpu_Index : 0 )
#else
#define LEON3_CLOCK_INDEX 0
#endif

/**
 * @brief This constant defines the index of the GPTIMER timer used by the
 *   CPU counter if the CPU counter uses the GPTIMER.
 */
#if defined(RTEMS_SMP)
#define LEON3_COUNTER_GPTIMER_INDEX ( LEON3_CLOCK_INDEX + 1 )
#else
#define LEON3_COUNTER_GPTIMER_INDEX LEON3_CLOCK_INDEX
#endif

/**
 * @brief This constant defines the frequency set by the boot loader of the
 *   first GPTIMER instance.
 *
 * We assume that a boot loader (usually GRMON) initialized the GPTIMER 0 to
 * run with 1MHz.  This is used to determine all clock frequencies of the PnP
 * devices.  See also ambapp_freq_init() and ambapp_freq_get().
 */
#define LEON3_GPTIMER_0_FREQUENCY_SET_BY_BOOT_LOADER 1000000

/**
 * @brief This pointer provides the GPTIMER register block address.
 */
#if defined(LEON3_GPTIMER_BASE)
#define LEON3_Timer_Regs ((gptimer *) LEON3_GPTIMER_BASE)
#else
extern gptimer *LEON3_Timer_Regs;
#endif

/**
 * @brief This pointer provides the GPTIMER device information block.
 */
extern struct ambapp_dev *LEON3_Timer_Adev;

/**
 * @brief Gets the processor local bus frequency in Hz.
 *
 * @return Returns the frequency.
 */
static inline uint32_t leon3_processor_local_bus_frequency( void )
{
#if defined(LEON3_PLB_FREQUENCY_DEFINED_BY_GPTIMER)
  return ( grlib_load_32( &LEON3_Timer_Regs->sreload ) + 1 ) *
    LEON3_GPTIMER_0_FREQUENCY_SET_BY_BOOT_LOADER;
#else
  /*
   * For simplicity, assume that the interrupt controller uses the processor
   * clock.  This is at least true on the GR740.
   */
  return ambapp_freq_get( ambapp_plb(), LEON3_IrqCtrl_Adev );
#endif
}

/**
 * @brief Gets the LEON up-counter low register (%ASR23) value.
 *
 * @return Returns the register value.
 */
static inline uint32_t leon3_up_counter_low( void )
{
  uint32_t asr23;

  __asm__ volatile (
    "mov %%asr23, %0"
    : "=&r" (asr23)
  );

  return asr23;
}

/**
 * @brief Gets the LEON up-counter high register (%ASR22) value.
 *
 * @return Returns the register value.
 */
static inline uint32_t leon3_up_counter_high(void)
{
  uint32_t asr22;

  __asm__ volatile (
    "mov %%asr22, %0"
    : "=&r" (asr22)
  );

  return asr22;
}

/**
 * @brief Enables the LEON up-counter.
 */
static inline void leon3_up_counter_enable( void )
{
  __asm__ volatile (
    "mov %g0, %asr22"
  );
}

#if !defined(LEON3_HAS_ASR_22_23_UP_COUNTER)
/**
 * @brief Checks if the LEON up-counter is available.
 *
 * The LEON up-counter must have been enabled.
 *
 * @return Returns true, if the LEON up-counter is available, otherwise false.
 */
static inline bool leon3_up_counter_is_available( void )
{
  return leon3_up_counter_low() != leon3_up_counter_low();
}
#endif

/**
 * @brief Gets the LEON up-counter frequency in Hz.
 *
 * @return Returns the frequency.
 */
static inline uint32_t leon3_up_counter_frequency( void )
{
  return leon3_processor_local_bus_frequency();
}

/**
 * @brief This pointer provides the debug APBUART register block address.
 */
#if defined(LEON3_APBUART_BASE)
#define leon3_debug_uart ((struct apbuart *) LEON3_APBUART_BASE)
#else
extern apbuart *leon3_debug_uart;
#endif

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* LIBBSP_SPARC_LEON3_BSP_LEON3_H */
