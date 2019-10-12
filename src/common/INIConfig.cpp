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
		table_entry.first.print(in_file);
		fputs(" = ", in_file);
		table_entry.second.println(in_file);
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

bool Jupiter::INIConfig::read_internal(const char *in_filename)
{
	Jupiter::ReferenceString line;
	std::stack<Jupiter::Config *> section_stack;

	section_stack.push(this);

	auto process_line = [&line, &section_stack]()
	{
		const char *itr = line.ptr();
		const char *end = itr + line.size(); // guaranteed to be greater than itr

		// Shift to right of spaces

		while (isspace(*itr))
			if (++itr == end)
				return; // Line is purely whitespace

		line.shiftRight(itr - line.ptr());

		if (*itr == ';')
			return; // Comment

		if (*itr == '[')
		{
			// Parse header

			size_t depth = 1;

			for (++itr; itr != end && *itr == '['; ++itr)
				++depth;

			line.shiftRight(itr - line.ptr());

			while (end != itr)
			{
				--end;
				if (*end != ']' && isspace(*end) == 0)
				{
					++end;
					break;
				}
			}

			line.set(itr, end - itr);

			// Add section to stack; pop sections or push blanks as necessary

			while (depth < section_stack.size())
				section_stack.pop();

			while (depth > section_stack.size())
				section_stack.push(std::addressof(section_stack.top()->getSectionReference(Jupiter::ReferenceString::empty)));

			section_stack.push(&section_stack.top()->getSectionReference(line));
		}
		else
		{
			// Truncate spaces
			--end;

			while (isspace(*end))
				--end; // don't need a safety check since we know there is at least 1 non-space character
			// end now points to a non-space character within the bounds

			++end;
			line.truncate(itr + line.size() - end);

			// Parse key (can be empty)

			while (*itr != '=')
				if (++itr == end)
					return; // Error: no assignment exists; ignore line

			Jupiter::ReferenceString key;

			if (itr != line.ptr())
			{
				// Truncate spaces from key; a non-space character is guaranteed
				end = itr - 1;

				while (isspace(*end))
					--end;

				key = line.substring(size_t{ 0 }, end + 1 - line.ptr());

				end = line.ptr() + line.size();
			}

			// Parse value (can be empty)

			if (++itr != end)
			{
				// Shift to right of spaces; a non-space character is guaranteed
				while (isspace(*itr))
					++itr;

				line.shiftRight(itr - line.ptr());
			}
			else
				line = Jupiter::ReferenceString::empty;

			// Add entry to current table on stack
			section_stack.top()->set(key, line);
		}
	};

	constexpr size_t READ_CHUNK_SIZE = 1024;
	size_t read_count = READ_CHUNK_SIZE;
	Buffer buffer;
	char *itr;
	char *end;

	// Open file
	FILE *file = fopen(in_filename, "rb");

	if (file == nullptr)
		return false;

	buffer.setBufferSize(READ_CHUNK_SIZE * 2);

	// Parse file contents
	while (read_count == READ_CHUNK_SIZE)
	{
		// Ensure the buffer has at least READ_CHUNK_SIZE space remaining
		buffer.setBufferSize(buffer.size() + READ_CHUNK_SIZE);

		// Read data from file to end of buffer
		itr = buffer.get_str();
		read_count = fread(itr + buffer.size(), sizeof(char), READ_CHUNK_SIZE, file);
		buffer.set_length(buffer.size() + read_count);
		end = itr + buffer.size();

		// Reset line
		line.set(buffer.ptr(), 0);

		// Parse buffer for lines
		while (itr != end)
		{
			// Check if the line is over
			if (*itr == '\n' || *itr == '\r')
			{
				// Process line
				line.set(buffer.ptr(), itr - buffer.ptr());
				if (line.isNotEmpty())
					process_line();

				// Keep iterating until next non-newline character
				while (true)
				{
					++itr;

					if (itr == end)
					{
						// No data remains to be parsed in buffer; erase buffer and break
						buffer.erase();
						break;
					}

					if (*itr != '\n' && *itr != '\r')
					{
						// Shift buffer and break
						buffer.shiftRight(itr - buffer.ptr());
						break;
					}
				}
			}
			else
				++itr;
		}
	}

	// Process data remaining in buffer as a line
	line.set(buffer.ptr(), buffer.size());
	if (line.isNotEmpty())
		process_line();

	// File has been successfully read, or an error occurred.
	fclose(file);
	return true;
}
