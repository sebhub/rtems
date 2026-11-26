/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRLIBNANDFCTRL2
 *
 * @brief This header file defines the NANDFCTRL2 register block interface.
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

/* Find related documentation with spec:/dev/grlib/if/nandfctrl2-header */

#ifndef _GRLIB_NANDFCTRL2_REGS_H
#define _GRLIB_NANDFCTRL2_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/nandfctrl2 */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2 NANDFCTRL2
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the NANDFCTRL2 interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CTRL0 Core control 0 register (CTRL0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CTRL0_WR_SHIFT 19
#define NANDFCTRL2_CTRL0_WR_MASK  0x380000U
#define NANDFCTRL2_CTRL0_WR_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CTRL0_WR_MASK ) >> NANDFCTRL2_CTRL0_WR_SHIFT )
#define NANDFCTRL2_CTRL0_WR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CTRL0_WR_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CTRL0_WR_SHIFT ) & \
      NANDFCTRL2_CTRL0_WR_MASK ) )
#define NANDFCTRL2_CTRL0_WR( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CTRL0_WR_SHIFT ) & NANDFCTRL2_CTRL0_WR_MASK )

#define NANDFCTRL2_CTRL0_WW_SHIFT 16
#define NANDFCTRL2_CTRL0_WW_MASK  0x70000U
#define NANDFCTRL2_CTRL0_WW_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CTRL0_WW_MASK ) >> NANDFCTRL2_CTRL0_WW_SHIFT )
#define NANDFCTRL2_CTRL0_WW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CTRL0_WW_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CTRL0_WW_SHIFT ) & \
      NANDFCTRL2_CTRL0_WW_MASK ) )
#define NANDFCTRL2_CTRL0_WW( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CTRL0_WW_SHIFT ) & NANDFCTRL2_CTRL0_WW_MASK )

#define NANDFCTRL2_CTRL0_EE 0x2000U

#define NANDFCTRL2_CTRL0_RE 0x1000U

#define NANDFCTRL2_CTRL0_LLM 0x400U

#define NANDFCTRL2_CTRL0_BBM 0x200U

#define NANDFCTRL2_CTRL0_EDO 0x80U

#define NANDFCTRL2_CTRL0_IFSEL_SHIFT 5
#define NANDFCTRL2_CTRL0_IFSEL_MASK  0x60U
#define NANDFCTRL2_CTRL0_IFSEL_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CTRL0_IFSEL_MASK ) >> \
    NANDFCTRL2_CTRL0_IFSEL_SHIFT )
#define NANDFCTRL2_CTRL0_IFSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CTRL0_IFSEL_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CTRL0_IFSEL_SHIFT ) & \
      NANDFCTRL2_CTRL0_IFSEL_MASK ) )
#define NANDFCTRL2_CTRL0_IFSEL( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CTRL0_IFSEL_SHIFT ) & \
    NANDFCTRL2_CTRL0_IFSEL_MASK )
#define NANDFCTRL2_CTRL0_IFSEL_SDR    0U
#define NANDFCTRL2_CTRL0_IFSEL_NV_DDR 2U

#define NANDFCTRL2_CTRL0_MSEL_SHIFT 3
#define NANDFCTRL2_CTRL0_MSEL_MASK  0x18U
#define NANDFCTRL2_CTRL0_MSEL_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CTRL0_MSEL_MASK ) >> NANDFCTRL2_CTRL0_MSEL_SHIFT )
#define NANDFCTRL2_CTRL0_MSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CTRL0_MSEL_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CTRL0_MSEL_SHIFT ) & \
      NANDFCTRL2_CTRL0_MSEL_MASK ) )
#define NANDFCTRL2_CTRL0_MSEL( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CTRL0_MSEL_SHIFT ) & NANDFCTRL2_CTRL0_MSEL_MASK )

#define NANDFCTRL2_CTRL0_CM 0x4U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CTRL1 Core control 1 register (CTRL1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CTRL1_IRQCERRDL 0x2000U

#define NANDFCTRL2_CTRL1_IRQCERRUL 0x1000U

#define NANDFCTRL2_CTRL1_IRQUCERRDL 0x800U

#define NANDFCTRL2_CTRL1_IRQUCERRUL 0x400U

#define NANDFCTRL2_CTRL1_IRQSTOPLL 0x200U

#define NANDFCTRL2_CTRL1_IRQABORT 0x100U

#define NANDFCTRL2_CTRL1_IRQTMOUT 0x40U

#define NANDFCTRL2_CTRL1_IRQCMD 0x20U

#define NANDFCTRL2_CTRL1_IRQDL 0x10U

#define NANDFCTRL2_CTRL1_IRQUL 0x8U

#define NANDFCTRL2_CTRL1_IRQECC 0x4U

#define NANDFCTRL2_CTRL1_IRQDS 0x2U

#define NANDFCTRL2_CTRL1_IRQ 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CTRL2 Core control 2 register (CTRL2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CTRL2_STOPLL 0x8U

#define NANDFCTRL2_CTRL2_ABORT 0x4U

#define NANDFCTRL2_CTRL2_DT 0x2U

#define NANDFCTRL2_CTRL2_RST 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CTRL3 Core control 3 register (CTRL3)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CTRL3_WP_SHIFT 16
#define NANDFCTRL2_CTRL3_WP_MASK  0xffff0000U
#define NANDFCTRL2_CTRL3_WP_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CTRL3_WP_MASK ) >> NANDFCTRL2_CTRL3_WP_SHIFT )
#define NANDFCTRL2_CTRL3_WP_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CTRL3_WP_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CTRL3_WP_SHIFT ) & \
      NANDFCTRL2_CTRL3_WP_MASK ) )
#define NANDFCTRL2_CTRL3_WP( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CTRL3_WP_SHIFT ) & NANDFCTRL2_CTRL3_WP_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CTRL4 Core control 4 register (CTRL4)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CTRL4_SEFI_FLAG_SHIFT 0
#define NANDFCTRL2_CTRL4_SEFI_FLAG_MASK  0xffffffffU
#define NANDFCTRL2_CTRL4_SEFI_FLAG_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CTRL4_SEFI_FLAG_MASK ) >> \
    NANDFCTRL2_CTRL4_SEFI_FLAG_SHIFT )
#define NANDFCTRL2_CTRL4_SEFI_FLAG_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CTRL4_SEFI_FLAG_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CTRL4_SEFI_FLAG_SHIFT ) & \
      NANDFCTRL2_CTRL4_SEFI_FLAG_MASK ) )
#define NANDFCTRL2_CTRL4_SEFI_FLAG( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CTRL4_SEFI_FLAG_SHIFT ) & \
    NANDFCTRL2_CTRL4_SEFI_FLAG_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2STS0 Core status 0 register (STS0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_STS0_WP_SHIFT 16
#define NANDFCTRL2_STS0_WP_MASK  0xffff0000U
#define NANDFCTRL2_STS0_WP_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_STS0_WP_MASK ) >> NANDFCTRL2_STS0_WP_SHIFT )
#define NANDFCTRL2_STS0_WP_SET( _reg, _val )  \
  ( ( ( _reg ) & ~NANDFCTRL2_STS0_WP_MASK ) | \
    ( ( ( _val ) << NANDFCTRL2_STS0_WP_SHIFT ) & NANDFCTRL2_STS0_WP_MASK ) )
#define NANDFCTRL2_STS0_WP( _val ) \
  ( ( ( _val ) << NANDFCTRL2_STS0_WP_SHIFT ) & NANDFCTRL2_STS0_WP_MASK )

#define NANDFCTRL2_STS0_DA 0x8U

#define NANDFCTRL2_STS0_RDY 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2STS1 Core status 1 register (STS1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_STS1_CERRDL 0x2000U

#define NANDFCTRL2_STS1_CERRUL 0x1000U

#define NANDFCTRL2_STS1_UCERRDL 0x800U

#define NANDFCTRL2_STS1_UCERRUL 0x400U

#define NANDFCTRL2_STS1_STOPLL 0x200U

#define NANDFCTRL2_STS1_ABORT 0x100U

#define NANDFCTRL2_STS1_TMOUT 0x40U

#define NANDFCTRL2_STS1_CMD 0x20U

#define NANDFCTRL2_STS1_DL 0x10U

#define NANDFCTRL2_STS1_UL 0x8U

#define NANDFCTRL2_STS1_ECC 0x4U

#define NANDFCTRL2_STS1_DS 0x2U

#define NANDFCTRL2_STS1_IRQ 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2STS2 Core status 2 register (STS2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_STS2_TMOUT_SHIFT 0
#define NANDFCTRL2_STS2_TMOUT_MASK  0xffffffffU
#define NANDFCTRL2_STS2_TMOUT_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_STS2_TMOUT_MASK ) >> NANDFCTRL2_STS2_TMOUT_SHIFT )
#define NANDFCTRL2_STS2_TMOUT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_STS2_TMOUT_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_STS2_TMOUT_SHIFT ) & \
      NANDFCTRL2_STS2_TMOUT_MASK ) )
#define NANDFCTRL2_STS2_TMOUT( _val ) \
  ( ( ( _val ) << NANDFCTRL2_STS2_TMOUT_SHIFT ) & NANDFCTRL2_STS2_TMOUT_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2STS3 Core status 3 register (STS3)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_STS3_RBSTS_SHIFT 0
#define NANDFCTRL2_STS3_RBSTS_MASK  0xffffffffU
#define NANDFCTRL2_STS3_RBSTS_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_STS3_RBSTS_MASK ) >> NANDFCTRL2_STS3_RBSTS_SHIFT )
#define NANDFCTRL2_STS3_RBSTS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_STS3_RBSTS_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_STS3_RBSTS_SHIFT ) & \
      NANDFCTRL2_STS3_RBSTS_MASK ) )
#define NANDFCTRL2_STS3_RBSTS( _val ) \
  ( ( ( _val ) << NANDFCTRL2_STS3_RBSTS_SHIFT ) & NANDFCTRL2_STS3_RBSTS_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CAP0 Capability 0 register (CAP0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CAP0_MEMSUP_SHIFT 23
#define NANDFCTRL2_CAP0_MEMSUP_MASK  0x1800000U
#define NANDFCTRL2_CAP0_MEMSUP_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP0_MEMSUP_MASK ) >> \
    NANDFCTRL2_CAP0_MEMSUP_SHIFT )
#define NANDFCTRL2_CAP0_MEMSUP_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP0_MEMSUP_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP0_MEMSUP_SHIFT ) & \
      NANDFCTRL2_CAP0_MEMSUP_MASK ) )
#define NANDFCTRL2_CAP0_MEMSUP( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP0_MEMSUP_SHIFT ) & \
    NANDFCTRL2_CAP0_MEMSUP_MASK )

#define NANDFCTRL2_CAP0_FT_SHIFT 20
#define NANDFCTRL2_CAP0_FT_MASK  0x700000U
#define NANDFCTRL2_CAP0_FT_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CAP0_FT_MASK ) >> NANDFCTRL2_CAP0_FT_SHIFT )
#define NANDFCTRL2_CAP0_FT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP0_FT_MASK ) | \
    ( ( ( _val ) << NANDFCTRL2_CAP0_FT_SHIFT ) & NANDFCTRL2_CAP0_FT_MASK ) )
#define NANDFCTRL2_CAP0_FT( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CAP0_FT_SHIFT ) & NANDFCTRL2_CAP0_FT_MASK )

#define NANDFCTRL2_CAP0_RND 0x40000U

#define NANDFCTRL2_CAP0_NECC_SHIFT 16
#define NANDFCTRL2_CAP0_NECC_MASK  0x30000U
#define NANDFCTRL2_CAP0_NECC_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CAP0_NECC_MASK ) >> NANDFCTRL2_CAP0_NECC_SHIFT )
#define NANDFCTRL2_CAP0_NECC_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP0_NECC_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP0_NECC_SHIFT ) & \
      NANDFCTRL2_CAP0_NECC_MASK ) )
#define NANDFCTRL2_CAP0_NECC( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CAP0_NECC_SHIFT ) & NANDFCTRL2_CAP0_NECC_MASK )

#define NANDFCTRL2_CAP0_NRB_SHIFT 11
#define NANDFCTRL2_CAP0_NRB_MASK  0xf800U
#define NANDFCTRL2_CAP0_NRB_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CAP0_NRB_MASK ) >> NANDFCTRL2_CAP0_NRB_SHIFT )
#define NANDFCTRL2_CAP0_NRB_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP0_NRB_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP0_NRB_SHIFT ) & \
      NANDFCTRL2_CAP0_NRB_MASK ) )
