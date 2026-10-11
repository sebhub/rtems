/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsIoValPrintkPrinter
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

#include <stdarg.h>
#include <string.h>
#include <rtems/bspIo.h>
#include <rtems/printer.h>

#include <rtems/test.h>

/**
 * @defgroup RtemsIoValPrintkPrinter spec:/rtems/io/val/printk-printer
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @brief Tests the rtems_printk_printer() and rtems_print_printer_printk()
 *   directives.
 *
 * This test case performs the following actions:
 *
 * - Call rtems_printk_printer() and vprintk() with the same format string and
 *   the same arguments. Capture the output of each call.
 *
 *   - Check that rtems_printk_printer() printed the characters which vprintk()
 *     printed.
 *
 *   - Check that rtems_printk_printer() returned the value which vprintk()
 *     returned.
 *
 * - Call rtems_print_printer_printk() for a printer with a handler which is
 *   not rtems_printk_printer().
 *
 *   - Check that the handler of the printer is rtems_printk_printer().
 *
 * @{
 */

#define FORMAT "%i:%s\n"

typedef struct {
  char   buf[ 32 ];
  size_t size;
} Output;

static Output *current_output;

static void PutChar( char c )
{
  Output *output;

  output = current_output;

  if ( output->size < sizeof( output->buf ) ) {
    output->buf[ output->size ] = c;
    ++output->size;
  }
}

static int Unused( void *context, const char *format, va_list ap )
{
  (void) context;
  (void) format;
  (void) ap;

  return 0;
}

static int CallPrintkPrinter( Output *output, const char *format, ... )
{
  BSP_output_char_function_type output_char;
  va_list                       ap;
  int                           context;
  int                           rv;

  memset( output, 0, sizeof( *output ) );
  current_output = output;
  output_char = BSP_output_char;
  BSP_output_char = PutChar;
  va_start( ap, format );
  rv = rtems_printk_printer( &context, format, ap );
  va_end( ap );
  BSP_output_char = output_char;

  return rv;
}

static int CallVprintk( Output *output, const char *format, ... )
{
  BSP_output_char_function_type output_char;
  va_list                       ap;
  int                           rv;

  memset( output, 0, sizeof( *output ) );
  current_output = output;
  output_char = BSP_output_char;
  BSP_output_char = PutChar;
  va_start( ap, format );
  rv = vprintk( format, ap );
  va_end( ap );
  BSP_output_char = output_char;

  return rv;
}

/**
 * @brief Call rtems_printk_printer() and vprintk() with the same format string
 *   and the same arguments. Capture the output of each call.
 */
static void RtemsIoValPrintkPrinter_Action_0( void )
{
  Output printer_output;
  Output vprintk_output;
  int    printer_rv;
  int    vprintk_rv;

  printer_rv = CallPrintkPrinter( &printer_output, FORMAT, 12, "ab" );
  vprintk_rv = CallVprintk( &vprintk_output, FORMAT, 12, "ab" );

  /*
   * Check that rtems_printk_printer() printed the characters which vprintk()
   * printed.
   */
  T_gt_sz( vprintk_output.size, 0 );
  T_eq_sz( printer_output.size, vprintk_output.size );
  T_eq_mem( printer_output.buf, vprintk_output.buf, vprintk_output.size );

  /*
   * Check that rtems_printk_printer() returned the value which vprintk()
   * returned.
   */
  T_eq_int( printer_rv, vprintk_rv );
}

/**
 * @brief Call rtems_print_printer_printk() for a printer with a handler which
 *   is not rtems_printk_printer().
 */
static void RtemsIoValPrintkPrinter_Action_1( void )
{
  rtems_printer printer;

  rtems_print_printer_empty( &printer );
  printer.printer = Unused;
  rtems_print_printer_printk( &printer );

  /*
   * Check that the handler of the printer is rtems_printk_printer().
   */
  T_eq_ptr( printer.printer, rtems_printk_printer );
}

/**
 * @fn void T_case_body_RtemsIoValPrintkPrinter( void )
 */
T_TEST_CASE( RtemsIoValPrintkPrinter )
{
  RtemsIoValPrintkPrinter_Action_0();
  RtemsIoValPrintkPrinter_Action_1();
}

/** @} */
