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

#if !defined _CSTRING_IMP_H_HEADER
#define _CSTRING_IMP_H_HEADER

/**
 * @file CString_Imp.h
 * @brief Provides the implementations for CString_Type functions, as well as extending classes.
 * Note: Modification of this file is not supported in any way.
 */

#include "Functions.h"
#include "CString.h"

#if !defined va_copy

#if defined __INTEL_COMPILER
#pragma message("Warning: va_copy not properly defined. Assuming common implementation.")
#define va_copy(dst, src) ((void)((dst) = (src)))
#else
#error "va_copy not defined."
#endif // __INTEL_COMPILER

#endif // va_copy

/**
* IMPLEMENTATION:
*	CString_Type
*/

template<typename T> Jupiter::CString_Type<T>::CString_Type() : Jupiter::CString_Type<T>::CString_Type(size_t(0))
{
}

template<typename T> Jupiter::CString_Type<T>::CString_Type(size_t len)
{
	Jupiter::Shift_String_Type<T>::base = new T[len + 1];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	*Jupiter::String_Type<T>::str = 0;
	Jupiter::String_Type<T>::length = 0;
}

template<typename T> Jupiter::CString_Type<T>::CString_Type(Jupiter::CString_Type<T> &&source) : Jupiter::Shift_String_Type<T>(std::move(source))
{
}

template<typename T> Jupiter::CString_Type<T>::CString_Type(const Jupiter::String_Type<T> &in) : Jupiter::CString_Type<T>::CString_Type(in.size())
{
	while (Jupiter::String_Type<T>::length < in.size() && in.get(Jupiter::String_Type<T>::length) != 0)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = in.get(Jupiter::String_Type<T>::length);
		Jupiter::String_Type<T>::length++;
	}
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
}

template<typename T> Jupiter::CString_Type<T>::CString_Type(const std::basic_string<T> &in) : Jupiter::CString_Type<T>::CString_Type(in.size())
{
	while (Jupiter::String_Type<T>::length < in.size() && in.at(Jupiter::String_Type<T>::length) != 0)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = in.at(Jupiter::String_Type<T>::length);
		Jupiter::String_Type<T>::length++;
	}
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
}

template<typename T> Jupiter::CString_Type<T>::CString_Type(const T *in)
{
	if (in == nullptr)
	{
		Jupiter::String_Type<T>::length = 0;
		Jupiter::Shift_String_Type<T>::base = new T[1];
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		*Jupiter::String_Type<T>::str = 0;
	}
	else
	{
		Jupiter::String_Type<T>::length = Jupiter::strlen<T>(in);
		Jupiter::Shift_String_Type<T>::base = new T[Jupiter::String_Type<T>::length + 1];
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in);
	}
}

template<typename T> bool Jupiter::CString_Type<T>::setBufferSize(size_t len)
{
	if (Jupiter::Shift_String_Type<T>::setBufferSize(len + 1))
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
		return true;
	}
	return false;
}

template<typename T> bool Jupiter::CString_Type<T>::setBufferSizeNoCopy(size_t len)
{
	bool r = Jupiter::Shift_String_Type<T>::setBufferSizeNoCopy(len + 1);
	*Jupiter::String_Type<T>::str = 0;
	return r;
}

template<typename T> const T *Jupiter::CString_Type<T>::c_str() const
{
	return Jupiter::String_Type<T>::str;
}

template<typename T> size_t Jupiter::CString_Type<T>::truncate(size_t n)
{
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::truncate(n)] = 0;
	return Jupiter::String_Type<T>::length;
}

template<typename T> bool Jupiter::CString_Type<T>::remove(const T &value)
{
	if (Jupiter::Shift_String_Type<T>::remove(value))
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
		return true;
	}
	return false;
}

// vformat()

template<> size_t inline Jupiter::CString_Type<char>::vformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vsnprintf(nullptr, 0, format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	this->setBufferSizeNoCopy(minLen);
	Jupiter::String_Type<char>::length = minLen;
	vsnprintf(Jupiter::String_Type<char>::str, Jupiter::String_Type<char>::length, format, args);
	Jupiter::String_Type<char>::str[Jupiter::String_Type<char>::length] = 0;
	return Jupiter::String_Type<char>::length;
}