#define NANDFCTRL2_CAP0_NRB( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CAP0_NRB_SHIFT ) & NANDFCTRL2_CAP0_NRB_MASK )

#define NANDFCTRL2_CAP0_NCHAN_SHIFT 6
#define NANDFCTRL2_CAP0_NCHAN_MASK  0x7c0U
#define NANDFCTRL2_CAP0_NCHAN_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CAP0_NCHAN_MASK ) >> NANDFCTRL2_CAP0_NCHAN_SHIFT )
#define NANDFCTRL2_CAP0_NCHAN_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP0_NCHAN_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP0_NCHAN_SHIFT ) & \
      NANDFCTRL2_CAP0_NCHAN_MASK ) )
#define NANDFCTRL2_CAP0_NCHAN( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CAP0_NCHAN_SHIFT ) & NANDFCTRL2_CAP0_NCHAN_MASK )

#define NANDFCTRL2_CAP0_NTARGS_SHIFT 0
#define NANDFCTRL2_CAP0_NTARGS_MASK  0x3fU
#define NANDFCTRL2_CAP0_NTARGS_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP0_NTARGS_MASK ) >> \
    NANDFCTRL2_CAP0_NTARGS_SHIFT )
#define NANDFCTRL2_CAP0_NTARGS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP0_NTARGS_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP0_NTARGS_SHIFT ) & \
      NANDFCTRL2_CAP0_NTARGS_MASK ) )
#define NANDFCTRL2_CAP0_NTARGS( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP0_NTARGS_SHIFT ) & \
    NANDFCTRL2_CAP0_NTARGS_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CAP1 Capability 1 register (CAP1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CAP1_E1CAP_SHIFT 26
#define NANDFCTRL2_CAP1_E1CAP_MASK  0xfc000000U
#define NANDFCTRL2_CAP1_E1CAP_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CAP1_E1CAP_MASK ) >> NANDFCTRL2_CAP1_E1CAP_SHIFT )
#define NANDFCTRL2_CAP1_E1CAP_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP1_E1CAP_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP1_E1CAP_SHIFT ) & \
      NANDFCTRL2_CAP1_E1CAP_MASK ) )
#define NANDFCTRL2_CAP1_E1CAP( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CAP1_E1CAP_SHIFT ) & NANDFCTRL2_CAP1_E1CAP_MASK )

#define NANDFCTRL2_CAP1_E1CHUNK_SHIFT 21
#define NANDFCTRL2_CAP1_E1CHUNK_MASK  0x3e00000U
#define NANDFCTRL2_CAP1_E1CHUNK_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP1_E1CHUNK_MASK ) >> \
    NANDFCTRL2_CAP1_E1CHUNK_SHIFT )
#define NANDFCTRL2_CAP1_E1CHUNK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP1_E1CHUNK_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP1_E1CHUNK_SHIFT ) & \
      NANDFCTRL2_CAP1_E1CHUNK_MASK ) )
#define NANDFCTRL2_CAP1_E1CHUNK( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP1_E1CHUNK_SHIFT ) & \
    NANDFCTRL2_CAP1_E1CHUNK_MASK )

#define NANDFCTRL2_CAP1_E1GF_SHIFT 16
#define NANDFCTRL2_CAP1_E1GF_MASK  0x1f0000U
#define NANDFCTRL2_CAP1_E1GF_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CAP1_E1GF_MASK ) >> NANDFCTRL2_CAP1_E1GF_SHIFT )
#define NANDFCTRL2_CAP1_E1GF_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP1_E1GF_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP1_E1GF_SHIFT ) & \
      NANDFCTRL2_CAP1_E1GF_MASK ) )
#define NANDFCTRL2_CAP1_E1GF( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CAP1_E1GF_SHIFT ) & NANDFCTRL2_CAP1_E1GF_MASK )

#define NANDFCTRL2_CAP1_E0CAP_SHIFT 10
#define NANDFCTRL2_CAP1_E0CAP_MASK  0xfc00U
#define NANDFCTRL2_CAP1_E0CAP_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CAP1_E0CAP_MASK ) >> NANDFCTRL2_CAP1_E0CAP_SHIFT )
#define NANDFCTRL2_CAP1_E0CAP_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP1_E0CAP_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP1_E0CAP_SHIFT ) & \
      NANDFCTRL2_CAP1_E0CAP_MASK ) )
#define NANDFCTRL2_CAP1_E0CAP( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CAP1_E0CAP_SHIFT ) & NANDFCTRL2_CAP1_E0CAP_MASK )

#define NANDFCTRL2_CAP1_E0CHUNK_SHIFT 5
#define NANDFCTRL2_CAP1_E0CHUNK_MASK  0x3e0U
#define NANDFCTRL2_CAP1_E0CHUNK_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP1_E0CHUNK_MASK ) >> \
    NANDFCTRL2_CAP1_E0CHUNK_SHIFT )
#define NANDFCTRL2_CAP1_E0CHUNK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP1_E0CHUNK_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP1_E0CHUNK_SHIFT ) & \
      NANDFCTRL2_CAP1_E0CHUNK_MASK ) )
#define NANDFCTRL2_CAP1_E0CHUNK( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP1_E0CHUNK_SHIFT ) & \
    NANDFCTRL2_CAP1_E0CHUNK_MASK )

#define NANDFCTRL2_CAP1_E0GF_SHIFT 0
#define NANDFCTRL2_CAP1_E0GF_MASK  0x1fU
#define NANDFCTRL2_CAP1_E0GF_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_CAP1_E0GF_MASK ) >> NANDFCTRL2_CAP1_E0GF_SHIFT )
#define NANDFCTRL2_CAP1_E0GF_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP1_E0GF_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP1_E0GF_SHIFT ) & \
      NANDFCTRL2_CAP1_E0GF_MASK ) )
#define NANDFCTRL2_CAP1_E0GF( _val ) \
  ( ( ( _val ) << NANDFCTRL2_CAP1_E0GF_SHIFT ) & NANDFCTRL2_CAP1_E0GF_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CAP2 Capability 2 register (CAP2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CAP2_M0SEL 0x80000000U

#define NANDFCTRL2_CAP2_M0SPARE_SHIFT 16
#define NANDFCTRL2_CAP2_M0SPARE_MASK  0x1fff0000U
#define NANDFCTRL2_CAP2_M0SPARE_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP2_M0SPARE_MASK ) >> \
    NANDFCTRL2_CAP2_M0SPARE_SHIFT )
#define NANDFCTRL2_CAP2_M0SPARE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP2_M0SPARE_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP2_M0SPARE_SHIFT ) & \
      NANDFCTRL2_CAP2_M0SPARE_MASK ) )
#define NANDFCTRL2_CAP2_M0SPARE( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP2_M0SPARE_SHIFT ) & \
    NANDFCTRL2_CAP2_M0SPARE_MASK )

#define NANDFCTRL2_CAP2_M0DATA_SHIFT 0
#define NANDFCTRL2_CAP2_M0DATA_MASK  0xffffU
#define NANDFCTRL2_CAP2_M0DATA_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP2_M0DATA_MASK ) >> \
    NANDFCTRL2_CAP2_M0DATA_SHIFT )
#define NANDFCTRL2_CAP2_M0DATA_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP2_M0DATA_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP2_M0DATA_SHIFT ) & \
      NANDFCTRL2_CAP2_M0DATA_MASK ) )
#define NANDFCTRL2_CAP2_M0DATA( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP2_M0DATA_SHIFT ) & \
    NANDFCTRL2_CAP2_M0DATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CAP3 Capability 3 register (CAP3)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CAP3_M1SEL 0x80000000U

#define NANDFCTRL2_CAP3_M1SPARE_SHIFT 16
#define NANDFCTRL2_CAP3_M1SPARE_MASK  0x1fff0000U
#define NANDFCTRL2_CAP3_M1SPARE_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP3_M1SPARE_MASK ) >> \
    NANDFCTRL2_CAP3_M1SPARE_SHIFT )
#define NANDFCTRL2_CAP3_M1SPARE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP3_M1SPARE_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP3_M1SPARE_SHIFT ) & \
      NANDFCTRL2_CAP3_M1SPARE_MASK ) )
#define NANDFCTRL2_CAP3_M1SPARE( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP3_M1SPARE_SHIFT ) & \
    NANDFCTRL2_CAP3_M1SPARE_MASK )

#define NANDFCTRL2_CAP3_M1DATA_SHIFT 0
#define NANDFCTRL2_CAP3_M1DATA_MASK  0xffffU
#define NANDFCTRL2_CAP3_M1DATA_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP3_M1DATA_MASK ) >> \
    NANDFCTRL2_CAP3_M1DATA_SHIFT )
#define NANDFCTRL2_CAP3_M1DATA_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP3_M1DATA_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP3_M1DATA_SHIFT ) & \
      NANDFCTRL2_CAP3_M1DATA_MASK ) )
#define NANDFCTRL2_CAP3_M1DATA( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP3_M1DATA_SHIFT ) & \
    NANDFCTRL2_CAP3_M1DATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CAP4 Capability 4 register (CAP4)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CAP4_M2SEL 0x80000000U

#define NANDFCTRL2_CAP4_M2SPARE_SHIFT 16
#define NANDFCTRL2_CAP4_M2SPARE_MASK  0x1fff0000U
#define NANDFCTRL2_CAP4_M2SPARE_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP4_M2SPARE_MASK ) >> \
    NANDFCTRL2_CAP4_M2SPARE_SHIFT )
#define NANDFCTRL2_CAP4_M2SPARE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP4_M2SPARE_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP4_M2SPARE_SHIFT ) & \
      NANDFCTRL2_CAP4_M2SPARE_MASK ) )
#define NANDFCTRL2_CAP4_M2SPARE( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP4_M2SPARE_SHIFT ) & \
    NANDFCTRL2_CAP4_M2SPARE_MASK )

#define NANDFCTRL2_CAP4_M2DATA_SHIFT 0
#define NANDFCTRL2_CAP4_M2DATA_MASK  0xffffU
#define NANDFCTRL2_CAP4_M2DATA_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP4_M2DATA_MASK ) >> \
    NANDFCTRL2_CAP4_M2DATA_SHIFT )
#define NANDFCTRL2_CAP4_M2DATA_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP4_M2DATA_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP4_M2DATA_SHIFT ) & \
      NANDFCTRL2_CAP4_M2DATA_MASK ) )
#define NANDFCTRL2_CAP4_M2DATA( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP4_M2DATA_SHIFT ) & \
    NANDFCTRL2_CAP4_M2DATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2CAP5 Capability 5 register (CAP5)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_CAP5_TAG_SIZE_SHIFT 8
#define NANDFCTRL2_CAP5_TAG_SIZE_MASK  0x3f00U
#define NANDFCTRL2_CAP5_TAG_SIZE_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP5_TAG_SIZE_MASK ) >> \
    NANDFCTRL2_CAP5_TAG_SIZE_SHIFT )
#define NANDFCTRL2_CAP5_TAG_SIZE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP5_TAG_SIZE_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP5_TAG_SIZE_SHIFT ) & \
      NANDFCTRL2_CAP5_TAG_SIZE_MASK ) )
#define NANDFCTRL2_CAP5_TAG_SIZE( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP5_TAG_SIZE_SHIFT ) & \
    NANDFCTRL2_CAP5_TAG_SIZE_MASK )

#define NANDFCTRL2_CAP5_RST_CYCLES_SHIFT 0
#define NANDFCTRL2_CAP5_RST_CYCLES_MASK  0xffU
#define NANDFCTRL2_CAP5_RST_CYCLES_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_CAP5_RST_CYCLES_MASK ) >> \
    NANDFCTRL2_CAP5_RST_CYCLES_SHIFT )
#define NANDFCTRL2_CAP5_RST_CYCLES_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_CAP5_RST_CYCLES_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_CAP5_RST_CYCLES_SHIFT ) & \
      NANDFCTRL2_CAP5_RST_CYCLES_MASK ) )
