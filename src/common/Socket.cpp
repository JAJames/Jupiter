/**
 * Copyright (C) 2013-2016 Jessica James.
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
#define INVALID_SOCKET (Jupiter::Socket::SocketType)(~0)
#define SOCKET_ERROR (-1)
#endif // _WIN32

#include "Socket.h"
#include "Functions.h"

/** Narrowing conversions; I just don't want to explicitly static_cast parameters to methods that might vary by platform */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4267)
#endif

constexpr size_t s_initial_buffer_size = 512;

Jupiter::Socket::Buffer::Buffer()
	: m_buffer{ ::operator new(s_initial_buffer_size) },
	m_buffer_capacity{ s_initial_buffer_size },
	m_buffer_size{0} {
}

Jupiter::Socket::Buffer::~Buffer() {
	::operator delete(m_buffer);
}

void Jupiter::Socket::Buffer::set_length(size_t in_length) {
	m_buffer_size = in_length;
}

size_t Jupiter::Socket::Buffer::capacity() const {
	return m_buffer_capacity;
}

size_t Jupiter::Socket::Buffer::size() const {
	return m_buffer_size;
}

void Jupiter::Socket::Buffer::reserve(size_t new_capacity) {
	if (capacity() == new_capacity // Skip when already this size
		|| new_capacity == 0 // Don't allow empty buffers, results in 0 from recv
		|| new_capacity < m_buffer_size) { // Don't shrink smaller than what we require
		return;
	}

	// We're gonna change the buffer one way or another; update our capacity
	m_buffer_capacity = new_capacity;

	if (m_buffer_size == 0) {
		// Nothing to copy; easy mode
		::operator delete(m_buffer);
		m_buffer = ::operator new(new_capacity);
		return;
	}

	// Backup current buffer data
	void* old_buffer = m_buffer;

	// Create new buffer
	m_buffer = ::operator new(new_capacity);
	std::memcpy(m_buffer, old_buffer, m_buffer_size);

	// Nuke the old buffer
	::operator delete(m_buffer);
}

void Jupiter::Socket::Buffer::clear() {
	m_buffer_size = 0;
}

std::string_view Jupiter::Socket::Buffer::view() const {
	return { static_cast<char*>(m_buffer), m_buffer_size };
}

void* Jupiter::Socket::Buffer::data() const {
	return m_buffer;
}

char* Jupiter::Socket::Buffer::chr_data() const {
	return static_cast<char*>(m_buffer);
}

struct Jupiter::Socket::Data {
	Jupiter::Socket::Buffer buffer;
	SocketType rawSock = INVALID_SOCKET;
	unsigned short remote_port = 0;
	unsigned short bound_port = 0;
	std::string remote_host;
	std::string bound_host;
	int sockType = SOCK_RAW;
	int sockProto = IPPROTO_RAW;
	bool is_shutdown = false;
#if defined _WIN32
	unsigned long blockMode = 0;
#endif
	Data(size_t buffer_size);
	Data(const Data &);
};

Jupiter::Socket::Data::Data(size_t buffer_size) {
	Jupiter::Socket::Data::buffer.reserve(buffer_size);
}

Jupiter::Socket::Data::Data(const Data &source) {
	Jupiter::Socket::Data::buffer = source.buffer;
	Jupiter::Socket::Data::remote_port = source.remote_port;
	Jupiter::Socket::Data::bound_port = source.bound_port;
	Jupiter::Socket::Data::rawSock = source.rawSock;
	Jupiter::Socket::Data::sockType = source.sockType;
	Jupiter::Socket::Data::sockProto = source.sockProto;
	Jupiter::Socket::Data::remote_host = source.remote_host;
	Jupiter::Socket::Data::bound_host = source.bound_host;
#if defined _WIN32
	Jupiter::Socket::Data::blockMode = source.blockMode;
#endif
}

Jupiter::Socket &Jupiter::Socket::operator=(Jupiter::Socket &&source) {
	m_data = source.m_data;
	source.m_data = nullptr;
	return *this;
}

Jupiter::Socket::Socket() : Jupiter::Socket::Socket(512) {
}

Jupiter::Socket::Socket(size_t bufferSize) {
	m_data = new Jupiter::Socket::Data(bufferSize);
}

