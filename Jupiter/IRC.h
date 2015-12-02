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
#define IRCITALICIZE "\035" /** IRC italicize character contained in a string literal */
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
		JUPITER_API extern const char italicize; /** IRC italicize character */
		JUPITER_API extern const char underline; /** IRC underline character */
	}
}

#endif // __cplusplus

#endif // _IRC_H_HEADER