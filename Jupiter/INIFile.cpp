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
#include "String.h"
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
	unsigned int keySum = 0;
	Jupiter::StringS key;
	Jupiter::StringS value;
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
	Jupiter::INIFile::Section::KeyValuePair::data_->keySum = Jupiter::INIFile::Section::KeyValuePair::data_->key.calcChecksumi();
}

Jupiter::INIFile::Section::KeyValuePair::~KeyValuePair()
{
	delete Jupiter::INIFile::Section::KeyValuePair::data_;
}

unsigned int Jupiter::INIFile::Section::KeyValuePair::getKeyChecksum() const
{
	return Jupiter::INIFile::Section::KeyValuePair::data_->keySum;
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
	unsigned int nameSum = 0;
	Jupiter::StringS name;
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
	Jupiter::INIFile::Section::data_->nameSum = name.calcChecksumi();
	Jupiter::INIFile::Section::data_->name = name;
}

Jupiter::INIFile::Section::Section(const Jupiter::INIFile::Section &source)
{
	Jupiter::INIFile::Section::data_ = new Jupiter::INIFile::Section::Data();
	Jupiter::INIFile::Section::data_->name = source.data_->name;
	for (size_t i = 0, sourceSize = source.data_->data.size(); i != sourceSize; i++)
		Jupiter::INIFile::Section::data_->data.add(new Jupiter::INIFile::Section::KeyValuePair(*source.data_->data.get(i)));
}

Jupiter::INIFile::Section::~Section()
{
	delete Jupiter::INIFile::Section::data_;
}

unsigned int Jupiter::INIFile::Section::getNameChecksum() const
{
	return Jupiter::INIFile::Section::data_->nameSum;
}

const Jupiter::ReadableString &Jupiter::INIFile::Section::getName() const
{
	return Jupiter::INIFile::Section::data_->name;
}

const Jupiter::ReadableString &Jupiter::INIFile::Section::getValue(size_t index) const
{
	return Jupiter::INIFile::Section::data_->data.get(index)->data_->value;
}

const Jupiter::ReadableString &Jupiter::INIFile::Section::getValue(const Jupiter::ReadableString &key) const
{
	Jupiter::INIFile::Section::KeyValuePair *pair;
	const unsigned int keySum = key.calcChecksumi();
	size_t index = Jupiter::INIFile::Section::data_->data.size();
	while (index != 0)
	{
		pair = Jupiter::INIFile::Section::data_->data.get(--index);
		if (keySum == pair->getKeyChecksum() && pair->getKey().equalsi(key))
			return pair->getValue();
	}
	return Jupiter::ReferenceString::empty;
}

Jupiter::INIFile::Section::KeyValuePair *Jupiter::INIFile::Section::getPair(size_t index) const
{
	return Jupiter::INIFile::Section::data_->data.get(index);
}

Jupiter::INIFile::Section::KeyValuePair *Jupiter::INIFile::Section::getPair(const Jupiter::ReadableString &key) const
{
	Jupiter::INIFile::Section::KeyValuePair *pair;
	const unsigned int keySum = key.calcChecksumi();
	size_t index = Jupiter::INIFile::Section::data_->data.size();
	while (index != 0)
	{
		pair = Jupiter::INIFile::Section::data_->data.get(--index);
		if (keySum == pair->getKeyChecksum() && pair->getKey().equalsi(key))
			return pair;
	}
	return nullptr;
}

size_t Jupiter::INIFile::Section::size() const
{
	return Jupiter::INIFile::Section::data_->data.size();
}

bool Jupiter::INIFile::Section::hasKey(const Jupiter::ReadableString &key) const
{
	Jupiter::INIFile::Section::KeyValuePair *pair;
	const unsigned int keySum = key.calcChecksumi();
	size_t index = Jupiter::INIFile::Section::data_->data.size();
	while (index != 0)
	{
		pair = Jupiter::INIFile::Section::data_->data.get(--index);
		if (keySum == pair->getKeyChecksum() && pair->getKey().equalsi(key))
			return true;
	}
	return false;
}

bool Jupiter::INIFile::Section::set(const Jupiter::ReadableString &key, const Jupiter::ReadableString &value)
{
	Jupiter::INIFile::Section::KeyValuePair *pair;
	const unsigned int keySum = key.calcChecksumi();
	size_t index = Jupiter::INIFile::Section::data_->data.size();
	while (index != 0)
	{
		pair = Jupiter::INIFile::Section::data_->data.get(--index);
		if (keySum == pair->getKeyChecksum() && pair->getKey().equalsi(key))
		{
			pair->data_->value = value;
			return false;
		}
	}
	pair = new Jupiter::INIFile::Section::KeyValuePair();
	pair->data_->keySum = keySum;
	pair->data_->key = key;
	pair->data_->value = value;
	Jupiter::INIFile::Section::data_->data.add(pair);
	return true;
}

