/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRSPFI
 *
 * @brief This header file defines the GRSPFI register block interface.
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

/* Find related documentation with spec:/dev/grlib/if/grspfi-header */

#ifndef _GRLIB_GRSPFI_REGS_H
#define _GRLIB_GRSPFI_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/grspfi-dmachan */

/**
 * @defgroup RTEMSDeviceGRSPFIDMAChannel GRSPFIDMACHAN
 *
 * @ingroup RTEMSDeviceGRSPFI
 *
 * @brief This group contains the GRSPFIDMACHAN interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRSPFIDMAChannelDCCTRL \
 *   DMA channel control register (DCCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_DMACHAN_DCCTRL_TS 0x8U

#define GRSPFI_DMACHAN_DCCTRL_RS 0x4U

#define GRSPFI_DMACHAN_DCCTRL_TE 0x2U

#define GRSPFI_DMACHAN_DCCTRL_RE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIDMAChannelDCSTAT \
 *   DMA channel status register (DCSTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_DMACHAN_DCSTAT_RD 0x2000000U

#define GRSPFI_DMACHAN_DCSTAT_BMEVC_SHIFT 20
#define GRSPFI_DMACHAN_DCSTAT_BMEVC_MASK  0x1f00000U
#define GRSPFI_DMACHAN_DCSTAT_BMEVC_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_DMACHAN_DCSTAT_BMEVC_MASK ) >> \
    GRSPFI_DMACHAN_DCSTAT_BMEVC_SHIFT )
#define GRSPFI_DMACHAN_DCSTAT_BMEVC_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_DMACHAN_DCSTAT_BMEVC_MASK ) |    \
    ( ( ( _val ) << GRSPFI_DMACHAN_DCSTAT_BMEVC_SHIFT ) & \
      GRSPFI_DMACHAN_DCSTAT_BMEVC_MASK ) )
#define GRSPFI_DMACHAN_DCSTAT_BMEVC( _val )             \
  ( ( ( _val ) << GRSPFI_DMACHAN_DCSTAT_BMEVC_SHIFT ) & \
    GRSPFI_DMACHAN_DCSTAT_BMEVC_MASK )

#define GRSPFI_DMACHAN_DCSTAT_BMEDSC_SHIFT 8
#define GRSPFI_DMACHAN_DCSTAT_BMEDSC_MASK  0x3ff00U
#define GRSPFI_DMACHAN_DCSTAT_BMEDSC_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_DMACHAN_DCSTAT_BMEDSC_MASK ) >> \
    GRSPFI_DMACHAN_DCSTAT_BMEDSC_SHIFT )
#define GRSPFI_DMACHAN_DCSTAT_BMEDSC_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_DMACHAN_DCSTAT_BMEDSC_MASK ) |    \
    ( ( ( _val ) << GRSPFI_DMACHAN_DCSTAT_BMEDSC_SHIFT ) & \
      GRSPFI_DMACHAN_DCSTAT_BMEDSC_MASK ) )
#define GRSPFI_DMACHAN_DCSTAT_BMEDSC( _val )             \
  ( ( ( _val ) << GRSPFI_DMACHAN_DCSTAT_BMEDSC_SHIFT ) & \
    GRSPFI_DMACHAN_DCSTAT_BMEDSC_MASK )

#define GRSPFI_DMACHAN_DCSTAT_BMECD_SHIFT 4
#define GRSPFI_DMACHAN_DCSTAT_BMECD_MASK  0xf0U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_DMACHAN_DCSTAT_BMECD_MASK ) >> \
    GRSPFI_DMACHAN_DCSTAT_BMECD_SHIFT )
#define GRSPFI_DMACHAN_DCSTAT_BMECD_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_DMACHAN_DCSTAT_BMECD_MASK ) |    \
    ( ( ( _val ) << GRSPFI_DMACHAN_DCSTAT_BMECD_SHIFT ) & \
      GRSPFI_DMACHAN_DCSTAT_BMECD_MASK ) )
#define GRSPFI_DMACHAN_DCSTAT_BMECD( _val )             \
  ( ( ( _val ) << GRSPFI_DMACHAN_DCSTAT_BMECD_SHIFT ) & \
    GRSPFI_DMACHAN_DCSTAT_BMECD_MASK )
#define GRSPFI_DMACHAN_DCSTAT_BMECD_NO_ERROR             0U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_FETCH_RX_DESCRIPTOR  1U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_STORE_RX_PACKET      2U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_UPDATE_RX_DESCRIPTOR 3U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_STORE_RX_BROADCAST   4U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_FETCH_TX_DESCRIPTOR  5U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_FETCH_TX_HEADER      6U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_FETCH_TX_DATA        7U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_UPDATE_TX_DESCRIPTOR 8U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_FETCH_TX_BROADCAST   9U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_RMAP_WRITE           10U
#define GRSPFI_DMACHAN_DCSTAT_BMECD_RMAP_READ            11U

#define GRSPFI_DMACHAN_DCSTAT_TX 0x8U

#define GRSPFI_DMACHAN_DCSTAT_RX 0x4U

#define GRSPFI_DMACHAN_DCSTAT_TA 0x2U

#define GRSPFI_DMACHAN_DCSTAT_RA 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIDMAChannelDCMAP \
 *   DMA channel VC mapping register (DCMAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_DMACHAN_DCMAP_VCMAP_SHIFT 0
#define GRSPFI_DMACHAN_DCMAP_VCMAP_MASK  0xffffffffU
#define GRSPFI_DMACHAN_DCMAP_VCMAP_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_DMACHAN_DCMAP_VCMAP_MASK ) >> \
    GRSPFI_DMACHAN_DCMAP_VCMAP_SHIFT )
#define GRSPFI_DMACHAN_DCMAP_VCMAP_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_DMACHAN_DCMAP_VCMAP_MASK ) |    \
    ( ( ( _val ) << GRSPFI_DMACHAN_DCMAP_VCMAP_SHIFT ) & \
      GRSPFI_DMACHAN_DCMAP_VCMAP_MASK ) )
#define GRSPFI_DMACHAN_DCMAP_VCMAP( _val )             \
  ( ( ( _val ) << GRSPFI_DMACHAN_DCMAP_VCMAP_SHIFT ) & \
    GRSPFI_DMACHAN_DCMAP_VCMAP_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIDMAChannelDCICTRL \
 *   DMA channel IRQ control register (DCICTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_DMACHAN_DCICTRL_RMI 0x8U

#define GRSPFI_DMACHAN_DCICTRL_BEI 0x4U

#define GRSPFI_DMACHAN_DCICTRL_TI 0x2U

#define GRSPFI_DMACHAN_DCICTRL_RI 0x1U

/** @} */

/**
 * @brief This structure defines the GRSPFIDMACHAN register block memory map.
 */
typedef struct grspfi_dmachan {
  /**
   * @brief See @ref RTEMSDeviceGRSPFIDMAChannelDCCTRL.
   */
  uint32_t dcctrl;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIDMAChannelDCSTAT.
   */
  uint32_t dcstat;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIDMAChannelDCMAP.
   */
  uint32_t dcmap;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIDMAChannelDCICTRL.
   */
  uint32_t dcictrl;
} grspfi_dmachan;

/** @} */

/* Find related documentation with spec:/dev/grlib/if/grspfi-dmachan2 */

