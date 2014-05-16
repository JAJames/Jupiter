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