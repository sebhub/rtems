/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSAPIStackChecker
 *
 * @brief This header file provides the stack checker API.
 */

/*
 * Copyright (C) 2024 Mohamed Hassan
 * Copyright (C) 2022, 2026 embedded brains GmbH & Co. KG
 * Copyright (C) 2016 Chris Johns
 * Copyright (C) 2007, 2008 On-Line Applications Research Corporation (OAR)
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

/*
 * The spec:/... comments allow you to identify content which is
 * duplicated in the rtems and rtems-docs repositories.
 */

/* Find related documentation with spec:/rtems/stackchk/if/header */

#ifndef _RTEMS_STACKCHK_H
#define _RTEMS_STACKCHK_H

#include <stdbool.h>
#include <stdint.h>
#include <rtems/print.h>
#include <rtems/rtems/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/rtems/stackchk/if/group */

/**
 * @defgroup RTEMSAPIStackChecker Stack Checker
 *
 * @ingroup RTEMSAPIClassic
 *
 * @brief The stack checker directives can be used to check the stack of the
 *   executing thread and to report the stack usage of the threads and the
 *   interrupt stacks.
 */

/* Find related documentation with spec:/rtems/stackchk/if/info */

/**
 * @ingroup RTEMSAPIStackChecker
 *
 * @brief This structure provides the information of a stack.
 */
typedef struct {
  /**
   * @brief This member contains the object identifier of the thread which uses
   *   the stack.
   *
   * For an interrupt stack, the member contains the processor index.
   */
  rtems_id id;

  /**
   * @brief This member references the name of the object which uses the stack.
   *
   * For an interrupt stack, the member references the string "Interrupt Stack".
   */
  const char *name;

  /**
   * @brief This member references the begin of the allocated stack area without
   *   the stack checker pattern area.
   */
  const void *begin;

  /**
   * @brief This member contains the size in bytes of the allocated stack area
   *   without the stack checker pattern area.
   */
  uintptr_t size;

  /**
   * @brief This member references the stack pointer of the stack.
   *
   * For a thread, the member contains the stack pointer of the saved thread
   * context. For an interrupt stack, the member is NULL.
   */
  const void *current;

  /**
   * @brief This member contains the maximum size in bytes which the used stack
   *   area reached.
   *
   * In case the stack checker is not initialized, the member is UINTPTR_MAX.
   */
  uintptr_t used;
} rtems_stack_checker_info;

/* Find related documentation with spec:/rtems/stackchk/if/is-blown */

/**
 * @ingroup RTEMSAPIStackChecker
 *
 * @brief Checks the stack of the executing thread.
 *
 * The directive checks that the stack pointer of the caller is inside the
 * allocated stack area of the executing thread. It checks that the stack
 * checker pattern area of this stack and the stack checker pattern area of the
 * interrupt stack of the current processor hold the pattern.
 *
 * @retval false The checks passed.
 *
 * @par Notes
 * The directive checks the stack of the executing thread before the interrupt
 * stack. A check of the stack of the executing thread which fails terminates
 * the system with the fatal source RTEMS_FATAL_SOURCE_STACK_CHECKER and the
 * object name of the executing thread as the fatal code. A check of the
 * interrupt stack which fails terminates the system with the fatal source
 * RTEMS_FATAL_SOURCE_STACK_CHECKER and the object name built from the
 * characters `INTR` as the fatal code. The directive therefore returns only
 * when all checks pass.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive may be called from within device driver initialization
 *   context.
 *
 * - The directive may be called from within task context.
 * @endparblock
 */
bool rtems_stack_checker_is_blown( void );

/* Find related documentation with spec:/rtems/stackchk/if/report-usage */

