/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "Functions.h"
#include "File.h"
#include "INIFile.h"
#include "CString.h"
#include "ArrayList.h"
#include "InvalidIndex.h"
#include "Reference_String.h"

/**
* TODO:
*	Rewrite using String, and ArrayList. Consider using File when reading data.
*	Add support for unspecified section.
*	Consider sort/search optimization.
*/

/** KeyValuePair implementation */

struct Jupiter::INIFile::Section::KeyValuePair::Data
{
	Jupiter::CStringS key;
	Jupiter::CStringS value;
};

Jupiter::INIFile::Section::KeyValuePair::KeyValuePair()
{
	Jupiter::INIFile::Section::KeyValuePair::data_ = new Jupiter::INIFile::Section::KeyValuePair::Data();
}

Jupiter::INIFile::Section::KeyValuePair::KeyValuePair(const KeyValuePair &source)
{
	Jupiter::INIFile::Section::KeyValuePair::data_ = new Jupiter::INIFile::Section::KeyValuePair::Data();
	Jupiter::INIFile::Section::KeyValuePair::data_->key = source.data_->key;
	Jupiter::INIFile::Section::KeyValuePair::data_->value = source.data_->value;
}

Jupiter::INIFile::Section::KeyValuePair::~KeyValuePair()
{
	delete Jupiter::INIFile::Section::KeyValuePair::data_;
}

const Jupiter::ReadableString &Jupiter::INIFile::Section::KeyValuePair::getKey() const
{
	return Jupiter::INIFile::Section::KeyValuePair::data_->key;
}

const Jupiter::ReadableString &Jupiter::INIFile::Section::KeyValuePair::getValue() const
{
	return Jupiter::INIFile::Section::KeyValuePair::data_->value;
}

void Jupiter::INIFile::Section::KeyValuePair::setValue(const Jupiter::ReadableString &value)
{
	Jupiter::INIFile::Section::KeyValuePair::data_->value = value;
}

/** Section implementation */

struct Jupiter::INIFile::Section::Data
{
	Jupiter::CStringS name;
	Jupiter::ArrayList<Jupiter::INIFile::Section::KeyValuePair> data;
	~Data();
};

Jupiter::INIFile::Section::Data::~Data()
{
	Jupiter::INIFile::Section::Data::data.emptyAndDelete();
}

Jupiter::INIFile::Section::Section()
{
	Jupiter::INIFile::Section::data_ = new Jupiter::INIFile::Section::Data();
}

Jupiter::INIFile::Section::Section(const Jupiter::ReadableString &name)
{
	Jupiter::INIFile::Section::data_ = new Jupiter::INIFile::Section::Data();
	Jupiter::INIFile::Section::data_->name = name;
}

Jupiter::INIFile::Section::Section(const Jupiter::INIFile::Section &source)
{
	Jupiter::INIFile::Section::data_ = new Jupiter::INIFile::Section::Data();
	Jupiter::INIFile::Section::data_->name = source.data_->name;
	unsigned int size = Jupiter::INIFile::Section::data_->data.size();
	for (unsigned int i = 0; i < size; i++)
	{
		Jupiter::INIFile::Section::data_->data.add(new Jupiter::INIFile::Section::KeyValuePair(*source.data_->data.get(i)));
	}
}

Jupiter::INIFile::Section::~Section()
{
	delete Jupiter::INIFile::Section::data_;
}

const Jupiter::ReadableString &Jupiter::INIFile::Section::getName() const
{
	return Jupiter::INIFile::Section::data_->name;
}

const Jupiter::ReadableString &Jupiter::INIFile::Section::getValue(unsigned int index) const
{
	return Jupiter::INIFile::Section::data_->data.get(index)->data_->value;
}

