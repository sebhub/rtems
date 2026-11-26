/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRSCRUB
 *
 * @brief This header file defines the GRSCRUB register block interface.
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

/* Find related documentation with spec:/dev/grlib/if/grscrub-header */

#ifndef _GRLIB_GRSCRUB_REGS_H
#define _GRLIB_GRSCRUB_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/grscrub */

/**
 * @defgroup RTEMSDeviceGRSCRUB GRSCRUB
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the GRSCRUB interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRSCRUBSTATUS Status register (STATUS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_STATUS_SCRUND 0x1000U

#define GRSCRUB_STATUS_HOLD 0x800U

#define GRSCRUB_STATUS_ERRID_SHIFT 6
#define GRSCRUB_STATUS_ERRID_MASK  0x7c0U
#define GRSCRUB_STATUS_ERRID_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_STATUS_ERRID_MASK ) >> GRSCRUB_STATUS_ERRID_SHIFT )
#define GRSCRUB_STATUS_ERRID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_STATUS_ERRID_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_STATUS_ERRID_SHIFT ) & \
      GRSCRUB_STATUS_ERRID_MASK ) )
#define GRSCRUB_STATUS_ERRID( _val ) \
  ( ( ( _val ) << GRSCRUB_STATUS_ERRID_SHIFT ) & GRSCRUB_STATUS_ERRID_MASK )
#define GRSCRUB_STATUS_ERRID_NO_ERROR                0U
#define GRSCRUB_STATUS_ERRID_PROGRAM_ERROR           1U
#define GRSCRUB_STATUS_ERRID_UNCORRECTABLE_BIT_ERROR 2U
#define GRSCRUB_STATUS_ERRID_DMA_MEM_READ_ERROR      3U
#define GRSCRUB_STATUS_ERRID_DMA_MEM_WRITE_ERROR     4U
#define GRSCRUB_STATUS_ERRID_READBACK_ERROR          5U
#define GRSCRUB_STATUS_ERRID_FRAME_MAPPING_ERROR     6U
#define GRSCRUB_STATUS_ERRID_WRITE_FRAME_ERROR       7U
#define GRSCRUB_STATUS_ERRID_CI_INTEGRITY_ERROR      8U

#define GRSCRUB_STATUS_SCRERR 0x20U

#define GRSCRUB_STATUS_OPDONE 0x10U

#define GRSCRUB_STATUS_STATE_SHIFT 0
#define GRSCRUB_STATUS_STATE_MASK  0xfU
#define GRSCRUB_STATUS_STATE_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_STATUS_STATE_MASK ) >> GRSCRUB_STATUS_STATE_SHIFT )
#define GRSCRUB_STATUS_STATE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_STATUS_STATE_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_STATUS_STATE_SHIFT ) & \
      GRSCRUB_STATUS_STATE_MASK ) )
#define GRSCRUB_STATUS_STATE( _val ) \
  ( ( ( _val ) << GRSCRUB_STATUS_STATE_SHIFT ) & GRSCRUB_STATUS_STATE_MASK )
#define GRSCRUB_STATUS_STATE_IDLE        0U
#define GRSCRUB_STATUS_STATE_PROGRAMMING 1U
#define GRSCRUB_STATUS_STATE_SCRUBBING   2U
#define GRSCRUB_STATUS_STATE_MAPPING     3U
#define GRSCRUB_STATUS_STATE_GOLDEN_CRC  4U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBCONFIG Configuration mode register (CONFIG)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_CONFIG_IRQSDEN 0x2000U

#define GRSCRUB_CONFIG_FFCEN 0x1000U

#define GRSCRUB_CONFIG_CRCEN 0x800U

#define GRSCRUB_CONFIG_WRBKEN 0x400U

#define GRSCRUB_CONFIG_IRQEEN 0x200U

#define GRSCRUB_CONFIG_IRQDEN 0x100U

#define GRSCRUB_CONFIG_OPMODE_SHIFT 4
#define GRSCRUB_CONFIG_OPMODE_MASK  0xf0U
#define GRSCRUB_CONFIG_OPMODE_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_CONFIG_OPMODE_MASK ) >> GRSCRUB_CONFIG_OPMODE_SHIFT )
#define GRSCRUB_CONFIG_OPMODE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_CONFIG_OPMODE_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_CONFIG_OPMODE_SHIFT ) & \
      GRSCRUB_CONFIG_OPMODE_MASK ) )
#define GRSCRUB_CONFIG_OPMODE( _val ) \
  ( ( ( _val ) << GRSCRUB_CONFIG_OPMODE_SHIFT ) & GRSCRUB_CONFIG_OPMODE_MASK )
#define GRSCRUB_CONFIG_OPMODE_IDLE        0U
#define GRSCRUB_CONFIG_OPMODE_PROGRAMMING 1U
#define GRSCRUB_CONFIG_OPMODE_SCRUBBING   2U
#define GRSCRUB_CONFIG_OPMODE_MAPPING     3U
#define GRSCRUB_CONFIG_OPMODE_GOLDEN_CRC  4U

#define GRSCRUB_CONFIG_CORM 0x8U

#define GRSCRUB_CONFIG_SCRM 0x4U

#define GRSCRUB_CONFIG_SCRUN 0x2U

#define GRSCRUB_CONFIG_EN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBIDCODE FPGA device identifier register (IDCODE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_IDCODE_IDCODE_SHIFT 0
#define GRSCRUB_IDCODE_IDCODE_MASK  0xffffffffU
#define GRSCRUB_IDCODE_IDCODE_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_IDCODE_IDCODE_MASK ) >> GRSCRUB_IDCODE_IDCODE_SHIFT )
#define GRSCRUB_IDCODE_IDCODE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_IDCODE_IDCODE_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_IDCODE_IDCODE_SHIFT ) & \
      GRSCRUB_IDCODE_IDCODE_MASK ) )
#define GRSCRUB_IDCODE_IDCODE( _val ) \
  ( ( ( _val ) << GRSCRUB_IDCODE_IDCODE_SHIFT ) & GRSCRUB_IDCODE_IDCODE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBDELAY Delay register (DELAY)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_DELAY_DELAY_SHIFT 0
#define GRSCRUB_DELAY_DELAY_MASK  0xffffffffU
#define GRSCRUB_DELAY_DELAY_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_DELAY_DELAY_MASK ) >> GRSCRUB_DELAY_DELAY_SHIFT )
#define GRSCRUB_DELAY_DELAY_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_DELAY_DELAY_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_DELAY_DELAY_SHIFT ) & \
      GRSCRUB_DELAY_DELAY_MASK ) )
#define GRSCRUB_DELAY_DELAY( _val ) \
  ( ( ( _val ) << GRSCRUB_DELAY_DELAY_SHIFT ) & GRSCRUB_DELAY_DELAY_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBFCR Frame configuration register (FCR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_FCR_FCNT_SHIFT 9
#define GRSCRUB_FCR_FCNT_MASK  0x1fffe00U
#define GRSCRUB_FCR_FCNT_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_FCR_FCNT_MASK ) >> GRSCRUB_FCR_FCNT_SHIFT )
#define GRSCRUB_FCR_FCNT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSCRUB_FCR_FCNT_MASK ) | \
    ( ( ( _val ) << GRSCRUB_FCR_FCNT_SHIFT ) & GRSCRUB_FCR_FCNT_MASK ) )
#define GRSCRUB_FCR_FCNT( _val ) \
  ( ( ( _val ) << GRSCRUB_FCR_FCNT_SHIFT ) & GRSCRUB_FCR_FCNT_MASK )

#define GRSCRUB_FCR_FLEN_SHIFT 2
#define GRSCRUB_FCR_FLEN_MASK  0x1fcU
#define GRSCRUB_FCR_FLEN_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_FCR_FLEN_MASK ) >> GRSCRUB_FCR_FLEN_SHIFT )
#define GRSCRUB_FCR_FLEN_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSCRUB_FCR_FLEN_MASK ) | \
    ( ( ( _val ) << GRSCRUB_FCR_FLEN_SHIFT ) & GRSCRUB_FCR_FLEN_MASK ) )
#define GRSCRUB_FCR_FLEN( _val ) \
  ( ( ( _val ) << GRSCRUB_FCR_FLEN_SHIFT ) & GRSCRUB_FCR_FLEN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBLFAR Low frame address register (LFAR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_LFAR_LFAR_SHIFT 0
#define GRSCRUB_LFAR_LFAR_MASK  0xffffffffU
#define GRSCRUB_LFAR_LFAR_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_LFAR_LFAR_MASK ) >> GRSCRUB_LFAR_LFAR_SHIFT )
#define GRSCRUB_LFAR_LFAR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSCRUB_LFAR_LFAR_MASK ) | \
    ( ( ( _val ) << GRSCRUB_LFAR_LFAR_SHIFT ) & GRSCRUB_LFAR_LFAR_MASK ) )
#define GRSCRUB_LFAR_LFAR( _val ) \
  ( ( ( _val ) << GRSCRUB_LFAR_LFAR_SHIFT ) & GRSCRUB_LFAR_LFAR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBLGBAR \
 *   Low golden bitstream address register (LGBAR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_LGBAR_LGBAR_SHIFT 0
#define GRSCRUB_LGBAR_LGBAR_MASK  0xffffffffU
#define GRSCRUB_LGBAR_LGBAR_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_LGBAR_LGBAR_MASK ) >> GRSCRUB_LGBAR_LGBAR_SHIFT )
#define GRSCRUB_LGBAR_LGBAR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_LGBAR_LGBAR_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_LGBAR_LGBAR_SHIFT ) & \
      GRSCRUB_LGBAR_LGBAR_MASK ) )
#define GRSCRUB_LGBAR_LGBAR( _val ) \
  ( ( ( _val ) << GRSCRUB_LGBAR_LGBAR_SHIFT ) & GRSCRUB_LGBAR_LGBAR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBHGBAR \
 *   High golden bitstream address register (HGBAR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_HGBAR_HGBAR_SHIFT 0
#define GRSCRUB_HGBAR_HGBAR_MASK  0xffffffffU
#define GRSCRUB_HGBAR_HGBAR_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_HGBAR_HGBAR_MASK ) >> GRSCRUB_HGBAR_HGBAR_SHIFT )
#define GRSCRUB_HGBAR_HGBAR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_HGBAR_HGBAR_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_HGBAR_HGBAR_SHIFT ) & \
      GRSCRUB_HGBAR_HGBAR_MASK ) )
#define GRSCRUB_HGBAR_HGBAR( _val ) \
  ( ( ( _val ) << GRSCRUB_HGBAR_HGBAR_SHIFT ) & GRSCRUB_HGBAR_HGBAR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBLGSFAR \
 *   Low golden start frame address register (LGSFAR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_LGSFAR_LGSFAR_SHIFT 0
#define GRSCRUB_LGSFAR_LGSFAR_MASK  0xffffffffU
#define GRSCRUB_LGSFAR_LGSFAR_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_LGSFAR_LGSFAR_MASK ) >> GRSCRUB_LGSFAR_LGSFAR_SHIFT )
#define GRSCRUB_LGSFAR_LGSFAR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_LGSFAR_LGSFAR_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_LGSFAR_LGSFAR_SHIFT ) & \
      GRSCRUB_LGSFAR_LGSFAR_MASK ) )
#define GRSCRUB_LGSFAR_LGSFAR( _val ) \
  ( ( ( _val ) << GRSCRUB_LGSFAR_LGSFAR_SHIFT ) & GRSCRUB_LGSFAR_LGSFAR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBLMASKAR \
 *   Low golden mask address register (LMASKAR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_LMASKAR_LMASKAR_SHIFT 0
#define GRSCRUB_LMASKAR_LMASKAR_MASK  0xffffffffU
#define GRSCRUB_LMASKAR_LMASKAR_GET( _reg )        \
  ( ( ( _reg ) & GRSCRUB_LMASKAR_LMASKAR_MASK ) >> \
    GRSCRUB_LMASKAR_LMASKAR_SHIFT )
#define GRSCRUB_LMASKAR_LMASKAR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_LMASKAR_LMASKAR_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_LMASKAR_LMASKAR_SHIFT ) & \
      GRSCRUB_LMASKAR_LMASKAR_MASK ) )
#define GRSCRUB_LMASKAR_LMASKAR( _val )             \
  ( ( ( _val ) << GRSCRUB_LMASKAR_LMASKAR_SHIFT ) & \
    GRSCRUB_LMASKAR_LMASKAR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBLFMAPAR \
 *   Low golden frame mapping address register (LFMAPAR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_LFMAPAR_LFMAPAR_SHIFT 0
#define GRSCRUB_LFMAPAR_LFMAPAR_MASK  0xffffffffU
#define GRSCRUB_LFMAPAR_LFMAPAR_GET( _reg )        \
  ( ( ( _reg ) & GRSCRUB_LFMAPAR_LFMAPAR_MASK ) >> \
    GRSCRUB_LFMAPAR_LFMAPAR_SHIFT )
#define GRSCRUB_LFMAPAR_LFMAPAR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_LFMAPAR_LFMAPAR_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_LFMAPAR_LFMAPAR_SHIFT ) & \
      GRSCRUB_LFMAPAR_LFMAPAR_MASK ) )
#define GRSCRUB_LFMAPAR_LFMAPAR( _val )             \
  ( ( ( _val ) << GRSCRUB_LFMAPAR_LFMAPAR_SHIFT ) & \
    GRSCRUB_LFMAPAR_LFMAPAR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBLCRCAR Low golden CRC address register (LCRCAR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_LCRCAR_LCRCAR_SHIFT 0
#define GRSCRUB_LCRCAR_LCRCAR_MASK  0xffffffffU
#define GRSCRUB_LCRCAR_LCRCAR_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_LCRCAR_LCRCAR_MASK ) >> GRSCRUB_LCRCAR_LCRCAR_SHIFT )
#define GRSCRUB_LCRCAR_LCRCAR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_LCRCAR_LCRCAR_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_LCRCAR_LCRCAR_SHIFT ) & \
      GRSCRUB_LCRCAR_LCRCAR_MASK ) )
#define GRSCRUB_LCRCAR_LCRCAR( _val ) \
  ( ( ( _val ) << GRSCRUB_LCRCAR_LCRCAR_SHIFT ) & GRSCRUB_LCRCAR_LCRCAR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBLGRBKAR \
 *   Low golden readback data address register (LGRBKAR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_LGRBKAR_LGRBKAR_SHIFT 0
#define GRSCRUB_LGRBKAR_LGRBKAR_MASK  0xffffffffU
#define GRSCRUB_LGRBKAR_LGRBKAR_GET( _reg )        \
  ( ( ( _reg ) & GRSCRUB_LGRBKAR_LGRBKAR_MASK ) >> \
    GRSCRUB_LGRBKAR_LGRBKAR_SHIFT )
#define GRSCRUB_LGRBKAR_LGRBKAR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_LGRBKAR_LGRBKAR_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_LGRBKAR_LGRBKAR_SHIFT ) & \
      GRSCRUB_LGRBKAR_LGRBKAR_MASK ) )
#define GRSCRUB_LGRBKAR_LGRBKAR( _val )             \
  ( ( ( _val ) << GRSCRUB_LGRBKAR_LGRBKAR_SHIFT ) & \
    GRSCRUB_LGRBKAR_LGRBKAR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBECNT Number of errors detected register (ECNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_ECNT_UECNT_SHIFT 16
#define GRSCRUB_ECNT_UECNT_MASK  0xffff0000U
#define GRSCRUB_ECNT_UECNT_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_ECNT_UECNT_MASK ) >> GRSCRUB_ECNT_UECNT_SHIFT )
#define GRSCRUB_ECNT_UECNT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSCRUB_ECNT_UECNT_MASK ) | \
    ( ( ( _val ) << GRSCRUB_ECNT_UECNT_SHIFT ) & GRSCRUB_ECNT_UECNT_MASK ) )
#define GRSCRUB_ECNT_UECNT( _val ) \
  ( ( ( _val ) << GRSCRUB_ECNT_UECNT_SHIFT ) & GRSCRUB_ECNT_UECNT_MASK )

#define GRSCRUB_ECNT_RECNT_SHIFT 0
#define GRSCRUB_ECNT_RECNT_MASK  0xffffU
#define GRSCRUB_ECNT_RECNT_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_ECNT_RECNT_MASK ) >> GRSCRUB_ECNT_RECNT_SHIFT )
#define GRSCRUB_ECNT_RECNT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSCRUB_ECNT_RECNT_MASK ) | \
    ( ( ( _val ) << GRSCRUB_ECNT_RECNT_SHIFT ) & GRSCRUB_ECNT_RECNT_MASK ) )
#define GRSCRUB_ECNT_RECNT( _val ) \
  ( ( ( _val ) << GRSCRUB_ECNT_RECNT_SHIFT ) & GRSCRUB_ECNT_RECNT_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBSETUP FPGA setup register (SETUP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_SETUP_CICHECK 0x40000000U

#define GRSCRUB_SETUP_BLKFRAME_SHIFT 22
#define GRSCRUB_SETUP_BLKFRAME_MASK  0x3fc00000U
#define GRSCRUB_SETUP_BLKFRAME_GET( _reg )        \
  ( ( ( _reg ) & GRSCRUB_SETUP_BLKFRAME_MASK ) >> \
    GRSCRUB_SETUP_BLKFRAME_SHIFT )
#define GRSCRUB_SETUP_BLKFRAME_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_SETUP_BLKFRAME_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_SETUP_BLKFRAME_SHIFT ) & \
      GRSCRUB_SETUP_BLKFRAME_MASK ) )
#define GRSCRUB_SETUP_BLKFRAME( _val )             \
  ( ( ( _val ) << GRSCRUB_SETUP_BLKFRAME_SHIFT ) & \
    GRSCRUB_SETUP_BLKFRAME_MASK )

#define GRSCRUB_SETUP_BITSWPEN 0x200000U

#define GRSCRUB_SETUP_CISEL 0x100000U

#define GRSCRUB_SETUP_TPROG_SHIFT 12
#define GRSCRUB_SETUP_TPROG_MASK  0xff000U
#define GRSCRUB_SETUP_TPROG_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_SETUP_TPROG_MASK ) >> GRSCRUB_SETUP_TPROG_SHIFT )
#define GRSCRUB_SETUP_TPROG_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_SETUP_TPROG_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_SETUP_TPROG_SHIFT ) & \
      GRSCRUB_SETUP_TPROG_MASK ) )
#define GRSCRUB_SETUP_TPROG( _val ) \
  ( ( ( _val ) << GRSCRUB_SETUP_TPROG_SHIFT ) & GRSCRUB_SETUP_TPROG_MASK )

#define GRSCRUB_SETUP_ROWBND_SHIFT 4
#define GRSCRUB_SETUP_ROWBND_MASK  0xff0U
#define GRSCRUB_SETUP_ROWBND_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_SETUP_ROWBND_MASK ) >> GRSCRUB_SETUP_ROWBND_SHIFT )
#define GRSCRUB_SETUP_ROWBND_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_SETUP_ROWBND_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_SETUP_ROWBND_SHIFT ) & \
      GRSCRUB_SETUP_ROWBND_MASK ) )
#define GRSCRUB_SETUP_ROWBND( _val ) \
  ( ( ( _val ) << GRSCRUB_SETUP_ROWBND_SHIFT ) & GRSCRUB_SETUP_ROWBND_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBCAP Capability register (CAP)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_CAP_GMSK 0x800000U

#define GRSCRUB_CAP_DATACK_SHIFT 20
#define GRSCRUB_CAP_DATACK_MASK  0x700000U
#define GRSCRUB_CAP_DATACK_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_CAP_DATACK_MASK ) >> GRSCRUB_CAP_DATACK_SHIFT )
#define GRSCRUB_CAP_DATACK_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSCRUB_CAP_DATACK_MASK ) | \
    ( ( ( _val ) << GRSCRUB_CAP_DATACK_SHIFT ) & GRSCRUB_CAP_DATACK_MASK ) )
#define GRSCRUB_CAP_DATACK( _val ) \
  ( ( ( _val ) << GRSCRUB_CAP_DATACK_SHIFT ) & GRSCRUB_CAP_DATACK_MASK )

#define GRSCRUB_CAP_SMBUS_SHIFT 18
#define GRSCRUB_CAP_SMBUS_MASK  0xc0000U
#define GRSCRUB_CAP_SMBUS_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_CAP_SMBUS_MASK ) >> GRSCRUB_CAP_SMBUS_SHIFT )
#define GRSCRUB_CAP_SMBUS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSCRUB_CAP_SMBUS_MASK ) | \
    ( ( ( _val ) << GRSCRUB_CAP_SMBUS_SHIFT ) & GRSCRUB_CAP_SMBUS_MASK ) )
#define GRSCRUB_CAP_SMBUS( _val ) \
  ( ( ( _val ) << GRSCRUB_CAP_SMBUS_SHIFT ) & GRSCRUB_CAP_SMBUS_MASK )

#define GRSCRUB_CAP_CINT_SHIFT 16
#define GRSCRUB_CAP_CINT_MASK  0x30000U
#define GRSCRUB_CAP_CINT_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_CAP_CINT_MASK ) >> GRSCRUB_CAP_CINT_SHIFT )
#define GRSCRUB_CAP_CINT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSCRUB_CAP_CINT_MASK ) | \
    ( ( ( _val ) << GRSCRUB_CAP_CINT_SHIFT ) & GRSCRUB_CAP_CINT_MASK ) )
#define GRSCRUB_CAP_CINT( _val ) \
  ( ( ( _val ) << GRSCRUB_CAP_CINT_SHIFT ) & GRSCRUB_CAP_CINT_MASK )

#define GRSCRUB_CAP_FAMILY_SHIFT 4
#define GRSCRUB_CAP_FAMILY_MASK  0xfff0U
#define GRSCRUB_CAP_FAMILY_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_CAP_FAMILY_MASK ) >> GRSCRUB_CAP_FAMILY_SHIFT )
#define GRSCRUB_CAP_FAMILY_SET( _reg, _val )  \
  ( ( ( _reg ) & ~GRSCRUB_CAP_FAMILY_MASK ) | \
    ( ( ( _val ) << GRSCRUB_CAP_FAMILY_SHIFT ) & GRSCRUB_CAP_FAMILY_MASK ) )
#define GRSCRUB_CAP_FAMILY( _val ) \
  ( ( ( _val ) << GRSCRUB_CAP_FAMILY_SHIFT ) & GRSCRUB_CAP_FAMILY_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBFRAMEID Frame id code register (FRAMEID)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_FRAMEID_FRMID_SHIFT 0
#define GRSCRUB_FRAMEID_FRMID_MASK  0xffffU
#define GRSCRUB_FRAMEID_FRMID_GET( _reg ) \
  ( ( ( _reg ) & GRSCRUB_FRAMEID_FRMID_MASK ) >> GRSCRUB_FRAMEID_FRMID_SHIFT )
#define GRSCRUB_FRAMEID_FRMID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_FRAMEID_FRMID_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_FRAMEID_FRMID_SHIFT ) & \
      GRSCRUB_FRAMEID_FRMID_MASK ) )
#define GRSCRUB_FRAMEID_FRMID( _val ) \
  ( ( ( _val ) << GRSCRUB_FRAMEID_FRMID_SHIFT ) & GRSCRUB_FRAMEID_FRMID_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRSCRUBERRFRMID Error frame id code register (ERRFRMID)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define GRSCRUB_ERRFRMID_UE_FRMID_SHIFT 16
#define GRSCRUB_ERRFRMID_UE_FRMID_MASK  0xffff0000U
#define GRSCRUB_ERRFRMID_UE_FRMID_GET( _reg )        \
  ( ( ( _reg ) & GRSCRUB_ERRFRMID_UE_FRMID_MASK ) >> \
    GRSCRUB_ERRFRMID_UE_FRMID_SHIFT )
#define GRSCRUB_ERRFRMID_UE_FRMID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_ERRFRMID_UE_FRMID_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_ERRFRMID_UE_FRMID_SHIFT ) & \
      GRSCRUB_ERRFRMID_UE_FRMID_MASK ) )
#define GRSCRUB_ERRFRMID_UE_FRMID( _val )             \
  ( ( ( _val ) << GRSCRUB_ERRFRMID_UE_FRMID_SHIFT ) & \
    GRSCRUB_ERRFRMID_UE_FRMID_MASK )

#define GRSCRUB_ERRFRMID_ERR_FRMID_SHIFT 0
#define GRSCRUB_ERRFRMID_ERR_FRMID_MASK  0xffffU
#define GRSCRUB_ERRFRMID_ERR_FRMID_GET( _reg )        \
  ( ( ( _reg ) & GRSCRUB_ERRFRMID_ERR_FRMID_MASK ) >> \
    GRSCRUB_ERRFRMID_ERR_FRMID_SHIFT )
#define GRSCRUB_ERRFRMID_ERR_FRMID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~GRSCRUB_ERRFRMID_ERR_FRMID_MASK ) |    \
    ( ( ( _val ) << GRSCRUB_ERRFRMID_ERR_FRMID_SHIFT ) & \
      GRSCRUB_ERRFRMID_ERR_FRMID_MASK ) )
#define GRSCRUB_ERRFRMID_ERR_FRMID( _val )             \
  ( ( ( _val ) << GRSCRUB_ERRFRMID_ERR_FRMID_SHIFT ) & \
    GRSCRUB_ERRFRMID_ERR_FRMID_MASK )

/** @} */

