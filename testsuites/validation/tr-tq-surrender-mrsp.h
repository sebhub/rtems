/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSTestCaseScoreTqReqSurrenderMrsp
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

#ifndef _TR_TQ_SURRENDER_MRSP_H
#define _TR_TQ_SURRENDER_MRSP_H

#include "tx-thread-queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup RTEMSTestCaseScoreTqReqSurrenderMrsp
 *
 * @{
 */

typedef enum {
  ScoreTqReqSurrenderMrsp_Pre_InheritedPriority_Vital,
  ScoreTqReqSurrenderMrsp_Pre_InheritedPriority_Dispensable,
  ScoreTqReqSurrenderMrsp_Pre_InheritedPriority_NA
} ScoreTqReqSurrenderMrsp_Pre_InheritedPriority;

typedef enum {
  ScoreTqReqSurrenderMrsp_Pre_PreviousHelping_Vital,
  ScoreTqReqSurrenderMrsp_Pre_PreviousHelping_Dispensable,
  ScoreTqReqSurrenderMrsp_Pre_PreviousHelping_NA
} ScoreTqReqSurrenderMrsp_Pre_PreviousHelping;

typedef enum {
  ScoreTqReqSurrenderMrsp_Pre_Scheduler_Home,
  ScoreTqReqSurrenderMrsp_Pre_Scheduler_Helping,
  ScoreTqReqSurrenderMrsp_Pre_Scheduler_NA
} ScoreTqReqSurrenderMrsp_Pre_Scheduler;

typedef enum {
  ScoreTqReqSurrenderMrsp_Pre_NewHelping_Vital,
  ScoreTqReqSurrenderMrsp_Pre_NewHelping_Dispensable,
  ScoreTqReqSurrenderMrsp_Pre_NewHelping_NA
} ScoreTqReqSurrenderMrsp_Pre_NewHelping;

typedef enum {
  ScoreTqReqSurrenderMrsp_Pre_Suspended_Yes,
  ScoreTqReqSurrenderMrsp_Pre_Suspended_No,
  ScoreTqReqSurrenderMrsp_Pre_Suspended_NA
} ScoreTqReqSurrenderMrsp_Pre_Suspended;

typedef enum {
  ScoreTqReqSurrenderMrsp_Pre_WaitState_IntendToBlock,
  ScoreTqReqSurrenderMrsp_Pre_WaitState_NA
} ScoreTqReqSurrenderMrsp_Pre_WaitState;

typedef enum {
  ScoreTqReqSurrenderMrsp_Post_Dequeue_Priority,
  ScoreTqReqSurrenderMrsp_Post_Dequeue_NA
} ScoreTqReqSurrenderMrsp_Post_Dequeue;

typedef enum {
  ScoreTqReqSurrenderMrsp_Post_Unblock_No,
  ScoreTqReqSurrenderMrsp_Post_Unblock_NA
} ScoreTqReqSurrenderMrsp_Post_Unblock;

typedef enum {
  ScoreTqReqSurrenderMrsp_Post_PreviousOwnerPriority_Drop,
  ScoreTqReqSurrenderMrsp_Post_PreviousOwnerPriority_Nop,
  ScoreTqReqSurrenderMrsp_Post_PreviousOwnerPriority_NA
} ScoreTqReqSurrenderMrsp_Post_PreviousOwnerPriority;

typedef enum {
  ScoreTqReqSurrenderMrsp_Post_RemoveHelper_Yes,
  ScoreTqReqSurrenderMrsp_Post_RemoveHelper_No,
  ScoreTqReqSurrenderMrsp_Post_RemoveHelper_NA
} ScoreTqReqSurrenderMrsp_Post_RemoveHelper;

typedef enum {
  ScoreTqReqSurrenderMrsp_Post_AddHelper_Yes,
  ScoreTqReqSurrenderMrsp_Post_AddHelper_No,
  ScoreTqReqSurrenderMrsp_Post_AddHelper_NA
} ScoreTqReqSurrenderMrsp_Post_AddHelper;

typedef enum {
  ScoreTqReqSurrenderMrsp_Post_Suspended_Yes,
  ScoreTqReqSurrenderMrsp_Post_Suspended_No,
  ScoreTqReqSurrenderMrsp_Post_Suspended_NA
} ScoreTqReqSurrenderMrsp_Post_Suspended;

/**
 * @brief Runs the parameterized test case.
 *
 * @param[in,out] tq_ctx is the thread queue test context.
 */
void ScoreTqReqSurrenderMrsp_Run( TQContext *tq_ctx );

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _TR_TQ_SURRENDER_MRSP_H */
