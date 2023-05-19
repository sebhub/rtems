/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplApplConfig
 *
 * @brief This header file evaluates configuration options related to the I/O
 *   driver configuration.
 */

/*
 * Copyright (C) 2020 embedded brains GmbH & Co. KG
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

#ifndef _RTEMS_CONFDEFS_IODRIVERS_H
#define _RTEMS_CONFDEFS_IODRIVERS_H

#ifndef __CONFIGURATION_TEMPLATE_h
#error "Do not include this file directly, use <rtems/confdefs.h> instead"
#endif

#ifdef CONFIGURE_INIT

#include <rtems/confdefs/bsp.h>

#if !defined(CONFIGURE_DISABLE_BSP_SETTINGS) && \
  defined(CONFIGURE_BSP_PREREQUISITE_DRIVERS)
#define _CONFIGURE_BSP_PREREQUISITE_DRIVERS CONFIGURE_BSP_PREREQUISITE_DRIVERS
#endif

#if defined(_CONFIGURE_BSP_PREREQUISITE_DRIVERS) || \
  defined(CONFIGURE_APPLICATION_PREREQUISITE_DRIVERS) || \
  defined(CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER) || \
  defined(CONFIGURE_APPLICATION_NEEDS_RTC_DRIVER) || \
  defined(CONFIGURE_APPLICATION_NEEDS_WATCHDOG_DRIVER) || \
  defined(CONFIGURE_APPLICATION_NEEDS_STUB_DRIVER) || \
  defined(CONFIGURE_APPLICATION_NEEDS_ZERO_DRIVER) || \
  defined(CONFIGURE_APPLICATION_NEEDS_IDE_DRIVER) || \
  defined(CONFIGURE_APPLICATION_NEEDS_ATA_DRIVER) || \
  defined(CONFIGURE_APPLICATION_NEEDS_FRAME_BUFFER_DRIVER) || \
  defined(CONFIGURE_APPLICATION_EXTRA_DRIVERS)
#define _CONFIGURE_HAS_IO_DRIVERS
#endif

#if defined(_CONFIGURE_HAS_IO_DRIVERS) || \
  defined(CONFIGURE_APPLICATION_NEEDS_NULL_DRIVER) || \
  CONFIGURE_MAXIMUM_DRIVERS > 0

#include <rtems/ioimpl.h>
#include <rtems/sysinit.h>

#ifdef CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
  #if defined(CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER) \
    || defined(CONFIGURE_APPLICATION_NEEDS_SIMPLE_TASK_CONSOLE_DRIVER)
    #error "CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER, CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER, and CONFIGURE_APPLICATION_NEEDS_SIMPLE_TASK_CONSOLE_DRIVER are mutually exclusive"
  #endif

  #include <rtems/console.h>
#endif

#ifdef CONFIGURE_APPLICATION_NEEDS_RTC_DRIVER
  #include <rtems/rtc.h>
#endif

#ifdef CONFIGURE_APPLICATION_NEEDS_WATCHDOG_DRIVER
  #include <rtems/watchdogdrv.h>
#endif

#ifdef CONFIGURE_APPLICATION_NEEDS_FRAME_BUFFER_DRIVER
  #include <rtems/framebuffer.h>
#endif

#ifdef CONFIGURE_APPLICATION_NEEDS_STUB_DRIVER
  #include <rtems/devnull.h>
#endif

#ifdef CONFIGURE_APPLICATION_NEEDS_ZERO_DRIVER
  #include <rtems/devzero.h>
#endif

#ifdef CONFIGURE_APPLICATION_NEEDS_IDE_DRIVER
  #ifndef CONFIGURE_APPLICATION_NEEDS_ATA_DRIVER
    #define CONFIGURE_APPLICATION_NEEDS_ATA_DRIVER
  #endif

  #include <libchip/ide_ctrl.h>
#endif

#ifdef CONFIGURE_APPLICATION_NEEDS_ATA_DRIVER
  #include <libchip/ata.h>
#endif

#ifndef CONFIGURE_MAXIMUM_DRIVERS
  #define CONFIGURE_MAXIMUM_DRIVERS
#endif

#define NULL_DRIVER_TABLE_ENTRY { NULL, NULL, NULL, NULL, NULL, NULL }

#ifdef __cplusplus
extern "C" {
#endif

rtems_driver_address_table
_IO_Driver_address_table[ CONFIGURE_MAXIMUM_DRIVERS ] = {
  #ifdef _CONFIGURE_BSP_PREREQUISITE_DRIVERS
    _CONFIGURE_BSP_PREREQUISITE_DRIVERS,
  #endif
  #ifdef CONFIGURE_APPLICATION_PREREQUISITE_DRIVERS
    CONFIGURE_APPLICATION_PREREQUISITE_DRIVERS,
  #endif
  #ifdef CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
    CONSOLE_DRIVER_TABLE_ENTRY,
  #endif
  #ifdef CONFIGURE_APPLICATION_NEEDS_RTC_DRIVER
    RTC_DRIVER_TABLE_ENTRY,
  #endif
  #ifdef CONFIGURE_APPLICATION_NEEDS_WATCHDOG_DRIVER
    WATCHDOG_DRIVER_TABLE_ENTRY,
  #endif
  #ifdef CONFIGURE_APPLICATION_NEEDS_STUB_DRIVER
    DEVNULL_DRIVER_TABLE_ENTRY,
  #endif
  #ifdef CONFIGURE_APPLICATION_NEEDS_ZERO_DRIVER
    DEVZERO_DRIVER_TABLE_ENTRY,
  #endif
  #ifdef CONFIGURE_APPLICATION_NEEDS_IDE_DRIVER
    IDE_CONTROLLER_DRIVER_TABLE_ENTRY,
  #endif
  #ifdef CONFIGURE_APPLICATION_NEEDS_ATA_DRIVER
    ATA_DRIVER_TABLE_ENTRY,
  #endif
  #ifdef CONFIGURE_APPLICATION_NEEDS_FRAME_BUFFER_DRIVER
    FRAME_BUFFER_DRIVER_TABLE_ENTRY,
  #endif
  #ifdef CONFIGURE_APPLICATION_EXTRA_DRIVERS
    CONFIGURE_APPLICATION_EXTRA_DRIVERS,
  #endif
  #if defined(CONFIGURE_APPLICATION_NEEDS_NULL_DRIVER) || \
    !defined(_CONFIGURE_HAS_IO_DRIVERS)
    NULL_DRIVER_TABLE_ENTRY
  #endif
};

const size_t _IO_Number_of_drivers =
  RTEMS_ARRAY_SIZE( _IO_Driver_address_table );

RTEMS_SYSINIT_ITEM(
  _IO_Initialize_all_drivers,
  RTEMS_SYSINIT_DEVICE_DRIVERS,
  RTEMS_SYSINIT_ORDER_MIDDLE
);

#ifdef CONFIGURE_APPLICATION_NEEDS_ATA_DRIVER
  #ifndef CONFIGURE_ATA_DRIVER_TASK_PRIORITY
    #define CONFIGURE_ATA_DRIVER_TASK_PRIORITY ATA_DRIVER_TASK_DEFAULT_PRIORITY
  #endif

  rtems_task_priority rtems_ata_driver_task_priority
    = CONFIGURE_ATA_DRIVER_TASK_PRIORITY;
#endif

#ifdef __cplusplus
}
#endif

#endif /* _CONFIGURE_HAS_IO_DRIVERS) ||
  CONFIGURE_APPLICATION_NEEDS_NULL_DRIVER ||
  CONFIGURE_MAXIMUM_DRIVERS */


/*
 * If any flavor of console driver is configured, then configure the post
 * driver hook which opens /dev/console as stdin, stdout, and stderr.
 *
 * NOTE: This also results in an atexit() handler being registered to close
 *       /dev/console.
 */
#if defined(CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER) || \
    defined(CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER) || \
    defined(CONFIGURE_APPLICATION_NEEDS_SIMPLE_TASK_CONSOLE_DRIVER)

  #include <rtems/libio.h>

  RTEMS_SYSINIT_ITEM(
    rtems_libio_post_driver,
    RTEMS_SYSINIT_STD_FILE_DESCRIPTORS,
    RTEMS_SYSINIT_ORDER_MIDDLE
  );
#endif

#endif /* CONFIGURE_INIT */

#endif /* _RTEMS_CONFDEFS_IODRIVERS_H */
