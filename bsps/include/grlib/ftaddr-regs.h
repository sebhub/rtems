/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSDeviceGRLIBFTADDR
 *
 * @brief This header file defines the FTADDR register block interface.
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

/* Find related documentation with spec:/dev/grlib/if/ftaddr-header */

#ifndef _GRLIB_FTADDR_REGS_H
#define _GRLIB_FTADDR_REGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Find related documentation with spec:/dev/grlib/if/ftaddr */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDR FTADDR
 *
 * @ingroup RTEMSDeviceGRLIB
 *
 * @brief This group contains the FTADDR interfaces.
 *
 * @{
 */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRFSR Feature set register (FSR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_FSR_PHYIMPL_SHIFT 24
#define FTADDR_FSR_PHYIMPL_MASK  0xff000000U
#define FTADDR_FSR_PHYIMPL_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_FSR_PHYIMPL_MASK ) >> FTADDR_FSR_PHYIMPL_SHIFT )
#define FTADDR_FSR_PHYIMPL_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_FSR_PHYIMPL_MASK ) | \
    ( ( ( _val ) << FTADDR_FSR_PHYIMPL_SHIFT ) & FTADDR_FSR_PHYIMPL_MASK ) )
#define FTADDR_FSR_PHYIMPL( _val ) \
  ( ( ( _val ) << FTADDR_FSR_PHYIMPL_SHIFT ) & FTADDR_FSR_PHYIMPL_MASK )

#define FTADDR_FSR_FREQRATIO_SHIFT 22
#define FTADDR_FSR_FREQRATIO_MASK  0xc00000U
#define FTADDR_FSR_FREQRATIO_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_FSR_FREQRATIO_MASK ) >> FTADDR_FSR_FREQRATIO_SHIFT )
#define FTADDR_FSR_FREQRATIO_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_FSR_FREQRATIO_MASK ) |    \
    ( ( ( _val ) << FTADDR_FSR_FREQRATIO_SHIFT ) & \
      FTADDR_FSR_FREQRATIO_MASK ) )
#define FTADDR_FSR_FREQRATIO( _val ) \
  ( ( ( _val ) << FTADDR_FSR_FREQRATIO_SHIFT ) & FTADDR_FSR_FREQRATIO_MASK )

#define FTADDR_FSR_TRANS 0x8000U

#define FTADDR_FSR_ECCS 0x4000U

#define FTADDR_FSR_PARS 0x2000U

#define FTADDR_FSR_NCSMAX_SHIFT 10
#define FTADDR_FSR_NCSMAX_MASK  0x1c00U
#define FTADDR_FSR_NCSMAX_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_FSR_NCSMAX_MASK ) >> FTADDR_FSR_NCSMAX_SHIFT )
#define FTADDR_FSR_NCSMAX_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_FSR_NCSMAX_MASK ) | \
    ( ( ( _val ) << FTADDR_FSR_NCSMAX_SHIFT ) & FTADDR_FSR_NCSMAX_MASK ) )
#define FTADDR_FSR_NCSMAX( _val ) \
  ( ( ( _val ) << FTADDR_FSR_NCSMAX_SHIFT ) & FTADDR_FSR_NCSMAX_MASK )

#define FTADDR_FSR_NPORT_SHIFT 8
#define FTADDR_FSR_NPORT_MASK  0x300U
#define FTADDR_FSR_NPORT_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_FSR_NPORT_MASK ) >> FTADDR_FSR_NPORT_SHIFT )
#define FTADDR_FSR_NPORT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_FSR_NPORT_MASK ) | \
    ( ( ( _val ) << FTADDR_FSR_NPORT_SHIFT ) & FTADDR_FSR_NPORT_MASK ) )
#define FTADDR_FSR_NPORT( _val ) \
  ( ( ( _val ) << FTADDR_FSR_NPORT_SHIFT ) & FTADDR_FSR_NPORT_MASK )

#define FTADDR_FSR_DDRWIDTH_SHIFT 0
#define FTADDR_FSR_DDRWIDTH_MASK  0xffU
#define FTADDR_FSR_DDRWIDTH_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_FSR_DDRWIDTH_MASK ) >> FTADDR_FSR_DDRWIDTH_SHIFT )
#define FTADDR_FSR_DDRWIDTH_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_FSR_DDRWIDTH_MASK ) |    \
    ( ( ( _val ) << FTADDR_FSR_DDRWIDTH_SHIFT ) & \
      FTADDR_FSR_DDRWIDTH_MASK ) )
#define FTADDR_FSR_DDRWIDTH( _val ) \
  ( ( ( _val ) << FTADDR_FSR_DDRWIDTH_SHIFT ) & FTADDR_FSR_DDRWIDTH_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRBSR Backend status register (BSR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_BSR_INTERROR_SHIFT 26
#define FTADDR_BSR_INTERROR_MASK  0xfc000000U
#define FTADDR_BSR_INTERROR_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BSR_INTERROR_MASK ) >> FTADDR_BSR_INTERROR_SHIFT )
#define FTADDR_BSR_INTERROR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BSR_INTERROR_MASK ) |    \
    ( ( ( _val ) << FTADDR_BSR_INTERROR_SHIFT ) & \
      FTADDR_BSR_INTERROR_MASK ) )
#define FTADDR_BSR_INTERROR( _val ) \
  ( ( ( _val ) << FTADDR_BSR_INTERROR_SHIFT ) & FTADDR_BSR_INTERROR_MASK )

#define FTADDR_BSR_BANKSTATUS1_SHIFT 16
#define FTADDR_BSR_BANKSTATUS1_MASK  0xf0000U
#define FTADDR_BSR_BANKSTATUS1_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_BSR_BANKSTATUS1_MASK ) >> \
    FTADDR_BSR_BANKSTATUS1_SHIFT )
#define FTADDR_BSR_BANKSTATUS1_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BSR_BANKSTATUS1_MASK ) |    \
    ( ( ( _val ) << FTADDR_BSR_BANKSTATUS1_SHIFT ) & \
      FTADDR_BSR_BANKSTATUS1_MASK ) )
#define FTADDR_BSR_BANKSTATUS1( _val )             \
  ( ( ( _val ) << FTADDR_BSR_BANKSTATUS1_SHIFT ) & \
    FTADDR_BSR_BANKSTATUS1_MASK )

#define FTADDR_BSR_BANKSTATUS0_SHIFT 12
#define FTADDR_BSR_BANKSTATUS0_MASK  0xf000U
#define FTADDR_BSR_BANKSTATUS0_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_BSR_BANKSTATUS0_MASK ) >> \
    FTADDR_BSR_BANKSTATUS0_SHIFT )
#define FTADDR_BSR_BANKSTATUS0_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BSR_BANKSTATUS0_MASK ) |    \
    ( ( ( _val ) << FTADDR_BSR_BANKSTATUS0_SHIFT ) & \
      FTADDR_BSR_BANKSTATUS0_MASK ) )
#define FTADDR_BSR_BANKSTATUS0( _val )             \
  ( ( ( _val ) << FTADDR_BSR_BANKSTATUS0_SHIFT ) & \
    FTADDR_BSR_BANKSTATUS0_MASK )

#define FTADDR_BSR_CMDFIFOVALID_SHIFT 8
#define FTADDR_BSR_CMDFIFOVALID_MASK  0xf00U
#define FTADDR_BSR_CMDFIFOVALID_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_BSR_CMDFIFOVALID_MASK ) >> \
    FTADDR_BSR_CMDFIFOVALID_SHIFT )
#define FTADDR_BSR_CMDFIFOVALID_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BSR_CMDFIFOVALID_MASK ) |    \
    ( ( ( _val ) << FTADDR_BSR_CMDFIFOVALID_SHIFT ) & \
      FTADDR_BSR_CMDFIFOVALID_MASK ) )
#define FTADDR_BSR_CMDFIFOVALID( _val )             \
  ( ( ( _val ) << FTADDR_BSR_CMDFIFOVALID_SHIFT ) & \
    FTADDR_BSR_CMDFIFOVALID_MASK )

#define FTADDR_BSR_RDS 0x20U

#define FTADDR_BSR_BESTATE_SHIFT 0
#define FTADDR_BSR_BESTATE_MASK  0x1fU
#define FTADDR_BSR_BESTATE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BSR_BESTATE_MASK ) >> FTADDR_BSR_BESTATE_SHIFT )
#define FTADDR_BSR_BESTATE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_BSR_BESTATE_MASK ) | \
    ( ( ( _val ) << FTADDR_BSR_BESTATE_SHIFT ) & FTADDR_BSR_BESTATE_MASK ) )
#define FTADDR_BSR_BESTATE( _val ) \
  ( ( ( _val ) << FTADDR_BSR_BESTATE_SHIFT ) & FTADDR_BSR_BESTATE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRMCFG1 \
 *   Memory configuration register 1 (MCFG1)
 *
 * @brief This group contains register bit definitions.
 *
 * CASLAT gives bits 3 to 0 and MCFG4.CASLAT4 gives bit 4 of the CAS latency
 * code. WLAT gives bits 1 and 0 and MCFG4.WLAT2 gives bit 2 of the CAS write
 * latency code.
 *
 * @{
 */

#define FTADDR_MCFG1_DDRT 0x80000000U

#define FTADDR_MCFG1_HWID 0x40000000U

#define FTADDR_MCFG1_ENX4 0x20000000U

#define FTADDR_MCFG1_PWRU 0x10000000U

#define FTADDR_MCFG1_SKIPCALIB 0x8000000U

#define FTADDR_MCFG1_SKIPINIT 0x4000000U

#define FTADDR_MCFG1_CASLAT_SHIFT 22
#define FTADDR_MCFG1_CASLAT_MASK  0x3c00000U
#define FTADDR_MCFG1_CASLAT_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG1_CASLAT_MASK ) >> FTADDR_MCFG1_CASLAT_SHIFT )
#define FTADDR_MCFG1_CASLAT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_MCFG1_CASLAT_MASK ) |    \
    ( ( ( _val ) << FTADDR_MCFG1_CASLAT_SHIFT ) & \
      FTADDR_MCFG1_CASLAT_MASK ) )
#define FTADDR_MCFG1_CASLAT( _val ) \
  ( ( ( _val ) << FTADDR_MCFG1_CASLAT_SHIFT ) & FTADDR_MCFG1_CASLAT_MASK )

#define FTADDR_MCFG1_WLAT_SHIFT 20
#define FTADDR_MCFG1_WLAT_MASK  0x300000U
#define FTADDR_MCFG1_WLAT_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG1_WLAT_MASK ) >> FTADDR_MCFG1_WLAT_SHIFT )
#define FTADDR_MCFG1_WLAT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG1_WLAT_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG1_WLAT_SHIFT ) & FTADDR_MCFG1_WLAT_MASK ) )
#define FTADDR_MCFG1_WLAT( _val ) \
  ( ( ( _val ) << FTADDR_MCFG1_WLAT_SHIFT ) & FTADDR_MCFG1_WLAT_MASK )

#define FTADDR_MCFG1_DRAS_SHIFT 13
#define FTADDR_MCFG1_DRAS_MASK  0xfe000U
#define FTADDR_MCFG1_DRAS_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG1_DRAS_MASK ) >> FTADDR_MCFG1_DRAS_SHIFT )
#define FTADDR_MCFG1_DRAS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG1_DRAS_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG1_DRAS_SHIFT ) & FTADDR_MCFG1_DRAS_MASK ) )
#define FTADDR_MCFG1_DRAS( _val ) \
  ( ( ( _val ) << FTADDR_MCFG1_DRAS_SHIFT ) & FTADDR_MCFG1_DRAS_MASK )

#define FTADDR_MCFG1_DRP_SHIFT 6
#define FTADDR_MCFG1_DRP_MASK  0x1fc0U
#define FTADDR_MCFG1_DRP_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG1_DRP_MASK ) >> FTADDR_MCFG1_DRP_SHIFT )
#define FTADDR_MCFG1_DRP_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG1_DRP_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG1_DRP_SHIFT ) & FTADDR_MCFG1_DRP_MASK ) )
#define FTADDR_MCFG1_DRP( _val ) \
  ( ( ( _val ) << FTADDR_MCFG1_DRP_SHIFT ) & FTADDR_MCFG1_DRP_MASK )

#define FTADDR_MCFG1_DFAW_SHIFT 3
#define FTADDR_MCFG1_DFAW_MASK  0x38U
#define FTADDR_MCFG1_DFAW_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG1_DFAW_MASK ) >> FTADDR_MCFG1_DFAW_SHIFT )
#define FTADDR_MCFG1_DFAW_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG1_DFAW_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG1_DFAW_SHIFT ) & FTADDR_MCFG1_DFAW_MASK ) )
#define FTADDR_MCFG1_DFAW( _val ) \
  ( ( ( _val ) << FTADDR_MCFG1_DFAW_SHIFT ) & FTADDR_MCFG1_DFAW_MASK )

#define FTADDR_MCFG1_DRRD_SHIFT 0
#define FTADDR_MCFG1_DRRD_MASK  0x7U
#define FTADDR_MCFG1_DRRD_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG1_DRRD_MASK ) >> FTADDR_MCFG1_DRRD_SHIFT )
#define FTADDR_MCFG1_DRRD_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG1_DRRD_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG1_DRRD_SHIFT ) & FTADDR_MCFG1_DRRD_MASK ) )
#define FTADDR_MCFG1_DRRD( _val ) \
  ( ( ( _val ) << FTADDR_MCFG1_DRRD_SHIFT ) & FTADDR_MCFG1_DRRD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRMCFG2 \
 *   Memory configuration register 2 (MCFG2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_MCFG2_DDRT 0x80000000U

#define FTADDR_MCFG2_HWID 0x40000000U

#define FTADDR_MCFG2_DWTR_SHIFT 25
#define FTADDR_MCFG2_DWTR_MASK  0x3e000000U
#define FTADDR_MCFG2_DWTR_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG2_DWTR_MASK ) >> FTADDR_MCFG2_DWTR_SHIFT )
#define FTADDR_MCFG2_DWTR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG2_DWTR_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG2_DWTR_SHIFT ) & FTADDR_MCFG2_DWTR_MASK ) )
#define FTADDR_MCFG2_DWTR( _val ) \
  ( ( ( _val ) << FTADDR_MCFG2_DWTR_SHIFT ) & FTADDR_MCFG2_DWTR_MASK )

#define FTADDR_MCFG2_DWR_SHIFT 20
#define FTADDR_MCFG2_DWR_MASK  0x1f00000U
#define FTADDR_MCFG2_DWR_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG2_DWR_MASK ) >> FTADDR_MCFG2_DWR_SHIFT )
#define FTADDR_MCFG2_DWR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG2_DWR_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG2_DWR_SHIFT ) & FTADDR_MCFG2_DWR_MASK ) )
#define FTADDR_MCFG2_DWR( _val ) \
  ( ( ( _val ) << FTADDR_MCFG2_DWR_SHIFT ) & FTADDR_MCFG2_DWR_MASK )

#define FTADDR_MCFG2_DRTP_SHIFT 16
#define FTADDR_MCFG2_DRTP_MASK  0xf0000U
#define FTADDR_MCFG2_DRTP_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG2_DRTP_MASK ) >> FTADDR_MCFG2_DRTP_SHIFT )
#define FTADDR_MCFG2_DRTP_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG2_DRTP_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG2_DRTP_SHIFT ) & FTADDR_MCFG2_DRTP_MASK ) )
#define FTADDR_MCFG2_DRTP( _val ) \
  ( ( ( _val ) << FTADDR_MCFG2_DRTP_SHIFT ) & FTADDR_MCFG2_DRTP_MASK )

#define FTADDR_MCFG2_DRCD_SHIFT 12
#define FTADDR_MCFG2_DRCD_MASK  0xf000U
#define FTADDR_MCFG2_DRCD_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG2_DRCD_MASK ) >> FTADDR_MCFG2_DRCD_SHIFT )
#define FTADDR_MCFG2_DRCD_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG2_DRCD_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG2_DRCD_SHIFT ) & FTADDR_MCFG2_DRCD_MASK ) )
#define FTADDR_MCFG2_DRCD( _val ) \
  ( ( ( _val ) << FTADDR_MCFG2_DRCD_SHIFT ) & FTADDR_MCFG2_DRCD_MASK )

#define FTADDR_MCFG2_DRFC_SHIFT 3
#define FTADDR_MCFG2_DRFC_MASK  0xff8U
#define FTADDR_MCFG2_DRFC_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG2_DRFC_MASK ) >> FTADDR_MCFG2_DRFC_SHIFT )
#define FTADDR_MCFG2_DRFC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG2_DRFC_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG2_DRFC_SHIFT ) & FTADDR_MCFG2_DRFC_MASK ) )
#define FTADDR_MCFG2_DRFC( _val ) \
  ( ( ( _val ) << FTADDR_MCFG2_DRFC_SHIFT ) & FTADDR_MCFG2_DRFC_MASK )

#define FTADDR_MCFG2_DLLDIS 0x4U

#define FTADDR_MCFG2_RDIMM 0x2U

#define FTADDR_MCFG2_T2 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRMCFG3 \
 *   Memory configuration register 3 (MCFG3)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_MCFG3_M3OVR 0x80000000U

#define FTADDR_MCFG3_DRTW_SHIFT 16
#define FTADDR_MCFG3_DRTW_MASK  0xf0000U
#define FTADDR_MCFG3_DRTW_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG3_DRTW_MASK ) >> FTADDR_MCFG3_DRTW_SHIFT )
#define FTADDR_MCFG3_DRTW_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG3_DRTW_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG3_DRTW_SHIFT ) & FTADDR_MCFG3_DRTW_MASK ) )
#define FTADDR_MCFG3_DRTW( _val ) \
  ( ( ( _val ) << FTADDR_MCFG3_DRTW_SHIFT ) & FTADDR_MCFG3_DRTW_MASK )

#define FTADDR_MCFG3_DRTRXCS_SHIFT 12
#define FTADDR_MCFG3_DRTRXCS_MASK  0xf000U
#define FTADDR_MCFG3_DRTRXCS_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG3_DRTRXCS_MASK ) >> FTADDR_MCFG3_DRTRXCS_SHIFT )
#define FTADDR_MCFG3_DRTRXCS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_MCFG3_DRTRXCS_MASK ) |    \
    ( ( ( _val ) << FTADDR_MCFG3_DRTRXCS_SHIFT ) & \
      FTADDR_MCFG3_DRTRXCS_MASK ) )
#define FTADDR_MCFG3_DRTRXCS( _val ) \
  ( ( ( _val ) << FTADDR_MCFG3_DRTRXCS_SHIFT ) & FTADDR_MCFG3_DRTRXCS_MASK )

#define FTADDR_MCFG3_DWTWXCS_SHIFT 8
#define FTADDR_MCFG3_DWTWXCS_MASK  0xf00U
#define FTADDR_MCFG3_DWTWXCS_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG3_DWTWXCS_MASK ) >> FTADDR_MCFG3_DWTWXCS_SHIFT )
#define FTADDR_MCFG3_DWTWXCS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_MCFG3_DWTWXCS_MASK ) |    \
    ( ( ( _val ) << FTADDR_MCFG3_DWTWXCS_SHIFT ) & \
      FTADDR_MCFG3_DWTWXCS_MASK ) )
#define FTADDR_MCFG3_DWTWXCS( _val ) \
  ( ( ( _val ) << FTADDR_MCFG3_DWTWXCS_SHIFT ) & FTADDR_MCFG3_DWTWXCS_MASK )

#define FTADDR_MCFG3_DRTWXCS_SHIFT 4
#define FTADDR_MCFG3_DRTWXCS_MASK  0xf0U
#define FTADDR_MCFG3_DRTWXCS_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG3_DRTWXCS_MASK ) >> FTADDR_MCFG3_DRTWXCS_SHIFT )
#define FTADDR_MCFG3_DRTWXCS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_MCFG3_DRTWXCS_MASK ) |    \
    ( ( ( _val ) << FTADDR_MCFG3_DRTWXCS_SHIFT ) & \
      FTADDR_MCFG3_DRTWXCS_MASK ) )
#define FTADDR_MCFG3_DRTWXCS( _val ) \
  ( ( ( _val ) << FTADDR_MCFG3_DRTWXCS_SHIFT ) & FTADDR_MCFG3_DRTWXCS_MASK )

#define FTADDR_MCFG3_DWTRXCS_SHIFT 0
#define FTADDR_MCFG3_DWTRXCS_MASK  0xfU
#define FTADDR_MCFG3_DWTRXCS_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG3_DWTRXCS_MASK ) >> FTADDR_MCFG3_DWTRXCS_SHIFT )
#define FTADDR_MCFG3_DWTRXCS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_MCFG3_DWTRXCS_MASK ) |    \
    ( ( ( _val ) << FTADDR_MCFG3_DWTRXCS_SHIFT ) & \
      FTADDR_MCFG3_DWTRXCS_MASK ) )
#define FTADDR_MCFG3_DWTRXCS( _val ) \
  ( ( ( _val ) << FTADDR_MCFG3_DWTRXCS_SHIFT ) & FTADDR_MCFG3_DWTRXCS_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRMCFG4 \
 *   Memory configuration register 4 (MCFG4)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_MCFG4_DATA_BYTE_DISABLE_SHIFT 20
#define FTADDR_MCFG4_DATA_BYTE_DISABLE_MASK  0xfff00000U
#define FTADDR_MCFG4_DATA_BYTE_DISABLE_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_MCFG4_DATA_BYTE_DISABLE_MASK ) >> \
    FTADDR_MCFG4_DATA_BYTE_DISABLE_SHIFT )
