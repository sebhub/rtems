/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspRiscvValReset
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

#include <setjmp.h>
#include <bsp/bootcard.h>
#include <rtems/rtems/cache.h>
#include <rtems/rtems/intr.h>

#include <rtems/test.h>

/**
 * @defgroup BspRiscvValReset spec:/bsp/riscv/val/reset
 *
 * @ingroup TestsuitesBspsValidationBsp0
 *
 * @brief Tests the bsp_reset() function of the BSP.
 *
 * This test case performs the following actions:
 *
 * - Find the semihosting trap sequence in the bsp_reset() function. Replace
 *   its first two instructions with a jump to a function which records the
 *   operation and the parameter block of the semihosting exit and which leaves
 *   the reset function. Call the reset function with a fatal code. Restore the
 *   instructions.
 *
 *   - Check that the reset function issued the semihosting exit.
 *
 *   - Check that the stop reason is ADP_Stopped_ApplicationExit.
 *
 *   - Check that the exit code is the fatal code.
 *
 *   - Check that the trap instruction is restored.
 *
 * @{
 */

#ifdef RTEMS_GCOV_COVERAGE
#define BSP_RESET __real_bsp_reset
#else
#define BSP_RESET bsp_reset
#endif

RTEMS_NO_RETURN void BSP_RESET(
  rtems_fatal_source source,
  rtems_fatal_code   code
);

#define FATAL_CODE 123

#define SEMIHOSTING_SLLI 0x01f01013

#define SEMIHOSTING_EBREAK 0x00100073

#define SEMIHOSTING_SRAI 0x40705013

#define TARGET_SYS_EXIT_EXTENDED 0x20

#define ADP_Stopped_ApplicationExit 0x20026

#define SCAN_HALFWORDS 256

typedef struct {
  jmp_buf   leave;
  uintptr_t op;
  uintptr_t block[ 2 ];
} ResetContext;

static ResetContext reset_context;

static RTEMS_NO_RETURN void ResetLeave( uintptr_t op, const uintptr_t *block )
{
  reset_context.op = op;
  reset_context.block[ 0 ] = block[ 0 ];
  reset_context.block[ 1 ] = block[ 1 ];
  longjmp( reset_context.leave, 1 );
}

static uint32_t ReadWord( const uint16_t *at )
{
  return (uint32_t) at[ 0 ] | ( (uint32_t) at[ 1 ] << 16 );
}

static void WriteWord( uint16_t *at, uint32_t value )
{
  at[ 0 ] = (uint16_t) value;
  at[ 1 ] = (uint16_t) ( value >> 16 );
}

/*
 * The trap sequence consists of three uncompressed instructions.  The scan
 * steps by halfwords, since compressed instructions may precede it.
 */
static uint16_t *FindSemihostingTrap( void )
{
  uint16_t *at;
  size_t    i;

  at = (uint16_t *) (uintptr_t) BSP_RESET;

  for ( i = 0; i < SCAN_HALFWORDS; ++i ) {
    if (
      ReadWord( &at[ i ] ) == SEMIHOSTING_SLLI &&
      ReadWord( &at[ i + 2 ] ) == SEMIHOSTING_EBREAK &&
      ReadWord( &at[ i + 4 ] ) == SEMIHOSTING_SRAI
    ) {
      return &at[ i ];
    }
  }

  return NULL;
}

/*
 * Encode "auipc t0, %hi(target)" and "jalr zero, %lo(target)(t0)" for the
 * address of the first instruction.
 */
static void EncodeJump( uint16_t *at, uintptr_t target, uint32_t insn[ 2 ] )
{
  int32_t offset;
  int32_t hi;
  int32_t lo;

  offset = (int32_t) ( target - (uintptr_t) at );
  lo = (int32_t) ( (uint32_t) offset << 20 ) >> 20;
  hi = offset - lo;
  insn[ 0 ] = ( (uint32_t) hi & 0xfffff000 ) | ( 5 << 7 ) | 0x17;
  insn[ 1 ] = ( (uint32_t) lo << 20 ) | ( 5 << 15 ) | 0x67;
}

static void Patch( uint16_t *at, const uint32_t insn[ 2 ] )
{
  WriteWord( &at[ 0 ], insn[ 0 ] );
  WriteWord( &at[ 2 ], insn[ 1 ] );
  rtems_cache_instruction_sync_after_code_change( at, 8 );
}

/**
 * @brief Find the semihosting trap sequence in the bsp_reset() function.
 *   Replace its first two instructions with a jump to a function which records
 *   the operation and the parameter block of the semihosting exit and which
 *   leaves the reset function. Call the reset function with a fatal code.
 *   Restore the instructions.
 */
static void BspRiscvValReset_Action_0( void )
{
  uint16_t             *at;
  uint32_t              jump[ 2 ];
  uint32_t              original[ 2 ];
  rtems_interrupt_level level;

  at = FindSemihostingTrap();
  T_step_assert_not_null( 0, at );

  original[ 0 ] = ReadWord( &at[ 0 ] );
  original[ 1 ] = ReadWord( &at[ 2 ] );
  EncodeJump( at, (uintptr_t) ResetLeave, jump );

  rtems_interrupt_local_disable( level );
  Patch( at, jump );

  if ( setjmp( reset_context.leave ) == 0 ) {
    BSP_RESET( RTEMS_FATAL_SOURCE_APPLICATION, FATAL_CODE );
  }

  Patch( at, original );
  rtems_interrupt_local_enable( level );

  /*
   * Check that the reset function issued the semihosting exit.
   */
  T_step_eq_uptr( 1, reset_context.op, TARGET_SYS_EXIT_EXTENDED );

  /*
   * Check that the stop reason is ADP_Stopped_ApplicationExit.
   */
  T_step_eq_uptr( 2, reset_context.block[ 0 ], ADP_Stopped_ApplicationExit );

  /*
   * Check that the exit code is the fatal code.
   */
  T_step_eq_uptr( 3, reset_context.block[ 1 ], FATAL_CODE );

  /*
   * Check that the trap instruction is restored.
   */
  T_step_eq_u32( 4, ReadWord( &at[ 2 ] ), SEMIHOSTING_EBREAK );
}

/**
 * @fn void T_case_body_BspRiscvValReset( void )
 */
T_TEST_CASE( BspRiscvValReset )
{
  T_plan( 5 );

  BspRiscvValReset_Action_0();
}

/** @} */
