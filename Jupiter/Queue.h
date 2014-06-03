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

#if !defined _QUEUE_H_HEADER
#define _QUEUE_H_HEADER

/**
 * @file Queue.h
 * @brief Provides a simple and efficient queue.
 */

#include "Jupiter.h"

namespace Jupiter
{
	/**
	* @brief Provides a simple and efficient queue.
	*/
	class JUPITER_API Queue
	{
	public:

		/**
		* @brief Adds data to the end of the Queue.
		*
		* @param data Pointer to the data to add.
		*/
		void enqueue(void *data);

		/**
		* @brief Removes and returns data from the front of the Queue.
		*
		* @return Data in the front of the Queue.
		*/
		void *dequeue();

		/**
		* @brief Returns the number of elements waiting for processing in the Queue.
		*
		* @return Number of elements in the Queue.
		*/
		size_t size() const;

		/**
		* @brief Default constructor for the Queue class.
		*/
		Queue();

		/**
		* @brief Destructor for the Queue class.
		* Note: This does not delete data which was added to the queue.
		*/
		~Queue();

	/** Private members */
	private:
		struct Data;
		Data *head;
		Data *end;
		size_t length;
	};

}

#endif // _QUEUE_H_HEADER