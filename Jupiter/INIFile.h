/**
 * Copyright (C) 2013-2015 Jessica James.
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

#if !defined _INIFILE_H_HEADER
#define _INIFILE_H_HEADER

/**
 * @file INIFile.h
 * @brief Provides interaction with INI files.
 */

#include "Jupiter.h"
#include "Reference_String.h"
#include "InvalidIndex.h"

namespace Jupiter
{
	/**
	* @brief Provides interfacing with INI files.
	* INIFile objects store the contents of .INI files into memory,
	* and access them as neccessary. The INIFile object can store
	* the contents of multiple files at a time, but does not store
	* where data came from. As such, you can read from multiple files,
	* however the contents will always go to a single file when synced.
	* Disk contents are only modified when sync() is called.
	*/
	class JUPITER_API INIFile
	{
	public:

		class JUPITER_API Section
		{
		public:
			class JUPITER_API KeyValuePair
			{
				friend class Jupiter::INIFile::Section;
			public:

				/**
				* @brief Fetches the checksum of the key in a key-value pair.
				*
				* @return Checksum of the key.
				*/
				unsigned int getKeyChecksum() const;

				/**
				* @brief Fetches the key in the key-value pair.
				*
				* @return Reference to the key String in the pair.
				*/
				const Jupiter::ReadableString &getKey() const;

				/**
				* @brief Fetches the value in the key-value pair.
				*
				* @return Reference to the value String in the pair.
				*/
				const Jupiter::ReadableString &getValue() const;

				/**
				* @brief Sets the value of a key-value pair.
				*
				* @param value Value to set the key-value pair to.
				*/
				void setValue(const Jupiter::ReadableString &value);

				/**
				* @brief Default constructor for the KeyValuePair class.
				*/
				KeyValuePair();

				/**
				* @brief Copy constructor for the KeyValuePair class.
				*/
				KeyValuePair(const KeyValuePair &);

				/**
				* @brief Destructor for the KeyValuePair class.
				*/
				~KeyValuePair();

			private:
				struct Data;
				Data *data_;
			};

			/**
			* @brief Fetches the checksum of the name of a section.
			*
			* @return Checmsum of the name.
			*/
			unsigned int getNameChecksum() const;

			/**
			* @brief Fetches the name of a section.
			*
			* @return Name of the section in a String.
			*/
			const Jupiter::ReadableString &getName() const;

			/**
			* @brief Fetches the value of a key-value pair at a specified index.
			*
			* @param index Index of the key-value pair.
			* @return Value of a key-value pair at a specified index.
			*/
			const Jupiter::ReadableString &get(size_t index) const;

			/**
			* @brief Fetches the value of a key-value pair.
			*
			* @param key Key of the key-value pair.
			* @return Value of a key-value pair, or an empty string if none is found.
			*/
			const Jupiter::ReadableString &get(const Jupiter::ReadableString &key, const Jupiter::ReadableString &defaultValue = Jupiter::ReferenceString::empty) const;

			/**
			* @brief Translates and returns the value of a key-value pair as a boolean.
			*
			* @param key String containing key name.
			* @param defaultValue Value to return if none is found.
			* @return true if key exists and is not "false" or "0", defaultValue otherwise.
			*/
			bool getBool(const Jupiter::ReadableString &key, bool defaultValue = false) const;

			/**
			* @brief Translates and returns the value of a key-value pair as a short.
			*
			* @param key String containing key name.
			* @param defaultValue Value to return if none is found.
			* @return short value of the key if it exits, defaultValue otherwise.
			*/
			short getShort(const Jupiter::ReadableString &key, short defaultValue = 0) const;

			/**
			* @brief Translates and returns the value of a key-value pair as an int.
			*
			* @param key String containing key name.
			* @param defaultValue Value to return if none is found.
			* @return int value of the key if it exits, defaultValue otherwise.
			*/
			int getInt(const Jupiter::ReadableString &key, int defaultValue = 0) const;

			/**
			* @brief Translates and returns the value of a key-value pair as a long.
			*
			* @param key String containing key name.
			* @param defaultValue Value to return if none is found.
			* @return long value of the key if it exits, defaultValue otherwise.
			*/
			long getLong(const Jupiter::ReadableString &key, long defaultValue = 0) const;

			/**
			* @brief Translates and returns the value of a key-value pair as a long long.
			*
			* @param key String containing key name.
			* @param defaultValue Value to return if none is found.
			* @return long long value of the key if it exits, defaultValue otherwise.
			*/
			long long getLongLong(const Jupiter::ReadableString &key, long long defaultValue = 0) const;

			/**
			* @brief Translates and returns the value of a key-value pair as a float.
			*
			* @param key String containing key name.
			* @param defaultValue Value to return if none is found.
			* @return float value of the key if it exits, defaultValue otherwise.
			*/
			float getFloat(const Jupiter::ReadableString &key, float defaultValue = 0) const;

