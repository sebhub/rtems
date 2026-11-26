/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRCANFD
 *
 * @brief This header file defines the GRCANFD register block interface.
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

/* Find related documentation with spec:/dev/grlib/if/grcanfd-header */

#ifndef _GRLIB_GRCANFD_REGS_H
#define _GRLIB_GRCANFD_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/grcanfd */

/**
 * @defgroup RTEMSDeviceGRCANFD GRCANFD
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the GRCANFD interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRCANFDCONF Configuration register (CONF)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_CONF_LBS 0x80U

#define GRCAN_CONF_LB 0x40U

#define GRCAN_CONF_SILENT 0x10U

#define GRCAN_CONF_SELECT 0x8U

#define GRCAN_CONF_ENABLE1 0x4U

#define GRCAN_CONF_ENABLE0 0x2U

#define GRCAN_CONF_ABORT 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDSTAT Status register (STAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_STAT_TXERRCNTR_SHIFT 16
#define GRCAN_STAT_TXERRCNTR_MASK  0xff0000U
#define GRCAN_STAT_TXERRCNTR_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_STAT_TXERRCNTR_MASK ) >> GRCAN_STAT_TXERRCNTR_SHIFT )
#define GRCAN_STAT_TXERRCNTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_STAT_TXERRCNTR_MASK ) |    \
    ( ( ( _val ) << GRCAN_STAT_TXERRCNTR_SHIFT ) & \
      GRCAN_STAT_TXERRCNTR_MASK ) )
#define GRCAN_STAT_TXERRCNTR( _val ) \
  ( ( ( _val ) << GRCAN_STAT_TXERRCNTR_SHIFT ) & GRCAN_STAT_TXERRCNTR_MASK )

#define GRCAN_STAT_RXERRCNTR_SHIFT 8
#define GRCAN_STAT_RXERRCNTR_MASK  0xff00U
#define GRCAN_STAT_RXERRCNTR_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_STAT_RXERRCNTR_MASK ) >> GRCAN_STAT_RXERRCNTR_SHIFT )
#define GRCAN_STAT_RXERRCNTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_STAT_RXERRCNTR_MASK ) |    \
    ( ( ( _val ) << GRCAN_STAT_RXERRCNTR_SHIFT ) & \
      GRCAN_STAT_RXERRCNTR_MASK ) )
#define GRCAN_STAT_RXERRCNTR( _val ) \
  ( ( ( _val ) << GRCAN_STAT_RXERRCNTR_SHIFT ) & GRCAN_STAT_RXERRCNTR_MASK )

#define GRCAN_STAT_MD 0x20U

#define GRCAN_STAT_ACTIVE 0x10U

#define GRCAN_STAT_BMERR 0x8U

#define GRCAN_STAT_OR 0x4U

#define GRCAN_STAT_BUSOFF 0x2U

#define GRCAN_STAT_ERRPASS 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDCTRL Control register (CTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_CTRL_DISREST 0x4U

#define GRCAN_CTRL_RESET 0x2U

#define GRCAN_CTRL_ENABLE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDCAP Capability register (CAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_CAP_REV1 0x80000000U

#define GRCAN_CAP_REV2 0x40000000U

#define GRCAN_CAP_TXBUFSIZE_SHIFT 8
#define GRCAN_CAP_TXBUFSIZE_MASK  0x700U
#define GRCAN_CAP_TXBUFSIZE_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_CAP_TXBUFSIZE_MASK ) >> GRCAN_CAP_TXBUFSIZE_SHIFT )
#define GRCAN_CAP_TXBUFSIZE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_CAP_TXBUFSIZE_MASK ) |    \
    ( ( ( _val ) << GRCAN_CAP_TXBUFSIZE_SHIFT ) & \
      GRCAN_CAP_TXBUFSIZE_MASK ) )
#define GRCAN_CAP_TXBUFSIZE( _val ) \
  ( ( ( _val ) << GRCAN_CAP_TXBUFSIZE_SHIFT ) & GRCAN_CAP_TXBUFSIZE_MASK )

#define GRCAN_CAP_RXBUFSIZE_SHIFT 4
#define GRCAN_CAP_RXBUFSIZE_MASK  0x70U
#define GRCAN_CAP_RXBUFSIZE_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_CAP_RXBUFSIZE_MASK ) >> GRCAN_CAP_RXBUFSIZE_SHIFT )
#define GRCAN_CAP_RXBUFSIZE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_CAP_RXBUFSIZE_MASK ) |    \
    ( ( ( _val ) << GRCAN_CAP_RXBUFSIZE_SHIFT ) & \
      GRCAN_CAP_RXBUFSIZE_MASK ) )
#define GRCAN_CAP_RXBUFSIZE( _val ) \
  ( ( ( _val ) << GRCAN_CAP_RXBUFSIZE_SHIFT ) & GRCAN_CAP_RXBUFSIZE_MASK )

#define GRCAN_CAP_SEPBUS 0x4U

#define GRCAN_CAP_COPEN 0x2U

#define GRCAN_CAP_SINGIRQ 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDSYNCMASK SYNC Mask Filter Register (SYNCMASK)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_SYNCMASK_MASK_SHIFT 0
#define GRCAN_SYNCMASK_MASK_MASK  0x1fffffffU
#define GRCAN_SYNCMASK_MASK_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_SYNCMASK_MASK_MASK ) >> GRCAN_SYNCMASK_MASK_SHIFT )
#define GRCAN_SYNCMASK_MASK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_SYNCMASK_MASK_MASK ) |    \
    ( ( ( _val ) << GRCAN_SYNCMASK_MASK_SHIFT ) & \
      GRCAN_SYNCMASK_MASK_MASK ) )
#define GRCAN_SYNCMASK_MASK( _val ) \
  ( ( ( _val ) << GRCAN_SYNCMASK_MASK_SHIFT ) & GRCAN_SYNCMASK_MASK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDSYNCCODE SYNC Code Filter Register (SYNCCODE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_SYNCCODE_CODE_SHIFT 0
#define GRCAN_SYNCCODE_CODE_MASK  0x1fffffffU
#define GRCAN_SYNCCODE_CODE_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_SYNCCODE_CODE_MASK ) >> GRCAN_SYNCCODE_CODE_SHIFT )
#define GRCAN_SYNCCODE_CODE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_SYNCCODE_CODE_MASK ) |    \
    ( ( ( _val ) << GRCAN_SYNCCODE_CODE_SHIFT ) & \
      GRCAN_SYNCCODE_CODE_MASK ) )
#define GRCAN_SYNCCODE_CODE( _val ) \
  ( ( ( _val ) << GRCAN_SYNCCODE_CODE_SHIFT ) & GRCAN_SYNCCODE_CODE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDNOMBR \
 *   Nominal Bit-Rate Configuration Register (NOMBR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_NOMBR_SCALER_SHIFT 16
#define GRCAN_NOMBR_SCALER_MASK  0xff0000U
#define GRCAN_NOMBR_SCALER_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_NOMBR_SCALER_MASK ) >> GRCAN_NOMBR_SCALER_SHIFT )
#define GRCAN_NOMBR_SCALER_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_NOMBR_SCALER_MASK ) | \
    ( ( ( _val ) << GRCAN_NOMBR_SCALER_SHIFT ) & GRCAN_NOMBR_SCALER_MASK ) )
#define GRCAN_NOMBR_SCALER( _val ) \
  ( ( ( _val ) << GRCAN_NOMBR_SCALER_SHIFT ) & GRCAN_NOMBR_SCALER_MASK )

#define GRCAN_NOMBR_PS1_SHIFT 10
#define GRCAN_NOMBR_PS1_MASK  0xfc00U
#define GRCAN_NOMBR_PS1_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_NOMBR_PS1_MASK ) >> GRCAN_NOMBR_PS1_SHIFT )
#define GRCAN_NOMBR_PS1_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_NOMBR_PS1_MASK ) | \
    ( ( ( _val ) << GRCAN_NOMBR_PS1_SHIFT ) & GRCAN_NOMBR_PS1_MASK ) )
#define GRCAN_NOMBR_PS1( _val ) \
  ( ( ( _val ) << GRCAN_NOMBR_PS1_SHIFT ) & GRCAN_NOMBR_PS1_MASK )

#define GRCAN_NOMBR_PS2_SHIFT 5
#define GRCAN_NOMBR_PS2_MASK  0x3e0U
#define GRCAN_NOMBR_PS2_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_NOMBR_PS2_MASK ) >> GRCAN_NOMBR_PS2_SHIFT )
#define GRCAN_NOMBR_PS2_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_NOMBR_PS2_MASK ) | \
    ( ( ( _val ) << GRCAN_NOMBR_PS2_SHIFT ) & GRCAN_NOMBR_PS2_MASK ) )
#define GRCAN_NOMBR_PS2( _val ) \
  ( ( ( _val ) << GRCAN_NOMBR_PS2_SHIFT ) & GRCAN_NOMBR_PS2_MASK )

#define GRCAN_NOMBR_SJW_SHIFT 0
#define GRCAN_NOMBR_SJW_MASK  0x1fU
#define GRCAN_NOMBR_SJW_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_NOMBR_SJW_MASK ) >> GRCAN_NOMBR_SJW_SHIFT )
#define GRCAN_NOMBR_SJW_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_NOMBR_SJW_MASK ) | \
    ( ( ( _val ) << GRCAN_NOMBR_SJW_SHIFT ) & GRCAN_NOMBR_SJW_MASK ) )
#define GRCAN_NOMBR_SJW( _val ) \
  ( ( ( _val ) << GRCAN_NOMBR_SJW_SHIFT ) & GRCAN_NOMBR_SJW_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDDATABR \
 *   Data Bit-Rate Configuration Register (DATABR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_DATABR_SCALER_SHIFT 16
#define GRCAN_DATABR_SCALER_MASK  0xff0000U
#define GRCAN_DATABR_SCALER_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_DATABR_SCALER_MASK ) >> GRCAN_DATABR_SCALER_SHIFT )
#define GRCAN_DATABR_SCALER_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_DATABR_SCALER_MASK ) |    \
    ( ( ( _val ) << GRCAN_DATABR_SCALER_SHIFT ) & \
      GRCAN_DATABR_SCALER_MASK ) )
#define GRCAN_DATABR_SCALER( _val ) \
  ( ( ( _val ) << GRCAN_DATABR_SCALER_SHIFT ) & GRCAN_DATABR_SCALER_MASK )

#define GRCAN_DATABR_PS1_SHIFT 10
#define GRCAN_DATABR_PS1_MASK  0x3c00U
#define GRCAN_DATABR_PS1_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_DATABR_PS1_MASK ) >> GRCAN_DATABR_PS1_SHIFT )
#define GRCAN_DATABR_PS1_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_DATABR_PS1_MASK ) | \
    ( ( ( _val ) << GRCAN_DATABR_PS1_SHIFT ) & GRCAN_DATABR_PS1_MASK ) )
#define GRCAN_DATABR_PS1( _val ) \
  ( ( ( _val ) << GRCAN_DATABR_PS1_SHIFT ) & GRCAN_DATABR_PS1_MASK )

#define GRCAN_DATABR_PS2_SHIFT 5
#define GRCAN_DATABR_PS2_MASK  0x1e0U
#define GRCAN_DATABR_PS2_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_DATABR_PS2_MASK ) >> GRCAN_DATABR_PS2_SHIFT )
#define GRCAN_DATABR_PS2_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_DATABR_PS2_MASK ) | \
    ( ( ( _val ) << GRCAN_DATABR_PS2_SHIFT ) & GRCAN_DATABR_PS2_MASK ) )
#define GRCAN_DATABR_PS2( _val ) \
  ( ( ( _val ) << GRCAN_DATABR_PS2_SHIFT ) & GRCAN_DATABR_PS2_MASK )

#define GRCAN_DATABR_SJW_SHIFT 0
#define GRCAN_DATABR_SJW_MASK  0xfU
#define GRCAN_DATABR_SJW_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_DATABR_SJW_MASK ) >> GRCAN_DATABR_SJW_SHIFT )
#define GRCAN_DATABR_SJW_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_DATABR_SJW_MASK ) | \
    ( ( ( _val ) << GRCAN_DATABR_SJW_SHIFT ) & GRCAN_DATABR_SJW_MASK ) )
#define GRCAN_DATABR_SJW( _val ) \
  ( ( ( _val ) << GRCAN_DATABR_SJW_SHIFT ) & GRCAN_DATABR_SJW_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDDELCOMP \
 *   Transmitter Delay Compensation Register (DELCOMP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_DELCOMP_TXCOMPVAL_SHIFT 0
#define GRCAN_DELCOMP_TXCOMPVAL_MASK  0x3fU
#define GRCAN_DELCOMP_TXCOMPVAL_GET( _reg )        \
  ( ( ( _reg ) & GRCAN_DELCOMP_TXCOMPVAL_MASK ) >> \
    GRCAN_DELCOMP_TXCOMPVAL_SHIFT )
#define GRCAN_DELCOMP_TXCOMPVAL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_DELCOMP_TXCOMPVAL_MASK ) |    \
    ( ( ( _val ) << GRCAN_DELCOMP_TXCOMPVAL_SHIFT ) & \
      GRCAN_DELCOMP_TXCOMPVAL_MASK ) )
#define GRCAN_DELCOMP_TXCOMPVAL( _val )             \
  ( ( ( _val ) << GRCAN_DELCOMP_TXCOMPVAL_SHIFT ) & \
    GRCAN_DELCOMP_TXCOMPVAL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDCOCTRL CANOpen Control Register (COCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_COCTRL_ID_SHIFT 4
#define GRCAN_COCTRL_ID_MASK  0x7f0U
#define GRCAN_COCTRL_ID_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_COCTRL_ID_MASK ) >> GRCAN_COCTRL_ID_SHIFT )
#define GRCAN_COCTRL_ID_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_COCTRL_ID_MASK ) | \
    ( ( ( _val ) << GRCAN_COCTRL_ID_SHIFT ) & GRCAN_COCTRL_ID_MASK ) )
#define GRCAN_COCTRL_ID( _val ) \
  ( ( ( _val ) << GRCAN_COCTRL_ID_SHIFT ) & GRCAN_COCTRL_ID_MASK )

#define GRCAN_COCTRL_RC 0x4U

#define GRCAN_COCTRL_SS 0x2U

#define GRCAN_COCTRL_EN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDCOHBTO \
 *   CANOpen Heartbeat Timeout Register (COHBTO)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_COHBTO_TO_SHIFT 0
#define GRCAN_COHBTO_TO_MASK  0xffffffffU
#define GRCAN_COHBTO_TO_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_COHBTO_TO_MASK ) >> GRCAN_COHBTO_TO_SHIFT )
#define GRCAN_COHBTO_TO_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_COHBTO_TO_MASK ) | \
    ( ( ( _val ) << GRCAN_COHBTO_TO_SHIFT ) & GRCAN_COHBTO_TO_MASK ) )
#define GRCAN_COHBTO_TO( _val ) \
  ( ( ( _val ) << GRCAN_COHBTO_TO_SHIFT ) & GRCAN_COHBTO_TO_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDCOHBCT CANOpen Heartbeat Count Register (COHBCT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_COHBCT_CT_SHIFT 0
#define GRCAN_COHBCT_CT_MASK  0xffffffffU
#define GRCAN_COHBCT_CT_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_COHBCT_CT_MASK ) >> GRCAN_COHBCT_CT_SHIFT )
#define GRCAN_COHBCT_CT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_COHBCT_CT_MASK ) | \
    ( ( ( _val ) << GRCAN_COHBCT_CT_SHIFT ) & GRCAN_COHBCT_CT_MASK ) )
#define GRCAN_COHBCT_CT( _val ) \
  ( ( ( _val ) << GRCAN_COHBCT_CT_SHIFT ) & GRCAN_COHBCT_CT_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDCOSTS CANOpen Status Register (COSTS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_COSTS_STS_SHIFT 0
#define GRCAN_COSTS_STS_MASK  0xfU
#define GRCAN_COSTS_STS_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_COSTS_STS_MASK ) >> GRCAN_COSTS_STS_SHIFT )
#define GRCAN_COSTS_STS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_COSTS_STS_MASK ) | \
    ( ( ( _val ) << GRCAN_COSTS_STS_SHIFT ) & GRCAN_COSTS_STS_MASK ) )
#define GRCAN_COSTS_STS( _val ) \
  ( ( ( _val ) << GRCAN_COSTS_STS_SHIFT ) & GRCAN_COSTS_STS_MASK )
#define GRCAN_COSTS_STS_IDLE         0U
#define GRCAN_COSTS_STS_RX_DATABYTES 1U
#define GRCAN_COSTS_STS_DECODE       2U
#define GRCAN_COSTS_STS_INIT_WRITE   3U
#define GRCAN_COSTS_STS_END_WRITE    4U
#define GRCAN_COSTS_STS_INIT_READ    5U
#define GRCAN_COSTS_STS_END_READ     6U
#define GRCAN_COSTS_STS_INIT_REPLY   7U
#define GRCAN_COSTS_STS_TX_DATABYTES 8U
#define GRCAN_COSTS_STS_REPLY_NOK    9U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDPIMSR \
 *   Pending interrupt masked status register (PIMSR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_PIMSR_COHBERR 0x100000U

#define GRCAN_PIMSR_COSYNC 0x80000U

#define GRCAN_PIMSR_COWRCMD 0x40000U

#define GRCAN_PIMSR_CORDCMD 0x20000U

#define GRCAN_PIMSR_TXLOSS 0x10000U

#define GRCAN_PIMSR_RXMISS 0x8000U

#define GRCAN_PIMSR_TXERRCNTR 0x4000U

#define GRCAN_PIMSR_RXERRCNTR 0x2000U

#define GRCAN_PIMSR_TXSYNC 0x1000U

#define GRCAN_PIMSR_RXSYNC 0x800U

#define GRCAN_PIMSR_TX 0x400U

#define GRCAN_PIMSR_RX 0x200U

#define GRCAN_PIMSR_TXEMPTY 0x100U

#define GRCAN_PIMSR_RXFULL 0x80U

#define GRCAN_PIMSR_TXIRQ 0x40U

#define GRCAN_PIMSR_RXIRQ 0x20U

#define GRCAN_PIMSR_BMRDERR 0x10U

#define GRCAN_PIMSR_BMWRERR 0x8U

#define GRCAN_PIMSR_OR 0x4U

#define GRCAN_PIMSR_OFF 0x2U

#define GRCAN_PIMSR_PASS 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDPIMR Pending interrupt masked register (PIMR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_PIMR_COHBERR 0x100000U

#define GRCAN_PIMR_COSYNC 0x80000U

#define GRCAN_PIMR_COWRCMD 0x40000U

#define GRCAN_PIMR_CORDCMD 0x20000U

#define GRCAN_PIMR_TXLOSS 0x10000U

#define GRCAN_PIMR_RXMISS 0x8000U

#define GRCAN_PIMR_TXERRCNTR 0x4000U

#define GRCAN_PIMR_RXERRCNTR 0x2000U

#define GRCAN_PIMR_TXSYNC 0x1000U

#define GRCAN_PIMR_RXSYNC 0x800U

#define GRCAN_PIMR_TX 0x400U

#define GRCAN_PIMR_RX 0x200U

#define GRCAN_PIMR_TXEMPTY 0x100U

#define GRCAN_PIMR_RXFULL 0x80U

#define GRCAN_PIMR_TXIRQ 0x40U

#define GRCAN_PIMR_RXIRQ 0x20U

#define GRCAN_PIMR_BMRDERR 0x10U

#define GRCAN_PIMR_BMWRERR 0x8U

#define GRCAN_PIMR_OR 0x4U

#define GRCAN_PIMR_OFF 0x2U

#define GRCAN_PIMR_PASS 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDPISR Pending interrupt status register (PISR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_PISR_COHBERR 0x100000U

#define GRCAN_PISR_COSYNC 0x80000U

#define GRCAN_PISR_COWRCMD 0x40000U

#define GRCAN_PISR_CORDCMD 0x20000U

#define GRCAN_PISR_TXLOSS 0x10000U

#define GRCAN_PISR_RXMISS 0x8000U

#define GRCAN_PISR_TXERRCNTR 0x4000U

#define GRCAN_PISR_RXERRCNTR 0x2000U

#define GRCAN_PISR_TXSYNC 0x1000U

#define GRCAN_PISR_RXSYNC 0x800U

#define GRCAN_PISR_TX 0x400U

#define GRCAN_PISR_RX 0x200U

#define GRCAN_PISR_TXEMPTY 0x100U

#define GRCAN_PISR_RXFULL 0x80U

#define GRCAN_PISR_TXIRQ 0x40U

#define GRCAN_PISR_RXIRQ 0x20U

#define GRCAN_PISR_BMRDERR 0x10U

#define GRCAN_PISR_BMWRERR 0x8U

#define GRCAN_PISR_OR 0x4U

#define GRCAN_PISR_OFF 0x2U

#define GRCAN_PISR_PASS 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDPIR Pending interrupt register (PIR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_PIR_COHBERR 0x100000U

#define GRCAN_PIR_COSYNC 0x80000U

#define GRCAN_PIR_COWRCMD 0x40000U

#define GRCAN_PIR_CORDCMD 0x20000U

#define GRCAN_PIR_TXLOSS 0x10000U

#define GRCAN_PIR_RXMISS 0x8000U

#define GRCAN_PIR_TXERRCNTR 0x4000U

#define GRCAN_PIR_RXERRCNTR 0x2000U

#define GRCAN_PIR_TXSYNC 0x1000U

#define GRCAN_PIR_RXSYNC 0x800U

#define GRCAN_PIR_TX 0x400U

#define GRCAN_PIR_RX 0x200U

#define GRCAN_PIR_TXEMPTY 0x100U

#define GRCAN_PIR_RXFULL 0x80U

#define GRCAN_PIR_TXIRQ 0x40U

#define GRCAN_PIR_RXIRQ 0x20U

#define GRCAN_PIR_BMRDERR 0x10U

#define GRCAN_PIR_BMWRERR 0x8U

#define GRCAN_PIR_OR 0x4U

#define GRCAN_PIR_OFF 0x2U

#define GRCAN_PIR_PASS 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDIMR Interrupt mask register (IMR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_IMR_COHBERR 0x100000U

#define GRCAN_IMR_COSYNC 0x80000U

#define GRCAN_IMR_COWRCMD 0x40000U

#define GRCAN_IMR_CORDCMD 0x20000U

#define GRCAN_IMR_TXLOSS 0x10000U

#define GRCAN_IMR_RXMISS 0x8000U

#define GRCAN_IMR_TXERRCNTR 0x4000U

#define GRCAN_IMR_RXERRCNTR 0x2000U

#define GRCAN_IMR_TXSYNC 0x1000U

#define GRCAN_IMR_RXSYNC 0x800U

#define GRCAN_IMR_TX 0x400U

#define GRCAN_IMR_RX 0x200U

#define GRCAN_IMR_TXEMPTY 0x100U

#define GRCAN_IMR_RXFULL 0x80U

#define GRCAN_IMR_TXIRQ 0x40U

#define GRCAN_IMR_RXIRQ 0x20U

#define GRCAN_IMR_BMRDERR 0x10U

#define GRCAN_IMR_BMWRERR 0x8U

#define GRCAN_IMR_OR 0x4U

#define GRCAN_IMR_OFF 0x2U

#define GRCAN_IMR_PASS 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDPICR Pending interrupt clear register (PICR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_PICR_COHBERR 0x100000U

#define GRCAN_PICR_COSYNC 0x80000U

#define GRCAN_PICR_COWRCMD 0x40000U

#define GRCAN_PICR_CORDCMD 0x20000U

#define GRCAN_PICR_TXLOSS 0x10000U

#define GRCAN_PICR_RXMISS 0x8000U

#define GRCAN_PICR_TXERRCNTR 0x4000U

#define GRCAN_PICR_RXERRCNTR 0x2000U

#define GRCAN_PICR_TXSYNC 0x1000U

#define GRCAN_PICR_RXSYNC 0x800U

#define GRCAN_PICR_TX 0x400U

#define GRCAN_PICR_RX 0x200U

#define GRCAN_PICR_TXEMPTY 0x100U

#define GRCAN_PICR_RXFULL 0x80U

#define GRCAN_PICR_TXIRQ 0x40U

#define GRCAN_PICR_RXIRQ 0x20U

#define GRCAN_PICR_BMRDERR 0x10U

#define GRCAN_PICR_BMWRERR 0x8U

#define GRCAN_PICR_OR 0x4U

#define GRCAN_PICR_OFF 0x2U

#define GRCAN_PICR_PASS 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDTXCTRL \
 *   Transmit channel control register (TXCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_TXCTRL_DISACK 0x8U

#define GRCAN_TXCTRL_SINGLE 0x4U

#define GRCAN_TXCTRL_ONGOING 0x2U

#define GRCAN_TXCTRL_ENABLE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDTXADDR \
 *   Transmit Channel Address Register (TXADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_TXADDR_ADDR_SHIFT 10
#define GRCAN_TXADDR_ADDR_MASK  0xfffffc00U
#define GRCAN_TXADDR_ADDR_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_TXADDR_ADDR_MASK ) >> GRCAN_TXADDR_ADDR_SHIFT )
#define GRCAN_TXADDR_ADDR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_TXADDR_ADDR_MASK ) | \
    ( ( ( _val ) << GRCAN_TXADDR_ADDR_SHIFT ) & GRCAN_TXADDR_ADDR_MASK ) )
#define GRCAN_TXADDR_ADDR( _val ) \
  ( ( ( _val ) << GRCAN_TXADDR_ADDR_SHIFT ) & GRCAN_TXADDR_ADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDTXSIZE Transmit Channel Size Register (TXSIZE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_TXSIZE_SIZE_SHIFT 6
#define GRCAN_TXSIZE_SIZE_MASK  0x1fffc0U
#define GRCAN_TXSIZE_SIZE_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_TXSIZE_SIZE_MASK ) >> GRCAN_TXSIZE_SIZE_SHIFT )
#define GRCAN_TXSIZE_SIZE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_TXSIZE_SIZE_MASK ) | \
    ( ( ( _val ) << GRCAN_TXSIZE_SIZE_SHIFT ) & GRCAN_TXSIZE_SIZE_MASK ) )
#define GRCAN_TXSIZE_SIZE( _val ) \
  ( ( ( _val ) << GRCAN_TXSIZE_SIZE_SHIFT ) & GRCAN_TXSIZE_SIZE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDTXWR Transmit Channel Write Register (TXWR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_TXWR_WRITE_SHIFT 4
#define GRCAN_TXWR_WRITE_MASK  0xffff0U
#define GRCAN_TXWR_WRITE_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_TXWR_WRITE_MASK ) >> GRCAN_TXWR_WRITE_SHIFT )
#define GRCAN_TXWR_WRITE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_TXWR_WRITE_MASK ) | \
    ( ( ( _val ) << GRCAN_TXWR_WRITE_SHIFT ) & GRCAN_TXWR_WRITE_MASK ) )
#define GRCAN_TXWR_WRITE( _val ) \
  ( ( ( _val ) << GRCAN_TXWR_WRITE_SHIFT ) & GRCAN_TXWR_WRITE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDTXRD Transmit Channel Read Register (TXRD)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_TXRD_READ_SHIFT 4
#define GRCAN_TXRD_READ_MASK  0xffff0U
#define GRCAN_TXRD_READ_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_TXRD_READ_MASK ) >> GRCAN_TXRD_READ_SHIFT )
#define GRCAN_TXRD_READ_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_TXRD_READ_MASK ) | \
    ( ( ( _val ) << GRCAN_TXRD_READ_SHIFT ) & GRCAN_TXRD_READ_MASK ) )
#define GRCAN_TXRD_READ( _val ) \
  ( ( ( _val ) << GRCAN_TXRD_READ_SHIFT ) & GRCAN_TXRD_READ_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDTXIRQ \
 *   Transmit Channel Interrupt Register (TXIRQ)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_TXIRQ_IRQ_SHIFT 4
#define GRCAN_TXIRQ_IRQ_MASK  0xffff0U
#define GRCAN_TXIRQ_IRQ_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_TXIRQ_IRQ_MASK ) >> GRCAN_TXIRQ_IRQ_SHIFT )
#define GRCAN_TXIRQ_IRQ_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_TXIRQ_IRQ_MASK ) | \
    ( ( ( _val ) << GRCAN_TXIRQ_IRQ_SHIFT ) & GRCAN_TXIRQ_IRQ_MASK ) )
#define GRCAN_TXIRQ_IRQ( _val ) \
  ( ( ( _val ) << GRCAN_TXIRQ_IRQ_SHIFT ) & GRCAN_TXIRQ_IRQ_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDRXCTRL Receive channel control register (RXCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_RXCTRL_OF 0x8U

#define GRCAN_RXCTRL_DISACK 0x4U

#define GRCAN_RXCTRL_ONGOING 0x2U

#define GRCAN_RXCTRL_ENABLE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDRXADDR Receive Channel Address Register (RXADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_RXADDR_ADDR_SHIFT 10
#define GRCAN_RXADDR_ADDR_MASK  0xfffffc00U
#define GRCAN_RXADDR_ADDR_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_RXADDR_ADDR_MASK ) >> GRCAN_RXADDR_ADDR_SHIFT )
#define GRCAN_RXADDR_ADDR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_RXADDR_ADDR_MASK ) | \
    ( ( ( _val ) << GRCAN_RXADDR_ADDR_SHIFT ) & GRCAN_RXADDR_ADDR_MASK ) )
#define GRCAN_RXADDR_ADDR( _val ) \
  ( ( ( _val ) << GRCAN_RXADDR_ADDR_SHIFT ) & GRCAN_RXADDR_ADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDRXSIZE Receive Channel Size Register (RXSIZE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_RXSIZE_SIZE_SHIFT 6
#define GRCAN_RXSIZE_SIZE_MASK  0x1fffc0U
#define GRCAN_RXSIZE_SIZE_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_RXSIZE_SIZE_MASK ) >> GRCAN_RXSIZE_SIZE_SHIFT )
#define GRCAN_RXSIZE_SIZE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_RXSIZE_SIZE_MASK ) | \
    ( ( ( _val ) << GRCAN_RXSIZE_SIZE_SHIFT ) & GRCAN_RXSIZE_SIZE_MASK ) )
#define GRCAN_RXSIZE_SIZE( _val ) \
  ( ( ( _val ) << GRCAN_RXSIZE_SIZE_SHIFT ) & GRCAN_RXSIZE_SIZE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDRXWR Receive Channel Write Register (RXWR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_RXWR_WRITE_SHIFT 4
#define GRCAN_RXWR_WRITE_MASK  0xffff0U
#define GRCAN_RXWR_WRITE_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_RXWR_WRITE_MASK ) >> GRCAN_RXWR_WRITE_SHIFT )
#define GRCAN_RXWR_WRITE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_RXWR_WRITE_MASK ) | \
    ( ( ( _val ) << GRCAN_RXWR_WRITE_SHIFT ) & GRCAN_RXWR_WRITE_MASK ) )
#define GRCAN_RXWR_WRITE( _val ) \
  ( ( ( _val ) << GRCAN_RXWR_WRITE_SHIFT ) & GRCAN_RXWR_WRITE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDRXRD Receive Channel Read Register (RXRD)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_RXRD_READ_SHIFT 4
#define GRCAN_RXRD_READ_MASK  0xffff0U
#define GRCAN_RXRD_READ_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_RXRD_READ_MASK ) >> GRCAN_RXRD_READ_SHIFT )
#define GRCAN_RXRD_READ_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_RXRD_READ_MASK ) | \
    ( ( ( _val ) << GRCAN_RXRD_READ_SHIFT ) & GRCAN_RXRD_READ_MASK ) )
#define GRCAN_RXRD_READ( _val ) \
  ( ( ( _val ) << GRCAN_RXRD_READ_SHIFT ) & GRCAN_RXRD_READ_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDRXIRQ Receive Channel Interrupt Register (RXIRQ)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_RXIRQ_IRQ_SHIFT 4
#define GRCAN_RXIRQ_IRQ_MASK  0xffff0U
#define GRCAN_RXIRQ_IRQ_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_RXIRQ_IRQ_MASK ) >> GRCAN_RXIRQ_IRQ_SHIFT )
#define GRCAN_RXIRQ_IRQ_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_RXIRQ_IRQ_MASK ) | \
    ( ( ( _val ) << GRCAN_RXIRQ_IRQ_SHIFT ) & GRCAN_RXIRQ_IRQ_MASK ) )
#define GRCAN_RXIRQ_IRQ( _val ) \
  ( ( ( _val ) << GRCAN_RXIRQ_IRQ_SHIFT ) & GRCAN_RXIRQ_IRQ_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDRXMASK \
 *   Receive Channel Acceptance Mask Register (RXMASK)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_RXMASK_AM_SHIFT 0
#define GRCAN_RXMASK_AM_MASK  0x1fffffffU
#define GRCAN_RXMASK_AM_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_RXMASK_AM_MASK ) >> GRCAN_RXMASK_AM_SHIFT )
#define GRCAN_RXMASK_AM_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_RXMASK_AM_MASK ) | \
    ( ( ( _val ) << GRCAN_RXMASK_AM_SHIFT ) & GRCAN_RXMASK_AM_MASK ) )
#define GRCAN_RXMASK_AM( _val ) \
  ( ( ( _val ) << GRCAN_RXMASK_AM_SHIFT ) & GRCAN_RXMASK_AM_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDRXCODE \
 *   Receive Channel Acceptance Code Register (RXCODE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_RXCODE_AC_SHIFT 0
#define GRCAN_RXCODE_AC_MASK  0x1fffffffU
#define GRCAN_RXCODE_AC_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_RXCODE_AC_MASK ) >> GRCAN_RXCODE_AC_SHIFT )
#define GRCAN_RXCODE_AC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRCAN_RXCODE_AC_MASK ) | \
    ( ( ( _val ) << GRCAN_RXCODE_AC_SHIFT ) & GRCAN_RXCODE_AC_MASK ) )
#define GRCAN_RXCODE_AC( _val ) \
  ( ( ( _val ) << GRCAN_RXCODE_AC_SHIFT ) & GRCAN_RXCODE_AC_MASK )

/** @} */

