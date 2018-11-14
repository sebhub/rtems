/*
 * Copyright (c) 2017 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <info@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <stdint.h>
#include <bspopts.h>

#include <arm/freescale/imx/imx_ccmvar.h>

uint32_t imx_ccm_ipg_hz(void)
{
  return 33000000;
}

uint32_t imx_ccm_uart_hz(void)
{
  return 33000000;
}

uint32_t imx_ccm_ahb_hz(void)
{
  return 66000000;
}

uint32_t imx_ccm_sdhci_hz(void)
{
  return 198000000;
}