/**
 * @brief This structure defines the GRSCRUB register block memory map.
 */
typedef struct grscrub {
  /**
   * @brief See @ref RTEMSDeviceGRSCRUBSTATUS.
   */
  uint32_t status;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBCONFIG.
   */
  uint32_t config;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBIDCODE.
   */
  uint32_t idcode;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBDELAY.
   */
  uint32_t delay;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBFCR.
   */
  uint32_t fcr;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBLFAR.
   */
  uint32_t lfar;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBLGBAR.
   */
  uint32_t lgbar;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBHGBAR.
   */
  uint32_t hgbar;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBLGSFAR.
   */
  uint32_t lgsfar;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBLMASKAR.
   */
  uint32_t lmaskar;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBLFMAPAR.
   */
  uint32_t lfmapar;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBLCRCAR.
   */
  uint32_t lcrcar;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBLGRBKAR.
   */
  uint32_t lgrbkar;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBECNT.
   */
  uint32_t ecnt;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBSETUP.
   */
  uint32_t setup;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBCAP.
   */
  uint32_t cap;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBFRAMEID.
   */
  uint32_t frameid;

  /**
   * @brief See @ref RTEMSDeviceGRSCRUBERRFRMID.
   */
  uint32_t errfrmid;
} grscrub;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_GRSCRUB_REGS_H */
