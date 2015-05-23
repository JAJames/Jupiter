/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include <cstdio>

#if defined _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
bool socketInit = false;
#else // _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
typedef int SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#endif // _WIN32

#include "Socket.h"
#include "Functions.h" // makestr()
#include "CString.h"

struct Jupiter::Socket::Data
{
	SOCKET rawSock;
	unsigned short port;
	Jupiter::CStringS host;
	char *buff;
	size_t bufflen;
	int sockType;
	int sockProto;
#if defined _WIN32
	unsigned long blockMode = 0;
#endif
	Data(char *buffer, size_t bufferSize);
	Data(const Data &);
	~Data();
};

Jupiter::Socket::Data::Data(char *buffer, size_t bufferSize)
{
	Jupiter::Socket::Data::buff = buffer;
	Jupiter::Socket::Data::bufflen = bufferSize;
	Jupiter::Socket::Data::port = 0;
	Jupiter::Socket::Data::rawSock = 0;
	Jupiter::Socket::Data::sockType = SOCK_RAW;
	Jupiter::Socket::Data::sockProto = IPPROTO_RAW;
	for (unsigned int i = 0; i < Jupiter::Socket::Data::bufflen; i++) Jupiter::Socket::Data::buff[i] = 0;
}

Jupiter::Socket::Data::Data(const Data &source)
{
	Jupiter::Socket::Data::bufflen = source.bufflen;
	Jupiter::Socket::Data::buff = new char[Jupiter::Socket::Data::bufflen];
	Jupiter::Socket::Data::port = source.port;
	Jupiter::Socket::Data::rawSock = source.rawSock;
	Jupiter::Socket::Data::sockType = source.sockType;
	Jupiter::Socket::Data::sockProto = source.sockProto;
	Jupiter::Socket::Data::host = source.host;
#if defined _WIN32
	Jupiter::Socket::Data::blockMode = source.blockMode;
#endif
	for (unsigned int i = 0; i < Jupiter::Socket::Data::bufflen; i++) Jupiter::Socket::Data::buff[i] = 0;
}

Jupiter::Socket::Data::~Data()
{
	if (Jupiter::Socket::Data::buff != nullptr) delete[] Jupiter::Socket::Data::buff;
}

Jupiter::Socket &Jupiter::Socket::operator=(Jupiter::Socket &&source)
{
	Jupiter::Socket::data_ = source.data_;
	source.data_ = nullptr;
	return *this;
}

Jupiter::Socket::Socket() : Jupiter::Socket::Socket(4096)
{
}

Jupiter::Socket::Socket(size_t bufferSize)
{
	Jupiter::Socket::data_ = new Jupiter::Socket::Data(new char[bufferSize], bufferSize);
}

Jupiter::Socket::Socket(Jupiter::Socket &&source)
{
	Jupiter::Socket::data_ = source.data_;
	source.data_ = nullptr;
}

Jupiter::Socket::Socket(char *buffer, size_t bufferSize)
{
	Jupiter::Socket::data_ = new Jupiter::Socket::Data(buffer, bufferSize);
}

Jupiter::Socket::~Socket()
{
	if (Jupiter::Socket::data_ != nullptr)
	{
		if (Jupiter::Socket::data_->rawSock > 0)
		{
			Jupiter::Socket::closeSocket();
		}
		delete Jupiter::Socket::data_;
	}
}

int Jupiter::Socket::getDescriptor() const
{
	return Jupiter::Socket::data_->rawSock;
}

void Jupiter::Socket::setDescriptor(int descriptor)
{
	Jupiter::Socket::data_->rawSock = descriptor;
}

void Jupiter::Socket::setType(int type)
{
	Jupiter::Socket::data_->sockType = type;
}

void Jupiter::Socket::setProtocol(int proto)
{
	Jupiter::Socket::data_->sockProto = proto;
}

int Jupiter::Socket::getType() const
{
	return Jupiter::Socket::data_->sockType;
}

int Jupiter::Socket::getProtocol() const
{
	return Jupiter::Socket::data_->sockProto;
}

bool Jupiter::Socket::connect(addrinfo *info)
{
#if defined _WIN32
	if (!socketInit && !Jupiter::Socket::init()) return false;
#endif // _WIN32
	Jupiter::Socket::data_->rawSock = socket(info->ai_family, Jupiter::Socket::data_->sockType, Jupiter::Socket::data_->sockProto);
	if (Jupiter::Socket::data_->rawSock == INVALID_SOCKET || (Jupiter::Socket::data_->sockType != SOCK_RAW && Jupiter::Socket::data_->sockProto != IPPROTO_RAW && ::connect(Jupiter::Socket::data_->rawSock, info->ai_addr, info->ai_addrlen) == SOCKET_ERROR)) return false;
	return true;
}

