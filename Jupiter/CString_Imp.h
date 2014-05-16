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

#include "Functions.h"
#include "CString.h"

/**
 * @file CString_Imp.h
 * @brief Provides the implementations for CString_Type functions, as well as extending classes.
 * Note: Modification of this file is not supported in any way.
 */

/**
* IMPLEMENTATION:
*	CString_Type
*/

template<typename T> const T *Jupiter::CString_Type<T>::c_str() const
{
	return Jupiter::CString_Type<T>::curr;
}

template<typename T> size_t Jupiter::CString_Type<T>::size() const
{
	return Jupiter::CString_Type<T>::strLen;
}

template<typename T> int Jupiter::CString_Type<T>::compare(const Jupiter::String_Type<T> &in) const
{
	return Jupiter::CString_Type<T>::compare(in.c_str());
}

template<typename T> int Jupiter::CString_Type<T>::compare(const std::basic_string<T> &in) const
{
	return Jupiter::CString_Type<T>::compare(in.c_str());
}

template<typename T> int Jupiter::CString_Type<T>::compare(const T *s2) const
{
	const T *s1 = Jupiter::CString_Type<T>::curr;
	while (*s1 != 0 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

template<typename T> int Jupiter::CString_Type<T>::compare(const T s2) const
{
	return *Jupiter::CString_Type<T>::curr - s2;
}

template<typename T> bool Jupiter::CString_Type<T>::equals(const Jupiter::String_Type<T> &in) const
{
	if (Jupiter::CString_Type<T>::strLen != in.size()) return false;
	return Jupiter::CString_Type<T>::equals(in.c_str());
}

template<typename T> bool Jupiter::CString_Type<T>::equals(const std::basic_string<T> &in) const
{
	if (Jupiter::CString_Type<T>::strLen != in.size()) return false;
	return Jupiter::CString_Type<T>::equals(in.c_str());
}

template<typename T> bool Jupiter::CString_Type<T>::equals(const T *in) const
{
	if (in == nullptr) return Jupiter::CString_Type<T>::strLen == 0;
	return Jupiter::streql<T>(Jupiter::CString_Type<T>::curr, in);
}

template<typename T> bool Jupiter::CString_Type<T>::equals(const T in) const
{
	return *Jupiter::CString_Type<T>::curr == in && ((in == 0) || Jupiter::CString_Type<T>::curr[1] == 0);
}

// equalsi()

template<typename T> bool Jupiter::CString_Type<T>::equalsi(const Jupiter::String_Type<T> &in) const
{
	if (Jupiter::CString_Type<T>::strLen != in.size()) return false;
	return Jupiter::CString_Type<T>::equalsi(in.c_str());
}

template<typename T> bool Jupiter::CString_Type<T>::equalsi(const std::basic_string<T> &in) const
{
	if (Jupiter::CString_Type<T>::strLen != in.size()) return false;
	return Jupiter::CString_Type<T>::equalsi(in.c_str());
}

template<> bool inline Jupiter::CString_Type<char>::equalsi(const char *in) const
{
	return streqli(Jupiter::CString_Type<char>::curr, in);
}

template<> bool inline Jupiter::CString_Type<wchar_t>::equalsi(const wchar_t *in) const
{
	return wstreqli(Jupiter::CString_Type<wchar_t>::curr, in);
}

template<typename T> bool Jupiter::CString_Type<T>::equalsi(const T *in) const
{
	return false; // Concept of "case" not supported for type.
}

template<> bool inline Jupiter::CString_Type<char>::equalsi(const char in) const
{
	return toupper(*Jupiter::CString_Type<char>::curr) == toupper(in) && ((in == 0) || Jupiter::CString_Type<char>::curr[1] == 0);
}

template<> bool inline Jupiter::CString_Type<wchar_t>::equalsi(const wchar_t in) const
{
	return towupper(*Jupiter::CString_Type<wchar_t>::curr) == towupper(in) && ((in == 0) || Jupiter::CString_Type<wchar_t>::curr[1] == 0);
}

template<typename T> bool Jupiter::CString_Type<T>::equalsi(const T in) const
{
	return false; // Concept of "case" not supported for type.
}

// match()

template<typename T> bool Jupiter::CString_Type<T>::match(const Jupiter::String_Type<T> &format) const
{
	return Jupiter::CString_Type<T>::match(format.c_str());
}

template<typename T> bool Jupiter::CString_Type<T>::match(const std::basic_string<T> &format) const
{
	return Jupiter::CString_Type<T>::match(format.c_str());
}

template<> inline bool Jupiter::CString_Type<char>::match(const char *format) const
{
	return strmatch(format, Jupiter::CString_Type<char>::curr);
}

template<> inline bool Jupiter::CString_Type<wchar_t>::match(const wchar_t *format) const
{
	return wstrmatch(format, Jupiter::CString_Type<wchar_t>::curr);
}

template<typename T> bool Jupiter::CString_Type<T>::match(const T *format) const
{
	return false; // Type is not comparable to wildcards.
}

// matchi()

template<typename T> bool Jupiter::CString_Type<T>::matchi(const Jupiter::String_Type<T> &format) const
{
	return Jupiter::CString_Type<T>::matchi(format.c_str());
}

template<typename T> bool Jupiter::CString_Type<T>::matchi(const std::basic_string<T> &format) const
{
	return Jupiter::CString_Type<T>::matchi(format.c_str());
}

template<> bool inline Jupiter::CString_Type<char>::matchi(const char *format) const
{
	return strmatchi(format, Jupiter::CString_Type<char>::curr);
}

template<> bool inline Jupiter::CString_Type<wchar_t>::matchi(const wchar_t *format) const
{
	return wstrmatchi(format, Jupiter::CString_Type<wchar_t>::curr);
}

template<typename T> bool Jupiter::CString_Type<T>::matchi(const T *format) const
{
	return false; // Type is not comparable to wildcards. Concept of "case" not supported for type.
}

// format forwards

template<typename T> size_t Jupiter::CString_Type<T>::format(const String_Type<T> &format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->vformat(format.c_str(), args);
	va_end(args);
	return r;
}

template<typename T> size_t Jupiter::CString_Type<T>::format(const std::basic_string<T> &format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->vformat(format.c_str(), args);
	va_end(args);
	return r;
}

template<typename T> size_t Jupiter::CString_Type<T>::format(const T *format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->vformat(format, args);
	va_end(args);
	return r;
}

// aformat forwards

template<typename T> size_t Jupiter::CString_Type<T>::aformat(const String_Type<T> &format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->avformat(format.c_str(), args);
	va_end(args);
	return r;
}

template<typename T> size_t Jupiter::CString_Type<T>::aformat(const std::basic_string<T> &format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->avformat(format.c_str(), args);
	va_end(args);
	return r;
}

template<typename T> size_t Jupiter::CString_Type<T>::aformat(const T *format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->avformat(format, args);
	va_end(args);
	return r;
}

template<typename T> unsigned int Jupiter::CString_Type<T>::wordCount(const T *whitespace) const
{
	unsigned int result = 0;
	T *p = Jupiter::CString_Type<T>::curr;
	bool prev = true;
	while (*p != 0)
	{
		if (Jupiter::strpbrk<T>(whitespace, *p) == nullptr) // This isn't whitespace!
		{
			if (prev == true) // We just left whitespace!
			{
				prev = false;
				result++;
			}
		}
		else prev = true; // This is whitespace!
		p++;
	}
	return result;
}

template<typename T> size_t Jupiter::CString_Type<T>::truncate(size_t n)
{
	if (n >= Jupiter::CString_Type<T>::strLen)
	{
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
		*Jupiter::CString_Type<T>::curr = 0;
		Jupiter::CString_Type<T>::strLen = 0;
		return 0;
	}
	Jupiter::CString_Type<T>::strLen -= n;
	Jupiter::CString_Type<T>::curr[Jupiter::CString_Type<T>::strLen] = 0;
	return Jupiter::CString_Type<T>::strLen;
}

template<typename T> T &Jupiter::CString_Type<T>::get(size_t index) const
{
	return Jupiter::CString_Type<T>::curr[index];
}

template<typename T> size_t Jupiter::CString_Type<T>::shiftLeft(size_t length)
{
	size_t offset = Jupiter::CString_Type<T>::curr - Jupiter::CString_Type<T>::base;
	if (length > offset) length = offset;
	Jupiter::CString_Type<T>::curr -= length;
	Jupiter::CString_Type<T>::strLen += length;
	return length;
}

template<typename T> size_t Jupiter::CString_Type<T>::shiftRight(size_t length)
{
	if (length > Jupiter::CString_Type<T>::strLen) length = Jupiter::CString_Type<T>::strLen;
	Jupiter::CString_Type<T>::curr += length;
	Jupiter::CString_Type<T>::strLen -= length;
	return length;
}

template<typename T> bool Jupiter::CString_Type<T>::remove(T &value)
{
	for (unsigned int i = 0; i < Jupiter::CString_Type<T>::strLen; i++)
	{
		if (Jupiter::CString_Type<T>::curr[i] == value)
		{
			if (i == strLen - 1) Jupiter::CString_Type<T>::truncate(1);
			else if (i == 0)
			{
				if (Jupiter::CString_Type<T>::strLen == 1) Jupiter::CString_Type<T>::truncate(1);
				else Jupiter::CString_Type<T>::shiftRight(1);
			}
			else
			{
				Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr + i, Jupiter::CString_Type<T>::curr + i + 1);
				Jupiter::CString_Type<T>::strLen--;
			}
			return true;
		}
	}
	return false;
}

template<typename T> bool Jupiter::CString_Type<T>::contains(const T &value)
{
	for (unsigned int i = 0; i < Jupiter::CString_Type<T>::strLen; i++) if (Jupiter::CString_Type<T>::curr[i] == value) return true;
	return false;
}

/**
* IMPLEMENTATION:
*	CString_Strict
*/

template<typename T> Jupiter::CString_Strict<T>::CString_Strict()
{
	Jupiter::CString_Type<T>::base = new T[1];
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	*Jupiter::CString_Type<T>::curr = 0;
	Jupiter::CString_Type<T>::strLen = 0;
}

template<typename T> Jupiter::CString_Strict<T>::CString_Strict(const Jupiter::CString_Strict<T> &in)
{
	Jupiter::CString_Type<T>::strLen = in.strLen;
	Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Type<T>::strLen + 1];
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.curr);
}