/**
 * @brief This structure defines the GRCANFD register block memory map.
 */
typedef struct grcanfd {
  /**
   * @brief See @ref RTEMSDeviceGRCANFDCONF.
   */
  uint32_t conf;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDSTAT.
   */
  uint32_t stat;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDCTRL.
   */
  uint32_t ctrl;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDCAP.
   */
  uint32_t cap;

  uint32_t reserved_10_18[ 2 ];

  /**
   * @brief See @ref RTEMSDeviceGRCANFDSYNCMASK.
   */
  uint32_t syncmask;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDSYNCCODE.
   */
  uint32_t synccode;

  uint32_t reserved_20_40[ 8 ];

  /**
   * @brief See @ref RTEMSDeviceGRCANFDNOMBR.
   */
  uint32_t nombr;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDDATABR.
   */
  uint32_t databr;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDDELCOMP.
   */
  uint32_t delcomp;

  uint32_t reserved_4c_80[ 13 ];

  /**
   * @brief See @ref RTEMSDeviceGRCANFDCOCTRL.
   */
  uint32_t coctrl;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDCOHBTO.
   */
  uint32_t cohbto;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDCOHBCT.
   */
  uint32_t cohbct;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDCOSTS.
   */
  uint32_t costs;

  uint32_t reserved_90_100[ 28 ];

  /**
   * @brief See @ref RTEMSDeviceGRCANFDPIMSR.
   */
  uint32_t pimsr;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDPIMR.
   */
  uint32_t pimr;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDPISR.
   */
  uint32_t pisr;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDPIR.
   */
  uint32_t pir;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDIMR.
   */
  uint32_t imr;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDPICR.
   */
  uint32_t picr;

  uint32_t reserved_118_200[ 58 ];

  /**
   * @brief See @ref RTEMSDeviceGRCANFDTXCTRL.
   */
  uint32_t txctrl;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDTXADDR.
   */
  uint32_t txaddr;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDTXSIZE.
   */
  uint32_t txsize;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDTXWR.
   */
  uint32_t txwr;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDTXRD.
   */
  uint32_t txrd;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDTXIRQ.
   */
  uint32_t txirq;

  uint32_t reserved_218_300[ 58 ];

  /**
   * @brief See @ref RTEMSDeviceGRCANFDRXCTRL.
   */
  uint32_t rxctrl;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDRXADDR.
   */
  uint32_t rxaddr;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDRXSIZE.
   */
  uint32_t rxsize;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDRXWR.
   */
  uint32_t rxwr;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDRXRD.
   */
  uint32_t rxrd;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDRXIRQ.
   */
  uint32_t rxirq;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDRXMASK.
   */
  uint32_t rxmask;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDRXCODE.
   */
  uint32_t rxcode;
} grcanfd;