#define FTADDR_MCFG4_DATA_BYTE_DISABLE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_MCFG4_DATA_BYTE_DISABLE_MASK ) |    \
    ( ( ( _val ) << FTADDR_MCFG4_DATA_BYTE_DISABLE_SHIFT ) & \
      FTADDR_MCFG4_DATA_BYTE_DISABLE_MASK ) )
#define FTADDR_MCFG4_DATA_BYTE_DISABLE( _val )             \
  ( ( ( _val ) << FTADDR_MCFG4_DATA_BYTE_DISABLE_SHIFT ) & \
    FTADDR_MCFG4_DATA_BYTE_DISABLE_MASK )

#define FTADDR_MCFG4_CASLAT4 0x10000U

#define FTADDR_MCFG4_DRRDL_SHIFT 13
#define FTADDR_MCFG4_DRRDL_MASK  0xe000U
#define FTADDR_MCFG4_DRRDL_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG4_DRRDL_MASK ) >> FTADDR_MCFG4_DRRDL_SHIFT )
#define FTADDR_MCFG4_DRRDL_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG4_DRRDL_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG4_DRRDL_SHIFT ) & FTADDR_MCFG4_DRRDL_MASK ) )
#define FTADDR_MCFG4_DRRDL( _val ) \
  ( ( ( _val ) << FTADDR_MCFG4_DRRDL_SHIFT ) & FTADDR_MCFG4_DRRDL_MASK )

#define FTADDR_MCFG4_DWTRL_SHIFT 9
#define FTADDR_MCFG4_DWTRL_MASK  0x1e00U
#define FTADDR_MCFG4_DWTRL_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG4_DWTRL_MASK ) >> FTADDR_MCFG4_DWTRL_SHIFT )
#define FTADDR_MCFG4_DWTRL_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG4_DWTRL_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG4_DWTRL_SHIFT ) & FTADDR_MCFG4_DWTRL_MASK ) )
#define FTADDR_MCFG4_DWTRL( _val ) \
  ( ( ( _val ) << FTADDR_MCFG4_DWTRL_SHIFT ) & FTADDR_MCFG4_DWTRL_MASK )

#define FTADDR_MCFG4_DCCDL_SHIFT 6
#define FTADDR_MCFG4_DCCDL_MASK  0x1c0U
#define FTADDR_MCFG4_DCCDL_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG4_DCCDL_MASK ) >> FTADDR_MCFG4_DCCDL_SHIFT )
#define FTADDR_MCFG4_DCCDL_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG4_DCCDL_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG4_DCCDL_SHIFT ) & FTADDR_MCFG4_DCCDL_MASK ) )
#define FTADDR_MCFG4_DCCDL( _val ) \
  ( ( ( _val ) << FTADDR_MCFG4_DCCDL_SHIFT ) & FTADDR_MCFG4_DCCDL_MASK )

#define FTADDR_MCFG4_WLAT2 0x20U

#define FTADDR_MCFG4_DMOD_SHIFT 0
#define FTADDR_MCFG4_DMOD_MASK  0x1fU
#define FTADDR_MCFG4_DMOD_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MCFG4_DMOD_MASK ) >> FTADDR_MCFG4_DMOD_SHIFT )
#define FTADDR_MCFG4_DMOD_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MCFG4_DMOD_MASK ) | \
    ( ( ( _val ) << FTADDR_MCFG4_DMOD_SHIFT ) & FTADDR_MCFG4_DMOD_MASK ) )
#define FTADDR_MCFG4_DMOD( _val ) \
  ( ( ( _val ) << FTADDR_MCFG4_DMOD_SHIFT ) & FTADDR_MCFG4_DMOD_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCFG1 \
 *   Service configuration register 1 (SCFG1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCFG1_SHOLD 0x80000000U

#define FTADDR_SCFG1_FREOP 0x10000000U

#define FTADDR_SCFG1_MRREPIVAL_SHIFT 20
#define FTADDR_SCFG1_MRREPIVAL_MASK  0xff00000U
#define FTADDR_SCFG1_MRREPIVAL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCFG1_MRREPIVAL_MASK ) >> \
    FTADDR_SCFG1_MRREPIVAL_SHIFT )
#define FTADDR_SCFG1_MRREPIVAL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCFG1_MRREPIVAL_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCFG1_MRREPIVAL_SHIFT ) & \
      FTADDR_SCFG1_MRREPIVAL_MASK ) )
#define FTADDR_SCFG1_MRREPIVAL( _val )             \
  ( ( ( _val ) << FTADDR_SCFG1_MRREPIVAL_SHIFT ) & \
    FTADDR_SCFG1_MRREPIVAL_MASK )

#define FTADDR_SCFG1_REFIVAL_SHIFT 12
#define FTADDR_SCFG1_REFIVAL_MASK  0xff000U
#define FTADDR_SCFG1_REFIVAL_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_SCFG1_REFIVAL_MASK ) >> FTADDR_SCFG1_REFIVAL_SHIFT )
#define FTADDR_SCFG1_REFIVAL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCFG1_REFIVAL_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCFG1_REFIVAL_SHIFT ) & \
      FTADDR_SCFG1_REFIVAL_MASK ) )
#define FTADDR_SCFG1_REFIVAL( _val ) \
  ( ( ( _val ) << FTADDR_SCFG1_REFIVAL_SHIFT ) & FTADDR_SCFG1_REFIVAL_MASK )

#define FTADDR_SCFG1_SRVBASEPER_SHIFT 0
#define FTADDR_SCFG1_SRVBASEPER_MASK  0xfffU
#define FTADDR_SCFG1_SRVBASEPER_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCFG1_SRVBASEPER_MASK ) >> \
    FTADDR_SCFG1_SRVBASEPER_SHIFT )
#define FTADDR_SCFG1_SRVBASEPER_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCFG1_SRVBASEPER_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCFG1_SRVBASEPER_SHIFT ) & \
      FTADDR_SCFG1_SRVBASEPER_MASK ) )
#define FTADDR_SCFG1_SRVBASEPER( _val )             \
  ( ( ( _val ) << FTADDR_SCFG1_SRVBASEPER_SHIFT ) & \
    FTADDR_SCFG1_SRVBASEPER_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCFG2 \
 *   Service configuration register 2 (SCFG2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCFG2_U100MULT_SHIFT 24
#define FTADDR_SCFG2_U100MULT_MASK  0xff000000U
#define FTADDR_SCFG2_U100MULT_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_SCFG2_U100MULT_MASK ) >> FTADDR_SCFG2_U100MULT_SHIFT )
#define FTADDR_SCFG2_U100MULT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCFG2_U100MULT_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCFG2_U100MULT_SHIFT ) & \
      FTADDR_SCFG2_U100MULT_MASK ) )
#define FTADDR_SCFG2_U100MULT( _val ) \
  ( ( ( _val ) << FTADDR_SCFG2_U100MULT_SHIFT ) & FTADDR_SCFG2_U100MULT_MASK )

#define FTADDR_SCFG2_ZQCSIVAL_SHIFT 16
#define FTADDR_SCFG2_ZQCSIVAL_MASK  0xff0000U
#define FTADDR_SCFG2_ZQCSIVAL_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_SCFG2_ZQCSIVAL_MASK ) >> FTADDR_SCFG2_ZQCSIVAL_SHIFT )
#define FTADDR_SCFG2_ZQCSIVAL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCFG2_ZQCSIVAL_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCFG2_ZQCSIVAL_SHIFT ) & \
      FTADDR_SCFG2_ZQCSIVAL_MASK ) )
#define FTADDR_SCFG2_ZQCSIVAL( _val ) \
  ( ( ( _val ) << FTADDR_SCFG2_ZQCSIVAL_SHIFT ) & FTADDR_SCFG2_ZQCSIVAL_MASK )

#define FTADDR_SCFG2_FTRAINIVAL_SHIFT 8
#define FTADDR_SCFG2_FTRAINIVAL_MASK  0xff00U
#define FTADDR_SCFG2_FTRAINIVAL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCFG2_FTRAINIVAL_MASK ) >> \
    FTADDR_SCFG2_FTRAINIVAL_SHIFT )
#define FTADDR_SCFG2_FTRAINIVAL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCFG2_FTRAINIVAL_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCFG2_FTRAINIVAL_SHIFT ) & \
      FTADDR_SCFG2_FTRAINIVAL_MASK ) )
#define FTADDR_SCFG2_FTRAINIVAL( _val )             \
  ( ( ( _val ) << FTADDR_SCFG2_FTRAINIVAL_SHIFT ) & \
    FTADDR_SCFG2_FTRAINIVAL_MASK )

#define FTADDR_SCFG2_ITRAINIVAL_SHIFT 0
#define FTADDR_SCFG2_ITRAINIVAL_MASK  0xffU
#define FTADDR_SCFG2_ITRAINIVAL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCFG2_ITRAINIVAL_MASK ) >> \
    FTADDR_SCFG2_ITRAINIVAL_SHIFT )
#define FTADDR_SCFG2_ITRAINIVAL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCFG2_ITRAINIVAL_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCFG2_ITRAINIVAL_SHIFT ) & \
      FTADDR_SCFG2_ITRAINIVAL_MASK ) )
#define FTADDR_SCFG2_ITRAINIVAL( _val )             \
  ( ( ( _val ) << FTADDR_SCFG2_ITRAINIVAL_SHIFT ) & \
    FTADDR_SCFG2_ITRAINIVAL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPHYTIM1 PHY timing register 1 (PHYTIM1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PHYTIM1_PTOR 0x80000000U

#define FTADDR_PHYTIM1_RDEN_MASK_SHIFT 0
#define FTADDR_PHYTIM1_RDEN_MASK_MASK  0xfffffffU
#define FTADDR_PHYTIM1_RDEN_MASK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PHYTIM1_RDEN_MASK_MASK ) >> \
    FTADDR_PHYTIM1_RDEN_MASK_SHIFT )
#define FTADDR_PHYTIM1_RDEN_MASK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PHYTIM1_RDEN_MASK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PHYTIM1_RDEN_MASK_SHIFT ) & \
      FTADDR_PHYTIM1_RDEN_MASK_MASK ) )
#define FTADDR_PHYTIM1_RDEN_MASK( _val )             \
  ( ( ( _val ) << FTADDR_PHYTIM1_RDEN_MASK_SHIFT ) & \
    FTADDR_PHYTIM1_RDEN_MASK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPHYTIM2 PHY timing register 2 (PHYTIM2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PHYTIM2_WREN_MASK_SHIFT 0
#define FTADDR_PHYTIM2_WREN_MASK_MASK  0x1fffffU
#define FTADDR_PHYTIM2_WREN_MASK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PHYTIM2_WREN_MASK_MASK ) >> \
    FTADDR_PHYTIM2_WREN_MASK_SHIFT )
#define FTADDR_PHYTIM2_WREN_MASK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PHYTIM2_WREN_MASK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PHYTIM2_WREN_MASK_SHIFT ) & \
      FTADDR_PHYTIM2_WREN_MASK_MASK ) )
#define FTADDR_PHYTIM2_WREN_MASK( _val )             \
  ( ( ( _val ) << FTADDR_PHYTIM2_WREN_MASK_SHIFT ) & \
    FTADDR_PHYTIM2_WREN_MASK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRDACTRL1 \
 *   Diagnostic access control register 1 (DACTRL1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_DACTRL1_DIAGBG_SHIFT 24
#define FTADDR_DACTRL1_DIAGBG_MASK  0x3000000U
#define FTADDR_DACTRL1_DIAGBG_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_DACTRL1_DIAGBG_MASK ) >> FTADDR_DACTRL1_DIAGBG_SHIFT )
#define FTADDR_DACTRL1_DIAGBG_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_DACTRL1_DIAGBG_MASK ) |    \
    ( ( ( _val ) << FTADDR_DACTRL1_DIAGBG_SHIFT ) & \
      FTADDR_DACTRL1_DIAGBG_MASK ) )
#define FTADDR_DACTRL1_DIAGBG( _val ) \
  ( ( ( _val ) << FTADDR_DACTRL1_DIAGBG_SHIFT ) & FTADDR_DACTRL1_DIAGBG_MASK )

#define FTADDR_DACTRL1_DIAGCS_SHIFT 20
#define FTADDR_DACTRL1_DIAGCS_MASK  0xf00000U
#define FTADDR_DACTRL1_DIAGCS_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_DACTRL1_DIAGCS_MASK ) >> FTADDR_DACTRL1_DIAGCS_SHIFT )
#define FTADDR_DACTRL1_DIAGCS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_DACTRL1_DIAGCS_MASK ) |    \
    ( ( ( _val ) << FTADDR_DACTRL1_DIAGCS_SHIFT ) & \
      FTADDR_DACTRL1_DIAGCS_MASK ) )
#define FTADDR_DACTRL1_DIAGCS( _val ) \
  ( ( ( _val ) << FTADDR_DACTRL1_DIAGCS_SHIFT ) & FTADDR_DACTRL1_DIAGCS_MASK )

#define FTADDR_DACTRL1_R16 0x80000U

#define FTADDR_DACTRL1_DIAGBANK_SHIFT 16
#define FTADDR_DACTRL1_DIAGBANK_MASK  0x70000U
#define FTADDR_DACTRL1_DIAGBANK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_DACTRL1_DIAGBANK_MASK ) >> \
    FTADDR_DACTRL1_DIAGBANK_SHIFT )
#define FTADDR_DACTRL1_DIAGBANK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_DACTRL1_DIAGBANK_MASK ) |    \
    ( ( ( _val ) << FTADDR_DACTRL1_DIAGBANK_SHIFT ) & \
      FTADDR_DACTRL1_DIAGBANK_MASK ) )
#define FTADDR_DACTRL1_DIAGBANK( _val )             \
  ( ( ( _val ) << FTADDR_DACTRL1_DIAGBANK_SHIFT ) & \
    FTADDR_DACTRL1_DIAGBANK_MASK )

#define FTADDR_DACTRL1_DIAGROW_SHIFT 0
#define FTADDR_DACTRL1_DIAGROW_MASK  0xffffU
#define FTADDR_DACTRL1_DIAGROW_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_DACTRL1_DIAGROW_MASK ) >> \
    FTADDR_DACTRL1_DIAGROW_SHIFT )
#define FTADDR_DACTRL1_DIAGROW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_DACTRL1_DIAGROW_MASK ) |    \
    ( ( ( _val ) << FTADDR_DACTRL1_DIAGROW_SHIFT ) & \
      FTADDR_DACTRL1_DIAGROW_MASK ) )
#define FTADDR_DACTRL1_DIAGROW( _val )             \
  ( ( ( _val ) << FTADDR_DACTRL1_DIAGROW_SHIFT ) & \
    FTADDR_DACTRL1_DIAGROW_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRDACTRL2 \
 *   Diagnostic access control register 2 (DACTRL2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_DACTRL2_DODG 0x80000000U

#define FTADDR_DACTRL2_DGWR 0x40000000U

#define FTADDR_DACTRL2_MANE 0x20000000U

#define FTADDR_DACTRL2_MRASN 0x10000000U

#define FTADDR_DACTRL2_MCASN 0x8000000U

#define FTADDR_DACTRL2_MWEN 0x4000000U

#define FTADDR_DACTRL2_MRDC 0x2000000U

#define FTADDR_DACTRL2_MWRC 0x1000000U

#define FTADDR_DACTRL2_MANUAL_CSN_SHIFT 16
#define FTADDR_DACTRL2_MANUAL_CSN_MASK  0xff0000U
#define FTADDR_DACTRL2_MANUAL_CSN_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_DACTRL2_MANUAL_CSN_MASK ) >> \
    FTADDR_DACTRL2_MANUAL_CSN_SHIFT )
#define FTADDR_DACTRL2_MANUAL_CSN_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_DACTRL2_MANUAL_CSN_MASK ) |    \
    ( ( ( _val ) << FTADDR_DACTRL2_MANUAL_CSN_SHIFT ) & \
      FTADDR_DACTRL2_MANUAL_CSN_MASK ) )
#define FTADDR_DACTRL2_MANUAL_CSN( _val )             \
  ( ( ( _val ) << FTADDR_DACTRL2_MANUAL_CSN_SHIFT ) & \
    FTADDR_DACTRL2_MANUAL_CSN_MASK )

#define FTADDR_DACTRL2_MGTRD 0x8000U

#define FTADDR_DACTRL2_MSREF 0x4000U

#define FTADDR_DACTRL2_DIAGCOL_SHIFT 0
#define FTADDR_DACTRL2_DIAGCOL_MASK  0xfffU
#define FTADDR_DACTRL2_DIAGCOL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_DACTRL2_DIAGCOL_MASK ) >> \
    FTADDR_DACTRL2_DIAGCOL_SHIFT )
#define FTADDR_DACTRL2_DIAGCOL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_DACTRL2_DIAGCOL_MASK ) |    \
    ( ( ( _val ) << FTADDR_DACTRL2_DIAGCOL_SHIFT ) & \
      FTADDR_DACTRL2_DIAGCOL_MASK ) )
#define FTADDR_DACTRL2_DIAGCOL( _val )             \
  ( ( ( _val ) << FTADDR_DACTRL2_DIAGCOL_SHIFT ) & \
    FTADDR_DACTRL2_DIAGCOL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRDASTAT \
 *   Diagnostic access status register (DASTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_DASTAT_DDONE 0x80000000U

#define FTADDR_DASTAT_DGUE 0x1000000U

#define FTADDR_DASTAT_DIAGCEMASK_SHIFT 0
#define FTADDR_DASTAT_DIAGCEMASK_MASK  0xffffffU
#define FTADDR_DASTAT_DIAGCEMASK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_DASTAT_DIAGCEMASK_MASK ) >> \
    FTADDR_DASTAT_DIAGCEMASK_SHIFT )
#define FTADDR_DASTAT_DIAGCEMASK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_DASTAT_DIAGCEMASK_MASK ) |    \
    ( ( ( _val ) << FTADDR_DASTAT_DIAGCEMASK_SHIFT ) & \
      FTADDR_DASTAT_DIAGCEMASK_MASK ) )
#define FTADDR_DASTAT_DIAGCEMASK( _val )             \
  ( ( ( _val ) << FTADDR_DASTAT_DIAGCEMASK_SHIFT ) & \
    FTADDR_DASTAT_DIAGCEMASK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRDACB Diagnostic checkbit register (DACB)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_DACB_DIAGCB_SHIFT 0
#define FTADDR_DACB_DIAGCB_MASK  0xffffffffU
#define FTADDR_DACB_DIAGCB_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_DACB_DIAGCB_MASK ) >> FTADDR_DACB_DIAGCB_SHIFT )
#define FTADDR_DACB_DIAGCB_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_DACB_DIAGCB_MASK ) | \
    ( ( ( _val ) << FTADDR_DACB_DIAGCB_SHIFT ) & FTADDR_DACB_DIAGCB_MASK ) )
#define FTADDR_DACB_DIAGCB( _val ) \
  ( ( ( _val ) << FTADDR_DACB_DIAGCB_SHIFT ) & FTADDR_DACB_DIAGCB_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRDADATA1 Diagnostic data register 1 (DADATA1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_DADATA1_DIAGDATA1_SHIFT 0
#define FTADDR_DADATA1_DIAGDATA1_MASK  0xffffffffU
#define FTADDR_DADATA1_DIAGDATA1_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_DADATA1_DIAGDATA1_MASK ) >> \
    FTADDR_DADATA1_DIAGDATA1_SHIFT )
#define FTADDR_DADATA1_DIAGDATA1_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_DADATA1_DIAGDATA1_MASK ) |    \
    ( ( ( _val ) << FTADDR_DADATA1_DIAGDATA1_SHIFT ) & \
      FTADDR_DADATA1_DIAGDATA1_MASK ) )
#define FTADDR_DADATA1_DIAGDATA1( _val )             \
  ( ( ( _val ) << FTADDR_DADATA1_DIAGDATA1_SHIFT ) & \
    FTADDR_DADATA1_DIAGDATA1_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRDADATA2 Diagnostic data register 2 (DADATA2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_DADATA2_DIAGDATA2_SHIFT 0
#define FTADDR_DADATA2_DIAGDATA2_MASK  0xffffffffU
#define FTADDR_DADATA2_DIAGDATA2_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_DADATA2_DIAGDATA2_MASK ) >> \
    FTADDR_DADATA2_DIAGDATA2_SHIFT )
#define FTADDR_DADATA2_DIAGDATA2_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_DADATA2_DIAGDATA2_MASK ) |    \
    ( ( ( _val ) << FTADDR_DADATA2_DIAGDATA2_SHIFT ) & \
      FTADDR_DADATA2_DIAGDATA2_MASK ) )
#define FTADDR_DADATA2_DIAGDATA2( _val )             \
  ( ( ( _val ) << FTADDR_DADATA2_DIAGDATA2_SHIFT ) & \
    FTADDR_DADATA2_DIAGDATA2_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRODTCFG \
 *   ODT configuration register for chip select (ODTCFG)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_ODTCFG_ODTWL 0x4000000U

#define FTADDR_ODTCFG_RTTWL_SHIFT 24
#define FTADDR_ODTCFG_RTTWL_MASK  0x3000000U
#define FTADDR_ODTCFG_RTTWL_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_ODTCFG_RTTWL_MASK ) >> FTADDR_ODTCFG_RTTWL_SHIFT )
#define FTADDR_ODTCFG_RTTWL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_ODTCFG_RTTWL_MASK ) |    \
    ( ( ( _val ) << FTADDR_ODTCFG_RTTWL_SHIFT ) & \
      FTADDR_ODTCFG_RTTWL_MASK ) )
#define FTADDR_ODTCFG_RTTWL( _val ) \
  ( ( ( _val ) << FTADDR_ODTCFG_RTTWL_SHIFT ) & FTADDR_ODTCFG_RTTWL_MASK )

