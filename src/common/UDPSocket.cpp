/**
 * Copyright (C) 2013-2021 Jessica James.
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