/** @} */

/* Find related documentation with spec:/dev/grlib/if/grcanfd-message */

/**
 * @defgroup RTEMSDeviceGRCANFDMessage GRCANFD Message
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the GRCANFD Message interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRCANFDMessageFLAGS0 Message Identifier Word (FLAGS_0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_MSG_FLAGS_0_ID_SHIFT 0
#define GRCAN_MSG_FLAGS_0_ID_MASK  0x1fffffffU
#define GRCAN_MSG_FLAGS_0_ID_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_MSG_FLAGS_0_ID_MASK ) >> GRCAN_MSG_FLAGS_0_ID_SHIFT )
#define GRCAN_MSG_FLAGS_0_ID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_MSG_FLAGS_0_ID_MASK ) |    \
    ( ( ( _val ) << GRCAN_MSG_FLAGS_0_ID_SHIFT ) & \
      GRCAN_MSG_FLAGS_0_ID_MASK ) )
#define GRCAN_MSG_FLAGS_0_ID( _val ) \
  ( ( ( _val ) << GRCAN_MSG_FLAGS_0_ID_SHIFT ) & GRCAN_MSG_FLAGS_0_ID_MASK )

#define GRCAN_MSG_FLAGS_0_RTR 0x40000000U

#define GRCAN_MSG_FLAGS_0_IDE 0x80000000U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDMessageFLAGS1 \
 *   Message Status and Length Word (FLAGS_1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_MSG_FLAGS_1_ERRPASS 0x1U

#define GRCAN_MSG_FLAGS_1_BUSOFF 0x2U

#define GRCAN_MSG_FLAGS_1_OR 0x4U

#define GRCAN_MSG_FLAGS_1_BMERR 0x8U

#define GRCAN_MSG_FLAGS_1_RXERRCNTR_SHIFT 8
#define GRCAN_MSG_FLAGS_1_RXERRCNTR_MASK  0xff00U
#define GRCAN_MSG_FLAGS_1_RXERRCNTR_GET( _reg )        \
  ( ( ( _reg ) & GRCAN_MSG_FLAGS_1_RXERRCNTR_MASK ) >> \
    GRCAN_MSG_FLAGS_1_RXERRCNTR_SHIFT )
#define GRCAN_MSG_FLAGS_1_RXERRCNTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_MSG_FLAGS_1_RXERRCNTR_MASK ) |    \
    ( ( ( _val ) << GRCAN_MSG_FLAGS_1_RXERRCNTR_SHIFT ) & \
      GRCAN_MSG_FLAGS_1_RXERRCNTR_MASK ) )
#define GRCAN_MSG_FLAGS_1_RXERRCNTR( _val )             \
  ( ( ( _val ) << GRCAN_MSG_FLAGS_1_RXERRCNTR_SHIFT ) & \
    GRCAN_MSG_FLAGS_1_RXERRCNTR_MASK )

#define GRCAN_MSG_FLAGS_1_TXERRCNTR_SHIFT 16
#define GRCAN_MSG_FLAGS_1_TXERRCNTR_MASK  0xff0000U
#define GRCAN_MSG_FLAGS_1_TXERRCNTR_GET( _reg )        \
  ( ( ( _reg ) & GRCAN_MSG_FLAGS_1_TXERRCNTR_MASK ) >> \
    GRCAN_MSG_FLAGS_1_TXERRCNTR_SHIFT )
#define GRCAN_MSG_FLAGS_1_TXERRCNTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_MSG_FLAGS_1_TXERRCNTR_MASK ) |    \
    ( ( ( _val ) << GRCAN_MSG_FLAGS_1_TXERRCNTR_SHIFT ) & \
      GRCAN_MSG_FLAGS_1_TXERRCNTR_MASK ) )
#define GRCAN_MSG_FLAGS_1_TXERRCNTR( _val )             \
  ( ( ( _val ) << GRCAN_MSG_FLAGS_1_TXERRCNTR_SHIFT ) & \
    GRCAN_MSG_FLAGS_1_TXERRCNTR_MASK )

#define GRCAN_MSG_FLAGS_1_BRS 0x2000000U

#define GRCAN_MSG_FLAGS_1_FDF 0x4000000U

#define GRCAN_MSG_FLAGS_1_DLC_SHIFT 28
#define GRCAN_MSG_FLAGS_1_DLC_MASK  0xf0000000U
#define GRCAN_MSG_FLAGS_1_DLC_GET( _reg ) \
  ( ( ( _reg ) & GRCAN_MSG_FLAGS_1_DLC_MASK ) >> GRCAN_MSG_FLAGS_1_DLC_SHIFT )
#define GRCAN_MSG_FLAGS_1_DLC_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_MSG_FLAGS_1_DLC_MASK ) |    \
    ( ( ( _val ) << GRCAN_MSG_FLAGS_1_DLC_SHIFT ) & \
      GRCAN_MSG_FLAGS_1_DLC_MASK ) )
#define GRCAN_MSG_FLAGS_1_DLC( _val ) \
  ( ( ( _val ) << GRCAN_MSG_FLAGS_1_DLC_SHIFT ) & GRCAN_MSG_FLAGS_1_DLC_MASK )
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_0  0U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_1  1U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_2  2U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_3  3U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_4  4U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_5  5U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_6  6U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_7  7U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_8  8U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_12 9U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_16 10U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_20 11U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_24 12U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_32 13U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_48 14U
#define GRCAN_MSG_FLAGS_1_DLC_BYTES_64 15U

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDMessageDATA0123 \
 *   Message Data Word for Byte 0 to Byte 3 (DATA_0_1_2_3)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_MSG_DATA_0_1_2_3_DATA_SHIFT 0
#define GRCAN_MSG_DATA_0_1_2_3_DATA_MASK  0xffffffffU
#define GRCAN_MSG_DATA_0_1_2_3_DATA_GET( _reg )        \
  ( ( ( _reg ) & GRCAN_MSG_DATA_0_1_2_3_DATA_MASK ) >> \
    GRCAN_MSG_DATA_0_1_2_3_DATA_SHIFT )
#define GRCAN_MSG_DATA_0_1_2_3_DATA_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_MSG_DATA_0_1_2_3_DATA_MASK ) |    \
    ( ( ( _val ) << GRCAN_MSG_DATA_0_1_2_3_DATA_SHIFT ) & \
      GRCAN_MSG_DATA_0_1_2_3_DATA_MASK ) )
#define GRCAN_MSG_DATA_0_1_2_3_DATA( _val )             \
  ( ( ( _val ) << GRCAN_MSG_DATA_0_1_2_3_DATA_SHIFT ) & \
    GRCAN_MSG_DATA_0_1_2_3_DATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRCANFDMessageDATA4567 \
 *   Message Data Word for Byte 4 to Byte 7 (DATA_4_5_6_7)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRCAN_MSG_DATA_4_5_6_7_DATA_SHIFT 0
#define GRCAN_MSG_DATA_4_5_6_7_DATA_MASK  0xffffffffU
#define GRCAN_MSG_DATA_4_5_6_7_DATA_GET( _reg )        \
  ( ( ( _reg ) & GRCAN_MSG_DATA_4_5_6_7_DATA_MASK ) >> \
    GRCAN_MSG_DATA_4_5_6_7_DATA_SHIFT )
#define GRCAN_MSG_DATA_4_5_6_7_DATA_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRCAN_MSG_DATA_4_5_6_7_DATA_MASK ) |    \
    ( ( ( _val ) << GRCAN_MSG_DATA_4_5_6_7_DATA_SHIFT ) & \
      GRCAN_MSG_DATA_4_5_6_7_DATA_MASK ) )
#define GRCAN_MSG_DATA_4_5_6_7_DATA( _val )             \
  ( ( ( _val ) << GRCAN_MSG_DATA_4_5_6_7_DATA_SHIFT ) & \
    GRCAN_MSG_DATA_4_5_6_7_DATA_MASK )

/** @} */

/**
 * @brief This structure defines the GRCANFD Message message descriptor memory
 *   map.
 */
typedef struct grcan_message {
  /**
   * @brief See @ref RTEMSDeviceGRCANFDMessageFLAGS0.
   */
  uint32_t flags_0;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDMessageFLAGS1.
   */
  uint32_t flags_1;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDMessageDATA0123.
   */
  uint32_t data_0_1_2_3;

  /**
   * @brief See @ref RTEMSDeviceGRCANFDMessageDATA4567.
   */
  uint32_t data_4_5_6_7;
} grcan_message;

/** @} */

/* Find related documentation with spec:/dev/grlib/if/grcanfd-message-max-count */

/**
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This constant defines the maximum count of GRCANFD message buffers
 *   used by a frame.
 */
#define GRCANFD_MSG_MAX_COUNT 5

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_GRCANFD_REGS_H */
