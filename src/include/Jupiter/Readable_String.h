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
#include "Functions.h"

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
		* @brief Returns a pointer to the underlying string of elements.
		*
		* @return Pointer to the underlying string of elements.
		*/
		virtual const T *ptr() const = 0;  // RENAME; 'data'

		/**
		* @brief Returns the number of elements in the String.
		*
		* @return Number of elements in the string.
		*/
		virtual size_t size() const = 0; // KEEP

		/**
		* @brief Checks if the String is empty.
		*
		* @return True if the String is empty, false otherwise.
		*/
		bool empty() const { return size() == 0; }; // KEEP

		/**
		* @brief Checks if the String is not empty.
		*
		* @return True if the String is not empty, false otherwise.
		*/
		virtual bool isNotEmpty() const { return !empty(); };  // REMOVE

		/**
		* @brief Returns the index of the first element in the string with the specified value.
		*
		* @param value Value of the element to search for.
		* @param index Index of the match to return (i.e: 0 returns the first match).
		* @return The index of an element if one is found, INVALID_INDEX otherwise.
		*/
		size_t find(const T &value, size_t index = 0) const; // KEEP
		size_t find(const Readable_String<T> &in) const; // KEEP

		/**
		* @brief Checks if the strings are equal.
		* Note: Case insensitive. Returns false for any type other than char and wchar_t.
		*
		* @param in String to compare against.
		* @return True if the contents of the strings are equal, false otherwise.
		*/
		bool equalsi(const Readable_String<T> &in) const; // REMOVE
		bool equalsi(const std::basic_string<T> &in) const; // REMOVE
		bool equalsi(const T *in, size_t len) const; // REMOVE
		bool equalsi(const T &in) const; // REMOVE

		/**
		* @brief Destructor for the Readable_String class.
		*/
		virtual ~Readable_String() = default;

		/** Access operator */
		inline const T &operator[](size_t index) const { return this->ptr()[index]; };

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
		explicit inline operator std::basic_string<T>() const { return std::basic_string<T>(this->ptr(), this->size()); }
		inline operator std::basic_string_view<T>() const { return std::basic_string_view<T>(this->ptr(), this->size()); }
	};

	/** Generic Readable String Type */
	typedef Readable_String<char> ReadableString;

	// TEMP; create some equivalent in jessilib
	template<typename CharT>
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

	template<typename CharT>
	int asInt(std::basic_string_view<CharT> in_string, int base = 0) {
		return Jupiter_strtoi_s(in_string.data(), in_string.size(), base);
	}

	template<typename CharT>
	long long asLongLong(std::basic_string_view<CharT> in_string, int base = 0) {
		return Jupiter_strtoll_s(in_string.data(), in_string.size(), base);
	}

	template<typename CharT>
	unsigned int asUnsignedInt(std::basic_string_view<CharT> in_string, int base = 0) {
		return Jupiter_strtoui_s(in_string.data(), in_string.size(), base);
	}

	template<typename CharT>
	unsigned long long asUnsignedLongLong(std::basic_string_view<CharT> in_string, int base = 0) {
		return Jupiter_strtoull_s(in_string.data(), in_string.size(), base);
	}

	template<typename CharT>
	double asDouble(std::basic_string_view<CharT> in_string) {
		return Jupiter_strtod_s(in_string.data(), in_string.size());
	}

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
}

#include "Readable_String_Imp.h"

#endif // _READABLE_STRING_H_HEADER