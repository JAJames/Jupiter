/**
 * Copyright (C) 2014-2017 Jessica James.
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

#if !defined _REFERENCE_STRING_H_HEADER
#define _REFERENCE_STRING_H_HEADER

/**
 * @file Reference_String.h
 * @brief Provides the basis for String types, of any implementation.
 * Note: DEPRECATED
 */

#include <string_view>
#include "Readable_String.h"

namespace Jupiter
{

	/**
	* @brief Provides the basis for String classes by providing implementations for operators, comparative operations, and defining abstract functions.
	* Note: This is an abstract type.
	*
	* @param T Element type which the String will store. Defaults to char.
	*/
	template<typename T = char> class Reference_String : public Jupiter::Readable_String<T>, public std::basic_string_view<T> {
		/** DEPRECATED IN FAVOR OF std::string_view */
	public:

		/**
		* @brief Fetches an element from the string.
		*
		* @param index Index of the element to return.
		* @return The element located at the specified index.
		*/
		const T &get(size_t index) const override;

		/**
		* @brief Returns the number of elements in the String.
		*
		* @return Number of elements in the string.
		*/
		size_t size() const override;

		/**
		* @brief Returns a pointer to the underlying string of elements.
		*
		* @return Pointer to the underlying string of elements.
		*/
		const T *ptr() const override;

		/**
		* @brief Truncates the string by a specified number of elements.
		*
		* @param n Number of elements to remove from the tail.
		* @return New size of the String.
		*/
		virtual size_t truncate(size_t n);

		/**
		* @brief Erases the string (sets the string's length to 0)
		*/
		void erase();

		/**
		* @brief Shifts the string pointer to the left.
		*
		* @param length Number of elements to shift
		* @return Number of elements shifted to the left.
		*/
		size_t shiftLeft(size_t length);

		/**
		* @brief Shifts the string pointer to the right.
		*
		* @param length Number of elements to shift
		* @return Number of elements shifted.
		*/
		size_t shiftRight(size_t length);

		/**
		* @brief Sets the reference to point to an input string.
		*
		* @param in String containing the data to be referenced.
		* @return New size of the String.
		*/
		size_t set(const Jupiter::Readable_String<T> &in);
		size_t set(const std::basic_string<T> &in);
		size_t set(const T *in, size_t len);
		size_t set(const T *in);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		Reference_String<T> substring(size_t pos) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		Reference_String<T> substring(size_t pos, size_t length) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		static Reference_String<T> substring(const Jupiter::Readable_String<T> &in, size_t pos);
		static Reference_String<T> substring(const T *in, size_t pos);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		static Reference_String<T> substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t length);
		static Reference_String<T> substring(const T *in, size_t pos, size_t length);

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		Reference_String<T> getWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		*
		* @param in String to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static Reference_String<T> getWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		*
		* @param in C-Style string to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static Reference_String<T> getWord(const T *in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of the string, based on a token.
		*
		* @param pos Position of the token in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		Reference_String<T> getToken(size_t pos, const T &token) const;
		Reference_String<T> getToken(size_t pos, const Jupiter::Readable_String<T> &token) const;

		/**
		* @brief Creates a partial copy of an input string, based on a token.
		*
		* @param in String to get a partial copy of.
		* @param pos Position of the token in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		static Reference_String<T> getToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token);
		static Reference_String<T> getToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token);

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		Reference_String<T> gotoWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static Reference_String<T> gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param in C-Style string to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static Reference_String<T> gotoWord(const T *in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of the string, based on a token.
		*
		* @param pos Position in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		Reference_String<T> gotoToken(size_t pos, const T &token) const;
		Reference_String<T> gotoToken(size_t pos, const Jupiter::Readable_String<T> &token) const;

		/**
		* @brief Creates a partial copy of the string, based on a token.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param token Token to scan for.
		* @return String containing a partial copy of the original string.
		*/
		static Reference_String<T> gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token);
		static Reference_String<T> gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token);

		/**
		* @brief Tokenizes a string, based on an input token separator
		*
		* @param separator Separator to split tokens by
		* @return TokenizeResult containing the results of the tokenization process.
		*/
		typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> tokenize(const T &separator) const;
		typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> tokenize(const Jupiter::Readable_String<T> &separator) const;
		typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> tokenize(const T *separator, size_t separator_size) const;

		/**
		* @brief Tokenizes a string, based on an input token separator
		*
		* @param in String to split into tokens
		* @param separator Separator to split tokens by
		* @return TokenizeResult containing the results of the tokenization process.
		*/
		static typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> tokenize(const Jupiter::Readable_String<T> &in, const T &separator);
		static typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> tokenize(const Jupiter::Readable_String<T> &in, const Jupiter::Readable_String<T> &separator);
		static typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> tokenize(const Jupiter::Readable_String<T> &in, const T *separator, size_t separator_size);

		/** Mutative operators */
		inline Reference_String<T>& operator-=(size_t right) { this->truncate(right); return *this; };
		inline Reference_String<T>& operator=(const Readable_String<T> &right) { std::basic_string_view<T>::operator=({right.ptr(), right.size()}); return *this; };
		inline Reference_String<T>& operator=(const Reference_String<T> &right) = default;
		inline Reference_String<T>& operator=(const std::basic_string<T> &right) { std::basic_string_view<T>::operator=(right); return *this; };
		inline Reference_String<T>& operator=(const std::basic_string_view<T> &right) { std::basic_string_view<T>::operator=(right); return *this; };
		inline Reference_String<T>& operator=(const T *right) { std::basic_string_view<T>::operator=(right); return *this; };

		/**
		* @brief Default constructor for the Reference_String class.
		*/
		Reference_String() = default;

		/**
		* @brief Creates a reference to a C-Style string.
		* Difference from basic_string_view: checks nullptr
		*
		* @param in String to get a reference of.
		*/
		Reference_String(const T *in);

		/**
		* @brief Move constructor for the Reference_String class.
		*/
		Reference_String(Jupiter::Reference_String<T> &&source) = default;

		/**
		* @brief Constructor for the Refererence_String class to create a reference to an existing string.
		*/
		Reference_String(const Jupiter::Readable_String<T> &in);

		/**
		* @brief Copy constructor for the Reference_String class.
		*/
		Reference_String(const Jupiter::Reference_String<T> &in) = default;

		// Bring in constructors from basic_string_view
		using std::basic_string_view<T>::basic_string_view;

		static const Jupiter::Reference_String<T> empty; /** Empty instantiation of Reference_String */

		/** Methods to force disambiguation between bases until this class is removed entirely */
		using std::basic_string_view<T>::find;
		using std::basic_string_view<T>::operator[];
	};

	/** Generic Reference String Type */
	typedef Reference_String<char> ReferenceString;

	namespace literals {
		/** Reference_String literals */
		inline Jupiter::ReferenceString operator"" _jrs(const char *str, size_t len) { return Jupiter::ReferenceString(str, len); }
	}
}

/** DEPRECATED */
#define STRING_LITERAL_AS_REFERENCE(str) Jupiter::literals::operator"" _jrs(str, sizeof(str) - 1)
#define STRING_LITERAL_AS_NAMED_REFERENCE(name, str) Jupiter::ReferenceString name = STRING_LITERAL_AS_REFERENCE(str)

#include "Reference_String_Imp.h"

#endif // _REFERENCE_STRING_H_HEADER