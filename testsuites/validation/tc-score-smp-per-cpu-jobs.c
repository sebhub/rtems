/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreSmpValPerCpuJobs
 */

/*
 * Copyright (C) 2021 embedded brains GmbH & Co. KG
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

#include <rtems.h>
#include <rtems/sysinit.h>
#include <rtems/dev/io.h>
#include <rtems/score/atomic.h>
#include <rtems/score/percpu.h>
#include <rtems/score/smpimpl.h>
#include <rtems/score/sysstate.h>

#include <rtems/test.h>

/**
 * @defgroup ScoreSmpValPerCpuJobs spec:/score/smp/val/per-cpu-jobs
 *
 * @ingroup TestsuitesValidationSmpOnly0
 *
 * @brief Tests the processing order of per-processor jobs.
 *
 * This test case performs the following actions:
 *
 * - Issue two jobs on the current processor with interrupts disabled. Wait for
 *   completion of the second job.
 *
 *   - Check that the first job was processed firstly.
 *
 *   - Check that the second job was processed secondly.
 *
 * - Actions were performed by
 *   \_\_wrap\_\_SMP_Start_multitasking_on_secondary_processor().
 *
 *   - Check that the unicast action was performed while the worker processor
 *     is in the initial state. Check that the counter was incremented at least
 *     once by the boot processor while it is waiting for the other online
 *     processors to get ready for start multiprocessing.
 *
 * @{
 */

static Atomic_Uint job_counter;

static void Increment( void *arg )
{
  unsigned int *value;

  value = (unsigned int *) arg;
  *value = _Atomic_Fetch_add_uint( &job_counter, 1, ATOMIC_ORDER_RELAXED ) + 1;
}

static unsigned int counter_0;

static const Per_CPU_Job_context job_context_0 = {
  .handler = Increment,
  .arg = &counter_0
};

Per_CPU_Job job_0 = { .context = &job_context_0 };

static unsigned int counter_1;

static const Per_CPU_Job_context job_context_1 = {
  .handler = Increment,
  .arg = &counter_1
};

Per_CPU_Job job_1 = { .context = &job_context_1 };

static Atomic_Uint boot_counter;

static Atomic_Uintptr worker_cpu;

static Per_CPU_State worker_state = -1;

static uint32_t GetBootProcessorIndex( void )
{
  uint32_t cpu_max;

  cpu_max = _SMP_Processor_configured_maximum;

  while ( true ) {
    uint32_t cpu_index;

    for ( cpu_index = 0; cpu_index < cpu_max; ++cpu_index ) {
      if ( _Per_CPU_Get_by_index( cpu_index )->boot ) {
        return cpu_index;
      }
    }

    _IO_Relax();
  }
}

static void IncrementCounter( void *arg )
{
  _Atomic_Fetch_add_uint( (Atomic_Uint *) arg, 1, ATOMIC_ORDER_RELAXED );
}

static void PrepareRequestStartMultitasking( void )
{
  Per_CPU_Control *cpu;

  cpu = (Per_CPU_Control *)
    _Atomic_Fetch_add_uintptr( &worker_cpu, 0, ATOMIC_ORDER_ACQUIRE );

  if ( cpu != 0 ) {
    /*
     * This is _Per_CPU_Set_state() without the
     * _Assert( cpu_self == _Per_CPU_Get() ) which would fail here.
     */
    _Atomic_Store_uint(
      &cpu->state,
      (unsigned int) PER_CPU_STATE_INITIAL,
      ATOMIC_ORDER_RELEASE
    );
  }
}

RTEMS_SYSINIT_ITEM(
  PrepareRequestStartMultitasking,
  RTEMS_SYSINIT_LAST,
  RTEMS_SYSINIT_ORDER_LAST
);

void __real__SMP_Start_multitasking_on_secondary_processor(
  Per_CPU_Control *cpu_self
);

void __wrap__SMP_Start_multitasking_on_secondary_processor(
  Per_CPU_Control *cpu_self
);

void __wrap__SMP_Start_multitasking_on_secondary_processor(
  Per_CPU_Control *cpu_self
)
{
  uintptr_t expected;
  bool      success;

  expected = 0;
  success = _Atomic_Compare_exchange_uintptr(
    &worker_cpu,
    &expected,
    (uintptr_t) cpu_self,
    ATOMIC_ORDER_RELEASE,
    ATOMIC_ORDER_RELAXED
  );

  if ( success ) {
    uint32_t cpu_boot;

    worker_state = cpu_self->state;
    cpu_boot = GetBootProcessorIndex();

    while ( true ) {
      _SMP_Unicast_action( cpu_boot, IncrementCounter, &boot_counter );

      if ( _System_state_Is_up( _System_state_Get() ) ) {
        break;
      }

      /*
       * If the system state is not up, then the boot processor probably waits
       * in _Per_CPU_Wait_for_non_initial_state().  Let it go.  In
       * PrepareRequestStartMultitasking() we will reset the state.
       */
      _Per_CPU_Set_state(
        cpu_self,
        PER_CPU_STATE_READY_TO_START_MULTITASKING
      );
    }
  }
  __real__SMP_Start_multitasking_on_secondary_processor( cpu_self );
}

/**
 * @brief Issue two jobs on the current processor with interrupts disabled.
 *   Wait for completion of the second job.
 */
static void ScoreSmpValPerCpuJobs_Action_0( void )
{
  rtems_interrupt_level level;
  Per_CPU_Control      *cpu;

  rtems_interrupt_local_disable( level );
  cpu = _Per_CPU_Get();
  _Per_CPU_Add_job( cpu, &job_0 );
  _Per_CPU_Submit_job( cpu, &job_1 );
  rtems_interrupt_local_enable( level );

  _Per_CPU_Wait_for_job( cpu, &job_1 );

  /*
   * Check that the first job was processed firstly.
   */
  T_step_eq_int( 0, counter_0, 1 );

  /*
   * Check that the second job was processed secondly.
   */
  T_step_eq_int( 1, counter_1, 2 );
}

/**
 * @brief Actions were performed by
 *   \_\_wrap\_\_SMP_Start_multitasking_on_secondary_processor().
 */
static void ScoreSmpValPerCpuJobs_Action_1( void )
{
  /* Nothing to do */

  /*
   * Check that the unicast action was performed while the worker processor is
   * in the initial state. Check that the counter was incremented at least once
   * by the boot processor while it is waiting for the other online processors
   * to get ready for start multiprocessing.
   */
  T_step_eq_int( 2, worker_state, PER_CPU_STATE_INITIAL );
  T_step_gt_uint(
    3,
    _Atomic_Fetch_add_uint( &boot_counter, 0, ATOMIC_ORDER_RELAXED ),
    0
  );
}

/**
 * @fn void T_case_body_ScoreSmpValPerCpuJobs( void )
 */
T_TEST_CASE( ScoreSmpValPerCpuJobs )
{
  T_plan( 4 );

  ScoreSmpValPerCpuJobs_Action_0();
  ScoreSmpValPerCpuJobs_Action_1();
}

/** @} */
