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

#if !defined _SOCKET_H_HEADER
#define _SOCKET_H_HEADER

/**
 * @file Socket.h
 * @brief Provides cross-platform socket interaction.
 */

#include <cstring>
#include "Jupiter.h"
#include "Readable_String.h"
#include "String.h"

struct addrinfo;
struct in_addr6;

namespace Jupiter
{
	/**
	* @brief Provides a cross-platform interface for sockets.
	* Note: This class SHOULD NOT be instantiated by itself.
	* By default, this class creates a RAW socket, meaning TCP,
	* UDP, and other protocol support requires this class to be
	* extended. Extensions for UDP, TCP, and TLS/SSL are available
	* in UDPSocket.h, TCPSocket.h, and SecureSocket.h, respectively.
	*/
	class JUPITER_API Socket
	{
	public:

		/**
		* @brief Sets the socket type. Primarily intended for use by class extensions.
		*
		* @param type Socket type to use.
		*/
		void setType(int type);

		/**
		* @brief Sets the protocol to be used by the socket. Primarily intended for use by class extensions.
		*
		* @param proto Socket type to use.
		*/
		void setProtocol(int proto);

		/**
		* @brief Resolves and stores address information in an addrinfo struct.
		* The addrinfo returned by this function should be freed by freeAddrInfo().
		*
		* @param host String containing the hostname of the target.
		* @param port String containing the target port.
		* @return Pointer to a NULL-terminated linked list of addrinfo on success, nullptr otherwise.
		*/
		static addrinfo *getAddrInfo(const char *hostname, const char *port);

		/**
		* @brief Resolves and stores address information in an addrinfo struct.
		* Unlike the function getAddrInfo, this function stores the returned addrinfo
		* as static, meaning that any subsequent calls will overwrite the previous call.
		*
		* @param host String containing the hostname of the target.
		* @param port String containing the target port.
		* @return Pointer to a NULL-terminated linked list of addrinfo on success, nullptr otherwise.
		*/
		static addrinfo *getStaticAddrInfo(const char *hostname, const char *port);

		/**
		* @brief Frees the resources associated with an addrinfo struct.
		*
		* @param info Pointer to the addrinfo to be freed.
		*/
		static void freeAddrInfo(addrinfo *info);

		/**
		* @brief Returns the N'th addrinfo in a linked list of addrinfo.
		* This function does not allocate any memory.
		*
		* @param addr Head addrinfo pointer.
		* @param N Position of the addrinfo in the list to return.
		* @return N'th addrinfo pointed to by addr.
		*/
		static addrinfo *getAddrInfo(addrinfo *addr, unsigned int N);

		/**
		* @brief Returns the text representation of an addrinfo's stored address.
		*
		* @param addr Address info containing IP address.
		* @return String containing the text representation of the stored address on success, nullptr otherwise.
		*/
		static char *resolveAddress(const addrinfo *addr);

		/**
		* @brief Returns the text representation of an addrinfo's stored address.
		*
		* @param addr Address info containing IP address.
		* @param result Which result to return.
		* @return String containing the text representation of the stored address on success, nullptr otherwise.
		*/
		static char *resolveAddress(addrinfo *addr, unsigned int result);

		/**
		* @brief Resolves a hostname to an IP address.
		*
		* @param hostname Hostname to resolve.
		* @param result Which resolution result to return.
		* @return String containing the text representation of the resolved address on success, nullptr otherwise.
		*/
		static char *resolveAddress(const char *hostname, unsigned int result);

		/**
		* @brief Resolves an address to a hostname. (Reverse DNS)
		*
		* @param addr Address info containing IP address.
		* @return String containing the hostname of the stored address on success, nullptr otherwise.
		*/
		static char *resolveHostname(addrinfo *addr);

		/**
		* @brief Resolves an address to a hostname. (Reverse DNS)
		*
		* @param addr Address info containing IP address.
		* @param result Which result to return.
		* @return String containing the hostname of the stored address on success, nullptr otherwise.
		*/
		static char *resolveHostname(addrinfo *addr, unsigned int result);

