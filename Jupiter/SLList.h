/**
 * Copyright (C) 2013-2016 Jessica James.
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

#if !defined _SLLIST_H_HEADER
#define _SLLIST_H_HEADER

/**
 * @file SLList.h
 * @brief Provides a generic Singly Linked List implementation using the List interface.
 */

#include "Jupiter.h"
#include "List.h"

namespace Jupiter
{
	/**
	* @brief Provides a Singly Linked List implementation using the List interface.
	*/
	template<typename T> class SLList : public List<T>
	{
	public:

		/**
		* @brief Stores a pointer to data, and a pointer to the next node in the list.
		*/
		struct Node
		{
			Node *next = nullptr;
			T *data = nullptr;
		};

		/*
		* @brief Returns the head of the list
		*
		* @return Head of the list
		*/
		Node *getHead() const;

		/*
		* @brief Returns the tail of the list
		*
		* @return Tail of the list
		*/
		Node *getTail() const;

		/**
		* @brief Returns the Node at the specified index in the list.
		*
		* @param index Index of the node to return.
		* @return Node at specified index in the list.
		*/
		Node *getNode(size_t index) const;

		/**
		* @brief Gets the data at a specified index.
		*
		* @param index Index of the data to get.
		* @return Data stored at the specified index.
		*/
		T *get(size_t index) const;

		/**
		* @brief Removes the head of the list
		*
		* @return Value that was stored in the head of the list
		*/
		T *removeHead();

		/**
		* @brief Removes the n'th Node in the list, and returns its contents.
		*
		* @param n Index of the node to remove.
		* @return Contents of the node removed.
		*/
		T *remove(size_t n);

		/**
		* @brief Removes the next node in the list.
		*
		* @param data Node that preceeds the node to be removed.
		* @return Contents of the node removed if the node exists, nullptr otherwise.
		*/
		T *removeNext(Node *data);

		/**
		* @brief Adds data to the end of the list.
		*
		* @param data Data to add to the list.
		*/
		void add(T *data);

		/**
		* @brief Inserts data to the specified index in the list.
		*
		* @param data Data to insert into the list.
		* @param index Position in the list to insert data to.
		*/
		void add(T *data, size_t index);

		/**
		* @brief Inserts data to the head of the list.
		*
		* @param data Data to insert into the list.
		*/
		void addHead(T *data);

		/**
		* @breif Erases all entries in the list
		*/
		void erase();

		/**
		* @breif Erases and deletes all entries in the list
		*/
		void eraseAndDelete();

		SLList &operator=(const SLList &in_list);
		SLList &operator=(SLList &&in_list);

		/**
		* @brief Default constructor for the SLList class.
		*/
		SLList() = default;

		/**
		* @brief Copy constructor for the SLList class.
		*
		* @param in_list List to copy data from
		*/
		SLList(const SLList<T> &in_list);

		/**
		* @brief Move constructor for the SLList class
		*
		* @param in_list List to move data from
		*/
		SLList(SLList<T> &&in_list);

		/**
		* @brief Destructor for the SLList class.
		* Note: This does not delete data added to the list.
		*/
		~SLList();

	/** Private members */
	private:
		void copy_from_internal(const SLList<T> &in_list);

		Node *m_head = nullptr;
		Node *m_tail = nullptr;
	};

}

// Implementation

template<typename T> typename Jupiter::SLList<T>::Node *Jupiter::SLList<T>::getHead() const
{
	return m_head;
}

template<typename T> typename Jupiter::SLList<T>::Node *Jupiter::SLList<T>::getTail() const
{
	return m_tail;
}

template<typename T> typename Jupiter::SLList<T>::Node *Jupiter::SLList<T>::getNode(size_t in_index) const
{
	if (in_index == Jupiter::SLList<T>::length)
		return m_tail;

	Jupiter::SLList<T>::Node *node = m_head;

	while (in_index != 0)
	{
		node = node->next;
		--in_index;
	}

	return node;
}

template<typename T> T *Jupiter::SLList<T>::get(size_t in_index) const
{
	return Jupiter::SLList<T>::getNode(in_index)->data;
}

template<typename T> T *Jupiter::SLList<T>::removeHead()
{
	if (m_head == nullptr)
		return nullptr;

	T *result = m_head->data;

	Jupiter::SLList<T>::Node *node = m_head;
	m_head = m_head->next;
	delete node;
	--Jupiter::List<T>::length;

	return result;
}

template<typename T> T *Jupiter::SLList<T>::remove(size_t in_index)
{
	if (in_index == 0)
		return Jupiter::SLList<T>::removeHead();

	Jupiter::SLList<T>::Node *node = m_head;

	while (in_index != 1)
	{
		node = node->next;
		--in_index;
	}

	Jupiter::SLList<T>::Node *tmp = node->next;
	T *result = tmp->data;

	node->next = tmp->next;
	delete tmp;
	--Jupiter::List<T>::length;

	return result;
}