/**
 * @defgroup RTEMSDeviceGRSPFIDMAChannelStatus GRSPFIDMACHAN2
 *
 * @ingroup RTEMSDeviceGRSPFI
 *
 * @brief This group contains the GRSPFIDMACHAN2 interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRSPFIDMAChannelStatusDCSTAT2 \
 *   DMA channel extended status register (DCSTAT2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_DMACHAN2_DCSTAT2_RC_SHIFT 16
#define GRSPFI_DMACHAN2_DCSTAT2_RC_MASK  0xffff0000U
#define GRSPFI_DMACHAN2_DCSTAT2_RC_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_DMACHAN2_DCSTAT2_RC_MASK ) >> \
    GRSPFI_DMACHAN2_DCSTAT2_RC_SHIFT )
#define GRSPFI_DMACHAN2_DCSTAT2_RC_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_DMACHAN2_DCSTAT2_RC_MASK ) |    \
    ( ( ( _val ) << GRSPFI_DMACHAN2_DCSTAT2_RC_SHIFT ) & \
      GRSPFI_DMACHAN2_DCSTAT2_RC_MASK ) )
#define GRSPFI_DMACHAN2_DCSTAT2_RC( _val )             \
  ( ( ( _val ) << GRSPFI_DMACHAN2_DCSTAT2_RC_SHIFT ) & \
    GRSPFI_DMACHAN2_DCSTAT2_RC_MASK )

#define GRSPFI_DMACHAN2_DCSTAT2_TAF 0x8000U

#define GRSPFI_DMACHAN2_DCSTAT2_TF 0x4000U

#define GRSPFI_DMACHAN2_DCSTAT2_TS 0x2000U

#define GRSPFI_DMACHAN2_DCSTAT2_TFS_SHIFT 8
#define GRSPFI_DMACHAN2_DCSTAT2_TFS_MASK  0x1f00U
#define GRSPFI_DMACHAN2_DCSTAT2_TFS_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_DMACHAN2_DCSTAT2_TFS_MASK ) >> \
    GRSPFI_DMACHAN2_DCSTAT2_TFS_SHIFT )
#define GRSPFI_DMACHAN2_DCSTAT2_TFS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_DMACHAN2_DCSTAT2_TFS_MASK ) |    \
    ( ( ( _val ) << GRSPFI_DMACHAN2_DCSTAT2_TFS_SHIFT ) & \
      GRSPFI_DMACHAN2_DCSTAT2_TFS_MASK ) )
#define GRSPFI_DMACHAN2_DCSTAT2_TFS( _val )             \
  ( ( ( _val ) << GRSPFI_DMACHAN2_DCSTAT2_TFS_SHIFT ) & \
    GRSPFI_DMACHAN2_DCSTAT2_TFS_MASK )

#define GRSPFI_DMACHAN2_DCSTAT2_RAE 0x80U

#define GRSPFI_DMACHAN2_DCSTAT2_RE 0x40U

#define GRSPFI_DMACHAN2_DCSTAT2_RS 0x20U

#define GRSPFI_DMACHAN2_DCSTAT2_RFS_SHIFT 0
#define GRSPFI_DMACHAN2_DCSTAT2_RFS_MASK  0x1fU
#define GRSPFI_DMACHAN2_DCSTAT2_RFS_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_DMACHAN2_DCSTAT2_RFS_MASK ) >> \
    GRSPFI_DMACHAN2_DCSTAT2_RFS_SHIFT )
#define GRSPFI_DMACHAN2_DCSTAT2_RFS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_DMACHAN2_DCSTAT2_RFS_MASK ) |    \
    ( ( ( _val ) << GRSPFI_DMACHAN2_DCSTAT2_RFS_SHIFT ) & \
      GRSPFI_DMACHAN2_DCSTAT2_RFS_MASK ) )
#define GRSPFI_DMACHAN2_DCSTAT2_RFS( _val )             \
  ( ( ( _val ) << GRSPFI_DMACHAN2_DCSTAT2_RFS_SHIFT ) & \
    GRSPFI_DMACHAN2_DCSTAT2_RFS_MASK )

/** @} */

/**
 * @brief This structure defines the GRSPFIDMACHAN2 register block memory map.
 */
typedef struct grspfi_dmachan2 {
  /**
   * @brief See @ref RTEMSDeviceGRSPFIDMAChannelStatusDCSTAT2.
   */
  uint32_t dcstat2;

  uint32_t reserved_4_10[ 3 ];
} grspfi_dmachan2;

/** @} */

/* Find related documentation with spec:/dev/grlib/if/grspfi-vc */

