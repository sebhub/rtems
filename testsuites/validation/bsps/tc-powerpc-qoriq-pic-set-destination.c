/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspPowerpcQoriqReqPicSetDestination
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

#if !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT)

/**
 * @defgroup BspPowerpcQoriqReqPicSetDestination \
 *   spec:/bsp/powerpc/qoriq/req/pic-set-destination
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @{
 */

typedef enum {
  BspPowerpcQoriqReqPicSetDestination_Pre_Vector_InternalExternal,
  BspPowerpcQoriqReqPicSetDestination_Pre_Vector_NonInternalExternal,
  BspPowerpcQoriqReqPicSetDestination_Pre_Vector_Invalid,
  BspPowerpcQoriqReqPicSetDestination_Pre_Vector_NA
} BspPowerpcQoriqReqPicSetDestination_Pre_Vector;

typedef enum {
  BspPowerpcQoriqReqPicSetDestination_Pre_Destination_Valid,
  BspPowerpcQoriqReqPicSetDestination_Pre_Destination_Invalid,
  BspPowerpcQoriqReqPicSetDestination_Pre_Destination_NA
} BspPowerpcQoriqReqPicSetDestination_Pre_Destination;

typedef enum {
  BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_Int,
  BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_CInt,
  BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_NA
} BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType;

typedef enum {
  BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_Int,
  BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_CInt,
  BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_MCP,
  BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_NA
} BspPowerpcQoriqReqPicSetDestination_Pre_SignalType;

typedef enum {
  BspPowerpcQoriqReqPicSetDestination_Post_Status_Ok,
  BspPowerpcQoriqReqPicSetDestination_Post_Status_InvId,
  BspPowerpcQoriqReqPicSetDestination_Post_Status_InvNum,
  BspPowerpcQoriqReqPicSetDestination_Post_Status_Unsat,
  BspPowerpcQoriqReqPicSetDestination_Post_Status_NA
} BspPowerpcQoriqReqPicSetDestination_Post_Status;

typedef enum {
  BspPowerpcQoriqReqPicSetDestination_Post_SignalType_Int,
  BspPowerpcQoriqReqPicSetDestination_Post_SignalType_CInt,
  BspPowerpcQoriqReqPicSetDestination_Post_SignalType_NA
} BspPowerpcQoriqReqPicSetDestination_Post_SignalType;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_Vector_NA : 1;
  uint16_t Pre_Destination_NA : 1;
  uint16_t Pre_CurrentSignalType_NA : 1;
  uint16_t Pre_SignalType_NA : 1;
  uint16_t Post_Status : 3;
  uint16_t Post_SignalType : 2;
} BspPowerpcQoriqReqPicSetDestination_Entry;

/**
 * @brief Test context for spec:/bsp/powerpc/qoriq/req/pic-set-destination test
 *   case.
 */
typedef struct {
  /**
   * @brief This member provides the ``vector`` parameter value.
   */
  rtems_vector_number vector;

  /**
   * @brief This member provides the ``destination`` parameter value.
   */
  uint32_t destination;

  /**
   * @brief This member provides the ``signal_type`` parameter value.
   */
  qoriq_pic_signal_type signal_type;

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
    BspPowerpcQoriqReqPicSetDestination_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} BspPowerpcQoriqReqPicSetDestination_Context;

static BspPowerpcQoriqReqPicSetDestination_Context
  BspPowerpcQoriqReqPicSetDestination_Instance;

static const char * const BspPowerpcQoriqReqPicSetDestination_PreDesc_Vector[] = {
  "InternalExternal",
  "NonInternalExternal",
  "Invalid",
  "NA"
};

static const char * const BspPowerpcQoriqReqPicSetDestination_PreDesc_Destination[] = {
  "Valid",
  "Invalid",
  "NA"
};

static const char * const BspPowerpcQoriqReqPicSetDestination_PreDesc_CurrentSignalType[] = {
  "Int",
  "CInt",
  "NA"
};

