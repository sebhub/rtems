/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup AcfgReqFatalInitTaskCreateFailed
 */

/*
 * Copyright (C) 2021, 2025 embedded brains GmbH & Co. KG
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
#include <rtems/malloc.h>
#include <rtems/sysinit.h>
#include <rtems/test-info.h>
#include <rtems/testopts.h>
#include <rtems/rtems/tasksdata.h>
#include <rtems/score/atomic.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup AcfgReqFatalInitTaskCreateFailed \
 *   spec:/acfg/req/fatal-init-task-create-failed
 *
 * @ingroup RTEMSTestSuitesValidation
 *
 * @brief This validation test suite contains a test case which triggers the
 *   fatal error of the INTERNAL_ERROR_CORE fatal source with the
 *   INTERNAL_ERROR_RTEMS_INIT_TASK_CREATE_FAILED fatal code during system
 *   initialization.
 *
 * The application configuration defines the priority of the Classic API
 * initialization task as zero. Zero is no valid task priority, so the task
 * creation fails. The test ends with a failure if the initialization task
 * starts or if the system deallocates a task stack.
 *
 * @{
 */

static void FatalErrorTestCase(
  rtems_fatal_source source,
  rtems_fatal_code   code
)
{
  T_eq_int( source, INTERNAL_ERROR_CORE );
  T_eq_ulong( code, INTERNAL_ERROR_RTEMS_INIT_TASK_CREATE_FAILED );

  /*
   * Check that the CONFIGURE_INIT_TASK_PRIORITY application configuration
   * option resulted in the expected system setting.
   */
  T_eq_u32( _RTEMS_tasks_User_task_table.initial_priority, 0 );
}

static void Init( rtems_task_argument arg )
{
  (void) arg;
  ExitWithFailure( "the initialization task started" );
}

static void StackDeallocate( void *unused )
{
  (void) unused;
  ExitWithFailure( "the system deallocated a task stack" );
}

#define CONFIGURE_TASK_STACK_ALLOCATOR_AVOIDS_WORK_SPACE

#define CONFIGURE_TASK_STACK_ALLOCATOR rtems_malloc

#define CONFIGURE_TASK_STACK_DEALLOCATOR StackDeallocate

#define CONFIGURE_TASK_STACK_FROM_ALLOCATOR( _stack_size ) 0

#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_INIT_TASK_PRIORITY 0

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_STACK_CHECKER_ENABLED

#define CONFIGURE_IDLE_TASK_STORAGE_SIZE RTEMS_MINIMUM_STACK_SIZE

static rtems_fatal_source fatal_error_test_source;

static rtems_fatal_code fatal_error_test_code;

T_TEST_CASE( AcfgReqFatalInitTaskCreateFailed )
{
  FatalErrorTestCase( fatal_error_test_source, fatal_error_test_code );
}

const char rtems_test_name[] = "AcfgReqFatalInitTaskCreateFailed";

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
