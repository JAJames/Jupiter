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

#if !defined _ARRAYLIST_H_HEADER
#define _ARRAYLIST_H_HEADER

/**
 * @file ArrayList.h
 * @brief Provides an array-based list implementation using the List interface.
 */

#include "Jupiter.h"
#include "List.h"

namespace Jupiter
{
	/**
	* @brief Provides an array-based list implementation using the List interface.
	*/
	template<typename T> class ArrayList : public List<T>
	{
	public:

		/**
		* @brief Gets the data at a specified index.
		*
		* @param index Index of the data to get.
		* @return Data stored at the specified index.
		*/
		T *get(unsigned int index) const;

		/**
		* @brief Removes the data at a specified index from the list, and returns the removed data.
		*
		* @param n Index of the node to remove.
		* @return Data removed.
		*/
		T *remove(unsigned int index);

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
		* @brief Empties the ArrayList of all elements.
		*/
		void empty();

		/**
		* @brief Remove and deletes all elements within the ArrayList.
		*/
		void emptyAndDelete();

		/**
		* @brief Default constructor for the ArrayList class.
		*/
		ArrayList();

		/**
		* @brief Initializes the ArrayList's underlying array to a specified size.
		*/
		ArrayList(size_t length);

		/**
		* @brief Copy constructor for the ArrayList class.
		*/
		ArrayList(const ArrayList<T> &);

		/**
		* @brief Destructor for the ArrayList class.
		* Note: This does not delete data added to the list.
		*/
		~ArrayList();

		/** Access Operator */
		inline T *operator[](size_t index) { return this->get(index); };

	/** Private members */
	private:
		T **data;
		unsigned int dataSize;
		unsigned int expandArray();
	};

}

// Implementation

const unsigned int INIT_SIZE = 8;

template<typename T> unsigned int Jupiter::ArrayList<T>::expandArray()
{
	T **tmp = new T *[Jupiter::ArrayList<T>::dataSize * 2];
	for (unsigned int i = 0; i < Jupiter::ArrayList<T>::dataSize; i++) tmp[i] = data[i];
	delete[] Jupiter::ArrayList<T>::data;
	Jupiter::ArrayList<T>::data = tmp;
	Jupiter::ArrayList<T>::dataSize *= 2;
	return Jupiter::ArrayList<T>::dataSize;
}

template<typename T> Jupiter::ArrayList<T>::ArrayList() : ArrayList(length)
{
}

template<typename T> Jupiter::ArrayList<T>::ArrayList(size_t length)
{
	Jupiter::ArrayList<T>::dataSize = length;
	Jupiter::ArrayList<T>::data = new T*[Jupiter::ArrayList<T>::dataSize];
	Jupiter::List<T>::length = 0;
}

template<typename T> Jupiter::ArrayList<T>::ArrayList(const Jupiter::ArrayList<T> &source)
{
	Jupiter::ArrayList<T>::dataSize = source.dataSize;
	Jupiter::ArrayList<T>::data = new T*[Jupiter::ArrayList<T>::dataSize];
	Jupiter::List<T>::length = source.length;
	for (unsigned int i = 0; i < Jupiter::List<T>::length; i++) Jupiter::ArrayList<T>::data[i] = source.data[i];
}

template<typename T> Jupiter::ArrayList<T>::~ArrayList()
{
	delete[] Jupiter::ArrayList<T>::data;
}

template<typename T> T *Jupiter::ArrayList<T>::get(unsigned int index) const
{
	return Jupiter::ArrayList<T>::data[index];
}

template<typename T> T *Jupiter::ArrayList<T>::remove(unsigned int index)
{
	T *r = Jupiter::ArrayList<T>::data[index];
	Jupiter::ArrayList<T>::data[index] = nullptr;
	for (unsigned int i = index + 1; i < Jupiter::List<T>::length; i++) Jupiter::ArrayList<T>::data[i - 1] = Jupiter::ArrayList<T>::data[i];
	Jupiter::List<T>::length--;
	return r;
}

template<typename T> void Jupiter::ArrayList<T>::add(T *ndata, unsigned int index)
{
	if (Jupiter::List<T>::length == Jupiter::ArrayList<T>::dataSize) Jupiter::ArrayList<T>::expandArray();
	for (unsigned int i = Jupiter::List<T>::length; i > index; i--) Jupiter::ArrayList<T>::data[i] = Jupiter::ArrayList<T>::data[i - 1];
	Jupiter::ArrayList<T>::data[index] = ndata;
	Jupiter::List<T>::length++;
}

template<typename T> void Jupiter::ArrayList<T>::add(T *ndata)
{
	Jupiter::ArrayList<T>::add(ndata, Jupiter::List<T>::length);
}

template<typename T> void Jupiter::ArrayList<T>::empty()
{
	Jupiter::List<T>::length = 0;
}

template<typename T> void Jupiter::ArrayList<T>::emptyAndDelete()
{
	for (unsigned int i = 0; i < Jupiter::List<T>::length; i++) delete Jupiter::ArrayList<T>::data[i];
	Jupiter::List<T>::length = 0;
}

#endif // _ARRAYLIST_H_HEADER