/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsStackchkValReportUsageWithPlugin
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

#include <inttypes.h>
#include <rtems.h>
#include <string.h>
#include <rtems/bspIo.h>
#include <rtems/printer.h>
#include <rtems/stackchk.h>
#include <rtems/dev/io.h>

#include <rtems/test.h>

/**
 * @defgroup RtemsStackchkValReportUsageWithPlugin \
 *   spec:/rtems/stackchk/val/report-usage-with-plugin
 *
 * @ingroup TestsuitesValidation0
 * @ingroup TestsuitesValidationAcfg1
 *
 * @brief Tests the rtems_stack_checker_report_usage_with_plugin() and
 *   rtems_stack_checker_report_usage() directives.
 *
 * This test case performs the following actions:
 *
 * - Record the stack information of each visited stack. Call
 *   rtems_stack_checker_report_usage_with_plugin() with a printer which
 *   captures the output. Record the stack information of each visited stack
 *   again.
 *
 *   - Check that the output reached the printer.
 *
 *   - Check that the first line of the output is the title line and that the
 *     second line is the column line.
 *
 *   - Check that each further line describes the stack of the visit at the
 *     same position in the form which the format requirements state. Check
 *     that the output holds no other line.
 *
 * - Record the stack information of each visited stack. Call
 *   rtems_stack_checker_report_usage() while the kernel character output
 *   captures the output. Record the stack information of each visited stack
 *   again.
 *
 *   - Check that the output reached the kernel character output. Check that a
 *     carriage return precedes each newline character, which the printer of
 *     rtems_printk_printer() outputs. Remove the carriage returns.
 *
 *   - Check that the output is the output of
 *     rtems_stack_checker_report_usage_with_plugin().
 *
 * @{
 */

#define TITLE_LINE "                             STACK USAGE BY THREAD\n"

#define COLUMN_LINE \
  "ID         NAME                  LOW        HIGH       CURRENT     AVAIL   USED\n"

typedef struct {
  rtems_stack_checker_info info;
  char                     name[ 22 ];
} Stack;

typedef struct {
  size_t count;
  Stack  stacks[ 32 ];
} Snapshot;

typedef struct {
  char   buffer[ 4096 ];
  size_t size;
  bool   overflow;
} Output;

static Snapshot before;

static Snapshot after;

static Output output;

static void PutChar( int c, void *arg )
{
  Output *out;

  out = arg;

  if ( out->size < sizeof( out->buffer ) - 1 ) {
    out->buffer[ out->size ] = (char) c;
    ++out->size;
  } else {
    out->overflow = true;
  }
}

static int Capture( void *arg, const char *format, va_list ap )
{
  return _IO_Vprintf( PutChar, arg, format, ap );
}

static uintptr_t ParseDecimal( const char *s )
{
  uintptr_t value;

  while ( *s == ' ' ) {
    ++s;
  }

  value = 0;

  while ( *s >= '0' && *s <= '9' ) {
    value = 10 * value + (uintptr_t) ( *s - '0' );
    ++s;
  }

  return value;
}

static rtems_printer printer = { .context = &output, .printer = Capture };

static void Visit( const rtems_stack_checker_info *info, void *arg )
{
  Snapshot *snapshot;

  snapshot = arg;

  if ( snapshot->count < RTEMS_ARRAY_SIZE( snapshot->stacks ) ) {
    Stack *stack;

    stack = &snapshot->stacks[ snapshot->count ];
    stack->info = *info;
    strlcpy( stack->name, info->name, sizeof( stack->name ) );
  }

  ++snapshot->count;
}

static void Record( Snapshot *snapshot )
{
  snapshot->count = 0;
  rtems_stack_checker_iterate( Visit, snapshot );
}

static const char *NextLine( const char *line )
{
  const char *end;

  if ( line == NULL ) {
    return NULL;
  }

  end = strchr( line, '\n' );

  if ( end == NULL ) {
    return NULL;
  }

  return end + 1;
}

/*
 * The used stack area of a stack grows between the two records, so the used
 * field of the line lies between the two recorded values.
 */
static void CheckLine(
  const char  *line,
  const Stack *stack_before,
  const Stack *stack_after
)
{
  const rtems_stack_checker_info *info;
  char                            expected[ 128 ];
  char                            used[ 16 ];
  const char                     *end;
  int                             n;
  uintptr_t                       value;

  info = &stack_before->info;
  T_eq_u32( stack_after->info.id, info->id );
  n = T_snprintf(
    expected,
    sizeof( expected ),
    "0x%08" PRIx32 " %-21s 0x%08" PRIxPTR " 0x%08" PRIxPTR " 0x%08" PRIxPTR
    " %6" PRIuPTR " ",
    info->id,
    stack_before->name,
    (uintptr_t) info->begin,
    (uintptr_t) info->begin + info->size - 1,
    (uintptr_t) info->current,
    info->size
  );
  T_assert_lt_int( n, (int) sizeof( expected ) );
  T_eq_nstr( line, expected, (size_t) n );

  line += n;
  end = strchr( line, '\n' );
  T_assert_not_null( end );

  if ( info->used == UINTPTR_MAX ) {
    T_eq_uptr( stack_after->info.used, UINTPTR_MAX );
    T_eq_sz( (size_t) ( end - line ), 3 );
    T_eq_nstr( line, "N/A", 3 );
    return;
  }

  value = ParseDecimal( line );
  T_ge_uptr( value, info->used );
  T_le_uptr( value, stack_after->info.used );
  n = T_snprintf( used, sizeof( used ), "%6" PRIuPTR, value );
  T_eq_sz( (size_t) ( end - line ), (size_t) n );
  T_eq_nstr( line, used, (size_t) n );
}
static void PutKernelChar( char c )
{
  PutChar( (unsigned char) c, &output );
}

