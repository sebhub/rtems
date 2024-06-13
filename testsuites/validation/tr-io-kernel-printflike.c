/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsIoValKernelPrintflike
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

#include <string.h>

#include "tr-io-kernel-printflike.h"

#include <rtems/test.h>

/**
 * @defgroup RtemsIoValKernelPrintflike spec:/rtems/io/val/kernel-printflike
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @brief Tests directives specified by spec:/rtems/io/req/kernel-printflike.
 *
 * This test case performs the following actions:
 *
 * - Call a kernel-printflike function with a NULL format string pointer.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a '%' character.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function with a 'D' conversion with a default
 *   width.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function with a 'D' conversion with a specified
 *   width.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function with an invalid format string.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character with a width.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character with a width
 *   argument.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character with left-justify
 *   and a non-default width.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character with a negative
 *   width.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character with left-justify
 *   and the default width.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a string.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a string with a NULL pointer
 *   argument.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a string with a width.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a string with a width and
 *   left-justify.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a string with a sufficient
 *   precision and left-justify.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a string with an insufficient
 *   precision.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a pointer integer.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a pointer integer with a width.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a zero integer in alternate
 *   form.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a non-zero integer in decimal
 *   alternate form.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a non-zero integer in unsigned
 *   alternate form.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer with a width.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer with a width and
 *   zero padding.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer with a width, a zero
 *   padding, and left-justify.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character integer in decimal
 *   notation with a precision.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character integer in decimal
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a short integer in decimal
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer in decimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long integer in decimal
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long long integer in decimal
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a maximum integer in decimal
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character integer in unsigned
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character integer in unsigned
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a short integer in unsigned
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer in unsigned
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long integer in unsigned
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long long integer in unsigned
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a maximum integer in unsigned
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a short integer in unsigned
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer in unsigned
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long integer in unsigned
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long long integer in unsigned
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a maximum integer in unsigned
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character integer in octal
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a short integer in octal
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer in octal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long integer in octal
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long long integer in octal
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a maximum integer in octal
 *   notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character integer in
 *   alternate octal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a short integer in alternate
 *   octal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer in alternate octal
 *   notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long integer in alternate
 *   octal notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long long integer in
 *   alternate octal notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a maximum integer in alternate
 *   octal notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character integer in lower
 *   case hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a short integer in lower case
 *   hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer in lower case
 *   hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long integer in lower case
 *   hexadecimal notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long long integer in lower
 *   case hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a maximum integer in lower case
 *   hexadecimal notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character integer in
 *   alternate lower case hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a short integer in alternate
 *   lower case hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer in alternate lower
 *   case hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long integer in alternate
 *   lower case hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long long integer in
 *   alternate lower case hexadecimal notation. Add 'h' type specifiers which
 *   should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a maximum integer in alternate
 *   lower case hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character integer in upper
 *   case hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a short integer in upper case
 *   hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer in upper case
 *   hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long integer in upper case
 *   hexadecimal notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long long integer in upper
 *   case hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a maximum integer in upper case
 *   hexadecimal notation. Add 'h' type specifiers which should be ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a character integer in lower
 *   case signed hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a short integer in lower case
 *   signed hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print an integer in lower case signed
 *   hexadecimal notation.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long integer in lower case
 *   signed hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a long long integer in lower
 *   case signed hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * - Call a kernel-printflike function to print a maximum integer in lower case
 *   signed hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 *
 *   - Check that the count of output characters is returned.
 *
 *   - Check that the expected output was produced.
 *
 * @{
 */

/**
 * @brief Test context for spec:/rtems/io/val/kernel-printflike test case.
 */
typedef struct {
  /**
   * @brief This member contains a copy of the corresponding
   *   RtemsIoValKernelPrintflike_Run() parameter.
   */
  KernelPrintflikeControl *control;
} RtemsIoValKernelPrintflike_Context;

static RtemsIoValKernelPrintflike_Context RtemsIoValKernelPrintflike_Instance;

