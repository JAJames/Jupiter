/**
 * Copyright (C) 2014-2015 Justin James.
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

template<typename T> void Jupiter::String_Type<T>::remove(size_t index, size_t length)
{
	if (index >= Jupiter::String_Type<T>::length)
		return;

	if (index + length >= Jupiter::String_Type<T>::length)
		Jupiter::String_Type<T>::length = index;
	else
	{
		Jupiter::String_Type<T>::length -= length;
		length += index;

		Jupiter::String_Type<T>::str[index] = Jupiter::String_Type<T>::str[length];
		while (++index != Jupiter::String_Type<T>::length)
			Jupiter::String_Type<T>::str[index] = Jupiter::String_Type<T>::str[++length];
	}
}

// processEscapeSequences

template<typename T> void Jupiter::String_Type<T>::processEscapeSequences()
{
}

template<> inline void Jupiter::String_Type<char>::processEscapeSequences()
{
	if (Jupiter::String_Type<char>::length == 0)
		return;

	size_t index = 0;
	while (index + 1 != Jupiter::String_Type<char>::length)
	{
		printf("Processing: %c (%u) @ %u" ENDL, this->get(index), this->get(index), index);
		if (this->get(index) == '\\')
		{
			switch (this->get(++index))
			{
			case '0':
			case '1':
			case '2':
			case '3':
				if (index + 1 != Jupiter::String_Type<char>::length && Jupiter_isBase(this->get(index + 1), 8))
				{
					if (index + 2 != Jupiter::String_Type<char>::length && Jupiter_isBase(this->get(index + 2), 8))
						this->replace(index - 1, 4, static_cast<uint8_t>(Jupiter_getBase(this->get(index), 8)) << 6 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 1), 8)) << 3 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 2), 8)));
					else
						this->replace(index - 1, 3, static_cast<uint8_t>(Jupiter_getBase(this->get(index), 8)) << 3 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 1), 8)));
				}
				else
					this->replace(index - 1, 2, static_cast<uint8_t>(Jupiter_getBase(this->get(index), 8)));
				break;
			case '4':
			case '5':
			case '6':
			case '7':
				if (index + 1 != Jupiter::String_Type<char>::length && Jupiter_isBase(this->get(index + 1), 8))
					this->replace(index - 1, 3, static_cast<uint8_t>(Jupiter_getBase(this->get(index), 8)) << 3 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 1), 8)));
				else
					this->replace(index - 1, 2, static_cast<uint8_t>(Jupiter_getBase(this->get(index), 8)));
				break;
			case 'a':
				this->replace(index - 1, 2, '\a');
				break;
			case 'b':
				this->replace(index - 1, 2, '\b');
				break;
			case 'f':
				this->replace(index - 1, 2, '\f');
				break;
			case 'n':
				this->replace(index - 1, 2, '\n');
				break;
			case 'r':
				this->replace(index - 1, 2, '\r');
				break;
			case 't':
				this->replace(index - 1, 2, '\t');
				break;
			case 'v':
				this->replace(index - 1, 2, '\v');
				break;
			case '?':
				this->replace(index - 1, 2, '\?');
				break;
			case '\'':
				this->replace(index - 1, 2, '\'');
				break;
			case '\"':
				this->replace(index - 1, 2, '\"');
				break;
			case '\\':
				this->replace(index - 1, 2, '\\');
				break;
			case 'x':
				if (Jupiter::String_Type<char>::length >= index + 2
					&& Jupiter_isBase(this->get(index + 1), 16) && Jupiter_isBase(this->get(index + 2), 16))
					this->replace(index - 1, 4, static_cast<uint8_t>(Jupiter_getBase(this->get(index + 1), 16)) << 4 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 2), 16)));
				break;
			case 'U':
				if (Jupiter::String_Type<char>::length >= index + 8
					&& Jupiter_isBase(this->get(index + 1), 16) && Jupiter_isBase(this->get(index + 2), 16) && Jupiter_isBase(this->get(index + 3), 16) && Jupiter_isBase(this->get(index + 4), 16) && Jupiter_isBase(this->get(index + 5), 16) && Jupiter_isBase(this->get(index + 6), 16) && Jupiter_isBase(this->get(index + 7), 16) && Jupiter_isBase(this->get(index + 8), 16))
				{
					uint32_t codepoint = static_cast<uint8_t>(Jupiter_getBase(this->get(index + 1), 16)) << 4 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 2), 16));
					codepoint <<= 8;
					codepoint |= static_cast<uint8_t>(Jupiter_getBase(this->get(index + 3), 16)) << 4 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 4), 16));
					codepoint <<= 8;
					codepoint |= static_cast<uint8_t>(Jupiter_getBase(this->get(index + 5), 16)) << 4 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 6), 16));
					codepoint <<= 8;
					codepoint |= static_cast<uint8_t>(Jupiter_getBase(this->get(index + 7), 16)) << 4 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 8), 16));
					if (codepoint <= 0x007F)
						this->replace(index - 1, 10, static_cast<uint8_t>(codepoint));
					else if (codepoint <= 0x07FF)
					{
						char bytes[2];
						bytes[0] = 0xC0 | ((codepoint >> 6) & 0x1F);
						bytes[1] = 0x80 | (codepoint & 0x3F);
						this->replace(index - 1, 10, bytes, sizeof(bytes));
						++index;
					}
					else if (codepoint <= 0xFFFF)
					{
						char bytes[3];
						bytes[0] = 0xE0 | ((codepoint >> 12) & 0x0F);
						bytes[1] = 0x80 | ((codepoint >> 6) & 0x3F);
						bytes[2] = 0x80 | (codepoint & 0x3F);
						this->replace(index - 1, 10, bytes, sizeof(bytes));
						index += 2;
					}
					else if (codepoint <= 0x10FFFF)
					{
						char bytes[4];
						bytes[0] = 0xE0 | ((codepoint >> 18) & 0x0F);
						bytes[1] = 0x80 | ((codepoint >> 12) & 0x3F);
						bytes[2] = 0x80 | ((codepoint >> 6) & 0x3F);
						bytes[3] = 0x80 | (codepoint & 0x3F);
						this->replace(index - 1, 10, bytes, sizeof(bytes));
						index += 3;
					}
				}
				break;
			case 'u':
				if (Jupiter::String_Type<char>::length >= index + 4
					&& Jupiter_isBase(this->get(index + 1), 16) && Jupiter_isBase(this->get(index + 2), 16) && Jupiter_isBase(this->get(index + 3), 16) && Jupiter_isBase(this->get(index + 4), 16))
				{
					uint16_t codepoint = static_cast<uint8_t>(Jupiter_getBase(this->get(index + 1), 16)) << 4 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 2), 16));
					codepoint <<= 8;
					codepoint |= static_cast<uint8_t>(Jupiter_getBase(this->get(index + 3), 16)) << 4 | static_cast<uint8_t>(Jupiter_getBase(this->get(index + 4), 16));
					if (codepoint <= 0x007F)
						this->replace(index - 1, 6, static_cast<uint8_t>(codepoint));
					else if (codepoint <= 0x07FF)
					{
						char bytes[2];
						bytes[0] = 0xC0 | ((codepoint >> 6) & 0x1F);
						bytes[1] = 0x80 | (codepoint & 0x3F);
						this->replace(index - 1, 6, bytes, sizeof(bytes));
						++index;
					}
					else if (codepoint <= 0xFFFF)
					{
						char bytes[3];
						bytes[0] = 0xE0 | ((codepoint >> 12) & 0x0F);
						bytes[1] = 0x80 | ((codepoint >> 6) & 0x3F);
						bytes[2] = 0x80 | (codepoint & 0x3F);
						this->replace(index - 1, 6, bytes, sizeof(bytes));
						index += 2;
					}
				}
				break;
			default:
				break;
			}
			if (index == Jupiter::String_Type<char>::length)
				break;
		}
		else
			++index;
	}
}

template<> inline void Jupiter::String_Type<wchar_t>::processEscapeSequences()
{
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
	return this->set(index, in.ptr(), in.size());
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
	return this->set(in.ptr(), in.size());
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
		Jupiter::String_Type<T>::str[i] = this->get(i-1);

	Jupiter::String_Type<T>::str[index] = value;
	return ++Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::insert(size_t index, const Jupiter::Readable_String<T> &value)
{
	if (index >= Jupiter::String_Type<T>::length)
		return this->concat(value);

	if (value.size() == 0)
		return Jupiter::String_Type<T>::length;

	if (value.size() == 1)
		return this->insert(index, value.get(0));

	this->setBufferSize(Jupiter::String_Type<T>::length + value.size());
	size_t i;
	for (i = Jupiter::String_Type<T>::length + value.size() - 1; i != index + value.size() - 1; i--)
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
			this->setBufferSize(Jupiter::String_Type<T>::length + valueSize - targetSize);
			Jupiter::String_Type<T>::length += valueSize - targetSize;

			size_t i = Jupiter::String_Type<T>::length;
			while (--i != index + targetSize + 1)
				Jupiter::String_Type<T>::str[i] = Jupiter::String_Type<T>::str[i - valueSize + targetSize];
			this->println(stdout);

			Jupiter::String_Type<T>::str[index] = *value;
			while (index != i)
				Jupiter::String_Type<T>::str[++index] = *++value;
		}
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::replace(size_t index, size_t targetSize, const Jupiter::Readable_String<T> &value)
{
	return this->replace(index, targetSize, value.ptr(), value.size());
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const T &target, const T &value)
{
	for (size_t i = 0; i != Jupiter::String_Type<T>::length; i++)
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

		if (targetSize < Jupiter::String_Type<T>::length)
		{
			size_t i = 0, j = 0, k;
			while (j <= Jupiter::String_Type<T>::length - targetSize)
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
			while (j < Jupiter::String_Type<T>::length)
				Jupiter::String_Type<T>::str[i++] = Jupiter::String_Type<T>::str[j++];

			Jupiter::String_Type<T>::length = i;
		}
		else if (targetSize == Jupiter::String_Type<T>::length && this->equals(target, targetSize))
			return this->set(value);
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const Jupiter::Readable_String<T> &target, const T &value)
{
	return this->replace(target.ptr(), target.size(), value);
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
					while (this->get(i + j) == target[j])
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
					while (this->get(j + k) == target[k])
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
		else if (targetSize == Jupiter::String_Type<T>::length && this->equals(target, targetSize))
			return this->set(value);
	}
	return Jupiter::String_Type<T>::length;
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const T *target, size_t targetSize, const Jupiter::Readable_String<T> &value)
{
	return this->replace(target, targetSize, value.ptr(), value.size());
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const Jupiter::Readable_String<T> &target, const T *value, size_t valueSize)
{
	return this->replace(target.ptr(), target.size(), value, valueSize);
}

template<typename T> size_t Jupiter::String_Type<T>::replace(const Jupiter::Readable_String<T> &target, const Jupiter::Readable_String<T> &value)
{
	return this->replace(target.ptr(), target.size(), value.ptr(), value.size());
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
	return this->concat(in.ptr(), in.size());
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

// Jupiter::DataBuffer specialization

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::String_Type>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::String_Type<Y> *data)
	{
		_Jupiter_DataBuffer_partial_specialization_impl<Jupiter::Readable_String>::push<Y>(buffer, data);
	};
};

#endif // _STRING_TYPE_IMP_H_HEADER