			/**
			* @brief Translates and returns the value of a key-value pair as a double.
			*
			* @param key String containing key name.
			* @param defaultValue Value to return if none is found.
			* @return double value of the key if it exits, defaultValue otherwise.
			*/
			double getDouble(const Jupiter::ReadableString &key, double defaultValue = 0) const;

			/**
			* @brief Translates and returns the value of a key-value pair as a long double.
			*
			* @param key String containing key name.
			* @param defaultValue Value to return if none is found.
			* @return long double value of the key if it exits, defaultValue otherwise.
			*/
			long double getLongDouble(const Jupiter::ReadableString &key, long double defaultValue = 0) const;

			/**
			* @brief Fetches a key-value pair at a specified index.
			*
			* @param index Index of the key-value pair to fetch.
			* @return The key-value pair at the specified index.
			*/
			Jupiter::INIFile::Section::KeyValuePair *getPair(size_t index) const;

			/**
			* @brief Fetches a key-value pair.
			*
			* @param key Key of the key-value pair.
			* @return A key-value pair if a match is found, nullptr otherwise.
			*/
			Jupiter::INIFile::Section::KeyValuePair *getPair(const Jupiter::ReadableString &key) const;

			/**
			* @brief Fetches the number of key-value pairs in the section.
			*
			* @return Number of key-value pairs in the section.
			*/
			size_t size() const;

			/**
			* @brief Checks if a section has a key-value pair with a specified key.
			*
			* @param key Key of the key-value pair to search for.
			* @return True if a match is found, false otherwise.
			*/
			bool hasKey(const Jupiter::ReadableString &key) const;

			/**
			* @brief Sets a key-value pair's value to a specified value. Creates a new pair if none is found.
			*
			* @param key Key of the key-value pair to search for or create.
			* @param value Value to set the key-value pair to.
			* @return True if a new key-value pair was created, false otherwise.
			*/
			bool set(const Jupiter::ReadableString &key, const Jupiter::ReadableString &value);

			/**
			* @brief Sets the name of a section.
			*
			* @param name String to set the section name to.
			*/
			void setName(const Jupiter::ReadableString &name);

			/**
			* @brief Removes a key-value pair from the section.
			*
			* @param key Key of the key-value pair to remove.
			* @return True if a key-value pair was removed, false otherwise.
			*/
			bool remove(const Jupiter::ReadableString &key);

			/**
			* @brief Default constructor for the Section class.
			*/
			Section();

			/**
			* @brief Name-specifiable constructors for the Section class
			*/
			Section(const Jupiter::ReadableString &name);

			/**
			* @brief Copy constructor for the Section class.
			*/
			Section(const Section &);

			/**
			* @brief Destructor for the Section class.
			*/
			~Section();

		private:
			struct Data;
			Data *data_;
		};

		/**
		* @brief Flushes all stored data.
		*/
		void flushData();

		/**
		* @brief Reads data from a file.
		* The parser will automatically remove any excessive whitespace, including any whitespace
		* prefixing a key name, prefixing a key value, trailing a key name, or trailing a key value.
		* Whitespace is defined by locale. Please refer to C's isspace() documentation for details.
		*
		* @param fileName String containing the name of a file.
		* @return The number of key-value pairs added on success, ERROR_INDICATOR (-1) otherwise.
		*/
		unsigned int readFile(const char *fileName);
		unsigned int readFile(const Jupiter::ReadableString &file);

		/**
		* @brief Flushes all stored data and recollects from the last read file.
		*
		* @return The number of key-value pairs added on success, ERROR_INDICATOR (-1) otherwise.
		*/
		unsigned int reload();

		/**
		* @brief Syncs data to a file.
		*
		* @param fileName String containing the name of a file.
		* @return True on success, false otherwise.
		*/
		bool sync(const char *fileName);
		bool sync(const Jupiter::ReadableString &file);

		/**
		* @brief Syncs data to the file that was last read from.
		*
		* @return True on success, false otherwise.
		*/
		bool sync();

		/**
		* @brief Sets a key's value. Adds a key if an existing one is not found. Does not modify any files.
		*
		* @param section Section within which the data is to be stored.
		* @param keyName String containing key name.
		* @param keyValue String containing new key value.
		* @return True if a new key was added, false otherwise.
		*/
		bool set(const Jupiter::ReadableString &section, const Jupiter::ReadableString &keyName, const Jupiter::ReadableString &keyValue);

		/**
		* @brief Removes a key. Does not modify any files.
		*
		* @param section String containing section.
		* @param keyName String containing key name.
		* @return True if an entry was removed, false otherwise.
		*/
		bool remove(const Jupiter::ReadableString &section, const Jupiter::ReadableString &keyName);

		/**
		* @brief Removes a section. Does not modify any files.
		*
		* @param section Section to remove.
		* @return True if a section was removed, false otherwise.
		*/
		bool remove(size_t section_index);
		bool remove(const Jupiter::INIFile::Section *section);
		bool remove(const Jupiter::ReadableString &section);

