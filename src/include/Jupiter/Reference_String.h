/**
 * Copyright (C) 2014-2017 Jessica James.
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

#if !defined _REFERENCE_STRING_H_HEADER
#define _REFERENCE_STRING_H_HEADER

/**
 * @file Reference_String.h
 * @brief Provides the basis for String types, of any implementation.
 * Note: DEPRECATED
 */

#include <string_view>
#include "Readable_String.h"

namespace Jupiter {
	/** DEPRECATED: Generic Reference String Type */
	using ReferenceString = std::string_view;

	namespace literals {
		/** DEPRECATED: Reference_String literals */
		inline constexpr std::string_view operator"" _jrs(const char *str, size_t len) { return { str, len }; }
	}
}

/** DEPRECATED */
#define STRING_LITERAL_AS_REFERENCE(str) Jupiter::literals::operator"" _jrs(str, sizeof(str) - 1)
#define STRING_LITERAL_AS_NAMED_REFERENCE(name, str) Jupiter::ReferenceString name = STRING_LITERAL_AS_REFERENCE(str)

#endif // _REFERENCE_STRING_H_HEADER