template<typename T> T *Jupiter::SLList<T>::removeNext(Node *in_data)
{
	Jupiter::SLList<T>::Node *node = in_data->next;

	if (node == nullptr)
		return nullptr;

	T *result = node->data;

	in_data->next = node->next;
	delete node;
	--Jupiter::List<T>::length;

	return result;
}

template<typename T> void Jupiter::SLList<T>::add(T *data)
{
	Jupiter::SLList<T>::Node *node = new Jupiter::SLList<T>::Node();
	node->data = data;

	if (m_head == nullptr)
		m_head = node;
	else
		m_tail->next = node;

	m_tail = node;
	++Jupiter::List<T>::length;
}

template<typename T> void Jupiter::SLList<T>::add(T *in_data, size_t in_index)
{
	if (in_index == 0)
	{
		Jupiter::SLList<T>::addHead(in_data);
		return;
	}

	if (in_index >= Jupiter::List<T>::length)
	{
		Jupiter::SLList<T>::add(in_data);
		return;
	}

	Jupiter::SLList<T>::Node *node = new Jupiter::SLList<T>::Node();
	node->data = in_data;

	Jupiter::SLList<T>::Node *itr = m_head;

	while (in_index != 1)
	{
		itr = itr->next;
		--in_index;
	}

	node->next = itr->next;
	itr->next = node;

	++Jupiter::List<T>::length;
}

template<typename T> void Jupiter::SLList<T>::addHead(T *data)
{
	Jupiter::SLList<T>::Node *node = new Jupiter::SLList<T>::Node();
	node->data = data;
	node->next = m_head;
	m_head = node;

	if (m_tail == nullptr)
		m_tail = node;

	++Jupiter::List<T>::length;
}

template<typename T> void Jupiter::SLList<T>::erase()
{
	Jupiter::SLList<T>::Node *node = m_head;

	if (node == nullptr)
		return;

	Jupiter::SLList<T>::Node *tmp;

	do
	{
		tmp = node;
		node = node->next;
		delete tmp;
	} while (node != nullptr);

	m_head = nullptr;
	m_tail = nullptr;
	Jupiter::List<T>::length = 0;
}

template<typename T> void Jupiter::SLList<T>::eraseAndDelete()
{
	Jupiter::SLList<T>::Node *node = m_head;

	if (node == nullptr)
		return;

	Jupiter::SLList<T>::Node *tmp;

	do
	{
		tmp = node;
		node = node->next;
		delete tmp->data;
		delete tmp;
	} while (node != nullptr);

	m_head = nullptr;
	m_tail = nullptr;
	Jupiter::List<T>::length = 0;
}

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::SLList>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::SLList<Y> *data)
	{
		buffer->push<size_t>(data->size());
		Jupiter::SLList<Y>::Node *head = data->getHead();
		while (head != nullptr)
			buffer->push<Y>(*head++->data);
	};

	template<typename Y> static Jupiter::SLList<Y> interpret(uint8_t *&head)
	{
		size_t size_ = *reinterpret_cast<size_t *>(head);
		head += sizeof(size_t);
		Jupiter::SLList<Y> r;
		while (size_-- != 0)
			r.add(Jupiter::DataBuffer::interpret_data<Y>(head));
		return r;
	}
};

template<typename T> Jupiter::SLList<T> &Jupiter::SLList<T>::operator=(const SLList &in_list)
{
	Jupiter::SLList<T>::erase();

	Jupiter::SLList<T>::copy_from_internal(in_list);

	return *this;
}

template<typename T> Jupiter::SLList<T> &Jupiter::SLList<T>::operator=(SLList &&in_list)
{
	m_head = in_list.m_head;
	m_tail = in_list.m_tail;

	return *this;
}

template<typename T> Jupiter::SLList<T>::SLList(const Jupiter::SLList<T> &in_list)
{
	Jupiter::SLList<T>::copy_from_internal(in_list);
}

template<typename T> Jupiter::SLList<T>::SLList(Jupiter::SLList<T> &&in_list)
{
	m_head = in_list.m_head;
	m_tail = in_list.m_tail;

	in_list.m_head = nullptr;
	in_list.m_tail = nullptr;
}

template<typename T> Jupiter::SLList<T>::~SLList()
{
	Jupiter::SLList<T>::erase();
}

/** Internal */

template<typename T> void Jupiter::SLList<T>::copy_from_internal(const SLList<T> &in_list)
{
	Jupiter::SLList<T>::Node *source_node = in_list.m_head;

	if (source_node == nullptr)
		return;

	Jupiter::SLList<T>::Node *node = new Jupiter::SLList<T>::Node();
	node->data = source_node->data;
	source_node = source_node->next;

	while (source_node != nullptr)
	{
		node->next = new Jupiter::SLList<T>::Node();
		node = node->next;
		node->data = source_node->data;
		source_node = source_node->next;
	}

	m_tail = node;
	Jupiter::List<T>::length = in_list.length;
}

#endif // _SLLIST_H_HEADER