/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspRiscvValMemory
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

#include <bsp/start.h>
#include <rtems/score/memory.h>

#include <rtems/test.h>

/**
 * @defgroup BspRiscvValMemory spec:/bsp/riscv/val/memory
 *
 * @ingroup TestsuitesBspsValidationBsp0
 *
 * @brief Tests the memory area of the BSP.
 *
 * This test case performs the following actions:
 *
 * - Get the end of the memory area of the BSP.
 *
 *   - Check that the stack pointer of the boot processor at the start entry is
 *     not zero.
 *
 *   - Check that the memory area ends at this stack pointer.
 *
 * @{
 */

/**
 * @brief Get the end of the memory area of the BSP.
 */
static void BspRiscvValMemory_Action_0( void )
{
  const void *end;

  end = _Memory_Get_end( _Memory_Get_area( _Memory_Get(), 0 ) );

  /*
   * Check that the stack pointer of the boot processor at the start entry is
   * not zero.
   */
  T_step_ne_uptr( 0, riscv_start_stack_pointer, 0 );

  /*
   * Check that the memory area ends at this stack pointer.
   */
  T_step_eq_ptr( 1, end, (void *) riscv_start_stack_pointer );
}

/**
 * @fn void T_case_body_BspRiscvValMemory( void )
 */
T_TEST_CASE( BspRiscvValMemory )
{
  T_plan( 2 );

  BspRiscvValMemory_Action_0();
}

/** @} */