Jupiter::Socket::Socket(Jupiter::Socket &&source) {
	m_data = source.m_data;
	source.m_data = nullptr;
}

Jupiter::Socket::~Socket() {
	if (m_data != nullptr) {
		if (m_data->rawSock > 0)
			Jupiter::Socket::close();
		delete m_data;
	}
}

void Jupiter::Socket::setType(int type) {
	m_data->sockType = type;
}

void Jupiter::Socket::setProtocol(int proto) {
	m_data->sockProto = proto;
}

int Jupiter::Socket::getType() const {
	return m_data->sockType;
}

int Jupiter::Socket::getProtocol() const {
	return m_data->sockProto;
}

bool Jupiter::Socket::connect(addrinfo *info) {
#if defined _WIN32
	if (!socketInit && !Jupiter::Socket::init()) {
		return false;
	}
#endif // _WIN32

	m_data->rawSock = socket(info->ai_family, m_data->sockType, m_data->sockProto);
	
	if (m_data->rawSock == INVALID_SOCKET
		|| (m_data->sockType != SOCK_RAW && m_data->sockProto != IPPROTO_RAW && ::connect(m_data->rawSock, info->ai_addr, info->ai_addrlen) == SOCKET_ERROR))
		return false;

	return true;
}

bool Jupiter::Socket::connect(const char *hostname, unsigned short iPort, const char *clientAddress, unsigned short clientPort) {
#if defined _WIN32
	if (!socketInit && !Jupiter::Socket::init())
		return false;
#endif // _WIN32
	m_data->remote_host = hostname;
	m_data->remote_port = iPort;
	addrinfo *info_head = Jupiter::Socket::getAddrInfo(m_data->remote_host.c_str(), std::to_string(m_data->remote_port).c_str());
	if (info_head != nullptr) {
		addrinfo *info = info_head;
		do {
			if (clientAddress != nullptr) {
				// bind will initialize our socket
				if (Jupiter::Socket::bind(clientAddress, clientPort, false) == false)
					break;
			}
			else {
				m_data->rawSock = socket(info->ai_family, m_data->sockType, m_data->sockProto);
				if (m_data->rawSock == INVALID_SOCKET) {
					info = info->ai_next;
					continue;
				}
			}

			if (::connect(m_data->rawSock, info->ai_addr, info->ai_addrlen) == SOCKET_ERROR) {
#if defined _WIN32
				::closesocket(m_data->rawSock);
#else // _WIN32
				::close(m_data->rawSock);
#endif // WIN32
				m_data->rawSock = INVALID_SOCKET;
				info = info->ai_next;
				continue;
			}

			Jupiter::Socket::freeAddrInfo(info_head);
			return true;
		} while (info != nullptr);
		Jupiter::Socket::freeAddrInfo(info_head);
	}
	return false;
}

bool Jupiter::Socket::bind(const char *hostname, unsigned short iPort, bool andListen) {
#if defined _WIN32
	if (!socketInit && !Jupiter::Socket::init()) {
		return false;
	}
#endif // _WIN32
	m_data->bound_host = hostname;
	m_data->bound_port = iPort;
	addrinfo *info_head = Jupiter::Socket::getAddrInfo(m_data->bound_host.c_str(), std::to_string(m_data->bound_port).c_str());
	if (info_head != nullptr) {
		addrinfo *info = info_head;
		do {
			m_data->rawSock = socket(info->ai_family, m_data->sockType, m_data->sockProto);
			if (m_data->rawSock == INVALID_SOCKET) {
				info = info->ai_next;
				continue;
			}

			if (::bind(m_data->rawSock, info->ai_addr, info->ai_addrlen) == SOCKET_ERROR) {
#if defined _WIN32
				::closesocket(m_data->rawSock);
#else // _WIN32
				::close(m_data->rawSock);
#endif // WIN32
				m_data->rawSock = INVALID_SOCKET;
				info = info->ai_next;
				continue;
			}

			Jupiter::Socket::freeAddrInfo(info_head);
			if (andListen && m_data->sockType == SOCK_STREAM && ::listen(m_data->rawSock, SOMAXCONN) == SOCKET_ERROR) {
				return false;
			}

			return true;
		} while (info != nullptr);
		Jupiter::Socket::freeAddrInfo(info_head);
	}
	return false;
}

