/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplApplConfig
 *
 * @brief This header file evaluates configuration options related to the RTEMS
 *   Workspace configuration.
 */

/*
 * Copyright (C) 2020, 2021 embedded brains GmbH & Co. KG
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

#ifndef _RTEMS_CONFDEFS_WKSPACE_H
#define _RTEMS_CONFDEFS_WKSPACE_H

#ifndef __CONFIGURATION_TEMPLATE_h
#error "Do not include this file directly, use <rtems/confdefs.h> instead"
#endif

#ifdef CONFIGURE_INIT

#include <rtems/confdefs/bdbuf.h>
#include <rtems/confdefs/inittask.h>
#include <rtems/confdefs/initthread.h>
#include <rtems/confdefs/objectsposix.h>
#include <rtems/confdefs/percpu.h>
#include <rtems/confdefs/threads.h>
#include <rtems/confdefs/wkspacesupport.h>
#include <rtems/score/coremsg.h>
#include <rtems/score/context.h>
#include <rtems/score/memory.h>
#include <rtems/score/stack.h>
#include <rtems/sysinit.h>

#if CPU_STACK_ALIGNMENT > CPU_HEAP_ALIGNMENT
  #define _CONFIGURE_TASK_STACK_ALLOC_SIZE( _stack_size ) \
    ( RTEMS_ALIGN_UP( \
        ( _stack_size ) + CONTEXT_FP_SIZE, \
        CPU_STACK_ALIGNMENT \
      ) + CPU_STACK_ALIGNMENT - CPU_HEAP_ALIGNMENT )
#else
  #define _CONFIGURE_TASK_STACK_ALLOC_SIZE( _stack_size ) \
    RTEMS_ALIGN_UP( ( _stack_size ) + CONTEXT_FP_SIZE, CPU_STACK_ALIGNMENT )
#endif

#ifdef CONFIGURE_TASK_STACK_FROM_ALLOCATOR
  #define _Configure_From_stackspace( _stack_size ) \
    CONFIGURE_TASK_STACK_FROM_ALLOCATOR( \
      _CONFIGURE_TASK_STACK_ALLOC_SIZE( _stack_size ) \
    )
#else
  #define _Configure_From_stackspace( _stack_size ) \
    _Configure_From_workspace( \
      _CONFIGURE_TASK_STACK_ALLOC_SIZE( _stack_size ) \
    )
#endif

#ifndef CONFIGURE_EXTRA_TASK_STACKS
  #define CONFIGURE_EXTRA_TASK_STACKS 0
#endif

#ifndef CONFIGURE_EXECUTIVE_RAM_SIZE

#define CONFIGURE_MESSAGE_BUFFERS_FOR_QUEUE( _messages, _size ) \
  _Configure_From_workspace( \
    ( _messages ) * ( _Configure_Align_up( _size, sizeof( uintptr_t ) ) \
        + sizeof( CORE_message_queue_Buffer ) ) )

#ifndef CONFIGURE_MESSAGE_BUFFER_MEMORY
  #define CONFIGURE_MESSAGE_BUFFER_MEMORY 0
#endif

#ifndef CONFIGURE_MEMORY_OVERHEAD
  #define CONFIGURE_MEMORY_OVERHEAD 0
#endif

/*
 * We must be able to split the free block used for the second last allocation
 * into two parts so that we have a free block for the last allocation.  See
 * _Heap_Block_split().
 */
#define _CONFIGURE_HEAP_HANDLER_OVERHEAD \
  _Configure_Align_up( HEAP_BLOCK_HEADER_SIZE, CPU_HEAP_ALIGNMENT )

#define CONFIGURE_EXECUTIVE_RAM_SIZE \
  ( _CONFIGURE_MEMORY_FOR_POSIX_OBJECTS \
    + CONFIGURE_MESSAGE_BUFFER_MEMORY \
    + 1024 * CONFIGURE_MEMORY_OVERHEAD \
    + _CONFIGURE_HEAP_HANDLER_OVERHEAD )

