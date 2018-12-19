/**
 * Copyright (C) 2014-2016 Jessica James.
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

#if !defined _REHASH_H_HEADER
#define _REHASH_H_HEADER

/**
 * @file Rehash.h
 * @brief Provides a C++ rehash system, which is also accessable through a C interface.
 */

#include "Jupiter.h"

namespace Jupiter
{
	class JUPITER_API Rehashable
	{
	public:

		/**
		* @brief Rehashes an object's status.
		*
		* @return 0 if no error occurs, a postive integer if an error occurs, or a negative integer if an error occurs and the object should be removed.
		*/
		virtual int OnRehash() = 0;

		/**
		* @brief Fires when a non-zero value is returned by an object during a call to rehash().
		*
		* @param removed True if the object was removed from the rehashable objects list, false otherwise.
		* @return True if the object should be deleted, false otherwise.
		*/
		virtual bool OnBadRehash(bool removed) = 0;

		/**
		* @brief Default constructor for the Rehashable class.
		*/
		Rehashable();

		/**
		* @brief Copy constructor for the Rehashable class.
		*/
		Rehashable(const Rehashable &);

		/**
		* @brief Destructor for the Rehashable class.
		*/
		virtual ~Rehashable();

	}; // Jupiter::Rehashable class

	/**
	* @brief Calls OnRehash() on every rehashable object.
	*
	* @return Number of objects which returned an error.
	*/
	JUPITER_API size_t rehash();

	/**
	* @brief Fetches the number of rehashable objects.
	*
	* @return Number of rehashable objects.
	*/
	JUPITER_API size_t getRehashableCount();

	typedef int(*OnRehashFunctionType)(void);

	/**
	* @brief Adds a function to be called during the rehash process.
	*
	* @param function Function to call (@see Jupiter::Rehashable::OnRehash()).
	*/
	JUPITER_API void addOnRehash(OnRehashFunctionType in_function);

	/**
	* @brief Removes a function from the rehash list.
	*
	* @param function Function to remove.
	* @return True if a function was removed, false otherwise.
	*/
	JUPITER_API bool removeOnRehash(OnRehashFunctionType in_function);

	/**
	* @brief Removes all functions from the rehash list.
	* Note: This should be called during application clean-up.
	*
	* @return Number of functions removed.
	*/
	JUPITER_API size_t removeAllOnRehash();

} // Jupiter namespace

#endif // _REHASH_H_HEADER