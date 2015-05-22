/**
 * Copyright (C) 2013-2015 Justin James.
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