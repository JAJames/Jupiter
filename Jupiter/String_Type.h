/**
 * Copyright (C) 2013-2014 Justin James.
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

#if !defined _STRING_TYPE_H_HEADER
#define _STRING_TYPE_H_HEADER

/**
 * @file String_Type.h
 * @brief Provides the basis for String types, of any implementation.
 * Note: Some methods are commented out. This means that they should be implemented, but could not be put declared in this template (return of abstract type).
 */

#include <cstdarg> // va_list
#include "Readable_String.h"

namespace Jupiter
{

	/**
	* @brief Provides the basis for String classes by providing implementations for operators, comparative operations, and defining abstract functions.
	* Note: This is an abstract type.
	*
	* @param T Element type which the String will store. Defaults to char.
	*/
	template<typename T = char> class String_Type : public Jupiter::Readable_String<T>
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
		* @brief Sets the String's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t format(const std::basic_string<T> &format, ...);
		size_t format(const T *format, ...);
		virtual size_t vformat(const T *format, va_list args) = 0;

		/**
		* @brief Appends to a String's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t aformat(const std::basic_string<T> &format, ...);
		size_t aformat(const T *format, ...);
		virtual size_t avformat(const T *format, va_list args) = 0;

		/**
		* @brief Truncates the string by a specified number of elements.
		*
		* @param n Number of elements to remove from the tail.
		* @return New size of the String.
		*/
		virtual size_t truncate(size_t n);

		/**
		* @brief Removes the first instance of an element from the string.
		*
		* @param value Value of the element to remove.
		* @return True if an element was removed, false otherwise.
		*/
		virtual bool remove(const T &value);

		/**
		* @brief Copies the data from the input string to the String.
		*
		* @param in String containing the data to be copied.
		* @return New size of the String.
		*/
		virtual size_t set(const Jupiter::Readable_String<T> &in);
		virtual size_t set(const std::basic_string<T> &in);
		virtual size_t set(const T *in);
		virtual size_t set(const T in);

		/**
		* @brief Inserts data into a position in the string.
		*
		* @param index Index to insert data to.
		* @param value Value to insert.
		*/
		virtual size_t insert(size_t index, const T &value);
		virtual size_t insert(size_t index, const Jupiter::Readable_String<T> &value);

		/**
		* @brief Replaces all instances of one value with another value.
		*
		* @param target Target to search for and replace.
		* @param value Value to replace the target with.
		* @return New size of the string.
		*/
		virtual size_t replace(const T &target, const T &value);
		virtual size_t replace(const T *target, size_t targetSize, const T &value);
		virtual size_t replace(const Jupiter::Readable_String<T> &target, const T &value);

		//virtual size_t replace(const T &target, const T *value, size_t valueSize);
		//virtual size_t replace(const T &target, const Jupiter::Readable_String<T> &value);

		virtual size_t replace(const T *target, size_t targetSize, const T *value, size_t valueSize);
		virtual size_t replace(const T *target, size_t targetSize, const Jupiter::Readable_String<T> &value);
		virtual size_t replace(const Jupiter::Readable_String<T> &target, const T *value, size_t valueSize);
		virtual size_t replace(const Jupiter::Readable_String<T> &target, const Jupiter::Readable_String<T> &value);

		/**
		* @brief Copies the data from the input string and concatenates it to the end of String.
		*
		* @param in String containing the data to be concatenated.
		* @return New size of the String.
		*/
		virtual size_t concat(const Jupiter::Readable_String<T> &in);
		virtual size_t concat(const std::basic_string<T> &in);
		virtual size_t concat(const T *in);
		virtual size_t concat(const T in);

		/**
		* @brief Copies a part of an input string and returns it in an output type.
		*
		* @param R Type to return. Must be a subclass of String_Type.
		*
		* @param in String to get a partial copy of.
		* @param pos Position to start copying from.
		* @return Partial copy of the input string.
		*/
		template<template<typename> class R> static R<T> substring(const Jupiter::Readable_String<T> &in, size_t pos);
		template<template<typename> class R> static R<T> substring(const T *in, size_t pos);

		/**
		* @brief Copies a part of an input string and returns it in an output type.
		*
		* @param R Type to return. Must be a subclass of String_Type.
		*
		* @param in String to get a partial copy of.
		* @param pos Position to start copying from.
		* @param len Number of elements to copy.
		* @return Partial copy of the input string.
		*/
		template<template<typename> class R> static R<T> substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t len);
		template<template<typename> class R> static R<T> substring(const T *in, size_t pos, size_t len);

		/** Mutative operators */
		inline String_Type<T> &operator+=(const Readable_String<T> &right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const String_Type<T> &right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const std::basic_string<T> &right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const T *right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const T right) { this->concat(right); return *this; };
		inline String_Type<T> &operator-=(size_t right) { this->truncate(right); return *this; };
		inline String_Type<T> &operator=(const Readable_String<T> &right) { this->set(right); return *this; };
		inline String_Type<T> &operator=(const String_Type<T> &right) { this->set(right); return *this; };
		inline String_Type<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline String_Type<T> &operator=(const T *right) { this->set(right); return *this; };
		inline String_Type<T> &operator=(const T right) { this->set(right); return *this; };

		/**
		* @brief Default constructor for the String_Type class.
		*/
		String_Type() {}

		/**
		* @brief Move constructor for the String_Type class.
		*/
		String_Type(Jupiter::String_Type<T> &&source);

		/**
		* The following constructors should exist:
		* A default constructor
		* A copy constructor for the same class.
		* A copy constructor for Readable_String.
		* A copy constructor for std::basic_string<T>.
		* A copy constructor for C-style strings.
		* A copy constructor for memory array strings.
		*/

	protected:

		/**
		* @brief Sets the internal buffer to be at least large enough to old a specified number of elements.
		* Note: This does nothing if len is less than the string's current length.
		*
		* @param len Minimum number of elements the string buffer must be able to hold.
		* @return True if a new buffer was allocated, false otherwise.
		*/
		virtual bool setBufferSize(size_t len) = 0;

		/**
		* @brief Empties the string, and sets the internal buffer to be at least large enough to old a specified number of elements.
		* Note: This does nothing if len is less than the string's current length.
		*
		* @param len Minimum number of elements the string buffer must be able to hold.
		* @return True if a new buffer was allocated, false otherwise.
		*/
		virtual bool setBufferSizeNoCopy(size_t len) = 0;

		T *str; /** Pointer for the underlying string of elements */
		size_t length; /** Number of representable elements in the string */
	};

	/** Generic String Type */
	typedef String_Type<char> StringType;

	/** Generic Wide String Type */
	typedef String_Type<wchar_t> WStringType;

}

/** Implementation for String_Type. */
#include "String_Type_Imp.h"

#endif // _STRING_TYPE_H_HEADER