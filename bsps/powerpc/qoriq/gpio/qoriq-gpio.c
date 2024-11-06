/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCQorIQGPIO
 *
 * @brief This source file contains the implementation of
 *   @ref RTEMSBSPsPowerPCQorIQGPIO
 */

/*
 * Copyright (C) 2024, 2025 embedded brains GmbH & Co. KG
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

#include <bsp/qoriq-gpio.h>

#include <rtems/sysinit.h>

#if !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT)

void qoriq_gpio_set_direction(
  const qoriq_gpio_pin *pin,
  qoriq_gpio_direction  direction
)
{
  rtems_interrupt_lock_context lock_context;
  qoriq_gpio_control          *ctrl;
  volatile qoriq_gpio         *gpio;
  uint32_t                     gpdir;

  ctrl = pin->ctrl;
  gpio = ctrl->gpio;
  rtems_interrupt_lock_acquire( &ctrl->lock, &lock_context );
  gpdir = gpio->gpdir;

  if ( direction == QORIQ_GPIO_IN ) {
    gpdir &= ~pin->mask;
  } else {
    gpdir |= pin->mask;
  }

  gpio->gpdir = gpdir;
  rtems_interrupt_lock_release( &ctrl->lock, &lock_context );
}

void qoriq_gpio_set_drive(
  const qoriq_gpio_pin *pin,
  qoriq_gpio_drive      drive
)
{
  rtems_interrupt_lock_context lock_context;
  qoriq_gpio_control          *ctrl;
  volatile qoriq_gpio         *gpio;
  uint32_t                     gpodr;

  ctrl = pin->ctrl;
  gpio = ctrl->gpio;
  rtems_interrupt_lock_acquire( &ctrl->lock, &lock_context );
  gpodr = gpio->gpodr;

  if ( drive == QORIQ_GPIO_PUSHPULL ) {
    gpodr &= ~pin->mask;
  } else {
    gpodr |= pin->mask;
  }

  gpio->gpodr = gpodr;
  rtems_interrupt_lock_release( &ctrl->lock, &lock_context );
}

qoriq_gpio_level qoriq_gpio_read( const qoriq_gpio_pin *pin )
{
  uint32_t gpdat;

  gpdat = pin->ctrl->gpio->gpdat & pin->mask;

  if ( gpdat == 0 ) {
    return QORIQ_GPIO_LOW;
  }

  return QORIQ_GPIO_HIGH;
}

void qoriq_gpio_write( const qoriq_gpio_pin *pin, qoriq_gpio_level level )
{
  rtems_interrupt_lock_context lock_context;
  qoriq_gpio_control          *ctrl;
  volatile qoriq_gpio         *gpio;
  uint32_t                     gpdat;

  ctrl = pin->ctrl;
  gpio = ctrl->gpio;
  rtems_interrupt_lock_acquire( &ctrl->lock, &lock_context );
  gpdat = gpio->gpdat;

  if (level == QORIQ_GPIO_LOW) {
    gpdat &= ~pin->mask;
  } else {
    gpdat |= pin->mask;
  }

  gpio->gpdat = gpdat;
  rtems_interrupt_lock_release( &ctrl->lock, &lock_context );
}

qoriq_gpio_control qoriq_gpio_controller[ 1 ];

static void qoriq_gpio_init_ctrl(
  qoriq_gpio_control  *ctrl,
  volatile qoriq_gpio *gpio
)
{
  ctrl->gpio = gpio;
  rtems_interrupt_lock_initialize( &ctrl->lock, "qoriq" );
}

static void qoriq_gpio_init( void )
{
  qoriq_gpio_init_ctrl( &qoriq_gpio_controller[ 0 ], &qoriq.gpio );
}

RTEMS_SYSINIT_ITEM(
  qoriq_gpio_init,
  RTEMS_SYSINIT_DEVICE_DRIVERS,
  RTEMS_SYSINIT_ORDER_FIRST
);

#endif /* !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT) */
