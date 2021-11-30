/**
 * Copyright (C) 2013-2017 Jessica James.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Written by Jessica James <jessica.aj@outlook.com>
 */

#if !defined _STRING_IMP_H_HEADER
#define _STRING_IMP_H_HEADER

/**
* @file String_Imp.h
* @brief Provides the implementations for String_Strict and String_Loose.
*/

#include "String.hpp"

#if !defined va_copy

#if defined __INTEL_COMPILER
#pragma message("Warning: va_copy not properly defined. Assuming common implementation.")
#define va_copy(dst, src) ((void)((dst) = (src)))
#else
#error "va_copy not defined."
#endif // __INTEL_COMPILER

#endif // va_copy

#if !defined JUPITER_VSCPRINTF

#if defined _WIN32
#define JUPITER_VSCPRINTF(format, format_args) _vscprintf(format, format_args)
#define JUPITER_VSCWPRINTF(format, format_args) _vscwprintf(format, format_args)
#else // _WIN32
#define JUPITER_VSCPRINTF(format, format_args) vsnprintf(nullptr, 0, format, format_args)
#define JUPITER_VSCWPRINTF(format, format_args) vswprintf(nullptr, 0, format, format_args)
#endif // _WIN32

#endif // JUPITER_VSCPRINTF

/**
* IMPLEMENTATION:
*	String_Strict
*/

template<typename T> Jupiter::String_Strict<T>::String_Strict()
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(size_t len)
{
	Jupiter::Shift_String_Type<T>::base = new T[len];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = 0;
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(Jupiter::String_Strict<T> &&source) : Jupiter::Shift_String_Type<T>(std::move(source))
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::String_Strict<T> &in) : Jupiter::String_Strict<T>::String_Strict(
	in.data(), in.size())
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::Readable_String<T> &in) : Jupiter::String_Strict<T>::String_Strict(
	in.data(), in.size())
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const std::basic_string<T> &in) : Jupiter::String_Strict<T>::String_Strict(in.data(), in.size())
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const T *in, size_t len) : Jupiter::String_Strict<T>::String_Strict(len)
{
	while (Jupiter::String_Type<T>::length != len)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = *in;
		Jupiter::String_Type<T>::length++;
		in++;
	}
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const T *in)
{
	if (in == nullptr) Jupiter::String_Type<T>::length = 0;
	else Jupiter::String_Type<T>::length = Jupiter::strlen<T>(in);

	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::String_Type<T>::length];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	for (size_t index = 0; index != Jupiter::String_Type<T>::length; index++, in++) Jupiter::String_Type<T>::str[index] = *in;
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::DataBuffer &in) : String_Strict(reinterpret_cast<T *>(in.getHead()), in.size() / sizeof(T))
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::Readable_String<T> &lhs, const T &rhs) : String_Strict<T>(lhs.size() + 1)
{
	const T *itr;
	const T *end;

	if (!lhs.empty())
	{
		itr = lhs.data();
		end = itr + lhs.size();
		*Jupiter::String_Type<T>::str = *itr;
		while (++itr != end)
			*++Jupiter::String_Type<T>::str = *itr;
		++Jupiter::String_Type<T>::str;
	}

	*Jupiter::String_Type<T>::str = rhs;

	Jupiter::String_Type<T>::length = Jupiter::String_Type<T>::str - Jupiter::Shift_String_Type<T>::base + 1;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::Readable_String<T> &lhs, const Jupiter::Readable_String<T> &rhs) : String_Strict<T>(lhs,
	rhs.data(), rhs.size())
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::Readable_String<T> &lhs, const std::basic_string<T> &rhs) : String_Strict<T>(lhs, rhs.data(), rhs.size())
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::Readable_String<T> &lhs, const T *rhs) : String_Strict<T>(lhs, rhs, Jupiter::strlen<T>(rhs))
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::Readable_String<T> &lhs, const T *rhs, size_t rhs_size) : String_Strict<T>(lhs.size() + rhs_size)
{
	const T *itr;
	const T *end;

	if (!lhs.empty())
	{
		itr = lhs.data();
		end = itr + lhs.size();
		*Jupiter::String_Type<T>::str = *itr;
		while (++itr != end)
			*++Jupiter::String_Type<T>::str = *itr;
		++Jupiter::String_Type<T>::str;
	}

	if (rhs_size != 0)
	{
		itr = rhs;
		end = itr + rhs_size;
		*Jupiter::String_Type<T>::str = *itr;
		while (++itr != end)
			*++Jupiter::String_Type<T>::str = *itr;
		++Jupiter::String_Type<T>::str;
	}

	Jupiter::String_Type<T>::length = Jupiter::String_Type<T>::str - Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
}

// vformat()

template<> size_t inline Jupiter::String_Strict<char>::vformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	this->setBufferSizeNoCopy(minLen + 1); // vsnprintf REQUIRES space for an additional null character.
	minLen = vsnprintf(Jupiter::String_Type<char>::str, minLen + 1, format, args);
	if (minLen < 0) return 0;
	return Jupiter::String_Type<char>::length = minLen;
}

template<> size_t inline Jupiter::String_Strict<wchar_t>::vformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCWPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	this->setBufferSizeNoCopy(minLen + 1); // vsnprintf REQUIRES space for an additional null character.
	minLen = vswprintf(Jupiter::String_Type<wchar_t>::str, minLen + 1, format, args);
	if (minLen < 0) return 0;
	return Jupiter::String_Type<wchar_t>::length = minLen;
}