#define NANDFCTRL2_CAP5_RST_CYCLES( _val )             \
  ( ( ( _val ) << NANDFCTRL2_CAP5_RST_CYCLES_SHIFT ) & \
    NANDFCTRL2_CAP5_RST_CYCLES_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME0 \
 *   Programmable timing 0 register (TME0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME0_TCS_SHIFT 16
#define NANDFCTRL2_TME0_TCS_MASK  0xffff0000U
#define NANDFCTRL2_TME0_TCS_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME0_TCS_MASK ) >> NANDFCTRL2_TME0_TCS_SHIFT )
#define NANDFCTRL2_TME0_TCS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME0_TCS_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME0_TCS_SHIFT ) & \
      NANDFCTRL2_TME0_TCS_MASK ) )
#define NANDFCTRL2_TME0_TCS( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME0_TCS_SHIFT ) & NANDFCTRL2_TME0_TCS_MASK )

#define NANDFCTRL2_TME0_TWW_SHIFT 0
#define NANDFCTRL2_TME0_TWW_MASK  0xffffU
#define NANDFCTRL2_TME0_TWW_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME0_TWW_MASK ) >> NANDFCTRL2_TME0_TWW_SHIFT )
#define NANDFCTRL2_TME0_TWW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME0_TWW_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME0_TWW_SHIFT ) & \
      NANDFCTRL2_TME0_TWW_MASK ) )
#define NANDFCTRL2_TME0_TWW( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME0_TWW_SHIFT ) & NANDFCTRL2_TME0_TWW_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME1 \
 *   Programmable timing 1 register (TME1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME1_TRR_SHIFT 16
#define NANDFCTRL2_TME1_TRR_MASK  0xffff0000U
#define NANDFCTRL2_TME1_TRR_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME1_TRR_MASK ) >> NANDFCTRL2_TME1_TRR_SHIFT )
#define NANDFCTRL2_TME1_TRR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME1_TRR_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME1_TRR_SHIFT ) & \
      NANDFCTRL2_TME1_TRR_MASK ) )
#define NANDFCTRL2_TME1_TRR( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME1_TRR_SHIFT ) & NANDFCTRL2_TME1_TRR_MASK )

#define NANDFCTRL2_TME1_TWB_SHIFT 0
#define NANDFCTRL2_TME1_TWB_MASK  0xffffU
#define NANDFCTRL2_TME1_TWB_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME1_TWB_MASK ) >> NANDFCTRL2_TME1_TWB_SHIFT )
#define NANDFCTRL2_TME1_TWB_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME1_TWB_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME1_TWB_SHIFT ) & \
      NANDFCTRL2_TME1_TWB_MASK ) )
#define NANDFCTRL2_TME1_TWB( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME1_TWB_SHIFT ) & NANDFCTRL2_TME1_TWB_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME2 \
 *   Programmable timing 2 register (TME2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME2_TRHW_SHIFT 16
#define NANDFCTRL2_TME2_TRHW_MASK  0xffff0000U
#define NANDFCTRL2_TME2_TRHW_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME2_TRHW_MASK ) >> NANDFCTRL2_TME2_TRHW_SHIFT )
#define NANDFCTRL2_TME2_TRHW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME2_TRHW_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME2_TRHW_SHIFT ) & \
      NANDFCTRL2_TME2_TRHW_MASK ) )
#define NANDFCTRL2_TME2_TRHW( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME2_TRHW_SHIFT ) & NANDFCTRL2_TME2_TRHW_MASK )

#define NANDFCTRL2_TME2_TWHR_SHIFT 0
#define NANDFCTRL2_TME2_TWHR_MASK  0xffffU
#define NANDFCTRL2_TME2_TWHR_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME2_TWHR_MASK ) >> NANDFCTRL2_TME2_TWHR_SHIFT )
#define NANDFCTRL2_TME2_TWHR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME2_TWHR_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME2_TWHR_SHIFT ) & \
      NANDFCTRL2_TME2_TWHR_MASK ) )
#define NANDFCTRL2_TME2_TWHR( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME2_TWHR_SHIFT ) & NANDFCTRL2_TME2_TWHR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME3 \
 *   Programmable timing 3 register (TME3)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME3_TADL_SHIFT 16
#define NANDFCTRL2_TME3_TADL_MASK  0xffff0000U
#define NANDFCTRL2_TME3_TADL_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME3_TADL_MASK ) >> NANDFCTRL2_TME3_TADL_SHIFT )
#define NANDFCTRL2_TME3_TADL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME3_TADL_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME3_TADL_SHIFT ) & \
      NANDFCTRL2_TME3_TADL_MASK ) )
#define NANDFCTRL2_TME3_TADL( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME3_TADL_SHIFT ) & NANDFCTRL2_TME3_TADL_MASK )

#define NANDFCTRL2_TME3_TCCS_SHIFT 0
#define NANDFCTRL2_TME3_TCCS_MASK  0xffffU
#define NANDFCTRL2_TME3_TCCS_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME3_TCCS_MASK ) >> NANDFCTRL2_TME3_TCCS_SHIFT )
#define NANDFCTRL2_TME3_TCCS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME3_TCCS_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME3_TCCS_SHIFT ) & \
      NANDFCTRL2_TME3_TCCS_MASK ) )
#define NANDFCTRL2_TME3_TCCS( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME3_TCCS_SHIFT ) & NANDFCTRL2_TME3_TCCS_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME4 \
 *   Programmable timing 4 register (TME4)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME4_TREH_SHIFT 16
#define NANDFCTRL2_TME4_TREH_MASK  0xffff0000U
#define NANDFCTRL2_TME4_TREH_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME4_TREH_MASK ) >> NANDFCTRL2_TME4_TREH_SHIFT )
#define NANDFCTRL2_TME4_TREH_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME4_TREH_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME4_TREH_SHIFT ) & \
      NANDFCTRL2_TME4_TREH_MASK ) )
#define NANDFCTRL2_TME4_TREH( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME4_TREH_SHIFT ) & NANDFCTRL2_TME4_TREH_MASK )

#define NANDFCTRL2_TME4_TRP_SHIFT 0
#define NANDFCTRL2_TME4_TRP_MASK  0xffffU
#define NANDFCTRL2_TME4_TRP_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME4_TRP_MASK ) >> NANDFCTRL2_TME4_TRP_SHIFT )
#define NANDFCTRL2_TME4_TRP_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME4_TRP_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME4_TRP_SHIFT ) & \
      NANDFCTRL2_TME4_TRP_MASK ) )
#define NANDFCTRL2_TME4_TRP( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME4_TRP_SHIFT ) & NANDFCTRL2_TME4_TRP_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME5 \
 *   Programmable timing 5 register (TME5)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME5_TWH_SHIFT 16
#define NANDFCTRL2_TME5_TWH_MASK  0xffff0000U
#define NANDFCTRL2_TME5_TWH_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME5_TWH_MASK ) >> NANDFCTRL2_TME5_TWH_SHIFT )
#define NANDFCTRL2_TME5_TWH_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME5_TWH_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME5_TWH_SHIFT ) & \
      NANDFCTRL2_TME5_TWH_MASK ) )
#define NANDFCTRL2_TME5_TWH( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME5_TWH_SHIFT ) & NANDFCTRL2_TME5_TWH_MASK )

#define NANDFCTRL2_TME5_TWP_SHIFT 0
#define NANDFCTRL2_TME5_TWP_MASK  0xffffU
#define NANDFCTRL2_TME5_TWP_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME5_TWP_MASK ) >> NANDFCTRL2_TME5_TWP_SHIFT )
#define NANDFCTRL2_TME5_TWP_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME5_TWP_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME5_TWP_SHIFT ) & \
      NANDFCTRL2_TME5_TWP_MASK ) )
#define NANDFCTRL2_TME5_TWP( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME5_TWP_SHIFT ) & NANDFCTRL2_TME5_TWP_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME6 \
 *   Programmable timing 6 register (TME6)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME6_TCALS_SHIFT 16
#define NANDFCTRL2_TME6_TCALS_MASK  0xffff0000U
#define NANDFCTRL2_TME6_TCALS_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME6_TCALS_MASK ) >> NANDFCTRL2_TME6_TCALS_SHIFT )
#define NANDFCTRL2_TME6_TCALS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME6_TCALS_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME6_TCALS_SHIFT ) & \
      NANDFCTRL2_TME6_TCALS_MASK ) )
#define NANDFCTRL2_TME6_TCALS( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME6_TCALS_SHIFT ) & NANDFCTRL2_TME6_TCALS_MASK )

#define NANDFCTRL2_TME6_TCSD_SHIFT 0
#define NANDFCTRL2_TME6_TCSD_MASK  0xffffU
#define NANDFCTRL2_TME6_TCSD_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME6_TCSD_MASK ) >> NANDFCTRL2_TME6_TCSD_SHIFT )
#define NANDFCTRL2_TME6_TCSD_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME6_TCSD_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME6_TCSD_SHIFT ) & \
      NANDFCTRL2_TME6_TCSD_MASK ) )
#define NANDFCTRL2_TME6_TCSD( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME6_TCSD_SHIFT ) & NANDFCTRL2_TME6_TCSD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME7 \
 *   Programmable timing 7 register (TME7)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME7_TCHZ_SHIFT 16
#define NANDFCTRL2_TME7_TCHZ_MASK  0xffff0000U
#define NANDFCTRL2_TME7_TCHZ_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME7_TCHZ_MASK ) >> NANDFCTRL2_TME7_TCHZ_SHIFT )
#define NANDFCTRL2_TME7_TCHZ_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME7_TCHZ_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME7_TCHZ_SHIFT ) & \
      NANDFCTRL2_TME7_TCHZ_MASK ) )
#define NANDFCTRL2_TME7_TCHZ( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME7_TCHZ_SHIFT ) & NANDFCTRL2_TME7_TCHZ_MASK )

#define NANDFCTRL2_TME7_TWPRE_SHIFT 0
#define NANDFCTRL2_TME7_TWPRE_MASK  0xffffU
#define NANDFCTRL2_TME7_TWPRE_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME7_TWPRE_MASK ) >> NANDFCTRL2_TME7_TWPRE_SHIFT )
#define NANDFCTRL2_TME7_TWPRE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME7_TWPRE_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME7_TWPRE_SHIFT ) & \
      NANDFCTRL2_TME7_TWPRE_MASK ) )
#define NANDFCTRL2_TME7_TWPRE( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME7_TWPRE_SHIFT ) & NANDFCTRL2_TME7_TWPRE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME8 \
 *   Programmable timing 8 register (TME8)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME8_TRPST_SHIFT 16
#define NANDFCTRL2_TME8_TRPST_MASK  0xffff0000U
#define NANDFCTRL2_TME8_TRPST_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME8_TRPST_MASK ) >> NANDFCTRL2_TME8_TRPST_SHIFT )
#define NANDFCTRL2_TME8_TRPST_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME8_TRPST_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME8_TRPST_SHIFT ) & \
      NANDFCTRL2_TME8_TRPST_MASK ) )
#define NANDFCTRL2_TME8_TRPST( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME8_TRPST_SHIFT ) & NANDFCTRL2_TME8_TRPST_MASK )

#define NANDFCTRL2_TME8_TDQSRH_SHIFT 0
#define NANDFCTRL2_TME8_TDQSRH_MASK  0xffffU
#define NANDFCTRL2_TME8_TDQSRH_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TME8_TDQSRH_MASK ) >> \
    NANDFCTRL2_TME8_TDQSRH_SHIFT )
#define NANDFCTRL2_TME8_TDQSRH_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME8_TDQSRH_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME8_TDQSRH_SHIFT ) & \
      NANDFCTRL2_TME8_TDQSRH_MASK ) )
#define NANDFCTRL2_TME8_TDQSRH( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TME8_TDQSRH_SHIFT ) & \
    NANDFCTRL2_TME8_TDQSRH_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME9 \
 *   Programmable timing 9 register (TME9)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME9_TWPST_SHIFT 16
#define NANDFCTRL2_TME9_TWPST_MASK  0xffff0000U
#define NANDFCTRL2_TME9_TWPST_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_TME9_TWPST_MASK ) >> NANDFCTRL2_TME9_TWPST_SHIFT )
#define NANDFCTRL2_TME9_TWPST_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME9_TWPST_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME9_TWPST_SHIFT ) & \
      NANDFCTRL2_TME9_TWPST_MASK ) )
