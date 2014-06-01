/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include <sys/stat.h>
#include "File.h"

/**
* TODO:
*	Upon writing Jupiter::String, port Jupiter::File to use Jupiter::String isntead of Jupiter::CString.
*	Instead of an array of String, consider an ArrayList to remove the cost of constant construction/destruction when expanding.
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

struct JUPITER_API Jupiter::File::Data
{
	Jupiter::CStringS fileName;
	size_t lineCount;
	Jupiter::CStringS *lines;

	Data();
	Data(const Data &data);
	~Data();
};

Jupiter::File::Data::Data()
{
	Jupiter::File::Data::lineCount = 0;
	Jupiter::File::Data::lines = nullptr;
}

Jupiter::File::Data::Data(const Jupiter::File::Data &data)
{
	Jupiter::File::Data::fileName = data.fileName;
	if ((Jupiter::File::Data::lineCount = data.lineCount) != 0)
	{
		Jupiter::File::Data::lines = new Jupiter::CStringS[Jupiter::File::Data::lineCount];
		for (unsigned int i = 0; i != Jupiter::File::Data::lineCount; i++) Jupiter::File::Data::lines[i] = data.lines[i];
	}
	else Jupiter::File::Data::lines = nullptr;
}

Jupiter::File::Data::~Data()
{
	if (Jupiter::File::Data::lines != nullptr) delete[] Jupiter::File::Data::lines;
}

Jupiter::File::File()
{
	Jupiter::File::data_ = new Jupiter::File::Data();
}

Jupiter::File::File(const File &file)
{
	Jupiter::File::data_ = new Jupiter::File::Data(*file.data_);
}

Jupiter::File::~File()
{
	delete Jupiter::File::data_;
}

unsigned int Jupiter::File::getLineCount() const
{
	return Jupiter::File::data_->lineCount;
}

const Jupiter::StringType &Jupiter::File::getLine(unsigned int line) const
{
	return Jupiter::File::data_->lines[line];
}

const Jupiter::StringType &Jupiter::File::getFileName() const
{
	return Jupiter::File::data_->fileName;
}

bool Jupiter::File::addData(const Jupiter::StringType &data)
{
	if (Jupiter::File::data_->lineCount == 0)
	{
		if ((Jupiter::File::data_->lineCount = data.wordCount(ENDL)) != 0)
		{
			Jupiter::File::data_->lines = new Jupiter::CStringS[Jupiter::File::data_->lineCount];
			for (unsigned int i = Jupiter::File::data_->lineCount - 1; i != 0; i--) Jupiter::File::data_->lines[i] = Jupiter::CStringS::getWord(data, i, ENDL);
			Jupiter::File::data_->lines[0] = Jupiter::CStringS::getWord(data, 0, ENDL);
			return true;
		}
		return false;
	}
	else
	{
		unsigned int wc = data.wordCount(ENDL);
		if (wc != 0)
		{
			Jupiter::CStringS *oldLines = Jupiter::File::data_->lines;
			Jupiter::File::data_->lines = new Jupiter::CStringS[Jupiter::File::data_->lineCount + wc];

			unsigned int i;
			for (i = 0; i < Jupiter::File::data_->lineCount; i++) Jupiter::File::data_->lines[i] = oldLines[i];
			delete[] oldLines;

			Jupiter::File::data_->lineCount += wc;
			for (unsigned int b = 0; b < wc; i++, b++) Jupiter::File::data_->lines[i] = Jupiter::CStringS::getWord(data, b, ENDL);
			return true;
		}
		return false;
	}
}

bool Jupiter::File::addData(const char *data)
{
	return Jupiter::File::addData(Jupiter::CStringS(data));
}

bool Jupiter::File::load(const char *file)
{
	FILE *filePtr = fopen(file, "rb");
	if (filePtr == nullptr) return false;
	if (Jupiter::File::data_->fileName.size() == 0) Jupiter::File::data_->fileName = file;
	bool r = Jupiter::File::load(filePtr);
	fclose(filePtr);
	return r;
}

bool Jupiter::File::load(FILE *file)
{
	Jupiter::CStringL fileBuffer;
	char buffer[defaultBufferSize];
	while (fgets(buffer, defaultBufferSize, file) != nullptr)
	{
		fileBuffer += buffer;
		if (feof(file)) break;
	}

	return Jupiter::File::addData(fileBuffer);
}

void Jupiter::File::unload()
{
	delete Jupiter::File::data_;
	Jupiter::File::data_ = new Jupiter::File::Data();
}

bool Jupiter::File::reload()
{
	if (Jupiter::File::data_->fileName.size() == 0) return false;
	Jupiter::CStringS fileName = Jupiter::File::data_->fileName;
	Jupiter::File::unload();
	return Jupiter::File::load(fileName.c_str());
}

bool Jupiter::File::reload(const char *file)
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
	if (Jupiter::File::data_->fileName.size() == 0) return false;
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

bool Jupiter::File::sync(FILE *file)
{
	for (unsigned int i = 0; i < Jupiter::File::data_->lineCount; i++)
	{
		Jupiter::File::data_->lines[i].println(file);
	}
	return true;
}