/**
 * @defgroup RTEMSDeviceGRSPFIVC GRSPFIVC
 *
 * @ingroup RTEMSDeviceGRSPFI
 *
 * @brief This group contains the GRSPFIVC interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRSPFIVCVCCTRL \
 *   Virtual channel control register (VCCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VC_VCCTRL_VBW_SHIFT 16
#define GRSPFI_VC_VCCTRL_VBW_MASK  0xffff0000U
#define GRSPFI_VC_VCCTRL_VBW_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_VC_VCCTRL_VBW_MASK ) >> GRSPFI_VC_VCCTRL_VBW_SHIFT )
#define GRSPFI_VC_VCCTRL_VBW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VC_VCCTRL_VBW_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VC_VCCTRL_VBW_SHIFT ) & \
      GRSPFI_VC_VCCTRL_VBW_MASK ) )
#define GRSPFI_VC_VCCTRL_VBW( _val ) \
  ( ( ( _val ) << GRSPFI_VC_VCCTRL_VBW_SHIFT ) & GRSPFI_VC_VCCTRL_VBW_MASK )

#define GRSPFI_VC_VCCTRL_PR_SHIFT 12
#define GRSPFI_VC_VCCTRL_PR_MASK  0xf000U
#define GRSPFI_VC_VCCTRL_PR_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_VC_VCCTRL_PR_MASK ) >> GRSPFI_VC_VCCTRL_PR_SHIFT )
#define GRSPFI_VC_VCCTRL_PR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VC_VCCTRL_PR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VC_VCCTRL_PR_SHIFT ) & \
      GRSPFI_VC_VCCTRL_PR_MASK ) )
#define GRSPFI_VC_VCCTRL_PR( _val ) \
  ( ( ( _val ) << GRSPFI_VC_VCCTRL_PR_SHIFT ) & GRSPFI_VC_VCCTRL_PR_MASK )

#define GRSPFI_VC_VCCTRL_EFC_SHIFT 4
#define GRSPFI_VC_VCCTRL_EFC_MASK  0x1f0U
#define GRSPFI_VC_VCCTRL_EFC_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_VC_VCCTRL_EFC_MASK ) >> GRSPFI_VC_VCCTRL_EFC_SHIFT )
#define GRSPFI_VC_VCCTRL_EFC_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VC_VCCTRL_EFC_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VC_VCCTRL_EFC_SHIFT ) & \
      GRSPFI_VC_VCCTRL_EFC_MASK ) )
#define GRSPFI_VC_VCCTRL_EFC( _val ) \
  ( ( ( _val ) << GRSPFI_VC_VCCTRL_EFC_SHIFT ) & GRSPFI_VC_VCCTRL_EFC_MASK )

#define GRSPFI_VC_VCCTRL_BD 0x8U

#define GRSPFI_VC_VCCTRL_RE 0x2U

#define GRSPFI_VC_VCCTRL_AE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIVCVCSTAT Virtual channel status register (VCSTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VC_VCSTAT_FCO 0x10U

#define GRSPFI_VC_VCSTAT_IBO 0x8U

#define GRSPFI_VC_VCSTAT_DHC 0x4U

#define GRSPFI_VC_VCSTAT_BUU 0x2U

#define GRSPFI_VC_VCSTAT_BOU 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIVCVCMAXLEN \
 *   Virtual channel RX max length register (VCMAXLEN)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VC_VCMAXLEN_MAXLGTH_SHIFT 0
#define GRSPFI_VC_VCMAXLEN_MAXLGTH_MASK  0x1ffffffU
#define GRSPFI_VC_VCMAXLEN_MAXLGTH_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VC_VCMAXLEN_MAXLGTH_MASK ) >> \
    GRSPFI_VC_VCMAXLEN_MAXLGTH_SHIFT )
#define GRSPFI_VC_VCMAXLEN_MAXLGTH_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VC_VCMAXLEN_MAXLGTH_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VC_VCMAXLEN_MAXLGTH_SHIFT ) & \
      GRSPFI_VC_VCMAXLEN_MAXLGTH_MASK ) )
#define GRSPFI_VC_VCMAXLEN_MAXLGTH( _val )             \
  ( ( ( _val ) << GRSPFI_VC_VCMAXLEN_MAXLGTH_SHIFT ) & \
    GRSPFI_VC_VCMAXLEN_MAXLGTH_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIVCVCADDR \
 *   Virtual channel address register (VCADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VC_VCADDR_VCMSK_SHIFT 8
#define GRSPFI_VC_VCADDR_VCMSK_MASK  0xff00U
#define GRSPFI_VC_VCADDR_VCMSK_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VC_VCADDR_VCMSK_MASK ) >> \
    GRSPFI_VC_VCADDR_VCMSK_SHIFT )
#define GRSPFI_VC_VCADDR_VCMSK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VC_VCADDR_VCMSK_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VC_VCADDR_VCMSK_SHIFT ) & \
      GRSPFI_VC_VCADDR_VCMSK_MASK ) )
#define GRSPFI_VC_VCADDR_VCMSK( _val )             \
  ( ( ( _val ) << GRSPFI_VC_VCADDR_VCMSK_SHIFT ) & \
    GRSPFI_VC_VCADDR_VCMSK_MASK )

#define GRSPFI_VC_VCADDR_VCADDR_SHIFT 0
#define GRSPFI_VC_VCADDR_VCADDR_MASK  0xffU
#define GRSPFI_VC_VCADDR_VCADDR_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VC_VCADDR_VCADDR_MASK ) >> \
    GRSPFI_VC_VCADDR_VCADDR_SHIFT )
#define GRSPFI_VC_VCADDR_VCADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VC_VCADDR_VCADDR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VC_VCADDR_VCADDR_SHIFT ) & \
      GRSPFI_VC_VCADDR_VCADDR_MASK ) )
#define GRSPFI_VC_VCADDR_VCADDR( _val )             \
  ( ( ( _val ) << GRSPFI_VC_VCADDR_VCADDR_SHIFT ) & \
    GRSPFI_VC_VCADDR_VCADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIVCVCTS1 \
 *   Virtual channel time-slot 1 register (VCTS1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VC_VCTS1_TSLOT1_SHIFT 0
#define GRSPFI_VC_VCTS1_TSLOT1_MASK  0xffffffffU
#define GRSPFI_VC_VCTS1_TSLOT1_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VC_VCTS1_TSLOT1_MASK ) >> \
    GRSPFI_VC_VCTS1_TSLOT1_SHIFT )
#define GRSPFI_VC_VCTS1_TSLOT1_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VC_VCTS1_TSLOT1_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VC_VCTS1_TSLOT1_SHIFT ) & \
      GRSPFI_VC_VCTS1_TSLOT1_MASK ) )
#define GRSPFI_VC_VCTS1_TSLOT1( _val )             \
  ( ( ( _val ) << GRSPFI_VC_VCTS1_TSLOT1_SHIFT ) & \
    GRSPFI_VC_VCTS1_TSLOT1_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIVCVCTS2 \
 *   Virtual channel time-slot 2 register (VCTS2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VC_VCTS2_TSLOT2_SHIFT 0
#define GRSPFI_VC_VCTS2_TSLOT2_MASK  0xffffffffU
#define GRSPFI_VC_VCTS2_TSLOT2_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VC_VCTS2_TSLOT2_MASK ) >> \
    GRSPFI_VC_VCTS2_TSLOT2_SHIFT )
#define GRSPFI_VC_VCTS2_TSLOT2_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VC_VCTS2_TSLOT2_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VC_VCTS2_TSLOT2_SHIFT ) & \
      GRSPFI_VC_VCTS2_TSLOT2_MASK ) )
#define GRSPFI_VC_VCTS2_TSLOT2( _val )             \
  ( ( ( _val ) << GRSPFI_VC_VCTS2_TSLOT2_SHIFT ) & \
    GRSPFI_VC_VCTS2_TSLOT2_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIVCVCKEY \
 *   Virtual channel destination key register (VCKEY)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VC_VCKEY_KEY_SHIFT 0
#define GRSPFI_VC_VCKEY_KEY_MASK  0xffU
#define GRSPFI_VC_VCKEY_KEY_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_VC_VCKEY_KEY_MASK ) >> GRSPFI_VC_VCKEY_KEY_SHIFT )
#define GRSPFI_VC_VCKEY_KEY_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VC_VCKEY_KEY_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VC_VCKEY_KEY_SHIFT ) & \
      GRSPFI_VC_VCKEY_KEY_MASK ) )
#define GRSPFI_VC_VCKEY_KEY( _val ) \
  ( ( ( _val ) << GRSPFI_VC_VCKEY_KEY_SHIFT ) & GRSPFI_VC_VCKEY_KEY_MASK )

/** @} */

/**
 * @brief This structure defines the GRSPFIVC register block memory map.
 */
typedef struct grspfi_vc {
  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCVCCTRL.
   */
  uint32_t vcctrl;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCVCSTAT.
   */
  uint32_t vcstat;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCVCMAXLEN.
   */
  uint32_t vcmaxlen;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCVCADDR.
   */
  uint32_t vcaddr;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCVCTS1.
   */
  uint32_t vcts1;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCVCTS2.
   */
  uint32_t vcts2;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCVCKEY.
   */
  uint32_t vckey;

  uint32_t reserved_1c_20;
} grspfi_vc;

/** @} */

/* Find related documentation with spec:/dev/grlib/if/grspfi-vcdesc */

