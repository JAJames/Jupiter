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

#include "Jupiter.h"
#include <string> // std::basic_string<T> type
#include <cstdarg> // va_list
#include <cwchar>
#include <cstdio>

namespace Jupiter
{

	/**
	* @brief Provides the basis for String classes by providing implementations for operators, comparative operations, and defining abstract functions.
	* Note: This is an abstract type.
	*
	* @param T Element type which the String will store. Defaults to char.
	*/
	template<typename T = char> class String_Type
	{
	public:

		/**
		* @brief Fetches an element from the string.
		*
		* @param index Index of the element to return.
		* @return The element located at the specified index.
		*/
		T &get(size_t index) const;

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
		* @brief Checks if the string contains an element with the specified value.
		*
		* @param value Value of the element to search for.
		* @return True if a match is found, false otherwise.
		*/
		bool contains(const T &value) const;

		/**
		* @brief Compares another string against the String.
		*
		* @param in String to compare against.
		* @return 0 if the strings are equal, negative if the first mismatched character is greater in the String, or positive if it's less.
		*/
		int compare(const String_Type<T> &in) const;
		int compare(const std::basic_string<T> &in) const;
		int compare(const T *in) const;
		int compare(const T &in) const;
		int compare(const std::nullptr_t) const;

		/**
		* @brief Checks if the strings are equal.
		* Note: Case sensitive.
		*
		* @param in String to compare against.
		* @return True if the contents of the strings are equal, false otherwise.
		*/
		bool equals(const String_Type<T> &in) const;
		bool equals(const std::basic_string<T> &in) const;
		bool equals(const T *in) const;
		bool equals(const T &in) const;
		bool equals(const std::nullptr_t) const;

		/**
		* @brief Checks if the strings are equal.
		* Note: Case insensitive. Returns false for any type other than char and wchar_t.
		*
		* @param in String to compare against.
		* @return True if the contents of the strings are equal, false otherwise.
		*/
		bool equalsi(const String_Type<T> &in) const;
		bool equalsi(const std::basic_string<T> &in) const;
		bool equalsi(const T *in) const;
		bool equalsi(const T &in) const;
		bool equalsi(const std::nullptr_t) const;

		/**
		* @brief Checks if the String matches a wildcard format.
		* Note: Case sensitive.
		*
		* @param format Format that the string is compared against.
		* @return True if the String matches the wildcard format, false otherwise.
		*/
		bool match(const String_Type<T> &format) const;
		bool match(const std::basic_string<T> &format) const;
		bool match(const T *format) const;

		/**
		* @brief Checks if the CString matches a wildcard format.
		* Note: Case insensitive. Returns false for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @return True if the CString matches the wildcard format, false otherwise.
		*/
		bool matchi(const String_Type<T> &format) const;
		bool matchi(const std::basic_string<T> &format) const;
		bool matchi(const T *format) const;

		/**
		* @brief Counts the number of token deliminated words.
		*
		* @param whitespace A string of tokens used to deliminate words.
		* @return Number of words found.
		*/
		unsigned int wordCount(const T *whitespace) const;

		/**
		* @brief Interprets the string as an integer.
		* Note: This returns 0 on any value string type other than char.
		*
		* @param base Base of the string representation.
		* @return Integer representation of the string.
		*/
		int asInt(int base = 0) const;

		/**
		* @brief Interprets the string as an integer.
		* Note: This returns 0 on any value string type other than char.
		*
		* @param base Base of the string representation.
		* @return Integer representation of the string.
		*/
		unsigned int asUnsignedInt(int base = 0) const;

		/**
		* @brief Returns a C-Style string version of the String.
		*
		* @return C-Style string representation of the String.
		*/
		virtual const T *c_str() const = 0;

		/**
		* @brief Outputs the string to a FILE stream.
		*
		* @param out Stream to output to.
		* @return Number of elements written successfully.
		*/
		size_t print(FILE *out) const;
		size_t print(std::basic_ostream<T> &out) const;

		/**
		* @brief Outputs the string and a newline to a FILE stream
		*
		* @param out Stream to output to.
		* @param Number of elements written successfully.
		*/
		size_t println(FILE *out) const;
		size_t println(std::basic_ostream<T> &out) const;

		/**
		* @brief Sets the CString's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t format(const String_Type<T> &format, ...);
		size_t format(const std::basic_string<T> &format, ...);
		size_t format(const T *format, ...);
		virtual size_t vformat(const T *format, va_list args) = 0;

		/**
		* @brief Appends to a CString's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t aformat(const String_Type<T> &format, ...);
		size_t aformat(const std::basic_string<T> &format, ...);
		size_t aformat(const T *format, ...);
		virtual size_t avformat(const T *format, va_list args) = 0;

		/**
		* @brief Copies the data from the input string to the String.
		*
		* @param in String containing the data to be copied.
		* @return New size of the String.
		*/
		virtual size_t set(const String_Type<T> &in) = 0;
		virtual size_t set(const std::basic_string<T> &in) = 0;
		virtual size_t set(const T *in) = 0;
		virtual size_t set(const T in) = 0;

		/**
		* @brief Copies the data from the input string and concatenates it to the end of String.
		*
		* @param in String containing the data to be concatenated.
		* @return New size of the CString.
		*/
		virtual size_t concat(const String_Type<T> &in) = 0;
		virtual size_t concat(const std::basic_string<T> &in) = 0;
		virtual size_t concat(const T *in) = 0;
		virtual size_t concat(const T in) = 0;

		/**
		* @brief Truncates the string by a specified number of elements.
		*
		* @param n Number of elements to remove from the tail.
		* @return New size of the String.
		*/
		virtual size_t truncate(size_t n) = 0;

		/** Access operator */
		inline T &operator[](size_t index) { return Jupiter::String_Type<T>::get(index); };

		// Mutative operators.
		// Note: All extending classes must overload operator= for its own type.
		inline String_Type<T> &operator+=(const String_Type<T> &right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const std::basic_string<T> &right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const T *right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const T right) { this->concat(right); return *this; };
		inline String_Type<T> &operator-=(size_t right) { this->truncate(right); return *this; };
		inline String_Type<T> &operator=(const String_Type<T> &right) { this->set(right); return *this; };
		inline String_Type<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline String_Type<T> &operator=(const T *right) { this->set(right); return *this; };
		inline String_Type<T> &operator=(const T right) { this->set(right); return *this; };

		// Comparative operators.
		inline bool operator==(const String_Type<T> &right)const{ return this->equals(right); }
		inline bool operator==(const std::basic_string<T> &right)const{ return this->equals(right); }
		inline bool operator==(const T *right)const{ return this->equals(right); }
		inline bool operator==(const T right)const{ return this->equals(right); }
		inline bool operator!=(const String_Type<T> &right)const{ return !operator==(right); }
		inline bool operator!=(const std::basic_string<T> &right)const{ return !operator==(right); }
		inline bool operator!=(const T *right)const{ return !operator==(right); }
		inline bool operator!=(const T right)const{ return !operator==(right); }
		inline bool operator<(const String_Type<T> &right)const{ return this->compare(right) < 0; }
		inline bool operator<(const std::basic_string<T> &right)const{ return this->compare(right) < 0; }
		inline bool operator<(const T *right)const{ return this->compare(right) < 0; }
		inline bool operator<(const T right)const{ return this->compare(right) < 0; }
		inline bool operator>(const String_Type<T> &right)const{ return  this->compare(right) > 0; }
		inline bool operator>(const std::basic_string<T> &right)const{ return  this->compare(right) > 0; }
		inline bool operator>(const T *right)const{ return  this->compare(right) > 0; }
		inline bool operator>(const T right)const{ return  this->compare(right) > 0; }
		inline bool operator<=(const String_Type<T> &right)const{ return !operator>(right); }
		inline bool operator<=(const std::basic_string<T> &right)const{ return !operator>(right); }
		inline bool operator<=(const T *right)const{ return !operator>(right); }
		inline bool operator<=(const T right)const{ return !operator>(right); }
		inline bool operator>=(const String_Type<T> &right)const{ return !operator<(right); }
		inline bool operator>=(const std::basic_string<T> &right)const{ return !operator<(right); }
		inline bool operator>=(const T *right)const{ return !operator<(right); }
		inline bool operator>=(const T right)const{ return !operator<(right); }

		/**
		* The following constructors should exist:
		* A default constructor
		* A copy constructor for the same class.
		* A copy constructor for String_Type.
		* A copy constructor for std::basic_string<T>.
		* A copy constructor for C-style strings.
		*/

	protected:
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