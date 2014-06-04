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

#if !defined _REFERENCE_STRING_H_HEADER
#define _REFERENCE_STRING_H_HEADER

/**
 * @file Reference_String.h
 * @brief Provides the basis for String types, of any implementation.
 * Note: Some methods are commented out. This means that they should be implemented, but could not be put declared in this template (return of abstract type).
 */

#include "Readable_String.h"

namespace Jupiter
{

	/**
	* @brief Provides the basis for String classes by providing implementations for operators, comparative operations, and defining abstract functions.
	* Note: This is an abstract type.
	*
	* @param T Element type which the String will store. Defaults to char.
	*/
	template<typename T = char> class Reference_String : public Jupiter::Readable_String<T>
	{
	public:

		/**
		* @brief Fetches an element from the string.
		*
		* @param index Index of the element to return.
		* @return The element located at the specified index.
		*/
		const T &get(size_t index) const;

		/**
		* @brief Returns the number of elements in the String.
		*
		* @return Number of elements in the string.
		*/
		size_t size() const;

		/**
		* @brief Returns a pointer to the underlying string of elements.
		*
		* @return Pointer to the underlying string of elements.
		*/
		const T *ptr() const;

		/**
		* @brief Truncates the string by a specified number of elements.
		*
		* @param n Number of elements to remove from the tail.
		* @return New size of the String.
		*/
		virtual size_t truncate(size_t n);

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
		* @brief Default constructor for the Reference_String class.
		*/
		Reference_String();

		/**
		* @brief Creates a reference to a C-Style string.
		*
		* @param in String to get a reference of.
		*/
		Reference_String(const T *in);

		/**
		* @brief Creates a reference to a string.
		*
		* @param in String to get a reference of.
		* @param len Length of the string to track.
		*/
		Reference_String(const T *in, size_t len);

		/**
		* @brief Move constructor for the Reference_String class.
		*/
		Reference_String(Jupiter::Reference_String<T> &&source);

		/**
		* @brief Constructor for the Refererence_String class to create a reference to an existing string.
		*/
		Reference_String(const Jupiter::Readable_String<T> &in);

		/**
		* @brief Copy constructor for the Reference_String class.
		*/
		Reference_String(const Jupiter::Reference_String<T> &in);

		static const Jupiter::Reference_String<T> empty; /** Empty instantiation of Reference_String */

	protected:

		const T *str; /** Pointer for the underlying string of elements */
		size_t length; /** Number of representable elements in the string */
	};

	/** Generic Reference String Type */
	typedef Reference_String<char> ReferenceString;

	/** Generic Wide Reference String Type */
	typedef Reference_String<wchar_t> ReferenceWString;

}

#define STRING_LITERAL_AS_REFERENCE(str) Jupiter::ReferenceString(str, sizeof(str) - 1)

#include "Reference_String_Imp.h"

#endif // _REFERENCE_STRING_H_HEADER