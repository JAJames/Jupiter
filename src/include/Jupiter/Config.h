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
#include "jessilib/unicode_compare.hpp"
#include "Jupiter.h"
#include "Readable_String.h" // from_string

/** DLL Linkage Nagging */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace Jupiter
{
#ifdef __cpp_lib_generic_unordered_lookup
	using InMapKeyType = std::string_view;
#define JUPITER_WRAP_MAP_KEY(in_key) in_key
#else // We can't use std::string_view for InKeyType until GCC 11 & clang 12, and I still want to support GCC 9
	using InMapKeyType = std::string;
#define JUPITER_WRAP_MAP_KEY(in_key) static_cast<Jupiter::InMapKeyType>(in_key)
#endif // __cpp_lib_generic_unordered_lookup

	/**
	* @brief Base class for all Config type files
	*/
	class JUPITER_API Config
	{
	public:
		/** Hash_Table type for sections */
		using SectionHashTable = std::unordered_map<std::string, Config, jessilib::text_hashi, jessilib::text_equali>;
		using ValuesHashTable = std::unordered_map<std::string, std::string, jessilib::text_hashi, jessilib::text_equali>;
		using InKeyType = InMapKeyType;

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
		std::string_view get(std::string_view in_key, std::string_view in_default_value = std::string_view{}) const;

		/**
		* @brief Fetches the value of an entry and interprets it as another type.
		*
		* @param T Type to interpret the value as
		*
		* @param in_key Key of the entry to fetch
		* @param in_default_value Value to return if no such entry exists
		* @return Value of the entry if it exists, 0 otherwise.
		*/
		template<typename T> T get(std::string_view in_key, T in_default_value = 0) const;

		/**
		* @brief Fetches a section based on its name
		*
		* @param in_key Name of the section to fetch
		* @return Pointer to a section if it exists, nullptr otherwise
		*/
		Config *getSection(std::string_view in_key) const;

		/**
		* @brief Fetches a section based on its name
		* Note: This will create new sections as necessary if they do not exist
		*
		* @param in_key Name of the section to fetch
		* @return Reference to the section
		*/
		Config &getSectionReference(std::string_view in_key);

		/**
		* @brief Sets an entry's value in the table
		*
		* @param in_key Key of the entry to write to
		* @param in_value Value to write to the entry
		* @return True if a new entry was added, false if an entry was overwritten
		*/
		bool set(std::string_view in_key, std::string in_value);

		/**
		* @brief Removes an entry from the table
		*
		* @param in_key Key of the entry to remove
		* @return True if an entry was removed, false otherwise
		*/
		bool remove(std::string_view in_key);

		/**
		* @brief Removes a section from the table
		*
		* @param in_key Key of the section to remove
		* @return True if an entry was removed, false otherwise
		*/
		bool removeSection(std::string_view in_key);

		/**
		* @brief Fetches the name of this config section
		* Note: This is the filename on the root node
		*
		* @return Name of this section
		*/
		const std::string& getName() const;

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
		bool read(std::string_view in_filename);

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
		bool write(std::string_view in_filename);

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
		bool reload(std::string_view in_filename);

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
		Config &operator[](std::string_view in_key);

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

template<typename T>
inline T Jupiter::Config::get(std::string_view in_key, T in_default_value) const {
	auto result = m_table.find(JUPITER_WRAP_MAP_KEY(in_key));

	if (result == m_table.end()) {
		return in_default_value;
	}

	return from_string<T>(result->second);
}

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // JUPITER_CONFIG_H_HEADER