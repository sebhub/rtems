/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup AcfgReqFatalStackCheckerInterruptStack
 */

/*
 * Copyright (C) 2026 embedded brains GmbH & Co. KG
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
#include <rtems/bspIo.h>
#include <rtems/sysinit.h>
#include <rtems/test-info.h>
#include <rtems/test-support.h>
#include <rtems/testopts.h>
#include <rtems/score/atomic.h>
#include <rtems/score/percpu.h>
#include <rtems/score/threadimpl.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup AcfgReqFatalStackCheckerInterruptStack \
 *   spec:/acfg/req/fatal-stack-checker-interrupt-stack
 *
 * @ingroup RTEMSTestSuitesValidation
 *
 * @brief This validation test suite contains a test case which triggers the
 *   fatal error of the RTEMS_FATAL_SOURCE_STACK_CHECKER fatal source with the
 *   object name built from the characters `INTR` as the fatal code.
 *
 * The first system initialization handler damages the stack checker pattern
 * area of the interrupt stack of processor 0. The second handler starts and
 * suspends a task and raises an interrupt which resumes the task. The
 * interrupt arrives before a thread begins on processor 0. The task preempts
 * the idle thread, so the task is the first thread which begins on processor
 * 0. In uniprocessor configurations, a thread switch from the idle thread to
 * the task precedes this begin. The test case checks that this fatal error
 * occurs in the task and on the stack of the task. The thread switch to the
 * task runs on the stack of the idle thread. The begin extensions of the task
 * run on the stack of the task.
 *
 * @{
 */

static void FatalErrorTestCase(
  rtems_fatal_source source,
  rtems_fatal_code   code
)
{
  const Thread_Control *executing;
  uintptr_t             stack_address;
  uintptr_t             stack_begin;

  T_eq_int( source, RTEMS_FATAL_SOURCE_STACK_CHECKER );
  T_eq_ulong( code, rtems_build_name( 'I', 'N', 'T', 'R' ) );

  executing = _Thread_Get_executing();
  T_eq_u32(
    executing->Object.name.name_u32,
    rtems_build_name( 'T', 'A', 'S', 'K' )
  );

  stack_address = (uintptr_t) &stack_address;
  stack_begin = (uintptr_t) executing->Start.Initial_stack.area;
  T_ge_uptr( stack_address, stack_begin );
  T_lt_uptr(
    stack_address,
    stack_begin + executing->Start.Initial_stack.size
  );
}

static void DamageInterruptStackPattern( void )
{
  const Per_CPU_Control *cpu;
  uint8_t               *pattern;

  cpu = _Per_CPU_Get_by_index( 0 );

#if CPU_STACK_GROWS_UP == TRUE
  pattern = (uint8_t *) cpu->interrupt_stack_high - 1;
#else
  pattern = cpu->interrupt_stack_low;
#endif

  *pattern = (uint8_t) ~*pattern;
}

RTEMS_SYSINIT_ITEM(
  DamageInterruptStackPattern,
  RTEMS_SYSINIT_IDLE_THREADS,
  RTEMS_SYSINIT_ORDER_LAST
);

static RTEMS_ALIGNED( RTEMS_TASK_STORAGE_ALIGNMENT ) char task_storage
  [ RTEMS_TASK_STORAGE_SIZE(
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_ATTRIBUTES
  ) ];

static const rtems_task_config task_config = {
  .name = rtems_build_name( 'T', 'A', 'S', 'K' ),
  .initial_priority = 1,
  .storage_area = task_storage,
  .storage_size = sizeof( task_storage ),
  .maximum_thread_local_storage_size = 0,
  .initial_modes = RTEMS_DEFAULT_MODES,
  .attributes = RTEMS_DEFAULT_ATTRIBUTES
};

static rtems_id task_id;

static CallWithinISRRequest resume_request;

static void TaskBody( rtems_task_argument arg )
{
  (void) arg;
}

static void Resume( void *arg )
{
  (void) arg;
  (void) rtems_task_resume( task_id );
}

static void PrepareThreadSwitch( void )
{
  rtems_status_code sc;

  sc = rtems_task_construct( &task_config, &task_id );
  T_quiet_rsc_success( sc );

  sc = rtems_task_start( task_id, TaskBody, 0 );
  T_quiet_rsc_success( sc );

  sc = rtems_task_suspend( task_id );
  T_quiet_rsc_success( sc );

  resume_request.handler = Resume;
  CallWithinISRSubmit( &resume_request );
}