template<typename T> Jupiter::CString_Strict<T>::CString_Strict(const Jupiter::String_Type<T> &in)
{
	Jupiter::CString_Type<T>::strLen = in.size();
	Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Type<T>::strLen + 1];
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.c_str());
}

template<typename T> Jupiter::CString_Strict<T>::CString_Strict(const std::basic_string<T> &in)
{
	Jupiter::CString_Type<T>::strLen = in.size();
	Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Type<T>::strLen + 1];
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.c_str());
}

template<typename T> Jupiter::CString_Strict<T>::CString_Strict(const T *in)
{
	if (in == nullptr)
	{
		Jupiter::CString_Type<T>::strLen = 0;
		Jupiter::CString_Type<T>::base = new T[1];
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
		*Jupiter::CString_Type<T>::curr = 0;
	}
	else
	{
		Jupiter::CString_Type<T>::strLen = Jupiter::strlen<T>(in);
		Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Type<T>::strLen + 1];
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
		Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in);
	}
}

template<typename T> Jupiter::CString_Strict<T>::CString_Strict(const T c)
{
	if (c == 0)
	{
		Jupiter::CString_Type<T>::strLen = 0;
		Jupiter::CString_Type<T>::base = new T[1];
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
		*Jupiter::CString_Type<T>::curr = 0;
	}
	else
	{
		Jupiter::CString_Type<T>::strLen = 1;
		Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Type<T>::strLen + 1];
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
		*Jupiter::CString_Type<T>::curr = c;
		Jupiter::CString_Type<T>::curr[1] = 0;
	}
}

