/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRIOMMU2
 *
 * @brief This header file defines the GRIOMMU2 register block interface.
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

/* Find related documentation with spec:/dev/grlib/if/griommu2-header */

#ifndef _GRLIB_GRIOMMU2_REGS_H
#define _GRLIB_GRIOMMU2_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/griommu2-bhpm */

/**
 * @defgroup RTEMSDeviceGRIOMMU2BridgeHPM GRIOMMU2BHPM
 *
 * @ingroup RTEMSDeviceGRIOMMU2Bridge
 *
 * @brief This group contains the GRIOMMU2BHPM interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRIOMMU2BridgeHPMCNT Bridge HPM counter (CNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_BHPM_CNT_CNT_SHIFT 0
#define GRIOMMU2_BHPM_CNT_CNT_MASK  0xffffffffffffffffULL
#define GRIOMMU2_BHPM_CNT_CNT_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_BHPM_CNT_CNT_MASK ) >> GRIOMMU2_BHPM_CNT_CNT_SHIFT )
#define GRIOMMU2_BHPM_CNT_CNT_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_BHPM_CNT_CNT_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_BHPM_CNT_CNT_SHIFT ) & \
      GRIOMMU2_BHPM_CNT_CNT_MASK ) )
#define GRIOMMU2_BHPM_CNT_CNT( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_BHPM_CNT_CNT_SHIFT ) & \
    GRIOMMU2_BHPM_CNT_CNT_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2BridgeHPMMSK Bridge HPM DMA mask (MSK)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_BHPM_MSK_MSK_SHIFT 0
#define GRIOMMU2_BHPM_MSK_MSK_MASK  0xffffffffffffffffULL
#define GRIOMMU2_BHPM_MSK_MSK_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_BHPM_MSK_MSK_MASK ) >> GRIOMMU2_BHPM_MSK_MSK_SHIFT )
#define GRIOMMU2_BHPM_MSK_MSK_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_BHPM_MSK_MSK_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_BHPM_MSK_MSK_SHIFT ) & \
      GRIOMMU2_BHPM_MSK_MSK_MASK ) )
#define GRIOMMU2_BHPM_MSK_MSK( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_BHPM_MSK_MSK_SHIFT ) & \
    GRIOMMU2_BHPM_MSK_MSK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2BridgeHPMFLTR Bridge HPM address filter (FLTR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_BHPM_FLTR_FLTR_SHIFT 2
#define GRIOMMU2_BHPM_FLTR_FLTR_MASK  0xfffffffffffffffcULL
#define GRIOMMU2_BHPM_FLTR_FLTR_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_BHPM_FLTR_FLTR_MASK ) >> \
    GRIOMMU2_BHPM_FLTR_FLTR_SHIFT )
#define GRIOMMU2_BHPM_FLTR_FLTR_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_BHPM_FLTR_FLTR_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_BHPM_FLTR_FLTR_SHIFT ) & \
      GRIOMMU2_BHPM_FLTR_FLTR_MASK ) )
#define GRIOMMU2_BHPM_FLTR_FLTR( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_BHPM_FLTR_FLTR_SHIFT ) & \
    GRIOMMU2_BHPM_FLTR_FLTR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2BridgeHPMFMSK \
 *   Bridge HPM address filter mask (FMSK)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_BHPM_FMSK_FMSK_SHIFT 2
#define GRIOMMU2_BHPM_FMSK_FMSK_MASK  0xfffffffffffffffcULL
#define GRIOMMU2_BHPM_FMSK_FMSK_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_BHPM_FMSK_FMSK_MASK ) >> \
    GRIOMMU2_BHPM_FMSK_FMSK_SHIFT )
#define GRIOMMU2_BHPM_FMSK_FMSK_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_BHPM_FMSK_FMSK_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_BHPM_FMSK_FMSK_SHIFT ) & \
      GRIOMMU2_BHPM_FMSK_FMSK_MASK ) )
#define GRIOMMU2_BHPM_FMSK_FMSK( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_BHPM_FMSK_FMSK_SHIFT ) & \
    GRIOMMU2_BHPM_FMSK_FMSK_MASK )

/** @} */

/**
 * @brief This structure defines the GRIOMMU2BHPM register block memory map.
 */
typedef struct griommu2_bhpm {
  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeHPMCNT.
   */
  uint64_t cnt;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeHPMMSK.
   */
  uint64_t msk;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeHPMFLTR.
   */
  uint64_t fltr;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeHPMFMSK.
   */
  uint64_t fmsk;
} griommu2_bhpm;

/** @} */

/* Find related documentation with spec:/dev/grlib/if/griommu2-bridge */

/**
 * @defgroup RTEMSDeviceGRIOMMU2Bridge GRIOMMU2BRIDGE
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the GRIOMMU2BRIDGE interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRIOMMU2BridgeCAP Capabilities register (CAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_BRIDGE_CAP_PADDRW_SHIFT 16
#define GRIOMMU2_BRIDGE_CAP_PADDRW_MASK  0xff0000U
#define GRIOMMU2_BRIDGE_CAP_PADDRW_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_BRIDGE_CAP_PADDRW_MASK ) >> \
    GRIOMMU2_BRIDGE_CAP_PADDRW_SHIFT )
#define GRIOMMU2_BRIDGE_CAP_PADDRW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_BRIDGE_CAP_PADDRW_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_BRIDGE_CAP_PADDRW_SHIFT ) & \
      GRIOMMU2_BRIDGE_CAP_PADDRW_MASK ) )
#define GRIOMMU2_BRIDGE_CAP_PADDRW( _val )             \
  ( ( ( _val ) << GRIOMMU2_BRIDGE_CAP_PADDRW_SHIFT ) & \
    GRIOMMU2_BRIDGE_CAP_PADDRW_MASK )

#define GRIOMMU2_BRIDGE_CAP_NDMAMST_SHIFT 8
#define GRIOMMU2_BRIDGE_CAP_NDMAMST_MASK  0xff00U
#define GRIOMMU2_BRIDGE_CAP_NDMAMST_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_BRIDGE_CAP_NDMAMST_MASK ) >> \
    GRIOMMU2_BRIDGE_CAP_NDMAMST_SHIFT )
#define GRIOMMU2_BRIDGE_CAP_NDMAMST_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_BRIDGE_CAP_NDMAMST_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_BRIDGE_CAP_NDMAMST_SHIFT ) & \
      GRIOMMU2_BRIDGE_CAP_NDMAMST_MASK ) )
#define GRIOMMU2_BRIDGE_CAP_NDMAMST( _val )             \
  ( ( ( _val ) << GRIOMMU2_BRIDGE_CAP_NDMAMST_SHIFT ) & \
    GRIOMMU2_BRIDGE_CAP_NDMAMST_MASK )

#define GRIOMMU2_BRIDGE_CAP_BHPM 0x10U

#define GRIOMMU2_BRIDGE_CAP_END 0x8U

#define GRIOMMU2_BRIDGE_CAP_BREG 0x4U

#define GRIOMMU2_BRIDGE_CAP_HPMP 0x2U

#define GRIOMMU2_BRIDGE_CAP_HMMU 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2BridgeCTRL Control register (CTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_BRIDGE_CTRL_IRQFFE 0x2U

#define GRIOMMU2_BRIDGE_CTRL_IRQFEN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2BridgeGCFG Global configuration register (GCFG)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_BRIDGE_GCFG_RM 0x100U

#define GRIOMMU2_BRIDGE_GCFG_GRAN1_SHIFT 2
#define GRIOMMU2_BRIDGE_GCFG_GRAN1_MASK  0xcU
#define GRIOMMU2_BRIDGE_GCFG_GRAN1_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_BRIDGE_GCFG_GRAN1_MASK ) >> \
    GRIOMMU2_BRIDGE_GCFG_GRAN1_SHIFT )
#define GRIOMMU2_BRIDGE_GCFG_GRAN1_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_BRIDGE_GCFG_GRAN1_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_BRIDGE_GCFG_GRAN1_SHIFT ) & \
      GRIOMMU2_BRIDGE_GCFG_GRAN1_MASK ) )
#define GRIOMMU2_BRIDGE_GCFG_GRAN1( _val )             \
  ( ( ( _val ) << GRIOMMU2_BRIDGE_GCFG_GRAN1_SHIFT ) & \
    GRIOMMU2_BRIDGE_GCFG_GRAN1_MASK )

#define GRIOMMU2_BRIDGE_GCFG_GRAN0_SHIFT 0
#define GRIOMMU2_BRIDGE_GCFG_GRAN0_MASK  0x3U
#define GRIOMMU2_BRIDGE_GCFG_GRAN0_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_BRIDGE_GCFG_GRAN0_MASK ) >> \
    GRIOMMU2_BRIDGE_GCFG_GRAN0_SHIFT )
#define GRIOMMU2_BRIDGE_GCFG_GRAN0_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_BRIDGE_GCFG_GRAN0_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_BRIDGE_GCFG_GRAN0_SHIFT ) & \
      GRIOMMU2_BRIDGE_GCFG_GRAN0_MASK ) )
#define GRIOMMU2_BRIDGE_GCFG_GRAN0( _val )             \
  ( ( ( _val ) << GRIOMMU2_BRIDGE_GCFG_GRAN0_SHIFT ) & \
    GRIOMMU2_BRIDGE_GCFG_GRAN0_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2BridgeDID Device ID area (DID)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_BRIDGE_DID_DID_SHIFT 0
#define GRIOMMU2_BRIDGE_DID_DID_MASK  0xffffffU
#define GRIOMMU2_BRIDGE_DID_DID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_BRIDGE_DID_DID_MASK ) >> \
    GRIOMMU2_BRIDGE_DID_DID_SHIFT )
#define GRIOMMU2_BRIDGE_DID_DID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_BRIDGE_DID_DID_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_BRIDGE_DID_DID_SHIFT ) & \
      GRIOMMU2_BRIDGE_DID_DID_MASK ) )
#define GRIOMMU2_BRIDGE_DID_DID( _val )             \
  ( ( ( _val ) << GRIOMMU2_BRIDGE_DID_DID_SHIFT ) & \
    GRIOMMU2_BRIDGE_DID_DID_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2BridgeHCONFIG HCONFIG area (HCONFIG)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_SHIFT 0
#define GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_MASK  0xffffffffU
#define GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_MASK ) >> \
    GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_SHIFT )
#define GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_SHIFT ) & \
      GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_MASK ) )
#define GRIOMMU2_BRIDGE_HCONFIG_HCONFIG( _val )             \
  ( ( ( _val ) << GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_SHIFT ) & \
    GRIOMMU2_BRIDGE_HCONFIG_HCONFIG_MASK )

/** @} */

