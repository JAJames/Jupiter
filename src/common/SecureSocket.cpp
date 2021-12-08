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

#include <utility> // std::move
#include <openssl/ssl.h> // OpenSSL SSL functions
#include <openssl/err.h> // OpenSSL SSL errors
#include "SecureSocket.h"

struct Jupiter::SecureSocket::SSLData {
	SSL* handle = nullptr;
    SSL_CTX* context = nullptr;
	const SSL_METHOD* method = nullptr;
	std::string cert;
	std::string key;
	~SSLData();
};

Jupiter::SecureSocket::SSLData::~SSLData() {
	if (Jupiter::SecureSocket::SSLData::handle != nullptr) {
		if (SSL_shutdown(Jupiter::SecureSocket::SSLData::handle) == 0) {
			SSL_shutdown(Jupiter::SecureSocket::SSLData::handle);
		}
		SSL_free(Jupiter::SecureSocket::SSLData::handle);
	}
	if (Jupiter::SecureSocket::SSLData::context != nullptr) SSL_CTX_free(Jupiter::SecureSocket::SSLData::context);
}

Jupiter::SecureSocket &Jupiter::SecureSocket::operator=(Jupiter::SecureSocket &&source) {
	Jupiter::Socket::operator=(std::move(source));
	m_ssl_data = source.m_ssl_data;
	source.m_ssl_data = nullptr;
	return *this;
}

Jupiter::SecureSocket::SecureSocket() : Jupiter::Socket() {
	m_ssl_data = new Jupiter::SecureSocket::SSLData();
}

Jupiter::SecureSocket::SecureSocket(size_t bufferSize)
	: Jupiter::Socket(bufferSize) {
	m_ssl_data = new Jupiter::SecureSocket::SSLData();
}

Jupiter::SecureSocket::SecureSocket(Jupiter::Socket &&source)
	: Jupiter::Socket(std::move(source)) {
	m_ssl_data = new Jupiter::SecureSocket::SSLData();
}

Jupiter::SecureSocket::SecureSocket(Jupiter::SecureSocket &&source)
	: Jupiter::Socket(std::move(source)) {
	m_ssl_data = source.m_ssl_data;
	source.m_ssl_data = nullptr;
}

Jupiter::SecureSocket::~SecureSocket() {
	if (m_ssl_data != nullptr) {
		delete m_ssl_data;
	}
}

Jupiter::SecureSocket *Jupiter::SecureSocket::accept() {
	SocketType tSock = SSL_accept(m_ssl_data->handle);
	if (tSock > 0) {
		SecureSocket *r = new SecureSocket(Jupiter::SecureSocket::getBufferSize());
		r->setDescriptor(tSock);
		r->setType(this->getType());
		r->setProtocol(this->getProtocol());
		return r;
	}
	return nullptr;
}

bool Jupiter::SecureSocket::bind(const char *hostname, unsigned short iPort, bool andListen) {
	return Jupiter::Socket::bind(hostname, iPort, andListen);
}

void Jupiter::SecureSocket::shutdown() {
	Jupiter::Socket::shutdown();
	if (m_ssl_data != nullptr && m_ssl_data->handle != nullptr)
	{
		if (SSL_shutdown(m_ssl_data->handle) == 0) {
			SSL_shutdown(m_ssl_data->handle);
		}
	}
}

void Jupiter::SecureSocket::close() {
	Jupiter::Socket::close();
	if (m_ssl_data != nullptr && m_ssl_data->handle != nullptr) {
		if (SSL_shutdown(m_ssl_data->handle) == 0) {
			SSL_shutdown(m_ssl_data->handle);
		}
		SSL_free(m_ssl_data->handle);
		m_ssl_data->handle = nullptr;
	}
}

const char *Jupiter::SecureSocket::getCipherName() const {
	return SSL_CIPHER_get_name(SSL_get_current_cipher(m_ssl_data->handle));
}

