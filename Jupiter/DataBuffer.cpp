/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
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

Jupiter::DataBuffer::~DataBuffer()
{
	free(Jupiter::DataBuffer::base);
}

void Jupiter::DataBuffer::push(const uint8_t *data, size_t size_)
{
	Jupiter::DataBuffer::secure(size_);
	fputs("Pushing: \"", stdout);
	while (size_-- != 0)
	{
		fputc(*data, stdout);
		*Jupiter::DataBuffer::end++ = *data++;
	}
	puts("\"");
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