#if defined(CONFIGURE_IDLE_TASK_STORAGE_SIZE) || \
  defined(CONFIGURE_TASK_STACK_ALLOCATOR_FOR_IDLE)
  #define _CONFIGURE_IDLE_TASK_STACKS 0
#else
  #define _CONFIGURE_IDLE_TASK_STACKS \
    ( _CONFIGURE_MAXIMUM_PROCESSORS * \
      _Configure_From_stackspace( CONFIGURE_IDLE_TASK_STACK_SIZE ) )
#endif

#define _CONFIGURE_STACK_SPACE_SIZE \
  ( _CONFIGURE_INIT_TASK_STACK_EXTRA \
    + _CONFIGURE_IDLE_TASK_STACKS \
    + _CONFIGURE_POSIX_INIT_THREAD_STACK_EXTRA \
    + _CONFIGURE_LIBBLOCK_TASKS_STACK_EXTRA \
    + CONFIGURE_EXTRA_TASK_STACKS \
    + rtems_resource_maximum_per_allocation( \
        _CONFIGURE_TASKS - CONFIGURE_MINIMUM_TASKS_WITH_USER_PROVIDED_STORAGE \
      ) \
      * _Configure_From_stackspace( CONFIGURE_MINIMUM_TASK_STACK_SIZE ) \
    + rtems_resource_maximum_per_allocation( CONFIGURE_MAXIMUM_POSIX_THREADS ) \
      * _Configure_From_stackspace( CONFIGURE_MINIMUM_POSIX_THREAD_STACK_SIZE ) \
    + _CONFIGURE_HEAP_HANDLER_OVERHEAD )

#else /* CONFIGURE_EXECUTIVE_RAM_SIZE */

#if CONFIGURE_EXTRA_TASK_STACKS != 0
  #error "CONFIGURE_EXECUTIVE_RAM_SIZE defined with request for CONFIGURE_EXTRA_TASK_STACKS"
#endif

#define _CONFIGURE_STACK_SPACE_SIZE 0

#endif /* CONFIGURE_EXECUTIVE_RAM_SIZE */

