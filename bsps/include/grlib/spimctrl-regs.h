/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRLIBSPIMCTRL
 *
 * @brief This header file defines the SPIMCTRL register block interface.
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

/* Find related documentation with spec:/dev/grlib/if/spimctrl-header */

#ifndef _GRLIB_SPIMCTRL_REGS_H
#define _GRLIB_SPIMCTRL_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/spimctrl */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRL SPIMCTRL
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the SPIMCTRL interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLCONF Configuration register (CONF)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPIMCTRL_CONF_WRITECMD_SHIFT 24
#define SPIMCTRL_CONF_WRITECMD_MASK  0xff000000U
#define SPIMCTRL_CONF_WRITECMD_GET( _reg )        \
  ( ( ( _reg ) & SPIMCTRL_CONF_WRITECMD_MASK ) >> \
    SPIMCTRL_CONF_WRITECMD_SHIFT )
#define SPIMCTRL_CONF_WRITECMD_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPIMCTRL_CONF_WRITECMD_MASK ) |    \
    ( ( ( _val ) << SPIMCTRL_CONF_WRITECMD_SHIFT ) & \
      SPIMCTRL_CONF_WRITECMD_MASK ) )
#define SPIMCTRL_CONF_WRITECMD( _val )             \
  ( ( ( _val ) << SPIMCTRL_CONF_WRITECMD_SHIFT ) & \
    SPIMCTRL_CONF_WRITECMD_MASK )

#define SPIMCTRL_CONF_XIP 0x100000U

#define SPIMCTRL_CONF_QIN 0x80000U

#define SPIMCTRL_CONF_DIN 0x40000U

#define SPIMCTRL_CONF_QOUT 0x20000U

#define SPIMCTRL_CONF_DOUT 0x10000U

#define SPIMCTRL_CONF_DBYTE 0x8000U

#define SPIMCTRL_CONF_ADDR 0x4000U

#define SPIMCTRL_CONF_QSPI 0x2000U

#define SPIMCTRL_CONF_DSPI 0x1000U

#define SPIMCTRL_CONF_DCYCLES_SHIFT 8
#define SPIMCTRL_CONF_DCYCLES_MASK  0xf00U
#define SPIMCTRL_CONF_DCYCLES_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_CONF_DCYCLES_MASK ) >> SPIMCTRL_CONF_DCYCLES_SHIFT )
#define SPIMCTRL_CONF_DCYCLES_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPIMCTRL_CONF_DCYCLES_MASK ) |    \
    ( ( ( _val ) << SPIMCTRL_CONF_DCYCLES_SHIFT ) & \
      SPIMCTRL_CONF_DCYCLES_MASK ) )
#define SPIMCTRL_CONF_DCYCLES( _val ) \
  ( ( ( _val ) << SPIMCTRL_CONF_DCYCLES_SHIFT ) & SPIMCTRL_CONF_DCYCLES_MASK )

#define SPIMCTRL_CONF_READCMD_SHIFT 0
#define SPIMCTRL_CONF_READCMD_MASK  0xffU
#define SPIMCTRL_CONF_READCMD_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_CONF_READCMD_MASK ) >> SPIMCTRL_CONF_READCMD_SHIFT )
#define SPIMCTRL_CONF_READCMD_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPIMCTRL_CONF_READCMD_MASK ) |    \
    ( ( ( _val ) << SPIMCTRL_CONF_READCMD_SHIFT ) & \
      SPIMCTRL_CONF_READCMD_MASK ) )
#define SPIMCTRL_CONF_READCMD( _val ) \
  ( ( ( _val ) << SPIMCTRL_CONF_READCMD_SHIFT ) & SPIMCTRL_CONF_READCMD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLCTRL Control register (CTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPIMCTRL_CTRL_DS_SHIFT 5
#define SPIMCTRL_CTRL_DS_MASK  0x60U
#define SPIMCTRL_CTRL_DS_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_CTRL_DS_MASK ) >> SPIMCTRL_CTRL_DS_SHIFT )
#define SPIMCTRL_CTRL_DS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPIMCTRL_CTRL_DS_MASK ) | \
    ( ( ( _val ) << SPIMCTRL_CTRL_DS_SHIFT ) & SPIMCTRL_CTRL_DS_MASK ) )
#define SPIMCTRL_CTRL_DS( _val ) \
  ( ( ( _val ) << SPIMCTRL_CTRL_DS_SHIFT ) & SPIMCTRL_CTRL_DS_MASK )

#define SPIMCTRL_CTRL_RST 0x10U

#define SPIMCTRL_CTRL_CSN 0x8U

#define SPIMCTRL_CTRL_EAS 0x4U

#define SPIMCTRL_CTRL_IEN 0x2U

#define SPIMCTRL_CTRL_USRC 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLSTAT Status register (STAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPIMCTRL_STAT_EDAC 0x800U

#define SPIMCTRL_STAT_DSCAL 0x400U

#define SPIMCTRL_STAT_DOFFS 0x200U

#define SPIMCTRL_STAT_MCSN 0x100U

#define SPIMCTRL_STAT_WRTR 0x80U

#define SPIMCTRL_STAT_RECONF 0x40U

#define SPIMCTRL_STAT_INIT 0x4U

#define SPIMCTRL_STAT_BUSY 0x2U

#define SPIMCTRL_STAT_DONE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLRX Receive register (RX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPIMCTRL_RX_RDATA_SHIFT 0
#define SPIMCTRL_RX_RDATA_MASK  0xffU
#define SPIMCTRL_RX_RDATA_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_RX_RDATA_MASK ) >> SPIMCTRL_RX_RDATA_SHIFT )
#define SPIMCTRL_RX_RDATA_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPIMCTRL_RX_RDATA_MASK ) | \
    ( ( ( _val ) << SPIMCTRL_RX_RDATA_SHIFT ) & SPIMCTRL_RX_RDATA_MASK ) )
