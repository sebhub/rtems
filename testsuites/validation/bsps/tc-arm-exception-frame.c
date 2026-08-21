/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuArmValExceptionFrame
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

#include <string.h>
#include <rtems/score/cpuimpl.h>
#include <rtems/score/isr.h>

#include "tr-arm-exception-frame.h"
#include "tx-register-check.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuArmValExceptionFrame \
 *   spec:/score/cpu/arm/val/exception-frame
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks the an exception frame has the expected values.
 *
 * This test case performs the following actions:
 *
 * - Run the scenario of the caller through the register check, once for each
 *   value with the value inverted, then once unchanged.
 *
 *   - Check that the ARM core register R0 was saved to the exception frame.
 *
 *   - Check that the ARM core register R1 was saved to the exception frame.
 *
 *   - Check that the ARM core register R2 was saved to the exception frame.
 *
 *   - Check that the ARM core register R3 was saved to the exception frame.
 *
 *   - Check that the ARM core register R4 was saved to the exception frame.
 *
 *   - Check that the ARM core register R5 was saved to the exception frame.
 *
 *   - Check that the ARM core register R6 was saved to the exception frame.
 *
 *   - Check that the ARM core register R7 was saved to the exception frame.
 *
 *   - Check that the ARM core register R8 was saved to the exception frame.
 *
 *   - Check that the ARM core register R9 was saved to the exception frame.
 *
 *   - Check that the ARM core register R10 was saved to the exception frame.
 *
 *   - Check that the ARM core register R11 was saved to the exception frame.
 *
 *   - Check that the ARM core register R12 was saved to the exception frame.
 *
 *   - Check that the ARM core register R13 was saved to the exception frame.
 *
 *   - Check that the ARM core register R14 was saved to the exception frame.
 *
 *   - Check that the ARM core register R15 was saved to the exception frame.
 *
 *   - Check that the Current Program Status Register (CPSR) or the xPSR was
 *     saved to the exception frame.
 *
 *   - Check that the ARM FPU register D0 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D1 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D2 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D3 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D4 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D5 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D6 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D7 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D8 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D9 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D10 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D11 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D12 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D13 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D14 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D15 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D16 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D17 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D18 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D19 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D20 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D21 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D22 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D23 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D24 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D25 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D26 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D27 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D28 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D29 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D30 was saved to the exception frame.
 *
 *   - Check that the ARM FPU register D31 was saved to the exception frame.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

/**
 * @brief Test context for spec:/score/cpu/arm/val/exception-frame test case.
 */
typedef struct {
  /**
   * @brief This member contains a copy of the corresponding
   *   ScoreCpuArmValExceptionFrame_Run() parameter.
   */
  void ( *scenario )( void );

  /**
   * @brief This member contains a copy of the corresponding
   *   ScoreCpuArmValExceptionFrame_Run() parameter.
   */
  uint32_t overrides;

  /**
   * @brief This member contains a copy of the corresponding
   *   ScoreCpuArmValExceptionFrame_Run() parameter.
   */
  uint32_t vector;
} ScoreCpuArmValExceptionFrame_Context;

static ScoreCpuArmValExceptionFrame_Context
  ScoreCpuArmValExceptionFrame_Instance;

/*
 * The fatal handler ends the FIQ of the test through _TM27_Clear_fast().
 */
#define _RTEMS_TMTEST27
#include <tm27.h>