static void Prepare( KernelPrintflikeControl *control )
{
  memset( control->buf, '\0', sizeof( control->buf ) );
  control->buf_index = 0;
}

static T_fixture RtemsIoValKernelPrintflike_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = NULL,
  .initial_context = &RtemsIoValKernelPrintflike_Instance
};

/**
 * @brief Call a kernel-printflike function with a NULL format string pointer.
 */
static void RtemsIoValKernelPrintflike_Action_0(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_no_arg )( NULL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 11 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "(fmt null)\n" );
}

/**
 * @brief Call a kernel-printflike function to print a '%' character.
 */
static void RtemsIoValKernelPrintflike_Action_1(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_no_arg )( "%%%%" );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "%%" );
}

/**
 * @brief Call a kernel-printflike function with a 'D' conversion with a
 *   default width.
 */
static void RtemsIoValKernelPrintflike_Action_2(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  static const unsigned char binary[] =
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_binary )( "%D", binary, ":" );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 47 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str(
    ctx->control->buf,
    "00:01:02:03:04:05:06:07:08:09:0a:0b:0c:0d:0e:0f"
  );
}

/**
 * @brief Call a kernel-printflike function with a 'D' conversion with a
 *   specified width.
 */
static void RtemsIoValKernelPrintflike_Action_3(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  static const unsigned char binary[] = { 255 };
  int                        rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_binary )( "%1D", binary, "?" );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "ff" );
}

/**
 * @brief Call a kernel-printflike function with an invalid format string.
 */
static void RtemsIoValKernelPrintflike_Action_4(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_no_arg )( "%0.0?%i" );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 7 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "%0.0?%i" );
}

/**
 * @brief Call a kernel-printflike function to print a character.
 */
static void RtemsIoValKernelPrintflike_Action_5(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%c", 'c' );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 1 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "c" );
}

/**
 * @brief Call a kernel-printflike function to print a character with a width.
 */
static void RtemsIoValKernelPrintflike_Action_6(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%10c", 'c' );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 10 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "         c" );
}

/**
 * @brief Call a kernel-printflike function to print a character with a width
 *   argument.
 */
static void RtemsIoValKernelPrintflike_Action_7(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int_int )( "%0*c", 2, 'c' );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0c" );
}

/**
 * @brief Call a kernel-printflike function to print a character with
 *   left-justify and a non-default width.
 */
static void RtemsIoValKernelPrintflike_Action_8(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%-2.c", 'c' );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "c " );
}

/**
 * @brief Call a kernel-printflike function to print a character with a
 *   negative width.
 */
static void RtemsIoValKernelPrintflike_Action_9(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int_int )( "%*c", -2, 'c' );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "c " );
}

/**
 * @brief Call a kernel-printflike function to print a character with
 *   left-justify and the default width.
 */
static void RtemsIoValKernelPrintflike_Action_10(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%-c", 'c' );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 1 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "c" );
}

/**
 * @brief Call a kernel-printflike function to print a string.
 */
static void RtemsIoValKernelPrintflike_Action_11(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_string )( "%s", "str" );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 3 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "str" );
}

/**
 * @brief Call a kernel-printflike function to print a string with a NULL
 *   pointer argument.
 */
static void RtemsIoValKernelPrintflike_Action_12(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_string )( "%s", NULL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 6 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "(null)" );
}

/**
 * @brief Call a kernel-printflike function to print a string with a width.
 */
static void RtemsIoValKernelPrintflike_Action_13(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_string )( "%5s", "str" );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 5 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "  str" );
}

/**
 * @brief Call a kernel-printflike function to print a string with a width and
 *   left-justify.
 */
static void RtemsIoValKernelPrintflike_Action_14(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_string )( "%-5s", "str" );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 5 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "str  " );
}

/**
 * @brief Call a kernel-printflike function to print a string with a sufficient
 *   precision and left-justify.
 */
static void RtemsIoValKernelPrintflike_Action_15(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_string )( "%-.5s", "str" );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 3 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "str" );
}

/**
 * @brief Call a kernel-printflike function to print a string with an
 *   insufficient precision.
 */