#define FTADDR_ODTCFG_RTT_SHIFT 21
#define FTADDR_ODTCFG_RTT_MASK  0xe00000U
#define FTADDR_ODTCFG_RTT_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_ODTCFG_RTT_MASK ) >> FTADDR_ODTCFG_RTT_SHIFT )
#define FTADDR_ODTCFG_RTT_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_ODTCFG_RTT_MASK ) | \
    ( ( ( _val ) << FTADDR_ODTCFG_RTT_SHIFT ) & FTADDR_ODTCFG_RTT_MASK ) )
#define FTADDR_ODTCFG_RTT( _val ) \
  ( ( ( _val ) << FTADDR_ODTCFG_RTT_SHIFT ) & FTADDR_ODTCFG_RTT_MASK )

#define FTADDR_ODTCFG_RTTWR_SHIFT 19
#define FTADDR_ODTCFG_RTTWR_MASK  0x180000U
#define FTADDR_ODTCFG_RTTWR_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_ODTCFG_RTTWR_MASK ) >> FTADDR_ODTCFG_RTTWR_SHIFT )
#define FTADDR_ODTCFG_RTTWR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_ODTCFG_RTTWR_MASK ) |    \
    ( ( ( _val ) << FTADDR_ODTCFG_RTTWR_SHIFT ) & \
      FTADDR_ODTCFG_RTTWR_MASK ) )
#define FTADDR_ODTCFG_RTTWR( _val ) \
  ( ( ( _val ) << FTADDR_ODTCFG_RTTWR_SHIFT ) & FTADDR_ODTCFG_RTTWR_MASK )

#define FTADDR_ODTCFG_DIC_SHIFT 17
#define FTADDR_ODTCFG_DIC_MASK  0x60000U
#define FTADDR_ODTCFG_DIC_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_ODTCFG_DIC_MASK ) >> FTADDR_ODTCFG_DIC_SHIFT )
#define FTADDR_ODTCFG_DIC_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_ODTCFG_DIC_MASK ) | \
    ( ( ( _val ) << FTADDR_ODTCFG_DIC_SHIFT ) & FTADDR_ODTCFG_DIC_MASK ) )
#define FTADDR_ODTCFG_DIC( _val ) \
  ( ( ( _val ) << FTADDR_ODTCFG_DIC_SHIFT ) & FTADDR_ODTCFG_DIC_MASK )

#define FTADDR_ODTCFG_RLODT 0x10000U

#define FTADDR_ODTCFG_RDODT_SHIFT 8
#define FTADDR_ODTCFG_RDODT_MASK  0xff00U
#define FTADDR_ODTCFG_RDODT_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_ODTCFG_RDODT_MASK ) >> FTADDR_ODTCFG_RDODT_SHIFT )
#define FTADDR_ODTCFG_RDODT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_ODTCFG_RDODT_MASK ) |    \
    ( ( ( _val ) << FTADDR_ODTCFG_RDODT_SHIFT ) & \
      FTADDR_ODTCFG_RDODT_MASK ) )
#define FTADDR_ODTCFG_RDODT( _val ) \
  ( ( ( _val ) << FTADDR_ODTCFG_RDODT_SHIFT ) & FTADDR_ODTCFG_RDODT_MASK )

#define FTADDR_ODTCFG_WRODT_SHIFT 0
#define FTADDR_ODTCFG_WRODT_MASK  0xffU
#define FTADDR_ODTCFG_WRODT_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_ODTCFG_WRODT_MASK ) >> FTADDR_ODTCFG_WRODT_SHIFT )
#define FTADDR_ODTCFG_WRODT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_ODTCFG_WRODT_MASK ) |    \
    ( ( ( _val ) << FTADDR_ODTCFG_WRODT_SHIFT ) & \
      FTADDR_ODTCFG_WRODT_MASK ) )
#define FTADDR_ODTCFG_WRODT( _val ) \
  ( ( ( _val ) << FTADDR_ODTCFG_WRODT_SHIFT ) & FTADDR_ODTCFG_WRODT_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRODTDEF ODT default value register (ODTDEF)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_ODTDEF_ODTDEFAULT_SHIFT 0
#define FTADDR_ODTDEF_ODTDEFAULT_MASK  0xffU
#define FTADDR_ODTDEF_ODTDEFAULT_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_ODTDEF_ODTDEFAULT_MASK ) >> \
    FTADDR_ODTDEF_ODTDEFAULT_SHIFT )
#define FTADDR_ODTDEF_ODTDEFAULT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_ODTDEF_ODTDEFAULT_MASK ) |    \
    ( ( ( _val ) << FTADDR_ODTDEF_ODTDEFAULT_SHIFT ) & \
      FTADDR_ODTDEF_ODTDEFAULT_MASK ) )
#define FTADDR_ODTDEF_ODTDEFAULT( _val )             \
  ( ( ( _val ) << FTADDR_ODTDEF_ODTDEFAULT_SHIFT ) & \
    FTADDR_ODTDEF_ODTDEFAULT_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRODTIRTIM \
 *   ODT internal read timing register (ODTIRTIM)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_ODTIRTIM_LODTRMASK_SHIFT 0
#define FTADDR_ODTIRTIM_LODTRMASK_MASK  0xfffffffU
#define FTADDR_ODTIRTIM_LODTRMASK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_ODTIRTIM_LODTRMASK_MASK ) >> \
    FTADDR_ODTIRTIM_LODTRMASK_SHIFT )
#define FTADDR_ODTIRTIM_LODTRMASK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_ODTIRTIM_LODTRMASK_MASK ) |    \
    ( ( ( _val ) << FTADDR_ODTIRTIM_LODTRMASK_SHIFT ) & \
      FTADDR_ODTIRTIM_LODTRMASK_MASK ) )
#define FTADDR_ODTIRTIM_LODTRMASK( _val )             \
  ( ( ( _val ) << FTADDR_ODTIRTIM_LODTRMASK_SHIFT ) & \
    FTADDR_ODTIRTIM_LODTRMASK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRCMD Command register (CMD)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_CMD_BL_DISABLE_SHIFT 20
#define FTADDR_CMD_BL_DISABLE_MASK  0xfff00000U
#define FTADDR_CMD_BL_DISABLE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_CMD_BL_DISABLE_MASK ) >> FTADDR_CMD_BL_DISABLE_SHIFT )
#define FTADDR_CMD_BL_DISABLE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_CMD_BL_DISABLE_MASK ) |    \
    ( ( ( _val ) << FTADDR_CMD_BL_DISABLE_SHIFT ) & \
      FTADDR_CMD_BL_DISABLE_MASK ) )
#define FTADDR_CMD_BL_DISABLE( _val ) \
  ( ( ( _val ) << FTADDR_CMD_BL_DISABLE_SHIFT ) & FTADDR_CMD_BL_DISABLE_MASK )

#define FTADDR_CMD_MREB 0x40000U

#define FTADDR_CMD_DOZQL 0x20000U

#define FTADDR_CMD_DOZQS 0x10000U

#define FTADDR_CMD_SPRST 0x8000U

#define FTADDR_CMD_TRLANE_SHIFT 11
#define FTADDR_CMD_TRLANE_MASK  0x7800U
#define FTADDR_CMD_TRLANE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_CMD_TRLANE_MASK ) >> FTADDR_CMD_TRLANE_SHIFT )
#define FTADDR_CMD_TRLANE_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_CMD_TRLANE_MASK ) | \
    ( ( ( _val ) << FTADDR_CMD_TRLANE_SHIFT ) & FTADDR_CMD_TRLANE_MASK ) )
#define FTADDR_CMD_TRLANE( _val ) \
  ( ( ( _val ) << FTADDR_CMD_TRLANE_SHIFT ) & FTADDR_CMD_TRLANE_MASK )

#define FTADDR_CMD_BLRDT 0x400U

#define FTADDR_CMD_BLGT 0x200U

#define FTADDR_CMD_BLWLT 0x100U

#define FTADDR_CMD_FRDT 0x80U

#define FTADDR_CMD_FGT 0x40U

#define FTADDR_CMD_FWLT 0x20U

#define FTADDR_CMD_INRDT 0x10U

#define FTADDR_CMD_INGT 0x8U

#define FTADDR_CMD_INWLT 0x4U

#define FTADDR_CMD_DOMR 0x2U

#define FTADDR_CMD_DOREF 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSREFCFG \
 *   Self-refresh mode configuration register (SREFCFG)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SREFCFG_SREFTHRES_SHIFT 1
#define FTADDR_SREFCFG_SREFTHRES_MASK  0xeU
#define FTADDR_SREFCFG_SREFTHRES_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SREFCFG_SREFTHRES_MASK ) >> \
    FTADDR_SREFCFG_SREFTHRES_SHIFT )
#define FTADDR_SREFCFG_SREFTHRES_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SREFCFG_SREFTHRES_MASK ) |    \
    ( ( ( _val ) << FTADDR_SREFCFG_SREFTHRES_SHIFT ) & \
      FTADDR_SREFCFG_SREFTHRES_MASK ) )
#define FTADDR_SREFCFG_SREFTHRES( _val )             \
  ( ( ( _val ) << FTADDR_SREFCFG_SREFTHRES_SHIFT ) & \
    FTADDR_SREFCFG_SREFTHRES_MASK )

#define FTADDR_SREFCFG_ASRFE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRBEECNT \
 *   Backend EDAC and FIFO error counter register (BEECNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_BEECNT_ECCMODE_SHIFT 30
#define FTADDR_BEECNT_ECCMODE_MASK  0xc0000000U
#define FTADDR_BEECNT_ECCMODE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEECNT_ECCMODE_MASK ) >> FTADDR_BEECNT_ECCMODE_SHIFT )
#define FTADDR_BEECNT_ECCMODE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEECNT_ECCMODE_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEECNT_ECCMODE_SHIFT ) & \
      FTADDR_BEECNT_ECCMODE_MASK ) )
#define FTADDR_BEECNT_ECCMODE( _val ) \
  ( ( ( _val ) << FTADDR_BEECNT_ECCMODE_SHIFT ) & FTADDR_BEECNT_ECCMODE_MASK )
#define FTADDR_BEECNT_ECCMODE_NONE      0U
#define FTADDR_BEECNT_ECCMODE_PARITY    1U
#define FTADDR_BEECNT_ECCMODE_EDAC      2U
#define FTADDR_BEECNT_ECCMODE_ECC_80_72 3U

#define FTADDR_BEECNT_ERET 0x20000000U

#define FTADDR_BEECNT_RDEEN 0x10000000U

#define FTADDR_BEECNT_DRMW 0x8000000U

#define FTADDR_BEECNT_RETTEST_SHIFT 0
#define FTADDR_BEECNT_RETTEST_MASK  0xfU
#define FTADDR_BEECNT_RETTEST_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEECNT_RETTEST_MASK ) >> FTADDR_BEECNT_RETTEST_SHIFT )
#define FTADDR_BEECNT_RETTEST_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEECNT_RETTEST_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEECNT_RETTEST_SHIFT ) & \
      FTADDR_BEECNT_RETTEST_MASK ) )
#define FTADDR_BEECNT_RETTEST( _val ) \
  ( ( ( _val ) << FTADDR_BEECNT_RETTEST_SHIFT ) & FTADDR_BEECNT_RETTEST_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSTCNT Service time counter register (STCNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_STCNT_SITICK 0x10000000U

#define FTADDR_STCNT_MRREPCTR_SHIFT 20
#define FTADDR_STCNT_MRREPCTR_MASK  0xff00000U
#define FTADDR_STCNT_MRREPCTR_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_STCNT_MRREPCTR_MASK ) >> FTADDR_STCNT_MRREPCTR_SHIFT )
#define FTADDR_STCNT_MRREPCTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_STCNT_MRREPCTR_MASK ) |    \
    ( ( ( _val ) << FTADDR_STCNT_MRREPCTR_SHIFT ) & \
      FTADDR_STCNT_MRREPCTR_MASK ) )
#define FTADDR_STCNT_MRREPCTR( _val ) \
  ( ( ( _val ) << FTADDR_STCNT_MRREPCTR_SHIFT ) & FTADDR_STCNT_MRREPCTR_MASK )

#define FTADDR_STCNT_REFCTR_SHIFT 12
#define FTADDR_STCNT_REFCTR_MASK  0xff000U
#define FTADDR_STCNT_REFCTR_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_STCNT_REFCTR_MASK ) >> FTADDR_STCNT_REFCTR_SHIFT )
#define FTADDR_STCNT_REFCTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_STCNT_REFCTR_MASK ) |    \
    ( ( ( _val ) << FTADDR_STCNT_REFCTR_SHIFT ) & \
      FTADDR_STCNT_REFCTR_MASK ) )
#define FTADDR_STCNT_REFCTR( _val ) \
  ( ( ( _val ) << FTADDR_STCNT_REFCTR_SHIFT ) & FTADDR_STCNT_REFCTR_MASK )

#define FTADDR_STCNT_SICTR_SHIFT 0
#define FTADDR_STCNT_SICTR_MASK  0xfffU
#define FTADDR_STCNT_SICTR_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_STCNT_SICTR_MASK ) >> FTADDR_STCNT_SICTR_SHIFT )
#define FTADDR_STCNT_SICTR_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_STCNT_SICTR_MASK ) | \
    ( ( ( _val ) << FTADDR_STCNT_SICTR_SHIFT ) & FTADDR_STCNT_SICTR_MASK ) )
#define FTADDR_STCNT_SICTR( _val ) \
  ( ( ( _val ) << FTADDR_STCNT_SICTR_SHIFT ) & FTADDR_STCNT_SICTR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPHYIADDR \
 *   PHY indirect address register (PHYIADDR)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_SHIFT 20
#define FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_MASK  0xfff00000U
#define FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_MASK ) >> \
    FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_SHIFT )
#define FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_MASK ) |    \
    ( ( ( _val ) << FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_SHIFT ) & \
      FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_MASK ) )
#define FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR( _val )             \
  ( ( ( _val ) << FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_SHIFT ) & \
    FTADDR_PHYIADDR_PHY_READ_ERROR_INDICATOR_MASK )

#define FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_SHIFT 0
#define FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_MASK  0xffU
#define FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_MASK ) >> \
    FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_SHIFT )
#define FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_MASK ) |    \
    ( ( ( _val ) << FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_SHIFT ) & \
      FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_MASK ) )
#define FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS( _val )             \
  ( ( ( _val ) << FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_SHIFT ) & \
    FTADDR_PHYIADDR_PHY_REGISTER_ADDRESS_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPHYIDATA \
 *   PHY indirect data register (PHYIDATA)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PHYIDATA_PHY_REGISTER_DATA_SHIFT 0
#define FTADDR_PHYIDATA_PHY_REGISTER_DATA_MASK  0xffffffffU
#define FTADDR_PHYIDATA_PHY_REGISTER_DATA_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PHYIDATA_PHY_REGISTER_DATA_MASK ) >> \
    FTADDR_PHYIDATA_PHY_REGISTER_DATA_SHIFT )
#define FTADDR_PHYIDATA_PHY_REGISTER_DATA_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PHYIDATA_PHY_REGISTER_DATA_MASK ) |    \
    ( ( ( _val ) << FTADDR_PHYIDATA_PHY_REGISTER_DATA_SHIFT ) & \
      FTADDR_PHYIDATA_PHY_REGISTER_DATA_MASK ) )
#define FTADDR_PHYIDATA_PHY_REGISTER_DATA( _val )             \
  ( ( ( _val ) << FTADDR_PHYIDATA_PHY_REGISTER_DATA_SHIFT ) & \
    FTADDR_PHYIDATA_PHY_REGISTER_DATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPHYGCTRL \
 *   PHY generic control register (PHYGCTRL)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_SHIFT 0
#define FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_MASK  0xffffffffU
#define FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_MASK ) >> \
    FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_SHIFT )
#define FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_MASK ) |    \
    ( ( ( _val ) << FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_SHIFT ) & \
      FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_MASK ) )
#define FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL( _val )             \
  ( ( ( _val ) << FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_SHIFT ) & \
    FTADDR_PHYGCTRL_PHY_GENERIC_CONTROL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRTTCNT Training time counter register (TTCNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_TTCNT_FTRAINCTR_SHIFT 24
#define FTADDR_TTCNT_FTRAINCTR_MASK  0xff000000U
#define FTADDR_TTCNT_FTRAINCTR_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_TTCNT_FTRAINCTR_MASK ) >> \
    FTADDR_TTCNT_FTRAINCTR_SHIFT )
#define FTADDR_TTCNT_FTRAINCTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TTCNT_FTRAINCTR_MASK ) |    \
    ( ( ( _val ) << FTADDR_TTCNT_FTRAINCTR_SHIFT ) & \
      FTADDR_TTCNT_FTRAINCTR_MASK ) )
#define FTADDR_TTCNT_FTRAINCTR( _val )             \
  ( ( ( _val ) << FTADDR_TTCNT_FTRAINCTR_SHIFT ) & \
    FTADDR_TTCNT_FTRAINCTR_MASK )

#define FTADDR_TTCNT_ITRAINCTR_SHIFT 16
#define FTADDR_TTCNT_ITRAINCTR_MASK  0xff0000U
#define FTADDR_TTCNT_ITRAINCTR_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_TTCNT_ITRAINCTR_MASK ) >> \
    FTADDR_TTCNT_ITRAINCTR_SHIFT )
#define FTADDR_TTCNT_ITRAINCTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TTCNT_ITRAINCTR_MASK ) |    \
    ( ( ( _val ) << FTADDR_TTCNT_ITRAINCTR_SHIFT ) & \
      FTADDR_TTCNT_ITRAINCTR_MASK ) )
#define FTADDR_TTCNT_ITRAINCTR( _val )             \
  ( ( ( _val ) << FTADDR_TTCNT_ITRAINCTR_SHIFT ) & \
    FTADDR_TTCNT_ITRAINCTR_MASK )

#define FTADDR_TTCNT_TICK10 0x8000U

#define FTADDR_TTCNT_M10CTR_SHIFT 8
#define FTADDR_TTCNT_M10CTR_MASK  0x7f00U
#define FTADDR_TTCNT_M10CTR_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_TTCNT_M10CTR_MASK ) >> FTADDR_TTCNT_M10CTR_SHIFT )
#define FTADDR_TTCNT_M10CTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TTCNT_M10CTR_MASK ) |    \
    ( ( ( _val ) << FTADDR_TTCNT_M10CTR_SHIFT ) & \
      FTADDR_TTCNT_M10CTR_MASK ) )
#define FTADDR_TTCNT_M10CTR( _val ) \
  ( ( ( _val ) << FTADDR_TTCNT_M10CTR_SHIFT ) & FTADDR_TTCNT_M10CTR_MASK )

#define FTADDR_TTCNT_U100CTR_SHIFT 0
#define FTADDR_TTCNT_U100CTR_MASK  0xffU
#define FTADDR_TTCNT_U100CTR_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_TTCNT_U100CTR_MASK ) >> FTADDR_TTCNT_U100CTR_SHIFT )
#define FTADDR_TTCNT_U100CTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TTCNT_U100CTR_MASK ) |    \
    ( ( ( _val ) << FTADDR_TTCNT_U100CTR_SHIFT ) & \
      FTADDR_TTCNT_U100CTR_MASK ) )
#define FTADDR_TTCNT_U100CTR( _val ) \
  ( ( ( _val ) << FTADDR_TTCNT_U100CTR_SHIFT ) & FTADDR_TTCNT_U100CTR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRBEFECNT \
 *   Back-end FIFO error counter register (BEFECNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_BEFECNT_RTRCTR_SHIFT 24
#define FTADDR_BEFECNT_RTRCTR_MASK  0x3000000U
#define FTADDR_BEFECNT_RTRCTR_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEFECNT_RTRCTR_MASK ) >> FTADDR_BEFECNT_RTRCTR_SHIFT )
#define FTADDR_BEFECNT_RTRCTR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEFECNT_RTRCTR_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEFECNT_RTRCTR_SHIFT ) & \
      FTADDR_BEFECNT_RTRCTR_MASK ) )
#define FTADDR_BEFECNT_RTRCTR( _val ) \
  ( ( ( _val ) << FTADDR_BEFECNT_RTRCTR_SHIFT ) & FTADDR_BEFECNT_RTRCTR_MASK )

#define FTADDR_BEFECNT_CM3UE 0x800000U

#define FTADDR_BEFECNT_CM3CE_SHIFT 21
#define FTADDR_BEFECNT_CM3CE_MASK  0x600000U
#define FTADDR_BEFECNT_CM3CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEFECNT_CM3CE_MASK ) >> FTADDR_BEFECNT_CM3CE_SHIFT )
#define FTADDR_BEFECNT_CM3CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEFECNT_CM3CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEFECNT_CM3CE_SHIFT ) & \
      FTADDR_BEFECNT_CM3CE_MASK ) )
#define FTADDR_BEFECNT_CM3CE( _val ) \
  ( ( ( _val ) << FTADDR_BEFECNT_CM3CE_SHIFT ) & FTADDR_BEFECNT_CM3CE_MASK )

#define FTADDR_BEFECNT_WD3UE 0x100000U

#define FTADDR_BEFECNT_WD3CE_SHIFT 18
#define FTADDR_BEFECNT_WD3CE_MASK  0xc0000U
#define FTADDR_BEFECNT_WD3CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEFECNT_WD3CE_MASK ) >> FTADDR_BEFECNT_WD3CE_SHIFT )
#define FTADDR_BEFECNT_WD3CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEFECNT_WD3CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEFECNT_WD3CE_SHIFT ) & \
      FTADDR_BEFECNT_WD3CE_MASK ) )
#define FTADDR_BEFECNT_WD3CE( _val ) \
  ( ( ( _val ) << FTADDR_BEFECNT_WD3CE_SHIFT ) & FTADDR_BEFECNT_WD3CE_MASK )