#define NANDFCTRL2_TME9_TWPST( _val ) \
  ( ( ( _val ) << NANDFCTRL2_TME9_TWPST_SHIFT ) & NANDFCTRL2_TME9_TWPST_MASK )

#define NANDFCTRL2_TME9_TWPSTH_SHIFT 0
#define NANDFCTRL2_TME9_TWPSTH_MASK  0xffffU
#define NANDFCTRL2_TME9_TWPSTH_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TME9_TWPSTH_MASK ) >> \
    NANDFCTRL2_TME9_TWPSTH_SHIFT )
#define NANDFCTRL2_TME9_TWPSTH_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME9_TWPSTH_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME9_TWPSTH_SHIFT ) & \
      NANDFCTRL2_TME9_TWPSTH_MASK ) )
#define NANDFCTRL2_TME9_TWPSTH( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TME9_TWPSTH_SHIFT ) & \
    NANDFCTRL2_TME9_TWPSTH_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME10 \
 *   Programmable timing 10 register (TME10)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME10_TDQSL_SHIFT 16
#define NANDFCTRL2_TME10_TDQSL_MASK  0xffff0000U
#define NANDFCTRL2_TME10_TDQSL_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TME10_TDQSL_MASK ) >> \
    NANDFCTRL2_TME10_TDQSL_SHIFT )
#define NANDFCTRL2_TME10_TDQSL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME10_TDQSL_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME10_TDQSL_SHIFT ) & \
      NANDFCTRL2_TME10_TDQSL_MASK ) )
#define NANDFCTRL2_TME10_TDQSL( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TME10_TDQSL_SHIFT ) & \
    NANDFCTRL2_TME10_TDQSL_MASK )

#define NANDFCTRL2_TME10_TDQSH_SHIFT 0
#define NANDFCTRL2_TME10_TDQSH_MASK  0xffffU
#define NANDFCTRL2_TME10_TDQSH_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TME10_TDQSH_MASK ) >> \
    NANDFCTRL2_TME10_TDQSH_SHIFT )
#define NANDFCTRL2_TME10_TDQSH_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME10_TDQSH_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME10_TDQSH_SHIFT ) & \
      NANDFCTRL2_TME10_TDQSH_MASK ) )
#define NANDFCTRL2_TME10_TDQSH( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TME10_TDQSH_SHIFT ) & \
    NANDFCTRL2_TME10_TDQSH_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TME11 \
 *   Programmable timing 11 register (TME11)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TME11_TCS_VOL_SHIFT 16
#define NANDFCTRL2_TME11_TCS_VOL_MASK  0xffff0000U
#define NANDFCTRL2_TME11_TCS_VOL_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TME11_TCS_VOL_MASK ) >> \
    NANDFCTRL2_TME11_TCS_VOL_SHIFT )
#define NANDFCTRL2_TME11_TCS_VOL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME11_TCS_VOL_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME11_TCS_VOL_SHIFT ) & \
      NANDFCTRL2_TME11_TCS_VOL_MASK ) )
#define NANDFCTRL2_TME11_TCS_VOL( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TME11_TCS_VOL_SHIFT ) & \
    NANDFCTRL2_TME11_TCS_VOL_MASK )

#define NANDFCTRL2_TME11_TVDLY_SHIFT 0
#define NANDFCTRL2_TME11_TVDLY_MASK  0xffffU
#define NANDFCTRL2_TME11_TVDLY_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TME11_TVDLY_MASK ) >> \
    NANDFCTRL2_TME11_TVDLY_SHIFT )
#define NANDFCTRL2_TME11_TVDLY_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TME11_TVDLY_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TME11_TVDLY_SHIFT ) & \
      NANDFCTRL2_TME11_TVDLY_MASK ) )
#define NANDFCTRL2_TME11_TVDLY( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TME11_TVDLY_SHIFT ) & \
    NANDFCTRL2_TME11_TVDLY_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TXSKEW0 \
 *   Skew Control Transfer register (TXSKEW0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TXSKEW0_TXRE_SHIFT 27
#define NANDFCTRL2_TXSKEW0_TXRE_MASK  0x38000000U
#define NANDFCTRL2_TXSKEW0_TXRE_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TXSKEW0_TXRE_MASK ) >> \
    NANDFCTRL2_TXSKEW0_TXRE_SHIFT )
#define NANDFCTRL2_TXSKEW0_TXRE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TXSKEW0_TXRE_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXRE_SHIFT ) & \
      NANDFCTRL2_TXSKEW0_TXRE_MASK ) )
#define NANDFCTRL2_TXSKEW0_TXRE( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXRE_SHIFT ) & \
    NANDFCTRL2_TXSKEW0_TXRE_MASK )

#define NANDFCTRL2_TXSKEW0_TXDQS_SHIFT 24
#define NANDFCTRL2_TXSKEW0_TXDQS_MASK  0x7000000U
#define NANDFCTRL2_TXSKEW0_TXDQS_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TXSKEW0_TXDQS_MASK ) >> \
    NANDFCTRL2_TXSKEW0_TXDQS_SHIFT )
#define NANDFCTRL2_TXSKEW0_TXDQS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TXSKEW0_TXDQS_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXDQS_SHIFT ) & \
      NANDFCTRL2_TXSKEW0_TXDQS_MASK ) )
#define NANDFCTRL2_TXSKEW0_TXDQS( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXDQS_SHIFT ) & \
    NANDFCTRL2_TXSKEW0_TXDQS_MASK )

#define NANDFCTRL2_TXSKEW0_TXD7_SHIFT 21
#define NANDFCTRL2_TXSKEW0_TXD7_MASK  0xe00000U
#define NANDFCTRL2_TXSKEW0_TXD7_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TXSKEW0_TXD7_MASK ) >> \
    NANDFCTRL2_TXSKEW0_TXD7_SHIFT )
#define NANDFCTRL2_TXSKEW0_TXD7_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TXSKEW0_TXD7_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD7_SHIFT ) & \
      NANDFCTRL2_TXSKEW0_TXD7_MASK ) )
#define NANDFCTRL2_TXSKEW0_TXD7( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD7_SHIFT ) & \
    NANDFCTRL2_TXSKEW0_TXD7_MASK )

#define NANDFCTRL2_TXSKEW0_TXD6_SHIFT 18
#define NANDFCTRL2_TXSKEW0_TXD6_MASK  0x1c0000U
#define NANDFCTRL2_TXSKEW0_TXD6_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TXSKEW0_TXD6_MASK ) >> \
    NANDFCTRL2_TXSKEW0_TXD6_SHIFT )
#define NANDFCTRL2_TXSKEW0_TXD6_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TXSKEW0_TXD6_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD6_SHIFT ) & \
      NANDFCTRL2_TXSKEW0_TXD6_MASK ) )
#define NANDFCTRL2_TXSKEW0_TXD6( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD6_SHIFT ) & \
    NANDFCTRL2_TXSKEW0_TXD6_MASK )

#define NANDFCTRL2_TXSKEW0_TXD5_SHIFT 15
#define NANDFCTRL2_TXSKEW0_TXD5_MASK  0x38000U
#define NANDFCTRL2_TXSKEW0_TXD5_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TXSKEW0_TXD5_MASK ) >> \
    NANDFCTRL2_TXSKEW0_TXD5_SHIFT )
#define NANDFCTRL2_TXSKEW0_TXD5_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TXSKEW0_TXD5_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD5_SHIFT ) & \
      NANDFCTRL2_TXSKEW0_TXD5_MASK ) )
#define NANDFCTRL2_TXSKEW0_TXD5( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD5_SHIFT ) & \
    NANDFCTRL2_TXSKEW0_TXD5_MASK )

#define NANDFCTRL2_TXSKEW0_TXD4_SHIFT 12
#define NANDFCTRL2_TXSKEW0_TXD4_MASK  0x7000U
#define NANDFCTRL2_TXSKEW0_TXD4_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TXSKEW0_TXD4_MASK ) >> \
    NANDFCTRL2_TXSKEW0_TXD4_SHIFT )
#define NANDFCTRL2_TXSKEW0_TXD4_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TXSKEW0_TXD4_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD4_SHIFT ) & \
      NANDFCTRL2_TXSKEW0_TXD4_MASK ) )
#define NANDFCTRL2_TXSKEW0_TXD4( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD4_SHIFT ) & \
    NANDFCTRL2_TXSKEW0_TXD4_MASK )

#define NANDFCTRL2_TXSKEW0_TXD3_SHIFT 9
#define NANDFCTRL2_TXSKEW0_TXD3_MASK  0xe00U
#define NANDFCTRL2_TXSKEW0_TXD3_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TXSKEW0_TXD3_MASK ) >> \
    NANDFCTRL2_TXSKEW0_TXD3_SHIFT )
#define NANDFCTRL2_TXSKEW0_TXD3_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TXSKEW0_TXD3_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD3_SHIFT ) & \
      NANDFCTRL2_TXSKEW0_TXD3_MASK ) )
#define NANDFCTRL2_TXSKEW0_TXD3( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD3_SHIFT ) & \
    NANDFCTRL2_TXSKEW0_TXD3_MASK )

#define NANDFCTRL2_TXSKEW0_TXD2_SHIFT 6
#define NANDFCTRL2_TXSKEW0_TXD2_MASK  0x1c0U
#define NANDFCTRL2_TXSKEW0_TXD2_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TXSKEW0_TXD2_MASK ) >> \
    NANDFCTRL2_TXSKEW0_TXD2_SHIFT )
#define NANDFCTRL2_TXSKEW0_TXD2_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TXSKEW0_TXD2_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD2_SHIFT ) & \
      NANDFCTRL2_TXSKEW0_TXD2_MASK ) )
#define NANDFCTRL2_TXSKEW0_TXD2( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD2_SHIFT ) & \
    NANDFCTRL2_TXSKEW0_TXD2_MASK )

#define NANDFCTRL2_TXSKEW0_TXD1_SHIFT 3
#define NANDFCTRL2_TXSKEW0_TXD1_MASK  0x38U
#define NANDFCTRL2_TXSKEW0_TXD1_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TXSKEW0_TXD1_MASK ) >> \
    NANDFCTRL2_TXSKEW0_TXD1_SHIFT )
#define NANDFCTRL2_TXSKEW0_TXD1_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TXSKEW0_TXD1_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD1_SHIFT ) & \
      NANDFCTRL2_TXSKEW0_TXD1_MASK ) )
#define NANDFCTRL2_TXSKEW0_TXD1( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD1_SHIFT ) & \
    NANDFCTRL2_TXSKEW0_TXD1_MASK )

#define NANDFCTRL2_TXSKEW0_TXD0_SHIFT 0
#define NANDFCTRL2_TXSKEW0_TXD0_MASK  0x7U
#define NANDFCTRL2_TXSKEW0_TXD0_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TXSKEW0_TXD0_MASK ) >> \
    NANDFCTRL2_TXSKEW0_TXD0_SHIFT )
#define NANDFCTRL2_TXSKEW0_TXD0_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TXSKEW0_TXD0_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD0_SHIFT ) & \
      NANDFCTRL2_TXSKEW0_TXD0_MASK ) )
#define NANDFCTRL2_TXSKEW0_TXD0( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TXSKEW0_TXD0_SHIFT ) & \
    NANDFCTRL2_TXSKEW0_TXD0_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2RXSKEW0 \
 *   Skew control receive register (RXSKEW0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_RXSKEW0_RXDQS_SHIFT 24
#define NANDFCTRL2_RXSKEW0_RXDQS_MASK  0x7000000U
#define NANDFCTRL2_RXSKEW0_RXDQS_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_RXSKEW0_RXDQS_MASK ) >> \
    NANDFCTRL2_RXSKEW0_RXDQS_SHIFT )
#define NANDFCTRL2_RXSKEW0_RXDQS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_RXSKEW0_RXDQS_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXDQS_SHIFT ) & \
      NANDFCTRL2_RXSKEW0_RXDQS_MASK ) )
#define NANDFCTRL2_RXSKEW0_RXDQS( _val )             \
  ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXDQS_SHIFT ) & \
    NANDFCTRL2_RXSKEW0_RXDQS_MASK )