typedef enum {
  SOURCE_R0,
  SOURCE_R1,
  SOURCE_R2,
  SOURCE_R3,
  SOURCE_R4,
  SOURCE_R5,
  SOURCE_R6,
  SOURCE_R7,
  SOURCE_R8,
  SOURCE_R9,
  SOURCE_R10,
  SOURCE_R11,
  SOURCE_R12,
  SOURCE_R13,
  SOURCE_R14,
  SOURCE_STATUS,
#ifdef ARM_MULTILIB_VFP
  SOURCE_D0,
  SOURCE_D1,
  SOURCE_D2,
  SOURCE_D3,
  SOURCE_D4,
  SOURCE_D5,
  SOURCE_D6,
  SOURCE_D7,
  SOURCE_D8,
  SOURCE_D9,
  SOURCE_D10,
  SOURCE_D11,
  SOURCE_D12,
  SOURCE_D13,
  SOURCE_D14,
  SOURCE_D15,
#ifdef ARM_MULTILIB_VFP_D32
  SOURCE_D16,
  SOURCE_D17,
  SOURCE_D18,
  SOURCE_D19,
  SOURCE_D20,
  SOURCE_D21,
  SOURCE_D22,
  SOURCE_D23,
  SOURCE_D24,
  SOURCE_D25,
  SOURCE_D26,
  SOURCE_D27,
  SOURCE_D28,
  SOURCE_D29,
  SOURCE_D30,
  SOURCE_D31,
#else
  SOURCE_D16_D31_UNUSED,
#endif
#else
  SOURCE_VFP_CONTEXT,
#endif
  SOURCE_R15,
  SOURCE_VECTOR,
#if defined( ARM_MULTILIB_ARCH_V7M )
  SOURCE_STACK,
#endif
  SOURCE_MAX
} Source;

typedef enum {
  SLOT_R0,
  SLOT_R1,
  SLOT_R2,
  SLOT_R3,
  SLOT_R4,
  SLOT_R5,
  SLOT_R6,
  SLOT_R7,
  SLOT_R8,
  SLOT_R9,
  SLOT_R10,
  SLOT_R11,
  SLOT_R12,
  SLOT_R13,
  SLOT_R14,
  SLOT_R15,
  SLOT_STATUS,
#ifdef ARM_MULTILIB_VFP
  SLOT_D0,
  SLOT_D1,
  SLOT_D2,
  SLOT_D3,
  SLOT_D4,
  SLOT_D5,
  SLOT_D6,
  SLOT_D7,
  SLOT_D8,
  SLOT_D9,
  SLOT_D10,
  SLOT_D11,
  SLOT_D12,
  SLOT_D13,
  SLOT_D14,
  SLOT_D15,
  SLOT_D16,
  SLOT_D17,
  SLOT_D18,
  SLOT_D19,
  SLOT_D20,
  SLOT_D21,
  SLOT_D22,
  SLOT_D23,
  SLOT_D24,
  SLOT_D25,
  SLOT_D26,
  SLOT_D27,
  SLOT_D28,
  SLOT_D29,
  SLOT_D30,
  SLOT_D31,
#else
  SLOT_VFP_CONTEXT,
#endif
  SLOT_VECTOR,
#if defined( ARM_MULTILIB_ARCH_V7M )
  SLOT_STACK,
#endif
  SLOT_MAX
} Slot;

RTEMS_STATIC_ASSERT( SOURCE_R14 == 14, source_r14 );
RTEMS_STATIC_ASSERT( SOURCE_STATUS == 15, source_status );
#ifdef ARM_MULTILIB_VFP
RTEMS_STATIC_ASSERT( SOURCE_D0 == 16, source_d0 );
#endif
RTEMS_STATIC_ASSERT( SLOT_R15 == 15, slot_r15 );

#define WORD UINT64_C( 0xffffffff )

#if defined( ARM_MULTILIB_ARCH_V7M )
#define STATUS_TEST_MASK XPSR_TEST_MASK
#define STATUS_TEST_BITS XPSR_TEST_BITS
#else
#define STATUS_TEST_MASK CPSR_TEST_MASK
#define STATUS_TEST_BITS CPSR_TEST_BITS
#endif

