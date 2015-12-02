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

		/**
		* @brief Returns the n'th Node in the list.
		*
		* @param n Index of the node to return.
		* @return n'th Node in the list.
		*/
		Node *getNode(size_t n) const;

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
		* @brief Removes the next node in the list.
		*
		* @param data Node that preceeds the node to be removed.
		* @return Contents of the node removed if the node exists, nullptr otherwise.
		*/
		T *removeNext(Node *data);

		/**
		* @brief Adds data to the list at a specified index.
		*
		* @param data Data to add to the list.
		* @param index Position in the list to add the data to.
		*/
		void add(T *data, size_t index);

		/**
		* @brief Adds data to the front of the list.
		*
		* @param data Data to add to the list.
		*/
		void add(T *data);

		/**
		* @brief Default constructor for the SLList class.
		*/
		SLList();

		/**
		* @brief Copy constructor for the SLList class.
		*/
		SLList(const SLList<T> &);

		/**
		* @brief Destructor for the SLList class.
		* Note: This does not delete data added to the list.
		*/
		~SLList();

	/** Private members */
	private:
		Node *head;
	};

}

// Implementation

template<typename T> Jupiter::SLList<T>::SLList()
{
	Jupiter::SLList<T>::head = new Jupiter::SLList<T>::Node();
	Jupiter::List<T>::length = 0;
}

template<typename T> Jupiter::SLList<T>::SLList(const Jupiter::SLList<T> &source)
{
	Jupiter::SLList<T>::head = new Jupiter::SLList<T>::Node();
	Jupiter::SLList<T>::Node *sourceNode = source.head;
	
	head->data = sourceNode->data;
	sourceNode = sourceNode->next;

	Jupiter::SLList<T>::Node *n = Jupiter::SLList<T>::head;

	while (sourceNode != nullptr)
	{
		n->next = new Jupiter::SLList<T>::Node();
		n = n->next;
		n->data = sourceNode->data;
		sourceNode = sourceNode->next;
	}
	Jupiter::List<T>::length = source.length;
}

template<typename T> Jupiter::SLList<T>::~SLList()
{
	Jupiter::SLList<T>::Node *p;
	Jupiter::SLList<T>::Node *c = head;
	do
	{
		p = c;
		c = c->next;
		delete p;
	} while (c != nullptr);
}

template<typename T> typename Jupiter::SLList<T>::Node *Jupiter::SLList<T>::getNode(size_t index) const
{
	Jupiter::SLList<T>::Node *t = head->next;
	for (size_t i = 0; i != index; i++) t = t->next;
	return t;
}

template<typename T> T *Jupiter::SLList<T>::get(size_t index) const
{
	return Jupiter::SLList<T>::getNode(index)->data;
}

template<typename T> T *Jupiter::SLList<T>::remove(size_t index)
{
	Jupiter::SLList<T>::Node *t = head;
	for (size_t i = 0; i != index; i++)
		t = t->next;
	Jupiter::SLList<T>::Node *t2 = t->next;
	t->next = t2->next;
	T *r = t2->data;
	delete t2;
	--Jupiter::List<T>::length;
	return r;
}

template<typename T> T *Jupiter::SLList<T>::removeNext(Node *data)
{
	Jupiter::SLList<T>::Node *t = data->next;
	if (t == nullptr) return nullptr;
	T *r = t->data;
	data->next = t->next;
	delete t;
	Jupiter::List<T>::length--;
	return r;
}

template<typename T> void Jupiter::SLList<T>::add(T *data, size_t index)
{
	Jupiter::SLList<T>::Node *n = new Jupiter::SLList<T>::Node();
	n->data = data;
	Jupiter::SLList<T>::Node *t = Jupiter::SLList<T>::head;
	for (size_t i = 0; i < index; i++) t = t->next;
	n->next = t->next;
	t->next = n;
	Jupiter::List<T>::length++;
}

template<typename T> void Jupiter::SLList<T>::add(T *data)
{
	Jupiter::SLList<T>::Node *n = new Jupiter::SLList<T>::Node();
	n->data = data;
	n->next = Jupiter::SLList<T>::head->next;
	Jupiter::SLList<T>::head->next = n;
	Jupiter::List<T>::length++;
}

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::SLList>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::SLList<Y> *data)
	{
		buffer->push<size_t>(data->size());
		Jupiter::SLList<Y>::Node *head = data->getNode(0);
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

#endif // _SLLIST_H_HEADER