#define NANDFCTRL2_RXSKEW0_RXD7_SHIFT 21
#define NANDFCTRL2_RXSKEW0_RXD7_MASK  0xe00000U
#define NANDFCTRL2_RXSKEW0_RXD7_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_RXSKEW0_RXD7_MASK ) >> \
    NANDFCTRL2_RXSKEW0_RXD7_SHIFT )
#define NANDFCTRL2_RXSKEW0_RXD7_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_RXSKEW0_RXD7_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD7_SHIFT ) & \
      NANDFCTRL2_RXSKEW0_RXD7_MASK ) )
#define NANDFCTRL2_RXSKEW0_RXD7( _val )             \
  ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD7_SHIFT ) & \
    NANDFCTRL2_RXSKEW0_RXD7_MASK )

#define NANDFCTRL2_RXSKEW0_RXD6_SHIFT 18
#define NANDFCTRL2_RXSKEW0_RXD6_MASK  0x1c0000U
#define NANDFCTRL2_RXSKEW0_RXD6_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_RXSKEW0_RXD6_MASK ) >> \
    NANDFCTRL2_RXSKEW0_RXD6_SHIFT )
#define NANDFCTRL2_RXSKEW0_RXD6_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_RXSKEW0_RXD6_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD6_SHIFT ) & \
      NANDFCTRL2_RXSKEW0_RXD6_MASK ) )
#define NANDFCTRL2_RXSKEW0_RXD6( _val )             \
  ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD6_SHIFT ) & \
    NANDFCTRL2_RXSKEW0_RXD6_MASK )

#define NANDFCTRL2_RXSKEW0_RXD5_SHIFT 15
#define NANDFCTRL2_RXSKEW0_RXD5_MASK  0x38000U
#define NANDFCTRL2_RXSKEW0_RXD5_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_RXSKEW0_RXD5_MASK ) >> \
    NANDFCTRL2_RXSKEW0_RXD5_SHIFT )
#define NANDFCTRL2_RXSKEW0_RXD5_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_RXSKEW0_RXD5_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD5_SHIFT ) & \
      NANDFCTRL2_RXSKEW0_RXD5_MASK ) )
#define NANDFCTRL2_RXSKEW0_RXD5( _val )             \
  ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD5_SHIFT ) & \
    NANDFCTRL2_RXSKEW0_RXD5_MASK )

#define NANDFCTRL2_RXSKEW0_RXD4_SHIFT 12
#define NANDFCTRL2_RXSKEW0_RXD4_MASK  0x7000U
#define NANDFCTRL2_RXSKEW0_RXD4_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_RXSKEW0_RXD4_MASK ) >> \
    NANDFCTRL2_RXSKEW0_RXD4_SHIFT )
#define NANDFCTRL2_RXSKEW0_RXD4_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_RXSKEW0_RXD4_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD4_SHIFT ) & \
      NANDFCTRL2_RXSKEW0_RXD4_MASK ) )
#define NANDFCTRL2_RXSKEW0_RXD4( _val )             \
  ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD4_SHIFT ) & \
    NANDFCTRL2_RXSKEW0_RXD4_MASK )

#define NANDFCTRL2_RXSKEW0_RXD3_SHIFT 9
#define NANDFCTRL2_RXSKEW0_RXD3_MASK  0xe00U
#define NANDFCTRL2_RXSKEW0_RXD3_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_RXSKEW0_RXD3_MASK ) >> \
    NANDFCTRL2_RXSKEW0_RXD3_SHIFT )
#define NANDFCTRL2_RXSKEW0_RXD3_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_RXSKEW0_RXD3_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD3_SHIFT ) & \
      NANDFCTRL2_RXSKEW0_RXD3_MASK ) )
#define NANDFCTRL2_RXSKEW0_RXD3( _val )             \
  ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD3_SHIFT ) & \
    NANDFCTRL2_RXSKEW0_RXD3_MASK )

#define NANDFCTRL2_RXSKEW0_RXD2_SHIFT 6
#define NANDFCTRL2_RXSKEW0_RXD2_MASK  0x1c0U
#define NANDFCTRL2_RXSKEW0_RXD2_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_RXSKEW0_RXD2_MASK ) >> \
    NANDFCTRL2_RXSKEW0_RXD2_SHIFT )
#define NANDFCTRL2_RXSKEW0_RXD2_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_RXSKEW0_RXD2_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD2_SHIFT ) & \
      NANDFCTRL2_RXSKEW0_RXD2_MASK ) )
#define NANDFCTRL2_RXSKEW0_RXD2( _val )             \
  ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD2_SHIFT ) & \
    NANDFCTRL2_RXSKEW0_RXD2_MASK )

#define NANDFCTRL2_RXSKEW0_RXD1_SHIFT 3
#define NANDFCTRL2_RXSKEW0_RXD1_MASK  0x38U
#define NANDFCTRL2_RXSKEW0_RXD1_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_RXSKEW0_RXD1_MASK ) >> \
    NANDFCTRL2_RXSKEW0_RXD1_SHIFT )
#define NANDFCTRL2_RXSKEW0_RXD1_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_RXSKEW0_RXD1_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD1_SHIFT ) & \
      NANDFCTRL2_RXSKEW0_RXD1_MASK ) )
#define NANDFCTRL2_RXSKEW0_RXD1( _val )             \
  ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD1_SHIFT ) & \
    NANDFCTRL2_RXSKEW0_RXD1_MASK )

#define NANDFCTRL2_RXSKEW0_RXD0_SHIFT 0
#define NANDFCTRL2_RXSKEW0_RXD0_MASK  0x7U
#define NANDFCTRL2_RXSKEW0_RXD0_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_RXSKEW0_RXD0_MASK ) >> \
    NANDFCTRL2_RXSKEW0_RXD0_SHIFT )
#define NANDFCTRL2_RXSKEW0_RXD0_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_RXSKEW0_RXD0_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD0_SHIFT ) & \
      NANDFCTRL2_RXSKEW0_RXD0_MASK ) )
#define NANDFCTRL2_RXSKEW0_RXD0( _val )             \
  ( ( ( _val ) << NANDFCTRL2_RXSKEW0_RXD0_SHIFT ) & \
    NANDFCTRL2_RXSKEW0_RXD0_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2TMOUT0 Timeout 0 register (TMOUT0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_TMOUT0_TIMEOUT_SHIFT 0
#define NANDFCTRL2_TMOUT0_TIMEOUT_MASK  0xffffffffU
#define NANDFCTRL2_TMOUT0_TIMEOUT_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_TMOUT0_TIMEOUT_MASK ) >> \
    NANDFCTRL2_TMOUT0_TIMEOUT_SHIFT )
#define NANDFCTRL2_TMOUT0_TIMEOUT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_TMOUT0_TIMEOUT_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_TMOUT0_TIMEOUT_SHIFT ) & \
      NANDFCTRL2_TMOUT0_TIMEOUT_MASK ) )
#define NANDFCTRL2_TMOUT0_TIMEOUT( _val )             \
  ( ( ( _val ) << NANDFCTRL2_TMOUT0_TIMEOUT_SHIFT ) & \
    NANDFCTRL2_TMOUT0_TIMEOUT_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2LLPL \
 *   Linked list pointer low register (LLPL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_LLPL_LINKED_LIST_POINTER_SHIFT 0
#define NANDFCTRL2_LLPL_LINKED_LIST_POINTER_MASK  0xffffffffU
#define NANDFCTRL2_LLPL_LINKED_LIST_POINTER_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_LLPL_LINKED_LIST_POINTER_MASK ) >> \
    NANDFCTRL2_LLPL_LINKED_LIST_POINTER_SHIFT )
#define NANDFCTRL2_LLPL_LINKED_LIST_POINTER_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_LLPL_LINKED_LIST_POINTER_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_LLPL_LINKED_LIST_POINTER_SHIFT ) & \
      NANDFCTRL2_LLPL_LINKED_LIST_POINTER_MASK ) )
#define NANDFCTRL2_LLPL_LINKED_LIST_POINTER( _val )             \
  ( ( ( _val ) << NANDFCTRL2_LLPL_LINKED_LIST_POINTER_SHIFT ) & \
    NANDFCTRL2_LLPL_LINKED_LIST_POINTER_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DCMD Descriptor command register (DCMD)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DCMD_CMD2_SHIFT 24
#define NANDFCTRL2_DCMD_CMD2_MASK  0xff000000U
#define NANDFCTRL2_DCMD_CMD2_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_DCMD_CMD2_MASK ) >> NANDFCTRL2_DCMD_CMD2_SHIFT )
#define NANDFCTRL2_DCMD_CMD2_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DCMD_CMD2_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DCMD_CMD2_SHIFT ) & \
      NANDFCTRL2_DCMD_CMD2_MASK ) )
#define NANDFCTRL2_DCMD_CMD2( _val ) \
  ( ( ( _val ) << NANDFCTRL2_DCMD_CMD2_SHIFT ) & NANDFCTRL2_DCMD_CMD2_MASK )

#define NANDFCTRL2_DCMD_CMD1_SHIFT 16
#define NANDFCTRL2_DCMD_CMD1_MASK  0xff0000U
#define NANDFCTRL2_DCMD_CMD1_GET( _reg ) \
  ( ( ( _reg ) & NANDFCTRL2_DCMD_CMD1_MASK ) >> NANDFCTRL2_DCMD_CMD1_SHIFT )
#define NANDFCTRL2_DCMD_CMD1_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DCMD_CMD1_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DCMD_CMD1_SHIFT ) & \
      NANDFCTRL2_DCMD_CMD1_MASK ) )
#define NANDFCTRL2_DCMD_CMD1( _val ) \
  ( ( ( _val ) << NANDFCTRL2_DCMD_CMD1_SHIFT ) & NANDFCTRL2_DCMD_CMD1_MASK )

#define NANDFCTRL2_DCMD_IRQ_UERRDL 0x8000U

#define NANDFCTRL2_DCMD_IRQ_UERRUL 0x4000U

#define NANDFCTRL2_DCMD_IRQ_ECC 0x2000U

#define NANDFCTRL2_DCMD_PRECH 0x1000U

#define NANDFCTRL2_DCMD_DD 0x800U

#define NANDFCTRL2_DCMD_ED 0x400U

#define NANDFCTRL2_DCMD_RD 0x200U

#define NANDFCTRL2_DCMD_SUB_CMD_SHIFT 7
#define NANDFCTRL2_DCMD_SUB_CMD_MASK  0x180U
#define NANDFCTRL2_DCMD_SUB_CMD_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DCMD_SUB_CMD_MASK ) >> \
    NANDFCTRL2_DCMD_SUB_CMD_SHIFT )
#define NANDFCTRL2_DCMD_SUB_CMD_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DCMD_SUB_CMD_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DCMD_SUB_CMD_SHIFT ) & \
      NANDFCTRL2_DCMD_SUB_CMD_MASK ) )
#define NANDFCTRL2_DCMD_SUB_CMD( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DCMD_SUB_CMD_SHIFT ) & \
    NANDFCTRL2_DCMD_SUB_CMD_MASK )
#define NANDFCTRL2_DCMD_SUB_CMD_ORDINARY            0U
#define NANDFCTRL2_DCMD_SUB_CMD_SMALL_DATA_MOVE     1U
#define NANDFCTRL2_DCMD_SUB_CMD_CHANGE_WRITE_COLUMN 2U
#define NANDFCTRL2_DCMD_SUB_CMD_CHANGE_ROW_ADDRESS  3U

#define NANDFCTRL2_DCMD_SRB 0x40U

#define NANDFCTRL2_DCMD_SA 0x20U

#define NANDFCTRL2_DCMD_SD 0x10U

#define NANDFCTRL2_DCMD_SC2 0x8U

#define NANDFCTRL2_DCMD_WARDY 0x4U

#define NANDFCTRL2_DCMD_IRQ_DS 0x2U

#define NANDFCTRL2_DCMD_EN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DTARSEL0 \
 *   Descriptor target 0 select register (DTARSEL0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DTARSEL0_TARGSEL_SHIFT 0
#define NANDFCTRL2_DTARSEL0_TARGSEL_MASK  0xffffffffU
#define NANDFCTRL2_DTARSEL0_TARGSEL_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DTARSEL0_TARGSEL_MASK ) >> \
    NANDFCTRL2_DTARSEL0_TARGSEL_SHIFT )
#define NANDFCTRL2_DTARSEL0_TARGSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DTARSEL0_TARGSEL_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DTARSEL0_TARGSEL_SHIFT ) & \
      NANDFCTRL2_DTARSEL0_TARGSEL_MASK ) )
