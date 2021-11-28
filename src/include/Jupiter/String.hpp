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
		* @brief Sets the String's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param args Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t vformat(const T *format, va_list args);

		/**
		* @brief Appends to a String's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param args Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t avformat(const T *format, va_list args);

		/**
		* @brief Sets the String's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return String containing the new format.
		*/
		static String_Strict Format(const T *format, ...);

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

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		String_Strict<T> getWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		*
		* @param in String to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static String_Strict<T> getWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		*
		* @param in C-Style string to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static String_Strict<T> getWord(const T *in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of the string, based on a token.
		*
		* @param pos Position of the token in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		String_Strict<T> getToken(size_t pos, const T &token) const;
		String_Strict<T> getToken(size_t pos, const Jupiter::Readable_String<T> &token) const;

		/**
		* @brief Creates a partial copy of an input string, based on a token.
		*
		* @param in String to get a partial copy of.
		* @param pos Position of the token in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		static String_Strict<T> getToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token);
		static String_Strict<T> getToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token);

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		String_Strict<T> gotoWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static String_Strict<T> gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of the string, based on a token.
		*
		* @param pos Position in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		String_Strict<T> gotoToken(size_t pos, const T &token) const;
		String_Strict<T> gotoToken(size_t pos, const Jupiter::Readable_String<T> &token) const;

		/**
		* @brief Creates a partial copy of the string, based on a token.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		static String_Strict<T> gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token);
		static String_Strict<T> gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token);

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
		//String_Strict(const std::basic_string_view<T> &in) : String_Strict<T>(in.data(), in.size()){};
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

		static const Jupiter::String_Strict<T> empty; /** Empty instantiation of String_Strict */
	};

#if defined JUPITER_STRING_STRICT_OPERATOR_PLUS
	/** String_Strict<T> Addition Operators */
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::Readable_String<T> &lhs, const Jupiter::Readable_String<T> &rhs);
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::Readable_String<T> &lhs, const T &rhs);
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::Readable_String<T> &lhs, const Jupiter::Readable_String<T> &rhs);
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::Readable_String<T> &lhs, const std::basic_string<T> &rhs);
	template<typename T> static inline Jupiter::String_Strict<T> operator+(const Jupiter::Readable_String<T> &lhs, const T *rhs);
