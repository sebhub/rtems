/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRWIZL
 *
 * @brief This header file defines the GRWIZL register block interface.
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

/* Find related documentation with spec:/dev/grlib/if/grwizl-header */

#ifndef _GRLIB_GRWIZL_REGS_H
#define _GRLIB_GRWIZL_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/grwizl */

/**
 * @defgroup RTEMSDeviceGRWIZL GRWIZL
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the GRWIZL interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRWIZLCCAP Codec Capabilities Register (CCAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_CCAP_TXB_SHIFT 9
#define GRWIZL_CCAP_TXB_MASK  0x3e00U
#define GRWIZL_CCAP_TXB_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_CCAP_TXB_MASK ) >> GRWIZL_CCAP_TXB_SHIFT )
#define GRWIZL_CCAP_TXB_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_CCAP_TXB_MASK ) | \
    ( ( ( _val ) << GRWIZL_CCAP_TXB_SHIFT ) & GRWIZL_CCAP_TXB_MASK ) )
#define GRWIZL_CCAP_TXB( _val ) \
  ( ( ( _val ) << GRWIZL_CCAP_TXB_SHIFT ) & GRWIZL_CCAP_TXB_MASK )

#define GRWIZL_CCAP_RXB_SHIFT 4
#define GRWIZL_CCAP_RXB_MASK  0x1f0U
#define GRWIZL_CCAP_RXB_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_CCAP_RXB_MASK ) >> GRWIZL_CCAP_RXB_SHIFT )
#define GRWIZL_CCAP_RXB_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_CCAP_RXB_MASK ) | \
    ( ( ( _val ) << GRWIZL_CCAP_RXB_SHIFT ) & GRWIZL_CCAP_RXB_MASK ) )
#define GRWIZL_CCAP_RXB( _val ) \
  ( ( ( _val ) << GRWIZL_CCAP_RXB_SHIFT ) & GRWIZL_CCAP_RXB_MASK )

#define GRWIZL_CCAP_EFC 0x8U

#define GRWIZL_CCAP_BM 0x4U

#define GRWIZL_CCAP_IED 0x2U

#define GRWIZL_CCAP_SCK 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLCCTRL Codec Control Register (CCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_CCTRL_BD 0x8U

#define GRWIZL_CCTRL_LB 0x4U

#define GRWIZL_CCTRL_TE 0x2U

#define GRWIZL_CCTRL_RE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLCSTS Codec Status Register (CSTS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_CSTS_OR 0x200U

#define GRWIZL_CSTS_REO 0x100U

#define GRWIZL_CSTS_REC_SHIFT 0
#define GRWIZL_CSTS_REC_MASK  0xffU
#define GRWIZL_CSTS_REC_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_CSTS_REC_MASK ) >> GRWIZL_CSTS_REC_SHIFT )
#define GRWIZL_CSTS_REC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_CSTS_REC_MASK ) | \
    ( ( ( _val ) << GRWIZL_CSTS_REC_SHIFT ) & GRWIZL_CSTS_REC_MASK ) )
#define GRWIZL_CSTS_REC( _val ) \
  ( ( ( _val ) << GRWIZL_CSTS_REC_SHIFT ) & GRWIZL_CSTS_REC_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLCCHAR \
 *   Control Characters Configuration Register (CCHAR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_CCHAR_IDLE_SHIFT 28
#define GRWIZL_CCHAR_IDLE_MASK  0xf0000000U
#define GRWIZL_CCHAR_IDLE_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_CCHAR_IDLE_MASK ) >> GRWIZL_CCHAR_IDLE_SHIFT )
#define GRWIZL_CCHAR_IDLE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_CCHAR_IDLE_MASK ) | \
    ( ( ( _val ) << GRWIZL_CCHAR_IDLE_SHIFT ) & GRWIZL_CCHAR_IDLE_MASK ) )
#define GRWIZL_CCHAR_IDLE( _val ) \
  ( ( ( _val ) << GRWIZL_CCHAR_IDLE_SHIFT ) & GRWIZL_CCHAR_IDLE_MASK )

#define GRWIZL_CCHAR_EEP_SHIFT 16
#define GRWIZL_CCHAR_EEP_MASK  0xff0000U
#define GRWIZL_CCHAR_EEP_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_CCHAR_EEP_MASK ) >> GRWIZL_CCHAR_EEP_SHIFT )
#define GRWIZL_CCHAR_EEP_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_CCHAR_EEP_MASK ) | \
    ( ( ( _val ) << GRWIZL_CCHAR_EEP_SHIFT ) & GRWIZL_CCHAR_EEP_MASK ) )
#define GRWIZL_CCHAR_EEP( _val ) \
  ( ( ( _val ) << GRWIZL_CCHAR_EEP_SHIFT ) & GRWIZL_CCHAR_EEP_MASK )

#define GRWIZL_CCHAR_EOP_SHIFT 8
#define GRWIZL_CCHAR_EOP_MASK  0xff00U
#define GRWIZL_CCHAR_EOP_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_CCHAR_EOP_MASK ) >> GRWIZL_CCHAR_EOP_SHIFT )
#define GRWIZL_CCHAR_EOP_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_CCHAR_EOP_MASK ) | \
    ( ( ( _val ) << GRWIZL_CCHAR_EOP_SHIFT ) & GRWIZL_CCHAR_EOP_MASK ) )
#define GRWIZL_CCHAR_EOP( _val ) \
  ( ( ( _val ) << GRWIZL_CCHAR_EOP_SHIFT ) & GRWIZL_CCHAR_EOP_MASK )

#define GRWIZL_CCHAR_COM_SHIFT 0
#define GRWIZL_CCHAR_COM_MASK  0xffU
#define GRWIZL_CCHAR_COM_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_CCHAR_COM_MASK ) >> GRWIZL_CCHAR_COM_SHIFT )
#define GRWIZL_CCHAR_COM_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_CCHAR_COM_MASK ) | \
    ( ( ( _val ) << GRWIZL_CCHAR_COM_SHIFT ) & GRWIZL_CCHAR_COM_MASK ) )
#define GRWIZL_CCHAR_COM( _val ) \
  ( ( ( _val ) << GRWIZL_CCHAR_COM_SHIFT ) & GRWIZL_CCHAR_COM_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLIDLE \
 *   Idle control word configuration register (IDLE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_IDLE_DATA_SHIFT 0
#define GRWIZL_IDLE_DATA_MASK  0xffffffffU
#define GRWIZL_IDLE_DATA_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_IDLE_DATA_MASK ) >> GRWIZL_IDLE_DATA_SHIFT )
#define GRWIZL_IDLE_DATA_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_IDLE_DATA_MASK ) | \
    ( ( ( _val ) << GRWIZL_IDLE_DATA_SHIFT ) & GRWIZL_IDLE_DATA_MASK ) )
#define GRWIZL_IDLE_DATA( _val ) \
  ( ( ( _val ) << GRWIZL_IDLE_DATA_SHIFT ) & GRWIZL_IDLE_DATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLIDLECNT \
 *   Idle Counter Configuration Register (IDLECNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_IDLECNT_CNT_SHIFT 0
#define GRWIZL_IDLECNT_CNT_MASK  0xffffU
#define GRWIZL_IDLECNT_CNT_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_IDLECNT_CNT_MASK ) >> GRWIZL_IDLECNT_CNT_SHIFT )
#define GRWIZL_IDLECNT_CNT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_IDLECNT_CNT_MASK ) | \
    ( ( ( _val ) << GRWIZL_IDLECNT_CNT_SHIFT ) & GRWIZL_IDLECNT_CNT_MASK ) )
#define GRWIZL_IDLECNT_CNT( _val ) \
  ( ( ( _val ) << GRWIZL_IDLECNT_CNT_SHIFT ) & GRWIZL_IDLECNT_CNT_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLADDR Address Register (ADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_ADDR_MASK_SHIFT 8
#define GRWIZL_ADDR_MASK_MASK  0xff00U
#define GRWIZL_ADDR_MASK_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_ADDR_MASK_MASK ) >> GRWIZL_ADDR_MASK_SHIFT )
#define GRWIZL_ADDR_MASK_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_ADDR_MASK_MASK ) | \
    ( ( ( _val ) << GRWIZL_ADDR_MASK_SHIFT ) & GRWIZL_ADDR_MASK_MASK ) )
#define GRWIZL_ADDR_MASK( _val ) \
  ( ( ( _val ) << GRWIZL_ADDR_MASK_SHIFT ) & GRWIZL_ADDR_MASK_MASK )

#define GRWIZL_ADDR_CODE_SHIFT 0
#define GRWIZL_ADDR_CODE_MASK  0xffU
#define GRWIZL_ADDR_CODE_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_ADDR_CODE_MASK ) >> GRWIZL_ADDR_CODE_SHIFT )
#define GRWIZL_ADDR_CODE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_ADDR_CODE_MASK ) | \
    ( ( ( _val ) << GRWIZL_ADDR_CODE_SHIFT ) & GRWIZL_ADDR_CODE_MASK ) )
#define GRWIZL_ADDR_CODE( _val ) \
  ( ( ( _val ) << GRWIZL_ADDR_CODE_SHIFT ) & GRWIZL_ADDR_CODE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLRXMAXLEN RX max length register (RXMAXLEN)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_RXMAXLEN_MAXLGTH_SHIFT 2
#define GRWIZL_RXMAXLEN_MAXLGTH_MASK  0x1fffffcU
#define GRWIZL_RXMAXLEN_MAXLGTH_GET( _reg )        \
  ( ( ( _reg ) & GRWIZL_RXMAXLEN_MAXLGTH_MASK ) >> \
    GRWIZL_RXMAXLEN_MAXLGTH_SHIFT )
#define GRWIZL_RXMAXLEN_MAXLGTH_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRWIZL_RXMAXLEN_MAXLGTH_MASK ) |    \
    ( ( ( _val ) << GRWIZL_RXMAXLEN_MAXLGTH_SHIFT ) & \
      GRWIZL_RXMAXLEN_MAXLGTH_MASK ) )
#define GRWIZL_RXMAXLEN_MAXLGTH( _val )             \
  ( ( ( _val ) << GRWIZL_RXMAXLEN_MAXLGTH_SHIFT ) & \
    GRWIZL_RXMAXLEN_MAXLGTH_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLPROGTX Programmable Commands TX Register (PROGTX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_PROGTX_REQ_SHIFT 0
#define GRWIZL_PROGTX_REQ_MASK  0xffU
#define GRWIZL_PROGTX_REQ_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGTX_REQ_MASK ) >> GRWIZL_PROGTX_REQ_SHIFT )
#define GRWIZL_PROGTX_REQ_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGTX_REQ_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGTX_REQ_SHIFT ) & GRWIZL_PROGTX_REQ_MASK ) )
#define GRWIZL_PROGTX_REQ( _val ) \
  ( ( ( _val ) << GRWIZL_PROGTX_REQ_SHIFT ) & GRWIZL_PROGTX_REQ_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLPROGRX Programmable Commands RX Register (PROGRX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_PROGRX_STS_SHIFT 0
#define GRWIZL_PROGRX_STS_MASK  0xffU
#define GRWIZL_PROGRX_STS_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGRX_STS_MASK ) >> GRWIZL_PROGRX_STS_SHIFT )
#define GRWIZL_PROGRX_STS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGRX_STS_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGRX_STS_SHIFT ) & GRWIZL_PROGRX_STS_MASK ) )
#define GRWIZL_PROGRX_STS( _val ) \
  ( ( ( _val ) << GRWIZL_PROGRX_STS_SHIFT ) & GRWIZL_PROGRX_STS_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLPROGEN \
 *   Programmable Commands RX Enable Register (PROGEN)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_PROGEN_EN_SHIFT 0
#define GRWIZL_PROGEN_EN_MASK  0xffU
#define GRWIZL_PROGEN_EN_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGEN_EN_MASK ) >> GRWIZL_PROGEN_EN_SHIFT )
#define GRWIZL_PROGEN_EN_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGEN_EN_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGEN_EN_SHIFT ) & GRWIZL_PROGEN_EN_MASK ) )
#define GRWIZL_PROGEN_EN( _val ) \
  ( ( ( _val ) << GRWIZL_PROGEN_EN_SHIFT ) & GRWIZL_PROGEN_EN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLPROGIRQ \
 *   Programmable Commands IRQ Enable Register (PROGIRQ)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_PROGIRQ_EN_SHIFT 0
#define GRWIZL_PROGIRQ_EN_MASK  0xffU
#define GRWIZL_PROGIRQ_EN_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGIRQ_EN_MASK ) >> GRWIZL_PROGIRQ_EN_SHIFT )
#define GRWIZL_PROGIRQ_EN_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGIRQ_EN_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGIRQ_EN_SHIFT ) & GRWIZL_PROGIRQ_EN_MASK ) )
#define GRWIZL_PROGIRQ_EN( _val ) \
  ( ( ( _val ) << GRWIZL_PROGIRQ_EN_SHIFT ) & GRWIZL_PROGIRQ_EN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLPROGKF \
 *   Programmable Commands K-flags Register (PROGKF)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_PROGKF_KF7_SHIFT 28
#define GRWIZL_PROGKF_KF7_MASK  0xf0000000U
#define GRWIZL_PROGKF_KF7_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGKF_KF7_MASK ) >> GRWIZL_PROGKF_KF7_SHIFT )
#define GRWIZL_PROGKF_KF7_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGKF_KF7_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGKF_KF7_SHIFT ) & GRWIZL_PROGKF_KF7_MASK ) )
#define GRWIZL_PROGKF_KF7( _val ) \
  ( ( ( _val ) << GRWIZL_PROGKF_KF7_SHIFT ) & GRWIZL_PROGKF_KF7_MASK )

#define GRWIZL_PROGKF_KF6_SHIFT 24
#define GRWIZL_PROGKF_KF6_MASK  0xf000000U
#define GRWIZL_PROGKF_KF6_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGKF_KF6_MASK ) >> GRWIZL_PROGKF_KF6_SHIFT )
#define GRWIZL_PROGKF_KF6_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGKF_KF6_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGKF_KF6_SHIFT ) & GRWIZL_PROGKF_KF6_MASK ) )
#define GRWIZL_PROGKF_KF6( _val ) \
  ( ( ( _val ) << GRWIZL_PROGKF_KF6_SHIFT ) & GRWIZL_PROGKF_KF6_MASK )

#define GRWIZL_PROGKF_KF5_SHIFT 20
#define GRWIZL_PROGKF_KF5_MASK  0xf00000U
#define GRWIZL_PROGKF_KF5_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGKF_KF5_MASK ) >> GRWIZL_PROGKF_KF5_SHIFT )
#define GRWIZL_PROGKF_KF5_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGKF_KF5_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGKF_KF5_SHIFT ) & GRWIZL_PROGKF_KF5_MASK ) )
#define GRWIZL_PROGKF_KF5( _val ) \
  ( ( ( _val ) << GRWIZL_PROGKF_KF5_SHIFT ) & GRWIZL_PROGKF_KF5_MASK )

#define GRWIZL_PROGKF_KF4_SHIFT 16
#define GRWIZL_PROGKF_KF4_MASK  0xf0000U
#define GRWIZL_PROGKF_KF4_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGKF_KF4_MASK ) >> GRWIZL_PROGKF_KF4_SHIFT )
#define GRWIZL_PROGKF_KF4_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGKF_KF4_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGKF_KF4_SHIFT ) & GRWIZL_PROGKF_KF4_MASK ) )
#define GRWIZL_PROGKF_KF4( _val ) \
  ( ( ( _val ) << GRWIZL_PROGKF_KF4_SHIFT ) & GRWIZL_PROGKF_KF4_MASK )

#define GRWIZL_PROGKF_KF3_SHIFT 12
#define GRWIZL_PROGKF_KF3_MASK  0xf000U
#define GRWIZL_PROGKF_KF3_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGKF_KF3_MASK ) >> GRWIZL_PROGKF_KF3_SHIFT )
#define GRWIZL_PROGKF_KF3_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGKF_KF3_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGKF_KF3_SHIFT ) & GRWIZL_PROGKF_KF3_MASK ) )
#define GRWIZL_PROGKF_KF3( _val ) \
  ( ( ( _val ) << GRWIZL_PROGKF_KF3_SHIFT ) & GRWIZL_PROGKF_KF3_MASK )

#define GRWIZL_PROGKF_KF2_SHIFT 8
#define GRWIZL_PROGKF_KF2_MASK  0xf00U
#define GRWIZL_PROGKF_KF2_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGKF_KF2_MASK ) >> GRWIZL_PROGKF_KF2_SHIFT )
#define GRWIZL_PROGKF_KF2_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGKF_KF2_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGKF_KF2_SHIFT ) & GRWIZL_PROGKF_KF2_MASK ) )
#define GRWIZL_PROGKF_KF2( _val ) \
  ( ( ( _val ) << GRWIZL_PROGKF_KF2_SHIFT ) & GRWIZL_PROGKF_KF2_MASK )

#define GRWIZL_PROGKF_KF1_SHIFT 4
#define GRWIZL_PROGKF_KF1_MASK  0xf0U
#define GRWIZL_PROGKF_KF1_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGKF_KF1_MASK ) >> GRWIZL_PROGKF_KF1_SHIFT )
#define GRWIZL_PROGKF_KF1_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGKF_KF1_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGKF_KF1_SHIFT ) & GRWIZL_PROGKF_KF1_MASK ) )
#define GRWIZL_PROGKF_KF1( _val ) \
  ( ( ( _val ) << GRWIZL_PROGKF_KF1_SHIFT ) & GRWIZL_PROGKF_KF1_MASK )

#define GRWIZL_PROGKF_KF0_SHIFT 0
#define GRWIZL_PROGKF_KF0_MASK  0xfU
#define GRWIZL_PROGKF_KF0_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGKF_KF0_MASK ) >> GRWIZL_PROGKF_KF0_SHIFT )
#define GRWIZL_PROGKF_KF0_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGKF_KF0_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGKF_KF0_SHIFT ) & GRWIZL_PROGKF_KF0_MASK ) )
#define GRWIZL_PROGKF_KF0( _val ) \
  ( ( ( _val ) << GRWIZL_PROGKF_KF0_SHIFT ) & GRWIZL_PROGKF_KF0_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLPROGCMD Programmable command registers (PROGCMD)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_PROGCMD_CMD_SHIFT 0
#define GRWIZL_PROGCMD_CMD_MASK  0xffffffffU
#define GRWIZL_PROGCMD_CMD_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_PROGCMD_CMD_MASK ) >> GRWIZL_PROGCMD_CMD_SHIFT )
#define GRWIZL_PROGCMD_CMD_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_PROGCMD_CMD_MASK ) | \
    ( ( ( _val ) << GRWIZL_PROGCMD_CMD_SHIFT ) & GRWIZL_PROGCMD_CMD_MASK ) )
#define GRWIZL_PROGCMD_CMD( _val ) \
  ( ( ( _val ) << GRWIZL_PROGCMD_CMD_SHIFT ) & GRWIZL_PROGCMD_CMD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLDMACAP DMA Capabilities Register (DMACAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_DMACAP_WIZ 0x80000000U

#define GRWIZL_DMACAP_NTXD_SHIFT 2
#define GRWIZL_DMACAP_NTXD_MASK  0xcU
#define GRWIZL_DMACAP_NTXD_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_DMACAP_NTXD_MASK ) >> GRWIZL_DMACAP_NTXD_SHIFT )
#define GRWIZL_DMACAP_NTXD_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_DMACAP_NTXD_MASK ) | \
    ( ( ( _val ) << GRWIZL_DMACAP_NTXD_SHIFT ) & GRWIZL_DMACAP_NTXD_MASK ) )
#define GRWIZL_DMACAP_NTXD( _val ) \
  ( ( ( _val ) << GRWIZL_DMACAP_NTXD_SHIFT ) & GRWIZL_DMACAP_NTXD_MASK )

#define GRWIZL_DMACAP_NRXD_SHIFT 0
#define GRWIZL_DMACAP_NRXD_MASK  0x3U
#define GRWIZL_DMACAP_NRXD_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_DMACAP_NRXD_MASK ) >> GRWIZL_DMACAP_NRXD_SHIFT )
#define GRWIZL_DMACAP_NRXD_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_DMACAP_NRXD_MASK ) | \
    ( ( ( _val ) << GRWIZL_DMACAP_NRXD_SHIFT ) & GRWIZL_DMACAP_NRXD_MASK ) )
#define GRWIZL_DMACAP_NRXD( _val ) \
  ( ( ( _val ) << GRWIZL_DMACAP_NRXD_SHIFT ) & GRWIZL_DMACAP_NRXD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLDMACTRL DMA Control Register (DMACTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_DMACTRL_TS 0x8U

#define GRWIZL_DMACTRL_RS 0x4U

#define GRWIZL_DMACTRL_TE 0x2U

#define GRWIZL_DMACTRL_RE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLDMASTS DMA status register (DMASTS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_DMASTS_TXC 0x800000U

#define GRWIZL_DMASTS_RXC 0x400000U

#define GRWIZL_DMASTS_LE 0x200000U

#define GRWIZL_DMASTS_TO 0x100000U

#define GRWIZL_DMASTS_BEDESC_SHIFT 8
#define GRWIZL_DMASTS_BEDESC_MASK  0x3ff00U
#define GRWIZL_DMASTS_BEDESC_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_DMASTS_BEDESC_MASK ) >> GRWIZL_DMASTS_BEDESC_SHIFT )
#define GRWIZL_DMASTS_BEDESC_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRWIZL_DMASTS_BEDESC_MASK ) |    \
    ( ( ( _val ) << GRWIZL_DMASTS_BEDESC_SHIFT ) & \
      GRWIZL_DMASTS_BEDESC_MASK ) )
#define GRWIZL_DMASTS_BEDESC( _val ) \
  ( ( ( _val ) << GRWIZL_DMASTS_BEDESC_SHIFT ) & GRWIZL_DMASTS_BEDESC_MASK )

#define GRWIZL_DMASTS_BECODE_SHIFT 4
#define GRWIZL_DMASTS_BECODE_MASK  0xf0U
#define GRWIZL_DMASTS_BECODE_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_DMASTS_BECODE_MASK ) >> GRWIZL_DMASTS_BECODE_SHIFT )
#define GRWIZL_DMASTS_BECODE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRWIZL_DMASTS_BECODE_MASK ) |    \
    ( ( ( _val ) << GRWIZL_DMASTS_BECODE_SHIFT ) & \
      GRWIZL_DMASTS_BECODE_MASK ) )
#define GRWIZL_DMASTS_BECODE( _val ) \
  ( ( ( _val ) << GRWIZL_DMASTS_BECODE_SHIFT ) & GRWIZL_DMASTS_BECODE_MASK )
#define GRWIZL_DMASTS_BECODE_NO_ERROR             0U
#define GRWIZL_DMASTS_BECODE_FETCH_RX_DESCRIPTOR  1U
#define GRWIZL_DMASTS_BECODE_STORE_RX_DATA        2U
#define GRWIZL_DMASTS_BECODE_STORE_RX_KFLAGS      3U
#define GRWIZL_DMASTS_BECODE_UPDATE_RX_DESCRIPTOR 4U
#define GRWIZL_DMASTS_BECODE_FETCH_TX_DESCRIPTOR  5U
#define GRWIZL_DMASTS_BECODE_FETCH_TX_KFLAGS      6U
#define GRWIZL_DMASTS_BECODE_FETCH_TX_DATA        7U
#define GRWIZL_DMASTS_BECODE_UPDATE_TX_DESCRIPTOR 8U

#define GRWIZL_DMASTS_TX 0x8U

#define GRWIZL_DMASTS_RX 0x4U

#define GRWIZL_DMASTS_TA 0x2U

#define GRWIZL_DMASTS_RA 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLWIZLEN WizardLink Enable Register (WIZLEN)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_WIZLEN_EN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLIRQ Interrupt Control Register (IRQ)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_IRQ_TXC 0x20U

#define GRWIZL_IRQ_RXC 0x10U

#define GRWIZL_IRQ_LE 0x8U

#define GRWIZL_IRQ_BEI 0x4U

#define GRWIZL_IRQ_TI 0x2U

#define GRWIZL_IRQ_RI 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLTXDADDR \
 *   TX Descriptor Table Address Register (TXDADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_TXDADDR_TXADDR_SHIFT 13
#define GRWIZL_TXDADDR_TXADDR_MASK  0xffffe000U
#define GRWIZL_TXDADDR_TXADDR_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_TXDADDR_TXADDR_MASK ) >> GRWIZL_TXDADDR_TXADDR_SHIFT )
#define GRWIZL_TXDADDR_TXADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRWIZL_TXDADDR_TXADDR_MASK ) |    \
    ( ( ( _val ) << GRWIZL_TXDADDR_TXADDR_SHIFT ) & \
      GRWIZL_TXDADDR_TXADDR_MASK ) )
#define GRWIZL_TXDADDR_TXADDR( _val ) \
  ( ( ( _val ) << GRWIZL_TXDADDR_TXADDR_SHIFT ) & GRWIZL_TXDADDR_TXADDR_MASK )

#define GRWIZL_TXDADDR_TXDESCSEL_SHIFT 4
#define GRWIZL_TXDADDR_TXDESCSEL_MASK  0x1ff0U
#define GRWIZL_TXDADDR_TXDESCSEL_GET( _reg )        \
  ( ( ( _reg ) & GRWIZL_TXDADDR_TXDESCSEL_MASK ) >> \
    GRWIZL_TXDADDR_TXDESCSEL_SHIFT )
#define GRWIZL_TXDADDR_TXDESCSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRWIZL_TXDADDR_TXDESCSEL_MASK ) |    \
    ( ( ( _val ) << GRWIZL_TXDADDR_TXDESCSEL_SHIFT ) & \
      GRWIZL_TXDADDR_TXDESCSEL_MASK ) )
#define GRWIZL_TXDADDR_TXDESCSEL( _val )             \
  ( ( ( _val ) << GRWIZL_TXDADDR_TXDESCSEL_SHIFT ) & \
    GRWIZL_TXDADDR_TXDESCSEL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLRXDADDR \
 *   RX Descriptor Table Address Register (RXDADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_RXDADDR_RXADDR_SHIFT 14
#define GRWIZL_RXDADDR_RXADDR_MASK  0xffffc000U
#define GRWIZL_RXDADDR_RXADDR_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_RXDADDR_RXADDR_MASK ) >> GRWIZL_RXDADDR_RXADDR_SHIFT )
#define GRWIZL_RXDADDR_RXADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRWIZL_RXDADDR_RXADDR_MASK ) |    \
    ( ( ( _val ) << GRWIZL_RXDADDR_RXADDR_SHIFT ) & \
      GRWIZL_RXDADDR_RXADDR_MASK ) )
#define GRWIZL_RXDADDR_RXADDR( _val ) \
  ( ( ( _val ) << GRWIZL_RXDADDR_RXADDR_SHIFT ) & GRWIZL_RXDADDR_RXADDR_MASK )

#define GRWIZL_RXDADDR_RXDESCSEL_SHIFT 4
#define GRWIZL_RXDADDR_RXDESCSEL_MASK  0x3ff0U
#define GRWIZL_RXDADDR_RXDESCSEL_GET( _reg )        \
  ( ( ( _reg ) & GRWIZL_RXDADDR_RXDESCSEL_MASK ) >> \
    GRWIZL_RXDADDR_RXDESCSEL_SHIFT )
#define GRWIZL_RXDADDR_RXDESCSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRWIZL_RXDADDR_RXDESCSEL_MASK ) |    \
    ( ( ( _val ) << GRWIZL_RXDADDR_RXDESCSEL_SHIFT ) & \
      GRWIZL_RXDADDR_RXDESCSEL_MASK ) )
#define GRWIZL_RXDADDR_RXDESCSEL( _val )             \
  ( ( ( _val ) << GRWIZL_RXDADDR_RXDESCSEL_SHIFT ) & \
    GRWIZL_RXDADDR_RXDESCSEL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLDCTRL Descriptor Control Register (DCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_DCTRL_TDA 0x80000000U

#define GRWIZL_DCTRL_RDA 0x40000000U

#define GRWIZL_DCTRL_CTD 0x8U

#define GRWIZL_DCTRL_CRD 0x4U

#define GRWIZL_DCTRL_NTD 0x2U

#define GRWIZL_DCTRL_NRD 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRWIZLSTAT Statistics Register (STAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRWIZL_STAT_TX_SHIFT 16
#define GRWIZL_STAT_TX_MASK  0xffff0000U
#define GRWIZL_STAT_TX_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_STAT_TX_MASK ) >> GRWIZL_STAT_TX_SHIFT )
#define GRWIZL_STAT_TX_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_STAT_TX_MASK ) | \
    ( ( ( _val ) << GRWIZL_STAT_TX_SHIFT ) & GRWIZL_STAT_TX_MASK ) )
#define GRWIZL_STAT_TX( _val ) \
  ( ( ( _val ) << GRWIZL_STAT_TX_SHIFT ) & GRWIZL_STAT_TX_MASK )

#define GRWIZL_STAT_RX_SHIFT 0
#define GRWIZL_STAT_RX_MASK  0xffffU
#define GRWIZL_STAT_RX_GET( _reg ) \
  ( ( ( _reg ) & GRWIZL_STAT_RX_MASK ) >> GRWIZL_STAT_RX_SHIFT )
#define GRWIZL_STAT_RX_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRWIZL_STAT_RX_MASK ) | \
    ( ( ( _val ) << GRWIZL_STAT_RX_SHIFT ) & GRWIZL_STAT_RX_MASK ) )
#define GRWIZL_STAT_RX( _val ) \
  ( ( ( _val ) << GRWIZL_STAT_RX_SHIFT ) & GRWIZL_STAT_RX_MASK )

/** @} */