#define NANDFCTRL2_DTARSEL0_TARGSEL( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DTARSEL0_TARGSEL_SHIFT ) & \
    NANDFCTRL2_DTARSEL0_TARGSEL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DTARSEL1 \
 *   Descriptor target 1 select register (DTARSEL1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DTARSEL1_TARGSEL_SHIFT 0
#define NANDFCTRL2_DTARSEL1_TARGSEL_MASK  0xffffffffU
#define NANDFCTRL2_DTARSEL1_TARGSEL_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DTARSEL1_TARGSEL_MASK ) >> \
    NANDFCTRL2_DTARSEL1_TARGSEL_SHIFT )
#define NANDFCTRL2_DTARSEL1_TARGSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DTARSEL1_TARGSEL_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DTARSEL1_TARGSEL_SHIFT ) & \
      NANDFCTRL2_DTARSEL1_TARGSEL_MASK ) )
#define NANDFCTRL2_DTARSEL1_TARGSEL( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DTARSEL1_TARGSEL_SHIFT ) & \
    NANDFCTRL2_DTARSEL1_TARGSEL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DCHSEL \
 *   Descriptor channel select register (DCHSEL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DCHSEL_CHSEL_SHIFT 0
#define NANDFCTRL2_DCHSEL_CHSEL_MASK  0xffffU
#define NANDFCTRL2_DCHSEL_CHSEL_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DCHSEL_CHSEL_MASK ) >> \
    NANDFCTRL2_DCHSEL_CHSEL_SHIFT )
#define NANDFCTRL2_DCHSEL_CHSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DCHSEL_CHSEL_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DCHSEL_CHSEL_SHIFT ) & \
      NANDFCTRL2_DCHSEL_CHSEL_MASK ) )
#define NANDFCTRL2_DCHSEL_CHSEL( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DCHSEL_CHSEL_SHIFT ) & \
    NANDFCTRL2_DCHSEL_CHSEL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DRBSEL \
 *   Descriptor ready/busy select register (DRBSEL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DRBSEL_RBSEL_SHIFT 0
#define NANDFCTRL2_DRBSEL_RBSEL_MASK  0xffffffffU
#define NANDFCTRL2_DRBSEL_RBSEL_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DRBSEL_RBSEL_MASK ) >> \
    NANDFCTRL2_DRBSEL_RBSEL_SHIFT )
#define NANDFCTRL2_DRBSEL_RBSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DRBSEL_RBSEL_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DRBSEL_RBSEL_SHIFT ) & \
      NANDFCTRL2_DRBSEL_RBSEL_MASK ) )
#define NANDFCTRL2_DRBSEL_RBSEL( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DRBSEL_RBSEL_SHIFT ) & \
    NANDFCTRL2_DRBSEL_RBSEL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DROW \
 *   Descriptor row & control register (DROW)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DROW_TAGEN 0x1000000U

#define NANDFCTRL2_DROW_ROWADDR_SHIFT 0
#define NANDFCTRL2_DROW_ROWADDR_MASK  0xffffffU
#define NANDFCTRL2_DROW_ROWADDR_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DROW_ROWADDR_MASK ) >> \
    NANDFCTRL2_DROW_ROWADDR_SHIFT )
#define NANDFCTRL2_DROW_ROWADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DROW_ROWADDR_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DROW_ROWADDR_SHIFT ) & \
      NANDFCTRL2_DROW_ROWADDR_MASK ) )
#define NANDFCTRL2_DROW_ROWADDR( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DROW_ROWADDR_SHIFT ) & \
    NANDFCTRL2_DROW_ROWADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DCOLSIZE \
 *   Descriptor column & size register (DCOLSIZE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DCOLSIZE_SIZE_SHIFT 16
#define NANDFCTRL2_DCOLSIZE_SIZE_MASK  0xffff0000U
#define NANDFCTRL2_DCOLSIZE_SIZE_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DCOLSIZE_SIZE_MASK ) >> \
    NANDFCTRL2_DCOLSIZE_SIZE_SHIFT )
#define NANDFCTRL2_DCOLSIZE_SIZE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DCOLSIZE_SIZE_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DCOLSIZE_SIZE_SHIFT ) & \
      NANDFCTRL2_DCOLSIZE_SIZE_MASK ) )
#define NANDFCTRL2_DCOLSIZE_SIZE( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DCOLSIZE_SIZE_SHIFT ) & \
    NANDFCTRL2_DCOLSIZE_SIZE_MASK )

#define NANDFCTRL2_DCOLSIZE_COLADDR_SHIFT 0
#define NANDFCTRL2_DCOLSIZE_COLADDR_MASK  0xffffU
#define NANDFCTRL2_DCOLSIZE_COLADDR_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DCOLSIZE_COLADDR_MASK ) >> \
    NANDFCTRL2_DCOLSIZE_COLADDR_SHIFT )
#define NANDFCTRL2_DCOLSIZE_COLADDR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DCOLSIZE_COLADDR_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DCOLSIZE_COLADDR_SHIFT ) & \
      NANDFCTRL2_DCOLSIZE_COLADDR_MASK ) )
#define NANDFCTRL2_DCOLSIZE_COLADDR( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DCOLSIZE_COLADDR_SHIFT ) & \
    NANDFCTRL2_DCOLSIZE_COLADDR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DSTS Descriptor status register (DSTS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DSTS_ECFAIL_SHIFT 16
#define NANDFCTRL2_DSTS_ECFAIL_MASK  0xffff0000U
#define NANDFCTRL2_DSTS_ECFAIL_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DSTS_ECFAIL_MASK ) >> \
    NANDFCTRL2_DSTS_ECFAIL_SHIFT )
#define NANDFCTRL2_DSTS_ECFAIL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DSTS_ECFAIL_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DSTS_ECFAIL_SHIFT ) & \
      NANDFCTRL2_DSTS_ECFAIL_MASK ) )
#define NANDFCTRL2_DSTS_ECFAIL( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DSTS_ECFAIL_SHIFT ) & \
    NANDFCTRL2_DSTS_ECFAIL_MASK )

#define NANDFCTRL2_DSTS_CERR_DL 0x100U

#define NANDFCTRL2_DSTS_CERR_UL 0x80U

#define NANDFCTRL2_DSTS_UCERR_DL 0x40U

#define NANDFCTRL2_DSTS_UCERR_UL 0x20U

#define NANDFCTRL2_DSTS_TMOUT 0x10U

#define NANDFCTRL2_DSTS_STOP_LL 0x8U

#define NANDFCTRL2_DSTS_ABORT 0x4U

#define NANDFCTRL2_DSTS_UE 0x2U

#define NANDFCTRL2_DSTS_INV 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DECCSTS0 \
 *   Descriptor ECC status register (DECCSTS0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DECCSTS0_CEC_SHIFT 16
#define NANDFCTRL2_DECCSTS0_CEC_MASK  0xff0000U
#define NANDFCTRL2_DECCSTS0_CEC_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DECCSTS0_CEC_MASK ) >> \
    NANDFCTRL2_DECCSTS0_CEC_SHIFT )
#define NANDFCTRL2_DECCSTS0_CEC_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DECCSTS0_CEC_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DECCSTS0_CEC_SHIFT ) & \
      NANDFCTRL2_DECCSTS0_CEC_MASK ) )
#define NANDFCTRL2_DECCSTS0_CEC( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DECCSTS0_CEC_SHIFT ) & \
    NANDFCTRL2_DECCSTS0_CEC_MASK )

#define NANDFCTRL2_DECCSTS0_CE_SHIFT 0
#define NANDFCTRL2_DECCSTS0_CE_MASK  0xfffU
#define NANDFCTRL2_DECCSTS0_CE_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DECCSTS0_CE_MASK ) >> \
    NANDFCTRL2_DECCSTS0_CE_SHIFT )
#define NANDFCTRL2_DECCSTS0_CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DECCSTS0_CE_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DECCSTS0_CE_SHIFT ) & \
      NANDFCTRL2_DECCSTS0_CE_MASK ) )
#define NANDFCTRL2_DECCSTS0_CE( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DECCSTS0_CE_SHIFT ) & \
    NANDFCTRL2_DECCSTS0_CE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DDPL \
 *   Descriptor data pointer low register (DDPL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DDPL_DATA_POINTER_SHIFT 0
#define NANDFCTRL2_DDPL_DATA_POINTER_MASK  0xffffffffU
#define NANDFCTRL2_DDPL_DATA_POINTER_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DDPL_DATA_POINTER_MASK ) >> \
    NANDFCTRL2_DDPL_DATA_POINTER_SHIFT )
#define NANDFCTRL2_DDPL_DATA_POINTER_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DDPL_DATA_POINTER_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DDPL_DATA_POINTER_SHIFT ) & \
      NANDFCTRL2_DDPL_DATA_POINTER_MASK ) )
#define NANDFCTRL2_DDPL_DATA_POINTER( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DDPL_DATA_POINTER_SHIFT ) & \
    NANDFCTRL2_DDPL_DATA_POINTER_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DATAIN0 Data-in 0 register (DATAIN0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DATAIN0_DATA_3_SHIFT 24
#define NANDFCTRL2_DATAIN0_DATA_3_MASK  0xff000000U
#define NANDFCTRL2_DATAIN0_DATA_3_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN0_DATA_3_MASK ) >> \
    NANDFCTRL2_DATAIN0_DATA_3_SHIFT )
#define NANDFCTRL2_DATAIN0_DATA_3_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN0_DATA_3_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN0_DATA_3_SHIFT ) & \
      NANDFCTRL2_DATAIN0_DATA_3_MASK ) )
#define NANDFCTRL2_DATAIN0_DATA_3( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN0_DATA_3_SHIFT ) & \
    NANDFCTRL2_DATAIN0_DATA_3_MASK )

#define NANDFCTRL2_DATAIN0_DATA_2_SHIFT 16
#define NANDFCTRL2_DATAIN0_DATA_2_MASK  0xff0000U
#define NANDFCTRL2_DATAIN0_DATA_2_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN0_DATA_2_MASK ) >> \
    NANDFCTRL2_DATAIN0_DATA_2_SHIFT )
#define NANDFCTRL2_DATAIN0_DATA_2_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN0_DATA_2_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN0_DATA_2_SHIFT ) & \
      NANDFCTRL2_DATAIN0_DATA_2_MASK ) )
#define NANDFCTRL2_DATAIN0_DATA_2( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN0_DATA_2_SHIFT ) & \
    NANDFCTRL2_DATAIN0_DATA_2_MASK )

#define NANDFCTRL2_DATAIN0_DATA_1_SHIFT 8
#define NANDFCTRL2_DATAIN0_DATA_1_MASK  0xff00U
#define NANDFCTRL2_DATAIN0_DATA_1_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN0_DATA_1_MASK ) >> \
    NANDFCTRL2_DATAIN0_DATA_1_SHIFT )
#define NANDFCTRL2_DATAIN0_DATA_1_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN0_DATA_1_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN0_DATA_1_SHIFT ) & \
      NANDFCTRL2_DATAIN0_DATA_1_MASK ) )
#define NANDFCTRL2_DATAIN0_DATA_1( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN0_DATA_1_SHIFT ) & \
    NANDFCTRL2_DATAIN0_DATA_1_MASK )

#define NANDFCTRL2_DATAIN0_DATA_0_SHIFT 0
#define NANDFCTRL2_DATAIN0_DATA_0_MASK  0xffU
#define NANDFCTRL2_DATAIN0_DATA_0_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN0_DATA_0_MASK ) >> \
    NANDFCTRL2_DATAIN0_DATA_0_SHIFT )
#define NANDFCTRL2_DATAIN0_DATA_0_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN0_DATA_0_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN0_DATA_0_SHIFT ) & \
      NANDFCTRL2_DATAIN0_DATA_0_MASK ) )
#define NANDFCTRL2_DATAIN0_DATA_0( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN0_DATA_0_SHIFT ) & \
    NANDFCTRL2_DATAIN0_DATA_0_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DATAIN1 Data-in 1 register (DATAIN1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DATAIN1_DATA_7_SHIFT 24
#define NANDFCTRL2_DATAIN1_DATA_7_MASK  0xff000000U
#define NANDFCTRL2_DATAIN1_DATA_7_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN1_DATA_7_MASK ) >> \
    NANDFCTRL2_DATAIN1_DATA_7_SHIFT )
