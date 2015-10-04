/**
 * Copyright (C) 2013-2015 Justin James.
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
 * Written by Justin James <justin.aj@hotmail.com>
 */

#if !defined _THINKER_H_HEADER
#define _THINKER_H_HEADER

/**
 * @file Thinker.h
 * @brief Provides the Thinker abstract interface.
 */

#include "Jupiter.h"

namespace Jupiter
{
	/**
	* @brief Provides an abstract interface with a think() method.
	* The Thinker interface is intended for objects that need to be regularly
	* processed by some sort of Thinker manager. Thinkers should return 1 if no
	* real error occurs, but do no require future processing.
	*/
	class JUPITER_API Thinker
	{
	public:
		/**
		* @brief Makes an object think. Should return 0 if no error occurs.
		*
		* @return Error number.
		*/
		virtual int think() = 0;

		/**
		* @brief Virtual destructor for the "Thinker" interface. Does nothing.
		*/
		virtual ~Thinker() {}
	};

}

#endif // _THINKER_H_HEADER