/**
 * @defgroup RTEMSDeviceGRSPFIVCDescriptor GRSPFIVCDESC
 *
 * @ingroup RTEMSDeviceGRSPFI
 *
 * @brief This group contains the GRSPFIVCDESC interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRSPFIVCDescriptorVCTXDADDR \
 *   VC TX descriptor table address register (VCTXDADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VCDESC_VCTXDADDR_TXADDR_SHIFT 13
#define GRSPFI_VCDESC_VCTXDADDR_TXADDR_MASK  0xffffe000U
#define GRSPFI_VCDESC_VCTXDADDR_TXADDR_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VCDESC_VCTXDADDR_TXADDR_MASK ) >> \
    GRSPFI_VCDESC_VCTXDADDR_TXADDR_SHIFT )
#define GRSPFI_VCDESC_VCTXDADDR_TXADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VCDESC_VCTXDADDR_TXADDR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VCDESC_VCTXDADDR_TXADDR_SHIFT ) & \
      GRSPFI_VCDESC_VCTXDADDR_TXADDR_MASK ) )
#define GRSPFI_VCDESC_VCTXDADDR_TXADDR( _val )             \
  ( ( ( _val ) << GRSPFI_VCDESC_VCTXDADDR_TXADDR_SHIFT ) & \
    GRSPFI_VCDESC_VCTXDADDR_TXADDR_MASK )

#define GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_SHIFT 4
#define GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_MASK  0x1ff0U
#define GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_MASK ) >> \
    GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_SHIFT )
#define GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_SHIFT ) & \
      GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_MASK ) )
#define GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL( _val )             \
  ( ( ( _val ) << GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_SHIFT ) & \
    GRSPFI_VCDESC_VCTXDADDR_TXDESCSEL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIVCDescriptorVCRXDADDR \
 *   VC RX descriptor table address register (VCRXDADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VCDESC_VCRXDADDR_RXADDR_SHIFT 13
#define GRSPFI_VCDESC_VCRXDADDR_RXADDR_MASK  0xffffe000U
#define GRSPFI_VCDESC_VCRXDADDR_RXADDR_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VCDESC_VCRXDADDR_RXADDR_MASK ) >> \
    GRSPFI_VCDESC_VCRXDADDR_RXADDR_SHIFT )
#define GRSPFI_VCDESC_VCRXDADDR_RXADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VCDESC_VCRXDADDR_RXADDR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VCDESC_VCRXDADDR_RXADDR_SHIFT ) & \
      GRSPFI_VCDESC_VCRXDADDR_RXADDR_MASK ) )
#define GRSPFI_VCDESC_VCRXDADDR_RXADDR( _val )             \
  ( ( ( _val ) << GRSPFI_VCDESC_VCRXDADDR_RXADDR_SHIFT ) & \
    GRSPFI_VCDESC_VCRXDADDR_RXADDR_MASK )

#define GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_SHIFT 3
#define GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_MASK  0x1ff8U
#define GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_MASK ) >> \
    GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_SHIFT )
#define GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_SHIFT ) & \
      GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_MASK ) )
#define GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL( _val )             \
  ( ( ( _val ) << GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_SHIFT ) & \
    GRSPFI_VCDESC_VCRXDADDR_RXDESCSEL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIVCDescriptorVCDCTRL \
 *   VC descriptor control register (VCDCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VCDESC_VCDCTRL_TDA 0x80000000U

#define GRSPFI_VCDESC_VCDCTRL_RDA 0x40000000U

#define GRSPFI_VCDESC_VCDCTRL_CTD 0x8U

#define GRSPFI_VCDESC_VCDCTRL_CRD 0x4U

#define GRSPFI_VCDESC_VCDCTRL_NTD 0x2U

#define GRSPFI_VCDESC_VCDCTRL_NRD 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIVCDescriptorVCSTAT \
 *   VC statistics register (VCSTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_VCDESC_VCSTAT_TX_SHIFT 16
#define GRSPFI_VCDESC_VCSTAT_TX_MASK  0xffff0000U
#define GRSPFI_VCDESC_VCSTAT_TX_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VCDESC_VCSTAT_TX_MASK ) >> \
    GRSPFI_VCDESC_VCSTAT_TX_SHIFT )
#define GRSPFI_VCDESC_VCSTAT_TX_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VCDESC_VCSTAT_TX_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VCDESC_VCSTAT_TX_SHIFT ) & \
      GRSPFI_VCDESC_VCSTAT_TX_MASK ) )
#define GRSPFI_VCDESC_VCSTAT_TX( _val )             \
  ( ( ( _val ) << GRSPFI_VCDESC_VCSTAT_TX_SHIFT ) & \
    GRSPFI_VCDESC_VCSTAT_TX_MASK )

#define GRSPFI_VCDESC_VCSTAT_RX_SHIFT 0
#define GRSPFI_VCDESC_VCSTAT_RX_MASK  0xffffU
#define GRSPFI_VCDESC_VCSTAT_RX_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_VCDESC_VCSTAT_RX_MASK ) >> \
    GRSPFI_VCDESC_VCSTAT_RX_SHIFT )
#define GRSPFI_VCDESC_VCSTAT_RX_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_VCDESC_VCSTAT_RX_MASK ) |    \
    ( ( ( _val ) << GRSPFI_VCDESC_VCSTAT_RX_SHIFT ) & \
      GRSPFI_VCDESC_VCSTAT_RX_MASK ) )
#define GRSPFI_VCDESC_VCSTAT_RX( _val )             \
  ( ( ( _val ) << GRSPFI_VCDESC_VCSTAT_RX_SHIFT ) & \
    GRSPFI_VCDESC_VCSTAT_RX_MASK )

/** @} */

/**
 * @brief This structure defines the GRSPFIVCDESC register block memory map.
 */
typedef struct grspfi_vcdesc {
  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCDescriptorVCTXDADDR.
   */
  uint32_t vctxdaddr;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCDescriptorVCRXDADDR.
   */
  uint32_t vcrxdaddr;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCDescriptorVCDCTRL.
   */
  uint32_t vcdctrl;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCDescriptorVCSTAT.
   */
  uint32_t vcstat;
} grspfi_vcdesc;

/** @} */

/* Find related documentation with spec:/dev/grlib/if/grspfi */

