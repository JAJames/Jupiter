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

#if !defined _SHIFT_STRING_IMP_H_HEADER
#define _SHIFT_STRING_IMP_H_HEADER

/**
 * @file Shift_String_Imp.h
 * @brief Provides the implementations for Shift_String_Type functions.
 * Note: Modification of this file is not supported in any way.
 */

template<typename T> size_t Jupiter::Shift_String_Type<T>::shiftLeft(size_t len)
{
	size_t offset = Jupiter::String_Type<T>::str - Jupiter::Shift_String_Type<T>::base;
	if (length > offset) length = offset;
	Jupiter::String_Type<T>::str -= len;
	Jupiter::String_Type<T>::length += len;
	return length;
}

template<typename T> size_t Jupiter::Shift_String_Type<T>::shiftRight(size_t len)
{
	if (length > Jupiter::String_Type<T>::length) length = Jupiter::String_Type<T>::length;
	Jupiter::String_Type<T>::str += len;
	Jupiter::String_Type<T>::length -= len;
	return len;
}

#endif // _SHIFT_STRING_IMP_H_HEADER