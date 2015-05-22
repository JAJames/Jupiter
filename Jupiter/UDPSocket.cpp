/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include "UDPSocket.h"

#if defined _WIN32
#include <WinSock2.h>
#else // _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#endif // _WIN32

void setSocketUDP(Jupiter::Socket *sock)
{
	sock->setType(SOCK_DGRAM);
	sock->setProtocol(IPPROTO_UDP);
}

/** UDPSocket Implementation */

Jupiter::UDPSocket &Jupiter::UDPSocket::operator=(Jupiter::UDPSocket &&source)
{
	Jupiter::Socket::operator=(std::move(source));
	return *this;
}

Jupiter::UDPSocket::UDPSocket() : Socket()
{
	setSocketUDP(this);
}

Jupiter::UDPSocket::UDPSocket(size_t bufferSize) : Socket(bufferSize)
{
	setSocketUDP(this);
}

Jupiter::UDPSocket::UDPSocket(Jupiter::Socket &&source) : Socket(std::move(source))
{
	setSocketUDP(this);
}