/**
 * @defgroup RTEMSDeviceGRSPFI GRSPFI
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the GRSPFI interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRSPFIGENCAP \
 *   Codec general capabilities register (GENCAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_GENCAP_NEB 0x10000000U

#define GRSPFI_GENCAP_NEV_SHIFT 22
#define GRSPFI_GENCAP_NEV_MASK  0xfc00000U
#define GRSPFI_GENCAP_NEV_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_GENCAP_NEV_MASK ) >> GRSPFI_GENCAP_NEV_SHIFT )
#define GRSPFI_GENCAP_NEV_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_GENCAP_NEV_MASK ) | \
    ( ( ( _val ) << GRSPFI_GENCAP_NEV_SHIFT ) & GRSPFI_GENCAP_NEV_MASK ) )
#define GRSPFI_GENCAP_NEV( _val ) \
  ( ( ( _val ) << GRSPFI_GENCAP_NEV_SHIFT ) & GRSPFI_GENCAP_NEV_MASK )

#define GRSPFI_GENCAP_WBWC_SHIFT 17
#define GRSPFI_GENCAP_WBWC_MASK  0x3e0000U
#define GRSPFI_GENCAP_WBWC_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_GENCAP_WBWC_MASK ) >> GRSPFI_GENCAP_WBWC_SHIFT )
#define GRSPFI_GENCAP_WBWC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_GENCAP_WBWC_MASK ) | \
    ( ( ( _val ) << GRSPFI_GENCAP_WBWC_SHIFT ) & GRSPFI_GENCAP_WBWC_MASK ) )
#define GRSPFI_GENCAP_WBWC( _val ) \
  ( ( ( _val ) << GRSPFI_GENCAP_WBWC_SHIFT ) & GRSPFI_GENCAP_WBWC_MASK )

#define GRSPFI_GENCAP_RFC_SHIFT 12
#define GRSPFI_GENCAP_RFC_MASK  0x1f000U
#define GRSPFI_GENCAP_RFC_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_GENCAP_RFC_MASK ) >> GRSPFI_GENCAP_RFC_SHIFT )
#define GRSPFI_GENCAP_RFC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_GENCAP_RFC_MASK ) | \
    ( ( ( _val ) << GRSPFI_GENCAP_RFC_SHIFT ) & GRSPFI_GENCAP_RFC_MASK ) )
#define GRSPFI_GENCAP_RFC( _val ) \
  ( ( ( _val ) << GRSPFI_GENCAP_RFC_SHIFT ) & GRSPFI_GENCAP_RFC_MASK )

#define GRSPFI_GENCAP_FCTM_SHIFT 9
#define GRSPFI_GENCAP_FCTM_MASK  0xe00U
#define GRSPFI_GENCAP_FCTM_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_GENCAP_FCTM_MASK ) >> GRSPFI_GENCAP_FCTM_SHIFT )
#define GRSPFI_GENCAP_FCTM_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_GENCAP_FCTM_MASK ) | \
    ( ( ( _val ) << GRSPFI_GENCAP_FCTM_SHIFT ) & GRSPFI_GENCAP_FCTM_MASK ) )
#define GRSPFI_GENCAP_FCTM( _val ) \
  ( ( ( _val ) << GRSPFI_GENCAP_FCTM_SHIFT ) & GRSPFI_GENCAP_FCTM_MASK )

#define GRSPFI_GENCAP_NVC_SHIFT 4
#define GRSPFI_GENCAP_NVC_MASK  0x1f0U
#define GRSPFI_GENCAP_NVC_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_GENCAP_NVC_MASK ) >> GRSPFI_GENCAP_NVC_SHIFT )
#define GRSPFI_GENCAP_NVC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_GENCAP_NVC_MASK ) | \
    ( ( ( _val ) << GRSPFI_GENCAP_NVC_SHIFT ) & GRSPFI_GENCAP_NVC_MASK ) )
#define GRSPFI_GENCAP_NVC( _val ) \
  ( ( ( _val ) << GRSPFI_GENCAP_NVC_SHIFT ) & GRSPFI_GENCAP_NVC_MASK )

#define GRSPFI_GENCAP_PI 0x8U

#define GRSPFI_GENCAP_BM 0x4U

#define GRSPFI_GENCAP_IED 0x2U

#define GRSPFI_GENCAP_SCK 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBUFCAP \
 *   Codec buffers capabilities register (BUFCAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BUFCAP_VT_SHIFT 20
#define GRSPFI_BUFCAP_VT_MASK  0x1f00000U
#define GRSPFI_BUFCAP_VT_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BUFCAP_VT_MASK ) >> GRSPFI_BUFCAP_VT_SHIFT )
#define GRSPFI_BUFCAP_VT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_BUFCAP_VT_MASK ) | \
    ( ( ( _val ) << GRSPFI_BUFCAP_VT_SHIFT ) & GRSPFI_BUFCAP_VT_MASK ) )
#define GRSPFI_BUFCAP_VT( _val ) \
  ( ( ( _val ) << GRSPFI_BUFCAP_VT_SHIFT ) & GRSPFI_BUFCAP_VT_MASK )

#define GRSPFI_BUFCAP_VR_SHIFT 15
#define GRSPFI_BUFCAP_VR_MASK  0xf8000U
#define GRSPFI_BUFCAP_VR_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BUFCAP_VR_MASK ) >> GRSPFI_BUFCAP_VR_SHIFT )
#define GRSPFI_BUFCAP_VR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_BUFCAP_VR_MASK ) | \
    ( ( ( _val ) << GRSPFI_BUFCAP_VR_SHIFT ) & GRSPFI_BUFCAP_VR_MASK ) )
#define GRSPFI_BUFCAP_VR( _val ) \
  ( ( ( _val ) << GRSPFI_BUFCAP_VR_SHIFT ) & GRSPFI_BUFCAP_VR_MASK )

#define GRSPFI_BUFCAP_RTB_SHIFT 10
#define GRSPFI_BUFCAP_RTB_MASK  0x7c00U
#define GRSPFI_BUFCAP_RTB_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BUFCAP_RTB_MASK ) >> GRSPFI_BUFCAP_RTB_SHIFT )
#define GRSPFI_BUFCAP_RTB_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_BUFCAP_RTB_MASK ) | \
    ( ( ( _val ) << GRSPFI_BUFCAP_RTB_SHIFT ) & GRSPFI_BUFCAP_RTB_MASK ) )
#define GRSPFI_BUFCAP_RTB( _val ) \
  ( ( ( _val ) << GRSPFI_BUFCAP_RTB_SHIFT ) & GRSPFI_BUFCAP_RTB_MASK )

#define GRSPFI_BUFCAP_RTF_SHIFT 5
#define GRSPFI_BUFCAP_RTF_MASK  0x3e0U
#define GRSPFI_BUFCAP_RTF_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BUFCAP_RTF_MASK ) >> GRSPFI_BUFCAP_RTF_SHIFT )
#define GRSPFI_BUFCAP_RTF_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_BUFCAP_RTF_MASK ) | \
    ( ( ( _val ) << GRSPFI_BUFCAP_RTF_SHIFT ) & GRSPFI_BUFCAP_RTF_MASK ) )
#define GRSPFI_BUFCAP_RTF( _val ) \
  ( ( ( _val ) << GRSPFI_BUFCAP_RTF_SHIFT ) & GRSPFI_BUFCAP_RTF_MASK )

#define GRSPFI_BUFCAP_RTD_SHIFT 0
#define GRSPFI_BUFCAP_RTD_MASK  0x1fU
#define GRSPFI_BUFCAP_RTD_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BUFCAP_RTD_MASK ) >> GRSPFI_BUFCAP_RTD_SHIFT )
#define GRSPFI_BUFCAP_RTD_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_BUFCAP_RTD_MASK ) | \
    ( ( ( _val ) << GRSPFI_BUFCAP_RTD_SHIFT ) & GRSPFI_BUFCAP_RTD_MASK ) )
#define GRSPFI_BUFCAP_RTD( _val ) \
  ( ( ( _val ) << GRSPFI_BUFCAP_RTD_SHIFT ) & GRSPFI_BUFCAP_RTD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFICCTRL Codec control register (CCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_CCTRL_SCR 0x2000U

#define GRSPFI_CCTRL_LB 0x1000U

#define GRSPFI_CCTRL_SBR_SHIFT 4
#define GRSPFI_CCTRL_SBR_MASK  0xff0U
#define GRSPFI_CCTRL_SBR_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_CCTRL_SBR_MASK ) >> GRSPFI_CCTRL_SBR_SHIFT )
#define GRSPFI_CCTRL_SBR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_CCTRL_SBR_MASK ) | \
    ( ( ( _val ) << GRSPFI_CCTRL_SBR_SHIFT ) & GRSPFI_CCTRL_SBR_MASK ) )
#define GRSPFI_CCTRL_SBR( _val ) \
  ( ( ( _val ) << GRSPFI_CCTRL_SBR_SHIFT ) & GRSPFI_CCTRL_SBR_MASK )

#define GRSPFI_CCTRL_AS 0x8U

#define GRSPFI_CCTRL_LS 0x4U

#define GRSPFI_CCTRL_LNR 0x2U

#define GRSPFI_CCTRL_LIR 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFILLSTAT Lane layer status register (LLSTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_LLSTAT_FEC_SHIFT 20
#define GRSPFI_LLSTAT_FEC_MASK  0xf00000U
#define GRSPFI_LLSTAT_FEC_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_LLSTAT_FEC_MASK ) >> GRSPFI_LLSTAT_FEC_SHIFT )
#define GRSPFI_LLSTAT_FEC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_LLSTAT_FEC_MASK ) | \
    ( ( ( _val ) << GRSPFI_LLSTAT_FEC_SHIFT ) & GRSPFI_LLSTAT_FEC_MASK ) )
#define GRSPFI_LLSTAT_FEC( _val ) \
  ( ( ( _val ) << GRSPFI_LLSTAT_FEC_SHIFT ) & GRSPFI_LLSTAT_FEC_MASK )

#define GRSPFI_LLSTAT_REC_SHIFT 12
#define GRSPFI_LLSTAT_REC_MASK  0xff000U
#define GRSPFI_LLSTAT_REC_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_LLSTAT_REC_MASK ) >> GRSPFI_LLSTAT_REC_SHIFT )
#define GRSPFI_LLSTAT_REC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_LLSTAT_REC_MASK ) | \
    ( ( ( _val ) << GRSPFI_LLSTAT_REC_SHIFT ) & GRSPFI_LLSTAT_REC_MASK ) )
#define GRSPFI_LLSTAT_REC( _val ) \
  ( ( ( _val ) << GRSPFI_LLSTAT_REC_SHIFT ) & GRSPFI_LLSTAT_REC_MASK )

#define GRSPFI_LLSTAT_REO 0x800U

#define GRSPFI_LLSTAT_RXP 0x400U

#define GRSPFI_LLSTAT_TO 0x200U

#define GRSPFI_LLSTAT_FELC_SHIFT 7
#define GRSPFI_LLSTAT_FELC_MASK  0x180U
#define GRSPFI_LLSTAT_FELC_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_LLSTAT_FELC_MASK ) >> GRSPFI_LLSTAT_FELC_SHIFT )
#define GRSPFI_LLSTAT_FELC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_LLSTAT_FELC_MASK ) | \
    ( ( ( _val ) << GRSPFI_LLSTAT_FELC_SHIFT ) & GRSPFI_LLSTAT_FELC_MASK ) )
#define GRSPFI_LLSTAT_FELC( _val ) \
  ( ( ( _val ) << GRSPFI_LLSTAT_FELC_SHIFT ) & GRSPFI_LLSTAT_FELC_MASK )

#define GRSPFI_LLSTAT_FEL 0x40U

#define GRSPFI_LLSTAT_FES 0x20U

#define GRSPFI_LLSTAT_FER 0x10U

#define GRSPFI_LLSTAT_LSTS_SHIFT 0
#define GRSPFI_LLSTAT_LSTS_MASK  0xfU
#define GRSPFI_LLSTAT_LSTS_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_LLSTAT_LSTS_MASK ) >> GRSPFI_LLSTAT_LSTS_SHIFT )
#define GRSPFI_LLSTAT_LSTS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_LLSTAT_LSTS_MASK ) | \
    ( ( ( _val ) << GRSPFI_LLSTAT_LSTS_SHIFT ) & GRSPFI_LLSTAT_LSTS_MASK ) )
#define GRSPFI_LLSTAT_LSTS( _val ) \
  ( ( ( _val ) << GRSPFI_LLSTAT_LSTS_SHIFT ) & GRSPFI_LLSTAT_LSTS_MASK )
#define GRSPFI_LLSTAT_LSTS_CLEAR_LINE         1U
#define GRSPFI_LLSTAT_LSTS_DISABLED           2U
#define GRSPFI_LLSTAT_LSTS_WAIT               3U
#define GRSPFI_LLSTAT_LSTS_STARTED            4U
#define GRSPFI_LLSTAT_LSTS_INVERT_RX_POLARITY 5U
#define GRSPFI_LLSTAT_LSTS_CONNECTING         6U
#define GRSPFI_LLSTAT_LSTS_CONNECTED          7U
#define GRSPFI_LLSTAT_LSTS_ACTIVE             8U
#define GRSPFI_LLSTAT_LSTS_LOSS_OF_SIGNAL     9U
#define GRSPFI_LLSTAT_LSTS_PREPARE_STANDBY    10U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIRLSTAT Retry layer status register (RLSTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_RLSTAT_RBE 0x400U

#define GRSPFI_RLSTAT_PE 0x200U

#define GRSPFI_RLSTAT_TME 0x100U

#define GRSPFI_RLSTAT_RTC_SHIFT 4
#define GRSPFI_RLSTAT_RTC_MASK  0xf0U
#define GRSPFI_RLSTAT_RTC_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_RLSTAT_RTC_MASK ) >> GRSPFI_RLSTAT_RTC_SHIFT )
#define GRSPFI_RLSTAT_RTC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_RLSTAT_RTC_MASK ) | \
    ( ( ( _val ) << GRSPFI_RLSTAT_RTC_SHIFT ) & GRSPFI_RLSTAT_RTC_MASK ) )
#define GRSPFI_RLSTAT_RTC( _val ) \
  ( ( ( _val ) << GRSPFI_RLSTAT_RTC_SHIFT ) & GRSPFI_RLSTAT_RTC_MASK )

#define GRSPFI_RLSTAT_SE 0x8U

#define GRSPFI_RLSTAT_FE 0x4U

#define GRSPFI_RLSTAT_C8E 0x2U

#define GRSPFI_RLSTAT_CE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIDEFADDR Default address register (DEFADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_DEFADDR_DEFMSK_SHIFT 8
#define GRSPFI_DEFADDR_DEFMSK_MASK  0xff00U
#define GRSPFI_DEFADDR_DEFMSK_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_DEFADDR_DEFMSK_MASK ) >> GRSPFI_DEFADDR_DEFMSK_SHIFT )
#define GRSPFI_DEFADDR_DEFMSK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_DEFADDR_DEFMSK_MASK ) |    \
    ( ( ( _val ) << GRSPFI_DEFADDR_DEFMSK_SHIFT ) & \
      GRSPFI_DEFADDR_DEFMSK_MASK ) )
#define GRSPFI_DEFADDR_DEFMSK( _val ) \
  ( ( ( _val ) << GRSPFI_DEFADDR_DEFMSK_SHIFT ) & GRSPFI_DEFADDR_DEFMSK_MASK )

#define GRSPFI_DEFADDR_DEFADDR_SHIFT 0
#define GRSPFI_DEFADDR_DEFADDR_MASK  0xffU
#define GRSPFI_DEFADDR_DEFADDR_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_DEFADDR_DEFADDR_MASK ) >> \
    GRSPFI_DEFADDR_DEFADDR_SHIFT )
#define GRSPFI_DEFADDR_DEFADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_DEFADDR_DEFADDR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_DEFADDR_DEFADDR_SHIFT ) & \
      GRSPFI_DEFADDR_DEFADDR_MASK ) )
#define GRSPFI_DEFADDR_DEFADDR( _val )             \
  ( ( ( _val ) << GRSPFI_DEFADDR_DEFADDR_SHIFT ) & \
    GRSPFI_DEFADDR_DEFADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFITS Time-slot register (TS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_TS_CNT_SHIFT 8
#define GRSPFI_TS_CNT_MASK  0x3fffff00U
#define GRSPFI_TS_CNT_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_TS_CNT_MASK ) >> GRSPFI_TS_CNT_SHIFT )
#define GRSPFI_TS_CNT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_TS_CNT_MASK ) | \
    ( ( ( _val ) << GRSPFI_TS_CNT_SHIFT ) & GRSPFI_TS_CNT_MASK ) )
#define GRSPFI_TS_CNT( _val ) \
  ( ( ( _val ) << GRSPFI_TS_CNT_SHIFT ) & GRSPFI_TS_CNT_MASK )

#define GRSPFI_TS_TS_SHIFT 0
#define GRSPFI_TS_TS_MASK  0x3fU
#define GRSPFI_TS_TS_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_TS_TS_MASK ) >> GRSPFI_TS_TS_SHIFT )
#define GRSPFI_TS_TS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_TS_TS_MASK ) | \
    ( ( ( _val ) << GRSPFI_TS_TS_SHIFT ) & GRSPFI_TS_TS_MASK ) )
#define GRSPFI_TS_TS( _val ) \
  ( ( ( _val ) << GRSPFI_TS_TS_SHIFT ) & GRSPFI_TS_TS_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFITSCTRL Time-slot control register (TSCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_TSCTRL_ADJ_SHIFT 4
#define GRSPFI_TSCTRL_ADJ_MASK  0x3ff0U
#define GRSPFI_TSCTRL_ADJ_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_TSCTRL_ADJ_MASK ) >> GRSPFI_TSCTRL_ADJ_SHIFT )
#define GRSPFI_TSCTRL_ADJ_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_TSCTRL_ADJ_MASK ) | \
    ( ( ( _val ) << GRSPFI_TSCTRL_ADJ_SHIFT ) & GRSPFI_TSCTRL_ADJ_MASK ) )
#define GRSPFI_TSCTRL_ADJ( _val ) \
  ( ( ( _val ) << GRSPFI_TSCTRL_ADJ_SHIFT ) & GRSPFI_TSCTRL_ADJ_MASK )

#define GRSPFI_TSCTRL_AS 0x8U

#define GRSPFI_TSCTRL_AV 0x4U

#define GRSPFI_TSCTRL_EG 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFITSLEN Time-slot length register (TSLEN)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_TSLEN_NOMLEN_SHIFT 0
#define GRSPFI_TSLEN_NOMLEN_MASK  0x1fffffU
#define GRSPFI_TSLEN_NOMLEN_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_TSLEN_NOMLEN_MASK ) >> GRSPFI_TSLEN_NOMLEN_SHIFT )
#define GRSPFI_TSLEN_NOMLEN_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_TSLEN_NOMLEN_MASK ) |    \
    ( ( ( _val ) << GRSPFI_TSLEN_NOMLEN_SHIFT ) & \
      GRSPFI_TSLEN_NOMLEN_MASK ) )
#define GRSPFI_TSLEN_NOMLEN( _val ) \
  ( ( ( _val ) << GRSPFI_TSLEN_NOMLEN_SHIFT ) & GRSPFI_TSLEN_NOMLEN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBCCONF \
 *   Broadcast channel configuration register (BCCONF)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BCCONF_BW_SHIFT 16
#define GRSPFI_BCCONF_BW_MASK  0xffff0000U
#define GRSPFI_BCCONF_BW_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BCCONF_BW_MASK ) >> GRSPFI_BCCONF_BW_SHIFT )
#define GRSPFI_BCCONF_BW_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_BCCONF_BW_MASK ) | \
    ( ( ( _val ) << GRSPFI_BCCONF_BW_SHIFT ) & GRSPFI_BCCONF_BW_MASK ) )
#define GRSPFI_BCCONF_BW( _val ) \
  ( ( ( _val ) << GRSPFI_BCCONF_BW_SHIFT ) & GRSPFI_BCCONF_BW_MASK )

#define GRSPFI_BCCONF_BD 0x100U

#define GRSPFI_BCCONF_CHN_SHIFT 0
#define GRSPFI_BCCONF_CHN_MASK  0xffU
#define GRSPFI_BCCONF_CHN_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BCCONF_CHN_MASK ) >> GRSPFI_BCCONF_CHN_SHIFT )
#define GRSPFI_BCCONF_CHN_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_BCCONF_CHN_MASK ) | \
    ( ( ( _val ) << GRSPFI_BCCONF_CHN_SHIFT ) & GRSPFI_BCCONF_CHN_MASK ) )
#define GRSPFI_BCCONF_CHN( _val ) \
  ( ( ( _val ) << GRSPFI_BCCONF_CHN_SHIFT ) & GRSPFI_BCCONF_CHN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIDLCAP DMA layer capabilities register (DLCAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_DLCAP_SPFI 0x80000000U

#define GRSPFI_DLCAP_CC 0x400U

#define GRSPFI_DLCAP_RC 0x200U

#define GRSPFI_DLCAP_RT 0x100U

#define GRSPFI_DLCAP_NDMA_SHIFT 4
#define GRSPFI_DLCAP_NDMA_MASK  0x70U
#define GRSPFI_DLCAP_NDMA_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_DLCAP_NDMA_MASK ) >> GRSPFI_DLCAP_NDMA_SHIFT )
#define GRSPFI_DLCAP_NDMA_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_DLCAP_NDMA_MASK ) | \
    ( ( ( _val ) << GRSPFI_DLCAP_NDMA_SHIFT ) & GRSPFI_DLCAP_NDMA_MASK ) )
#define GRSPFI_DLCAP_NDMA( _val ) \
  ( ( ( _val ) << GRSPFI_DLCAP_NDMA_SHIFT ) & GRSPFI_DLCAP_NDMA_MASK )

#define GRSPFI_DLCAP_NTXD_SHIFT 2
#define GRSPFI_DLCAP_NTXD_MASK  0xcU
#define GRSPFI_DLCAP_NTXD_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_DLCAP_NTXD_MASK ) >> GRSPFI_DLCAP_NTXD_SHIFT )
#define GRSPFI_DLCAP_NTXD_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_DLCAP_NTXD_MASK ) | \
    ( ( ( _val ) << GRSPFI_DLCAP_NTXD_SHIFT ) & GRSPFI_DLCAP_NTXD_MASK ) )
#define GRSPFI_DLCAP_NTXD( _val ) \
  ( ( ( _val ) << GRSPFI_DLCAP_NTXD_SHIFT ) & GRSPFI_DLCAP_NTXD_MASK )

#define GRSPFI_DLCAP_NRXD_SHIFT 0
#define GRSPFI_DLCAP_NRXD_MASK  0x3U
#define GRSPFI_DLCAP_NRXD_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_DLCAP_NRXD_MASK ) >> GRSPFI_DLCAP_NRXD_SHIFT )
#define GRSPFI_DLCAP_NRXD_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_DLCAP_NRXD_MASK ) | \
    ( ( ( _val ) << GRSPFI_DLCAP_NRXD_SHIFT ) & GRSPFI_DLCAP_NRXD_MASK ) )
#define GRSPFI_DLCAP_NRXD( _val ) \
  ( ( ( _val ) << GRSPFI_DLCAP_NRXD_SHIFT ) & GRSPFI_DLCAP_NRXD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIDLCTRL DMA layer control register (DLCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_DLCTRL_BTE 0x4U

#define GRSPFI_DLCTRL_BRE 0x2U

#define GRSPFI_DLCTRL_LE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIDLSTAT DMA layer status register (DLSTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_DLSTAT_DCI_SHIFT 16
#define GRSPFI_DLSTAT_DCI_MASK  0xff0000U
#define GRSPFI_DLSTAT_DCI_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_DLSTAT_DCI_MASK ) >> GRSPFI_DLSTAT_DCI_SHIFT )
#define GRSPFI_DLSTAT_DCI_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_DLSTAT_DCI_MASK ) | \
    ( ( ( _val ) << GRSPFI_DLSTAT_DCI_SHIFT ) & GRSPFI_DLSTAT_DCI_MASK ) )
#define GRSPFI_DLSTAT_DCI( _val ) \
  ( ( ( _val ) << GRSPFI_DLSTAT_DCI_SHIFT ) & GRSPFI_DLSTAT_DCI_MASK )

#define GRSPFI_DLSTAT_TO 0x8U

#define GRSPFI_DLSTAT_BT 0x4U

#define GRSPFI_DLSTAT_BR 0x2U

#define GRSPFI_DLSTAT_LE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFISPFIEN SpaceFibre enable register (SPFIEN)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_SPFIEN_EN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBCTXADDR \
 *   Broadcast channel TX address register (BCTXADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BCTXADDR_TXADDR_SHIFT 10
#define GRSPFI_BCTXADDR_TXADDR_MASK  0xfffffc00U
#define GRSPFI_BCTXADDR_TXADDR_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_BCTXADDR_TXADDR_MASK ) >> \
    GRSPFI_BCTXADDR_TXADDR_SHIFT )
#define GRSPFI_BCTXADDR_TXADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_BCTXADDR_TXADDR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_BCTXADDR_TXADDR_SHIFT ) & \
      GRSPFI_BCTXADDR_TXADDR_MASK ) )
#define GRSPFI_BCTXADDR_TXADDR( _val )             \
  ( ( ( _val ) << GRSPFI_BCTXADDR_TXADDR_SHIFT ) & \
    GRSPFI_BCTXADDR_TXADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBCTXSIZE \
 *   Broadcast channel TX size register (BCTXSIZE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BCTXSIZE_SIZE_SHIFT 4
#define GRSPFI_BCTXSIZE_SIZE_MASK  0xffff0U
#define GRSPFI_BCTXSIZE_SIZE_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BCTXSIZE_SIZE_MASK ) >> GRSPFI_BCTXSIZE_SIZE_SHIFT )
#define GRSPFI_BCTXSIZE_SIZE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_BCTXSIZE_SIZE_MASK ) |    \
    ( ( ( _val ) << GRSPFI_BCTXSIZE_SIZE_SHIFT ) & \
      GRSPFI_BCTXSIZE_SIZE_MASK ) )
#define GRSPFI_BCTXSIZE_SIZE( _val ) \
  ( ( ( _val ) << GRSPFI_BCTXSIZE_SIZE_SHIFT ) & GRSPFI_BCTXSIZE_SIZE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBCTXWPTR \
 *   Broadcast channel TX write pointer register (BCTXWPTR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BCTXWPTR_WRPTR_SHIFT 4
#define GRSPFI_BCTXWPTR_WRPTR_MASK  0xffff0U
#define GRSPFI_BCTXWPTR_WRPTR_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BCTXWPTR_WRPTR_MASK ) >> GRSPFI_BCTXWPTR_WRPTR_SHIFT )
#define GRSPFI_BCTXWPTR_WRPTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_BCTXWPTR_WRPTR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_BCTXWPTR_WRPTR_SHIFT ) & \
      GRSPFI_BCTXWPTR_WRPTR_MASK ) )
#define GRSPFI_BCTXWPTR_WRPTR( _val ) \
  ( ( ( _val ) << GRSPFI_BCTXWPTR_WRPTR_SHIFT ) & GRSPFI_BCTXWPTR_WRPTR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBCTXRPTR \
 *   Broadcast channel TX read pointer register (BCTXRPTR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BCTXRPTR_RDPTR_SHIFT 4
#define GRSPFI_BCTXRPTR_RDPTR_MASK  0xffff0U
#define GRSPFI_BCTXRPTR_RDPTR_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BCTXRPTR_RDPTR_MASK ) >> GRSPFI_BCTXRPTR_RDPTR_SHIFT )
#define GRSPFI_BCTXRPTR_RDPTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_BCTXRPTR_RDPTR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_BCTXRPTR_RDPTR_SHIFT ) & \
      GRSPFI_BCTXRPTR_RDPTR_MASK ) )
#define GRSPFI_BCTXRPTR_RDPTR( _val ) \
  ( ( ( _val ) << GRSPFI_BCTXRPTR_RDPTR_SHIFT ) & GRSPFI_BCTXRPTR_RDPTR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBCRXADDR \
 *   Broadcast channel RX address register (BCRXADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BCRXADDR_RXADDR_SHIFT 10
#define GRSPFI_BCRXADDR_RXADDR_MASK  0xfffffc00U
#define GRSPFI_BCRXADDR_RXADDR_GET( _reg )        \
  ( ( ( _reg ) & GRSPFI_BCRXADDR_RXADDR_MASK ) >> \
    GRSPFI_BCRXADDR_RXADDR_SHIFT )
#define GRSPFI_BCRXADDR_RXADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_BCRXADDR_RXADDR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_BCRXADDR_RXADDR_SHIFT ) & \
      GRSPFI_BCRXADDR_RXADDR_MASK ) )
#define GRSPFI_BCRXADDR_RXADDR( _val )             \
  ( ( ( _val ) << GRSPFI_BCRXADDR_RXADDR_SHIFT ) & \
    GRSPFI_BCRXADDR_RXADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBCRXSIZE \
 *   Broadcast channel RX size register (BCRXSIZE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BCRXSIZE_SIZE_SHIFT 4
#define GRSPFI_BCRXSIZE_SIZE_MASK  0xffff0U
#define GRSPFI_BCRXSIZE_SIZE_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BCRXSIZE_SIZE_MASK ) >> GRSPFI_BCRXSIZE_SIZE_SHIFT )
#define GRSPFI_BCRXSIZE_SIZE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_BCRXSIZE_SIZE_MASK ) |    \
    ( ( ( _val ) << GRSPFI_BCRXSIZE_SIZE_SHIFT ) & \
      GRSPFI_BCRXSIZE_SIZE_MASK ) )
#define GRSPFI_BCRXSIZE_SIZE( _val ) \
  ( ( ( _val ) << GRSPFI_BCRXSIZE_SIZE_SHIFT ) & GRSPFI_BCRXSIZE_SIZE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBCRXWPTR \
 *   Broadcast channel RX write pointer register (BCRXWPTR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BCRXWPTR_WRPTR_SHIFT 4
#define GRSPFI_BCRXWPTR_WRPTR_MASK  0xffff0U
#define GRSPFI_BCRXWPTR_WRPTR_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BCRXWPTR_WRPTR_MASK ) >> GRSPFI_BCRXWPTR_WRPTR_SHIFT )
#define GRSPFI_BCRXWPTR_WRPTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_BCRXWPTR_WRPTR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_BCRXWPTR_WRPTR_SHIFT ) & \
      GRSPFI_BCRXWPTR_WRPTR_MASK ) )
#define GRSPFI_BCRXWPTR_WRPTR( _val ) \
  ( ( ( _val ) << GRSPFI_BCRXWPTR_WRPTR_SHIFT ) & GRSPFI_BCRXWPTR_WRPTR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBCRXRPTR \
 *   Broadcast channel RX read pointer register (BCRXRPTR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BCRXRPTR_RDPTR_SHIFT 4
#define GRSPFI_BCRXRPTR_RDPTR_MASK  0xffff0U
#define GRSPFI_BCRXRPTR_RDPTR_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BCRXRPTR_RDPTR_MASK ) >> GRSPFI_BCRXRPTR_RDPTR_SHIFT )
#define GRSPFI_BCRXRPTR_RDPTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSPFI_BCRXRPTR_RDPTR_MASK ) |    \
    ( ( ( _val ) << GRSPFI_BCRXRPTR_RDPTR_SHIFT ) & \
      GRSPFI_BCRXRPTR_RDPTR_MASK ) )
#define GRSPFI_BCRXRPTR_RDPTR( _val ) \
  ( ( ( _val ) << GRSPFI_BCRXRPTR_RDPTR_SHIFT ) & GRSPFI_BCRXRPTR_RDPTR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSPFIBCMAP Broadcast channel mapping register (BCMAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSPFI_BCMAP_MAP_SHIFT 0
#define GRSPFI_BCMAP_MAP_MASK  0x1fU
#define GRSPFI_BCMAP_MAP_GET( _reg ) \
  ( ( ( _reg ) & GRSPFI_BCMAP_MAP_MASK ) >> GRSPFI_BCMAP_MAP_SHIFT )
#define GRSPFI_BCMAP_MAP_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSPFI_BCMAP_MAP_MASK ) | \
    ( ( ( _val ) << GRSPFI_BCMAP_MAP_SHIFT ) & GRSPFI_BCMAP_MAP_MASK ) )
#define GRSPFI_BCMAP_MAP( _val ) \
  ( ( ( _val ) << GRSPFI_BCMAP_MAP_SHIFT ) & GRSPFI_BCMAP_MAP_MASK )

/** @} */

