/**
 * Copyright (C) 2014-2017 Jessica James.
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

#if !defined _READABLE_STRING_IMP_H_HEADER
#define _READABLE_STRING_IMP_H_HEADER

/**
 * @file Readable_String_Imp.h
 * @brief Provides the implementations for Readable_String functions.
 * Note: Modification of this file is not supported in any way.
 */

#include "Readable_String.h"

/**
* IMPLEMENTATION:
*	Readable_String
*/

// find

template<typename T> size_t Jupiter::Readable_String<T>::find(const T &value, size_t index) const
{
	for (size_t i = 0; i != this->size(); i++)
	{
		if (operator[](i) == value)
		{
			if (index == 0) return i;
			else index--;
		}
	}
	return Jupiter::INVALID_INDEX;
}

template<typename T> size_t Jupiter::Readable_String<T>::find(const Jupiter::Readable_String<T> &in) const
{
	if (in.size() > this->size())
		return Jupiter::INVALID_INDEX;
	if (in.size() == this->size())
		return (*this == in) ? 0 : Jupiter::INVALID_INDEX;
	if (in.empty())
		return 0;

	for (size_t i = 0, j; i != this->size() - in.size() + 1; i++)
	{
		j = 0;
		while (operator[](i + j) == in[j])
			if (++j == in.size()) return i;
	}
	return Jupiter::INVALID_INDEX;
}

// equalsi()

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const Jupiter::Readable_String<T> &in) const
{
	return this->equalsi(in.data(), in.size());
}

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const std::basic_string<T> &in) const
{
	return this->equalsi(in.data(), in.size());
}

template<> bool inline Jupiter::Readable_String<char>::equalsi(const char *in, size_t len) const
{
	if (this->size() != len)
		return false;

	in += len;
	while (len != 0)
		if (toupper(operator[](--len)) != toupper(*(--in)))
			return false;

	return true;
}

template<> bool inline Jupiter::Readable_String<wchar_t>::equalsi(const wchar_t *in, size_t len) const
{
	if (this->size() != len)
		return false;

	while (len != 0)
		if (towupper(operator[](--len)) != towupper(*(--in)))
			return false;

	return true;
}

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const T *in, size_t len) const
{
	return *this == std::basic_string_view<T>(in); // Concept of "case" not supported for type.
}

template<> bool inline Jupiter::Readable_String<char>::equalsi(const char &in) const
{
	return this->size() == 1 && toupper(operator[](0)) == toupper(in);
}

template<> bool inline Jupiter::Readable_String<wchar_t>::equalsi(const wchar_t &in) const
{
	return this->size() == 1 && towupper(operator[](0)) == towupper(in);
}

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const T &in) const
{
	return *this == in; // Concept of "case" not supported for type.
}

// Jupiter::DataBuffer specialization

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::Readable_String>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::Readable_String<Y> *data)
	{
		buffer->secure(sizeof(size_t) + data->size() * sizeof(Y));
		buffer->push<size_t>(data->size());
		buffer->push(reinterpret_cast<const uint8_t *>(data->data()), data->size() * sizeof(Y));
	};
};

#endif // _READABLE_STRING_IMP_H_HEADER