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

/**
 * @file CString.h
 * @brief Defines the base CString_Type, as well as a series of CString types.
 * Note: Functions which take "case" or "wildcards" into consideration will only function
 * for types char and wchar_t; inputs with other types will simply return false.
 */

#include "Shift_String.h"

namespace Jupiter
{
	/**
	* @brief Provides the basis for CString classes by providing a minimalist implementation (i.e: no extra variables).
	*
	* @param T Element type which the CString will store. Defaults to char.
	*/
	template<typename T = char> class CString_Type : public Shift_String_Type<T>
	{
	public:

		/**
		* @brief Returns the C-Style string behind the CString.
		*
		* @return C-Style string that the CString represents.
		*/
		const T *c_str() const;

		/**
		* @brief Truncates the string by a specified number of elements.
		*
		* @param n Number of elements to remove from the tail.
		* @return New size of the CString.
		*/
		size_t truncate(size_t n);

		/**
		* @brief Removes the first instance of an element from the string.
		*
		* @param value Value of the element to remove.
		* @return True if an element was removed, false otherwise.
		*/
		bool remove(const T &value);

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
		static CString_Type Format(const T *format, ...);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		CString_Type<T> substring(size_t pos) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		CString_Type<T> substring(size_t pos, size_t length) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Type<T> substring(const Jupiter::Readable_String<T> &in, size_t pos);
		static CString_Type<T> substring(const T *in, size_t pos);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Type<T> substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t length);
		static CString_Type<T> substring(const T *in, size_t pos, size_t length);

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		CString_Type<T> getWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		*
		* @param in String to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Type<T> getWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of an input string, based on a set of tokens.
		*
		* @param in C-Style string to get a partial copy of.
		* @param pos Position of word in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Type<T> getWord(const T *in, size_t pos, const T *whitespace);

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		CString_Type<T> gotoWord(size_t pos, const T *whitespace) const;

		/**
		* @brief Creates a partial copy of the string, based on a set of tokens.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param whitespace A string of tokens used to deliminate words.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Type<T> gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);

		/**
		* @brief Copies the data from the input string to the CString.
		*
		* @param in String containing the data to be copied.
		* @return New size of the CString.
		*/
		size_t set(const Readable_String<T> &in);
		size_t set(const std::basic_string<T> &in);
		size_t set(const T *in);
		size_t set(const T in);

		/**
		* @brief Copies the data from the input string and concatenates it to the end of CString.
		*
		* @param in String containing the data to be concatenated.
		* @return New size of the CString.
		*/
		size_t concat(const Readable_String<T> &in);
		size_t concat(const std::basic_string<T> &in);
		size_t concat(const T *in);
		size_t concat(const T in);

		/** Assignment Operators */
		inline CString_Type<T> &operator=(const CString_Type<T> &right) { this->set(right); return *this; };
		inline CString_Type<T> &operator=(const Readable_String<T> &right) { this->set(right); return *this; };
		inline CString_Type<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline CString_Type<T> &operator=(const T *right) { this->set(right); return *this; };
		inline CString_Type<T> &operator=(const T right) { this->set(right); return *this; };

		static const Jupiter::CString_Type<T> empty; /** Empty instantation of CString_Type */

		/** Default Constructor */
		CString_Type();

		/**
		* @brief Size hint constructor.
		* Note: For the CString_Type base class, this is only truly useful internally.
		*
		* @param size Minimum number of elements the string must be able to hold.
		*/
		CString_Type(size_t size);

		/** Move Constructor */
		CString_Type(CString_Type<T> &&source);

		/** Copy Constructors */
		CString_Type(const CString_Type<T> &in) : CString_Type((Readable_String<T> &)in) {}
		CString_Type(const Readable_String<T> &in);
		CString_Type(const std::basic_string<T> &in);
		CString_Type(const T *in, size_t len);
		CString_Type(const T *in);

	protected:

		/**
		* @brief Sets the internal buffer to be at least large enough to old a specified number of elements.
		* Note: This does nothing if len is less than the string's current length.
		*
		* @param len Minimum number of elements the string buffer must be able to hold.
		* @return True if a new buffer was allocated, false otherwise.
		*/
		virtual bool setBufferSize(size_t len);

