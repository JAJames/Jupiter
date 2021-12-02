/**
 * Copyright (C) 2014-2016 Jessica James.
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

template<typename T> size_t Jupiter::String_Type<T>::size() const
{
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::capacity() const
{
	return this->size();
}

template<typename T> const T *Jupiter::String_Type<T>::data() const
{
	return Jupiter::String_Type<T>::str;
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

template<typename T> void Jupiter::String_Type<T>::remove(size_t index, size_t len)
{
	if (index >= Jupiter::String_Type<T>::length)
		return;

	if (index + len >= Jupiter::String_Type<T>::length)
		Jupiter::String_Type<T>::length = index;
	else
	{
		Jupiter::String_Type<T>::length -= len;
		len += index;

		Jupiter::String_Type<T>::str[index] = Jupiter::String_Type<T>::str[len];
		while (++index != Jupiter::String_Type<T>::length)
			Jupiter::String_Type<T>::str[index] = Jupiter::String_Type<T>::str[++len];
	}
}

// erase

template<typename T> void Jupiter::String_Type<T>::clear()
{
	Jupiter::String_Type<T>::length = 0;
}

// set

template<typename T> size_t Jupiter::String_Type<T>::set(size_t index, const T &in)
{
	if (index == Jupiter::String_Type<T>::length)
		return this->concat(in);

	if (index > Jupiter::String_Type<T>::length)
	{
		this->setBufferSize(index + 1);
		while (Jupiter::String_Type<T>::length != index)
			Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length++] = 0;
		Jupiter::String_Type<T>::length++;
	}
	Jupiter::String_Type<T>::str[index] = in;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::set(size_t index, const T *in, size_t inSize)
{
	if (index == Jupiter::String_Type<T>::length)
		return this->concat(in);

	if (index > Jupiter::String_Type<T>::length)
	{
		this->setBufferSize(index + inSize);
		while (Jupiter::String_Type<T>::length != index)
			Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length++] = 0;
		index = 0;
		while (index != inSize)
			Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length++] = in[index];
	}
	else
	{
		index += inSize;
		if (index > Jupiter::String_Type<T>::length)
		{
			this->setBufferSize(index);
			Jupiter::String_Type<T>::length = index;
		}
		while (inSize != 0)
			Jupiter::String_Type<T>::str[--index] = in[--inSize];
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::set(size_t index, const Jupiter::Readable_String<T> &in)
{
	return this->set(index, in.data(), in.size());
}

template<typename T> size_t Jupiter::String_Type<T>::set(size_t index, const std::basic_string<T> &in)
{
	return this->set(index, in.c_str(), in.size());
}

template<typename T> size_t Jupiter::String_Type<T>::set(size_t index, const T *in)
{
	return this->set(index, in, Jupiter::strlen<T>(in));
}

template<typename T> size_t Jupiter::String_Type<T>::set(const T *in, size_t inSize)
{
	this->setBufferSizeNoCopy(inSize);
	Jupiter::String_Type<T>::length = inSize;
	while (inSize-- != 0)
		*Jupiter::String_Type<T>::str++ = *in++;
	Jupiter::String_Type<T>::str -= Jupiter::String_Type<T>::length;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::set(const Jupiter::Readable_String<T> &in)
{
	return this->set(in.data(), in.size());
}

template<typename T> size_t Jupiter::String_Type<T>::set(const std::basic_string<T> &in)
{
	return this->set(in.c_str(), in.size());
}

template<typename T> size_t Jupiter::String_Type<T>::set(const T *in)
{
	return this->set(in, Jupiter::strlen<T>(in));
}

template<typename T> size_t Jupiter::String_Type<T>::set(const T &in)
{
	this->setBufferSizeNoCopy(1);
	*Jupiter::String_Type<T>::str = in;
	return Jupiter::String_Type<T>::length = 1;
}

// insert

template<typename T> size_t Jupiter::String_Type<T>::insert(size_t index, const T &value)
{
	if (index >= Jupiter::String_Type<T>::length)
		return this->concat(value);

	this->setBufferSize(Jupiter::String_Type<T>::length + 1);
	for (size_t i = Jupiter::String_Type<T>::length; i != index; i--)
		Jupiter::String_Type<T>::str[i] = operator[](i-1);

	Jupiter::String_Type<T>::str[index] = value;
	return ++Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::insert(size_t index, const Jupiter::Readable_String<T> &value)
{
	if (index >= Jupiter::String_Type<T>::length)
		return this->concat(value);

	if (value.empty())
		return Jupiter::String_Type<T>::length;

	if (value.size() == 1)
		return this->insert(index, value[0]);

	this->setBufferSize(Jupiter::String_Type<T>::length + value.size());
	size_t i;
	for (i = Jupiter::String_Type<T>::length + value.size() - 1; i != index + value.size() - 1; i--)
		Jupiter::String_Type<T>::str[i] = operator[](i - value.size());

	while (i != index)
	{
		Jupiter::String_Type<T>::str[i] = value[i - index];
		i--;
	}
	Jupiter::String_Type<T>::str[index] = value[0];

	return Jupiter::String_Type<T>::length += value.size();
}

// replace

template<typename T> size_t Jupiter::String_Type<T>::replace(size_t index, size_t targetSize, const T &value)
{
	if (targetSize != 0)
	{
		if (index >= Jupiter::String_Type<T>::length)
			return this->set(index, value);

		if (index + targetSize > Jupiter::String_Type<T>::length)
		{
			Jupiter::String_Type<T>::str[index] = value;
			return Jupiter::String_Type<T>::length = index + 1;
		}

		if (targetSize == 1)
			return this->set(index, value);

		Jupiter::String_Type<T>::str[index] = value;
		Jupiter::String_Type<T>::length -= targetSize - 1;
		while (++index != Jupiter::String_Type<T>::length)
			Jupiter::String_Type<T>::str[index] = Jupiter::String_Type<T>::str[index + targetSize - 1];
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::replace(size_t index, size_t targetSize, const T *value, size_t valueSize)
{
	if (valueSize == 1)
		return this->replace(index, targetSize, *value);

	if (valueSize == 0)
	{
		this->remove(index, targetSize);
		return Jupiter::String_Type<T>::length;
	}

	if (targetSize != 0)
	{
		if (index >= Jupiter::String_Type<T>::length)
			return this->set(index, value);

		if (index + targetSize > Jupiter::String_Type<T>::length)
		{
			if (index + valueSize > Jupiter::String_Type<T>::length)
				this->setBufferSize(index + valueSize);

			Jupiter::String_Type<T>::length = index + valueSize;
			Jupiter::String_Type<T>::str[index] = *value;
			while (++index != Jupiter::String_Type<T>::length)
				Jupiter::String_Type<T>::str[index] = *++value;
			return Jupiter::String_Type<T>::length;
		}

		if (targetSize == valueSize)
			return this->set(index, value, valueSize);

		if (valueSize < targetSize)
		{
			targetSize -= valueSize;
			Jupiter::String_Type<T>::length -= targetSize;

			Jupiter::String_Type<T>::str[index] = *value;
			while (--valueSize != 0)
				Jupiter::String_Type<T>::str[++index] = *++value;

			while (++index != Jupiter::String_Type<T>::length)
				Jupiter::String_Type<T>::str[index] = Jupiter::String_Type<T>::str[index + targetSize];
		}
		else
		{
			size_t i = Jupiter::String_Type<T>::length;
			valueSize -= targetSize;
			this->setBufferSize(Jupiter::String_Type<T>::length + valueSize);
			Jupiter::String_Type<T>::length += valueSize;

			index += targetSize;
			while (i-- != index)
				Jupiter::String_Type<T>::str[i + valueSize] = Jupiter::String_Type<T>::str[i];
			index -= targetSize;
			valueSize += targetSize;

			Jupiter::String_Type<T>::str[index] = *value;
			while (--valueSize != 0)
				Jupiter::String_Type<T>::str[++index] = *++value;
		}
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::replace(size_t index, size_t targetSize, std::basic_string_view<T> value) {
	return this->replace(index, targetSize, value.data(), value.size());
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const T &target, const T &value)
{
	for (size_t i = 0; i != Jupiter::String_Type<T>::length; i++)
	{
		if (operator[](i) == target)
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

		if (targetSize < Jupiter::String_Type<T>::length)
		{
			size_t i = 0, j = 0, k;
			while (j <= Jupiter::String_Type<T>::length - targetSize)
			{
				k = 0;
				while (operator[](j + k) == target[k])
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
			while (j < Jupiter::String_Type<T>::length)
				Jupiter::String_Type<T>::str[i++] = Jupiter::String_Type<T>::str[j++];

			Jupiter::String_Type<T>::length = i;
		}
		else if (targetSize == Jupiter::String_Type<T>::length && *this == std::basic_string_view<T>(target, targetSize))
			return this->set(value);
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const Jupiter::Readable_String<T> &target, const T &value)
{
	return this->replace(target.data(), target.size(), value);
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const T *target, size_t targetSize, const T *value, size_t valueSize)
{
	if (valueSize == 1)
		return this->replace(target, targetSize, *value);

	if (targetSize != 0)
	{
		//if (targetSize == 1)
		//	return this->replace(*target, value, valueSize);

		if (targetSize < Jupiter::String_Type<T>::length)
		{
			if (valueSize > targetSize)
			{
				size_t instancesSize = 4;
				size_t *instances = new size_t[instancesSize];
				// pass 1 (count instances)
				size_t instanceCount = 0, i = 0, j;
				while (i <= Jupiter::String_Type<T>::length - targetSize)
				{
					j = 0;
					while (operator[](i + j) == target[j])
					{
						if (++j == targetSize)
						{
							i += targetSize;
							if (instanceCount == instancesSize)
							{
								size_t *tInstances = new size_t[instancesSize * 2];
								for (instanceCount = 0; instanceCount != instancesSize; instanceCount++)
									tInstances[instanceCount] = instances[instanceCount];
								delete[] instances;
								instances = tInstances;
								instancesSize *= 2;
							}
							instances[instanceCount] = i;
							instanceCount++;
							break;
						}
					}
					if (j != targetSize)
						i++;
				}
				
				if (instanceCount != 0)
				{
					// pass 2 (adjust string and replace values)
					size_t endSize = Jupiter::String_Type<T>::length + (valueSize - targetSize) * instanceCount;
					this->setBufferSize(endSize);
					instancesSize = endSize; // Repurposing. Now used as just another index.
					j = Jupiter::String_Type<T>::length;
					while (instanceCount != 0)
					{
						i = instances[--instanceCount];
						while (j != i)
							Jupiter::String_Type<T>::str[--instancesSize] = Jupiter::String_Type<T>::str[--j];

						j = valueSize;
						while (j != 0)
							Jupiter::String_Type<T>::str[--instancesSize] = value[--j];
						j = i - targetSize;
					}
					Jupiter::String_Type<T>::length = endSize;
				}
				delete[] instances;
			}
			else
			{
				size_t i = 0, j = 0, k, l;
				while (j <= Jupiter::String_Type<T>::length - targetSize)
				{
					k = 0;
					while (operator[](j + k) == target[k])
					{
						if (++k == targetSize) // match found
						{
							for (l = 0; l != valueSize; l++)
								Jupiter::String_Type<T>::str[i + l] = value[l];
							i += valueSize;
							j += k;
							break;
						}
					}
					if (k != targetSize)
						Jupiter::String_Type<T>::str[i++] = Jupiter::String_Type<T>::str[j++];
				}
				while (j < Jupiter::String_Type<T>::length)
					Jupiter::String_Type<T>::str[i++] = Jupiter::String_Type<T>::str[j++];

				Jupiter::String_Type<T>::length = i;
			}
		}
		else if (targetSize == Jupiter::String_Type<T>::length && *this == std::basic_string_view<T>(target, targetSize))
			return this->set(value);
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const T *target, size_t targetSize, const Jupiter::Readable_String<T> &value)
{
	return this->replace(target, targetSize, value.data(), value.size());
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const Jupiter::Readable_String<T> &target, const T *value, size_t valueSize)
{
	return this->replace(target.data(), target.size(), value, valueSize);
}

template<typename T> size_t Jupiter::String_Type<T>::replace(std::basic_string_view<T> target, std::basic_string_view<T> value)
{
	return this->replace(target.data(), target.size(), value.data(), value.size());
}

// concat

template<typename T> size_t Jupiter::String_Type<T>::concat(const T *in, size_t inSize)
{
	this->setBufferSize(Jupiter::String_Type<T>::length + inSize);
	while (inSize-- != 0)
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length++] = *in++;
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::concat(const Jupiter::Readable_String<T> &in)
{
	return this->concat(in.data(), in.size());
}

template<typename T> size_t Jupiter::String_Type<T>::concat(const std::basic_string<T> &in)
{
	return this->concat(in.c_str(), in.size());
}

template<typename T> size_t Jupiter::String_Type<T>::concat(const T *in)
{
	return this->concat(in, Jupiter::strlen<T>(in));
}

template<typename T> size_t Jupiter::String_Type<T>::concat(const T &c)
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
	for (r.length = 0; pos + r.length != in.size(); r.length++) r.str[r.length] = in[pos + r.length];
	return r;
}

template<typename T> template<template<typename> class R> R<T> Jupiter::String_Type<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t len)
{
	if (pos + len >= in.size()) return R<T>::substring(in, pos);
	R<T> r = R<T>(len);
	for (r.length = 0; r.length != len; r.length++) r.str[r.length] = in[pos + r.length];
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

// Jupiter::DataBuffer specialization

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::String_Type> {
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::String_Type<Y> *data) {
		buffer->secure(sizeof(size_t) + data->size() * sizeof(Y));
		buffer->push<size_t>(data->size());
		buffer->push(reinterpret_cast<const uint8_t *>(data->data()), data->size() * sizeof(Y));
	};
};

#endif // _STRING_TYPE_IMP_H_HEADER