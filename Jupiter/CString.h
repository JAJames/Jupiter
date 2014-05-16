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

#if !defined _CSTRING_H_HEADER
#define _CSTRING_H_HEADER

#include <stdarg.h>
#include "String_Type.h"

/**
 * @file CString.h
 * @brief Defines the base CString_Type, as well as a series of CString types.
 * Note: Functions which take "case" or "wildcards" into consideration will only function
 * for types char and wchar_t; inputs with other types will simply return false.
 */

namespace Jupiter
{
	/**
	* @brief Provides the basis for CString classes by providing the implementations for many abstract methods in String_Type.
	* Note: This is an abstract type.
	*
	* @param T Element type which the CString will store. Defaults to char.
	*/
	template<typename T = char> class CString_Type : public String_Type<T>
	{
	public:

		/**
		* @brief Returns the C-Style string behind the CString.
		*
		* @return C-Style string that the CString represents.
		*/
		const T *c_str() const;

		/**
		* @brief Returns the number of elements in the CString.
		*
		* @return Number of elements in the string.
		*/
		size_t size() const;

		/**
		* @brief Compares another string against the CString.
		*
		* @param in String to compare against.
		* @return 0 if the strings are equal, negative if the first mismatched character is greater in the CString, or positive if it's less.
		*/
		int compare(const String_Type<T> &in) const;
		int compare(const std::basic_string<T> &in) const;
		int compare(const T *in) const;
		int compare(const T in) const;

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
		bool equals(const T in) const;

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
		bool equalsi(const T in) const;

		/**
		* @brief Checks if the CString matches a wildcard format.
		* Note: Case sensitive. Returns false for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @return True if the CString matches the wildcard format, false otherwise.
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
		* @brief Sets the CString's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		virtual size_t format(const String_Type<T> &format, ...);
		virtual size_t format(const std::basic_string<T> &format, ...);
		virtual size_t format(const T *format, ...);
		virtual size_t vformat(const T *format, va_list args) = 0;

		/**
		* @brief Appends to a CString's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		virtual size_t aformat(const String_Type<T> &format, ...);
		virtual size_t aformat(const std::basic_string<T> &format, ...);
		virtual size_t aformat(const T *format, ...);
		virtual size_t avformat(const T *format, va_list args) = 0;

		/**
		* @brief Counts the number of token deliminated words.
		*
		* @param whitespace A string of tokens used to deliminate words.
		* @return Number of words found.
		*/
		unsigned int wordCount(const T *whitespace) const;

		/**
		* @brief Truncates the string by a specified number of elements.
		*
		* @param n Number of elements to remove from the tail.
		* @return New size of the CString.
		*/
		size_t truncate(size_t n);

		/**
		* @brief Fetches an element from the string.
		*
		* @param index Index of the element to return.
		* @return The element located at the specified index.
		*/
		T &get(size_t index) const;

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
		* @brief Removes the first instance of an element from the string.
		*
		* @param value Value of the element to remove.
		* @return True if an element was removed, false otherwise.
		*/
		bool remove(T &value);

		/**
		* @brief Checks if the string contains an element with the specified value.
		*
		* @param value Value of the element to search for.
		* @return True if a match is found, false otherwise.
		*/
		bool contains(const T &value);

		/** Access Operator */
		inline T &operator[](size_t pos) { return Jupiter::CString_Type<T>::curr[pos]; };

		/** Assignment Operators */
		inline CString_Type<T> &operator=(const CString_Type<T> &right) { this->set(right); return *this; };
		inline CString_Type<T> &operator=(const String_Type<T> &right) { this->set(right); return *this; };
		inline CString_Type<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline CString_Type<T> &operator=(const T *right) { this->set(right); return *this; };
		inline CString_Type<T> &operator=(const T right) { this->set(right); return *this; };

	protected:
		T *base; /** Base pointer for the underlying C-style string */
		T *curr; /** Active pointer for the underlying C-style string */
		size_t strLen; /** Length of underlying C-style string */
	};

