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

#if !defined _FUNCTIONS_H_HEADER
#define _FUNCTIONS_H_HEADER

#include "Jupiter.h"
#define ENDL "\r\n"
#define WHITESPACE " \t"

/**
 * @file Functions.h
 * @brief Provides some functions to be used at your convenience.
 */

#if defined __cplusplus

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace Jupiter
{
	/**
	* @brief Returns the length of a C-style string.
	* This will often get optimized by the compiler such that the calculation time is 0.
	*
	* @param str C-style string to get the length of.
	* @return Length of the C-style string.
	*/
	template<typename T = char> inline size_t strlen(const T *str);

	/**
	* @brief Copies a string of elements from one buffer to another.
	* Note: The destination buffer must be at least as long as the source + 1.
	*
	* @param dest Destination buffer for the string of elements.
	* @param source Buffer containing the source elements.
	* @return Pointer to the destination.
	*/
	template<typename T = char> inline T *strcpy(T *dest, const T *source);

	/**
	* @brief Copies a string of elements from one buffer to another.
	* Note: The destination buffer must be at least as long as sourceLength + 1.
	*
	* @param dest Destination buffer for the string of elements.
	* @param source Buffer containing the source elements.
	* @param sourceLength Number of elements to copy, excluding null-terminator.
	* @return Pointer to the destination.
	*/
	template<typename T = char> inline T *strcpy(T *dest, const T *source, size_t sourceLength);

	/**
	* @brief Returns a pointer to the first occurance of an element from str2 appearing in str1, or nullptr.
	* Note: The destination buffer must be at least as long as sourceLength + 1.
	*
	* @param str1 String to be scanned.
	* @param str2 String containing elements to find.
	* @return Pointer to the first instance of a character in str2 being in str1, or nullptr if there is none.
	*/
	template<typename T = char> inline const T *strpbrk(const T *str1, const T *str2);
	template<typename T = char> inline const T *strpbrk(const T *str, const T element);

	/**
	* @brief Checks if two strings are equal.
	*
	* @param str1 String to be compared.
	* @param str2 String to be compared.
	* @return True if the strings are equal, false otherwise.
	*/
	template<typename T = char> bool streql(const T *str1, const T *str2);

	/**
	* @brief Gets the first power of two from in_value
	*
	* @param in_value Integer to get power of two from
	* @return First power of two at least as large as in_value
	*/
	template<typename T> T getPowerTwo(T in_value);
}