template<typename T> Jupiter::CString_Strict<T>::~CString_Strict()
{
	delete[] Jupiter::CString_Type<T>::base;
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
	if ((unsigned)minLen > Jupiter::CString_Type<char>::strLen)
	{
		delete[] Jupiter::CString_Type<char>::base;
		Jupiter::CString_Type<char>::base = new char[minLen + 1];
	}
	Jupiter::CString_Type<char>::curr = Jupiter::CString_Type<char>::base;
	Jupiter::CString_Type<char>::strLen = minLen;
	vsnprintf(Jupiter::CString_Type<char>::curr, Jupiter::CString_Type<char>::strLen, format, args);
	Jupiter::CString_Type<char>::curr[Jupiter::CString_Type<char>::strLen] = 0;
	return Jupiter::CString_Type<char>::strLen;
}

template<> size_t inline Jupiter::CString_Strict<wchar_t>::vformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vswprintf(nullptr, 0, format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	if ((unsigned)minLen > Jupiter::CString_Type<wchar_t>::strLen)
	{
		delete[] Jupiter::CString_Type<wchar_t>::base;
		Jupiter::CString_Type<wchar_t>::base = new wchar_t[minLen + 1];
	}
	Jupiter::CString_Type<wchar_t>::curr = Jupiter::CString_Type<wchar_t>::base;
	Jupiter::CString_Type<wchar_t>::strLen = minLen;
	vswprintf(Jupiter::CString_Type<wchar_t>::curr, Jupiter::CString_Type<wchar_t>::strLen, format, args);
	Jupiter::CString_Type<wchar_t>::curr[Jupiter::CString_Type<wchar_t>::strLen] = 0;
	return Jupiter::CString_Type<wchar_t>::strLen;
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

	char *t = new char[minLen + Jupiter::CString_Type<char>::strLen + 1];
	Jupiter::strcpy<char>(t, Jupiter::CString_Type<char>::curr);
	delete[] Jupiter::CString_Type<char>::base;
	Jupiter::CString_Type<char>::base = t;
	Jupiter::CString_Type<char>::curr = Jupiter::CString_Type<char>::base;

	vsnprintf(Jupiter::CString_Type<char>::curr + Jupiter::CString_Type<char>::strLen, minLen, format, args);
	Jupiter::CString_Type<char>::strLen += minLen;
	Jupiter::CString_Type<char>::curr[Jupiter::CString_Type<char>::strLen] = 0;
	return Jupiter::CString_Type<char>::strLen;
}

