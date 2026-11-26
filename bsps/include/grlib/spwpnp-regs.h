/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRLIBSPWPNP
 *
 * @brief This header file defines the SPWPNP register block interface.
 */

/*
 * Copyright (C) 2021 embedded brains GmbH & Co. KG
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

/* Find related documentation with spec:/dev/grlib/if/spwpnp-header */

#ifndef _GRLIB_SPWPNP_REGS_H
#define _GRLIB_SPWPNP_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/spwpnp */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNP SpaceWire Plug-and-Play
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the SpaceWire Plug-and-Play interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPVEND \
 *   SpaceWire Plug-and-Play - Device Vendor and Product ID (PNPVEND)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPVEND_VEND_SHIFT 16
#define SPWPNP_PNPVEND_VEND_MASK  0xffff0000U
#define SPWPNP_PNPVEND_VEND_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPVEND_VEND_MASK ) >> SPWPNP_PNPVEND_VEND_SHIFT )
#define SPWPNP_PNPVEND_VEND_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPVEND_VEND_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPVEND_VEND_SHIFT ) & \
      SPWPNP_PNPVEND_VEND_MASK ) )
#define SPWPNP_PNPVEND_VEND( _val ) \
  ( ( ( _val ) << SPWPNP_PNPVEND_VEND_SHIFT ) & SPWPNP_PNPVEND_VEND_MASK )

#define SPWPNP_PNPVEND_PROD_SHIFT 0
#define SPWPNP_PNPVEND_PROD_MASK  0xffffU
#define SPWPNP_PNPVEND_PROD_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPVEND_PROD_MASK ) >> SPWPNP_PNPVEND_PROD_SHIFT )
#define SPWPNP_PNPVEND_PROD_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPVEND_PROD_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPVEND_PROD_SHIFT ) & \
      SPWPNP_PNPVEND_PROD_MASK ) )
#define SPWPNP_PNPVEND_PROD( _val ) \
  ( ( ( _val ) << SPWPNP_PNPVEND_PROD_SHIFT ) & SPWPNP_PNPVEND_PROD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPVER \
 *   SpaceWire Plug-and-Play - Version (PNPVER)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPVER_MAJOR_SHIFT 24
#define SPWPNP_PNPVER_MAJOR_MASK  0xff000000U
#define SPWPNP_PNPVER_MAJOR_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPVER_MAJOR_MASK ) >> SPWPNP_PNPVER_MAJOR_SHIFT )
#define SPWPNP_PNPVER_MAJOR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPVER_MAJOR_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPVER_MAJOR_SHIFT ) & \
      SPWPNP_PNPVER_MAJOR_MASK ) )
#define SPWPNP_PNPVER_MAJOR( _val ) \
  ( ( ( _val ) << SPWPNP_PNPVER_MAJOR_SHIFT ) & SPWPNP_PNPVER_MAJOR_MASK )

#define SPWPNP_PNPVER_MINOR_SHIFT 16
#define SPWPNP_PNPVER_MINOR_MASK  0xff0000U
#define SPWPNP_PNPVER_MINOR_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPVER_MINOR_MASK ) >> SPWPNP_PNPVER_MINOR_SHIFT )
#define SPWPNP_PNPVER_MINOR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPVER_MINOR_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPVER_MINOR_SHIFT ) & \
      SPWPNP_PNPVER_MINOR_MASK ) )
#define SPWPNP_PNPVER_MINOR( _val ) \
  ( ( ( _val ) << SPWPNP_PNPVER_MINOR_SHIFT ) & SPWPNP_PNPVER_MINOR_MASK )

#define SPWPNP_PNPVER_PATCH_SHIFT 8
#define SPWPNP_PNPVER_PATCH_MASK  0xff00U
#define SPWPNP_PNPVER_PATCH_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPVER_PATCH_MASK ) >> SPWPNP_PNPVER_PATCH_SHIFT )
#define SPWPNP_PNPVER_PATCH_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPVER_PATCH_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPVER_PATCH_SHIFT ) & \
      SPWPNP_PNPVER_PATCH_MASK ) )