static const RegisterCheckSource exception_frame_base_sources[ SOURCE_MAX ] = {
  { "r0", REGISTER_CHECK_INITIAL, WORD },
  { "r1", REGISTER_CHECK_INITIAL, WORD },
  { "r2", REGISTER_CHECK_INITIAL, WORD },
  { "r3", REGISTER_CHECK_INITIAL, WORD },
  { "r4", REGISTER_CHECK_INITIAL, WORD },
  { "r5", REGISTER_CHECK_INITIAL, WORD },
  { "r6", REGISTER_CHECK_INITIAL, WORD },
  { "r7", REGISTER_CHECK_INITIAL, WORD },
  { "r8", REGISTER_CHECK_INITIAL, WORD },
  { "r9", REGISTER_CHECK_INITIAL, WORD },
  { "r10", REGISTER_CHECK_INITIAL, WORD },
  { "r11", REGISTER_CHECK_INITIAL, WORD },
  { "r12", REGISTER_CHECK_INITIAL, WORD },
  { "r13", REGISTER_CHECK_INITIAL, WORD },
  { "r14", REGISTER_CHECK_INITIAL, WORD },
  { "status", REGISTER_CHECK_INITIAL, CPSR_SET_MASK },
#ifdef ARM_MULTILIB_VFP
  { "d0", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d1", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d2", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d3", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d4", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d5", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d6", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d7", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d8", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d9", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d10", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d11", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d12", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d13", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d14", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d15", REGISTER_CHECK_INITIAL, UINT64_MAX },
#ifdef ARM_MULTILIB_VFP_D32
  { "d16", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d17", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d18", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d19", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d20", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d21", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d22", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d23", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d24", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d25", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d26", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d27", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d28", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d29", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d30", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "d31", REGISTER_CHECK_INITIAL, UINT64_MAX },
#else
  { "d16 to d31 unused", REGISTER_CHECK_EXPECTED, UINT64_MAX },
#endif
#else
  { "vfp_context", REGISTER_CHECK_EXPECTED, WORD },
#endif
  { "r15", REGISTER_CHECK_EXPECTED, WORD },
  { "vector", REGISTER_CHECK_EXPECTED, WORD },
#if defined( ARM_MULTILIB_ARCH_V7M )
  { "stack", REGISTER_CHECK_EXPECTED, WORD },
#endif
};

static RegisterCheckSource exception_frame_sources[ SOURCE_MAX ];

static const RegisterCheckSlot exception_frame_slots[ SLOT_MAX ] = {
  { "r0", SOURCE_R0, WORD },
  { "r1", SOURCE_R1, WORD },
  { "r2", SOURCE_R2, WORD },
  { "r3", SOURCE_R3, WORD },
  { "r4", SOURCE_R4, WORD },
  { "r5", SOURCE_R5, WORD },
  { "r6", SOURCE_R6, WORD },
  { "r7", SOURCE_R7, WORD },
  { "r8", SOURCE_R8, WORD },
  { "r9", SOURCE_R9, WORD },
  { "r10", SOURCE_R10, WORD },
  { "r11", SOURCE_R11, WORD },
  { "r12", SOURCE_R12, WORD },
  { "r13", SOURCE_R13, WORD },
  { "r14", SOURCE_R14, WORD },
  { "r15", SOURCE_R15, WORD },
  { "status", SOURCE_STATUS, STATUS_TEST_MASK },
#ifdef ARM_MULTILIB_VFP
  { "d0", SOURCE_D0, UINT64_MAX },
  { "d1", SOURCE_D1, UINT64_MAX },
  { "d2", SOURCE_D2, UINT64_MAX },
  { "d3", SOURCE_D3, UINT64_MAX },
  { "d4", SOURCE_D4, UINT64_MAX },
  { "d5", SOURCE_D5, UINT64_MAX },
  { "d6", SOURCE_D6, UINT64_MAX },
  { "d7", SOURCE_D7, UINT64_MAX },
  { "d8", SOURCE_D8, UINT64_MAX },
  { "d9", SOURCE_D9, UINT64_MAX },
  { "d10", SOURCE_D10, UINT64_MAX },
  { "d11", SOURCE_D11, UINT64_MAX },
  { "d12", SOURCE_D12, UINT64_MAX },
  { "d13", SOURCE_D13, UINT64_MAX },
  { "d14", SOURCE_D14, UINT64_MAX },
  { "d15", SOURCE_D15, UINT64_MAX },
#ifdef ARM_MULTILIB_VFP_D32
  { "d16", SOURCE_D16, UINT64_MAX },
  { "d17", SOURCE_D17, UINT64_MAX },
  { "d18", SOURCE_D18, UINT64_MAX },
  { "d19", SOURCE_D19, UINT64_MAX },
  { "d20", SOURCE_D20, UINT64_MAX },
  { "d21", SOURCE_D21, UINT64_MAX },
  { "d22", SOURCE_D22, UINT64_MAX },
  { "d23", SOURCE_D23, UINT64_MAX },
  { "d24", SOURCE_D24, UINT64_MAX },
  { "d25", SOURCE_D25, UINT64_MAX },
  { "d26", SOURCE_D26, UINT64_MAX },
  { "d27", SOURCE_D27, UINT64_MAX },
  { "d28", SOURCE_D28, UINT64_MAX },
  { "d29", SOURCE_D29, UINT64_MAX },
  { "d30", SOURCE_D30, UINT64_MAX },
  { "d31", SOURCE_D31, UINT64_MAX },
#else
  { "d16", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d17", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d18", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d19", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d20", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d21", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d22", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d23", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d24", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d25", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d26", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d27", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d28", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d29", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d30", SOURCE_D16_D31_UNUSED, UINT64_MAX },
  { "d31", SOURCE_D16_D31_UNUSED, UINT64_MAX },
#endif
#else
  { "vfp_context", SOURCE_VFP_CONTEXT, WORD },
#endif
  { "vector", SOURCE_VECTOR, WORD },
#if defined( ARM_MULTILIB_ARCH_V7M )
  { "stack", SOURCE_STACK, WORD },
#endif
};

static const uint64_t exception_frame_patterns[ SOURCE_MAX ] = {
  0x01020304,
  0x05060708,
  0x090a0b0c,
  0x0d0e0f10,
  0x11121314,
  0x15161718,
  0x191a1b1c,
  0x1d1e1f10,
  0x21222324,
  0x25262728,
  0x292a2b2c,
  0x2d2e2f20,
  0x31323334,
  0x35363738,
  0x393a3b3c,
  CPSR_SET_BITS,
#ifdef ARM_MULTILIB_VFP
  0x0102030405060708,
  0x090a0b0c0d0e0f10,
  0x1112131415161718,
  0x191a1b1c1d1e1f20,
  0x2122232425262728,
  0x292a2b2c2d2e2f30,
  0x3132333435363738,
  0x393a3b3c3d3e3f40,
  0x4142434445464748,
  0x494a4b4c4d4e4f50,
  0x5152535455565758,
  0x595a5b5c5d5e5f60,
  0x6162636465666768,
  0x696a6b6c6d6e6f70,
  0x7172737475767778,
  0x797a7b7c7d7e7f80,
#ifdef ARM_MULTILIB_VFP_D32
  0x8182838485868788,
  0x898a8b8c8d8e8f90,
  0x9192939495969798,
  0x999a9b9c9d9e9fa0,
  0xa1a2a3a4a5a6a7a8,
  0xa9aaabacadaeafb0,
  0xb1b2b3b4b5b6b7b8,
  0xb9babbbcbdbebfc0,
  0xc1c2c3c4c5c6c7c8,
  0xc9cacbcccdcecfd0,
  0xd1d2d3d4d5d6d7d8,
  0xd9dadbdcdddedfe0,
  0xe1e2e3e4e5e6e7e8,
  0xe9eaebecedeeeff0,
  0xf1f2f3f4f5f6f7f8,
  0xf9fafbfcfdfeff00,
#endif
#endif
};

uint64_t arm_exception_initial[ SOURCE_MAX ];

uint32_t exception_expected_registers[ 15 ];

const void *exception_expected_pc;

static CPU_Exception_frame exception_frame;

static void ( *exception_frame_scenario )( void );

static ARM_VFP_context vfp_context;

volatile uint32_t exception_restore_r7;

volatile uint32_t exception_restore_sp;

const void *volatile exception_resume_pc;

const CPU_Exception_frame *volatile exception_frame_address;

static void Fatal(
  rtems_fatal_source source,
  rtems_fatal_code   code,
  void              *arg
)
{
  (void) arg;

  SetFatalHandler( NULL, NULL );

  if ( source == RTEMS_FATAL_SOURCE_EXCEPTION ) {
    CPU_Exception_frame *frame;

    frame = (CPU_Exception_frame *) code;
    exception_frame = *frame;

#ifdef ARM_MULTILIB_VFP
    if ( frame->vfp_context != NULL ) {
      vfp_context = *frame->vfp_context;
      exception_frame.vfp_context = &vfp_context;
    }
#endif

    exception_frame_address = frame;

#if defined( ARM_MULTILIB_ARCH_V4 )
    switch ( frame->vector ) {
      case ARM_EXCEPTION_PREF_ABORT:
        frame->register_pc = exception_resume_prefetch_abort_label;
        break;
      case ARM_EXCEPTION_FIQ:
#ifdef TM27_INTERRUPT_VECTOR_FAST
        _TM27_Clear_fast();
#endif
        frame->register_cpsr |= ARM_PSR_F;
        break;
      default:
        break;
    }
#else
    /*
     * The hardware frame of a synchronous fault holds the address of the
     * faulting instruction.  A resume there would fault again.
     */
    if ( exception_resume_pc != NULL ) {
      frame->register_pc = (void *) exception_resume_pc;
    }
#endif

    frame->register_r7 = exception_restore_r7;
    frame->register_sp = exception_restore_sp;
    _CPU_Exception_resume( frame );
  }
}

CPU_Exception_frame *ExceptionFrameInitialize( void )
{
  memset( &exception_frame, 0xff, sizeof( exception_frame ) );
  memset( &vfp_context, 0xff, sizeof( vfp_context ) );
  exception_frame.vfp_context = &vfp_context;
  exception_resume_pc = NULL;
  exception_frame_address = NULL;
  SetFatalHandler( Fatal, NULL );

  return &exception_frame;
}

#if defined( ARM_MULTILIB_ARCH_V7M )
static bool IsOnInterruptStack( const CPU_Exception_frame *frame )
{
  uintptr_t address;

  address = (uintptr_t) frame;

  return address >= (uintptr_t) _ISR_Stack_area_begin &&
         address < (uintptr_t) _ISR_Stack_area_end;
}
#endif

static void ExceptionFrameRun( RegisterCheck *self, void *arg )
{
  const CPU_Exception_frame *frame;
  const uint64_t            *patterns;
  uint32_t                   overrides;
  uint32_t                   vector;
  size_t                     i;

  overrides = ( (const uint32_t *) arg )[ 0 ];
  vector = ( (const uint32_t *) arg )[ 1 ];
  memset(
    exception_expected_registers,
    0,
    sizeof( exception_expected_registers )
  );
  exception_expected_pc = NULL;
  ( *exception_frame_scenario )();
  frame = &exception_frame;
  patterns = exception_frame_patterns;

  for ( i = 0; i < 15; ++i ) {
    uint64_t expected;

    if ( ( overrides & ( UINT32_C( 1 ) << i ) ) != 0 ) {
      expected = exception_expected_registers[ i ];
    } else {
      expected = patterns[ SOURCE_R0 + i ];
    }

    RegisterCheckRecord( self, SLOT_R0 + i, frame->registers[ i ], expected );
  }

  RegisterCheckRecord(
    self,
    SLOT_R15,
    (uintptr_t) frame->register_pc,
    (uintptr_t) exception_expected_pc
  );
#if defined( ARM_MULTILIB_ARCH_V7M )
  RegisterCheckRecord(
    self,
    SLOT_STATUS,
    frame->register_xpsr,
    ( patterns[ SOURCE_STATUS ] & CPSR_SET_MASK ) |
      ( STATUS_TEST_BITS & ~CPSR_SET_MASK )
  );
#else
  RegisterCheckRecord(
    self,
    SLOT_STATUS,
    frame->register_cpsr,
    ( patterns[ SOURCE_STATUS ] & CPSR_SET_MASK ) |
      ( STATUS_TEST_BITS & ~CPSR_SET_MASK )
  );
#endif

#ifdef ARM_MULTILIB_VFP
  for ( i = 0; i < 32; ++i ) {
    uint64_t expected;

#ifdef ARM_MULTILIB_VFP_D32
    expected = patterns[ SOURCE_D0 + i ];
#else
    if ( i < 16 ) {
      expected = patterns[ SOURCE_D0 + i ];
    } else {
      expected = 0;
    }
#endif

    RegisterCheckRecord(
      self,
      SLOT_D0 + i,
      ( &vfp_context.register_d0 )[ i ],
      expected
    );
  }
#else
  RegisterCheckRecord(
    self,
    SLOT_VFP_CONTEXT,
    (uintptr_t) frame->vfp_context,
    0
  );
#endif

  RegisterCheckRecord( self, SLOT_VECTOR, (uint32_t) frame->vector, vector );
#if defined( ARM_MULTILIB_ARCH_V7M )
  RegisterCheckRecord(
    self,
    SLOT_STACK,
    IsOnInterruptStack( exception_frame_address ) ? 1 : 0,
    1
  );
#endif
}

static uint32_t exception_frame_params[ 2 ];

static RegisterCheck exception_frame_check = {
  .sources = exception_frame_sources,
  .source_count = SOURCE_MAX,
  .slots = exception_frame_slots,
  .slot_count = SLOT_MAX,
  .initial = arm_exception_initial,
  .run = ExceptionFrameRun,
  .arg = exception_frame_params
};

static void CheckSlot( Slot slot )
{
  RegisterCheckVerify( &exception_frame_check, slot );
}

const CPU_Exception_frame *ExceptionFrameGet( void )
{
  return &exception_frame;
}

void ExceptionFrameCheckVector( void )
{
  CheckSlot( SLOT_VECTOR );
}

#if defined( ARM_MULTILIB_ARCH_V7M )
void ExceptionFrameCheckStack( void )
{
  CheckSlot( SLOT_STACK );
}
#endif

static void ExceptionFrameSetup( uint32_t overrides, uint32_t vector )
{
  size_t i;

#if defined( ARM_MULTILIB_ARCH_V7M )
  /*
   * The processor stacks the hardware frame of the exception on the stack
   * of the interrupted context, so the stack pointer keeps its value.
   */
  overrides |= UINT32_C( 1 ) << 13;
#endif

  memcpy(
    exception_frame_sources,
    exception_frame_base_sources,
    sizeof( exception_frame_sources )
  );

  for ( i = 0; i < 15; ++i ) {
    if ( ( overrides & ( UINT32_C( 1 ) << i ) ) != 0 ) {
      exception_frame_sources[ SOURCE_R0 + i ].kind = REGISTER_CHECK_EXPECTED;
    }
  }

  memcpy(
    arm_exception_initial,
    exception_frame_patterns,
    sizeof( arm_exception_initial )
  );
  exception_frame_params[ 0 ] = overrides;
  exception_frame_params[ 1 ] = vector;
}

static T_fixture ScoreCpuArmValExceptionFrame_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = NULL,
  .initial_context = &ScoreCpuArmValExceptionFrame_Instance
};

