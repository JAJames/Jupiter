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

/**
* IMPLEMENTATION:
*	CString_Type
*/

template<typename T> const T *Jupiter::CString_Type<T>::c_str() const
{
	return Jupiter::String_Type<T>::str;
}

template<typename T> size_t Jupiter::CString_Type<T>::truncate(size_t n)
{
	if (n >= Jupiter::String_Type<T>::length)
	{
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		*Jupiter::String_Type<T>::str = 0;
		Jupiter::String_Type<T>::length = 0;
		return 0;
	}
	Jupiter::String_Type<T>::length -= n;
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
	return Jupiter::String_Type<T>::length;
}

template<typename T> bool Jupiter::CString_Type<T>::remove(T &value)
{
	for (unsigned int i = 0; i < Jupiter::String_Type<T>::length; i++)
	{
		if (Jupiter::String_Type<T>::str[i] == value)
		{
			if (i == Jupiter::String_Type<T>::length - 1) Jupiter::CString_Type<T>::truncate(1);
			else if (i == 0)
			{
				if (Jupiter::String_Type<T>::length == 1) Jupiter::CString_Type<T>::truncate(1);
				else Jupiter::CString_Type<T>::shiftRight(1);
			}
			else
			{
				Jupiter::strcpy<T>(Jupiter::String_Type<T>::str + i, Jupiter::String_Type<T>::str + i + 1);
				Jupiter::String_Type<T>::length--;
			}
			return true;
		}
	}
	return false;
}

/**
* IMPLEMENTATION:
*	CString_Strict
*/

template<typename T> Jupiter::CString_Strict<T>::CString_Strict() : Jupiter::CString_Strict<T>::CString_Strict(size_t(0))
{
}

template<typename T> Jupiter::CString_Strict<T>::CString_Strict(size_t size)
{
	// We need a size variable to actually make any real use of this.
	Jupiter::Shift_String_Type<T>::base = new T[size+1];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	*Jupiter::String_Type<T>::str = 0;
	Jupiter::String_Type<T>::length = 0;
}

template<typename T> Jupiter::CString_Strict<T>::CString_Strict(const Jupiter::String_Type<T> &in)
{
	Jupiter::String_Type<T>::length = in.size();
	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::String_Type<T>::length + 1];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	size_t index;
	for (index = 0; index < Jupiter::String_Type<T>::length && in.get(index) != 0; index++) Jupiter::String_Type<T>::str[index] = in.get(index);
	Jupiter::String_Type<T>::length = index;
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
}

template<typename T> Jupiter::CString_Strict<T>::CString_Strict(const std::basic_string<T> &in)
{
	Jupiter::String_Type<T>::length = in.size();
	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::String_Type<T>::length + 1];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in.c_str());
}

template<typename T> Jupiter::CString_Strict<T>::CString_Strict(const T *in)
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

template<typename T> Jupiter::CString_Strict<T>::~CString_Strict()
{
	delete[] Jupiter::Shift_String_Type<T>::base;
}

// vformat()

template<> size_t inline Jupiter::CString_Strict<char>::vformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vsnprintf(nullptr, 0, format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	if ((unsigned)minLen > Jupiter::String_Type<char>::length)
	{
		delete[] Jupiter::Shift_String_Type<char>::base;
		Jupiter::Shift_String_Type<char>::base = new char[minLen + 1];
	}
	Jupiter::String_Type<char>::str = Jupiter::Shift_String_Type<char>::base;
	Jupiter::String_Type<char>::length = minLen;
	vsnprintf(Jupiter::String_Type<char>::str, Jupiter::String_Type<char>::length, format, args);
	Jupiter::String_Type<char>::str[Jupiter::String_Type<char>::length] = 0;
	return Jupiter::String_Type<char>::length;
}

template<> size_t inline Jupiter::CString_Strict<wchar_t>::vformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vswprintf(nullptr, 0, format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	if ((unsigned)minLen > Jupiter::String_Type<wchar_t>::length)
	{
		delete[] Jupiter::CString_Type<wchar_t>::base;
		Jupiter::CString_Type<wchar_t>::base = new wchar_t[minLen + 1];
	}
	Jupiter::String_Type<wchar_t>::str = Jupiter::CString_Type<wchar_t>::base;
	Jupiter::String_Type<wchar_t>::length = minLen;
	vswprintf(Jupiter::String_Type<wchar_t>::str, Jupiter::String_Type<wchar_t>::length, format, args);
	Jupiter::String_Type<wchar_t>::str[Jupiter::String_Type<wchar_t>::length] = 0;
	return Jupiter::String_Type<wchar_t>::length;
}