#define SPWPNP_PNPVER_PATCH( _val ) \
  ( ( ( _val ) << SPWPNP_PNPVER_PATCH_SHIFT ) & SPWPNP_PNPVER_PATCH_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPDEVSTS \
 *   SpaceWire Plug-and-Play - Device Status (PNPDEVSTS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPDEVSTS_STATUS_SHIFT 0
#define SPWPNP_PNPDEVSTS_STATUS_MASK  0xffU
#define SPWPNP_PNPDEVSTS_STATUS_GET( _reg )        \
  ( ( ( _reg ) & SPWPNP_PNPDEVSTS_STATUS_MASK ) >> \
    SPWPNP_PNPDEVSTS_STATUS_SHIFT )
#define SPWPNP_PNPDEVSTS_STATUS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPDEVSTS_STATUS_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPDEVSTS_STATUS_SHIFT ) & \
      SPWPNP_PNPDEVSTS_STATUS_MASK ) )
#define SPWPNP_PNPDEVSTS_STATUS( _val )             \
  ( ( ( _val ) << SPWPNP_PNPDEVSTS_STATUS_SHIFT ) & \
    SPWPNP_PNPDEVSTS_STATUS_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPACTLNK \
 *   SpaceWire Plug-and-Play - Active Links (PNPACTLNK)
 *
 * @brief This group contains register bit definitions.
 *
 * Bit 0 of ACTIVE is reserved. ACTIVE includes it, so that bit n of ACTIVE
 * corresponds to port n.
 *
 * @{
 */

#define SPWPNP_PNPACTLNK_ACTIVE_SHIFT 0
#define SPWPNP_PNPACTLNK_ACTIVE_MASK  0xffffffffU
#define SPWPNP_PNPACTLNK_ACTIVE_GET( _reg )        \
  ( ( ( _reg ) & SPWPNP_PNPACTLNK_ACTIVE_MASK ) >> \
    SPWPNP_PNPACTLNK_ACTIVE_SHIFT )
#define SPWPNP_PNPACTLNK_ACTIVE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPACTLNK_ACTIVE_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPACTLNK_ACTIVE_SHIFT ) & \
      SPWPNP_PNPACTLNK_ACTIVE_MASK ) )
#define SPWPNP_PNPACTLNK_ACTIVE( _val )             \
  ( ( ( _val ) << SPWPNP_PNPACTLNK_ACTIVE_SHIFT ) & \
    SPWPNP_PNPACTLNK_ACTIVE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPLINFO \
 *   SpaceWire Plug-and-Play - Link Information (PNPLINFO)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPLINFO_OLA_SHIFT 24
#define SPWPNP_PNPLINFO_OLA_MASK  0xff000000U
#define SPWPNP_PNPLINFO_OLA_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPLINFO_OLA_MASK ) >> SPWPNP_PNPLINFO_OLA_SHIFT )
#define SPWPNP_PNPLINFO_OLA_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPLINFO_OLA_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPLINFO_OLA_SHIFT ) & \
      SPWPNP_PNPLINFO_OLA_MASK ) )
#define SPWPNP_PNPLINFO_OLA( _val ) \
  ( ( ( _val ) << SPWPNP_PNPLINFO_OLA_SHIFT ) & SPWPNP_PNPLINFO_OLA_MASK )

#define SPWPNP_PNPLINFO_OAL_SHIFT 22
#define SPWPNP_PNPLINFO_OAL_MASK  0xc00000U
#define SPWPNP_PNPLINFO_OAL_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPLINFO_OAL_MASK ) >> SPWPNP_PNPLINFO_OAL_SHIFT )
#define SPWPNP_PNPLINFO_OAL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPLINFO_OAL_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPLINFO_OAL_SHIFT ) & \
      SPWPNP_PNPLINFO_OAL_MASK ) )