RTEMS_SYSINIT_ITEM(
  PrepareThreadSwitch,
  RTEMS_SYSINIT_LAST,
  RTEMS_SYSINIT_ORDER_MIDDLE
);

#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER

#define CONFIGURE_STACK_CHECKER_ENABLED

#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_MINIMUM_TASKS_WITH_USER_PROVIDED_STORAGE 1

#define CONFIGURE_IDLE_TASK_STORAGE_SIZE RTEMS_MINIMUM_STACK_SIZE

static rtems_fatal_source fatal_error_test_source;

static rtems_fatal_code fatal_error_test_code;

T_TEST_CASE( AcfgReqFatalStackCheckerInterruptStack )
{
  FatalErrorTestCase( fatal_error_test_source, fatal_error_test_code );
}

const char rtems_test_name[] = "AcfgReqFatalStackCheckerInterruptStack";

static char fatal_error_test_buffer[ 512 ];

static const T_action fatal_error_test_actions[] = { T_report_hash_sha256 };

static const T_config fatal_error_test_config = {
  .name = rtems_test_name,
  .buf = fatal_error_test_buffer,
  .buf_size = sizeof( fatal_error_test_buffer ),
  .putchar = rtems_put_char,
  .verbosity = RTEMS_TEST_VERBOSITY,
#if defined( CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER )
  .now = T_now_clock,
#else
  .now = T_now_tick,
#endif
  .allocate = T_memory_allocate,
  .deallocate = T_memory_deallocate,
  .action_count = T_ARRAY_SIZE( fatal_error_test_actions ),
  .actions = fatal_error_test_actions
};

static bool fatal_error_test_initialized;

static void FatalErrorTestInitialize( void )
{
  if ( !fatal_error_test_initialized ) {
    fatal_error_test_initialized = true;
    rtems_test_begin( rtems_test_name, TEST_STATE );
    T_register();
    T_run_initialize( &fatal_error_test_config );
  }
}

static Atomic_Uint fatal_error_test_counter;

static void FatalErrorTestExtension(
  rtems_fatal_source source,
  bool               always_set_to_false,
  rtems_fatal_code   code
)
{
  rtems_fatal_code exit_code;

  (void) always_set_to_false;

  if ( source == RTEMS_FATAL_SOURCE_EXIT ) {
    return;
  }

  if (
    _Atomic_Fetch_add_uint(
      &fatal_error_test_counter,
      1,
      ATOMIC_ORDER_RELAXED
    ) != 0
  ) {
    return;
  }

  fatal_error_test_source = source;
  fatal_error_test_code = code;
  FatalErrorTestInitialize();
  T_make_runner();
  T_run_all();

  if ( T_run_finalize() ) {
    rtems_test_end( rtems_test_name );
    exit_code = 0;
  } else {
    exit_code = 1;
  }

  rtems_fatal( RTEMS_FATAL_SOURCE_EXIT, exit_code );
}

RTEMS_SYSINIT_ITEM(
  FatalErrorTestInitialize,
  RTEMS_SYSINIT_BSP_EARLY,
  RTEMS_SYSINIT_ORDER_FIRST
);

#if !defined( CONFIGURE_MAXIMUM_FILE_DESCRIPTORS )
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 0

#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM
#endif

#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY

#ifdef FATAL_ERROR_TEST_INITIAL_EXTENSION
#define OPTIONAL_FATAL_ERROR_TEST_INITIAL_EXTENSION \
  FATAL_ERROR_TEST_INITIAL_EXTENSION,
#else
#define OPTIONAL_FATAL_ERROR_TEST_INITIAL_EXTENSION
#endif

#define CONFIGURE_INITIAL_EXTENSIONS           \
  OPTIONAL_FATAL_ERROR_TEST_INITIAL_EXTENSION{ \
    .fatal = FatalInitialExtension             \
  },                                           \
  {                                            \
    .fatal = FatalErrorTestExtension           \
  }

#if !defined( CONFIGURE_RTEMS_INIT_TASKS_TABLE )

#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION

#if !defined( CONFIGURE_IDLE_TASK_BODY )

#define CONFIGURE_IDLE_TASK_BODY IdleBody

void *IdleBody( uintptr_t ignored )
{
  (void) ignored;

  rtems_fatal( RTEMS_FATAL_SOURCE_EXIT, 1 );
}

#endif /* CONFIGURE_IDLE_TASK_BODY */

#endif /* CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION */

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/** @} */
