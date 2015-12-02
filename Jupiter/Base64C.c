/**
 * Copyright (C) 2014-2015 Jessica James.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Written by Jessica James <jessica.aj@outlook.com>
 */

#include "Base64.h"

unsigned int Jupiter_base64encode(const void *data_buf, size_t dataLength, char *result)
{
	const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	const uint8_t *data = (const uint8_t *)data_buf;
	char *resultPtr = result;
	uint32_t n = 0;
	int padCount = dataLength % 3;
	uint8_t n0, n1, n2, n3;

	/* increment over the length of the string, three characters at a time */
	for (size_t x = 0; x < dataLength; x += 3)
	{
		/* these three 8-bit (ASCII) characters become one 24-bit number */
		n = data[x] << 16;

		if ((x + 1) < dataLength) n += data[x + 1] << 8;
		if ((x + 2) < dataLength) n += data[x + 2];

		/* this 24-bit number gets separated into four 6-bit numbers */
		n0 = (uint8_t)(n >> 18) & 63;
		n1 = (uint8_t)(n >> 12) & 63;
		n2 = (uint8_t)(n >> 6) & 63;
		n3 = (uint8_t)n & 63;

		/*
		* if we have one byte available, then its encoding is spread
		* out over two characters
		*/
		*resultPtr = base64chars[n0];
		resultPtr++;
		*resultPtr = base64chars[n1];
		resultPtr++;

		/*
		* if we have only two bytes available, then their encoding is
		* spread out over three chars
		*/
		if ((x + 1) < dataLength)
		{
			*resultPtr = base64chars[n2];
			resultPtr++;
		}

		/*
		* if we have all three bytes available, then their encoding is spread
		* out over four characters
		*/
		if ((x + 2) < dataLength)
		{
			*resultPtr = base64chars[n3];
			resultPtr++;
		}
	}

	/*
	* create and add padding that is required if we did not have a multiple of 3
	* number of characters available
	*/
	if (padCount > 0)
	{
		while (padCount < 3)
		{
			*resultPtr = '=';
			resultPtr++;
			padCount++;
		}
	}
	*resultPtr = 0;
	return resultPtr - result;
}

static const unsigned char Jupiter_base64DecodeTable[256] = {
	66, 66, 66, 66, 66, 66, 66, 66, 66, 64, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
	66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 62, 66, 66, 66, 63, 52, 53,
	54, 55, 56, 57, 58, 59, 60, 61, 66, 66, 66, 65, 66, 66, 66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 66, 66, 66, 66, 66, 66, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 66, 66,
	66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
	66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
	66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
	66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
	66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
	66, 66, 66, 66, 66, 66
};

unsigned int Jupiter_minBase64DecodeLength(const char *data, size_t inLen)
{
	if (inLen == 0) return 0;
	data += inLen - 1;
	if (*data == '=')
	{
		data--;
		if (*data == '=') return inLen / 4 * 3 - 2;
		return inLen / 4 * 3 - 1;
	}
	return inLen / 4 * 3;
}

bool Jupiter_isBase64(const char *in)
{
	while (*in != 0)
	{
		switch (Jupiter_base64DecodeTable[*in++])
		{
		case 66:
			return false;
		default:
			break;
		}
	}
	return true;
}

bool Jupiter_isBase64_s(const char *in, size_t inLen)
{
	if (inLen % 4 != 0)
	{
		return false;
	}
	const char *end = in + inLen;
	while (in != end)
	{
		switch (Jupiter_base64DecodeTable[*in++])
		{
		case 66:
			return false;
		default:
			break;
		}
	}
	return true;
}

unsigned int Jupiter_base64decode(const char *in, unsigned char *out)
{
	unsigned char *outOrig = out;
	size_t buf = 1;

	while (*in != 0)
	{
		unsigned char c = Jupiter_base64DecodeTable[*in++];

		switch (c)
		{
		case 66:
			/* invalid input */
			return 0;
		case 65:
			/* pad character, end of data */
			goto endLoop;
		case 64:
			break;
		default:
			buf = buf << 6 | c;

			/* If the buffer is full, split it into bytes */
			if (buf & 0x1000000)
			{
				*out++ = (unsigned char) (buf >> 16);
				*out++ = (unsigned char) (buf >> 8);
				*out++ = (unsigned char) buf;
				buf = 1;
			}
			break;
		}
	}
endLoop:

	if (buf & 0x40000)
	{
		*out++ = (unsigned char) (buf >> 10);
		*out++ = (unsigned char) (buf >> 2);
	}
	else if (buf & 0x1000) *out++ = (unsigned char) (buf >> 4);

	return out - outOrig;
}

unsigned int Jupiter_base64decode_s(const char *in, size_t inLen, unsigned char *out)
{
	unsigned char *outOrig = out;
	const char *end = in + inLen;
	size_t buf = 1;

	while (in != end)
	{
		unsigned char c = Jupiter_base64DecodeTable[*in++];

		switch (c)
		{
		case 66:
			/* invalid input */
			return false;
		case 65:
			/* pad character, end of data */
			in = end;
		case 64:
			break;
		default:
			buf = buf << 6 | c;

			/* If the buffer is full, split it into bytes */
			if (buf & 0x1000000)
			{
				*out++ = (unsigned char) (buf >> 16);
				*out++ = (unsigned char) (buf >> 8);
				*out++ = (unsigned char) buf;
				buf = 1;
			}
			break;
		}
	}

	if (buf & 0x40000)
	{
		*out++ = (unsigned char) (buf >> 10);
		*out++ = (unsigned char) (buf >> 2);
	}
	else if (buf & 0x1000)
		*out++ = (unsigned char) (buf >> 4);

	return out - outOrig;
}
