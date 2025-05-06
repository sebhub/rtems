/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspPowerpcQoriqReqPicSetSenseAndPolarity
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

#include <bsp/irq.h>
#include <bsp/qoriq.h>

#include <rtems/test.h>

/**
 * @defgroup BspPowerpcQoriqReqPicSetSenseAndPolarity \
 *   spec:/bsp/powerpc/qoriq/req/pic-set-sense-and-polarity
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @{
 */

typedef enum {
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_External,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_NonExternal,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_Invalid,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_NA
} BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector;

typedef enum {
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_EdgeFalling,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_EdgeRaising,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_LevelLow,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_LevelHigh,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_NA
} BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity;

typedef enum {
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_EdgeFalling,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_EdgeRaising,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_LevelLow,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_LevelHigh,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_NA
} BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity;

typedef enum {
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity_Valid,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity_Null,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity_NA
} BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity;

typedef enum {
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_InvId,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Unsat,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_NA
} BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status;

typedef enum {
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeFalling,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeRaising,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelLow,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelHigh,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_NA
} BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity;

typedef enum {
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeFalling,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeRaising,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelLow,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelHigh,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_NA
} BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Vector_NA : 1;
  uint16_t Pre_CurrentSenseAndPolarity_NA : 1;
  uint16_t Pre_NewSenseAndPolarity_NA : 1;
  uint16_t Pre_OldSenseAndPolarity_NA : 1;
  uint16_t Post_Status : 2;
  uint16_t Post_SenseAndPolarity : 3;
  uint16_t Post_OldSenseAndPolarity : 3;
} BspPowerpcQoriqReqPicSetSenseAndPolarity_Entry;

/**
 * @brief Test context for
 *   spec:/bsp/powerpc/qoriq/req/pic-set-sense-and-polarity test case.
 */
typedef struct {
  /**
   * @brief This member provides a sense and polarity object.
   */
  qoriq_eirq_sense_and_polarity sense_and_polarity;

  /**
   * @brief This member provides the ``vector`` parameter value.
   */
  rtems_vector_number vector;

  /**
   * @brief This member provides the ``new_sense_and_polarity`` parameter
   *   value.
   */
  qoriq_eirq_sense_and_polarity new_sense_and_polarity;

  /**
   * @brief This member provides the ``old_sense_and_polarity`` parameter
   *   value.
   */
  qoriq_eirq_sense_and_polarity *old_sense_and_polarity;

  /**
   * @brief This member contains the return status.
   */
  rtems_status_code status;

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 4 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 4 ];

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
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} BspPowerpcQoriqReqPicSetSenseAndPolarity_Context;

static BspPowerpcQoriqReqPicSetSenseAndPolarity_Context
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Instance;

static const char * const BspPowerpcQoriqReqPicSetSenseAndPolarity_PreDesc_Vector[] = {
  "External",
  "NonExternal",
  "Invalid",
  "NA"
};

static const char * const BspPowerpcQoriqReqPicSetSenseAndPolarity_PreDesc_CurrentSenseAndPolarity[] = {
  "EdgeFalling",
  "EdgeRaising",
  "LevelLow",
  "LevelHigh",
  "NA"
};

static const char * const BspPowerpcQoriqReqPicSetSenseAndPolarity_PreDesc_NewSenseAndPolarity[] = {
  "EdgeFalling",
  "EdgeRaising",
  "LevelLow",
  "LevelHigh",
  "NA"
};

static const char * const BspPowerpcQoriqReqPicSetSenseAndPolarity_PreDesc_OldSenseAndPolarity[] = {
  "Valid",
  "Null",
  "NA"
};

static const char * const * const BspPowerpcQoriqReqPicSetSenseAndPolarity_PreDesc[] = {
  BspPowerpcQoriqReqPicSetSenseAndPolarity_PreDesc_Vector,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_PreDesc_CurrentSenseAndPolarity,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_PreDesc_NewSenseAndPolarity,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_PreDesc_OldSenseAndPolarity,
  NULL
};

