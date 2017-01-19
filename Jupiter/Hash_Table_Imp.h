/**
 * Copyright (C) 2016-2017 Jessica James.
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

/** Conditional is constant; can probably be removed when 'if constexpr' support is added to MSVC */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127)
#endif

template<typename T> inline size_t Jupiter::default_hash_function(const T &in)
{
	if (sizeof(size_t) >= sizeof(uint64_t))
		return static_cast<size_t>(Jupiter::fnv1a(in));

	return static_cast<size_t>(Jupiter::fnv1a_32(in));
}

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

/** Hash_Table::Bucket::Entry */

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::Entry::Entry(const InKeyT &in_key)
{
	key = in_key;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::Entry::Entry(const InKeyT &in_key, const InValueT &in_value)
{
	key = in_key;
	value = in_value;
}

/** Hash_Table::Bucket */

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
const ValueT *Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::get(const InKeyT &in_key) const
{
	for (auto node = m_entries.begin(); node != m_entries.end(); ++node)
		if (node->key == in_key)
			return &node->value;

	return nullptr;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
ValueT *Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::get(const InKeyT &in_key)
{
	for (auto node = m_entries.begin(); node != m_entries.end(); ++node)
		if (node->key == in_key)
			return &node->value;

	return nullptr;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
const InValueT &Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::get(const InKeyT &in_key, const InValueT &in_value) const
{
	for (auto node = m_entries.begin(); node != m_entries.end(); ++node)
		if (node->key == in_key)
			return node->value;

	return in_value;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
template<typename CastT>
CastT Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::getCast(const InKeyT &in_key, const CastT &in_value) const
{
	for (auto node = m_entries.begin(); node != m_entries.end(); ++node)
		if (node->key == in_key)
			return static_cast<CastT>(node->value);

	return in_value;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
bool Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::set(const InKeyT &in_key, const InValueT &in_value)
{
	for (auto node = m_entries.begin(); node != m_entries.end(); ++node)
		if (node->key == in_key)
		{
			node->value = in_value;
			return false;
		}

	m_entries.emplace_front(in_key, in_value);
	return true;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
bool Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::set(const InKeyT &in_key)
{
	for (auto node = m_entries.begin(); node != m_entries.end(); ++node)
		if (node->key == in_key)
			return false;

	m_entries.emplace_front(in_key);
	return true;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
bool Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::remove(const InKeyT &in_key)
{
	auto node = m_entries.begin();
	auto end = m_entries.end();

	// No nodes in the bucket
	if (node == end)
		return false;

	// Check if the head is the desired node
	if (node->key == in_key)
	{
		m_entries.pop_front();
		return true;
	}

	auto next_node = node;
	++next_node;

	// iterate through list
	while (next_node != end)
	{
		if (next_node->key == in_key)
		{
			// The next node is the desired node
			m_entries.erase_after(node);
			return true;
		}

		node = next_node;
		++next_node;
	}

	return false;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
template<typename CallT>
void Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::callback(CallT &in_callback) const
{
	for (auto node = m_entries.begin(); node != m_entries.end(); ++node)
		in_callback(*node);
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
template<typename CallT>
void Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::callback(CallT &in_callback)
{
	for (auto node = m_entries.begin(); node != m_entries.end(); ++node)
		in_callback(*node);
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
typename Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket &Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::operator=(const Bucket &in_bucket)
{
	m_entries.clear();
	for (auto node = in_bucket.m_entries.begin(); node != m_entries.end(); ++node)
		m_entries.emplace_front(node->key, node->value);

	return *this;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
typename Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket &Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::operator=(Bucket &&in_bucket)
{
	m_entries = std::move(in_bucket.m_entries);
	return *this;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::Bucket(const Bucket &in_bucket)
{
	for (auto node = in_bucket.m_entries.getHead(); node != in_bucket.m_entries.end(); ++node)
		m_entries.emplace_front(node->key, node->value);
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::Bucket(Bucket &&in_bucket)
{
	m_entries = std::move(in_bucket.m_entries);
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket::~Bucket()
{
	m_entries.clear();
}

/** Hash_Table */

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
typename Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket *Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::begin() const
{
	return m_buckets;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
typename Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::Bucket *Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::end() const
{
	return m_buckets + m_buckets_size;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
ValueT *Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::get(const InKeyT &in_key) const
{
	return m_buckets[HashF(in_key) % m_buckets_size].get(in_key);
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
const InValueT &Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::get(const InKeyT &in_key, const InValueT &in_value) const
{
	return m_buckets[HashF(in_key) % m_buckets_size].get(in_key, in_value);
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
template<typename CastT>
CastT Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::getCast(const InKeyT &in_key, const CastT &in_value) const
{
	return m_buckets[HashF(in_key) % m_buckets_size].getCast(in_key, in_value);
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
bool Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::set(const InKeyT &in_key, const InValueT &in_value)
{
	if (m_buckets[HashF(in_key) % m_buckets_size].set(in_key, in_value))
	{
		if (++m_length == m_buckets_size)
			expand();

		return true;
	}

	return false;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
bool Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::set(const InKeyT &in_key)
{
	if (m_buckets[HashF(in_key) % m_buckets_size].set(in_key))
	{
		if (++m_length == m_buckets_size)
			expand();

		return true;
	}

	return false;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
bool Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::remove(const InKeyT &in_key)
{
	if (m_buckets[HashF(in_key) % m_buckets_size].remove(in_key))
	{
		--m_length;
		return true;
	}

	return false;
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
template<typename CallT>
void Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::callback(CallT &in_callback) const
{
	Bucket *itr = m_buckets;
	Bucket *end = m_buckets + m_buckets_size;

	while (itr != end)
	{
		itr->callback<CallT>(in_callback);
		++itr;
	}
}

template<typename KeyT, typename ValueT, typename InKeyT, typename InValueT, size_t(*HashF)(const InKeyT &)>
template<typename CallT>
void Jupiter::Hash_Table<KeyT, ValueT, InKeyT, InValueT, HashF>::callback(CallT &in_callback)
{
	Bucket *itr = m_buckets;
	Bucket *end = m_buckets + m_buckets_size;

	while (itr != end)
	{
		itr->callback<CallT>(in_callback);
		++itr;
	}
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
			std::forward_list<Bucket::Entry>::iterator node;

			size_t index = 0;
			while (index != in_table.m_buckets_size)
			{
				in_table.m_buckets[index].m_entries.clear();

				for (node = in_table.m_buckets[index].m_entries.begin(); node != in_table.m_buckets[index].m_entries.end(); ++node)
					m_buckets[HashF(node->key) % m_buckets_size].set(node->key, node->value);

				++index;
			}

			while (index != m_buckets_size)
			{
				in_table.m_buckets[index].m_entries.clear();
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
	std::forward_list<Bucket::Entry>::iterator node;

	for (size_t index = 0; index != m_buckets_size; ++index)
		for (node = m_buckets[index].m_entries.begin(); node != m_buckets[index].m_entries.end(); ++node)
			in_buckets[HashF(node->key) % in_buckets_size].set(node->key, node->value);
}

#endif // _HASH_TABLE_IMP_H_HEADER