/**
 * @brief This structure defines the GRWIZL register block memory map.
 */
typedef struct grwizl {
  /**
   * @brief See @ref RTEMSDeviceGRWIZLCCAP.
   */
  uint32_t ccap;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLCCTRL.
   */
  uint32_t cctrl;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLCSTS.
   */
  uint32_t csts;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLCCHAR.
   */
  uint32_t cchar;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLIDLE.
   */
  uint32_t idle;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLIDLECNT.
   */
  uint32_t idlecnt;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLADDR.
   */
  uint32_t addr;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLRXMAXLEN.
   */
  uint32_t rxmaxlen;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLPROGTX.
   */
  uint32_t progtx;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLPROGRX.
   */
  uint32_t progrx;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLPROGEN.
   */
  uint32_t progen;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLPROGIRQ.
   */
  uint32_t progirq;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLPROGKF.
   */
  uint32_t progkf;

  uint32_t reserved_34_40[ 3 ];

  /**
   * @brief See @ref RTEMSDeviceGRWIZLPROGCMD.
   */
  uint32_t progcmd[ 8 ];

  uint32_t reserved_60_80[ 8 ];

  /**
   * @brief See @ref RTEMSDeviceGRWIZLDMACAP.
   */
  uint32_t dmacap;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLDMACTRL.
   */
  uint32_t dmactrl;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLDMASTS.
   */
  uint32_t dmasts;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLWIZLEN.
   */
  uint32_t wizlen;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLIRQ.
   */
  uint32_t irq;

  uint32_t reserved_94_a0[ 3 ];

  /**
   * @brief See @ref RTEMSDeviceGRWIZLTXDADDR.
   */
  uint32_t txdaddr;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLRXDADDR.
   */
  uint32_t rxdaddr;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLDCTRL.
   */
  uint32_t dctrl;

  /**
   * @brief See @ref RTEMSDeviceGRWIZLSTAT.
   */
  uint32_t stat;
} grwizl;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_GRWIZL_REGS_H */