#define NANDFCTRL2_DATAIN1_DATA_7_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN1_DATA_7_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN1_DATA_7_SHIFT ) & \
      NANDFCTRL2_DATAIN1_DATA_7_MASK ) )
#define NANDFCTRL2_DATAIN1_DATA_7( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN1_DATA_7_SHIFT ) & \
    NANDFCTRL2_DATAIN1_DATA_7_MASK )

#define NANDFCTRL2_DATAIN1_DATA_6_SHIFT 16
#define NANDFCTRL2_DATAIN1_DATA_6_MASK  0xff0000U
#define NANDFCTRL2_DATAIN1_DATA_6_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN1_DATA_6_MASK ) >> \
    NANDFCTRL2_DATAIN1_DATA_6_SHIFT )
#define NANDFCTRL2_DATAIN1_DATA_6_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN1_DATA_6_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN1_DATA_6_SHIFT ) & \
      NANDFCTRL2_DATAIN1_DATA_6_MASK ) )
#define NANDFCTRL2_DATAIN1_DATA_6( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN1_DATA_6_SHIFT ) & \
    NANDFCTRL2_DATAIN1_DATA_6_MASK )

#define NANDFCTRL2_DATAIN1_DATA_5_SHIFT 8
#define NANDFCTRL2_DATAIN1_DATA_5_MASK  0xff00U
#define NANDFCTRL2_DATAIN1_DATA_5_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN1_DATA_5_MASK ) >> \
    NANDFCTRL2_DATAIN1_DATA_5_SHIFT )
#define NANDFCTRL2_DATAIN1_DATA_5_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN1_DATA_5_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN1_DATA_5_SHIFT ) & \
      NANDFCTRL2_DATAIN1_DATA_5_MASK ) )
#define NANDFCTRL2_DATAIN1_DATA_5( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN1_DATA_5_SHIFT ) & \
    NANDFCTRL2_DATAIN1_DATA_5_MASK )

#define NANDFCTRL2_DATAIN1_DATA_4_SHIFT 0
#define NANDFCTRL2_DATAIN1_DATA_4_MASK  0xffU
#define NANDFCTRL2_DATAIN1_DATA_4_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN1_DATA_4_MASK ) >> \
    NANDFCTRL2_DATAIN1_DATA_4_SHIFT )
#define NANDFCTRL2_DATAIN1_DATA_4_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN1_DATA_4_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN1_DATA_4_SHIFT ) & \
      NANDFCTRL2_DATAIN1_DATA_4_MASK ) )
#define NANDFCTRL2_DATAIN1_DATA_4( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN1_DATA_4_SHIFT ) & \
    NANDFCTRL2_DATAIN1_DATA_4_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DATAIN2 Data-in 2 register (DATAIN2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DATAIN2_DATA_11_SHIFT 24
#define NANDFCTRL2_DATAIN2_DATA_11_MASK  0xff000000U
#define NANDFCTRL2_DATAIN2_DATA_11_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN2_DATA_11_MASK ) >> \
    NANDFCTRL2_DATAIN2_DATA_11_SHIFT )
#define NANDFCTRL2_DATAIN2_DATA_11_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN2_DATA_11_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN2_DATA_11_SHIFT ) & \
      NANDFCTRL2_DATAIN2_DATA_11_MASK ) )
#define NANDFCTRL2_DATAIN2_DATA_11( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN2_DATA_11_SHIFT ) & \
    NANDFCTRL2_DATAIN2_DATA_11_MASK )

#define NANDFCTRL2_DATAIN2_DATA_10_SHIFT 16
#define NANDFCTRL2_DATAIN2_DATA_10_MASK  0xff0000U
#define NANDFCTRL2_DATAIN2_DATA_10_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN2_DATA_10_MASK ) >> \
    NANDFCTRL2_DATAIN2_DATA_10_SHIFT )
#define NANDFCTRL2_DATAIN2_DATA_10_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN2_DATA_10_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN2_DATA_10_SHIFT ) & \
      NANDFCTRL2_DATAIN2_DATA_10_MASK ) )
#define NANDFCTRL2_DATAIN2_DATA_10( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN2_DATA_10_SHIFT ) & \
    NANDFCTRL2_DATAIN2_DATA_10_MASK )

#define NANDFCTRL2_DATAIN2_DATA_9_SHIFT 8
#define NANDFCTRL2_DATAIN2_DATA_9_MASK  0xff00U
#define NANDFCTRL2_DATAIN2_DATA_9_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN2_DATA_9_MASK ) >> \
    NANDFCTRL2_DATAIN2_DATA_9_SHIFT )
#define NANDFCTRL2_DATAIN2_DATA_9_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN2_DATA_9_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN2_DATA_9_SHIFT ) & \
      NANDFCTRL2_DATAIN2_DATA_9_MASK ) )
#define NANDFCTRL2_DATAIN2_DATA_9( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN2_DATA_9_SHIFT ) & \
    NANDFCTRL2_DATAIN2_DATA_9_MASK )

#define NANDFCTRL2_DATAIN2_DATA_8_SHIFT 0
#define NANDFCTRL2_DATAIN2_DATA_8_MASK  0xffU
#define NANDFCTRL2_DATAIN2_DATA_8_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN2_DATA_8_MASK ) >> \
    NANDFCTRL2_DATAIN2_DATA_8_SHIFT )
#define NANDFCTRL2_DATAIN2_DATA_8_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN2_DATA_8_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN2_DATA_8_SHIFT ) & \
      NANDFCTRL2_DATAIN2_DATA_8_MASK ) )
#define NANDFCTRL2_DATAIN2_DATA_8( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN2_DATA_8_SHIFT ) & \
    NANDFCTRL2_DATAIN2_DATA_8_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DATAIN3 Data-in 3 register (DATAIN3)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DATAIN3_DATA_15_SHIFT 24
#define NANDFCTRL2_DATAIN3_DATA_15_MASK  0xff000000U
#define NANDFCTRL2_DATAIN3_DATA_15_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN3_DATA_15_MASK ) >> \
    NANDFCTRL2_DATAIN3_DATA_15_SHIFT )
#define NANDFCTRL2_DATAIN3_DATA_15_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN3_DATA_15_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN3_DATA_15_SHIFT ) & \
      NANDFCTRL2_DATAIN3_DATA_15_MASK ) )
#define NANDFCTRL2_DATAIN3_DATA_15( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN3_DATA_15_SHIFT ) & \
    NANDFCTRL2_DATAIN3_DATA_15_MASK )

#define NANDFCTRL2_DATAIN3_DATA_14_SHIFT 16
#define NANDFCTRL2_DATAIN3_DATA_14_MASK  0xff0000U
#define NANDFCTRL2_DATAIN3_DATA_14_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN3_DATA_14_MASK ) >> \
    NANDFCTRL2_DATAIN3_DATA_14_SHIFT )
#define NANDFCTRL2_DATAIN3_DATA_14_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN3_DATA_14_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN3_DATA_14_SHIFT ) & \
      NANDFCTRL2_DATAIN3_DATA_14_MASK ) )
#define NANDFCTRL2_DATAIN3_DATA_14( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN3_DATA_14_SHIFT ) & \
    NANDFCTRL2_DATAIN3_DATA_14_MASK )

#define NANDFCTRL2_DATAIN3_DATA_13_SHIFT 8
#define NANDFCTRL2_DATAIN3_DATA_13_MASK  0xff00U
#define NANDFCTRL2_DATAIN3_DATA_13_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN3_DATA_13_MASK ) >> \
    NANDFCTRL2_DATAIN3_DATA_13_SHIFT )
#define NANDFCTRL2_DATAIN3_DATA_13_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN3_DATA_13_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN3_DATA_13_SHIFT ) & \
      NANDFCTRL2_DATAIN3_DATA_13_MASK ) )
#define NANDFCTRL2_DATAIN3_DATA_13( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN3_DATA_13_SHIFT ) & \
    NANDFCTRL2_DATAIN3_DATA_13_MASK )

#define NANDFCTRL2_DATAIN3_DATA_12_SHIFT 0
#define NANDFCTRL2_DATAIN3_DATA_12_MASK  0xffU
#define NANDFCTRL2_DATAIN3_DATA_12_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAIN3_DATA_12_MASK ) >> \
    NANDFCTRL2_DATAIN3_DATA_12_SHIFT )
#define NANDFCTRL2_DATAIN3_DATA_12_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAIN3_DATA_12_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAIN3_DATA_12_SHIFT ) & \
      NANDFCTRL2_DATAIN3_DATA_12_MASK ) )
#define NANDFCTRL2_DATAIN3_DATA_12( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAIN3_DATA_12_SHIFT ) & \
    NANDFCTRL2_DATAIN3_DATA_12_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DATAOUT0 Data-out 0 register (DATAOUT0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DATAOUT0_DATA_3_SHIFT 24
#define NANDFCTRL2_DATAOUT0_DATA_3_MASK  0xff000000U
#define NANDFCTRL2_DATAOUT0_DATA_3_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT0_DATA_3_MASK ) >> \
    NANDFCTRL2_DATAOUT0_DATA_3_SHIFT )
#define NANDFCTRL2_DATAOUT0_DATA_3_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT0_DATA_3_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT0_DATA_3_SHIFT ) & \
      NANDFCTRL2_DATAOUT0_DATA_3_MASK ) )
#define NANDFCTRL2_DATAOUT0_DATA_3( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT0_DATA_3_SHIFT ) & \
    NANDFCTRL2_DATAOUT0_DATA_3_MASK )

#define NANDFCTRL2_DATAOUT0_DATA_2_SHIFT 16
#define NANDFCTRL2_DATAOUT0_DATA_2_MASK  0xff0000U
#define NANDFCTRL2_DATAOUT0_DATA_2_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT0_DATA_2_MASK ) >> \
    NANDFCTRL2_DATAOUT0_DATA_2_SHIFT )
#define NANDFCTRL2_DATAOUT0_DATA_2_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT0_DATA_2_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT0_DATA_2_SHIFT ) & \
      NANDFCTRL2_DATAOUT0_DATA_2_MASK ) )
#define NANDFCTRL2_DATAOUT0_DATA_2( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT0_DATA_2_SHIFT ) & \
    NANDFCTRL2_DATAOUT0_DATA_2_MASK )

#define NANDFCTRL2_DATAOUT0_DATA_1_SHIFT 8
#define NANDFCTRL2_DATAOUT0_DATA_1_MASK  0xff00U
#define NANDFCTRL2_DATAOUT0_DATA_1_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT0_DATA_1_MASK ) >> \
    NANDFCTRL2_DATAOUT0_DATA_1_SHIFT )
#define NANDFCTRL2_DATAOUT0_DATA_1_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT0_DATA_1_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT0_DATA_1_SHIFT ) & \
      NANDFCTRL2_DATAOUT0_DATA_1_MASK ) )
#define NANDFCTRL2_DATAOUT0_DATA_1( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT0_DATA_1_SHIFT ) & \
    NANDFCTRL2_DATAOUT0_DATA_1_MASK )

#define NANDFCTRL2_DATAOUT0_DATA_0_SHIFT 0
#define NANDFCTRL2_DATAOUT0_DATA_0_MASK  0xffU
#define NANDFCTRL2_DATAOUT0_DATA_0_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT0_DATA_0_MASK ) >> \
    NANDFCTRL2_DATAOUT0_DATA_0_SHIFT )
#define NANDFCTRL2_DATAOUT0_DATA_0_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT0_DATA_0_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT0_DATA_0_SHIFT ) & \
      NANDFCTRL2_DATAOUT0_DATA_0_MASK ) )
#define NANDFCTRL2_DATAOUT0_DATA_0( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT0_DATA_0_SHIFT ) & \
    NANDFCTRL2_DATAOUT0_DATA_0_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DATAOUT1 Data-out 1 register (DATAOUT1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DATAOUT1_DATA_7_SHIFT 24
#define NANDFCTRL2_DATAOUT1_DATA_7_MASK  0xff000000U
#define NANDFCTRL2_DATAOUT1_DATA_7_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT1_DATA_7_MASK ) >> \
    NANDFCTRL2_DATAOUT1_DATA_7_SHIFT )
#define NANDFCTRL2_DATAOUT1_DATA_7_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT1_DATA_7_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT1_DATA_7_SHIFT ) & \
      NANDFCTRL2_DATAOUT1_DATA_7_MASK ) )
