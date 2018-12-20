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

#if !defined _INVALIDINDEX_H_HEADER
#define _INVALIDINDEX_H_HEADER

/**
 * @file InvalidIndex.h
 * @brief Defines some constants for invalid indexes for unsigned types.
 */

#if defined __cplusplus
#include <cstdint>
#include <climits>

namespace Jupiter
{
	static const size_t INVALID_INDEX = SIZE_MAX;
	static const uint32_t INVALID_INDEX32 = UINT32_MAX;
	static const uint64_t INVALID_INDEX64 = UINT64_MAX;

	static const unsigned int ERROR_INDICATOR = UINT_MAX;
	static const uint32_t ERROR_INDICATOR32 = INVALID_INDEX32;
	static const uint64_t ERROR_INDICATOR64 = INVALID_INDEX32;
}

extern "C"
{
#else
#include <stdint.h>
#include <limits.h>
#endif // __cplusplus

static const size_t JUPITER_INVALID_INDEX = SIZE_MAX;
static const uint32_t JUPITER_INVALID_INDEX32 = UINT32_MAX;
static const uint64_t JUPITER_INVALID_INDEX64 = UINT64_MAX;

static const unsigned int JUPITER_ERROR_INDICATOR = UINT_MAX;
static const uint32_t JUPITER_ERROR_INDICATOR32 = JUPITER_INVALID_INDEX32;
static const uint64_t JUPITER_ERROR_INDICATOR64 = JUPITER_INVALID_INDEX32;

#if defined __cplusplus
}
#endif // __cplusplus

#endif // _INVALIDINDEX_H_HEADER