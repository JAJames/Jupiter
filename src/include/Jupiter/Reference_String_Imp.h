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

template<typename T> Jupiter::Reference_String<T>::Reference_String() : Reference_String(nullptr, 0)
{
}

template<typename T> Jupiter::Reference_String<T>::Reference_String(const T *in)
{
	Jupiter::Reference_String<T>::str = in;
	if (in == nullptr) Jupiter::Reference_String<T>::length = 0;
	else Jupiter::Reference_String<T>::length = Jupiter::strlen<T>(in);
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
	source.str = nullptr;
}

template<typename T> Jupiter::Reference_String<T>::Reference_String(const Jupiter::Readable_String<T> &in) : Reference_String(in.ptr(), in.size())
{
}

template<typename T> Jupiter::Reference_String<T>::Reference_String(const Jupiter::Reference_String<T> &in) : Reference_String(in.str, in.length)
{
}

template<typename T> Jupiter::Reference_String<T>::Reference_String(const Jupiter::DataBuffer &in) : Reference_String(reinterpret_cast<T *>(in.getHead()), in.size() / sizeof(T))
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

// truncate

template<typename T> size_t Jupiter::Reference_String<T>::truncate(size_t n)
{
	if (n >= Jupiter::Reference_String<T>::length) return (Jupiter::Reference_String<T>::length = 0);
	return (Jupiter::Reference_String<T>::length -= n);
}

// erase

template<typename T> void Jupiter::Reference_String<T>::erase()
{
	Jupiter::Reference_String<T>::length = 0;
}

// shift

template<typename T> size_t Jupiter::Reference_String<T>::shiftLeft(size_t len)
{
	Jupiter::Reference_String<T>::str -= len;
	Jupiter::Reference_String<T>::length += len;
	return len;
}

template<typename T> size_t Jupiter::Reference_String<T>::shiftRight(size_t len)
{
	if (len > Jupiter::Reference_String<T>::length) len = Jupiter::Reference_String<T>::length;
	Jupiter::Reference_String<T>::str += len;
	Jupiter::Reference_String<T>::length -= len;
	return len;
}

// set

template<typename T> size_t Jupiter::Reference_String<T>::set(const Jupiter::Readable_String<T> &in)
{
	return this->set(in.ptr(), in.size());
}

template<typename T> size_t Jupiter::Reference_String<T>::set(const std::basic_string<T> &in)
{
	return this->set(in.data(), in.size());
}

template<typename T> size_t Jupiter::Reference_String<T>::set(const T *in, size_t len)
{
	Jupiter::Reference_String<T>::str = in;
	return Jupiter::Reference_String<T>::length = len;
}

template<typename T> size_t Jupiter::Reference_String<T>::set(const T *in)
{
	if (in == nullptr) return this->set(in, 0);
	return this->set(in, Jupiter::strlen<T>(in));
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
	return Jupiter::Readable_String<T>::template getWord<Jupiter::Reference_String>(in, pos, whitespace);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::getWord(const T *in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::template getWord<Jupiter::Reference_String>(in, pos, whitespace);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::getToken(size_t pos, const T &token) const
{
	return Jupiter::Reference_String<T>::getToken(*this, pos, token);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::getToken(size_t pos, const Jupiter::Readable_String<T> &token) const
{
	return Jupiter::Reference_String<T>::getToken(*this, pos, token);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::getToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token)
{
	return Jupiter::Readable_String<T>::template getToken<Jupiter::Reference_String>(in, pos, token);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::getToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token)
{
	return Jupiter::Readable_String<T>::template getToken<Jupiter::Reference_String>(in, pos, token);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::gotoWord(size_t pos, const T *whitespace) const
{
	return Jupiter::Reference_String<T>::gotoWord(*this, pos, whitespace);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::template gotoWord<Jupiter::Reference_String>(in, pos, whitespace);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::gotoWord(const T *in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::template gotoWord<Jupiter::Reference_String>(in, pos, whitespace);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::gotoToken(size_t pos, const T &token) const
{
	return Jupiter::Reference_String<T>::gotoToken(*this, pos, token);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::gotoToken(size_t pos, const Jupiter::Readable_String<T> &token) const
{
	return Jupiter::Reference_String<T>::gotoToken(*this, pos, token);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token)
{
	return Jupiter::Readable_String<T>::template gotoToken<Jupiter::Reference_String>(in, pos, token);
}

template<typename T> Jupiter::Reference_String<T> Jupiter::Reference_String<T>::gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token)
{
	return Jupiter::Readable_String<T>::template gotoToken<Jupiter::Reference_String>(in, pos, token);
}

template<typename T> const Jupiter::Reference_String<T> Jupiter::Reference_String<T>::empty = Jupiter::Reference_String<T>();

// tokenize

template<typename T> typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> Jupiter::Reference_String<T>::tokenize(const T &separator) const
{
	return Jupiter::Reference_String<T>::tokenize(*this, separator);
}

template<typename T> typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> Jupiter::Reference_String<T>::tokenize(const Jupiter::Readable_String<T> &separator) const
{
	return Jupiter::Reference_String<T>::tokenize(*this, separator);
}

template<typename T> typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> Jupiter::Reference_String<T>::tokenize(const T *separator, size_t separator_size) const
{
	return Jupiter::Reference_String<T>::tokenize(*this, separator, separator_size);
}

template<typename T> typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> Jupiter::Reference_String<T>::tokenize(const Jupiter::Readable_String<T> &in, const T &token)
{
	return Jupiter::Readable_String<T>::template tokenize<Jupiter::Reference_String>(in, token);
}

template<typename T> typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> Jupiter::Reference_String<T>::tokenize(const Jupiter::Readable_String<T> &in, const Jupiter::Readable_String<T> &separator)
{
	return Jupiter::Readable_String<T>::template tokenize<Jupiter::Reference_String>(in, separator);
}

template<typename T> typename Jupiter::Readable_String<T>::template TokenizeResult<Jupiter::Reference_String> Jupiter::Reference_String<T>::tokenize(const Jupiter::Readable_String<T> &in, const T *separator, size_t separator_size)
{
	return Jupiter::Readable_String<T>::template tokenize<Jupiter::Reference_String>(in, separator, separator_size);
}

// Jupiter::DataBuffer specialization

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::Reference_String>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::Reference_String<Y> *data)
	{
		_Jupiter_DataBuffer_partial_specialization_impl<Jupiter::Readable_String>::push<Y>(buffer, data);
	};

	template<typename Y> static Jupiter::Reference_String<Y> interpret(uint8_t *&head)
	{
		size_t size_ = *reinterpret_cast<size_t *>(head);
		head += sizeof(size_t);
		Jupiter::Reference_String<Y> r = Jupiter::Reference_String<Y>(reinterpret_cast<Y *>(head), size_);
		head += size_;
		return r;
	}
};

#endif // _REFERENCE_STRING_IMP