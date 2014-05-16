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

Jupiter::UDPSocket::UDPSocket()
{
	this->setType(SOCK_DGRAM);
	this->setProtocol(IPPROTO_UDP);
}