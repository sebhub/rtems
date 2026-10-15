/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSPrintSupport
 *
 * @brief This header file provides the printer interfaces of the user print
 *   support.
 *
 * This include file defines the user interface to kernel print methods.
 */

/*
 * Copyright (C) 2016, 2026 embedded brains GmbH & Co. KG
 * Copyright (C) 2016 Chris Johns
 * Copyright (C) 1989, 2007 On-Line Applications Research Corporation (OAR)
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

/* Find related documentation with spec:/rtems/io/if/header-5 */

#ifndef _RTEMS_PRINTER_H
#define _RTEMS_PRINTER_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <rtems/chain.h>
#include <rtems/print.h>
#include <rtems/rtems/intr.h>
#include <rtems/rtems/tasks.h>
#include <rtems/rtems/types.h>
#include <rtems/score/isrlock.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/rtems/io/if/group-print-support-impl */

/**
 * @defgroup RTEMSImplPrintSupport Print Support
 *
 * @ingroup RTEMSImpl
 *
 * @brief This group contains the print support implementation.
 */

/* Find related documentation with spec:/rtems/io/if/print-printer */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Printer handlers shall have this type.
 *
 * Functions of this type can be plugged in to certain reporting routines to
 * redirect the output.
 *
 * Use the RTEMS Print interface to call these functions. Do not directly use
 * them.
 *
 * If the user provides their own printer, then they may redirect those reports
 * as they see fit.
 */
typedef int ( *rtems_print_printer )( void *, const char *, va_list );

/* Find related documentation with spec:/rtems/io/if/print-printer-fprintf */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Initializes the printer to print through fprintf() to the file
 *   stream.
 *
 * @param[out] printer is the printer to initialize.
 *
 * @param[in] file is the file stream of the output.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
void rtems_print_printer_fprintf( rtems_printer *printer, FILE *file );

/* Find related documentation with spec:/rtems/io/if/print-printer-fprintf-putc */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Initializes the printer to print through fprintf() to an unbuffered
 *   file stream which outputs through rtems_putc().
 *
 * @param[out] printer is the printer to initialize.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
void rtems_print_printer_fprintf_putc( rtems_printer *printer );

/* Find related documentation with spec:/rtems/io/if/print-printer-printf */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Initializes the printer to print through printf().
 *
 * @param[out] printer is the printer to initialize.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
void rtems_print_printer_printf( rtems_printer *printer );

/* Find related documentation with spec:/rtems/io/if/print-printer-printk */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Initializes the printer to output through rtems_printk_printer().
 *
 * @param[out] printer is the printer to initialize.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
void rtems_print_printer_printk( rtems_printer *printer );

/* Find related documentation with spec:/rtems/io/if/printer */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief This structure represents a printer handler with an associated
 *   printer context.
 */
struct rtems_printer {
  /**
   * @brief This member references the printer context used for the printer.
   */
  void *context;

  /**
   * @brief This member references the printer handler.
   */
  rtems_print_printer printer;
};

/* Find related documentation with spec:/rtems/io/if/print-printer-empty */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Initializes the printer to print nothing.
 *
 * @param[out] printer is the printer to initialize.
 *
 * @par Notes
 * An empty printer prints nothing. It can be used to enable and disable the
 * output of a print implementation.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
static inline void rtems_print_printer_empty( rtems_printer *printer )
{
  printer->context = NULL;
  printer->printer = NULL;
}

/* Find related documentation with spec:/rtems/io/if/print-printer-valid */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Checks if the printer is valid.
 *
 * @param[in] printer is the printer to check.
 *
 * @return Returns true, if the printer is not NULL and the printer handler of
 *   the printer is not NULL, otherwise false.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
static inline bool rtems_print_printer_valid( const rtems_printer *printer )
{
  return printer != NULL && printer->printer != NULL;
}

/* Find related documentation with spec:/rtems/io/if/printer-task-context */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief This structure provides the context of a printer task.
 *
 * Use rtems_printer_task_initialize() and the setter directives to set up the
 * context.
 */
typedef struct {
  /**
   * @brief This member contains the identifier of the printer task.
   */
  rtems_id task;

  #if ISR_LOCK_NEEDS_OBJECT
  /**
   * @brief This member provides the lock which protects the buffer chains.
   */
  rtems_interrupt_lock lock;
  #endif

  /**
   * @brief This member contains the chain of free buffers.
   */
  rtems_chain_control free_buffers;

  /**
   * @brief This member contains the work queue of buffers which the printer task
   *   writes.
   */
  rtems_chain_control todo_buffers;

  /**
   * @brief This member contains the task stack size of the printer task.
   */
  size_t task_stack_size;

  /**
   * @brief This member contains the task priority of the printer task.
   */
  rtems_task_priority task_priority;

  /**
   * @brief This member contains the file descriptor to which the printer task
   *   writes the output.
   */
  int fd;

  /**
   * @brief This member references the begin of the buffer table.
   */
  void *buffer_table;

  /**
   * @brief This member contains the count of buffers in the buffer table.
   */
  size_t buffer_count;

  /**
   * @brief This member contains the size in bytes of one buffer.
   */
  size_t buffer_size;
} rtems_printer_task_context;

