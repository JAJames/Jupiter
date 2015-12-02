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

#if !defined _UDPSOCKET_H_HEADER
#define _UDPSOCKET_H_HEADER

/**
 * @file UDPSocket.h
 * @brief Provides UDP socket interaction.
 */

#include "Jupiter.h"
#include "Socket.h"

namespace Jupiter
{
	/**
	* @brief Provides UDP/IP support using sockets.
	* @see Socket
	*/
	class JUPITER_API UDPSocket : public Socket
	{
	public:
		UDPSocket &UDPSocket::operator=(UDPSocket &&source);
		UDPSocket();
		UDPSocket(const UDPSocket &) = delete;
		UDPSocket(size_t bufferSize);
		UDPSocket(Jupiter::Socket &&source);
	};

}

#endif // _UDPSOCKET_H_HEADER