#define NANDFCTRL2_DATAOUT1_DATA_7( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT1_DATA_7_SHIFT ) & \
    NANDFCTRL2_DATAOUT1_DATA_7_MASK )

#define NANDFCTRL2_DATAOUT1_DATA_6_SHIFT 16
#define NANDFCTRL2_DATAOUT1_DATA_6_MASK  0xff0000U
#define NANDFCTRL2_DATAOUT1_DATA_6_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT1_DATA_6_MASK ) >> \
    NANDFCTRL2_DATAOUT1_DATA_6_SHIFT )
#define NANDFCTRL2_DATAOUT1_DATA_6_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT1_DATA_6_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT1_DATA_6_SHIFT ) & \
      NANDFCTRL2_DATAOUT1_DATA_6_MASK ) )
#define NANDFCTRL2_DATAOUT1_DATA_6( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT1_DATA_6_SHIFT ) & \
    NANDFCTRL2_DATAOUT1_DATA_6_MASK )

#define NANDFCTRL2_DATAOUT1_DATA_5_SHIFT 8
#define NANDFCTRL2_DATAOUT1_DATA_5_MASK  0xff00U
#define NANDFCTRL2_DATAOUT1_DATA_5_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT1_DATA_5_MASK ) >> \
    NANDFCTRL2_DATAOUT1_DATA_5_SHIFT )
#define NANDFCTRL2_DATAOUT1_DATA_5_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT1_DATA_5_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT1_DATA_5_SHIFT ) & \
      NANDFCTRL2_DATAOUT1_DATA_5_MASK ) )
#define NANDFCTRL2_DATAOUT1_DATA_5( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT1_DATA_5_SHIFT ) & \
    NANDFCTRL2_DATAOUT1_DATA_5_MASK )

#define NANDFCTRL2_DATAOUT1_DATA_4_SHIFT 0
#define NANDFCTRL2_DATAOUT1_DATA_4_MASK  0xffU
#define NANDFCTRL2_DATAOUT1_DATA_4_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT1_DATA_4_MASK ) >> \
    NANDFCTRL2_DATAOUT1_DATA_4_SHIFT )
#define NANDFCTRL2_DATAOUT1_DATA_4_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT1_DATA_4_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT1_DATA_4_SHIFT ) & \
      NANDFCTRL2_DATAOUT1_DATA_4_MASK ) )
#define NANDFCTRL2_DATAOUT1_DATA_4( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT1_DATA_4_SHIFT ) & \
    NANDFCTRL2_DATAOUT1_DATA_4_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DATAOUT2 Data-out 2 register (DATAOUT2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DATAOUT2_DATA_11_SHIFT 24
#define NANDFCTRL2_DATAOUT2_DATA_11_MASK  0xff000000U
#define NANDFCTRL2_DATAOUT2_DATA_11_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT2_DATA_11_MASK ) >> \
    NANDFCTRL2_DATAOUT2_DATA_11_SHIFT )
#define NANDFCTRL2_DATAOUT2_DATA_11_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT2_DATA_11_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT2_DATA_11_SHIFT ) & \
      NANDFCTRL2_DATAOUT2_DATA_11_MASK ) )
#define NANDFCTRL2_DATAOUT2_DATA_11( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT2_DATA_11_SHIFT ) & \
    NANDFCTRL2_DATAOUT2_DATA_11_MASK )

#define NANDFCTRL2_DATAOUT2_DATA_10_SHIFT 16
#define NANDFCTRL2_DATAOUT2_DATA_10_MASK  0xff0000U
#define NANDFCTRL2_DATAOUT2_DATA_10_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT2_DATA_10_MASK ) >> \
    NANDFCTRL2_DATAOUT2_DATA_10_SHIFT )
#define NANDFCTRL2_DATAOUT2_DATA_10_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT2_DATA_10_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT2_DATA_10_SHIFT ) & \
      NANDFCTRL2_DATAOUT2_DATA_10_MASK ) )
#define NANDFCTRL2_DATAOUT2_DATA_10( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT2_DATA_10_SHIFT ) & \
    NANDFCTRL2_DATAOUT2_DATA_10_MASK )

#define NANDFCTRL2_DATAOUT2_DATA_9_SHIFT 8
#define NANDFCTRL2_DATAOUT2_DATA_9_MASK  0xff00U
#define NANDFCTRL2_DATAOUT2_DATA_9_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT2_DATA_9_MASK ) >> \
    NANDFCTRL2_DATAOUT2_DATA_9_SHIFT )
#define NANDFCTRL2_DATAOUT2_DATA_9_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT2_DATA_9_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT2_DATA_9_SHIFT ) & \
      NANDFCTRL2_DATAOUT2_DATA_9_MASK ) )
#define NANDFCTRL2_DATAOUT2_DATA_9( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT2_DATA_9_SHIFT ) & \
    NANDFCTRL2_DATAOUT2_DATA_9_MASK )

#define NANDFCTRL2_DATAOUT2_DATA_8_SHIFT 0
#define NANDFCTRL2_DATAOUT2_DATA_8_MASK  0xffU
#define NANDFCTRL2_DATAOUT2_DATA_8_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT2_DATA_8_MASK ) >> \
    NANDFCTRL2_DATAOUT2_DATA_8_SHIFT )
#define NANDFCTRL2_DATAOUT2_DATA_8_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT2_DATA_8_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT2_DATA_8_SHIFT ) & \
      NANDFCTRL2_DATAOUT2_DATA_8_MASK ) )
#define NANDFCTRL2_DATAOUT2_DATA_8( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT2_DATA_8_SHIFT ) & \
    NANDFCTRL2_DATAOUT2_DATA_8_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBNANDFCTRL2DATAOUT3 Data-out 3 register (DATAOUT3)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define NANDFCTRL2_DATAOUT3_DATA_15_SHIFT 24
#define NANDFCTRL2_DATAOUT3_DATA_15_MASK  0xff000000U
#define NANDFCTRL2_DATAOUT3_DATA_15_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT3_DATA_15_MASK ) >> \
    NANDFCTRL2_DATAOUT3_DATA_15_SHIFT )
#define NANDFCTRL2_DATAOUT3_DATA_15_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT3_DATA_15_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT3_DATA_15_SHIFT ) & \
      NANDFCTRL2_DATAOUT3_DATA_15_MASK ) )
#define NANDFCTRL2_DATAOUT3_DATA_15( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT3_DATA_15_SHIFT ) & \
    NANDFCTRL2_DATAOUT3_DATA_15_MASK )

#define NANDFCTRL2_DATAOUT3_DATA_14_SHIFT 16
#define NANDFCTRL2_DATAOUT3_DATA_14_MASK  0xff0000U
#define NANDFCTRL2_DATAOUT3_DATA_14_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT3_DATA_14_MASK ) >> \
    NANDFCTRL2_DATAOUT3_DATA_14_SHIFT )
#define NANDFCTRL2_DATAOUT3_DATA_14_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT3_DATA_14_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT3_DATA_14_SHIFT ) & \
      NANDFCTRL2_DATAOUT3_DATA_14_MASK ) )
#define NANDFCTRL2_DATAOUT3_DATA_14( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT3_DATA_14_SHIFT ) & \
    NANDFCTRL2_DATAOUT3_DATA_14_MASK )

#define NANDFCTRL2_DATAOUT3_DATA_13_SHIFT 8
#define NANDFCTRL2_DATAOUT3_DATA_13_MASK  0xff00U
#define NANDFCTRL2_DATAOUT3_DATA_13_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT3_DATA_13_MASK ) >> \
    NANDFCTRL2_DATAOUT3_DATA_13_SHIFT )
#define NANDFCTRL2_DATAOUT3_DATA_13_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT3_DATA_13_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT3_DATA_13_SHIFT ) & \
      NANDFCTRL2_DATAOUT3_DATA_13_MASK ) )
#define NANDFCTRL2_DATAOUT3_DATA_13( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT3_DATA_13_SHIFT ) & \
    NANDFCTRL2_DATAOUT3_DATA_13_MASK )

#define NANDFCTRL2_DATAOUT3_DATA_12_SHIFT 0
#define NANDFCTRL2_DATAOUT3_DATA_12_MASK  0xffU
#define NANDFCTRL2_DATAOUT3_DATA_12_GET( _reg )        \
  ( ( ( _reg ) & NANDFCTRL2_DATAOUT3_DATA_12_MASK ) >> \
    NANDFCTRL2_DATAOUT3_DATA_12_SHIFT )
#define NANDFCTRL2_DATAOUT3_DATA_12_SET( _reg, _val )     \
  ( ( ( _reg ) & ~NANDFCTRL2_DATAOUT3_DATA_12_MASK ) |    \
    ( ( ( _val ) << NANDFCTRL2_DATAOUT3_DATA_12_SHIFT ) & \
      NANDFCTRL2_DATAOUT3_DATA_12_MASK ) )
#define NANDFCTRL2_DATAOUT3_DATA_12( _val )             \
  ( ( ( _val ) << NANDFCTRL2_DATAOUT3_DATA_12_SHIFT ) & \
    NANDFCTRL2_DATAOUT3_DATA_12_MASK )

/** @} */

/**
 * @brief This structure defines the NANDFCTRL2 register block memory map.
 */
typedef struct nandfctrl2 {
  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CTRL0.
   */
  uint32_t ctrl0;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CTRL1.
   */
  uint32_t ctrl1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CTRL2.
   */
  uint32_t ctrl2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CTRL3.
   */
  uint32_t ctrl3;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CTRL4.
   */
  uint32_t ctrl4;

  uint32_t reserved_14_20[ 3 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2STS0.
   */
  uint32_t sts0;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2STS1.
   */
  uint32_t sts1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2STS2.
   */
  uint32_t sts2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2STS3.
   */
  uint32_t sts3;

  uint32_t reserved_30_38[ 2 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CAP0.
   */
  uint32_t cap0;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CAP1.
   */
  uint32_t cap1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CAP2.
   */
  uint32_t cap2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CAP3.
   */
  uint32_t cap3;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CAP4.
   */
  uint32_t cap4;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2CAP5.
   */
  uint32_t cap5;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME0.
   */
  uint32_t tme0;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME1.
   */
  uint32_t tme1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME2.
   */
  uint32_t tme2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME3.
   */
  uint32_t tme3;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME4.
   */
  uint32_t tme4;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME5.
   */
  uint32_t tme5;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME6.
   */
  uint32_t tme6;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME7.
   */
  uint32_t tme7;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME8.
   */
  uint32_t tme8;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME9.
   */
  uint32_t tme9;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME10.
   */
  uint32_t tme10;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TME11.
   */
  uint32_t tme11;

  uint32_t reserved_80_c0[ 16 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TXSKEW0.
   */
  uint32_t txskew0;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2RXSKEW0.
   */
  uint32_t rxskew0;

  uint32_t reserved_c8_d0[ 2 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2TMOUT0.
   */
  uint32_t tmout0;

  uint32_t reserved_d4_150[ 31 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2LLPL.
   */
  uint32_t llpl;

  uint32_t reserved_154_160[ 3 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DCMD.
   */
  uint32_t dcmd;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DTARSEL0.
   */
  uint32_t dtarsel0;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DTARSEL1.
   */
  uint32_t dtarsel1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DCHSEL.
   */
  uint32_t dchsel;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DRBSEL.
   */
  uint32_t drbsel;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DROW.
   */
  uint32_t drow;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DCOLSIZE.
   */
  uint32_t dcolsize;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DSTS.
   */
  uint32_t dsts;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DECCSTS0.
   */
  uint32_t deccsts0;

  uint32_t reserved_184_190[ 3 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DDPL.
   */
  uint32_t ddpl;

  uint32_t reserved_194_1e0[ 19 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DATAIN0.
   */
  uint32_t datain0;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DATAIN1.
   */
  uint32_t datain1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DATAIN2.
   */
  uint32_t datain2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DATAIN3.
   */
  uint32_t datain3;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DATAOUT0.
   */
  uint32_t dataout0;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DATAOUT1.
   */
  uint32_t dataout1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DATAOUT2.
   */
  uint32_t dataout2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBNANDFCTRL2DATAOUT3.
   */
  uint32_t dataout3;
} nandfctrl2;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_NANDFCTRL2_REGS_H */
