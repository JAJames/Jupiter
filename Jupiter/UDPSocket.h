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