/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include <sys/stat.h>
#include "File.h"
#include "CString.h"
#include "String.h"
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

template class JUPITER_API Jupiter::CString_Type<char>;
template class JUPITER_API Jupiter::ArrayList<Jupiter::StringS>;

struct JUPITER_API Jupiter::File::Data
{
	Jupiter::CStringS fileName;
	Jupiter::ArrayList<Jupiter::StringS> lines;

	Data();
	Data(const Data &data);
	~Data();
};

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
	int c = fgetc(file);

	if (c == EOF)
		return false;
	if (containsSymbol(ENDL, static_cast<char>(c)))
	{
		Jupiter::File::data_->lines.add(new Jupiter::StringS(buffer));
		c = fgetc(file);
		if (c == EOF)
			return true;
		if (containsSymbol(ENDL, static_cast<char>(c)) == false)
			buffer.concat(static_cast<char>(c));
	}
	else
		buffer.concat(static_cast<char>(c));

	for (;;)
	{
		c = fgetc(file);
		if (c == EOF)
			return true;
		if (containsSymbol(ENDL, static_cast<char>(c)))
		{
			Jupiter::File::data_->lines.add(new Jupiter::StringS(buffer));
			buffer.truncate(buffer.size());
			c = fgetc(file);
			if (c == EOF)
				return true;
			if (containsSymbol(ENDL, static_cast<char>(c)) == false)
				buffer.concat(static_cast<char>(c));
		}
		else
			buffer.concat(static_cast<char>(c));
	}

	return true;
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