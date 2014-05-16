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
#include "INIFile.h"
#include "CString.h"
#include "ArrayList.h"
#include "InvalidIndex.h"

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

const Jupiter::StringType &Jupiter::INIFile::Section::KeyValuePair::getKey() const
{
	return Jupiter::INIFile::Section::KeyValuePair::data_->key;
}

const Jupiter::StringType &Jupiter::INIFile::Section::KeyValuePair::getValue() const
{
	return Jupiter::INIFile::Section::KeyValuePair::data_->value;
}

void Jupiter::INIFile::Section::KeyValuePair::setValue(const Jupiter::StringType &value)
{
	Jupiter::INIFile::Section::KeyValuePair::data_->value = value;
}

void Jupiter::INIFile::Section::KeyValuePair::setValue(const char *value)
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

Jupiter::INIFile::Section::Section(const Jupiter::StringType &name)
{
	Jupiter::INIFile::Section::data_ = new Jupiter::INIFile::Section::Data();
	Jupiter::INIFile::Section::data_->name = name;
}

Jupiter::INIFile::Section::Section(const char *name)
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

const Jupiter::StringType &Jupiter::INIFile::Section::getName() const
{
	return Jupiter::INIFile::Section::data_->name;
}

const Jupiter::StringType &Jupiter::INIFile::Section::getValue(unsigned int index) const
{
	return Jupiter::INIFile::Section::data_->data.get(index)->data_->value;
}

const Jupiter::StringType &Jupiter::INIFile::Section::getValue(const Jupiter::StringType &key) const
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

const Jupiter::StringType &Jupiter::INIFile::Section::getValue(const char *key) const
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

Jupiter::INIFile::Section::KeyValuePair *Jupiter::INIFile::Section::getPair(const Jupiter::StringType &key) const
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

Jupiter::INIFile::Section::KeyValuePair *Jupiter::INIFile::Section::getPair(const char *key) const
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

bool Jupiter::INIFile::Section::hasKey(const Jupiter::StringType &key) const
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

bool Jupiter::INIFile::Section::hasKey(const char *key) const
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

bool Jupiter::INIFile::Section::set(Jupiter::StringType &key, Jupiter::StringType &value)
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

bool Jupiter::INIFile::Section::set(const char *key, const char *value)
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

void Jupiter::INIFile::Section::setName(const Jupiter::StringType &name)
{
	Jupiter::INIFile::Section::data_->name = name;
}

void Jupiter::INIFile::Section::setName(const char *name)
{
	Jupiter::INIFile::Section::data_->name = name;
}

bool Jupiter::INIFile::Section::remove(Jupiter::StringType &key)
{
	unsigned int size = Jupiter::INIFile::Section::data_->data.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (Jupiter::INIFile::Section::data_->data.get(i)->getKey().equalsi(key))
		{
			delete Jupiter::INIFile::Section::data_->data.remove(i);
			return true;
		}
	}
	return false;
}

