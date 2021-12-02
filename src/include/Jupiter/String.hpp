/**
 * Copyright (C) 2013-2017 Jessica James.
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

#if !defined _STRING_H_HEADER
#define _STRING_H_HEADER

/**
 * @file String.h
 * @brief Defines the base String_Base, as well as a series of String types.
 * Note: Functions which take "case" or "wildcards" into consideration will only function
 * for types char and wchar_t; inputs with other types will simply return false.
 */

#include "Shift_String.h"
#include "Hash.h"

/** Disable warning 4458 */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4458) // declaration of 'length' hides class member
#endif

namespace Jupiter
{

	/**
	* @brief Provides a minimal String implementation (i.e: no extra variables).
	*
	* @param T Element type which the String will store. Defaults to char.
	*/
	template<typename T = char> class String_Strict : public Shift_String_Type<T>
	{
	public:

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		typename Jupiter::template String_Strict<T> substring(size_t pos) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		typename Jupiter::template String_Strict<T> substring(size_t pos, size_t length) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		static typename Jupiter::template String_Strict<T> substring(const Jupiter::Readable_String<T> &in, size_t pos);
		static typename Jupiter::template String_Strict<T> substring(const T *in, size_t pos);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		static typename Jupiter::template String_Strict<T> substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t length);
		static typename Jupiter::template String_Strict<T> substring(const T *in, size_t pos, size_t length);

		/** Default Constructor */
		String_Strict();

		/**
		* @brief Size hint constructor.
		* Note: For the String_Strict base class, this is only truly useful internally.
		*
		* @param size Minimum number of elements the string must be able to hold.
		*/
		String_Strict(size_t size);

		/** Move Constructor */
		String_Strict(String_Strict<T> &&source);

		/** Copy Constructors */
		String_Strict(const String_Strict<T> &in);
		String_Strict(const Readable_String<T> &in);
		String_Strict(const std::basic_string<T> &in);
		String_Strict(const T *in, size_t len);
		String_Strict(const T *in);
		String_Strict(const Jupiter::DataBuffer &in);

		/** Concatenation Constructors */
		String_Strict(const Readable_String<T> &lhs, const T &rhs);
		String_Strict(const Readable_String<T> &lhs, const Readable_String<T> &rhs);
		String_Strict(const Readable_String<T> &lhs, const std::basic_string<T> &rhs);
		String_Strict(const Readable_String<T> &lhs, const T *rhs);
		String_Strict(const Readable_String<T> &lhs, const T *rhs, size_t rhs_size);

		/** Addition Operators */
		inline String_Strict<T> operator+(const T &rhs) const;
		inline String_Strict<T> operator+(const String_Strict<T> &rhs) const;
		inline String_Strict<T> operator+(const Readable_String<T> &rhs) const;
		inline String_Strict<T> operator+(const std::basic_string<T> &rhs) const;
		inline String_Strict<T> operator+(const T *rhs) const;

		/** Assignment Operators */
		inline String_Strict<T> &operator=(const String_Strict<T> &right) { this->set(right); return *this; };
		inline String_Strict<T> &operator=(const Readable_String<T> &right) { this->set(right); return *this; };
		inline String_Strict<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline String_Strict<T> &operator=(const T *right) { this->set(right); return *this; };
		inline String_Strict<T> &operator=(const T right) { this->set(right); return *this; };
	};

	/** String_Strict<T> Addition Operators */
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::String_Type<T> &lhs, const Jupiter::String_Type<T> &rhs);
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::String_Type<T> &lhs, const T &rhs);
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::String_Type<T> &lhs, const Jupiter::String_Type<T> &rhs);
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::String_Type<T> &lhs, const std::basic_string<T> &rhs);
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::String_Type<T> &lhs, const std::basic_string_view<T> &rhs);
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::String_Type<T> &lhs, const T *rhs);

	/** Definition of a Strict String. */
	typedef String_Strict<char> StringS;

	/** Definition of a Loose String. */
	typedef String_Strict<char> StringL;

	/** Definition of a String. */
	typedef StringS String;

	namespace literals {
		/** String_Strict literals */
		inline Jupiter::StringS operator""_jss(const char *str, size_t len) { return Jupiter::StringS(str, len); }
	}

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

		// DEPRECATED:
		auto operator()(const String_Type<CharT>& in_key) const noexcept {
			return operator()(static_cast<std::basic_string_view<CharT>>(in_key));
		}
	};

	using default_hash_function = str_hash<char>;
}

/** Re-enable warning */
#if defined _MSC_VER
#pragma warning(pop)
#endif

/** Implementation for String_Strict and String_Loose. Very scary. */
#include "String_Imp.h"

#endif // _STRING_H_HEADER