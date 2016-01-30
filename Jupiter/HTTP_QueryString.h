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
		class JUPITER_API QueryString : public Jupiter::StringS
		{
		public:
			QueryString() = delete;
			QueryString(const Jupiter::ReadableString &parameters);
			QueryString(const char *ptr, size_t str_size);
		};
	}
}

#endif // _HTTP_QUERYSTRING_H_HEADER