		/**
		* @brief Empties the string, and sets the internal buffer to be at least large enough to old a specified number of elements.
		* Note: This does nothing if len is less than the string's current length.
		*
		* @param len Minimum number of elements the string buffer must be able to hold.
		* @return True if a new buffer was allocated, false otherwise.
		*/
		virtual bool setBufferSizeNoCopy(size_t len);

		/** Dummy constructor to prevent string initialization */
		CString_Type(Jupiter::String_Constructor_Base &) {};
	};
	template<typename T = char> using CString_Strict = CString_Type<T>;

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
		* @return String containing the new format.
		*/
		static CString_Loose<T> Format(const T *format, ...);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		CString_Loose<T> substring(size_t pos) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		CString_Loose<T> substring(size_t pos, size_t length) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Loose<T> substring(const Jupiter::Readable_String<T> &in, size_t pos);
		static CString_Loose<T> substring(const T *in, size_t pos);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		static CString_Loose<T> substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t length);
		static CString_Loose<T> substring(const T *in, size_t pos, size_t length);

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
		static CString_Loose<T> getWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);

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
		static CString_Loose<T> gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace);

		/** Default constructor */
		CString_Loose();

		/**
		* @brief Size hint constructor.
		*
		* @param size Minimum number of elements the string must be able to hold.
		*/
		CString_Loose(size_t size);

		/** Move Constructor */
		CString_Loose(CString_Loose<T> &&source);

		/** Copy Constructors */
		CString_Loose(const CString_Loose &in);
		CString_Loose(const Readable_String<T> &in);
		CString_Loose(const std::basic_string<T> &in);
		CString_Loose(const T *in, size_t len);
		CString_Loose(const T *in);

		static const Jupiter::CString_Loose<T> empty; /** Empty instantation of CString_Loose */
		static const size_t start_size = 8; /** Starting size for loose CStrings. */

		/** Assignment Operators */
		inline CString_Loose<T> &operator=(const CString_Loose<T> &right) { this->set(right); return *this; };
		inline CString_Loose<T> &operator=(const CString_Type<T> &right) { this->set(right); return *this; };
		inline CString_Loose<T> &operator=(const Readable_String<T> &right) { this->set(right); return *this; };
		inline CString_Loose<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline CString_Loose<T> &operator=(const T *right) { this->set(right); return *this; };
		inline CString_Loose<T> &operator=(const T right) { this->set(right); return *this; };

	protected:

		/**
		* @brief Sets the internal buffer to be at least large enough to old a specified number of elements.
		* Note: This does nothing if len is less than the string's current length.
		*
		* @param len Minimum number of elements the string buffer must be able to hold.
		* @return True if a new buffer was allocated, false otherwise.
		*/
		virtual bool setBufferSize(size_t len);

		/**
		* @brief Empties the string, and sets the internal buffer to be at least large enough to old a specified number of elements.
		* Note: This does nothing if len is less than the string's current length.
		*
		* @param len Minimum number of elements the string buffer must be able to hold.
		* @return True if a new buffer was allocated, false otherwise.
		*/
		virtual bool setBufferSizeNoCopy(size_t len);

		/** Dummy constructor to prevent string initialization */
		CString_Loose(Jupiter::String_Constructor_Base &) {};

		size_t strSize; /** Size of underlying C-string buffer */
	};

	/** Definition of a Loose CString. */
	typedef CString_Loose<char> CStringL;

	/** Definition of a Loose Wide CString */
	typedef CString_Loose<wchar_t> WCStringL;

	/** Definition of a CString. */
	typedef CStringL CString;

	/** Definition of a Wide CString */
	typedef WCStringL WCString;

	/** Generic CString Type */
	typedef CString_Type<char> CStringType;

	/** Generic Wide CString Type */
	typedef CString_Type<wchar_t> WCStringType;

	/** Definition of a Strict CString. */
	typedef CStringType CStringS;

	/** Definition of a Strict Wide CString */
	typedef WCStringType WCStringS;

	/** Empty String constants */
	static const Jupiter::CStringS &emptyCStringS = Jupiter::CStringS::empty;
	static const Jupiter::CStringL &emptyCStringL = Jupiter::CStringL::empty;
	static const Jupiter::CStringType &emptyCString = emptyCStringS;
	static const Jupiter::StringType &emptyString = emptyCString;
}

/** Implementation for CString_Type and CString_Loose. Very scary. */
#include "CString_Imp.h"

#endif // _CSTRING_H_HEADER