/**
 * @brief This structure defines the GRSPFI register block memory map.
 */
typedef struct grspfi {
  /**
   * @brief See @ref RTEMSDeviceGRSPFIGENCAP.
   */
  uint32_t gencap;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBUFCAP.
   */
  uint32_t bufcap;

  /**
   * @brief See @ref RTEMSDeviceGRSPFICCTRL.
   */
  uint32_t cctrl;

  /**
   * @brief See @ref RTEMSDeviceGRSPFILLSTAT.
   */
  uint32_t llstat;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIRLSTAT.
   */
  uint32_t rlstat;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIDEFADDR.
   */
  uint32_t defaddr;

  uint32_t reserved_18_20[ 2 ];

  /**
   * @brief See @ref RTEMSDeviceGRSPFITS.
   */
  uint32_t ts;

  /**
   * @brief See @ref RTEMSDeviceGRSPFITSCTRL.
   */
  uint32_t tsctrl;

  /**
   * @brief See @ref RTEMSDeviceGRSPFITSLEN.
   */
  uint32_t tslen;

  uint32_t reserved_2c_30;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBCCONF.
   */
  uint32_t bcconf;

  uint32_t reserved_34_100[ 51 ];

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVC.
   */
  grspfi_vc vc[ 32 ];

  uint32_t reserved_500_800[ 192 ];

  /**
   * @brief See @ref RTEMSDeviceGRSPFIDLCAP.
   */
  uint32_t dlcap;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIDLCTRL.
   */
  uint32_t dlctrl;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIDLSTAT.
   */
  uint32_t dlstat;

  /**
   * @brief See @ref RTEMSDeviceGRSPFISPFIEN.
   */
  uint32_t spfien;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBCTXADDR.
   */
  uint32_t bctxaddr;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBCTXSIZE.
   */
  uint32_t bctxsize;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBCTXWPTR.
   */
  uint32_t bctxwptr;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBCTXRPTR.
   */
  uint32_t bctxrptr;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBCRXADDR.
   */
  uint32_t bcrxaddr;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBCRXSIZE.
   */
  uint32_t bcrxsize;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBCRXWPTR.
   */
  uint32_t bcrxwptr;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBCRXRPTR.
   */
  uint32_t bcrxrptr;

  /**
   * @brief See @ref RTEMSDeviceGRSPFIBCMAP.
   */
  uint32_t bcmap;

  uint32_t reserved_834_900[ 51 ];

  /**
   * @brief See @ref RTEMSDeviceGRSPFIVCDescriptor.
   */
  grspfi_vcdesc vcdesc[ 32 ];

  /**
   * @brief See @ref RTEMSDeviceGRSPFIDMAChannel.
   */
  grspfi_dmachan dmachan[ 8 ];

  /**
   * @brief See @ref RTEMSDeviceGRSPFIDMAChannelStatus.
   */
  grspfi_dmachan2 dmachanstat[ 8 ];
} grspfi;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_GRSPFI_REGS_H */
