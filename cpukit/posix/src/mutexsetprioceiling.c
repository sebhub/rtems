/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplPOSIXMutex
 *
 * @brief This source file contains the implementation of
 *   pthread_mutex_setprioceiling().
 */

/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
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

#include <rtems/posix/muteximpl.h>
#include <rtems/posix/priorityimpl.h>

/*
 *  13.6.2 Change the Priority Ceiling of a Mutex, P1003.1c/Draft 10, p. 131
 */

int pthread_mutex_setprioceiling(
  pthread_mutex_t *mutex,
  int              prioceiling,
  int             *old_ceiling
)
{
  POSIX_Mutex_Control     *the_mutex;
  unsigned long            flags;
  const Scheduler_Control *scheduler;
  Priority_Control         new_priority;
  Priority_Control         old_priority;
  bool                     valid;
  int                      eno;
  Thread_queue_Context     queue_context;
  Per_CPU_Control         *cpu_self;

  if ( old_ceiling == NULL ) {
    return EINVAL;
  }

  *old_ceiling = 0;
  the_mutex = _POSIX_Mutex_Get( mutex );
  POSIX_MUTEX_VALIDATE_OBJECT( the_mutex, flags );

  if ( _POSIX_Mutex_Get_protocol( flags ) != POSIX_MUTEX_PRIORITY_CEILING ) {
    return EINVAL;
  }

  scheduler = _POSIX_Mutex_Get_scheduler( the_mutex );
  _POSIX_Mutex_Acquire( the_mutex, &queue_context );
  old_priority = _POSIX_Mutex_Get_priority( the_mutex );
  _POSIX_Mutex_Release( the_mutex, &queue_context );
  *old_ceiling = _POSIX_Priority_From_core( scheduler, old_priority );

  new_priority = _POSIX_Priority_To_core( scheduler, prioceiling, &valid );

  if ( !valid ) {
    return EINVAL;
  }

  eno = pthread_mutex_lock( mutex );

  if ( eno != 0 ) {
    return eno;
  }

  old_priority = _POSIX_Mutex_Get_priority( the_mutex );
  *old_ceiling = _POSIX_Priority_From_core( scheduler, old_priority );

  _Thread_queue_Context_initialize( &queue_context );
  _Thread_queue_Context_clear_priority_updates( &queue_context );
  _POSIX_Mutex_Set_priority(
    the_mutex,
    _Thread_Get_executing(),
    new_priority,
    &queue_context
  );
  cpu_self = _Thread_Dispatch_disable();
  _Thread_Priority_update( &queue_context );
  _Thread_Dispatch_enable( cpu_self );

  eno = pthread_mutex_unlock( mutex );
  _Assert( eno == 0 );
  return eno;
}