template<> size_t inline Jupiter::CString_Strict<wchar_t>::avformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vswprintf(nullptr, 0, format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	wchar_t *t = new wchar_t[minLen + Jupiter::CString_Type<wchar_t>::strLen + 1];
	Jupiter::strcpy<wchar_t>(t, Jupiter::CString_Type<wchar_t>::curr);
	delete[] Jupiter::CString_Type<wchar_t>::base;
	Jupiter::CString_Type<wchar_t>::base = t;
	Jupiter::CString_Type<wchar_t>::curr = Jupiter::CString_Type<wchar_t>::base;

	vswprintf(Jupiter::CString_Type<wchar_t>::curr + Jupiter::CString_Type<wchar_t>::strLen, minLen, format, args);
	Jupiter::CString_Type<wchar_t>::strLen += minLen;
	Jupiter::CString_Type<wchar_t>::curr[Jupiter::CString_Type<wchar_t>::strLen] = 0;
	return Jupiter::CString_Type<wchar_t>::strLen;
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

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::substring(size_t pos, size_t length) const
{
	Jupiter::CString_Strict<T> r;
	r.setString(new char[length + 1], length);
	r.strLen = Jupiter::strlen<T>(Jupiter::strcpy<T>(r.curr, Jupiter::CString_Type<T>::curr + pos, length));
	return r;
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::getWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Strict<T>::getWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::getWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::CString_Strict<T>::getWord(in.c_str(), pos, whitespace);
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
	r.strLen = y - x;
	delete[] r.base;
	r.base = new char[r.strLen + 1];
	r.curr = r.base;
	for (i = 0; x < y; i++)
	{
		r.curr[i] = in[x];
		x++;
	}
	r.curr[r.strLen] = 0;
	return r;
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::gotoWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Strict<T>::gotoWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Strict<T> Jupiter::CString_Strict<T>::gotoWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	Jupiter::CString_Strict<T> r;
	unsigned int x = 0;
	bool y = true;
	for (unsigned int i = 0; i < pos || y == true; x++)
	{
		if (in.get(x) == 0) return r;
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
	// Remake later to get character-by-character -- c_str not guaranteed to be fast.
	r = in.c_str() + x;
	return r;
}

template<typename T> size_t Jupiter::CString_Strict<T>::set(const String_Type<T> &in)
{
	size_t sSize = in.size();
	if (Jupiter::CString_Type<T>::strLen < sSize)
	{
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Type<T>::base = new T[sSize + 1];
	}
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = sSize;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.c_str());
	return sSize;
}

template<typename T> size_t Jupiter::CString_Strict<T>::set(const std::basic_string<T> &in)
{
	size_t sSize = in.size();
	if (Jupiter::CString_Type<T>::strLen < sSize)
	{
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Type<T>::base = new T[sSize + 1];
	}
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = sSize;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.c_str());
	return sSize;
}

template<typename T> size_t Jupiter::CString_Strict<T>::set(const T *in)
{
	size_t sSize = Jupiter::strlen<T>(in);
	if (Jupiter::CString_Type<T>::strLen < sSize)
	{
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Type<T>::base = new T[sSize + 1];
	}
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = sSize;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in);
	return sSize;
}

