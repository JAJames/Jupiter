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

#if !defined _SHIFT_STRING_H_HEADER
#define _SHIFT_STRING_H_HEADER

/**
 * @file Shift_String.h
 * @brief Allows for separation of a string's representation from its memory base.
 */

#include "String_Type.h"

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
	template<typename T = char> class Shift_String_Type : public Jupiter::String_Type<T>
	{
	public:

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
		virtual bool remove(const T &value) override;

		/**
		* @brief Removes a number of elements starting at an index.
		*
		* @param index Index to start removing elements at.
		* @param length Number of elements to remove.
		*/
		virtual void remove(size_t index, size_t length) override;

		/**
		* @brief Removes all elements from the string.
		*/
		virtual void erase();

		/**
		* @brief Default constructor for the Shift_String_Type class.
		*/
		Shift_String_Type() {}

		/**
		* @brief Move constructor for the Shift_String_Type class.
		*/
		Shift_String_Type(Jupiter::Shift_String_Type<T> &&source);

		/**
		* @brief Destructor for the Shift_String_Type class.
		*/
		virtual ~Shift_String_Type();

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
		* Note: This still empties the string if len is less than the string's current length.
		*
		* @param len Minimum number of elements the string buffer must be able to hold.
		* @return True if a new buffer was allocated, false otherwise.
		*/
		virtual bool setBufferSizeNoCopy(size_t len);

		T *base; /** Base pointer for the underlying String's memory allocation */
	};
}

/** Re-enable warning */
#if defined _MSC_VER
#pragma warning(pop)
#endif

#include "Shift_String_Imp.h"

#endif // _SHIFT_STRING_H_HEADER