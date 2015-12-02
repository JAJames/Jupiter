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

#if !defined _LIST_H_HEADER
#define _LIST_H_HEADER

/**
 * @file List.h
 * @brief Provides a generic List interface.
 */

#include "Jupiter.h"
#include "DataBuffer.h"

namespace Jupiter
{
	template<typename T> class List
	{
	public:

		/**
		* @brief Gets the data at a specified index.
		*
		* @param index Index of the data to get.
		* @return Data stored at the specified index.
		*/
		virtual T *get(size_t index) const = 0;

		/**
		* @brief Removes the n'th Node in the list, and returns its contents.
		*
		* @param n Index of the node to remove.
		* @return Contents of the node removed.
		*/
		virtual T *remove(size_t n) = 0;

		/**
		* @brief Adds data to the list at a specified index.
		*
		* @param data Data to add to the list.
		* @param index Position in the list to add the data to.
		*/
		virtual void add(T *data, size_t index) = 0;

		/**
		* @brief Adds data to the list in an efficient manner.
		*
		* @param data Data to add to the list.
		*/
		virtual void add(T *data) = 0;

		/**
		* @brief Returns the size of the list.
		*
		* @return Number of nodes in the list.
		*/
		size_t size() const { return Jupiter::List<T>::length; };

	protected:
		size_t length = 0; /** Length (size) of the list. Returned by size(). Must be managed by extending classes. */
	};
}

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::List>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::List<Y> *data)
	{
		buffer->push<size_t>(data->size());
		for (size_t index = 0; index != data->size(); index++)
			buffer->push<Y>(*data->get(index));
	};
};

#endif // _LIST_H_HEADER