static void SetSenseAndPolarity( uint32_t sense, uint32_t polarity )
{
  uint32_t vpr;

  vpr = qoriq.pic.ei[ 0 ].vpr;
  vpr &= ~0xc00000U;
  vpr |= sense << 22;
  vpr |= polarity << 23;
  qoriq.pic.ei[ 0 ].vpr = vpr;
}

static void BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_Prepare(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context   *ctx,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_External: {
      /*
       * While the ``vector`` parameter is associated with an interrupt vector,
       * while the interrupt source is external.
       */
      ctx->vector = QORIQ_IRQ_EXT_0;
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_NonExternal: {
      /*
       * While the ``vector`` parameter is associated with an interrupt vector,
       * while the interrupt source is not external.
       */
      ctx->vector = QORIQ_IRQ_IPI_0;
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_Invalid: {
      /*
       * While the ``vector`` parameter is not associated with an interrupt
       * vector.
       */
      ctx->vector = BSP_INTERRUPT_VECTOR_COUNT;
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_NA:
      break;
  }
}

static void
BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_Prepare(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_EdgeFalling: {
      /*
       * While The sense bit of the Interrupt Vector/Priority Register
       * associated with the interrupt vector specified by ``vector`` is zero,
       * while the polarity bit of the Interrupt Vector/Priority Register
       * associated with the interrupt vector specified by ``vector`` is zero.
       */
      SetSenseAndPolarity( 0, 0 );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_EdgeRaising: {
      /*
       * While The sense bit of the Interrupt Vector/Priority Register
       * associated with the interrupt vector specified by ``vector`` is zero,
       * while the polarity bit of the Interrupt Vector/Priority Register
       * associated with the interrupt vector specified by ``vector`` is one.
       */
      SetSenseAndPolarity( 0, 1 );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_LevelLow: {
      /*
       * While The sense bit of the Interrupt Vector/Priority Register
       * associated with the interrupt vector specified by ``vector`` is one,
       * while the polarity bit of the Interrupt Vector/Priority Register
       * associated with the interrupt vector specified by ``vector`` is zero.
       */
      SetSenseAndPolarity( 1, 0 );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_LevelHigh: {
      /*
       * While the sense bit of the Interrupt Vector/Priority Register
       * associated with the interrupt vector specified by ``vector`` is one,
       * while the polarity bit of the Interrupt Vector/Priority Register
       * associated with the interrupt vector specified by ``vector`` is one.
       */
      SetSenseAndPolarity( 1, 1 );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_NA:
      break;
  }
}

static void
BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_Prepare(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context                *ctx,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_EdgeFalling: {
      /*
       * While the ``new_sense_and_polarity`` parameter is
       * QORIQ_EIRQ_TRIGGER_EDGE_FALLING.
       */
      ctx->new_sense_and_polarity = QORIQ_EIRQ_TRIGGER_EDGE_FALLING;
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_EdgeRaising: {
      /*
       * While the ``new_sense_and_polarity`` parameter is
       * QORIQ_EIRQ_TRIGGER_EDGE_FALLING.
       */
      ctx->new_sense_and_polarity = QORIQ_EIRQ_TRIGGER_EDGE_RAISING;
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_LevelLow: {
      /*
       * While the ``new_sense_and_polarity`` parameter is
       * QORIQ_EIRQ_TRIGGER_LEVEL_LOW.
       */
      ctx->new_sense_and_polarity = QORIQ_EIRQ_TRIGGER_LEVEL_LOW;
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_LevelHigh: {
      /*
       * While the ``new_sense_and_polarity`` parameter is
       * QORIQ_EIRQ_TRIGGER_LEVEL_LOW.
       */
      ctx->new_sense_and_polarity = QORIQ_EIRQ_TRIGGER_LEVEL_HIGH;
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_NA:
      break;
  }
}

static void
BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity_Prepare(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context                *ctx,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity_Valid: {
      /*
       * While the ``old_sense_and_polarity`` parameter references an object of
       * type qoriq_eirq_sense_and_polarity.
       */
      ctx->old_sense_and_polarity = &ctx->sense_and_polarity;
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity_Null: {
      /*
       * While the ``new_sense_and_polarity`` parameter is equal to NULL.
       */
      ctx->old_sense_and_polarity = NULL;
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity_NA:
      break;
  }
}

static void BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Check(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context    *ctx,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok: {
      /*
       * The returned status shall be RTEMS_SUCCESSFUL.
       */
      T_rsc_success( ctx->status );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_InvId: {
      /*
       * The returned status shall be RTEMS_INVALID_ID.
       */
      T_rsc( ctx->status, RTEMS_INVALID_ID );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Unsat: {
      /*
       * The returned status shall be RTEMS_UNSATISFIED.
       */
      T_rsc( ctx->status, RTEMS_UNSATISFIED );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_NA:
      break;
  }
}

static void
BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_Check(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeFalling: {
      /*
       * The sense bit of the Interrupt Vector/Priority Register associated
       * with the interrupt vector specified by ``vector`` shall be zero, the
       * polarity bit of the Interrupt Vector/Priority Register associated with
       * the interrupt vector specified by ``vector`` shall be zero.
       */
      T_eq_u32( qoriq.pic.ei[ 0 ].vpr & 0xc00000U, 0x000000U );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeRaising: {
      /*
       * The sense bit of the Interrupt Vector/Priority Register associated
       * with the interrupt vector specified by ``vector`` shall be zero, the
       * polarity bit of the Interrupt Vector/Priority Register associated with
       * the interrupt vector specified by ``vector`` shall be one.
       */
      T_eq_u32( qoriq.pic.ei[ 0 ].vpr & 0xc00000U, 0x800000U );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelLow: {
      /*
       * The sense bit of the Interrupt Vector/Priority Register associated
       * with the interrupt vector specified by ``vector`` shall be one, the
       * polarity bit of the Interrupt Vector/Priority Register associated with
       * the interrupt vector specified by ``vector`` shall be zero.
       */
      T_eq_u32( qoriq.pic.ei[ 0 ].vpr & 0xc00000U, 0x400000U );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelHigh: {
      /*
       * The sense bit of the Interrupt Vector/Priority Register associated
       * with the interrupt vector specified by ``vector`` shall be one, the
       * polarity bit of the Interrupt Vector/Priority Register associated with
       * the interrupt vector specified by ``vector`` shall be one.
       */
      T_eq_u32( qoriq.pic.ei[ 0 ].vpr & 0xc00000U, 0xc00000U );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_NA:
      break;
  }
}

static void
BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_Check(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context                 *ctx,
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeFalling: {
      /*
       * The value of the object referenced by the ``old_sense_and_polarity``
       * shall QORIQ_EIRQ_TRIGGER_EDGE_FALLING.
       */
      T_eq_int( ctx->sense_and_polarity, QORIQ_EIRQ_TRIGGER_EDGE_FALLING );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeRaising: {
      /*
       * The value of the object referenced by the ``old_sense_and_polarity``
       * shall QORIQ_EIRQ_TRIGGER_EDGE_RAISING.
       */
      T_eq_int( ctx->sense_and_polarity, QORIQ_EIRQ_TRIGGER_EDGE_RAISING );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelLow: {
      /*
       * The value of the object referenced by the ``old_sense_and_polarity``
       * shall QORIQ_EIRQ_TRIGGER_LEVEL_LOW.
       */
      T_eq_int( ctx->sense_and_polarity, QORIQ_EIRQ_TRIGGER_LEVEL_LOW );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelHigh: {
      /*
       * The value of the object referenced by the ``old_sense_and_polarity``
       * shall QORIQ_EIRQ_TRIGGER_LEVEL_HIGH.
       */
      T_eq_int( ctx->sense_and_polarity, QORIQ_EIRQ_TRIGGER_LEVEL_HIGH );
      break;
    }

    case BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_NA:
      break;
  }
}

static void BspPowerpcQoriqReqPicSetSenseAndPolarity_Action(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context *ctx
)
{
  ctx->status = qoriq_pic_set_sense_and_polarity(
    ctx->vector,
    ctx->new_sense_and_polarity,
    ctx->old_sense_and_polarity
  );
}

static const BspPowerpcQoriqReqPicSetSenseAndPolarity_Entry
BspPowerpcQoriqReqPicSetSenseAndPolarity_Entries[] = {
  { 0, 0, 1, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Unsat,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_NA,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_NA },
  { 0, 0, 1, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_InvId,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_NA,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_NA },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeFalling,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_NA },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeRaising,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_NA },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelLow,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_NA },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelHigh,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_NA },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeFalling,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeFalling },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeRaising,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeFalling },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelLow,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeFalling },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelHigh,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeFalling },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeFalling,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeRaising },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeRaising,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeRaising },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelLow,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeRaising },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelHigh,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_EdgeRaising },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeFalling,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelLow },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeRaising,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelLow },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelLow,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelLow },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelHigh,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelLow },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeFalling,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelHigh },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_EdgeRaising,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelHigh },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelLow,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelHigh },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_LevelHigh,
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_LevelHigh }
};

