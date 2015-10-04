/**
 * Copyright (C) 2015 Justin James.
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
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include <cstdlib>
#include "DataBuffer.h"
#include "Reference_String.h"

Jupiter::DataBuffer::DataBuffer()
{
	Jupiter::DataBuffer::base = nullptr;
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
	Jupiter::DataBuffer::end = Jupiter::DataBuffer::head;
	Jupiter::DataBuffer::bufferSize = 0U;
}

Jupiter::DataBuffer::DataBuffer(size_t size_)
{
	Jupiter::DataBuffer::base = reinterpret_cast<uint8_t *>(malloc(size_));
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
	Jupiter::DataBuffer::end = Jupiter::DataBuffer::head;
	Jupiter::DataBuffer::bufferSize = size_;
}

Jupiter::DataBuffer::DataBuffer(FILE *file)
{
	fread(std::addressof(Jupiter::DataBuffer::bufferSize), sizeof(size_t), 1, file);
	Jupiter::DataBuffer::base = reinterpret_cast<uint8_t *>(malloc(Jupiter::DataBuffer::bufferSize * sizeof(uint8_t)));
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
	Jupiter::DataBuffer::end = Jupiter::DataBuffer::head + Jupiter::DataBuffer::bufferSize;
	int chr;
	while (Jupiter::DataBuffer::head != Jupiter::DataBuffer::end)
	{
		chr = fgetc(file);
		if (chr == EOF)
		{
			Jupiter::DataBuffer::end = Jupiter::DataBuffer::head + 1;
			break;
		}
		*Jupiter::DataBuffer::head++ = static_cast<uint8_t>(chr);
	}
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
}

Jupiter::DataBuffer::DataBuffer(FILE *file, size_t size_)
{
	Jupiter::DataBuffer::bufferSize = size_;
	Jupiter::DataBuffer::base = reinterpret_cast<uint8_t *>(malloc(Jupiter::DataBuffer::bufferSize * sizeof(uint8_t)));
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
	Jupiter::DataBuffer::end = Jupiter::DataBuffer::head + Jupiter::DataBuffer::bufferSize;
	int chr;
	while (Jupiter::DataBuffer::head != Jupiter::DataBuffer::end)
	{
		chr = fgetc(file);
		if (chr == EOF)
		{
			Jupiter::DataBuffer::end = Jupiter::DataBuffer::head + 1;
			break;
		}
		*Jupiter::DataBuffer::head++ = static_cast<uint8_t>(chr);
	}
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
}

Jupiter::DataBuffer::~DataBuffer()
{
	free(Jupiter::DataBuffer::base);
}

void Jupiter::DataBuffer::push(const uint8_t *data, size_t size_)
{
	Jupiter::DataBuffer::secure(size_);
	while (size_-- != 0)
		*Jupiter::DataBuffer::end++ = *data++;
}

void Jupiter::DataBuffer::peek_from(FILE *file)
{
	fpos_t pos;
	fgetpos(file, &pos);
	Jupiter::DataBuffer::pop_from(file);
	fsetpos(file, &pos);
}

void Jupiter::DataBuffer::peek_from(FILE *file, size_t size_)
{
	fpos_t pos;
	fgetpos(file, &pos);
	Jupiter::DataBuffer::pop_from(file, size_);
	fsetpos(file, &pos);
}

void Jupiter::DataBuffer::pop_from(FILE *file)
{
	size_t size_;
	fread(&size_, sizeof(size_t), 1, file);
	Jupiter::DataBuffer::pop_from(file, size_);
}

void Jupiter::DataBuffer::pop_from(FILE *file, size_t size_)
{
	Jupiter::DataBuffer::secure(size_);
	int chr;
	while (size_-- != 0)
	{
		chr = fgetc(file);
		if (chr == EOF)
			break;
		*Jupiter::DataBuffer::end++ = static_cast<uint8_t>(chr);
	}
}

void Jupiter::DataBuffer::copy_to(FILE *file)
{
	fwrite(std::addressof<const size_t>(Jupiter::DataBuffer::size()), sizeof(size_t), 1, file);
	Jupiter::DataBuffer::copy_to(file, Jupiter::DataBuffer::size());
}

void Jupiter::DataBuffer::copy_to(FILE *file, size_t size_)
{
	fwrite(Jupiter::DataBuffer::head, sizeof(uint8_t), size_, file);
}

void Jupiter::DataBuffer::copy_to(FILE *file, size_t index, size_t size_)
{
	fwrite(Jupiter::DataBuffer::head + index, sizeof(uint8_t), size_, file);
}

void Jupiter::DataBuffer::push_to(FILE *file)
{
	Jupiter::DataBuffer::copy_to(file);
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
	Jupiter::DataBuffer::end = Jupiter::DataBuffer::head;
}

void Jupiter::DataBuffer::push_to(FILE *file, size_t size_)
{
	Jupiter::DataBuffer::copy_to(file, size_);
	Jupiter::DataBuffer::head += size_;
}

size_t Jupiter::DataBuffer::shrink()
{
	Jupiter::DataBuffer::bufferSize = Jupiter::DataBuffer::size();

	if (Jupiter::DataBuffer::base != Jupiter::DataBuffer::head)
		memmove(Jupiter::DataBuffer::base, Jupiter::DataBuffer::head, Jupiter::DataBuffer::bufferSize);

	Jupiter::DataBuffer::base = reinterpret_cast<uint8_t *>(realloc(Jupiter::DataBuffer::base, Jupiter::DataBuffer::bufferSize));
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
	Jupiter::DataBuffer::end = Jupiter::DataBuffer::head + Jupiter::DataBuffer::bufferSize;
	return Jupiter::DataBuffer::bufferSize;
}

size_t Jupiter::DataBuffer::expandBuffer(size_t size_)
{
	Jupiter::DataBuffer::bufferSize += size_;
	size_ = Jupiter::DataBuffer::end - Jupiter::DataBuffer::head;

	if (Jupiter::DataBuffer::base != Jupiter::DataBuffer::head)
		memmove(Jupiter::DataBuffer::base, Jupiter::DataBuffer::head, size_);

	Jupiter::DataBuffer::base = reinterpret_cast<uint8_t *>(realloc(Jupiter::DataBuffer::base, Jupiter::DataBuffer::bufferSize));
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
	Jupiter::DataBuffer::end = Jupiter::DataBuffer::head + size_;
	return Jupiter::DataBuffer::bufferSize;
}

void Jupiter::DataBuffer::secure(size_t size_)
{
	size_t data_size = Jupiter::DataBuffer::end - Jupiter::DataBuffer::head;
	size_ += data_size;
	if (Jupiter::DataBuffer::bufferSize < size_)
	{
		if (Jupiter::DataBuffer::base != Jupiter::DataBuffer::head)
			memmove(Jupiter::DataBuffer::base, Jupiter::DataBuffer::head, data_size);

		Jupiter::DataBuffer::base = reinterpret_cast<uint8_t *>(realloc(Jupiter::DataBuffer::base, size_));
		Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
		Jupiter::DataBuffer::end = Jupiter::DataBuffer::head + data_size;
		Jupiter::DataBuffer::bufferSize = size_;
	}
}

void Jupiter::DataBuffer::setBufferSize(size_t size_)
{
	Jupiter::DataBuffer::bufferSize = size_;
	size_ = Jupiter::DataBuffer::end - Jupiter::DataBuffer::head;

	if (Jupiter::DataBuffer::base != Jupiter::DataBuffer::head)
		memmove(Jupiter::DataBuffer::base, Jupiter::DataBuffer::head, size_);

	Jupiter::DataBuffer::base = reinterpret_cast<uint8_t *>(realloc(Jupiter::DataBuffer::base, Jupiter::DataBuffer::bufferSize));
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
	Jupiter::DataBuffer::end = Jupiter::DataBuffer::head + size_;
}

size_t Jupiter::DataBuffer::size() const
{
	return Jupiter::DataBuffer::end - Jupiter::DataBuffer::head;
}

size_t Jupiter::DataBuffer::getBufferSize() const
{
	return Jupiter::DataBuffer::bufferSize;
}

uint8_t *Jupiter::DataBuffer::getHead() const
{
	return Jupiter::DataBuffer::head;
}

void Jupiter::DataBuffer::empty()
{
	Jupiter::DataBuffer::head = Jupiter::DataBuffer::base;
	Jupiter::DataBuffer::end = Jupiter::DataBuffer::head;
}