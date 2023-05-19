/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSScoreSchedulerPrioritySMP
 *
 * @brief This header file provides interfaces of the
 *   @ref RTEMSScoreSchedulerPrioritySMP which are used by the implementation
 *   and the @ref RTEMSImplApplConfig.
 */

/*
 * Copyright (C) 2013, 2018 embedded brains GmbH & Co. KG
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

#ifndef _RTEMS_SCORE_SCHEDULERPRIORITYSMP_H
#define _RTEMS_SCORE_SCHEDULERPRIORITYSMP_H

#include <rtems/score/scheduler.h>
#include <rtems/score/schedulerpriority.h>
#include <rtems/score/schedulersmp.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup RTEMSScoreSchedulerPrioritySMP Deterministic Priority SMP Scheduler
 *
 * @ingroup RTEMSScoreSchedulerSMP
 *
 * @brief This group contains the Deterministic Priority SMP Scheduler implementation.
 *
 * This is an implementation of the global fixed priority scheduler (G-FP).  It
 * uses one ready chain per priority to ensure constant time insert operations.
 * The scheduled chain uses linear insert operations and has at most processor
 * count entries.  Since the processor and priority count are constants all
 * scheduler operations complete in a bounded execution time.
 *
 * The thread preempt mode will be ignored.
 *
 * @{
 */

/**
 * @brief Scheduler context specialization for Deterministic Priority SMP
 * schedulers.
 */
typedef struct {
  Scheduler_SMP_Context    Base;
  Chain_Control           *idle_ready_queue;
  Priority_bit_map_Control Bit_map;
  Chain_Control            Ready[ RTEMS_ZERO_LENGTH_ARRAY ];
} Scheduler_priority_SMP_Context;

/**
 * @brief Scheduler node specialization for Deterministic Priority SMP
 * schedulers.
 */
typedef struct {
  /**
   * @brief SMP scheduler node.
   */
  Scheduler_SMP_Node Base;

  /**
   * @brief The associated ready queue of this node.
   */
  Scheduler_priority_Ready_queue Ready_queue;
} Scheduler_priority_SMP_Node;

/**
 * @brief Entry points for the Priority SMP Scheduler.
 */
#define SCHEDULER_PRIORITY_SMP_ENTRY_POINTS \
  { \
    _Scheduler_priority_SMP_Initialize, \
    _Scheduler_default_Schedule, \
    _Scheduler_priority_SMP_Yield, \
    _Scheduler_priority_SMP_Block, \
    _Scheduler_priority_SMP_Unblock, \
    _Scheduler_priority_SMP_Update_priority, \
    _Scheduler_default_Map_priority, \
    _Scheduler_default_Unmap_priority, \
    _Scheduler_priority_SMP_Ask_for_help, \
    _Scheduler_priority_SMP_Reconsider_help_request, \
    _Scheduler_priority_SMP_Withdraw_node, \
    _Scheduler_priority_SMP_Make_sticky, \
    _Scheduler_priority_SMP_Clean_sticky, \
    _Scheduler_default_Pin_or_unpin_not_supported, \
    _Scheduler_default_Pin_or_unpin_not_supported, \
    _Scheduler_priority_SMP_Add_processor, \
    _Scheduler_priority_SMP_Remove_processor, \
    _Scheduler_priority_SMP_Node_initialize, \
    _Scheduler_default_Node_destroy, \
    _Scheduler_default_Release_job, \
    _Scheduler_default_Cancel_job, \
    _Scheduler_SMP_Start_idle \
    SCHEDULER_DEFAULT_SET_AFFINITY_OPERATION \
  }

/**
 * @brief Initializes the priority SMP scheduler.
 *
 * This routine initializes the priority SMP scheduler.
 *
 * @param scheduler The scheduler to initialize.
 */
void _Scheduler_priority_SMP_Initialize( const Scheduler_Control *scheduler );

/**
 * @brief Initializes the node with the given priority.
 *
 * @param scheduler The scheduler instance.
 * @param[out] node The node to initialize.
 * @param the_thread The thread of the scheduler node.
 * @param priority The priority for the initialization.
 */
