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

#if !defined _DLLIST_H_HEADER
#define _DLLIST_H_HEADER

/**
 * @file DLList.h
 * @brief Provides a generic Doubly Linked List implementation using the List interface.
 */

#include "Jupiter.h"
#include "List.h"

namespace Jupiter
{
	/**
	* @brief Provides a Doubly Linked List implementation using the List interface.
	*/
	template<typename T> class DLList : public List<T>
	{
	public:

		/**
		* @brief Stores a pointer to data, and a pointer to the next node in the list.
		*/
		struct Node
		{
			Node *next;
			Node *previous;
			T *data;
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
		* @brief Removes the n'th Node in the list, and returns its contents.
		*
		* @param n Index of the node to remove.
		* @return Contents of the node removed.
		*/
		T *remove(size_t n);

		/**
		* @brief Removes a node from the list.
		*
		* @param data Node to remove.
		* @return Contents of the node removed.
		*/
		T *remove(Node *data);

		/**
		* @brief Adds data to the list at a specified index.
		*
		* @param data Data to add to the list.
		* @param index Position in the list to add the data to.
		*/
		void add(T *data, size_t index);

		/**
		* @brief Adds data to the tail of the list.
		*
		* @param data Data to add to the list.
		*/
		void add(T *data);

		/**
		* @brief Default constructor for the DLList class.
		*/
		DLList() = default;

		/**
		* @brief Copy constructor for the DLList class.
		*/
		DLList(const DLList<T> &);

		/**
		* @brief Destructor for the DLList class.
		* Note: This does not delete data added to the list.
		*/
		~DLList();

	/** Private members */
	private:
		Node *m_head = nullptr;
		Node *m_tail = nullptr;
	};

}

// Implementation

template<typename T> Jupiter::DLList<T>::DLList(const Jupiter::DLList<T> &source)
{
	Jupiter::List<T>::length = source.length;
	if (Jupiter::List<T>::length == 0)
	{
		m_head = nullptr;
		m_tail = nullptr;
	}
	else if (Jupiter::List<T>::length == 1)
	{
		Jupiter::DLList<T>::Node *n = new Jupiter::DLList<T>::Node;
		n->data = source.getNode(0)->data;
		m_head = n;
		m_tail = n;
	}
	else
	{
		Jupiter::DLList<T>::Node *sourceNode = source.getNode(0);
		Jupiter::DLList<T>::Node *n = new Jupiter::DLList<T>::Node;
		n->data = sourceNode->data;
		m_head = n;
		sourceNode = sourceNode->next;

		while (sourceNode->next != nullptr)
		{
			n->next = new Jupiter::DLList<T>::Node;
			n = n->next;
			n->data = sourceNode->data;
			sourceNode = sourceNode->next;
		}

		n->next = new Jupiter::DLList<T>::Node;
		n = n->next;
		n->data = sourceNode->data;

		m_tail = n;
	}
}

template<typename T> Jupiter::DLList<T>::~DLList()
{
	Jupiter::DLList<T>::Node *p;
	Jupiter::DLList<T>::Node *c = m_head;
	while (c != nullptr)
	{
		p = c;
		c = c->next;
		delete p;
	}
}

template<typename T> typename Jupiter::DLList<T>::Node *Jupiter::DLList<T>::getHead() const
{
	return m_head;
}

template<typename T> typename Jupiter::DLList<T>::Node *Jupiter::DLList<T>::getTail() const
{
	return m_tail;
}

template<typename T> typename Jupiter::DLList<T>::Node *Jupiter::DLList<T>::getNode(size_t index) const
{
	Jupiter::DLList<T>::Node *r;
	if (index * 2 < Jupiter::List<T>::length)
	{
		r = m_head;
		for (size_t i = 0; i < index; i++) r = r->next;
		return r;
	}
	r = m_tail;
	for (size_t i = Jupiter::List<T>::length - 1; i > index; i--) r = r->previous;
	return r;
}

template<typename T> T *Jupiter::DLList<T>::get(size_t index) const
{
	return Jupiter::DLList<T>::getNode(index)->data;
}

template<typename T> T *Jupiter::DLList<T>::remove(size_t index)
{
	return Jupiter::DLList<T>::remove(Jupiter::DLList<T>::getNode(index));
}

template<typename T> T *Jupiter::DLList<T>::remove(Node *data)
{
	if (m_head == data)
	{
		m_head = data->next;
		if (data->next != nullptr) data->next->previous = data->previous;
		else m_tail = nullptr;
	}
	else if (m_tail == data)
	{
		m_tail = data->previous;
		m_tail->next = nullptr;
	}
	else
	{
		data->next->previous = data->previous;
		data->previous->next = data->next;
	}
	T *r = data->data;
	delete data;
	Jupiter::List<T>::length--;
	return r;
}

template<typename T> void Jupiter::DLList<T>::add(T *data, size_t index)
{
	Jupiter::DLList<T>::Node *node = new Jupiter::DLList<T>::Node();
	node->data = data;
	if (index == 0)
	{
		node->next = m_head;
		m_head->previous = node;
		m_head = node;
		node->previous = nullptr;
	}
	else if (index == Jupiter::List<T>::length)
	{
		node->previous = m_tail;
		m_tail->next = node;
		m_tail = node;
		node->next = nullptr;
	}
	else
	{
		Jupiter::DLList<T>::Node *n = Jupiter::DLList<T>::getNode(index);
		node->next = n;
		node->previous = n->previous;
		node->previous->next = node;
		n->previous = node;
	}
	Jupiter::List<T>::length++;
}

template<typename T> void Jupiter::DLList<T>::add(T *data)
{
	Jupiter::DLList<T>::Node *n = new Jupiter::DLList<T>::Node();
	n->data = data;
	n->next = nullptr;
	if (Jupiter::List<T>::length == 0)
	{
		m_head = n;
		m_tail = n;
		n->previous = nullptr;
	}
	else
	{
		n->previous = m_tail;
		m_tail->next = n;
		m_tail = n;
	}
	Jupiter::List<T>::length++;
}

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::DLList>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::DLList<Y> *data)
	{
		buffer->push<size_t>(data->size());
		Jupiter::DLList<Y>::Node *head = data->getNode(0);
		while (head != nullptr)
		{
			buffer->push<Y>(*head->data);
			head++;
		}
	};

	template<typename Y> static Jupiter::DLList<Y> interpret(uint8_t *&head)
	{
		size_t size_ = *reinterpret_cast<size_t *>(head);
		head += sizeof(size_t);
		Jupiter::DLList<Y> r;
		while (size_-- != 0)
			r.add(Jupiter::DataBuffer::interpret_data<Y>(head));
		return r;
	}
};

#endif // _DLLIST_H_HEADER