static void RtemsIoValKernelPrintflike_Action_16(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_string )( "%.1s", "str" );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 1 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "s" );
}

/**
 * @brief Call a kernel-printflike function to print a pointer integer.
 */
static void RtemsIoValKernelPrintflike_Action_17(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_string )( "%+p", (void *) (uintptr_t) 0x1234 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 6 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0x1234" );
}

/**
 * @brief Call a kernel-printflike function to print a pointer integer with a
 *   width.
 */
static void RtemsIoValKernelPrintflike_Action_18(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_string )( "%6p", (void *) (uintptr_t) 0x1234 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 6 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "  1234" );
}

/**
 * @brief Call a kernel-printflike function to print a zero integer in
 *   alternate form.
 */
static void RtemsIoValKernelPrintflike_Action_19(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%#o", 0 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 1 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0" );
}

/**
 * @brief Call a kernel-printflike function to print a non-zero integer in
 *   decimal alternate form.
 */
static void RtemsIoValKernelPrintflike_Action_20(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%#i", -1 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-1" );
}

/**
 * @brief Call a kernel-printflike function to print a non-zero integer in
 *   unsigned alternate form.
 */
static void RtemsIoValKernelPrintflike_Action_21(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%#u", 1 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 1 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "1" );
}

/**
 * @brief Call a kernel-printflike function to print an integer with a width.
 */
static void RtemsIoValKernelPrintflike_Action_22(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%2i", 1 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, " 1" );
}

/**
 * @brief Call a kernel-printflike function to print an integer with a width
 *   and zero padding.
 */
static void RtemsIoValKernelPrintflike_Action_23(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%02i", 1 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "01" );
}

/**
 * @brief Call a kernel-printflike function to print an integer with a width, a
 *   zero padding, and left-justify.
 */
static void RtemsIoValKernelPrintflike_Action_24(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%-02i", 1 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "1 " );
}

/**
 * @brief Call a kernel-printflike function to print a character integer in
 *   decimal notation with a precision.
 */
static void RtemsIoValKernelPrintflike_Action_25(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int_int )( "%.*hhi", 5, 0xf1234567 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 5 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "00103" );
}

/**
 * @brief Call a kernel-printflike function to print a character integer in
 *   decimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_26(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hhd", 0x012345f7 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-9" );
}

/**
 * @brief Call a kernel-printflike function to print a short integer in decimal
 *   notation.
 */
static void RtemsIoValKernelPrintflike_Action_27(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hi", 0x1234f678 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 5 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-2440" );
}

/**
 * @brief Call a kernel-printflike function to print an integer in decimal
 *   notation.
 */
static void RtemsIoValKernelPrintflike_Action_28(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%d", -213555319 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 10 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-213555319" );
}

/**
 * @brief Call a kernel-printflike function to print a long integer in decimal
 *   notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_29(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_long )( "%hltzhi", -195659622L );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 10 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-195659622" );
}

/**
 * @brief Call a kernel-printflike function to print a long long integer in
 *   decimal notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_30(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hllhd", -763490240828599756LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 19 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-763490240828599756" );
}

/**
 * @brief Call a kernel-printflike function to print a maximum integer in
 *   decimal notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_31(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hjhi", -686628807189126331LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 19 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-686628807189126331" );
}

/**
 * @brief Call a kernel-printflike function to print a character integer in
 *   unsigned notation.
 */
static void RtemsIoValKernelPrintflike_Action_32(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hhu", 0x012345f7 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 3 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "247" );
}

/**
 * @brief Call a kernel-printflike function to print a character integer in
 *   unsigned notation.
 */
static void RtemsIoValKernelPrintflike_Action_33(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hhu", 0x012345f7 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 3 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "247" );
}

/**
 * @brief Call a kernel-printflike function to print a short integer in
 *   unsigned notation.
 */
static void RtemsIoValKernelPrintflike_Action_34(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hu", 0x1234f678 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 5 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "63096" );
}

/**
 * @brief Call a kernel-printflike function to print an integer in unsigned
 *   notation.
 */
