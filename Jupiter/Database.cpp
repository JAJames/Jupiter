/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include "Database.h"

void Jupiter::Database::process_header(FILE *)
{
}

void Jupiter::Database::create_header(FILE *)
{
}

bool Jupiter::Database::process_file(Jupiter::ReadableString &file)
{
	Jupiter::Database::file_name = file;
	return Jupiter::Database::process_file();
}

bool Jupiter::Database::process_file(Jupiter::CStringType &file)
{
	Jupiter::Database::file_name = file;
	return Jupiter::Database::process_file();
}

bool Jupiter::Database::process_file(const char *file)
{
	Jupiter::Database::file_name = file;
	return Jupiter::Database::process_file();
}

bool Jupiter::Database::process_file()
{
	FILE *file = fopen(Jupiter::Database::file_name.c_str(), "rb");
	if (file == nullptr)
	{
		if (Jupiter::Database::auto_create)
		{
			file = fopen(Jupiter::Database::file_name.c_str(), "wb");
			if (file != nullptr)
			{
				this->create_header(file);
				return true;
			}
		}
		return false;
	}
	bool r = Jupiter::Database::process_file(file);
	fclose(file);
	return r;
}

bool Jupiter::Database::process_file(FILE *file)
{
	/**
	* FILE FORMAT:
	*	HEADER
	*	DATA (Buffer[x])
	*/
	Jupiter::DataBuffer buffer;
	this->process_header(file);
	fpos_t pos;
	fgetpos(file, &pos);

	while (fgetc(file) != EOF)
	{
		fsetpos(file, &pos);
		buffer.pop_from(file);
		this->process_data(buffer, file, pos);
		buffer.empty();
		fgetpos(file, &pos);
	}
	this->process_file_finish(file);
	return true;
}

void Jupiter::Database::process_file_finish(FILE *)
{
}

bool Jupiter::Database::append(Jupiter::DataBuffer &data)
{
	return Jupiter::Database::append(Jupiter::Database::file_name, data);
}

bool Jupiter::Database::append(Jupiter::ReadableString &file, Jupiter::DataBuffer &data)
{
	char *str = new char[file.size() + 1];
	memcpy(str, file.ptr(), file.size() * sizeof(char));
	str[file.size()] = '\0';
	bool r = Jupiter::Database::append(str, data);
	delete[] str;
	return r;
}

bool Jupiter::Database::append(Jupiter::CStringType &file, Jupiter::DataBuffer &data)
{
	return Jupiter::Database::append(file.c_str(), data);
}

bool Jupiter::Database::append(const char *file, Jupiter::DataBuffer &data)
{
	FILE *f = fopen(file, "ab");
	if (f == nullptr)
		return false;
	bool r = Jupiter::Database::append(f, data);
	fclose(f);
	return r;
}

template<typename T> T *get_ptr(T &in)
{
	return &in;
}

bool Jupiter::Database::append(FILE *file, Jupiter::DataBuffer &data)
{
	fwrite(std::addressof<const size_t>(data.size()), sizeof(size_t), 1, file);
	fwrite(data.getHead(), sizeof(uint8_t), data.size(), file);
	return true;
}

bool Jupiter::Database::create_database(Jupiter::ReadableString &file, Jupiter::DataBuffer *header)
{
	char *str = new char[file.size() + 1];
	memcpy(str, file.ptr(), file.size() * sizeof(char));
	str[file.size()] = '\0';
	bool r = Jupiter::Database::create_database(str, header);
	delete[] str;
	return r;
}

bool Jupiter::Database::create_database(Jupiter::CStringType &file, Jupiter::DataBuffer *header)
{
	return Jupiter::Database::create_database(file.c_str(), header);
}

bool Jupiter::Database::create_database(const char *file, Jupiter::DataBuffer *header)
{
	FILE *f = fopen(file, "wb");
	if (file == nullptr)
		return false;
	return Jupiter::Database::create_database(f, header);
}

bool Jupiter::Database::create_database(FILE *file, Jupiter::DataBuffer *header)
{
	if (header != nullptr)
		fwrite(header->getHead(), sizeof(uint8_t), header->size(), file);
	fclose(file);
	return true;
}

bool Jupiter::Database::is_auto_create()
{
	return Jupiter::Database::auto_create;
}

void Jupiter::Database::set_auto_create(bool auto_create_)
{
	Jupiter::Database::auto_create = auto_create_;
}