bool Jupiter::Socket::connect(const char *hostname, unsigned short iPort, const char *clientAddress, unsigned short clientPort)
{
#if defined _WIN32
	if (!socketInit && !Jupiter::Socket::init()) return false;
#endif // _WIN32
	Jupiter::Socket::data_->host.set(hostname);
	Jupiter::Socket::data_->port = iPort;
	int i = 0;
	static char portString[256];
	sprintf(portString, "%hu", Jupiter::Socket::data_->port);
	addrinfo *info = Jupiter::Socket::getAddrInfo(Jupiter::Socket::data_->host.c_str(), portString);
	while (info != nullptr)
	{
		addrinfo *ainfo = Jupiter::Socket::getAddrInfo(info, i);
		if (ainfo == nullptr)
		{
			Jupiter::Socket::freeAddrInfo(info);
			return false;
		}
		sockaddr *asock = ainfo->ai_addr;
		Jupiter::Socket::data_->rawSock = socket(ainfo->ai_family, Jupiter::Socket::data_->sockType, Jupiter::Socket::data_->sockProto);
		if (Jupiter::Socket::data_->rawSock == INVALID_SOCKET) continue;
		if (clientAddress != nullptr) this->bind(clientAddress, clientPort, false);
		if (::connect(Jupiter::Socket::data_->rawSock, asock, ainfo->ai_addrlen) == SOCKET_ERROR)
		{
			i++;
			continue;
		}
		Jupiter::Socket::freeAddrInfo(info);
		return true;
	}
	return false;
}

bool Jupiter::Socket::bind(const char *hostname, unsigned short iPort, bool andListen)
{
#if defined _WIN32
	if (!socketInit && !Jupiter::Socket::init()) return false;
#endif // _WIN32
	Jupiter::Socket::data_->host.set(hostname);
	Jupiter::Socket::data_->port = iPort;
	int i = 0;
	static char portString[256];
	sprintf(portString, "%hu", Jupiter::Socket::data_->port);
	addrinfo *info = Jupiter::Socket::getAddrInfo(Jupiter::Socket::data_->host.c_str(), portString);
	while (info != nullptr)
	{
		addrinfo *ainfo = Jupiter::Socket::getAddrInfo(info, i);
		if (ainfo == nullptr)
		{
			Jupiter::Socket::freeAddrInfo(info);
			return false;
		}
		Jupiter::Socket::data_->rawSock = socket(ainfo->ai_family, Jupiter::Socket::data_->sockType, Jupiter::Socket::data_->sockProto);
		if (Jupiter::Socket::data_->rawSock == INVALID_SOCKET || ::bind(Jupiter::Socket::data_->rawSock, ainfo->ai_addr, ainfo->ai_addrlen) == SOCKET_ERROR)
		{
			i++;
			continue;
		}
		Jupiter::Socket::freeAddrInfo(info);
		if (andListen && Jupiter::Socket::data_->sockType == SOCK_STREAM && listen(Jupiter::Socket::data_->rawSock, SOMAXCONN) == SOCKET_ERROR) return false;
		return true;
	}
	return false;
}

void Jupiter::Socket::closeSocket()
{
	if (Jupiter::Socket::data_ != nullptr)
	{
		shutdown(Jupiter::Socket::data_->rawSock, 2);
#if defined _WIN32
		closesocket(Jupiter::Socket::data_->rawSock);
#else // _WIN32
		close(Jupiter::Socket::data_->rawSock);
#endif // _WIN32
		Jupiter::Socket::data_->rawSock = 0;
	}
}

addrinfo *Jupiter::Socket::getAddrInfo(const char *hostname, const char *port) // static
{
	addrinfo *ptr;
	if (getaddrinfo(hostname, port, nullptr, &ptr)) return nullptr;
	return ptr;
}

addrinfo *Jupiter::Socket::getStaticAddrInfo(const char *hostname, const char *port) // static
{
	static addrinfo *ptr = nullptr;
	if (ptr != nullptr) freeaddrinfo(ptr);
	ptr = Jupiter::Socket::getAddrInfo(hostname, port);
	return ptr;
}

void Jupiter::Socket::freeAddrInfo(addrinfo *info) // static
{
	freeaddrinfo(info);
}