#define FTADDR_BEFECNT_CM2UE 0x20000U

#define FTADDR_BEFECNT_CM2CE_SHIFT 15
#define FTADDR_BEFECNT_CM2CE_MASK  0x18000U
#define FTADDR_BEFECNT_CM2CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEFECNT_CM2CE_MASK ) >> FTADDR_BEFECNT_CM2CE_SHIFT )
#define FTADDR_BEFECNT_CM2CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEFECNT_CM2CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEFECNT_CM2CE_SHIFT ) & \
      FTADDR_BEFECNT_CM2CE_MASK ) )
#define FTADDR_BEFECNT_CM2CE( _val ) \
  ( ( ( _val ) << FTADDR_BEFECNT_CM2CE_SHIFT ) & FTADDR_BEFECNT_CM2CE_MASK )

#define FTADDR_BEFECNT_WD2UE 0x4000U

#define FTADDR_BEFECNT_WD2CE_SHIFT 12
#define FTADDR_BEFECNT_WD2CE_MASK  0x3000U
#define FTADDR_BEFECNT_WD2CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEFECNT_WD2CE_MASK ) >> FTADDR_BEFECNT_WD2CE_SHIFT )
#define FTADDR_BEFECNT_WD2CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEFECNT_WD2CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEFECNT_WD2CE_SHIFT ) & \
      FTADDR_BEFECNT_WD2CE_MASK ) )
#define FTADDR_BEFECNT_WD2CE( _val ) \
  ( ( ( _val ) << FTADDR_BEFECNT_WD2CE_SHIFT ) & FTADDR_BEFECNT_WD2CE_MASK )

#define FTADDR_BEFECNT_CM1UE 0x800U

#define FTADDR_BEFECNT_CM1CE_SHIFT 9
#define FTADDR_BEFECNT_CM1CE_MASK  0x600U
#define FTADDR_BEFECNT_CM1CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEFECNT_CM1CE_MASK ) >> FTADDR_BEFECNT_CM1CE_SHIFT )
#define FTADDR_BEFECNT_CM1CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEFECNT_CM1CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEFECNT_CM1CE_SHIFT ) & \
      FTADDR_BEFECNT_CM1CE_MASK ) )
#define FTADDR_BEFECNT_CM1CE( _val ) \
  ( ( ( _val ) << FTADDR_BEFECNT_CM1CE_SHIFT ) & FTADDR_BEFECNT_CM1CE_MASK )

#define FTADDR_BEFECNT_WD1UE 0x100U

#define FTADDR_BEFECNT_WD1CE_SHIFT 6
#define FTADDR_BEFECNT_WD1CE_MASK  0xc0U
#define FTADDR_BEFECNT_WD1CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEFECNT_WD1CE_MASK ) >> FTADDR_BEFECNT_WD1CE_SHIFT )
#define FTADDR_BEFECNT_WD1CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEFECNT_WD1CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEFECNT_WD1CE_SHIFT ) & \
      FTADDR_BEFECNT_WD1CE_MASK ) )
#define FTADDR_BEFECNT_WD1CE( _val ) \
  ( ( ( _val ) << FTADDR_BEFECNT_WD1CE_SHIFT ) & FTADDR_BEFECNT_WD1CE_MASK )

#define FTADDR_BEFECNT_CM0UE 0x20U

#define FTADDR_BEFECNT_CM0CE_SHIFT 3
#define FTADDR_BEFECNT_CM0CE_MASK  0x18U
#define FTADDR_BEFECNT_CM0CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEFECNT_CM0CE_MASK ) >> FTADDR_BEFECNT_CM0CE_SHIFT )
#define FTADDR_BEFECNT_CM0CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEFECNT_CM0CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEFECNT_CM0CE_SHIFT ) & \
      FTADDR_BEFECNT_CM0CE_MASK ) )
#define FTADDR_BEFECNT_CM0CE( _val ) \
  ( ( ( _val ) << FTADDR_BEFECNT_CM0CE_SHIFT ) & FTADDR_BEFECNT_CM0CE_MASK )

#define FTADDR_BEFECNT_WD0UE 0x4U

#define FTADDR_BEFECNT_WD0CE_SHIFT 0
#define FTADDR_BEFECNT_WD0CE_MASK  0x3U
#define FTADDR_BEFECNT_WD0CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_BEFECNT_WD0CE_MASK ) >> FTADDR_BEFECNT_WD0CE_SHIFT )
#define FTADDR_BEFECNT_WD0CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BEFECNT_WD0CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_BEFECNT_WD0CE_SHIFT ) & \
      FTADDR_BEFECNT_WD0CE_MASK ) )
#define FTADDR_BEFECNT_WD0CE( _val ) \
  ( ( ( _val ) << FTADDR_BEFECNT_WD0CE_SHIFT ) & FTADDR_BEFECNT_WD0CE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRTSCFG1 \
 *   Time slot configuration register 1 (TSCFG1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_TSCFG1_SRVSLT_MAX_SHIFT 20
#define FTADDR_TSCFG1_SRVSLT_MAX_MASK  0x3ff00000U
#define FTADDR_TSCFG1_SRVSLT_MAX_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_TSCFG1_SRVSLT_MAX_MASK ) >> \
    FTADDR_TSCFG1_SRVSLT_MAX_SHIFT )
#define FTADDR_TSCFG1_SRVSLT_MAX_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TSCFG1_SRVSLT_MAX_MASK ) |    \
    ( ( ( _val ) << FTADDR_TSCFG1_SRVSLT_MAX_SHIFT ) & \
      FTADDR_TSCFG1_SRVSLT_MAX_MASK ) )
#define FTADDR_TSCFG1_SRVSLT_MAX( _val )             \
  ( ( ( _val ) << FTADDR_TSCFG1_SRVSLT_MAX_SHIFT ) & \
    FTADDR_TSCFG1_SRVSLT_MAX_MASK )

#define FTADDR_TSCFG1_TSTRANDUR_SHIFT 8
#define FTADDR_TSCFG1_TSTRANDUR_MASK  0x3ff00U
#define FTADDR_TSCFG1_TSTRANDUR_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_TSCFG1_TSTRANDUR_MASK ) >> \
    FTADDR_TSCFG1_TSTRANDUR_SHIFT )
#define FTADDR_TSCFG1_TSTRANDUR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TSCFG1_TSTRANDUR_MASK ) |    \
    ( ( ( _val ) << FTADDR_TSCFG1_TSTRANDUR_SHIFT ) & \
      FTADDR_TSCFG1_TSTRANDUR_MASK ) )
#define FTADDR_TSCFG1_TSTRANDUR( _val )             \
  ( ( ( _val ) << FTADDR_TSCFG1_TSTRANDUR_SHIFT ) & \
    FTADDR_TSCFG1_TSTRANDUR_MASK )

#define FTADDR_TSCFG1_CURTS_SHIFT 4
#define FTADDR_TSCFG1_CURTS_MASK  0x70U
#define FTADDR_TSCFG1_CURTS_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_TSCFG1_CURTS_MASK ) >> FTADDR_TSCFG1_CURTS_SHIFT )
#define FTADDR_TSCFG1_CURTS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TSCFG1_CURTS_MASK ) |    \
    ( ( ( _val ) << FTADDR_TSCFG1_CURTS_SHIFT ) & \
      FTADDR_TSCFG1_CURTS_MASK ) )
#define FTADDR_TSCFG1_CURTS( _val ) \
  ( ( ( _val ) << FTADDR_TSCFG1_CURTS_SHIFT ) & FTADDR_TSCFG1_CURTS_MASK )

#define FTADDR_TSCFG1_TSSRV 0x8U

#define FTADDR_TSCFG1_TSTRANS 0x4U

#define FTADDR_TSCFG1_TSIDLE 0x2U

#define FTADDR_TSCFG1_TSEN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRTSCFG \
 *   Time slot configuration register (TSCFG)
 *
 * @brief This group contains register bit definitions.
 *
 * Register N holds the time slots 2N and 2N + 1. A duration of zero disables
 * the time slot.
 *
 * @{
 */

#define FTADDR_TSCFG_TS_2N_1_MSK_SHIFT 28
#define FTADDR_TSCFG_TS_2N_1_MSK_MASK  0xf0000000U
#define FTADDR_TSCFG_TS_2N_1_MSK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_TSCFG_TS_2N_1_MSK_MASK ) >> \
    FTADDR_TSCFG_TS_2N_1_MSK_SHIFT )
#define FTADDR_TSCFG_TS_2N_1_MSK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TSCFG_TS_2N_1_MSK_MASK ) |    \
    ( ( ( _val ) << FTADDR_TSCFG_TS_2N_1_MSK_SHIFT ) & \
      FTADDR_TSCFG_TS_2N_1_MSK_MASK ) )
#define FTADDR_TSCFG_TS_2N_1_MSK( _val )             \
  ( ( ( _val ) << FTADDR_TSCFG_TS_2N_1_MSK_SHIFT ) & \
    FTADDR_TSCFG_TS_2N_1_MSK_MASK )

#define FTADDR_TSCFG_TS_2N_1_DUR_SHIFT 16
#define FTADDR_TSCFG_TS_2N_1_DUR_MASK  0xfff0000U
#define FTADDR_TSCFG_TS_2N_1_DUR_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_TSCFG_TS_2N_1_DUR_MASK ) >> \
    FTADDR_TSCFG_TS_2N_1_DUR_SHIFT )
#define FTADDR_TSCFG_TS_2N_1_DUR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TSCFG_TS_2N_1_DUR_MASK ) |    \
    ( ( ( _val ) << FTADDR_TSCFG_TS_2N_1_DUR_SHIFT ) & \
      FTADDR_TSCFG_TS_2N_1_DUR_MASK ) )
#define FTADDR_TSCFG_TS_2N_1_DUR( _val )             \
  ( ( ( _val ) << FTADDR_TSCFG_TS_2N_1_DUR_SHIFT ) & \
    FTADDR_TSCFG_TS_2N_1_DUR_MASK )

#define FTADDR_TSCFG_TS_2N_0_MSK_SHIFT 12
#define FTADDR_TSCFG_TS_2N_0_MSK_MASK  0xf000U
#define FTADDR_TSCFG_TS_2N_0_MSK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_TSCFG_TS_2N_0_MSK_MASK ) >> \
    FTADDR_TSCFG_TS_2N_0_MSK_SHIFT )
#define FTADDR_TSCFG_TS_2N_0_MSK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TSCFG_TS_2N_0_MSK_MASK ) |    \
    ( ( ( _val ) << FTADDR_TSCFG_TS_2N_0_MSK_SHIFT ) & \
      FTADDR_TSCFG_TS_2N_0_MSK_MASK ) )
#define FTADDR_TSCFG_TS_2N_0_MSK( _val )             \
  ( ( ( _val ) << FTADDR_TSCFG_TS_2N_0_MSK_SHIFT ) & \
    FTADDR_TSCFG_TS_2N_0_MSK_MASK )

#define FTADDR_TSCFG_TS_2N_0_DUR_SHIFT 0
#define FTADDR_TSCFG_TS_2N_0_DUR_MASK  0xfffU
#define FTADDR_TSCFG_TS_2N_0_DUR_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_TSCFG_TS_2N_0_DUR_MASK ) >> \
    FTADDR_TSCFG_TS_2N_0_DUR_SHIFT )
#define FTADDR_TSCFG_TS_2N_0_DUR_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_TSCFG_TS_2N_0_DUR_MASK ) |    \
    ( ( ( _val ) << FTADDR_TSCFG_TS_2N_0_DUR_SHIFT ) & \
      FTADDR_TSCFG_TS_2N_0_DUR_MASK ) )
#define FTADDR_TSCFG_TS_2N_0_DUR( _val )             \
  ( ( ( _val ) << FTADDR_TSCFG_TS_2N_0_DUR_SHIFT ) & \
    FTADDR_TSCFG_TS_2N_0_DUR_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPARCFG \
 *   Parity configuration register (PARCFG)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PARCFG_INV 0x80000000U

#define FTADDR_PARCFG_CAPLM_SHIFT 28
#define FTADDR_PARCFG_CAPLM_MASK  0x70000000U
#define FTADDR_PARCFG_CAPLM_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_PARCFG_CAPLM_MASK ) >> FTADDR_PARCFG_CAPLM_SHIFT )
#define FTADDR_PARCFG_CAPLM_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PARCFG_CAPLM_MASK ) |    \
    ( ( ( _val ) << FTADDR_PARCFG_CAPLM_SHIFT ) & \
      FTADDR_PARCFG_CAPLM_MASK ) )
#define FTADDR_PARCFG_CAPLM( _val ) \
  ( ( ( _val ) << FTADDR_PARCFG_CAPLM_SHIFT ) & FTADDR_PARCFG_CAPLM_MASK )

#define FTADDR_PARCFG_ACTMSK 0x400000U

#define FTADDR_PARCFG_BAMSK_SHIFT 20
#define FTADDR_PARCFG_BAMSK_MASK  0x300000U
#define FTADDR_PARCFG_BAMSK_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_PARCFG_BAMSK_MASK ) >> FTADDR_PARCFG_BAMSK_SHIFT )
#define FTADDR_PARCFG_BAMSK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PARCFG_BAMSK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PARCFG_BAMSK_SHIFT ) & \
      FTADDR_PARCFG_BAMSK_MASK ) )
#define FTADDR_PARCFG_BAMSK( _val ) \
  ( ( ( _val ) << FTADDR_PARCFG_BAMSK_SHIFT ) & FTADDR_PARCFG_BAMSK_MASK )

#define FTADDR_PARCFG_BGMSK_SHIFT 18
#define FTADDR_PARCFG_BGMSK_MASK  0xc0000U
#define FTADDR_PARCFG_BGMSK_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_PARCFG_BGMSK_MASK ) >> FTADDR_PARCFG_BGMSK_SHIFT )
#define FTADDR_PARCFG_BGMSK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PARCFG_BGMSK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PARCFG_BGMSK_SHIFT ) & \
      FTADDR_PARCFG_BGMSK_MASK ) )
#define FTADDR_PARCFG_BGMSK( _val ) \
  ( ( ( _val ) << FTADDR_PARCFG_BGMSK_SHIFT ) & FTADDR_PARCFG_BGMSK_MASK )

#define FTADDR_PARCFG_ADDRMSK_SHIFT 0
#define FTADDR_PARCFG_ADDRMSK_MASK  0x1ffffU
#define FTADDR_PARCFG_ADDRMSK_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_PARCFG_ADDRMSK_MASK ) >> FTADDR_PARCFG_ADDRMSK_SHIFT )
#define FTADDR_PARCFG_ADDRMSK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PARCFG_ADDRMSK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PARCFG_ADDRMSK_SHIFT ) & \
      FTADDR_PARCFG_ADDRMSK_MASK ) )
#define FTADDR_PARCFG_ADDRMSK( _val ) \
  ( ( ( _val ) << FTADDR_PARCFG_ADDRMSK_SHIFT ) & FTADDR_PARCFG_ADDRMSK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRMRDEF \
 *   Mode register default value register (MRDEF)
 *
 * @brief This group contains register bit definitions.
 *
 * Register N holds the default value of the mode register N of the memory
 * devices.
 *
 * @{
 */

#define FTADDR_MRDEF_OVR 0x80000000U

#define FTADDR_MRDEF_MRDEF_SHIFT 0
#define FTADDR_MRDEF_MRDEF_MASK  0x1ffffU
#define FTADDR_MRDEF_MRDEF_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_MRDEF_MRDEF_MASK ) >> FTADDR_MRDEF_MRDEF_SHIFT )
#define FTADDR_MRDEF_MRDEF_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_MRDEF_MRDEF_MASK ) | \
    ( ( ( _val ) << FTADDR_MRDEF_MRDEF_SHIFT ) & FTADDR_MRDEF_MRDEF_MASK ) )
#define FTADDR_MRDEF_MRDEF( _val ) \
  ( ( ( _val ) << FTADDR_MRDEF_MRDEF_SHIFT ) & FTADDR_MRDEF_MRDEF_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRARBNUDGE \
 *   Arbiter nudge control register (ARBNUDGE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_ARBNUDGE_NEN 0x80000000U

#define FTADDR_ARBNUDGE_NDLY_SHIFT 0
#define FTADDR_ARBNUDGE_NDLY_MASK  0xffU
#define FTADDR_ARBNUDGE_NDLY_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_ARBNUDGE_NDLY_MASK ) >> FTADDR_ARBNUDGE_NDLY_SHIFT )
#define FTADDR_ARBNUDGE_NDLY_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_ARBNUDGE_NDLY_MASK ) |    \
    ( ( ( _val ) << FTADDR_ARBNUDGE_NDLY_SHIFT ) & \
      FTADDR_ARBNUDGE_NDLY_MASK ) )
#define FTADDR_ARBNUDGE_NDLY( _val ) \
  ( ( ( _val ) << FTADDR_ARBNUDGE_NDLY_SHIFT ) & FTADDR_ARBNUDGE_NDLY_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPHYTIM3 PHY timing register 3 (PHYTIM3)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PHYTIM3_WRDATA_MASK_SHIFT 0
#define FTADDR_PHYTIM3_WRDATA_MASK_MASK  0x1fffffU
#define FTADDR_PHYTIM3_WRDATA_MASK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PHYTIM3_WRDATA_MASK_MASK ) >> \
    FTADDR_PHYTIM3_WRDATA_MASK_SHIFT )
#define FTADDR_PHYTIM3_WRDATA_MASK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PHYTIM3_WRDATA_MASK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PHYTIM3_WRDATA_MASK_SHIFT ) & \
      FTADDR_PHYTIM3_WRDATA_MASK_MASK ) )
#define FTADDR_PHYTIM3_WRDATA_MASK( _val )             \
  ( ( ( _val ) << FTADDR_PHYTIM3_WRDATA_MASK_SHIFT ) & \
    FTADDR_PHYTIM3_WRDATA_MASK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRODTERTIM \
 *   ODT external read timing register (ODTERTIM)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_ODTERTIM_ODTRDMASK_SHIFT 1
#define FTADDR_ODTERTIM_ODTRDMASK_MASK  0xffffffeU
#define FTADDR_ODTERTIM_ODTRDMASK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_ODTERTIM_ODTRDMASK_MASK ) >> \
    FTADDR_ODTERTIM_ODTRDMASK_SHIFT )
#define FTADDR_ODTERTIM_ODTRDMASK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_ODTERTIM_ODTRDMASK_MASK ) |    \
    ( ( ( _val ) << FTADDR_ODTERTIM_ODTRDMASK_SHIFT ) & \
      FTADDR_ODTERTIM_ODTRDMASK_MASK ) )
#define FTADDR_ODTERTIM_ODTRDMASK( _val )             \
  ( ( ( _val ) << FTADDR_ODTERTIM_ODTRDMASK_SHIFT ) & \
    FTADDR_ODTERTIM_ODTRDMASK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRODTEWTIM \
 *   ODT external write timing register (ODTEWTIM)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_ODTEWTIM_ODTWRMASK_SHIFT 1
#define FTADDR_ODTEWTIM_ODTWRMASK_MASK  0xffffeU
#define FTADDR_ODTEWTIM_ODTWRMASK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_ODTEWTIM_ODTWRMASK_MASK ) >> \
    FTADDR_ODTEWTIM_ODTWRMASK_SHIFT )
#define FTADDR_ODTEWTIM_ODTWRMASK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_ODTEWTIM_ODTWRMASK_MASK ) |    \
    ( ( ( _val ) << FTADDR_ODTEWTIM_ODTWRMASK_SHIFT ) & \
      FTADDR_ODTEWTIM_ODTWRMASK_MASK ) )
#define FTADDR_ODTEWTIM_ODTWRMASK( _val )             \
  ( ( ( _val ) << FTADDR_ODTEWTIM_ODTWRMASK_SHIFT ) & \
    FTADDR_ODTEWTIM_ODTWRMASK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRAHBADEC \
 *   AHB address decode register (AHBADEC)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_AHBADEC_OORER 0x80000000U

#define FTADDR_AHBADEC_MEMMAP_SHIFT 21
#define FTADDR_AHBADEC_MEMMAP_MASK  0x600000U
#define FTADDR_AHBADEC_MEMMAP_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_AHBADEC_MEMMAP_MASK ) >> FTADDR_AHBADEC_MEMMAP_SHIFT )
#define FTADDR_AHBADEC_MEMMAP_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBADEC_MEMMAP_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBADEC_MEMMAP_SHIFT ) & \
      FTADDR_AHBADEC_MEMMAP_MASK ) )
#define FTADDR_AHBADEC_MEMMAP( _val ) \
  ( ( ( _val ) << FTADDR_AHBADEC_MEMMAP_SHIFT ) & FTADDR_AHBADEC_MEMMAP_MASK )

#define FTADDR_AHBADEC_HADDRWM32_SHIFT 16
#define FTADDR_AHBADEC_HADDRWM32_MASK  0x1f0000U
#define FTADDR_AHBADEC_HADDRWM32_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBADEC_HADDRWM32_MASK ) >> \
    FTADDR_AHBADEC_HADDRWM32_SHIFT )
#define FTADDR_AHBADEC_HADDRWM32_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBADEC_HADDRWM32_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBADEC_HADDRWM32_SHIFT ) & \
      FTADDR_AHBADEC_HADDRWM32_MASK ) )
#define FTADDR_AHBADEC_HADDRWM32( _val )             \
  ( ( ( _val ) << FTADDR_AHBADEC_HADDRWM32_SHIFT ) & \
    FTADDR_AHBADEC_HADDRWM32_MASK )

