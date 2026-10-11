/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspRiscvValPlicAffinity
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

#include <rtems.h>
#include <bsp/irq.h>
#include <rtems/score/percpu.h>

#include <rtems/test.h>

/**
 * @defgroup BspRiscvValPlicAffinity spec:/bsp/riscv/val/plic-affinity
 *
 * @ingroup TestsuitesValidationIntr
 *
 * @brief Tests the routing of an external interrupt to all processors through
 *   the PLIC.
 *
 * This test case performs the following actions:
 *
 * - Set the affinity of the external interrupt vector to all processors.
 *
 *   - Check that the directive call was successful.
 *
 * - Enable the external interrupt vector.
 *
 *   - Check that the directive call was successful.
 *
 *   - Check that the interrupt source is enabled in the PLIC context of each
 *     processor which has a PLIC context.
 *
 * - Disable the external interrupt vector.
 *
 *   - Check that the directive call was successful.
 *
 *   - Check that the interrupt source is disabled in the PLIC context of each
 *     processor which has a PLIC context.
 *
 * - Set the affinity of the external interrupt vector to the first processor.
 *
 *   - Check that the directive call was successful.
 *
 * @{
 */

/*
 * The interrupt line 63 of the GR765 serves a CAN FD controller.  The
 * simulator models no CAN FD controller, so the line stays inactive.
 */
#define INDEX 63

#define VECTOR RISCV_INTERRUPT_VECTOR_EXTERNAL( INDEX )

static uint32_t CountContexts( void )
{
  uint32_t count;
  uint32_t cpu_index;

  count = 0;

  for (
    cpu_index = 0; cpu_index < _SMP_Get_processor_maximum(); ++cpu_index
  ) {
    if ( _Per_CPU_Get_by_index( cpu_index )->cpu_per_cpu.plic_m_ie != NULL ) {
      ++count;
    }
  }

  return count;
}

static uint32_t CountEnabled( void )
{
  uint32_t count;
  uint32_t cpu_index;

  count = 0;

  for (
    cpu_index = 0; cpu_index < _SMP_Get_processor_maximum(); ++cpu_index
  ) {
    volatile uint32_t *enable;

    enable = _Per_CPU_Get_by_index( cpu_index )->cpu_per_cpu.plic_m_ie;

    if (
      enable != NULL &&
      ( enable[ INDEX / 32 ] & ( UINT32_C( 1 ) << ( INDEX % 32 ) ) ) != 0
    ) {
      ++count;
    }
  }

  return count;
}

/**
 * @brief Set the affinity of the external interrupt vector to all processors.
 */
static void BspRiscvValPlicAffinity_Action_0( void )
{
  rtems_status_code sc;
  cpu_set_t         set;

  CPU_FILL( &set );
  sc = rtems_interrupt_set_affinity( VECTOR, sizeof( set ), &set );

  /*
   * Check that the directive call was successful.
   */
  T_step_rsc_success( 0, sc );
}

/**
 * @brief Enable the external interrupt vector.
 */
static void BspRiscvValPlicAffinity_Action_1( void )
{
  rtems_status_code sc;

  sc = rtems_interrupt_vector_enable( VECTOR );

  /*
   * Check that the directive call was successful.
   */
  T_step_rsc_success( 1, sc );

  /*
   * Check that the interrupt source is enabled in the PLIC context of each
   * processor which has a PLIC context.
   */
  T_step_eq_u32( 2, CountEnabled(), CountContexts() );
}

/**
 * @brief Disable the external interrupt vector.
 */
static void BspRiscvValPlicAffinity_Action_2( void )
{
  rtems_status_code sc;

  sc = rtems_interrupt_vector_disable( VECTOR );

  /*
   * Check that the directive call was successful.
   */
  T_step_rsc_success( 3, sc );

  /*
   * Check that the interrupt source is disabled in the PLIC context of each
   * processor which has a PLIC context.
   */
  T_step_eq_u32( 4, CountEnabled(), 0 );
}

/**
 * @brief Set the affinity of the external interrupt vector to the first
 *   processor.
 */
static void BspRiscvValPlicAffinity_Action_3( void )
{
  rtems_status_code sc;
  cpu_set_t         set;

  CPU_ZERO( &set );
  CPU_SET( 0, &set );
  sc = rtems_interrupt_set_affinity( VECTOR, sizeof( set ), &set );

  /*
   * Check that the directive call was successful.
   */
  T_step_rsc_success( 5, sc );
}

/**
 * @fn void T_case_body_BspRiscvValPlicAffinity( void )
 */
T_TEST_CASE( BspRiscvValPlicAffinity )
{
  T_plan( 6 );

  BspRiscvValPlicAffinity_Action_0();
  BspRiscvValPlicAffinity_Action_1();
  BspRiscvValPlicAffinity_Action_2();
  BspRiscvValPlicAffinity_Action_3();
}

/** @} */