addrinfo *Jupiter::Socket::getAddrInfo(addrinfo *addr, unsigned int result) // static
{
	addrinfo *ptr = addr;
	for (unsigned int i = 0; i != result && ptr != nullptr; i++) ptr = ptr->ai_next;
	return ptr;
}

char *Jupiter::Socket::resolveAddress(const addrinfo *addr) // static
{
	static char resolved[NI_MAXHOST];
	getnameinfo(addr->ai_addr, addr->ai_addrlen, resolved, NI_MAXHOST, 0, 0, NI_NUMERICHOST);
	return resolved;
}

char *Jupiter::Socket::resolveAddress(addrinfo *addr, unsigned int result) // static
{
	addrinfo *ptr = Jupiter::Socket::getAddrInfo(addr, result);
	if (ptr == nullptr) return nullptr;
	return Jupiter::Socket::resolveAddress(ptr);
}

char *Jupiter::Socket::resolveAddress(const char *hostname, unsigned int result) // static
{
	addrinfo *info = Jupiter::Socket::getAddrInfo(hostname, 0);
	if (info == nullptr) return nullptr;
	return Jupiter::Socket::resolveAddress(info, result);
}

char *Jupiter::Socket::resolveHostname(addrinfo *addr) // static
{
	static char resolved[NI_MAXHOST];
	getnameinfo(addr->ai_addr, addr->ai_addrlen, resolved, NI_MAXHOST, 0, 0, 0);
	return resolved;
}

char *Jupiter::Socket::resolveHostname(addrinfo *addr, unsigned int result) // static
{
	addrinfo *ptr = Jupiter::Socket::getAddrInfo(addr, result);
	if (ptr == nullptr) return nullptr;
	return Jupiter::Socket::resolveHostname(ptr);
}

char *Jupiter::Socket::resolveHostname(const char *hostname, unsigned int result) // static
{
	addrinfo *info = Jupiter::Socket::getAddrInfo(hostname, 0);
	if (info == nullptr) return nullptr;
	return Jupiter::Socket::resolveHostname(info, result);
}

uint32_t Jupiter::Socket::pton4(const char *str)
{
	in_addr r;
	if (inet_pton(AF_INET, str, &r) <= 0)
		return 0;
	return *reinterpret_cast<uint32_t *>(&r);
}

in_addr6 Jupiter::Socket::pton6(const char *str)
{
	in_addr6 r;
	if (inet_pton(AF_INET6, str, &r) <= 0)
		memset(&r, 0, sizeof(in_addr6));
	return r;
}

Jupiter::StringS Jupiter::Socket::ntop4(uint32_t ip)
{
	static char buf[16];
	if (inet_ntop(AF_INET, &ip, buf, sizeof(buf)) == nullptr)
		return Jupiter::StringS::empty;
	return Jupiter::String(buf);
}

Jupiter::StringS Jupiter::Socket::ntop6(in_addr6 ip)
{
	static char buf[46];
	if (inet_ntop(AF_INET6, &ip, buf, sizeof(buf)) == nullptr)
		return Jupiter::StringS::empty;
	return Jupiter::String(buf);
}

Jupiter::StringS Jupiter::Socket::ntop(void *ip, size_t size)
{
	switch (size)
	{
	case 4:
		return ntop4(*reinterpret_cast<uint32_t *>(ip));
	case 16:
		return ntop6(*reinterpret_cast<in_addr6 *>(ip));
	default:
		return Jupiter::StringS::empty;
	}
}

Jupiter::Socket *Jupiter::Socket::accept()
{
	sockaddr addr;
	int size = sizeof(addr);
	SOCKET tSock = ::accept(Socket::data_->rawSock, &addr, &size);
	if (tSock != INVALID_SOCKET)
	{
		char resolved[NI_MAXHOST];
		getnameinfo(&addr, size, resolved, NI_MAXHOST, 0, 0, NI_NUMERICHOST);
		Socket *r = new Socket(Jupiter::Socket::data_->bufflen);
		r->data_->rawSock = tSock;
		r->data_->sockType = Jupiter::Socket::data_->sockType;
		r->data_->sockProto = Jupiter::Socket::data_->sockProto;
		r->data_->host.set(resolved);
		switch (addr.sa_family)
		{
		case AF_INET:
			r->data_->port = ntohs(reinterpret_cast<sockaddr_in *>(&addr)->sin_port);
			break;
		case AF_INET6:
			r->data_->port = ntohs(reinterpret_cast<sockaddr_in6 *>(&addr)->sin6_port);
			break;
		default:
			r->data_->port = 0x00000000;
			break;
		}
		return r;
	}
	return nullptr;
}