void Jupiter::Socket::shutdown() {
	if (m_data != nullptr) {
		::shutdown(m_data->rawSock, 2);
		m_data->is_shutdown = true;
	}
}

void Jupiter::Socket::close() {
	if (m_data != nullptr) {
		if (m_data->is_shutdown == false)
			this->shutdown();
#if defined _WIN32
		::closesocket(m_data->rawSock);
#else // _WIN32
		::close(m_data->rawSock);
#endif // _WIN32
		m_data->rawSock = 0;
	}
}

bool Jupiter::Socket::isShutdown() const {
	return m_data->is_shutdown;
}

addrinfo* Jupiter::Socket::getAddrInfo(const char *hostname, const char *port) { // static
	addrinfo *ptr{};
	getaddrinfo(hostname, port, nullptr, &ptr);
	return ptr;
}

void Jupiter::Socket::freeAddrInfo(addrinfo *info) { // static
	freeaddrinfo(info);
}

addrinfo *Jupiter::Socket::getAddrInfo(addrinfo *addr, unsigned int result) { // static
	addrinfo* ptr = addr;
	while (ptr != nullptr && result != 0) {
		ptr = ptr->ai_next;
		--result;
	}

	return ptr;
}

std::string Jupiter::Socket::resolveAddress(const addrinfo *addr) { // static
	char resolved[NI_MAXHOST];
	getnameinfo(addr->ai_addr, addr->ai_addrlen, resolved, NI_MAXHOST, 0, 0, NI_NUMERICHOST);
	return resolved;
}

std::string Jupiter::Socket::resolveAddress(addrinfo *addr, unsigned int result) { // static
	addrinfo *ptr = Jupiter::Socket::getAddrInfo(addr, result);
	if (ptr == nullptr) {
		return {};
	}

	return Jupiter::Socket::resolveAddress(ptr);
}

std::string Jupiter::Socket::resolveAddress(const char *hostname, unsigned int result) { // static
	addrinfo *info = Jupiter::Socket::getAddrInfo(hostname, 0);
	if (info == nullptr) {
		return {};
	};

	return Jupiter::Socket::resolveAddress(info, result);
}

std::string Jupiter::Socket::resolveHostname(addrinfo *addr) { // static
	char resolved[NI_MAXHOST];
	getnameinfo(addr->ai_addr, addr->ai_addrlen, resolved, sizeof(resolved), 0, 0, 0);
	return resolved;
}

std::string Jupiter::Socket::resolveHostname(addrinfo *addr, unsigned int result) { // static
	addrinfo *ptr = Jupiter::Socket::getAddrInfo(addr, result);
	if (ptr == nullptr) {
		return {};
	}

	return Jupiter::Socket::resolveHostname(ptr);
}

std::string Jupiter::Socket::resolveHostname(const char *hostname, unsigned int result) { // static
	addrinfo *info = Jupiter::Socket::getAddrInfo(hostname, 0);
	if (info == nullptr) {
		return {};
	}

	return Jupiter::Socket::resolveHostname(info, result);
}

uint32_t Jupiter::Socket::pton4(const char *str) {
	in_addr r;
	if (inet_pton(AF_INET, str, &r) <= 0)
		return 0;
	return *reinterpret_cast<uint32_t *>(&r);
}

in_addr6 Jupiter::Socket::pton6(const char *str) {
	in_addr6 r;
	if (inet_pton(AF_INET6, str, &r) <= 0)
		memset(&r, 0, sizeof(in_addr6));
	return r;
}

std::string Jupiter::Socket::ntop4(uint32_t ip) {
	static char buf[16];
	if (inet_ntop(AF_INET, &ip, buf, sizeof(buf)) == nullptr) {
		return {};
	}
	return std::string(buf);
}

std::string Jupiter::Socket::ntop6(in_addr6 ip) {
	static char buf[46];
	if (inet_ntop(AF_INET6, &ip, buf, sizeof(buf)) == nullptr) {
		return {};
	}
	return std::string(buf);
}

std::string Jupiter::Socket::ntop(void *ip, size_t size) {
	switch (size) {
	case 4:
		return ntop4(*reinterpret_cast<uint32_t *>(ip));
	case 16:
		return ntop6(*reinterpret_cast<in_addr6 *>(ip));
	default:
		return {};
	}
}