/**
 * @brief Run the scenario of the caller through the register check, once for
 *   each value with the value inverted, then once unchanged.
 */
static void ScoreCpuArmValExceptionFrame_Action_0(
  ScoreCpuArmValExceptionFrame_Context *ctx
)
{
  exception_frame_scenario = ctx->scenario;
  ExceptionFrameSetup( ctx->overrides, ctx->vector );
  RegisterCheckRun( &exception_frame_check );

  /*
   * Check that the ARM core register R0 was saved to the exception frame.
   */
  CheckSlot( SLOT_R0 );

  /*
   * Check that the ARM core register R1 was saved to the exception frame.
   */
  CheckSlot( SLOT_R1 );

  /*
   * Check that the ARM core register R2 was saved to the exception frame.
   */
  CheckSlot( SLOT_R2 );

  /*
   * Check that the ARM core register R3 was saved to the exception frame.
   */
  CheckSlot( SLOT_R3 );

  /*
   * Check that the ARM core register R4 was saved to the exception frame.
   */
  CheckSlot( SLOT_R4 );

  /*
   * Check that the ARM core register R5 was saved to the exception frame.
   */
  CheckSlot( SLOT_R5 );

  /*
   * Check that the ARM core register R6 was saved to the exception frame.
   */
  CheckSlot( SLOT_R6 );

  /*
   * Check that the ARM core register R7 was saved to the exception frame.
   */
  CheckSlot( SLOT_R7 );

  /*
   * Check that the ARM core register R8 was saved to the exception frame.
   */
  CheckSlot( SLOT_R8 );

  /*
   * Check that the ARM core register R9 was saved to the exception frame.
   */
  CheckSlot( SLOT_R9 );

  /*
   * Check that the ARM core register R10 was saved to the exception frame.
   */
  CheckSlot( SLOT_R10 );

  /*
   * Check that the ARM core register R11 was saved to the exception frame.
   */
  CheckSlot( SLOT_R11 );

  /*
   * Check that the ARM core register R12 was saved to the exception frame.
   */
  CheckSlot( SLOT_R12 );

  /*
   * Check that the ARM core register R13 was saved to the exception frame.
   */
  CheckSlot( SLOT_R13 );

  /*
   * Check that the ARM core register R14 was saved to the exception frame.
   */
  CheckSlot( SLOT_R14 );

  /*
   * Check that the ARM core register R15 was saved to the exception frame.
   */
  CheckSlot( SLOT_R15 );

  /*
   * Check that the Current Program Status Register (CPSR) or the xPSR was
   * saved to the exception frame.
   */
  CheckSlot( SLOT_STATUS );

  /*
   * Check that the ARM FPU register D0 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D0 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D1 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D1 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D2 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D2 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D3 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D3 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D4 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D4 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D5 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D5 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D6 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D6 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D7 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D7 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D8 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D8 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D9 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D9 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D10 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D10 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D11 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D11 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D12 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D12 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D13 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D13 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D14 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D14 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D15 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D15 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D16 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D16 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D17 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D17 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D18 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D18 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D19 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D19 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D20 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D20 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D21 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D21 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D22 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D22 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D23 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D23 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D24 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D24 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D25 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D25 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D26 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D26 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D27 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D27 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D28 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D28 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D29 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D29 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D30 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D30 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that the ARM FPU register D31 was saved to the exception frame.
   */
  #ifdef ARM_MULTILIB_VFP
  CheckSlot( SLOT_D31 );
  #else
  CheckSlot( SLOT_VFP_CONTEXT );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  RegisterCheckReport( &exception_frame_check );
}

static T_fixture_node ScoreCpuArmValExceptionFrame_Node;

static T_remark ScoreCpuArmValExceptionFrame_Remark = {
  .next = NULL,
  .remark = "ScoreCpuArmValExceptionFrame"
};

void ScoreCpuArmValExceptionFrame_Run(
  void ( *scenario )( void ),
  uint32_t overrides,
  uint32_t vector
)
{
  ScoreCpuArmValExceptionFrame_Context *ctx;

  ctx = &ScoreCpuArmValExceptionFrame_Instance;
  ctx->scenario = scenario;
  ctx->overrides = overrides;
  ctx->vector = vector;

  ctx = T_push_fixture(
    &ScoreCpuArmValExceptionFrame_Node,
    &ScoreCpuArmValExceptionFrame_Fixture
  );

  ScoreCpuArmValExceptionFrame_Action_0( ctx );

  T_add_remark( &ScoreCpuArmValExceptionFrame_Remark );
  T_pop_fixture();
}

/** @} */