#define FTADDR_AHBADEC_NCS_SHIFT 13
#define FTADDR_AHBADEC_NCS_MASK  0xe000U
#define FTADDR_AHBADEC_NCS_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_AHBADEC_NCS_MASK ) >> FTADDR_AHBADEC_NCS_SHIFT )
#define FTADDR_AHBADEC_NCS_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_AHBADEC_NCS_MASK ) | \
    ( ( ( _val ) << FTADDR_AHBADEC_NCS_SHIFT ) & FTADDR_AHBADEC_NCS_MASK ) )
#define FTADDR_AHBADEC_NCS( _val ) \
  ( ( ( _val ) << FTADDR_AHBADEC_NCS_SHIFT ) & FTADDR_AHBADEC_NCS_MASK )

#define FTADDR_AHBADEC_CSBASE_SHIFT 9
#define FTADDR_AHBADEC_CSBASE_MASK  0x1e00U
#define FTADDR_AHBADEC_CSBASE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_AHBADEC_CSBASE_MASK ) >> FTADDR_AHBADEC_CSBASE_SHIFT )
#define FTADDR_AHBADEC_CSBASE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBADEC_CSBASE_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBADEC_CSBASE_SHIFT ) & \
      FTADDR_AHBADEC_CSBASE_MASK ) )
#define FTADDR_AHBADEC_CSBASE( _val ) \
  ( ( ( _val ) << FTADDR_AHBADEC_CSBASE_SHIFT ) & FTADDR_AHBADEC_CSBASE_MASK )

#define FTADDR_AHBADEC_BANKBASE_SHIFT 5
#define FTADDR_AHBADEC_BANKBASE_MASK  0x1e0U
#define FTADDR_AHBADEC_BANKBASE_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBADEC_BANKBASE_MASK ) >> \
    FTADDR_AHBADEC_BANKBASE_SHIFT )
#define FTADDR_AHBADEC_BANKBASE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBADEC_BANKBASE_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBADEC_BANKBASE_SHIFT ) & \
      FTADDR_AHBADEC_BANKBASE_MASK ) )
#define FTADDR_AHBADEC_BANKBASE( _val )             \
  ( ( ( _val ) << FTADDR_AHBADEC_BANKBASE_SHIFT ) & \
    FTADDR_AHBADEC_BANKBASE_MASK )

#define FTADDR_AHBADEC_ROWBASE_SHIFT 2
#define FTADDR_AHBADEC_ROWBASE_MASK  0x1cU
#define FTADDR_AHBADEC_ROWBASE_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBADEC_ROWBASE_MASK ) >> \
    FTADDR_AHBADEC_ROWBASE_SHIFT )
#define FTADDR_AHBADEC_ROWBASE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBADEC_ROWBASE_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBADEC_ROWBASE_SHIFT ) & \
      FTADDR_AHBADEC_ROWBASE_MASK ) )
#define FTADDR_AHBADEC_ROWBASE( _val )             \
  ( ( ( _val ) << FTADDR_AHBADEC_ROWBASE_SHIFT ) & \
    FTADDR_AHBADEC_ROWBASE_MASK )

#define FTADDR_AHBADEC_COLBASE_SHIFT 0
#define FTADDR_AHBADEC_COLBASE_MASK  0x3U
#define FTADDR_AHBADEC_COLBASE_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBADEC_COLBASE_MASK ) >> \
    FTADDR_AHBADEC_COLBASE_SHIFT )
#define FTADDR_AHBADEC_COLBASE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBADEC_COLBASE_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBADEC_COLBASE_SHIFT ) & \
      FTADDR_AHBADEC_COLBASE_MASK ) )
#define FTADDR_AHBADEC_COLBASE( _val )             \
  ( ( ( _val ) << FTADDR_AHBADEC_COLBASE_SHIFT ) & \
    FTADDR_AHBADEC_COLBASE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRAHBACFG \
 *   AHB access configuration register (AHBACFG)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_AHBACFG_PRELIMHI_SHIFT 28
#define FTADDR_AHBACFG_PRELIMHI_MASK  0xf0000000U
#define FTADDR_AHBACFG_PRELIMHI_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBACFG_PRELIMHI_MASK ) >> \
    FTADDR_AHBACFG_PRELIMHI_SHIFT )
#define FTADDR_AHBACFG_PRELIMHI_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBACFG_PRELIMHI_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBACFG_PRELIMHI_SHIFT ) & \
      FTADDR_AHBACFG_PRELIMHI_MASK ) )
#define FTADDR_AHBACFG_PRELIMHI( _val )             \
  ( ( ( _val ) << FTADDR_AHBACFG_PRELIMHI_SHIFT ) & \
    FTADDR_AHBACFG_PRELIMHI_MASK )

#define FTADDR_AHBACFG_PREFLIMLO_SHIFT 24
#define FTADDR_AHBACFG_PREFLIMLO_MASK  0xf000000U
#define FTADDR_AHBACFG_PREFLIMLO_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBACFG_PREFLIMLO_MASK ) >> \
    FTADDR_AHBACFG_PREFLIMLO_SHIFT )
#define FTADDR_AHBACFG_PREFLIMLO_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBACFG_PREFLIMLO_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBACFG_PREFLIMLO_SHIFT ) & \
      FTADDR_AHBACFG_PREFLIMLO_MASK ) )
#define FTADDR_AHBACFG_PREFLIMLO( _val )             \
  ( ( ( _val ) << FTADDR_AHBACFG_PREFLIMLO_SHIFT ) & \
    FTADDR_AHBACFG_PREFLIMLO_MASK )

#define FTADDR_AHBACFG_WQUEUEMAX_SHIFT 16
#define FTADDR_AHBACFG_WQUEUEMAX_MASK  0x70000U
#define FTADDR_AHBACFG_WQUEUEMAX_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBACFG_WQUEUEMAX_MASK ) >> \
    FTADDR_AHBACFG_WQUEUEMAX_SHIFT )
#define FTADDR_AHBACFG_WQUEUEMAX_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBACFG_WQUEUEMAX_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBACFG_WQUEUEMAX_SHIFT ) & \
      FTADDR_AHBACFG_WQUEUEMAX_MASK ) )
#define FTADDR_AHBACFG_WQUEUEMAX( _val )             \
  ( ( ( _val ) << FTADDR_AHBACFG_WQUEUEMAX_SHIFT ) & \
    FTADDR_AHBACFG_WQUEUEMAX_MASK )

#define FTADDR_AHBACFG_ROWIDLECNT_SHIFT 13
#define FTADDR_AHBACFG_ROWIDLECNT_MASK  0xe000U
#define FTADDR_AHBACFG_ROWIDLECNT_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBACFG_ROWIDLECNT_MASK ) >> \
    FTADDR_AHBACFG_ROWIDLECNT_SHIFT )
#define FTADDR_AHBACFG_ROWIDLECNT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBACFG_ROWIDLECNT_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBACFG_ROWIDLECNT_SHIFT ) & \
      FTADDR_AHBACFG_ROWIDLECNT_MASK ) )
#define FTADDR_AHBACFG_ROWIDLECNT( _val )             \
  ( ( ( _val ) << FTADDR_AHBACFG_ROWIDLECNT_SHIFT ) & \
    FTADDR_AHBACFG_ROWIDLECNT_MASK )

#define FTADDR_AHBACFG_ROWOTM_SHIFT 5
#define FTADDR_AHBACFG_ROWOTM_MASK  0x1fe0U
#define FTADDR_AHBACFG_ROWOTM_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_AHBACFG_ROWOTM_MASK ) >> FTADDR_AHBACFG_ROWOTM_SHIFT )
#define FTADDR_AHBACFG_ROWOTM_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBACFG_ROWOTM_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBACFG_ROWOTM_SHIFT ) & \
      FTADDR_AHBACFG_ROWOTM_MASK ) )
#define FTADDR_AHBACFG_ROWOTM( _val ) \
  ( ( ( _val ) << FTADDR_AHBACFG_ROWOTM_SHIFT ) & FTADDR_AHBACFG_ROWOTM_MASK )

#define FTADDR_AHBACFG_ROWOPOL_SHIFT 3
#define FTADDR_AHBACFG_ROWOPOL_MASK  0x18U
#define FTADDR_AHBACFG_ROWOPOL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBACFG_ROWOPOL_MASK ) >> \
    FTADDR_AHBACFG_ROWOPOL_SHIFT )
#define FTADDR_AHBACFG_ROWOPOL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBACFG_ROWOPOL_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBACFG_ROWOPOL_SHIFT ) & \
      FTADDR_AHBACFG_ROWOPOL_MASK ) )
#define FTADDR_AHBACFG_ROWOPOL( _val )             \
  ( ( ( _val ) << FTADDR_AHBACFG_ROWOPOL_SHIFT ) & \
    FTADDR_AHBACFG_ROWOPOL_MASK )

#define FTADDR_AHBACFG_UEERR 0x4U

#define FTADDR_AHBACFG_UESIG 0x2U

#define FTADDR_AHBACFG_CESIG 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPFCFG \
 *   Prefetch configuration register (PFCFG)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PFCFG_PREFTO_SHIFT 16
#define FTADDR_PFCFG_PREFTO_MASK  0x3ff0000U
#define FTADDR_PFCFG_PREFTO_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_PFCFG_PREFTO_MASK ) >> FTADDR_PFCFG_PREFTO_SHIFT )
#define FTADDR_PFCFG_PREFTO_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFCFG_PREFTO_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFCFG_PREFTO_SHIFT ) & \
      FTADDR_PFCFG_PREFTO_MASK ) )
#define FTADDR_PFCFG_PREFTO( _val ) \
  ( ( ( _val ) << FTADDR_PFCFG_PREFTO_SHIFT ) & FTADDR_PFCFG_PREFTO_MASK )

#define FTADDR_PFCFG_PREFETCH_MASTER_MASK_SHIFT 0
#define FTADDR_PFCFG_PREFETCH_MASTER_MASK_MASK  0xffffU
#define FTADDR_PFCFG_PREFETCH_MASTER_MASK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFCFG_PREFETCH_MASTER_MASK_MASK ) >> \
    FTADDR_PFCFG_PREFETCH_MASTER_MASK_SHIFT )
#define FTADDR_PFCFG_PREFETCH_MASTER_MASK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFCFG_PREFETCH_MASTER_MASK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFCFG_PREFETCH_MASTER_MASK_SHIFT ) & \
      FTADDR_PFCFG_PREFETCH_MASTER_MASK_MASK ) )
#define FTADDR_PFCFG_PREFETCH_MASTER_MASK( _val )             \
  ( ( ( _val ) << FTADDR_PFCFG_PREFETCH_MASTER_MASK_SHIFT ) & \
    FTADDR_PFCFG_PREFETCH_MASTER_MASK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCRUBCFG1 \
 *   Scrubber configuration register 1 (SCRUBCFG1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCRUBCFG1_INITEN_SHIFT 28
#define FTADDR_SCRUBCFG1_INITEN_MASK  0xf0000000U
#define FTADDR_SCRUBCFG1_INITEN_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBCFG1_INITEN_MASK ) >> \
    FTADDR_SCRUBCFG1_INITEN_SHIFT )
#define FTADDR_SCRUBCFG1_INITEN_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBCFG1_INITEN_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBCFG1_INITEN_SHIFT ) & \
      FTADDR_SCRUBCFG1_INITEN_MASK ) )
#define FTADDR_SCRUBCFG1_INITEN( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBCFG1_INITEN_SHIFT ) & \
    FTADDR_SCRUBCFG1_INITEN_MASK )

#define FTADDR_SCRUBCFG1_READBACK_SHIFT 24
#define FTADDR_SCRUBCFG1_READBACK_MASK  0xf000000U
#define FTADDR_SCRUBCFG1_READBACK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBCFG1_READBACK_MASK ) >> \
    FTADDR_SCRUBCFG1_READBACK_SHIFT )
#define FTADDR_SCRUBCFG1_READBACK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBCFG1_READBACK_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBCFG1_READBACK_SHIFT ) & \
      FTADDR_SCRUBCFG1_READBACK_MASK ) )
#define FTADDR_SCRUBCFG1_READBACK( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBCFG1_READBACK_SHIFT ) & \
    FTADDR_SCRUBCFG1_READBACK_MASK )

#define FTADDR_SCRUBCFG1_SCRUBPEND_SHIFT 20
#define FTADDR_SCRUBCFG1_SCRUBPEND_MASK  0xf00000U
#define FTADDR_SCRUBCFG1_SCRUBPEND_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBCFG1_SCRUBPEND_MASK ) >> \
    FTADDR_SCRUBCFG1_SCRUBPEND_SHIFT )
#define FTADDR_SCRUBCFG1_SCRUBPEND_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBCFG1_SCRUBPEND_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBCFG1_SCRUBPEND_SHIFT ) & \
      FTADDR_SCRUBCFG1_SCRUBPEND_MASK ) )
#define FTADDR_SCRUBCFG1_SCRUBPEND( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBCFG1_SCRUBPEND_SHIFT ) & \
    FTADDR_SCRUBCFG1_SCRUBPEND_MASK )

#define FTADDR_SCRUBCFG1_SBDEN 0x2000U

#define FTADDR_SCRUBCFG1_RGALL 0x1000U

#define FTADDR_SCRUBCFG1_IEXD 0x800U

#define FTADDR_SCRUBCFG1_IADR 0x400U

#define FTADDR_SCRUBCFG1_SFCAL 0x200U

#define FTADDR_SCRUBCFG1_BESFI 0x100U

#define FTADDR_SCRUBCFG1_SFIDBL 0x80U

#define FTADDR_SCRUBCFG1_SEFISTATE_SHIFT 5
#define FTADDR_SCRUBCFG1_SEFISTATE_MASK  0x60U
#define FTADDR_SCRUBCFG1_SEFISTATE_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBCFG1_SEFISTATE_MASK ) >> \
    FTADDR_SCRUBCFG1_SEFISTATE_SHIFT )
#define FTADDR_SCRUBCFG1_SEFISTATE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBCFG1_SEFISTATE_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBCFG1_SEFISTATE_SHIFT ) & \
      FTADDR_SCRUBCFG1_SEFISTATE_MASK ) )
#define FTADDR_SCRUBCFG1_SEFISTATE( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBCFG1_SEFISTATE_SHIFT ) & \
    FTADDR_SCRUBCFG1_SEFISTATE_MASK )

#define FTADDR_SCRUBCFG1_SHOLD 0x10U

#define FTADDR_SCRUBCFG1_SEFIEN 0x8U

#define FTADDR_SCRUBCFG1_SCRUBCNT_SHIFT 1
#define FTADDR_SCRUBCFG1_SCRUBCNT_MASK  0x6U
#define FTADDR_SCRUBCFG1_SCRUBCNT_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBCFG1_SCRUBCNT_MASK ) >> \
    FTADDR_SCRUBCFG1_SCRUBCNT_SHIFT )
#define FTADDR_SCRUBCFG1_SCRUBCNT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBCFG1_SCRUBCNT_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBCFG1_SCRUBCNT_SHIFT ) & \
      FTADDR_SCRUBCFG1_SCRUBCNT_MASK ) )
#define FTADDR_SCRUBCFG1_SCRUBCNT( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBCFG1_SCRUBCNT_SHIFT ) & \
    FTADDR_SCRUBCFG1_SCRUBCNT_MASK )

#define FTADDR_SCRUBCFG1_SCREN 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCRUBCFG2 \
 *   Scrubber configuration register 2 (SCRUBCFG2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCRUBCFG2_SEFI_BL_MSK_SHIFT 16
#define FTADDR_SCRUBCFG2_SEFI_BL_MSK_MASK  0xfff0000U
#define FTADDR_SCRUBCFG2_SEFI_BL_MSK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBCFG2_SEFI_BL_MSK_MASK ) >> \
    FTADDR_SCRUBCFG2_SEFI_BL_MSK_SHIFT )
#define FTADDR_SCRUBCFG2_SEFI_BL_MSK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBCFG2_SEFI_BL_MSK_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBCFG2_SEFI_BL_MSK_SHIFT ) & \
      FTADDR_SCRUBCFG2_SEFI_BL_MSK_MASK ) )
#define FTADDR_SCRUBCFG2_SEFI_BL_MSK( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBCFG2_SEFI_BL_MSK_SHIFT ) & \
    FTADDR_SCRUBCFG2_SEFI_BL_MSK_MASK )

#define FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_SHIFT 0
#define FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_MASK  0xffffU
#define FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_MASK ) >> \
    FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_SHIFT )
#define FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_SHIFT ) & \
      FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_MASK ) )
#define FTADDR_SCRUBCFG2_SCRUBB_INTERVAL( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_SHIFT ) & \
    FTADDR_SCRUBCFG2_SCRUBB_INTERVAL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRIRQPEND IRQ pending register (IRQPEND)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_IRQPEND_STKB 0x1000U

#define FTADDR_IRQPEND_PSFI 0x800U

#define FTADDR_IRQPEND_BEREI 0x400U

#define FTADDR_IRQPEND_BESCI 0x200U

#define FTADDR_IRQPEND_SFI 0x100U

#define FTADDR_IRQPEND_UEI_SHIFT 4
#define FTADDR_IRQPEND_UEI_MASK  0xf0U
#define FTADDR_IRQPEND_UEI_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_IRQPEND_UEI_MASK ) >> FTADDR_IRQPEND_UEI_SHIFT )
#define FTADDR_IRQPEND_UEI_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_IRQPEND_UEI_MASK ) | \
    ( ( ( _val ) << FTADDR_IRQPEND_UEI_SHIFT ) & FTADDR_IRQPEND_UEI_MASK ) )
#define FTADDR_IRQPEND_UEI( _val ) \
  ( ( ( _val ) << FTADDR_IRQPEND_UEI_SHIFT ) & FTADDR_IRQPEND_UEI_MASK )

#define FTADDR_IRQPEND_CEI_SHIFT 0
#define FTADDR_IRQPEND_CEI_MASK  0xfU
#define FTADDR_IRQPEND_CEI_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_IRQPEND_CEI_MASK ) >> FTADDR_IRQPEND_CEI_SHIFT )
#define FTADDR_IRQPEND_CEI_SET( _reg, _val )  \
  ( ( ( _reg ) & ~FTADDR_IRQPEND_CEI_MASK ) | \
    ( ( ( _val ) << FTADDR_IRQPEND_CEI_SHIFT ) & FTADDR_IRQPEND_CEI_MASK ) )
#define FTADDR_IRQPEND_CEI( _val ) \
  ( ( ( _val ) << FTADDR_IRQPEND_CEI_SHIFT ) & FTADDR_IRQPEND_CEI_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRIRQEN IRQ enable register (IRQEN)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_IRQEN_STKBE 0x1000U

#define FTADDR_IRQEN_PSFIE 0x800U

#define FTADDR_IRQEN_BEREIE 0x400U

#define FTADDR_IRQEN_BESCIE 0x200U

#define FTADDR_IRQEN_SFIE 0x100U

#define FTADDR_IRQEN_UEIE 0x10U

#define FTADDR_IRQEN_CEIE 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCRUBUE \
 *   Scrubber uncorrectable error register (SCRUBUE)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCRUBUE_SCRUE 0x80000000U

#define FTADDR_SCRUBUE_RBERR 0x40000000U

#define FTADDR_SCRUBUE_SECSNO_SHIFT 27
#define FTADDR_SCRUBUE_SECSNO_MASK  0x38000000U
#define FTADDR_SCRUBUE_SECSNO_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_SCRUBUE_SECSNO_MASK ) >> FTADDR_SCRUBUE_SECSNO_SHIFT )
#define FTADDR_SCRUBUE_SECSNO_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBUE_SECSNO_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBUE_SECSNO_SHIFT ) & \
      FTADDR_SCRUBUE_SECSNO_MASK ) )
#define FTADDR_SCRUBUE_SECSNO( _val ) \
  ( ( ( _val ) << FTADDR_SCRUBUE_SECSNO_SHIFT ) & FTADDR_SCRUBUE_SECSNO_MASK )

#define FTADDR_SCRUBUE_SEBANK_SHIFT 24
#define FTADDR_SCRUBUE_SEBANK_MASK  0x7000000U
#define FTADDR_SCRUBUE_SEBANK_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_SCRUBUE_SEBANK_MASK ) >> FTADDR_SCRUBUE_SEBANK_SHIFT )
#define FTADDR_SCRUBUE_SEBANK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBUE_SEBANK_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBUE_SEBANK_SHIFT ) & \
      FTADDR_SCRUBUE_SEBANK_MASK ) )
#define FTADDR_SCRUBUE_SEBANK( _val ) \
  ( ( ( _val ) << FTADDR_SCRUBUE_SEBANK_SHIFT ) & FTADDR_SCRUBUE_SEBANK_MASK )

#define FTADDR_SCRUBUE_SEROW_SHIFT 8
#define FTADDR_SCRUBUE_SEROW_MASK  0xffff00U
#define FTADDR_SCRUBUE_SEROW_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_SCRUBUE_SEROW_MASK ) >> FTADDR_SCRUBUE_SEROW_SHIFT )
#define FTADDR_SCRUBUE_SEROW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBUE_SEROW_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBUE_SEROW_SHIFT ) & \
      FTADDR_SCRUBUE_SEROW_MASK ) )
#define FTADDR_SCRUBUE_SEROW( _val ) \
  ( ( ( _val ) << FTADDR_SCRUBUE_SEROW_SHIFT ) & FTADDR_SCRUBUE_SEROW_MASK )

#define FTADDR_SCRUBUE_SECOLHI_SHIFT 0
#define FTADDR_SCRUBUE_SECOLHI_MASK  0xffU
#define FTADDR_SCRUBUE_SECOLHI_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBUE_SECOLHI_MASK ) >> \
    FTADDR_SCRUBUE_SECOLHI_SHIFT )
