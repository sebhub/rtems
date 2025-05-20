/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup AcfgValInitTaskStackSize
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
#include <rtems/test-support.h>
#include <rtems/rtems/tasksdata.h>
#include <rtems/score/threadimpl.h>

#include "ts-config.h"

#include <rtems/test.h>

/**
 * @defgroup AcfgValInitTaskStackSize spec:/acfg/val/init-task-stack-size
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @brief Tests the task stack size of the Classic API initialization task
 *   which the CONFIGURE_INIT_TASK_STACK_SIZE application configuration option
 *   defines.
 *
 * The test suite defines CONFIGURE_INIT_TASK_STACK_SIZE to
 * `TEST_INIT_TASK_STACK_SIZE`. This value is greater than the value of
 * CONFIGURE_MINIMUM_TASK_STACK_SIZE, which is the default value of the option.
 * The checks fail if the system ignores the option.
 *
 * This test case performs the following actions:
 *
 * - Get the stack size of the Classic API initialization task.
 *
 *   - Check that the task stack size of the Classic API initialization task in
 *     the initialization task table is the configured value.
 *
 *   - Check that the stack of the Classic API initialization task is at least
 *     as large as the configured value.
 *
 * @{
 */

/**
 * @brief Get the stack size of the Classic API initialization task.
 */
static void AcfgValInitTaskStackSize_Action_0( void )
{
  rtems_status_code     sc;
  rtems_id              id;
  const Thread_Control *executing;
  size_t                stack_size;

  sc = rtems_task_ident( TEST_RUNNER_NAME, RTEMS_SEARCH_LOCAL_NODE, &id );
  T_assert_rsc_success( sc );
  T_assert_eq_u32( id, rtems_task_self() );
  executing = _Thread_Get_executing();
  stack_size = executing->Start.Initial_stack.size;

  /*
   * Check that the task stack size of the Classic API initialization task in
   * the initialization task table is the configured value.
   */
  T_eq_sz(
    _RTEMS_tasks_User_task_table.stack_size,
    TEST_INIT_TASK_STACK_SIZE
  );

  /*
   * Check that the stack of the Classic API initialization task is at least as
   * large as the configured value.
   */
  T_ge_sz( stack_size, TEST_INIT_TASK_STACK_SIZE );
}

/**
 * @fn void T_case_body_AcfgValInitTaskStackSize( void )
 */
T_TEST_CASE( AcfgValInitTaskStackSize )
{
  AcfgValInitTaskStackSize_Action_0();
}

/** @} */
