/**
 * @file
 *
 * @ingroup riscv_interrupt
 *
 * @brief Interrupt support.
 */

/*
 * Copyright (c) 2018 embedded brains GmbH & Co. KG
 *
 * Copyright (c) 2015 University of York.
 * Hesham Almatary <hesham@alumni.york.ac.uk>
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <bsp/fatal.h>
#include <bsp/irq-generic.h>
#include <amba.h>

#include <rtems/score/percpu.h>
#include <rtems/score/riscv-utility.h>
#include <rtems/score/smpimpl.h>

#if defined(RTEMS_SMP)
/* The affinity of each interrupt line which the next enable applies */
static Processor_mask griscv_interrupt_affinities[BSP_INTERRUPT_VECTOR_COUNT];
#endif

void _RISCV_Interrupt_dispatch(uintptr_t mcause, Per_CPU_Control *cpu_self)
{
  (void) cpu_self;

  if (mcause & 0x80000000) {

    bsp_interrupt_handler_dispatch(mcause & 0xf);

  } else {
    bsp_fatal(RISCV_FATAL_UNEXPECTED_INTERRUPT_EXCEPTION);
  }
}

void bsp_interrupt_facility_initialize(void)
{
#if defined(RTEMS_SMP)
  Processor_mask affinity;
  size_t i;

  _Processor_mask_From_index(&affinity, rtems_scheduler_get_processor());

  for (i = 0; i < RTEMS_ARRAY_SIZE(griscv_interrupt_affinities); ++i) {
    griscv_interrupt_affinities[i] = affinity;
  }
#endif

  /*
   * External M-mode interrupts on secondary processors are enabled in
   * bsp_start_on_secondary_processor().
   */
  set_csr(mie, MIP_MEIP);
}

bool bsp_interrupt_is_valid_vector(rtems_vector_number vector)
{
  return vector != 0 && vector < BSP_INTERRUPT_VECTOR_COUNT;
}

static inline uint32_t bsp_irq_current_cpu(void)
{
  return read_csr(mhartid);
}

rtems_status_code bsp_interrupt_get_attributes(
  rtems_vector_number         vector,
  rtems_interrupt_attributes *attributes
)
{
  (void) vector;

  attributes->is_maskable = true;
  attributes->can_enable = true;
  attributes->maybe_enable = true;
  attributes->can_disable = true;
  attributes->maybe_disable = true;
  attributes->can_raise = true;
  attributes->can_raise_on = true;
  attributes->can_clear = true;
  attributes->cleared_by_acknowledge = true;
  attributes->can_get_affinity = true;
  attributes->can_set_affinity = true;
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_is_pending(
  rtems_vector_number vector,
  bool               *pending
)
{
  rtems_interrupt_level level;
  uint32_t bit;

  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  bsp_interrupt_assert(pending != NULL);

  bit = 1U << vector;
  rtems_interrupt_local_disable(level);
  *pending = (GRLIB_IrqCtrl_Regs->ipend & bit) != 0 ||
    (GRLIB_IrqCtrl_Regs->piforce[bsp_irq_current_cpu()] & bit) != 0;
  rtems_interrupt_local_enable(level);
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_raise(rtems_vector_number vector)
{
  uint32_t bit;
#if defined(RTEMS_SMP)
  uint32_t cpu_count;
  uint32_t cpu_index;
#endif

  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));

  bit = 1U << vector;
#if defined(RTEMS_SMP)
  cpu_count = rtems_scheduler_get_processor_maximum();

  for (cpu_index = 0; cpu_index < cpu_count; ++cpu_index) {
    GRLIB_IrqCtrl_Regs->piforce[cpu_index] = bit;
  }
#else
  GRLIB_IrqCtrl_Regs->piforce[bsp_irq_current_cpu()] = bit;
#endif

  return RTEMS_SUCCESSFUL;
}

#if defined(RTEMS_SMP)
rtems_status_code bsp_interrupt_raise_on(
  rtems_vector_number vector,
  uint32_t            cpu_index
)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  bsp_interrupt_assert(cpu_index < rtems_scheduler_get_processor_maximum());

  GRLIB_IrqCtrl_Regs->piforce[cpu_index] = 1U << vector;
  return RTEMS_SUCCESSFUL;
}
#endif

rtems_status_code bsp_interrupt_clear(rtems_vector_number vector)
{
  uint32_t bit;

  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));

  bit = 1U << vector;
  GRLIB_IrqCtrl_Regs->iclear = bit;
  GRLIB_IrqCtrl_Regs->piforce[bsp_irq_current_cpu()] = bit << 16;
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_vector_is_enabled(
  rtems_vector_number vector,
  bool               *enabled
)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  bsp_interrupt_assert(enabled != NULL);

  *enabled = !BSP_Cpu_Is_interrupt_masked(vector, bsp_irq_current_cpu());
  return RTEMS_SUCCESSFUL;
}

