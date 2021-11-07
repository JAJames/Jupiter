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

#if !defined _FILE_H_HEADER
#define _FILE_H_HEADER

/**
 * @file File.h
 * @brief Provides a fast interface for accessing stored file data.
 */

#include "Jupiter.h"
#include "Readable_String.h"
#include <cstdio>

namespace Jupiter
{

	class JUPITER_API File
	{
	public:

		/**
		* @brief Fetches the number of lines in the file.
		*
		* @return Total number of lines.
		*/
		size_t getLineCount() const;

		/**
		* @brief Fetches a line of the file.
		*
		* @param line Index of the line to fetch.
		* @return Line of text at the specified index.
		*/
		const Jupiter::ReadableString &getLine(size_t line) const;

		/**
		* @brief Returns the name of the first raw file originally loaded.
		*
		* @return String containing the name of the first file loaded into this file.
		*/
		const std::string &getFileName() const;

		/**
		* @brief Adds data to a file, which may consist of one or more lines.
		*
		* @param data Data to add to the file.
		* @param True if data was added to the file, false otherwise.
		*/
		bool addData(const Jupiter::ReadableString &data);

		/**
		* @brief Loads a file from the drive into this file.
		*
		* @param file name of the file to load from drive.
		* @return True if a file was successfully loaded from the drive, false otherwise.
		*/
		bool load(const char *file);
		bool load(const Jupiter::ReadableString &file);

		/**
		* @brief Loads a file from the drive into this file.
		*
		* @param file C FILE stream to read from. This must not be an endless stream (such as stdin).
		* @return True if a file was successfully loaded from the drive, false otherwise.
		*/
		bool load(FILE *file);

		/**
		* @brief Frees all of the stored data of the file, and puts it in a new state, ready for loading.
		*/
		void unload();

		/**
		* @brief Unloads all of a file's contents, and attempts to load from the originally loaded file.
		*
		* @return True if a file was successfully loaded from the drive, false otherwise.
		*/
		bool reload();

		/**
		* @brief Unloads all of a file's contents, and attempts to load from a specified file.
		*
		* @param file Name of a file to load from the drive.
		* @return True if a file was successfully loaded from the drive, false otherwise.
		*/
		bool reload(const char *file);
		bool reload(const Jupiter::ReadableString &file);

		/**
		* @brief Unloads all of a file's contents, and attempts to load from a specified file stream.
		*
		* @param file C FILE stream to read from. This must not be an endless stream (such as stdin).
		* @return True if a file was successfully loaded from the drive, false otherwise.
		*/
		bool reload(FILE *file);

		/**
		* @brief Syncs data from the file to the drive. This will write to the first loaded file.
		*
		* @return True if the file was successfully written to the drive, false otherwise.
		*/
		bool sync();

		/**
		* @brief Syncs data from the file to the drive.
		*
		* @param file Name of the file to write to.
		* @return True if the file was successfully written to the drive, false otherwise.
		*/
		bool sync(const char *file);
		bool sync(const Jupiter::ReadableString &file);

		/**
		* @brief Syncs data from the file to the drive.
		*
		* @param file C FILE stream to output to.
		* @return True if the file was successfully written to the drive, false otherwise.
		*/
		bool sync(FILE *file);

		/**
		* @brief Default constructor for File class.
		*/
		File();

		/**
		* @brief Move constructor for File class.
		*/
		File(File &&file);

		/**
		* @brief Copy constructor for File class.
		*/
		File(const File &file);

		/**
		* @brief Destructor for File class.
		*/
		virtual ~File();

	/** Private members */
	private:
		struct Data;
		Data *m_data;
	};

}

#endif // _FILE_H_HEADER