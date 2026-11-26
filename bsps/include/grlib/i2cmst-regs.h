/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRLIBI2CMST
 *
 * @brief This header file defines the I2CMST register block interface.
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

/*
 * The spec:/... comments allow you to identify content which is
 * duplicated in the rtems and rtems-docs repositories.
 */

/* Find related documentation with spec:/dev/grlib/if/i2cmst-header */

#ifndef _GRLIB_I2CMST_REGS_H
#define _GRLIB_I2CMST_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/i2cmst */

/**
 * @defgroup RTEMSDeviceGRLIBI2CMST I2CMST
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the I2CMST interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRLIBI2CMSTPRESCALE \
 *   I2C-master clock prescale register (PRESCALE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CMST_PRESCALE_CLOCK_PRESCALE_SHIFT 0
#define I2CMST_PRESCALE_CLOCK_PRESCALE_MASK  0xffffU
#define I2CMST_PRESCALE_CLOCK_PRESCALE_GET( _reg )        \
  ( ( ( _reg ) & I2CMST_PRESCALE_CLOCK_PRESCALE_MASK ) >> \
    I2CMST_PRESCALE_CLOCK_PRESCALE_SHIFT )
#define I2CMST_PRESCALE_CLOCK_PRESCALE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~I2CMST_PRESCALE_CLOCK_PRESCALE_MASK ) |    \
    ( ( ( _val ) << I2CMST_PRESCALE_CLOCK_PRESCALE_SHIFT ) & \
      I2CMST_PRESCALE_CLOCK_PRESCALE_MASK ) )
#define I2CMST_PRESCALE_CLOCK_PRESCALE( _val )             \
  ( ( ( _val ) << I2CMST_PRESCALE_CLOCK_PRESCALE_SHIFT ) & \
    I2CMST_PRESCALE_CLOCK_PRESCALE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBI2CMSTCTRL I2C-master control register (CTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CMST_CTRL_EN 0x80U

#define I2CMST_CTRL_IEN 0x40U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBI2CMSTTXRX \
 *   I2C-master transmit and receive register (TXRX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CMST_TXRX_TDATA_SHIFT 1
#define I2CMST_TXRX_TDATA_MASK  0xfeU
#define I2CMST_TXRX_TDATA_GET( _reg ) \
  ( ( ( _reg ) & I2CMST_TXRX_TDATA_MASK ) >> I2CMST_TXRX_TDATA_SHIFT )
#define I2CMST_TXRX_TDATA_SET( _reg, _val )  \
  ( ( ( _reg ) & ~I2CMST_TXRX_TDATA_MASK ) | \
    ( ( ( _val ) << I2CMST_TXRX_TDATA_SHIFT ) & I2CMST_TXRX_TDATA_MASK ) )
#define I2CMST_TXRX_TDATA( _val ) \
  ( ( ( _val ) << I2CMST_TXRX_TDATA_SHIFT ) & I2CMST_TXRX_TDATA_MASK )

#define I2CMST_TXRX_RW 0x1U

#define I2CMST_TXRX_RDATA_SHIFT 0
#define I2CMST_TXRX_RDATA_MASK  0xffU
#define I2CMST_TXRX_RDATA_GET( _reg ) \
  ( ( ( _reg ) & I2CMST_TXRX_RDATA_MASK ) >> I2CMST_TXRX_RDATA_SHIFT )
#define I2CMST_TXRX_RDATA_SET( _reg, _val )  \
  ( ( ( _reg ) & ~I2CMST_TXRX_RDATA_MASK ) | \
    ( ( ( _val ) << I2CMST_TXRX_RDATA_SHIFT ) & I2CMST_TXRX_RDATA_MASK ) )
#define I2CMST_TXRX_RDATA( _val ) \
  ( ( ( _val ) << I2CMST_TXRX_RDATA_SHIFT ) & I2CMST_TXRX_RDATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBI2CMSTCMDSTAT \
 *   I2C-master command and status register (CMDSTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CMST_CMDSTAT_STA 0x80U

#define I2CMST_CMDSTAT_STO 0x40U

#define I2CMST_CMDSTAT_RD 0x20U

#define I2CMST_CMDSTAT_WR 0x10U

#define I2CMST_CMDSTAT_ACK 0x8U

#define I2CMST_CMDSTAT_IACK 0x1U

#define I2CMST_CMDSTAT_RXACK 0x80U

#define I2CMST_CMDSTAT_BUSY 0x40U

#define I2CMST_CMDSTAT_AL 0x20U

#define I2CMST_CMDSTAT_TIP 0x2U

#define I2CMST_CMDSTAT_IF 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBI2CMSTFILT \
 *   I2C-master dynamic filter register (FILT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CMST_FILT_FILT_SHIFT 0
#define I2CMST_FILT_FILT_MASK  0xffffffffU
#define I2CMST_FILT_FILT_GET( _reg ) \
  ( ( ( _reg ) & I2CMST_FILT_FILT_MASK ) >> I2CMST_FILT_FILT_SHIFT )
#define I2CMST_FILT_FILT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~I2CMST_FILT_FILT_MASK ) | \
    ( ( ( _val ) << I2CMST_FILT_FILT_SHIFT ) & I2CMST_FILT_FILT_MASK ) )
#define I2CMST_FILT_FILT( _val ) \
  ( ( ( _val ) << I2CMST_FILT_FILT_SHIFT ) & I2CMST_FILT_FILT_MASK )

/** @} */

/**
 * @brief This structure defines the I2CMST register block memory map.
 */
typedef struct i2cmst {
  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CMSTPRESCALE.
   */
  uint32_t prescale;

  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CMSTCTRL.
   */
  uint32_t ctrl;

  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CMSTTXRX.
   */
  uint32_t txrx;

  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CMSTCMDSTAT.
   */
  uint32_t cmdstat;

  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CMSTFILT.
   */
  uint32_t filt;
} i2cmst;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_I2CMST_REGS_H */
