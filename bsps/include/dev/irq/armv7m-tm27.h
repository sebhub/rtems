/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSTimeTest27Support
 *
 * @brief Time Test 27 support for the ARMv7-M NVIC.
 *
 * The NVIC has no equivalent of the software generated interrupts of the
 * ARM GIC, so a BSP which uses this file has to nominate two interrupt
 * vectors which no peripheral of the device drives.
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

#ifndef _RTEMS_TMTEST27
#error "This is an RTEMS internal file you must not include directly."
#endif

#ifndef LIBBSP_SHARED_ARMV7M_TM27_H
#define LIBBSP_SHARED_ARMV7M_TM27_H

#include <bsp.h>
#include <bsp/irq.h>

#include <rtems/score/assert.h>

#if !defined(ARMV7M_TM27_IRQ_LOW) || !defined(ARMV7M_TM27_IRQ_HIGH)
#error "the BSP must define ARMV7M_TM27_IRQ_LOW and ARMV7M_TM27_IRQ_HIGH"
#endif

#define MUST_WAIT_FOR_INTERRUPT 1

#define TM27_INTERRUPT_VECTOR_DEFAULT ARMV7M_TM27_IRQ_LOW

#define TM27_INTERRUPT_VECTOR_ALTERNATIVE ARMV7M_TM27_IRQ_HIGH

/*
 * Only the most significant bits of the priority are implemented, so keep the
 * two priorities well apart to make sure the high priority interrupt really
 * does preempt the low priority one.
 *
 * Both must stay at or above ARM_INTERRUPT_DISABLE_BASEPRI.  The handler uses
 * operating system services, so it must not be able to run inside a critical
 * section established by rtems_interrupt_local_disable().
 */
#define ARMV7M_TM27_PRIO_LOW 0xc0

#define ARMV7M_TM27_PRIO_HIGH 0x90

RTEMS_STATIC_ASSERT(
  ARMV7M_TM27_PRIO_HIGH >= ARM_INTERRUPT_DISABLE_BASEPRI
    && ARMV7M_TM27_PRIO_HIGH < ARMV7M_TM27_PRIO_LOW,
  ARMV7M_TM27_PRIO
);

static inline void Install_tm27_vector( rtems_interrupt_handler handler )
{
  static rtems_interrupt_entry entry_low;
  static rtems_interrupt_entry entry_high;
  rtems_status_code sc;

  rtems_interrupt_entry_initialize(
    &entry_low,
    handler,
    NULL,
    "tm27 low"
  );
  sc = rtems_interrupt_entry_install(
    ARMV7M_TM27_IRQ_LOW,
    RTEMS_INTERRUPT_SHARED,
    &entry_low
  );
  _Assert_Unused_variable_equals( sc, RTEMS_SUCCESSFUL );

  sc = rtems_interrupt_set_priority(
    ARMV7M_TM27_IRQ_LOW,
    ARMV7M_TM27_PRIO_LOW
  );
  _Assert_Unused_variable_equals( sc, RTEMS_SUCCESSFUL );

  rtems_interrupt_entry_initialize(
    &entry_high,
    handler,
    NULL,
    "tm27 high"
  );
  sc = rtems_interrupt_entry_install(
    ARMV7M_TM27_IRQ_HIGH,
    RTEMS_INTERRUPT_SHARED,
    &entry_high
  );
  _Assert_Unused_variable_equals( sc, RTEMS_SUCCESSFUL );

  sc = rtems_interrupt_set_priority(
    ARMV7M_TM27_IRQ_HIGH,
    ARMV7M_TM27_PRIO_HIGH
  );
  _Assert_Unused_variable_equals( sc, RTEMS_SUCCESSFUL );
}

static inline void Cause_tm27_intr( void )
{
  rtems_status_code sc;

  sc = rtems_interrupt_raise( ARMV7M_TM27_IRQ_LOW );
  _Assert_Unused_variable_equals( sc, RTEMS_SUCCESSFUL );
}

static inline void Clear_tm27_intr( void )
{
  rtems_status_code sc;

  sc = rtems_interrupt_clear( ARMV7M_TM27_IRQ_LOW );
  _Assert_Unused_variable_equals( sc, RTEMS_SUCCESSFUL );
}

static inline void Lower_tm27_intr( void )
{
  /* The interrupt is cleared by the acknowledge of the processor. */
}

/*
 * The NVIC needs nothing special to raise the alternative vector, unlike
 * interrupt controllers where it is wired to a dedicated trigger register.
 */
static inline rtems_status_code _TM27_Raise_alternative( void )
{
  return rtems_interrupt_raise( ARMV7M_TM27_IRQ_HIGH );
}

static inline rtems_status_code _TM27_Clear_alternative( void )
{
  return rtems_interrupt_clear( ARMV7M_TM27_IRQ_HIGH );
}

#endif /* LIBBSP_SHARED_ARMV7M_TM27_H */