bool Jupiter::INIFile::Section::remove(const char *key)
{
	unsigned int size = Jupiter::INIFile::Section::data_->data.size();
	for (unsigned int i = 0; i < size; i++)
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
	unsigned int size = Jupiter::INIFile::data_->data.size();
	for (unsigned int i = 0; i < size; i++)
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
	int count = 0;
	if (Jupiter::INIFile::data_->fName.size() != 0)
	{
		Jupiter::INIFile::data_->fName = "";
	}
	Jupiter::INIFile::data_->fName = fileName;
	FILE *file = fopen(Jupiter::INIFile::data_->fName.c_str(), "rb");
	if (file == nullptr) return Jupiter::ERROR_INDICATOR;
	char fileBuffer[8192];
	char *section;
	//int nSection = 0;
	while (!feof(file))
	{
		if (fgets(fileBuffer, sizeof(fileBuffer), file))
		{
do1:
			if (fileBuffer[strspn(fileBuffer, WHITESPACE)] != ';' && fileBuffer[strspn(fileBuffer, WHITESPACE)] == '[')
			{
				section = charToChar(fileBuffer, strspn(fileBuffer, WHITESPACE) + 1, strcspn(fileBuffer, "]"));
do2:
				if (fgets(fileBuffer, sizeof(fileBuffer), file))
				{
					size_t eqlpos = strcspn(fileBuffer, "=");
					if (fileBuffer[strspn(fileBuffer, WHITESPACE)] != ';' && eqlpos != strlen(fileBuffer))
					{
						size_t a = strspn(fileBuffer, WHITESPACE);
						char *tkey = charToChar(fileBuffer, a, a + strcspn(fileBuffer + a, WHITESPACE "="));
						char *tval = charToChar(fileBuffer, eqlpos + 1 + strspn(fileBuffer + eqlpos + 1, WHITESPACE), strcspn(fileBuffer, ENDL));
						if (Jupiter::INIFile::set(section, tkey, tval)) count++;
						delete[] tval;
						delete[] tkey;
					}
				}
				if (!feof(file))
				{
					if (fileBuffer[strspn(fileBuffer, WHITESPACE)] != '[') goto do2;
					else goto do1;
				}
			}
		}
	}
	fclose(file);
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
		fputs(section->getName().c_str(), file);
		fputs("]" ENDL, file);

		sectionSize = section->size();
		for (unsigned int b = 0; b < sectionSize; b++)
		{
			pair = section->getPair(b);
			fputs(pair->getKey().c_str(), file);
			fputc('=', file);
			fputs(pair->getValue().c_str(), file);
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

bool Jupiter::INIFile::set(const char *section, const char *key, const char *value)
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

bool Jupiter::INIFile::remove(const char *section, const char *key)
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

unsigned int Jupiter::INIFile::getSectionIndex(const char *section) const
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

const char *Jupiter::INIFile::getKey(const char *section, unsigned int index) const
{
	unsigned int sectionIndex = Jupiter::INIFile::getSectionIndex(section);
	if (sectionIndex == Jupiter::INVALID_INDEX) return nullptr;
	return Jupiter::INIFile::data_->data.get(sectionIndex)->getPair(index)->getKey().c_str();
}

unsigned int Jupiter::INIFile::getKeyIndex(const char *section, const char *key) const
{
	unsigned int sectionIndex = Jupiter::INIFile::getSectionIndex(section);
	if (sectionIndex == Jupiter::INVALID_INDEX) return Jupiter::INVALID_INDEX;
	Jupiter::INIFile::Section *sect = Jupiter::INIFile::data_->data.get(sectionIndex);
	unsigned int sectionSize = sect->size();
	for (unsigned int i = 0; i < sectionSize; i++) if (sect->getPair(i)->getKey().equalsi(key)) return i;
	return Jupiter::INVALID_INDEX;
}

const char *Jupiter::INIFile::get(const char *section, const char *key, const char *defaultValue) const
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
			return pair->getValue().c_str();
		}
	}
	return defaultValue;
}

bool Jupiter::INIFile::getBool(const char *section, const char *key, bool defaultValue) const
{
	const char *val = Jupiter::INIFile::get(section, key);
	if (val != nullptr)
	{
		if (streqli(val, "FALSE")) return false;
		if (streql(val, "0")) return false;
		if (streqli(val, "OFF")) return false;
		return true;
	}
	return defaultValue;
}

/*char Jupiter::INIFile::getChar(const char *section, const char *key) const
{
	const char *val = Jupiter::INIFile::get(section,key);
	if (val != nullptr) return val[0];
	return 0;
}*/

short Jupiter::INIFile::getShort(const char *section, const char *key, short defaultValue) const
{
	return (short)Jupiter::INIFile::getInt(section, key, defaultValue);
}

int Jupiter::INIFile::getInt(const char *section, const char *key, int defaultValue) const
{
	const char *val = Jupiter::INIFile::get(section, key);
	if (val != nullptr) return atoi(val);
	return defaultValue;
}

long Jupiter::INIFile::getLong(const char *section, const char *key, long defaultValue) const
{
	const char *val = Jupiter::INIFile::get(section, key);
	if (val != nullptr) return strtol(val, nullptr, 0);
	return defaultValue;
}

long long Jupiter::INIFile::getLongLong(const char *section, const char *key, long long defaultValue) const
{
	const char *val = Jupiter::INIFile::get(section, key);
	if (val != nullptr) return strtoll(val, nullptr, 0);
	return defaultValue;
}

float Jupiter::INIFile::getFloat(const char *section, const char *key, float defaultValue) const
{
	const char *val = Jupiter::INIFile::get(section, key);
	if (val != nullptr) return strtof(val, nullptr);
	return defaultValue;
}

double Jupiter::INIFile::getDouble(const char *section, const char *key, double defaultValue) const
{
	const char *val = Jupiter::INIFile::get(section, key);
	if (val != nullptr) return strtod(val, nullptr);
	return defaultValue;
}

long double Jupiter::INIFile::getLongDouble(const char *section, const char *key, long double defaultValue) const
{
	const char *val = Jupiter::INIFile::get(section, key);
	if (val != nullptr) return strtold(val, nullptr);
	return defaultValue;
}
