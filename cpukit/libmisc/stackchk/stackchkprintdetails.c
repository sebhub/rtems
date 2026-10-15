/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplStackChecker
 *
 * @brief This source file contains the definition of
 *   rtems_stack_checker_reporter_print_details().
 */

/*
 *  COPYRIGHT (c) 1989-2024 On-Line Applications Research Corporation (OAR).
 *  COPYRIGHT (c) 2024 Mohamed Hassan <muhammad.hamdy.hassan@gmail.com>
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
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <inttypes.h>

#include <rtems/bspIo.h>
#include <rtems/stackchkimpl.h>
#include <rtems/score/threadimpl.h>

/*
 *  rtems_stack_checker_reporter_print_details
 *
 *  Report a blown stack. Needs to be a separate routine
 *  so that interrupt handlers can use this too.
 *
 *  NOTE: The system is in a questionable state... we may not get
 *        the following message out.
 */
void rtems_stack_checker_reporter_print_details(
  const Thread_Control *running,
  bool                  pattern_ok
)
{
  const Stack_Control *stack = &running->Start.Initial_stack;
  void                *pattern_area = _Stack_check_Get_pattern( stack );
  char                 name[ 2 * THREAD_DEFAULT_MAXIMUM_NAME_SIZE ];

  printk( "BLOWN STACK!!!\n" );
  printk( "task control block: 0x%08" PRIxPTR "\n", (intptr_t) running );
  printk( "task ID: 0x%08lx\n", (unsigned long) running->Object.id );
  printk( "task name: 0x%08" PRIx32 "\n", running->Object.name.name_u32 );
  _Thread_Get_name( running, name, sizeof( name ) );
  printk( "task name string: %s\n", name );
  printk(
    "task stack area (%lu Bytes): 0x%08" PRIxPTR " .. 0x%08" PRIxPTR "\n",
    (unsigned long) stack->size,
    (intptr_t) stack->area,
    (intptr_t) ( (char *) stack->area + stack->size )
  );
  if ( !pattern_ok ) {
    printk(
      "damaged pattern area (%lu Bytes): 0x%08" PRIxPTR " .. 0x%08" PRIxPTR
      "\n",
      (unsigned long) STACK_CHECK_PATTERN_SIZE,
      (intptr_t) pattern_area,
      (intptr_t) ( pattern_area + STACK_CHECK_PATTERN_SIZE )
    );
  }

  #if defined( RTEMS_MULTIPROCESSING )
  printk(
    "node: 0x%08" PRIxPTR "\n",
    (intptr_t) rtems_configuration_get_user_multiprocessing_table()->node
  );
  #endif

  rtems_fatal(
    RTEMS_FATAL_SOURCE_STACK_CHECKER,
    running->Object.name.name_u32
  );
}
