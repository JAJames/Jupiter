/**
 * Copyright (C) 2013-2015 Justin James.
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

Jupiter::TCPSocket &Jupiter::TCPSocket::operator=(Jupiter::TCPSocket &&source)
{
	Jupiter::Socket::operator=(std::move(source));
	return *this;
}

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

Jupiter::SecureTCPSocket &Jupiter::SecureTCPSocket::operator=(Jupiter::SecureTCPSocket &&source)
{
	Jupiter::SecureSocket::operator=(std::move(source));
	return *this;
}

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