/* SPDX-License-Identifier: ISC */

/**
 * @file
 *
 * @ingroup RTEMSScoreIO
 *
 * @brief This source file contains the implementation of
 *   _IO_Base64() and _IO_Base64url().
 */

/*
 * Copyright (C) 2020, 2021 embedded brains GmbH & Co. KG
 * Copyright (C) 2004, 2005, 2007, 2009  Internet Systems Consortium, Inc. ("ISC")
 * Copyright (C) 1998-2001, 2003  Internet Software Consortium.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <rtems/score/io.h>

static void
_IO_Put(int c, void *arg, IO_Put_char put_char)
{
	(*put_char)(c, arg);
}

static int
_IO_Base64_with_encoding(IO_Put_char put_char, void *arg, const void *src,
    size_t srclen, const char *wordbreak, int wordlen, const char *encoding)
{
	unsigned int loops = 0;
	const unsigned char *in = src;
	int out = 0;

	if (wordlen < 4) {
		wordlen = 4;
	}

	while (srclen > 2) {
		_IO_Put(encoding[(in[0]>>2)&0x3f], arg, put_char);
		_IO_Put(encoding[((in[0]<<4)&0x30)|
				((in[1]>>4)&0x0f)], arg, put_char);
		_IO_Put(encoding[((in[1]<<2)&0x3c)|
				((in[2]>>6)&0x03)], arg, put_char);
		_IO_Put(encoding[in[2]&0x3f], arg, put_char);
		in += 3;
		srclen -= 3;
		out += 4;

		loops++;
		if (srclen != 0 &&
		    (int)((loops + 1) * 4) >= wordlen)
		{
			const char *w = wordbreak;
			loops = 0;
			while (*w != '\0') {
				_IO_Put(*w, arg, put_char);
				++w;
				++out;
			}
		}
	}
	if (srclen == 2) {
		_IO_Put(encoding[(in[0]>>2)&0x3f], arg, put_char);
		_IO_Put(encoding[((in[0]<<4)&0x30)|
				((in[1]>>4)&0x0f)], arg, put_char);
		_IO_Put(encoding[((in[1]<<2)&0x3c)], arg, put_char);
		_IO_Put('=', arg, put_char);
		out += 4;
	} else if (srclen == 1) {
		_IO_Put(encoding[(in[0]>>2)&0x3f], arg, put_char);
		_IO_Put(encoding[((in[0]<<4)&0x30)], arg, put_char);
		_IO_Put('=', arg, put_char);
		_IO_Put('=', arg, put_char);
		out += 4;
	}
	return out;
}

static const char base64[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

int
_IO_Base64(IO_Put_char put_char, void *arg, const void *src, size_t srclen,
    const char *wordbreak, int wordlen)
{
	return _IO_Base64_with_encoding(put_char, arg, src, srclen, wordbreak,
	    wordlen, base64);
}

static const char base64url[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";

int
_IO_Base64url(IO_Put_char put_char, void *arg, const void *src, size_t srclen,
    const char *wordbreak, int wordlen)
{
	return _IO_Base64_with_encoding(put_char, arg, src, srclen, wordbreak,
	    wordlen, base64url);
}