template<typename T> size_t Jupiter::CString_Strict<T>::set(const T in)
{
	if (in == 0)
	{
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
		*Jupiter::CString_Type<T>::curr = 0;
		return Jupiter::CString_Type<T>::strLen = 0;
	}
	if (Jupiter::CString_Type<T>::strLen < 1)
	{
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Type<T>::base = new T[2];
	}
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	*Jupiter::CString_Type<T>::curr = in;
	Jupiter::CString_Type<T>::curr[1] = 0;
	return Jupiter::CString_Type<T>::strLen = 1;
}

template<typename T> size_t Jupiter::CString_Strict<T>::setString(T *in)
{
	return Jupiter::CString_Strict<T>::setString(in, Jupiter::strlen<T>(in));
}

template<typename T> size_t Jupiter::CString_Strict<T>::setString(T *in, size_t size)
{
	delete[] Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::base = in;
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	return Jupiter::CString_Type<T>::strLen = size;
}

template<typename T> size_t Jupiter::CString_Strict<T>::concat(const String_Type<T> &in)
{
	size_t nSize = Jupiter::CString_Type<T>::strLen + in.size();
	T *tmp = new T[nSize + 1];
	Jupiter::strcpy<T>(tmp, Jupiter::CString_Type<T>::curr);
	Jupiter::strcpy<T>(tmp + Jupiter::CString_Type<T>::strLen, in.c_str());
	delete[] Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::base = tmp;
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = nSize;
	return nSize;
}

template<typename T> size_t Jupiter::CString_Strict<T>::concat(const std::basic_string<T> &in)
{
	size_t nSize = Jupiter::CString_Type<T>::strLen + in.size();
	T *tmp = new T[nSize + 1];
	Jupiter::strcpy<T>(tmp, Jupiter::CString_Type<T>::curr);
	Jupiter::strcpy<T>(tmp + Jupiter::CString_Type<T>::strLen, in.c_str());
	delete[] Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::base = tmp;
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = nSize;
	return nSize;
}

template<typename T> size_t Jupiter::CString_Strict<T>::concat(const T *in)
{
	size_t nSize = Jupiter::CString_Type<T>::strLen + Jupiter::strlen<T>(in);
	T *tmp = new T[nSize + 1];
	Jupiter::strcpy<T>(tmp, Jupiter::CString_Type<T>::curr);
	Jupiter::strcpy<T>(tmp + Jupiter::CString_Type<T>::strLen, in);
	delete[] Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::base = tmp;
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = nSize;
	return nSize;
}

template<typename T> size_t Jupiter::CString_Strict<T>::concat(const T c)
{
	size_t nSize = Jupiter::CString_Type<T>::strLen + 1;
	T *tmp = new T[nSize + 1];
	Jupiter::strcpy<T>(tmp, Jupiter::CString_Type<T>::curr);
	tmp[nSize - 1] = c;
	tmp[nSize] = 0;
	delete[] Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::base = tmp;
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = nSize;
	return nSize;
}

/**
* IMPLEMENTATION:
*	CString_Loose
*/

template<typename T> Jupiter::CString_Loose<T>::CString_Loose()
{
	Jupiter::CString_Loose<T>::strSize = Jupiter::CString_Loose<T>::start_size;
	Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	*Jupiter::CString_Type<T>::curr = 0;
	Jupiter::CString_Type<T>::strLen = 0;
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const Jupiter::CString_Loose<T> &in)
{
	Jupiter::CString_Loose<T>::strSize = in.strSize;
	Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.curr);
	Jupiter::CString_Type<T>::strLen = in.strLen;
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const Jupiter::String_Type<T> &in)
{
	Jupiter::CString_Type<T>::strLen = in.size();
	Jupiter::CString_Loose<T>::strSize = getPowerTwo32(Jupiter::CString_Type<T>::strLen + 1);
	if (Jupiter::CString_Loose<T>::strSize < 8) Jupiter::CString_Loose<T>::strSize = 8;
	Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.c_str());
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const std::basic_string<T> &in)
{
	Jupiter::CString_Type<T>::strLen = in.size();
	Jupiter::CString_Loose<T>::strSize = getPowerTwo32(Jupiter::CString_Type<T>::strLen + 1);
	if (Jupiter::CString_Loose<T>::strSize < 8) Jupiter::CString_Loose<T>::strSize = 8;
	Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.c_str());
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const T *in)
{
	if (in == nullptr)
	{
		Jupiter::CString_Loose<T>::strSize = Jupiter::CString_Loose<T>::start_size;
		Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
		*Jupiter::CString_Type<T>::curr = 0;
		Jupiter::CString_Type<T>::strLen = 0;
	}
	else
	{
		Jupiter::CString_Type<T>::strLen = Jupiter::strlen<T>(in);
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(Jupiter::CString_Type<T>::strLen + 1);
		if (Jupiter::CString_Loose<T>::strSize < 8) Jupiter::CString_Loose<T>::strSize = 8;
		Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
		Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in);
	}
}

