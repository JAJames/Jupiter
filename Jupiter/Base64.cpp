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
#include "Functions.h"

unsigned int Jupiter::base64encode(const void *data, size_t dataLength, char *result)
{
	return Jupiter_base64encode(data, dataLength, result);
}

unsigned int Jupiter::base64encode(const void *data, size_t dataLength, char *result, size_t resultSize)
{
	if (resultSize < (dataLength / 3) * 4 + 5) return 0;
	return Jupiter::base64encode(data, dataLength, result);
}

char *Jupiter::base64encode(const void *data, size_t dataLength)
{
	size_t resultSize = (dataLength / 3) * 4 + 5;
	char *result = new char[resultSize];
	Jupiter::base64encode(data, dataLength, result, resultSize);
	return result;
}

char *Jupiter::base64encode(const char *str)
{
	return Jupiter::base64encode(str, strlen(str));
}

unsigned int Jupiter::base64decode(const char *data, size_t dataLength, unsigned char *result)
{
	return Jupiter_base64decode_s(data, dataLength, result);
}

unsigned int Jupiter::base64decode(const char *data, size_t dataLength, unsigned char *result, size_t resultSize)
{
	if (resultSize < Jupiter_minBase64DecodeLength(data, dataLength)) return 0;
	return Jupiter::base64decode(data, dataLength, result);
}

char *Jupiter::base64decode(const char *data, size_t dataLength)
{
	char *result = new char[Jupiter_minBase64DecodeLength(data, dataLength) + 1];
	result[Jupiter_base64decode(data, (unsigned char *)result)] = 0;
	return result;
}

char *Jupiter::base64decode(const char *data)
{
	return Jupiter::base64decode(data, strlen(data));
}
