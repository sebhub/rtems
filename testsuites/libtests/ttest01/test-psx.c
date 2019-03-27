#include <t.h>

#include <sys/stat.h>
#include <errno.h>

T_TEST_CASE(stat)
{
	struct stat st;
	int rv;

	errno = 0;
	rv = stat("foobar", &st);
	T_psx_error(rv, ENOENT);
}

T_TEST_CASE(test_psx_error)
{
	errno = ENOMEM;
	T_psx_error(-1, ENOMEM);
	T_psx_error(-1, ERANGE);
	T_psx_error(0, ENOMEM);
	T_quiet_psx_error(-1, ENOMEM);
	T_quiet_psx_error(-1, ERANGE);
	T_quiet_psx_error(0, ENOMEM);
	T_assert_psx_error(-1, ENOMEM);
	T_assert_psx_error(0, ENOMEM);
}

T_TEST_CASE(test_psx_success)
{
	T_psx_success(0);
	T_psx_success(-1);
	T_quiet_psx_success(0);
	T_quiet_psx_success(-1);
	T_assert_psx_success(0);
	T_assert_psx_success(-1);
}

/*
 * SPDX-License-Identifier: BSD-2-Clause
 * SPDX-License-Identifier: CC-BY-SA-4.0
 *
 * Copyright (C) 2018, 2019 embedded brains GmbH
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
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International Public License as
 * published by Creative Commons, PO Box 1866, Mountain View, CA 94042
 * (https://creativecommons.org/licenses/by-sa/4.0/legalcode).
 */
