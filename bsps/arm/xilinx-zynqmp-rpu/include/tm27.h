/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsARMZynqMPRPU
 *
 * @brief This header file provides a Time Test 27 support implementation.
 */

/*
 * Copyright (C) 2013, 2024 embedded brains GmbH & Co. KG
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

#ifndef __tm27_h
#define __tm27_h

#define TM27_INTERRUPT_VECTOR_FAST ARM_GIC_IRQ_SGI_14

#include <dev/irq/arm-gic-tm27.h>

typedef uint32_t TM27_Trigger_fast_register;

static inline void _TM27_Prepare_fast(
  TM27_Trigger_fast_register volatile * volatile *trigger_address,
  TM27_Trigger_fast_register volatile *trigger_value
)
{
  rtems_vector_number vector;
  volatile gic_dist *dist;
  volatile gic_cpuif *cpuif;

  vector = TM27_INTERRUPT_VECTOR_FAST;
  cpuif = (volatile gic_cpuif *) BSP_ARM_GIC_CPUIF_BASE;
  cpuif->iccicr |= GIC_CPUIF_ICCICR_FIQ_EN;
  _ARM_Data_synchronization_barrier();

  dist = (volatile gic_dist *) BSP_ARM_GIC_DIST_BASE;
  *trigger_address = &dist->icdsgir;
  *trigger_value = GIC_DIST_ICDSGIR_TARGET_LIST_FILTER( 0 )
    | GIC_DIST_ICDSGIR_CPU_TARGET_LIST( 1U << _SMP_Get_current_processor() )
    | GIC_DIST_ICDSGIR_SGIINTID( vector );
}

static inline void _TM27_Clear_fast( void )
{
  volatile gic_cpuif *cpuif;

  cpuif = (volatile gic_cpuif *) BSP_ARM_GIC_CPUIF_BASE;
  cpuif->icceoir = cpuif->icciar;
  cpuif->iccicr &= ~GIC_CPUIF_ICCICR_FIQ_EN;
}

#endif /* __tm27_h */
