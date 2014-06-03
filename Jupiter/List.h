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

#if !defined _LIST_H_HEADER
#define _LIST_H_HEADER

/**
 * @file List.h
 * @brief Provides a generic List interface.
 */

#include "Jupiter.h"

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

#endif // _LIST_H_HEADER