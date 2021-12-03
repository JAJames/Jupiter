/**
 * Copyright (C) 2015 Jessica James.
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

#if !defined _DATABASE_H_HEADER
#define _DATABASE_H_HEADER

/**
 * @file Database.h
 * @brief Defines a database file structure.
 */

#include <cstdio>
#include <string>
#include <string_view>
#include "Jupiter.h"

namespace Jupiter
{
	class DataBuffer;

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
		bool process_file(std::string_view file);
		bool process_file(const std::string &file);
		bool process_file(const char *file);
		bool process_file(FILE *file);

		/**
		* @brief Called when process_file() is successfully completed.
		*
		* @param file File being processed
		*/
		virtual void process_file_finish(FILE *file);

		/**
		* @brief Returns the name of the file which was loaded in process_file.
		*
		* @return Name of the file on the file system.
		*/
		const std::string &getFilename() const;

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
		static bool append(std::string_view file, Jupiter::DataBuffer &data);
		static bool append(const std::string& file, Jupiter::DataBuffer &data);
		static bool append(const char *file, Jupiter::DataBuffer &data);
		static bool append(FILE *file, Jupiter::DataBuffer &data);

		/**
		* @brief Generates a new database, consisting of only a header
		* Note: This writes the data *in* the DataBuffer to the file, *not* the DataBuffer itself.
		*
		* @param flie Name of the Database to generate
		* @param header DataBuffer containing the header to write to the file
		*/
		static bool create_database(std::string_view file, const Jupiter::DataBuffer *header = nullptr);
		static bool create_database(const std::string &file, const Jupiter::DataBuffer *header = nullptr);
		static bool create_database(const char *file, const Jupiter::DataBuffer *header = nullptr);

		/**
		* @brief When True, process_file() will automatically call create_header() if no such file exists.
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

		/**
		* @brief Default constructor for the Database class.
		*/
		Database();

		/**
		* @brief Copy constructor for the Database class.
		*/
		Database(const Database &) = delete;

		/**
		* @brief Destructor for the Database class.
		*/
		~Database();

	private:
		struct Data;
		Data *data_;
	};
}

#endif // _DATABASE_H_HEADER