Jupiter::Socket* Jupiter::Socket::accept() {
	sockaddr addr;
	socklen_t size = sizeof(addr);
	SocketType tSock = ::accept(m_data->rawSock, &addr, &size);
	if (tSock != INVALID_SOCKET) {
		char resolved[NI_MAXHOST];
		char resolved_port[NI_MAXSERV];
		getnameinfo(&addr, size, resolved, sizeof(resolved), resolved_port, sizeof(resolved_port), NI_NUMERICHOST | NI_NUMERICSERV);
		Socket *result = new Socket(m_data->buffer.capacity());
		result->m_data->rawSock = tSock;
		result->m_data->sockType = m_data->sockType;
		result->m_data->sockProto = m_data->sockProto;
		result->m_data->remote_host = resolved;
		result->m_data->remote_port = static_cast<unsigned short>(Jupiter_strtoi(resolved_port, 10));
		return result;
	}
	return nullptr;
}

bool Jupiter::Socket::setReadTimeout(unsigned long milliseconds) {
#if defined _WIN32
	return setsockopt(m_data->rawSock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&milliseconds), sizeof(milliseconds)) == 0;
#else // _WIN32
	timeval time;
	time.tv_sec = milliseconds / 1000;
	time.tv_usec = (milliseconds % 1000) * 1000;
	return setsockopt(m_data->rawSock, SOL_SOCKET, SO_RCVTIMEO, (const void *) &time, sizeof(time)) == 0;
#endif // _WIN32
}

bool Jupiter::Socket::setSendTimeout(unsigned long milliseconds) {
#if defined _WIN32
	return setsockopt(m_data->rawSock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&milliseconds), sizeof(milliseconds)) == 0;
#else // _WIN32
	timeval time;
	time.tv_sec = milliseconds / 1000;
	time.tv_usec = (milliseconds % 1000) * 1000;
	return setsockopt(m_data->rawSock, SOL_SOCKET, SO_SNDTIMEO, (const void *) &time, sizeof(time)) == 0;
#endif // _WIN32
}

bool Jupiter::Socket::setTimeout(unsigned long milliseconds) {
	if (Jupiter::Socket::setReadTimeout(milliseconds) && Jupiter::Socket::setSendTimeout(milliseconds)) return true;
	return false;
}

bool Jupiter::Socket::setBlocking(bool mode) {
#if defined _WIN32
	m_data->blockMode = !mode;
	return ioctlsocket(m_data->rawSock, FIONBIO, &m_data->blockMode) == 0;
#else // _WIN32
	int flags = fcntl(m_data->rawSock, F_GETFL, 0);
	if (flags < 0) return 0;
	if (mode) {
		flags &= ~O_NONBLOCK;
	}
	else {
		flags |= O_NONBLOCK;
	}
	return fcntl(m_data->rawSock, F_SETFL, flags) == 0;
#endif // _WIN32
}

bool Jupiter::Socket::getBlockingMode() const {
#if defined _WIN32
	return !m_data->blockMode;
#else // _WIN32
	int flags = fcntl(m_data->rawSock, F_GETFL, 0);
	if (flags == -1) return false;
	return !(flags & O_NONBLOCK);
#endif
}

const std::string &Jupiter::Socket::getRemoteHostname() const {
	return m_data->remote_host;
}

const char *Jupiter::Socket::getRemoteHostnameC() const {
	return m_data->remote_host.c_str();
}

const std::string &Jupiter::Socket::getBoundHostname() const {
	return m_data->bound_host;
}

const char *Jupiter::Socket::getBoundHostnameC() const {
	return m_data->bound_host.c_str();
}

unsigned short Jupiter::Socket::getRemotePort() const {
	return m_data->remote_port;
}

unsigned short Jupiter::Socket::getBoundPort() const {
	return m_data->bound_port;
}

std::string_view Jupiter::Socket::getBuffer() const {
	return m_data->buffer.view();
}

size_t Jupiter::Socket::getBufferSize() const {
	return m_data->buffer.capacity();
}

std::string_view Jupiter::Socket::setBufferSize(size_t size) {
	m_data->buffer.reserve(size);
	return m_data->buffer.view();
}

std::string_view Jupiter::Socket::getData() {
	if (this->recv() <= 0) {
		m_data->buffer.clear();
	}
	return m_data->buffer.view();
}

