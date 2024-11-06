/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspPowerpcQoriqReqGpioSetDrive
 */

/*
 * Copyright (C) 2025 embedded brains GmbH & Co. KG
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

#include <bsp/qoriq-gpio.h>

#include <rtems/test.h>

#if !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT)

/**
 * @defgroup BspPowerpcQoriqReqGpioSetDrive \
 *   spec:/bsp/powerpc/qoriq/req/gpio-set-drive
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @{
 */

typedef enum {
  BspPowerpcQoriqReqGpioSetDrive_Pre_Drive_PushPull,
  BspPowerpcQoriqReqGpioSetDrive_Pre_Drive_OpenDrain,
  BspPowerpcQoriqReqGpioSetDrive_Pre_Drive_NA
} BspPowerpcQoriqReqGpioSetDrive_Pre_Drive;

typedef enum {
  BspPowerpcQoriqReqGpioSetDrive_Post_Drive_PushPull,
  BspPowerpcQoriqReqGpioSetDrive_Post_Drive_OpenDrain,
  BspPowerpcQoriqReqGpioSetDrive_Post_Drive_NA
} BspPowerpcQoriqReqGpioSetDrive_Post_Drive;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Drive_NA : 1;
  uint8_t Post_Drive : 2;
} BspPowerpcQoriqReqGpioSetDrive_Entry;

/**
 * @brief Test context for spec:/bsp/powerpc/qoriq/req/gpio-set-drive test
 *   case.
 */
typedef struct {
  /**
   * @brief This member provides the GPIO registers.
   */
  qoriq_gpio gpio;

  /**
   * @brief This member provides the GPIO controller.
   */
  qoriq_gpio_control controller;

  /**
   * @brief This member provides the GPIO pin.
   */
  qoriq_gpio_pin pin;

  /**
   * @brief This member provides the GPIO drive.
   */
  qoriq_gpio_drive drive;

  struct {
    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 1 ];

    /**
     * @brief If this member is true, then the test action loop is executed.
     */
    bool in_action_loop;

    /**
     * @brief This member contains the next transition map index.
     */
    size_t index;

    /**
     * @brief This member contains the current transition map entry.
     */
    BspPowerpcQoriqReqGpioSetDrive_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} BspPowerpcQoriqReqGpioSetDrive_Context;

static BspPowerpcQoriqReqGpioSetDrive_Context
  BspPowerpcQoriqReqGpioSetDrive_Instance;

static const char * const BspPowerpcQoriqReqGpioSetDrive_PreDesc_Drive[] = {
  "PushPull",
  "OpenDrain",
  "NA"
};

static const char * const * const BspPowerpcQoriqReqGpioSetDrive_PreDesc[] = {
  BspPowerpcQoriqReqGpioSetDrive_PreDesc_Drive,
  NULL
};

static void BspPowerpcQoriqReqGpioSetDrive_Pre_Drive_Prepare(
  BspPowerpcQoriqReqGpioSetDrive_Context  *ctx,
  BspPowerpcQoriqReqGpioSetDrive_Pre_Drive state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqGpioSetDrive_Pre_Drive_PushPull: {
      /*
       * While the drive specified by the ``direction`` parameter is
       * QORIQ_GPIO_PUSHPULL.
       */
      ctx->drive = QORIQ_GPIO_PUSHPULL;
      ctx->gpio.gpodr = 0xffffffff;
      break;
    }

    case BspPowerpcQoriqReqGpioSetDrive_Pre_Drive_OpenDrain: {
      /*
       * While the drive specified by the ``direction`` parameter is
       * QORIQ_GPIO_OPENDRAIN.
       */
      ctx->drive = QORIQ_GPIO_OPENDRAIN;
      ctx->gpio.gpodr = 0;
      break;
    }

    case BspPowerpcQoriqReqGpioSetDrive_Pre_Drive_NA:
      break;
  }
}

