/**
 * Copyright (C) 2015 Jessica James.
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

#if !defined _DATABUFFER_IMP_H_HEADER
#define _DATABUFFER_IMP_H_HEADER

/**
 * @file DataBuffer_Imp.h
 * @brief Provides the implementations for DataBuffer template functions.
 * Note: Modification of this file is not supported in any way.
 */

#include <vector>
#include <array>

/**
* IMPLEMENTATION:
*	DataBuffer
*/

template<typename T> T Jupiter::DataBuffer::interpret_data(uint8_t *&head)
{
	T r = T(*(reinterpret_cast<T *>(head)));
	head += sizeof(T);
	return r;
}

template<template<typename> class T, typename Y> static T<Y> interpret_data(uint8_t *&head)
{
	return _Jupiter_DataBuffer_partial_specialization_impl_std<T>::interpret<Y>(head);
}

template<template<typename, typename> class T, typename X, typename Y> static T<X, Y> interpret_data(uint8_t *&head)
{
	return _Jupiter_DataBuffer_partial_specialization_impl_std_vector<T>::interpret<X, Y>(head);
}

template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> static T<X, Y, Z> interpret_data(uint8_t *&head)
{
	return _Jupiter_DataBuffer_partial_specialization_impl_std_string<T>::interpret<X, Y, Z>(head);
}

// Basic peek

template<typename T> T Jupiter::DataBuffer::peek() const
{
	uint8_t *ptr = Jupiter::DataBuffer::head;
	return Jupiter::DataBuffer::interpret_data<T>(ptr);
}

template<template<typename> class T, typename Y> T<Y> Jupiter::DataBuffer::peek() const
{
	uint8_t *ptr = Jupiter::DataBuffer::head;
	return Jupiter::DataBuffer::interpret_data<T, Y>(ptr);
}

template<template<typename, typename> class T, typename X, typename Y> T<X, Y> Jupiter::DataBuffer::peek() const
{
	uint8_t *ptr = Jupiter::DataBuffer::head;
	return Jupiter::DataBuffer::interpret_data<T, X, Y>(ptr);
}

template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> T<X, Y, Z> Jupiter::DataBuffer::peek() const
{
	uint8_t *ptr = Jupiter::DataBuffer::head;
	return Jupiter::DataBuffer::interpret_data<T, X, Y, Z>(ptr);
}

template<typename T> void Jupiter::DataBuffer::peek(T &out) const
{
	out = Jupiter::DataBuffer::peek<T>();
}

template<template<typename> class T, typename Y> void Jupiter::DataBuffer::peek(T<Y> &out) const
{
	out = Jupiter::DataBuffer::peek<T, Y>();
}

template<template<typename, typename> class T, typename X, typename Y> void Jupiter::DataBuffer::peek(T<X, Y> &out) const
{
	out = Jupiter::DataBuffer::peek<T, X, Y>();
}

template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> void Jupiter::DataBuffer::peek(T<X, Y, Z> &out) const
{
	out = Jupiter::DataBuffer::peek<T, X, Y, Z>();
}

template<typename T> T *Jupiter::DataBuffer::peek(size_t size_) const
{
	uint8_t *ptr = Jupiter::DataBuffer::head;
	T *r = new T[size_];
	T *itr = r;
	while (size_-- != 0)
		*itr++ = Jupiter::DataBuffer::interpret_data<T>(ptr);
	return r;
}

// Basic pop

template<typename T> T Jupiter::DataBuffer::pop()
{
	return Jupiter::DataBuffer::interpret_data<T>(Jupiter::DataBuffer::head);
}

template<template<typename> class T, typename Y> T<Y> Jupiter::DataBuffer::pop()
{
	return Jupiter::DataBuffer::interpret_data<T, Y>(Jupiter::DataBuffer::head);
}

template<template<typename, typename> class T, typename X, typename Y> T<X, Y> Jupiter::DataBuffer::pop()
{
	return Jupiter::DataBuffer::interpret_data<T, X, Y>(Jupiter::DataBuffer::head);
}

template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> T<X, Y, Z> Jupiter::DataBuffer::pop()
{
	return Jupiter::DataBuffer::interpret_data<T, X, Y, Z>(Jupiter::DataBuffer::head);
}

