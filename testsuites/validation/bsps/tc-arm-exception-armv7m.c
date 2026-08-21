/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuArmValExceptionArmv7m
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

#include <stddef.h>
#include <rtems/score/armv7m.h>
#include <rtems/score/isr.h>

#include "tr-arm-exception-frame.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuArmValExceptionArmv7m \
 *   spec:/score/cpu/arm/val/exception-armv7m
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Tests the default exception handling of ARMv7-M.
 *
 * This test case performs the following actions:
 *
 * - Enable the UsageFault exception. Prepare register values. Execute an
 *   undefined instruction. Save the exception frame. Resume after the
 *   instruction. Run this through the register check, once for each value
 *   inverted, then once unchanged.
 *
 *   - Check that the exception frame has the vector number of the UsageFault
 *     exception.
 *
 *   - Check that the default exception handler placed the exception frame on
 *     the interrupt stack.
 *
 * - Disable the UsageFault exception. Prepare register values. Execute an
 *   undefined instruction, which escalates to a HardFault exception. Save the
 *   exception frame. Resume after the instruction. Run this through the
 *   register check, once for each value inverted, then once unchanged.
 *
 *   - Check that the exception frame has the vector number of the HardFault
 *     exception.
 *
 *   - Check that the default exception handler placed the exception frame on
 *     the interrupt stack.
 *
 * - Enable the BusFault exception. Prepare register values. Load from an
 *   address which answers with a bus error. Save the exception frame. Resume
 *   after the load. Run this through the register check, once for each value
 *   inverted, then once unchanged.
 *
 *   - Check that the exception frame has the vector number of the BusFault
 *     exception.
 *
 *   - Check that the default exception handler placed the exception frame on
 *     the interrupt stack.
 *
 * - Enable the MemManage exception. Prepare register values. Branch to the
 *   System region, which never executes. Save the exception frame. Resume
 *   after the branch. Run this through the register check, once for each value
 *   inverted, then once unchanged.
 *
 *   - Check that the exception frame has the vector number of the MemManage
 *     exception.
 *
 *   - Check that the default exception handler placed the exception frame on
 *     the interrupt stack.
 *
 * - Set the NMI pending. Save the exception frame. Resume at the interrupted
 *   instruction. Run this through the register check, once for each value
 *   inverted, then once unchanged.
 *
 *   - Check that the exception frame has the vector number of the NMI
 *     exception.
 *
 *   - Check that the default exception handler placed the exception frame on
 *     the interrupt stack.
 *
 * @{
 */

#define _RTEMS_TMTEST27
#include <tm27.h>

#define FAULT_ENABLE_MASK                                         \
  ( ARMV7M_SCB_SHCSR_USGFAULTENA | ARMV7M_SCB_SHCSR_BUSFAULTENA | \
    ARMV7M_SCB_SHCSR_MEMFAULTENA )

/* The System region at this address is never executable. */
#define MEM_MANAGE_ADDRESS 0xe0000001

static uint32_t fault_shcsr;

extern const char usage_fault_label[];

extern const char usage_fault_resume_label[];

extern const char hard_fault_label[];

extern const char hard_fault_resume_label[];

extern const char bus_fault_label[];

extern const char bus_fault_resume_label[];

extern const char mem_manage_resume_label[];

static const volatile uint32_t bus_fault_address = TEST_BUS_FAULT_ADDRESS;

static void FaultEnable( uint32_t enable )
{
  volatile ARMV7M_SCB *scb;

  scb = _ARMV7M_SCB;
  scb->shcsr = ( scb->shcsr & ~FAULT_ENABLE_MASK ) | enable;
  _ARM_Data_synchronization_barrier();
  _ARM_Instruction_synchronization_barrier();
}

/* The NMI block loads the address of the ICSR and the NMIPENDSET bit. */
RTEMS_STATIC_ASSERT(
  ARMV7M_SCB_BASE + offsetof( ARMV7M_SCB, icsr ) == 0xe000ed04,
  icsr_address
);
RTEMS_STATIC_ASSERT( ARMV7M_SCB_ICSR_NMIPENDSET == 0x80000000, nmipendset );

extern const char nmi_dsb_label[];

extern const char nmi_isb_label[];

extern const char nmi_done_label[];

/*
 * The processor takes the pending NMI after the store, the data
 * synchronisation barrier or the instruction synchronisation barrier.
 */
static const void *NmiReturnAddress( void )
{
  const void *pc;

  pc = ExceptionFrameGet()->register_pc;

  if ( pc == nmi_dsb_label || pc == nmi_isb_label || pc == nmi_done_label ) {
    return pc;
  }

  return nmi_done_label;
}

static void UsageFaultScenario( void )
{
  (void) ExceptionFrameInitialize();
  FaultEnable( ARMV7M_SCB_SHCSR_USGFAULTENA );
  exception_resume_pc = usage_fault_resume_label;
  __asm__ volatile( ASM_PREPARE "usage_fault_label:\n"
                                "udf #0\n"
                                "usage_fault_resume_label:\n"
                    :
                    :
                    : ASM_CLOBBER );
  exception_expected_registers[ 13 ] = exception_restore_sp;
  exception_expected_pc = usage_fault_label;
}

static void HardFaultScenario( void )
{
  (void) ExceptionFrameInitialize();
  FaultEnable( 0 );
  exception_resume_pc = hard_fault_resume_label;
  __asm__ volatile( ASM_PREPARE "hard_fault_label:\n"
                                "udf #0\n"
                                "hard_fault_resume_label:\n"
                    :
                    :
                    : ASM_CLOBBER );
  exception_expected_registers[ 13 ] = exception_restore_sp;
  exception_expected_pc = hard_fault_label;
}

static void BusFaultScenario( void )
{
  (void) ExceptionFrameInitialize();
  FaultEnable( ARMV7M_SCB_SHCSR_BUSFAULTENA );
  exception_resume_pc = bus_fault_resume_label;
  __asm__ volatile( ASM_PREPARE "ldr lr, =bus_fault_address\n"
                                "ldr lr, [lr]\n"
                                "bus_fault_label:\n"
                                "ldr.w lr, [lr]\n"
                                "bus_fault_resume_label:\n"
                    :
                    :
                    : ASM_CLOBBER );
  exception_expected_registers[ 13 ] = exception_restore_sp;
  exception_expected_registers[ 14 ] = bus_fault_address;
  exception_expected_pc = bus_fault_label;
}

static void MemManageScenario( void )
{
  (void) ExceptionFrameInitialize();
  FaultEnable( ARMV7M_SCB_SHCSR_MEMFAULTENA );
  exception_resume_pc = mem_manage_resume_label;
  __asm__ volatile( ASM_PREPARE "ldr lr, =" RTEMS_XSTRING(
                      MEM_MANAGE_ADDRESS
  ) "\n"
    "bx lr\n"
    "mem_manage_resume_label:\n"
                    :
                    :
                    : ASM_CLOBBER );
  exception_expected_registers[ 13 ] = exception_restore_sp;
  exception_expected_registers[ 14 ] = MEM_MANAGE_ADDRESS;
  exception_expected_pc = (void *) ( MEM_MANAGE_ADDRESS & ~UINT32_C( 1 ) );
}

static void NmiScenario( void )
{
  (void) ExceptionFrameInitialize();
  __asm__ volatile( ASM_PREPARE "ldr r0, =0xe000ed04\n"
                                "ldr r1, =0x80000000\n"
                                "str r1, [r0]\n"
                                ".globl nmi_dsb_label\n"
                                "nmi_dsb_label:\n"
                                "dsb\n"
                                ".globl nmi_isb_label\n"
                                "nmi_isb_label:\n"
                                "isb\n"
                                ".globl nmi_done_label\n"
                                "nmi_done_label:\n"
                    :
                    :
                    : ASM_CLOBBER );
  exception_expected_registers[ 0 ] = (uint32_t) (uintptr_t) &_ARMV7M_SCB
                                        ->icsr;
  exception_expected_registers[ 1 ] = ARMV7M_SCB_ICSR_NMIPENDSET;
  exception_expected_registers[ 13 ] = exception_restore_sp;
  exception_expected_pc = NmiReturnAddress();
}

static void ScoreCpuArmValExceptionArmv7m_Setup( void )
{
  fault_shcsr = _ARMV7M_SCB->shcsr;
}

static void ScoreCpuArmValExceptionArmv7m_Setup_Wrap( void *arg )
{
  (void) arg;
  ScoreCpuArmValExceptionArmv7m_Setup();
}

static void ScoreCpuArmValExceptionArmv7m_Teardown( void )
{
  volatile ARMV7M_SCB *scb;

  scb = _ARMV7M_SCB;
  scb->cfsr = scb->cfsr;
  scb->hfsr = scb->hfsr;
  FaultEnable( fault_shcsr & FAULT_ENABLE_MASK );
}

static void ScoreCpuArmValExceptionArmv7m_Teardown_Wrap( void *arg )
{
  (void) arg;
  ScoreCpuArmValExceptionArmv7m_Teardown();
}

static T_fixture ScoreCpuArmValExceptionArmv7m_Fixture = {
  .setup = ScoreCpuArmValExceptionArmv7m_Setup_Wrap,
  .stop = NULL,
  .teardown = ScoreCpuArmValExceptionArmv7m_Teardown_Wrap,
  .scope = NULL,
  .initial_context = NULL
};

/**
 * @brief Enable the UsageFault exception. Prepare register values. Execute an
 *   undefined instruction. Save the exception frame. Resume after the
 *   instruction. Run this through the register check, once for each value
 *   inverted, then once unchanged.
 */
static void ScoreCpuArmValExceptionArmv7m_Action_0( void )
{
  ScoreCpuArmValExceptionFrame_Run(
    UsageFaultScenario,
    0,
    ARMV7M_VECTOR_USAGE_FAULT
  );

  /*
   * Check that the exception frame has the vector number of the UsageFault
   * exception.
   */
  ExceptionFrameCheckVector();

  /*
   * Check that the default exception handler placed the exception frame on the
   * interrupt stack.
   */
  ExceptionFrameCheckStack();
}

/**
 * @brief Disable the UsageFault exception. Prepare register values. Execute an
 *   undefined instruction, which escalates to a HardFault exception. Save the
 *   exception frame. Resume after the instruction. Run this through the
 *   register check, once for each value inverted, then once unchanged.
 */
static void ScoreCpuArmValExceptionArmv7m_Action_1( void )
{
  ScoreCpuArmValExceptionFrame_Run(
    HardFaultScenario,
    0,
    ARMV7M_VECTOR_HARD_FAULT
  );

  /*
   * Check that the exception frame has the vector number of the HardFault
   * exception.
   */
  ExceptionFrameCheckVector();

  /*
   * Check that the default exception handler placed the exception frame on the
   * interrupt stack.
   */
  ExceptionFrameCheckStack();
}

/**
 * @brief Enable the BusFault exception. Prepare register values. Load from an
 *   address which answers with a bus error. Save the exception frame. Resume
 *   after the load. Run this through the register check, once for each value
 *   inverted, then once unchanged.
 */
static void ScoreCpuArmValExceptionArmv7m_Action_2( void )
{
  ScoreCpuArmValExceptionFrame_Run(
    BusFaultScenario,
    UINT32_C( 1 ) << 14,
    ARMV7M_VECTOR_BUS_FAULT
  );

  /*
   * Check that the exception frame has the vector number of the BusFault
   * exception.
   */
  ExceptionFrameCheckVector();

  /*
   * Check that the default exception handler placed the exception frame on the
   * interrupt stack.
   */
  ExceptionFrameCheckStack();
}

/**
 * @brief Enable the MemManage exception. Prepare register values. Branch to
 *   the System region, which never executes. Save the exception frame. Resume
 *   after the branch. Run this through the register check, once for each value
 *   inverted, then once unchanged.
 */
static void ScoreCpuArmValExceptionArmv7m_Action_3( void )
{
  ScoreCpuArmValExceptionFrame_Run(
    MemManageScenario,
    UINT32_C( 1 ) << 14,
    ARMV7M_VECTOR_MEM_MANAGE
  );

  /*
   * Check that the exception frame has the vector number of the MemManage
   * exception.
   */
  ExceptionFrameCheckVector();

  /*
   * Check that the default exception handler placed the exception frame on the
   * interrupt stack.
   */
  ExceptionFrameCheckStack();
}

/**
 * @brief Set the NMI pending. Save the exception frame. Resume at the
 *   interrupted instruction. Run this through the register check, once for
 *   each value inverted, then once unchanged.
 */
static void ScoreCpuArmValExceptionArmv7m_Action_4( void )
{
  ScoreCpuArmValExceptionFrame_Run(
    NmiScenario,
    ( UINT32_C( 1 ) << 0 ) | ( UINT32_C( 1 ) << 1 ),
    ARMV7M_VECTOR_NMI
  );

  /*
   * Check that the exception frame has the vector number of the NMI exception.
   */
  ExceptionFrameCheckVector();

  /*
   * Check that the default exception handler placed the exception frame on the
   * interrupt stack.
   */
  ExceptionFrameCheckStack();
}

/**
 * @fn void T_case_body_ScoreCpuArmValExceptionArmv7m( void )
 */
T_TEST_CASE_FIXTURE(
  ScoreCpuArmValExceptionArmv7m,
  &ScoreCpuArmValExceptionArmv7m_Fixture
)
{
  ScoreCpuArmValExceptionArmv7m_Action_0();
  ScoreCpuArmValExceptionArmv7m_Action_1();
  ScoreCpuArmValExceptionArmv7m_Action_2();
  ScoreCpuArmValExceptionArmv7m_Action_3();
  ScoreCpuArmValExceptionArmv7m_Action_4();
}

/** @} */
