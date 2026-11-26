/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRLIBI2CSLV
 *
 * @brief This header file defines the I2CSLV register block interface.
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

/* Find related documentation with spec:/dev/grlib/if/i2cslv-header */

#ifndef _GRLIB_I2CSLV_REGS_H
#define _GRLIB_I2CSLV_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/i2cslv */

/**
 * @defgroup RTEMSDeviceGRLIBI2CSLV I2CSLV
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the I2CSLV interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRLIBI2CSLVSLVADDR Slave address register (SLVADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CSLV_SLVADDR_TBA 0x80000000U

#define I2CSLV_SLVADDR_SLVADDR_SHIFT 0
#define I2CSLV_SLVADDR_SLVADDR_MASK  0x3ffU
#define I2CSLV_SLVADDR_SLVADDR_GET( _reg )        \
  ( ( ( _reg ) & I2CSLV_SLVADDR_SLVADDR_MASK ) >> \
    I2CSLV_SLVADDR_SLVADDR_SHIFT )
#define I2CSLV_SLVADDR_SLVADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~I2CSLV_SLVADDR_SLVADDR_MASK ) |    \
    ( ( ( _val ) << I2CSLV_SLVADDR_SLVADDR_SHIFT ) & \
      I2CSLV_SLVADDR_SLVADDR_MASK ) )
#define I2CSLV_SLVADDR_SLVADDR( _val )             \
  ( ( ( _val ) << I2CSLV_SLVADDR_SLVADDR_SHIFT ) & \
    I2CSLV_SLVADDR_SLVADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBI2CSLVCTRL Control register (CTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CSLV_CTRL_RMOD 0x10U

#define I2CSLV_CTRL_TMOD 0x8U

#define I2CSLV_CTRL_TV 0x4U

#define I2CSLV_CTRL_TAV 0x2U

#define I2CSLV_CTRL_EN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBI2CSLVSTAT Status register (STAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CSLV_STAT_REC 0x4U

#define I2CSLV_STAT_TRA 0x2U

#define I2CSLV_STAT_NAK 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBI2CSLVMASK Mask register (MASK)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CSLV_MASK_RECE 0x4U

#define I2CSLV_MASK_TRAE 0x2U

#define I2CSLV_MASK_NAKE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBI2CSLVRX Receive register (RX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CSLV_RX_RECBYTE_SHIFT 0
#define I2CSLV_RX_RECBYTE_MASK  0xffU
#define I2CSLV_RX_RECBYTE_GET( _reg ) \
  ( ( ( _reg ) & I2CSLV_RX_RECBYTE_MASK ) >> I2CSLV_RX_RECBYTE_SHIFT )
#define I2CSLV_RX_RECBYTE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~I2CSLV_RX_RECBYTE_MASK ) | \
    ( ( ( _val ) << I2CSLV_RX_RECBYTE_SHIFT ) & I2CSLV_RX_RECBYTE_MASK ) )
#define I2CSLV_RX_RECBYTE( _val ) \
  ( ( ( _val ) << I2CSLV_RX_RECBYTE_SHIFT ) & I2CSLV_RX_RECBYTE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBI2CSLVTX Transmit register (TX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define I2CSLV_TX_TRABYTE_SHIFT 0
#define I2CSLV_TX_TRABYTE_MASK  0xffU
#define I2CSLV_TX_TRABYTE_GET( _reg ) \
  ( ( ( _reg ) & I2CSLV_TX_TRABYTE_MASK ) >> I2CSLV_TX_TRABYTE_SHIFT )
#define I2CSLV_TX_TRABYTE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~I2CSLV_TX_TRABYTE_MASK ) | \
    ( ( ( _val ) << I2CSLV_TX_TRABYTE_SHIFT ) & I2CSLV_TX_TRABYTE_MASK ) )
#define I2CSLV_TX_TRABYTE( _val ) \
  ( ( ( _val ) << I2CSLV_TX_TRABYTE_SHIFT ) & I2CSLV_TX_TRABYTE_MASK )

/** @} */

/**
 * @brief This structure defines the I2CSLV register block memory map.
 */
typedef struct i2cslv {
  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CSLVSLVADDR.
   */
  uint32_t slvaddr;

  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CSLVCTRL.
   */
  uint32_t ctrl;

  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CSLVSTAT.
   */
  uint32_t stat;

  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CSLVMASK.
   */
  uint32_t mask;

  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CSLVRX.
   */
  uint32_t rx;

  /**
   * @brief See @ref RTEMSDeviceGRLIBI2CSLVTX.
   */
  uint32_t tx;
} i2cslv;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_I2CSLV_REGS_H */
