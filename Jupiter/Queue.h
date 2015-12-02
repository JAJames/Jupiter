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