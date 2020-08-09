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

#if !defined JUPITER_CONFIG_H_HEADER
#define JUPITER_CONFIG_H_HEADER

/**
 * @file Config.h
 * @brief Defines the Config class, which provides common functionality for Config files in differing formats.
 */

#include <unordered_map>
#include <memory>
#include "Jupiter.h"
#include "Hash.h"
#include "Reference_String.h"
#include "String.hpp"

/** DLL Linkage Nagging */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace Jupiter
{
	/**
	* @brief Base class for all Config type files
	*/
	class JUPITER_API Config
	{
	public:
		/** Hash_Table type for sections */
		using SectionHashTable = std::unordered_map<StringS, Config, default_hash_function>;
		using ValuesHashTable = std::unordered_map<StringS, StringS, default_hash_function>;

		Config() = default;
		Config(const Config& in_config);
		Config(Config&& in_config) = default;
		Config& operator=(const Config& in_config);
		Config& operator=(Config&& in_config) = default;

		/**
		* @brief Fetches the value of an entry.
		*
		* @param in_key Key of the entry to fetch
		* @param in_default_value Value to return if no such entry exists
		* @return Value of the entry if it exists, an empty string otherwise.
		*/
		const Jupiter::ReadableString &get(const Jupiter::ReadableString &in_key, const Jupiter::ReadableString &in_default_value = Jupiter::ReferenceString::empty) const;

		/**
		* @brief Fetches the value of an entry and interprets it as another type.
		*
		* @param T Type to interpret the value as
		*
		* @param in_key Key of the entry to fetch
		* @param in_default_value Value to return if no such entry exists
		* @return Value of the entry if it exists, 0 otherwise.
		*/
		template<typename T> T get(const Jupiter::ReadableString &in_key, T in_default_value = 0) const;

		/**
		* @brief Fetches a section based on its name
		*
		* @param in_key Name of the section to fetch
		* @return Pointer to a section if it exists, nullptr otherwise
		*/
		Config *getSection(const Jupiter::ReadableString &in_key) const;

		/**
		* @brief Fetches a section based on its name
		* Note: This will create new sections as necessary if they do not exist
		*
		* @param in_key Name of the section to fetch
		* @return Reference to the section
		*/
		Config &getSectionReference(const Jupiter::ReadableString &in_key);

		/**
		* @brief Sets an entry's value in the table
		*
		* @param in_key Key of the entry to write to
		* @param in_value Value to write to the entry
		* @return True if a new entry was added, false if an entry was overwritten
		*/
		bool set(const Jupiter::ReadableString &in_key, const Jupiter::ReadableString &in_value);

		/**
		* @brief Removes an entry from the table
		*
		* @param in_key Key of the entry to remove
		* @return True if an entry was removed, false otherwise
		*/
		bool remove(const Jupiter::ReadableString &in_key);

		/**
		* @brief Removes a section from the table
		*
		* @param in_key Key of the section to remove
		* @return True if an entry was removed, false otherwise
		*/
		bool removeSection(const Jupiter::ReadableString &in_key);

		/**
		* @brief Fetches the name of this config section
		* Note: This is the filename on the root node
		*
		* @return Name of this section
		*/
		const std::string &getName() const;

		/**
		* @brief Erases all data from this section
		*/
		void erase();

		/**
		* @brief Reads config data from a file and stores it in this config
		*
		* @param in_filename Name of the file to read from
		* @return True on success, false otherwise
		*/
		bool read(const char *in_filename);

		/**
		* @brief Reads config data from a file and stores it in this config
		*
		* @param in_filename Name of the file to read from
		* @return True on success, false otherwise
		*/
		bool read(const Jupiter::ReadableString &in_filename);

		/**
		* @brief Writes config data to the last read file
		*
		* @return True on success, false otherwise
		*/
		bool write();

		/**
		* @brief Writes config data to a file
		*
		* @param in_filename Name of the file to write to
		* @return True on success, false otherwise
		*/
		bool write(const char *in_filename);

		/**
		* @brief Writes config data to a file
		*
		* @param in_filename Name of the file to write to
		* @return True on success, false otherwise
		*/
		bool write(const Jupiter::ReadableString &in_filename);

		/**
		* @brief Empties config data from memory and reads from the last read file
		*
		* @return True on success, false otherwise
		*/
		bool reload();

		/**
		* @brief Empties config data from memory and reads from a file
		*
		* @param in_filename Name of the file to read from
		* @return True on success, false otherwise
		*/
		bool reload(const char *in_filename);

		/**
		* @brief Empties config data from memory and reads from a file
		*
		* @param in_filename Name of the file to read from
		* @return True on success, false otherwise
		*/
		bool reload(const Jupiter::ReadableString &in_filename);

		/**
		* @brief Fetches a reference to this config's entry table
		*
		* @return Reference to m_table
		*/
		inline const ValuesHashTable &getTable() const { return m_table; }

		/**
		* @brief Fetches a reference to this config's subsections
		*
		* @return Reference to m_sections
		*/
		const SectionHashTable &getSections() const;

		/** Subscript operator */
		Config &operator[](const Jupiter::ReadableString &in_key);

		/** Used for low-level string operations */
		class Buffer : public Jupiter::StringL
		{
		public:
			/**
			* @brief Sets the length of the string
			*
			* @param in_length Length of the string
			*/
			void set_length(size_t in_length);

			/**
			* @brief Returns a pointer to the underlying string
			*
			* @return Pointer to an array of characters
			*/
			char *get_str() const;
		};

	protected:
		virtual bool read_internal(const char *in_filename);
		virtual bool write_internal(const char *in_filename);

		/** Name of this Config section. This is empty or the filename at the root level. */
		std::string m_name;

		/** Table of entries within this section */
		ValuesHashTable m_table;

		/** Table of sections within this section */
		std::unique_ptr<SectionHashTable> m_sections;
	};
}

/** Template function implementations */

template<typename T> inline T Jupiter::Config::get(const Jupiter::ReadableString &in_key, T in_default_value) const
{
	auto result = m_table.find(in_key);

	if (result == m_table.end())
		return in_default_value;

	return static_cast<T>(result->second);
}

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // JUPITER_CONFIG_H_HEADER