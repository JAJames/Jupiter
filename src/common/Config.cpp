/**
 * Copyright (C) 2016 Jessica James.
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

#include "Config.h"

const Jupiter::Config o_null_section;

Jupiter::Config::Config(const Config& in_config)
	: m_sections{ std::make_unique<SectionHashTable>(*in_config.m_sections) } {
}

Jupiter::Config& Jupiter::Config::operator=(const Config& in_config) {
	m_sections = std::make_unique<SectionHashTable>(*in_config.m_sections);
}

const Jupiter::ReadableString &Jupiter::Config::get(const Jupiter::ReadableString &in_key, const Jupiter::ReadableString &in_default_value) const
{
	auto value = m_table.find(in_key);
	if (value != m_table.end()) {
		return value->second;
	}

	return in_default_value;
}

Jupiter::Config *Jupiter::Config::getSection(const Jupiter::ReadableString &in_key) const
{
	if (m_sections != nullptr) {
		auto section = m_sections->find(in_key);
		if (section != m_sections->end()) {
			return &section->second;
		}
	}

	return nullptr;
}

Jupiter::Config &Jupiter::Config::getSectionReference(const Jupiter::ReadableString &in_key)
{
	if (m_sections == nullptr) {
		m_sections = std::make_unique<SectionHashTable>();
	}

	Config& section = (*m_sections)[in_key];

	if (section.m_name.empty()) {
		section.m_name = static_cast<std::string>(in_key);
	}

	return section;
}

bool Jupiter::Config::set(const Jupiter::ReadableString &in_key, const Jupiter::ReadableString &in_value)
{
	return m_table.insert_or_assign(in_key, in_value).second;
}

bool Jupiter::Config::remove(const Jupiter::ReadableString &in_key)
{
	return m_table.erase(in_key) > 0;
}

const std::string &Jupiter::Config::getName() const
{
	return m_name;
}

void Jupiter::Config::erase()
{
	m_table.clear();
	m_sections = nullptr;
}

bool Jupiter::Config::read(const char *in_filename)
{
	m_name = in_filename;
	return this->read_internal(in_filename);
}

bool Jupiter::Config::read(const Jupiter::ReadableString &in_filename)
{
	m_name = static_cast<std::string>(in_filename);
	return this->read_internal(m_name.c_str());
}

bool Jupiter::Config::write()
{
	return this->write(m_name.c_str());
}

bool Jupiter::Config::write(const char *in_filename)
{
	return this->write_internal(in_filename);
}

bool Jupiter::Config::write(const Jupiter::ReadableString &in_filename)
{
	return this->write(static_cast<std::string>(in_filename).c_str());
}

bool Jupiter::Config::reload()
{
	this->erase();
	return this->read_internal(m_name.c_str());
}

bool Jupiter::Config::reload(const char *in_filename)
{
	this->erase();
	return this->read(in_filename);
}

bool Jupiter::Config::reload(const Jupiter::ReadableString &in_filename)
{
	this->erase();
	return this->read(in_filename);
}

const Jupiter::Config::SectionHashTable &Jupiter::Config::getSections() const {
	if (m_sections == nullptr) {
		const_cast<Config*>(this)->m_sections = std::make_unique<SectionHashTable>();
	}

	return *m_sections;
}

/** Operators */
Jupiter::Config &Jupiter::Config::operator[](const Jupiter::ReadableString &in_key)
{
	return this->getSectionReference(in_key);
}

/** Private functions */

bool Jupiter::Config::read_internal(const char *)
{
	return false;
}

bool Jupiter::Config::write_internal(const char *)
{
	return false;
}

void Jupiter::Config::Buffer::set_length(size_t in_length)
{
	this->length = in_length;
}

char *Jupiter::Config::Buffer::get_str() const
{
	return this->str;
}