static const uint8_t
BspPowerpcQoriqReqPicSetSenseAndPolarity_Map[] = {
  6, 2, 7, 3, 8, 4, 9, 5, 10, 2, 11, 3, 12, 4, 13, 5, 14, 2, 15, 3, 16, 4, 17,
  5, 18, 2, 19, 3, 20, 4, 21, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

static size_t BspPowerpcQoriqReqPicSetSenseAndPolarity_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      BspPowerpcQoriqReqPicSetSenseAndPolarity_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture BspPowerpcQoriqReqPicSetSenseAndPolarity_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = BspPowerpcQoriqReqPicSetSenseAndPolarity_Scope,
  .initial_context = &BspPowerpcQoriqReqPicSetSenseAndPolarity_Instance
};

static inline BspPowerpcQoriqReqPicSetSenseAndPolarity_Entry
BspPowerpcQoriqReqPicSetSenseAndPolarity_PopEntry(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return BspPowerpcQoriqReqPicSetSenseAndPolarity_Entries[
    BspPowerpcQoriqReqPicSetSenseAndPolarity_Map[ index ]
  ];
}

static void BspPowerpcQoriqReqPicSetSenseAndPolarity_SetPreConditionStates(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];

  if ( ctx->Map.entry.Pre_CurrentSenseAndPolarity_NA ) {
    ctx->Map.pcs[ 1 ] = BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_NA;
  } else {
    ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];
  }

  ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
}