#ifdef __cplusplus
extern "C" {
#endif

const uintptr_t _Workspace_Size = CONFIGURE_EXECUTIVE_RAM_SIZE;

#ifdef CONFIGURE_UNIFIED_WORK_AREAS
  const bool _Workspace_Is_unified = true;

  struct Heap_Control *( * const _Workspace_Malloc_initializer )( void ) =
    _Workspace_Malloc_initialize_unified;
#endif

uint32_t rtems_minimum_stack_size = CONFIGURE_MINIMUM_TASK_STACK_SIZE;

const uintptr_t _Stack_Space_size = _CONFIGURE_STACK_SPACE_SIZE;

#if defined(CONFIGURE_TASK_STACK_ALLOCATOR) \
  && defined(CONFIGURE_TASK_STACK_DEALLOCATOR)
  /*
   * Ignore the following warnings from g++ and clang in the uses of
   * _CONFIGURE_ASSERT_NOT_NULL() below:
   *
   * warning: the address of 'f()' will never be NULL [-Waddress]
   *
   * warning: comparison of function 'f' not equal to a null pointer is always
   * true [-Wtautological-pointer-compare]
   */
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Waddress"
  #pragma GCC diagnostic ignored "-Wpragmas"
  #pragma GCC diagnostic ignored "-Wtautological-pointer-compare"

  /* Custom allocator may or may not use the work space. */ 
  #ifdef CONFIGURE_TASK_STACK_ALLOCATOR_AVOIDS_WORK_SPACE
    const bool _Stack_Allocator_avoids_workspace = true;
  #else
    const bool _Stack_Allocator_avoids_workspace = false;
  #endif

  /* Custom allocator may or may not need initialization. */
  #ifdef CONFIGURE_TASK_STACK_ALLOCATOR_INIT
    const Stack_Allocator_initialize _Stack_Allocator_initialize =
      _CONFIGURE_ASSERT_NOT_NULL(
        Stack_Allocator_initialize,
        CONFIGURE_TASK_STACK_ALLOCATOR_INIT
      );

    RTEMS_SYSINIT_ITEM(
      _Stack_Allocator_do_initialize,
      RTEMS_SYSINIT_STACK_ALLOCATOR,
      RTEMS_SYSINIT_ORDER_MIDDLE
    );
  #endif

  /* Custom allocator must include allocate and free */
  const Stack_Allocator_allocate _Stack_Allocator_allocate =
    _CONFIGURE_ASSERT_NOT_NULL(
      Stack_Allocator_allocate,
      CONFIGURE_TASK_STACK_ALLOCATOR
    );

  const Stack_Allocator_free _Stack_Allocator_free =
    _CONFIGURE_ASSERT_NOT_NULL(
      Stack_Allocator_free,
      CONFIGURE_TASK_STACK_DEALLOCATOR
    );

  #pragma GCC diagnostic pop

/*
 * Must provide both a custom stack allocator and deallocator
 */
#elif defined(CONFIGURE_TASK_STACK_ALLOCATOR) \
  || defined(CONFIGURE_TASK_STACK_DEALLOCATOR)
  #error "CONFIGURE_TASK_STACK_ALLOCATOR and CONFIGURE_TASK_STACK_DEALLOCATOR must be both defined or both undefined"
#endif

#ifdef CONFIGURE_IDLE_TASK_STORAGE_SIZE
  #ifdef CONFIGURE_TASK_STACK_ALLOCATOR_FOR_IDLE
    #error "CONFIGURE_IDLE_TASK_STORAGE_SIZE and CONFIGURE_TASK_STACK_ALLOCATOR_FOR_IDLE are mutually exclusive"
  #endif

  #define _CONFIGURE_IDLE_TASK_STORAGE_SIZE \
    RTEMS_ALIGN_UP( \
      RTEMS_TASK_STORAGE_SIZE( \
        CONFIGURE_IDLE_TASK_STORAGE_SIZE, \
        RTEMS_DEFAULT_ATTRIBUTES \
      ), \
      CPU_INTERRUPT_STACK_ALIGNMENT \
    )

  const size_t _Stack_Allocator_allocate_for_idle_storage_size =
    _CONFIGURE_IDLE_TASK_STORAGE_SIZE;

  char _Stack_Allocator_allocate_for_idle_storage_areas[
    _CONFIGURE_MAXIMUM_PROCESSORS * _CONFIGURE_IDLE_TASK_STORAGE_SIZE
  ] RTEMS_ALIGNED( CPU_INTERRUPT_STACK_ALIGNMENT )
  RTEMS_SECTION( ".rtemsstack.idle" );

  #define CONFIGURE_TASK_STACK_ALLOCATOR_FOR_IDLE \
    _Stack_Allocator_allocate_for_idle_static
#endif

#ifndef CONFIGURE_TASK_STACK_ALLOCATOR_FOR_IDLE
  #define CONFIGURE_TASK_STACK_ALLOCATOR_FOR_IDLE \
    _Stack_Allocator_allocate_for_idle_workspace
#endif

const Stack_Allocator_allocate_for_idle _Stack_Allocator_allocate_for_idle =
  CONFIGURE_TASK_STACK_ALLOCATOR_FOR_IDLE;

#ifdef CONFIGURE_DIRTY_MEMORY
  RTEMS_SYSINIT_ITEM(
    _Memory_Dirty_free_areas,
    RTEMS_SYSINIT_DIRTY_MEMORY,
    RTEMS_SYSINIT_ORDER_MIDDLE
  );
#endif

#ifdef CONFIGURE_ZERO_WORKSPACE_AUTOMATICALLY
  const bool _Memory_Zero_before_use = true;

  RTEMS_SYSINIT_ITEM(
    _Memory_Zero_free_areas,
    RTEMS_SYSINIT_ZERO_MEMORY,
    RTEMS_SYSINIT_ORDER_MIDDLE
  );
#endif

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURE_INIT */

#endif /* _RTEMS_CONFDEFS_WKSPACE_H */