static const char * const BspPowerpcQoriqReqPicSetDestination_PreDesc_SignalType[] = {
  "Int",
  "CInt",
  "MCP",
  "NA"
};

static const char * const * const BspPowerpcQoriqReqPicSetDestination_PreDesc[] = {
  BspPowerpcQoriqReqPicSetDestination_PreDesc_Vector,
  BspPowerpcQoriqReqPicSetDestination_PreDesc_Destination,
  BspPowerpcQoriqReqPicSetDestination_PreDesc_CurrentSignalType,
  BspPowerpcQoriqReqPicSetDestination_PreDesc_SignalType,
  NULL
};

static void BspPowerpcQoriqReqPicSetDestination_Pre_Vector_Prepare(
  BspPowerpcQoriqReqPicSetDestination_Context   *ctx,
  BspPowerpcQoriqReqPicSetDestination_Pre_Vector state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetDestination_Pre_Vector_InternalExternal: {
      /*
       * While the ``vector`` parameter is associated with an interrupt vector,
       * while the interrupt source is internal or external.
       */
      ctx->vector = QORIQ_IRQ_DUART_1;
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Pre_Vector_NonInternalExternal: {
      /*
       * While the ``vector`` parameter is associated with an interrupt vector,
       * while the interrupt source is not internal or external.
       */
      ctx->vector = QORIQ_IRQ_IPI_0;
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Pre_Vector_Invalid: {
      /*
       * While the ``vector`` parameter is not associated with an interrupt
       * vector.
       */
      ctx->vector = BSP_INTERRUPT_VECTOR_COUNT;
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Pre_Vector_NA:
      break;
  }
}

static void BspPowerpcQoriqReqPicSetDestination_Pre_Destination_Prepare(
  BspPowerpcQoriqReqPicSetDestination_Context        *ctx,
  BspPowerpcQoriqReqPicSetDestination_Pre_Destination state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetDestination_Pre_Destination_Valid: {
      /*
       * While the ``destination`` parameter is less than the maximum processor
       * index.
       */
      ctx->destination = 0;
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Pre_Destination_Invalid: {
      /*
       * While the ``destination`` parameter is greater than or equal to the
       * maximum processor index.
       */
      ctx->destination = rtems_scheduler_get_processor_maximum();
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Pre_Destination_NA:
      break;
  }
}

static void BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_Prepare(
  BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_Int: {
      /*
       * While the critical interrupt bit of the Interrupt Destination Register
       * associated with the interrupt vector specified by ``vector`` and the
       * destination specified by ``destination`` is zero, While the normal
       * interrupt bit of the Interrupt Destination Register associated with
       * the interrupt vector specified by ``vector`` and the destination
       * specified by ``destination`` is one.
       */
      qoriq.pic.ii_0[ QORIQ_IRQ_DUART_1 ].dr = 0x1;
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_CInt: {
      /*
       * While the critical interrupt bit of the Interrupt Destination Register
       * associated with the interrupt vector specified by ``vector`` and the
       * destination specified by ``destination`` is one, while the normal
       * interrupt bit of the Interrupt Destination Register associated with
       * the interrupt vector specified by ``vector`` and the destination
       * specified by ``destination`` is zero.
       */
      qoriq.pic.ii_0[ QORIQ_IRQ_DUART_1 ].dr = 0x40000000;
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_NA:
      break;
  }
}

static void BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_Prepare(
  BspPowerpcQoriqReqPicSetDestination_Context       *ctx,
  BspPowerpcQoriqReqPicSetDestination_Pre_SignalType state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_Int: {
      /*
       * While the ``signal_type`` parameter is QORIQ_PIC_SIGNAL_INT.
       */
      ctx->signal_type = QORIQ_PIC_SIGNAL_INT;
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_CInt: {
      /*
       * While the ``signal_type`` parameter is QORIQ_PIC_SIGNAL_CINT.
       */
      ctx->signal_type = QORIQ_PIC_SIGNAL_CINT;
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_MCP: {
      /*
       * While the ``signal_type`` parameter is QORIQ_PIC_SIGNAL_MCP.
       */
      ctx->signal_type = QORIQ_PIC_SIGNAL_MCP;
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_NA:
      break;
  }
}

static void BspPowerpcQoriqReqPicSetDestination_Post_Status_Check(
  BspPowerpcQoriqReqPicSetDestination_Context    *ctx,
  BspPowerpcQoriqReqPicSetDestination_Post_Status state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetDestination_Post_Status_Ok: {
      /*
       * The returned status shall be RTEMS_SUCCESSFUL.
       */
      T_rsc_success( ctx->status );
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Post_Status_InvId: {
      /*
       * The returned status shall be RTEMS_INVALID_ID.
       */
      T_rsc( ctx->status, RTEMS_INVALID_ID );
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Post_Status_InvNum: {
      /*
       * The returned status shall be RTEMS_INVALID_NUMBER.
       */
      T_rsc( ctx->status, RTEMS_INVALID_NUMBER );
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Post_Status_Unsat: {
      /*
       * The returned status shall be RTEMS_UNSATISFIED.
       */
      T_rsc( ctx->status, RTEMS_UNSATISFIED );
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Post_Status_NA:
      break;
  }
}

static void BspPowerpcQoriqReqPicSetDestination_Post_SignalType_Check(
  BspPowerpcQoriqReqPicSetDestination_Post_SignalType state
)
{
  switch ( state ) {
    case BspPowerpcQoriqReqPicSetDestination_Post_SignalType_Int: {
      /*
       * The critical interrupt bit of the Interrupt Destination Register
       * associated with the interrupt vector specified by ``vector`` and the
       * destination specified by ``destination`` shall be zero, the normal
       * interrupt bit of the Interrupt Destination Register associated with
       * the interrupt vector specified by ``vector`` and the destination
       * specified by ``destination`` shall be one.
       */
      T_eq_u32( qoriq.pic.ii_0[ QORIQ_IRQ_DUART_1 ].dr, 0x1 );
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Post_SignalType_CInt: {
      /*
       * The critical interrupt bit of the Interrupt Destination Register
       * associated with the interrupt vector specified by ``vector`` and the
       * destination specified by ``destination`` shall be one, the normal
       * interrupt bit of the Interrupt Destination Register associated with
       * the interrupt vector specified by ``vector`` and the destination
       * specified by ``destination`` shall be zero.
       */
      T_eq_u32( qoriq.pic.ii_0[ QORIQ_IRQ_DUART_1 ].dr, 0x40000000 );
      break;
    }

    case BspPowerpcQoriqReqPicSetDestination_Post_SignalType_NA:
      break;
  }
}

static void BspPowerpcQoriqReqPicSetDestination_Action(
  BspPowerpcQoriqReqPicSetDestination_Context *ctx
)
{
  ctx->status = qoriq_pic_set_destination(
    ctx->vector,
    ctx->destination,
    ctx->signal_type
  );
}

static const BspPowerpcQoriqReqPicSetDestination_Entry
BspPowerpcQoriqReqPicSetDestination_Entries[] = {
  { 0, 0, 0, 1, 0, BspPowerpcQoriqReqPicSetDestination_Post_Status_InvNum,
    BspPowerpcQoriqReqPicSetDestination_Post_SignalType_NA },
  { 0, 0, 0, 1, 0, BspPowerpcQoriqReqPicSetDestination_Post_Status_InvId,
    BspPowerpcQoriqReqPicSetDestination_Post_SignalType_NA },
  { 0, 0, 0, 1, 0, BspPowerpcQoriqReqPicSetDestination_Post_Status_Unsat,
    BspPowerpcQoriqReqPicSetDestination_Post_SignalType_NA },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetDestination_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetDestination_Post_SignalType_Int },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetDestination_Post_Status_Ok,
    BspPowerpcQoriqReqPicSetDestination_Post_SignalType_CInt },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetDestination_Post_Status_Unsat,
    BspPowerpcQoriqReqPicSetDestination_Post_SignalType_Int },
  { 0, 0, 0, 0, 0, BspPowerpcQoriqReqPicSetDestination_Post_Status_Unsat,
    BspPowerpcQoriqReqPicSetDestination_Post_SignalType_CInt }
};

static const uint8_t
BspPowerpcQoriqReqPicSetDestination_Map[] = {
  3, 4, 5, 3, 4, 6, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

static size_t BspPowerpcQoriqReqPicSetDestination_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  BspPowerpcQoriqReqPicSetDestination_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      BspPowerpcQoriqReqPicSetDestination_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture BspPowerpcQoriqReqPicSetDestination_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = BspPowerpcQoriqReqPicSetDestination_Scope,
  .initial_context = &BspPowerpcQoriqReqPicSetDestination_Instance
};

static inline BspPowerpcQoriqReqPicSetDestination_Entry
BspPowerpcQoriqReqPicSetDestination_PopEntry(
  BspPowerpcQoriqReqPicSetDestination_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return BspPowerpcQoriqReqPicSetDestination_Entries[
    BspPowerpcQoriqReqPicSetDestination_Map[ index ]
  ];
}

static void BspPowerpcQoriqReqPicSetDestination_SetPreConditionStates(
  BspPowerpcQoriqReqPicSetDestination_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];

  if ( ctx->Map.entry.Pre_CurrentSignalType_NA ) {
    ctx->Map.pcs[ 2 ] = BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];
}

static void BspPowerpcQoriqReqPicSetDestination_TestVariant(
  BspPowerpcQoriqReqPicSetDestination_Context *ctx
)
{
  BspPowerpcQoriqReqPicSetDestination_Pre_Vector_Prepare(
    ctx,
    ctx->Map.pcs[ 0 ]
  );
  BspPowerpcQoriqReqPicSetDestination_Pre_Destination_Prepare(
    ctx,
    ctx->Map.pcs[ 1 ]
  );
  BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_Prepare(
    ctx->Map.pcs[ 2 ]
  );
  BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_Prepare(
    ctx,
    ctx->Map.pcs[ 3 ]
  );
  BspPowerpcQoriqReqPicSetDestination_Action( ctx );
  BspPowerpcQoriqReqPicSetDestination_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
  BspPowerpcQoriqReqPicSetDestination_Post_SignalType_Check(
    ctx->Map.entry.Post_SignalType
  );
}

/**
 * @fn void T_case_body_BspPowerpcQoriqReqPicSetDestination( void )
 */
T_TEST_CASE_FIXTURE(
  BspPowerpcQoriqReqPicSetDestination,
  &BspPowerpcQoriqReqPicSetDestination_Fixture
)
{
  BspPowerpcQoriqReqPicSetDestination_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = BspPowerpcQoriqReqPicSetDestination_Pre_Vector_InternalExternal;
    ctx->Map.pci[ 0 ] < BspPowerpcQoriqReqPicSetDestination_Pre_Vector_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = BspPowerpcQoriqReqPicSetDestination_Pre_Destination_Valid;
      ctx->Map.pci[ 1 ] < BspPowerpcQoriqReqPicSetDestination_Pre_Destination_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_Int;
        ctx->Map.pci[ 2 ] < BspPowerpcQoriqReqPicSetDestination_Pre_CurrentSignalType_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_Int;
          ctx->Map.pci[ 3 ] < BspPowerpcQoriqReqPicSetDestination_Pre_SignalType_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          ctx->Map.entry = BspPowerpcQoriqReqPicSetDestination_PopEntry( ctx );
          BspPowerpcQoriqReqPicSetDestination_SetPreConditionStates( ctx );
          BspPowerpcQoriqReqPicSetDestination_TestVariant( ctx );
        }
      }
    }
  }
}

/** @} */

#endif /* !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT) */
