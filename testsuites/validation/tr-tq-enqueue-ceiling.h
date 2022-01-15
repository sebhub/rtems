/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSTestCaseScoreTqReqEnqueueCeiling
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

#ifndef _TR_TQ_ENQUEUE_CEILING_H
#define _TR_TQ_ENQUEUE_CEILING_H

#include "tx-thread-queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup RTEMSTestCaseScoreTqReqEnqueueCeiling
 *
 * @{
 */

typedef enum {
  ScoreTqReqEnqueueCeiling_Pre_EligibleScheduler_Home,
  ScoreTqReqEnqueueCeiling_Pre_EligibleScheduler_Helping,
  ScoreTqReqEnqueueCeiling_Pre_EligibleScheduler_NA
} ScoreTqReqEnqueueCeiling_Pre_EligibleScheduler;

typedef enum {
  ScoreTqReqEnqueueCeiling_Pre_QueueEligible_None,
  ScoreTqReqEnqueueCeiling_Pre_QueueEligible_Equal,
  ScoreTqReqEnqueueCeiling_Pre_QueueEligible_Low,
  ScoreTqReqEnqueueCeiling_Pre_QueueEligible_NA
} ScoreTqReqEnqueueCeiling_Pre_QueueEligible;

typedef enum {
  ScoreTqReqEnqueueCeiling_Pre_QueueIneligible_None,
  ScoreTqReqEnqueueCeiling_Pre_QueueIneligible_Before,
  ScoreTqReqEnqueueCeiling_Pre_QueueIneligible_After,
  ScoreTqReqEnqueueCeiling_Pre_QueueIneligible_NA
} ScoreTqReqEnqueueCeiling_Pre_QueueIneligible;

typedef enum {
  ScoreTqReqEnqueueCeiling_Post_Position_InitialFirst,
  ScoreTqReqEnqueueCeiling_Post_Position_First,
  ScoreTqReqEnqueueCeiling_Post_Position_Second,
  ScoreTqReqEnqueueCeiling_Post_Position_FirstFirst,
  ScoreTqReqEnqueueCeiling_Post_Position_SecondFirst,
  ScoreTqReqEnqueueCeiling_Post_Position_SecondQueue,
  ScoreTqReqEnqueueCeiling_Post_Position_NA
} ScoreTqReqEnqueueCeiling_Post_Position;

/**
 * @brief Runs the parameterized test case.
 *
 * @param[in,out] tq_ctx is the thread queue context.
 */
void ScoreTqReqEnqueueCeiling_Run( TQContext *tq_ctx );

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _TR_TQ_ENQUEUE_CEILING_H */