#define SPWPNP_PNPLINFO_OAL( _val ) \
  ( ( ( _val ) << SPWPNP_PNPLINFO_OAL_SHIFT ) & SPWPNP_PNPLINFO_OAL_MASK )

#define SPWPNP_PNPLINFO_OL_SHIFT 16
#define SPWPNP_PNPLINFO_OL_MASK  0x1f0000U
#define SPWPNP_PNPLINFO_OL_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPLINFO_OL_MASK ) >> SPWPNP_PNPLINFO_OL_SHIFT )
#define SPWPNP_PNPLINFO_OL_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPLINFO_OL_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPLINFO_OL_SHIFT ) & SPWPNP_PNPLINFO_OL_MASK ) )
#define SPWPNP_PNPLINFO_OL( _val ) \
  ( ( ( _val ) << SPWPNP_PNPLINFO_OL_SHIFT ) & SPWPNP_PNPLINFO_OL_MASK )

#define SPWPNP_PNPLINFO_RL_SHIFT 8
#define SPWPNP_PNPLINFO_RL_MASK  0x1f00U
#define SPWPNP_PNPLINFO_RL_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPLINFO_RL_MASK ) >> SPWPNP_PNPLINFO_RL_SHIFT )
#define SPWPNP_PNPLINFO_RL_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPLINFO_RL_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPLINFO_RL_SHIFT ) & SPWPNP_PNPLINFO_RL_MASK ) )
#define SPWPNP_PNPLINFO_RL( _val ) \
  ( ( ( _val ) << SPWPNP_PNPLINFO_RL_SHIFT ) & SPWPNP_PNPLINFO_RL_MASK )

#define SPWPNP_PNPLINFO_T 0x80U

#define SPWPNP_PNPLINFO_U 0x40U

