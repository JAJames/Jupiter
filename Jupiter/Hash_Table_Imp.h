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

#if !defined _HASH_TABLE_IMP_H_HEADER
#define _HASH_TABLE_IMP_H_HEADER

/**
* @file Hash_Table_Imp.h
* @brief Provides the implementation for Hash_Table.
*/

#include "Hash_Table.h"
#include "Hash.h"

/**
* IMPLEMENTATION:
*	Hash_Table
*/

template<typename T> inline size_t Jupiter::default_hash_function(const T &in)
{
	if (sizeof(size_t) >= sizeof(uint64_t))
		return static_cast<size_t>(Jupiter::fnv1a(in));

	return static_cast<size_t>(Jupiter::fnv1a_32(in));
}

/** Hash_Table::Bucket::Entry */

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::Entry::Entry(const InKeyT &in_key, const InValueT &in_value)
{
	key = in_key;
	value = in_value;
}

/** Hash_Table::Bucket */

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
ValueT *Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::get(const InKeyT &in_key) const
{
	for (Jupiter::SLList<Entry>::Node *node = m_pairs.getHead(); node != nullptr; node = node->next)
		if (node->data->key == in_key)
			return &node->data->value;

	return nullptr;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
bool Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::set(const InKeyT &in_key, const InValueT &in_value)
{
	for (Jupiter::SLList<Entry>::Node *node = m_pairs.getHead(); node != nullptr; node = node->next)
		if (node->data->key == in_key)
		{
			node->data->value = in_value;
			return false;
		}

	m_pairs.add(new Entry(in_key, in_value));
	return true;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
ValueT *Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::remove(const InKeyT &in_key)
{
	Jupiter::SLList<Entry>::Node *node = m_pairs.getHead();
	Entry *pair;
	ValueT *result;

	// No nodes in the bucket
	if (node == nullptr)
		return nullptr;

	// Check if the head is the desired node
	if (node->data->key == in_key)
	{
		pair = m_pairs.removeHead();
		result = new ValueT(std::move(pair->value));
		delete pair;

		return result;
	}

	// iterate through list
	while (node->next != nullptr)
	{
		if (node->next->data->key == in_key)
		{
			// The next node is the desired node
			pair = m_pairs.removeNext(node);
			result = new ValueT(std::move(pair->value));
			delete pair;

			return result;
		}
		node = node->next;
	}

	return nullptr;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
size_t Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::erase()
{
	size_t length = m_pairs.size();
	m_pairs.eraseAndDelete();

	return length;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
typename Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket &Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::operator=(const Bucket &in_bucket)
{
	m_pairs.eraseAndDelete();
	for (Jupiter::SLList<Entry>::Node *node = in_bucket.m_pairs.getHead(); node != nullptr; node = node->next)
		m_pairs.add(new Entry(node->data->key, node->data->value));

	return *this;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
typename Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket &Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::operator=(Bucket &&in_bucket)
{
	m_pairs = std::move(in_bucket.m_pairs);
	return *this;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::Bucket(const Bucket &in_bucket)
{
	for (Jupiter::SLList<Entry>::Node *node = in_bucket.m_pairs.getHead(); node != nullptr; node = node->next)
		m_pairs.add(new Entry(node->data->key, node->data->value));
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::Bucket(Bucket &&in_bucket)
{
	m_pairs = std::move(in_bucket.m_pairs);
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::~Bucket()
{
	m_pairs.eraseAndDelete();
}

/** Hash_Table */

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
ValueT *Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::get(const InKeyT &in_key) const
{
	return m_buckets[HashF(in_key) % m_buckets_size].get(in_key);
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
void Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::set(const InKeyT &in_key, const InValueT &in_value)
{
	if (m_buckets[HashF(in_key) % m_buckets_size].set(in_key, in_value))
		if (++m_length == m_buckets_size)
			expand();
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
ValueT *Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::remove(const InKeyT &in_key)
{
	ValueT *value = m_buckets[HashF(in_key) % m_buckets_size].remove(in_key);

	if (value != nullptr)
		--m_length;

	return value;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
size_t Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::size() const
{
	return m_length;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
void Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::erase()
{
	m_length = 0;
	delete[] m_buckets;

	m_buckets_size = INIT_SIZE;
	m_buckets = new Bucket[m_buckets_size];
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
void Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::shrink()
{
	size_t buckets_size = m_length < INIT_SIZE ? INIT_SIZE : m_length;
	Bucket *buckets = new Bucket[buckets_size];

	copy_to_buckets(buckets, buckets_size);

	delete[] m_buckets;
	m_buckets = buckets;
	m_buckets_size = buckets_size;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
typename Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF> &Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::operator=(const Hash_Table &in_table)
{
	if (m_buckets_size >= in_table.m_buckets_size && m_buckets_size <= in_table.m_buckets_size * 2)
	{
		// We're larger than what we're copying, but not drastically (2x) larger. Just erase our current data and copy their entries.

		if (m_length == 0) // we don't need to erase any data
			in_table.copy_to_buckets(m_buckets, m_buckets_size);
		else
		{
			// we need to erase data; slightly modified version of copy_to_buckets()
			Jupiter::SLList<Bucket::Entry>::Node *node;

			size_t index = 0;
			while (index != in_table.m_buckets_size)
			{
				in_table.m_buckets[index].m_pairs.eraseAndDelete();

				for (node = in_table.m_buckets[index].m_pairs.getHead(); node != nullptr; node = node->next)
					m_buckets[HashF(node->data->key) % m_buckets_size].set(node->data->key, node->data->value);

				++index;
			}

			while (index != m_buckets_size)
			{
				in_table.m_buckets[index].m_pairs.eraseAndDelete();
				++index;
			}
		}
	}
	else
	{
		// m_buckets is either too small to copy the data, or it'd be too wasteful

		delete[] m_buckets;

		m_buckets_size = in_table.m_buckets_size;
		m_buckets = new Bucket[m_buckets_size];

		for (size_t index = 0; index != m_buckets_size; ++index)
			m_buckets[index] = in_table.m_buckets[index];
	}

	m_length = in_table.m_length;

	return *this;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
typename Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF> &Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::operator=(Hash_Table &&in_table)
{
	if (m_length == 0)
	{
		Bucket *old_buckets = m_buckets;
		size_t old_buckets_size = m_buckets_size;

		m_buckets = in_table.m_buckets;
		m_buckets_size = in_table.m_buckets_size;
		m_length = in_table.m_length;

		in_table.m_buckets = old_buckets;
		in_table.m_buckets_size = old_buckets_size;
	}
	else
	{
		delete[] m_buckets;

		m_buckets = in_table.m_buckets;
		m_buckets_size = in_table.m_buckets_size;
		m_length = in_table.m_length;

		in_table.m_buckets = new Bucket[1];
		in_table.m_buckets_size = 1;
	}

	in_table.m_length = 0;

	return *this;
}

/** Constructors */

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Hash_Table()
{
	m_buckets_size = INIT_SIZE;
	m_buckets = new Bucket[m_buckets_size];
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Hash_Table(size_t in_buckets_size)
{
	m_buckets_size = in_buckets_size;
	m_buckets = new Bucket[m_buckets_size];
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Hash_Table(const Hash_Table &in_table)
{
	m_length = in_table.m_length;
	m_buckets_size = in_table.m_buckets_size;
	m_buckets = new Bucket[m_buckets_size];

	for (size_t index = 0; index != m_buckets_size; ++index)
		m_buckets[index] = in_table.m_buckets[index];
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Hash_Table(Hash_Table &&in_table)
{
	m_buckets = in_table.m_buckets;
	m_buckets_size = in_table.m_buckets_size;
	m_length = in_table.m_length;

	in_table.m_buckets = new Bucket[1];
	in_table.m_buckets_size = 1;
	in_table.m_length = 0;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::~Hash_Table()
{
	delete[] m_buckets;
}

/** Hash_Table/private */

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
void Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::expand()
{
	size_t buckets_size = m_buckets_size * 2;
	Bucket *buckets = new Bucket[buckets_size];

	copy_to_buckets(buckets, buckets_size);

	delete[] m_buckets;
	m_buckets = buckets;
	m_buckets_size = buckets_size;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
void Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::copy_to_buckets(Bucket *in_buckets, size_t in_buckets_size) const
{
	Jupiter::SLList<Bucket::Entry>::Node *node;

	for (size_t index = 0; index != m_buckets_size; ++index)
		for (node = m_buckets[index].m_pairs.getHead(); node != nullptr; node = node->next)
			in_buckets[HashF(node->data->key) % in_buckets_size].set(node->data->key, node->data->value);
}

#endif // _HASH_TABLE_IMP_H_HEADER