template<typename T> Jupiter::CString_Loose<T>::CString_Loose(const T in)
{
	Jupiter::CString_Loose<T>::strSize = Jupiter::CString_Loose<T>::start_size;
	Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	if (in == 0)
	{
		Jupiter::CString_Type<T>::strLen = 0;
		*Jupiter::CString_Type<T>::curr = 0;
	}
	else
	{
		Jupiter::CString_Type<T>::strLen = 1;
		*Jupiter::CString_Type<T>::curr = in;
		Jupiter::CString_Type<T>::curr[1] = 0;
	}
}

template<typename T> Jupiter::CString_Loose<T>::~CString_Loose()
{
	delete[] Jupiter::CString_Type<T>::base;
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
		delete[] Jupiter::CString_Type<char>::base;
		Jupiter::CString_Loose<char>::strSize = getPowerTwo32(minLen + 1);
		Jupiter::CString_Type<char>::base = new char[Jupiter::CString_Loose<char>::strSize];
	}
	Jupiter::CString_Type<char>::curr = Jupiter::CString_Type<char>::base;
	Jupiter::CString_Type<char>::strLen = minLen;
	vsnprintf(Jupiter::CString_Type<char>::curr, Jupiter::CString_Loose<char>::strSize, format, args);
	Jupiter::CString_Type<char>::curr[Jupiter::CString_Type<char>::strLen] = 0;
	return Jupiter::CString_Type<char>::strLen;
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
	Jupiter::CString_Type<wchar_t>::curr = Jupiter::CString_Type<wchar_t>::base;
	Jupiter::CString_Type<wchar_t>::strLen = minLen;
	vswprintf(Jupiter::CString_Type<wchar_t>::curr, Jupiter::CString_Loose<wchar_t>::strSize, format, args);
	Jupiter::CString_Type<wchar_t>::curr[Jupiter::CString_Type<wchar_t>::strLen] = 0;
	return Jupiter::CString_Type<wchar_t>::strLen;
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
	minLen += Jupiter::CString_Type<char>::strLen;
	if ((unsigned)minLen + 1 > Jupiter::CString_Loose<char>::strSize)
	{
		Jupiter::CString_Loose<char>::strSize = getPowerTwo32(minLen + 1);
		char *tmpStr = new char[Jupiter::CString_Loose<char>::strSize];
		Jupiter::strcpy<char>(tmpStr, Jupiter::CString_Type<char>::curr);
		delete[] Jupiter::CString_Type<char>::base;
		Jupiter::CString_Type<char>::base = tmpStr;
		Jupiter::CString_Type<char>::curr = Jupiter::CString_Type<char>::base;
	}
	else if ((unsigned)minLen + 1 + (Jupiter::CString_Type<char>::curr - Jupiter::CString_Type<char>::base) > Jupiter::CString_Loose<char>::strSize)
	{
		Jupiter::strcpy<char>(Jupiter::CString_Type<char>::base, Jupiter::CString_Type<char>::curr);
		Jupiter::CString_Type<char>::curr = Jupiter::CString_Type<char>::base;
	}
	vsnprintf(Jupiter::CString_Type<char>::curr + Jupiter::CString_Type<char>::strLen, Jupiter::CString_Loose<char>::strSize - Jupiter::CString_Type<char>::strLen, format, args);
	Jupiter::CString_Type<char>::strLen = minLen;
	Jupiter::CString_Type<char>::curr[Jupiter::CString_Type<char>::strLen] = 0;
	return Jupiter::CString_Type<char>::strLen;
}

