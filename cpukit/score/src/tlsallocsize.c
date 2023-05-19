/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSScoreTLS
 *
 * @brief This source file contains the implementation of
 *   _TLS_Get_allocation_size().
 */

/*
 * Copyright (C) 2014, 2022 embedded brains GmbH & Co. KG
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

#include <rtems/score/tls.h>
#include <rtems/score/interr.h>
#include <rtems/score/thread.h>

static uintptr_t _TLS_Allocation_size;

uintptr_t _TLS_Get_allocation_size( void )
{
  uintptr_t size;
  uintptr_t allocation_size;

  size = _TLS_Get_size();

  if ( size == 0 ) {
    return 0;
  }

  allocation_size = _TLS_Allocation_size;

  if ( allocation_size == 0 ) {
    uintptr_t tls_align;
    uintptr_t stack_align;

    /*
     * The TLS area is allocated in the thread storage area.  Each allocation
     * shall meet the stack alignment requirement.
     */
    stack_align = CPU_STACK_ALIGNMENT;
    tls_align = RTEMS_ALIGN_UP( (uintptr_t) _TLS_Alignment, stack_align );

#ifndef __i386__
    /* Reserve space for the dynamic thread vector */
    allocation_size +=
      RTEMS_ALIGN_UP( sizeof( TLS_Dynamic_thread_vector ), stack_align );
#endif

    /* Reserve space for the thread control block */
    allocation_size +=
#if CPU_THREAD_LOCAL_STORAGE_VARIANT == 11
      RTEMS_ALIGN_UP( sizeof( TLS_Thread_control_block ), tls_align );
#else
      RTEMS_ALIGN_UP( sizeof( TLS_Thread_control_block ), stack_align );
#endif

    /* Reserve space for the thread-local storage data */
    allocation_size +=
#if CPU_THREAD_LOCAL_STORAGE_VARIANT == 20
      RTEMS_ALIGN_UP( size, tls_align );
#else
      RTEMS_ALIGN_UP( size, stack_align );
#endif

    /*
     * The stack allocator does not support aligned allocations.  Allocate
     * enough to do the alignment manually.
     */
    if ( tls_align > stack_align ) {
      _Assert( tls_align % stack_align == 0 );
      allocation_size += tls_align - stack_align;
    }

    if ( _Thread_Maximum_TLS_size != 0 ) {
      if ( allocation_size <= _Thread_Maximum_TLS_size ) {
        _Assert( _Thread_Maximum_TLS_size % CPU_STACK_ALIGNMENT == 0 );
        allocation_size = _Thread_Maximum_TLS_size;
      } else {
        _Internal_error( INTERNAL_ERROR_TOO_LARGE_TLS_SIZE );
      }
    }

    _TLS_Allocation_size = allocation_size;
  }

  return allocation_size;
}
