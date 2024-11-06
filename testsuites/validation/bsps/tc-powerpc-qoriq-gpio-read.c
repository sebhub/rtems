/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspPowerpcQoriqReqGpioRead
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
 * @defgroup BspPowerpcQoriqReqGpioRead spec:/bsp/powerpc/qoriq/req/gpio-read
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @{
 */

typedef enum {
  BspPowerpcQoriqReqGpioRead_Pre_Level_Low,
  BspPowerpcQoriqReqGpioRead_Pre_Level_High,
  BspPowerpcQoriqReqGpioRead_Pre_Level_NA
} BspPowerpcQoriqReqGpioRead_Pre_Level;

typedef enum {
  BspPowerpcQoriqReqGpioRead_Post_Level_Low,
  BspPowerpcQoriqReqGpioRead_Post_Level_High,
  BspPowerpcQoriqReqGpioRead_Post_Level_NA
} BspPowerpcQoriqReqGpioRead_Post_Level;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Level_NA : 1;
  uint8_t Post_Level : 2;
} BspPowerpcQoriqReqGpioRead_Entry;

/**
 * @brief Test context for spec:/bsp/powerpc/qoriq/req/gpio-read test case.
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
   * @brief This member contains the returned GPIO level.
   */
  qoriq_gpio_level level;

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
    BspPowerpcQoriqReqGpioRead_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} BspPowerpcQoriqReqGpioRead_Context;

static BspPowerpcQoriqReqGpioRead_Context
  BspPowerpcQoriqReqGpioRead_Instance;

static const char * const BspPowerpcQoriqReqGpioRead_PreDesc_Level[] = {
  "Low",
  "High",
  "NA"
};

static const char * const * const BspPowerpcQoriqReqGpioRead_PreDesc[] = {
  BspPowerpcQoriqReqGpioRead_PreDesc_Level,
  NULL
};

static void BspPowerpcQoriqReqGpioRead_Pre_Level_Prepare(
  BspPowerpcQoriqReqGpioRead_Context  *ctx,
  BspPowerpcQoriqReqGpioRead_Pre_Level state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqGpioRead_Pre_Level_Low: {
      /*
       * While the all bits of the GPIO Data Register (GPIO_GPDAT) associated
       * with the pin referenced by the ``pin`` parameter are cleared.
       */
      ctx->gpio.gpdat = 0;
      break;
    }

    case BspPowerpcQoriqReqGpioRead_Pre_Level_High: {
      /*
       * While the at least one bit of the GPIO Data Register (GPIO_GPDAT)
       * associated with the pin referenced by the ``pin`` parameter is set.
       */
      ctx->gpio.gpdat = 1U << ( 31 - 13 );
      break;
    }

    case BspPowerpcQoriqReqGpioRead_Pre_Level_NA:
      break;
  }
}

static void BspPowerpcQoriqReqGpioRead_Post_Level_Check(
  BspPowerpcQoriqReqGpioRead_Context   *ctx,
  BspPowerpcQoriqReqGpioRead_Post_Level state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqGpioRead_Post_Level_Low: {
      /*
       * The returned level shall be QORIQ_GPIO_LOW.
       */
      T_eq_int( ctx->level, QORIQ_GPIO_LOW );
      break;
    }

    case BspPowerpcQoriqReqGpioRead_Post_Level_High: {
      /*
       * The returned level shall be QORIQ_GPIO_HIGH.
       */
      T_eq_int( ctx->level, QORIQ_GPIO_HIGH );
      break;
    }

    case BspPowerpcQoriqReqGpioRead_Post_Level_NA:
      break;
  }
}

static void BspPowerpcQoriqReqGpioRead_Setup(
  BspPowerpcQoriqReqGpioRead_Context *ctx
)
{
  ctx->controller.gpio = &ctx->gpio;
  rtems_interrupt_lock_initialize( &ctx->controller.lock, "GPIO" );
  ctx->pin.ctrl = &ctx->controller;
  ctx->pin.mask = QORIQ_GPIO_PIN( 13 );
}

static void BspPowerpcQoriqReqGpioRead_Setup_Wrap( void *arg )
{
  BspPowerpcQoriqReqGpioRead_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  BspPowerpcQoriqReqGpioRead_Setup( ctx );
}

static void BspPowerpcQoriqReqGpioRead_Action(
  BspPowerpcQoriqReqGpioRead_Context *ctx
)
{
  ctx->level = qoriq_gpio_read( &ctx->pin );
}

static const BspPowerpcQoriqReqGpioRead_Entry
BspPowerpcQoriqReqGpioRead_Entries[] = {
  { 0, 0, BspPowerpcQoriqReqGpioRead_Post_Level_Low },
  { 0, 0, BspPowerpcQoriqReqGpioRead_Post_Level_High }
};

static const uint8_t
BspPowerpcQoriqReqGpioRead_Map[] = {
  0, 1
};

static size_t BspPowerpcQoriqReqGpioRead_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  BspPowerpcQoriqReqGpioRead_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      BspPowerpcQoriqReqGpioRead_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture BspPowerpcQoriqReqGpioRead_Fixture = {
  .setup = BspPowerpcQoriqReqGpioRead_Setup_Wrap,
  .stop = NULL,
  .teardown = NULL,
  .scope = BspPowerpcQoriqReqGpioRead_Scope,
  .initial_context = &BspPowerpcQoriqReqGpioRead_Instance
};

static inline BspPowerpcQoriqReqGpioRead_Entry
BspPowerpcQoriqReqGpioRead_PopEntry( BspPowerpcQoriqReqGpioRead_Context *ctx )
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return BspPowerpcQoriqReqGpioRead_Entries[
    BspPowerpcQoriqReqGpioRead_Map[ index ]
  ];
}

static void BspPowerpcQoriqReqGpioRead_TestVariant(
  BspPowerpcQoriqReqGpioRead_Context *ctx
)
{
  BspPowerpcQoriqReqGpioRead_Pre_Level_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  BspPowerpcQoriqReqGpioRead_Action( ctx );
  BspPowerpcQoriqReqGpioRead_Post_Level_Check(
    ctx,
    ctx->Map.entry.Post_Level
  );
}

/**
 * @fn void T_case_body_BspPowerpcQoriqReqGpioRead( void )
 */
T_TEST_CASE_FIXTURE(
  BspPowerpcQoriqReqGpioRead,
  &BspPowerpcQoriqReqGpioRead_Fixture
)
{
  BspPowerpcQoriqReqGpioRead_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = BspPowerpcQoriqReqGpioRead_Pre_Level_Low;
    ctx->Map.pcs[ 0 ] < BspPowerpcQoriqReqGpioRead_Pre_Level_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = BspPowerpcQoriqReqGpioRead_PopEntry( ctx );
    BspPowerpcQoriqReqGpioRead_TestVariant( ctx );
  }
}

/** @} */

#endif /* !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT) */
