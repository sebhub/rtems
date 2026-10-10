/**
 * @file
 *
 * @ingroup RISCV_IRQ
 *
 * @brief Interrupt definitions.
 */

/*
 * Copyright (c) 2018 embedded brains GmbH & Co. KG
 *
 * Copyright (c) 2015 University of York.
 * Hesham Almatary <hesham@alumni.york.ac.uk>
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef LIBBSP_RISCV_GRISCV_IRQ_H
#define LIBBSP_RISCV_GRISCV_IRQ_H

#ifndef ASM

#include <bsp.h>

/*
 * The interrupt vector number is the interrupt line of the IRQ(A)MP.  The
 * interrupt dispatch supports the standard interrupt lines 1 to 15.
 */
#define BSP_INTERRUPT_VECTOR_MAX_STD 15

#define BSP_INTERRUPT_VECTOR_COUNT (BSP_INTERRUPT_VECTOR_MAX_STD + 1)

#define BSP_INTERRUPT_CUSTOM_VALID_VECTOR

#define RISCV_INTERRUPT_VECTOR_EXTERNAL(x) (x)

#endif /* ASM */

#endif /* LIBBSP_RISCV_GRISCV_IRQ_H */