/**
 * @ingroup RTEMSAPIStackChecker
 *
 * @brief Reports the stack usage of the threads and the interrupt stacks using
 *   the printk() printer.
 *
 * @par Notes
 * See also rtems_stack_checker_report_usage_with_plugin().
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive may be called from within device driver initialization
 *   context.
 *
 * - The directive may be called from within task context.
 *
 * - The directive may obtain and release the object allocator mutex. This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
void rtems_stack_checker_report_usage( void );

/* Find related documentation with spec:/rtems/stackchk/if/report-usage-with-plugin */

/**
 * @ingroup RTEMSAPIStackChecker
 *
 * @brief Reports the stack usage of the threads and the interrupt stacks using
 *   the printer plugin.
 *
 * @param printer is the printer plugin to output the report.
 *
 * The directive outputs the report through the printer. Each line of the
 * report ends with a newline character. The first line holds the title of the
 * report. The second line holds the column titles ID, NAME, LOW, HIGH,
 * CURRENT, AVAIL and USED.
 *
 * Each further line describes one stack which rtems_stack_checker_iterate()
 * visits, in the order of the visits. One space character separates two fields
 * of the line. The fields are the following:
 *
 * - the id member as `0x` followed by hexadecimal digits padded on the left
 *   with zeros to eight digits,
 *
 * - the name member padded on the right with space characters to 21
 *   characters,
 *
 * - the begin member as `0x` followed by hexadecimal digits padded on the left
 *   with zeros to eight digits,
 *
 * - the address of the last byte of the area which the begin and size members
 *   describe as `0x` followed by hexadecimal digits padded on the left with
 *   zeros to eight digits,
 *
 * - the current member as `0x` followed by hexadecimal digits padded on the
 *   left with zeros to eight digits,
 *
 * - the size member as a decimal number padded with space characters to six
 *   characters, and
 *
 * - the used member as a decimal number padded with space characters to six
 *   characters, or `N/A` where the member is UINTPTR_MAX.
 *
 * A hexadecimal digit is a digit or a lower case letter. A decimal number is
 * padded on the left. A field of a member which exceeds the size takes the
 * digits or characters which it needs.
 *
 * @par Notes
 * See also rtems_stack_checker_report_usage().
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive may be called from within device driver initialization
 *   context.
 *
 * - The directive may be called from within task context.
 *
 * - The directive may obtain and release the object allocator mutex. This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
void rtems_stack_checker_report_usage_with_plugin(
  const rtems_printer *printer
);

/* Find related documentation with spec:/rtems/stackchk/if/visitor */

/**
 * @ingroup RTEMSAPIStackChecker
 *
 * @brief Visitor routines invoked by rtems_stack_checker_iterate() shall have
 *   this type.
 *
 * The first parameter is the stack information. The second parameter is the
 * argument passed to rtems_stack_checker_iterate().
 */
typedef void ( *rtems_stack_checker_visitor )(
  const rtems_stack_checker_info *,
  void *
);

/* Find related documentation with spec:/rtems/stackchk/if/iterate */

/**
 * @ingroup RTEMSAPIStackChecker
 *
 * @brief Iterates over the stacks of the threads and the interrupt stacks and
 *   invokes the visitor routine for each stack.
 *
 * @param visit is the visitor routine invoked for each stack.
 *
 * @param arg is the argument passed to each visitor routine invocation.
 *
 * The directive invokes the visitor routine specified by `visit` for the stack
 * of each thread. It then invokes the visitor routine for the interrupt stack
 * of each configured processor. It skips a stack which has no allocated stack
 * area. The interrupt stack of a processor has its allocated stack area after
 * the first thread started on that processor.
 *
 * @par Notes
 * The directive holds the object allocator lock while it invokes the visitor
 * routine for the stacks of the threads.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive may be called from within device driver initialization
 *   context.
 *
 * - The directive may be called from within task context.
 *
 * - The directive may obtain and release the object allocator mutex. This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
void rtems_stack_checker_iterate(
  rtems_stack_checker_visitor visit,
  void                       *arg
);

#ifdef __cplusplus
}
#endif

#endif /* _RTEMS_STACKCHK_H */
