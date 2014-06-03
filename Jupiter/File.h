/**
 * Copyright (C) 2014 Justin James.
 *
 * This license must be preserved.
 * Any applications, libraries, or code which make any use of any
 * component of this program must not be commercial, unless explicit
 * permission is granted from the original author. The use of this
 * program for non-profit purposes is permitted.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In the event that this license restricts you from making desired use of this program, contact the original author.
 * Written by Justin James <justin.aj@hotmail.com>
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
		const Jupiter::ReadableString &getFileName() const;

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
		Data *data_;
	};

}

#endif // _FILE_H_HEADER