#define FTADDR_SCRUBUE_SECOLHI_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBUE_SECOLHI_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBUE_SECOLHI_SHIFT ) & \
      FTADDR_SCRUBUE_SECOLHI_MASK ) )
#define FTADDR_SCRUBUE_SECOLHI( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBUE_SECOLHI_SHIFT ) & \
    FTADDR_SCRUBUE_SECOLHI_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRBLECNT \
 *   Byte lane error counter register (BLECNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_BLECNT_LANE_4N_3_ERRCNT_SHIFT 24
#define FTADDR_BLECNT_LANE_4N_3_ERRCNT_MASK  0x3f000000U
#define FTADDR_BLECNT_LANE_4N_3_ERRCNT_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_BLECNT_LANE_4N_3_ERRCNT_MASK ) >> \
    FTADDR_BLECNT_LANE_4N_3_ERRCNT_SHIFT )
#define FTADDR_BLECNT_LANE_4N_3_ERRCNT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BLECNT_LANE_4N_3_ERRCNT_MASK ) |    \
    ( ( ( _val ) << FTADDR_BLECNT_LANE_4N_3_ERRCNT_SHIFT ) & \
      FTADDR_BLECNT_LANE_4N_3_ERRCNT_MASK ) )
#define FTADDR_BLECNT_LANE_4N_3_ERRCNT( _val )             \
  ( ( ( _val ) << FTADDR_BLECNT_LANE_4N_3_ERRCNT_SHIFT ) & \
    FTADDR_BLECNT_LANE_4N_3_ERRCNT_MASK )

#define FTADDR_BLECNT_LANE_4N_2_ERRCNT_SHIFT 16
#define FTADDR_BLECNT_LANE_4N_2_ERRCNT_MASK  0x3f0000U
#define FTADDR_BLECNT_LANE_4N_2_ERRCNT_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_BLECNT_LANE_4N_2_ERRCNT_MASK ) >> \
    FTADDR_BLECNT_LANE_4N_2_ERRCNT_SHIFT )
#define FTADDR_BLECNT_LANE_4N_2_ERRCNT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BLECNT_LANE_4N_2_ERRCNT_MASK ) |    \
    ( ( ( _val ) << FTADDR_BLECNT_LANE_4N_2_ERRCNT_SHIFT ) & \
      FTADDR_BLECNT_LANE_4N_2_ERRCNT_MASK ) )
#define FTADDR_BLECNT_LANE_4N_2_ERRCNT( _val )             \
  ( ( ( _val ) << FTADDR_BLECNT_LANE_4N_2_ERRCNT_SHIFT ) & \
    FTADDR_BLECNT_LANE_4N_2_ERRCNT_MASK )

#define FTADDR_BLECNT_LANE_4N_1_ERRCNT_SHIFT 8
#define FTADDR_BLECNT_LANE_4N_1_ERRCNT_MASK  0x3f00U
#define FTADDR_BLECNT_LANE_4N_1_ERRCNT_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_BLECNT_LANE_4N_1_ERRCNT_MASK ) >> \
    FTADDR_BLECNT_LANE_4N_1_ERRCNT_SHIFT )
#define FTADDR_BLECNT_LANE_4N_1_ERRCNT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BLECNT_LANE_4N_1_ERRCNT_MASK ) |    \
    ( ( ( _val ) << FTADDR_BLECNT_LANE_4N_1_ERRCNT_SHIFT ) & \
      FTADDR_BLECNT_LANE_4N_1_ERRCNT_MASK ) )
#define FTADDR_BLECNT_LANE_4N_1_ERRCNT( _val )             \
  ( ( ( _val ) << FTADDR_BLECNT_LANE_4N_1_ERRCNT_SHIFT ) & \
    FTADDR_BLECNT_LANE_4N_1_ERRCNT_MASK )

#define FTADDR_BLECNT_LANE_4N_0_ERRCNT_SHIFT 0
#define FTADDR_BLECNT_LANE_4N_0_ERRCNT_MASK  0x3fU
#define FTADDR_BLECNT_LANE_4N_0_ERRCNT_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_BLECNT_LANE_4N_0_ERRCNT_MASK ) >> \
    FTADDR_BLECNT_LANE_4N_0_ERRCNT_SHIFT )
#define FTADDR_BLECNT_LANE_4N_0_ERRCNT_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BLECNT_LANE_4N_0_ERRCNT_MASK ) |    \
    ( ( ( _val ) << FTADDR_BLECNT_LANE_4N_0_ERRCNT_SHIFT ) & \
      FTADDR_BLECNT_LANE_4N_0_ERRCNT_MASK ) )
#define FTADDR_BLECNT_LANE_4N_0_ERRCNT( _val )             \
  ( ( ( _val ) << FTADDR_BLECNT_LANE_4N_0_ERRCNT_SHIFT ) & \
    FTADDR_BLECNT_LANE_4N_0_ERRCNT_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRBLECNT4 \
 *   Byte lane error counter register 4 (BLECNT4)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_BLECNT4_BLCLR 0x80000000U

#define FTADDR_BLECNT4_BL_WMRK_SHIFT 0
#define FTADDR_BLECNT4_BL_WMRK_MASK  0x3fU
#define FTADDR_BLECNT4_BL_WMRK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_BLECNT4_BL_WMRK_MASK ) >> \
    FTADDR_BLECNT4_BL_WMRK_SHIFT )
#define FTADDR_BLECNT4_BL_WMRK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_BLECNT4_BL_WMRK_MASK ) |    \
    ( ( ( _val ) << FTADDR_BLECNT4_BL_WMRK_SHIFT ) & \
      FTADDR_BLECNT4_BL_WMRK_MASK ) )
#define FTADDR_BLECNT4_BL_WMRK( _val )             \
  ( ( ( _val ) << FTADDR_BLECNT4_BL_WMRK_SHIFT ) & \
    FTADDR_BLECNT4_BL_WMRK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRAECNT1 \
 *   Address error counter register 1 (AECNT1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_AECNT1_AECLR 0x80000000U

#define FTADDR_AECNT1_ADCTRCS2_SHIFT 24
#define FTADDR_AECNT1_ADCTRCS2_MASK  0xf000000U
#define FTADDR_AECNT1_ADCTRCS2_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT1_ADCTRCS2_MASK ) >> \
                    FTADDR_AECNT1_ADCTRCS2_SHIFT ) ^              \
                  0x8U ) ) -                                      \
    0x8 )
#define FTADDR_AECNT1_ADCTRCS2_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT1_ADCTRCS2_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRCS2_SHIFT ) & \
      FTADDR_AECNT1_ADCTRCS2_MASK ) )
#define FTADDR_AECNT1_ADCTRCS2( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRCS2_SHIFT ) & \
    FTADDR_AECNT1_ADCTRCS2_MASK )

#define FTADDR_AECNT1_ADCTRCS1_SHIFT 20
#define FTADDR_AECNT1_ADCTRCS1_MASK  0xf00000U
#define FTADDR_AECNT1_ADCTRCS1_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT1_ADCTRCS1_MASK ) >> \
                    FTADDR_AECNT1_ADCTRCS1_SHIFT ) ^              \
                  0x8U ) ) -                                      \
    0x8 )
#define FTADDR_AECNT1_ADCTRCS1_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT1_ADCTRCS1_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRCS1_SHIFT ) & \
      FTADDR_AECNT1_ADCTRCS1_MASK ) )
#define FTADDR_AECNT1_ADCTRCS1( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRCS1_SHIFT ) & \
    FTADDR_AECNT1_ADCTRCS1_MASK )

#define FTADDR_AECNT1_ADCTRCS0_SHIFT 16
#define FTADDR_AECNT1_ADCTRCS0_MASK  0xf0000U
#define FTADDR_AECNT1_ADCTRCS0_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT1_ADCTRCS0_MASK ) >> \
                    FTADDR_AECNT1_ADCTRCS0_SHIFT ) ^              \
                  0x8U ) ) -                                      \
    0x8 )
#define FTADDR_AECNT1_ADCTRCS0_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT1_ADCTRCS0_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRCS0_SHIFT ) & \
      FTADDR_AECNT1_ADCTRCS0_MASK ) )
#define FTADDR_AECNT1_ADCTRCS0( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRCS0_SHIFT ) & \
    FTADDR_AECNT1_ADCTRCS0_MASK )

#define FTADDR_AECNT1_ADCTRBA2_SHIFT 8
#define FTADDR_AECNT1_ADCTRBA2_MASK  0xf00U
#define FTADDR_AECNT1_ADCTRBA2_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT1_ADCTRBA2_MASK ) >> \
                    FTADDR_AECNT1_ADCTRBA2_SHIFT ) ^              \
                  0x8U ) ) -                                      \
    0x8 )
#define FTADDR_AECNT1_ADCTRBA2_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT1_ADCTRBA2_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRBA2_SHIFT ) & \
      FTADDR_AECNT1_ADCTRBA2_MASK ) )
#define FTADDR_AECNT1_ADCTRBA2( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRBA2_SHIFT ) & \
    FTADDR_AECNT1_ADCTRBA2_MASK )

#define FTADDR_AECNT1_ADCTRBA1_SHIFT 4
#define FTADDR_AECNT1_ADCTRBA1_MASK  0xf0U
#define FTADDR_AECNT1_ADCTRBA1_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT1_ADCTRBA1_MASK ) >> \
                    FTADDR_AECNT1_ADCTRBA1_SHIFT ) ^              \
                  0x8U ) ) -                                      \
    0x8 )
#define FTADDR_AECNT1_ADCTRBA1_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT1_ADCTRBA1_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRBA1_SHIFT ) & \
      FTADDR_AECNT1_ADCTRBA1_MASK ) )
#define FTADDR_AECNT1_ADCTRBA1( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRBA1_SHIFT ) & \
    FTADDR_AECNT1_ADCTRBA1_MASK )

#define FTADDR_AECNT1_ADCTRBA0_SHIFT 0
#define FTADDR_AECNT1_ADCTRBA0_MASK  0xfU
#define FTADDR_AECNT1_ADCTRBA0_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT1_ADCTRBA0_MASK ) >> \
                    FTADDR_AECNT1_ADCTRBA0_SHIFT ) ^              \
                  0x8U ) ) -                                      \
    0x8 )
#define FTADDR_AECNT1_ADCTRBA0_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT1_ADCTRBA0_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRBA0_SHIFT ) & \
      FTADDR_AECNT1_ADCTRBA0_MASK ) )
#define FTADDR_AECNT1_ADCTRBA0( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT1_ADCTRBA0_SHIFT ) & \
    FTADDR_AECNT1_ADCTRBA0_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRAECNT2 \
 *   Address error counter register 2 and 3 (AECNT2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_AECNT2_ADCTRROW15_7_SHIFT 28
#define FTADDR_AECNT2_ADCTRROW15_7_MASK  0xf0000000U
#define FTADDR_AECNT2_ADCTRROW15_7_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT2_ADCTRROW15_7_MASK ) >> \
                    FTADDR_AECNT2_ADCTRROW15_7_SHIFT ) ^              \
                  0x8U ) ) -                                          \
    0x8 )
#define FTADDR_AECNT2_ADCTRROW15_7_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT2_ADCTRROW15_7_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW15_7_SHIFT ) & \
      FTADDR_AECNT2_ADCTRROW15_7_MASK ) )
#define FTADDR_AECNT2_ADCTRROW15_7( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW15_7_SHIFT ) & \
    FTADDR_AECNT2_ADCTRROW15_7_MASK )

#define FTADDR_AECNT2_ADCTRROW14_6_SHIFT 24
#define FTADDR_AECNT2_ADCTRROW14_6_MASK  0xf000000U
#define FTADDR_AECNT2_ADCTRROW14_6_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT2_ADCTRROW14_6_MASK ) >> \
                    FTADDR_AECNT2_ADCTRROW14_6_SHIFT ) ^              \
                  0x8U ) ) -                                          \
    0x8 )
#define FTADDR_AECNT2_ADCTRROW14_6_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT2_ADCTRROW14_6_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW14_6_SHIFT ) & \
      FTADDR_AECNT2_ADCTRROW14_6_MASK ) )
#define FTADDR_AECNT2_ADCTRROW14_6( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW14_6_SHIFT ) & \
    FTADDR_AECNT2_ADCTRROW14_6_MASK )

#define FTADDR_AECNT2_ADCTRROW13_5_SHIFT 20
#define FTADDR_AECNT2_ADCTRROW13_5_MASK  0xf00000U
#define FTADDR_AECNT2_ADCTRROW13_5_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT2_ADCTRROW13_5_MASK ) >> \
                    FTADDR_AECNT2_ADCTRROW13_5_SHIFT ) ^              \
                  0x8U ) ) -                                          \
    0x8 )
#define FTADDR_AECNT2_ADCTRROW13_5_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT2_ADCTRROW13_5_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW13_5_SHIFT ) & \
      FTADDR_AECNT2_ADCTRROW13_5_MASK ) )
#define FTADDR_AECNT2_ADCTRROW13_5( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW13_5_SHIFT ) & \
    FTADDR_AECNT2_ADCTRROW13_5_MASK )

#define FTADDR_AECNT2_ADCTRROW12_4_SHIFT 16
#define FTADDR_AECNT2_ADCTRROW12_4_MASK  0xf0000U
#define FTADDR_AECNT2_ADCTRROW12_4_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT2_ADCTRROW12_4_MASK ) >> \
                    FTADDR_AECNT2_ADCTRROW12_4_SHIFT ) ^              \
                  0x8U ) ) -                                          \
    0x8 )
#define FTADDR_AECNT2_ADCTRROW12_4_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT2_ADCTRROW12_4_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW12_4_SHIFT ) & \
      FTADDR_AECNT2_ADCTRROW12_4_MASK ) )
#define FTADDR_AECNT2_ADCTRROW12_4( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW12_4_SHIFT ) & \
    FTADDR_AECNT2_ADCTRROW12_4_MASK )

#define FTADDR_AECNT2_ADCTRROW11_3_SHIFT 12
#define FTADDR_AECNT2_ADCTRROW11_3_MASK  0xf000U
#define FTADDR_AECNT2_ADCTRROW11_3_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT2_ADCTRROW11_3_MASK ) >> \
                    FTADDR_AECNT2_ADCTRROW11_3_SHIFT ) ^              \
                  0x8U ) ) -                                          \
    0x8 )
#define FTADDR_AECNT2_ADCTRROW11_3_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT2_ADCTRROW11_3_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW11_3_SHIFT ) & \
      FTADDR_AECNT2_ADCTRROW11_3_MASK ) )
#define FTADDR_AECNT2_ADCTRROW11_3( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW11_3_SHIFT ) & \
    FTADDR_AECNT2_ADCTRROW11_3_MASK )

#define FTADDR_AECNT2_ADCTRROW10_2_SHIFT 8
#define FTADDR_AECNT2_ADCTRROW10_2_MASK  0xf00U
#define FTADDR_AECNT2_ADCTRROW10_2_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT2_ADCTRROW10_2_MASK ) >> \
                    FTADDR_AECNT2_ADCTRROW10_2_SHIFT ) ^              \
                  0x8U ) ) -                                          \
    0x8 )
#define FTADDR_AECNT2_ADCTRROW10_2_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT2_ADCTRROW10_2_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW10_2_SHIFT ) & \
      FTADDR_AECNT2_ADCTRROW10_2_MASK ) )
#define FTADDR_AECNT2_ADCTRROW10_2( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW10_2_SHIFT ) & \
    FTADDR_AECNT2_ADCTRROW10_2_MASK )

#define FTADDR_AECNT2_ADCTRROW9_1_SHIFT 4
#define FTADDR_AECNT2_ADCTRROW9_1_MASK  0xf0U
#define FTADDR_AECNT2_ADCTRROW9_1_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT2_ADCTRROW9_1_MASK ) >> \
                    FTADDR_AECNT2_ADCTRROW9_1_SHIFT ) ^              \
                  0x8U ) ) -                                         \
    0x8 )
#define FTADDR_AECNT2_ADCTRROW9_1_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT2_ADCTRROW9_1_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW9_1_SHIFT ) & \
      FTADDR_AECNT2_ADCTRROW9_1_MASK ) )
#define FTADDR_AECNT2_ADCTRROW9_1( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW9_1_SHIFT ) & \
    FTADDR_AECNT2_ADCTRROW9_1_MASK )

#define FTADDR_AECNT2_ADCTRROW8_0_SHIFT 0
#define FTADDR_AECNT2_ADCTRROW8_0_MASK  0xfU
#define FTADDR_AECNT2_ADCTRROW8_0_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT2_ADCTRROW8_0_MASK ) >> \
                    FTADDR_AECNT2_ADCTRROW8_0_SHIFT ) ^              \
                  0x8U ) ) -                                         \
    0x8 )
#define FTADDR_AECNT2_ADCTRROW8_0_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT2_ADCTRROW8_0_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW8_0_SHIFT ) & \
      FTADDR_AECNT2_ADCTRROW8_0_MASK ) )
#define FTADDR_AECNT2_ADCTRROW8_0( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2_ADCTRROW8_0_SHIFT ) & \
    FTADDR_AECNT2_ADCTRROW8_0_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRAECNT4 \
 *   Address error counter register 4 (AECNT4)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_AECNT4_ADCTRCOL11_SHIFT 28
#define FTADDR_AECNT4_ADCTRCOL11_MASK  0xf0000000U
#define FTADDR_AECNT4_ADCTRCOL11_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT4_ADCTRCOL11_MASK ) >> \
                    FTADDR_AECNT4_ADCTRCOL11_SHIFT ) ^              \
                  0x8U ) ) -                                        \
    0x8 )
#define FTADDR_AECNT4_ADCTRCOL11_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT4_ADCTRCOL11_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL11_SHIFT ) & \
      FTADDR_AECNT4_ADCTRCOL11_MASK ) )
#define FTADDR_AECNT4_ADCTRCOL11( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL11_SHIFT ) & \
    FTADDR_AECNT4_ADCTRCOL11_MASK )

#define FTADDR_AECNT4_ADCTRCOL10_SHIFT 24
#define FTADDR_AECNT4_ADCTRCOL10_MASK  0xf000000U
#define FTADDR_AECNT4_ADCTRCOL10_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT4_ADCTRCOL10_MASK ) >> \
                    FTADDR_AECNT4_ADCTRCOL10_SHIFT ) ^              \
                  0x8U ) ) -                                        \
    0x8 )
#define FTADDR_AECNT4_ADCTRCOL10_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT4_ADCTRCOL10_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL10_SHIFT ) & \
      FTADDR_AECNT4_ADCTRCOL10_MASK ) )
#define FTADDR_AECNT4_ADCTRCOL10( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL10_SHIFT ) & \
    FTADDR_AECNT4_ADCTRCOL10_MASK )

#define FTADDR_AECNT4_ADCTRCOL9_SHIFT 20
#define FTADDR_AECNT4_ADCTRCOL9_MASK  0xf00000U
#define FTADDR_AECNT4_ADCTRCOL9_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT4_ADCTRCOL9_MASK ) >> \
                    FTADDR_AECNT4_ADCTRCOL9_SHIFT ) ^              \
                  0x8U ) ) -                                       \
    0x8 )
#define FTADDR_AECNT4_ADCTRCOL9_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT4_ADCTRCOL9_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL9_SHIFT ) & \
      FTADDR_AECNT4_ADCTRCOL9_MASK ) )
#define FTADDR_AECNT4_ADCTRCOL9( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL9_SHIFT ) & \
    FTADDR_AECNT4_ADCTRCOL9_MASK )

#define FTADDR_AECNT4_ADCTRCOL8_SHIFT 16
#define FTADDR_AECNT4_ADCTRCOL8_MASK  0xf0000U
#define FTADDR_AECNT4_ADCTRCOL8_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT4_ADCTRCOL8_MASK ) >> \
                    FTADDR_AECNT4_ADCTRCOL8_SHIFT ) ^              \
                  0x8U ) ) -                                       \
    0x8 )
#define FTADDR_AECNT4_ADCTRCOL8_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT4_ADCTRCOL8_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL8_SHIFT ) & \
      FTADDR_AECNT4_ADCTRCOL8_MASK ) )
#define FTADDR_AECNT4_ADCTRCOL8( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL8_SHIFT ) & \
    FTADDR_AECNT4_ADCTRCOL8_MASK )

#define FTADDR_AECNT4_ADCTRCOL7_SHIFT 12
#define FTADDR_AECNT4_ADCTRCOL7_MASK  0xf000U
#define FTADDR_AECNT4_ADCTRCOL7_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT4_ADCTRCOL7_MASK ) >> \
                    FTADDR_AECNT4_ADCTRCOL7_SHIFT ) ^              \
                  0x8U ) ) -                                       \
    0x8 )
#define FTADDR_AECNT4_ADCTRCOL7_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT4_ADCTRCOL7_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL7_SHIFT ) & \
      FTADDR_AECNT4_ADCTRCOL7_MASK ) )
#define FTADDR_AECNT4_ADCTRCOL7( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL7_SHIFT ) & \
    FTADDR_AECNT4_ADCTRCOL7_MASK )

#define FTADDR_AECNT4_ADCTRCOL6_SHIFT 8
#define FTADDR_AECNT4_ADCTRCOL6_MASK  0xf00U
#define FTADDR_AECNT4_ADCTRCOL6_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT4_ADCTRCOL6_MASK ) >> \
                    FTADDR_AECNT4_ADCTRCOL6_SHIFT ) ^              \
                  0x8U ) ) -                                       \
    0x8 )
#define FTADDR_AECNT4_ADCTRCOL6_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT4_ADCTRCOL6_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL6_SHIFT ) & \
      FTADDR_AECNT4_ADCTRCOL6_MASK ) )
#define FTADDR_AECNT4_ADCTRCOL6( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL6_SHIFT ) & \
    FTADDR_AECNT4_ADCTRCOL6_MASK )