const char *Jupiter::Socket::getLocalHostname() { // static
	static char localHostname[NI_MAXHOST];
	gethostname(localHostname, NI_MAXHOST);
	return localHostname;
}

void Jupiter::Socket::clearBuffer() {
	m_data->buffer.clear();
}

int Jupiter::Socket::send(const char *data, size_t datalen) {
	return ::send(m_data->rawSock, data, datalen, 0);
}

int Jupiter::Socket::send(std::string_view str) {
	return this->send(str.data(), str.size());
}

int Jupiter::Socket::send(const char *msg) {
	return this->send(msg, strlen(msg));
}

int Jupiter::Socket::sendTo(const addrinfo *info, const char *data, size_t datalen) {
	return sendto(m_data->rawSock, data, datalen, 0, info->ai_addr, info->ai_addrlen);
}

int Jupiter::Socket::sendTo(const addrinfo *info, const char *msg) {
	return sendto(m_data->rawSock, msg, strlen(msg), 0, info->ai_addr, info->ai_addrlen);
}

int Jupiter::Socket::peek() {
	m_data->buffer.clear();
	int r = ::recv(m_data->rawSock, m_data->buffer.chr_data(), m_data->buffer.capacity(), MSG_PEEK);
	if (r > 0)
		m_data->buffer.set_length(r);
	return r;
}

int Jupiter::Socket::peekFrom(addrinfo *info) {
	m_data->buffer.clear();
	if (info == nullptr)
		return recvfrom(m_data->rawSock, m_data->buffer.chr_data(), m_data->buffer.capacity(), MSG_PEEK, nullptr, nullptr);

	socklen_t len = info->ai_addrlen;
	if (len <= 0) {
		info->ai_addr = new sockaddr();
		len = sizeof(sockaddr);
	}
	int r = recvfrom(m_data->rawSock, m_data->buffer.chr_data(), m_data->buffer.capacity(), MSG_PEEK, info->ai_addr, &len);
	if (r >= 0) {
		info->ai_addrlen = len;
		info->ai_family = info->ai_addr->sa_family;
		info->ai_protocol = Jupiter::Socket::getProtocol();
		info->ai_socktype = Jupiter::Socket::getType();
	}
	return r;
}

int Jupiter::Socket::recv() {
	m_data->buffer.clear();
	int r = ::recv(m_data->rawSock, m_data->buffer.chr_data(), m_data->buffer.capacity(), 0);
	if (r > 0) {
		m_data->buffer.set_length(r);
	}
	return r;
}

int Jupiter::Socket::recvFrom(addrinfo* info) {
	m_data->buffer.clear();
	if (info == nullptr) {
		return recvfrom(m_data->rawSock, m_data->buffer.chr_data(), m_data->buffer.capacity(), 0, nullptr, nullptr);
	}

	socklen_t len = info->ai_addrlen;
	if (len <= 0) {
		info->ai_addr = new sockaddr();
		len = sizeof(sockaddr);
	}
	int r = recvfrom(m_data->rawSock, m_data->buffer.chr_data(), m_data->buffer.capacity(), 0, info->ai_addr, &len);
	if (r >= 0) {
		info->ai_addrlen = len;
		info->ai_family = info->ai_addr->sa_family;
		info->ai_protocol = Jupiter::Socket::getProtocol();
		info->ai_socktype = Jupiter::Socket::getType();
	}
	return r;
}

int Jupiter::Socket::receive() {
	return this->recv();
}

int Jupiter::Socket::getLastError() { // static
#if defined _WIN32
	int lastError = WSAGetLastError();
#else // _WIN32
	int lastError = errno;
#endif // _WIN32
    return lastError;
}

bool Jupiter::Socket::init() { // static
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

bool Jupiter::Socket::cleanup() { // static
#if defined _WIN32 // _WIN32
	WSACleanup();
#endif // _WIN32
	return true;
}

Jupiter::Socket::SocketType Jupiter::Socket::getDescriptor() const {
	return m_data->rawSock;
}

void Jupiter::Socket::setDescriptor(SocketType descriptor) {
	m_data->rawSock = descriptor;
}

Jupiter::Socket::Buffer &Jupiter::Socket::getInternalBuffer() const {
	return m_data->buffer;
}

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif
