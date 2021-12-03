/**
 * Copyright (C) 2015-2016 Jessica James.
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

#include <cstdint>
#include <string_view>
#include "Jupiter.h"

namespace Jupiter
{
	/** Sums */
	template<typename T = char, typename R = uint64_t> inline R calcsum(const T *in_data, size_t in_length);
	template<typename T = char, typename R = uint64_t> inline R calcsum(const std::basic_string_view<T> &str);

	/** Fowler-Noll-Vo hash algorithms */
	template<typename T> inline uint64_t fnv1(const T &data);
	template<typename T = char> inline uint64_t fnv1(const T *str, size_t length);
	template<typename T = char> inline uint64_t fnv1(const std::basic_string_view<T> &str);

	template<typename T> inline uint32_t fnv1_32(const T &data);
	template<typename T = char> inline uint32_t fnv1_32(const T *str, size_t length);
	template<typename T = char> inline uint32_t fnv1_32(const std::basic_string_view<T> &str);

	template<typename T> inline uint64_t fnv1a(const T &data);
	template<typename T = char> inline uint64_t fnv1a(const T *str, size_t length);
	template<typename T = char> inline uint64_t fnv1a(const std::basic_string_view<T> &str);

	template<typename T> inline uint32_t fnv1a_32(const T &data);
	template<typename T = char> inline uint32_t fnv1a_32(const T *str, size_t length);
	template<typename T = char> inline uint32_t fnv1a_32(const std::basic_string_view<T> &str);

	JUPITER_API uint64_t fnv1(const uint8_t *data, const uint8_t *end);
	JUPITER_API uint64_t fnv1a(const uint8_t *data, const uint8_t *end);

	JUPITER_API uint32_t fnv1_32(const uint8_t *data, const uint8_t *end);
	JUPITER_API uint32_t fnv1a_32(const uint8_t *data, const uint8_t *end);

	/** unordered_map helpers */
	template<typename CharT>
	struct str_hash {
		using is_transparent = std::true_type;

		// C++17 introduces a requirement that these two operators return the same values for same CharT type, but not
		// any requirement that std::hash<> be able to accept both key types. This just ties them for convenience
		auto operator()(std::basic_string_view<CharT> in_key) const noexcept {
			return std::hash<std::basic_string_view<CharT>>()(in_key);
		}

		auto operator()(const std::basic_string<CharT>& in_key) const noexcept {
			return std::hash<std::basic_string<CharT>>()(in_key);
		}
	};

#ifdef __cpp_lib_generic_unordered_lookup
	using InMapKeyType = std::string_view;
#define JUPITER_WRAP_MAP_KEY(in_key) in_key
#else // We can't use std::string_view for InKeyType until GCC 11 & clang 12, and I still want to support GCC 9
	using InMapKeyType = std::string;
#define JUPITER_WRAP_MAP_KEY(in_key) static_cast<Jupiter::InMapKeyType>(in_key)
#endif // __cpp_lib_generic_unordered_lookup

	using default_hash_function = str_hash<char>;
}

/** Calcsum implementation */

template<typename T, typename R> inline R Jupiter::calcsum(const T *in_data, size_t in_length)
{
	const uint8_t *itr = reinterpret_cast<const uint8_t *>(in_data);
	const uint8_t *end = reinterpret_cast<const uint8_t *>(in_data + in_length);
	R sum = 0;

	while (itr != end)
	{
		sum += *itr;
		++itr;
	}

	return sum;
}

template<typename T, typename R> inline R Jupiter::calcsum(const std::basic_string_view<T> &str)
{
	return Jupiter::calcsum<T, R>(str.data(), str.size());
}

/** fnv1 implementation */
template<typename T> inline uint64_t Jupiter::fnv1(const T &data)
{
	return Jupiter::fnv1(&data, &data + 1);
}

template<typename T> inline uint64_t Jupiter::fnv1(const T *data, size_t length)
{
	return Jupiter::fnv1(reinterpret_cast<const uint8_t *>(data), reinterpret_cast<const uint8_t *>(data + length));
}

template<typename T> inline uint64_t Jupiter::fnv1(const std::basic_string_view<T> &data)
{
	return Jupiter::fnv1(reinterpret_cast<const uint8_t *>(data.data()), reinterpret_cast<const uint8_t *>(data.data() + data.size()));
}

template<typename T> inline uint32_t Jupiter::fnv1_32(const T &data)
{
	return Jupiter::fnv1_32(&data, &data + 1);
}

template<typename T> inline uint32_t Jupiter::fnv1_32(const T *data, size_t length)
{
	return Jupiter::fnv1_32(reinterpret_cast<const uint8_t *>(data), reinterpret_cast<const uint8_t *>(data + length));
}

template<typename T> inline uint32_t Jupiter::fnv1_32(const std::basic_string_view<T> &data)
{
	return Jupiter::fnv1_32(reinterpret_cast<const uint8_t *>(data.data()), reinterpret_cast<const uint8_t *>(data.data() + data.size()));
}

template<typename T> inline uint64_t Jupiter::fnv1a(const T &data)
{
	return Jupiter::fnv1a(reinterpret_cast<const uint8_t *>(&data), reinterpret_cast<const uint8_t *>(&data + 1));
}

template<typename T> inline uint64_t Jupiter::fnv1a(const T *data, size_t length)
{
	return Jupiter::fnv1a(reinterpret_cast<const uint8_t *>(data), reinterpret_cast<const uint8_t *>(data + length));
}

template<typename T> inline uint64_t Jupiter::fnv1a(const std::basic_string_view<T> &data)
{
	return Jupiter::fnv1a(data.data(), data.size());
}

template<typename T> inline uint32_t Jupiter::fnv1a_32(const T &data)
{
	return Jupiter::fnv1a_32(reinterpret_cast<const uint8_t *>(&data), reinterpret_cast<const uint8_t *>(&data + 1));
}

template<typename T> inline uint32_t Jupiter::fnv1a_32(const T *data, size_t length)
{
	return Jupiter::fnv1a_32(reinterpret_cast<const uint8_t *>(data), reinterpret_cast<const uint8_t *>(data + length));
}

template<typename T> inline uint32_t Jupiter::fnv1a_32(const std::basic_string_view<T> &data)
{
	return Jupiter::fnv1a_32(data.data(), data.size());
}

#endif // _HASH_H_HEADER