#define SPWPNP_PNPLINFO_LC_SHIFT 0
#define SPWPNP_PNPLINFO_LC_MASK  0x1fU
#define SPWPNP_PNPLINFO_LC_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPLINFO_LC_MASK ) >> SPWPNP_PNPLINFO_LC_SHIFT )
#define SPWPNP_PNPLINFO_LC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPLINFO_LC_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPLINFO_LC_SHIFT ) & SPWPNP_PNPLINFO_LC_MASK ) )
#define SPWPNP_PNPLINFO_LC( _val ) \
  ( ( ( _val ) << SPWPNP_PNPLINFO_LC_SHIFT ) & SPWPNP_PNPLINFO_LC_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPOA0 \
 *   SpaceWire Plug-and-Play - Owner Address 0 (PNPOA0)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPOA0_RA_SHIFT 0
#define SPWPNP_PNPOA0_RA_MASK  0xffffffffU
#define SPWPNP_PNPOA0_RA_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPOA0_RA_MASK ) >> SPWPNP_PNPOA0_RA_SHIFT )
#define SPWPNP_PNPOA0_RA_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPOA0_RA_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPOA0_RA_SHIFT ) & SPWPNP_PNPOA0_RA_MASK ) )
#define SPWPNP_PNPOA0_RA( _val ) \
  ( ( ( _val ) << SPWPNP_PNPOA0_RA_SHIFT ) & SPWPNP_PNPOA0_RA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPOA1 \
 *   SpaceWire Plug-and-Play - Owner Address 1 (PNPOA1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPOA1_RA_SHIFT 0
#define SPWPNP_PNPOA1_RA_MASK  0xffffffffU
#define SPWPNP_PNPOA1_RA_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPOA1_RA_MASK ) >> SPWPNP_PNPOA1_RA_SHIFT )
#define SPWPNP_PNPOA1_RA_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPOA1_RA_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPOA1_RA_SHIFT ) & SPWPNP_PNPOA1_RA_MASK ) )
#define SPWPNP_PNPOA1_RA( _val ) \
  ( ( ( _val ) << SPWPNP_PNPOA1_RA_SHIFT ) & SPWPNP_PNPOA1_RA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPOA2 \
 *   SpaceWire Plug-and-Play - Owner Address 2 (PNPOA2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPOA2_RA_SHIFT 0
#define SPWPNP_PNPOA2_RA_MASK  0xffffffffU
#define SPWPNP_PNPOA2_RA_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPOA2_RA_MASK ) >> SPWPNP_PNPOA2_RA_SHIFT )
#define SPWPNP_PNPOA2_RA_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPOA2_RA_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPOA2_RA_SHIFT ) & SPWPNP_PNPOA2_RA_MASK ) )
#define SPWPNP_PNPOA2_RA( _val ) \
  ( ( ( _val ) << SPWPNP_PNPOA2_RA_SHIFT ) & SPWPNP_PNPOA2_RA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPDEVID \
 *   SpaceWire Plug-and-Play - Device ID (PNPDEVID)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPDEVID_DID_SHIFT 0
#define SPWPNP_PNPDEVID_DID_MASK  0xffffffffU
#define SPWPNP_PNPDEVID_DID_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPDEVID_DID_MASK ) >> SPWPNP_PNPDEVID_DID_SHIFT )
#define SPWPNP_PNPDEVID_DID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPDEVID_DID_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPDEVID_DID_SHIFT ) & \
      SPWPNP_PNPDEVID_DID_MASK ) )
#define SPWPNP_PNPDEVID_DID( _val ) \
  ( ( ( _val ) << SPWPNP_PNPDEVID_DID_SHIFT ) & SPWPNP_PNPDEVID_DID_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPUVEND \
 *   SpaceWire Plug-and-Play - Unit Vendor and Product ID (PNPUVEND)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPUVEND_VEND_SHIFT 16
#define SPWPNP_PNPUVEND_VEND_MASK  0xffff0000U
#define SPWPNP_PNPUVEND_VEND_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPUVEND_VEND_MASK ) >> SPWPNP_PNPUVEND_VEND_SHIFT )
#define SPWPNP_PNPUVEND_VEND_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPUVEND_VEND_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPUVEND_VEND_SHIFT ) & \
      SPWPNP_PNPUVEND_VEND_MASK ) )
#define SPWPNP_PNPUVEND_VEND( _val ) \
  ( ( ( _val ) << SPWPNP_PNPUVEND_VEND_SHIFT ) & SPWPNP_PNPUVEND_VEND_MASK )

#define SPWPNP_PNPUVEND_PROD_SHIFT 0
#define SPWPNP_PNPUVEND_PROD_MASK  0xffffU
#define SPWPNP_PNPUVEND_PROD_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPUVEND_PROD_MASK ) >> SPWPNP_PNPUVEND_PROD_SHIFT )
#define SPWPNP_PNPUVEND_PROD_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPUVEND_PROD_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPUVEND_PROD_SHIFT ) & \
      SPWPNP_PNPUVEND_PROD_MASK ) )
#define SPWPNP_PNPUVEND_PROD( _val ) \
  ( ( ( _val ) << SPWPNP_PNPUVEND_PROD_SHIFT ) & SPWPNP_PNPUVEND_PROD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPUSN \
 *   SpaceWire Plug-and-Play - Unit Serial Number (PNPUSN)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPUSN_USN_SHIFT 0
#define SPWPNP_PNPUSN_USN_MASK  0xffffffffU
#define SPWPNP_PNPUSN_USN_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPUSN_USN_MASK ) >> SPWPNP_PNPUSN_USN_SHIFT )
#define SPWPNP_PNPUSN_USN_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPUSN_USN_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPUSN_USN_SHIFT ) & SPWPNP_PNPUSN_USN_MASK ) )
#define SPWPNP_PNPUSN_USN( _val ) \
  ( ( ( _val ) << SPWPNP_PNPUSN_USN_SHIFT ) & SPWPNP_PNPUSN_USN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPVSTRL \
 *   SpaceWire Plug-and-Play - Vendor String Length (PNPVSTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPVSTRL_LEN_SHIFT 0
#define SPWPNP_PNPVSTRL_LEN_MASK  0x7fffU
#define SPWPNP_PNPVSTRL_LEN_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPVSTRL_LEN_MASK ) >> SPWPNP_PNPVSTRL_LEN_SHIFT )
#define SPWPNP_PNPVSTRL_LEN_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPVSTRL_LEN_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPVSTRL_LEN_SHIFT ) & \
      SPWPNP_PNPVSTRL_LEN_MASK ) )
#define SPWPNP_PNPVSTRL_LEN( _val ) \
  ( ( ( _val ) << SPWPNP_PNPVSTRL_LEN_SHIFT ) & SPWPNP_PNPVSTRL_LEN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPPSTRL \
 *   SpaceWire Plug-and-Play - Product String Length (PNPPSTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPPSTRL_LEN_SHIFT 0
#define SPWPNP_PNPPSTRL_LEN_MASK  0x7fffU
#define SPWPNP_PNPPSTRL_LEN_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPPSTRL_LEN_MASK ) >> SPWPNP_PNPPSTRL_LEN_SHIFT )
#define SPWPNP_PNPPSTRL_LEN_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPPSTRL_LEN_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPPSTRL_LEN_SHIFT ) & \
      SPWPNP_PNPPSTRL_LEN_MASK ) )
#define SPWPNP_PNPPSTRL_LEN( _val ) \
  ( ( ( _val ) << SPWPNP_PNPPSTRL_LEN_SHIFT ) & SPWPNP_PNPPSTRL_LEN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPPCNT \
 *   SpaceWire Plug-and-Play - Protocol Count (PNPPCNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPPCNT_PC_SHIFT 0
#define SPWPNP_PNPPCNT_PC_MASK  0x1fU
#define SPWPNP_PNPPCNT_PC_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPPCNT_PC_MASK ) >> SPWPNP_PNPPCNT_PC_SHIFT )
#define SPWPNP_PNPPCNT_PC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPPCNT_PC_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPPCNT_PC_SHIFT ) & SPWPNP_PNPPCNT_PC_MASK ) )
#define SPWPNP_PNPPCNT_PC( _val ) \
  ( ( ( _val ) << SPWPNP_PNPPCNT_PC_SHIFT ) & SPWPNP_PNPPCNT_PC_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPPID1 \
 *   SpaceWire Plug-and-Play - Protocol Identification 1 (PNPPID1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPPID1_PID1_SHIFT 0
#define SPWPNP_PNPPID1_PID1_MASK  0xffffffffU
#define SPWPNP_PNPPID1_PID1_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPPID1_PID1_MASK ) >> SPWPNP_PNPPID1_PID1_SHIFT )
#define SPWPNP_PNPPID1_PID1_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPPID1_PID1_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPPID1_PID1_SHIFT ) & \
      SPWPNP_PNPPID1_PID1_MASK ) )
#define SPWPNP_PNPPID1_PID1( _val ) \
  ( ( ( _val ) << SPWPNP_PNPPID1_PID1_SHIFT ) & SPWPNP_PNPPID1_PID1_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPPID2 \
 *   SpaceWire Plug-and-Play - Protocol Identification 2 (PNPPID2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPPID2_PID2_SHIFT 0
#define SPWPNP_PNPPID2_PID2_MASK  0xffffffffU
#define SPWPNP_PNPPID2_PID2_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPPID2_PID2_MASK ) >> SPWPNP_PNPPID2_PID2_SHIFT )
#define SPWPNP_PNPPID2_PID2_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPPID2_PID2_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPPID2_PID2_SHIFT ) & \
      SPWPNP_PNPPID2_PID2_MASK ) )
#define SPWPNP_PNPPID2_PID2( _val ) \
  ( ( ( _val ) << SPWPNP_PNPPID2_PID2_SHIFT ) & SPWPNP_PNPPID2_PID2_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPACNT \
 *   SpaceWire Plug-and-Play - Application Count (PNPACNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPACNT_AC_SHIFT 0
#define SPWPNP_PNPACNT_AC_MASK  0xffU
#define SPWPNP_PNPACNT_AC_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPACNT_AC_MASK ) >> SPWPNP_PNPACNT_AC_SHIFT )
#define SPWPNP_PNPACNT_AC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPACNT_AC_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPACNT_AC_SHIFT ) & SPWPNP_PNPACNT_AC_MASK ) )
#define SPWPNP_PNPACNT_AC( _val ) \
  ( ( ( _val ) << SPWPNP_PNPACNT_AC_SHIFT ) & SPWPNP_PNPACNT_AC_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPTCC \
 *   SpaceWire Plug-and-Play - Time-Code Counter (PNPTCC)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPTCC_TC_SHIFT 0
#define SPWPNP_PNPTCC_TC_MASK  0x3fU
#define SPWPNP_PNPTCC_TC_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPTCC_TC_MASK ) >> SPWPNP_PNPTCC_TC_SHIFT )
#define SPWPNP_PNPTCC_TC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPTCC_TC_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPTCC_TC_SHIFT ) & SPWPNP_PNPTCC_TC_MASK ) )
#define SPWPNP_PNPTCC_TC( _val ) \
  ( ( ( _val ) << SPWPNP_PNPTCC_TC_SHIFT ) & SPWPNP_PNPTCC_TC_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPLSTS \
 *   SpaceWire Plug-and-Play - Link Status (PNPLSTS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPLSTS_ND 0x80000000U

#define SPWPNP_PNPLSTS_LT 0x40000000U

#define SPWPNP_PNPLSTS_LS_SHIFT 16
#define SPWPNP_PNPLSTS_LS_MASK  0x70000U
#define SPWPNP_PNPLSTS_LS_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPLSTS_LS_MASK ) >> SPWPNP_PNPLSTS_LS_SHIFT )
#define SPWPNP_PNPLSTS_LS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~SPWPNP_PNPLSTS_LS_MASK ) | \
    ( ( ( _val ) << SPWPNP_PNPLSTS_LS_SHIFT ) & SPWPNP_PNPLSTS_LS_MASK ) )
#define SPWPNP_PNPLSTS_LS( _val ) \
  ( ( ( _val ) << SPWPNP_PNPLSTS_LS_SHIFT ) & SPWPNP_PNPLSTS_LS_MASK )
#define SPWPNP_PNPLSTS_LS_ERROR_RESET 0U
#define SPWPNP_PNPLSTS_LS_ERROR_WAIT  1U
#define SPWPNP_PNPLSTS_LS_READY       2U
#define SPWPNP_PNPLSTS_LS_STARTED     3U
#define SPWPNP_PNPLSTS_LS_CONNECTING  4U
#define SPWPNP_PNPLSTS_LS_RUN         5U

#define SPWPNP_PNPLSTS_CE 0x40U

#define SPWPNP_PNPLSTS_ER 0x20U

#define SPWPNP_PNPLSTS_PE 0x10U

#define SPWPNP_PNPLSTS_DE 0x8U

#define SPWPNP_PNPLSTS_IA 0x2U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPLCTRL \
 *   SpaceWire Plug-and-Play - Link Control (PNPLCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPLCTRL_TT 0x10U

#define SPWPNP_PNPLCTRL_LD 0x4U

#define SPWPNP_PNPLCTRL_LS 0x2U

#define SPWPNP_PNPLCTRL_AS 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPMWLEN \
 *   SpaceWire Plug-and-Play - Maximum Write Length (PNPMWLEN)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPMWLEN_LEN_SHIFT 0
#define SPWPNP_PNPMWLEN_LEN_MASK  0x7fffU
#define SPWPNP_PNPMWLEN_LEN_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPMWLEN_LEN_MASK ) >> SPWPNP_PNPMWLEN_LEN_SHIFT )
#define SPWPNP_PNPMWLEN_LEN_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPMWLEN_LEN_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPMWLEN_LEN_SHIFT ) & \
      SPWPNP_PNPMWLEN_LEN_MASK ) )
#define SPWPNP_PNPMWLEN_LEN( _val ) \
  ( ( ( _val ) << SPWPNP_PNPMWLEN_LEN_SHIFT ) & SPWPNP_PNPMWLEN_LEN_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBSPWPNPPNPMRLEN \
 *   SpaceWire Plug-and-Play - Maximum Read Length (PNPMRLEN)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define SPWPNP_PNPMRLEN_LEN_SHIFT 0
#define SPWPNP_PNPMRLEN_LEN_MASK  0x7fffU
#define SPWPNP_PNPMRLEN_LEN_GET( _reg ) \
  ( ( ( _reg ) & SPWPNP_PNPMRLEN_LEN_MASK ) >> SPWPNP_PNPMRLEN_LEN_SHIFT )
#define SPWPNP_PNPMRLEN_LEN_SET( _reg, _val )     \
  ( ( ( _reg ) & ~SPWPNP_PNPMRLEN_LEN_MASK ) |    \
    ( ( ( _val ) << SPWPNP_PNPMRLEN_LEN_SHIFT ) & \
      SPWPNP_PNPMRLEN_LEN_MASK ) )
#define SPWPNP_PNPMRLEN_LEN( _val ) \
  ( ( ( _val ) << SPWPNP_PNPMRLEN_LEN_SHIFT ) & SPWPNP_PNPMRLEN_LEN_MASK )

/** @} */

/**
 * @name Registers
 *
 * @brief This set of defines the SpaceWire Plug-and-Play address map.
 *
 * The offsets are the word addresses which an RMAP command of the SpaceWire
 * Plug-and-Play protocol carries. The registers are not mapped into a memory
 * address space, so the block defines the offsets and no structure.
 *
 * @{
 */

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPVEND.
 */
#define SPWPNP_PNPVEND 0x0

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPVER.
 */
#define SPWPNP_PNPVER 0x1

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPDEVSTS.
 */
#define SPWPNP_PNPDEVSTS 0x2

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPACTLNK.
 */
#define SPWPNP_PNPACTLNK 0x3

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPLINFO.
 */
#define SPWPNP_PNPLINFO 0x4

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPOA0.
 */
#define SPWPNP_PNPOA0 0x5

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPOA1.
 */
#define SPWPNP_PNPOA1 0x6

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPOA2.
 */
#define SPWPNP_PNPOA2 0x7

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPDEVID.
 */
#define SPWPNP_PNPDEVID 0x8

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPUVEND.
 */
#define SPWPNP_PNPUVEND 0x9

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPUSN.
 */
#define SPWPNP_PNPUSN 0xa

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPVSTRL.
 */
#define SPWPNP_PNPVSTRL 0x4000

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPPSTRL.
 */
#define SPWPNP_PNPPSTRL 0x6000

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPPCNT.
 */
#define SPWPNP_PNPPCNT 0x8000

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPPID1.
 */
#define SPWPNP_PNPPID1 0x8001

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPPID2.
 */
#define SPWPNP_PNPPID2 0x8002

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPACNT.
 */
#define SPWPNP_PNPACNT 0xc000

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPTCC.
 */
#define SPWPNP_PNPTCC 0x80000

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPLSTS.
 */
#define SPWPNP_PNPLSTS 0x84008

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPLCTRL.
 */
#define SPWPNP_PNPLCTRL 0x84009

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPMWLEN.
 */
#define SPWPNP_PNPMWLEN 0x100000

/**
 * @brief See @ref RTEMSDeviceGRLIBSPWPNPPNPMRLEN.
 */
#define SPWPNP_PNPMRLEN 0x100001

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_SPWPNP_REGS_H */
