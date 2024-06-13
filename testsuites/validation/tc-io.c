/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsIoValIo
 */

/*
 * Copyright (C) 2024 embedded brains GmbH & Co. KG
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

#include "tr-io-kernel-printflike.h"

#include <rtems/test.h>

/**
 * @defgroup RtemsIoValIo spec:/rtems/io/val/io
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @brief Tests the kernel-printflike directives.
 *
 * This test case performs the following actions:
 *
 * - Call printk() with a format string which outputs a newline to notice the
 *   rtems_putc() newline translation.
 *
 *   - Check that printk() used rtems_putc() for output.
 *
 * - Call vprintk() with a format string which outputs a newline to notice the
 *   rtems_putc() newline translation.
 *
 *   - Check that vprintk() used rtems_putc() for output.
 *
 * - Check that printk() is a kernel-printflike function.
 *
 * - Check that vprintk() is a kernel-printflike function.
 *
 * @{
 */

static KernelPrintflikeControl kernel_printflike_control;

static void Putc( char c )
{
  KernelPrintflikeControl *control;
  size_t                   index;

  control = &kernel_printflike_control;
  index = control->buf_index;

  if ( index < sizeof( control->buf ) - 2 ) {
    control->buf[ index ] = c;
    control->buf_index = index + 1;
  }
}

static void PutcIgnoreCR( char c )
{
  if ( c == '\r' ) {
    return;
  }

  Putc( c );
}

static int PrintkNoArg( const char *fmt )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = printk( fmt );
  BSP_output_char = output_char;
  return rv;
}

static int PrintkInt( const char *fmt, int arg )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = printk( fmt, arg );
  BSP_output_char = output_char;
  return rv;
}

static int PrintkIntInt( const char *fmt, int arg, int arg_2 )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = printk( fmt, arg, arg_2 );
  BSP_output_char = output_char;
  return rv;
}

static int PrintkLong( const char *fmt, long arg )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = printk( fmt, arg );
  BSP_output_char = output_char;
  return rv;
}

static int PrintkIntmax( const char *fmt, intmax_t arg )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = printk( fmt, arg );
  BSP_output_char = output_char;
  return rv;
}

static int PrintkString( const char *fmt, const char *arg )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = printk( fmt, arg );
  BSP_output_char = output_char;
  return rv;
}

static int PrintkBinary(
  const char          *fmt,
  const unsigned char *data,
  const char          *sep
)
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = printk( fmt, data, sep );
  BSP_output_char = output_char;
  return rv;
}

static int CallVprintk( const char *fmt, ... )
{
  va_list ap;
  int     rv;

  va_start( ap, fmt );
  rv = vprintk( fmt, ap );
  va_end( ap );
  return rv;
}

static int VprintkNoArg( const char *fmt )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = CallVprintk( fmt );
  BSP_output_char = output_char;
  return rv;
}

static int VprintkInt( const char *fmt, int arg )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = CallVprintk( fmt, arg );
  BSP_output_char = output_char;
  return rv;
}

static int VprintkIntInt( const char *fmt, int arg, int arg_2 )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = CallVprintk( fmt, arg, arg_2 );
  BSP_output_char = output_char;
  return rv;
}

static int VprintkLong( const char *fmt, long arg )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = CallVprintk( fmt, arg );
  BSP_output_char = output_char;
  return rv;
}

static int VprintkIntmax( const char *fmt, intmax_t arg )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = CallVprintk( fmt, arg );
  BSP_output_char = output_char;
  return rv;
}

static int VprintkString( const char *fmt, const char *arg )
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = CallVprintk( fmt, arg );
  BSP_output_char = output_char;
  return rv;
}

static int VprintkBinary(
  const char          *fmt,
  const unsigned char *data,
  const char          *sep
)
{
  BSP_output_char_function_type output_char;
  int                           rv;

  output_char = BSP_output_char;
  BSP_output_char = PutcIgnoreCR;
  rv = CallVprintk( fmt, data, sep );
  BSP_output_char = output_char;
  return rv;
}

/**
 * @brief Call printk() with a format string which outputs a newline to notice
 *   the rtems_putc() newline translation.
 */
static void RtemsIoValIo_Action_0( void )
{
  KernelPrintflikeControl      *control;
  BSP_output_char_function_type output_char;
  int                           rv;

  control = &kernel_printflike_control;
  memset( control->buf, '\0', sizeof( control->buf ) );
  control->buf_index = 0;
  output_char = BSP_output_char;
  BSP_output_char = Putc;
  rv = printk( "\n" );
  BSP_output_char = output_char;

  /*
   * Check that printk() used rtems_putc() for output.
   */
  T_eq_int( rv, 1 );
  T_eq_str( control->buf, "\r\n" );
}

/**
 * @brief Call vprintk() with a format string which outputs a newline to notice
 *   the rtems_putc() newline translation.
 */
static void RtemsIoValIo_Action_1( void )
{
  KernelPrintflikeControl      *control;
  BSP_output_char_function_type output_char;
  int                           rv;

  control = &kernel_printflike_control;
  memset( control->buf, '\0', sizeof( control->buf ) );
  control->buf_index = 0;
  output_char = BSP_output_char;
  BSP_output_char = Putc;
  rv = CallVprintk( "\n" );
  BSP_output_char = output_char;

  /*
   * Check that vprintk() used rtems_putc() for output.
   */
  T_eq_int( rv, 1 );
  T_eq_str( control->buf, "\r\n" );
}

/**
 * @brief Check that printk() is a kernel-printflike function.
 */
static void RtemsIoValIo_Action_2( void )
{
  KernelPrintflikeControl *control;

  control = &kernel_printflike_control;
  control->print_no_arg = PrintkNoArg;
  control->print_int = PrintkInt;
  control->print_int_int = PrintkIntInt;
  control->print_long = PrintkLong;
  control->print_intmax = PrintkIntmax;
  control->print_string = PrintkString;
  control->print_binary = PrintkBinary;
  RtemsIoValKernelPrintflike_Run( control );
}

/**
 * @brief Check that vprintk() is a kernel-printflike function.
 */
static void RtemsIoValIo_Action_3( void )
{
  KernelPrintflikeControl *control;

  control = &kernel_printflike_control;
  control->print_no_arg = VprintkNoArg;
  control->print_int = VprintkInt;
  control->print_int_int = VprintkIntInt;
  control->print_long = VprintkLong;
  control->print_intmax = VprintkIntmax;
  control->print_string = VprintkString;
  control->print_binary = VprintkBinary;
  RtemsIoValKernelPrintflike_Run( control );
}

/**
 * @fn void T_case_body_RtemsIoValIo( void )
 */
T_TEST_CASE( RtemsIoValIo )
{
  RtemsIoValIo_Action_0();
  RtemsIoValIo_Action_1();
  RtemsIoValIo_Action_2();
  RtemsIoValIo_Action_3();
}

/** @} */
