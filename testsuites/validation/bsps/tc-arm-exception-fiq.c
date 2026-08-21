/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuArmValExceptionFiq
 */

/*
 * Copyright (C) 2024 embedded brains GmbH & Co. KG
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

#include <rtems/score/cpuimpl.h>

#include "tr-arm-exception-frame.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuArmValExceptionFiq spec:/score/cpu/arm/val/exception-fiq
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Tests the default FIQ exception handling.
 *
 * This test case performs the following actions:
 *
 * - Initialize the exception frame and VFP context. Prepare register values.
 *   Trigger an FIQ exception. Save the exception frame. Return to the action.
 *   Run this through the register check, once for each value inverted, then
 *   once unchanged.
 *
 *   - Check that the exception frame has the expected vector number.
 *
 * @{
 */

#define _RTEMS_TMTEST27
#include <tm27.h>

#ifdef TM27_INTERRUPT_VECTOR_FAST
static TM27_Trigger_fast_register volatile *volatile fiq_trigger_address;

static TM27_Trigger_fast_register volatile fiq_trigger_value;

extern const char after_fiq_enable_instruction_label[];

static const void *const
  after_fiq_enable_instruction_address = after_fiq_enable_instruction_label +
                                         4;
#endif

static void FiqScenario( void )
{
  (void) ExceptionFrameInitialize();
#ifdef TM27_INTERRUPT_VECTOR_FAST
  _TM27_Prepare_fast( &fiq_trigger_address, &fiq_trigger_value );
  __asm__ volatile( ASM_PREPARE "cpsie f\n"
                                "ldr sp, =fiq_trigger_address\n"
                                "ldr sp, [sp]\n"
                                "ldr lr, =fiq_trigger_value\n"
                                "ldr lr, [lr]\n"
                                "str lr, [sp]\n"
                                "after_fiq_enable_instruction_label:\n"
                                "1: b 1b\n"
                                "nop\n"
                                "nop\n"
                    :
                    :
                    : ASM_CLOBBER );
  exception_expected_registers[ 13 ] = (uint32_t) (uintptr_t)
    fiq_trigger_address;
  exception_expected_registers[ 14 ] = fiq_trigger_value;
  exception_expected_pc = after_fiq_enable_instruction_address;
#endif
}

/**
 * @brief Initialize the exception frame and VFP context. Prepare register
 *   values. Trigger an FIQ exception. Save the exception frame. Return to the
 *   action. Run this through the register check, once for each value inverted,
 *   then once unchanged.
 */
static void ScoreCpuArmValExceptionFiq_Action_0( void )
{
  #ifdef TM27_INTERRUPT_VECTOR_FAST
  _TM27_Prepare_fast( &fiq_trigger_address, &fiq_trigger_value );
  T_not_null( fiq_trigger_address );

  if ( fiq_trigger_address != NULL ) {
    ScoreCpuArmValExceptionFrame_Run(
      FiqScenario,
      ( UINT32_C( 1 ) << 13 ) | ( UINT32_C( 1 ) << 14 ),
      ARM_EXCEPTION_FIQ
    );
  }
  #else
  T_true( false );
  #endif

  /*
   * Check that the exception frame has the expected vector number.
   */
  ExceptionFrameCheckVector();
}

/**
 * @fn void T_case_body_ScoreCpuArmValExceptionFiq( void )
 */
T_TEST_CASE( ScoreCpuArmValExceptionFiq )
{
  ScoreCpuArmValExceptionFiq_Action_0();
}

/** @} */