		/**
		* @brief Resolves an address to a hostname. (Reverse DNS)
		*
		* @param hostname Address to resolve.
		* @param result Which resolution result to return.
		* @return String containing the hostname of the resolved address on success, nullptr otherwise.
		*/
		static char *resolveHostname(const char *hostname, unsigned int result);

		/**
		* @brief Reinterprets an IPv4 address as a 32-bit integer in network byte order.
		*
		* @param str String representation of an IPv4 address.
		* @return IPv4 address in network byte order.
		*/
		static uint32_t pton4(const char *str);

		/**
		* @brief Reinterprets an IPv6 address as a 128-bit integer in network byte order.
		*
		* @param str String representation of an IPv6 address.
		* @return IPv6 address in network byte order.
		*/
		static in_addr6 pton6(const char *str);

		/**
		* @brief Formats an IPv4 address in its string presentation format.
		*
		* @param ip IP to format
		* @return String containing the address's string presentation.
		*/
		static Jupiter::StringS ntop4(uint32_t ip);

		/**
		* @brief Formats an IPv6 address in its string presentation format.
		*
		* @param ip IP to format
		* @return String containing the address's string presentation.
		*/
		static Jupiter::StringS ntop6(in_addr6 ip);

		/**
		* @brief Formats an IPvX address in its string presentation format.
		* Currently supports: IPv4, IPv6.
		*
		* @param ip IP to format
		* @param size Size of the input type
		* @return String containing the address's string presentation.
		*/
		static Jupiter::StringS ntop(void *ip, size_t size);

		/**
		* @brief Interface to provide simple connection establishing.
		* Note: When using this method, the hostname and port are NOT stored; thus getHost(), getHostname(),
		* and getPort() should not be called. This is used solely to initialize the raw sockets.
		*
		* @param info addrinfo containing the information required to initialize the socket and attempt a connection.
		* @return True on success, false otherwise.
		*/
		virtual bool connect(addrinfo *info);

		/**
		* @brief Interface to provide simple connection establishing.
		*
		* @param hostname String containing hostname of server to connect to.
		* @param iPort Port to connect on.
		* @param clientHostname Optional parameter to specify the address for socket to bind to.
		* @param clientPort Optional parameter to specify the port for socket to bind to.
		* @return True on success, false otherwise.
		*/
		virtual bool connect(const char *hostname, unsigned short iPort, const char *clientHostname = nullptr, unsigned short clientPort = 0);

		/**
		* @brief Interface to provide simple binding to ports.
		*
		* @param hostname String containing hostname to bind to.
		* @param iPort Port to bind to.
		* @param andListen True if listen() should be called, false otherwise.
		* @return True on success, false otherwise.
		*/
		virtual bool bind(const char *hostname, unsigned short iPort, bool andListen = true);

		/**
		* @brief Accepts an incoming connection for the port bound to.
		*
		* @return A valid Socket on success, nullptr otherwise.
		*/
		virtual Socket *accept();

		/**
		* @brief Sets the timeout for recv() in milliseconds.
		* Note: This only affects blocking sockets.
		*
		* @param milliseconds Desired amount of time for a timeout for recv(), in milliseconds.
		* @return True on success, false otherwise.
		*/
		bool setReadTimeout(unsigned long milliseconds);

		/**
		* @brief Sets the timeout for send() in milliseconds.
		* Note: This only affects blocking sockets.
		*
		* @param milliseconds Desired amount of time for a timeout for send(), in milliseconds.
		* @return True on success, false otherwise.
		*/
		bool setSendTimeout(unsigned long milliseconds);

		/**
		* @brief Calls setReadTimeout() and setSendTimeout().
		* Note: This only affects blocking sockets.
		*
		* @param milliseconds Desired amount of time for timeout, in milliseconds.
		* @return True on complete success, false otherwise.
		*/
		bool setTimeout(unsigned long milliseconds);

		/**
		* @brief Sets the blocking mode.
		*
		* @param mode True if the socket should be blocking, false otherwise.
		* @return True on success, false otherwise.
		*/
		bool setBlocking(bool mode);

		/**
		* @brief Returns the blocking mode.
		*
		* @return True if the socket is blocking, false otherwise.
		*/
		bool getBlockingMode() const;

		/**
		* @brief Closes the socket.
		*/
		virtual void closeSocket();

