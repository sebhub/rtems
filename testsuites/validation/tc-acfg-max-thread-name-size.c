/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup AcfgValMaxThreadNameSize
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

#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <rtems/score/thread.h>

#include <rtems/test.h>

/**
 * @defgroup AcfgValMaxThreadNameSize spec:/acfg/val/max-thread-name-size
 *
 * @ingroup TestsuitesValidationAcfg1
 * @ingroup TestsuitesValidationNoClock0
 * @ingroup TestsuitesValidationNoClock1
 *
 * @brief Tests the maximum thread name size which the
 *   CONFIGURE_MAXIMUM_THREAD_NAME_SIZE application configuration option
 *   defines.
 *
 * This test case performs the following actions:
 *
 * - Set the name of the calling thread to strings of the length of the maximum
 *   thread name size and of the length of the maximum thread name size minus
 *   one.
 *
 *   - Check that a name of the length of the maximum thread name size does not
 *     fit.
 *
 *   - Check that a name of the length of the maximum thread name size minus
 *     one fits, where the maximum thread name size is greater than zero.
 *
 * @{
 */

/**
 * @brief Set the name of the calling thread to strings of the length of the
 *   maximum thread name size and of the length of the maximum thread name size
 *   minus one.
 */
static void AcfgValMaxThreadNameSize_Action_0( void )
{
  size_t    size;
  char      name[ 64 ];
  pthread_t self;
  int       eno_size;
  int       eno_size_minus_one;

  size = _Thread_Maximum_name_size;
  T_assert_lt_sz( size, sizeof( name ) );
  self = pthread_self();

  memset( name, 'n', size );
  name[ size ] = '\0';
  eno_size = pthread_setname_np( self, name );

  if ( size > 0 ) {
    name[ size - 1 ] = '\0';
    eno_size_minus_one = pthread_setname_np( self, name );
  } else {
    eno_size_minus_one = 0;
  }

  (void) pthread_setname_np( self, "" );

  /*
   * Check that a name of the length of the maximum thread name size does not
   * fit.
   */
  T_eq_int( eno_size, ERANGE );

  /*
   * Check that a name of the length of the maximum thread name size minus one
   * fits, where the maximum thread name size is greater than zero.
   */
  if ( size > 0 ) {
    T_eq_int( eno_size_minus_one, 0 );
  }
}

/**
 * @fn void T_case_body_AcfgValMaxThreadNameSize( void )
 */
T_TEST_CASE( AcfgValMaxThreadNameSize )
{
  AcfgValMaxThreadNameSize_Action_0();
}

/** @} */
