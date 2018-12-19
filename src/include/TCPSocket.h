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

#if !defined _TCPSOCKET_H_HEADER
#define _TCPSOCKET_H_HEADER

/**
 * @file TCPSocket.h
 * @brief Provides TCP socket interaction.
 */

#include "Jupiter.h"
#include "Socket.h"
#include "SecureSocket.h"

namespace Jupiter
{
	/**
	* @brief Provides TCP/IP support using sockets.
	* @see Socket
	*/
	class JUPITER_API TCPSocket : public Socket
	{
	public:
		TCPSocket &TCPSocket::operator=(TCPSocket &&source);
		TCPSocket();
		TCPSocket(const TCPSocket &) = delete;
		TCPSocket(size_t bufferSize);
		TCPSocket(Jupiter::Socket &&source);
	};

	/**
	* @brief Provides TCP/IP support using sockets and OpenSSL.
	* @see SecureSocket
	*/
	class JUPITER_API SecureTCPSocket : public SecureSocket
	{
	public:
		SecureTCPSocket &SecureTCPSocket::operator=(SecureTCPSocket &&source);
		SecureTCPSocket();
		SecureTCPSocket(const SecureTCPSocket &) = delete;
		SecureTCPSocket(size_t bufferSize);
		SecureTCPSocket(Jupiter::Socket &&source);
		SecureTCPSocket(Jupiter::SecureSocket &&source);
	};

}

#endif // _TCPSOCKET_H_HEADER