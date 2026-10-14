/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplStackChecker
 *
 * @brief This header file provides interfaces of the
 *   @ref RTEMSImplStackChecker implementation.
 */

/*
 * COPYRIGHT (c) 1989-2024 On-Line Applications Research Corporation (OAR).
 * COPYRIGHT (c) 2024 Mohamed Hassan <muhammad.hamdy.hassan@gmail.com>
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

#ifndef _RTEMS_STACKCHKIMPL_H
#define _RTEMS_STACKCHKIMPL_H

#include <rtems.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup RTEMSImplStackChecker Stack Checker
 *
 * @ingroup RTEMSImpl
 *
 * @brief This group contains the implementation of
 *   @ref libmisc_stackchk.
 *
 * @{
 */

/**
 * @brief Stack Checker Task Create Extension
 *
 * This method is the task create extension for the stack checker.
 *
 * @param[in] running points to the currently executing task
 *
 * @param[in] the_thread points to the newly created task
 *
 * @note If this this the first task created, the stack checker
 *       will automatically intialize itself.
 */
bool rtems_stack_checker_create_extension(
  rtems_tcb *running,
  rtems_tcb *the_thread
);

void rtems_stack_checker_begin_extension( rtems_tcb *executing );

/**
 * @brief Stack Checker Task Context Switch Extension
 *
 * This method is the task context switch extension for the stack checker.
 *
 * @param[in] running points to the currently executing task which
 *                    is being context switched out
 *
 * @param[in] heir points to the heir task which we are switching to
 *
 * @note This is called from the internal method _Thread_Dispatch.
 */
void rtems_stack_checker_switch_extension(
  rtems_tcb *running,
  rtems_tcb *heir
);

/**
 * @brief A Quiet Version of Stack Checker Reporter.
 *
 * @param[in] running running points to the currently executing thread which
 *                    is being context switched out.
 *
 * @param[in] pattern_ok bool variable to check if the pattern is
 *                       still valid or not
 */

void rtems_stack_checker_reporter_quiet(
  const rtems_tcb *running,
  bool             pattern_ok
);

/**
 * @brief The Default Function to Report a Blown Stack.
 *
 * @param[in] running running points to the currently executing thread which
 *                    is being context switched out.
 *
 * @param[in] pattern_ok bool variable to check if the pattern is
 *                       still valid or not
 */
void rtems_stack_checker_reporter_print_details(
  const rtems_tcb *running,
  bool             pattern_ok
);

/**
 *  @brief Stack Checker Extension Set Definition
 *
 *  This macro defines the user extension handler set for the stack
 *  checker. This macro is normally only used by confdefs.h.
 */
#define RTEMS_STACK_CHECKER_EXTENSION                              \
  {                                                                \
    rtems_stack_checker_create_extension, /* rtems_task_create  */ \
    0,                                    /* rtems_task_start   */ \
    0,                                    /* rtems_task_restart */ \
    0,                                    /* rtems_task_delete  */ \
    rtems_stack_checker_switch_extension, /* task_switch  */       \
    rtems_stack_checker_begin_extension,  /* task_begin   */       \
    0,                                    /* task_exitted */       \
    0,                                    /* fatal        */       \
    0                                     /* terminate    */       \
  }

/**
 * @brief The Stack Checker Reporter Initialization Handler.
 *
 * @param[in] running running points to the currently executing thread which
 *                    is being context switched out.
 *
 * @param[in] pattern_ok bool variable to check if the pattern is
 *                       still valid or not.
 */
typedef void ( *Stack_checker_Reporter_handler )(
  const rtems_tcb *running,
  bool             pattern_ok
);

/**
 * @brief The Stack Checker Reporter Initialization Handler.
 *
 * Application provided via <rtems/confdefs.h>
 */
extern const Stack_checker_Reporter_handler Stack_checker_Reporter;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _RTEMS_STACKCHKIMPL_H */
