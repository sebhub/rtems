/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSScoreCPUARM
 *
 * @brief This source file contains the ARMv7-M implementation of
 *   _CPU_Exception_resume().
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/cpuimpl.h>

#ifdef ARM_MULTILIB_ARCH_V7M

/*
 * The function writes a hardware exception frame of the type which the
 * EXC_RETURN value of the exception frame selects.  It places the hardware
 * frame below the stack pointer of the exception frame, on the stack which the
 * EXC_RETURN value selects.  The exception return then restores the mode, the
 * stack pointer and the registers of the hardware frame.
 *
 * On a return to PSP, the main stack pointer takes the address above the
 * exception frame.  The default exception handler reserved the exception
 * frame at that address.  On a return to MSP, the hardware frame lies above
 * the exception frame.
 */
void __attribute__((naked, no_profile_instrument_function))
_CPU_Exception_resume( const CPU_Exception_frame *frame )
{
  (void) frame;

  __asm__ volatile (
#ifdef ARM_MULTILIB_VFP
    "ldr r1, [r0, %[vfpoff]]\n"
    "cbz r1, 1f\n"
    "ldr r2, [r1, %[fpscroff]]\n"
    "vmsr FPSCR, r2\n"
    "add r1, %[d0off]\n"
    "vldmia r1, {d0-d15}\n"
    "1:\n"
#endif

    "ldr r1, [r0, %[excretoff]]\n"
    "ldr r2, [r0, %[spoff]]\n"
    "ldr r3, [r0, %[xpsroff]]\n"

    /* A stack pointer which is not 8-byte aligned sets the xPSR bit 9 */
    "tst r2, #4\n"
    "ite ne\n"
    "orrne r3, r3, #512\n"
    "biceq r3, r3, #512\n"
    "bic r2, r2, #7\n"

#ifdef ARM_MULTILIB_VFP
    "tst r1, #16\n"
    "ite eq\n"
    "subeq r2, r2, %[hwextsz]\n"
    "subne r2, r2, %[hwsz]\n"
#else
    "sub r2, r2, %[hwsz]\n"
#endif

    /* Write r0 to r3, r12, lr, pc and xPSR of the hardware frame */
    "ldm r0, {r4-r7}\n"
    "stm r2, {r4-r7}\n"
    "ldr r4, [r0, %[r12off]]\n"
    "ldr r5, [r0, %[lroff]]\n"
    "ldr r6, [r0, %[pcoff]]\n"
    "bic r6, r6, #1\n"
    "str r4, [r2, #16]\n"
    "str r5, [r2, #20]\n"
    "str r6, [r2, #24]\n"
    "str r3, [r2, #28]\n"

#ifdef ARM_MULTILIB_VFP
    /* An extended frame receives s0 to s15 and FPSCR */
    "tst r1, #16\n"
    "bne 2f\n"
    "add r4, r2, #32\n"
    "vstmia r4!, {s0-s15}\n"
    "vmrs r5, FPSCR\n"
    "str r5, [r4]\n"
    "2:\n"
#endif

    /* EXC_RETURN bit 2 selects PSP */
    "tst r1, #4\n"
    "itt ne\n"
    "msrne psp, r2\n"
    "addne r2, r0, %[framesz]\n"

    "ldr r3, [r0, %[basepriof]]\n"
    "add r4, r0, %[r4off]\n"
    "ldm r4, {r4-r11}\n"
    "msr basepri, r3\n"
    "mov sp, r2\n"
    "bx r1\n"
    :
    : [vfpoff] "i" ( offsetof( CPU_Exception_frame, vfp_context ) ),
      [fpscroff] "i" ( offsetof( ARM_VFP_context, register_fpscr ) ),
      [d0off] "i" ( offsetof( ARM_VFP_context, register_d0 ) ),
      [excretoff] "i" ( offsetof( CPU_Exception_frame, exc_return ) ),
      [spoff] "i" ( offsetof( CPU_Exception_frame, register_sp ) ),
      [xpsroff] "i" ( offsetof( CPU_Exception_frame, register_xpsr ) ),
      [hwsz] "i" ( 32 ),
      [hwextsz] "i" ( 104 ),
      [r12off] "i" ( offsetof( CPU_Exception_frame, register_r12 ) ),
      [lroff] "i" ( offsetof( CPU_Exception_frame, register_lr ) ),
      [pcoff] "i" ( offsetof( CPU_Exception_frame, register_pc ) ),
      [framesz] "i" ( sizeof( CPU_Exception_frame ) ),
      [basepriof] "i" ( offsetof( CPU_Exception_frame, register_basepri ) ),
      [r4off] "i" ( offsetof( CPU_Exception_frame, register_r4 ) )
  );
}

#endif /* ARM_MULTILIB_ARCH_V7M */