static void RtemsIoValKernelPrintflike_Action_35(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%u", 0xf3456789 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 10 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "4081411977" );
}

/**
 * @brief Call a kernel-printflike function to print a long integer in unsigned
 *   notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_36(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_long )( "%hltzhu", 0xf456789aL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 10 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "4099307674" );
}

/**
 * @brief Call a kernel-printflike function to print a long long integer in
 *   unsigned notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_37(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hllhu", 0xf56789abcdef1234LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 20 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "17683253832880951860" );
}

/**
 * @brief Call a kernel-printflike function to print a maximum integer in
 *   unsigned notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_38(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hjhu", 0xf6789abcdef12345LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 20 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "17760115266520425285" );
}

/**
 * @brief Call a kernel-printflike function to print a short integer in
 *   unsigned notation.
 */
static void RtemsIoValKernelPrintflike_Action_39(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hu", 0x1234f678 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 5 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "63096" );
}

/**
 * @brief Call a kernel-printflike function to print an integer in unsigned
 *   notation.
 */
static void RtemsIoValKernelPrintflike_Action_40(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%u", 0xf3456789 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 10 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "4081411977" );
}

/**
 * @brief Call a kernel-printflike function to print a long integer in unsigned
 *   notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_41(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_long )( "%hltzhu", 0xf456789aL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 10 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "4099307674" );
}

/**
 * @brief Call a kernel-printflike function to print a long long integer in
 *   unsigned notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_42(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hllhu", 0xf56789abcdef1234LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 20 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "17683253832880951860" );
}

/**
 * @brief Call a kernel-printflike function to print a maximum integer in
 *   unsigned notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_43(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hjhu", 0xf6789abcdef12345LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 20 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "17760115266520425285" );
}

/**
 * @brief Call a kernel-printflike function to print a character integer in
 *   octal notation.
 */
static void RtemsIoValKernelPrintflike_Action_44(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hho", 0x012345f7 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 3 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "367" );
}

/**
 * @brief Call a kernel-printflike function to print a short integer in octal
 *   notation.
 */
static void RtemsIoValKernelPrintflike_Action_45(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%ho", 0x1234f678 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 6 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "173170" );
}

/**
 * @brief Call a kernel-printflike function to print an integer in octal
 *   notation.
 */
static void RtemsIoValKernelPrintflike_Action_46(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%o", 0xf3456789 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 11 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "36321263611" );
}

/**
 * @brief Call a kernel-printflike function to print a long integer in octal
 *   notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_47(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_long )( "%hltzho", 0xf456789aL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 11 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "36425474232" );
}

/**
 * @brief Call a kernel-printflike function to print a long long integer in
 *   octal notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_48(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hllho", 0xf56789abcdef1234LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 22 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "1725474232571573611064" );
}

/**
 * @brief Call a kernel-printflike function to print a maximum integer in octal
 *   notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_49(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hjho", 0xf6789abcdef12345LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 22 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "1731704653633674221505" );
}

/**
 * @brief Call a kernel-printflike function to print a character integer in
 *   alternate octal notation.
 */
static void RtemsIoValKernelPrintflike_Action_50(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%#hho", 0x012345f7 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 4 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0367" );
}

/**
 * @brief Call a kernel-printflike function to print a short integer in
 *   alternate octal notation.
 */
static void RtemsIoValKernelPrintflike_Action_51(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%#ho", 0x1234f678 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 7 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0173170" );
}

/**
 * @brief Call a kernel-printflike function to print an integer in alternate
 *   octal notation.
 */
static void RtemsIoValKernelPrintflike_Action_52(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%#o", 0xf3456789 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 12 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "036321263611" );
}