const Jupiter::ReadableString &Jupiter::INIFile::Section::getValue(const Jupiter::ReadableString &key) const
{
	unsigned int size = Jupiter::INIFile::Section::data_->data.size();
	Jupiter::INIFile::Section::KeyValuePair *pair;
	for (unsigned int i = 0; i < size; i++)
	{
		pair = Jupiter::INIFile::Section::data_->data.get(i);
		if (pair->getKey().equalsi(key)) return pair->getValue();
	}
	return Jupiter::emptyString;
}

Jupiter::INIFile::Section::KeyValuePair *Jupiter::INIFile::Section::getPair(unsigned int index) const
{
	return Jupiter::INIFile::Section::data_->data.get(index);
}

Jupiter::INIFile::Section::KeyValuePair *Jupiter::INIFile::Section::getPair(const Jupiter::ReadableString &key) const
{
	unsigned int size = Jupiter::INIFile::Section::data_->data.size();
	Jupiter::INIFile::Section::KeyValuePair *pair;
	for (unsigned int i = 0; i < size; i++)
	{
		pair = Jupiter::INIFile::Section::data_->data.get(i);
		if (pair->getKey().equalsi(key)) return pair;
	}
	return nullptr;
}

unsigned int Jupiter::INIFile::Section::size() const
{
	return Jupiter::INIFile::Section::data_->data.size();
}

bool Jupiter::INIFile::Section::hasKey(const Jupiter::ReadableString &key) const
{
	unsigned int size = Jupiter::INIFile::Section::data_->data.size();
	Jupiter::INIFile::Section::KeyValuePair *pair;
	for (unsigned int i = 0; i < size; i++)
	{
		pair = Jupiter::INIFile::Section::data_->data.get(i);
		if (pair->getKey().equalsi(key)) return true;
	}
	return false;
}

bool Jupiter::INIFile::Section::set(const Jupiter::ReadableString &key, const Jupiter::ReadableString &value)
{
	unsigned int size = Jupiter::INIFile::Section::data_->data.size();
	Jupiter::INIFile::Section::KeyValuePair *pair;
	for (unsigned int i = 0; i < size; i++)
	{
		pair = Jupiter::INIFile::Section::data_->data.get(i);
		if (pair->getKey().equalsi(key))
		{
			pair->data_->value = value;
			return false;
		}
	}
	pair = new Jupiter::INIFile::Section::KeyValuePair();
	pair->data_->key = key;
	pair->data_->value = value;
	Jupiter::INIFile::Section::data_->data.add(pair);
	return true;
}

void Jupiter::INIFile::Section::setName(const Jupiter::ReadableString &name)
{
	Jupiter::INIFile::Section::data_->name = name;
}

bool Jupiter::INIFile::Section::remove(const Jupiter::ReadableString &key)
{
	for (size_t i = 0; i < Jupiter::INIFile::Section::data_->data.size(); i++)
	{
		if (Jupiter::INIFile::Section::data_->data.get(i)->getKey().equalsi(key))
		{
			delete Jupiter::INIFile::Section::data_->data.remove(i);
			return true;
		}
	}
	return false;
}

/** INIFile implementation */

struct Jupiter::INIFile::Data
{
	Jupiter::ArrayList<Section> data;
	Jupiter::CStringS fName;
	~Data();
};

Jupiter::INIFile::Data::~Data()
{
	Jupiter::INIFile::Data::data.emptyAndDelete();
}

Jupiter::INIFile::INIFile()
{
	Jupiter::INIFile::data_ = new Jupiter::INIFile::Data;
}

Jupiter::INIFile::INIFile(const INIFile &source)
{
	Jupiter::INIFile::data_ = new Jupiter::INIFile::Data;
	Jupiter::INIFile::data_->fName = source.data_->fName;
	for (size_t i = 0; i < Jupiter::INIFile::data_->data.size(); i++)
	{
		Jupiter::INIFile::data_->data.add(new Jupiter::INIFile::Section(*source.data_->data.get(i)));
	}
}

Jupiter::INIFile::~INIFile()
{
	delete Jupiter::INIFile::data_;
}