bool loadCertificate(SSL_CTX *context, const char *cert, const char *key) {
	if (SSL_CTX_load_verify_locations(context, cert, key) != 1) ERR_print_errors_fp(stderr);

	if (SSL_CTX_set_default_verify_paths(context) != 1) ERR_print_errors_fp(stderr);

	if (SSL_CTX_use_certificate_file(context, cert, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		return false;
	}

	if (SSL_CTX_use_PrivateKey_file(context, key, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		return false;
	}

	if (!SSL_CTX_check_private_key(context)) return false;
	return true;
}

void Jupiter::SecureSocket::setCertificate(std::string cert, std::string key) {
	m_ssl_data->cert = std::move(cert);
	m_ssl_data->key = std::move(key);
}

void Jupiter::SecureSocket::setCertificate(std::string_view pem) {
	Jupiter::SecureSocket::setCertificate(static_cast<std::string>(pem), static_cast<std::string>(pem));
}

bool Jupiter::SecureSocket::connect(const char *hostname, unsigned short iPort, const char *clientAddress, unsigned short clientPort) {
	return Jupiter::Socket::connect(hostname, iPort, clientAddress, clientPort) && this->initSSL();
}

int Jupiter::SecureSocket::peek() {
	if (m_ssl_data->handle == nullptr)
		return -1;
	Jupiter::Socket::Buffer &buffer = this->getInternalBuffer();
	buffer.clear();
	int r = SSL_peek(m_ssl_data->handle, buffer.data(), static_cast<int>(this->getBufferSize()));
	if (r > 0)
		buffer.set_length(r);
	return r;
}

int Jupiter::SecureSocket::recv() {
	if (m_ssl_data->handle == nullptr)
		return -1;
	Jupiter::Socket::Buffer &buffer = this->getInternalBuffer();
	buffer.clear();
	int r = SSL_read(m_ssl_data->handle, buffer.data(), static_cast<int>(this->getBufferSize()));
	if (r > 0)
		buffer.set_length(r);
	return r;
}

int Jupiter::SecureSocket::send(const char *data, size_t datalen) {
	return SSL_write(m_ssl_data->handle, data, static_cast<int>(datalen));
}

bool Jupiter::SecureSocket::initSSL() {
	SSL_load_error_strings();
	SSL_library_init();

	if (m_ssl_data->context == nullptr) {
		if (m_ssl_data->method == nullptr) {
			m_ssl_data->method = TLS_method();
			if (m_ssl_data->method == nullptr)
				return false;
		}

		m_ssl_data->context = SSL_CTX_new(m_ssl_data->method);
		if (m_ssl_data->context == nullptr) {
			ERR_print_errors_fp(stderr);
			return false;
		}
		if (!m_ssl_data->cert.empty()) {
			loadCertificate(m_ssl_data->context, m_ssl_data->cert.c_str(), m_ssl_data->key.c_str());
		}
	}

	m_ssl_data->handle = SSL_new(m_ssl_data->context);
	if (m_ssl_data->handle == nullptr) {
		ERR_print_errors_fp(stderr);
		return false;
	}

	// Not suppressing this warning; descriptor ideally shouldn't be getting shortened here, even if it's "safe"
	// TODO: Can't resolve this without removing OpenSSL usage on Windows; we should ideally use Windows APIs
	if (SSL_set_fd(m_ssl_data->handle, this->getDescriptor()) == 0) {
		ERR_print_errors_fp(stderr);
		return false;
	}
	if (SSL_set_tlsext_host_name(m_ssl_data->handle, this->getRemoteHostnameC()) != 1) {
		// This error check is potentially redundant, but no documentation has been found.
		ERR_print_errors_fp(stderr);
		return false;
	}
	int t = SSL_connect(m_ssl_data->handle);
	if (t != 1)
	{
		ERR_print_errors_fp(stderr);
		return false;
	}
	return true;
}