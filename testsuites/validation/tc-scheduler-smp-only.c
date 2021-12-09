/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSTestCaseRtemsSchedulerValSmpOnly
 */

/*
 * Copyright (C) 2021 embedded brains GmbH (http://www.embedded-brains.de)
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
 * This file is part of the RTEMS quality process and was automatically
 * generated.  If you find something that needs to be fixed or
 * worded better please post a report or patch to an RTEMS mailing list
 * or raise a bug report:
 *
 * https://www.rtems.org/bugs.html
 *
 * For information on updating and regenerating please refer to the How-To
 * section in the Software Requirements Engineering chapter of the
 * RTEMS Software Engineering manual.  The manual is provided as a part of
 * a release.  For development sources please refer to the online
 * documentation at:
 *
 * https://docs.rtems.org
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RTEMSTestCaseRtemsSchedulerValSmpOnly \
 *   spec:/rtems/scheduler/val/smp-only
 *
 * @ingroup RTEMSTestSuiteTestsuitesValidationSmpOnly0
 *
 * @brief This test case collection provides validation test cases for SMP-only
 *   requirements of the @ref RTEMSAPIClassicScheduler.
 *
 * This test case performs the following actions:
 *
 * - Call rtems_scheduler_get_processor() on all online processors and check
 *   the returned value.
 *
 * - Call rtems_scheduler_get_processor_maximum() and check the returned value.
 *
 *   - Check that the returned value is greater than or equal to one.
 *
 *   - Check that the returned value is less than or equal to
 *     rtems_configuration_get_maximum_processors().
 *
 * @{
 */

/**
 * @brief Call rtems_scheduler_get_processor() on all online processors and
 *   check the returned value.
 */
static void RtemsSchedulerValSmpOnly_Action_0( void )
{
  rtems_id            scheduler_id;
  rtems_task_priority priority;
  uint32_t            cpu_index;
  uint32_t            cpu_max;

  scheduler_id = GetSelfScheduler();
  priority = GetSelfPriority();
  cpu_max = rtems_scheduler_get_processor_maximum();
  T_step_ge_u32( 0, cpu_max, 1 );

  for ( cpu_index = 0; cpu_index < cpu_max; ++cpu_index ) {
    rtems_status_code sc;
    rtems_id          id;

    sc = rtems_scheduler_ident_by_processor( cpu_index, &id );
    T_quiet_rsc_success( sc );

    SetSelfScheduler( id, priority );
    SetSelfAffinityOne( cpu_index );

    T_quiet_eq_u32( rtems_scheduler_get_processor(), cpu_index );

    SetSelfAffinityAll();
  }

  SetSelfScheduler( scheduler_id, priority );
}

/**
 * @brief Call rtems_scheduler_get_processor_maximum() and check the returned
 *   value.
 */
static void RtemsSchedulerValSmpOnly_Action_1( void )
{
  uint32_t cpu_max;

  cpu_max = rtems_scheduler_get_processor_maximum();

  /*
   * Check that the returned value is greater than or equal to one.
   */
  T_step_ge_u32( 1, cpu_max, 1 );

  /*
   * Check that the returned value is less than or equal to
   * rtems_configuration_get_maximum_processors().
   */
  T_step_le_u32(
    2,
    cpu_max,
    rtems_configuration_get_maximum_processors()
  );
}

/**
 * @fn void T_case_body_RtemsSchedulerValSmpOnly( void )
 */
T_TEST_CASE( RtemsSchedulerValSmpOnly )
{
  T_plan( 3 );

  RtemsSchedulerValSmpOnly_Action_0();
  RtemsSchedulerValSmpOnly_Action_1();
}

/** @} */
