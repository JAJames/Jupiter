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

#if !defined _SECURESOCKET_H_HEADER
#define _SECURESOCKET_H_HEADER

#include "Socket.h"

/**
 * @file SecureSocket.h
 * @brief Provides an OpenSSL implementation on the Socket interface.
 */

namespace Jupiter
{
	/**
	* @brief Implements TLS/SSL support using SSL on the Socket interface.
	* Numerous functions here use various OpenSSL functions. Therefore, some
	* functions will return values which correspond to the OpenSSL functions,
	* as opposed to the standard socket functions. In general however, output
	* return values should be similar.
	*/
	class JUPITER_API SecureSocket : public Socket
	{
	public:

		/**
		* @brief Enumerator translating to various SSL/TLS protocols.
		* Used in getMethod() and setMethod().
		*/
		enum EncryptionMethod
		{
			SSL3 = 1,	/** SSL 3.0 - Unrecommended */
			TLS1 = 2,	/** TLS 1.0 - Unrecommended */
			TLS1_1 = 3,	/** TLS 1.1 */
			TLS1_2 = 4,	/** TLS 1.2 */
			DTLS1 = 5,	/** DTLS 1.0 */
			ANY = 126,	/** Attempt to use any of the above encryption methods (generally the most secure available between both client and server) */
			END = 127	/** END OF ENUM */
		};

		/**
		* @brief Returns the name of the cipher currently in use.
		* @return Name of cipher currently in use, or "NONE" if none is in use.
		*/
		const char *getCipherName() const;

		/**
		* @brief Returns the encryption method that the socket attempts to use.
		* This is ANY by default.
		* @return Encrpytion method that the socket attempts to use.
		*/
		EncryptionMethod getMethod() const;

		/**
		* @brief Sets the encryption method to be used when connecting.
		*/
		void setMethod(EncryptionMethod method);

		/**
		* @brief Loads a certificate and key for use.
		*
		* @param cert String containing file location of certificate.
		* @param key String containing file location of private key.
		* @return True on success, false otherwise.
		*/
		void setCertificate(const char *cert, const char *key);

		/**
		* @brief Loads a certificate and key for use.
		*
		* @param pem Combined certificate/key file.
		* @return True on success, false otherwise.
		*/
		void setCertificate(const char *pem);

		/**
		* @brief Interface to provide simple connection establishing.
		*
		* @param hostname String containing hostname of server to connect to.
		* @param iPort Port to connect on.
		* @param Address for client to bind to.
		* @return True on success, false otherwise.
		*/
		virtual bool connectToHost(const char *hostname, unsigned short iPort, const char *clientAddress = nullptr, unsigned short clientPort = 0);

		/**
		* @brief Interface to provide simple binding to ports.
		*
		* @param hostname String containing hostname to bind to.
		* @param iPort Port to bind to.
		* @param andListen True if listen() should be called, false otherwise.
		* @return True on success, false otherwise.
		*/
		virtual bool bindToPort(const char *hostname, unsigned short iPort, bool andListen = true);

		/**
		* @brief Accepts an incoming connection for the port bound to.
		*
		* @return A valid SecureSocket on success, nullptr otherwise.
		*/
		virtual SecureSocket *acceptConnection();

		/**
		* @brief Closes the socket.
		*/
		virtual void closeSocket();

		/**
		* @brief Writes new data from the socket to the buffer, without removing it from the socket queue.
		* The data written by this function will always end with a null character, which is not counted in the returned value.
		*
		* @return Number of bytes received on success, less than or equal to 0 otherwise.
		* Note: Refer to SSL_read() for detailed return values.
		*/
		virtual int peek();

		/**
		* @brief Writes new data from the socket to the buffer.
		* The data written by this function will always end with a null character, which is not counted in the returned value.
		*
		* @return Number of bytes received on success, less than or equal to 0 otherwise.
		* Note: Refer to SSL_read() for detailed return values.
		*/
		virtual int recv();

		/**
		* @brief Sends data across the socket.
		*
		* @param data String containing the data to be send.
		* @param datalen The size of the data to be sent, in chars.
		* @return Number of bytes sent on success, less than or equal to 0 otherwise.
		* Note: Refer to SSL_write() for detailed return values.
		*/
		virtual int send(const char *data, size_t datalen);

		/**
		* @brief Sends a null-terminated string of data across the socket.
		*
		* @param String containing the null-terminated data to send.
		* @return Number of bytes sent on success, less than or equal to 0 otherwise.
		* Note: Refer to SSL_write() for detailed return values.
		*/
		virtual int send(const char *msg);

		/**
		* @brief Initializes SSL on the socket.
		* Note: This is only relevant when elevating an existing Socket to a SecureSocket.
		*
		* @return True on success, false otherwise.
		*/
		bool initSSL();

		/**
		* @brief Default constructor for the SecureSocket class.
		* This constructor will set the buffer size to 4096 chars.
		*/
		SecureSocket();

		/**
		* @brief Copying a Socket is forbidden.
		*/
		SecureSocket(const SecureSocket &) = delete;

		/**
		* @brief Constructor for the SecureSocket class which allows for the setting of a specific buffer size.
		*
		* @param bufferSize Size of the buffer to allocate, measured in chars.
		*/
		SecureSocket(size_t bufferSize);

		/**
		* @brief Socket move constructor for the SecureSocket class.
		* Note: initSSL() must be called immediately.
		*
		* @param source Socket to move data from.
		*/
		SecureSocket(Jupiter::Socket &&source);

		/**
		* @brief SecureSocket move constructor for the SecureSocket class.
		*
		* @param source SecureSocket to move data from.
		*/
		SecureSocket(Jupiter::SecureSocket &&source);

		/**
		* @brief Destructor for the SecureSocket class.
		*/
		virtual ~SecureSocket();

	/** Private members */
	private:
		struct SSLData;
		SSLData *SSLdata_;
	};

}

#endif // _SECURESOCKET_H_HEADER