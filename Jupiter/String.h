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

#if !defined _STRING_H_HEADER
#define _STRING_H_HEADER

/**
 * @file String.h
 * @brief Defines the base String_Base, as well as a series of String types.
 * Note: Functions which take "case" or "wildcards" into consideration will only function
 * for types char and wchar_t; inputs with other types will simply return false.
 */

#include "Shift_String.h"

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
		* @brief Returns a C-Style string representation of the String.
		*
		* @return C-Style string representation of the String.
		*/
		const T *c_str() const;

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
		String_Strict<T> substring(size_t pos) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		String_Strict<T> substring(size_t pos, size_t length) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		static String_Strict<T> substring(const Jupiter::String_Type<T> &in, size_t pos);
		static String_Strict<T> substring(const T *in, size_t pos);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		static String_Strict<T> substring(const Jupiter::String_Type<T> &in, size_t pos, size_t length);
		static String_Strict<T> substring(const T *in, size_t pos, size_t length);

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
		static String_Strict<T> getWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace);

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
		static String_Strict<T> gotoWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace);

		/** Assignment Operators */
		inline String_Strict<T> &operator=(const String_Strict<T> &right) { this->set(right); return *this; };
		inline String_Strict<T> &operator=(const String_Type<T> &right) { this->set(right); return *this; };
		inline String_Strict<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline String_Strict<T> &operator=(const T *right) { this->set(right); return *this; };
		inline String_Strict<T> &operator=(const T right) { this->set(right); return *this; };

		static const Jupiter::String_Strict<T> empty; /** Empty instantation of String_Strict */

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
		String_Strict(const String_Strict<T> &in) : String_Strict((String_Type<T> &)in) {}
		String_Strict(const String_Type<T> &in);
		String_Strict(const std::basic_string<T> &in);
		String_Strict(const T *in);

	protected:

		/** Dummy constructor to prevent string initialization */
		String_Strict(Jupiter::String_Constructor_Base &) {};
	};

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
		* @brief Returns a C-Style string representation of the String.
		*
		* @return C-Style string representation of the String.
		*/
		const T *c_str() const;

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
		String_Loose<T> substring(size_t pos) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		String_Loose<T> substring(size_t pos, size_t length) const;

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @return String containing a partial copy of the original string.
		*/
		static String_Loose<T> substring(const Jupiter::String_Type<T> &in, size_t pos);
		static String_Loose<T> substring(const T *in, size_t pos);

		/**
		* @brief Creates a partial copy of the string.
		*
		* @param in String to get a partial copy of.
		* @param pos Position in the string to start copying from.
		* @param length Number of characters to copy.
		* @return String containing a partial copy of the original string.
		*/
		static String_Loose<T> substring(const Jupiter::String_Type<T> &in, size_t pos, size_t length);
		static String_Loose<T> substring(const T *in, size_t pos, size_t length);

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
		static String_Loose<T> getWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace);

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
		static String_Loose<T> gotoWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace);

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
		String_Loose(const String_Loose &in);
		String_Loose(const String_Type<T> &in);
		String_Loose(const std::basic_string<T> &in);
		String_Loose(const T *in);

		static const Jupiter::String_Loose<T> empty; /** Empty instantation of String_Loose */
		static const size_t start_size = 8; /** Starting size for loose Strings. */

		/** Assignment Operators */
		inline String_Loose<T> &operator=(const String_Loose<T> &right) { this->set(right); return *this; };
		inline String_Loose<T> &operator=(const String_Type<T> &right) { this->set(right); return *this; };
		inline String_Loose<T> &operator=(const std::basic_string<T> &right) { this->set(right); return *this; };
		inline String_Loose<T> &operator=(const T *right) { this->set(right); return *this; };
		inline String_Loose<T> &operator=(const T right) { this->set(right); return *this; };

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
		String_Loose(Jupiter::String_Constructor_Base &) {};

		size_t strSize; /** Size of underlying string buffer */
	};

	/** Definition of a Loose String. */
	typedef String_Loose<char> StringL;

	/** Definition of a Loose Wide String */
	typedef String_Loose<wchar_t> WStringL;

	/** Definition of a Strict String. */
	typedef String_Strict<char> StringS;

	/** Definition of a Strict Wide String */
	typedef String_Strict<wchar_t> WStringS;

	/** Definition of a String. */
	typedef StringL String;

	/** Definition of a Wide String */
	typedef WStringL WString;

	/** Empty String constants */
	static const Jupiter::StringS &emptyStringS = Jupiter::StringS::empty;
	static const Jupiter::StringL &emptyStringL = Jupiter::StringL::empty;
	//static const Jupiter::StringType &emptyString = emptyStringS;
}

/** Implementation for String_Strict and String_Loose. Very scary. */
#include "String_Imp.h"

#endif // _STRING_H_HEADER