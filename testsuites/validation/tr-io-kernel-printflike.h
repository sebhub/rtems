/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsIoValKernelPrintflike
 */

/*
 * Copyright (C) 2024 embedded brains GmbH & Co. KG
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

#ifndef _TR_IO_KERNEL_PRINTFLIKE_H
#define _TR_IO_KERNEL_PRINTFLIKE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup RtemsIoValKernelPrintflike
 *
 * @{
 */

typedef struct {
  int ( *print_no_arg )( const char * );
  int ( *print_int )( const char *, int );
  int ( *print_int_int )( const char *, int, int );
  int ( *print_long )( const char *, long );
  int ( *print_intmax )( const char *, intmax_t );
  int ( *print_string )( const char *, const char * );
  int ( *print_binary )( const char *, const unsigned char *, const char * );
  char   buf[ 128 ];
  size_t buf_index;
} KernelPrintflikeControl;

/**
 * @brief Runs the parameterized test case.
 *
 * @param control is the test runner control.
 */
void RtemsIoValKernelPrintflike_Run( KernelPrintflikeControl *control );

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _TR_IO_KERNEL_PRINTFLIKE_H */
