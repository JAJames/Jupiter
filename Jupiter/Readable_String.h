/**
 * Copyright (C) 2014-2015 Justin James.
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

#if !defined _READABLE_STRING_H_HEADER
#define _READABLE_STRING_H_HEADER

/**
 * @file Readable_String.h
 * @brief Defines several basic accessive and comparative virtual functions for strings.
 */

#include <cwchar> // wchar_t
#include <cstdio> // FILE
#include <string> // std::basic_string<T> type
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
		virtual bool isEmpty() const;

		/**
		* @brief Checks if the String is not empty.
		*
		* @return True if the String is not empty, false otherwise.
		*/
		virtual bool isNotEmpty() const;

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
		* @brief Returns the index of the first element in the string with the specified value.
		* Note: Case insensitive. Returns false for any type other than char and wchar_t.
		*
		* @param value Value of the element to search for.
		* @param index Index of the match to return (i.e: 0 returns the first match).
		* @return The index of an element if one is found, INVALID_INDEX otherwise.
		*/
		size_t findi(const T &value, size_t index = 0) const;
		size_t findi(const Readable_String<T> &in) const;

		/**
		* @brief Returns the number of elements of the string which match the input string.
		*
		* @param in Character set to match against.
		* @return Number of elements at the start of the string that match the character set.
		*/
		size_t span(const Readable_String<T> &in) const;
		size_t span(const T *str) const;

		/**
		* @brief Compares another string against the String.
		*
		* @param in String to compare against.
		* @return 0 if the strings are equal, negative if the first mismatched character is greater in the String, or positive if it's less.
		*/
		int compare(const Readable_String<T> &in) const;
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
		bool equals(const Readable_String<T> &in) const;
		bool equals(const std::basic_string<T> &in) const;
		bool equals(const T *in, size_t len) const;
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
		bool equalsi(const Readable_String<T> &in) const;
		bool equalsi(const std::basic_string<T> &in) const;
		bool equalsi(const T *in, size_t len) const;
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
		* @brief Counts the number of token-deliminated strings in the string.
		*
		* @param token Token to scan for.
		* @return Number of token-deliminated strings in the string.
		*/
		size_t tokenCount(const T &token) const;
		size_t tokenCount(const Readable_String<T> &token) const;
		size_t tokenCount(const std::basic_string<T> &token) const;
		size_t tokenCount(const T *token, size_t tokenLength) const;

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
		* @brief Sums together all of the elements in the string.
		*
		* @param T Integral type to return
		*
		* @return Sum of all the elements in the string
		*/
		template<typename R = unsigned int> R calcChecksum() const;

		/**
		* @brief Sums together the uppercase version of all of the elements in the string.
		*
		* @param T Integral type to return
		*
		* @return Sum of all the elements in the string
		*/
		template<typename R = unsigned int> R calcChecksumi() const;

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
		* @brief Copies a the elements between two tokens from an input string and returns it in an output type.
		*
		* @param R Type to return. Must be a subclass of String_Type.
		*
		* @param in String to get a partial copy of.
		* @param pos Index of the token to copy.
		* @param token Token to scan for.
		* @return Copy of the token at the specified index on success, an empty string otherwise.
		*/
		template<template<typename> class R> static R<T> getToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token);
		template<template<typename> class R> static R<T> getToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token);

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
		* @brief Copies a part of an input string starting at a specified token and returns it in an output type.
		*
		* @param R Type to return. Must be a subclass of String_Type.
		*
		* @param in String to get a partial copy of.
		* @param pos Index of the word to start copying from.
		* @param token Token to scan for.
		* @return Copy of the string starting at the specified word on success, an empty string otherwise.
		*/
		template<template<typename> class R> static R<T> gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token);
		template<template<typename> class R> static R<T> gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token);

		/**
		* @brief Contains the results from a call to tokenize().
		*/
		template<template<typename> class R> struct TokenizeResult
		{
			/** Underlying type used to represent each token */
			typedef R<T> token_type;

			/** Array of tokens, with a length of 'token_count' */
			R<T> *tokens;

			/** Length of 'tokens' array */
			size_t token_count;

			/**
			* @brief Default constructor for the TokenizeResult class.
			* Note: 'tokens' assigned nullptr, and 'token_count' is assigned 0.
			*/
			TokenizeResult();

			/**
			* @brief Allocation constructor for the TokenizeResult class.
			*/
			TokenizeResult(size_t in_token_count);

			/**
			* @brief Capture constructor for the TokenizeResult class.
			*/
			TokenizeResult(R<T> *in_tokens, size_t in_token_count);

			/**
			* @brief Copy constructor for the TokenizeResult class.
			*/
			TokenizeResult(const TokenizeResult &source);

			/**
			* @brief Move constructor for the TokenizeResult class.
			*/
			TokenizeResult(const TokenizeResult &&source);

			/**
			* @brief Destructor for the TokenizeResult class.
			*/
			~TokenizeResult();
		};

		/**
		* @brief Tokenizes a string, based on an input token separator
		*
		* @param R String type to use when constructing results
		*
		* @param in String to split into tokens
		* @param separator Separator to split tokens by
		* @return TokenizeResult containing the results of the tokenization process.
		*/
		template<template<typename> class R> static TokenizeResult<R> tokenize(const Jupiter::Readable_String<T> &in, const T &separator);
		template<template<typename> class R> static TokenizeResult<R> tokenize(const Jupiter::Readable_String<T> &in, const Jupiter::Readable_String<T> &separator);
		template<template<typename> class R> static TokenizeResult<R> tokenize(const Jupiter::Readable_String<T> &in, const T *separator, size_t separator_size);

		/** Access operator */
		inline const T &operator[](size_t index) const { return this->get(index); };

		/** Comparative operators */
		inline bool operator==(const Readable_String<T> &right)const{ return this->equals(right); }
		inline bool operator==(const std::basic_string<T> &right)const{ return this->equals(right); }
		inline bool operator==(const T *right)const{ return this->equals(right); }
		inline bool operator==(const T right)const{ return this->equals(right); }
		inline bool operator!=(const Readable_String<T> &right)const{ return !operator==(right); }
		inline bool operator!=(const std::basic_string<T> &right)const{ return !operator==(right); }
		inline bool operator!=(const T *right)const{ return !operator==(right); }
		inline bool operator!=(const T right)const{ return !operator==(right); }
		inline bool operator<(const Readable_String<T> &right)const{ return this->compare(right) < 0; }
		inline bool operator<(const std::basic_string<T> &right)const{ return this->compare(right) < 0; }
		inline bool operator<(const T *right)const{ return this->compare(right) < 0; }
		inline bool operator<(const T right)const{ return this->compare(right) < 0; }
		inline bool operator>(const Readable_String<T> &right)const{ return  this->compare(right) > 0; }
		inline bool operator>(const std::basic_string<T> &right)const{ return  this->compare(right) > 0; }
		inline bool operator>(const T *right)const{ return  this->compare(right) > 0; }
		inline bool operator>(const T right)const{ return  this->compare(right) > 0; }
		inline bool operator<=(const Readable_String<T> &right)const{ return !operator>(right); }
		inline bool operator<=(const std::basic_string<T> &right)const{ return !operator>(right); }
		inline bool operator<=(const T *right)const{ return !operator>(right); }
		inline bool operator<=(const T right)const{ return !operator>(right); }
		inline bool operator>=(const Readable_String<T> &right)const{ return !operator<(right); }
		inline bool operator>=(const std::basic_string<T> &right)const{ return !operator<(right); }
		inline bool operator>=(const T *right)const{ return !operator<(right); }
		inline bool operator>=(const T right)const{ return !operator<(right); }

		/** Conversion operators */
		explicit inline operator std::basic_string<T>() { return std::basic_string<T>(this->ptr(), this->size()); }

	private:
		template<typename R> R calcChecksumiHelper() const;
	};

	/** Generic Readable String Type */
	typedef Readable_String<char> ReadableString;

	/** Generic Wide Readable String Type */
	typedef Readable_String<wchar_t> ReadableWString;
}

#include "Readable_String_Imp.h"

#endif // _READABLE_STRING_H_HEADER