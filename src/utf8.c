/*
 * Copyright (C) 2010-2013 nsf <no.smile.face@gmail.com>
 *                    2021 kiedtl <kiedtl@tilde.team>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

#include "utf8.h"
#include "unicode.h"

/*
 * Invalid starter bytes are marked with 0:
 *
 * The first two... cells (C0 and C1) could be used only for a 2-byte encoding
 * of a 7-bit ASCII character which should be encoded in 1 byte...  such
 * "overlong" sequences are disallowed. The red cells in the F_ row (F5 to FD)
 * indicate leading bytes of 4-byte or longer sequences that cannot be valid
 * because they would encode code points larger than the U+10FFFF limit of
 * Unicode (a limit derived from the maximum code point encodable in UTF-16).
 * FE and FF do not match any allowed character pattern and are therefore not
 * valid start bytes.            -- Wikipedia article on UTF8
 *
 */
static const uint8_t utf8_length[256] = {
     /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/* 0 */ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 1 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 2 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 3 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 4 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 5 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 6 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 7 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 8 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 9 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* A */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* B */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* C */ 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
/* D */ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
/* E */ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
/* F */ 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const uint8_t utf8_mask[6] = {
	0x7F, 0x1F, 0x0F, 0x07, 0x03, 0x01
};

ssize_t utf8_char_to_unicode(uint32_t *out, const char *c);
size_t  utf8_unicode_to_char(char *out, uint32_t c);

uint8_t
utf8_bytesz(char c)
{
	return utf8_length[(uint8_t)c];
}

ssize_t
utf8_decode(uint32_t *out, const char *c)
{
	uint8_t len = utf8_bytesz(*c);

	if (len == 0)
		return -1;

	uint32_t result = c[0] & utf8_mask[len-1];

	for (size_t i = 1; i < len; ++i) {
		if ((c[i] & 0xc0) != 0x80)
			return -1; /* not a continuation byte */
		result <<= 6;
		result |= c[i] & 0x3f;
	}

	if (result > UNICODE_MAX)
		return -1; /* value beyond unicode's 21-bit max */
	if (result >= 0xD800 && result <= 0xDFFF)
		return -1; /* surrogate chars */
	if (result >= 0xFDD0 && result <= 0xFDEF)
		return -1; /* non-character range */
	if ((result & 0xFFFE) == 0xFFFE)
		return -1; /* non-character at plane end */

	*out = result;
	return (size_t)len;
}

ssize_t
utf8_encode(char *out, uint32_t c)
{
	size_t len = 0, first, i;

	if (c < 0x80) {
		first = 0;
		len = 1;
	} else if (c < 0x800) {
		/* XXX: we allow encoding surrogate chars, even
		 * though that's invalid UTF8 */
		first = 0xc0;
		len = 2;
	} else if (c < 0x10000) {
		first = 0xe0;
		len = 3;
	} else if (c < 0x110000) {
		first = 0xf0;
		len = 4;
	} else {
		return -1;
	}

	for (i = len - 1; i > 0; --i) {
		out[i] = (c & 0x3f) | 0x80;
		c >>= 6;
	}
	out[0] = c | first;

	return len;
}