/*
 * The kernel character output turns each newline character into a carriage
 * return followed by a newline character.
 */
static bool RemoveCarriageReturns( void )
{
  size_t i;
  size_t j;
  bool   ok;

  ok = true;
  j = 0;

  for ( i = 0; i < output.size; ++i ) {
    char c;

    c = output.buffer[ i ];

    if ( c == '\r' ) {
      ok = ok && i + 1 < output.size && output.buffer[ i + 1 ] == '\n';
      continue;
    }

    if ( c == '\n' ) {
      ok = ok && i > 0 && output.buffer[ i - 1 ] == '\r';
    }

    output.buffer[ j ] = c;
    ++j;
  }

  output.size = j;
  output.buffer[ j ] = '\0';

  return ok;
}

static void CheckTitleLines( void )
{
  const char *line;

  line = output.buffer;
  T_eq_nstr( line, TITLE_LINE, sizeof( TITLE_LINE ) - 1 );
  line = NextLine( line );
  T_eq_nstr( line, COLUMN_LINE, sizeof( COLUMN_LINE ) - 1 );
}

static void CheckStackLines( void )
{
  const char *stack_line;
  size_t      i;

  stack_line = NextLine( NextLine( output.buffer ) );

  for ( i = 0; i < before.count; ++i ) {
    T_assert_not_null( stack_line );
    CheckLine( stack_line, &before.stacks[ i ], &after.stacks[ i ] );
    stack_line = NextLine( stack_line );
  }

  T_not_null( stack_line );
  T_eq_char( *stack_line, '\0' );
}

/**
 * @brief Record the stack information of each visited stack. Call
 *   rtems_stack_checker_report_usage_with_plugin() with a printer which
 *   captures the output. Record the stack information of each visited stack
 *   again.
 */
static void RtemsStackchkValReportUsageWithPlugin_Action_0( void )
{
  output.size = 0;
  output.overflow = false;
  Record( &before );
  rtems_stack_checker_report_usage_with_plugin( &printer );
  Record( &after );
  output.buffer[ output.size ] = '\0';
  T_false( output.overflow );
  T_le_sz( before.count, RTEMS_ARRAY_SIZE( before.stacks ) );
  T_eq_sz( after.count, before.count );

  /*
   * Check that the output reached the printer.
   */
  T_gt_sz( output.size, 0 );

  /*
   * Check that the first line of the output is the title line and that the
   * second line is the column line.
   */
  CheckTitleLines();

  /*
   * Check that each further line describes the stack of the visit at the same
   * position in the form which the format requirements state. Check that the
   * output holds no other line.
   */
  CheckStackLines();
}

/**
 * @brief Record the stack information of each visited stack. Call
 *   rtems_stack_checker_report_usage() while the kernel character output
 *   captures the output. Record the stack information of each visited stack
 *   again.
 */
static void RtemsStackchkValReportUsageWithPlugin_Action_1( void )
{
  BSP_output_char_function_type output_char;

  output.size = 0;
  output.overflow = false;
  Record( &before );
  output_char = BSP_output_char;
  BSP_output_char = PutKernelChar;
  rtems_stack_checker_report_usage();
  BSP_output_char = output_char;
  Record( &after );
  output.buffer[ output.size ] = '\0';
  T_false( output.overflow );
  T_le_sz( before.count, RTEMS_ARRAY_SIZE( before.stacks ) );
  T_eq_sz( after.count, before.count );

  /*
   * Check that the output reached the kernel character output. Check that a
   * carriage return precedes each newline character, which the printer of
   * rtems_printk_printer() outputs. Remove the carriage returns.
   */
  T_gt_sz( output.size, 0 );
  T_true( RemoveCarriageReturns() );

  /*
   * Check that the output is the output of
   * rtems_stack_checker_report_usage_with_plugin().
   */
  CheckTitleLines();
  CheckStackLines();
}

/**
 * @fn void T_case_body_RtemsStackchkValReportUsageWithPlugin( void )
 */
T_TEST_CASE( RtemsStackchkValReportUsageWithPlugin )
{
  RtemsStackchkValReportUsageWithPlugin_Action_0();
  RtemsStackchkValReportUsageWithPlugin_Action_1();
}

/** @} */