		/**
		* @brief Returns the hostname that the socket is connected to.
		*
		* @return String containing the hostname.
		*/
		const Jupiter::ReadableString &getRemoteHostname() const;
		const char *getRemoteHostnameC() const;

		/**
		* @brief Returns the hostname that the socket is bound/listening to.
		*
		* @return String containing the hostname.
		*/
		const Jupiter::ReadableString &getBoundHostname() const;
		const char *getBoundHostnameC() const;

		/**
		* @brief Returns the port which the Socket is connected to.
		*
		* @return Port number.
		*/
		unsigned short getRemotePort() const;

		/**
		* @brief Returns the port which the Socket is bound/listening to.
		*
		* @return Port number.
		*/
		unsigned short getBoundPort() const;

		/**
		* @brief Returns the type of the socket.
		*
		* @return Type of the socket.
		*/
		int getType() const;

		/**
		* @brief Returns the protocol being used by the socket.
		*
		* @return Protocol of the socket.
		*/
		int getProtocol() const;

		/**
		* @brief Returns the buffer.
		*
		* @return Buffer.
		*/
		const Jupiter::ReadableString &getBuffer() const;

		/**
		* @brief Returns the size of the socket buffer.
		*
		* @return Size of the buffer.
		*/
		size_t getBufferSize() const;

		/**
		* @brief Expands the buffer, if necessary.
		*
		* @param size The size of the buffer to be made.
		* @return The newly created buffer.
		*/
		const Jupiter::ReadableString &setBufferSize(size_t size);

		/**
		* @brief Copies any new socket data to the buffer and returns it.
		*
		* @return Socket buffer if new data is successfully received, an empty buffer otherwise.
		*/
		const Jupiter::ReadableString &getData();

		/**
		* @brief Returns the local hostname of the local machine.
		* Note: This allocated static memory, and should be copied for storage.
		*
		* @return String containing the hostname.
		*/
		static const char *getLocalHostname();

		/**
		* @brief Sets every byte in the buffer to 0.
		*/
		void clearBuffer();

		/**
		* @brief Writes new data from the socket to the buffer, without removing it from the socket queue.
		* The data written by this function will always end with a null character, which is not counted in the returned value.
		*
		* @return Number of bytes written to buffer on success, SOCKET_ERROR (-1) otherwise.
		* Note: Any returned value less than or equal to 0 should be treated as an error.
		*/
		virtual int peek();

		/**
		* @brief Writes new data from the socket to the buffer, without removing it from the socket queue.
		* The data written by this function will always end with a null character, which is not counted in the returned value.
		* A valid addrinfo object can be created using generateInfo().
		*
		* @param info Optional pointer to a valid addrinfo object to store the sender's information in.
		* @return Number of bytes written to buffer on success, SOCKET_ERROR (-1) otherwise.
		* Note: Any returned value less than or equal to 0 should be treated as an error.
		*/
		virtual int peekFrom(addrinfo *info);

		/**
		* @brief Writes new data from the socket to the buffer.
		* The data written by this function will always end with a null character, which is not counted in the returned value.
		*
		* @return Number of bytes written to buffer on success, SOCKET_ERROR (-1) otherwise.
		* Note: Any returned value less than or equal to 0 should be treated as an error.
		*/
		virtual int recv();

		/**
		* @brief Writes new data from the socket to the buffer.
		* The data written by this function will always end with a null character, which is not counted in the returned value.
		* A valid addrinfo object can be created using Use generateInfo().
		*
		* @param info Optional pointer to a valid addrinfo object to store the sender's information in.
		* @return Number of bytes written to buffer on success, SOCKET_ERROR (-1) otherwise.
		* Note: Any returned value less than or equal to 0 should be treated as an error.
		*/
		virtual int recvFrom(addrinfo *info);

		int receive(); /** @see recv() */

		/**
		* @brief Sends data across the socket.
		*
		* @param data String containing the data to be send.
		* @param datalen The size of the data to be sent, in chars.
		* @return Number of bytes sent on success, SOCKET_ERROR (-1) otherwise.
		* Note: Any returned value less than or equal to 0 should be treated as an error.
		*/
		virtual int send(const char *data, size_t datalen);

