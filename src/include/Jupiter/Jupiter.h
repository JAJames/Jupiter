/**
 * Copyright (C) 2013-2016 Jessica James.
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

#if !defined JUPITER_API

/**
 * @file Jupiter.h
 * @brief Defines JUPITER_API and provides metadata about the program.
 * On anything other than Windows, JUPITER_API is defined as nothing, to prevent compiler errors.
 */

#if defined _WIN32

#if defined JUPITER_EXPORTS
#define JUPITER_API __declspec(dllexport) 
#else // JUPITER_EXPORTS
#define JUPITER_API __declspec(dllimport) 
#endif // JUPITER_EXPORTS

#else // _WIN32
#define JUPITER_API
#endif // _WIN32

#if !defined JUPITER_PLATFORM
#define JUPITER_PLATFORM
#endif

#define JUPITER_VERSION "Jupiter 1.0.1 (" JUPITER_PLATFORM __DATE__ ")" /** Version of this program at compile time. */

#if defined __cplusplus
extern "C"
{
#endif // __cplusplus

	JUPITER_API extern const char *Jupiter_version; /** Jupiter version number at runtime */
	JUPITER_API extern const char *Jupiter_copyright; /** Jupiter copyright notice */

#if defined __cplusplus
}

namespace Jupiter
{
	JUPITER_API extern const char *version; /** @see Jupiter_version */
	JUPITER_API extern const char *copyright; /** @see Jupiter_copyright */
}

#endif // __cplusplus

#endif // JUPITER_API