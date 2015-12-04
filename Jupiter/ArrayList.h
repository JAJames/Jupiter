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
		T *get(size_t index) const;

		/**
		* @brief Removes the data at a specified index from the list, and returns the removed data.
		*
		* @param n Index of the node to remove.
		* @return Data removed.
		*/
		T *remove(size_t index);

		/**
		* @brief Adds data to the list at a specified index.
		*
		* @param data Data to add to the list.
		* @param index Position in the list to add the data to.
		*/
		void add(T *data, size_t index);

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
		* @brief Move constructor for the ArrayList class.
		*/
		ArrayList(ArrayList<T> &&);

		/**
		* @brief Destructor for the ArrayList class.
		* Note: This does not delete data added to the list.
		*/
		~ArrayList();

		/** Initial size constant */
		static const size_t start_size = 8;

		/** Access Operator */
		inline T *operator[](size_t index) { return this->get(index); };

	/** Private members */
	private:
		T **data;
		size_t dataSize;
		size_t expandArray();
	};

}

// Implementation

template<typename T> size_t Jupiter::ArrayList<T>::expandArray()
{
	T **tmp = new T *[Jupiter::ArrayList<T>::dataSize * 2];
	for (size_t i = 0; i < Jupiter::ArrayList<T>::dataSize; i++) tmp[i] = data[i];
	delete[] Jupiter::ArrayList<T>::data;
	Jupiter::ArrayList<T>::data = tmp;
	Jupiter::ArrayList<T>::dataSize *= 2;
	return Jupiter::ArrayList<T>::dataSize;
}

template<typename T> Jupiter::ArrayList<T>::ArrayList() : ArrayList(Jupiter::ArrayList<T>::start_size)
{
}

template<typename T> Jupiter::ArrayList<T>::ArrayList(size_t length_)
{
	Jupiter::ArrayList<T>::dataSize = length_;
	Jupiter::ArrayList<T>::data = new T*[Jupiter::ArrayList<T>::dataSize];
	Jupiter::List<T>::length = 0;
}

template<typename T> Jupiter::ArrayList<T>::ArrayList(const Jupiter::ArrayList<T> &source)
{
	Jupiter::ArrayList<T>::dataSize = source.dataSize;
	Jupiter::ArrayList<T>::data = new T*[Jupiter::ArrayList<T>::dataSize];
	Jupiter::List<T>::length = 0;
	while (Jupiter::List<T>::length != source.length)
	{
		Jupiter::ArrayList<T>::data[Jupiter::List<T>::length] = source.data[Jupiter::List<T>::length];
		++Jupiter::List<T>::length;
	}
}

template<typename T> Jupiter::ArrayList<T>::ArrayList(ArrayList<T> &&source)
{
	Jupiter::ArrayList<T>::dataSize = source.dataSize;
	Jupiter::ArrayList<T>::data = source.data;
	Jupiter::List<T>::length = source.length;
	source.dataSize = Jupiter::ArrayList<T>::start_size;
	source.data = new T*[source.dataSize];
	source.length = 0;
}

template<typename T> Jupiter::ArrayList<T>::~ArrayList()
{
	delete[] Jupiter::ArrayList<T>::data;
}

template<typename T> T *Jupiter::ArrayList<T>::get(size_t index) const
{
	return Jupiter::ArrayList<T>::data[index];
}

template<typename T> T *Jupiter::ArrayList<T>::remove(size_t index)
{
	T *r = Jupiter::ArrayList<T>::data[index];
	Jupiter::ArrayList<T>::data[index] = nullptr;
	for (size_t i = index + 1; i < Jupiter::List<T>::length; i++) Jupiter::ArrayList<T>::data[i - 1] = Jupiter::ArrayList<T>::data[i];
	Jupiter::List<T>::length--;
	return r;
}

template<typename T> void Jupiter::ArrayList<T>::add(T *ndata, size_t index)
{
	if (Jupiter::List<T>::length == Jupiter::ArrayList<T>::dataSize) Jupiter::ArrayList<T>::expandArray();
	for (size_t i = Jupiter::List<T>::length; i > index; i--) Jupiter::ArrayList<T>::data[i] = Jupiter::ArrayList<T>::data[i - 1];
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
	while (Jupiter::List<T>::length != 0)
		delete Jupiter::ArrayList<T>::data[--Jupiter::List<T>::length];
}

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::ArrayList>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::ArrayList<Y> *data)
	{
		_Jupiter_DataBuffer_partial_specialization_impl<Jupiter::List>::push<Y>(buffer, data);
	};

	template<typename Y> static Jupiter::ArrayList<Y> interpret(uint8_t *&head)
	{
		size_t size_ = *reinterpret_cast<size_t *>(head);
		head += sizeof(size_t);
		Jupiter::ArrayList<Y> r = Jupiter::ArrayList<Y>(size_);
		while (size_-- != 0)
			r.add(Jupiter::DataBuffer::interpret_data<Y>(head));
		return r;
	}
};

#endif // _ARRAYLIST_H_HEADER