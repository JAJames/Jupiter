/**
 * Copyright (C) 2014-2015 Justin James.
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
 * Written by Justin James <justin.aj@hotmail.com>
 */

#if !defined _BASE64_H_HEADER
#define _BASE64_H_HEADER

/**
 * @file Base64.h
 * @brief Provides C and C++ functions to encode/decode using base64.
 */

#include "Jupiter.h"

#if defined __cplusplus

#include <cstdint>
#include <cstddef>

namespace Jupiter
{
	/**
	* @brief Encodes an input buffer into a base64 C-String.
	*
	* @param data Data to encode.
	* @param dataLength Number of bytes to encode.
	* @param result Character buffer for output.
	* @return Number of bytes written to the output buffer.
	*/
	JUPITER_API unsigned int base64encode(const void *data, size_t dataLength, char *result);

	/**
	* @brief Checks if a buffer is large enough, and if so, encodes an input buffer into a base64 C-String.
	*
	* @param data Data to encode.
	* @param dataLength Number of bytes to encode.
	* @param result Character buffer for output.
	* @param outputSize Size of the "result" buffer.
	* @return Number of bytes written to the output buffer.
	*/
	JUPITER_API unsigned int base64encode(const void *data, size_t dataLength, char *result, size_t outputSize);

	/**
	* @brief Encodes an input C-String into a base64 C-String.
	*
	* @param data String to encode.
	* @param dataLength Number of bytes to encode.
	* @return Buffer containing the encoded base64 string.
	*/
	JUPITER_API char *base64encode(const void *data, size_t dataLength);

	/**
	* @brief Encodes an input C-String into a base64 C-String.
	*
	* @param data String to encode.
	* @return Buffer containing the encoded base64 string.
	*/
	JUPITER_API char *base64encode(const char *data);

	/**
	* @brief Decodes an input base64 C-String into a data buffer.
	*
	* @param str Data to decode.
	* @param result Data buffer for output.
	* @return Number of bytes written to the output buffer.
	*/
	JUPITER_API unsigned int base64decode(const char *data, size_t dataLength, unsigned char *result, size_t resultSize);

	/**
	* @brief Decodes an input base64 character string into a C-String buffer.
	*
	* @param data Data to decode.
	* @param dataLength Length of the data to decode.
	* @return Number of bytes written to the output buffer.
	*/
	JUPITER_API unsigned int base64decode(const char *data, size_t dataLength, unsigned char *result);

	/**
	* @brief Decodes an input base64 character string into a C-String buffer.
	*
	* @param data Data to decode.
	* @param dataLength Length of the data to decode.
	* @return C-String containing the decoded buffer.
	*/
	JUPITER_API char *base64decode(const char *data, size_t dataLength);

	/**
	* @brief Decodes an input base64 C-String into a C-String buffer.
	*
	* @param str Data to decode.
	* @return C-String containing the decoded buffer.
	*/
	JUPITER_API char *base64decode(const char *str);
}

extern "C"
{
#else
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#endif // __cplusplus

/**
* @brief Encodes an input buffer into a base64 C-String.
*
* @param data Data to encode.
* @param dataLength Number of bytes to encode.
* @param result Character buffer for output.
* @return Number of bytes written to the output buffer.
*/
JUPITER_API unsigned int Jupiter_base64encode(const void *data, size_t dataLength, char *result);

/**
* @brief Calculates the minimum buffer size to decode a specified base64 string.
* Note: This does NOT include the neccessary space for a null-terminator.
*
* @param data Data which would be decoded.
* @param inLen Length of the input string.
* @return Minimum number of bytes to fit the decoded buffer.
*/
JUPITER_API unsigned int Jupiter_minBase64DecodeLength(const char *data, size_t inLen);

/**
* @brief Checks if a buffer is a valid base64 string.
*
* @param str C-String containing data to check.
* @return True if the input buffer is a valid base64 string, false otherwise.
*/
JUPITER_API bool Jupiter_isBase64(const char *str);

/**
* @brief Checks if a buffer is a valid base64 string.
*
* @param data Buffer to check.
* @param dataLength Length of the data to check.
* @return True if the input buffer is a valid base64 string, false otherwise.
*/
JUPITER_API bool Jupiter_isBase64_s(const char *data, size_t dataLength);

/**
* @brief Decodes an input base64 C-String into a data buffer.
*
* @param str C-String containing data to decode.
* @param result Data buffer for output.
* @return Number of bytes written to the output buffer.
*/
JUPITER_API unsigned int Jupiter_base64decode(const char *str, unsigned char *result);

/**
* @brief Decodes an input base64 string into a data buffer.
*
* @param data Data to decode.
* @param dataLength Length of the base64 string.
* @param result Data buffer for output.
* @return Number of bytes written to the output buffer.
*/
JUPITER_API unsigned int Jupiter_base64decode_s(const char *data, size_t dataLength, unsigned char *result);

#if defined __cplusplus
}
#endif // __cplusplus

#endif // _BASE64_H_HEADER