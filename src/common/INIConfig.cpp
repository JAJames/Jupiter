/**
 * Copyright (C) 2016-2017 Jessica James.
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

#include <stack>
#include "INIConfig.h"
#include "Socket.h"

void Jupiter::INIConfig::write_helper(FILE *in_file, const Jupiter::Config *in_section, size_t in_depth)
{
	size_t index;

	if (in_depth != 0)
	{
		// Write header

		fputs("\r\n", in_file);

		// Tabs
		for (index = 1; index < in_depth; ++index)
			fputc('\t', in_file);

		for (index = 0; index != in_depth; ++index)
			fputc('[', in_file);

		fputs(in_section->getName().c_str(), in_file);

		for (index = 0; index != in_depth; ++index)
			fputc(']', in_file);

		fputs("\r\n", in_file);
	}

	// Write table entries
	for (auto& table_entry : in_section->getTable())
	{
		// Tabs
		for (index = 1; index < in_depth; ++index)
			fputc('\t', in_file);

		// Write entry
		fwrite(table_entry.first.data(), sizeof(char), table_entry.first.size(), in_file);
		fputs(" = ", in_file);
		fwrite(table_entry.second.data(), sizeof(char), table_entry.second.size(), in_file);
		fputs("\r\n", in_file);
	}

	// Write subsections
	for (auto& subsection : in_section->getSections()) {
		write_helper(in_file, &subsection.second, in_depth + 1);
	}
}

bool Jupiter::INIConfig::write_internal(const char *in_filename)
{
	// Open file
	FILE *file = fopen(in_filename, "wb");

	if (file == nullptr)
		return false;

	// Iterate through table and sections
	write_helper(file, this, 0);

	// File writing completed; close and return
	fclose(file);
	return true;
}

bool Jupiter::INIConfig::read_internal(const char *in_filename) {
	std::stack<Jupiter::Config *> section_stack;
	section_stack.push(this);

	auto process_line = [&section_stack](std::string_view line) {
		const char *itr = line.data();
		const char *end = itr + line.size(); // guaranteed to be greater than itr

		// Shift to right of spaces

		while (isspace(*itr))
			if (++itr == end)
				return; // Line is purely whitespace

		line.remove_prefix(itr - line.data());

		if (*itr == ';')
			return; // Comment

		if (*itr == '[')
		{
			// Parse header

			size_t depth = 1;

			for (++itr; itr != end && *itr == '['; ++itr)
				++depth;

			line.remove_prefix(itr - line.data());

			while (end != itr)
			{
				--end;
				if (*end != ']' && isspace(*end) == 0)
				{
					++end;
					break;
				}
			}

			line = std::string_view(itr, end - itr);

			// Add section to stack; pop sections or push blanks as necessary

			while (depth < section_stack.size())
				section_stack.pop();

			while (depth > section_stack.size())
				section_stack.push(std::addressof(section_stack.top()->getSectionReference({})));

			section_stack.push(&section_stack.top()->getSectionReference(line));
		}
		else
		{
			// Truncate spaces
			--end;

			while (isspace(static_cast<unsigned char>(*end)))
				--end; // don't need a safety check since we know there is at least 1 non-space character
			// end now points to a non-space character within the bounds

			++end;
			line.remove_suffix(itr + line.size() - end);

			// Parse key (can be empty)

			while (*itr != '=')
				if (++itr == end)
					return; // Error: no assignment exists; ignore line

			std::string_view key;

			if (itr != line.data())
			{
				// Truncate spaces from key; a non-space character is guaranteed
				end = itr - 1;

				while (isspace(*end))
					--end;

				key = line.substr(size_t{ 0 }, end + 1 - line.data());

				end = line.data() + line.size();
			}

			// Parse value (can be empty)

			if (++itr != end)
			{
				// Shift to right of spaces; a non-space character is guaranteed
				while (isspace(*itr))
					++itr;

				line.remove_prefix(itr - line.data());
			}
			else
				line = std::string_view{};

			// Add entry to current table on stack
			section_stack.top()->set(std::string{key}, static_cast<std::string>(line));
		}
	};

	constexpr size_t READ_CHUNK_SIZE = 1024;
	size_t read_count = READ_CHUNK_SIZE;
	char* line_head;
	char* read_head;
	char* read_end;

	// Open file
	FILE *file = fopen(in_filename, "rb");

	if (file == nullptr)
		return false;

	// Parse file contents
	char buffer[READ_CHUNK_SIZE];
	std::string line;
	do {
		// Read data from file to end of buffer
		read_head = buffer;
		read_count = fread(buffer, sizeof(char), READ_CHUNK_SIZE, file);
		read_end = read_head + read_count;
		line_head = nullptr;

		// Parse buffer for lines
		while (read_head != read_end) {
			// Check if the line is over
			if (*read_head == '\n' || *read_head == '\r') {
				if (line_head == nullptr) {
					// There's no line_head; append what we have read to line and process that
					line.append(buffer, read_head);
					process_line(line);
					line.clear();
				}
				else {
					// There's not any pending data; process a view from the line_head to here
					process_line({ line_head, static_cast<size_t>(read_head - line_head) });
				}

				// Keep iterating until next non-newline character
				for (++read_head; read_head != read_end; ++read_head) {
					if (*read_head != '\n' && *read_head != '\r') {
						// Shift buffer and break
						line_head = read_head;
						break;
					}
				}
			}
			else {
				++read_head;
			}
		}

		// We've processed the read buffer; push any pending line data to the line buffer
		if (line_head == nullptr) {
			// No newlines in the entire buffer; should be rare
			line_head = buffer;
		}
		line.append(line_head, read_head);
	} while (read_count == READ_CHUNK_SIZE);

	// Process data remaining in buffer as a line
	if (!line.empty()) {
		process_line(line);
	}

	// File has been successfully read, or an error occurred.
	fclose(file);
	return true;
}
