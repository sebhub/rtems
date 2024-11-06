/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCQorIQGPIO
 *
 * @brief This header file provides the interfaces of
 *   @ref RTEMSBSPsPowerPCQorIQGPIO.
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

#ifndef LIBBSP_POWERPC_QORIQ_GPIO_H
#define LIBBSP_POWERPC_QORIQ_GPIO_H

#include <rtems.h>

#include <bsp/qoriq.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup RTEMSBSPsPowerPCQorIQGPIO NXP QorIQ GPIO
 *
 * @ingroup RTEMSBSPsPowerPCQorIQ
 *
 * @brief This group contains the GPIO support of the NXP QorIQ BSP.
 *
 * Please have a look at the following example:
 *
 * @code
 * const qoriq_gpio_pin pin = {
 *   .ctrl = &qoriq_gpio_controller[ 0 ],
 *   .mask = QORIQ_GPIO_PIN( 5 )
 * };
 *
 * void configure_pin( void )
 * {
 *   qoriq_gpio_set_direction( &pin, QORIQ_GPIO_OUT );
 *   qoriq_gpio_set_drive( &pin, QORIQ_GPIO_PUSHPULL );
 *   qoriq_gpio_level( &pin, QORIQ_GPIO_HIGH );
 * }
 * @endcode
 *
 * @{
 */

/**
 * @brief Creates the bit mask associated with the GPIO pin specified by
 *   ``number``.
 */
#define QORIQ_GPIO_PIN( number) ( 1U << ( 31 - ( number) ) )

/**
 * @brief This structure represents a GPIO controller.
 *
 * Exactly one instance of this structure shall exist in the system for each
 * GPIO controller hardware instance.
 */
typedef struct qoriq_gpio_controller {
  /**
   * @brief This member references the associated GPIO registers.
   */
  volatile qoriq_gpio *gpio;

#if RTEMS_INTERRUPT_LOCK_NEEDS_OBJECT
  /**
   * @brief This member provides a lock.
   */
  rtems_interrupt_lock lock;
#endif
} qoriq_gpio_control;

/**
 * @brief These objects are provided to control the GPIO pins.
 */
extern qoriq_gpio_control qoriq_gpio_controller[];

/**
 * @brief This structure represents a single GPIO pin.
 */
typedef struct qoriq_gpio_pin {
  /**
   * @brief This member references the GPIO controller that controls the pin.
   */
  qoriq_gpio_control *ctrl;

  /**
   * @brief This member contains the pin mask associated with the pin.
   *
   * @see QORIQ_GPIO_PIN()
   */
  uint32_t mask;
} qoriq_gpio_pin;

/**
 * @brief This enumeration defines the direction of a GPIO pin.
 */
typedef enum qoriq_gpio_direction {
  /**
   * @brief Indicates that the GPIO pin is an input.
   */
  QORIQ_GPIO_IN,

  /**
   * @brief Indicates that the GPIO pin is an output.
   */
  QORIQ_GPIO_OUT
} qoriq_gpio_direction;

/**
 * @brief This enumeration defines the driver property of a GPIO pin.
 */
typedef enum qoriq_gpio_drive {
  /**
   * @brief Indicates that the GPIO pin use a push-pull drive.
   */
  QORIQ_GPIO_PUSHPULL,

  /**
   * @brief Indicates that the GPIO pin use an open-drain drive.
   */
  QORIQ_GPIO_OPENDRAIN
} qoriq_gpio_drive;

/**
 * @brief This enumeration defines the level of a GPIO pin.
 */
typedef enum qoriq_gpio_level {
  /**
   * @brief Indicates that the GPIO pin level is low.
   */
  QORIQ_GPIO_LOW,

  /**
   * @brief Indicates that the GPIO pin level is high.
   */
  QORIQ_GPIO_HIGH
} qoriq_gpio_level;

/**
 * @brief Set the direction of the GPIO pin.
 *
 * @param pin is the GPIO pin.
 *
 * @param direction is the new GPIO pin direction.
 */
void qoriq_gpio_set_direction(
  const qoriq_gpio_pin *pin,
  qoriq_gpio_direction  direction
);

/**
 * @brief Set the drive of the GPIO pin.
 *
 * @par Notes
 * The pin drive is only relevant for pins that are set to output.
 *
 * @param pin is the GPIO pin.
 *
 * @param direction is the new GPIO pin drive.
 */
void qoriq_gpio_set_drive(
  const qoriq_gpio_pin *pin,
  qoriq_gpio_drive      drive
);

/**
 * @brief Reads the level of the GPIO pin.
 *
 * @param pin is the GPIO pin.
 *
 * @retval QORIQ_GPIO_LOW The GPIO pin level was low at some time point during
 *   the call.
 *
 * @retval QORIQ_GPIO_HIGH The GPIO pin level was high at some time point
 *   during the call.
 */
qoriq_gpio_level qoriq_gpio_read( const qoriq_gpio_pin *pin );

/**
 * @brief Set the level of the GPIO pin.
 *
 * @par Notes
 * The physical level of the pin will only change if it is set to output
 * direction.
 *
 * @param pin is the GPIO pin.
 *
 * @param level is the new GPIO pin level.
 */
void qoriq_gpio_write( const qoriq_gpio_pin *pin, qoriq_gpio_level level );

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBBSP_POWERPC_QORIQ_GPIO_H */