#endif // JUPITER_STRING_STRICT_OPERATOR_PLUS

	/**
	* @brief Provides a "loose" String implementation that's more optimized for repeated concatenations.
	* Note: The underlying string will always have a size which is a power of 2, but no fewer than 8 elements.
	*
	* @param T Element type which the String will store. Defaults to char.
	*/
	template<typename T = char> class String_Loose : public Shift_String_Type<T>
	{
	public:

		/**
		* @brief Returns the maximum number of elements the String can contain,
		* without expanding the socket buffer. This is generally the size of the
		* underlying memory buffer.
		*
		* @return Number of elements the string can contain without reallocation.
		*/
		virtual size_t capacity() const;

		/**
		* @brief Sets the String's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param args Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t vformat(const T *format, va_list args);

		/**
		* @brief Appends to a String's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param args Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t avformat(const T *format, va_list args);

		/**
		* @brief Sets the String's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return String containing the new format.
		*/
		static String_Loose<T> Format(const T *format, ...);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		typename Jupiter::template String_Loose<T> substring(size_t pos) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		typename Jupiter::template String_Loose<T> substring(size_t pos, size_t length) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		static typename Jupiter::template String_Loose<T> substring(const Jupiter::Readable_String<T> &in, size_t pos);
		static typename Jupiter::template String_Loose<T> substring(const T *in, size_t pos);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		static typename Jupiter::template String_Loose<T> substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t length);
		static typename Jupiter::template String_Loose<T> substring(const T *in, size_t pos, size_t length);

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		String_Loose<T> getWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		* Useful when the input string's type isn't known.
		*
		* @param in String to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static String_Loose<T> getWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		* Useful when the input string's type isn't known.
		*
		* @param in C-Style string to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static String_Loose<T> getWord(const T *in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of the string, based on a token.
		*
		* @param pos Position of the token in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		String_Loose<T> getToken(size_t pos, const T &token) const;
		String_Loose<T> getToken(size_t pos, const Jupiter::Readable_String<T> &token) const;

		/**
		* @brief Creates a partial copy of an input string, based on a token.
		*
		* @param in String to get a partial copy of.
		* @param pos Position of the token in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		static String_Loose<T> getToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token);
		static String_Loose<T> getToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token);

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		String_Loose<T> gotoWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static String_Loose<T> gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of the string, based on a token.
		*
		* @param pos Position in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		String_Loose<T> gotoToken(size_t pos, const T &token) const;
		String_Loose<T> gotoToken(size_t pos, const Jupiter::Readable_String<T> &token) const;

		/**
		* @brief Creates a partial copy of the string, based on a token.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		static String_Loose<T> gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token);
		static String_Loose<T> gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token);

		/**
		* @brief Sets the internal buffer to be at least large enough to old a specified number of elements.
		* Note: This does nothing if len is less than the string's current length.
		*
		* @param len Minimum number of elements the string buffer must be able to hold.
		* @return True if a new buffer was allocated, false otherwise.
		*/
		virtual bool setBufferSize(size_t len) override;

		/**
		* @brief Empties the string, and sets the internal buffer to be at least large enough to old a specified number of elements.
		* Note: This does nothing if len is less than the string's current length.
		*
		* @param len Minimum number of elements the string buffer must be able to hold.
		* @return True if a new buffer was allocated, false otherwise.
		*/
		virtual bool setBufferSizeNoCopy(size_t len) override;

		/** Default constructor */
		String_Loose();

		/**
		* @brief Size hint constructor.
		*
		* @param size Minimum number of elements the string must be able to hold.
		*/
		String_Loose(size_t size);

		/** Move Constructor */
		String_Loose(String_Loose<T> &&source);

		/** Copy Constructors */
		String_Loose(const String_Loose<T> &in);
		String_Loose(const Readable_String<T> &in);
		String_Loose(const std::basic_string<T> &in);
		String_Loose(const T *in, size_t len);
		String_Loose(const T *in);
		String_Loose(const Jupiter::DataBuffer &in);

		/** Concatenation Constructors */
		String_Loose(const Readable_String<T> &lhs, const T &rhs);
		String_Loose(const Readable_String<T> &lhs, const Readable_String<T> &rhs);
		String_Loose(const Readable_String<T> &lhs, const std::basic_string<T> &rhs);
		String_Loose(const Readable_String<T> &lhs, const T *rhs);
		String_Loose(const Readable_String<T> &lhs, const T *rhs, size_t rhs_size);

		/** Addition Operators */
		inline String_Loose<T> operator+(const T &rhs) const;
		inline String_Loose<T> operator+(const String_Loose<T> &rhs) const;
		inline String_Loose<T> operator+(const Readable_String<T> &rhs) const;
		inline String_Loose<T> operator+(const std::basic_string<T> &rhs) const;
		inline String_Loose<T> operator+(const T *rhs) const;

		/** Assignment Operators */
		inline String_Loose<T> &operator=(const String_Loose<T> &right) { this->set(right); return *this; };
		inline String_Loose<T> &operator=(const Readable_String<T> &right) { this->set(right); return *this; };
		inline String_Loose<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline String_Loose<T> &operator=(const T *right) { this->set(right); return *this; };
		inline String_Loose<T> &operator=(const T right) { this->set(right); return *this; };

		static const Jupiter::String_Loose<T> empty; /** Empty instantiation of String_Loose */
		static const size_t start_size = 8; /** Starting size for loose Strings. */

	protected:
		size_t strSize; /** Size of underlying string buffer */
	};

#if !defined JUPITER_STRING_STRICT_OPERATOR_PLUS
#if !defined DISABLE_DEFAULT_JUPITER_STRING_OPERATOR_PLUS
	/** String_Loose<T> Addition Operator */
	template<typename T> static inline Jupiter::String_Loose<T> operator+(const Jupiter::Readable_String<T> &lhs, const T &rhs);
	template<typename T> static inline Jupiter::String_Loose<T> operator+(const Jupiter::Readable_String<T> &lhs, const Jupiter::Readable_String<T> &rhs);
	template<typename T> static inline Jupiter::String_Loose<T> operator+(const Jupiter::Readable_String<T> &lhs, const std::basic_string<T> &rhs);
	template<typename T> static inline Jupiter::String_Loose<T> operator+(const Jupiter::Readable_String<T> &lhs, const T *rhs);
#endif // DISABLE_DEFAULT_JUPITER_STRING_OPERATOR_PLUS
#endif // JUPITER_STRING_STRICT_OPERATOR_PLUS

	/** Definition of a Strict String. */
	typedef String_Strict<char> StringS;

	/** Definition of a Loose String. */
	typedef String_Loose<char> StringL;

	/** Definition of a String. */
	typedef StringL String;

	namespace literals {
		/** String_Strict literals */
		inline Jupiter::StringS operator""_jss(const char *str, size_t len) { return Jupiter::StringS(str, len); }
	}

	// Carried over from Hash_Table.h for compatibility
	struct default_hash_function {
		size_t operator()(const Jupiter::ReadableString& in) const {
			if constexpr (sizeof(size_t) >= sizeof(uint64_t))
				return static_cast<size_t>(Jupiter::fnv1a<char>(in));

			return static_cast<size_t>(Jupiter::fnv1a_32<char>(in));
		}
	};
}

/** Re-enable warning */
#if defined _MSC_VER
#pragma warning(pop)
#endif

/** Implementation for String_Strict and String_Loose. Very scary. */
#include "String_Imp.h"

#endif // _STRING_H_HEADER