template<typename T> void Jupiter::DataBuffer::pop(T &out)
{
	out = Jupiter::DataBuffer::pop<T>();
}

template<template<typename> class T, typename Y> void Jupiter::DataBuffer::pop(T<Y> &out)
{
	out = Jupiter::DataBuffer::pop<T, Y>();
}

template<template<typename, typename> class T, typename X, typename Y> void Jupiter::DataBuffer::pop(T<X, Y> &out)
{
	out = Jupiter::DataBuffer::pop<T, X, Y>();
}

template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> void Jupiter::DataBuffer::pop(T<X, Y, Z> &out)
{
	out = Jupiter::DataBuffer::pop<T, X, Y, Z>();
}

template<typename T> T *Jupiter::DataBuffer::pop(size_t size_)
{
	T *r = new T[size_];
	T *itr = r;
	while (size_-- != 0)
		*itr++ = Jupiter::DataBuffer::interpret_data<T>(Jupiter::DataBuffer::head);
	return r;
}

// Basic push

template<typename T> void Jupiter::DataBuffer::push(const T *data)
{
	Jupiter::DataBuffer::secure(sizeof(T));
	*reinterpret_cast<T *>(Jupiter::DataBuffer::end) = *data;
	Jupiter::DataBuffer::end += sizeof(T);
}

template<typename T> void Jupiter::DataBuffer::push(const T &data)
{
	Jupiter::DataBuffer::push<T>(&data);
}

// SPECIALIZATION: Jupiter::DataBuffer

template<> void inline Jupiter::DataBuffer::push(const Jupiter::DataBuffer *data)
{
	size_t size_ = data->size();
	Jupiter::DataBuffer::secure(sizeof(size_t) + size_);
	Jupiter::DataBuffer::push<size_t>(size_);
	Jupiter::DataBuffer::push(data->getHead(), size_);
}

template<> Jupiter::DataBuffer inline Jupiter::DataBuffer::interpret_data(uint8_t *&head_)
{
	size_t size_ = *reinterpret_cast<size_t *>(head_);
	head_ += sizeof(size_t);
	Jupiter::DataBuffer r = Jupiter::DataBuffer(size_);
	while (size_-- != 0)
		*r.end++ = *head_++;
	return r;
}

// template / string push

template<template<typename> class T> struct _Jupiter_DataBuffer_partial_specialization_impl
{
};

template<template<typename> class T, typename Y> void Jupiter::DataBuffer::push(const T<Y> *data)
{
	_Jupiter_DataBuffer_partial_specialization_impl<T>::push<Y>(this, data);
}

template<template<typename> class T, typename Y> void Jupiter::DataBuffer::push(const T<Y> &data)
{
	Jupiter::DataBuffer::push<T, Y>(&data);
}

template<template<typename> class T, typename Y> T<Y> Jupiter::DataBuffer::interpret_data(uint8_t *&head)
{
	return _Jupiter_DataBuffer_partial_specialization_impl<T>::interpret<Y>(head);
}

// PUSH SPECIALIZATION: std::array

template<template<typename, size_t> class T> struct _Jupiter_DataBuffer_partial_specialization_impl_std_array
{
};

template<> struct _Jupiter_DataBuffer_partial_specialization_impl_std_array<std::array>
{
	template<typename X, size_t Y> static void push(Jupiter::DataBuffer *buffer, const std::array<X, Y> *data)
	{
		buffer->push<size_t>(Y);
		if (std::is_fundamental<std::array<X, Y>::value_type>::value)
			buffer->push(reinterpret_cast<const uint8_t *>(data->data()), data->size() * sizeof(std::array<X, Y>::value_type));
		else
		{
			std::array<X, Y>::const_iterator itr = data->begin();
			std::array<X, Y>::const_iterator end = data->end();
			while (itr != end)
				buffer->push<std::array<X, Y>::value_type>(*itr++);
		}
	};
};

template<template<typename, size_t> class T, typename X, size_t Y> void Jupiter::DataBuffer::push(const T<X, Y> *data)
{
	_Jupiter_DataBuffer_partial_specialization_impl_std_array<T>::push<X, Y>(this, data);
}

template<template<typename, size_t> class T, typename X, size_t Y> void Jupiter::DataBuffer::push(const T<X, Y> &data)
{
	Jupiter::DataBuffer::push<T, X, Y>(&data);
}

