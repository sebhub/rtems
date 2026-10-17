/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuAarch64ValException
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
#include <rtems/score/cpuimpl.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuAarch64ValException spec:/score/cpu/aarch64/val/exception
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Tests the default exception handling.
 *
 * This test case performs the following actions:
 *
 * - Initialize the exception frame. Do a system call. Save the exception
 *   frame. Return to the action.
 *
 *   - Check that the exception frame has the expected vector number.
 *
 * @{
 */

#define _RTEMS_TMTEST27
#include <tm27.h>

static CPU_Exception_frame exception_frame;

static void Fatal(
  rtems_fatal_source source,
  rtems_fatal_code   code,
  void              *arg
)
{
  (void) arg;

  SetFatalHandler( NULL, NULL );

  if ( source == RTEMS_FATAL_SOURCE_EXCEPTION ) {
    CPU_Exception_frame *frame;

    frame = (CPU_Exception_frame *) code;
    exception_frame = *frame;

    _CPU_Exception_resume( frame );
  }
}

static void Initialize( void )
{
  memset( &exception_frame, 0xff, sizeof( exception_frame ) );
  SetFatalHandler( Fatal, NULL );
}

/**
 * @brief Initialize the exception frame. Do a system call. Save the exception
 *   frame. Return to the action.
 */
static void ScoreCpuAarch64ValException_Action_0( void )
{
  Initialize();
  __asm__ volatile( "svc #0"
                    :
                    :
                    : "memory" );

  /*
   * Check that the exception frame has the expected vector number.
   */
  T_eq_int( exception_frame.vector, 4 );
}

/**
 * @fn void T_case_body_ScoreCpuAarch64ValException( void )
 */
T_TEST_CASE( ScoreCpuAarch64ValException )
{
  ScoreCpuAarch64ValException_Action_0();
}

/** @} */
