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

#if !defined _HASH_TABLE_H_HEADER
#define _HASH_TABLE_H_HEADER

/**
 * @file Hash_Table.h
 * @brief Defines a generic hash table structure
 */

#include "String.h"
#include "SLList.h"

namespace Jupiter
{
	template<typename T> inline size_t default_hash_function(const T &in);
	
	/**
	* @brief Provides a generic hash table structure
	*
	* @param KeyT Type the table will use for keys; must implement following: operator==, move constructor
	* @param ValueT Type the table will use for values
	* @param InKeyT Type the table will accept for keys (Default: KeyT)
	* @param InValueT Type the table will accept for values (Default: KeyT)
	* @param HashF Function to be used for generating hashes (Default: Fowler-Noll-Vo 1a)
	*/
	template<typename KeyT, typename ValueT, typename InKeyT = KeyT, typename InValueT = ValueT, size_t(*HashF)(const InKeyT &) = Jupiter::default_hash_function<InKeyT>> class Hash_Table
	{
	public:
		/** Initial number of buckets to allocate; m_buckets_size is never less than INIT_SIZE. */
		static constexpr const size_t INIT_SIZE = 8;

		/**
		* @brief Container for table entries
		*/
		struct Bucket
		{
			/**
			* @brief An individual entry within the table, including its normal key and value.
			*/
			struct Entry
			{
				KeyT key;
				ValueT value;

				Entry(const InKeyT &in_key, const InValueT &in_value);
			};

			/**
			* @brief Searches for an entry in the bucket and returns its value if it exists.
			*
			* @param in_key Key of the entry to search for
			* @return Pointer to the value of the entry if it exists, nullptr otherwise
			*/
			ValueT *get(const InKeyT &in_key) const;

			/**
			* @brief Sets the value for an entry in the bucket
			*
			* @param in_key Key of the entry to set
			* @param in_value Value to set in the entry
			* @return True if a new entry was added, false if an entry was overwritten
			*/
			bool set(const InKeyT &in_key, const InValueT &in_value);

			/**
			* @brief Removes an entry from the bucket
			*
			* @param in_key Key of the entry to search for
			* @return Value of the entry which was removed on success, nullptr otherwise
			*/
			ValueT *remove(const InKeyT &in_key);

			/**
			* @brief Copy assignment operator
			*
			* @param in_bucket Bucket to copy entries from
			* @return Reference to this bucket
			*/
			Bucket &operator=(const Bucket &in_bucket);

			/**
			* @brief Move assignment operator
			*
			* @param in_bucket Bucket to move entries from
			* @return Reference to this bucket
			*/
			Bucket &operator=(Bucket &&in_bucket);

			/**
			* @brief Default constructor for the Bucket class
			*/
			Bucket() = default;

			/**
			* @brief Copy constructor for the Bucket class
			*/
			Bucket(const Bucket &in_bucket);

			/**
			* @brief Move constructor for the Bucket class
			*/
			Bucket(Bucket &&in_bucket);

			/**
			* @brief Destructor for the Bucket class
			*/
			~Bucket();

			/** List of entries in the bucket */
			Jupiter::SLList<Entry> m_pairs;
		};

		/**
		* @brief Fetches the value of an entry based on its key
		*
		* @param in_key Key of the entry to search for
		* @return Value of the entry if it exists, nullptr otherwise
		*/
		ValueT *get(const InKeyT &in_key) const;

		/**
		* @brief Sets the value for an entry in the table
		*
		* @param in_key Key of the entry to set
		* @param in_value Value of the entry to set
		*/
		void set(const InKeyT &in_key, const InValueT &in_value);

		/**
		* @brief Removes an entry from the table and returns its value
		*
		* @param in_key Key of the entry to remove
		* @return Value of the entry that was removed if it exists, nullptr otherwise
		*/
		ValueT *remove(const InKeyT &in_key);

		/**
		* @brief Returns the number of entries in the table
		*
		* @return Number of entries in the table
		*/
		size_t size() const;

		/**
		* @brief Erases the table's contents
		*/
		void erase();

		/**
		* @brief Shrinks the table's internal bucket pool to the current number of entries, not to be less than INIT_SIZE.
		*/
		void shrink();

		/**
		* @brief Copy assignment operator
		*
		* @param in_table Table to copy entries from
		* @return Reference to this table
		*/
		Hash_Table &operator=(const Hash_Table &in_table);

		/**
		* @brief Move assignment operator
		*
		* @param in_table Table to move entries from
		* @return Reference to this table
		*/
		Hash_Table &operator=(Hash_Table &&in_table);

		/**
		* @brief Default constructor for the Hash_Table class
		*/
		Hash_Table();

		/**
		* @brief Allocation constructor for the Hash_Table class
		*
		* @param in_buckets_size Number of buckets to initialize to
		*/
		Hash_Table(size_t in_buckets_size);

		/**
		* @brief Copy constructor for the Hash_Table class
		*
		* @param in_table Table to copy entries from
		*/
		Hash_Table(const Hash_Table &in_table);

		/**
		* @brief Move constructor for the Hash_Table class
		*
		* @param in_table Table to move entries from
		*/
		Hash_Table(Hash_Table &&in_table);

		/**
		* @brief Destructor for the Hash_Table class
		*/
		~Hash_Table();

	private:
		/**
		* @brief Doubles the size of m_buckets
		*/
		void expand();

		/**
		* @brief Copies entries from m_buckets to in_buckets; used when expanding or shrinking m_buckets.
		*
		* @param in_buckets Array of buckets to copy entries into
		* @param in_buckets_size Number of buckets in in_buckets
		*/
		void copy_to_buckets(Bucket *in_buckets, size_t in_buckets_size);

		Bucket *m_buckets;		/** Array of Buckets */
		size_t m_buckets_size;	/** Number of buckets */
		size_t m_length = 0;	/** Number of entries */
	};

	typedef Hash_Table<Jupiter::StringS, Jupiter::StringS, Jupiter::ReadableString, Jupiter::ReadableString, Jupiter::default_hash_function<Jupiter::ReadableString>> HashTable;
}

#include "Hash_Table_Imp.h"

#endif // _HASH_TABLE_H_HEADER