void _Scheduler_priority_SMP_Node_initialize(
  const Scheduler_Control *scheduler,
  Scheduler_Node          *node,
  Thread_Control          *the_thread,
  Priority_Control         priority
);

/**
 * @brief Blocks the thread.
 *
 * @param scheduler The scheduler instance.
 * @param[in, out] the_thread The thread to block.
 * @param[in, out] node The @a thread's scheduler node.
 */
void _Scheduler_priority_SMP_Block(
  const Scheduler_Control *scheduler,
  Thread_Control          *thread,
  Scheduler_Node          *node
);

/**
 * @brief Unblocks the thread.
 *
 * @param scheduler The scheduler instance.
 * @param[in, out] the_thread The thread to unblock.
 * @param[in, out] node The @a thread's scheduler node.
 */
void _Scheduler_priority_SMP_Unblock(
  const Scheduler_Control *scheduler,
  Thread_Control          *thread,
  Scheduler_Node          *node
);

/**
 * @brief Updates the priority of the node.
 *
 * @param scheduler The scheduler instance.
 * @param the_thread The thread for the operation.
 * @param base_node The thread's scheduler node.
 */
void _Scheduler_priority_SMP_Update_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Scheduler_Node          *node
);

/**
 * @brief Asks for help operation.
 *
 * @param scheduler The scheduler instance to ask for help.
 * @param the_thread The thread needing help.
 * @param node The scheduler node.
 *
 * @retval true Ask for help was successful.
 * @retval false Ask for help was not successful.
 */
bool _Scheduler_priority_SMP_Ask_for_help(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Scheduler_Node          *node
);

/**
 * @brief Reconsiders help operation.
 *
 * @param scheduler The scheduler instance to reconsider the help
 *   request.
 * @param the_thread The thread reconsidering a help request.
 * @param node The scheduler node.
 */
void _Scheduler_priority_SMP_Reconsider_help_request(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Scheduler_Node          *node
);

/**
 * @brief Withdraws node operation.
 *
 * @param scheduler The scheduler instance to withdraw the node.
 * @param the_thread The thread using the node.
 * @param node The scheduler node to withdraw.
 * @param next_state The next thread scheduler state in case the node is
 *   scheduled.
 */
void _Scheduler_priority_SMP_Withdraw_node(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Scheduler_Node          *node,
  Thread_Scheduler_state   next_state
);

/**
 * @brief Makes the node sticky.
 *
 * @param scheduler is the scheduler of the node.
 *
 * @param[in, out] the_thread is the thread owning the node.
 *
 * @param[in, out] node is the scheduler node to make sticky.
 */
void _Scheduler_priority_SMP_Make_sticky(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Scheduler_Node          *node
);

/**
 * @brief Cleans the sticky property from the node.
 *
 * @param scheduler is the scheduler of the node.
 *
 * @param[in, out] the_thread is the thread owning the node.
 *
 * @param[in, out] node is the scheduler node to clean the sticky property.
 */
void _Scheduler_priority_SMP_Clean_sticky(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Scheduler_Node          *node
);

/**
 * @brief Adds @a idle to @a scheduler.
 *
 * @param[in, out] scheduler The scheduler instance to add the processor to.
 * @param idle The idle thrad control.
 */
void _Scheduler_priority_SMP_Add_processor(
  const Scheduler_Control *scheduler,
  Thread_Control          *idle
);

/**
 * @brief Removes an idle thread from the given cpu.
 *
 * @param scheduler The scheduler instance.
 * @param cpu The cpu control to remove from @a scheduler.
 *
 * @return The idle thread of the processor.
 */
Thread_Control *_Scheduler_priority_SMP_Remove_processor(
  const Scheduler_Control *scheduler,
  struct Per_CPU_Control  *cpu
);

/**
 * @brief Performs the yield of a thread.
 *
 * @param scheduler The scheduler instance.
 * @param[in, out] the_thread The thread that performed the yield operation.
 * @param node The scheduler node of @a the_thread.
 */
void _Scheduler_priority_SMP_Yield(
  const Scheduler_Control *scheduler,
  Thread_Control          *thread,
  Scheduler_Node          *node
);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RTEMS_SCORE_SCHEDULERPRIORITYSMP_H */