// SPECIALIZATION: std::vector

template<template<typename, typename> class T> struct _Jupiter_DataBuffer_partial_specialization_impl_std_vector
{
};

template<> struct _Jupiter_DataBuffer_partial_specialization_impl_std_vector<std::vector>
{
	template<typename X, typename Y> static void push(Jupiter::DataBuffer *buffer, const std::vector<X, Y> *data)
	{
		buffer->push<std::vector<X, Y>::size_type>(data->size());
		if (std::is_fundamental<std::vector<X, Y>::value_type>::value)
			buffer->push(reinterpret_cast<const uint8_t *>(data->data()), data->size() * sizeof(std::vector<X, Y>::value_type));
		else
		{
			std::vector<X, Y>::const_iterator itr = data->begin();
			std::vector<X, Y>::const_iterator end = data->end();
			while (itr != end)
				buffer->push<std::vector<X, Y>::value_type>(*itr++);
		}
	};

	template<typename X, typename Y> static std::vector<X, Y> interpret(uint8_t *&head)
	{
		size_t size_ = *reinterpret_cast<size_t *>(head);
		head += sizeof(size_t);
		std::vector<X, Y> r;
		r.reserve(size_);
		while (size_-- != 0)
			r.push_back(Jupiter::DataBuffer::interpret_data<std::vector<X, Y>::value_type>(head));
		return r;
	}
};

template<template<typename, typename> class T, typename X, typename Y> void Jupiter::DataBuffer::push(const T<X, Y> *data)
{
	_Jupiter_DataBuffer_partial_specialization_impl_std_vector<T>::push<X, Y>(this, data);
}

template<template<typename, typename> class T, typename X, typename Y> void Jupiter::DataBuffer::push(const T<X, Y> &data)
{
	Jupiter::DataBuffer::push<T, X, Y>(&data);
}

template<template<typename, typename> class T, typename X, typename Y> T<X, Y> Jupiter::DataBuffer::interpret_data(uint8_t *&head)
{
	return _Jupiter_DataBuffer_partial_specialization_impl_std_vector<T>::interpret<X, Y>(head);
}

// SPECIALIZATION: std::string

template<template<typename, typename, typename> class T> struct _Jupiter_DataBuffer_partial_specialization_impl_std_string
{
};

template<> struct _Jupiter_DataBuffer_partial_specialization_impl_std_string<std::basic_string>
{
	template<typename X, typename Y, typename Z> static void push(Jupiter::DataBuffer *buffer, const std::basic_string<X, Y, Z> *data)
	{
		buffer->push(data->size());
		if (std::is_fundamental<std::basic_string<X, Y, Z>::value_type>::value)
			buffer->push(reinterpret_cast<const uint8_t *>(data->data()), data->size() * sizeof(X));
		else
		{
			std::basic_string<X, Y, Z>::const_iterator itr = data->begin();
			std::basic_string<X, Y, Z>::const_iterator end = data->end();
			while (itr != end)
				buffer->push<std::basic_string<X, Y, Z>::value_type>(*itr++);
		}
	};

	template<typename X, typename Y, typename Z> static std::basic_string<X, Y, Z> interpret(uint8_t *&head)
	{
		size_t size_ = *reinterpret_cast<size_t *>(head);
		head += sizeof(size_t);
		std::basic_string<X, Y, Z> r;
		r.reserve(size_);
		while (size_-- != 0)
			r.push_back(Jupiter::DataBuffer::interpret_data<std::basic_string<X, Y, Z>::value_type>(head));
		return r;
	}
};

template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> void Jupiter::DataBuffer::push(const T<X, Y, Z> *data)
{
	_Jupiter_DataBuffer_partial_specialization_impl_std_string<T>::push<X, Y, Z>(this, data);
}

template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> void Jupiter::DataBuffer::push(const T<X, Y, Z> &data)
{
	Jupiter::DataBuffer::push<T, X, Y, Z>(&data);
}

template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> T<X, Y, Z> Jupiter::DataBuffer::interpret_data(uint8_t *&head)
{
	return _Jupiter_DataBuffer_partial_specialization_impl_std_string<T>::interpret<X, Y, Z>(head);
}

#endif // _DATABUFFER_IMP_H_HEADER