/**
 * Copyright (C) 2013-2016 Jessica James.
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
		constexpr const char CTCP = 0x01; /** IRC CTCP character */
		constexpr const char bold = 0x02; /** IRC bold character */
		constexpr const char BOLD = Jupiter::IRC::bold;
		constexpr const char color = 0x03; /** IRC color character */
		constexpr const char COLOR = Jupiter::IRC::color;
		constexpr const char normal = 0x0F; /** IRC normal character */
		constexpr const char NORMAL = Jupiter::IRC::normal;
		constexpr const char reverse = 0x16; /** IRC reverse character */
		constexpr const char REVERSE = Jupiter::IRC::reverse;
		constexpr const char italicize = 0x1D; /** IRC italicize character */
		constexpr const char ITALICIZE = Jupiter::IRC::italicize;
		constexpr const char underline = 0x1F; /** IRC underline character */
		constexpr const char UNDERLINE = Jupiter::IRC::underline;
	}
}

#endif // __cplusplus

#endif // _IRC_H_HEADER