#define SPIMCTRL_RX_RDATA( _val ) \
  ( ( ( _val ) << SPIMCTRL_RX_RDATA_SHIFT ) & SPIMCTRL_RX_RDATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLTX Transmit register (TX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPIMCTRL_TX_TDATA_SHIFT 0
#define SPIMCTRL_TX_TDATA_MASK  0xffU
#define SPIMCTRL_TX_TDATA_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_TX_TDATA_MASK ) >> SPIMCTRL_TX_TDATA_SHIFT )
#define SPIMCTRL_TX_TDATA_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPIMCTRL_TX_TDATA_MASK ) | \
    ( ( ( _val ) << SPIMCTRL_TX_TDATA_SHIFT ) & SPIMCTRL_TX_TDATA_MASK ) )
#define SPIMCTRL_TX_TDATA( _val ) \
  ( ( ( _val ) << SPIMCTRL_TX_TDATA_SHIFT ) & SPIMCTRL_TX_TDATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLECTRL EDAC control register (ECTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPIMCTRL_ECTRL_TYPE_SHIFT 28
#define SPIMCTRL_ECTRL_TYPE_MASK  0xf0000000U
#define SPIMCTRL_ECTRL_TYPE_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_ECTRL_TYPE_MASK ) >> SPIMCTRL_ECTRL_TYPE_SHIFT )
#define SPIMCTRL_ECTRL_TYPE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPIMCTRL_ECTRL_TYPE_MASK ) |    \
    ( ( ( _val ) << SPIMCTRL_ECTRL_TYPE_SHIFT ) & \
      SPIMCTRL_ECTRL_TYPE_MASK ) )
#define SPIMCTRL_ECTRL_TYPE( _val ) \
  ( ( ( _val ) << SPIMCTRL_ECTRL_TYPE_SHIFT ) & SPIMCTRL_ECTRL_TYPE_MASK )

#define SPIMCTRL_ECTRL_AERR 0x4U

#define SPIMCTRL_ECTRL_UERR 0x2U

#define SPIMCTRL_ECTRL_EN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLESTAT EDAC status register (ESTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPIMCTRL_ESTAT_UERR 0x2U

#define SPIMCTRL_ESTAT_CERR 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLCTRL2 Control register 2 (CTRL2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPIMCTRL_CTRL2_CSNWR_SHIFT 24
#define SPIMCTRL_CTRL2_CSNWR_MASK  0xff000000U
#define SPIMCTRL_CTRL2_CSNWR_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_CTRL2_CSNWR_MASK ) >> SPIMCTRL_CTRL2_CSNWR_SHIFT )
#define SPIMCTRL_CTRL2_CSNWR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPIMCTRL_CTRL2_CSNWR_MASK ) |    \
    ( ( ( _val ) << SPIMCTRL_CTRL2_CSNWR_SHIFT ) & \
      SPIMCTRL_CTRL2_CSNWR_MASK ) )
#define SPIMCTRL_CTRL2_CSNWR( _val ) \
  ( ( ( _val ) << SPIMCTRL_CTRL2_CSNWR_SHIFT ) & SPIMCTRL_CTRL2_CSNWR_MASK )

#define SPIMCTRL_CTRL2_CSNRD_SHIFT 16
#define SPIMCTRL_CTRL2_CSNRD_MASK  0xff0000U
#define SPIMCTRL_CTRL2_CSNRD_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_CTRL2_CSNRD_MASK ) >> SPIMCTRL_CTRL2_CSNRD_SHIFT )
#define SPIMCTRL_CTRL2_CSNRD_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPIMCTRL_CTRL2_CSNRD_MASK ) |    \
    ( ( ( _val ) << SPIMCTRL_CTRL2_CSNRD_SHIFT ) & \
      SPIMCTRL_CTRL2_CSNRD_MASK ) )
#define SPIMCTRL_CTRL2_CSNRD( _val ) \
  ( ( ( _val ) << SPIMCTRL_CTRL2_CSNRD_SHIFT ) & SPIMCTRL_CTRL2_CSNRD_MASK )

#define SPIMCTRL_CTRL2_WRT 0x10U

