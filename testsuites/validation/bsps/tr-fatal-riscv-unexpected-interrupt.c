/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspRiscvValFatalUnexpectedInterrupt
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

#include <bsp.h>
#include <rtems.h>
#include <bsp/fatal.h>
#include <rtems/sysinit.h>
#include <rtems/score/cpuimpl.h>
#include <rtems/score/percpu.h>

#include "tr-fatal-riscv-unexpected-interrupt.h"

#include <rtems/test.h>

/**
 * @defgroup BspRiscvValFatalUnexpectedInterrupt \
 *   spec:/bsp/riscv/val/fatal-unexpected-interrupt
 *
 * @ingroup TestsuitesBspsFatalRiscvUnexpectedInterrupt
 *
 * @brief Tests a fatal error.
 *
 * This test case performs the following actions:
 *
 * - The test action is carried out by the DispatchUnexpectedInterrupt() system
 *   initialization handler.
 *
 *   - Check that the expected fatal source is present.
 *
 *   - Check that the expected fatal code is present.
 *
 * @{
 */

/**
 * @brief Test context for spec:/bsp/riscv/val/fatal-unexpected-interrupt test
 *   case.
 */
typedef struct {
  /**
   * @brief This member contains a copy of the corresponding
   *   BspRiscvValFatalUnexpectedInterrupt_Run() parameter.
   */
  rtems_fatal_source source;

  /**
   * @brief This member contains a copy of the corresponding
   *   BspRiscvValFatalUnexpectedInterrupt_Run() parameter.
   */
  rtems_fatal_code code;
} BspRiscvValFatalUnexpectedInterrupt_Context;

static BspRiscvValFatalUnexpectedInterrupt_Context
  BspRiscvValFatalUnexpectedInterrupt_Instance;

static void DispatchUnexpectedInterrupt( void )
{
  rtems_interrupt_level level;
  uintptr_t             cause;

  /*
   * The interrupt cause zero is the user software interrupt.  No processor
   * mode of the BSP handles it.
   */
  cause = (uintptr_t) 1 << ( 8 * sizeof( uintptr_t ) - 1 );

  rtems_interrupt_local_disable( level );
  _RISCV_Interrupt_dispatch( cause, _Per_CPU_Get() );
  rtems_interrupt_local_enable( level );
}

RTEMS_SYSINIT_ITEM(
  DispatchUnexpectedInterrupt,
  RTEMS_SYSINIT_DEVICE_DRIVERS,
  RTEMS_SYSINIT_ORDER_FIRST
);

static T_fixture BspRiscvValFatalUnexpectedInterrupt_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = NULL,
  .initial_context = &BspRiscvValFatalUnexpectedInterrupt_Instance
};

/**
 * @brief The test action is carried out by the DispatchUnexpectedInterrupt()
 *   system initialization handler.
 */
static void BspRiscvValFatalUnexpectedInterrupt_Action_0(
  BspRiscvValFatalUnexpectedInterrupt_Context *ctx
)
{
  /* Nothing to do */

  /*
   * Check that the expected fatal source is present.
   */
  T_step_eq_int( 0, ctx->source, RTEMS_FATAL_SOURCE_BSP );

  /*
   * Check that the expected fatal code is present.
   */
  T_step_eq_ulong( 1, ctx->code, RISCV_FATAL_UNEXPECTED_INTERRUPT_EXCEPTION );
}

void BspRiscvValFatalUnexpectedInterrupt_Run(
  rtems_fatal_source source,
  rtems_fatal_code   code
)
{
  BspRiscvValFatalUnexpectedInterrupt_Context *ctx;

  ctx = &BspRiscvValFatalUnexpectedInterrupt_Instance;
  ctx->source = source;
  ctx->code = code;

  ctx = T_case_begin(
    "BspRiscvValFatalUnexpectedInterrupt",
    &BspRiscvValFatalUnexpectedInterrupt_Fixture
  );

  T_plan( 2 );

  BspRiscvValFatalUnexpectedInterrupt_Action_0( ctx );

  T_case_end();
}

/** @} */