#define FTADDR_AECNT4_ADCTRCOL5_SHIFT 4
#define FTADDR_AECNT4_ADCTRCOL5_MASK  0xf0U
#define FTADDR_AECNT4_ADCTRCOL5_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT4_ADCTRCOL5_MASK ) >> \
                    FTADDR_AECNT4_ADCTRCOL5_SHIFT ) ^              \
                  0x8U ) ) -                                       \
    0x8 )
#define FTADDR_AECNT4_ADCTRCOL5_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT4_ADCTRCOL5_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL5_SHIFT ) & \
      FTADDR_AECNT4_ADCTRCOL5_MASK ) )
#define FTADDR_AECNT4_ADCTRCOL5( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL5_SHIFT ) & \
    FTADDR_AECNT4_ADCTRCOL5_MASK )

#define FTADDR_AECNT4_ADCTRCOL4_SHIFT 0
#define FTADDR_AECNT4_ADCTRCOL4_MASK  0xfU
#define FTADDR_AECNT4_ADCTRCOL4_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT4_ADCTRCOL4_MASK ) >> \
                    FTADDR_AECNT4_ADCTRCOL4_SHIFT ) ^              \
                  0x8U ) ) -                                       \
    0x8 )
#define FTADDR_AECNT4_ADCTRCOL4_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT4_ADCTRCOL4_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL4_SHIFT ) & \
      FTADDR_AECNT4_ADCTRCOL4_MASK ) )
#define FTADDR_AECNT4_ADCTRCOL4( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT4_ADCTRCOL4_SHIFT ) & \
    FTADDR_AECNT4_ADCTRCOL4_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRAHBERRLOC \
 *   AHB correctable and uncorrectable error location (AHBERRLOC)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_AHBERRLOC_CEUEADDRESS_SHIFT 0
#define FTADDR_AHBERRLOC_CEUEADDRESS_MASK  0xffffffffU
#define FTADDR_AHBERRLOC_CEUEADDRESS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBERRLOC_CEUEADDRESS_MASK ) >> \
    FTADDR_AHBERRLOC_CEUEADDRESS_SHIFT )
#define FTADDR_AHBERRLOC_CEUEADDRESS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBERRLOC_CEUEADDRESS_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBERRLOC_CEUEADDRESS_SHIFT ) & \
      FTADDR_AHBERRLOC_CEUEADDRESS_MASK ) )
#define FTADDR_AHBERRLOC_CEUEADDRESS( _val )             \
  ( ( ( _val ) << FTADDR_AHBERRLOC_CEUEADDRESS_SHIFT ) & \
    FTADDR_AHBERRLOC_CEUEADDRESS_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPFSTAT Prefetch status register (PFSTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PFSTAT_PFACT 0x80000000U

#define FTADDR_PFSTAT_PFDRN 0x40000000U

#define FTADDR_PFSTAT_PREF_FETCH_POS_SHIFT 25
#define FTADDR_PFSTAT_PREF_FETCH_POS_MASK  0x3e000000U
#define FTADDR_PFSTAT_PREF_FETCH_POS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFSTAT_PREF_FETCH_POS_MASK ) >> \
    FTADDR_PFSTAT_PREF_FETCH_POS_SHIFT )
#define FTADDR_PFSTAT_PREF_FETCH_POS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFSTAT_PREF_FETCH_POS_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFSTAT_PREF_FETCH_POS_SHIFT ) & \
      FTADDR_PFSTAT_PREF_FETCH_POS_MASK ) )
#define FTADDR_PFSTAT_PREF_FETCH_POS( _val )             \
  ( ( ( _val ) << FTADDR_PFSTAT_PREF_FETCH_POS_SHIFT ) & \
    FTADDR_PFSTAT_PREF_FETCH_POS_MASK )

#define FTADDR_PFSTAT_PREF_READOUT_POS_SHIFT 16
#define FTADDR_PFSTAT_PREF_READOUT_POS_MASK  0x1ff0000U
#define FTADDR_PFSTAT_PREF_READOUT_POS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFSTAT_PREF_READOUT_POS_MASK ) >> \
    FTADDR_PFSTAT_PREF_READOUT_POS_SHIFT )
#define FTADDR_PFSTAT_PREF_READOUT_POS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFSTAT_PREF_READOUT_POS_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFSTAT_PREF_READOUT_POS_SHIFT ) & \
      FTADDR_PFSTAT_PREF_READOUT_POS_MASK ) )
#define FTADDR_PFSTAT_PREF_READOUT_POS( _val )             \
  ( ( ( _val ) << FTADDR_PFSTAT_PREF_READOUT_POS_SHIFT ) & \
    FTADDR_PFSTAT_PREF_READOUT_POS_MASK )

#define FTADDR_PFSTAT_PREFETCH_ROW_SHIFT 0
#define FTADDR_PFSTAT_PREFETCH_ROW_MASK  0xffffU
#define FTADDR_PFSTAT_PREFETCH_ROW_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFSTAT_PREFETCH_ROW_MASK ) >> \
    FTADDR_PFSTAT_PREFETCH_ROW_SHIFT )
#define FTADDR_PFSTAT_PREFETCH_ROW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFSTAT_PREFETCH_ROW_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFSTAT_PREFETCH_ROW_SHIFT ) & \
      FTADDR_PFSTAT_PREFETCH_ROW_MASK ) )
#define FTADDR_PFSTAT_PREFETCH_ROW( _val )             \
  ( ( ( _val ) << FTADDR_PFSTAT_PREFETCH_ROW_SHIFT ) & \
    FTADDR_PFSTAT_PREFETCH_ROW_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPFBSTAT \
 *   Prefetch bank status register (PFBSTAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PFBSTAT_PREF3BG_SHIFT 30
#define FTADDR_PFBSTAT_PREF3BG_MASK  0xc0000000U
#define FTADDR_PFBSTAT_PREF3BG_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF3BG_MASK ) >> \
    FTADDR_PFBSTAT_PREF3BG_SHIFT )
#define FTADDR_PFBSTAT_PREF3BG_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF3BG_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF3BG_SHIFT ) & \
      FTADDR_PFBSTAT_PREF3BG_MASK ) )
#define FTADDR_PFBSTAT_PREF3BG( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF3BG_SHIFT ) & \
    FTADDR_PFBSTAT_PREF3BG_MASK )

#define FTADDR_PFBSTAT_PREF3CS_SHIFT 27
#define FTADDR_PFBSTAT_PREF3CS_MASK  0x38000000U
#define FTADDR_PFBSTAT_PREF3CS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF3CS_MASK ) >> \
    FTADDR_PFBSTAT_PREF3CS_SHIFT )
#define FTADDR_PFBSTAT_PREF3CS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF3CS_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF3CS_SHIFT ) & \
      FTADDR_PFBSTAT_PREF3CS_MASK ) )
#define FTADDR_PFBSTAT_PREF3CS( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF3CS_SHIFT ) & \
    FTADDR_PFBSTAT_PREF3CS_MASK )

#define FTADDR_PFBSTAT_PREF3IBANK_SHIFT 24
#define FTADDR_PFBSTAT_PREF3IBANK_MASK  0x7000000U
#define FTADDR_PFBSTAT_PREF3IBANK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF3IBANK_MASK ) >> \
    FTADDR_PFBSTAT_PREF3IBANK_SHIFT )
#define FTADDR_PFBSTAT_PREF3IBANK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF3IBANK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF3IBANK_SHIFT ) & \
      FTADDR_PFBSTAT_PREF3IBANK_MASK ) )
#define FTADDR_PFBSTAT_PREF3IBANK( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF3IBANK_SHIFT ) & \
    FTADDR_PFBSTAT_PREF3IBANK_MASK )

#define FTADDR_PFBSTAT_PREF2BG_SHIFT 22
#define FTADDR_PFBSTAT_PREF2BG_MASK  0xc00000U
#define FTADDR_PFBSTAT_PREF2BG_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF2BG_MASK ) >> \
    FTADDR_PFBSTAT_PREF2BG_SHIFT )
#define FTADDR_PFBSTAT_PREF2BG_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF2BG_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF2BG_SHIFT ) & \
      FTADDR_PFBSTAT_PREF2BG_MASK ) )
#define FTADDR_PFBSTAT_PREF2BG( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF2BG_SHIFT ) & \
    FTADDR_PFBSTAT_PREF2BG_MASK )

#define FTADDR_PFBSTAT_PREF2CS_SHIFT 19
#define FTADDR_PFBSTAT_PREF2CS_MASK  0x380000U
#define FTADDR_PFBSTAT_PREF2CS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF2CS_MASK ) >> \
    FTADDR_PFBSTAT_PREF2CS_SHIFT )
#define FTADDR_PFBSTAT_PREF2CS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF2CS_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF2CS_SHIFT ) & \
      FTADDR_PFBSTAT_PREF2CS_MASK ) )
#define FTADDR_PFBSTAT_PREF2CS( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF2CS_SHIFT ) & \
    FTADDR_PFBSTAT_PREF2CS_MASK )

#define FTADDR_PFBSTAT_PREF2IBANK_SHIFT 16
#define FTADDR_PFBSTAT_PREF2IBANK_MASK  0x70000U
#define FTADDR_PFBSTAT_PREF2IBANK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF2IBANK_MASK ) >> \
    FTADDR_PFBSTAT_PREF2IBANK_SHIFT )
#define FTADDR_PFBSTAT_PREF2IBANK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF2IBANK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF2IBANK_SHIFT ) & \
      FTADDR_PFBSTAT_PREF2IBANK_MASK ) )
#define FTADDR_PFBSTAT_PREF2IBANK( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF2IBANK_SHIFT ) & \
    FTADDR_PFBSTAT_PREF2IBANK_MASK )

#define FTADDR_PFBSTAT_PREF1BG_SHIFT 14
#define FTADDR_PFBSTAT_PREF1BG_MASK  0xc000U
#define FTADDR_PFBSTAT_PREF1BG_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF1BG_MASK ) >> \
    FTADDR_PFBSTAT_PREF1BG_SHIFT )
#define FTADDR_PFBSTAT_PREF1BG_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF1BG_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF1BG_SHIFT ) & \
      FTADDR_PFBSTAT_PREF1BG_MASK ) )
#define FTADDR_PFBSTAT_PREF1BG( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF1BG_SHIFT ) & \
    FTADDR_PFBSTAT_PREF1BG_MASK )

#define FTADDR_PFBSTAT_PREF1CS_SHIFT 11
#define FTADDR_PFBSTAT_PREF1CS_MASK  0x3800U
#define FTADDR_PFBSTAT_PREF1CS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF1CS_MASK ) >> \
    FTADDR_PFBSTAT_PREF1CS_SHIFT )
#define FTADDR_PFBSTAT_PREF1CS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF1CS_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF1CS_SHIFT ) & \
      FTADDR_PFBSTAT_PREF1CS_MASK ) )
#define FTADDR_PFBSTAT_PREF1CS( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF1CS_SHIFT ) & \
    FTADDR_PFBSTAT_PREF1CS_MASK )

#define FTADDR_PFBSTAT_PREF1IBANK_SHIFT 8
#define FTADDR_PFBSTAT_PREF1IBANK_MASK  0x700U
#define FTADDR_PFBSTAT_PREF1IBANK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF1IBANK_MASK ) >> \
    FTADDR_PFBSTAT_PREF1IBANK_SHIFT )
#define FTADDR_PFBSTAT_PREF1IBANK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF1IBANK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF1IBANK_SHIFT ) & \
      FTADDR_PFBSTAT_PREF1IBANK_MASK ) )
#define FTADDR_PFBSTAT_PREF1IBANK( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF1IBANK_SHIFT ) & \
    FTADDR_PFBSTAT_PREF1IBANK_MASK )

#define FTADDR_PFBSTAT_PREF0BG_SHIFT 6
#define FTADDR_PFBSTAT_PREF0BG_MASK  0xc0U
#define FTADDR_PFBSTAT_PREF0BG_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF0BG_MASK ) >> \
    FTADDR_PFBSTAT_PREF0BG_SHIFT )
#define FTADDR_PFBSTAT_PREF0BG_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF0BG_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF0BG_SHIFT ) & \
      FTADDR_PFBSTAT_PREF0BG_MASK ) )
#define FTADDR_PFBSTAT_PREF0BG( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF0BG_SHIFT ) & \
    FTADDR_PFBSTAT_PREF0BG_MASK )

#define FTADDR_PFBSTAT_PREF0CS_SHIFT 3
#define FTADDR_PFBSTAT_PREF0CS_MASK  0x38U
#define FTADDR_PFBSTAT_PREF0CS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF0CS_MASK ) >> \
    FTADDR_PFBSTAT_PREF0CS_SHIFT )
#define FTADDR_PFBSTAT_PREF0CS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF0CS_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF0CS_SHIFT ) & \
      FTADDR_PFBSTAT_PREF0CS_MASK ) )
#define FTADDR_PFBSTAT_PREF0CS( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF0CS_SHIFT ) & \
    FTADDR_PFBSTAT_PREF0CS_MASK )

#define FTADDR_PFBSTAT_PREF0IBANK_SHIFT 0
#define FTADDR_PFBSTAT_PREF0IBANK_MASK  0x7U
#define FTADDR_PFBSTAT_PREF0IBANK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_PFBSTAT_PREF0IBANK_MASK ) >> \
    FTADDR_PFBSTAT_PREF0IBANK_SHIFT )
#define FTADDR_PFBSTAT_PREF0IBANK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_PFBSTAT_PREF0IBANK_MASK ) |    \
    ( ( ( _val ) << FTADDR_PFBSTAT_PREF0IBANK_SHIFT ) & \
      FTADDR_PFBSTAT_PREF0IBANK_MASK ) )
#define FTADDR_PFBSTAT_PREF0IBANK( _val )             \
  ( ( ( _val ) << FTADDR_PFBSTAT_PREF0IBANK_SHIFT ) & \
    FTADDR_PFBSTAT_PREF0IBANK_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCRUBSTART \
 *   Scrubber start address register (SCRUBSTART)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCRUBSTART_SCSTCS_SHIFT 28
#define FTADDR_SCRUBSTART_SCSTCS_MASK  0x70000000U
#define FTADDR_SCRUBSTART_SCSTCS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBSTART_SCSTCS_MASK ) >> \
    FTADDR_SCRUBSTART_SCSTCS_SHIFT )
#define FTADDR_SCRUBSTART_SCSTCS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBSTART_SCSTCS_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBSTART_SCSTCS_SHIFT ) & \
      FTADDR_SCRUBSTART_SCSTCS_MASK ) )
#define FTADDR_SCRUBSTART_SCSTCS( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBSTART_SCSTCS_SHIFT ) & \
    FTADDR_SCRUBSTART_SCSTCS_MASK )

#define FTADDR_SCRUBSTART_SCSTIBANK_SHIFT 25
#define FTADDR_SCRUBSTART_SCSTIBANK_MASK  0xe000000U
#define FTADDR_SCRUBSTART_SCSTIBANK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBSTART_SCSTIBANK_MASK ) >> \
    FTADDR_SCRUBSTART_SCSTIBANK_SHIFT )
#define FTADDR_SCRUBSTART_SCSTIBANK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBSTART_SCSTIBANK_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBSTART_SCSTIBANK_SHIFT ) & \
      FTADDR_SCRUBSTART_SCSTIBANK_MASK ) )
#define FTADDR_SCRUBSTART_SCSTIBANK( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBSTART_SCSTIBANK_SHIFT ) & \
    FTADDR_SCRUBSTART_SCSTIBANK_MASK )

#define FTADDR_SCRUBSTART_SCSTROW_SHIFT 9
#define FTADDR_SCRUBSTART_SCSTROW_MASK  0x1fffe00U
#define FTADDR_SCRUBSTART_SCSTROW_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBSTART_SCSTROW_MASK ) >> \
    FTADDR_SCRUBSTART_SCSTROW_SHIFT )
#define FTADDR_SCRUBSTART_SCSTROW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBSTART_SCSTROW_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBSTART_SCSTROW_SHIFT ) & \
      FTADDR_SCRUBSTART_SCSTROW_MASK ) )
#define FTADDR_SCRUBSTART_SCSTROW( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBSTART_SCSTROW_SHIFT ) & \
    FTADDR_SCRUBSTART_SCSTROW_MASK )

#define FTADDR_SCRUBSTART_SCSTCOL_SHIFT 0
#define FTADDR_SCRUBSTART_SCSTCOL_MASK  0x1ffU
#define FTADDR_SCRUBSTART_SCSTCOL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBSTART_SCSTCOL_MASK ) >> \
    FTADDR_SCRUBSTART_SCSTCOL_SHIFT )
#define FTADDR_SCRUBSTART_SCSTCOL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBSTART_SCSTCOL_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBSTART_SCSTCOL_SHIFT ) & \
      FTADDR_SCRUBSTART_SCSTCOL_MASK ) )
#define FTADDR_SCRUBSTART_SCSTCOL( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBSTART_SCSTCOL_SHIFT ) & \
    FTADDR_SCRUBSTART_SCSTCOL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCRUBEND \
 *   Scrubber end address register (SCRUBEND)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCRUBEND_SCEDCS_SHIFT 28
#define FTADDR_SCRUBEND_SCEDCS_MASK  0x70000000U
#define FTADDR_SCRUBEND_SCEDCS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBEND_SCEDCS_MASK ) >> \
    FTADDR_SCRUBEND_SCEDCS_SHIFT )
#define FTADDR_SCRUBEND_SCEDCS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBEND_SCEDCS_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBEND_SCEDCS_SHIFT ) & \
      FTADDR_SCRUBEND_SCEDCS_MASK ) )
#define FTADDR_SCRUBEND_SCEDCS( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBEND_SCEDCS_SHIFT ) & \
    FTADDR_SCRUBEND_SCEDCS_MASK )

#define FTADDR_SCRUBEND_SCEDIBANK_SHIFT 25
#define FTADDR_SCRUBEND_SCEDIBANK_MASK  0xe000000U
#define FTADDR_SCRUBEND_SCEDIBANK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBEND_SCEDIBANK_MASK ) >> \
    FTADDR_SCRUBEND_SCEDIBANK_SHIFT )
#define FTADDR_SCRUBEND_SCEDIBANK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBEND_SCEDIBANK_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBEND_SCEDIBANK_SHIFT ) & \
      FTADDR_SCRUBEND_SCEDIBANK_MASK ) )
#define FTADDR_SCRUBEND_SCEDIBANK( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBEND_SCEDIBANK_SHIFT ) & \
    FTADDR_SCRUBEND_SCEDIBANK_MASK )

#define FTADDR_SCRUBEND_SCEDROW_SHIFT 9
#define FTADDR_SCRUBEND_SCEDROW_MASK  0x1fffe00U
#define FTADDR_SCRUBEND_SCEDROW_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBEND_SCEDROW_MASK ) >> \
    FTADDR_SCRUBEND_SCEDROW_SHIFT )
#define FTADDR_SCRUBEND_SCEDROW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBEND_SCEDROW_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBEND_SCEDROW_SHIFT ) & \
      FTADDR_SCRUBEND_SCEDROW_MASK ) )
#define FTADDR_SCRUBEND_SCEDROW( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBEND_SCEDROW_SHIFT ) & \
    FTADDR_SCRUBEND_SCEDROW_MASK )

#define FTADDR_SCRUBEND_SCEDCOL_SHIFT 0
#define FTADDR_SCRUBEND_SCEDCOL_MASK  0x1ffU
#define FTADDR_SCRUBEND_SCEDCOL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBEND_SCEDCOL_MASK ) >> \
    FTADDR_SCRUBEND_SCEDCOL_SHIFT )
#define FTADDR_SCRUBEND_SCEDCOL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBEND_SCEDCOL_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBEND_SCEDCOL_SHIFT ) & \
      FTADDR_SCRUBEND_SCEDCOL_MASK ) )
#define FTADDR_SCRUBEND_SCEDCOL( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBEND_SCEDCOL_SHIFT ) & \
    FTADDR_SCRUBEND_SCEDCOL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRFEFECNT \
 *   Frontend FIFO error counter register (FEFECNT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_FEFECNT_RS3UE 0x40000000U

#define FTADDR_FEFECNT_RS3CE_SHIFT 28
#define FTADDR_FEFECNT_RS3CE_MASK  0x30000000U
#define FTADDR_FEFECNT_RS3CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_FEFECNT_RS3CE_MASK ) >> FTADDR_FEFECNT_RS3CE_SHIFT )
#define FTADDR_FEFECNT_RS3CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_FEFECNT_RS3CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_FEFECNT_RS3CE_SHIFT ) & \
      FTADDR_FEFECNT_RS3CE_MASK ) )
#define FTADDR_FEFECNT_RS3CE( _val ) \
  ( ( ( _val ) << FTADDR_FEFECNT_RS3CE_SHIFT ) & FTADDR_FEFECNT_RS3CE_MASK )

#define FTADDR_FEFECNT_RD3UE 0x8000000U

#define FTADDR_FEFECNT_PF3UE 0x4000000U

#define FTADDR_FEFECNT_RDPF3CE_SHIFT 24
#define FTADDR_FEFECNT_RDPF3CE_MASK  0x3000000U
#define FTADDR_FEFECNT_RDPF3CE_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_FEFECNT_RDPF3CE_MASK ) >> \
    FTADDR_FEFECNT_RDPF3CE_SHIFT )
#define FTADDR_FEFECNT_RDPF3CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_FEFECNT_RDPF3CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_FEFECNT_RDPF3CE_SHIFT ) & \
      FTADDR_FEFECNT_RDPF3CE_MASK ) )
#define FTADDR_FEFECNT_RDPF3CE( _val )             \
  ( ( ( _val ) << FTADDR_FEFECNT_RDPF3CE_SHIFT ) & \
    FTADDR_FEFECNT_RDPF3CE_MASK )

