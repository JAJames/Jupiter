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

#if !defined _STRING_TYPE_IMP_H_HEADER
#define _STRING_TYPE_IMP_H_HEADER

/**
 * @file String_Type_Imp.h
 * @brief Provides the implementations for String_Type functions.
 * Note: Modification of this file is not supported in any way.
 */

#include "String_Type.h"
#include "Functions.h"

/**
* IMPLEMENTATION:
*	String_Type
*/

template<typename T> Jupiter::String_Type<T>::String_Type(Jupiter::String_Type<T> &&source)
{
	Jupiter::String_Type<T>::str = source.str;
	Jupiter::String_Type<T>::length = source.length;
	source.length = 0;
	source.str = nullptr;
}

template<typename T> const T &Jupiter::String_Type<T>::get(size_t index) const
{
	return Jupiter::String_Type<T>::str[index];
}

template<typename T> size_t Jupiter::String_Type<T>::size() const
{
	return Jupiter::String_Type<T>::length;
}

template<typename T> const T *Jupiter::String_Type<T>::ptr() const
{
	return Jupiter::String_Type<T>::str;
}

// format forwards

template<typename T> size_t Jupiter::String_Type<T>::format(const std::basic_string<T> &format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->vformat(format.c_str(), args);
	va_end(args);
	return r;
}

template<typename T> size_t Jupiter::String_Type<T>::format(const T *format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->vformat(format, args);
	va_end(args);
	return r;
}

// aformat forwards

template<typename T> size_t Jupiter::String_Type<T>::aformat(const std::basic_string<T> &format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->avformat(format.c_str(), args);
	va_end(args);
	return r;
}

template<typename T> size_t Jupiter::String_Type<T>::aformat(const T *format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->avformat(format, args);
	va_end(args);
	return r;
}

// truncate base

template<typename T> size_t Jupiter::String_Type<T>::truncate(size_t n)
{
	if (n >= Jupiter::String_Type<T>::length) return (Jupiter::String_Type<T>::length = 0);
	return (Jupiter::String_Type<T>::length -= n);
}

// remove base

template<typename T> bool Jupiter::String_Type<T>::remove(const T &value)
{
	for (size_t i = 0; i < Jupiter::String_Type<T>::length - 1; i++)
	{
		if (Jupiter::String_Type<T>::str[i] == value)
		{
			while (i < Jupiter::String_Type<T>::length)
			{
				i++;
				Jupiter::String_Type<T>::str[i - 1] = Jupiter::String_Type<T>::str[i];
			}
			Jupiter::String_Type<T>::length--;
			return true;
		}
	}
	if (Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length - 1] == value)
	{
		this->truncate(1);
		return true;
	}
	return false;
}