/**
 * @brief Call a kernel-printflike function to print a long integer in
 *   alternate octal notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_53(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_long )( "%hltzho", 0xf456789aL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 11 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "36425474232" );
}

/**
 * @brief Call a kernel-printflike function to print a long long integer in
 *   alternate octal notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_54(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%#hllho", 0xf56789abcdef1234LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 23 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "01725474232571573611064" );
}

/**
 * @brief Call a kernel-printflike function to print a maximum integer in
 *   alternate octal notation. Add 'h' type specifiers which should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_55(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%#hjho", 0xf6789abcdef12345LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 23 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "01731704653633674221505" );
}

/**
 * @brief Call a kernel-printflike function to print a character integer in
 *   lower case hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_56(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hhx", 0x012345f7 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "f7" );
}

/**
 * @brief Call a kernel-printflike function to print a short integer in lower
 *   case hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_57(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hx", 0x1234f678 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 4 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "f678" );
}

/**
 * @brief Call a kernel-printflike function to print an integer in lower case
 *   hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_58(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%x", 0xf3456789 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 8 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "f3456789" );
}

/**
 * @brief Call a kernel-printflike function to print a long integer in lower
 *   case hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 */
static void RtemsIoValKernelPrintflike_Action_59(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_long )( "%hltzhx", 0xf456789aL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 8 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "f456789a" );
}

/**
 * @brief Call a kernel-printflike function to print a long long integer in
 *   lower case hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 */
static void RtemsIoValKernelPrintflike_Action_60(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hllhx", 0xf56789abcdef1234LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 16 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "f56789abcdef1234" );
}

/**
 * @brief Call a kernel-printflike function to print a maximum integer in lower
 *   case hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 */
static void RtemsIoValKernelPrintflike_Action_61(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hjhx", 0xf6789abcdef12345LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 16 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "f6789abcdef12345" );
}

/**
 * @brief Call a kernel-printflike function to print a character integer in
 *   alternate lower case hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_62(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%#hhx", 0x012345f7 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 4 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0xf7" );
}

/**
 * @brief Call a kernel-printflike function to print a short integer in
 *   alternate lower case hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_63(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%#hx", 0x1234f678 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 6 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0xf678" );
}

/**
 * @brief Call a kernel-printflike function to print an integer in alternate
 *   lower case hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_64(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%#x", 0xf3456789 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 10 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0xf3456789" );
}

/**
 * @brief Call a kernel-printflike function to print a long integer in
 *   alternate lower case hexadecimal notation. Add 'h' type specifiers which
 *   should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_65(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_long )( "%#hltzhx", 0xf456789aL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 10 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0xf456789a" );
}

/**
 * @brief Call a kernel-printflike function to print a long long integer in
 *   alternate lower case hexadecimal notation. Add 'h' type specifiers which
 *   should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_66(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%#hllhx", 0xf56789abcdef1234LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 18 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0xf56789abcdef1234" );
}

/**
 * @brief Call a kernel-printflike function to print a maximum integer in
 *   alternate lower case hexadecimal notation. Add 'h' type specifiers which
 *   should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_67(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%#hjhx", 0xf6789abcdef12345LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 18 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "0xf6789abcdef12345" );
}

/**
 * @brief Call a kernel-printflike function to print a character integer in
 *   upper case hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_68(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hhX", 0x012345f7 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "F7" );
}

/**
 * @brief Call a kernel-printflike function to print a short integer in upper
 *   case hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_69(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hX", 0x1234f678 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 4 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "F678" );
}

/**
 * @brief Call a kernel-printflike function to print an integer in upper case
 *   hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_70(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%X", 0xf3456789 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 8 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "F3456789" );
}

/**
 * @brief Call a kernel-printflike function to print a long integer in upper
 *   case hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 */
static void RtemsIoValKernelPrintflike_Action_71(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_long )( "%hltzhX", 0xf456789aL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 8 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "F456789A" );
}

/**
 * @brief Call a kernel-printflike function to print a long long integer in
 *   upper case hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 */
static void RtemsIoValKernelPrintflike_Action_72(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hllhX", 0xf56789abcdef1234LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 16 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "F56789ABCDEF1234" );
}

/**
 * @brief Call a kernel-printflike function to print a maximum integer in upper
 *   case hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 */
static void RtemsIoValKernelPrintflike_Action_73(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hjhX", 0xf6789abcdef12345LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 16 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "F6789ABCDEF12345" );
}

