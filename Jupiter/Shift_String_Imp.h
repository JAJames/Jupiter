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

#if !defined _SHIFT_STRING_IMP_H_HEADER
#define _SHIFT_STRING_IMP_H_HEADER

/**
 * @file Shift_String_Imp.h
 * @brief Provides the implementations for Shift_String_Type functions.
 * Note: Modification of this file is not supported in any way.
 */

template<typename T> Jupiter::Shift_String_Type<T>::Shift_String_Type(Jupiter::Shift_String_Type<T> &&source) : Jupiter::String_Type<T>(std::move(source))
{
	Jupiter::Shift_String_Type<T>::base = source.base;
	source.base = nullptr;
}

template<typename T> Jupiter::Shift_String_Type<T>::~Shift_String_Type()
{
	if (Jupiter::Shift_String_Type<T>::base != nullptr) delete[] Jupiter::Shift_String_Type<T>::base;
}

template<typename T> size_t Jupiter::Shift_String_Type<T>::shiftLeft(size_t len)
{
	size_t offset = Jupiter::String_Type<T>::str - Jupiter::Shift_String_Type<T>::base;
	if (len > offset) len = offset;
	Jupiter::String_Type<T>::str -= len;
	Jupiter::String_Type<T>::length += len;
	return len;
}

template<typename T> size_t Jupiter::Shift_String_Type<T>::shiftRight(size_t len)
{
	if (len > Jupiter::String_Type<T>::length) len = Jupiter::String_Type<T>::length;
	Jupiter::String_Type<T>::str += len;
	Jupiter::String_Type<T>::length -= len;
	return len;
}

template<typename T> bool Jupiter::Shift_String_Type<T>::remove(const T &value)
{
	if (Jupiter::String_Type<T>::length == 0) return false;
	if (*Jupiter::String_Type<T>::str == value)
	{
		if (Jupiter::String_Type<T>::length == 1) this->truncate(1);
		else this->shiftRight(1);
		return true;
	}
	return Jupiter::String_Type<T>::remove(value);
}

template<typename T> void Jupiter::Shift_String_Type<T>::remove(size_t index, size_t len)
{
	if (index == 0)
		this->shiftRight(len);
	else
		Jupiter::String_Type<T>::remove(index, len);
}

template<typename T> bool Jupiter::Shift_String_Type<T>::setBufferSize(size_t len)
{
	if (len > Jupiter::String_Type<T>::length)
	{
		T *ptr = new T[len];
		for (unsigned int i = 0; i < Jupiter::String_Type<T>::length; i++) ptr[i] = Jupiter::String_Type<T>::str[i];
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = ptr;
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		return true;
	}
	return false;
}

template<typename T> bool Jupiter::Shift_String_Type<T>::setBufferSizeNoCopy(size_t len)
{
	if (len > Jupiter::String_Type<T>::length)
	{
		Jupiter::String_Type<T>::length = 0;
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = new T[len];
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		return true;
	}
	Jupiter::String_Type<T>::length = 0;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	return false;
}

// Jupiter::DataBuffer specialization

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::Shift_String_Type>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::Shift_String_Type<Y> *data)
	{
		_Jupiter_DataBuffer_partial_specialization_impl<Jupiter::Readable_String>::push<Y>(buffer, data);
	};
};

#endif // _SHIFT_STRING_IMP_H_HEADER