#if defined(RTEMS_SMP)
static void griscv_interrupt_vector_enable(rtems_vector_number vector)
{
  const Processor_mask *affinity;
  uint32_t cpu_count;
  uint32_t cpu_index;
  uint32_t bit;
  uint32_t unmasked;

  affinity = &griscv_interrupt_affinities[vector];
  cpu_count = rtems_scheduler_get_processor_maximum();
  bit = 1U << vector;
  unmasked = 0;

  for (cpu_index = 0; cpu_index < cpu_count; ++cpu_index) {
    if (_Processor_mask_Is_set(affinity, cpu_index)) {
      GRLIB_IrqCtrl_Regs->pimask[cpu_index] |= bit;
      ++unmasked;
    } else {
      GRLIB_IrqCtrl_Regs->pimask[cpu_index] &= ~bit;
    }
  }

  if (unmasked > 1) {
    GRLIB_IrqCtrl_Regs->brdcst |= bit;
  } else {
    GRLIB_IrqCtrl_Regs->brdcst &= ~bit;
  }
}
#endif

rtems_status_code bsp_interrupt_vector_enable(rtems_vector_number vector)
{
  rtems_interrupt_lock_context lock_context;

  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));

  GRLIB_IRQCTRL_ACQUIRE(&lock_context);
#if defined(RTEMS_SMP)
  griscv_interrupt_vector_enable(vector);
#else
  GRLIB_IrqCtrl_Regs->pimask[bsp_irq_current_cpu()] |= 1U << vector;
#endif
  GRLIB_IRQCTRL_RELEASE(&lock_context);
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_vector_disable(rtems_vector_number vector)
{
  rtems_interrupt_lock_context lock_context;
  uint32_t bit;
#if defined(RTEMS_SMP)
  uint32_t cpu_count;
  uint32_t cpu_index;
#endif

  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));

  bit = 1U << vector;
  GRLIB_IRQCTRL_ACQUIRE(&lock_context);
#if defined(RTEMS_SMP)
  cpu_count = rtems_scheduler_get_processor_maximum();

  for (cpu_index = 0; cpu_index < cpu_count; ++cpu_index) {
    GRLIB_IrqCtrl_Regs->pimask[cpu_index] &= ~bit;
  }

  GRLIB_IrqCtrl_Regs->brdcst &= ~bit;
#else
  GRLIB_IrqCtrl_Regs->pimask[bsp_irq_current_cpu()] &= ~bit;
#endif
  GRLIB_IRQCTRL_RELEASE(&lock_context);
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_set_priority(
  rtems_vector_number vector,
  uint32_t priority
)
{
  (void) vector;
  (void) priority;

  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  return RTEMS_UNSATISFIED;
}

rtems_status_code bsp_interrupt_get_priority(
  rtems_vector_number vector,
  uint32_t *priority
)
{
  (void) vector;
  (void) priority;

  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  bsp_interrupt_assert(priority != NULL);
  return RTEMS_UNSATISFIED;
}

#if defined(RTEMS_SMP)
rtems_status_code bsp_interrupt_get_affinity(
  rtems_vector_number vector,
  Processor_mask *affinity
)
{
  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  bsp_interrupt_assert(affinity != NULL);

  *affinity = griscv_interrupt_affinities[vector];
  return RTEMS_SUCCESSFUL;
}

rtems_status_code bsp_interrupt_set_affinity(
  rtems_vector_number vector,
  const Processor_mask *affinity
)
{
  rtems_interrupt_lock_context lock_context;
  uint32_t cpu_count;
  uint32_t cpu_index;
  uint32_t bit;

  bsp_interrupt_assert(bsp_interrupt_is_valid_vector(vector));
  bsp_interrupt_assert(affinity != NULL);

  cpu_count = rtems_scheduler_get_processor_maximum();
  bit = 1U << vector;

  GRLIB_IRQCTRL_ACQUIRE(&lock_context);
  griscv_interrupt_affinities[vector] = *affinity;

  /*
   * If the interrupt is enabled on at least one processor, then enable it
   * again with the new affinity.
   */
  for (cpu_index = 0; cpu_index < cpu_count; ++cpu_index) {
    if ((GRLIB_IrqCtrl_Regs->pimask[cpu_index] & bit) != 0) {
      griscv_interrupt_vector_enable(vector);
      break;
    }
  }

  GRLIB_IRQCTRL_RELEASE(&lock_context);
  return RTEMS_SUCCESSFUL;
}
#endif