bool Jupiter::Socket::setReadTimeout(unsigned long milliseconds)
{
#if defined _WIN32
	return setsockopt(Jupiter::Socket::data_->rawSock, SOL_SOCKET, SO_RCVTIMEO, (const char *)milliseconds, sizeof(milliseconds)) == 0;
#else // _WIN32
	timeval time;
	time.tv_sec = milliseconds / 1000;
	time.tv_usec = (milliseconds % 1000) * 1000;
	return setsockopt(Jupiter::Socket::data_->rawSock, SOL_SOCKET, SO_RCVTIMEO, (const void *) &time, sizeof(time)) == 0;
#endif // _WIN32
}

bool Jupiter::Socket::setSendTimeout(unsigned long milliseconds)
{
#if defined _WIN32
	return setsockopt(Jupiter::Socket::data_->rawSock, SOL_SOCKET, SO_SNDTIMEO, (const char *)milliseconds, sizeof(milliseconds)) == 0;
#else // _WIN32
	timeval time;
	time.tv_sec = milliseconds / 1000;
	time.tv_usec = (milliseconds % 1000) * 1000;
	return setsockopt(Jupiter::Socket::data_->rawSock, SOL_SOCKET, SO_SNDTIMEO, (const void *) &time, sizeof(time)) == 0;
#endif // _WIN32
}

bool Jupiter::Socket::setTimeout(unsigned long milliseconds)
{
	if (Jupiter::Socket::setReadTimeout(milliseconds) && Jupiter::Socket::setSendTimeout(milliseconds)) return true;
	return false;
}

bool Jupiter::Socket::setBlocking(bool mode)
{
#if defined _WIN32
	Jupiter::Socket::data_->blockMode = !mode;
	return ioctlsocket(Jupiter::Socket::data_->rawSock, FIONBIO, &Jupiter::Socket::data_->blockMode) == 0;
#else // _WIN32
	int flags = fcntl(Jupiter::Socket::data_->rawSock, F_GETFL, 0);
	if (flags < 0) return 0;
	flags = mode ? (flags|O_NONBLOCK) : (flags&~O_NONBLOCK);
	return fcntl(Jupiter::Socket::data_->rawSock, F_SETFL, flags) == 0;
#endif // _WIN32
}

bool Jupiter::Socket::getBlockingMode() const
{
#if defined _WIN32
	return !Jupiter::Socket::data_->blockMode;
#else // _WIN32
	int flags = fcntl(sock_fd, F_GETFL, 0);
	if (flags == -1) return false;
	return !(flags & O_NONBLOCK);
#endif
}

const Jupiter::ReadableString &Jupiter::Socket::getHostname() const
{
	return Jupiter::Socket::data_->host;
}

const char *Jupiter::Socket::getHost() const
{
	return Jupiter::Socket::data_->host.c_str();
}

unsigned short Jupiter::Socket::getPort() const
{
	return Jupiter::Socket::data_->port;
}

const char *Jupiter::Socket::getBuffer() const
{
	return Jupiter::Socket::data_->buff;
}

void Jupiter::Socket::setBuffer(char *buffer, size_t size)
{
	if (Jupiter::Socket::data_->buff != nullptr) delete[] Jupiter::Socket::data_->buff;
	Jupiter::Socket::data_->bufflen = size;
	Jupiter::Socket::data_->buff = buffer;
}

char *Jupiter::Socket::setBufferSize(size_t size)
{
	if (Jupiter::Socket::data_->buff != nullptr) delete[] Jupiter::Socket::data_->buff;
	Jupiter::Socket::data_->bufflen = size;
	Jupiter::Socket::data_->buff = new char[Jupiter::Socket::data_->bufflen];
	return Jupiter::Socket::data_->buff;
}

size_t Jupiter::Socket::getBufferSize() const
{
	return Jupiter::Socket::data_->bufflen;
}

const char *Jupiter::Socket::getData()
{
	if (this->recv() > 0) return this->getBuffer();
	return nullptr;
}

const char *Jupiter::Socket::getLocalHostname() // static
{
	static char localHostname[NI_MAXHOST];
	gethostname(localHostname, NI_MAXHOST);
	return localHostname;
}

void Jupiter::Socket::clearBuffer()
{
	for (unsigned int i = 0; i < Jupiter::Socket::data_->bufflen; i++) Jupiter::Socket::data_->buff[i] = 0;
}