template<> size_t inline Jupiter::CString_Loose<wchar_t>::avformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = vswprintf(nullptr, 0, format, sargs); // Portable as of C++11.
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	minLen += Jupiter::CString_Type<wchar_t>::strLen;
	if ((unsigned)minLen + 1 > Jupiter::CString_Loose<wchar_t>::strSize)
	{
		Jupiter::CString_Loose<wchar_t>::strSize = getPowerTwo32(minLen + 1);
		wchar_t *tmpStr = new wchar_t[Jupiter::CString_Loose<wchar_t>::strSize];
		Jupiter::strcpy<wchar_t>(tmpStr, Jupiter::CString_Type<wchar_t>::curr);
		delete[] Jupiter::CString_Type<wchar_t>::base;
		Jupiter::CString_Type<wchar_t>::base = tmpStr;
		Jupiter::CString_Type<wchar_t>::curr = Jupiter::CString_Type<wchar_t>::base;
	}
	else if ((unsigned)minLen + 1 + (Jupiter::CString_Type<wchar_t>::curr - Jupiter::CString_Type<wchar_t>::base) > Jupiter::CString_Loose<wchar_t>::strSize)
	{
		Jupiter::strcpy(Jupiter::CString_Type<wchar_t>::base, Jupiter::CString_Type<wchar_t>::curr);
		Jupiter::CString_Type<wchar_t>::curr = Jupiter::CString_Type<wchar_t>::base;
	}
	vswprintf(Jupiter::CString_Type<wchar_t>::curr + Jupiter::CString_Type<wchar_t>::strLen, Jupiter::CString_Loose<wchar_t>::strSize - Jupiter::CString_Type<wchar_t>::strLen, format, args);
	Jupiter::CString_Type<wchar_t>::strLen = minLen;
	Jupiter::CString_Type<wchar_t>::curr[Jupiter::CString_Type<wchar_t>::strLen] = 0;
	return Jupiter::CString_Type<wchar_t>::strLen;
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

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::substring(size_t pos, size_t length) const
{
	Jupiter::CString_Loose<T> r;
	r.strSize = getPowerTwo32(length + 1);
	delete[] r.base;
	r.base = new char[r.strSize];
	r.curr = r.base;
	r.strLen = length;
	Jupiter::strcpy<T>(r.curr, Jupiter::CString_Type<T>::curr + pos, length);
	return r;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::getWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Loose<T>::getWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::getWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::CString_Loose<T>::getWord(in.c_str(), pos, whitespace);
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
	r.strLen = y - x;
	r.strSize = getPowerTwo32(r.strLen + 1);
	delete[] r.base;
	r.base = new char[r.strSize];
	r.curr = r.base;
	for (i = 0; x < y; i++)
	{
		r.curr[i] = in[x];
		x++;
	}
	r.curr[i] = 0;
	return r;
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::gotoWord(size_t pos, const T *whitespace) const
{
	return Jupiter::CString_Loose<T>::gotoWord(*this, pos, whitespace);
}

template<typename T> Jupiter::CString_Loose<T> Jupiter::CString_Loose<T>::gotoWord(const Jupiter::String_Type<T> &in, size_t pos, const T *whitespace)
{
	Jupiter::CString_Loose<T> r;
	unsigned int x = 0;
	bool y = true;
	for (unsigned int i = 0; i < pos || y == true; x++)
	{
		if (in.get(x) == 0) return r;
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
	// Remake later to get character-by-character -- c_str not guaranteed to be fast.
	r = in.c_str() + x;
	return r;
}

template<typename T> size_t Jupiter::CString_Loose<T>::set(const CString_Loose<T> &in)
{
	if (Jupiter::CString_Loose<T>::strSize <= in.strLen)
	{
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Loose<T>::strSize = in.strSize;
		Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	}
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = in.strLen;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.curr);
	return Jupiter::CString_Type<T>::strLen;
}

template<typename T> size_t Jupiter::CString_Loose<T>::set(const String_Type<T> &in)
{
	size_t sSize = in.size() + 1;
	if (Jupiter::CString_Loose<T>::strSize < sSize)
	{
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(sSize);
		Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	}
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = sSize - 1;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.c_str());
	return Jupiter::CString_Type<T>::strLen;
}

template<typename T> size_t Jupiter::CString_Loose<T>::set(const std::basic_string<T> &in)
{
	size_t sSize = in.size() + 1;
	if (Jupiter::CString_Loose<T>::strSize < sSize)
	{
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(sSize);
		Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	}
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = sSize - 1;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in.c_str());
	return Jupiter::CString_Type<T>::strLen;
}

template<typename T> size_t Jupiter::CString_Loose<T>::set(const T *in)
{
	size_t sSize = Jupiter::strlen<T>(in) + 1;
	if (Jupiter::CString_Loose<T>::strSize < sSize)
	{
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(sSize);
		Jupiter::CString_Type<T>::base = new T[Jupiter::CString_Loose<T>::strSize];
	}
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	Jupiter::CString_Type<T>::strLen = sSize - 1;
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr, in);
	return Jupiter::CString_Type<T>::strLen;
}