void Jupiter::INIFile::flushData()
{
	delete Jupiter::INIFile::data_;
	Jupiter::INIFile::data_ = new Jupiter::INIFile::Data();
}

// TODO: Rewrite this.
unsigned int Jupiter::INIFile::readFile(const char *fileName)
{
	unsigned int count = 0;
	Jupiter::File file;
	if (file.load(fileName) == false) return Jupiter::ERROR_INDICATOR;
	Jupiter::INIFile::data_->fName = file.getFileName();

	const char *ptr;
	const char *ptrEnd;
	Jupiter::ReferenceString section;
	for (unsigned int index = 0; index != file.getLineCount(); index++)
	{
		const Jupiter::ReadableString &line = file.getLine(index);

		// check if line is a comment.
		ptr = line.ptr();
		ptrEnd = ptr + line.size();
		while (ptr != ptrEnd && isspace(*ptr)) ptr++;

		if (ptr == ptrEnd) continue; // line is completely whitespace.

		if (*ptr == '[') // changing sections.
		{
			section.set(Jupiter::ReferenceString::getWord(++ptr, 0, "]"));
		}
		else if (line.contains('=')) // key/value pair.
		{
			size_t eqlPos = line.find('=');

			const char *endOfKey = line.ptr() + eqlPos;
			while (endOfKey != ptr)
			{
				ptr--;
				if (!isspace(*endOfKey)) break;
			}

			const char *startOfValue = line.ptr() + eqlPos + 1;
			while (startOfValue != ptrEnd && isspace(*startOfValue)) startOfValue++;

			Jupiter::INIFile::set(section, Jupiter::ReferenceString(ptr, endOfKey - ptr), Jupiter::ReferenceString(startOfValue, ptrEnd - startOfValue));
		}
	}
	return count;
}

unsigned int Jupiter::INIFile::reload()
{
	Jupiter::CStringS fileName = Jupiter::INIFile::data_->fName;
	Jupiter::INIFile::flushData();
	return Jupiter::INIFile::readFile(fileName.c_str());
}

bool Jupiter::INIFile::sync(const char *fileName)
{
	unsigned int size = Jupiter::INIFile::data_->data.size();
	unsigned int sectionSize;
	Jupiter::INIFile::Section *section;
	Jupiter::INIFile::Section::KeyValuePair *pair;

	FILE *file = fopen(fileName, "wb");
	if (file == nullptr) return false;
	fputs("; This file was automatically generated by Agent's INI parser." ENDL "; The currently used data values are as follows." ENDL ENDL, file);
	for (unsigned int a = 0; a < size; a++)
	{
		section = Jupiter::INIFile::data_->data.get(a);
		fputc('[', file);
		section->getName().print(file);
		fputs("]" ENDL, file);

		sectionSize = section->size();
		for (unsigned int b = 0; b < sectionSize; b++)
		{
			pair = section->getPair(b);
			pair->getKey().print(file);
			fputc('=', file);
			pair->getValue().print(file);
			fputs(ENDL, file);
		}
		fputs(ENDL, file);
	}
	fputs("; EOF", file);
	fclose(file);
	return true;
}

bool Jupiter::INIFile::sync()
{
	if (Jupiter::INIFile::data_->fName.size() == 0) return false;
	return Jupiter::INIFile::sync(Jupiter::INIFile::data_->fName.c_str());
}

bool Jupiter::INIFile::set(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, const Jupiter::ReadableString &value)
{
	unsigned int i = 0;
	unsigned int size = Jupiter::INIFile::data_->data.size();
	Jupiter::INIFile::Section *sec;
	while (i < size)
	{
		sec = Jupiter::INIFile::data_->data.get(i);
		if (sec->getName().equalsi(section))
		{
			sec->set(key, value);
			return false;
		}
		i++;
	}
	sec = new Jupiter::INIFile::Section(section);
	sec->set(key, value);
	Jupiter::INIFile::data_->data.add(sec);
	return true;
}

