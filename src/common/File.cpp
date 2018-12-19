/**
 * Copyright (C) 2014-2015 Jessica James.
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

#include <sys/stat.h>
#include "File.h"
#include "CString.h"
#include "String.hpp"
#include "Reference_String.h"
#include "ArrayList.h"

/**
* TODO:
*	Consider replacing the ArrayList of Strings with a Rope (requires a Rope implementation).
*/

#if defined _WIN32
#define stat64 _stat64
#endif

int64_t getFileSize(const char *file)
{
	struct stat64 data;
	if (stat64(file, &data) == 0) return data.st_size;
	return -1;
}

const size_t defaultBufferSize = 8192;

/** DLL Linkage Nagging */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

struct JUPITER_API Jupiter::File::Data
{
	Jupiter::CStringS fileName;
	Jupiter::ArrayList<Jupiter::StringS> lines;

	Data();
	Data(const Data &data);
	~Data();
};

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

Jupiter::File::Data::Data()
{
}

Jupiter::File::Data::Data(const Jupiter::File::Data &data)
{
	Jupiter::File::Data::fileName = data.fileName;

	for (size_t i = 0; i != Jupiter::File::Data::lines.size(); i++)
		Jupiter::File::Data::lines.add(new Jupiter::StringS(*data.lines.get(i)));
}

Jupiter::File::Data::~Data()
{
	Jupiter::File::Data::lines.emptyAndDelete();
}

Jupiter::File::File()
{
	Jupiter::File::data_ = new Jupiter::File::Data();
}

Jupiter::File::File(File &&file)
{
	Jupiter::File::data_ = file.data_;
	file.data_ = new Jupiter::File::Data();
}

Jupiter::File::File(const File &file)
{
	Jupiter::File::data_ = new Jupiter::File::Data(*file.data_);
}

Jupiter::File::~File()
{
	delete Jupiter::File::data_;
}

size_t Jupiter::File::getLineCount() const
{
	return Jupiter::File::data_->lines.size();
}

const Jupiter::ReadableString &Jupiter::File::getLine(size_t line) const
{
	return *Jupiter::File::data_->lines.get(line);
}

const Jupiter::ReadableString &Jupiter::File::getFileName() const
{
	return Jupiter::File::data_->fileName;
}

bool Jupiter::File::addData(const Jupiter::ReadableString &data)
{
	unsigned int wc = data.wordCount(ENDL);
	if (wc == 0) return false;

	for (unsigned int i = 0; i < wc; i++) Jupiter::File::data_->lines.add(new Jupiter::StringS(std::move(Jupiter::StringS::getWord(data, i, ENDL))));
	return true;
}

bool Jupiter::File::load(const char *file)
{
	FILE *filePtr = fopen(file, "rb");
	if (filePtr == nullptr) return false;
	if (Jupiter::File::data_->fileName.isEmpty())
		Jupiter::File::data_->fileName = file;
	bool r = Jupiter::File::load(filePtr);
	fclose(filePtr);
	return r;
}

bool Jupiter::File::load(const Jupiter::ReadableString &file)
{
	Jupiter::CStringS fileName = file;
	FILE *filePtr = fopen(fileName.c_str(), "rb");
	if (filePtr == nullptr) return false;
	if (Jupiter::File::data_->fileName.isEmpty())
		Jupiter::File::data_->fileName = file;
	bool r = Jupiter::File::load(filePtr);
	fclose(filePtr);
	return r;
}

bool Jupiter::File::load(FILE *file)
{
	Jupiter::String buffer(defaultBufferSize);

	int chr;

	while (true)
	{
		chr = fgetc(file);

		if (chr == EOF)
		{
			if (buffer.isNotEmpty())
			{
				Jupiter::File::data_->lines.add(new Jupiter::StringS(buffer));
				return true;
			}

			return Jupiter::File::data_->lines.size() != 0;
		}

		if (static_cast<char>(chr) == '\r') // new line
		{
		new_line_r:

			Jupiter::File::data_->lines.add(new Jupiter::StringS(buffer));
			buffer.erase();

			// check for optional trailing \n

			chr = fgetc(file);

			switch (chr)
			{
			case EOF:
				return true;

			case '\r':
				goto new_line_r;

			case '\n':
				continue;

			default:
				buffer += static_cast<char>(chr);
			}

			continue;
		}

		if (static_cast<char>(chr) == '\n') // new line
		{
		new_line_n:

			Jupiter::File::data_->lines.add(new Jupiter::StringS(buffer));
			buffer.erase();

			// check for optional trailing \r

			chr = fgetc(file);

			switch (chr)
			{
			case EOF:
				return true;

			case '\n':
				goto new_line_n;

			case '\r':
				continue;

			default:
				buffer += static_cast<char>(chr);
			}

			continue;
		}

		buffer += static_cast<char>(chr);
	}
}

void Jupiter::File::unload()
{
	Jupiter::File::data_->fileName.set("");
	Jupiter::File::data_->lines.emptyAndDelete();
}

bool Jupiter::File::reload()
{
	if (Jupiter::File::data_->fileName.isEmpty())
		return false;
	Jupiter::CStringS fileName(std::move(Jupiter::File::data_->fileName));
	Jupiter::File::unload();
	return Jupiter::File::load(fileName.c_str());
}

bool Jupiter::File::reload(const char *file)
{
	Jupiter::File::unload();
	return Jupiter::File::load(file);
}

bool Jupiter::File::reload(const Jupiter::ReadableString &file)
{
	Jupiter::File::unload();
	return Jupiter::File::load(file);
}

bool Jupiter::File::reload(FILE *file)
{
	Jupiter::File::unload();
	return Jupiter::File::load(file);
}

bool Jupiter::File::sync()
{
	if (Jupiter::File::data_->fileName.isEmpty())
		return false;
	return Jupiter::File::sync(Jupiter::File::data_->fileName.c_str());
}

bool Jupiter::File::sync(const char *file)
{
	FILE *filePtr = fopen(file, "wb");
	if (filePtr == nullptr) return false;
	Jupiter::File::sync(filePtr); // Always returns true.
	fclose(filePtr);
	return true;
}

bool Jupiter::File::sync(const Jupiter::ReadableString &file)
{
	return Jupiter::File::sync(Jupiter::CStringS(file).c_str());
}

bool Jupiter::File::sync(FILE *file)
{
	for (size_t i = 0; i != Jupiter::File::data_->lines.size(); i++)
		Jupiter::File::data_->lines.get(i)->println(file);
	return true;
}