/**
 * @brief This structure defines the GRIOMMU2BRIDGE register block memory map.
 */
typedef struct griommu2_bridge {
  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeCAP.
   */
  uint32_t cap;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeCTRL.
   */
  uint32_t ctrl;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeGCFG.
   */
  uint32_t gcfg;

  uint32_t reserved_c_100[ 61 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeDID.
   */
  uint32_t did[ 64 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeHPM.
   */
  griommu2_bhpm bhpmread;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeHPM.
   */
  griommu2_bhpm bhpmwrite;

  uint32_t reserved_240_260[ 8 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeHPM.
   */
  griommu2_bhpm bhpmbypass;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeHPM.
   */
  griommu2_bhpm bhpmerror;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeHPM.
   */
  griommu2_bhpm bhpmabort;

  uint32_t reserved_2c0_1000[ 848 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2BridgeHCONFIG.
   */
  uint32_t hconfig[ 512 ];
} griommu2_bridge;

/** @} */

/* Find related documentation with spec:/dev/grlib/if/griommu2-msicfg */

/**
 * @defgroup RTEMSDeviceGRIOMMU2MSIConfig GRIOMMU2MSICFG
 *
 * @ingroup RTEMSDeviceGRIOMMU2
 *
 * @brief This group contains the GRIOMMU2MSICFG interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRIOMMU2MSIConfigMSGADDR \
 *   MSI configuration table message address (MSGADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_MSICFG_MSGADDR_ADDR_SHIFT 2
#define GRIOMMU2_MSICFG_MSGADDR_ADDR_MASK  0xfffffffffffffcULL
#define GRIOMMU2_MSICFG_MSGADDR_ADDR_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_MSICFG_MSGADDR_ADDR_MASK ) >> \
    GRIOMMU2_MSICFG_MSGADDR_ADDR_SHIFT )
#define GRIOMMU2_MSICFG_MSGADDR_ADDR_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_MSICFG_MSGADDR_ADDR_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSICFG_MSGADDR_ADDR_SHIFT ) & \
      GRIOMMU2_MSICFG_MSGADDR_ADDR_MASK ) )
#define GRIOMMU2_MSICFG_MSGADDR_ADDR( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSICFG_MSGADDR_ADDR_SHIFT ) & \
    GRIOMMU2_MSICFG_MSGADDR_ADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2MSIConfigMSGDATA \
 *   MSI configuration table message data (MSGDATA)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_MSICFG_MSGDATA_DATA_SHIFT 0
#define GRIOMMU2_MSICFG_MSGDATA_DATA_MASK  0xffffffffU
#define GRIOMMU2_MSICFG_MSGDATA_DATA_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_MSICFG_MSGDATA_DATA_MASK ) >> \
    GRIOMMU2_MSICFG_MSGDATA_DATA_SHIFT )
#define GRIOMMU2_MSICFG_MSGDATA_DATA_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_MSICFG_MSGDATA_DATA_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_MSICFG_MSGDATA_DATA_SHIFT ) & \
      GRIOMMU2_MSICFG_MSGDATA_DATA_MASK ) )
#define GRIOMMU2_MSICFG_MSGDATA_DATA( _val )             \
  ( ( ( _val ) << GRIOMMU2_MSICFG_MSGDATA_DATA_SHIFT ) & \
    GRIOMMU2_MSICFG_MSGDATA_DATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2MSIConfigVECTCTRL \
 *   MSI configuration table vector control (VECTCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_MSICFG_VECTCTRL_M 0x1U

/** @} */

/**
 * @brief This structure defines the GRIOMMU2MSICFG register block memory map.
 */
typedef struct griommu2_msicfg {
  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2MSIConfigMSGADDR.
   */
  uint64_t msgaddr;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2MSIConfigMSGDATA.
   */
  uint32_t msgdata;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2MSIConfigVECTCTRL.
   */
  uint32_t vectctrl;
} griommu2_msicfg;

/** @} */

/* Find related documentation with spec:/dev/grlib/if/griommu2 */

/**
 * @defgroup RTEMSDeviceGRIOMMU2 GRIOMMU2
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the GRIOMMU2 interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CAP Capabilities (CAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CAP_PD20 0x10000000000ULL

#define GRIOMMU2_CAP_PD17 0x8000000000ULL

#define GRIOMMU2_CAP_PD8 0x4000000000ULL

#define GRIOMMU2_CAP_PAS_SHIFT 32
#define GRIOMMU2_CAP_PAS_MASK  0x3f00000000ULL
#define GRIOMMU2_CAP_PAS_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_CAP_PAS_MASK ) >> GRIOMMU2_CAP_PAS_SHIFT )
#define GRIOMMU2_CAP_PAS_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CAP_PAS_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CAP_PAS_SHIFT ) & \
      GRIOMMU2_CAP_PAS_MASK ) )
#define GRIOMMU2_CAP_PAS( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CAP_PAS_SHIFT ) & \
    GRIOMMU2_CAP_PAS_MASK )

#define GRIOMMU2_CAP_DBG 0x80000000U

#define GRIOMMU2_CAP_HPM 0x40000000U

#define GRIOMMU2_CAP_IGS_SHIFT 28
#define GRIOMMU2_CAP_IGS_MASK  0x30000000U
#define GRIOMMU2_CAP_IGS_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_CAP_IGS_MASK ) >> GRIOMMU2_CAP_IGS_SHIFT )
#define GRIOMMU2_CAP_IGS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_CAP_IGS_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_CAP_IGS_SHIFT ) & GRIOMMU2_CAP_IGS_MASK ) )
#define GRIOMMU2_CAP_IGS( _val ) \
  ( ( ( _val ) << GRIOMMU2_CAP_IGS_SHIFT ) & GRIOMMU2_CAP_IGS_MASK )
#define GRIOMMU2_CAP_IGS_MSI     0U
#define GRIOMMU2_CAP_IGS_WSI     1U
#define GRIOMMU2_CAP_IGS_MSI_WSI 2U

#define GRIOMMU2_CAP_END 0x8000000U

#define GRIOMMU2_CAP_T2GPA 0x4000000U

#define GRIOMMU2_CAP_ATS 0x2000000U

#define GRIOMMU2_CAP_AMO_HWAD 0x1000000U

#define GRIOMMU2_CAP_MSI_MRIF 0x800000U

#define GRIOMMU2_CAP_MSI_FLAT 0x400000U

#define GRIOMMU2_CAP_AMO_MRIF 0x200000U

#define GRIOMMU2_CAP_SV57X4 0x80000U

#define GRIOMMU2_CAP_SV48X4 0x40000U

#define GRIOMMU2_CAP_SV39X4 0x20000U

#define GRIOMMU2_CAP_SV32X4 0x10000U

#define GRIOMMU2_CAP_SVPBMT 0x8000U

#define GRIOMMU2_CAP_SVRSW60T59B 0x4000U

#define GRIOMMU2_CAP_SV57 0x800U

#define GRIOMMU2_CAP_SV48 0x400U

#define GRIOMMU2_CAP_SV39 0x200U

#define GRIOMMU2_CAP_SV32 0x100U

#define GRIOMMU2_CAP_VERSION_SHIFT 0
#define GRIOMMU2_CAP_VERSION_MASK  0xffU
#define GRIOMMU2_CAP_VERSION_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_CAP_VERSION_MASK ) >> GRIOMMU2_CAP_VERSION_SHIFT )
#define GRIOMMU2_CAP_VERSION_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_CAP_VERSION_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_CAP_VERSION_SHIFT ) & \
      GRIOMMU2_CAP_VERSION_MASK ) )
#define GRIOMMU2_CAP_VERSION( _val ) \
  ( ( ( _val ) << GRIOMMU2_CAP_VERSION_SHIFT ) & GRIOMMU2_CAP_VERSION_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2FCTL Feature control (FCTL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_FCTL_BPEN 0x200000U

#define GRIOMMU2_FCTL_MSIEN 0x100000U

#define GRIOMMU2_FCTL_GEN 0x80000U

#define GRIOMMU2_FCTL_SVEN 0x40000U

#define GRIOMMU2_FCTL_CTXEN 0x20000U

#define GRIOMMU2_FCTL_INVL 0x10000U

#define GRIOMMU2_FCTL_GXL 0x4U

#define GRIOMMU2_FCTL_WSI 0x2U

#define GRIOMMU2_FCTL_BE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2DDTP Device directory table pointer (DDTP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_DDTP_PPN_SHIFT 10
#define GRIOMMU2_DDTP_PPN_MASK  0x3ffffffffffc00ULL
#define GRIOMMU2_DDTP_PPN_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_DDTP_PPN_MASK ) >> GRIOMMU2_DDTP_PPN_SHIFT )
#define GRIOMMU2_DDTP_PPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_DDTP_PPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_DDTP_PPN_SHIFT ) & \
      GRIOMMU2_DDTP_PPN_MASK ) )
#define GRIOMMU2_DDTP_PPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_DDTP_PPN_SHIFT ) & \
    GRIOMMU2_DDTP_PPN_MASK )

#define GRIOMMU2_DDTP_BUSY 0x10U

#define GRIOMMU2_DDTP_MODE_SHIFT 0
#define GRIOMMU2_DDTP_MODE_MASK  0xfU
#define GRIOMMU2_DDTP_MODE_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_DDTP_MODE_MASK ) >> GRIOMMU2_DDTP_MODE_SHIFT )
#define GRIOMMU2_DDTP_MODE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_DDTP_MODE_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_DDTP_MODE_SHIFT ) & GRIOMMU2_DDTP_MODE_MASK ) )
#define GRIOMMU2_DDTP_MODE( _val ) \
  ( ( ( _val ) << GRIOMMU2_DDTP_MODE_SHIFT ) & GRIOMMU2_DDTP_MODE_MASK )
#define GRIOMMU2_DDTP_MODE_OFF     0U
#define GRIOMMU2_DDTP_MODE_BARE    1U
#define GRIOMMU2_DDTP_MODE_LEVEL_1 2U
#define GRIOMMU2_DDTP_MODE_LEVEL_2 3U
#define GRIOMMU2_DDTP_MODE_LEVEL_3 4U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CQB Command-queue base (CQB)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CQB_PPN_SHIFT 10
#define GRIOMMU2_CQB_PPN_MASK  0x3ffffffffffc00ULL
#define GRIOMMU2_CQB_PPN_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_CQB_PPN_MASK ) >> GRIOMMU2_CQB_PPN_SHIFT )
#define GRIOMMU2_CQB_PPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CQB_PPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CQB_PPN_SHIFT ) & \
      GRIOMMU2_CQB_PPN_MASK ) )
#define GRIOMMU2_CQB_PPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CQB_PPN_SHIFT ) & \
    GRIOMMU2_CQB_PPN_MASK )

#define GRIOMMU2_CQB_SIZE_SHIFT 0
#define GRIOMMU2_CQB_SIZE_MASK  0x1fU
#define GRIOMMU2_CQB_SIZE_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_CQB_SIZE_MASK ) >> GRIOMMU2_CQB_SIZE_SHIFT )
#define GRIOMMU2_CQB_SIZE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_CQB_SIZE_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_CQB_SIZE_SHIFT ) & GRIOMMU2_CQB_SIZE_MASK ) )
#define GRIOMMU2_CQB_SIZE( _val ) \
  ( ( ( _val ) << GRIOMMU2_CQB_SIZE_SHIFT ) & GRIOMMU2_CQB_SIZE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CQH Command-queue head (CQH)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CQH_INDEX_SHIFT 0
#define GRIOMMU2_CQH_INDEX_MASK  0xffffffffU
#define GRIOMMU2_CQH_INDEX_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_CQH_INDEX_MASK ) >> GRIOMMU2_CQH_INDEX_SHIFT )
#define GRIOMMU2_CQH_INDEX_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_CQH_INDEX_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_CQH_INDEX_SHIFT ) & GRIOMMU2_CQH_INDEX_MASK ) )
#define GRIOMMU2_CQH_INDEX( _val ) \
  ( ( ( _val ) << GRIOMMU2_CQH_INDEX_SHIFT ) & GRIOMMU2_CQH_INDEX_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CQT Command-queue tail (CQT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CQT_INDEX_SHIFT 0
#define GRIOMMU2_CQT_INDEX_MASK  0xffffffffU
#define GRIOMMU2_CQT_INDEX_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_CQT_INDEX_MASK ) >> GRIOMMU2_CQT_INDEX_SHIFT )
#define GRIOMMU2_CQT_INDEX_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_CQT_INDEX_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_CQT_INDEX_SHIFT ) & GRIOMMU2_CQT_INDEX_MASK ) )
#define GRIOMMU2_CQT_INDEX( _val ) \
  ( ( ( _val ) << GRIOMMU2_CQT_INDEX_SHIFT ) & GRIOMMU2_CQT_INDEX_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2FQB Fault-queue base (FQB)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_FQB_PPN_SHIFT 10
#define GRIOMMU2_FQB_PPN_MASK  0x3ffffffffffc00ULL
#define GRIOMMU2_FQB_PPN_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_FQB_PPN_MASK ) >> GRIOMMU2_FQB_PPN_SHIFT )
#define GRIOMMU2_FQB_PPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_FQB_PPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_FQB_PPN_SHIFT ) & \
      GRIOMMU2_FQB_PPN_MASK ) )
#define GRIOMMU2_FQB_PPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_FQB_PPN_SHIFT ) & \
    GRIOMMU2_FQB_PPN_MASK )

#define GRIOMMU2_FQB_SIZE_SHIFT 0
#define GRIOMMU2_FQB_SIZE_MASK  0x1fU
#define GRIOMMU2_FQB_SIZE_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_FQB_SIZE_MASK ) >> GRIOMMU2_FQB_SIZE_SHIFT )
#define GRIOMMU2_FQB_SIZE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_FQB_SIZE_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_FQB_SIZE_SHIFT ) & GRIOMMU2_FQB_SIZE_MASK ) )
#define GRIOMMU2_FQB_SIZE( _val ) \
  ( ( ( _val ) << GRIOMMU2_FQB_SIZE_SHIFT ) & GRIOMMU2_FQB_SIZE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2FQH Fault-queue head (FQH)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_FQH_INDEX_SHIFT 0
#define GRIOMMU2_FQH_INDEX_MASK  0xffffffffU
#define GRIOMMU2_FQH_INDEX_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_FQH_INDEX_MASK ) >> GRIOMMU2_FQH_INDEX_SHIFT )
#define GRIOMMU2_FQH_INDEX_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_FQH_INDEX_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_FQH_INDEX_SHIFT ) & GRIOMMU2_FQH_INDEX_MASK ) )
#define GRIOMMU2_FQH_INDEX( _val ) \
  ( ( ( _val ) << GRIOMMU2_FQH_INDEX_SHIFT ) & GRIOMMU2_FQH_INDEX_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2FQT Fault-queue tail (FQT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_FQT_INDEX_SHIFT 0
#define GRIOMMU2_FQT_INDEX_MASK  0xffffffffU
#define GRIOMMU2_FQT_INDEX_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_FQT_INDEX_MASK ) >> GRIOMMU2_FQT_INDEX_SHIFT )
#define GRIOMMU2_FQT_INDEX_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_FQT_INDEX_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_FQT_INDEX_SHIFT ) & GRIOMMU2_FQT_INDEX_MASK ) )
#define GRIOMMU2_FQT_INDEX( _val ) \
  ( ( ( _val ) << GRIOMMU2_FQT_INDEX_SHIFT ) & GRIOMMU2_FQT_INDEX_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CQCSR Command-queue CSR (CQCSR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CQCSR_BUSY 0x20000U

#define GRIOMMU2_CQCSR_CQON 0x10000U

#define GRIOMMU2_CQCSR_FWIP 0x800U

#define GRIOMMU2_CQCSR_CMD_ILL 0x400U

#define GRIOMMU2_CQCSR_CMD_TO 0x200U

#define GRIOMMU2_CQCSR_CQMF 0x100U

#define GRIOMMU2_CQCSR_CIE 0x2U

#define GRIOMMU2_CQCSR_CQEN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2FQCSR Fault-queue CSR (FQCSR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_FQCSR_BUSY 0x20000U

#define GRIOMMU2_FQCSR_FQON 0x10000U

#define GRIOMMU2_FQCSR_FQOF 0x200U

#define GRIOMMU2_FQCSR_FQMF 0x100U

#define GRIOMMU2_FQCSR_FIE 0x2U

#define GRIOMMU2_FQCSR_FQEN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2IPSR Interrupt pending status register (IPSR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_IPSR_PIP 0x8U

#define GRIOMMU2_IPSR_PMIP 0x4U

#define GRIOMMU2_IPSR_FIP 0x2U

#define GRIOMMU2_IPSR_CIP 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2IOCOUNTOVF \
 *   Performance-monitoring counter overflow status (IOCOUNTOVF)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_IOCOUNTOVF_HPM_SHIFT 1
#define GRIOMMU2_IOCOUNTOVF_HPM_MASK  0xfffffffeU
#define GRIOMMU2_IOCOUNTOVF_HPM_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_IOCOUNTOVF_HPM_MASK ) >> \
    GRIOMMU2_IOCOUNTOVF_HPM_SHIFT )
#define GRIOMMU2_IOCOUNTOVF_HPM_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_IOCOUNTOVF_HPM_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_IOCOUNTOVF_HPM_SHIFT ) & \
      GRIOMMU2_IOCOUNTOVF_HPM_MASK ) )
#define GRIOMMU2_IOCOUNTOVF_HPM( _val )             \
  ( ( ( _val ) << GRIOMMU2_IOCOUNTOVF_HPM_SHIFT ) & \
    GRIOMMU2_IOCOUNTOVF_HPM_MASK )

#define GRIOMMU2_IOCOUNTOVF_CY 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2IOCOUNTINH \
 *   Performance-monitoring counter inhibits (IOCOUNTINH)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_IOCOUNTINH_HPM_SHIFT 1
#define GRIOMMU2_IOCOUNTINH_HPM_MASK  0xfffffffeU
#define GRIOMMU2_IOCOUNTINH_HPM_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_IOCOUNTINH_HPM_MASK ) >> \
    GRIOMMU2_IOCOUNTINH_HPM_SHIFT )
#define GRIOMMU2_IOCOUNTINH_HPM_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_IOCOUNTINH_HPM_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_IOCOUNTINH_HPM_SHIFT ) & \
      GRIOMMU2_IOCOUNTINH_HPM_MASK ) )
#define GRIOMMU2_IOCOUNTINH_HPM( _val )             \
  ( ( ( _val ) << GRIOMMU2_IOCOUNTINH_HPM_SHIFT ) & \
    GRIOMMU2_IOCOUNTINH_HPM_MASK )

#define GRIOMMU2_IOCOUNTINH_CY 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2IOHPMCYCLES \
 *   Performance-monitoring cycles counter (IOHPMCYCLES)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_IOHPMCYCLES_OF 0x8000000000000000ULL

#define GRIOMMU2_IOHPMCYCLES_COUNTER_SHIFT 0
#define GRIOMMU2_IOHPMCYCLES_COUNTER_MASK  0x7fffffffffffffffULL
#define GRIOMMU2_IOHPMCYCLES_COUNTER_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_IOHPMCYCLES_COUNTER_MASK ) >> \
    GRIOMMU2_IOHPMCYCLES_COUNTER_SHIFT )
#define GRIOMMU2_IOHPMCYCLES_COUNTER_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_IOHPMCYCLES_COUNTER_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_IOHPMCYCLES_COUNTER_SHIFT ) & \
      GRIOMMU2_IOHPMCYCLES_COUNTER_MASK ) )
#define GRIOMMU2_IOHPMCYCLES_COUNTER( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_IOHPMCYCLES_COUNTER_SHIFT ) & \
    GRIOMMU2_IOHPMCYCLES_COUNTER_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2IOHPMCTR \
 *   Performance-monitoring event counter (IOHPMCTR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_IOHPMCTR_COUNTER_SHIFT 0
#define GRIOMMU2_IOHPMCTR_COUNTER_MASK  0xffffffffffffffffULL
#define GRIOMMU2_IOHPMCTR_COUNTER_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_IOHPMCTR_COUNTER_MASK ) >> \
    GRIOMMU2_IOHPMCTR_COUNTER_SHIFT )
#define GRIOMMU2_IOHPMCTR_COUNTER_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_IOHPMCTR_COUNTER_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_IOHPMCTR_COUNTER_SHIFT ) & \
      GRIOMMU2_IOHPMCTR_COUNTER_MASK ) )
#define GRIOMMU2_IOHPMCTR_COUNTER( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_IOHPMCTR_COUNTER_SHIFT ) & \
    GRIOMMU2_IOHPMCTR_COUNTER_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2IOHPMEVT \
 *   Performance-monitoring event selector (IOHPMEVT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_IOHPMEVT_OF 0x8000000000000000ULL

#define GRIOMMU2_IOHPMEVT_IDT 0x4000000000000000ULL

#define GRIOMMU2_IOHPMEVT_DV_GSCV 0x2000000000000000ULL

#define GRIOMMU2_IOHPMEVT_PV_PSCV 0x1000000000000000ULL

#define GRIOMMU2_IOHPMEVT_DID_GSCID_SHIFT 36
#define GRIOMMU2_IOHPMEVT_DID_GSCID_MASK  0xffffff000000000ULL
#define GRIOMMU2_IOHPMEVT_DID_GSCID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_IOHPMEVT_DID_GSCID_MASK ) >> \
    GRIOMMU2_IOHPMEVT_DID_GSCID_SHIFT )
#define GRIOMMU2_IOHPMEVT_DID_GSCID_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_IOHPMEVT_DID_GSCID_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_IOHPMEVT_DID_GSCID_SHIFT ) & \
      GRIOMMU2_IOHPMEVT_DID_GSCID_MASK ) )
#define GRIOMMU2_IOHPMEVT_DID_GSCID( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_IOHPMEVT_DID_GSCID_SHIFT ) & \
    GRIOMMU2_IOHPMEVT_DID_GSCID_MASK )

#define GRIOMMU2_IOHPMEVT_PID_PSCID_SHIFT 16
#define GRIOMMU2_IOHPMEVT_PID_PSCID_MASK  0xfffff0000ULL
#define GRIOMMU2_IOHPMEVT_PID_PSCID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_IOHPMEVT_PID_PSCID_MASK ) >> \
    GRIOMMU2_IOHPMEVT_PID_PSCID_SHIFT )
#define GRIOMMU2_IOHPMEVT_PID_PSCID_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_IOHPMEVT_PID_PSCID_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_IOHPMEVT_PID_PSCID_SHIFT ) & \
      GRIOMMU2_IOHPMEVT_PID_PSCID_MASK ) )
#define GRIOMMU2_IOHPMEVT_PID_PSCID( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_IOHPMEVT_PID_PSCID_SHIFT ) & \
    GRIOMMU2_IOHPMEVT_PID_PSCID_MASK )

#define GRIOMMU2_IOHPMEVT_DMASK 0x8000U

#define GRIOMMU2_IOHPMEVT_EVENTID_SHIFT 0
#define GRIOMMU2_IOHPMEVT_EVENTID_MASK  0x7fffU
#define GRIOMMU2_IOHPMEVT_EVENTID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_IOHPMEVT_EVENTID_MASK ) >> \
    GRIOMMU2_IOHPMEVT_EVENTID_SHIFT )
#define GRIOMMU2_IOHPMEVT_EVENTID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_IOHPMEVT_EVENTID_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_IOHPMEVT_EVENTID_SHIFT ) & \
      GRIOMMU2_IOHPMEVT_EVENTID_MASK ) )
#define GRIOMMU2_IOHPMEVT_EVENTID( _val )             \
  ( ( ( _val ) << GRIOMMU2_IOHPMEVT_EVENTID_SHIFT ) & \
    GRIOMMU2_IOHPMEVT_EVENTID_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2TRREQIOVA \
 *   Translation-request IOVA (TR_REQ_IOVA)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_TR_REQ_IOVA_VPN_SHIFT 12
#define GRIOMMU2_TR_REQ_IOVA_VPN_MASK  0xfffffffffffff000ULL
#define GRIOMMU2_TR_REQ_IOVA_VPN_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_TR_REQ_IOVA_VPN_MASK ) >> \
    GRIOMMU2_TR_REQ_IOVA_VPN_SHIFT )
#define GRIOMMU2_TR_REQ_IOVA_VPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_TR_REQ_IOVA_VPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_TR_REQ_IOVA_VPN_SHIFT ) & \
      GRIOMMU2_TR_REQ_IOVA_VPN_MASK ) )
#define GRIOMMU2_TR_REQ_IOVA_VPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_TR_REQ_IOVA_VPN_SHIFT ) & \
    GRIOMMU2_TR_REQ_IOVA_VPN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2TRREQCTL \
 *   Translation-request control (TR_REQ_CTL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_TR_REQ_CTL_DID_SHIFT 40
#define GRIOMMU2_TR_REQ_CTL_DID_MASK  0xffffff0000000000ULL
#define GRIOMMU2_TR_REQ_CTL_DID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_TR_REQ_CTL_DID_MASK ) >> \
    GRIOMMU2_TR_REQ_CTL_DID_SHIFT )
#define GRIOMMU2_TR_REQ_CTL_DID_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_TR_REQ_CTL_DID_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_TR_REQ_CTL_DID_SHIFT ) & \
      GRIOMMU2_TR_REQ_CTL_DID_MASK ) )
#define GRIOMMU2_TR_REQ_CTL_DID( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_TR_REQ_CTL_DID_SHIFT ) & \
    GRIOMMU2_TR_REQ_CTL_DID_MASK )

#define GRIOMMU2_TR_REQ_CTL_CACHE 0x1000000000ULL

#define GRIOMMU2_TR_REQ_CTL_PV 0x100000000ULL

#define GRIOMMU2_TR_REQ_CTL_PID_SHIFT 12
#define GRIOMMU2_TR_REQ_CTL_PID_MASK  0xfffff000U
#define GRIOMMU2_TR_REQ_CTL_PID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_TR_REQ_CTL_PID_MASK ) >> \
    GRIOMMU2_TR_REQ_CTL_PID_SHIFT )
#define GRIOMMU2_TR_REQ_CTL_PID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_TR_REQ_CTL_PID_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_TR_REQ_CTL_PID_SHIFT ) & \
      GRIOMMU2_TR_REQ_CTL_PID_MASK ) )
#define GRIOMMU2_TR_REQ_CTL_PID( _val )             \
  ( ( ( _val ) << GRIOMMU2_TR_REQ_CTL_PID_SHIFT ) & \
    GRIOMMU2_TR_REQ_CTL_PID_MASK )

#define GRIOMMU2_TR_REQ_CTL_NW 0x8U

#define GRIOMMU2_TR_REQ_CTL_EXE 0x4U

#define GRIOMMU2_TR_REQ_CTL_PRIV 0x2U

#define GRIOMMU2_TR_REQ_CTL_GO 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2TRRESPONSE Translation-response (TR_RESPONSE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_TR_RESPONSE_PPN_SHIFT 10
#define GRIOMMU2_TR_RESPONSE_PPN_MASK  0x3ffffffffffc00ULL
#define GRIOMMU2_TR_RESPONSE_PPN_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_TR_RESPONSE_PPN_MASK ) >> \
    GRIOMMU2_TR_RESPONSE_PPN_SHIFT )
#define GRIOMMU2_TR_RESPONSE_PPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_TR_RESPONSE_PPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_TR_RESPONSE_PPN_SHIFT ) & \
      GRIOMMU2_TR_RESPONSE_PPN_MASK ) )
#define GRIOMMU2_TR_RESPONSE_PPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_TR_RESPONSE_PPN_SHIFT ) & \
    GRIOMMU2_TR_RESPONSE_PPN_MASK )

#define GRIOMMU2_TR_RESPONSE_S 0x200U

#define GRIOMMU2_TR_RESPONSE_PBMT_SHIFT 7
#define GRIOMMU2_TR_RESPONSE_PBMT_MASK  0x180U
#define GRIOMMU2_TR_RESPONSE_PBMT_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_TR_RESPONSE_PBMT_MASK ) >> \
    GRIOMMU2_TR_RESPONSE_PBMT_SHIFT )
#define GRIOMMU2_TR_RESPONSE_PBMT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_TR_RESPONSE_PBMT_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_TR_RESPONSE_PBMT_SHIFT ) & \
      GRIOMMU2_TR_RESPONSE_PBMT_MASK ) )
#define GRIOMMU2_TR_RESPONSE_PBMT( _val )             \
  ( ( ( _val ) << GRIOMMU2_TR_RESPONSE_PBMT_SHIFT ) & \
    GRIOMMU2_TR_RESPONSE_PBMT_MASK )

#define GRIOMMU2_TR_RESPONSE_FAULT 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2RCMD0 In-register command 0 (RCMD0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_RCMD0_CMD_SHIFT 0
#define GRIOMMU2_RCMD0_CMD_MASK  0xffffffffffffffffULL
#define GRIOMMU2_RCMD0_CMD_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_RCMD0_CMD_MASK ) >> GRIOMMU2_RCMD0_CMD_SHIFT )
#define GRIOMMU2_RCMD0_CMD_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_RCMD0_CMD_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_RCMD0_CMD_SHIFT ) & \
      GRIOMMU2_RCMD0_CMD_MASK ) )
#define GRIOMMU2_RCMD0_CMD( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_RCMD0_CMD_SHIFT ) & \
    GRIOMMU2_RCMD0_CMD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2RCMD1 In-register command 1 (RCMD1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_RCMD1_CMD_SHIFT 0
#define GRIOMMU2_RCMD1_CMD_MASK  0xffffffffffffffffULL
#define GRIOMMU2_RCMD1_CMD_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_RCMD1_CMD_MASK ) >> GRIOMMU2_RCMD1_CMD_SHIFT )
#define GRIOMMU2_RCMD1_CMD_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_RCMD1_CMD_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_RCMD1_CMD_SHIFT ) & \
      GRIOMMU2_RCMD1_CMD_MASK ) )
#define GRIOMMU2_RCMD1_CMD( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_RCMD1_CMD_SHIFT ) & \
    GRIOMMU2_RCMD1_CMD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2ICVEC \
 *   Interrupt-cause-to-vector register (ICVEC)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_ICVEC_PIV_SHIFT 12
#define GRIOMMU2_ICVEC_PIV_MASK  0xf000U
#define GRIOMMU2_ICVEC_PIV_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_ICVEC_PIV_MASK ) >> GRIOMMU2_ICVEC_PIV_SHIFT )
#define GRIOMMU2_ICVEC_PIV_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_ICVEC_PIV_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_ICVEC_PIV_SHIFT ) & GRIOMMU2_ICVEC_PIV_MASK ) )
#define GRIOMMU2_ICVEC_PIV( _val ) \
  ( ( ( _val ) << GRIOMMU2_ICVEC_PIV_SHIFT ) & GRIOMMU2_ICVEC_PIV_MASK )

#define GRIOMMU2_ICVEC_PMIV_SHIFT 8
#define GRIOMMU2_ICVEC_PMIV_MASK  0xf00U
#define GRIOMMU2_ICVEC_PMIV_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_ICVEC_PMIV_MASK ) >> GRIOMMU2_ICVEC_PMIV_SHIFT )
#define GRIOMMU2_ICVEC_PMIV_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_ICVEC_PMIV_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_ICVEC_PMIV_SHIFT ) & \
      GRIOMMU2_ICVEC_PMIV_MASK ) )
#define GRIOMMU2_ICVEC_PMIV( _val ) \
  ( ( ( _val ) << GRIOMMU2_ICVEC_PMIV_SHIFT ) & GRIOMMU2_ICVEC_PMIV_MASK )

#define GRIOMMU2_ICVEC_FIV_SHIFT 4
#define GRIOMMU2_ICVEC_FIV_MASK  0xf0U
#define GRIOMMU2_ICVEC_FIV_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_ICVEC_FIV_MASK ) >> GRIOMMU2_ICVEC_FIV_SHIFT )
#define GRIOMMU2_ICVEC_FIV_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_ICVEC_FIV_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_ICVEC_FIV_SHIFT ) & GRIOMMU2_ICVEC_FIV_MASK ) )
#define GRIOMMU2_ICVEC_FIV( _val ) \
  ( ( ( _val ) << GRIOMMU2_ICVEC_FIV_SHIFT ) & GRIOMMU2_ICVEC_FIV_MASK )

#define GRIOMMU2_ICVEC_CIV_SHIFT 0
#define GRIOMMU2_ICVEC_CIV_MASK  0xfU
#define GRIOMMU2_ICVEC_CIV_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_ICVEC_CIV_MASK ) >> GRIOMMU2_ICVEC_CIV_SHIFT )
#define GRIOMMU2_ICVEC_CIV_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRIOMMU2_ICVEC_CIV_MASK ) | \
    ( ( ( _val ) << GRIOMMU2_ICVEC_CIV_SHIFT ) & GRIOMMU2_ICVEC_CIV_MASK ) )
#define GRIOMMU2_ICVEC_CIV( _val ) \
  ( ( ( _val ) << GRIOMMU2_ICVEC_CIV_SHIFT ) & GRIOMMU2_ICVEC_CIV_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CTXINDEX CTX TLB index selector (CTX_INDEX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CTX_INDEX_NUM_CTX_SHIFT 16
#define GRIOMMU2_CTX_INDEX_NUM_CTX_MASK  0xffff0000U
#define GRIOMMU2_CTX_INDEX_NUM_CTX_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_INDEX_NUM_CTX_MASK ) >> \
    GRIOMMU2_CTX_INDEX_NUM_CTX_SHIFT )
#define GRIOMMU2_CTX_INDEX_NUM_CTX_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_INDEX_NUM_CTX_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_CTX_INDEX_NUM_CTX_SHIFT ) & \
      GRIOMMU2_CTX_INDEX_NUM_CTX_MASK ) )
#define GRIOMMU2_CTX_INDEX_NUM_CTX( _val )             \
  ( ( ( _val ) << GRIOMMU2_CTX_INDEX_NUM_CTX_SHIFT ) & \
    GRIOMMU2_CTX_INDEX_NUM_CTX_MASK )

#define GRIOMMU2_CTX_INDEX_IND_SHIFT 0
#define GRIOMMU2_CTX_INDEX_IND_MASK  0xffffU
#define GRIOMMU2_CTX_INDEX_IND_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_INDEX_IND_MASK ) >> \
    GRIOMMU2_CTX_INDEX_IND_SHIFT )
#define GRIOMMU2_CTX_INDEX_IND_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_INDEX_IND_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_CTX_INDEX_IND_SHIFT ) & \
      GRIOMMU2_CTX_INDEX_IND_MASK ) )
#define GRIOMMU2_CTX_INDEX_IND( _val )             \
  ( ( ( _val ) << GRIOMMU2_CTX_INDEX_IND_SHIFT ) & \
    GRIOMMU2_CTX_INDEX_IND_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CTXDIAG0 \
 *   CTX TLB diagnostic interface, word 0 (CTX_DIAG0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CTX_DIAG0_LCK 0x8000000000000000ULL

#define GRIOMMU2_CTX_DIAG0_PIDV 0x200000000000ULL

#define GRIOMMU2_CTX_DIAG0_PID_SHIFT 25
#define GRIOMMU2_CTX_DIAG0_PID_MASK  0x1ffffe000000ULL
#define GRIOMMU2_CTX_DIAG0_PID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG0_PID_MASK ) >> \
    GRIOMMU2_CTX_DIAG0_PID_SHIFT )
#define GRIOMMU2_CTX_DIAG0_PID_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG0_PID_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG0_PID_SHIFT ) & \
      GRIOMMU2_CTX_DIAG0_PID_MASK ) )
#define GRIOMMU2_CTX_DIAG0_PID( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG0_PID_SHIFT ) & \
    GRIOMMU2_CTX_DIAG0_PID_MASK )

#define GRIOMMU2_CTX_DIAG0_DID_SHIFT 1
#define GRIOMMU2_CTX_DIAG0_DID_MASK  0x1fffffeU
#define GRIOMMU2_CTX_DIAG0_DID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG0_DID_MASK ) >> \
    GRIOMMU2_CTX_DIAG0_DID_SHIFT )
#define GRIOMMU2_CTX_DIAG0_DID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG0_DID_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_CTX_DIAG0_DID_SHIFT ) & \
      GRIOMMU2_CTX_DIAG0_DID_MASK ) )
#define GRIOMMU2_CTX_DIAG0_DID( _val )             \
  ( ( ( _val ) << GRIOMMU2_CTX_DIAG0_DID_SHIFT ) & \
    GRIOMMU2_CTX_DIAG0_DID_MASK )

#define GRIOMMU2_CTX_DIAG0_V 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CTXDIAG1 \
 *   CTX TLB diagnostic interface, word 1 (CTX_DIAG1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CTX_DIAG1_SATP_M_SHIFT 44
#define GRIOMMU2_CTX_DIAG1_SATP_M_MASK  0xf00000000000ULL
#define GRIOMMU2_CTX_DIAG1_SATP_M_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG1_SATP_M_MASK ) >> \
    GRIOMMU2_CTX_DIAG1_SATP_M_SHIFT )
#define GRIOMMU2_CTX_DIAG1_SATP_M_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG1_SATP_M_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG1_SATP_M_SHIFT ) & \
      GRIOMMU2_CTX_DIAG1_SATP_M_MASK ) )
#define GRIOMMU2_CTX_DIAG1_SATP_M( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG1_SATP_M_SHIFT ) & \
    GRIOMMU2_CTX_DIAG1_SATP_M_MASK )

#define GRIOMMU2_CTX_DIAG1_SATP_PPN_SHIFT 0
#define GRIOMMU2_CTX_DIAG1_SATP_PPN_MASK  0xfffffffffffULL
#define GRIOMMU2_CTX_DIAG1_SATP_PPN_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG1_SATP_PPN_MASK ) >> \
    GRIOMMU2_CTX_DIAG1_SATP_PPN_SHIFT )
#define GRIOMMU2_CTX_DIAG1_SATP_PPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG1_SATP_PPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG1_SATP_PPN_SHIFT ) & \
      GRIOMMU2_CTX_DIAG1_SATP_PPN_MASK ) )
#define GRIOMMU2_CTX_DIAG1_SATP_PPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG1_SATP_PPN_SHIFT ) & \
    GRIOMMU2_CTX_DIAG1_SATP_PPN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CTXDIAG2 \
 *   CTX TLB diagnostic interface, word 2 (CTX_DIAG2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CTX_DIAG2_GATP_M_SHIFT 44
#define GRIOMMU2_CTX_DIAG2_GATP_M_MASK  0xf00000000000ULL
#define GRIOMMU2_CTX_DIAG2_GATP_M_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG2_GATP_M_MASK ) >> \
    GRIOMMU2_CTX_DIAG2_GATP_M_SHIFT )
#define GRIOMMU2_CTX_DIAG2_GATP_M_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG2_GATP_M_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG2_GATP_M_SHIFT ) & \
      GRIOMMU2_CTX_DIAG2_GATP_M_MASK ) )
#define GRIOMMU2_CTX_DIAG2_GATP_M( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG2_GATP_M_SHIFT ) & \
    GRIOMMU2_CTX_DIAG2_GATP_M_MASK )

#define GRIOMMU2_CTX_DIAG2_GATP_PPN_SHIFT 0
#define GRIOMMU2_CTX_DIAG2_GATP_PPN_MASK  0xfffffffffffULL
#define GRIOMMU2_CTX_DIAG2_GATP_PPN_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG2_GATP_PPN_MASK ) >> \
    GRIOMMU2_CTX_DIAG2_GATP_PPN_SHIFT )
#define GRIOMMU2_CTX_DIAG2_GATP_PPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG2_GATP_PPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG2_GATP_PPN_SHIFT ) & \
      GRIOMMU2_CTX_DIAG2_GATP_PPN_MASK ) )
#define GRIOMMU2_CTX_DIAG2_GATP_PPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG2_GATP_PPN_SHIFT ) & \
    GRIOMMU2_CTX_DIAG2_GATP_PPN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CTXDIAG3 \
 *   CTX TLB diagnostic interface, word 3 (CTX_DIAG3)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CTX_DIAG3_GADE 0x20000000000ULL

#define GRIOMMU2_CTX_DIAG3_SADE 0x10000000000ULL

#define GRIOMMU2_CTX_DIAG3_DTF 0x8000000000ULL

#define GRIOMMU2_CTX_DIAG3_BYP 0x4000000000ULL

#define GRIOMMU2_CTX_DIAG3_SUM 0x2000000000ULL

#define GRIOMMU2_CTX_DIAG3_ENS 0x1000000000ULL

#define GRIOMMU2_CTX_DIAG3_GSCID_SHIFT 20
#define GRIOMMU2_CTX_DIAG3_GSCID_MASK  0xffff00000ULL
#define GRIOMMU2_CTX_DIAG3_GSCID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG3_GSCID_MASK ) >> \
    GRIOMMU2_CTX_DIAG3_GSCID_SHIFT )
#define GRIOMMU2_CTX_DIAG3_GSCID_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG3_GSCID_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG3_GSCID_SHIFT ) & \
      GRIOMMU2_CTX_DIAG3_GSCID_MASK ) )
#define GRIOMMU2_CTX_DIAG3_GSCID( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG3_GSCID_SHIFT ) & \
    GRIOMMU2_CTX_DIAG3_GSCID_MASK )

#define GRIOMMU2_CTX_DIAG3_PSCID_SHIFT 0
#define GRIOMMU2_CTX_DIAG3_PSCID_MASK  0xfffffU
#define GRIOMMU2_CTX_DIAG3_PSCID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG3_PSCID_MASK ) >> \
    GRIOMMU2_CTX_DIAG3_PSCID_SHIFT )
#define GRIOMMU2_CTX_DIAG3_PSCID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG3_PSCID_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_CTX_DIAG3_PSCID_SHIFT ) & \
      GRIOMMU2_CTX_DIAG3_PSCID_MASK ) )
#define GRIOMMU2_CTX_DIAG3_PSCID( _val )             \
  ( ( ( _val ) << GRIOMMU2_CTX_DIAG3_PSCID_SHIFT ) & \
    GRIOMMU2_CTX_DIAG3_PSCID_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CTXDIAG4 \
 *   CTX TLB diagnostic interface, word 4 (CTX_DIAG4)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CTX_DIAG4_MSI_M_SHIFT 44
#define GRIOMMU2_CTX_DIAG4_MSI_M_MASK  0xf00000000000ULL
#define GRIOMMU2_CTX_DIAG4_MSI_M_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG4_MSI_M_MASK ) >> \
    GRIOMMU2_CTX_DIAG4_MSI_M_SHIFT )
#define GRIOMMU2_CTX_DIAG4_MSI_M_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG4_MSI_M_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG4_MSI_M_SHIFT ) & \
      GRIOMMU2_CTX_DIAG4_MSI_M_MASK ) )
#define GRIOMMU2_CTX_DIAG4_MSI_M( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG4_MSI_M_SHIFT ) & \
    GRIOMMU2_CTX_DIAG4_MSI_M_MASK )

#define GRIOMMU2_CTX_DIAG4_MSI_PPN_SHIFT 0
#define GRIOMMU2_CTX_DIAG4_MSI_PPN_MASK  0xfffffffffffULL
#define GRIOMMU2_CTX_DIAG4_MSI_PPN_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG4_MSI_PPN_MASK ) >> \
    GRIOMMU2_CTX_DIAG4_MSI_PPN_SHIFT )
#define GRIOMMU2_CTX_DIAG4_MSI_PPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG4_MSI_PPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG4_MSI_PPN_SHIFT ) & \
      GRIOMMU2_CTX_DIAG4_MSI_PPN_MASK ) )
#define GRIOMMU2_CTX_DIAG4_MSI_PPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG4_MSI_PPN_SHIFT ) & \
    GRIOMMU2_CTX_DIAG4_MSI_PPN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CTXDIAG5 \
 *   CTX TLB diagnostic interface, word 5 (CTX_DIAG5)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CTX_DIAG5_MSI_MSK_SHIFT 0
#define GRIOMMU2_CTX_DIAG5_MSI_MSK_MASK  0xfffffffffffffULL
#define GRIOMMU2_CTX_DIAG5_MSI_MSK_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG5_MSI_MSK_MASK ) >> \
    GRIOMMU2_CTX_DIAG5_MSI_MSK_SHIFT )
#define GRIOMMU2_CTX_DIAG5_MSI_MSK_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG5_MSI_MSK_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG5_MSI_MSK_SHIFT ) & \
      GRIOMMU2_CTX_DIAG5_MSI_MSK_MASK ) )
#define GRIOMMU2_CTX_DIAG5_MSI_MSK( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG5_MSI_MSK_SHIFT ) & \
    GRIOMMU2_CTX_DIAG5_MSI_MSK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2CTXDIAG6 \
 *   CTX TLB diagnostic interface, word 6 (CTX_DIAG6)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_CTX_DIAG6_MSI_PAT_SHIFT 0
#define GRIOMMU2_CTX_DIAG6_MSI_PAT_MASK  0xfffffffffffffULL
#define GRIOMMU2_CTX_DIAG6_MSI_PAT_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_CTX_DIAG6_MSI_PAT_MASK ) >> \
    GRIOMMU2_CTX_DIAG6_MSI_PAT_SHIFT )
#define GRIOMMU2_CTX_DIAG6_MSI_PAT_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_CTX_DIAG6_MSI_PAT_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG6_MSI_PAT_SHIFT ) & \
      GRIOMMU2_CTX_DIAG6_MSI_PAT_MASK ) )
#define GRIOMMU2_CTX_DIAG6_MSI_PAT( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_CTX_DIAG6_MSI_PAT_SHIFT ) & \
    GRIOMMU2_CTX_DIAG6_MSI_PAT_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2SVINDEX SV-stage TLB index selector (SV_INDEX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_SV_INDEX_NUM_SV_SHIFT 16
#define GRIOMMU2_SV_INDEX_NUM_SV_MASK  0xffff0000U
#define GRIOMMU2_SV_INDEX_NUM_SV_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_SV_INDEX_NUM_SV_MASK ) >> \
    GRIOMMU2_SV_INDEX_NUM_SV_SHIFT )
#define GRIOMMU2_SV_INDEX_NUM_SV_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_SV_INDEX_NUM_SV_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_SV_INDEX_NUM_SV_SHIFT ) & \
      GRIOMMU2_SV_INDEX_NUM_SV_MASK ) )
#define GRIOMMU2_SV_INDEX_NUM_SV( _val )             \
  ( ( ( _val ) << GRIOMMU2_SV_INDEX_NUM_SV_SHIFT ) & \
    GRIOMMU2_SV_INDEX_NUM_SV_MASK )

#define GRIOMMU2_SV_INDEX_IND_SHIFT 0
#define GRIOMMU2_SV_INDEX_IND_MASK  0xffffU
#define GRIOMMU2_SV_INDEX_IND_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_SV_INDEX_IND_MASK ) >> GRIOMMU2_SV_INDEX_IND_SHIFT )
#define GRIOMMU2_SV_INDEX_IND_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_SV_INDEX_IND_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_SV_INDEX_IND_SHIFT ) & \
      GRIOMMU2_SV_INDEX_IND_MASK ) )
#define GRIOMMU2_SV_INDEX_IND( _val ) \
  ( ( ( _val ) << GRIOMMU2_SV_INDEX_IND_SHIFT ) & GRIOMMU2_SV_INDEX_IND_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2SVDIAG0 \
 *   SV-stage TLB diagnostic interface, word 0 (SV_DIAG0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_SV_DIAG0_LCK 0x8000000000000000ULL

#define GRIOMMU2_SV_DIAG0_PPN_SHIFT 2
#define GRIOMMU2_SV_DIAG0_PPN_MASK  0x3ffffffffffcULL
#define GRIOMMU2_SV_DIAG0_PPN_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_SV_DIAG0_PPN_MASK ) >> GRIOMMU2_SV_DIAG0_PPN_SHIFT )
#define GRIOMMU2_SV_DIAG0_PPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_SV_DIAG0_PPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_SV_DIAG0_PPN_SHIFT ) & \
      GRIOMMU2_SV_DIAG0_PPN_MASK ) )
#define GRIOMMU2_SV_DIAG0_PPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_SV_DIAG0_PPN_SHIFT ) & \
    GRIOMMU2_SV_DIAG0_PPN_MASK )

#define GRIOMMU2_SV_DIAG0_GSV 0x2U

#define GRIOMMU2_SV_DIAG0_V 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2SVDIAG1 \
 *   SV-stage TLB diagnostic interface, word 1 (SV_DIAG1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_SV_DIAG1_SADE 0x1000000000ULL

#define GRIOMMU2_SV_DIAG1_PSCID_SHIFT 16
#define GRIOMMU2_SV_DIAG1_PSCID_MASK  0xfffff0000ULL
#define GRIOMMU2_SV_DIAG1_PSCID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_SV_DIAG1_PSCID_MASK ) >> \
    GRIOMMU2_SV_DIAG1_PSCID_SHIFT )
#define GRIOMMU2_SV_DIAG1_PSCID_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_SV_DIAG1_PSCID_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_SV_DIAG1_PSCID_SHIFT ) & \
      GRIOMMU2_SV_DIAG1_PSCID_MASK ) )
#define GRIOMMU2_SV_DIAG1_PSCID( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_SV_DIAG1_PSCID_SHIFT ) & \
    GRIOMMU2_SV_DIAG1_PSCID_MASK )

#define GRIOMMU2_SV_DIAG1_GSCID_SHIFT 0
#define GRIOMMU2_SV_DIAG1_GSCID_MASK  0xffffU
#define GRIOMMU2_SV_DIAG1_GSCID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_SV_DIAG1_GSCID_MASK ) >> \
    GRIOMMU2_SV_DIAG1_GSCID_SHIFT )
#define GRIOMMU2_SV_DIAG1_GSCID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_SV_DIAG1_GSCID_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_SV_DIAG1_GSCID_SHIFT ) & \
      GRIOMMU2_SV_DIAG1_GSCID_MASK ) )
#define GRIOMMU2_SV_DIAG1_GSCID( _val )             \
  ( ( ( _val ) << GRIOMMU2_SV_DIAG1_GSCID_SHIFT ) & \
    GRIOMMU2_SV_DIAG1_GSCID_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2SVDIAG2 \
 *   SV-stage TLB diagnostic interface, word 2 (SV_DIAG2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_SV_DIAG2_X 0x8000000000000ULL

#define GRIOMMU2_SV_DIAG2_D 0x4000000000000ULL

#define GRIOMMU2_SV_DIAG2_A 0x2000000000000ULL

#define GRIOMMU2_SV_DIAG2_U 0x1000000000000ULL

#define GRIOMMU2_SV_DIAG2_W 0x400000000000ULL

#define GRIOMMU2_SV_DIAG2_PPN_SHIFT 2
#define GRIOMMU2_SV_DIAG2_PPN_MASK  0x3ffffffffffcULL
#define GRIOMMU2_SV_DIAG2_PPN_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_SV_DIAG2_PPN_MASK ) >> GRIOMMU2_SV_DIAG2_PPN_SHIFT )
#define GRIOMMU2_SV_DIAG2_PPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_SV_DIAG2_PPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_SV_DIAG2_PPN_SHIFT ) & \
      GRIOMMU2_SV_DIAG2_PPN_MASK ) )
#define GRIOMMU2_SV_DIAG2_PPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_SV_DIAG2_PPN_SHIFT ) & \
    GRIOMMU2_SV_DIAG2_PPN_MASK )

#define GRIOMMU2_SV_DIAG2_LVL_SHIFT 0
#define GRIOMMU2_SV_DIAG2_LVL_MASK  0x3U
#define GRIOMMU2_SV_DIAG2_LVL_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_SV_DIAG2_LVL_MASK ) >> GRIOMMU2_SV_DIAG2_LVL_SHIFT )
#define GRIOMMU2_SV_DIAG2_LVL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_SV_DIAG2_LVL_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_SV_DIAG2_LVL_SHIFT ) & \
      GRIOMMU2_SV_DIAG2_LVL_MASK ) )
#define GRIOMMU2_SV_DIAG2_LVL( _val ) \
  ( ( ( _val ) << GRIOMMU2_SV_DIAG2_LVL_SHIFT ) & GRIOMMU2_SV_DIAG2_LVL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2GINDEX G-stage TLB index selector (G_INDEX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_G_INDEX_NUM_G_SHIFT 16
#define GRIOMMU2_G_INDEX_NUM_G_MASK  0xffff0000U
#define GRIOMMU2_G_INDEX_NUM_G_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_G_INDEX_NUM_G_MASK ) >> \
    GRIOMMU2_G_INDEX_NUM_G_SHIFT )
#define GRIOMMU2_G_INDEX_NUM_G_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_G_INDEX_NUM_G_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_G_INDEX_NUM_G_SHIFT ) & \
      GRIOMMU2_G_INDEX_NUM_G_MASK ) )
#define GRIOMMU2_G_INDEX_NUM_G( _val )             \
  ( ( ( _val ) << GRIOMMU2_G_INDEX_NUM_G_SHIFT ) & \
    GRIOMMU2_G_INDEX_NUM_G_MASK )

#define GRIOMMU2_G_INDEX_IND_SHIFT 0
#define GRIOMMU2_G_INDEX_IND_MASK  0xffffU
#define GRIOMMU2_G_INDEX_IND_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_G_INDEX_IND_MASK ) >> GRIOMMU2_G_INDEX_IND_SHIFT )
#define GRIOMMU2_G_INDEX_IND_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_G_INDEX_IND_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_G_INDEX_IND_SHIFT ) & \
      GRIOMMU2_G_INDEX_IND_MASK ) )
#define GRIOMMU2_G_INDEX_IND( _val ) \
  ( ( ( _val ) << GRIOMMU2_G_INDEX_IND_SHIFT ) & GRIOMMU2_G_INDEX_IND_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2GDIAG0 \
 *   G-stage TLB diagnostic interface, word 0 (G_DIAG0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_G_DIAG0_LCK 0x8000000000000000ULL

#define GRIOMMU2_G_DIAG0_GSCID_SHIFT 45
#define GRIOMMU2_G_DIAG0_GSCID_MASK  0x1fffe00000000000ULL
#define GRIOMMU2_G_DIAG0_GSCID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_G_DIAG0_GSCID_MASK ) >> \
    GRIOMMU2_G_DIAG0_GSCID_SHIFT )
#define GRIOMMU2_G_DIAG0_GSCID_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_G_DIAG0_GSCID_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_G_DIAG0_GSCID_SHIFT ) & \
      GRIOMMU2_G_DIAG0_GSCID_MASK ) )
#define GRIOMMU2_G_DIAG0_GSCID( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_G_DIAG0_GSCID_SHIFT ) & \
    GRIOMMU2_G_DIAG0_GSCID_MASK )

#define GRIOMMU2_G_DIAG0_GPA_SHIFT 1
#define GRIOMMU2_G_DIAG0_GPA_MASK  0x1ffffffffffeULL
#define GRIOMMU2_G_DIAG0_GPA_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_G_DIAG0_GPA_MASK ) >> GRIOMMU2_G_DIAG0_GPA_SHIFT )
#define GRIOMMU2_G_DIAG0_GPA_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_G_DIAG0_GPA_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_G_DIAG0_GPA_SHIFT ) & \
      GRIOMMU2_G_DIAG0_GPA_MASK ) )
#define GRIOMMU2_G_DIAG0_GPA( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_G_DIAG0_GPA_SHIFT ) & \
    GRIOMMU2_G_DIAG0_GPA_MASK )

#define GRIOMMU2_G_DIAG0_V 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2GDIAG1 \
 *   G-stage TLB diagnostic interface, word 1 (G_DIAG1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_G_DIAG1_X 0x8000000000000ULL

#define GRIOMMU2_G_DIAG1_D 0x4000000000000ULL

#define GRIOMMU2_G_DIAG1_A 0x2000000000000ULL

#define GRIOMMU2_G_DIAG1_U 0x1000000000000ULL

#define GRIOMMU2_G_DIAG1_W 0x400000000000ULL

#define GRIOMMU2_G_DIAG1_PPN_SHIFT 2
#define GRIOMMU2_G_DIAG1_PPN_MASK  0x3ffffffffffcULL
#define GRIOMMU2_G_DIAG1_PPN_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_G_DIAG1_PPN_MASK ) >> GRIOMMU2_G_DIAG1_PPN_SHIFT )
#define GRIOMMU2_G_DIAG1_PPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_G_DIAG1_PPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_G_DIAG1_PPN_SHIFT ) & \
      GRIOMMU2_G_DIAG1_PPN_MASK ) )
#define GRIOMMU2_G_DIAG1_PPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_G_DIAG1_PPN_SHIFT ) & \
    GRIOMMU2_G_DIAG1_PPN_MASK )

#define GRIOMMU2_G_DIAG1_LVL_SHIFT 0
#define GRIOMMU2_G_DIAG1_LVL_MASK  0x3U
#define GRIOMMU2_G_DIAG1_LVL_GET( _reg ) \
  ( ( ( _reg ) & GRIOMMU2_G_DIAG1_LVL_MASK ) >> GRIOMMU2_G_DIAG1_LVL_SHIFT )
#define GRIOMMU2_G_DIAG1_LVL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_G_DIAG1_LVL_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_G_DIAG1_LVL_SHIFT ) & \
      GRIOMMU2_G_DIAG1_LVL_MASK ) )
#define GRIOMMU2_G_DIAG1_LVL( _val ) \
  ( ( ( _val ) << GRIOMMU2_G_DIAG1_LVL_SHIFT ) & GRIOMMU2_G_DIAG1_LVL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2MSIINDEX MSI TLB index selector (MSI_INDEX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_MSI_INDEX_NUM_MSI_SHIFT 16
#define GRIOMMU2_MSI_INDEX_NUM_MSI_MASK  0xffff0000U
#define GRIOMMU2_MSI_INDEX_NUM_MSI_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_MSI_INDEX_NUM_MSI_MASK ) >> \
    GRIOMMU2_MSI_INDEX_NUM_MSI_SHIFT )
#define GRIOMMU2_MSI_INDEX_NUM_MSI_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_MSI_INDEX_NUM_MSI_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_MSI_INDEX_NUM_MSI_SHIFT ) & \
      GRIOMMU2_MSI_INDEX_NUM_MSI_MASK ) )
#define GRIOMMU2_MSI_INDEX_NUM_MSI( _val )             \
  ( ( ( _val ) << GRIOMMU2_MSI_INDEX_NUM_MSI_SHIFT ) & \
    GRIOMMU2_MSI_INDEX_NUM_MSI_MASK )

#define GRIOMMU2_MSI_INDEX_IND_SHIFT 0
#define GRIOMMU2_MSI_INDEX_IND_MASK  0xffffU
#define GRIOMMU2_MSI_INDEX_IND_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_MSI_INDEX_IND_MASK ) >> \
    GRIOMMU2_MSI_INDEX_IND_SHIFT )
#define GRIOMMU2_MSI_INDEX_IND_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRIOMMU2_MSI_INDEX_IND_MASK ) |    \
    ( ( ( _val ) << GRIOMMU2_MSI_INDEX_IND_SHIFT ) & \
      GRIOMMU2_MSI_INDEX_IND_MASK ) )
#define GRIOMMU2_MSI_INDEX_IND( _val )             \
  ( ( ( _val ) << GRIOMMU2_MSI_INDEX_IND_SHIFT ) & \
    GRIOMMU2_MSI_INDEX_IND_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2MSIDIAG0 \
 *   MSI TLB diagnostic interface, word 0 (MSI_DIAG0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_MSI_DIAG0_LCK 0x8000000000000000ULL

#define GRIOMMU2_MSI_DIAG0_GSCID_SHIFT 45
#define GRIOMMU2_MSI_DIAG0_GSCID_MASK  0x1fffe00000000000ULL
#define GRIOMMU2_MSI_DIAG0_GSCID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_MSI_DIAG0_GSCID_MASK ) >> \
    GRIOMMU2_MSI_DIAG0_GSCID_SHIFT )
#define GRIOMMU2_MSI_DIAG0_GSCID_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_MSI_DIAG0_GSCID_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSI_DIAG0_GSCID_SHIFT ) & \
      GRIOMMU2_MSI_DIAG0_GSCID_MASK ) )
#define GRIOMMU2_MSI_DIAG0_GSCID( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSI_DIAG0_GSCID_SHIFT ) & \
    GRIOMMU2_MSI_DIAG0_GSCID_MASK )

#define GRIOMMU2_MSI_DIAG0_GPA_SHIFT 1
#define GRIOMMU2_MSI_DIAG0_GPA_MASK  0x1ffffffffffeULL
#define GRIOMMU2_MSI_DIAG0_GPA_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_MSI_DIAG0_GPA_MASK ) >> \
    GRIOMMU2_MSI_DIAG0_GPA_SHIFT )
#define GRIOMMU2_MSI_DIAG0_GPA_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_MSI_DIAG0_GPA_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSI_DIAG0_GPA_SHIFT ) & \
      GRIOMMU2_MSI_DIAG0_GPA_MASK ) )
#define GRIOMMU2_MSI_DIAG0_GPA( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSI_DIAG0_GPA_SHIFT ) & \
    GRIOMMU2_MSI_DIAG0_GPA_MASK )

#define GRIOMMU2_MSI_DIAG0_V 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2MSIDIAG1 \
 *   MSI TLB diagnostic interface, word 1 (MSI_DIAG1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_MSI_DIAG1_NID_SHIFT 49
#define GRIOMMU2_MSI_DIAG1_NID_MASK  0xffe000000000000ULL
#define GRIOMMU2_MSI_DIAG1_NID_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_MSI_DIAG1_NID_MASK ) >> \
    GRIOMMU2_MSI_DIAG1_NID_SHIFT )
#define GRIOMMU2_MSI_DIAG1_NID_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_MSI_DIAG1_NID_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSI_DIAG1_NID_SHIFT ) & \
      GRIOMMU2_MSI_DIAG1_NID_MASK ) )
#define GRIOMMU2_MSI_DIAG1_NID( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSI_DIAG1_NID_SHIFT ) & \
    GRIOMMU2_MSI_DIAG1_NID_MASK )

#define GRIOMMU2_MSI_DIAG1_C 0x1000000000000ULL

#define GRIOMMU2_MSI_DIAG1_ADDR_SHIFT 1
#define GRIOMMU2_MSI_DIAG1_ADDR_MASK  0xfffffffffffeULL
#define GRIOMMU2_MSI_DIAG1_ADDR_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_MSI_DIAG1_ADDR_MASK ) >> \
    GRIOMMU2_MSI_DIAG1_ADDR_SHIFT )
#define GRIOMMU2_MSI_DIAG1_ADDR_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_MSI_DIAG1_ADDR_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSI_DIAG1_ADDR_SHIFT ) & \
      GRIOMMU2_MSI_DIAG1_ADDR_MASK ) )
#define GRIOMMU2_MSI_DIAG1_ADDR( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSI_DIAG1_ADDR_SHIFT ) & \
    GRIOMMU2_MSI_DIAG1_ADDR_MASK )

#define GRIOMMU2_MSI_DIAG1_MRIF 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRIOMMU2MSIDIAG2 \
 *   MSI TLB diagnostic interface, word 2 (MSI_DIAG2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRIOMMU2_MSI_DIAG2_NPPN_SHIFT 0
#define GRIOMMU2_MSI_DIAG2_NPPN_MASK  0xfffffffffffULL
#define GRIOMMU2_MSI_DIAG2_NPPN_GET( _reg )        \
  ( ( ( _reg ) & GRIOMMU2_MSI_DIAG2_NPPN_MASK ) >> \
    GRIOMMU2_MSI_DIAG2_NPPN_SHIFT )
#define GRIOMMU2_MSI_DIAG2_NPPN_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~GRIOMMU2_MSI_DIAG2_NPPN_MASK ) |                   \
    ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSI_DIAG2_NPPN_SHIFT ) & \
      GRIOMMU2_MSI_DIAG2_NPPN_MASK ) )
#define GRIOMMU2_MSI_DIAG2_NPPN( _val )                            \
  ( ( ( (uint64_t) ( _val ) ) << GRIOMMU2_MSI_DIAG2_NPPN_SHIFT ) & \
    GRIOMMU2_MSI_DIAG2_NPPN_MASK )

/** @} */

/**
 * @brief This structure defines the GRIOMMU2 register block memory map.
 */
typedef struct griommu2 {
  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CAP.
   */
  uint64_t cap;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2FCTL.
   */
  uint32_t fctl;

  uint32_t reserved_c_10;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2DDTP.
   */
  uint64_t ddtp;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CQB.
   */
  uint64_t cqb;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CQH.
   */
  uint32_t cqh;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CQT.
   */
  uint32_t cqt;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2FQB.
   */
  uint64_t fqb;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2FQH.
   */
  uint32_t fqh;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2FQT.
   */
  uint32_t fqt;

  uint32_t reserved_38_48[ 4 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CQCSR.
   */
  uint32_t cqcsr;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2FQCSR.
   */
  uint32_t fqcsr;

  uint32_t reserved_50_54;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2IPSR.
   */
  uint32_t ipsr;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2IOCOUNTOVF.
   */
  uint32_t iocountovf;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2IOCOUNTINH.
   */
  uint32_t iocountinh;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2IOHPMCYCLES.
   */
  uint64_t iohpmcycles;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2IOHPMCTR.
   */
  uint64_t iohpmctr[ 31 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2IOHPMEVT.
   */
  uint64_t iohpmevt[ 31 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2TRREQIOVA.
   */
  uint64_t tr_req_iova;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2TRREQCTL.
   */
  uint64_t tr_req_ctl;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2TRRESPONSE.
   */
  uint64_t tr_response;

  uint32_t reserved_270_2b0[ 16 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2RCMD0.
   */
  uint64_t rcmd0;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2RCMD1.
   */
  uint64_t rcmd1;

  uint32_t reserved_2c0_2f8[ 14 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2ICVEC.
   */
  uint64_t icvec;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2MSIConfig.
   */
  griommu2_msicfg msicfg[ 16 ];

  uint32_t reserved_400_1000[ 768 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CTXINDEX.
   */
  uint32_t ctx_index;

  uint32_t reserved_1004_1008;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CTXDIAG0.
   */
  uint64_t ctx_diag0;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CTXDIAG1.
   */
  uint64_t ctx_diag1;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CTXDIAG2.
   */
  uint64_t ctx_diag2;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CTXDIAG3.
   */
  uint64_t ctx_diag3;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CTXDIAG4.
   */
  uint64_t ctx_diag4;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CTXDIAG5.
   */
  uint64_t ctx_diag5;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2CTXDIAG6.
   */
  uint64_t ctx_diag6;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2SVINDEX.
   */
  uint32_t sv_index;

  uint32_t reserved_1044_1048;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2SVDIAG0.
   */
  uint64_t sv_diag0;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2SVDIAG1.
   */
  uint64_t sv_diag1;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2SVDIAG2.
   */
  uint64_t sv_diag2;

  uint32_t reserved_1060_1080[ 8 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2GINDEX.
   */
  uint32_t g_index;

  uint32_t reserved_1084_1088;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2GDIAG0.
   */
  uint64_t g_diag0;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2GDIAG1.
   */
  uint64_t g_diag1;

  uint32_t reserved_1098_10c0[ 10 ];

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2MSIINDEX.
   */
  uint32_t msi_index;

  uint32_t reserved_10c4_10c8;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2MSIDIAG0.
   */
  uint64_t msi_diag0;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2MSIDIAG1.
   */
  uint64_t msi_diag1;

  /**
   * @brief See @ref RTEMSDeviceGRIOMMU2MSIDIAG2.
   */
  uint64_t msi_diag2;
} griommu2;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_GRIOMMU2_REGS_H */
