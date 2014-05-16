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

#if !defined _IRC_H_HEADER
#define _IRC_H_HEADER

/**
 * @file IRC.h
 * @brief Defines IRC protocol constants.
 */

#include "Jupiter.h"

/** Special Character Definitions */

#define IRCCTCP "\001" /** IRC CTCP character contained in a string literal */
#define IRCBOLD "\002" /** IRC bold character contained in a string literal */
#define IRCCOLOR "\003" /** IRC color character contained in a string literal */
#define IRCNORMAL "\017" /** IRC normal character contained in a string literal */
#define IRCREVERSE "\026" /** IRC reverse character contained in a string literal */
#define IRCUNDERLINE "\037" /** IRC underline character contained in a string literal */

#if defined __cplusplus

namespace Jupiter
{
	namespace IRC
	{
		JUPITER_API extern const char CTCP; /** IRC CTCP character */
		JUPITER_API extern const char bold; /** IRC bold character */
		JUPITER_API extern const char color; /** IRC color character */
		JUPITER_API extern const char normal; /** IRC normal character */
		JUPITER_API extern const char reverse; /** IRC reverse character */
		JUPITER_API extern const char underline; /** IRC underline character */
	}
}

#endif // __cplusplus

#endif // _IRC_H_HEADER