		/**
		* @brief Sends a string of data across the socket.
		* Note: Internally, this just calls send(str.ptr(), str.size()).
		*
		* @param String containing the data to send.
		* @return Number of bytes sent on success, SOCKET_ERROR (-1) otherwise.
		* Note: Any returned value less than or equal to 0 should be treated as an error.
		*/
		int send(const Jupiter::ReadableString &str);

		/**
		* @brief Sends a null-terminated string of data across the socket.
		* Note: Internally, this just calls send(msg, strlen(msg)).
		*
		* @param String containing the null-terminated data to send.
		* @return Number of bytes sent on success, SOCKET_ERROR (-1) otherwise.
		* Note: Any returned value less than or equal to 0 should be treated as an error.
		*/
		int send(const char *msg);

		/**
		* @brief Sends data across the socket.
		*
		* @param info addrinfo containing the address information of the recipient.
		* @param data String containing the data to be send.
		* @param datalen The size of the data to be sent, in chars.
		* @return Number of bytes sent on success, SOCKET_ERROR (-1) otherwise.
		* Note: Any returned value less than or equal to 0 should be treated as an error.
		*/
		virtual int sendTo(const addrinfo *info, const char *data, size_t datalen);

		/**
		* @brief Sends a null-terminated string of data across the socket.
		*
		* @param info addrinfo containing the address information of the recipient.
		* @param String containing the null-terminated data to send.
		* @return Number of bytes sent on success, SOCKET_ERROR (-1) otherwise.
		* Note: Any returned value less than or equal to 0 should be treated as an error.
		*/
		virtual int sendTo(const addrinfo *info, const char *msg);

		/**
		* @brief Returns the last error.
		* On Windows, this currently just calls and returns WSAGetLastError().
		* On other operating systems (UNIX), this returns errno.
		* @return Error code.
		*/
		static int getLastError();

		/**
		* @brief Initializes any one-time background requirements for the Socket class to operate.
		* In particular, this initializes WSA on Windows, required for any Socket operations.
		* On Linux, however, this simply returns true.
		* @return True on success, false otherwise.
		*/
		static bool init();

		/**
		* @brief Cleans up background requirements for the Socket class to operate.
		* In particular, this cleans up WSA on Windows.
		* On Linux, however, this simply returns true.
		* @return True on success, false otherwise.
		*/
		static bool cleanup();

		/**
		* @brief Move assignment operator for the Socket class
		*
		* @param source Socket to move data from
		* @return Reference to this socket.
		*/
		Socket &Socket::operator=(Socket &&source);

		/**
		* @brief Default constructor for the Socket class.
		* This constructor will set the buffer size to 4096 chars.
		*/
		Socket();

		/**
		* @brief Copying a Socket is forbidden.
		*/
		Socket(const Socket &) = delete;

		/**
		* @brief Constructor for the Socket class which allows for the setting of a specific buffer size.
		*
		* @param bufferSize Size of the buffer to allocate, measured in chars.
		*/
		Socket(size_t bufferSize);

		/**
		* @brief Move constructor for the Socket class.
		*
		* @param Source Socket to move data from.
		*/
		Socket(Socket &&source);

		/**
		* @brief Destructor for the Socket class.
		*/
		virtual ~Socket();

	/** Protected functions and members*/
	protected:

		/**
		* @brief An extended verison of the string class, which allows for low-level length and string modification.
		*/
		class Buffer : public Jupiter::StringL
		{
		public:
			void set_length(size_t in_length);
			char *get_str() const;
		};

		/**
		* @brief Fetches the buffer where data is stored
		*
		* @return Buffer where data is stored
		*/
		Buffer &getInternalBuffer() const;

		/**
		* @brief Used by class extensions to get the socket descriptor.
		*
		* @return A raw socket descriptor.
		*/
		int getDescriptor() const;

		/**
		* @brief Used by class extensions to set the appropriate socket descriptor.
		*
		* @param descript Socket descriptor.
		*/
		void setDescriptor(int descript);

	/** Private members */
	private:
		struct Data;
		Data *data_;
	};

}

#endif // _SOCKET_H_HEADER