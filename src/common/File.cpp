/**
 * Copyright (C) 2014-2021 Jessica James.
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
#include <string>
#include "File.h"
#include "String.hpp"

#if defined _WIN32
#define stat64 _stat64
#endif

int64_t getFileSize(const char *file) {
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

struct JUPITER_API Jupiter::File::Data { // TODO: remove pimpl
	std::string fileName;
	std::vector<Jupiter::StringS> lines;

	Data();
	Data(const Data &data);
	~Data();
};

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

Jupiter::File::Data::Data() {
}

Jupiter::File::Data::Data(const Jupiter::File::Data &data) {
	fileName = data.fileName;

	// TODO: fix or remove this
	//for (size_t i = 0; i != lines.size(); i++) // lines.size() will always be 0 here; this code does nothing?
	//	lines.emplace_back(data.lines[i]);
}

Jupiter::File::Data::~Data() {
}

Jupiter::File::File() {
	m_data = new Data();
}

Jupiter::File::File(File &&file) {
	m_data = file.m_data;
	file.m_data = new Data();
}

Jupiter::File::File(const File &file) {
	m_data = new Data(*file.m_data);
}

Jupiter::File::~File() {
	delete m_data; // TODO: remove raw delete
}

size_t Jupiter::File::getLineCount() const {
	return m_data->lines.size();
}

const Jupiter::ReadableString &Jupiter::File::getLine(size_t line) const {
	return m_data->lines[line];
}

const std::string &Jupiter::File::getFileName() const {
	return m_data->fileName;
}

bool Jupiter::File::addData(const Jupiter::ReadableString &data) {
	unsigned int word_count = data.wordCount(ENDL);
	if (word_count == 0) return false;

	for (unsigned int i = 0; i < word_count; i++) {
		m_data->lines.emplace_back(std::move(Jupiter::StringS::getWord(data, i, ENDL)));
	}
	return true;
}

bool Jupiter::File::load(const char *file) {
	FILE *filePtr = fopen(file, "rb");
	if (filePtr == nullptr) {
		return false;
	}

	if (m_data->fileName.empty()) {
		m_data->fileName = file;
	}

	bool result = load(filePtr);
	fclose(filePtr);
	return result;
}

bool Jupiter::File::load(const Jupiter::ReadableString &file) {
	std::string fileName = static_cast<std::string>(file);
	FILE *filePtr = fopen(fileName.c_str(), "rb");
	if (filePtr == nullptr) {
		return false;
	}

	if (m_data->fileName.empty()) {
		m_data->fileName = fileName;
	}

	bool r = load(filePtr);
	fclose(filePtr);
	return r;
}

bool Jupiter::File::load(FILE *file) {
	Jupiter::String buffer(defaultBufferSize);

	int chr;

	while (true) {
		chr = fgetc(file);

		if (chr == EOF) {
			if (buffer.isNotEmpty()) {
				m_data->lines.emplace_back(buffer);
				return true;
			}

			return !m_data->lines.empty();
		}

		// TODO: refactor this nonsense
		if (static_cast<char>(chr) == '\r') { // new line
		new_line_r:

			m_data->lines.emplace_back(buffer);
			buffer.erase();

			// check for optional trailing \n

			chr = fgetc(file);

			switch (chr) {
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

		if (static_cast<char>(chr) == '\n') { // new line
		new_line_n:

			m_data->lines.emplace_back(buffer);
			buffer.erase();

			// check for optional trailing \r

			chr = fgetc(file);

			switch (chr) {
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

void Jupiter::File::unload() {
	m_data->fileName.clear();
	m_data->lines.clear();
}

bool Jupiter::File::reload() {
	if (m_data->fileName.empty()) {
		return false;
	}

	std::string fileName(std::move(m_data->fileName));
	unload();
	return load(fileName.c_str());
}

bool Jupiter::File::reload(const char *file) {
	unload();
	return load(file);
}

bool Jupiter::File::reload(const Jupiter::ReadableString &file) {
	unload();
	return load(file);
}

bool Jupiter::File::reload(FILE *file) {
	unload();
	return load(file);
}

bool Jupiter::File::sync() {
	if (m_data->fileName.empty()) {
		return false;
	}

	return sync(m_data->fileName.c_str());
}

bool Jupiter::File::sync(const char *file) {
	FILE *filePtr = fopen(file, "wb");
	if (filePtr == nullptr) return false;
	sync(filePtr); // Always returns true.
	fclose(filePtr);
	return true;
}

bool Jupiter::File::sync(const Jupiter::ReadableString &file) {
	return sync(static_cast<std::string>(file).c_str());
}

bool Jupiter::File::sync(FILE *file) {
	for (const auto& line : m_data->lines) {
		line.println(file);
	}

	return true;
}