/**
 * @brief Call a kernel-printflike function to print a character integer in
 *   lower case signed hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_74(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hhy", 0x012345f7 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 2 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-9" );
}

/**
 * @brief Call a kernel-printflike function to print a short integer in lower
 *   case signed hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_75(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%hy", 0x1234f678 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 4 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-988" );
}

/**
 * @brief Call a kernel-printflike function to print an integer in lower case
 *   signed hexadecimal notation.
 */
static void RtemsIoValKernelPrintflike_Action_76(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_int )( "%y", -213555319 );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 8 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-cba9877" );
}

/**
 * @brief Call a kernel-printflike function to print a long integer in lower
 *   case signed hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 */
static void RtemsIoValKernelPrintflike_Action_77(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_long )( "%hltzhy", -195659622L );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 8 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-ba98766" );
}

/**
 * @brief Call a kernel-printflike function to print a long long integer in
 *   lower case signed hexadecimal notation. Add 'h' type specifiers which
 *   should be ignored.
 */
static void RtemsIoValKernelPrintflike_Action_78(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hllhy", -763490240828599756LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 16 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-a9876543210edcc" );
}

/**
 * @brief Call a kernel-printflike function to print a maximum integer in lower
 *   case signed hexadecimal notation. Add 'h' type specifiers which should be
 *   ignored.
 */
static void RtemsIoValKernelPrintflike_Action_79(
  RtemsIoValKernelPrintflike_Context *ctx
)
{
  int rv;

  Prepare( ctx->control );
  rv = ( *ctx->control->print_intmax )( "%hjhy", -686628807189126331LL );

  /*
   * Check that the count of output characters is returned.
   */
  T_eq_int( rv, 16 );

  /*
   * Check that the expected output was produced.
   */
  T_eq_str( ctx->control->buf, "-9876543210edcbb" );
}

static T_fixture_node RtemsIoValKernelPrintflike_Node;

static T_remark RtemsIoValKernelPrintflike_Remark = {
  .next = NULL,
  .remark = "RtemsIoValKernelPrintflike"
};

