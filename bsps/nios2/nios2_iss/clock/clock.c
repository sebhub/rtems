/*
 * Use SYS_CLK as system clock
 *
 * Copyright (c) 2005-2006 Kolja Waschk, rtemsdev/ixo.de
 */

#include <rtems.h>
#include <bsp.h>

#define CLOCK_REGS ((altera_avalon_timer_regs*)NIOS2_IO_BASE(CLOCK_BASE))

/*
 * Periodic interval timer interrupt handler
 */
#define Clock_driver_support_at_tick(arg) \
  do { CLOCK_REGS->status = 0; } while(0)

/*
 * Attach clock interrupt handler
 */
#define Clock_driver_support_install_isr(_new) \
  set_vector(_new, CLOCK_VECTOR, 1)

/*
 * Set up the clock hardware
 */
static void Clock_driver_support_initialize_hardware(void)
{
  uint32_t period;

  CLOCK_REGS->control = ALTERA_AVALON_TIMER_CONTROL_STOP_MSK;

  period = (CLOCK_FREQ/1000000L)*rtems_configuration_get_microseconds_per_tick() - 1;
  CLOCK_REGS->period_hi = period >> 16;
  CLOCK_REGS->period_lo = period & 0xFFFF;

  CLOCK_REGS->control = ALTERA_AVALON_TIMER_CONTROL_ITO_MSK  |
                        ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
                        ALTERA_AVALON_TIMER_CONTROL_START_MSK;

  NIOS2_IENABLE(1 << CLOCK_VECTOR);
}

#define CLOCK_DRIVER_USE_DUMMY_TIMECOUNTER

#include "../../../shared/dev/clock/clockimpl.h"

