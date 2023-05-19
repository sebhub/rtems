/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplClassicIO
 *
 * @brief This source file contains the implementation of
 *   rtems_io_register_driver().
 */

/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Copyright (c) 2009 embedded brains GmbH & Co. KG
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

#include <rtems/ioimpl.h>
#include <rtems/rtems/intr.h>

ISR_LOCK_DEFINE( , _IO_Driver_registration_lock, "IO Driver Registration" )

static inline bool rtems_io_is_empty_table(
  const rtems_driver_address_table *table
)
{
  return table->initialization_entry == NULL && table->open_entry == NULL;
}

static rtems_status_code rtems_io_obtain_major_number(
  rtems_device_major_number *major
)
{
  rtems_device_major_number n = _IO_Number_of_drivers;
  rtems_device_major_number m = 0;

  /* major is error checked by caller */

  for ( m = 0; m < n; ++m ) {
    rtems_driver_address_table *const table = _IO_Driver_address_table + m;

    if ( rtems_io_is_empty_table( table ) )
      break;
  }

  /* Assigns invalid value in case of failure */
  *major = m;

  if ( m != n )
    return RTEMS_SUCCESSFUL;

  return RTEMS_TOO_MANY;
}

rtems_status_code rtems_io_register_driver(
  rtems_device_major_number         major,
  const rtems_driver_address_table *driver_table,
  rtems_device_major_number        *registered_major
)
{
  rtems_device_major_number major_limit = _IO_Number_of_drivers;
  ISR_lock_Context lock_context;

  if ( rtems_interrupt_is_in_progress() )
    return RTEMS_CALLED_FROM_ISR;

  if ( registered_major == NULL )
    return RTEMS_INVALID_ADDRESS;

  /* Set it to an invalid value */
  *registered_major = major_limit;

  if ( driver_table == NULL )
    return RTEMS_INVALID_ADDRESS;

  if ( rtems_io_is_empty_table( driver_table ) )
    return RTEMS_INVALID_ADDRESS;

  if ( major >= major_limit )
    return RTEMS_INVALID_NUMBER;

  _IO_Driver_registration_acquire( &lock_context );

  if ( major == 0 ) {
    rtems_status_code sc = rtems_io_obtain_major_number( registered_major );

    if ( sc != RTEMS_SUCCESSFUL ) {
      _IO_Driver_registration_release( &lock_context );
      return sc;
    }
    major = *registered_major;
  } else {
    rtems_driver_address_table *const table = _IO_Driver_address_table + major;

    if ( !rtems_io_is_empty_table( table ) ) {
      _IO_Driver_registration_release( &lock_context );
      return RTEMS_RESOURCE_IN_USE;
    }

    *registered_major = major;
  }

  _IO_Driver_address_table [major] = *driver_table;

  _IO_Driver_registration_release( &lock_context );

  if ( _IO_All_drivers_initialized ) {
    /* Other drivers have already been initialized, we initialize
     * the driver directly.
     */
    return rtems_io_initialize( major, 0, NULL );
  } else {
    /* The driver will be initialized together with all other drivers
     * in a later stage by _IO_Initialize_all_drivers().
     */
    return RTEMS_SUCCESSFUL;
  }
}
