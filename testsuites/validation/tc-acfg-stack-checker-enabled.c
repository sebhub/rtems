/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup AcfgValStackCheckerEnabled
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

#include <rtems.h>
#include <string.h>
#include <rtems/stackchk.h>

#include <rtems/test.h>

/**
 * @defgroup AcfgValStackCheckerEnabled spec:/acfg/val/stack-checker-enabled
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @brief Tests the effect of the CONFIGURE_STACK_CHECKER_ENABLED application
 *   configuration option.
 *
 * This test case performs the following actions:
 *
 * - Visit the stacks of the threads and the interrupt stack of each processor
 *   owned by a scheduler.
 *
 *   - Check that the stack usage of the executing thread is known.
 *
 *   - Check that the interrupt stack of each processor owned by a scheduler
 *     was visited.
 *
 * - Check the stack of the executing thread.
 *
 *   - Check that the stack checker found no problem.
 *
 * @{
 */

typedef struct {
  rtems_id  self;
  size_t    self_count;
  uintptr_t self_used;
  size_t    self_size;
  uint32_t  interrupt_count;
} Visitor;

static uint32_t CountOwnedProcessors( void )
{
  uint32_t cpu_index;
  uint32_t cpu_max;
  uint32_t count;

  cpu_max = rtems_scheduler_get_processor_maximum();
  count = 0;

  for ( cpu_index = 0; cpu_index < cpu_max; ++cpu_index ) {
    rtems_status_code sc;
    rtems_id          id;

    sc = rtems_scheduler_ident_by_processor( cpu_index, &id );

    if ( sc == RTEMS_SUCCESSFUL ) {
      ++count;
    }
  }

  return count;
}

static void Visit( const rtems_stack_checker_info *info, void *arg )
{
  Visitor *visitor;

  visitor = arg;

  if ( info->current == NULL ) {
    ++visitor->interrupt_count;
  } else if ( info->id == visitor->self ) {
    ++visitor->self_count;
    visitor->self_used = info->used;
    visitor->self_size = info->size;
  }
}

/**
 * @brief Visit the stacks of the threads and the interrupt stack of each
 *   processor owned by a scheduler.
 */
static void AcfgValStackCheckerEnabled_Action_0( void )
{
  Visitor visitor;

  memset( &visitor, 0, sizeof( visitor ) );
  visitor.self = rtems_task_self();
  rtems_stack_checker_iterate( Visit, &visitor );

  /*
   * Check that the stack usage of the executing thread is known.
   */
  T_step_eq_sz( 0, visitor.self_count, 1 );
  T_step_gt_uptr( 1, visitor.self_used, 0 );
  T_step_le_uptr( 2, visitor.self_used, visitor.self_size );

  /*
   * Check that the interrupt stack of each processor owned by a scheduler was
   * visited.
   */
  T_step_eq_u32( 3, visitor.interrupt_count, CountOwnedProcessors() );
}

/**
 * @brief Check the stack of the executing thread.
 */
static void AcfgValStackCheckerEnabled_Action_1( void )
{
  bool blown;

  blown = rtems_stack_checker_is_blown();

  /*
   * Check that the stack checker found no problem.
   */
  T_step_false( 4, blown );
}

/**
 * @fn void T_case_body_AcfgValStackCheckerEnabled( void )
 */
T_TEST_CASE( AcfgValStackCheckerEnabled )
{
  T_plan( 5 );

  AcfgValStackCheckerEnabled_Action_0();
  AcfgValStackCheckerEnabled_Action_1();
}

/** @} */
