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

#if !defined _REFERENCE_STRING_IMP_H_HEADER
#define _REFERENCE_STRING_IMP_H_HEADER

/**
 * @file Readable_String_Imp.h
 * @brief Provides the implementations for Readable_String functions.
 * Note: Modification of this file is not supported in any way.
 */

#include "Reference_String.h"
#include "Functions.h"

/**
* IMPLEMENTATION:
*	Reference_String
*/

template<typename T> Jupiter::Reference_String<T>::Reference_String()
{
	Jupiter::Reference_String<T>::str = "";
	Jupiter::Reference_String<T>::length = 0;
}

template<typename T> Jupiter::Reference_String<T>::Reference_String(const T *in)
{
	Jupiter::Reference_String<T>::str = in;
	Jupiter::Reference_String<T>::length = Jupiter::strlen<T>(in);
}

template<typename T> Jupiter::Reference_String<T>::Reference_String(const T *in, size_t len)
{
	Jupiter::Reference_String<T>::str = in;
	Jupiter::Reference_String<T>::length = len;
}

template<typename T> Jupiter::Reference_String<T>::Reference_String(Jupiter::Reference_String<T> &&source)
{
	Jupiter::Reference_String<T>::str = source.str;
	Jupiter::Reference_String<T>::length = source.length;
	source.length = 0;
	source.str = "";
}

template<typename T> Jupiter::Reference_String<T>::Reference_String(const Jupiter::Readable_String<T> &in) : Reference_String(in.ptr(), in.size())
{
}

template<typename T> Jupiter::Reference_String<T>::Reference_String(const Jupiter::Reference_String<T> &in) : Reference_String(in.str, in.length)
{
}

template<typename T> const T &Jupiter::Reference_String<T>::get(size_t index) const
{
	return Jupiter::Reference_String<T>::str[index];
}

template<typename T> size_t Jupiter::Reference_String<T>::size() const
{
	return Jupiter::Reference_String<T>::length;
}

template<typename T> const T *Jupiter::Reference_String<T>::ptr() const
{
	return Jupiter::Reference_String<T>::str;
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::substring(size_t pos) const
{
	return Jupiter::Reference_String<T>::substring(*this, pos);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::substring(size_t pos, size_t len) const
{
	return Jupiter::Reference_String<T>::substring(*this, pos, len);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos)
{
	if (pos > in.size()) pos = in.size();
	return Jupiter::Reference_String<T>(in.ptr() + pos, in.size() - pos);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::substring(const T *in, size_t pos)
{
	return Jupiter::Reference_String<T>(in + pos);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t len)
{
	if (pos + len > in.size()) return Jupiter::Reference_String<T>::substring(in, pos);
	return Jupiter::Reference_String<T>(in.ptr() + pos, len);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::substring(const T *in, size_t pos, size_t len)
{
	return Jupiter::Reference_String<T>(in + pos, len);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::getWord(size_t pos, const T *whitespace) const
{
	return Jupiter::Reference_String<T>::getWord(*this, pos, whitespace);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::getWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::getWord<Jupiter::Reference_String>(in, pos, whitespace);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::getWord(const T *in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::getWord<Jupiter::Reference_String>(in, pos, whitespace);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::gotoWord(size_t pos, const T *whitespace) const
{
	return Jupiter::Reference_String<T>::gotoWord(*this, pos, whitespace);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::gotoWord<Jupiter::Reference_String>(in, pos, whitespace);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::gotoWord(const T *in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::gotoWord<Jupiter::Reference_String>(in, pos, whitespace);
}

#endif // _REFERENCE_STRING_IMP