static void BspPowerpcQoriqReqPicSetSenseAndPolarity_TestVariant(
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context *ctx
)
{
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_Prepare(
    ctx->Map.pcs[ 1 ]
  );
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_Prepare(
    ctx,
    ctx->Map.pcs[ 2 ]
  );
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity_Prepare(
    ctx,
    ctx->Map.pcs[ 3 ]
  );
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Action( ctx );
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_SenseAndPolarity_Check(
    ctx->Map.entry.Post_SenseAndPolarity
  );
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Post_OldSenseAndPolarity_Check(
    ctx,
    ctx->Map.entry.Post_OldSenseAndPolarity
  );
}

/**
 * @fn void T_case_body_BspPowerpcQoriqReqPicSetSenseAndPolarity( void )
 */
T_TEST_CASE_FIXTURE(
  BspPowerpcQoriqReqPicSetSenseAndPolarity,
  &BspPowerpcQoriqReqPicSetSenseAndPolarity_Fixture
)
{
  BspPowerpcQoriqReqPicSetSenseAndPolarity_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_External;
    ctx->Map.pci[ 0 ] < BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_Vector_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_EdgeFalling;
      ctx->Map.pci[ 1 ] < BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_CurrentSenseAndPolarity_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_EdgeFalling;
        ctx->Map.pci[ 2 ] < BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_NewSenseAndPolarity_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity_Valid;
          ctx->Map.pci[ 3 ] < BspPowerpcQoriqReqPicSetSenseAndPolarity_Pre_OldSenseAndPolarity_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = BspPowerpcQoriqReqPicSetSenseAndPolarity_PopEntry(
            ctx
          );
          BspPowerpcQoriqReqPicSetSenseAndPolarity_SetPreConditionStates(
            ctx
          );
          BspPowerpcQoriqReqPicSetSenseAndPolarity_TestVariant( ctx );
        }
      }
    }
  }
}

/** @} */
