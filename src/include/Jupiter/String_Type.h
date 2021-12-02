/**
 * Copyright (C) 2013-2015 Jessica James.
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

#if !defined _STRING_TYPE_H_HEADER
#define _STRING_TYPE_H_HEADER

/**
 * @file String_Type.h
 * @brief Provides the basis for String types, of any implementation.
 * Note: Some methods are commented out. This means that they should be implemented, but could not be put declared in this template (return of abstract type).
 */

#include <cstdarg> // va_list
#include "Readable_String.h"

/** Disable warning 4458 */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4458) // declaration of 'length' hides class member
#endif

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
		* @brief Returns the number of elements in the String.
		*
		* @return Number of elements in the string.
		*/
		size_t size() const;

		/**
		* @brief Returns the maximum number of elements the String can contain,
		* without expanding the socket buffer. This is generally the size of the
		* underlying memory buffer.
		*
		* @return Number of elements the string can contain without reallocation.
		*/
		virtual size_t capacity() const;

		/**
		* @brief Returns a pointer to the underlying string of elements.
		*
		* @return Pointer to the underlying string of elements.
		*/
		const T *data() const;

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
		* @brief Removes a number of elements starting at an index.
		*
		* @param index Index to start removing elements at.
		* @param length Number of elements to remove.
		*/
		virtual void remove(size_t index, size_t length);

		/**
		* @brief Removes all elements from the string.
		*/
		virtual void clear();

		/**
		* @brief Sets the value of an element at the specified index.
		* Note: If the index is not in the string, it will be added.
		*
		* @param index Index of element to replace.
		* @param in Value to set element to.
		* @return New size of the String.
		*/
		virtual size_t set(size_t index, const T &in);
		virtual size_t set(size_t index, const T *in, size_t inSize);
		virtual size_t set(size_t index, const Jupiter::Readable_String<T> &in);
		virtual size_t set(size_t index, const std::basic_string<T> &in);
		virtual size_t set(size_t index, const T *in);
		
		/**
		* @brief Copies the data from the input string to the String.
		*
		* @param in String containing the data to be copied.
		* @return New size of the String.
		*/
		virtual size_t set(const T *in, size_t inSize);
		virtual size_t set(const Jupiter::Readable_String<T> &in);
		virtual size_t set(const std::basic_string<T> &in);
		virtual size_t set(const T *in);
		virtual size_t set(const T &in);

		/**
		* @brief Inserts data into a position in the string.
		*
		* @param index Index to insert data to.
		* @param value Value to insert.
		*/
		virtual size_t insert(size_t index, const T &value);
		virtual size_t insert(size_t index, const Jupiter::Readable_String<T> &value);

		/**
		* @brief Replaces data at an index with a specified value.
		*
		* @param index Index to start replacing at.
		* @param length Number of elements to replace.
		* @param value Value to write over the elements.
		* @return New size of the string.
		*/
		virtual size_t replace(size_t index, size_t length, const T &value);
		virtual size_t replace(size_t index, size_t length, const T *value, size_t valueSize);
		virtual size_t replace(size_t index, size_t length, std::basic_string_view<T> value);

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

		virtual size_t replace(const T *target, size_t targetSize, const T *value, size_t valueSize);
		virtual size_t replace(const T *target, size_t targetSize, const Jupiter::Readable_String<T> &value);
		virtual size_t replace(const Jupiter::Readable_String<T> &target, const T *value, size_t valueSize);
		virtual size_t replace(std::basic_string_view<T> target, std::basic_string_view<T> value);

		/**
		* @brief Copies the data from the input string and concatenates it to the end of String.
		*
		* @param in String containing the data to be concatenated.
		* @return New size of the String.
		*/
		virtual size_t concat(const T *in, size_t inSize);
		virtual size_t concat(const Jupiter::Readable_String<T> &in);
		virtual size_t concat(const std::basic_string<T> &in);
		virtual size_t concat(const T *in);
		virtual size_t concat(const T &in);

		/**
		* @brief Copies a part of an input string and returns it in an output type.
		*
		* @param R Type to return. Must be a subclass of String_Type.
		*
		* @param in String to get a partial copy of.
		* @param pos Position to start copying from.
		* @return Partial copy of the input string.
		*/
		template<template<typename> class R> static R<T> substring(const Jupiter::Readable_String<T> &in, size_t pos); // REPLACE
		template<template<typename> class R> static R<T> substring(const T *in, size_t pos); // REPLACE

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
		template<template<typename> class R> static R<T> substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t len); // REPLACE
		template<template<typename> class R> static R<T> substring(const T *in, size_t pos, size_t len); // REPLACE

		/** Mutative operators */
		inline String_Type<T> &operator+=(const String_Type<T> &right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const std::basic_string<T> &right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const std::basic_string_view<T> &right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const T *right) { this->concat(right); return *this; };
		inline String_Type<T> &operator+=(const T right) { this->concat(right); return *this; };
		inline String_Type<T> &operator-=(size_t right) { this->truncate(right); return *this; };
		inline String_Type<T> &operator=(const String_Type<T> &right) { this->set(right); return *this; };
		inline String_Type<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline String_Type<T> &operator=(const std::basic_string_view<T> &right) { this->set(right.data(), right.size()); return *this; };
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
		* A conversion constructor for Jupiter::DataBuffer
		*/

		/**
		* @brief Destructor for the String_Type class.
		*/
		virtual ~String_Type() = default;

		/**
		 * Necessities when removing Readable_String & Reference_String
		 */
		bool empty() const { return size() == 0; }; // KEEP

		/** Access operator */
		inline const T &operator[](size_t index) const { return this->data()[index]; };

		/** Conversion operators */
		explicit inline operator std::basic_string<T>() const { return std::basic_string<T>(this->data(), this->size()); }
		inline operator std::basic_string_view<T>() const { return std::basic_string_view<T>(this->data(), this->size()); }

		/** Comparative operators */
		inline bool operator==(const String_Type<T>& right)const{ return operator==(std::basic_string_view<T>{right}); }
		inline bool operator==(const std::basic_string<T>& right)const{ return operator==(std::basic_string_view<T>{right}); }
		inline bool operator==(const std::basic_string_view<T>& right)const{ return std::basic_string_view<T>(data(), size()) == right; }
		inline bool operator==(const T right)const{ return this->size() == 1 && this->get(0) == right; }
		inline bool operator==(std::nullptr_t) = delete;
		inline bool operator!=(const String_Type<T> &right)const{ return !operator==(right); }
		inline bool operator!=(const std::basic_string<T> &right)const{ return !operator==(right); }
		inline bool operator!=(const T right)const{ return !operator==(right); }
		inline bool operator!=(std::nullptr_t) = delete;
		inline bool operator<(const String_Type<T> &right)const{ return this->compare(right) < 0; }
		inline bool operator<(const std::basic_string<T> &right)const{ return this->compare(right) < 0; }
		inline bool operator<(const T right)const{ return this->compare(right) < 0; }
		inline bool operator>(const String_Type<T> &right)const{ return  this->compare(right) > 0; }
		inline bool operator>(const std::basic_string<T> &right)const{ return  this->compare(right) > 0; }
		inline bool operator>(const T right)const{ return  this->compare(right) > 0; }
		inline bool operator<=(const String_Type<T> &right)const{ return !operator>(right); }
		inline bool operator<=(const std::basic_string<T> &right)const{ return !operator>(right); }
		inline bool operator<=(const T right)const{ return !operator>(right); }
		inline bool operator>=(const String_Type<T> &right)const{ return !operator<(right); }
		inline bool operator>=(const std::basic_string<T> &right)const{ return !operator<(right); }
		inline bool operator>=(const T right)const{ return !operator<(right); }

	protected: // Things which cannot be removed right now
		T *str{}; /** Pointer for the underlying string of elements */
		size_t length{}; /** Number of representable elements in the string */

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
	};

	namespace literals {
		// Not truly a literal, but I just want this available in places I'm using literals
		template<typename T>
		static inline std::basic_string<T> operator+(std::basic_string<T> lhs, std::basic_string_view<T> rhs) {
			lhs += rhs;
			return lhs;
		}

		template<typename T>
		static inline std::basic_string<T> operator+(std::basic_string<T> lhs, const String_Type<T>& rhs) {
			lhs += std::basic_string_view<T>{rhs};
			return lhs;
		}
	}

	/** Generic String Type */
	typedef String_Type<char> StringType;
}

/** Re-enable warning */
#if defined _MSC_VER
#pragma warning(pop)
#endif

/** Implementation for String_Type. */
#include "String_Type_Imp.h"

#endif // _STRING_TYPE_H_HEADER