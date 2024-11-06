/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspPowerpcQoriqReqGpioSetDirection
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
 * @defgroup BspPowerpcQoriqReqGpioSetDirection \
 *   spec:/bsp/powerpc/qoriq/req/gpio-set-direction
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @{
 */

typedef enum {
  BspPowerpcQoriqReqGpioSetDirection_Pre_Direction_Input,
  BspPowerpcQoriqReqGpioSetDirection_Pre_Direction_Output,
  BspPowerpcQoriqReqGpioSetDirection_Pre_Direction_NA
} BspPowerpcQoriqReqGpioSetDirection_Pre_Direction;

typedef enum {
  BspPowerpcQoriqReqGpioSetDirection_Post_Direction_Input,
  BspPowerpcQoriqReqGpioSetDirection_Post_Direction_Output,
  BspPowerpcQoriqReqGpioSetDirection_Post_Direction_NA
} BspPowerpcQoriqReqGpioSetDirection_Post_Direction;

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Direction_NA : 1;
  uint8_t Post_Direction : 2;
} BspPowerpcQoriqReqGpioSetDirection_Entry;

/**
 * @brief Test context for spec:/bsp/powerpc/qoriq/req/gpio-set-direction test
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
   * @brief This member provides the GPIO direction.
   */
  qoriq_gpio_direction direction;

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
    BspPowerpcQoriqReqGpioSetDirection_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} BspPowerpcQoriqReqGpioSetDirection_Context;

static BspPowerpcQoriqReqGpioSetDirection_Context
  BspPowerpcQoriqReqGpioSetDirection_Instance;

static const char * const BspPowerpcQoriqReqGpioSetDirection_PreDesc_Direction[] = {
  "Input",
  "Output",
  "NA"
};

static const char * const * const BspPowerpcQoriqReqGpioSetDirection_PreDesc[] = {
  BspPowerpcQoriqReqGpioSetDirection_PreDesc_Direction,
  NULL
};

static void BspPowerpcQoriqReqGpioSetDirection_Pre_Direction_Prepare(
  BspPowerpcQoriqReqGpioSetDirection_Context      *ctx,
  BspPowerpcQoriqReqGpioSetDirection_Pre_Direction state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqGpioSetDirection_Pre_Direction_Input: {
      /*
       * While the direction specified by the ``direction`` parameter is
       * QORIQ_GPIO_IN.
       */
      ctx->direction = QORIQ_GPIO_IN;
      ctx->gpio.gpdir = 0xffffffff;
      break;
    }

    case BspPowerpcQoriqReqGpioSetDirection_Pre_Direction_Output: {
      /*
       * While the direction specified by the ``direction`` parameter is
       * QORIQ_GPIO_OUT.
       */
      ctx->direction = QORIQ_GPIO_OUT;
      ctx->gpio.gpdir = 0;
      break;
    }

    case BspPowerpcQoriqReqGpioSetDirection_Pre_Direction_NA:
      break;
  }
}

static void BspPowerpcQoriqReqGpioSetDirection_Post_Direction_Check(
  BspPowerpcQoriqReqGpioSetDirection_Context       *ctx,
  BspPowerpcQoriqReqGpioSetDirection_Post_Direction state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqGpioSetDirection_Post_Direction_Input: {
      /*
       * The bits of the GPIO Direction Register (GPIO_GPDIR) associated with
       * the pin referenced by the ``pin`` parameter shall be cleared.
       */
      T_eq_u32( ctx->gpio.gpdir, ~( 1U << ( 31 - 13 ) ) );
      break;
    }

    case BspPowerpcQoriqReqGpioSetDirection_Post_Direction_Output: {
      /*
       * The bits of the GPIO Direction Register (GPIO_GPDIR) associated with
       * the pin referenced by the ``pin`` parameter shall be set.
       */
      T_eq_u32( ctx->gpio.gpdir, 1U << ( 31 - 13 ) );
      break;
    }

    case BspPowerpcQoriqReqGpioSetDirection_Post_Direction_NA:
      break;
  }
}

static void BspPowerpcQoriqReqGpioSetDirection_Setup(
  BspPowerpcQoriqReqGpioSetDirection_Context *ctx
)
{
  ctx->controller.gpio = &ctx->gpio;
  rtems_interrupt_lock_initialize( &ctx->controller.lock, "GPIO" );
  ctx->pin.ctrl = &ctx->controller;
  ctx->pin.mask = QORIQ_GPIO_PIN( 13 );
}

static void BspPowerpcQoriqReqGpioSetDirection_Setup_Wrap( void *arg )
{
  BspPowerpcQoriqReqGpioSetDirection_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  BspPowerpcQoriqReqGpioSetDirection_Setup( ctx );
}

static void BspPowerpcQoriqReqGpioSetDirection_Action(
  BspPowerpcQoriqReqGpioSetDirection_Context *ctx
)
{
  qoriq_gpio_set_direction( &ctx->pin, ctx->direction );
}

static const BspPowerpcQoriqReqGpioSetDirection_Entry
BspPowerpcQoriqReqGpioSetDirection_Entries[] = {
  { 0, 0, BspPowerpcQoriqReqGpioSetDirection_Post_Direction_Input },
  { 0, 0, BspPowerpcQoriqReqGpioSetDirection_Post_Direction_Output }
};

static const uint8_t
BspPowerpcQoriqReqGpioSetDirection_Map[] = {
  0, 1
};

static size_t BspPowerpcQoriqReqGpioSetDirection_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  BspPowerpcQoriqReqGpioSetDirection_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      BspPowerpcQoriqReqGpioSetDirection_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture BspPowerpcQoriqReqGpioSetDirection_Fixture = {
  .setup = BspPowerpcQoriqReqGpioSetDirection_Setup_Wrap,
  .stop = NULL,
  .teardown = NULL,
  .scope = BspPowerpcQoriqReqGpioSetDirection_Scope,
  .initial_context = &BspPowerpcQoriqReqGpioSetDirection_Instance
};

static inline BspPowerpcQoriqReqGpioSetDirection_Entry
BspPowerpcQoriqReqGpioSetDirection_PopEntry(
  BspPowerpcQoriqReqGpioSetDirection_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return BspPowerpcQoriqReqGpioSetDirection_Entries[
    BspPowerpcQoriqReqGpioSetDirection_Map[ index ]
  ];
}

static void BspPowerpcQoriqReqGpioSetDirection_TestVariant(
  BspPowerpcQoriqReqGpioSetDirection_Context *ctx
)
{
  BspPowerpcQoriqReqGpioSetDirection_Pre_Direction_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  BspPowerpcQoriqReqGpioSetDirection_Action( ctx );
  BspPowerpcQoriqReqGpioSetDirection_Post_Direction_Check(
    ctx,
    ctx->Map.entry.Post_Direction
  );
}

/**
 * @fn void T_case_body_BspPowerpcQoriqReqGpioSetDirection( void )
 */
T_TEST_CASE_FIXTURE(
  BspPowerpcQoriqReqGpioSetDirection,
  &BspPowerpcQoriqReqGpioSetDirection_Fixture
)
{
  BspPowerpcQoriqReqGpioSetDirection_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = BspPowerpcQoriqReqGpioSetDirection_Pre_Direction_Input;
    ctx->Map.pcs[ 0 ] < BspPowerpcQoriqReqGpioSetDirection_Pre_Direction_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = BspPowerpcQoriqReqGpioSetDirection_PopEntry( ctx );
    BspPowerpcQoriqReqGpioSetDirection_TestVariant( ctx );
  }
}

/** @} */

#endif /* !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT) */