	/**
	* @brief Provides a "strict" CString implementation that's more optimized for minimal memory usage.
	* Note: This recreates the underlying C-style string with every concatenation.
	*
	* @param T Element type which the CString will store. Defaults to char.
	*/
	template<typename T = char> class CString_Strict : public CString_Type<T>
	{
	public:

		/**
		* @brief Sets the CString's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param args Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t vformat(const T *format, va_list args);

		/**
		* @brief Appends to a CString's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param args Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t avformat(const T *format, va_list args);

		/**
		* @brief Sets the CString's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return String containing the new format.
		*/
		static CString_Strict Format(const T *format, ...);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		CString_Strict<T> substring(size_t pos, size_t length) const;

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		CString_Strict<T> getWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		*
		* @param in String to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Strict<T> getWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		*
		* @param in C-Style string to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Strict<T> getWord(const T *in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		CString_Strict<T> gotoWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Strict<T> gotoWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Copies the data from the input string to the CString.
		*
		* @param in String containing the data to be copied.
		* @return New size of the CString.
		*/
		size_t set(const String_Type<T> &in);
		size_t set(const std::basic_string<T> &in);
		size_t set(const T *in);
		size_t set(const T in);

		/**
		* @brief Sets the string buffer.
		* Note: This class will free the buffer for you when it's done.
		*
		* @param in New buffer to be used.
		* @return The length of the string.
		*/
		size_t setString(T *in);

		/**
		* @brief Sets the string buffer.
		* Note: This class will free the buffer for you when it's done. DO NOT DELETE THE INPUT BUFFER.
		* Note: This method is unique to the CString_Strict template class, and does not appear in CString_Loose.
		*
		* @param in New buffer to be used.
		* @param size At least the number of characters in the buffer, not including the null-terminator.
		* @return The length of the string.
		*/
		size_t setString(T *in, size_t size);

		/**
		* @brief Copies the data from the input string and concatenates it to the end of CString.
		*
		* @param in String containing the data to be concatenated.
		* @return New size of the CString.
		*/
		size_t concat(const String_Type<T> &in);
		size_t concat(const std::basic_string<T> &in);
		size_t concat(const T *in);
		size_t concat(const T in);

		/** Default Constructor */
		CString_Strict();

		/** Copy Constructors */
		CString_Strict(const CString_Strict &in);
		CString_Strict(const String_Type<T> &in);
		CString_Strict(const std::basic_string<T> &in);
		CString_Strict(const T *in);
		CString_Strict(const T in);

		/** Destructor */
		virtual ~CString_Strict();

		/** Assignment Operators */
		inline CString_Strict<T> &operator=(const CString_Strict<T> &right) { this->set(right); return *this; };
		inline CString_Strict<T> &operator=(const CString_Type<T> &right) { this->set(right); return *this; };
		inline CString_Strict<T> &operator=(const String_Type<T> &right) { this->set(right); return *this; };
		inline CString_Strict<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline CString_Strict<T> &operator=(const T *right) { this->set(right); return *this; };
		inline CString_Strict<T> &operator=(const T right) { this->set(right); return *this; };
	};

	/**
	* @brief Provides a "loose" CString implementation that's more optimized for repeated concatenations.
	* Note: The underlying C-style string will always have a size which is a power of 2, but no fewer than 8 elements.
	*
	* @param T Element type which the CString will store. Defaults to char.
	*/
	template<typename T = char> class CString_Loose : public CString_Type<T>
	{
	public:

		/**
		* @brief Sets the CString's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t vformat(const T *format, va_list args);

		/**
		* @brief Appends to a CString's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return Number of characters written.
		*/
		size_t avformat(const T *format, va_list args);

		/**
		* @brief Sets the CString's contents based on the format string and input variables.
		* Note: Format specifiers similar to printf. Returns 0 for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @param ... Inputs to match the format specifiers.
		* @return String containing the new format.
		*/
		static CString_Loose<T> Format(const T *format, ...);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		CString_Loose<T> substring(size_t pos, size_t length) const;

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		CString_Loose<T> getWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		* Useful when the input string's type isn't known.
		*
		* @param in String to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Loose<T> getWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		* Useful when the input string's type isn't known.
		*
		* @param in C-Style string to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Loose<T> getWord(const T *in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		CString_Loose<T> gotoWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Loose<T> gotoWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Copies the data from the input string to the CString.
		*
		* @param in String containing the data to be copied.
		* @return New size of the CString.
		*/
		size_t set(const CString_Loose &in);
		size_t set(const String_Type<T> &in);
		size_t set(const std::basic_string<T> &in);
		size_t set(const T *in);
		size_t set(const T in);

		/**
		* @brief Copies the data from the input string and concatenats it to the end of CString.
		*
		* @param in String containing the data to be concatenated.
		* @return New size of the CString.
		*/
		size_t concat(const String_Type<T> &in);
		size_t concat(const std::basic_string<T> &in);
		size_t concat(const T *in);
		size_t concat(const T in);

		/** Default constructor */
		CString_Loose();

		/** Copy Constructors */
		CString_Loose(const CString_Loose &in);
		CString_Loose(const String_Type<T> &in);
		CString_Loose(const std::basic_string<T> &in);
		CString_Loose(const T *in);
		CString_Loose(const T in);
		
		/** Destructor */
		virtual ~CString_Loose();

		static const size_t start_size = 8; /** Starting size for loose CStrings. */

		/** Assignment Operators */
		inline CString_Loose<T> &operator=(const CString_Loose<T> &right) { this->set(right); return *this; };
		inline CString_Loose<T> &operator=(const CString_Type<T> &right) { this->set(right); return *this; };
		inline CString_Loose<T> &operator=(const String_Type<T> &right) { this->set(right); return *this; };
		inline CString_Loose<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline CString_Loose<T> &operator=(const T *right) { this->set(right); return *this; };
		inline CString_Loose<T> &operator=(const T right) { this->set(right); return *this; };

	protected:
		size_t strSize; /** Size of underlying C-string buffer */
	};

	/** Definition of a Loose CString. */
	typedef CString_Loose<char> CStringL;

	/** Definition of a Loose Wide CString */
	typedef CString_Loose<wchar_t> WCStringL;

	/** Definition of a Strict CString. */
	typedef CString_Strict<char> CStringS;

	/** Definition of a Strict Wide CString */
	typedef CString_Strict<wchar_t> WCStringS;

	/** Definition of a CString. */
	typedef CStringL CString;

	/** Definition of a Wide CString */
	typedef WCStringL WCString;

	/** Generic CString Type */
	typedef CString_Type<char> CStringType;

	/** Generic Wide CString Type */
	typedef CString_Type<wchar_t> WCStringType;

	/** Empty String constants */
	static const Jupiter::CStringS emptyCStringS;
	static const Jupiter::CStringS emptyCStringL;
	static const Jupiter::CStringS &emptyCString = emptyCStringS;
	static const Jupiter::CStringS &emptyString = emptyCString;
}

/** Implementation for CString_Type, CString_Strict, and CString_Loose. Very scary. */
#include "CString_IMP.h"

#endif // _CSTRING_H_HEADER