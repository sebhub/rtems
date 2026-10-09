/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup BspSparcLeon3ValMpIrq
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

#include <bsp.h>

#include <rtems/test.h>

/**
 * @defgroup BspSparcLeon3ValMpIrq spec:/bsp/sparc/leon3/val/mp-irq
 *
 * @ingroup TestsuitesBspsValidationBsp0
 *
 * @brief Tests the default value of LEON3_mp_irq.
 *
 * This test case performs the following actions:
 *
 * - Read the value of LEON3_mp_irq. The test program defines no LEON3_mp_irq
 *   constant.
 *
 *   - Check that the value is the value of the BSP option LEON3_IPI_BUS_LINE.
 *
 * @{
 */

/**
 * @brief Read the value of LEON3_mp_irq. The test program defines no
 *   LEON3_mp_irq constant.
 */
static void BspSparcLeon3ValMpIrq_Action_0( void )
{
  unsigned char bus_line;

  bus_line = LEON3_mp_irq;

  /*
   * Check that the value is the value of the BSP option LEON3_IPI_BUS_LINE.
   */
  T_eq_uint( bus_line, LEON3_IPI_BUS_LINE );
}

/**
 * @fn void T_case_body_BspSparcLeon3ValMpIrq( void )
 */
T_TEST_CASE( BspSparcLeon3ValMpIrq )
{
  BspSparcLeon3ValMpIrq_Action_0();
}

/** @} */