extern "C"
{
#else // __cplusplus
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#endif // __cplusplus

/**
* @brief Checks if a character exists in a string.
*
* @param str String to scan.
* @return True if it contains a given character, false otherwise.
*/
JUPITER_API bool containsSymbol(const char *str, char c);

/**
* @brief Returns the current time in a string format.
* This will vary depending on locale.
* Format: Day_of_week Month Day hh:mm:ss Year Timezone.
* Example 1: Sun Aug 18 13:52:21 2013 EST
* Example 2: Thu Nov 14 03:52:57 2013 Eastern Standard Time
* Exmaple 3: Sun Nov 17 15:06:19 2013 Tokyo Standard Time
* This is the same as getTimeFormat("%a %b %d %H:%M:%S %Y %Z").
*
* @return A string containing the time in the format specified above.
*/
JUPITER_API char *getTime();

/**
* @brief Returns the current time in a string format, based on an input format.
* Format specifiers are the same as strftime().
*
* @param format String denoting the desired format of the output string.
* @return A string containing the time in the format specified above.
*/
JUPITER_API char *getTimeFormat(const char *format);

/**
* @brief Gets the next power of 2 after a specified number.
*
* @param number 32-bit integer to start from.
* @return Next highest power of 2.
*/
JUPITER_API uint32_t getPowerTwo32(uint32_t number);

/**
* @brief Gets the next power of 2 after a specified number.
*
* @param number 64-bit integer to start from.
* @return Next highest power of 2.
*/
JUPITER_API uint64_t getPowerTwo64(uint64_t number);

/**
* @brief Checks if a character is a digit character of a specified radix.
* Note: The maximum base is 36 (0-9 and a-z)
*
* @param chr Character to check.
* @return True if the character is a hexadecimal digit, false otherwise.
*/
JUPITER_API bool Jupiter_isBase(unsigned char chr, int base);

/**
* @brief Checks if a character is a hexadecimal digit character. (base 16)
*
* @param chr Character to check.
* @return True if the character is a hexadecimal digit, false otherwise.
*/
JUPITER_API bool Jupiter_isHex(unsigned char chr);

/**
* @brief Checks if a character is a decimal digit character. (base 10)
*
* @param chr Character to check.
* @return True if the character is a decimal digit, false otherwise.
*/
JUPITER_API bool Jupiter_isDecimal(unsigned char chr);

/**
* @brief Checks if a character is an octal digit character. (base 10)
*
* @param chr Character to check.
* @return True if the character is a octal digit, false otherwise.
*/
JUPITER_API bool Jupiter_isOctal(unsigned char chr);

/**
* @brief Fetches a character's represented integral value.
* Note: The maximum base is 36 (0-9 and a-z)
*
* @param chr Character to fetch value of.
* @param base Base of the representation.
* @return A character's represented integral value on success, -1 otherwise.
*/
JUPITER_API int Jupiter_getBase(unsigned char chr, int base);

/**
* @brief Fetches a hexadecimal character's represented integral value. (base 16)
*
* @param chr Character to fetch value of.
* @return A character's represented integral value on success, -1 otherwise.
*/
JUPITER_API int Jupiter_getHex(unsigned char chr);

/**
* @brief Fetches a decimal character's represented integral value. (base 10)
*
* @param chr Character to fetch value of.
* @return A character's represented integral value on success, -1 otherwise.
*/
JUPITER_API int Jupiter_getDecimal(unsigned char chr);

/**
* @brief Fetches a octal character's represented integral value. (base 8)
*
* @param chr Character to fetch value of.
* @return A character's represented integral value on success, -1 otherwise.
*/
JUPITER_API int Jupiter_getOctal(unsigned char chr);

/**
* @brief Fetches the string hexadecimal representation of an octet.
* Note: This function returns the upper-case hexadecimal notation
*
* @param oct Octet to fetch hexadecimal representation of
* @return The octet's hexadecimal representation as a string
*/
JUPITER_API const char *Jupiter_asHex_upper(uint8_t oct);

/**
* @brief Fetches the string hexadecimal representation of an octet.
* Note: This function returns the lower-case hexadecimal notation
*
* @param oct Octet to fetch hexadecimal representation of
* @return The octet's hexadecimal representation as a string
*/
JUPITER_API const char *Jupiter_asHex_lower(uint8_t oct);

/**
* @brief Interprets a string into an integer.
*
* @param str String to interpret.
* @param base Base of the string to interpret.
* @return Interpretation of the string as an integer on success, 0 otherwise.
*/
JUPITER_API int Jupiter_strtoi(const char *str, int base);
JUPITER_API long long Jupiter_strtoll(const char *str, int base);
JUPITER_API unsigned int Jupiter_strtoui(const char *str, int base);
JUPITER_API unsigned long long Jupiter_strtoull(const char *str, int base);

/**
* @brief Interpets a string into a floating point decimal number.
*
* @param str String to interpret.
* @param length Length of the string to interpret.
* @return Interpretation of the string as a floating point decimal on success, 0.0 otherwise.
*/
JUPITER_API double Jupiter_strtod(const char *str);

/**
* @brief Interprets a string into an integer.
* Note: This function assumes no preceeding space characters exist.
*
* @param str String to interpret.
* @param base Base of the string to interpret.
* @return Interpretation of the string as an integer on success, 0 otherwise.
*/
JUPITER_API int Jupiter_strtoi_nospace(const char *str, int base);
JUPITER_API long long Jupiter_strtoll_nospace(const char *str, int base);
JUPITER_API unsigned int Jupiter_strtoui_nospace(const char *str, int base);
JUPITER_API unsigned long long Jupiter_strtoull_nospace(const char *str, int base);

/**
* @brief Interpets a string into a floating point decimal number.
* Note: This function assumes no preceeding space characters exist.
*
* @param str String to interpret.
* @param length Length of the string to interpret.
* @return Interpretation of the string as a floating point decimal on success, 0.0 otherwise.
*/
JUPITER_API double Jupiter_strtod_nospace(const char *str);

/**
* @brief Interprets a string into an integer.
*
* @param str String to interpret.
* @param length Length of str.
* @param base Base of the string to interpret.
* @return Interpretation of the string as an integer on success, 0 otherwise.
*/
JUPITER_API int Jupiter_strtoi_s(const char *str, size_t length, int base);
JUPITER_API long long Jupiter_strtoll_s(const char *str, size_t length, int base);
JUPITER_API unsigned int Jupiter_strtoui_s(const char *str, size_t length, int base);
JUPITER_API unsigned long long Jupiter_strtoull_s(const char *str, size_t length, int base);

/**
* @brief Interpets a string into a floating point decimal number.
*
* @param str String to interpret.
* @param length Length of the string to interpret.
* @return Interpretation of the string as a floating point decimal on success, 0.0 otherwise.
*/
JUPITER_API double Jupiter_strtod_s(const char *str, size_t length);

/**
* @brief Interprets a string into an integer.
* Note: This function assumes no preceeding space characters exist.
*
* @param str String to interpret.
* @param length Length of str.
* @param base Base of the string to interpret.
* @return Interpretation of the string as an integer on success, 0 otherwise.
*/
JUPITER_API int Jupiter_strtoi_nospace_s(const char *str, size_t length, int base);
JUPITER_API long long Jupiter_strtoll_nospace_s(const char *str, size_t length, int base);
JUPITER_API unsigned int Jupiter_strtoui_nospace_s(const char *str, size_t length, int base);
JUPITER_API unsigned long long Jupiter_strtoull_nospace_s(const char *str, size_t length, int base);

/**
* @brief Interpets a string into a floating point decimal number.
* Note: This function assumes no preceeding space characters exist.
*
* @param str String to interpret.
* @param length Length of the string to interpret.
* @return Interpretation of the string as a floating point decimal on success, 0.0 otherwise.
*/
JUPITER_API double Jupiter_strtod_nospace_s(const char *str, size_t length);

/**
* @brief Converts an input prefix length into a bitmask
* Note: Local byte order
*
* @param prefix_length Prefix length to convert
* @return Bitmask represented by prefix_length
*/
JUPITER_API uint32_t Jupiter_prefix_length_to_bitmask(uint8_t prefix_length);

/**
* @brief Converts an input prefix length into a netmask
* Note: Network byte order
*
* @param prefix_length Prefix length to convert
* @return Netmask represented by prefix_length
*/
JUPITER_API uint32_t Jupiter_prefix_length_to_netmask(uint8_t prefix_length);

#if defined __cplusplus
}

