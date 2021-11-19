/**
 * Copyright (C) 2013-2017 Jessica James.
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

#include <utility> // std::move
#include <openssl/ssl.h> // OpenSSL SSL functions
#include <openssl/err.h> // OpenSSL SSL errors
#include "SecureSocket.h"

struct Jupiter::SecureSocket::SSLData
{
	SSL *handle = nullptr;
    SSL_CTX *context = nullptr;
	const SSL_METHOD *method = nullptr;
	Jupiter::SecureSocket::EncryptionMethod eMethod = ANY;
	std::string cert;
	std::string key;
	~SSLData();
};

Jupiter::SecureSocket::SSLData::~SSLData()
{
	if (Jupiter::SecureSocket::SSLData::handle != nullptr)
	{
		if (SSL_shutdown(Jupiter::SecureSocket::SSLData::handle) == 0) SSL_shutdown(Jupiter::SecureSocket::SSLData::handle);
		SSL_free(Jupiter::SecureSocket::SSLData::handle);
	}
	if (Jupiter::SecureSocket::SSLData::context != nullptr) SSL_CTX_free(Jupiter::SecureSocket::SSLData::context);
}

Jupiter::SecureSocket &Jupiter::SecureSocket::operator=(Jupiter::SecureSocket &&source)
{
	Jupiter::Socket::operator=(std::move(source));
	Jupiter::SecureSocket::SSLdata_ = source.SSLdata_;
	source.SSLdata_ = nullptr;
	return *this;
}

Jupiter::SecureSocket::SecureSocket() : Jupiter::Socket()
{
	Jupiter::SecureSocket::SSLdata_ = new Jupiter::SecureSocket::SSLData();
}

Jupiter::SecureSocket::SecureSocket(size_t bufferSize) : Jupiter::Socket(bufferSize)
{
	Jupiter::SecureSocket::SSLdata_ = new Jupiter::SecureSocket::SSLData();
}

Jupiter::SecureSocket::SecureSocket(Jupiter::Socket &&source) : Jupiter::Socket(std::move(source))
{
	Jupiter::SecureSocket::SSLdata_ = new Jupiter::SecureSocket::SSLData();
}

Jupiter::SecureSocket::SecureSocket(Jupiter::SecureSocket &&source) : Jupiter::Socket(std::move(source))
{
	Jupiter::SecureSocket::SSLdata_ = source.SSLdata_;
	source.SSLdata_ = nullptr;
}

Jupiter::SecureSocket::~SecureSocket()
{
	if (Jupiter::SecureSocket::SSLdata_ != nullptr) delete Jupiter::SecureSocket::SSLdata_;
}

Jupiter::SecureSocket *Jupiter::SecureSocket::accept()
{
	SocketType tSock = SSL_accept(Jupiter::SecureSocket::SSLdata_->handle);
	if (tSock > 0)
	{
		SecureSocket *r = new SecureSocket(Jupiter::SecureSocket::getBufferSize());
		r->setDescriptor(tSock);
		r->setType(this->getType());
		r->setProtocol(this->getProtocol());
		return r;
	}
	return nullptr;
}

bool Jupiter::SecureSocket::bind(const char *hostname, unsigned short iPort, bool andListen)
{
	return Jupiter::Socket::bind(hostname, iPort, andListen);
}

void Jupiter::SecureSocket::shutdown()
{
	Jupiter::Socket::shutdown();
	if (Jupiter::SecureSocket::SSLdata_ != nullptr && Jupiter::SecureSocket::SSLdata_->handle != nullptr)
	{
		if (SSL_shutdown(Jupiter::SecureSocket::SSLdata_->handle) == 0)
			SSL_shutdown(Jupiter::SecureSocket::SSLdata_->handle);
	}
}

void Jupiter::SecureSocket::close()
{
	Jupiter::Socket::close();
	if (Jupiter::SecureSocket::SSLdata_ != nullptr && Jupiter::SecureSocket::SSLdata_->handle != nullptr)
	{
		if (SSL_shutdown(Jupiter::SecureSocket::SSLdata_->handle) == 0)
			SSL_shutdown(Jupiter::SecureSocket::SSLdata_->handle);
		SSL_free(Jupiter::SecureSocket::SSLdata_->handle);
		Jupiter::SecureSocket::SSLdata_->handle = nullptr;
	}
}

const SSL_METHOD *translateEncryptionMethod(Jupiter::SecureSocket::EncryptionMethod method) {
	// These are disabled due to deprecations, but also there's not that much reason to actually maintain this method
	// since we realistically never use anything except EncryptionMethod::ANY. It was an interesting idea, but there
	// just isn't a real use case for these methods. Regardless, better way to implement setMethod/getMethod() would be
	// to not provide any abstract implementation at all, and to instead include such option setting as part of some
	// OpenSSL provider plugin
	switch (method) {
	//case Jupiter::SecureSocket::EncryptionMethod::SSL3:
	//	return SSLv3_method();
	case Jupiter::SecureSocket::EncryptionMethod::TLS1:
		//return TLSv1_method();
	case Jupiter::SecureSocket::EncryptionMethod::TLS1_1:
		//return TLSv1_1_method();
	case Jupiter::SecureSocket::EncryptionMethod::TLS1_2:
		//return TLSv1_2_method();
	case Jupiter::SecureSocket::EncryptionMethod::DTLS1:
		//return DTLSv1_method();
	case Jupiter::SecureSocket::EncryptionMethod::ANY:
		return SSLv23_method();
	default:
		return nullptr;
	}
}

const char *Jupiter::SecureSocket::getCipherName() const
{
	return SSL_CIPHER_get_name(SSL_get_current_cipher(Jupiter::SecureSocket::SSLdata_->handle));
}

Jupiter::SecureSocket::EncryptionMethod Jupiter::SecureSocket::getMethod() const
{
	return Jupiter::SecureSocket::SSLdata_->eMethod;
}

void Jupiter::SecureSocket::setMethod(Jupiter::SecureSocket::EncryptionMethod method)
{
	Jupiter::SecureSocket::SSLdata_->eMethod = method;
}

bool loadCertificate(SSL_CTX *context, const char *cert, const char *key)
{
	if (SSL_CTX_load_verify_locations(context, cert, key) != 1) ERR_print_errors_fp(stderr);

	if (SSL_CTX_set_default_verify_paths(context) != 1) ERR_print_errors_fp(stderr);

	if (SSL_CTX_use_certificate_file(context, cert, SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		return false;
	}

	if (SSL_CTX_use_PrivateKey_file(context, key, SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		return false;
	}

	if (!SSL_CTX_check_private_key(context)) return false;
	return true;
}

void Jupiter::SecureSocket::setCertificate(const Jupiter::ReadableString &cert, const Jupiter::ReadableString &key)
{
	Jupiter::SecureSocket::SSLdata_->cert = static_cast<std::string>(cert);
	Jupiter::SecureSocket::SSLdata_->key = static_cast<std::string>(key);
}

void Jupiter::SecureSocket::setCertificate(const Jupiter::ReadableString &pem)
{
	Jupiter::SecureSocket::setCertificate(pem, pem);
}

bool Jupiter::SecureSocket::connect(const char *hostname, unsigned short iPort, const char *clientAddress, unsigned short clientPort)
{
	return Jupiter::Socket::connect(hostname, iPort, clientAddress, clientPort) && this->initSSL();
}

int Jupiter::SecureSocket::peek()
{
	if (Jupiter::SecureSocket::SSLdata_->handle == nullptr)
		return -1;
	Jupiter::Socket::Buffer &buffer = this->getInternalBuffer();
	buffer.erase();
	int r = SSL_peek(Jupiter::SecureSocket::SSLdata_->handle, buffer.get_str(), static_cast<int>(this->getBufferSize()));
	if (r > 0)
		buffer.set_length(r);
	return r;
}

int Jupiter::SecureSocket::recv()
{
	if (Jupiter::SecureSocket::SSLdata_->handle == nullptr)
		return -1;
	Jupiter::Socket::Buffer &buffer = this->getInternalBuffer();
	buffer.erase();
	int r = SSL_read(Jupiter::SecureSocket::SSLdata_->handle, buffer.get_str(), static_cast<int>(this->getBufferSize()));
	if (r > 0)
		buffer.set_length(r);
	return r;
}

int Jupiter::SecureSocket::send(const char *data, size_t datalen)
{
	return SSL_write(Jupiter::SecureSocket::SSLdata_->handle, data, static_cast<int>(datalen));
}

bool Jupiter::SecureSocket::initSSL()
{
	SSL_load_error_strings();
	SSL_library_init();

	if (Jupiter::SecureSocket::SSLdata_->context == nullptr)
	{
		if (Jupiter::SecureSocket::SSLdata_->method == nullptr)
		{
			Jupiter::SecureSocket::SSLdata_->method = translateEncryptionMethod(Jupiter::SecureSocket::SSLdata_->eMethod);
			if (Jupiter::SecureSocket::SSLdata_->method == nullptr)
				return false;
		}

		Jupiter::SecureSocket::SSLdata_->context = SSL_CTX_new(Jupiter::SecureSocket::SSLdata_->method);
		if (Jupiter::SecureSocket::SSLdata_->context == nullptr)
		{
			ERR_print_errors_fp(stderr);
			return false;
		}
		if (!Jupiter::SecureSocket::SSLdata_->cert.empty())
			loadCertificate(Jupiter::SecureSocket::SSLdata_->context, Jupiter::SecureSocket::SSLdata_->cert.c_str(), Jupiter::SecureSocket::SSLdata_->key.c_str());
	}

	Jupiter::SecureSocket::SSLdata_->handle = SSL_new(Jupiter::SecureSocket::SSLdata_->context);
	if (Jupiter::SecureSocket::SSLdata_->handle == nullptr)
	{
		ERR_print_errors_fp(stderr);
		return false;
	}

	// Not suppressing this warning; descriptor ideally shouldn't be getting shortened here, even if it's "safe"
	// TODO: Can't resolve this without removing OpenSSL usage on Windows; we should ideally use Windows APIs
	if (SSL_set_fd(Jupiter::SecureSocket::SSLdata_->handle, this->getDescriptor()) == 0)
	{
		ERR_print_errors_fp(stderr);
		return false;
	}
	if (SSL_set_tlsext_host_name(Jupiter::SecureSocket::SSLdata_->handle, this->getRemoteHostnameC()) != 1) // This error check is potentially redundant, but no documentation has been found.
	{
		ERR_print_errors_fp(stderr);
		return false;
	}
	int t = SSL_connect(Jupiter::SecureSocket::SSLdata_->handle);
	if (t != 1)
	{
		ERR_print_errors_fp(stderr);
		return false;
	}
	return true;
}