int Jupiter::Socket::send(const char *data, size_t datalen)
{
	return ::send(Jupiter::Socket::data_->rawSock, data, datalen, 0);
}

int Jupiter::Socket::send(const Jupiter::ReadableString &str)
{
	return this->send(str.ptr(), str.size());
}

int Jupiter::Socket::send(const char *msg)
{
	return this->send(msg, strlen(msg));
}

int Jupiter::Socket::sendTo(const addrinfo *info, const char *data, size_t datalen)
{
	return sendto(Jupiter::Socket::data_->rawSock, data, datalen, 0, info->ai_addr, info->ai_addrlen);
}

int Jupiter::Socket::sendTo(const addrinfo *info, const char *msg)
{
	return sendto(Jupiter::Socket::data_->rawSock, msg, strlen(msg), 0, info->ai_addr, info->ai_addrlen);
}

int Jupiter::Socket::sendData(const char *data, size_t datalen)
{
	return this->send(data, datalen);
}

int Jupiter::Socket::sendData(const char *msg)
{
	return this->send(msg);
}

int Jupiter::Socket::peek()
{
	int r = ::recv(Jupiter::Socket::data_->rawSock, Jupiter::Socket::data_->buff, Jupiter::Socket::data_->bufflen - 1, MSG_PEEK);
	if (r >= 0) Jupiter::Socket::data_->buff[r] = 0;
	return r;
}

int Jupiter::Socket::peekFrom(addrinfo *info)
{
	if (info == nullptr) return recvfrom(Jupiter::Socket::data_->rawSock, Jupiter::Socket::data_->buff, Jupiter::Socket::data_->bufflen - 1, 0, nullptr, nullptr);
	socklen_t len = info->ai_addrlen;
	if (len <= 0)
	{
		info->ai_addr = new sockaddr();
		len = sizeof(sockaddr);
	}
	int r = recvfrom(Jupiter::Socket::data_->rawSock, Jupiter::Socket::data_->buff, Jupiter::Socket::data_->bufflen - 1, MSG_PEEK, info->ai_addr, &len);
	if (r >= 0)
	{
		Jupiter::Socket::data_->buff[r] = 0;
		info->ai_addrlen = len;
		info->ai_canonname = nullptr;
		info->ai_family = info->ai_addr->sa_family;
		info->ai_flags = 0;
		info->ai_next = nullptr;
		info->ai_protocol = this->getProtocol();
		info->ai_socktype = this->getType();
	}
	return r;
}

int Jupiter::Socket::recv()
{
	int r = ::recv(Jupiter::Socket::data_->rawSock, Jupiter::Socket::data_->buff, Jupiter::Socket::data_->bufflen - 1, 0);
	if (r >= 0) Jupiter::Socket::data_->buff[r] = 0;
	return r;
}

int Jupiter::Socket::recvFrom(addrinfo *info)
{
	if (info == nullptr) return recvfrom(Jupiter::Socket::data_->rawSock, Jupiter::Socket::data_->buff, Jupiter::Socket::data_->bufflen - 1, 0, nullptr, nullptr);
	memset(info, 0, sizeof(addrinfo));
	info->ai_addr = new sockaddr();
	socklen_t len = sizeof(sockaddr);
	int r = recvfrom(Jupiter::Socket::data_->rawSock, Jupiter::Socket::data_->buff, Jupiter::Socket::data_->bufflen-1, 0, info->ai_addr, &len);
	if (r >= 0)
	{
		Jupiter::Socket::data_->buff[r] = 0;
		info->ai_addrlen = len;
		info->ai_family = info->ai_addr->sa_family;
		info->ai_protocol = this->getProtocol();
		info->ai_socktype = this->getType();
	}
	return r;
}

int Jupiter::Socket::receive()
{
	return this->recv();
}

int Jupiter::Socket::getLastError() // static
{
#if defined _WIN32
	int lastError = WSAGetLastError();
#else // _WIN32
	int lastError = errno;
#endif // _WIN32
    return lastError;
}

bool Jupiter::Socket::init() // static
{
#if defined _WIN32 // _WIN32
	WSADATA wsadata;
	if (WSAStartup(0x0202, &wsadata)) return false;
	if (wsadata.wVersion != 0x0202)
	{
		WSACleanup();
		return false;
	}
	socketInit = true;
#endif // _WIN32
	return true;
}

bool Jupiter::Socket::cleanup() // static
{
#if defined _WIN32 // _WIN32
	WSACleanup();
#endif // _WIN32
	return true;
}