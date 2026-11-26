/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRDMAC2
 *
 * @brief This header file defines the GRDMAC2 register block interface.
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

/* Find related documentation with spec:/dev/grlib/if/grdmac2-header */

#ifndef _GRLIB_GRDMAC2_REGS_H
#define _GRLIB_GRDMAC2_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/grdmac2 */

/**
 * @defgroup RTEMSDeviceGRDMAC2 GRDMAC2
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the GRDMAC2 interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRDMAC2CTRL Control register (CTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_CTRL_TE 0x80U

#define GRDMAC2_CTRL_IER 0x40U

#define GRDMAC2_CTRL_IM 0x20U

#define GRDMAC2_CTRL_IE 0x10U

#define GRDMAC2_CTRL_RT 0x8U

#define GRDMAC2_CTRL_KICK 0x4U

#define GRDMAC2_CTRL_RST 0x2U

#define GRDMAC2_CTRL_EN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2STS Status register (STS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_STS_ST_SHIFT 27
#define GRDMAC2_STS_ST_MASK  0xf8000000U
#define GRDMAC2_STS_ST_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_STS_ST_MASK ) >> GRDMAC2_STS_ST_SHIFT )
#define GRDMAC2_STS_ST_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_STS_ST_MASK ) | \
    ( ( ( _val ) << GRDMAC2_STS_ST_SHIFT ) & GRDMAC2_STS_ST_MASK ) )
#define GRDMAC2_STS_ST( _val ) \
  ( ( ( _val ) << GRDMAC2_STS_ST_SHIFT ) & GRDMAC2_STS_ST_MASK )

#define GRDMAC2_STS_FP_SHIFT 17
#define GRDMAC2_STS_FP_MASK  0x7fe0000U
#define GRDMAC2_STS_FP_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_STS_FP_MASK ) >> GRDMAC2_STS_FP_SHIFT )
#define GRDMAC2_STS_FP_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_STS_FP_MASK ) | \
    ( ( ( _val ) << GRDMAC2_STS_FP_SHIFT ) & GRDMAC2_STS_FP_MASK ) )
#define GRDMAC2_STS_FP( _val ) \
  ( ( ( _val ) << GRDMAC2_STS_FP_SHIFT ) & GRDMAC2_STS_FP_MASK )

#define GRDMAC2_STS_FE 0x10000U

#define GRDMAC2_STS_NPE 0x8000U

#define GRDMAC2_STS_WDE 0x4000U

#define GRDMAC2_STS_RDE 0x2000U

#define GRDMAC2_STS_TO 0x1000U

#define GRDMAC2_STS_WBE 0x800U

#define GRDMAC2_STS_TRE 0x400U

#define GRDMAC2_STS_PE 0x200U

#define GRDMAC2_STS_RE 0x100U

#define GRDMAC2_STS_DE 0x80U

#define GRDMAC2_STS_KP 0x40U

#define GRDMAC2_STS_RSP 0x20U

#define GRDMAC2_STS_IF 0x10U

#define GRDMAC2_STS_PAU 0x8U

#define GRDMAC2_STS_ONG 0x4U

#define GRDMAC2_STS_ERR 0x2U

#define GRDMAC2_STS_CMP 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2TRST Timer reset value register (TRST)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_TRST_VAL_SHIFT 0
#define GRDMAC2_TRST_VAL_MASK  0xffffffffU
#define GRDMAC2_TRST_VAL_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_TRST_VAL_MASK ) >> GRDMAC2_TRST_VAL_SHIFT )
#define GRDMAC2_TRST_VAL_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_TRST_VAL_MASK ) | \
    ( ( ( _val ) << GRDMAC2_TRST_VAL_SHIFT ) & GRDMAC2_TRST_VAL_MASK ) )
#define GRDMAC2_TRST_VAL( _val ) \
  ( ( ( _val ) << GRDMAC2_TRST_VAL_SHIFT ) & GRDMAC2_TRST_VAL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2CAP Capability Register (CAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_CAP_BFDP_SHIFT 28
#define GRDMAC2_CAP_BFDP_MASK  0xf0000000U
#define GRDMAC2_CAP_BFDP_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_CAP_BFDP_MASK ) >> GRDMAC2_CAP_BFDP_SHIFT )
#define GRDMAC2_CAP_BFDP_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_CAP_BFDP_MASK ) | \
    ( ( ( _val ) << GRDMAC2_CAP_BFDP_SHIFT ) & GRDMAC2_CAP_BFDP_MASK ) )
#define GRDMAC2_CAP_BFDP( _val ) \
  ( ( ( _val ) << GRDMAC2_CAP_BFDP_SHIFT ) & GRDMAC2_CAP_BFDP_MASK )

#define GRDMAC2_CAP_ACC_SHIFT 12
#define GRDMAC2_CAP_ACC_MASK  0x1f000U
#define GRDMAC2_CAP_ACC_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_CAP_ACC_MASK ) >> GRDMAC2_CAP_ACC_SHIFT )
#define GRDMAC2_CAP_ACC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_CAP_ACC_MASK ) | \
    ( ( ( _val ) << GRDMAC2_CAP_ACC_SHIFT ) & GRDMAC2_CAP_ACC_MASK ) )
#define GRDMAC2_CAP_ACC( _val ) \
  ( ( ( _val ) << GRDMAC2_CAP_ACC_SHIFT ) & GRDMAC2_CAP_ACC_MASK )

#define GRDMAC2_CAP_DW_SHIFT 9
#define GRDMAC2_CAP_DW_MASK  0xe00U
#define GRDMAC2_CAP_DW_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_CAP_DW_MASK ) >> GRDMAC2_CAP_DW_SHIFT )
#define GRDMAC2_CAP_DW_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_CAP_DW_MASK ) | \
    ( ( ( _val ) << GRDMAC2_CAP_DW_SHIFT ) & GRDMAC2_CAP_DW_MASK ) )
#define GRDMAC2_CAP_DW( _val ) \
  ( ( ( _val ) << GRDMAC2_CAP_DW_SHIFT ) & GRDMAC2_CAP_DW_MASK )

#define GRDMAC2_CAP_TE 0x100U

#define GRDMAC2_CAP_FT_SHIFT 5
#define GRDMAC2_CAP_FT_MASK  0xe0U
#define GRDMAC2_CAP_FT_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_CAP_FT_MASK ) >> GRDMAC2_CAP_FT_SHIFT )
#define GRDMAC2_CAP_FT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_CAP_FT_MASK ) | \
    ( ( ( _val ) << GRDMAC2_CAP_FT_SHIFT ) & GRDMAC2_CAP_FT_MASK ) )
#define GRDMAC2_CAP_FT( _val ) \
  ( ( ( _val ) << GRDMAC2_CAP_FT_SHIFT ) & GRDMAC2_CAP_FT_MASK )

#define GRDMAC2_CAP_BM1 0x10U

#define GRDMAC2_CAP_VER_SHIFT 0
#define GRDMAC2_CAP_VER_MASK  0xfU
#define GRDMAC2_CAP_VER_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_CAP_VER_MASK ) >> GRDMAC2_CAP_VER_SHIFT )
#define GRDMAC2_CAP_VER_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_CAP_VER_MASK ) | \
    ( ( ( _val ) << GRDMAC2_CAP_VER_SHIFT ) & GRDMAC2_CAP_VER_MASK ) )
#define GRDMAC2_CAP_VER( _val ) \
  ( ( ( _val ) << GRDMAC2_CAP_VER_SHIFT ) & GRDMAC2_CAP_VER_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2FPTR First descriptor pointer (FPTR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_FPTR_ADDR_SHIFT 0
#define GRDMAC2_FPTR_ADDR_MASK  0xffffffffU
#define GRDMAC2_FPTR_ADDR_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_FPTR_ADDR_MASK ) >> GRDMAC2_FPTR_ADDR_SHIFT )
#define GRDMAC2_FPTR_ADDR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_FPTR_ADDR_MASK ) | \
    ( ( ( _val ) << GRDMAC2_FPTR_ADDR_SHIFT ) & GRDMAC2_FPTR_ADDR_MASK ) )
#define GRDMAC2_FPTR_ADDR( _val ) \
  ( ( ( _val ) << GRDMAC2_FPTR_ADDR_SHIFT ) & GRDMAC2_FPTR_ADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2DCTR \
 *   Descriptor control word for debug capability (DCTR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_DCTR_CTRL_SHIFT 0
#define GRDMAC2_DCTR_CTRL_MASK  0xffffffffU
#define GRDMAC2_DCTR_CTRL_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_DCTR_CTRL_MASK ) >> GRDMAC2_DCTR_CTRL_SHIFT )
#define GRDMAC2_DCTR_CTRL_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_DCTR_CTRL_MASK ) | \
    ( ( ( _val ) << GRDMAC2_DCTR_CTRL_SHIFT ) & GRDMAC2_DCTR_CTRL_MASK ) )
#define GRDMAC2_DCTR_CTRL( _val ) \
  ( ( ( _val ) << GRDMAC2_DCTR_CTRL_SHIFT ) & GRDMAC2_DCTR_CTRL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2DNXT \
 *   Next Descriptor pointer for debug capability (DNXT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_DNXT_ADDR_SHIFT 0
#define GRDMAC2_DNXT_ADDR_MASK  0xffffffffU
#define GRDMAC2_DNXT_ADDR_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_DNXT_ADDR_MASK ) >> GRDMAC2_DNXT_ADDR_SHIFT )
#define GRDMAC2_DNXT_ADDR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_DNXT_ADDR_MASK ) | \
    ( ( ( _val ) << GRDMAC2_DNXT_ADDR_SHIFT ) & GRDMAC2_DNXT_ADDR_MASK ) )
#define GRDMAC2_DNXT_ADDR( _val ) \
  ( ( ( _val ) << GRDMAC2_DNXT_ADDR_SHIFT ) & GRDMAC2_DNXT_ADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2DFNX fnext/dest.addr for debug capability (DFNX)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_DFNX_ADDR_SHIFT 0
#define GRDMAC2_DFNX_ADDR_MASK  0xffffffffU
#define GRDMAC2_DFNX_ADDR_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_DFNX_ADDR_MASK ) >> GRDMAC2_DFNX_ADDR_SHIFT )
#define GRDMAC2_DFNX_ADDR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_DFNX_ADDR_MASK ) | \
    ( ( ( _val ) << GRDMAC2_DFNX_ADDR_SHIFT ) & GRDMAC2_DFNX_ADDR_MASK ) )
#define GRDMAC2_DFNX_ADDR( _val ) \
  ( ( ( _val ) << GRDMAC2_DFNX_ADDR_SHIFT ) & GRDMAC2_DFNX_ADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2DPOL \
 *   Poll or source address for debug capability (DPOL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_DPOL_ADDR_SHIFT 0
#define GRDMAC2_DPOL_ADDR_MASK  0xffffffffU
#define GRDMAC2_DPOL_ADDR_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_DPOL_ADDR_MASK ) >> GRDMAC2_DPOL_ADDR_SHIFT )
#define GRDMAC2_DPOL_ADDR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_DPOL_ADDR_MASK ) | \
    ( ( ( _val ) << GRDMAC2_DPOL_ADDR_SHIFT ) & GRDMAC2_DPOL_ADDR_MASK ) )
#define GRDMAC2_DPOL_ADDR( _val ) \
  ( ( ( _val ) << GRDMAC2_DPOL_ADDR_SHIFT ) & GRDMAC2_DPOL_ADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2DSTS \
 *   Descriptor status for debug capability (DSTS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_DSTS_STS_SHIFT 0
#define GRDMAC2_DSTS_STS_MASK  0xffffffffU
#define GRDMAC2_DSTS_STS_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_DSTS_STS_MASK ) >> GRDMAC2_DSTS_STS_SHIFT )
#define GRDMAC2_DSTS_STS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_DSTS_STS_MASK ) | \
    ( ( ( _val ) << GRDMAC2_DSTS_STS_SHIFT ) & GRDMAC2_DSTS_STS_MASK ) )
#define GRDMAC2_DSTS_STS( _val ) \
  ( ( ( _val ) << GRDMAC2_DSTS_STS_SHIFT ) & GRDMAC2_DSTS_STS_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2DDTA Expected data for debug capability (DDTA)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_DDTA_DATA_SHIFT 0
#define GRDMAC2_DDTA_DATA_MASK  0xffffffffU
#define GRDMAC2_DDTA_DATA_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_DDTA_DATA_MASK ) >> GRDMAC2_DDTA_DATA_SHIFT )
#define GRDMAC2_DDTA_DATA_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_DDTA_DATA_MASK ) | \
    ( ( ( _val ) << GRDMAC2_DDTA_DATA_SHIFT ) & GRDMAC2_DDTA_DATA_MASK ) )
#define GRDMAC2_DDTA_DATA( _val ) \
  ( ( ( _val ) << GRDMAC2_DDTA_DATA_SHIFT ) & GRDMAC2_DDTA_DATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2DMSK \
 *   Conditional mask for debug capability (DMSK)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_DMSK_MASK_SHIFT 0
#define GRDMAC2_DMSK_MASK_MASK  0xffffffffU
#define GRDMAC2_DMSK_MASK_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_DMSK_MASK_MASK ) >> GRDMAC2_DMSK_MASK_SHIFT )
#define GRDMAC2_DMSK_MASK_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_DMSK_MASK_MASK ) | \
    ( ( ( _val ) << GRDMAC2_DMSK_MASK_SHIFT ) & GRDMAC2_DMSK_MASK_MASK ) )
#define GRDMAC2_DMSK_MASK( _val ) \
  ( ( ( _val ) << GRDMAC2_DMSK_MASK_SHIFT ) & GRDMAC2_DMSK_MASK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRDMAC2DPTR \
 *   Current descriptor pointer for debug capability (DPTR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRDMAC2_DPTR_ADDR_SHIFT 0
#define GRDMAC2_DPTR_ADDR_MASK  0xffffffffU
#define GRDMAC2_DPTR_ADDR_GET( _reg ) \
  ( ( ( _reg ) & GRDMAC2_DPTR_ADDR_MASK ) >> GRDMAC2_DPTR_ADDR_SHIFT )
#define GRDMAC2_DPTR_ADDR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRDMAC2_DPTR_ADDR_MASK ) | \
    ( ( ( _val ) << GRDMAC2_DPTR_ADDR_SHIFT ) & GRDMAC2_DPTR_ADDR_MASK ) )
#define GRDMAC2_DPTR_ADDR( _val ) \
  ( ( ( _val ) << GRDMAC2_DPTR_ADDR_SHIFT ) & GRDMAC2_DPTR_ADDR_MASK )

/** @} */

/**
 * @brief This structure defines the GRDMAC2 register block memory map.
 */
typedef struct grdmac2 {
  /**
   * @brief See @ref RTEMSDeviceGRDMAC2CTRL.
   */
  uint32_t ctrl;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2STS.
   */
  uint32_t sts;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2TRST.
   */
  uint32_t trst;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2CAP.
   */
  uint32_t cap;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2FPTR.
   */
  uint32_t fptr;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2DCTR.
   */
  uint32_t dctr;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2DNXT.
   */
  uint32_t dnxt;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2DFNX.
   */
  uint32_t dfnx;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2DPOL.
   */
  uint32_t dpol;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2DSTS.
   */
  uint32_t dsts;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2DDTA.
   */
  uint32_t ddta;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2DMSK.
   */
  uint32_t dmsk;

  /**
   * @brief See @ref RTEMSDeviceGRDMAC2DPTR.
   */
  uint32_t dptr;
} grdmac2;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_GRDMAC2_REGS_H */
