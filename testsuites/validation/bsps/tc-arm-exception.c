/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuArmValException
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

#include "tr-arm-exception-frame.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuArmValException spec:/score/cpu/arm/val/exception
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Tests the default exception handling.
 *
 * This test case performs the following actions:
 *
 * - Initialize the exception frame and VFP context. Prepare register values.
 *   Trigger an undefined instruction exception. Save the exception frame.
 *   Return to the action. Run this through the register check, once for each
 *   value inverted, then once unchanged.
 *
 *   - Check that the exception frame has the expected vector number.
 *
 * - Initialize the exception frame and VFP context. Prepare register values.
 *   Do a system call. Save the exception frame. Return to the action. Run this
 *   through the register check, once for each value inverted, then once
 *   unchanged.
 *
 *   - Check that the exception frame has the expected vector number.
 *
 * - Initialize the exception frame and VFP context. Prepare register values.
 *   Provoke a data abort. Save the exception frame. Return to the action. Run
 *   this through the register check, once for each value inverted, then once
 *   unchanged.
 *
 *   - Check that the exception frame has the expected vector number.
 *
 * - Initialize the exception frame and VFP context. Prepare register values.
 *   Provoke a prefetch abort. Save the exception frame. Return to the action.
 *   Run this through the register check, once for each value inverted, then
 *   once unchanged.
 *
 *   - Check that the exception frame has the expected vector number.
 *
 * @{
 */

#if defined( ARM_MULTILIB_ARCH_V4 )
  #define _RTEMS_TMTEST27
  #include <tm27.h>

extern const char undefined_instruction_label[];

static const void *const undefined_instruction_address =
  #ifdef __thumb__
  2
  #else
  4
  #endif
  + undefined_instruction_label;

extern const char system_call_label[];

static const void *const system_call_address =
  #ifdef __thumb__
  2
  #else
  4
  #endif
  + system_call_label;

static const volatile uint32_t data_abort_address =
  #ifdef TEST_DATA_ABORT_ADDRESS
  TEST_DATA_ABORT_ADDRESS
  #else
  0
  #endif
  ;

extern const char data_abort_instruction_label[];

static const void *const
  data_abort_instruction_address = data_abort_instruction_label + 8;

static const volatile uint32_t prefetch_abort_address =
  #ifdef TEST_PREFETCH_ABORT_ADDRESS
  TEST_PREFETCH_ABORT_ADDRESS
  #else
  0
  #endif
  #ifdef __thumb__
  | 1
  #endif
  ;

static void UndefinedInstructionScenario( void )
{
  (void) ExceptionFrameInitialize();
  __asm__ volatile( ASM_PREPARE "undefined_instruction_label:\n"
                                "udf\n"
                    :
                    :
                    : ASM_CLOBBER );
  exception_expected_pc = undefined_instruction_address;
}

static void SystemCallScenario( void )
{
  (void) ExceptionFrameInitialize();
  __asm__ volatile( ASM_PREPARE "ldr sp, =exception_restore_sp\n"
                                "ldr sp, [sp]\n"
                                "system_call_label:\n"
                                "svc #0\n"
                    :
                    :
                    : ASM_CLOBBER );
  exception_expected_registers[ 13 ] = exception_restore_sp;
  exception_expected_registers[ 14 ] = (uint32_t) (uintptr_t)
    system_call_address;
  exception_expected_pc = system_call_address;
}

static void DataAbortScenario( void )
{
  (void) ExceptionFrameInitialize();
  __asm__ volatile( ASM_PREPARE "ldr sp, =data_abort_address\n"
                                "ldr sp, [sp]\n"
                                "data_abort_instruction_label:\n"
                                "ldr sp, [sp]\n"
                                "nop\n"
                                "nop\n"
                    :
                    :
                    : ASM_CLOBBER );
  exception_expected_registers[ 13 ] = data_abort_address;
  exception_expected_pc = data_abort_instruction_address;
}

static void PrefetchAbortScenario( void )
{
  (void) ExceptionFrameInitialize();
  __asm__ volatile( ASM_PREPARE
                    "ldr sp, =prefetch_abort_address\n"
                    "ldr sp, [sp]\n"
                    "bx sp\n"
                    ".globl exception_resume_prefetch_abort_label\n"
                    "exception_resume_prefetch_abort_label:\n"
                    :
                    :
                    : ASM_CLOBBER );
  exception_expected_registers[ 13 ] = prefetch_abort_address;
  exception_expected_pc = (void *) ( ( prefetch_abort_address &
                                       ~UINT32_C( 1 ) ) +
                                     4 );
}
#endif

/**
 * @brief Initialize the exception frame and VFP context. Prepare register
 *   values. Trigger an undefined instruction exception. Save the exception
 *   frame. Return to the action. Run this through the register check, once for
 *   each value inverted, then once unchanged.
 */
static void ScoreCpuArmValException_Action_0( void )
{
  #if defined( ARM_MULTILIB_ARCH_V4 )
  ScoreCpuArmValExceptionFrame_Run(
    UndefinedInstructionScenario,
    0,
    ARM_EXCEPTION_UNDEF
  );
  #endif

  /*
   * Check that the exception frame has the expected vector number.
   */
  #if defined( ARM_MULTILIB_ARCH_V4 )
  ExceptionFrameCheckVector();
  #endif
}

/**
 * @brief Initialize the exception frame and VFP context. Prepare register
 *   values. Do a system call. Save the exception frame. Return to the action.
 *   Run this through the register check, once for each value inverted, then
 *   once unchanged.
 */
static void ScoreCpuArmValException_Action_1( void )
{
  #if defined( ARM_MULTILIB_ARCH_V4 )
  ScoreCpuArmValExceptionFrame_Run(
    SystemCallScenario,
    ( UINT32_C( 1 ) << 13 ) | ( UINT32_C( 1 ) << 14 ),
    ARM_EXCEPTION_SWI
  );
  #endif

  /*
   * Check that the exception frame has the expected vector number.
   */
  #if defined( ARM_MULTILIB_ARCH_V4 )
  ExceptionFrameCheckVector();
  #endif
}

/**
 * @brief Initialize the exception frame and VFP context. Prepare register
 *   values. Provoke a data abort. Save the exception frame. Return to the
 *   action. Run this through the register check, once for each value inverted,
 *   then once unchanged.
 */
static void ScoreCpuArmValException_Action_2( void )
{
  #if defined( ARM_MULTILIB_ARCH_V4 )
  ScoreCpuArmValExceptionFrame_Run(
    DataAbortScenario,
    UINT32_C( 1 ) << 13,
    ARM_EXCEPTION_DATA_ABORT
  );
  #endif

  /*
   * Check that the exception frame has the expected vector number.
   */
  #if defined( ARM_MULTILIB_ARCH_V4 )
  ExceptionFrameCheckVector();
  #endif
}

/**
 * @brief Initialize the exception frame and VFP context. Prepare register
 *   values. Provoke a prefetch abort. Save the exception frame. Return to the
 *   action. Run this through the register check, once for each value inverted,
 *   then once unchanged.
 */
static void ScoreCpuArmValException_Action_3( void )
{
  #if defined( ARM_MULTILIB_ARCH_V4 )
  ScoreCpuArmValExceptionFrame_Run(
    PrefetchAbortScenario,
    UINT32_C( 1 ) << 13,
    ARM_EXCEPTION_PREF_ABORT
  );
  #endif

  /*
   * Check that the exception frame has the expected vector number.
   */
  #if defined( ARM_MULTILIB_ARCH_V4 )
  ExceptionFrameCheckVector();
  #endif
}

/**
 * @fn void T_case_body_ScoreCpuArmValException( void )
 */
T_TEST_CASE( ScoreCpuArmValException )
{
  ScoreCpuArmValException_Action_0();
  ScoreCpuArmValException_Action_1();
  ScoreCpuArmValException_Action_2();
  ScoreCpuArmValException_Action_3();
}

/** @} */