		/**
		* @brief Returns the number of sections in memory.
		*
		* @return Number of sections.
		*/
		size_t getSections() const;

		/**
		* @brief Returns a section.
		*
		* @param index Index of the section to get.
		* @return Section if it exists, nullptr otherwise.
		*/
		Jupiter::INIFile::Section *getSection(size_t index) const;

		/**
		* @brief Returns a section.
		*
		* @param section Name of the section.
		* @return Section if it exists, nullptr otherwise.
		*/
		Jupiter::INIFile::Section *getSection(const Jupiter::ReadableString &section) const;

		/**
		* @brief Returns the index of a section.
		*
		* @param section Name of the section.
		* @return The index of section if it exists, INVALID_INDEX (-1) otherwise.
		*/
		size_t getSectionIndex(const Jupiter::ReadableString &section) const;

		/**
		* @brief Returns the number of keys in a section.
		*
		* @return Number of data entries in a section.
		*/
		size_t getSectionLength(size_t index) const;

		/**
		* @brief Returns a key name.
		*
		* @param section String containing section.
		* @param keyIndex Index of the key.
		* @return Key name on success, nullptr otherwise.
		*/
		const Jupiter::ReadableString &getKey(const Jupiter::ReadableString &section, size_t index) const;

		/**
		* @brief Returns the index of a key.
		*
		* @param section String containing section.
		* @param key String containing key name.
		* @return Index of key if it exists, INVALID_INDEX (-1) otherwise.
		*/
		size_t getKeyIndex(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key) const;

		/**
		* @brief Returns the value of a key.
		*
		* @param section String containing section.
		* @param key String containing key name.
		* @param defaultValue Value to return if none is found.
		* @return Pointer to a string containing key value if it exists, defaultValue otherwise.
		*/
		const Jupiter::ReadableString &get(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, const Jupiter::ReadableString &defaultValue = Jupiter::ReferenceString::empty) const;

		/**
		* @brief Translates and returns the value of a key as a boolean.
		*
		* @param section String containing section.
		* @param key String containing key name.
		* @param defaultValue Value to return if none is found.
		* @return true if key exists and is not "false" or "0", defaultValue otherwise.
		*/
		bool getBool(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, bool defaultValue = false) const;

		/**
		* @brief Translates and returns the value of a key as a short.
		*
		* @param section String containing section.
		* @param key String containing key name.
		* @param defaultValue Value to return if none is found.
		* @return short value of the key if it exits, defaultValue otherwise.
		*/
		short getShort(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, short defaultValue = 0) const;

		/**
		* @brief Translates and returns the value of a key as an int.
		*
		* @param section String containing section.
		* @param key String containing key name.
		* @param defaultValue Value to return if none is found.
		* @return int value of the key if it exits, defaultValue otherwise.
		*/
		int getInt(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, int defaultValue = 0) const;

		/**
		* @brief Translates and returns the value of a key as a long.
		*
		* @param section String containing section.
		* @param key String containing key name.
		* @param defaultValue Value to return if none is found.
		* @return long value of the key if it exits, defaultValue otherwise.
		*/
		long getLong(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, long defaultValue = 0) const;

		/**
		* @brief Translates and returns the value of a key as a long long.
		*
		* @param section String containing section.
		* @param key String containing key name.
		* @param defaultValue Value to return if none is found.
		* @return long long value of the key if it exits, defaultValue otherwise.
		*/
		long long getLongLong(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, long long defaultValue = 0) const;

		/**
		* @brief Translates and returns the value of a key as a float.
		*
		* @param section String containing section.
		* @param key String containing key name.
		* @param defaultValue Value to return if none is found.
		* @return float value of the key if it exits, defaultValue otherwise.
		*/
		float getFloat(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, float defaultValue = 0) const;

		/**
		* @brief Translates and returns the value of a key as a double.
		*
		* @param section String containing section.
		* @param key String containing key name.
		* @param defaultValue Value to return if none is found.
		* @return double value of the key if it exits, defaultValue otherwise.
		*/
		double getDouble(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, double defaultValue = 0) const;

		/**
		* @brief Translates and returns the value of a key as a long double.
		*
		* @param section String containing section.
		* @param key String containing key name.
		* @param defaultValue Value to return if none is found.
		* @return long double value of the key if it exits, defaultValue otherwise.
		*/
		long double getLongDouble(const Jupiter::ReadableString &section, const Jupiter::ReadableString &key, long double defaultValue = 0) const;

		/**
		* @brief Default constructor for the INIFile class.
		*/
		INIFile();

		/**
		* @brief Copy constructor for the INIFile class.
		*/
		INIFile(const INIFile &);

		/**
		* @brief Destructor for the INIFile class.
		*/
		~INIFile();

	/** Private members */
	private:
		struct Data;
		Data *data_;
	};

}

#endif // _INIFILE_H_HEADER