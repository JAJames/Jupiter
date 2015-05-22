/**
 * Copyright (C) 2015 Justin James.
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

#if !defined _DATABASE_H_HEADER
#define _DATABASE_H_HEADER

/**
 * @file Database.h
 * @brief Defines a database file structure.
 */

#include "CString.h"

namespace Jupiter
{

	/**
	* @brief Provides an implementation for database files.
	*
	* @param T Element type which the String will store. Defaults to char.
	*/
	class JUPITER_API Database
	{
	public:
		/**
		* @brief Processes a chunk of data in a database.
		*
		* @param buffer Buffer to process
		* @param file File being processed
		* @param pos position that the buffer starts at in the file
		*/
		virtual void process_data(Jupiter::DataBuffer &buffer, FILE *file, fpos_t pos) = 0;

		/**
		* @brief Processes the header for a database.
		*
		* @param file File being processed
		*/
		virtual void process_header(FILE *file);

		/**
		* @brief Generates a header for a database.
		*
		* @param file File being created
		*/
		virtual void create_header(FILE *file);

		/**
		* @brief Processes a database file. This function calls process_data().
		*
		* @param file File being processed
		* @return True on success, false otherwise
		*/
		bool process_file(Jupiter::ReadableString &file);
		bool process_file(Jupiter::CStringType &file);
		bool process_file(const char *file);
		bool process_file(FILE *file);

		/**
		* @brief Called when process_file() is successfully completed.
		*
		* @param file File being processed
		*/
		virtual void process_file_finish(FILE *file);

		/**
		* @brief Appends a DataBuffer to the end of the Database.
		*
		* @param data DataBuffer to append
		*/
		bool append(Jupiter::DataBuffer &data);

		/**
		* @brief Appends a DataBuffer to the end of a Database file.
		*
		* @param file File to append the DataBuffer to
		* @param data DataBuffer to append
		* @return True on success, false otherwise.
		*/
		static bool append(Jupiter::ReadableString &file, Jupiter::DataBuffer &data);
		static bool append(Jupiter::CStringType &file, Jupiter::DataBuffer &data);
		static bool append(const char *file, Jupiter::DataBuffer &data);
		static bool append(FILE *file, Jupiter::DataBuffer &data);

		/**
		* @brief Generates a new database, consisting of only a header
		* Note: This writes the data *in* the DataBuffer to the file, *not* the DataBuffer itself.
		*
		* @param flie Name of the Database to generate
		* @param header DataBuffer containing the header to write to the file
		*/
		static bool create_database(Jupiter::ReadableString &file, Jupiter::DataBuffer *header = nullptr);
		static bool create_database(Jupiter::CStringType &file, Jupiter::DataBuffer *header = nullptr);
		static bool create_database(const char *file, Jupiter::DataBuffer *header = nullptr);

		/**
		* @brief When True, process_file() will automatically call create_header() if so such file exists.
		* Default: true
		*
		* @return True if databases should automatically be generated, false otherwise.
		*/
		bool is_auto_create();

		/**
		* @brief Sets if databases should be generated automatically in process_file().
		*
		* @param auto_create True if databases should be automatically generated, false otherwise.
		*/
		void set_auto_create(bool auto_create);

	private:
		struct Data;
		Data *data_;
	};
}

#endif // _DATABASE_H_HEADER