template<typename T> size_t Jupiter::CString_Strict<T>::vformat(const T *format, va_list args)
{
	return 0;
}

// avformat()

template<> size_t inline Jupiter::CString_Strict<char>::avformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vsnprintf(nullptr, 0, format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	char *t = new char[minLen + Jupiter::String_Type<char>::length + 1];
	Jupiter::strcpy<char>(t, Jupiter::String_Type<char>::str);
	delete[] Jupiter::Shift_String_Type<char>::base;
	Jupiter::Shift_String_Type<char>::base = t;
	Jupiter::String_Type<char>::str = Jupiter::Shift_String_Type<char>::base;

	minLen = vsnprintf(Jupiter::String_Type<char>::str + Jupiter::String_Type<char>::length, minLen, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<char>::length += minLen;
	Jupiter::String_Type<char>::str[Jupiter::String_Type<char>::length] = 0;
	return minLen;
}

template<> size_t inline Jupiter::CString_Strict<wchar_t>::avformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vswprintf(nullptr, 0, format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	wchar_t *t = new wchar_t[minLen + Jupiter::String_Type<wchar_t>::length + 1];
	Jupiter::strcpy<wchar_t>(t, Jupiter::String_Type<wchar_t>::str);
	delete[] Jupiter::CString_Type<wchar_t>::base;
	Jupiter::CString_Type<wchar_t>::base = t;
	Jupiter::String_Type<wchar_t>::str = Jupiter::CString_Type<wchar_t>::base;

	minLen = vswprintf(Jupiter::String_Type<wchar_t>::str + Jupiter::String_Type<wchar_t>::length, minLen, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<wchar_t>::length += minLen;
	Jupiter::String_Type<wchar_t>::str[Jupiter::String_Type<wchar_t>::length] = 0;
	return minLen;
}

template<typename T> size_t Jupiter::CString_Strict<T>::avformat(const T *format, va_list args)
{
	return 0;
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::Format(const T *format, ...)
{
	CString_Strict<T> r;
	va_list args;
	va_start(args, format);
	r.vformat(format, args);
	va_end(args);
	return r;
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::substring(size_t pos) const
{
	return Jupiter::CString_Strict<T>::substring(*this, pos, Jupiter::String_Type<T>::length - pos);
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::substring(size_t pos, size_t length) const
{
	return Jupiter::CString_Strict<T>::substring(*this, pos, length);
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::substring(const Jupiter::String_Type<T> &in, size_t pos)
{
	if (pos > in.size()) return Jupiter::CString_Strict<T>();
	Jupiter::CString_Strict<T> r = Jupiter::CString_Strict<T>(in.size() - pos);
	size_t index;
	for (index = pos; index != in.size() && in.get(index) != 0; index++) r.str[index - pos] = in.get(index);
	r.str[index - pos] = 0;
	r.length = index;
	return r;
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::substring(const Jupiter::String_Type<T> &in, size_t pos, size_t length)
{
	if (pos > in.size()) return Jupiter::CString_Strict<T>();
	if (length > in.size() - pos) length = in.size() - pos;
	Jupiter::CString_Strict<T> r = Jupiter::CString_Strict<T>(length);
	size_t index;
	for (index = 0; index != length && in.get(index + pos) != 0; index++) r.str[index] = in.get(index + pos);
	r.str[index] = 0;
	r.length = index;
	return r;
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::getWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Strict<T>::getWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::getWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	unsigned int x = 0;
	unsigned int y = 1;
	for (unsigned int i = 0; i < pos || y == 1; x++)
	{
		if (in.get(x) == 0) return Jupiter::CString_Strict<T>();
		if (Jupiter::strpbrk<T>(whitespace, in.get(x)) != nullptr)
		{
			if (y != 1)
			{
				y = 1;
				i++;
			}
		}
		else
		{
			if (i >= pos) break;
			y = 0;
		}
	}
	for (y = x; Jupiter::strpbrk<T>(whitespace, in.get(y)) == nullptr && in.get(y) != 0; y++);
	return Jupiter::CString_Strict<T>::substring(in, x, y - x);
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::getWord(const T *in, size_t pos, const T *whitespace)
{
	Jupiter::CString_Strict<T> r;
	unsigned int x = 0;
	unsigned int y = 1;
	unsigned int i;
	for (i = 0; i < pos || y == 1; x++)
	{
		if (in[x] == 0) return r;
		if (Jupiter::strpbrk<T>(whitespace, in[x]) != nullptr)
		{
			if (y != 1)
			{
				y = 1;
				i++;
			}
		}
		else
		{
			if (i >= pos) break;
			y = 0;
		}
	}

	for (y = x; Jupiter::strpbrk<T>(whitespace, in[y]) == nullptr && in[y] != 0; y++);
	r.length = y - x;
	delete[] r.base;
	r.base = new T[r.length + 1];
	r.str = r.base;
	for (i = 0; x < y; i++)
	{
		r.str[i] = in[x];
		x++;
	}
	r.str[r.length] = 0;
	return r;
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::gotoWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Strict<T>::gotoWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::gotoWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	unsigned int x = 0;
	bool y = true;
	for (unsigned int i = 0; i < pos || y == true; x++)
	{
		if (in.get(x) == 0) return Jupiter::CString_Strict<T>();
		if (Jupiter::strpbrk<T>(whitespace, in.get(x)) != nullptr)
		{
			if (y != 1)
			{
				y = true;
				i++;
			}
		}
		else
		{
			if (i >= pos) break;
			y = false;
		}
	}
	return Jupiter::CString_Strict<T>::substring(in, x);
}

template<typename T> size_t Jupiter::CString_Strict<T>::set(const String_Type<T> &in)
{
	size_t sSize = in.size();
	if (Jupiter::String_Type<T>::length < sSize)
	{
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = new T[sSize + 1];
	}
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = sSize;
	size_t index;
	for (index = 0; index < Jupiter::String_Type<T>::length && in.get(index) != 0; index++) Jupiter::String_Type<T>::str[index] = in.get(index);
	Jupiter::String_Type<T>::length = index;
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Strict<T>::set(const std::basic_string<T> &in)
{
	size_t sSize = in.size();
	if (Jupiter::String_Type<T>::length < sSize)
	{
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = new T[sSize + 1];
	}
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = sSize;
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in.c_str());
	return sSize;
}

template<typename T> size_t Jupiter::CString_Strict<T>::set(const T *in)
{
	size_t sSize = Jupiter::strlen<T>(in);
	if (Jupiter::String_Type<T>::length < sSize)
	{
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = new T[sSize + 1];
	}
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = sSize;
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in);
	return sSize;
}

template<typename T> size_t Jupiter::CString_Strict<T>::set(const T in)
{
	if (in == 0)
	{
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		*Jupiter::String_Type<T>::str = 0;
		return Jupiter::String_Type<T>::length = 0;
	}
	if (Jupiter::String_Type<T>::length < 1)
	{
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = new T[2];
	}
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	*Jupiter::String_Type<T>::str = in;
	Jupiter::String_Type<T>::str[1] = 0;
	return Jupiter::String_Type<T>::length = 1;
}

template<typename T> size_t Jupiter::CString_Strict<T>::setString(T *in)
{
	return Jupiter::CString_Strict<T>::setString(in, Jupiter::strlen<T>(in));
}

template<typename T> size_t Jupiter::CString_Strict<T>::setString(T *in, size_t size)
{
	delete[] Jupiter::Shift_String_Type<T>::base;
	Jupiter::Shift_String_Type<T>::base = in;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	return Jupiter::String_Type<T>::length = size;
}

template<typename T> size_t Jupiter::CString_Strict<T>::concat(const String_Type<T> &in)
{
	size_t nSize = Jupiter::String_Type<T>::length + in.size();
	T *tmp = new T[nSize + 1];
	Jupiter::strcpy<T>(tmp, Jupiter::String_Type<T>::str); // str guaranteed to be null-terminated.
	size_t index;
	for (index = Jupiter::String_Type<T>::length; index < nSize && in.get(index - Jupiter::String_Type<T>::length); index++) Jupiter::String_Type<T>::str[index] = in.get(index - Jupiter::String_Type<T>::length);
	Jupiter::String_Type<T>::str[index] = 0;
	delete[] Jupiter::Shift_String_Type<T>::base;
	Jupiter::Shift_String_Type<T>::base = tmp;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = index;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Strict<T>::concat(const std::basic_string<T> &in)
{
	size_t nSize = Jupiter::String_Type<T>::length + in.size();
	T *tmp = new T[nSize + 1];
	Jupiter::strcpy<T>(tmp, Jupiter::String_Type<T>::str);
	Jupiter::strcpy<T>(tmp + Jupiter::String_Type<T>::length, in.c_str());
	delete[] Jupiter::Shift_String_Type<T>::base;
	Jupiter::Shift_String_Type<T>::base = tmp;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = nSize;
	return nSize;
}

template<typename T> size_t Jupiter::CString_Strict<T>::concat(const T *in)
{
	size_t nSize = Jupiter::String_Type<T>::length + Jupiter::strlen<T>(in);
	T *tmp = new T[nSize + 1];
	Jupiter::strcpy<T>(tmp, Jupiter::String_Type<T>::str);
	Jupiter::strcpy<T>(tmp + Jupiter::String_Type<T>::length, in);
	delete[] Jupiter::Shift_String_Type<T>::base;
	Jupiter::Shift_String_Type<T>::base = tmp;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = nSize;
	return nSize;
}

template<typename T> size_t Jupiter::CString_Strict<T>::concat(const T c)
{
	size_t nSize = Jupiter::String_Type<T>::length + 1;
	T *tmp = new T[nSize + 1];
	Jupiter::strcpy<T>(tmp, Jupiter::String_Type<T>::str);
	tmp[nSize - 1] = c;
	tmp[nSize] = 0;
	delete[] Jupiter::Shift_String_Type<T>::base;
	Jupiter::Shift_String_Type<T>::base = tmp;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = nSize;
	return nSize;
}

/**
* IMPLEMENTATION:
*	CString_Loose
*/

template<typename T> Jupiter::CString_Loose<T>::CString_Loose() : Jupiter::CString_Loose<T>::CString_Loose(Jupiter::CString_Loose<T>::start_size)
{
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(size_t size)
{
	if (size < Jupiter::CString_Loose<T>::start_size) size = Jupiter::CString_Loose<T>::start_size;
	Jupiter::CString_Loose<T>::strSize = size;
	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	*Jupiter::String_Type<T>::str = 0;
	Jupiter::String_Type<T>::length = 0;
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const Jupiter::CString_Loose<T> &in)
{
	Jupiter::CString_Loose<T>::strSize = in.strSize;
	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in.str);
	Jupiter::String_Type<T>::length = in.length;
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const Jupiter::String_Type<T> &in)
{
	Jupiter::String_Type<T>::length = in.size();
	Jupiter::CString_Loose<T>::strSize = getPowerTwo32(Jupiter::String_Type<T>::length + 1);
	if (Jupiter::CString_Loose<T>::strSize < 8) Jupiter::CString_Loose<T>::strSize = 8;
	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	size_t index;
	for (index = 0; index < Jupiter::String_Type<T>::length && in.get(index) != 0; index++) Jupiter::String_Type<T>::str[index] = in.get(index);
	Jupiter::String_Type<T>::length = index;
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const std::basic_string<T> &in)
{
	Jupiter::String_Type<T>::length = in.size();
	Jupiter::CString_Loose<T>::strSize = getPowerTwo32(Jupiter::String_Type<T>::length + 1);
	if (Jupiter::CString_Loose<T>::strSize < 8) Jupiter::CString_Loose<T>::strSize = 8;
	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in.c_str());
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const T *in)
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

template<typename T> Jupiter::CString_Loose<T>::~CString_Loose()
{
	delete[] Jupiter::Shift_String_Type<T>::base;
}

template<> size_t inline Jupiter::CString_Loose<char>::vformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vsnprintf(nullptr, 0, format, sargs); // Portable as of C++11.
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	if ((unsigned)minLen > Jupiter::CString_Loose<char>::strSize)
	{
		delete[] Jupiter::Shift_String_Type<char>::base;
		Jupiter::CString_Loose<char>::strSize = getPowerTwo32(minLen + 1);
		Jupiter::Shift_String_Type<char>::base = new char[Jupiter::CString_Loose<char>::strSize];
	}
	Jupiter::String_Type<char>::str = Jupiter::Shift_String_Type<char>::base;
	Jupiter::String_Type<char>::length = minLen;
	vsnprintf(Jupiter::String_Type<char>::str, Jupiter::CString_Loose<char>::strSize, format, args);
	Jupiter::String_Type<char>::str[Jupiter::String_Type<char>::length] = 0;
	return Jupiter::String_Type<char>::length;
}

template<> size_t inline Jupiter::CString_Loose<wchar_t>::vformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vswprintf(nullptr, 0, format, sargs); // Portable as of C++11.
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	if ((unsigned)minLen > Jupiter::CString_Loose<wchar_t>::strSize)
	{
		delete[] Jupiter::CString_Type<wchar_t>::base;
		Jupiter::CString_Loose<wchar_t>::strSize = getPowerTwo32(minLen + 1);
		Jupiter::CString_Type<wchar_t>::base = new wchar_t[Jupiter::CString_Loose<wchar_t>::strSize];
	}
	Jupiter::String_Type<wchar_t>::str = Jupiter::CString_Type<wchar_t>::base;
	Jupiter::String_Type<wchar_t>::length = minLen;
	vswprintf(Jupiter::String_Type<wchar_t>::str, Jupiter::CString_Loose<wchar_t>::strSize, format, args);
	Jupiter::String_Type<wchar_t>::str[Jupiter::String_Type<wchar_t>::length] = 0;
	return Jupiter::String_Type<wchar_t>::length;
}

template<typename T> size_t Jupiter::CString_Loose<T>::vformat(const T *format, va_list args)
{
	return 0;
}

template<> size_t inline Jupiter::CString_Loose<char>::avformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vsnprintf(nullptr, 0, format, sargs); // Portable as of C++11.
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	minLen += Jupiter::String_Type<char>::length;
	if ((unsigned)minLen + 1 > Jupiter::CString_Loose<char>::strSize)
	{
		Jupiter::CString_Loose<char>::strSize = getPowerTwo32(minLen + 1);
		char *tmpStr = new char[Jupiter::CString_Loose<char>::strSize];
		Jupiter::strcpy<char>(tmpStr, Jupiter::String_Type<char>::str);
		delete[] Jupiter::Shift_String_Type<char>::base;
		Jupiter::Shift_String_Type<char>::base = tmpStr;
		Jupiter::String_Type<char>::str = Jupiter::Shift_String_Type<char>::base;
	}
	else if ((unsigned)minLen + 1 + (Jupiter::String_Type<char>::str - Jupiter::Shift_String_Type<char>::base) > Jupiter::CString_Loose<char>::strSize)
	{
		Jupiter::strcpy<char>(Jupiter::Shift_String_Type<char>::base, Jupiter::String_Type<char>::str);
		Jupiter::String_Type<char>::str = Jupiter::Shift_String_Type<char>::base;
	}
	minLen = vsnprintf(Jupiter::String_Type<char>::str + Jupiter::String_Type<char>::length, Jupiter::CString_Loose<char>::strSize - Jupiter::String_Type<char>::length, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<char>::length += minLen;
	Jupiter::String_Type<char>::str[Jupiter::String_Type<char>::length] = 0;
	return minLen;
}

template<> size_t inline Jupiter::CString_Loose<wchar_t>::avformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vswprintf(nullptr, 0, format, sargs); // Portable as of C++11.
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	minLen += Jupiter::String_Type<wchar_t>::length;
	if ((unsigned)minLen + 1 > Jupiter::CString_Loose<wchar_t>::strSize)
	{
		Jupiter::CString_Loose<wchar_t>::strSize = getPowerTwo32(minLen + 1);
		wchar_t *tmpStr = new wchar_t[Jupiter::CString_Loose<wchar_t>::strSize];
		Jupiter::strcpy<wchar_t>(tmpStr, Jupiter::String_Type<wchar_t>::str);
		delete[] Jupiter::CString_Type<wchar_t>::base;
		Jupiter::CString_Type<wchar_t>::base = tmpStr;
		Jupiter::String_Type<wchar_t>::str = Jupiter::CString_Type<wchar_t>::base;
	}
	else if ((unsigned)minLen + 1 + (Jupiter::String_Type<wchar_t>::str - Jupiter::CString_Type<wchar_t>::base) > Jupiter::CString_Loose<wchar_t>::strSize)
	{
		Jupiter::strcpy(Jupiter::CString_Type<wchar_t>::base, Jupiter::String_Type<wchar_t>::str);
		Jupiter::String_Type<wchar_t>::str = Jupiter::CString_Type<wchar_t>::base;
	}
	minLen = vswprintf(Jupiter::String_Type<wchar_t>::str + Jupiter::String_Type<wchar_t>::length, Jupiter::CString_Loose<wchar_t>::strSize - Jupiter::String_Type<wchar_t>::length, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<wchar_t>::length += minLen;
	Jupiter::String_Type<wchar_t>::str[Jupiter::String_Type<wchar_t>::length] = 0;
	return minLen;
}

template<typename T> size_t Jupiter::CString_Loose<T>::avformat(const T *format, va_list args)
{
	return 0;
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
	for (index = pos; index != in.size() && in.get(index) != 0; index++) r.str[index - pos] = in.get(index);
	r.str[index - pos] = 0;
	r.length = index;
	return r;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::substring(const Jupiter::String_Type<T> &in, size_t pos, size_t length)
{
	if (pos > in.size()) return Jupiter::CString_Loose<T>();
	if (length > in.size() - pos) length = in.size() - pos;
	Jupiter::CString_Loose<T> r = Jupiter::CString_Loose<T>(length);
	size_t index;
	for (index = 0; index != length && in.get(index + pos) != 0; index++) r.str[index] = in.get(index + pos);
	r.str[index] = 0;
	r.length = index;
	return r;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::getWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Loose<T>::getWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::getWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	unsigned int x = 0;
	unsigned int y = 1;
	for (unsigned int i = 0; i < pos || y == 1; x++)
	{
		if (in.get(x) == 0) return Jupiter::CString_Loose<T>();
		if (Jupiter::strpbrk<T>(whitespace, in.get(x)) != nullptr)
		{
			if (y != 1)
			{
				y = 1;
				i++;
			}
		}
		else
		{
			if (i >= pos) break;
			y = 0;
		}
	}
	for (y = x; Jupiter::strpbrk<T>(whitespace, in.get(y)) == nullptr && in.get(y) != 0; y++);
	return Jupiter::CString_Loose<T>::substring(in, x, y - x);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::getWord(const T *in, size_t pos, const T *whitespace)
{
	Jupiter::CString_Loose<T> r;
	unsigned int x = 0;
	unsigned int y = 1;
	unsigned int i;
	for (i = 0; i < pos || y == 1; x++)
	{
		if (in[x] == 0) return r;
		if (Jupiter::strpbrk<T>(whitespace, in[x]) != nullptr)
		{
			if (y != 1)
			{
				y = 1;
				i++;
			}
		}
		else
		{
			if (i >= pos) break;
			y = 0;
		}
	}
	for (y = x; Jupiter::strpbrk<T>(whitespace, in[y]) == nullptr && in[y] != 0; y++);
	r.length = y - x;
	r.strSize = getPowerTwo32(r.length + 1);
	delete[] r.base;
	r.base = new T[r.strSize];
	r.str = r.base;
	for (i = 0; x < y; i++)
	{
		r.str[i] = in[x];
		x++;
	}
	r.str[i] = 0;
	return r;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::gotoWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Loose<T>::gotoWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::gotoWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	unsigned int x = 0;
	bool y = true;
	for (unsigned int i = 0; i < pos || y == true; x++)
	{
		if (in.get(x) == 0) return Jupiter::CString_Loose<T>();
		if (Jupiter::strpbrk<T>(whitespace, in.get(x)) != nullptr)
		{
			if (y == false)
			{
				y = true;
				i++;
			}
		}
		else
		{
			if (i >= pos) break;
			y = false;
		}
	}
	return Jupiter::CString_Loose<T>::substring(in, x);
}

template<typename T> size_t Jupiter::CString_Loose<T>::set(const CString_Loose<T> &in)
{
	if (Jupiter::CString_Loose<T>::strSize <= in.length)
	{
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::CString_Loose<T>::strSize = in.strSize;
		Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	}
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = in.length;
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in.str);
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Loose<T>::set(const String_Type<T> &in)
{
	size_t sSize = in.size() + 1;
	if (Jupiter::CString_Loose<T>::strSize < sSize)
	{
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(sSize);
		Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	}
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = sSize - 1;
	size_t index;
	for (index = 0; index != Jupiter::String_Type<T>::length && in.get(index) != 0; index++) Jupiter::String_Type<T>::str[index] = in.get(index);
	Jupiter::String_Type<T>::length = index;
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Loose<T>::set(const std::basic_string<T> &in)
{
	size_t sSize = in.size() + 1;
	if (Jupiter::CString_Loose<T>::strSize < sSize)
	{
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(sSize);
		Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	}
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = sSize - 1;
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in.c_str());
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Loose<T>::set(const T *in)
{
	size_t sSize = Jupiter::strlen<T>(in) + 1;
	if (Jupiter::CString_Loose<T>::strSize < sSize)
	{
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(sSize);
		Jupiter::Shift_String_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	}
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = sSize - 1;
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str, in);
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Loose<T>::set(const T in)
{
	if (in == 0)
	{
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		*Jupiter::String_Type<T>::str = 0;
		return Jupiter::String_Type<T>::length = 0;
	}
	Jupiter::String_Type<T>::length = 1;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	*Jupiter::String_Type<T>::str = in;
	Jupiter::String_Type<T>::str[1] = 0;
	return 1;
}

template<typename T> size_t Jupiter::CString_Loose<T>::concat(const String_Type<T> &in)
{
	size_t nLen = Jupiter::String_Type<T>::length + in.size();
	if (nLen + 1 > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(nLen + 1);
		T *tmp = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::strcpy<T>(tmp, Jupiter::String_Type<T>::str);
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = tmp;
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	}
	else if (nLen + 1 + (Jupiter::String_Type<T>::str - Jupiter::Shift_String_Type<T>::base) > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::strcpy<T>(Jupiter::Shift_String_Type<T>::base, Jupiter::String_Type<T>::str);
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	}
	size_t index;
	for (index = Jupiter::String_Type<T>::length; index < nLen && in.get(index - Jupiter::String_Type<T>::length); index++) Jupiter::String_Type<T>::str[index] = in.get(index - Jupiter::String_Type<T>::length);
	Jupiter::String_Type<T>::length = index;
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Loose<T>::concat(const std::basic_string<T> &in)
{
	size_t nLen = Jupiter::String_Type<T>::length + in.size();
	if (nLen + 1 > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(nLen + 1);
		T *tmp = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::strcpy<T>(tmp, Jupiter::String_Type<T>::str);
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = tmp;
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	}
	else if (nLen + 1 + (Jupiter::String_Type<T>::str - Jupiter::Shift_String_Type<T>::base) > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::strcpy<T>(Jupiter::Shift_String_Type<T>::base, Jupiter::String_Type<T>::str);
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	}
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str + Jupiter::String_Type<T>::length, in.c_str());
	Jupiter::String_Type<T>::length = nLen;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Loose<T>::concat(const T *in)
{
	size_t nLen = Jupiter::String_Type<T>::length + Jupiter::strlen<T>(in);
	if (nLen + 1 > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(nLen + 1);
		T *tmp = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::strcpy<T>(tmp, Jupiter::String_Type<T>::str);
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = tmp;
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	}
	else if (nLen + 1 + (Jupiter::String_Type<T>::str - Jupiter::Shift_String_Type<T>::base) > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::strcpy<T>(Jupiter::Shift_String_Type<T>::base, Jupiter::String_Type<T>::str);
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	}
	Jupiter::strcpy<T>(Jupiter::String_Type<T>::str + Jupiter::String_Type<T>::length, in);
	Jupiter::String_Type<T>::length = nLen;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::CString_Loose<T>::concat(const T in)
{
	if (Jupiter::String_Type<T>::length + 1 == Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(Jupiter::CString_Loose<T>::strSize + 1);
		T *tmp = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::strcpy<T>(tmp, Jupiter::String_Type<T>::str);
		delete[] Jupiter::Shift_String_Type<T>::base;
		Jupiter::Shift_String_Type<T>::base = tmp;
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	}
	else if (Jupiter::String_Type<T>::length + 1 + (Jupiter::String_Type<T>::str - Jupiter::Shift_String_Type<T>::base) >= Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::strcpy<T>(Jupiter::Shift_String_Type<T>::base, Jupiter::String_Type<T>::str);
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	}
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = in;
	Jupiter::String_Type<T>::length++;
	Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = 0;
	return Jupiter::String_Type<T>::length;
}

#endif // _CSTRING_IMP_H_HEADER