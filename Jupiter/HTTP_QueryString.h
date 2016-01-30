/**
 * Copyright (C) 2016 Jessica James.
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

#if !defined _HTTP_QUERYSTRING_H_HEADER
#define _HTTP_QUERYSTRING_H_HEADER

#include "String.h"

/**
 * @file HTTP_QueryString.h
 * @brief Provides parsing for HTTP Query Strings.
 */

namespace Jupiter
{
	namespace HTTP
	{
		/**
		* @brief Provides parsing for HTTP Query Strings.
		*/
		class QueryString : public Jupiter::StringS
		{
		public:
			QueryString() = delete;
			inline QueryString(const Jupiter::ReadableString &parameters) : QueryString(parameters.ptr(), parameters.size()) {}
			inline QueryString(const char *ptr, size_t str_size);
		};
	}
}

/** Implementation */
inline Jupiter::HTTP::QueryString::QueryString(const char *ptr, size_t str_size) : Jupiter::StringS(str_size)
{
	if (str_size < 3) // not enough room for "%XX", therefore no parameters to parse
	{
		Jupiter::StringType::length = str_size;
		switch (str_size)
		{
		case 2:
			str[1] = ptr[1];
		case 1:
			*str = *ptr;
		case 0:
			// nothing to copy
			return;
		}
	}

	const char *end = ptr + str_size - 2;
	char *buf = str;
	int val;

	while (ptr != end)
	{
		if (*ptr == '%')
		{
			if ((val = Jupiter_getHex(*++ptr)) != -1)
			{
				*buf = static_cast<uint8_t>(val) << 4;
				if ((val = Jupiter_getHex(*++ptr)) != -1)
					*buf |= val;

				++buf, ++ptr;
				if (ptr > end)
				{
					if (ptr == end + 1) // copy 1 remaining character
					{
						*buf = *ptr;
						++buf;
					}
					Jupiter::StringType::length = buf - Jupiter::StringType::str;
					return;
				}
			}
		}
		else // Copy character
		{
			*buf = *ptr;
			++buf, ++ptr;
		}
	}

	// copy last 2 characters
	*buf = *ptr;
	*++buf = *++ptr;
	Jupiter::StringType::length = buf + 1 - str;
}

#endif // _HTTP_QUERYSTRING_H_HEADER