template<typename T> size_t Jupiter::CString_Loose<T>::set(const T in)
{
	if (in == 0)
	{
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
		*Jupiter::CString_Type<T>::curr = 0;
		return Jupiter::CString_Type<T>::strLen = 0;
	}
	Jupiter::CString_Type<T>::strLen = 1;
	Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	*Jupiter::CString_Type<T>::curr = in;
	Jupiter::CString_Type<T>::curr[1] = 0;
	return 1;
}

template<typename T> size_t Jupiter::CString_Loose<T>::concat(const String_Type<T> &in)
{
	size_t nLen = Jupiter::CString_Type<T>::strLen + in.size();
	if (nLen + 1 > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(nLen + 1);
		T *tmp = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::strcpy<T>(tmp, Jupiter::CString_Type<T>::curr);
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Type<T>::base = tmp;
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	}
	else if (nLen + 1 + (Jupiter::CString_Type<T>::curr - Jupiter::CString_Type<T>::base) > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::strcpy<T>(Jupiter::CString_Type<T>::base, Jupiter::CString_Type<T>::curr);
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	}
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr + Jupiter::CString_Type<T>::strLen, in.c_str());
	Jupiter::CString_Type<T>::strLen = nLen;
	return Jupiter::CString_Type<T>::strLen;
}

template<typename T> size_t Jupiter::CString_Loose<T>::concat(const std::basic_string<T> &in)
{
	size_t nLen = Jupiter::CString_Type<T>::strLen + in.size();
	if (nLen + 1 > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(nLen + 1);
		T *tmp = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::strcpy<T>(tmp, Jupiter::CString_Type<T>::curr);
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Type<T>::base = tmp;
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	}
	else if (nLen + 1 + (Jupiter::CString_Type<T>::curr - Jupiter::CString_Type<T>::base) > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::strcpy<T>(Jupiter::CString_Type<T>::base, Jupiter::CString_Type<T>::curr);
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	}
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr + Jupiter::CString_Type<T>::strLen, in.c_str());
	Jupiter::CString_Type<T>::strLen = nLen;
	return Jupiter::CString_Type<T>::strLen;
}

template<typename T> size_t Jupiter::CString_Loose<T>::concat(const T *in)
{
	size_t nLen = Jupiter::CString_Type<T>::strLen + Jupiter::strlen<T>(in);
	if (nLen + 1 > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(nLen + 1);
		T *tmp = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::strcpy<T>(tmp, Jupiter::CString_Type<T>::curr);
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Type<T>::base = tmp;
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	}
	else if (nLen + 1 + (Jupiter::CString_Type<T>::curr - Jupiter::CString_Type<T>::base) > Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::strcpy<T>(Jupiter::CString_Type<T>::base, Jupiter::CString_Type<T>::curr);
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	}
	Jupiter::strcpy<T>(Jupiter::CString_Type<T>::curr + Jupiter::CString_Type<T>::strLen, in);
	Jupiter::CString_Type<T>::strLen = nLen;
	return Jupiter::CString_Type<T>::strLen;
}

template<typename T> size_t Jupiter::CString_Loose<T>::concat(const T in)
{
	if (Jupiter::CString_Type<T>::strLen + 1 == Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::CString_Loose<T>::strSize = getPowerTwo32(Jupiter::CString_Loose<T>::strSize + 1);
		T *tmp = new T[Jupiter::CString_Loose<T>::strSize];
		Jupiter::strcpy<T>(tmp, Jupiter::CString_Type<T>::curr);
		delete[] Jupiter::CString_Type<T>::base;
		Jupiter::CString_Type<T>::base = tmp;
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	}
	else if (Jupiter::CString_Type<T>::strLen + 1 + (Jupiter::CString_Type<T>::curr - Jupiter::CString_Type<T>::base) >= Jupiter::CString_Loose<T>::strSize)
	{
		Jupiter::strcpy<T>(Jupiter::CString_Type<T>::base, Jupiter::CString_Type<T>::curr);
		Jupiter::CString_Type<T>::curr = Jupiter::CString_Type<T>::base;
	}
	Jupiter::CString_Type<T>::curr[Jupiter::CString_Type<T>::strLen] = in;
	Jupiter::CString_Type<T>::strLen++;
	Jupiter::CString_Type<T>::curr[Jupiter::CString_Type<T>::strLen] = 0;
	return Jupiter::CString_Type<T>::strLen;
}

#endif // _CSTRING_IMP_H_HEADER