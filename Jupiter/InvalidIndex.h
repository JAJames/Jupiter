/**
 * Copyright (C) 2014 Justin James.
 *
 * This license must be preserved.
 * Any applications, libraries, or code which make any use of any
 * component of this program must not be commercial, unless explicit
 * permission is granted from the original author. The use of this
 * program for non-profit purposes is permitted.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In the event that this license restricts you from making desired use of this program, contact the original author.
 * Written by Justin James <justin.aj@hotmail.com>
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