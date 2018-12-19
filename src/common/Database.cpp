/**
 * Copyright (C) 2015-2016 Jessica James.
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

#include "Database.h"
#include <cstring>

struct Jupiter::Database::Data
{
	static bool process_file(Jupiter::Database *db);
	static bool create_database(FILE *file, const Jupiter::DataBuffer *header);

	bool auto_create = true;
	Jupiter::CStringS file_name;
};

void Jupiter::Database::process_header(FILE *)
{
}

void Jupiter::Database::create_header(FILE *)
{
}

bool Jupiter::Database::process_file(const Jupiter::ReadableString &file)
{
	Jupiter::Database::data_->file_name = file;
	return Jupiter::Database::Data::process_file(this);
}

bool Jupiter::Database::process_file(const Jupiter::CStringType &file)
{
	Jupiter::Database::data_->file_name = file;
	return Jupiter::Database::Data::process_file(this);
}

bool Jupiter::Database::process_file(const char *file)
{
	Jupiter::Database::data_->file_name = file;
	return Jupiter::Database::Data::process_file(this);
}

bool Jupiter::Database::Data::process_file(Jupiter::Database *db)
{
	FILE *file = fopen(db->data_->file_name.c_str(), "rb");
	if (file == nullptr)
	{
		if (db->data_->auto_create)
		{
			file = fopen(db->data_->file_name.c_str(), "wb");
			if (file != nullptr)
			{
				db->create_header(file);
				db->process_file_finish(file);
				fclose(file);
				return true;
			}
		}
		return false;
	}
	bool r = db->process_file(file);
	fclose(file);
	return r;
}

Jupiter::Database::Database()
{
	Jupiter::Database::data_ = new Jupiter::Database::Data();
}

Jupiter::Database::~Database()
{
	delete Jupiter::Database::data_;
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

const Jupiter::CStringType &Jupiter::Database::getFilename() const
{
	return Jupiter::Database::data_->file_name;
}

bool Jupiter::Database::append(Jupiter::DataBuffer &data)
{
	return Jupiter::Database::append(Jupiter::Database::data_->file_name, data);
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

bool Jupiter::Database::append(FILE *file, Jupiter::DataBuffer &data)
{
	size_t data_size = data.size();
	fwrite(&data_size, sizeof(size_t), 1, file);
	fwrite(data.getHead(), sizeof(uint8_t), data.size(), file);
	return true;
}

bool Jupiter::Database::create_database(const Jupiter::ReadableString &file, const Jupiter::DataBuffer *header)
{
	char *str = new char[file.size() + 1];
	memcpy(str, file.ptr(), file.size() * sizeof(char));
	str[file.size()] = '\0';
	bool r = Jupiter::Database::create_database(str, header);
	delete[] str;
	return r;
}

bool Jupiter::Database::create_database(const Jupiter::CStringType &file, const Jupiter::DataBuffer *header)
{
	return Jupiter::Database::create_database(file.c_str(), header);
}

bool Jupiter::Database::create_database(const char *file, const Jupiter::DataBuffer *header)
{
	FILE *f = fopen(file, "wb");
	if (file == nullptr)
		return false;
	return Jupiter::Database::Data::create_database(f, header);
}

bool Jupiter::Database::Data::create_database(FILE *file, const Jupiter::DataBuffer *header)
{
	if (header != nullptr)
		fwrite(header->getHead(), sizeof(uint8_t), header->size(), file);
	fclose(file);
	return true;
}

bool Jupiter::Database::is_auto_create()
{
	return Jupiter::Database::data_->auto_create;
}

void Jupiter::Database::set_auto_create(bool in)
{
	Jupiter::Database::data_->auto_create = in;
}