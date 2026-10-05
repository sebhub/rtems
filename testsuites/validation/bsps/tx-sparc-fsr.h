/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSTestSuitesValidation
 *
 * @brief This header file provides the implemented bits of the SPARC FSR.
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

#ifndef _TX_SPARC_FSR_H
#define _TX_SPARC_FSR_H

#include <rtems/score/sparc.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup RTEMSTestSuitesValidation
 *
 * @{
 */

#if SPARC_HAS_FPU == 1
/**
 * @brief Gets the mask of the FSR bits which the floating-point unit
 *   implements.
 *
 * The floating-point unit of the ERC32 implements no NS bit.  The bit reads
 * as zero.  The function writes the NS bit to the FSR and reads it back.  It
 * restores the FSR afterwards.
 *
 * @return Returns the mask of the FSR bits which the floating-point unit
 *   implements.
 */
static inline uint32_t GetFSRImplemented( void )
{
  uint32_t fsr;
  uint32_t probe;

  /* clang-format off */
  __asm__ volatile ( "st %%fsr, %0" : "=m" ( fsr ) );
  probe = fsr | SPARC_FSR_NS_MASK;
  __asm__ volatile ( "ld %0, %%fsr\nnop\nnop\nnop" : : "m" ( probe ) );
  __asm__ volatile ( "st %%fsr, %0" : "=m" ( probe ) );
  __asm__ volatile ( "ld %0, %%fsr\nnop\nnop\nnop" : : "m" ( fsr ) );
  /* clang-format on */

  if ( ( probe & SPARC_FSR_NS_MASK ) != 0 ) {
    return UINT32_MAX;
  }

  return ~(uint32_t) SPARC_FSR_NS_MASK;
}
#endif

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _TX_SPARC_FSR_H */