static void BspPowerpcQoriqReqGpioSetDrive_Post_Drive_Check(
  BspPowerpcQoriqReqGpioSetDrive_Context   *ctx,
  BspPowerpcQoriqReqGpioSetDrive_Post_Drive state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqGpioSetDrive_Post_Drive_PushPull: {
      /*
       * The bits of the GPIO Open Drain Register (GPIO_GPODR) associated with
       * the pin referenced by the ``pin`` parameter shall be cleared.
       */
      T_eq_u32( ctx->gpio.gpodr, ~( 1U << ( 31 - 13 ) ) );
      break;
    }

    case BspPowerpcQoriqReqGpioSetDrive_Post_Drive_OpenDrain: {
      /*
       * The bits of the GPIO Open Drain Register (GPIO_GPODR) associated with
       * the pin referenced by the ``pin`` parameter shall be set.
       */
      T_eq_u32( ctx->gpio.gpodr, 1U << ( 31 - 13 ) );
      break;
    }

    case BspPowerpcQoriqReqGpioSetDrive_Post_Drive_NA:
      break;
  }
}

static void BspPowerpcQoriqReqGpioSetDrive_Setup(
  BspPowerpcQoriqReqGpioSetDrive_Context *ctx
)
{
  ctx->controller.gpio = &ctx->gpio;
  rtems_interrupt_lock_initialize( &ctx->controller.lock, "GPIO" );
  ctx->pin.ctrl = &ctx->controller;
  ctx->pin.mask = QORIQ_GPIO_PIN( 13 );
}

static void BspPowerpcQoriqReqGpioSetDrive_Setup_Wrap( void *arg )
{
  BspPowerpcQoriqReqGpioSetDrive_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  BspPowerpcQoriqReqGpioSetDrive_Setup( ctx );
}

static void BspPowerpcQoriqReqGpioSetDrive_Action(
  BspPowerpcQoriqReqGpioSetDrive_Context *ctx
)
{
  qoriq_gpio_set_drive( &ctx->pin, ctx->drive );
}

static const BspPowerpcQoriqReqGpioSetDrive_Entry
BspPowerpcQoriqReqGpioSetDrive_Entries[] = {
  { 0, 0, BspPowerpcQoriqReqGpioSetDrive_Post_Drive_PushPull },
  { 0, 0, BspPowerpcQoriqReqGpioSetDrive_Post_Drive_OpenDrain }
};

static const uint8_t
BspPowerpcQoriqReqGpioSetDrive_Map[] = {
  0, 1
};

static size_t BspPowerpcQoriqReqGpioSetDrive_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  BspPowerpcQoriqReqGpioSetDrive_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      BspPowerpcQoriqReqGpioSetDrive_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture BspPowerpcQoriqReqGpioSetDrive_Fixture = {
  .setup = BspPowerpcQoriqReqGpioSetDrive_Setup_Wrap,
  .stop = NULL,
  .teardown = NULL,
  .scope = BspPowerpcQoriqReqGpioSetDrive_Scope,
  .initial_context = &BspPowerpcQoriqReqGpioSetDrive_Instance
};

static inline BspPowerpcQoriqReqGpioSetDrive_Entry
BspPowerpcQoriqReqGpioSetDrive_PopEntry(
  BspPowerpcQoriqReqGpioSetDrive_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return BspPowerpcQoriqReqGpioSetDrive_Entries[
    BspPowerpcQoriqReqGpioSetDrive_Map[ index ]
  ];
}

static void BspPowerpcQoriqReqGpioSetDrive_TestVariant(
  BspPowerpcQoriqReqGpioSetDrive_Context *ctx
)
{
  BspPowerpcQoriqReqGpioSetDrive_Pre_Drive_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  BspPowerpcQoriqReqGpioSetDrive_Action( ctx );
  BspPowerpcQoriqReqGpioSetDrive_Post_Drive_Check(
    ctx,
    ctx->Map.entry.Post_Drive
  );
}

/**
 * @fn void T_case_body_BspPowerpcQoriqReqGpioSetDrive( void )
 */
T_TEST_CASE_FIXTURE(
  BspPowerpcQoriqReqGpioSetDrive,
  &BspPowerpcQoriqReqGpioSetDrive_Fixture
)
{
  BspPowerpcQoriqReqGpioSetDrive_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = BspPowerpcQoriqReqGpioSetDrive_Pre_Drive_PushPull;
    ctx->Map.pcs[ 0 ] < BspPowerpcQoriqReqGpioSetDrive_Pre_Drive_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = BspPowerpcQoriqReqGpioSetDrive_PopEntry( ctx );
    BspPowerpcQoriqReqGpioSetDrive_TestVariant( ctx );
  }
}

/** @} */

#endif /* !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT) */
