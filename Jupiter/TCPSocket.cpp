/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include <utility> // std::move
#include "TCPSocket.h"

#if defined _WIN32
#include <WinSock2.h>
#else // _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#endif // _WIN32

void setSocketTCP(Jupiter::Socket *sock)
{
	sock->setType(SOCK_STREAM);
	sock->setProtocol(IPPROTO_IP);
}

/** TCPSocket Implementation */

Jupiter::TCPSocket::TCPSocket() : Socket()
{
	setSocketTCP(this);
}

Jupiter::TCPSocket::TCPSocket(size_t bufferSize) : Socket(bufferSize)
{
	setSocketTCP(this);
}

Jupiter::TCPSocket::TCPSocket(Jupiter::Socket &&source) : Socket(std::move(source))
{
	setSocketTCP(this);
}

/** SecureTCPSocket Implementation */

Jupiter::SecureTCPSocket::SecureTCPSocket() : SecureSocket()
{
	setSocketTCP(this);
}

Jupiter::SecureTCPSocket::SecureTCPSocket(size_t bufferSize) : SecureSocket(bufferSize)
{
	setSocketTCP(this);
}

Jupiter::SecureTCPSocket::SecureTCPSocket(Jupiter::Socket &&source) : SecureSocket(std::move(source))
{
	setSocketTCP(this);
}

Jupiter::SecureTCPSocket::SecureTCPSocket(Jupiter::SecureSocket &&source) : SecureSocket(std::move(source))
{
	setSocketTCP(this);
}