bool Jupiter::INIFile::remove(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key)
{
	unsigned int size = Jupiter::INIFile::data_->data.size();
	Jupiter::INIFile::Section *sec;
	for (unsigned int i = 0; i < size; i++)
	{
		sec = Jupiter::INIFile::data_->data.get(i);
		if (sec->getName().equalsi(section)) return sec->remove(key);
		i++;
	}
	return false;
}

unsigned int Jupiter::INIFile::getSections() const
{
	return Jupiter::INIFile::data_->data.size();
}

Jupiter::INIFile::Section *Jupiter::INIFile::getSection(unsigned int index) const
{
	return Jupiter::INIFile::data_->data.get(index);
}

unsigned int Jupiter::INIFile::getSectionIndex(const Jupiter::ReadableString &section) const
{
	unsigned int size = Jupiter::INIFile::data_->data.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (Jupiter::INIFile::data_->data.get(i)->getName().equalsi(section)) return i;
	}
	return Jupiter::INVALID_INDEX;
}

unsigned int Jupiter::INIFile::getSectionLength(unsigned int index) const
{
	return Jupiter::INIFile::data_->data.get(index)->size();
}

const Jupiter::ReadableString &Jupiter::INIFile::getKey(const Jupiter::ReadableString &section, unsigned int index) const
{
	unsigned int sectionIndex = Jupiter::INIFile::getSectionIndex(section);
	if (sectionIndex == Jupiter::INVALID_INDEX) return Jupiter::ReferenceString::empty;
	return Jupiter::INIFile::data_->data.get(sectionIndex)->getPair(index)->getKey();
}

unsigned int Jupiter::INIFile::getKeyIndex(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key) const
{
	unsigned int sectionIndex = Jupiter::INIFile::getSectionIndex(section);
	if (sectionIndex == Jupiter::INVALID_INDEX) return Jupiter::INVALID_INDEX;
	Jupiter::INIFile::Section *sect = Jupiter::INIFile::data_->data.get(sectionIndex);
	unsigned int sectionSize = sect->size();
	for (unsigned int i = 0; i < sectionSize; i++) if (sect->getPair(i)->getKey().equalsi(key)) return i;
	return Jupiter::INVALID_INDEX;
}

const Jupiter::ReadableString &Jupiter::INIFile::get(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, const Jupiter::ReadableString &defaultValue) const
{
	unsigned int size = Jupiter::INIFile::data_->data.size();
	Jupiter::INIFile::Section *sect;

	for (unsigned int i = 0; i < size; i++)
	{
		sect = Jupiter::INIFile::data_->data.get(i);
		if (sect->getName().equalsi(section))
		{
			Jupiter::INIFile::Section::KeyValuePair *pair = sect->getPair(key);
			if (pair == nullptr) return defaultValue;
			return pair->getValue();
		}
	}
	return defaultValue;
}

bool Jupiter::INIFile::getBool(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, bool defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.size() != 0) return val.asBool();
	return defaultValue;
}

short Jupiter::INIFile::getShort(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, short defaultValue) const
{
	return (short)Jupiter::INIFile::getInt(section, key, defaultValue);
}

int Jupiter::INIFile::getInt(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, int defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.size() != 0) return val.asInt();
	return defaultValue;
}

long Jupiter::INIFile::getLong(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, long defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.size() != 0) return val.asInt();
	return defaultValue;
}

long long Jupiter::INIFile::getLongLong(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, long long defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.size() != 0) return val.asInt();
	return defaultValue;
}

float Jupiter::INIFile::getFloat(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, float defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.size() != 0) return float(val.asDouble());
	return defaultValue;
}

double Jupiter::INIFile::getDouble(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, double defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.size() != 0) return val.asDouble();
	return defaultValue;
}

long double Jupiter::INIFile::getLongDouble(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, long double defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.size() != 0) return val.asDouble();
	return defaultValue;
}