void RtemsIoValKernelPrintflike_Run( KernelPrintflikeControl *control )
{
  RtemsIoValKernelPrintflike_Context *ctx;

  ctx = &RtemsIoValKernelPrintflike_Instance;
  ctx->control = control;

  ctx = T_push_fixture(
    &RtemsIoValKernelPrintflike_Node,
    &RtemsIoValKernelPrintflike_Fixture
  );

  RtemsIoValKernelPrintflike_Action_0( ctx );
  RtemsIoValKernelPrintflike_Action_1( ctx );
  RtemsIoValKernelPrintflike_Action_2( ctx );
  RtemsIoValKernelPrintflike_Action_3( ctx );
  RtemsIoValKernelPrintflike_Action_4( ctx );
  RtemsIoValKernelPrintflike_Action_5( ctx );
  RtemsIoValKernelPrintflike_Action_6( ctx );
  RtemsIoValKernelPrintflike_Action_7( ctx );
  RtemsIoValKernelPrintflike_Action_8( ctx );
  RtemsIoValKernelPrintflike_Action_9( ctx );
  RtemsIoValKernelPrintflike_Action_10( ctx );
  RtemsIoValKernelPrintflike_Action_11( ctx );
  RtemsIoValKernelPrintflike_Action_12( ctx );
  RtemsIoValKernelPrintflike_Action_13( ctx );
  RtemsIoValKernelPrintflike_Action_14( ctx );
  RtemsIoValKernelPrintflike_Action_15( ctx );
  RtemsIoValKernelPrintflike_Action_16( ctx );
  RtemsIoValKernelPrintflike_Action_17( ctx );
  RtemsIoValKernelPrintflike_Action_18( ctx );
  RtemsIoValKernelPrintflike_Action_19( ctx );
  RtemsIoValKernelPrintflike_Action_20( ctx );
  RtemsIoValKernelPrintflike_Action_21( ctx );
  RtemsIoValKernelPrintflike_Action_22( ctx );
  RtemsIoValKernelPrintflike_Action_23( ctx );
  RtemsIoValKernelPrintflike_Action_24( ctx );
  RtemsIoValKernelPrintflike_Action_25( ctx );
  RtemsIoValKernelPrintflike_Action_26( ctx );
  RtemsIoValKernelPrintflike_Action_27( ctx );
  RtemsIoValKernelPrintflike_Action_28( ctx );
  RtemsIoValKernelPrintflike_Action_29( ctx );
  RtemsIoValKernelPrintflike_Action_30( ctx );
  RtemsIoValKernelPrintflike_Action_31( ctx );
  RtemsIoValKernelPrintflike_Action_32( ctx );
  RtemsIoValKernelPrintflike_Action_33( ctx );
  RtemsIoValKernelPrintflike_Action_34( ctx );
  RtemsIoValKernelPrintflike_Action_35( ctx );
  RtemsIoValKernelPrintflike_Action_36( ctx );
  RtemsIoValKernelPrintflike_Action_37( ctx );
  RtemsIoValKernelPrintflike_Action_38( ctx );
  RtemsIoValKernelPrintflike_Action_39( ctx );
  RtemsIoValKernelPrintflike_Action_40( ctx );
  RtemsIoValKernelPrintflike_Action_41( ctx );
  RtemsIoValKernelPrintflike_Action_42( ctx );
  RtemsIoValKernelPrintflike_Action_43( ctx );
  RtemsIoValKernelPrintflike_Action_44( ctx );
  RtemsIoValKernelPrintflike_Action_45( ctx );
  RtemsIoValKernelPrintflike_Action_46( ctx );
  RtemsIoValKernelPrintflike_Action_47( ctx );
  RtemsIoValKernelPrintflike_Action_48( ctx );
  RtemsIoValKernelPrintflike_Action_49( ctx );
  RtemsIoValKernelPrintflike_Action_50( ctx );
  RtemsIoValKernelPrintflike_Action_51( ctx );
  RtemsIoValKernelPrintflike_Action_52( ctx );
  RtemsIoValKernelPrintflike_Action_53( ctx );
  RtemsIoValKernelPrintflike_Action_54( ctx );
  RtemsIoValKernelPrintflike_Action_55( ctx );
  RtemsIoValKernelPrintflike_Action_56( ctx );
  RtemsIoValKernelPrintflike_Action_57( ctx );
  RtemsIoValKernelPrintflike_Action_58( ctx );
  RtemsIoValKernelPrintflike_Action_59( ctx );
  RtemsIoValKernelPrintflike_Action_60( ctx );
  RtemsIoValKernelPrintflike_Action_61( ctx );
  RtemsIoValKernelPrintflike_Action_62( ctx );
  RtemsIoValKernelPrintflike_Action_63( ctx );
  RtemsIoValKernelPrintflike_Action_64( ctx );
  RtemsIoValKernelPrintflike_Action_65( ctx );
  RtemsIoValKernelPrintflike_Action_66( ctx );
  RtemsIoValKernelPrintflike_Action_67( ctx );
  RtemsIoValKernelPrintflike_Action_68( ctx );
  RtemsIoValKernelPrintflike_Action_69( ctx );
  RtemsIoValKernelPrintflike_Action_70( ctx );
  RtemsIoValKernelPrintflike_Action_71( ctx );
  RtemsIoValKernelPrintflike_Action_72( ctx );
  RtemsIoValKernelPrintflike_Action_73( ctx );
  RtemsIoValKernelPrintflike_Action_74( ctx );
  RtemsIoValKernelPrintflike_Action_75( ctx );
  RtemsIoValKernelPrintflike_Action_76( ctx );
  RtemsIoValKernelPrintflike_Action_77( ctx );
  RtemsIoValKernelPrintflike_Action_78( ctx );
  RtemsIoValKernelPrintflike_Action_79( ctx );

  T_add_remark( &RtemsIoValKernelPrintflike_Remark );
  T_pop_fixture();
}

/** @} */
