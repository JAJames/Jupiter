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

#if !defined _READABLE_STRING_H_HEADER
#define _READABLE_STRING_H_HEADER

/**
 * @file Readable_String.h
 * @brief Defines several basic accessive and comparative virtual functions for strings.
 */

#include <cwchar> // wchar_t
#include <cwctype> // towupper
#include <cstdio> // FILE
#include <string> // std::basic_string<T> type
#include <iostream> // std::endl
#include "InvalidIndex.h"
#include "DataBuffer.h"

namespace Jupiter
{

	/**
	* @brief Provides the basis for String classes by providing implementations for operators, comparative operations, and defining abstract functions.
	* Note: This is an abstract type.
	*
	* @param T Element type which the String will store. Defaults to char.
	*/
	template<typename T = char> class Readable_String
	{
	public:

		/**
		* @brief Fetches an element from the string.
		*
		* @param index Index of the element to return.
		* @return The element located at the specified index.
		*/
		virtual const T &get(size_t index) const = 0;

		/**
		* @brief Returns a pointer to the underlying string of elements.
		*
		* @return Pointer to the underlying string of elements.
		*/
		virtual const T *ptr() const = 0;

		/**
		* @brief Returns the number of elements in the String.
		*
		* @return Number of elements in the string.
		*/
		virtual size_t size() const = 0;

		/**
		* @brief Checks if the String is empty.
		*
		* @return True if the String is empty, false otherwise.
		*/
		bool isEmpty() const { return size() == 0; };
		bool empty() const { return size() == 0; };

		/**
		* @brief Checks if the String is not empty.
		*
		* @return True if the String is not empty, false otherwise.
		*/
		virtual bool isNotEmpty() const { return !empty(); };

		/**
		* @brief Checks if the string contains an element with the specified value.
		*
		* @param value Value of the element to search for.
		* @return True if a match is found, false otherwise.
		*/
		bool contains(const T &value) const;

		/**
		* @brief Returns the index of the first element in the string with the specified value.
		*
		* @param value Value of the element to search for.
		* @param index Index of the match to return (i.e: 0 returns the first match).
		* @return The index of an element if one is found, INVALID_INDEX otherwise.
		*/
		size_t find(const T &value, size_t index = 0) const;
		size_t find(const Readable_String<T> &in) const;

		/**
		* @brief Returns the number of elements of the string which match the input string.
		*
		* @param in Character set to match against.
		* @return Number of elements at the start of the string that match the character set.
		*/
		size_t span(const Readable_String<T> &in) const;
		size_t span(const T *str) const;
		size_t span(const T &in) const;

		/**
		* @brief Checks if the strings are equal.
		* Note: Case insensitive. Returns false for any type other than char and wchar_t.
		*
		* @param in String to compare against.
		* @return True if the contents of the strings are equal, false otherwise.
		*/
		bool equalsi(const Readable_String<T> &in) const;
		bool equalsi(const std::basic_string<T> &in) const;
		bool equalsi(const T *in, size_t len) const;
		bool equalsi(const T &in) const;

		/**
		* @brief Checks if the String matches a wildcard format.
		* Note: Case sensitive.
		*
		* @param format Format that the string is compared against.
		* @return True if the String matches the wildcard format, false otherwise.
		*/
		bool match(const Readable_String<T> &format) const;
		bool match(const std::basic_string<T> &format) const;
		bool match(const T *format, size_t formatSize) const;
		bool match(const T *format) const;

		/**
		* @brief Checks if the CString matches a wildcard format.
		* Note: Case insensitive. Returns false for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @return True if the CString matches the wildcard format, false otherwise.
		*/
		bool matchi(const Readable_String<T> &format) const;
		bool matchi(const std::basic_string<T> &format) const;
		bool matchi(const T *format, size_t formatSize) const;
		bool matchi(const T *format) const;

		/**
		* @brief Counts the number of token deliminated words.
		*
		* @param whitespace A string of tokens used to deliminate words.
		* @return Number of words found.
		*/
		unsigned int wordCount(const T *whitespace) const;

		/**
		* @brief Interprets the string as a bool.
		*
		* @return Bool interpretation of the string.
		*/
		bool asBool() const;

		/**
		* @brief Interprets the string as an integer.
		* Note: This returns 0 on any value string type other than char.
		*
		* @param base Base of the string representation.
		* @return Integer representation of the string.
		*/
		int asInt(int base = 0) const;
		long long asLongLong(int base = 0) const;
		unsigned int asUnsignedInt(int base = 0) const;
		unsigned long long asUnsignedLongLong(int base = 0) const;

		/**
		* @brief Interprets the string as a floating-point decimal number.
		* Note: This returns 0 on any value string type other than char.
		*
		* @param base Base of the string representation.
		* @return Integer representation of the string.
		*/
		double asDouble() const;

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
		* @brief Copies a "word" from an input string and returns it in an output type.
		*
		* @param R Type to return. Must be a subclass of String_Type.
		*
		* @param in String to get a partial copy of.
		* @param pos Index of the word to copy.
		* @param whitespace String of characters that are to be considered whitespace.
		* @return Copy of the word at the specified index on success, an empty string otherwise.
		*/
		template<template<typename> class R> static R<T> getWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);
		template<template<typename> class R> static R<T> getWord(const T *in, size_t pos, const T *whitespace);