/** strlen implementation */
template<typename T> inline size_t Jupiter::strlen(const T *str)
{
	const T *s = str;
	while (*s != 0)
		++s;
	return s - str;
}

/** strcpy implementation */
template<typename T> inline T *Jupiter::strcpy(T *dest, const T *source)
{
	T *d = dest;
	while (*source != 0)
	{
		*d = *source;
		++d, ++source;
	}
	*d = *source;
	return dest;
}

/** strcpy implementation */
template<typename T> inline T *Jupiter::strcpy(T *dest, const T *source, size_t length)
{
	dest[length] = 0;
	while (length > 0)
	{
		--length;
		dest[length] = source[length];
	}
	return dest;
}

template<typename T> inline const T *Jupiter::strpbrk(const T *str1, const T *str2)
{
	const T *s;
	while (*str1 != 0)
	{
		s = str2;
		while (*s != 0)
		{
			if (*str1 == *s)
				return str1;
			++s;
		}
		++str1;
	}
	return nullptr;
}

template<typename T> inline const T *Jupiter::strpbrk(const T *str, const T element)
{
	while (*str != 0)
	{
		if (*str == element)
			return str;
		++str;
	}
	return nullptr;
}

template<typename T> inline bool Jupiter::streql(const T *str1, const T *str2)
{
	if (str1 == str2)
		return true;
	while (*str1 != 0 && *str1 == *str2)
		++str1, ++str2;
	return (*str1 == *str2);
}

#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127) /** Conditional is constant; can probably be removed when 'if constexpr' support is added to MSVC */
#pragma warning(disable: 4293) /** shift count negative or too big, undefined behavior */
#endif

template<typename T> T Jupiter::getPowerTwo(T in_value)
{
	static_assert(std::is_integral<T>::value, "Integer type required");

	--in_value;
	in_value |= in_value >> 1;
	in_value |= in_value >> 2;
	in_value |= in_value >> 4;

	if (sizeof(T) >= 2)
		in_value |= in_value >> 8;

	if (sizeof(T) >= 4)
		in_value |= in_value >> 16;

	if (sizeof(T) >= 8)
		in_value |= in_value >> 32;

	if (sizeof(T) >= 16)
		in_value |= in_value >> 64;

	return ++in_value;
}

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // __cplusplus

#endif // _FUNCTIONS_H_HEADER