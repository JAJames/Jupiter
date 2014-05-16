/**
 * Copyright (C) 2013-2014 Justin James.
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

#if !defined _DLLIST_H_HEADER
#define _DLLIST_H_HEADER

#include "Jupiter.h"
#include "List.h"

/**
* @file DLList.h
* @brief Provides a generic Doubly Linked List implementation using the List interface.
*/

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

		/**
		* @brief Returns the n'th Node in the list.
		*
		* @param n Index of the node to return.
		* @return n'th Node in the list.
		*/
		Node *getNode(unsigned int n) const;

		/**
		* @brief Gets the data at a specified index.
		*
		* @param index Index of the data to get.
		* @return Data stored at the specified index.
		*/
		T *get(unsigned int index) const;

		/**
		* @brief Removes the n'th Node in the list, and returns its contents.
		*
		* @param n Index of the node to remove.
		* @return Contents of the node removed.
		*/
		T *remove(unsigned int n);

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
		void add(T *data, unsigned int index);

		/**
		* @brief Adds data to the end of the list.
		*
		* @param data Data to add to the list.
		*/
		void add(T *data);

		/**
		* @brief Default constructor for the DLList class.
		*/
		DLList();

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
		Node *head;
		Node *end;
	};

}

// Implementation

template<typename T> Jupiter::DLList<T>::DLList()
{
	Jupiter::DLList<T>::head = nullptr;
	Jupiter::DLList<T>::end = nullptr;
	Jupiter::List<T>::length = 0;
}

template<typename T> Jupiter::DLList<T>::DLList(const Jupiter::DLList<T> &source)
{
	Jupiter::List<T>::length = source.length;
	if (Jupiter::List<T>::length == 0)
	{
		Jupiter::DLList<T>::head = nullptr;
		Jupiter::DLList<T>::end = nullptr;
	}
	else if (Jupiter::List<T>::length == 1)
	{
		Jupiter::DLList<T>::Node *n = new Jupiter::DLList<T>::Node;
		n->data = source.getNode(0)->data;
		Jupiter::DLList<T>::head = n;
		Jupiter::DLList<T>::end = n;
	}
	else
	{
		Jupiter::DLList<T>::Node *sourceNode = source.getNode(0);
		Jupiter::DLList<T>::Node *n = new Jupiter::DLList<T>::Node;
		n->data = sourceNode->data;
		Jupiter::DLList<T>::head = n;
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

		Jupiter::DLList<T>::end = n;
	}
}

template<typename T> Jupiter::DLList<T>::~DLList()
{
	Jupiter::DLList<T>::Node *p;
	Jupiter::DLList<T>::Node *c = Jupiter::DLList<T>::head;
	while (c != nullptr)
	{
		p = c;
		c = c->next;
		delete p;
	}
}

template<typename T> typename Jupiter::DLList<T>::Node *Jupiter::DLList<T>::getNode(unsigned int index) const
{
	Jupiter::DLList<T>::Node *r;
	if (index * 2 < Jupiter::List<T>::length)
	{
		r = Jupiter::DLList<T>::head;
		for (unsigned int i = 0; i < index; i++) r = r->next;
		return r;
	}
	r = Jupiter::DLList<T>::end;
	for (unsigned int i = Jupiter::List<T>::length - 1; i > index; i--) r = r->previous;
	return r;
}

template<typename T> T *Jupiter::DLList<T>::get(unsigned int index) const
{
	return Jupiter::DLList<T>::getNode(index)->data;
}

template<typename T> T *Jupiter::DLList<T>::remove(unsigned int index)
{
	return Jupiter::DLList<T>::remove(Jupiter::DLList<T>::getNode(index));
}

template<typename T> T *Jupiter::DLList<T>::remove(Node *data)
{
	if (Jupiter::DLList<T>::head == data)
	{
		Jupiter::DLList<T>::head = data->next;
		if (data->next != nullptr) data->next->previous = data->previous;
		else Jupiter::DLList<T>::end = nullptr;
	}
	else if (Jupiter::DLList<T>::end == data)
	{
		Jupiter::DLList<T>::end = data->previous;
		Jupiter::DLList<T>::end->next = nullptr;
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

template<typename T> void Jupiter::DLList<T>::add(T *data, unsigned int index)
{
	Jupiter::DLList<T>::Node *node = new Jupiter::DLList<T>::Node();
	node->data = data;
	if (index == 0)
	{
		node->next = Jupiter::DLList<T>::head;
		Jupiter::DLList<T>::head->previous = node;
		Jupiter::DLList<T>::head = node;
		node->previous = nullptr;
	}
	else if (index == Jupiter::List<T>::length)
	{
		node->previous = Jupiter::DLList<T>::end;
		Jupiter::DLList<T>::end->next = node;
		Jupiter::DLList<T>::end = node;
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
		Jupiter::DLList<T>::head = n;
		Jupiter::DLList<T>::end = n;
		n->previous = nullptr;
	}
	else
	{
		n->previous = Jupiter::DLList<T>::end;
		Jupiter::DLList<T>::end->next = n;
		Jupiter::DLList<T>::end = n;
	}
	Jupiter::List<T>::length++;
}

#endif // _DLLIST_H_HEADER