template<typename T> size_t Jupiter::String_Type<T>::set(const Jupiter::Readable_String<T> &in)
{
	this->setBufferSizeNoCopy(in.size());
	for (Jupiter::String_Type<T>::length = 0; Jupiter::String_Type<T>::length < in.size() != 0; Jupiter::String_Type<T>::length++)
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = in.get(Jupiter::String_Type<T>::length);
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::set(const std::basic_string<T> &in)
{
	this->setBufferSizeNoCopy(in.size());
	for (Jupiter::String_Type<T>::length = 0; Jupiter::String_Type<T>::length < in.size(); Jupiter::String_Type<T>::length++)
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = in.at(Jupiter::String_Type<T>::length);
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::set(const T *in)
{
	size_t nLen = Jupiter::strlen<T>(in);
	this->setBufferSizeNoCopy(nLen);
	for (Jupiter::String_Type<T>::length = 0; *in != 0; Jupiter::String_Type<T>::length++, in++) Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = *in;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::set(const T in)
{
	this->setBufferSizeNoCopy(1);
	*Jupiter::String_Type<T>::str = in;
	return Jupiter::String_Type<T>::length = 1;
}

// replace

template<typename T> size_t Jupiter::String_Type<T>::insert(size_t index, const T &value)
{
	if (index >= this->size())
		return this->concat(value);

	this->setBufferSize(this->size() + 1);
	for (size_t i = this->size(); i != index; i--)
		Jupiter::String_Type<T>::str[i] = this->get(i-1);

	Jupiter::String_Type<T>::str[index] = value;
	return ++Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::insert(size_t index, const Jupiter::Readable_String<T> &value)
{
	if (index >= this->size())
		return this->concat(value);

	if (value.size() == 0)
		return Jupiter::String_Type<T>::length;

	if (value.size() == 1)
		return this->insert(index, value.get(0));

	this->setBufferSize(this->size() + value.size());
	size_t i;
	for (i = this->size() + value.size() - 1; i != index + value.size() - 1; i--)
		Jupiter::String_Type<T>::str[i] = this->get(i - value.size());

	while (i != index)
	{
		Jupiter::String_Type<T>::str[i] = value.get(i - index);
		i--;
	}
	Jupiter::String_Type<T>::str[index] = value.get(0);

	return Jupiter::String_Type<T>::length += value.size();
}

// replace

template<typename T> size_t Jupiter::String_Type<T>::replace(const T &target, const T &value)
{
	for (size_t i = 0; i != this->size(); i++)
	{
		if (this->get(i) == target)
			Jupiter::String_Type<T>::str[i] = value;
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const T *target, size_t targetSize, const T &value)
{
	if (targetSize != 0)
	{
		if (targetSize == 1)
			return this->replace(*target, value);

		if (targetSize < this->size())
		{
			size_t i = 0, j = 0, k;
			while (j <= this->size() - targetSize)
			{
				k = 0;
				while (this->get(j + k) == target[k])
				{
					if (++k == targetSize) // match found
					{
						Jupiter::String_Type<T>::str[i] = value;
						i += 1;
						j += k;
						break;
					}
				}
				if (k != targetSize)
					Jupiter::String_Type<T>::str[i++] = Jupiter::String_Type<T>::str[j++];
			}
			while (j < this->size())
				Jupiter::String_Type<T>::str[i++] = Jupiter::String_Type<T>::str[j++];

			Jupiter::String_Type<T>::length = i;
		}
		else if (targetSize == this->size() && this->equals(target, targetSize))
			return this->set(value);
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const Jupiter::Readable_String<T> &target, const T &value)
{
	return this->replace(target.ptr(), target.size(), value);
}

// concat

template<typename T> size_t Jupiter::String_Type<T>::concat(const Jupiter::Readable_String<T> &in)
{
	size_t nSize = Jupiter::String_Type<T>::length + in.size();
	const T *inData = in.ptr();
	this->setBufferSize(nSize);
	while (Jupiter::String_Type<T>::length != nSize)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = *inData;
		Jupiter::String_Type<T>::length++;
		inData++;
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::concat(const std::basic_string<T> &in)
{
	size_t nSize = Jupiter::String_Type<T>::length + in.size();
	const T *inData = in.data();
	this->setBufferSize(nSize);
	while (Jupiter::String_Type<T>::length != nSize)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = *inData;
		Jupiter::String_Type<T>::length++;
		inData++;
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::concat(const T *in)
{
	size_t nSize = Jupiter::String_Type<T>::length + Jupiter::strlen<T>(in);
	this->setBufferSize(nSize);
	while (*in != 0)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = *in;
		Jupiter::String_Type<T>::length++;
		in++;
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::concat(const T c)
{
	this->setBufferSize(Jupiter::String_Type<T>::length + 1);
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = c;
	return ++Jupiter::String_Type<T>::length;
}

/**
* IMPLEMENTATION:
*	String helper templates
*/

// substring

template<typename T> template<template<typename> class R> R<T> Jupiter::String_Type<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos)
{
	if (pos >= in.size()) return R<T>();
	R<T> r = R<T>(in.size() - pos);
	for (r.length = 0; pos + r.length != in.size(); r.length++) r.str[r.length] = in.get(pos + r.length);
	return r;
}

template<typename T> template<template<typename> class R> R<T> Jupiter::String_Type<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t len)
{
	if (pos + len >= in.size()) return R<T>::substring(in, pos);
	R<T> r = R<T>(len);
	for (r.length = 0; r.length != len; r.length++) r.str[r.length] = in.get(pos + r.length);
	return r;
}

template<typename T> template<template<typename> class R> R<T> Jupiter::String_Type<T>::substring(const T *in, size_t pos)
{
	size_t strLen = Jupiter::strlen<T>(in);
	if (pos >= strLen) return R<T>();
	R<T> r = R<T>(strLen - pos);
	in += pos;
	for (r.length = 0; *in != 0; r.length++, in++) r.str[r.length] = *in;
	return r;
}

template<typename T> template<template<typename> class R> R<T> Jupiter::String_Type<T>::substring(const T *in, size_t pos, size_t len)
{
	R<T> r = R<T>(len);
	in += pos;
	for (r.length = 0; r.length != len; r.length++, in++) r.str[r.length] = *in;
	return r;
}

namespace Jupiter
{
	static struct String_Constructor_Base {} stringConstructorBase;
}

#endif // _STRING_TYPE_IMP_H_HEADER