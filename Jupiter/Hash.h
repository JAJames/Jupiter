/**
 * Copyright (C) 2015 Jessica James.
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

#if !defined _HASH_H_HEADER
#define _HASH_H_HEADER

/**
 * @file Hash.h
 * @brief Defines some hashing algorithms.
 */

#include "Jupiter.h"
#if defined __cplusplus
#include <cstdint>

namespace Jupiter
{
	/** Fowler-Noll-Vo hash algorithms */
	template<typename T = char> inline uint64_t fnv1(const T *str, size_t length);
	template<typename T = char> inline uint64_t fnv1(const Jupiter::Readable_String<T> &str);

	template<typename T = char> inline uint64_t fnv1a(const T *str, size_t length);
	template<typename T = char> inline uint64_t fnv1a(const Jupiter::Readable_String<T> &str);
}

extern "C"
{
#else
#include <stdint.h>
#endif // __cplusplus

JUPITER_API uint64_t Jupiter_fnv1(void *data, size_t length);
JUPITER_API uint64_t Jupiter_fnv1a(void *data, size_t length);

#if defined __cplusplus
}

/** fnv1 implementation */
template<typename T> inline uint64_t Jupiter::fnv1(const T *data, size_t length)
{
	return Jupiter_fnv1(data, length * sizeof(T));
}

template<typename T> inline uint64_t Jupiter::fnv1(const Jupiter::Readable_String<T> &data)
{
	return Jupiter_fnv1(data.ptr(), data.size() * sizeof(T));
}

template<typename T> inline uint64_t Jupiter::fnv1a(const T *data, size_t length)
{
	return Jupiter_fnv1a(data, length * sizeof(T));
}

template<typename T> inline uint64_t Jupiter::fnv1a(const Jupiter::Readable_String<T> &data)
{
	return Jupiter_fnv1a(data.ptr(), data.size() * sizeof(T));
}

#endif // __cplusplus

#endif // _HASH_H_HEADER