		/**
		* @brief Copies a part of an input string starting at a specified "word" and returns it in an output type.
		*
		* @param R Type to return. Must be a subclass of String_Type.
		*
		* @param in String to get a partial copy of.
		* @param pos Index of the word to start copying from.
		* @param whitespace String of characters that are to be considered whitespace.
		* @return Copy of the string starting at the specified word on success, an empty string otherwise.
		*/
		template<template<typename> class R> static R<T> gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);
		template<template<typename> class R> static R<T> gotoWord(const T *in, size_t pos, const T *whitespace);

		/**
		* @brief Destructor for the Readable_String class.
		*/
		virtual ~Readable_String() = default;

		/** Access operator */
		inline const T &operator[](size_t index) const { return this->get(index); };

		/** Comparative operators */
		inline bool operator==(const Readable_String<T>& right)const{ return operator==(std::basic_string_view<T>{right}); }
		inline bool operator==(const std::basic_string<T>& right)const{ return operator==(std::basic_string_view<T>{right}); }
		inline bool operator==(const std::basic_string_view<T>& right)const{ return std::basic_string_view<T>(ptr(), size()) == right; }
		inline bool operator==(const T right)const{ return this->size() == 1 && this->get(0) == right; }
		inline bool operator==(std::nullptr_t) = delete;
		inline bool operator!=(const Readable_String<T> &right)const{ return !operator==(right); }
		inline bool operator!=(const std::basic_string<T> &right)const{ return !operator==(right); }
		inline bool operator!=(const T right)const{ return !operator==(right); }
		inline bool operator!=(std::nullptr_t) = delete;
		inline bool operator<(const Readable_String<T> &right)const{ return this->compare(right) < 0; }
		inline bool operator<(const std::basic_string<T> &right)const{ return this->compare(right) < 0; }
		inline bool operator<(const T right)const{ return this->compare(right) < 0; }
		inline bool operator>(const Readable_String<T> &right)const{ return  this->compare(right) > 0; }
		inline bool operator>(const std::basic_string<T> &right)const{ return  this->compare(right) > 0; }
		inline bool operator>(const T right)const{ return  this->compare(right) > 0; }
		inline bool operator<=(const Readable_String<T> &right)const{ return !operator>(right); }
		inline bool operator<=(const std::basic_string<T> &right)const{ return !operator>(right); }
		inline bool operator<=(const T right)const{ return !operator>(right); }
		inline bool operator>=(const Readable_String<T> &right)const{ return !operator<(right); }
		inline bool operator>=(const std::basic_string<T> &right)const{ return !operator<(right); }
		inline bool operator>=(const T right)const{ return !operator<(right); }

		/** Conversion operators */
		explicit inline operator bool() const { return this->asBool(); }
		explicit inline operator signed char() const { return static_cast<signed char>(this->asInt()); }
		explicit inline operator unsigned char() const { return static_cast<unsigned char>(this->asUnsignedInt()); }
		explicit inline operator short() const { return static_cast<short>(this->asInt()); }
		explicit inline operator unsigned short() const { return static_cast<unsigned short>(this->asUnsignedInt()); }
		explicit inline operator int() const { return this->asInt(); }
		explicit inline operator unsigned int() const { return this->asUnsignedInt(); }
		explicit inline operator long() const { return static_cast<long>(this->asLongLong()); }
		explicit inline operator unsigned long() const { return static_cast<unsigned long>(this->asLongLong()); }
		explicit inline operator long long() const { return this->asLongLong(); }
		explicit inline operator unsigned long long() const { return this->asUnsignedLongLong(); }
		explicit inline operator float() const { return static_cast<float>(this->asDouble()); }
		explicit inline operator double() const { return this->asDouble(); }
		explicit inline operator long double() const { return this->asDouble(); } // NEEDS TO NOT CAST FROM DOUBLE
		explicit inline operator std::basic_string<T>() const { return std::basic_string<T>(this->ptr(), this->size()); }
		inline operator std::basic_string_view<T>() const { return std::basic_string_view<T>(this->ptr(), this->size()); }
	};

	/** Generic Readable String Type */
	typedef Readable_String<char> ReadableString;
}

#include "Readable_String_Imp.h"

#endif // _READABLE_STRING_H_HEADER