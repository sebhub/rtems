/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSTestCaseAcfgValOneCpu
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

#include "ts-config.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RTEMSTestCaseAcfgValOneCpu spec:/acfg/val/one-cpu
 *
 * @ingroup RTEMSTestSuiteTestsuitesValidationOneCpu0
 *
 * @brief Tests the effect of application configuration options.
 *
 * This test case performs the following actions:
 *
 * - Check the effect of application configuration options.
 *
 *   - Check the configured CONFIGURE_MAXIMUM_PRIORITY value using
 *     /rtems/task/if/maximum-priority.  This configuration option affects only
 *     the Deterministic Priority Scheduler which was explicitly configured by
 *     CONFIGURE_SCHEDULER_PRIORITY in the test suite.
 *
 *   - Check the configured CONFIGURE_MAXIMUM_PRIORITY value using
 *     /rtems/task/if/maximum-priority.  This configuration option affects only
 *     the Deterministic Priority Scheduler which was explicitly configured by
 *     CONFIGURE_SCHEDULER_PRIORITY in the test suite.
 *
 * @{
 */

/**
 * @brief Check the effect of application configuration options.
 */
static void AcfgValOneCpu_Action_0( void )
{
  rtems_status_code sc;
  rtems_id          id;

  /*
   * Check the configured CONFIGURE_MAXIMUM_PRIORITY value using
   * /rtems/task/if/maximum-priority.  This configuration option affects only
   * the Deterministic Priority Scheduler which was explicitly configured by
   * CONFIGURE_SCHEDULER_PRIORITY in the test suite.
   */
  T_eq_u32( RTEMS_MAXIMUM_PRIORITY, 127 );

  /*
   * Check the configured CONFIGURE_MAXIMUM_PRIORITY value using
   * /rtems/task/if/maximum-priority.  This configuration option affects only
   * the Deterministic Priority Scheduler which was explicitly configured by
   * CONFIGURE_SCHEDULER_PRIORITY in the test suite.
   */
  sc = rtems_scheduler_ident( TEST_SCHEDULER_A_NAME, &id );
  T_rsc_success( sc );
}

/**
 * @fn void T_case_body_AcfgValOneCpu( void )
 */
T_TEST_CASE( AcfgValOneCpu )
{
  AcfgValOneCpu_Action_0();
}

/** @} */
