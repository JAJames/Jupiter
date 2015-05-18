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
		virtual void process_data(Jupiter::DataBuffer &buffer, FILE *file, fpos_t pos) = 0;
		virtual void process_header(FILE *file);
		virtual void create_header(FILE *file);

		bool process_file(Jupiter::ReadableString &file);
		bool process_file(Jupiter::CStringType &file);
		bool process_file(const char *file);
		bool process_file(FILE *file);
		virtual void process_file_finish(FILE *file);

		bool append(Jupiter::DataBuffer &data);

		static bool append(Jupiter::ReadableString &file, Jupiter::DataBuffer &data);
		static bool append(Jupiter::CStringType &file, Jupiter::DataBuffer &data);
		static bool append(const char *file, Jupiter::DataBuffer &data);
		static bool append(FILE *file, Jupiter::DataBuffer &data);

		static bool create_database(Jupiter::ReadableString &file, Jupiter::DataBuffer *header = nullptr);
		static bool create_database(Jupiter::CStringType &file, Jupiter::DataBuffer *header = nullptr);
		static bool create_database(const char *file, Jupiter::DataBuffer *header = nullptr);

		bool is_auto_create();
		void set_auto_create(bool auto_create);
	private:
		bool process_file();
		static bool create_database(FILE *file, Jupiter::DataBuffer *header);

		bool auto_create = true;
		Jupiter::CStringS file_name;
	};
}

#endif // _DATABASE_H_HEADER