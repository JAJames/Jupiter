/**
 * Copyright (C) 2014-2021 Jessica James.
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

#if !defined _READABLE_STRING_H_HEADER
#define _READABLE_STRING_H_HEADER

/**
 * @file Readable_String.h
 * @brief Defines various legacy string utilities which should be removed
 */

#include <string> // std::basic_string<T> type
#include <string_view> // std::basic_string_view<T> type
#include <cstdarg> // arg helpers needed by string_printf
#include "Functions.h" // string -> bool/number converters

namespace Jupiter {
	// these methods will be written in a similar fashion in jessilib, to retain behavior / ease of use
	template<typename CharT> /** DEPRECATED */
	bool asBool(std::basic_string_view<CharT> in_string) {
		using namespace std::literals;

		// Invert the logic here, so that we return true for numerics and "true"/"on"/"+", false otherwise
		// Note: temporarily removed case insensitivity, solely to avoid including unicode.hpp here
		if (in_string == "FALSE"sv
			|| in_string == "false"sv
			|| in_string == "0"sv
			|| in_string == "OFF"sv
			|| in_string == "off"sv
			|| in_string == "-"sv) {
			return false;
		}

		return true;
	}

	template<typename CharT> /** DEPRECATED */
	int asInt(std::basic_string_view<CharT> in_string, int base = 0) {
		return Jupiter_strtoi_s(in_string.data(), in_string.size(), base);
	}

	template<typename CharT> /** DEPRECATED */
	long long asLongLong(std::basic_string_view<CharT> in_string, int base = 0) {
		return Jupiter_strtoll_s(in_string.data(), in_string.size(), base);
	}

	template<typename CharT> /** DEPRECATED */
	unsigned int asUnsignedInt(std::basic_string_view<CharT> in_string, int base = 0) {
		return Jupiter_strtoui_s(in_string.data(), in_string.size(), base);
	}

	template<typename CharT> /** DEPRECATED */
	unsigned long long asUnsignedLongLong(std::basic_string_view<CharT> in_string, int base = 0) {
		return Jupiter_strtoull_s(in_string.data(), in_string.size(), base);
	}

	template<typename CharT> /** DEPRECATED */
	double asDouble(std::basic_string_view<CharT> in_string) {
		return Jupiter_strtod_s(in_string.data(), in_string.size());
	}

	/** DEPRECATED (similar function w/ identical semantics to be implemented) */
	template<typename OutT>
	OutT from_string(std::string_view in_string) {
		return static_cast<OutT>(in_string);
	}

	template<>
	inline bool from_string<bool>(std::string_view in_string) {
		return asBool(in_string);
	}

	template<>
	inline signed char from_string<signed char>(std::string_view in_string) {
		return static_cast<signed char>(asInt(in_string));
	}

	template<>
	inline unsigned char from_string<unsigned char>(std::string_view in_string) {
		return static_cast<unsigned char>(asUnsignedInt(in_string));
	}

	template<>
	inline short from_string<short>(std::string_view in_string) {
		return static_cast<short>(asInt(in_string));
	}

	template<>
	inline unsigned short from_string<unsigned short>(std::string_view in_string) {
		return static_cast<unsigned short>(asUnsignedInt(in_string));
	}

	template<>
	inline int from_string<int>(std::string_view in_string) {
		return asInt(in_string);
	}

	template<>
	inline unsigned int from_string<unsigned int>(std::string_view in_string) {
		return asUnsignedInt(in_string);
	}

	template<>
	inline long from_string<long>(std::string_view in_string) {
		return asInt(in_string);
	}

	template<>
	inline unsigned long from_string<unsigned long>(std::string_view in_string) {
		return asUnsignedInt(in_string);
	}

	template<>
	inline long long from_string<long long>(std::string_view in_string) {
		return asLongLong(in_string);
	}

	template<>
	inline unsigned long long from_string<unsigned long long>(std::string_view in_string) {
		return asUnsignedLongLong(in_string);
	}

	template<>
	inline float from_string<float>(std::string_view in_string) {
		return static_cast<float>(asDouble(in_string));
	}

	template<>
	inline double from_string<double>(std::string_view in_string) {
		return asDouble(in_string);
	}

	template<>
	inline long double from_string<long double>(std::string_view in_string) {
		return asDouble(in_string);
	}

} // namespace Jupiter

/** DEPRECATED */
inline std::string vstring_printf(const char* format, va_list args) {
	std::string result;

	// Calculate size needed for formatted string
	va_list args_copy;
	va_copy(args_copy, args);
	int min_length = std::vsnprintf(nullptr, 0, format, args_copy);
	va_end(args_copy);

	if (min_length > 0) {
		result.resize(min_length);
		min_length = vsnprintf(result.data(), result.size() + 1, format, args);
		if (min_length <= 0) {
			result.clear();
		}

		if (result.size() != min_length) {
			// This should never happen; if it does, it's probably indicative of a race condition
			result.erase(min_length);
		}
	}

	return result;
}

/** DEPRECATED */
inline std::string string_printf(const char* format, ...) {
	va_list args;
	va_start(args, format);

	std::string result = vstring_printf(format, args);

	va_end(args);
	return result;
}

#endif // _READABLE_STRING_H_HEADER