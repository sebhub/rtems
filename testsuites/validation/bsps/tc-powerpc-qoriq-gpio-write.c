/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspPowerpcQoriqReqGpioWrite
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
 * @defgroup BspPowerpcQoriqReqGpioWrite spec:/bsp/powerpc/qoriq/req/gpio-write
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @{
 */

typedef enum {
  BspPowerpcQoriqReqGpioWrite_Pre_Level_Low,
  BspPowerpcQoriqReqGpioWrite_Pre_Level_High,
  BspPowerpcQoriqReqGpioWrite_Pre_Level_NA
} BspPowerpcQoriqReqGpioWrite_Pre_Level;

typedef enum {
  BspPowerpcQoriqReqGpioWrite_Post_Level_Low,
  BspPowerpcQoriqReqGpioWrite_Post_Level_High,
  BspPowerpcQoriqReqGpioWrite_Post_Level_NA
} BspPowerpcQoriqReqGpioWrite_Post_Level;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Level_NA : 1;
  uint8_t Post_Level : 2;
} BspPowerpcQoriqReqGpioWrite_Entry;

/**
 * @brief Test context for spec:/bsp/powerpc/qoriq/req/gpio-write test case.
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
   * @brief This member provides the GPIO level.
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
    BspPowerpcQoriqReqGpioWrite_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} BspPowerpcQoriqReqGpioWrite_Context;

static BspPowerpcQoriqReqGpioWrite_Context
  BspPowerpcQoriqReqGpioWrite_Instance;

static const char * const BspPowerpcQoriqReqGpioWrite_PreDesc_Level[] = {
  "Low",
  "High",
  "NA"
};

static const char * const * const BspPowerpcQoriqReqGpioWrite_PreDesc[] = {
  BspPowerpcQoriqReqGpioWrite_PreDesc_Level,
  NULL
};

static void BspPowerpcQoriqReqGpioWrite_Pre_Level_Prepare(
  BspPowerpcQoriqReqGpioWrite_Context  *ctx,
  BspPowerpcQoriqReqGpioWrite_Pre_Level state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqGpioWrite_Pre_Level_Low: {
      /*
       * While the level specified by the ``level`` parameter is
       * QORIQ_GPIO_LOW.
       */
      ctx->level = QORIQ_GPIO_LOW;
      ctx->gpio.gpdat = 0xffffffff;
      break;
    }

    case BspPowerpcQoriqReqGpioWrite_Pre_Level_High: {
      /*
       * While the level specified by the ``level`` parameter is
       * QORIQ_GPIO_HIGH.
       */
      ctx->level = QORIQ_GPIO_HIGH;
      ctx->gpio.gpdat = 0;
      break;
    }

    case BspPowerpcQoriqReqGpioWrite_Pre_Level_NA:
      break;
  }
}

static void BspPowerpcQoriqReqGpioWrite_Post_Level_Check(
  BspPowerpcQoriqReqGpioWrite_Context   *ctx,
  BspPowerpcQoriqReqGpioWrite_Post_Level state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqGpioWrite_Post_Level_Low: {
      /*
       * The bits of the GPIO Data Register (GPIO_GPDAT) associated with the
       * pin referenced by the ``pin`` parameter shall be cleared.
       */
      T_eq_u32( ctx->gpio.gpdat, ~( 1U << ( 31 - 13 ) ) );
      break;
    }

    case BspPowerpcQoriqReqGpioWrite_Post_Level_High: {
      /*
       * The bits of the GPIO Data Register (GPIO_GPDAT) associated with the
       * pin referenced by the ``pin`` parameter shall be set.
       */
      T_eq_u32( ctx->gpio.gpdat, 1U << ( 31 - 13 ) );
      break;
    }

    case BspPowerpcQoriqReqGpioWrite_Post_Level_NA:
      break;
  }
}

static void BspPowerpcQoriqReqGpioWrite_Setup(
  BspPowerpcQoriqReqGpioWrite_Context *ctx
)
{
  ctx->controller.gpio = &ctx->gpio;
  rtems_interrupt_lock_initialize( &ctx->controller.lock, "GPIO" );
  ctx->pin.ctrl = &ctx->controller;
  ctx->pin.mask = QORIQ_GPIO_PIN( 13 );
}

static void BspPowerpcQoriqReqGpioWrite_Setup_Wrap( void *arg )
{
  BspPowerpcQoriqReqGpioWrite_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  BspPowerpcQoriqReqGpioWrite_Setup( ctx );
}

static void BspPowerpcQoriqReqGpioWrite_Action(
  BspPowerpcQoriqReqGpioWrite_Context *ctx
)
{
  qoriq_gpio_write( &ctx->pin, ctx->level );
}

static const BspPowerpcQoriqReqGpioWrite_Entry
BspPowerpcQoriqReqGpioWrite_Entries[] = {
  { 0, 0, BspPowerpcQoriqReqGpioWrite_Post_Level_Low },
  { 0, 0, BspPowerpcQoriqReqGpioWrite_Post_Level_High }
};

static const uint8_t
BspPowerpcQoriqReqGpioWrite_Map[] = {
  0, 1
};

static size_t BspPowerpcQoriqReqGpioWrite_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  BspPowerpcQoriqReqGpioWrite_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      BspPowerpcQoriqReqGpioWrite_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture BspPowerpcQoriqReqGpioWrite_Fixture = {
  .setup = BspPowerpcQoriqReqGpioWrite_Setup_Wrap,
  .stop = NULL,
  .teardown = NULL,
  .scope = BspPowerpcQoriqReqGpioWrite_Scope,
  .initial_context = &BspPowerpcQoriqReqGpioWrite_Instance
};

static inline BspPowerpcQoriqReqGpioWrite_Entry
BspPowerpcQoriqReqGpioWrite_PopEntry(
  BspPowerpcQoriqReqGpioWrite_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return BspPowerpcQoriqReqGpioWrite_Entries[
    BspPowerpcQoriqReqGpioWrite_Map[ index ]
  ];
}

static void BspPowerpcQoriqReqGpioWrite_TestVariant(
  BspPowerpcQoriqReqGpioWrite_Context *ctx
)
{
  BspPowerpcQoriqReqGpioWrite_Pre_Level_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  BspPowerpcQoriqReqGpioWrite_Action( ctx );
  BspPowerpcQoriqReqGpioWrite_Post_Level_Check(
    ctx,
    ctx->Map.entry.Post_Level
  );
}

/**
 * @fn void T_case_body_BspPowerpcQoriqReqGpioWrite( void )
 */
T_TEST_CASE_FIXTURE(
  BspPowerpcQoriqReqGpioWrite,
  &BspPowerpcQoriqReqGpioWrite_Fixture
)
{
  BspPowerpcQoriqReqGpioWrite_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = BspPowerpcQoriqReqGpioWrite_Pre_Level_Low;
    ctx->Map.pcs[ 0 ] < BspPowerpcQoriqReqGpioWrite_Pre_Level_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = BspPowerpcQoriqReqGpioWrite_PopEntry( ctx );
    BspPowerpcQoriqReqGpioWrite_TestVariant( ctx );
  }
}

/** @} */

#endif /* !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT) */