template<> size_t inline Jupiter::CString_Type<wchar_t>::vformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vswprintf(nullptr, 0, format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	this->setBufferSizeNoCopy(minLen);
	Jupiter::String_Type<wchar_t>::length = minLen;
	vswprintf(Jupiter::String_Type<wchar_t>::str, Jupiter::String_Type<wchar_t>::length, format, args);
	Jupiter::String_Type<wchar_t>::str[Jupiter::String_Type<wchar_t>::length] = 0;
	return Jupiter::String_Type<wchar_t>::length;
}

template<typename T> size_t Jupiter::CString_Type<T>::vformat(const T *format, va_list args)
{
	return 0;
}

// avformat()

template<> size_t inline Jupiter::CString_Type<char>::avformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vsnprintf(nullptr, 0, format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	this->setBufferSize(Jupiter::String_Type<char>::length + minLen);

	minLen = vsnprintf(Jupiter::String_Type<char>::str + Jupiter::String_Type<char>::length, minLen, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<char>::length += minLen;
	Jupiter::String_Type<char>::str[Jupiter::String_Type<char>::length] = 0;
	return minLen;
}

template<> size_t inline Jupiter::CString_Type<wchar_t>::avformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vswprintf(nullptr, 0, format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	this->setBufferSize(minLen + Jupiter::String_Type<wchar_t>::length);

	minLen = vswprintf(Jupiter::String_Type<wchar_t>::str + Jupiter::String_Type<wchar_t>::length, minLen, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<wchar_t>::length += minLen;
	Jupiter::String_Type<wchar_t>::str[Jupiter::String_Type<wchar_t>::length] = 0;
	return minLen;
}

template<typename T> size_t Jupiter::CString_Type<T>::avformat(const T *format, va_list args)
{
	return 0;
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::Format(const T *format, ...)
{
	CString_Type<T> r;
	va_list args;
	va_start(args, format);
	r.vformat(format, args);
	va_end(args);
	return r;
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::substring(size_t pos) const
{
	return Jupiter::CString_Type<T>::substring(*this, pos);
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::substring(size_t pos, size_t len) const
{
	return Jupiter::CString_Type<T>::substring(*this, pos, len);
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::substring(const Jupiter::String_Type<T> &in, size_t pos)
{
	if (pos >= in.size()) return Jupiter::CString_Type<T>();
	Jupiter::CString_Type<T> r = Jupiter::CString_Type<T>(in.size() - pos);
	for (r.length = 0; pos + r.length != in.size() && in.get(pos + r.length) != 0; r.length++) r.str[r.length] = in.get(pos + r.length);
	r.str[r.length] = 0;
	return r;
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::substring(const T *in, size_t pos)
{
	Jupiter::CString_Type<T> r = Jupiter::String_Type<T>::substring<Jupiter::CString_Type>(in, pos);
	r.str[r.length] = 0;
	return r;
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::substring(const Jupiter::String_Type<T> &in, size_t pos, size_t len)
{
	if (pos + len >= in.size()) return Jupiter::CString_Type<T>::substring(in, pos);
	Jupiter::CString_Type<T> r = Jupiter::CString_Type<T>(len);
	for (r.length = 0; r.length != len && in.get(pos + r.length) != 0; r.length++) r.str[r.length] = in.get(pos + r.length);
	r.str[r.length] = 0;
	return r;
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::substring(const T *in, size_t pos, size_t len)
{
	Jupiter::CString_Type<T> r = Jupiter::String_Type<T>::substring<Jupiter::CString_Type>(in, pos, len);
	r.str[r.length] = 0;
	return r;
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::getWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Type<T>::getWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::getWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::String_Type<T>::getWord<Jupiter::CString_Type>(in, pos, whitespace);
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::getWord(const T *in, size_t pos, const T *whitespace)
{
	return Jupiter::String_Type<T>::getWord<Jupiter::CString_Type>(in, pos, whitespace);
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::gotoWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Type<T>::gotoWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Type<T> Jupiter::CString_Type<T>::gotoWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::String_Type<T>::gotoWord<Jupiter::CString_Type>(in, pos, whitespace);
}

template<typename T> size_t Jupiter::CString_Type<T>::set(const String_Type<T> &in)
{
	this->setBufferSizeNoCopy(in.size());
	for (Jupiter::String_Type<T>::length = 0; Jupiter::String_Type<T>::length < in.size() && in.get(Jupiter::String_Type<T>::length) != 0; Jupiter::String_Type<T>::length++)
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = in.get(Jupiter::String_Type<T>::length);
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Type<T>::set(const std::basic_string<T> &in)
{
	this->setBufferSizeNoCopy(in.size());
	for (Jupiter::String_Type<T>::length = 0; Jupiter::String_Type<T>::length < in.size() && in.at(Jupiter::String_Type<T>::length) != 0; Jupiter::String_Type<T>::length++)
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = in.at(Jupiter::String_Type<T>::length);
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Type<T>::set(const T *in)
{
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::set(in)] = 0;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Type<T>::set(const T in)
{
	if (in == 0)
	{
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		*Jupiter::String_Type<T>::str = 0;
		return Jupiter::String_Type<T>::length = 0;
	}
	this->setBufferSizeNoCopy(1);
	*Jupiter::String_Type<T>::str = in;
	Jupiter::String_Type<T>::str[1] = 0;
	return Jupiter::String_Type<T>::length = 1;
}

template<typename T> size_t Jupiter::CString_Type<T>::concat(const String_Type<T> &in)
{
	size_t nSize = Jupiter::String_Type<T>::length + in.size();
	const T *inData = in.ptr();
	this->setBufferSize(nSize);
	while (Jupiter::String_Type<T>::length != nSize && *inData != 0)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = *inData;
		Jupiter::String_Type<T>::length++;
		inData++;
	}
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Type<T>::concat(const std::basic_string<T> &in)
{
	size_t nSize = Jupiter::String_Type<T>::length + in.size();
	const T *inData = in.data();
	this->setBufferSize(nSize);
	while (Jupiter::String_Type<T>::length != nSize && *inData != 0)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = *inData;
		Jupiter::String_Type<T>::length++;
		inData++;
	}
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Type<T>::concat(const T *in)
{
	size_t nSize = Jupiter::String_Type<T>::length + Jupiter::strlen<T>(in);
	this->setBufferSize(nSize);
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str + Jupiter::String_Type<T>::length, in);
	return Jupiter::String_Type<T>::length = nSize;
}

template<typename T> size_t Jupiter::CString_Type<T>::concat(const T c)
{
	this->setBufferSize(Jupiter::String_Type<T>::length + 1);
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = c;
	Jupiter::String_Type<T>::str[++Jupiter::String_Type<T>::length] = 0;
	return Jupiter::String_Type<T>::length;
}

template<typename T> const Jupiter::CString_Type<T> Jupiter::CString_Type<T>::empty = Jupiter::CString_Type<T>();

/**
* IMPLEMENTATION:
*	CString_Loose
*/

template<typename T> Jupiter::CString_Loose<T>::CString_Loose() : Jupiter::CString_Loose<T>::CString_Loose(Jupiter::CString_Loose<T>::start_size)
{
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(Jupiter::CString_Loose<T> &&source) : Jupiter::CString_Type<T>(std::move(source))
{
	Jupiter::CString_Loose<T>::strSize = source.strSize;
	source.strSize = 0;
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(size_t len) : Jupiter::CString_Type<T>::CString_Type(Jupiter::stringConstructorBase)
{
	if (len < Jupiter::CString_Loose<T>::start_size) Jupiter::CString_Loose<T>::strSize = Jupiter::CString_Loose<T>::start_size;
	else Jupiter::CString_Loose<T>::strSize = getPowerTwo32(len + 1);
	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	*Jupiter::String_Type<T>::str = 0;
	Jupiter::String_Type<T>::length = 0;
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const Jupiter::CString_Loose<T> &in) : Jupiter::CString_Type<T>::CString_Type(Jupiter::stringConstructorBase)
{
	Jupiter::CString_Loose<T>::strSize = in.strSize;
	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in.str);
	Jupiter::String_Type<T>::length = in.length;
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const Jupiter::String_Type<T> &in) : Jupiter::CString_Loose<T>::CString_Loose(in.size())
{
	while (Jupiter::String_Type<T>::length < in.size() && in.get(Jupiter::String_Type<T>::length) != 0)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = in.get(Jupiter::String_Type<T>::length);
		Jupiter::String_Type<T>::length++;
	}
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const std::basic_string<T> &in) : Jupiter::CString_Loose<T>::CString_Loose(in.size())
{
	while (Jupiter::String_Type<T>::length < in.size() && in.at(Jupiter::String_Type<T>::length) != 0)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = in.at(Jupiter::String_Type<T>::length);
		Jupiter::String_Type<T>::length++;
	}
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const T *in) : Jupiter::CString_Type<T>::CString_Type(Jupiter::stringConstructorBase)
{
	if (in == nullptr)
	{
		Jupiter::CString_Loose<T>::strSize = Jupiter::CString_Loose<T>::start_size;
		Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		*Jupiter::String_Type<T>::str = 0;
		Jupiter::String_Type<T>::length = 0;
	}
	else
	{
		Jupiter::String_Type<T>::length = Jupiter::strlen<T>(in);
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(Jupiter::String_Type<T>::length + 1);
		if (Jupiter::CString_Loose<T>::strSize < 8) Jupiter::CString_Loose<T>::strSize = 8;
		Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in);
	}
}

template<typename T> bool Jupiter::CString_Loose<T>::setBufferSize(size_t len)
{
	len = getPowerTwo32(len + 1);
	if (len > Jupiter::CString_Loose<T>::strSize)
	{
		T *ptr = new T[len];
		for (unsigned int i = 0; i < Jupiter::String_Type<T>::length; i++) ptr[i] = Jupiter::Shift_String_Type<T>::str[i];
		ptr[Jupiter::String_Type<T>::length] = 0;
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = ptr;
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		return true;
	}
	return false;
}

template<typename T> bool Jupiter::CString_Loose<T>::setBufferSizeNoCopy(size_t len)
{
	len = getPowerTwo32(len + 1);
	if (len > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::String_Type<T>::length = 0;
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = new T[len];
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		*Jupiter::String_Type<T>::str = 0;
		return true;
	}
	Jupiter::String_Type<T>::length = 0;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	*Jupiter::String_Type<T>::str = 0;
	return false;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::Format(const T *format, ...)
{
	CString_Loose<T> r;
	va_list args;
	va_start(args, format);
	r.vformat(format, args);
	va_end(args);
	return r;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::substring(size_t pos) const
{
	return Jupiter::CString_Loose<T>::substring(*this, pos, Jupiter::String_Type<T>::length - pos);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::substring(size_t pos, size_t length) const
{
	return Jupiter::CString_Loose<T>::substring(*this, pos, length);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::substring(const Jupiter::String_Type<T> &in, size_t pos)
{
	if (pos > in.size()) return Jupiter::CString_Loose<T>();
	Jupiter::CString_Loose<T> r = Jupiter::CString_Loose<T>(in.size() - pos);
	size_t index;
	for (index = pos; pos + index != in.size() && in.get(index) != 0; index++) r.str[index - pos] = in.get(index);
	r.str[index - pos] = 0;
	r.length = index;
	return r;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::substring(const T *in, size_t pos)
{
	Jupiter::CString_Loose<T> r = Jupiter::String_Type<T>::substring<Jupiter::CString_Loose>(in, pos);
	r.str[r.length] = 0;
	return r;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::substring(const Jupiter::String_Type<T> &in, size_t pos, size_t len)
{
	if (pos + len >= in.size()) return Jupiter::CString_Loose<T>::substring(in, pos);
	Jupiter::CString_Loose<T> r = Jupiter::CString_Loose<T>(len);
	for (r.length = 0; r.length != len && in.get(r.length + pos) != 0; r.length++) r.str[r.length] = in.get(r.length + pos);
	r.str[r.length] = 0;
	return r;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::substring(const T *in, size_t pos, size_t len)
{
	Jupiter::CString_Loose<T> r = Jupiter::String_Type<T>::substring<Jupiter::CString_Loose>(in, pos, len);
	r.str[r.length] = 0;
	return r;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::getWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Loose<T>::getWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::getWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::String_Type<T>::getWord<Jupiter::CString_Loose>(in, pos, whitespace);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::getWord(const T *in, size_t pos, const T *whitespace)
{
	return Jupiter::String_Type<T>::getWord<Jupiter::CString_Loose>(in, pos, whitespace);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::gotoWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Loose<T>::gotoWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::gotoWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::String_Type<T>::gotoWord<Jupiter::CString_Loose>(in, pos, whitespace);
}

template<typename T> const Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::empty = Jupiter::CString_Loose<T>();

#endif // _CSTRING_IMP_H_HEADER