#define SPIMCTRL_CTRL2_DSCONF_SHIFT 0
#define SPIMCTRL_CTRL2_DSCONF_MASK  0xfU
#define SPIMCTRL_CTRL2_DSCONF_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_CTRL2_DSCONF_MASK ) >> SPIMCTRL_CTRL2_DSCONF_SHIFT )
#define SPIMCTRL_CTRL2_DSCONF_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPIMCTRL_CTRL2_DSCONF_MASK ) |    \
    ( ( ( _val ) << SPIMCTRL_CTRL2_DSCONF_SHIFT ) & \
      SPIMCTRL_CTRL2_DSCONF_MASK ) )
#define SPIMCTRL_CTRL2_DSCONF( _val ) \
  ( ( ( _val ) << SPIMCTRL_CTRL2_DSCONF_SHIFT ) & SPIMCTRL_CTRL2_DSCONF_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLSOFF Static address offset register (SOFF)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPIMCTRL_SOFF_SOFF_SHIFT 0
#define SPIMCTRL_SOFF_SOFF_MASK  0xffffffffU
#define SPIMCTRL_SOFF_SOFF_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_SOFF_SOFF_MASK ) >> SPIMCTRL_SOFF_SOFF_SHIFT )
#define SPIMCTRL_SOFF_SOFF_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPIMCTRL_SOFF_SOFF_MASK ) | \
    ( ( ( _val ) << SPIMCTRL_SOFF_SOFF_SHIFT ) & SPIMCTRL_SOFF_SOFF_MASK ) )
#define SPIMCTRL_SOFF_SOFF( _val ) \
  ( ( ( _val ) << SPIMCTRL_SOFF_SOFF_SHIFT ) & SPIMCTRL_SOFF_SOFF_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLDOFF \
 *   Dynamic address offset register (DOFF)
 *
 * @brief This group contains register bit definitions.
 *
 * DOFF is writable if the core implements the dynamic address offset, see
 * STAT.DOFFS.
 *
 * @{
 */

#define SPIMCTRL_DOFF_DOFF_SHIFT 0
#define SPIMCTRL_DOFF_DOFF_MASK  0xffffffffU
#define SPIMCTRL_DOFF_DOFF_GET( _reg ) \
  ( ( ( _reg ) & SPIMCTRL_DOFF_DOFF_MASK ) >> SPIMCTRL_DOFF_DOFF_SHIFT )
#define SPIMCTRL_DOFF_DOFF_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPIMCTRL_DOFF_DOFF_MASK ) | \
    ( ( ( _val ) << SPIMCTRL_DOFF_DOFF_SHIFT ) & SPIMCTRL_DOFF_DOFF_MASK ) )
#define SPIMCTRL_DOFF_DOFF( _val ) \
  ( ( ( _val ) << SPIMCTRL_DOFF_DOFF_SHIFT ) & SPIMCTRL_DOFF_DOFF_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPIMCTRLSCALER SPI clock scaler register (SCALER)
 *
 * @brief This group contains register bit definitions.
 *
 * SCALER is writable if the core implements the dynamic scaler, see
 * STAT.DSCAL. The field takes the bits from bit 0 up to the configured scaler
 * width.
 *
 * @{
 */

#define SPIMCTRL_SCALER_SCALER_SHIFT 0
#define SPIMCTRL_SCALER_SCALER_MASK  0xffffffffU
#define SPIMCTRL_SCALER_SCALER_GET( _reg )        \
  ( ( ( _reg ) & SPIMCTRL_SCALER_SCALER_MASK ) >> \
    SPIMCTRL_SCALER_SCALER_SHIFT )
#define SPIMCTRL_SCALER_SCALER_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPIMCTRL_SCALER_SCALER_MASK ) |    \
    ( ( ( _val ) << SPIMCTRL_SCALER_SCALER_SHIFT ) & \
      SPIMCTRL_SCALER_SCALER_MASK ) )
#define SPIMCTRL_SCALER_SCALER( _val )             \
  ( ( ( _val ) << SPIMCTRL_SCALER_SCALER_SHIFT ) & \
    SPIMCTRL_SCALER_SCALER_MASK )

/** @} */

/**
 * @brief This structure defines the SPIMCTRL register block memory map.
 */
typedef struct spimctrl {
  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLCONF.
   */
  uint32_t conf;

  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLCTRL.
   */
  uint32_t ctrl;

  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLSTAT.
   */
  uint32_t stat;

  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLRX.
   */
  uint32_t rx;

  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLTX.
   */
  uint32_t tx;

  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLECTRL.
   */
  uint32_t ectrl;

  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLESTAT.
   */
  uint32_t estat;

  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLCTRL2.
   */
  uint32_t ctrl2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLSOFF.
   */
  uint32_t soff;

  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLDOFF.
   */
  uint32_t doff;

  /**
   * @brief See @ref RTEMSDeviceGRLIBSPIMCTRLSCALER.
   */
  uint32_t scaler;
} spimctrl;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_SPIMCTRL_REGS_H */