template<typename T> size_t Jupiter::String_Strict<T>::vformat(const T *format, va_list args)
{
	return 0;
}

// avformat()

template<> size_t inline Jupiter::String_Strict<char>::avformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	this->setBufferSize(Jupiter::String_Type<char>::length + minLen + 1); // vsnprintf REQUIRES space for an additional null character.
	minLen = vsnprintf(Jupiter::String_Type<char>::str + Jupiter::String_Type<char>::length, minLen + 1, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<char>::length += minLen;
	return minLen;
}

template<> size_t inline Jupiter::String_Strict<wchar_t>::avformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCWPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	this->setBufferSize(minLen + Jupiter::String_Type<wchar_t>::length + 1); // vsnprintf REQUIRES space for an additional null character.

	minLen = vswprintf(Jupiter::String_Type<wchar_t>::str + Jupiter::String_Type<wchar_t>::length, minLen + 1, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<wchar_t>::length += minLen;
	return minLen;
}

template<typename T> size_t Jupiter::String_Strict<T>::avformat(const T *format, va_list args)
{
	return 0;
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::Format(const T *format, ...)
{
	String_Strict<T> r;
	va_list args;
	va_start(args, format);
	r.vformat(format, args);
	va_end(args);
	return r;
}

template<typename T> typename Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(size_t pos) const
{
	return Jupiter::String_Strict<T>::substring(*this, pos);
}

template<typename T> typename Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(size_t pos, size_t len) const
{
	return Jupiter::String_Strict<T>::substring(*this, pos, len);
}

template<typename T> typename Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos)
{
	return Jupiter::String_Type<T>::template substring<Jupiter::template String_Strict>(in, pos);
}

template<typename T> typename Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(const T *in, size_t pos)
{
	return Jupiter::String_Type<T>::template substring<Jupiter::template String_Strict>(in, pos);
}

template<typename T> typename Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t len)
{
	return Jupiter::String_Type<T>::template substring<Jupiter::template String_Strict>(in, pos, len);
}

template<typename T> typename Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(const T *in, size_t pos, size_t len)
{
	return Jupiter::String_Type<T>::template substring<Jupiter::template String_Strict>(in, pos, len);
}

// Operators

template<typename T> inline Jupiter::String_Strict<T> Jupiter::String_Strict<T>::operator+(const T &rhs) const
{
	return Jupiter::operator+(*this, rhs);
}

template<typename T> inline Jupiter::String_Strict<T> Jupiter::String_Strict<T>::operator+(const Jupiter::String_Strict<T> &rhs) const
{
	return Jupiter::String_Strict<T>::operator+(reinterpret_cast<const Jupiter::Readable_String<T> &>(rhs));
}

template<typename T> inline Jupiter::String_Strict<T> Jupiter::String_Strict<T>::operator+(const Jupiter::Readable_String<T> &rhs) const
{
	return Jupiter::operator+(*this, rhs);
}

template<typename T> inline Jupiter::String_Strict<T> Jupiter::String_Strict<T>::operator+(const std::basic_string<T> &rhs) const
{
	return Jupiter::operator+(*this, rhs);
}

template<typename T> inline Jupiter::String_Strict<T> Jupiter::String_Strict<T>::operator+(const T *rhs) const
{
	return Jupiter::operator+(*this, rhs);
}

template<typename T> static inline Jupiter::String_Strict<T> Jupiter::operator+(const Jupiter::String_Type<T> &lhs, const T &rhs)
{
	return Jupiter::String_Strict<T>(lhs, rhs);
}

template<typename T> static inline Jupiter::String_Strict<T> Jupiter::operator+(const Jupiter::String_Type<T> &lhs, const Jupiter::String_Type<T> &rhs)
{
	return Jupiter::String_Strict<T>(lhs, rhs);
}

template<typename T> static inline Jupiter::String_Strict<T> Jupiter::operator+(const Jupiter::String_Type<T> &lhs, const std::basic_string<T> &rhs)
{
	return Jupiter::String_Strict<T>(lhs, rhs);
}

template<typename T> static inline Jupiter::String_Strict<T> Jupiter::operator+(const Jupiter::String_Type<T> &lhs, const std::basic_string_view<T> &rhs)
{
	return Jupiter::String_Strict<T>(lhs, rhs);
}

template<typename T> static inline Jupiter::String_Strict<T> Jupiter::operator+(const Jupiter::String_Type<T> &lhs, const T *rhs)
{
	return Jupiter::String_Strict<T>(lhs, rhs);
}

// Jupiter::DataBuffer specialization

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::String_Strict>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::String_Strict<Y> *data) {
		_Jupiter_DataBuffer_partial_specialization_impl<Jupiter::String_Type>::push<Y>(buffer, data);
	};

	template<typename Y> static Jupiter::String_Strict<Y> interpret(uint8_t *&head)
	{
		size_t size_ = *reinterpret_cast<size_t *>(head);
		head += sizeof(size_t);
		Jupiter::String_Strict<Y> r = Jupiter::String_Strict<Y>(reinterpret_cast<Y *>(head), size_);
		head += size_;
		return r;
	}
};

#endif // _STRING_IMP_H_HEADER