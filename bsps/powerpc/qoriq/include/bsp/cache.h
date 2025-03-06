/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSBSPsPowerPCQorIQ
 *
 * @brief This header file provides L1 and L2 cache support functions.
 */

/*
 * Copyright (C) 2025 embedded brains GmbH & Co. KG
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

#ifndef LIBBSP_POWERPC_QORIQ_CACHE_H
#define LIBBSP_POWERPC_QORIQ_CACHE_H

#include <libcpu/powerpc-utility.h>

#define L2CTL        0x000
#define L2CWAP       0x004
#define L2CEWAR0     0x010
#define L2CEWAREA0   0x014
#define L2CEWCR0     0x018
#define L2CEWAR1     0x020
#define L2CEWAREA1   0x024
#define L2CEWCR1     0x028
#define L2CEWAR2     0x030
#define L2CEWAREA2   0x034
#define L2CEWCR2     0x038
#define L2CEWAR3     0x040
#define L2CEWAREA3   0x044
#define L2CEWCR3     0x048
#define L2SRBAR0     0x100
#define L2SRBAREA0   0x104
#define L2SRBAR1     0x108
#define L2SRBAREA1   0x10c
#define L2ERRINJHI   0xe00
#define L2ERRINJLO   0xe04
#define L2ERRINJCTL  0xe08
#define L2CAPTDATAHI 0xe20
#define L2CAPTDATALO 0xe24
#define L2CAPTECC    0xe28
#define L2ERRDET     0xe40
#define L2ERRDIS     0xe44
#define L2ERRINTEN   0xe48
#define L2ERRATTR    0xe4c
#define L2ERRADDRL   0xe50
#define L2ERRADDRH   0xe54
#define L2ERRCTL     0xe58

#define L2CTL_L2E 0x80000000
#define L2CTL_L2I 0x40000000
#define L2CTL_L2SIZ_512 0x20000000
#define L2CTL_L2DO 0x400000
#define L2CTL_L2IO 0x200000
#define L2CTL_L2INTDIS 0x80000
#define L2CTL_L2SRAM_MASK 0x70000
#define L2CTL_L2LO 0x2000
#define L2CTL_L2SLC 0x1000
#define L2CTL_L2LFR 0x400
#define L2CTL_L2LFRID_MASK 0x300
#define L2CTL_L2STASHDIS 0x8
#define L2CTL_L2STASHCTL_MASK 0x3

#define L2ERRDIS_MULL2ERR 0x80000000

#define L2_TPARERR 0x10
#define L2_MBECCERR 0x8
#define L2_SBECCERR 0x4
#define L2_L2CFGERR 0x1

#define L2ERRINJCTL_TERRIEN 0x10000
#define L2ERRINJCTL_ECCMB 0x200
#define L2ERRINJCTL_DERRIEN 0x100
#define L2ERRINJCTL_ECCERRIM 0xff

#ifdef ASM

.macro INITIALIZE_L2_CACHE base, reg_0, reg_1
	LA	\base, qoriq + 0x20000

	/* Enable, invalidate, and configure */
	lwz	\reg_0, L2CTL(\base)
	LWI	\reg_1, L2CTL_L2SRAM_MASK | L2CTL_L2DO | L2CTL_L2IO
	andc	\reg_0, \reg_0, \reg_1
	oris	\reg_0, \reg_0, (L2CTL_L2E | L2CTL_L2I) >> 16
	msync
	isync
	stw	\reg_0, L2CTL(\base)
	lwz	\reg_0, L2CTL(\base)
	msync

	/* Wait for cleared L2CTL_L2I */
1:
	lwz	\reg_0, L2CTL(\base)
	andis.	\reg_0, \reg_0, L2CTL_L2I >> 16
	bne	1b

	/* Make sure errors are enabled */
	lwz	\reg_0, L2ERRDIS(\base)
	LWI	\reg_1, L2ERRDIS_MULL2ERR | L2_TPARERR | \
		L2_MBECCERR | L2_SBECCERR | L2_L2CFGERR
	andc	\reg_0, \reg_0, \reg_1
	stw	\reg_0, L2ERRDIS(\base)

	/* Enable error interrupts */
	lwz	\reg_0, L2ERRINTEN(\base)
	ori	\reg_0, \reg_0, L2_TPARERR | L2_MBECCERR | L2_SBECCERR
	stw	\reg_0, L2ERRINTEN(\base)

	/* Initialize SBE threshold */
	lis	\reg_0, 255
	stw	\reg_0, L2ERRCTL(\base)
.endm

.macro INITIALIZE_L1_DATA_CACHE reg_0
	/* Invalidate and configure the L1 data cache */
	LWI	\reg_0, FSL_EIS_L1CSR0_CPE | FSL_EIS_L1CSR0_CE | \
		FSL_EIS_L1CSR0_CFI | FSL_EIS_L1CSR0_CLFR
	msync
	isync
	mtspr	FSL_EIS_L1CSR0, \reg_0
	isync

	/* Wait for cleared FSL_EIS_L1CSR0_CFI and FSL_EIS_L1CSR0_CLFR */
1:
	mfspr	\reg_0, FSL_EIS_L1CSR0
	andi.	\reg_0, \reg_0, FSL_EIS_L1CSR0_CFI | FSL_EIS_L1CSR0_CLFR
	bne	1b
.endm

.macro INITIALIZE_L1_INSTRUCTION_CACHE reg_0
	/* Invalidate and configure the L1 instruction cache */
	LWI	\reg_0, FSL_EIS_L1CSR1_ICPE | FSL_EIS_L1CSR1_ICE | \
		FSL_EIS_L1CSR1_ICFI | FSL_EIS_L1CSR1_ICLFR
	msync
	isync
	mtspr	FSL_EIS_L1CSR1, \reg_0
	isync

	/* Wait for cleared FSL_EIS_L1CSR1_ICFI and FSL_EIS_L1CSR1_ICLFR */
1:
	mfspr	\reg_0, FSL_EIS_L1CSR1
	andi.	\reg_0, \reg_0, FSL_EIS_L1CSR1_ICFI | FSL_EIS_L1CSR1_ICLFR
	bne	1b
.endm

#endif /* ASM */

#endif /* LIBBSP_POWERPC_QORIQ_CACHE_H */