#define FTADDR_FEFECNT_RS2UE 0x400000U

#define FTADDR_FEFECNT_RS2CE_SHIFT 20
#define FTADDR_FEFECNT_RS2CE_MASK  0x300000U
#define FTADDR_FEFECNT_RS2CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_FEFECNT_RS2CE_MASK ) >> FTADDR_FEFECNT_RS2CE_SHIFT )
#define FTADDR_FEFECNT_RS2CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_FEFECNT_RS2CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_FEFECNT_RS2CE_SHIFT ) & \
      FTADDR_FEFECNT_RS2CE_MASK ) )
#define FTADDR_FEFECNT_RS2CE( _val ) \
  ( ( ( _val ) << FTADDR_FEFECNT_RS2CE_SHIFT ) & FTADDR_FEFECNT_RS2CE_MASK )

#define FTADDR_FEFECNT_RD2UE 0x80000U

#define FTADDR_FEFECNT_PF2UE 0x40000U

#define FTADDR_FEFECNT_RDPF2CE_SHIFT 16
#define FTADDR_FEFECNT_RDPF2CE_MASK  0x30000U
#define FTADDR_FEFECNT_RDPF2CE_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_FEFECNT_RDPF2CE_MASK ) >> \
    FTADDR_FEFECNT_RDPF2CE_SHIFT )
#define FTADDR_FEFECNT_RDPF2CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_FEFECNT_RDPF2CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_FEFECNT_RDPF2CE_SHIFT ) & \
      FTADDR_FEFECNT_RDPF2CE_MASK ) )
#define FTADDR_FEFECNT_RDPF2CE( _val )             \
  ( ( ( _val ) << FTADDR_FEFECNT_RDPF2CE_SHIFT ) & \
    FTADDR_FEFECNT_RDPF2CE_MASK )

#define FTADDR_FEFECNT_RS1UE 0x4000U

#define FTADDR_FEFECNT_RS1CE_SHIFT 12
#define FTADDR_FEFECNT_RS1CE_MASK  0x3000U
#define FTADDR_FEFECNT_RS1CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_FEFECNT_RS1CE_MASK ) >> FTADDR_FEFECNT_RS1CE_SHIFT )
#define FTADDR_FEFECNT_RS1CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_FEFECNT_RS1CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_FEFECNT_RS1CE_SHIFT ) & \
      FTADDR_FEFECNT_RS1CE_MASK ) )
#define FTADDR_FEFECNT_RS1CE( _val ) \
  ( ( ( _val ) << FTADDR_FEFECNT_RS1CE_SHIFT ) & FTADDR_FEFECNT_RS1CE_MASK )

#define FTADDR_FEFECNT_RD1UE 0x800U

#define FTADDR_FEFECNT_PF1UE 0x400U

#define FTADDR_FEFECNT_RDPF1CE_SHIFT 8
#define FTADDR_FEFECNT_RDPF1CE_MASK  0x300U
#define FTADDR_FEFECNT_RDPF1CE_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_FEFECNT_RDPF1CE_MASK ) >> \
    FTADDR_FEFECNT_RDPF1CE_SHIFT )
#define FTADDR_FEFECNT_RDPF1CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_FEFECNT_RDPF1CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_FEFECNT_RDPF1CE_SHIFT ) & \
      FTADDR_FEFECNT_RDPF1CE_MASK ) )
#define FTADDR_FEFECNT_RDPF1CE( _val )             \
  ( ( ( _val ) << FTADDR_FEFECNT_RDPF1CE_SHIFT ) & \
    FTADDR_FEFECNT_RDPF1CE_MASK )

#define FTADDR_FEFECNT_RS0UE 0x40U

#define FTADDR_FEFECNT_RS0CE_SHIFT 4
#define FTADDR_FEFECNT_RS0CE_MASK  0x30U
#define FTADDR_FEFECNT_RS0CE_GET( _reg ) \
  ( ( ( _reg ) & FTADDR_FEFECNT_RS0CE_MASK ) >> FTADDR_FEFECNT_RS0CE_SHIFT )
#define FTADDR_FEFECNT_RS0CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_FEFECNT_RS0CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_FEFECNT_RS0CE_SHIFT ) & \
      FTADDR_FEFECNT_RS0CE_MASK ) )
#define FTADDR_FEFECNT_RS0CE( _val ) \
  ( ( ( _val ) << FTADDR_FEFECNT_RS0CE_SHIFT ) & FTADDR_FEFECNT_RS0CE_MASK )

#define FTADDR_FEFECNT_RD0UE 0x8U

#define FTADDR_FEFECNT_PF0UE 0x4U

#define FTADDR_FEFECNT_RDPF0CE_SHIFT 0
#define FTADDR_FEFECNT_RDPF0CE_MASK  0x3U
#define FTADDR_FEFECNT_RDPF0CE_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_FEFECNT_RDPF0CE_MASK ) >> \
    FTADDR_FEFECNT_RDPF0CE_SHIFT )
#define FTADDR_FEFECNT_RDPF0CE_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_FEFECNT_RDPF0CE_MASK ) |    \
    ( ( ( _val ) << FTADDR_FEFECNT_RDPF0CE_SHIFT ) & \
      FTADDR_FEFECNT_RDPF0CE_MASK ) )
#define FTADDR_FEFECNT_RDPF0CE( _val )             \
  ( ( ( _val ) << FTADDR_FEFECNT_RDPF0CE_SHIFT ) & \
    FTADDR_FEFECNT_RDPF0CE_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRINITPAT \
 *   Initialization pattern register (INITPAT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_INITPAT_INIT_PATTERN_DATA_SHIFT 0
#define FTADDR_INITPAT_INIT_PATTERN_DATA_MASK  0xffffffffU
#define FTADDR_INITPAT_INIT_PATTERN_DATA_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_INITPAT_INIT_PATTERN_DATA_MASK ) >> \
    FTADDR_INITPAT_INIT_PATTERN_DATA_SHIFT )
#define FTADDR_INITPAT_INIT_PATTERN_DATA_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_INITPAT_INIT_PATTERN_DATA_MASK ) |    \
    ( ( ( _val ) << FTADDR_INITPAT_INIT_PATTERN_DATA_SHIFT ) & \
      FTADDR_INITPAT_INIT_PATTERN_DATA_MASK ) )
#define FTADDR_INITPAT_INIT_PATTERN_DATA( _val )             \
  ( ( ( _val ) << FTADDR_INITPAT_INIT_PATTERN_DATA_SHIFT ) & \
    FTADDR_INITPAT_INIT_PATTERN_DATA_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCRUBPOS \
 *   Scrubber position register (SCRUBPOS)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCRUBPOS_SCWR 0x80000000U

#define FTADDR_SCRUBPOS_SCCURCS_SHIFT 28
#define FTADDR_SCRUBPOS_SCCURCS_MASK  0x70000000U
#define FTADDR_SCRUBPOS_SCCURCS_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBPOS_SCCURCS_MASK ) >> \
    FTADDR_SCRUBPOS_SCCURCS_SHIFT )
#define FTADDR_SCRUBPOS_SCCURCS_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBPOS_SCCURCS_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBPOS_SCCURCS_SHIFT ) & \
      FTADDR_SCRUBPOS_SCCURCS_MASK ) )
#define FTADDR_SCRUBPOS_SCCURCS( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBPOS_SCCURCS_SHIFT ) & \
    FTADDR_SCRUBPOS_SCCURCS_MASK )

#define FTADDR_SCRUBPOS_SCCURIBANK_SHIFT 25
#define FTADDR_SCRUBPOS_SCCURIBANK_MASK  0xe000000U
#define FTADDR_SCRUBPOS_SCCURIBANK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBPOS_SCCURIBANK_MASK ) >> \
    FTADDR_SCRUBPOS_SCCURIBANK_SHIFT )
#define FTADDR_SCRUBPOS_SCCURIBANK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBPOS_SCCURIBANK_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBPOS_SCCURIBANK_SHIFT ) & \
      FTADDR_SCRUBPOS_SCCURIBANK_MASK ) )
#define FTADDR_SCRUBPOS_SCCURIBANK( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBPOS_SCCURIBANK_SHIFT ) & \
    FTADDR_SCRUBPOS_SCCURIBANK_MASK )

#define FTADDR_SCRUBPOS_SCCURROW_SHIFT 9
#define FTADDR_SCRUBPOS_SCCURROW_MASK  0x1fffe00U
#define FTADDR_SCRUBPOS_SCCURROW_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBPOS_SCCURROW_MASK ) >> \
    FTADDR_SCRUBPOS_SCCURROW_SHIFT )
#define FTADDR_SCRUBPOS_SCCURROW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBPOS_SCCURROW_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBPOS_SCCURROW_SHIFT ) & \
      FTADDR_SCRUBPOS_SCCURROW_MASK ) )
#define FTADDR_SCRUBPOS_SCCURROW( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBPOS_SCCURROW_SHIFT ) & \
    FTADDR_SCRUBPOS_SCCURROW_MASK )

#define FTADDR_SCRUBPOS_SCCURCOL_SHIFT 0
#define FTADDR_SCRUBPOS_SCCURCOL_MASK  0x1ffU
#define FTADDR_SCRUBPOS_SCCURCOL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBPOS_SCCURCOL_MASK ) >> \
    FTADDR_SCRUBPOS_SCCURCOL_SHIFT )
#define FTADDR_SCRUBPOS_SCCURCOL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBPOS_SCCURCOL_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBPOS_SCCURCOL_SHIFT ) & \
      FTADDR_SCRUBPOS_SCCURCOL_MASK ) )
#define FTADDR_SCRUBPOS_SCCURCOL( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBPOS_SCCURCOL_SHIFT ) & \
    FTADDR_SCRUBPOS_SCCURCOL_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRAHBERRLOCHI \
 *   AHB correctable and uncorrectable error location high part (AHBERRLOCHI)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_SHIFT 0
#define FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_MASK  0xffffffffU
#define FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_MASK ) >> \
    FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_SHIFT )
#define FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_MASK ) |    \
    ( ( ( _val ) << FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_SHIFT ) & \
      FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_MASK ) )
#define FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART( _val )             \
  ( ( ( _val ) << FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_SHIFT ) & \
    FTADDR_AHBERRLOCHI_CEUEADDRESSHIGHPART_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCRUBUEEXT \
 *   Scrubber uncorrectable error extended register (SCRUBUEEXT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCRUBUEEXT_SEBG_SHIFT 2
#define FTADDR_SCRUBUEEXT_SEBG_MASK  0xcU
#define FTADDR_SCRUBUEEXT_SEBG_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_SCRUBUEEXT_SEBG_MASK ) >> \
    FTADDR_SCRUBUEEXT_SEBG_SHIFT )
#define FTADDR_SCRUBUEEXT_SEBG_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_SCRUBUEEXT_SEBG_MASK ) |    \
    ( ( ( _val ) << FTADDR_SCRUBUEEXT_SEBG_SHIFT ) & \
      FTADDR_SCRUBUEEXT_SEBG_MASK ) )
#define FTADDR_SCRUBUEEXT_SEBG( _val )             \
  ( ( ( _val ) << FTADDR_SCRUBUEEXT_SEBG_SHIFT ) & \
    FTADDR_SCRUBUEEXT_SEBG_MASK )

#define FTADDR_SCRUBUEEXT_SEROW16 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRAECNT2EXT \
 *   Address error counter register 2 extended (AECNT2EXT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_AECNT2EXT_ADCTRROW16_SHIFT 0
#define FTADDR_AECNT2EXT_ADCTRROW16_MASK  0xfU
#define FTADDR_AECNT2EXT_ADCTRROW16_GET( _reg )                        \
  ( ( (int32_t) ( ( ( ( _reg ) & FTADDR_AECNT2EXT_ADCTRROW16_MASK ) >> \
                    FTADDR_AECNT2EXT_ADCTRROW16_SHIFT ) ^              \
                  0x8U ) ) -                                           \
    0x8 )
#define FTADDR_AECNT2EXT_ADCTRROW16_SET( _reg, _val )                    \
  ( ( ( _reg ) & ~FTADDR_AECNT2EXT_ADCTRROW16_MASK ) |                   \
    ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2EXT_ADCTRROW16_SHIFT ) & \
      FTADDR_AECNT2EXT_ADCTRROW16_MASK ) )
#define FTADDR_AECNT2EXT_ADCTRROW16( _val )                            \
  ( ( ( (uint32_t) ( _val ) ) << FTADDR_AECNT2EXT_ADCTRROW16_SHIFT ) & \
    FTADDR_AECNT2EXT_ADCTRROW16_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRPFSTATEXT \
 *   Prefetch status extended register (PFSTATEXT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_PFSTATEXT_PFR16 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCRUBSTARTEXT \
 *   Scrubber start address extended register (SCRUBSTARTEXT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCRUBSTARTEXT_SSBG1 0x8U

#define FTADDR_SCRUBSTARTEXT_SSR16 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCRUBENDEXT \
 *   Scrubber end address extended register (SCRUBENDEXT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCRUBENDEXT_SEBG1 0x8U

#define FTADDR_SCRUBENDEXT_SER16 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSCRUBPOSEXT \
 *   Scrubber position extended register (SCRUBPOSEXT)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_SCRUBPOSEXT_SCR16 0x1U

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSTKERR1 \
 *   Stuck-bit detection register 1 (STKERR1)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_STKERR1_VA 0x80000000U

#define FTADDR_STKERR1_MU 0x40000000U

#define FTADDR_STKERR1_STKERR_CSEL_SHIFT 22
#define FTADDR_STKERR1_STKERR_CSEL_MASK  0x1c00000U
#define FTADDR_STKERR1_STKERR_CSEL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_STKERR1_STKERR_CSEL_MASK ) >> \
    FTADDR_STKERR1_STKERR_CSEL_SHIFT )
#define FTADDR_STKERR1_STKERR_CSEL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_STKERR1_STKERR_CSEL_MASK ) |    \
    ( ( ( _val ) << FTADDR_STKERR1_STKERR_CSEL_SHIFT ) & \
      FTADDR_STKERR1_STKERR_CSEL_MASK ) )
#define FTADDR_STKERR1_STKERR_CSEL( _val )             \
  ( ( ( _val ) << FTADDR_STKERR1_STKERR_CSEL_SHIFT ) & \
    FTADDR_STKERR1_STKERR_CSEL_MASK )

#define FTADDR_STKERR1_STKERR_BANK_SHIFT 19
#define FTADDR_STKERR1_STKERR_BANK_MASK  0x380000U
#define FTADDR_STKERR1_STKERR_BANK_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_STKERR1_STKERR_BANK_MASK ) >> \
    FTADDR_STKERR1_STKERR_BANK_SHIFT )
#define FTADDR_STKERR1_STKERR_BANK_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_STKERR1_STKERR_BANK_MASK ) |    \
    ( ( ( _val ) << FTADDR_STKERR1_STKERR_BANK_SHIFT ) & \
      FTADDR_STKERR1_STKERR_BANK_MASK ) )
#define FTADDR_STKERR1_STKERR_BANK( _val )             \
  ( ( ( _val ) << FTADDR_STKERR1_STKERR_BANK_SHIFT ) & \
    FTADDR_STKERR1_STKERR_BANK_MASK )

#define FTADDR_STKERR1_STKERR_ROW_SHIFT 2
#define FTADDR_STKERR1_STKERR_ROW_MASK  0x7fffcU
#define FTADDR_STKERR1_STKERR_ROW_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_STKERR1_STKERR_ROW_MASK ) >> \
    FTADDR_STKERR1_STKERR_ROW_SHIFT )
#define FTADDR_STKERR1_STKERR_ROW_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_STKERR1_STKERR_ROW_MASK ) |    \
    ( ( ( _val ) << FTADDR_STKERR1_STKERR_ROW_SHIFT ) & \
      FTADDR_STKERR1_STKERR_ROW_MASK ) )
#define FTADDR_STKERR1_STKERR_ROW( _val )             \
  ( ( ( _val ) << FTADDR_STKERR1_STKERR_ROW_SHIFT ) & \
    FTADDR_STKERR1_STKERR_ROW_MASK )

/** @} */

/**
 * @defgroup RTEMSDeviceGRLIBFTADDRSTKERR2 \
 *   Stuck-bit detection register 2 (STKERR2)
 *
 * @brief This group contains register bit definitions.
 *
 * @{
 */

#define FTADDR_STKERR2_STKERR_COL_SHIFT 0
#define FTADDR_STKERR2_STKERR_COL_MASK  0x1ffU
#define FTADDR_STKERR2_STKERR_COL_GET( _reg )        \
  ( ( ( _reg ) & FTADDR_STKERR2_STKERR_COL_MASK ) >> \
    FTADDR_STKERR2_STKERR_COL_SHIFT )
#define FTADDR_STKERR2_STKERR_COL_SET( _reg, _val )     \
  ( ( ( _reg ) & ~FTADDR_STKERR2_STKERR_COL_MASK ) |    \
    ( ( ( _val ) << FTADDR_STKERR2_STKERR_COL_SHIFT ) & \
      FTADDR_STKERR2_STKERR_COL_MASK ) )
#define FTADDR_STKERR2_STKERR_COL( _val )             \
  ( ( ( _val ) << FTADDR_STKERR2_STKERR_COL_SHIFT ) & \
    FTADDR_STKERR2_STKERR_COL_MASK )

/** @} */

/**
 * @brief This structure defines the FTADDR register block memory map.
 */
typedef struct ftaddr {
  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRFSR.
   */
  uint32_t fsr;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRBSR.
   */
  uint32_t bsr;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRMCFG1.
   */
  uint32_t mcfg1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRMCFG2.
   */
  uint32_t mcfg2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRMCFG3.
   */
  uint32_t mcfg3;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRMCFG4.
   */
  uint32_t mcfg4;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCFG1.
   */
  uint32_t scfg1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCFG2.
   */
  uint32_t scfg2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPHYTIM1.
   */
  uint32_t phytim1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPHYTIM2.
   */
  uint32_t phytim2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRDACTRL1.
   */
  uint32_t dactrl1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRDACTRL2.
   */
  uint32_t dactrl2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRDASTAT.
   */
  uint32_t dastat;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRDACB.
   */
  uint32_t dacb;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRDADATA1.
   */
  uint32_t dadata1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRDADATA2.
   */
  uint32_t dadata2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRODTCFG.
   */
  uint32_t odtcfg[ 8 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRODTDEF.
   */
  uint32_t odtdef;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRODTIRTIM.
   */
  uint32_t odtirtim;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRCMD.
   */
  uint32_t cmd;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSREFCFG.
   */
  uint32_t srefcfg;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRBEECNT.
   */
  uint32_t beecnt;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSTCNT.
   */
  uint32_t stcnt;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPHYIADDR.
   */
  uint32_t phyiaddr;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPHYIDATA.
   */
  uint32_t phyidata;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPHYGCTRL.
   */
  uint32_t phygctrl;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRTTCNT.
   */
  uint32_t ttcnt;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRBEFECNT.
   */
  uint32_t befecnt;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRTSCFG1.
   */
  uint32_t tscfg1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRTSCFG.
   */
  uint32_t tscfg[ 4 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPARCFG.
   */
  uint32_t parcfg;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRMRDEF.
   */
  uint32_t mrdef[ 7 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRARBNUDGE.
   */
  uint32_t arbnudge;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPHYTIM3.
   */
  uint32_t phytim3;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRODTERTIM.
   */
  uint32_t odtertim;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRODTEWTIM.
   */
  uint32_t odtewtim;

  uint32_t reserved_d0_200[ 76 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRAHBADEC.
   */
  uint32_t ahbadec;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRAHBACFG.
   */
  uint32_t ahbacfg;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPFCFG.
   */
  uint32_t pfcfg;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCRUBCFG1.
   */
  uint32_t scrubcfg1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCRUBCFG2.
   */
  uint32_t scrubcfg2;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRIRQPEND.
   */
  uint32_t irqpend;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRIRQEN.
   */
  uint32_t irqen;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCRUBUE.
   */
  uint32_t scrubue;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRBLECNT.
   */
  uint32_t blecnt[ 3 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRBLECNT4.
   */
  uint32_t blecnt4;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRAECNT1.
   */
  uint32_t aecnt1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRAECNT2.
   */
  uint32_t aecnt2[ 2 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRAECNT4.
   */
  uint32_t aecnt4;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRAHBERRLOC.
   */
  uint32_t ahberrloc[ 8 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPFSTAT.
   */
  uint32_t pfstat[ 4 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPFBSTAT.
   */
  uint32_t pfbstat;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCRUBSTART.
   */
  uint32_t scrubstart;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCRUBEND.
   */
  uint32_t scrubend;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRFEFECNT.
   */
  uint32_t fefecnt;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRINITPAT.
   */
  uint32_t initpat[ 4 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCRUBPOS.
   */
  uint32_t scrubpos[ 4 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRAHBERRLOCHI.
   */
  uint32_t ahberrlochi[ 8 ];

  uint32_t reserved_2c0_300[ 16 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCRUBUEEXT.
   */
  uint32_t scrubueext;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRAECNT2EXT.
   */
  uint32_t aecnt2ext;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRPFSTATEXT.
   */
  uint32_t pfstatext[ 4 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCRUBSTARTEXT.
   */
  uint32_t scrubstartext;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCRUBENDEXT.
   */
  uint32_t scrubendext;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSCRUBPOSEXT.
   */
  uint32_t scrubposext[ 4 ];

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSTKERR1.
   */
  uint32_t stkerr1;

  /**
   * @brief See @ref RTEMSDeviceGRLIBFTADDRSTKERR2.
   */
  uint32_t stkerr2;
} ftaddr;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GRLIB_FTADDR_REGS_H */
