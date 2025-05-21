/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsIoValPrinter
 */

/*
 * Copyright (C) 2025 embedded brains GmbH & Co. KG
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

#include <stdarg.h>
#include <rtems/printer.h>

#include <rtems/test.h>

/**
 * @defgroup RtemsIoValPrinter spec:/rtems/io/val/printer
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @brief Tests the printer directives.
 *
 * This test case performs the following actions:
 *
 * - Validate the printer directives while using an invalid printer.
 *
 *   - Check that rtems_printf() returns zero for the empty printer.
 *
 *   - Check that rtems_vprintf() returns zero for the NULL printer.
 *
 * - Validate the printer directives while using a valid printer.
 *
 *   - Check that rtems_printf() returns the expected value.
 *
 *   - Check that the printer was called exactly once.
 *
 *   - Check that rtems_vprintf() returns the expected value.
 *
 *   - Check that the printer was called exactly once.
 *
 * @{
 */

static int CallVprintf( const rtems_printer *printer, const char *fmt, ... )
{
  va_list ap;
  int     n;

  va_start( ap, fmt );
  n = rtems_vprintf( printer, fmt, ap );
  va_end( ap );
  return n;
}

static int Printer( void *context, const char *format, va_list ap )
{
  uint32_t *counter;

  counter = context;
  ++( *counter );
  T_eq_str( format, "%i" );

  return va_arg( ap, int );
}

/**
 * @brief Validate the printer directives while using an invalid printer.
 */
static void RtemsIoValPrinter_Action_0( void )
{
  rtems_printer printer;

  rtems_print_printer_empty( &printer );

  /*
   * Check that rtems_printf() returns zero for the empty printer.
   */
  T_eq_int( rtems_printf( &printer, "x" ), 0 );

  /*
   * Check that rtems_vprintf() returns zero for the NULL printer.
   */
  T_eq_int( CallVprintf( NULL, "x" ), 0 );
}

/**
 * @brief Validate the printer directives while using a valid printer.
 */
static void RtemsIoValPrinter_Action_1( void )
{
  rtems_printer printer;
  uint32_t      counter;

  counter = 0;
  rtems_print_printer_empty( &printer );
  printer.context = &counter;
  printer.printer = Printer;

  /*
   * Check that rtems_printf() returns the expected value.
   */
  T_eq_int( rtems_printf( &printer, "%i", 123 ), 123 );

  /*
   * Check that the printer was called exactly once.
   */
  T_eq_u32( counter, 1 );

  /*
   * Check that rtems_vprintf() returns the expected value.
   */
  T_eq_int( CallVprintf( &printer, "%i", 456 ), 456 );

  /*
   * Check that the printer was called exactly once.
   */
  T_eq_u32( counter, 2 );
}

/**
 * @fn void T_case_body_RtemsIoValPrinter( void )
 */
T_TEST_CASE( RtemsIoValPrinter )
{
  RtemsIoValPrinter_Action_0();
  RtemsIoValPrinter_Action_1();
}

/** @} */