/* Find related documentation with spec:/rtems/io/if/print-printer-task */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Creates a printer task.
 *
 * @param[out] printer is the printer to initialize.
 *
 * @param[in] context is the initialized printer task context.
 *
 * Print requests through rtems_printf() or rtems_vprintf() with a printer task
 * printer output to a buffer. The directive places the buffer on a work queue
 * in FIFO order. The printer task empties the work queue. It writes the
 * content of each buffer to the file descriptor of the context. The buffers
 * come from the pool of buffers which the context specifies.
 *
 * @return Returns 0 for a successful operation, EINVAL for invalid context
 *   parameters and ENOMEM for missing resources.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within task context.
 * @endparblock
 */
int rtems_print_printer_task(
  rtems_printer              *printer,
  rtems_printer_task_context *context
);

/* Find related documentation with spec:/rtems/io/if/printer-task-drain */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Drains the work queue of the printer task.
 *
 * @param[in] context is the printer task context of a successfully started
 *   printer task.
 *
 * The directive waits until the printer task wrote all output buffers of the
 * work queue at the time of the call to the file descriptor and an fsync()
 * completed.
 *
 * @par Notes
 * The printer task shall be successfully started through
 * rtems_print_printer_task() before the directive is called. Otherwise, the
 * behaviour is undefined.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within task context.
 * @endparblock
 */
void rtems_printer_task_drain( rtems_printer_task_context *context );

/* Find related documentation with spec:/rtems/io/if/printer-task-initialize */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Initializes the printer task context.
 *
 * @param[out] context is the printer task context to initialize.
 *
 * @par Notes
 * The directive sets all members of the context to zero.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
static inline void rtems_printer_task_initialize(
  rtems_printer_task_context *context
)
{
  /*
   * Some C++ compiler think that the structure is complex enough to need a
   * proper constructor. Cast to void * to silence a warning.
   */
  memset( (void *) context, 0, sizeof( *context ) );
}

/* Find related documentation with spec:/rtems/io/if/printer-task-set-buffer-count */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Sets the buffer count of the printer task context.
 *
 * @param[out] context is the printer task context.
 *
 * @param[in] buffer_count is the count of buffers in the buffer table.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
static inline void rtems_printer_task_set_buffer_count(
  rtems_printer_task_context *context,
  size_t                      buffer_count
)
{
  context->buffer_count = buffer_count;
}

/* Find related documentation with spec:/rtems/io/if/printer-task-set-buffer-size */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Sets the buffer size of the printer task context.
 *
 * @param[out] context is the printer task context.
 *
 * @param[in] buffer_size is the size in bytes of one buffer.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
static inline void rtems_printer_task_set_buffer_size(
  rtems_printer_task_context *context,
  size_t                      buffer_size
)
{
  context->buffer_size = buffer_size;
}

/* Find related documentation with spec:/rtems/io/if/printer-task-set-buffer-table */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Sets the buffer table of the printer task context.
 *
 * @param[out] context is the printer task context.
 *
 * @param[in] buffer_table is the begin of the buffer table.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
static inline void rtems_printer_task_set_buffer_table(
  rtems_printer_task_context *context,
  void                       *buffer_table
)
{
  context->buffer_table = buffer_table;
}

/* Find related documentation with spec:/rtems/io/if/printer-task-set-file-descriptor */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Sets the file descriptor of the printer task context.
 *
 * @param[out] context is the printer task context.
 *
 * @param[in] fd is the file descriptor to which the printer task writes the
 *   output.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
static inline void rtems_printer_task_set_file_descriptor(
  rtems_printer_task_context *context,
  int                         fd
)
{
  context->fd = fd;
}

/* Find related documentation with spec:/rtems/io/if/printer-task-set-priority */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Sets the task priority of the printer task context.
 *
 * @param[out] context is the printer task context.
 *
 * @param[in] priority is the task priority of the printer task.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
static inline void rtems_printer_task_set_priority(
  rtems_printer_task_context *context,
  rtems_task_priority         priority
)
{
  context->task_priority = priority;
}

/* Find related documentation with spec:/rtems/io/if/printer-task-set-stack-size */

/**
 * @ingroup RTEMSPrintSupport
 *
 * @brief Sets the task stack size of the printer task context.
 *
 * @param[out] context is the printer task context.
 *
 * @param[in] stack_size is the task stack size of the printer task.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * - The directive is not included in the pre-qualified feature set of RTEMS.
 *   Applications which are restricted to only use interfaces of the
 *   pre-qualified feature set of RTEMS shall not use the directive.
 *
 * - The directive may be called from within any runtime context.
 *
 * - The directive will not cause the calling task to be preempted.
 * @endparblock
 */
static inline void rtems_printer_task_set_stack_size(
  rtems_printer_task_context *context,
  size_t                      stack_size
)
{
  context->task_stack_size = stack_size;
}

#ifdef __cplusplus
}
#endif

#endif /* _RTEMS_PRINTER_H */
