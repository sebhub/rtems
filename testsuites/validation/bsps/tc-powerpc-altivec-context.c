/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreCpuPowerpcValAltivecContext
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

#include <stdint.h>
#include <string.h>
#include <rtems/score/cpuimpl.h>
#include <rtems/score/thread.h>

#include "tx-register-check.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreCpuPowerpcValAltivecContext \
 *   spec:/score/cpu/powerpc/val/altivec-context
 *
 * @ingroup TestsuitesBspsValidationBspNoClock0
 *
 * @brief Checks that the context switch saves and restores the AltiVec state
 *   of a thread.
 *
 * This test case performs the following actions:
 *
 * - Load a distinct pattern into every call-saved vector register, into VSCR
 *   and into VRSAVE. Call the context switch directly. The heir records its
 *   VSCR and its VRSAVE and switches back. Trap, so the frame reports every
 *   register. Run this once for each value with the pattern of that value
 *   inverted, then once unchanged.
 *
 *   - Check that the switch back restored the PowerPC vector register V20.
 *
 *   - Check that the switch back restored the PowerPC vector register V21.
 *
 *   - Check that the switch back restored the PowerPC vector register V22.
 *
 *   - Check that the switch back restored the PowerPC vector register V23.
 *
 *   - Check that the switch back restored the PowerPC vector register V24.
 *
 *   - Check that the switch back restored the PowerPC vector register V25.
 *
 *   - Check that the switch back restored the PowerPC vector register V26.
 *
 *   - Check that the switch back restored the PowerPC vector register V27.
 *
 *   - Check that the switch back restored the PowerPC vector register V28.
 *
 *   - Check that the switch back restored the PowerPC vector register V29.
 *
 *   - Check that the switch back restored the PowerPC vector register V30.
 *
 *   - Check that the switch back restored the PowerPC vector register V31.
 *
 *   - Check that the switch saved the PowerPC vector register V20.
 *
 *   - Check that the switch saved the PowerPC vector register V21.
 *
 *   - Check that the switch saved the PowerPC vector register V22.
 *
 *   - Check that the switch saved the PowerPC vector register V23.
 *
 *   - Check that the switch saved the PowerPC vector register V24.
 *
 *   - Check that the switch saved the PowerPC vector register V25.
 *
 *   - Check that the switch saved the PowerPC vector register V26.
 *
 *   - Check that the switch saved the PowerPC vector register V27.
 *
 *   - Check that the switch saved the PowerPC vector register V28.
 *
 *   - Check that the switch saved the PowerPC vector register V29.
 *
 *   - Check that the switch saved the PowerPC vector register V30.
 *
 *   - Check that the switch saved the PowerPC vector register V31.
 *
 *   - Check that the switch saved VSCR.
 *
 *   - Check that the switch back restored VSCR.
 *
 *   - Check that the heir received the VSCR of its context.
 *
 *   - Check that the switch saved VRSAVE.
 *
 *   - Check that the switch back restored VRSAVE.
 *
 *   - Check that the heir received the VRSAVE of its context.
 *
 *   - Check that the trap after the switch back reached the fatal handler
 *     once.
 *
 *   - Check that each run with a changed value flagged exactly the checks of
 *     that value, and that each run recorded every check.
 *
 * @{
 */

#if defined( PPC_MULTILIB_ALTIVEC )
extern const char altivec_context_done_label[];

/*
 * The low doubleword of a vector register holds the pattern of its high
 * doubleword exclusive-or this value.
 */
#define LOW UINT64_C( 0x5a5a5a5a5a5a5a5a )

/* These are NJ and SAT of VSCR. */
#define VSCR_MASK UINT64_C( 0x00010001 )

/*
 * These are the bits of VRSAVE for v0 to v19.  The bits for v20 to v31 stay
 * set, so the context switch saves these registers.
 */
#define VRSAVE_MASK UINT64_C( 0xfffff000 )

#define WORD UINT64_C( 0xffffffff )

typedef enum {
  CTX_SOURCE_V20,
  CTX_SOURCE_V21,
  CTX_SOURCE_V22,
  CTX_SOURCE_V23,
  CTX_SOURCE_V24,
  CTX_SOURCE_V25,
  CTX_SOURCE_V26,
  CTX_SOURCE_V27,
  CTX_SOURCE_V28,
  CTX_SOURCE_V29,
  CTX_SOURCE_V30,
  CTX_SOURCE_V31,
  CTX_SOURCE_VSCR,
  CTX_SOURCE_HEIR_VSCR,
  CTX_SOURCE_VRSAVE,
  CTX_SOURCE_HEIR_VRSAVE,
  CTX_SOURCE_TRAP,
  CTX_SOURCE_MAX
} CtxSource;

typedef enum {
  CTX_SLOT_FRAME_V20_HI,
  CTX_SLOT_FRAME_V20_LO,
  CTX_SLOT_FRAME_V21_HI,
  CTX_SLOT_FRAME_V21_LO,
  CTX_SLOT_FRAME_V22_HI,
  CTX_SLOT_FRAME_V22_LO,
  CTX_SLOT_FRAME_V23_HI,
  CTX_SLOT_FRAME_V23_LO,
  CTX_SLOT_FRAME_V24_HI,
  CTX_SLOT_FRAME_V24_LO,
  CTX_SLOT_FRAME_V25_HI,
  CTX_SLOT_FRAME_V25_LO,
  CTX_SLOT_FRAME_V26_HI,
  CTX_SLOT_FRAME_V26_LO,
  CTX_SLOT_FRAME_V27_HI,
  CTX_SLOT_FRAME_V27_LO,
  CTX_SLOT_FRAME_V28_HI,
  CTX_SLOT_FRAME_V28_LO,
  CTX_SLOT_FRAME_V29_HI,
  CTX_SLOT_FRAME_V29_LO,
  CTX_SLOT_FRAME_V30_HI,
  CTX_SLOT_FRAME_V30_LO,
  CTX_SLOT_FRAME_V31_HI,
  CTX_SLOT_FRAME_V31_LO,
  CTX_SLOT_SAVED_V20_HI,
  CTX_SLOT_SAVED_V20_LO,
  CTX_SLOT_SAVED_V21_HI,
  CTX_SLOT_SAVED_V21_LO,
  CTX_SLOT_SAVED_V22_HI,
  CTX_SLOT_SAVED_V22_LO,
  CTX_SLOT_SAVED_V23_HI,
  CTX_SLOT_SAVED_V23_LO,
  CTX_SLOT_SAVED_V24_HI,
  CTX_SLOT_SAVED_V24_LO,
  CTX_SLOT_SAVED_V25_HI,
  CTX_SLOT_SAVED_V25_LO,
  CTX_SLOT_SAVED_V26_HI,
  CTX_SLOT_SAVED_V26_LO,
  CTX_SLOT_SAVED_V27_HI,
  CTX_SLOT_SAVED_V27_LO,
  CTX_SLOT_SAVED_V28_HI,
  CTX_SLOT_SAVED_V28_LO,
  CTX_SLOT_SAVED_V29_HI,
  CTX_SLOT_SAVED_V29_LO,
  CTX_SLOT_SAVED_V30_HI,
  CTX_SLOT_SAVED_V30_LO,
  CTX_SLOT_SAVED_V31_HI,
  CTX_SLOT_SAVED_V31_LO,
  CTX_SLOT_SAVED_VSCR,
  CTX_SLOT_FRAME_VSCR,
  CTX_SLOT_HEIR_VSCR,
  CTX_SLOT_SAVED_VRSAVE,
  CTX_SLOT_FRAME_VRSAVE,
  CTX_SLOT_HEIR_VRSAVE,
  CTX_SLOT_TRAP,
  CTX_SLOT_MAX
} CtxSlot;

static const RegisterCheckSource altivec_context_sources[ CTX_SOURCE_MAX ] = {
  { "v20", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v21", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v22", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v23", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v24", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v25", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v26", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v27", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v28", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v29", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v30", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "v31", REGISTER_CHECK_INITIAL, UINT64_MAX },
  { "vscr", REGISTER_CHECK_INITIAL, VSCR_MASK },
  { "heir vscr", REGISTER_CHECK_INITIAL, VSCR_MASK },
  { "vrsave", REGISTER_CHECK_INITIAL, VRSAVE_MASK },
  { "heir vrsave", REGISTER_CHECK_INITIAL, VRSAVE_MASK },
  { "trap", REGISTER_CHECK_EXPECTED, WORD }
};

static const RegisterCheckSlot altivec_context_slots[ CTX_SLOT_MAX ] = {
  { "frame v20 high", CTX_SOURCE_V20, UINT64_MAX },
  { "frame v20 low", CTX_SOURCE_V20, UINT64_MAX },
  { "frame v21 high", CTX_SOURCE_V21, UINT64_MAX },
  { "frame v21 low", CTX_SOURCE_V21, UINT64_MAX },
  { "frame v22 high", CTX_SOURCE_V22, UINT64_MAX },
  { "frame v22 low", CTX_SOURCE_V22, UINT64_MAX },
  { "frame v23 high", CTX_SOURCE_V23, UINT64_MAX },
  { "frame v23 low", CTX_SOURCE_V23, UINT64_MAX },
  { "frame v24 high", CTX_SOURCE_V24, UINT64_MAX },
  { "frame v24 low", CTX_SOURCE_V24, UINT64_MAX },
  { "frame v25 high", CTX_SOURCE_V25, UINT64_MAX },
  { "frame v25 low", CTX_SOURCE_V25, UINT64_MAX },
  { "frame v26 high", CTX_SOURCE_V26, UINT64_MAX },
  { "frame v26 low", CTX_SOURCE_V26, UINT64_MAX },
  { "frame v27 high", CTX_SOURCE_V27, UINT64_MAX },
  { "frame v27 low", CTX_SOURCE_V27, UINT64_MAX },
  { "frame v28 high", CTX_SOURCE_V28, UINT64_MAX },
  { "frame v28 low", CTX_SOURCE_V28, UINT64_MAX },
  { "frame v29 high", CTX_SOURCE_V29, UINT64_MAX },
  { "frame v29 low", CTX_SOURCE_V29, UINT64_MAX },
  { "frame v30 high", CTX_SOURCE_V30, UINT64_MAX },
  { "frame v30 low", CTX_SOURCE_V30, UINT64_MAX },
  { "frame v31 high", CTX_SOURCE_V31, UINT64_MAX },
  { "frame v31 low", CTX_SOURCE_V31, UINT64_MAX },
  { "saved v20 high", CTX_SOURCE_V20, UINT64_MAX },
  { "saved v20 low", CTX_SOURCE_V20, UINT64_MAX },
  { "saved v21 high", CTX_SOURCE_V21, UINT64_MAX },
  { "saved v21 low", CTX_SOURCE_V21, UINT64_MAX },
  { "saved v22 high", CTX_SOURCE_V22, UINT64_MAX },
  { "saved v22 low", CTX_SOURCE_V22, UINT64_MAX },
  { "saved v23 high", CTX_SOURCE_V23, UINT64_MAX },
  { "saved v23 low", CTX_SOURCE_V23, UINT64_MAX },
  { "saved v24 high", CTX_SOURCE_V24, UINT64_MAX },
  { "saved v24 low", CTX_SOURCE_V24, UINT64_MAX },
  { "saved v25 high", CTX_SOURCE_V25, UINT64_MAX },
  { "saved v25 low", CTX_SOURCE_V25, UINT64_MAX },
  { "saved v26 high", CTX_SOURCE_V26, UINT64_MAX },
  { "saved v26 low", CTX_SOURCE_V26, UINT64_MAX },
  { "saved v27 high", CTX_SOURCE_V27, UINT64_MAX },
  { "saved v27 low", CTX_SOURCE_V27, UINT64_MAX },
  { "saved v28 high", CTX_SOURCE_V28, UINT64_MAX },
  { "saved v28 low", CTX_SOURCE_V28, UINT64_MAX },
  { "saved v29 high", CTX_SOURCE_V29, UINT64_MAX },
  { "saved v29 low", CTX_SOURCE_V29, UINT64_MAX },
  { "saved v30 high", CTX_SOURCE_V30, UINT64_MAX },
  { "saved v30 low", CTX_SOURCE_V30, UINT64_MAX },
  { "saved v31 high", CTX_SOURCE_V31, UINT64_MAX },
  { "saved v31 low", CTX_SOURCE_V31, UINT64_MAX },
  { "saved vscr", CTX_SOURCE_VSCR, VSCR_MASK },
  { "frame vscr", CTX_SOURCE_VSCR, VSCR_MASK },
  { "heir vscr", CTX_SOURCE_HEIR_VSCR, VSCR_MASK },
  { "saved vrsave", CTX_SOURCE_VRSAVE, VRSAVE_MASK },
  { "frame vrsave", CTX_SOURCE_VRSAVE, VRSAVE_MASK },
  { "heir vrsave", CTX_SOURCE_HEIR_VRSAVE, VRSAVE_MASK },
  { "trap", CTX_SOURCE_TRAP, WORD }
};

static const uint64_t altivec_context_patterns[ CTX_SOURCE_MAX ] = {
  UINT64_C( 0xa1a2a3a441424344 ),
  UINT64_C( 0xa2a3a4a542434445 ),
  UINT64_C( 0xa3a4a5a643444546 ),
  UINT64_C( 0xa4a5a6a744454647 ),
  UINT64_C( 0xa5a6a7a845464748 ),
  UINT64_C( 0xa6a7a8a946474849 ),
  UINT64_C( 0xa7a8a9aa4748494a ),
  UINT64_C( 0xa8a9aaab48494a4b ),
  UINT64_C( 0xa9aaabac494a4b4c ),
  UINT64_C( 0xaaabacad4a4b4c4d ),
  UINT64_C( 0xabacadae4b4c4d4e ),
  UINT64_C( 0xacadaeaf4c4d4e4f ),
  UINT64_C( 0x00010000 ),
  UINT64_C( 0x00010001 ),
  UINT64_C( 0x5a5a5fff ),
  UINT64_C( 0x3c3c3fff ),
  1
};

static uint64_t altivec_context_initial[ CTX_SOURCE_MAX ];

/* The block loads each vector register from the row of its number. */
uint64_t altivec_context_vectors[ 32 ][ 2 ] RTEMS_ALIGNED( 16 );

uint32_t altivec_context_vscr_in[ 4 ] RTEMS_ALIGNED( 16 );

uint32_t altivec_context_vrsave_in;

uint32_t altivec_context_vscr_out[ 4 ] RTEMS_ALIGNED( 16 );

static void SetVector( int reg, uint64_t value )
{
  altivec_context_vectors[ reg ][ 0 ] = value;
  altivec_context_vectors[ reg ][ 1 ] = value ^ LOW;
}

static uint32_t GetVSCR( void )
{
  __asm__ volatile( "mfvscr 0\n"
                    "stvx 0, 0, %0\n"
                    :
                    : "r"( altivec_context_vscr_out )
                    : "memory", "v0" );
  return altivec_context_vscr_out[ 3 ];
}

/*
 * Records the high doubleword of a vector register in its slot and the low
 * doubleword in the slot which follows.
 */
static void RecordVector(
  RegisterCheck *self,
  size_t         slot,
  const uint8_t *vector,
  uint64_t       expected
)
{
  uint64_t high;
  uint64_t low;

  memcpy( &high, vector, sizeof( high ) );
  memcpy( &low, vector + 8, sizeof( low ) );
  RegisterCheckRecord( self, slot, high, expected );
  RegisterCheckRecord( self, slot + 1, low ^ LOW, expected );
}

static CPU_Exception_frame altivec_context_frame;

static volatile int altivec_context_trap;

Context_Control altivec_context_executing;

Context_Control altivec_context_heir;

uint32_t altivec_context_heir_vscr[ 4 ] RTEMS_ALIGNED( 16 );

uint32_t altivec_context_heir_vrsave;

static uint32_t altivec_context_vscr;

static uint64_t altivec_context_heir_stack[ 512 ] RTEMS_ALIGNED(
  CPU_STACK_ALIGNMENT
);

/*
 * The block sets VSCR through v0, loads each call-saved vector register and
 * sets VRSAVE through r0.
 */
#define CTX_LOAD                            \
  "lis 3, altivec_context_vscr_in@ha\n"     \
  "addi 3, 3, altivec_context_vscr_in@l\n"  \
  "lvx 0, 0, 3\n"                           \
  "mtvscr 0\n"                              \
  "lis 3, altivec_context_vectors@ha\n"     \
  "addi 3, 3, altivec_context_vectors@l\n"  \
  "li 0, 320\n"                             \
  "lvx 20, 3, 0\n"                          \
  "li 0, 336\n"                             \
  "lvx 21, 3, 0\n"                          \
  "li 0, 352\n"                             \
  "lvx 22, 3, 0\n"                          \
  "li 0, 368\n"                             \
  "lvx 23, 3, 0\n"                          \
  "li 0, 384\n"                             \
  "lvx 24, 3, 0\n"                          \
  "li 0, 400\n"                             \
  "lvx 25, 3, 0\n"                          \
  "li 0, 416\n"                             \
  "lvx 26, 3, 0\n"                          \
  "li 0, 432\n"                             \
  "lvx 27, 3, 0\n"                          \
  "li 0, 448\n"                             \
  "lvx 28, 3, 0\n"                          \
  "li 0, 464\n"                             \
  "lvx 29, 3, 0\n"                          \
  "li 0, 480\n"                             \
  "lvx 30, 3, 0\n"                          \
  "li 0, 496\n"                             \
  "lvx 31, 3, 0\n"                          \
  "lis 3, altivec_context_vrsave_in@ha\n"   \
  "lwz 0, altivec_context_vrsave_in@l(3)\n" \
  "mtvrsave 0\n"

static void AltivecContextFatal(
  rtems_fatal_source source,
  rtems_fatal_code   code,
  void              *arg
)
{
  CPU_Exception_frame *frame;

  (void) arg;

  if ( source != RTEMS_FATAL_SOURCE_EXCEPTION ) {
    return;
  }

  frame = (CPU_Exception_frame *) code;
  altivec_context_frame = *frame;
  ++altivec_context_trap;
  SetFatalHandler( NULL, NULL );
  frame->VSCR = altivec_context_vscr;
  frame->EXC_SRR0 = (uintptr_t) altivec_context_done_label;
  _CPU_Exception_resume( frame );
}

/*
 * The heir records its VSCR and its VRSAVE and switches straight back.  It is
 * assembly, so no prologue saves a register of the context under test.
 */
void AltivecContextHeirEntry( void );

__asm__( "  .globl AltivecContextHeirEntry\n"
         "  .type AltivecContextHeirEntry, @function\n"
         "AltivecContextHeirEntry:\n"
         "  mfvscr 0\n"
         "  lis 5, altivec_context_heir_vscr@ha\n"
         "  addi 5, 5, altivec_context_heir_vscr@l\n"
         "  stvx 0, 0, 5\n"
         "  mfvrsave 6\n"
         "  lis 5, altivec_context_heir_vrsave@ha\n"
         "  stw 6, altivec_context_heir_vrsave@l(5)\n"
         "  lis 3, altivec_context_heir@ha\n"
         "  addi 3, 3, altivec_context_heir@l\n"
         "  lis 4, altivec_context_executing@ha\n"
         "  addi 4, 4, altivec_context_executing@l\n"
         "  b _CPU_Context_switch\n" );

static void AltivecContextRunSwitch( void )
{
  ppc_context *heir;
  int          i;

  altivec_context_trap = 0;
  memset( &altivec_context_frame, 0, sizeof( altivec_context_frame ) );
  memset( &altivec_context_executing, 0, sizeof( altivec_context_executing ) );
  memset( altivec_context_heir_vscr, 0, sizeof( altivec_context_heir_vscr ) );
  altivec_context_heir_vrsave = 0;
  ppc_get_context( &altivec_context_executing )->tp =
    ppc_get_context( &GetExecuting()->Registers )->tp;
  _CPU_Context_Initialize(
    &altivec_context_heir,
    altivec_context_heir_stack,
    sizeof( altivec_context_heir_stack ),
    0,
    AltivecContextHeirEntry,
    true,
    NULL
  );
  heir = ppc_get_context( &altivec_context_heir );
  heir->vscr = (uint32_t) altivec_context_initial[ CTX_SOURCE_HEIR_VSCR ];
  heir->vrsave = (uint32_t) altivec_context_initial[ CTX_SOURCE_HEIR_VRSAVE ];

  for ( i = 20; i < 32; ++i ) {
    SetVector( i, altivec_context_initial[ CTX_SOURCE_V20 + i - 20 ] );
  }

  altivec_context_vscr_in[ 3 ] = (uint32_t)
    altivec_context_initial[ CTX_SOURCE_VSCR ];
  altivec_context_vrsave_in = (uint32_t)
    altivec_context_initial[ CTX_SOURCE_VRSAVE ];
  altivec_context_vscr = GetVSCR();
  SetFatalHandler( AltivecContextFatal, NULL );

  __asm__ volatile( CTX_LOAD "lis 3, altivec_context_executing@ha\n"
                             "addi 3, 3, altivec_context_executing@l\n"
                             "lis 4, altivec_context_heir@ha\n"
                             "addi 4, 4, altivec_context_heir@l\n"
                             "bl _CPU_Context_switch\n"
                             "trap\n"
                             ".globl altivec_context_done_label\n"
                             "altivec_context_done_label:\n"
                    :
                    :
                    : "memory",
                      "cr0",
                      "cr1",
                      "cr2",
                      "cr3",
                      "cr4",
                      "cr5",
                      "cr6",
                      "cr7",
                      "r0",
                      "r3",
                      "r4",
                      "r5",
                      "r6",
                      "r7",
                      "r8",
                      "r9",
                      "r10",
                      "r11",
                      "r12",
                      "r14",
                      "r15",
                      "r16",
                      "r17",
                      "r18",
                      "r19",
                      "r20",
                      "r21",
                      "r22",
                      "r23",
                      "r24",
                      "r25",
                      "r26",
                      "r27",
                      "r28",
                      "r29",
                      "r30",
                      "r31",
                      "lr",
                      "ctr",
                      "xer",
                      "v0",
                      "v20",
                      "v21",
                      "v22",
                      "v23",
                      "v24",
                      "v25",
                      "v26",
                      "v27",
                      "v28",
                      "v29",
                      "v30",
                      "v31",
                      "vrsave",
                      "vscr" );
}

static void AltivecContextRun( RegisterCheck *self, void *arg )
{
  const ppc_context *ctx;

  (void) arg;
  AltivecContextRunSwitch();
  ctx = ppc_get_context( &altivec_context_executing );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V20_HI,
    altivec_context_frame.V20,
    altivec_context_patterns[ CTX_SOURCE_V20 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V21_HI,
    altivec_context_frame.V21,
    altivec_context_patterns[ CTX_SOURCE_V21 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V22_HI,
    altivec_context_frame.V22,
    altivec_context_patterns[ CTX_SOURCE_V22 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V23_HI,
    altivec_context_frame.V23,
    altivec_context_patterns[ CTX_SOURCE_V23 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V24_HI,
    altivec_context_frame.V24,
    altivec_context_patterns[ CTX_SOURCE_V24 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V25_HI,
    altivec_context_frame.V25,
    altivec_context_patterns[ CTX_SOURCE_V25 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V26_HI,
    altivec_context_frame.V26,
    altivec_context_patterns[ CTX_SOURCE_V26 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V27_HI,
    altivec_context_frame.V27,
    altivec_context_patterns[ CTX_SOURCE_V27 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V28_HI,
    altivec_context_frame.V28,
    altivec_context_patterns[ CTX_SOURCE_V28 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V29_HI,
    altivec_context_frame.V29,
    altivec_context_patterns[ CTX_SOURCE_V29 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V30_HI,
    altivec_context_frame.V30,
    altivec_context_patterns[ CTX_SOURCE_V30 ]
  );
  RecordVector(
    self,
    CTX_SLOT_FRAME_V31_HI,
    altivec_context_frame.V31,
    altivec_context_patterns[ CTX_SOURCE_V31 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V20_HI,
    ctx->v20,
    altivec_context_patterns[ CTX_SOURCE_V20 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V21_HI,
    ctx->v21,
    altivec_context_patterns[ CTX_SOURCE_V21 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V22_HI,
    ctx->v22,
    altivec_context_patterns[ CTX_SOURCE_V22 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V23_HI,
    ctx->v23,
    altivec_context_patterns[ CTX_SOURCE_V23 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V24_HI,
    ctx->v24,
    altivec_context_patterns[ CTX_SOURCE_V24 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V25_HI,
    ctx->v25,
    altivec_context_patterns[ CTX_SOURCE_V25 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V26_HI,
    ctx->v26,
    altivec_context_patterns[ CTX_SOURCE_V26 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V27_HI,
    ctx->v27,
    altivec_context_patterns[ CTX_SOURCE_V27 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V28_HI,
    ctx->v28,
    altivec_context_patterns[ CTX_SOURCE_V28 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V29_HI,
    ctx->v29,
    altivec_context_patterns[ CTX_SOURCE_V29 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V30_HI,
    ctx->v30,
    altivec_context_patterns[ CTX_SOURCE_V30 ]
  );
  RecordVector(
    self,
    CTX_SLOT_SAVED_V31_HI,
    ctx->v31,
    altivec_context_patterns[ CTX_SOURCE_V31 ]
  );
  RegisterCheckRecord(
    self,
    CTX_SLOT_SAVED_VSCR,
    ctx->vscr & VSCR_MASK,
    altivec_context_patterns[ CTX_SOURCE_VSCR ]
  );
  RegisterCheckRecord(
    self,
    CTX_SLOT_FRAME_VSCR,
    altivec_context_frame.VSCR & VSCR_MASK,
    altivec_context_patterns[ CTX_SOURCE_VSCR ]
  );
  RegisterCheckRecord(
    self,
    CTX_SLOT_HEIR_VSCR,
    altivec_context_heir_vscr[ 3 ] & VSCR_MASK,
    altivec_context_patterns[ CTX_SOURCE_HEIR_VSCR ]
  );
  RegisterCheckRecord(
    self,
    CTX_SLOT_SAVED_VRSAVE,
    ctx->vrsave & VRSAVE_MASK,
    altivec_context_patterns[ CTX_SOURCE_VRSAVE ] & VRSAVE_MASK
  );
  RegisterCheckRecord(
    self,
    CTX_SLOT_FRAME_VRSAVE,
    altivec_context_frame.VRSAVE & VRSAVE_MASK,
    altivec_context_patterns[ CTX_SOURCE_VRSAVE ] & VRSAVE_MASK
  );
  RegisterCheckRecord(
    self,
    CTX_SLOT_HEIR_VRSAVE,
    altivec_context_heir_vrsave & VRSAVE_MASK,
    altivec_context_patterns[ CTX_SOURCE_HEIR_VRSAVE ] & VRSAVE_MASK
  );
  RegisterCheckRecord(
    self,
    CTX_SLOT_TRAP,
    (uint64_t) altivec_context_trap,
    1
  );
}

static RegisterCheck altivec_context_check = {
  .sources = altivec_context_sources,
  .source_count = CTX_SOURCE_MAX,
  .slots = altivec_context_slots,
  .slot_count = CTX_SLOT_MAX,
  .initial = altivec_context_initial,
  .run = AltivecContextRun
};

static void CheckSlot( CtxSlot slot )
{
  RegisterCheckVerify( &altivec_context_check, slot );
}

static void CheckVector( CtxSlot slot )
{
  RegisterCheckVerify( &altivec_context_check, slot );
  RegisterCheckVerify( &altivec_context_check, slot + 1 );
}
#endif

/**
 * @brief Load a distinct pattern into every call-saved vector register, into
 *   VSCR and into VRSAVE. Call the context switch directly. The heir records
 *   its VSCR and its VRSAVE and switches back. Trap, so the frame reports
 *   every register. Run this once for each value with the pattern of that
 *   value inverted, then once unchanged.
 */
static void ScoreCpuPowerpcValAltivecContext_Action_0( void )
{
  #if defined( PPC_MULTILIB_ALTIVEC )
  memcpy(
    altivec_context_initial,
    altivec_context_patterns,
    sizeof( altivec_context_initial )
  );
  RegisterCheckRun( &altivec_context_check );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V20.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V20_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V21.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V21_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V22.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V22_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V23.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V23_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V24.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V24_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V25.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V25_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V26.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V26_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V27.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V27_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V28.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V28_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V29.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V29_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V30.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V30_HI );
  #endif

  /*
   * Check that the switch back restored the PowerPC vector register V31.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_FRAME_V31_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V20.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V20_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V21.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V21_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V22.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V22_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V23.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V23_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V24.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V24_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V25.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V25_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V26.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V26_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V27.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V27_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V28.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V28_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V29.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V29_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V30.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V30_HI );
  #endif

  /*
   * Check that the switch saved the PowerPC vector register V31.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckVector( CTX_SLOT_SAVED_V31_HI );
  #endif

  /*
   * Check that the switch saved VSCR.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckSlot( CTX_SLOT_SAVED_VSCR );
  #endif

  /*
   * Check that the switch back restored VSCR.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckSlot( CTX_SLOT_FRAME_VSCR );
  #endif

  /*
   * Check that the heir received the VSCR of its context.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckSlot( CTX_SLOT_HEIR_VSCR );
  #endif

  /*
   * Check that the switch saved VRSAVE.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckSlot( CTX_SLOT_SAVED_VRSAVE );
  #endif

  /*
   * Check that the switch back restored VRSAVE.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckSlot( CTX_SLOT_FRAME_VRSAVE );
  #endif

  /*
   * Check that the heir received the VRSAVE of its context.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckSlot( CTX_SLOT_HEIR_VRSAVE );
  #endif

  /*
   * Check that the trap after the switch back reached the fatal handler once.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  CheckSlot( CTX_SLOT_TRAP );
  #endif

  /*
   * Check that each run with a changed value flagged exactly the checks of
   * that value, and that each run recorded every check.
   */
  #if defined( PPC_MULTILIB_ALTIVEC )
  RegisterCheckReport( &altivec_context_check );
  #endif
}

/**
 * @fn void T_case_body_ScoreCpuPowerpcValAltivecContext( void )
 */
T_TEST_CASE( ScoreCpuPowerpcValAltivecContext )
{
  ScoreCpuPowerpcValAltivecContext_Action_0();
}

/** @} */