void Jupiter::INIFile::Section::setName(const Jupiter::ReadableString &name)
{
	Jupiter::INIFile::Section::data_->nameSum = name.calcChecksumi();
	Jupiter::INIFile::Section::data_->name = name;
}

bool Jupiter::INIFile::Section::remove(const Jupiter::ReadableString &key)
{
	Jupiter::INIFile::Section::KeyValuePair *pair;
	const unsigned int keySum = key.calcChecksumi();
	size_t index = Jupiter::INIFile::Section::data_->data.size();
	while (index != 0)
	{
		pair = Jupiter::INIFile::Section::data_->data.get(--index);
		if (keySum == pair->getKeyChecksum() && pair->getKey().equalsi(key))
		{
			delete Jupiter::INIFile::Section::data_->data.remove(index);
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
	for (size_t i = 0, sourceSize = source.data_->data.size(); i != sourceSize; i++)
		Jupiter::INIFile::data_->data.add(new Jupiter::INIFile::Section(*source.data_->data.get(i)));
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

unsigned int Jupiter::INIFile::readFile(const char *fileName)
{
	size_t total = 0;
	Jupiter::File file;
	if (file.load(fileName) == false)
		return Jupiter::ERROR_INDICATOR;

	Jupiter::INIFile::data_->fName = file.getFileName();
	Jupiter::ReferenceString section;
	Jupiter::ReferenceString line;
	for (size_t index = 0; index != file.getLineCount(); index++)
	{
		line = file.getLine(index);

		// check if line is a comment.
		while (line.size() != 0 && isspace(line.get(0)))
			line.shiftRight(1);

		if (line.size() == 0) continue; // Completely whitespace.
		if (line.get(0) == ';') continue; // Comment.

		while (isspace(line.get(line.size() - 1))) // This is safe due to the previous check, which confirms that there is a non-whitespace character.
			line.truncate(1);

		if (line.get(0) == '[') // changing sections.
		{
			line.shiftRight(1);

			// Truncate up to the last ']'.
			while (line.size() != 0 && line.get(line.size() - 1) != ']')
				line.truncate(1);
			line.truncate(1); // Truncate the ']' we stoped at.

			section.set(line);
		}
		else if (line.contains('=')) // key/value pair.
		{
			Jupiter::ReferenceString key = line.getWord(0, "=");
			Jupiter::ReferenceString value = line.substring(key.size() + 1);
			while (key.size() != 0 && isspace(key.get(key.size() - 1)))
				key.truncate(1);

			while (value.size() != 0 && isspace(value.get(0)))
				value.shiftRight(1);

			if (Jupiter::INIFile::set(section, key, value))
				total++;
		}
	}
	return total;
}

unsigned int Jupiter::INIFile::readFile(const Jupiter::ReadableString &file)
{
	return Jupiter::INIFile::readFile(Jupiter::CStringS(file).c_str());
}

unsigned int Jupiter::INIFile::reload()
{
	Jupiter::CStringS fileName = Jupiter::INIFile::data_->fName;
	Jupiter::INIFile::flushData();
	return Jupiter::INIFile::readFile(fileName.c_str());
}

bool Jupiter::INIFile::sync(const char *fileName)
{
	Jupiter::INIFile::Section *section;
	Jupiter::INIFile::Section::KeyValuePair *pair;

	FILE *file = fopen(fileName, "wb");
	if (file == nullptr) return false;
	fputs("; This file was automatically generated by Agent's INI parser." ENDL "; The currently used data values are as follows." ENDL ENDL, file);
	for (size_t a = 0; a != Jupiter::INIFile::data_->data.size(); a++)
	{
		section = Jupiter::INIFile::data_->data.get(a);
		fputc('[', file);
		section->getName().print(file);
		fputs("]" ENDL, file);

		for (size_t b = 0; b != section->size(); b++)
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

bool Jupiter::INIFile::sync(const Jupiter::ReadableString &file)
{
	return Jupiter::INIFile::sync(Jupiter::CStringS(file).c_str());
}

bool Jupiter::INIFile::sync()
{
	if (Jupiter::INIFile::data_->fName.size() == 0) return false;
	return Jupiter::INIFile::sync(Jupiter::INIFile::data_->fName.c_str());
}

bool Jupiter::INIFile::set(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, const Jupiter::ReadableString &value)
{
	Jupiter::INIFile::Section *sec;
	unsigned int nameSum = section.calcChecksumi();
	size_t index = Jupiter::INIFile::data_->data.size();
	while (index != 0)
	{
		sec = Jupiter::INIFile::data_->data.get(--index);
		if (nameSum == sec->getNameChecksum() && sec->getName().equalsi(section))
			return sec->set(key, value);
	}
	sec = new Jupiter::INIFile::Section(section);
	sec->set(key, value);
	Jupiter::INIFile::data_->data.add(sec);
	return true;
}

bool Jupiter::INIFile::remove(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key)
{
	Jupiter::INIFile::Section *sec;
	unsigned int nameSum = section.calcChecksumi();
	size_t index = Jupiter::INIFile::data_->data.size();
	while (index != 0)
	{
		sec = Jupiter::INIFile::data_->data.get(--index);
		if (nameSum == sec->getNameChecksum() && sec->getName().equalsi(section))
			return sec->remove(key);
	}
	return false;
}

size_t Jupiter::INIFile::getSections() const
{
	return Jupiter::INIFile::data_->data.size();
}

Jupiter::INIFile::Section *Jupiter::INIFile::getSection(size_t index) const
{
	return Jupiter::INIFile::data_->data.get(index);
}

Jupiter::INIFile::Section *Jupiter::INIFile::getSection(const Jupiter::ReadableString &section) const
{
	Jupiter::INIFile::Section *sec;
	unsigned int nameSum = section.calcChecksumi();
	size_t index = Jupiter::INIFile::data_->data.size();
	while (index != 0)
	{
		sec = Jupiter::INIFile::data_->data.get(--index);
		if (nameSum == sec->getNameChecksum() && sec->getName().equalsi(section))
			return Jupiter::INIFile::data_->data.get(index);
	}
	return nullptr;
}

size_t Jupiter::INIFile::getSectionIndex(const Jupiter::ReadableString &section) const
{
	Jupiter::INIFile::Section *sec;
	unsigned int nameSum = section.calcChecksumi();
	size_t index = Jupiter::INIFile::data_->data.size();
	while (index != 0)
	{
		sec = Jupiter::INIFile::data_->data.get(--index);
		if (nameSum == sec->getNameChecksum() && sec->getName().equalsi(section))
			return index;
	}
	return Jupiter::INVALID_INDEX;
}

size_t Jupiter::INIFile::getSectionLength(size_t index) const
{
	return Jupiter::INIFile::data_->data.get(index)->size();
}

const Jupiter::ReadableString &Jupiter::INIFile::getKey(const Jupiter::ReadableString &section, size_t index) const
{
	size_t sectionIndex = Jupiter::INIFile::getSectionIndex(section);
	if (sectionIndex == Jupiter::INVALID_INDEX)
		return Jupiter::ReferenceString::empty;
	return Jupiter::INIFile::data_->data.get(sectionIndex)->getPair(index)->getKey();
}

size_t Jupiter::INIFile::getKeyIndex(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key) const
{
	size_t sectionIndex = Jupiter::INIFile::getSectionIndex(section);
	if (sectionIndex == Jupiter::INVALID_INDEX)
		return Jupiter::INVALID_INDEX;

	Jupiter::INIFile::Section::KeyValuePair *pair;
	Jupiter::INIFile::Section *sec = Jupiter::INIFile::data_->data.get(sectionIndex);
	unsigned int keySum = key.calcChecksumi();
	size_t index = sec->size();
	while (index != 0)
	{
		pair = sec->getPair(--index);
		if (keySum == pair->getKeyChecksum() && pair->getKey().equalsi(key))
			return index;
	}
	return Jupiter::INVALID_INDEX;
}

const Jupiter::ReadableString &Jupiter::INIFile::get(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, const Jupiter::ReadableString &defaultValue) const
{
	Jupiter::INIFile::Section *sec;
	unsigned int nameSum = section.calcChecksumi();
	size_t index = Jupiter::INIFile::data_->data.size();
	while (index != 0)
	{
		sec = Jupiter::INIFile::data_->data.get(--index);
		if (nameSum == sec->getNameChecksum() && sec->getName().equalsi(section))
		{
			Jupiter::INIFile::Section::KeyValuePair *pair = sec->getPair(key);
			if (pair == nullptr)
				return defaultValue;
			return pair->getValue();
		}
	}
	return defaultValue;
}

bool Jupiter::INIFile::getBool(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, bool defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.isEmpty() == false)
		return val.asBool();
	return defaultValue;
}

short Jupiter::INIFile::getShort(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, short defaultValue) const
{
	return static_cast<short>(Jupiter::INIFile::getInt(section, key, defaultValue));
}

int Jupiter::INIFile::getInt(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, int defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.isEmpty() == false)
		return val.asInt();
	return defaultValue;
}

long Jupiter::INIFile::getLong(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, long defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.isEmpty() == false)
		return val.asInt();
	return defaultValue;
}

long long Jupiter::INIFile::getLongLong(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, long long defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.isEmpty() == false)
		return val.asLongLong();
	return defaultValue;
}

float Jupiter::INIFile::getFloat(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, float defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.isEmpty() == false)
		return float(val.asDouble());
	return defaultValue;
}

double Jupiter::INIFile::getDouble(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, double defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.isEmpty() == false)
		return val.asDouble();
	return defaultValue;
}

long double Jupiter::INIFile::getLongDouble(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, long double defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::INIFile::get(section, key);
	if (val.isEmpty() == false)
		return val.asDouble();
	return defaultValue;
}
