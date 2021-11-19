/**
 * Copyright (C) 2015-2021 Jessica James.
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

#include <ctime>
#include <chrono>
#include "String.hpp"
#include "Reference_String.h"
#include "TCPSocket.h"
#include "HTTP.h"
#include "HTTP_Server.h"

using namespace Jupiter::literals;

static const Jupiter::ReferenceString HTTP_REQUEST_ENDING = "\r\n\r\n"_jrs;

// HTTPCommand

enum HTTPCommand
{
	GET,
	HEAD,
	UNKNOWN,
	NONE_SPECIFIED
};

// HTTPVersion

enum HTTPVersion
{
	HTTP_1_0,
	HTTP_1_1,
	HTTP_Unsupported
};

// HTTP::Server::Content

Jupiter::HTTP::Server::Content::Content(const Jupiter::ReadableString &in_name, Jupiter::HTTP::Server::HTTPFunction in_function) : name(in_name) {
	Jupiter::HTTP::Server::Content::function = in_function;
	Jupiter::HTTP::Server::Content::name_checksum = Jupiter::HTTP::Server::Content::name.calcChecksum(); // switch to calcChecksumi to make case-insensitive
}

Jupiter::ReadableString *Jupiter::HTTP::Server::Content::execute(const Jupiter::ReadableString &query_string) {
	return Jupiter::HTTP::Server::Content::function(query_string);
}

// HTTP::Server::Directory

Jupiter::HTTP::Server::Directory::Directory(const Jupiter::ReadableString &in_name) : name(in_name) {
	name_checksum = Jupiter::HTTP::Server::Directory::name.calcChecksum();
}

Jupiter::HTTP::Server::Directory::~Directory() {
}

// host/dir/content
// .hook("dir/subdir/", content)

void Jupiter::HTTP::Server::Directory::hook(const Jupiter::ReadableString &in_name, std::unique_ptr<Content> in_content) {
	Jupiter::ReferenceString in_name_ref = in_name;
	in_name_ref.shiftRight(in_name_ref.span('/'));

	if (in_name_ref.isEmpty()) { // Hook content
		content.push_back(std::move(in_content));
		return;
	}

	size_t index = in_name_ref.find('/');
	Jupiter::ReferenceString dir_name;
	if (index == Jupiter::INVALID_INDEX) {
		dir_name = in_name_ref;
	}
	else {
		dir_name = in_name_ref.substring(size_t{ 0 }, index);
	}

	in_name_ref.shiftRight(dir_name.size());
	unsigned int dir_name_checksum = dir_name.calcChecksum();
	index = directories.size();
	for (auto& directory : directories) {
		if (directory->name_checksum == dir_name_checksum && directory->name.equals(dir_name)) {
			directory->hook(in_name_ref, std::move(in_content));
			return;
		}
	}

	// create directories
	Directory* directory = directories.emplace_back(std::make_unique<Directory>(dir_name)).get();

directory_add_loop: // TODO: for the love of god, why why why
	in_name_ref.shiftRight(in_name_ref.span('/'));
	if (in_name_ref.isNotEmpty()) {
		// add directory
		index = in_name_ref.find('/');
		if (index != Jupiter::INVALID_INDEX) {
			directory->directories.push_back(std::make_unique<Directory>(in_name_ref.substring(size_t{ 0 }, index)));
			directory = directory->directories[directories.size() - 1].get();
			in_name_ref.shiftRight(index + 1);
			goto directory_add_loop;
		}
		directory->directories.push_back(std::make_unique<Directory>(in_name_ref));
		directory = directory->directories[directories.size() - 1].get();
	}

	// add content
	directory->content.push_back(std::move(in_content));
}

bool Jupiter::HTTP::Server::Directory::remove(const Jupiter::ReadableString &path, const Jupiter::ReadableString &content_name) {
	Jupiter::ReferenceString in_name_ref = path;
	in_name_ref.shiftRight(in_name_ref.span('/'));
	unsigned int checksum;

	if (in_name_ref.isEmpty()) { // Remove content
		checksum = content_name.calcChecksum();
		for (auto itr = content.begin(); itr != content.end(); ++itr) {
			auto& content_node = *itr;
			if (content_node->name_checksum == checksum && content_node->name.equals(content_name)) {
				content.erase(itr);
				return true;
			}
		}

		return false;
	}

	// Call remove() on next directory in path
	size_t index = in_name_ref.find('/');
	Jupiter::ReferenceString dir_name;
	if (index == Jupiter::INVALID_INDEX)
		dir_name = in_name_ref;
	else
		dir_name = in_name_ref.substring(size_t{ 0 }, index);

	in_name_ref.shiftRight(dir_name.size());
	checksum = dir_name.calcChecksum();
	for (auto& directory : directories) {
		if (directory->name_checksum == checksum && directory->name.equals(dir_name)) {
			return directory->remove(in_name_ref, content_name);
		}
	}
	return false;
}

bool Jupiter::HTTP::Server::Directory::has(const Jupiter::ReadableString &in_name)
{
	return find(in_name) != nullptr;
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::Directory::find(const Jupiter::ReadableString &in_name)
{
	Jupiter::ReferenceString in_name_ref = in_name;
	in_name_ref.shiftRight(in_name_ref.span("/"_jrs));

	size_t index = in_name_ref.find('/');
	if (index == Jupiter::INVALID_INDEX) { // Search content
		unsigned int content_name_checksum = in_name_ref.calcChecksum();
		index = content.size();
		for (auto& content_item : content) {
			if (content_item->name_checksum == content_name_checksum && content_item->name.equals(in_name_ref)) {
				return content_item.get();
			}
		}
		return nullptr; // No such content
	}

	Jupiter::ReferenceString dir_name(in_name_ref.ptr(), index);
	in_name_ref.shiftRight(dir_name.size() + 1);
	unsigned int dir_name_checksum = dir_name.calcChecksum();

	for (auto& directory : directories) {
		if (directory->name_checksum == dir_name_checksum && directory->name.equals(dir_name)) {
			return directory->find(in_name_ref);
		}
	}

	return nullptr; // No such directory
}

Jupiter::ReadableString* Jupiter::HTTP::Server::Directory::execute(const Jupiter::ReadableString &in_name, const Jupiter::ReadableString &query_string) {
	Jupiter::HTTP::Server::Content *content_ptr = find(in_name);
	if (content_ptr == nullptr)
		return nullptr;

	return content_ptr->execute(query_string);
}

// HTTP::Server::Host

Jupiter::HTTP::Server::Host::Host(const Jupiter::ReadableString &in_name) : Directory(in_name) {
	name_checksum = Jupiter::HTTP::Server::Host::name.calcChecksumi();
}

// HTTPSession struct

struct HTTPSession {
	Jupiter::Socket sock;
	Jupiter::String request;
	bool keep_alive = false;
	Jupiter::HTTP::Server::Host* host = nullptr;
	HTTPVersion version = HTTPVersion::HTTP_1_0;
	std::chrono::steady_clock::time_point last_active = std::chrono::steady_clock::now();
	HTTPSession(Jupiter::Socket&& in_sock);
	~HTTPSession();
};

HTTPSession::HTTPSession(Jupiter::Socket&& in_sock)
	: sock(std::move(in_sock)) {
}

HTTPSession::~HTTPSession() {
}

// Server::Data struct

struct Jupiter::HTTP::Server::Data {
	/** Data */
	std::vector<std::unique_ptr<Jupiter::HTTP::Server::Host>> m_hosts; // TODO: remove heap allocation, requires move semantics
	std::vector<std::unique_ptr<Socket>> m_ports; // TODO: remove heap allocation, sockets are already pimpl
	std::vector<std::unique_ptr<HTTPSession>> m_sessions; // TODO: consider removing heap allocation
	std::chrono::milliseconds session_timeout = std::chrono::milliseconds(2000); // TODO: Config variable
	std::chrono::milliseconds keep_alive_session_timeout = std::chrono::milliseconds(5000); // TODO: Config variable
	size_t max_request_size = 8192; // TODO: Config variable
	bool permit_keept_alive = true; // TODO: Config variable

	/** Foward functions */
	void hook(const Jupiter::ReadableString &host, const Jupiter::ReadableString &path, std::unique_ptr<Content> in_content);
	bool remove(const Jupiter::ReadableString &hostname);
	//bool remove(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &path);
	bool remove(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &path, const Jupiter::ReadableString &name);
	bool has(const Jupiter::ReadableString &hostname);
	bool has(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name);
	Jupiter::HTTP::Server::Host *find_host(const Jupiter::ReadableString &name);
	Content* find(const Jupiter::ReadableString &name);
	Content* find(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name);
	Jupiter::ReadableString *execute(const Jupiter::ReadableString &name, const Jupiter::ReadableString &query_string);
	Jupiter::ReadableString *execute(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name, const Jupiter::ReadableString &query_string);

	int process_request(HTTPSession &session);

	/** Constructors */
	Data();
	Data(const Data &source) = delete;
	Data(Data &&source) = delete;

	/** Destructor */
	~Data();
};

// Data constructor

Jupiter::HTTP::Server::Data::Data() {
	// hosts[0] is always the "global" namespace.
	m_hosts.push_back(std::make_unique<Host>(Jupiter::HTTP::Server::global_namespace));
}

// Data destructor

Jupiter::HTTP::Server::Data::~Data() {
}

// Data functions

void Jupiter::HTTP::Server::Data::hook(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &in_path, std::unique_ptr<Content> in_content) {
	Jupiter::ReferenceString path = in_path;
	Jupiter::ReferenceString dir_name;
	Jupiter::HTTP::Server::Host* host = find_host(hostname);

	if (host == nullptr) {
		//host = new Jupiter::HTTP::Server::Host(hostname);
		host = m_hosts.emplace_back(std::make_unique<Host>(hostname)).get();
		// OPTIMIZE: create directory tree and return.
	}

	path.shiftRight(path.span('/'));
	if (path.isEmpty()) {
		host->content.push_back(std::move(in_content));
	}
	else {
		host->hook(path, std::move(in_content));
	}
}

bool Jupiter::HTTP::Server::Data::remove(const Jupiter::ReadableString &hostname) {
	unsigned int name_checksum = hostname.calcChecksumi();
	for (auto host_itr = m_hosts.begin(); host_itr != m_hosts.end(); ++host_itr) {
		if (name_checksum == (*host_itr)->name_checksum && (*host_itr)->name.equalsi(hostname)) {
			m_hosts.erase(host_itr);
			return true;
		}
	}
	return false;
}

// name: path/to/resource OR path/
bool Jupiter::HTTP::Server::Data::remove(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &path, const Jupiter::ReadableString &name) {
	Jupiter::HTTP::Server::Host *host = find_host(hostname);
	if (host == nullptr) {
		return false;
	}
	return host->remove(path, name);
}

bool Jupiter::HTTP::Server::Data::has(const Jupiter::ReadableString &hostname) {
	unsigned int name_checksum = hostname.calcChecksumi();
	for (const auto& host : m_hosts) {
		if (name_checksum == host->name_checksum && host->name.equalsi(hostname)) {
			return true;
		}
	}

	return false;
}

bool Jupiter::HTTP::Server::Data::has(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name) {
	unsigned int name_checksum = hostname.calcChecksumi();
	for (const auto& host : m_hosts) {
		if (name_checksum == host->name_checksum && host->name.equalsi(hostname)) {
			name_checksum = name.calcChecksum(); // switch to equalsi to make case-insensitive
			for (const auto& content : host->content) {
				if (name_checksum == content->name_checksum && content->name.equals(name)) { // switch to equalsi to make case-insensitive
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

Jupiter::HTTP::Server::Host *Jupiter::HTTP::Server::Data::find_host(const Jupiter::ReadableString &name) {
	unsigned int name_checksum = name.calcChecksumi();
	for (const auto& host : m_hosts) {
		if (name_checksum == host->name_checksum && host->name.equalsi(name)) {
			return host.get();
		}
	}

	return nullptr;
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::Data::find(const Jupiter::ReadableString &name) {
	return m_hosts[0]->find(name);
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::Data::find(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name) {
	Jupiter::HTTP::Server::Host *host = find_host(hostname);
	if (host == nullptr)
		return nullptr;

	return host->find(name);
}

Jupiter::ReadableString *Jupiter::HTTP::Server::Data::execute(const Jupiter::ReadableString &name, const Jupiter::ReadableString &query_string) {
	Jupiter::HTTP::Server::Content *content = find(name);
	if (content == nullptr)
		return nullptr;

	return content->execute(query_string);
}

Jupiter::ReadableString *Jupiter::HTTP::Server::Data::execute(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name, const Jupiter::ReadableString &query_string) {
	Jupiter::HTTP::Server::Content *content = find(hostname, name);
	if (content == nullptr)
		return nullptr;

	return content->execute(query_string);
}

char* html_time() {
	time_t rawtime = time(0);
	char *rtime = new char[64];
	strftime(rtime, 64, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&rawtime));
	return rtime;
}

int Jupiter::HTTP::Server::Data::process_request(HTTPSession &session) {
	Jupiter::ReadableString::TokenizeResult<Jupiter::Reference_String> lines = Jupiter::ReferenceString::tokenize(session.request, STRING_LITERAL_AS_REFERENCE(ENDL));
	HTTPCommand command = HTTPCommand::NONE_SPECIFIED;
	Content *content = nullptr;
	Jupiter::ReferenceString query_string;
	Jupiter::ReferenceString first_token;
	size_t index = 0;
	size_t span;

	auto get_line_offset = [&session, &lines](size_t index)
	{
		size_t offset = 0;
		while (index != 0)
			offset += lines.tokens[--index].size() + 2;
		return offset;
	};

	while (index != lines.token_count)
	{
		Jupiter::ReferenceString &line = lines.tokens[index++];

		// trim front-end spaces.
		span = line.span(" "_jrs);
		if (span != 0)
			line.shiftRight(span);

		if (line.isEmpty()) // end of http request
		{
			Jupiter::String result(256);
			switch (command)
			{
			case HTTPCommand::GET:
			case HTTPCommand::HEAD:
				if (content != nullptr)
				{
					// 200 (success)
					Jupiter::ReadableString *content_result = content->execute(query_string);

					switch (session.version)
					{
					default:
					case HTTPVersion::HTTP_1_0:
						result = "HTTP/1.0 200 OK"_jrs ENDL;
						break;
					case HTTPVersion::HTTP_1_1:
						result = "HTTP/1.1 200 OK"_jrs ENDL;
						break;
					}

					result += "Date: "_jrs;
					char *time_header = html_time();
					result += time_header;
					delete[] time_header;
					result += ENDL;

					result += "Server: "_jrs JUPITER_VERSION ENDL;

					result += Jupiter::StringS::Format("Content-Length: %u" ENDL, content_result->size());

					if (session.keep_alive)
						result += "Connection: keep-alive"_jrs ENDL;
					else
						result += "Connection: close"_jrs ENDL;

					result += "Content-Type: "_jrs;
					if (content->type == nullptr)
						result += Jupiter::HTTP::Content::Type::Text::PLAIN;
					else
						result += *content->type;
					if (content->charset != nullptr)
					{
						result += "; charset="_jrs;
						result += *content->charset;
					}
					result += ENDL;

					if (content->language != nullptr)
					{
						result += "Content-Language: "_jrs;
						result += *content->language;
						result += ENDL;
					}

					result += ENDL;
					if (command == HTTPCommand::GET)
						result += *content_result;

					if (content->free_result)
						delete content_result;

					session.sock.send(result);
				}
				else
				{
					// 404 (not found)

					switch (session.version)
					{
					default:
					case HTTPVersion::HTTP_1_0:
						result = "HTTP/1.0 404 Not Found"_jrs ENDL;
						break;
					case HTTPVersion::HTTP_1_1:
						result = "HTTP/1.1 404 Not Found"_jrs ENDL;
						break;
					}

					char *time_header = html_time();
					result += "Date: "_jrs ENDL;
					result += time_header;
					delete[] time_header;

					result += "Server: "_jrs JUPITER_VERSION ENDL;

					result += "Content-Length: 0"_jrs ENDL;

					if (session.keep_alive)
						result += "Connection: keep-alive"_jrs ENDL;
					else
						result += "Connection: close"_jrs ENDL;

					result += ENDL ENDL;
					session.sock.send(result);
				}
				break;
			default:
				break;
			}

			if (session.keep_alive == false) // not keep-alive -- will be destroyed on return
				break;
			if (index == lines.token_count) // end of packet
				session.request.erase();
			else // end of request -- another request is following
				session.request.shiftRight(get_line_offset(index));

			if (session.request.find(HTTP_REQUEST_ENDING) != Jupiter::INVALID_INDEX) // there's another full request already received
				return process_request(session);
			break;
		}

		// Not empty
		first_token = line.getToken(0, ' ');

		if (first_token.get(first_token.size() - 1) == ':') // header field
		{
			first_token.truncate(1); // trim trailing ':'
			if (first_token.equalsi("HOST"_jrs))
				session.host = find_host(line.getWord(1, " "));
			else if (first_token.equalsi("CONNECTION"_jrs))
			{
				Jupiter::ReferenceString connection_type = line.getWord(1, " ");
				if (connection_type.equalsi("keep-alive"_jrs))
					session.keep_alive = permit_keept_alive;
			}
		}
		else // command
		{
			if (first_token.equals("GET"_jrs))
			{
				command = HTTPCommand::GET;
				
				query_string = line.getWord(1, " ");
				span = query_string.find('?'); // repurposing 'span'
				if (span == Jupiter::INVALID_INDEX)
				{
					if (session.host == nullptr)
						content = find(query_string);
					else
						content = session.host->find(query_string);
					query_string.erase();
				}
				else
				{
					if (session.host == nullptr)
						content = find(query_string.substring(size_t{ 0 }, span));
					else
						content = session.host->find(query_string.substring(size_t{ 0 }, span));
					query_string.shiftRight(span + 1);
					// decode query_string here
				}

				Jupiter::ReferenceString protocol_str = line.getWord(2, " ");
				if (protocol_str.equalsi("http/1.0"_jrs))
					session.version = HTTPVersion::HTTP_1_0;
				else if (protocol_str.equalsi("http/1.1"_jrs))
				{
					session.version = HTTPVersion::HTTP_1_1;
					session.keep_alive = permit_keept_alive;
				}
			}
			else if (first_token.equals("HEAD"_jrs))
			{
				command = HTTPCommand::HEAD;
				
				query_string = line.getWord(1, " ");
				span = query_string.find('?'); // repurposing 'span'
				if (span == Jupiter::INVALID_INDEX)
				{
					if (session.host == nullptr)
						content = find(query_string);
					else
						content = session.host->find(query_string);
					query_string.erase();
				}
				else
				{
					if (session.host == nullptr)
						content = find(query_string.substring(size_t{ 0 }, span));
					else
						content = session.host->find(query_string.substring(size_t{ 0 }, span));
					query_string.shiftRight(span + 1);
					// decode query_string here
				}

				Jupiter::ReferenceString protocol_str = line.getWord(2, " ");
				if (protocol_str.equalsi("http/1.0"_jrs))
					session.version = HTTPVersion::HTTP_1_0;
				else if (protocol_str.equalsi("http/1.1"_jrs))
				{
					session.version = HTTPVersion::HTTP_1_1;
					session.keep_alive = permit_keept_alive;
				}
			}
			else
				command = HTTPCommand::UNKNOWN;
		}
	}
	return 0;
}

/** HTTP::Server */

// Server constructors

Jupiter::HTTP::Server::Server() {
	m_data = new Data();
}

Jupiter::HTTP::Server::Server(Jupiter::HTTP::Server &&source) {
	m_data = source.m_data;
	source.m_data = new Data();
}

Jupiter::HTTP::Server::~Server() {
	delete m_data;
}

// Server functions

void Jupiter::HTTP::Server::hook(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name, std::unique_ptr<Content> content) {
	return m_data->hook(host, name, std::move(content));
}

bool Jupiter::HTTP::Server::remove(const Jupiter::ReadableString &host) {
	return m_data->remove(host);
}

/*bool Jupiter::HTTP::Server::remove(const Jupiter::ReadableString &host, const Jupiter::ReadableString &path) {
	return m_data->remove(host, path);
}*/

bool Jupiter::HTTP::Server::remove(const Jupiter::ReadableString &host, const Jupiter::ReadableString &path, const Jupiter::ReadableString &name) {
	return m_data->remove(host, path, name);
}

bool Jupiter::HTTP::Server::has(const Jupiter::ReadableString &host) {
	return m_data->has(host);
}

bool Jupiter::HTTP::Server::has(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name) {
	return m_data->has(host, name);
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::find(const Jupiter::ReadableString &name) {
	return m_data->find(name);
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::find(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name) {
	return m_data->find(host, name);
}

Jupiter::ReadableString *Jupiter::HTTP::Server::execute(const Jupiter::ReadableString &name, const Jupiter::ReadableString &query_string) {
	return m_data->execute(name, query_string);
}

Jupiter::ReadableString *Jupiter::HTTP::Server::execute(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name, const Jupiter::ReadableString &query_string) {
	return m_data->execute(host, name, query_string);
}

bool Jupiter::HTTP::Server::bind(const Jupiter::ReadableString &hostname, uint16_t port) {
	auto socket = std::make_unique<Jupiter::TCPSocket>();
	if (socket->bind(static_cast<std::string>(hostname).c_str(), port, true)) {
		socket->setBlocking(false);
		m_data->m_ports.push_back(std::move(socket));
		return true;
	}

	return false;
}

bool Jupiter::HTTP::Server::tls_bind(const Jupiter::ReadableString &hostname, uint16_t port) {
	auto socket = std::make_unique<Jupiter::SecureTCPSocket>();
	if (socket->bind(static_cast<std::string>(hostname).c_str(), port, true)) {
		m_data->m_ports.push_back(std::move(socket));
		return true;
	}

	return false;
}

int Jupiter::HTTP::Server::think() {
	// Process existing clients
	auto sessions_end = m_data->m_sessions.end();
	for (auto itr = m_data->m_sessions.begin(); itr != sessions_end;) {
		auto& session = *itr;
		if (session->sock.isShutdown()) {
			if (session->sock.recv() == 0) {
				itr = m_data->m_sessions.erase(itr);
				continue;
			}
		}
		else if ((std::chrono::steady_clock::now() > session->last_active + m_data->keep_alive_session_timeout)
			|| (session->keep_alive == false && std::chrono::steady_clock::now() > session->last_active + m_data->session_timeout)) {
			itr = m_data->m_sessions.erase(itr);
			continue;
		}
		else if (session->sock.recv() > 0) {
			const Jupiter::ReadableString &sock_buffer = session->sock.getBuffer();
			if (session->request.size() + sock_buffer.size() <= m_data->max_request_size) { // accept
				session->request += sock_buffer;
				if (session->request.find(HTTP_REQUEST_ENDING) != Jupiter::INVALID_INDEX) { // completed request
					session->last_active = std::chrono::steady_clock::now();
					m_data->process_request(*session);
					if (session->keep_alive == false) { // remove completed session
						itr = m_data->m_sessions.erase(itr);
						//session->sock.shutdown();
						continue;
					}
					// else // keep_alive: session not deleted
				}
				else if (session->request.size() == m_data->max_request_size) { // reject (full buffer)
					itr = m_data->m_sessions.erase(itr);
					continue;
				}
				// else // request not over: session not deleted
			}
			else { // reject
				itr = m_data->m_sessions.erase(itr);
				continue;
			}
		}
		else if (session->sock.getLastError() != JUPITER_SOCK_EWOULDBLOCK) {
			itr = m_data->m_sessions.erase(itr);
			continue;
		}
		// else // EWOULDBLOCK: session not deleted

		++itr;
	}

	// Process incoming clients
	std::unique_ptr<Jupiter::Socket> socket;
	for (auto& port : m_data->m_ports) {
		socket.reset(socket->accept());
		if (socket != nullptr) {
			socket->setBlocking(false);
			auto session = std::make_unique<HTTPSession>(std::move(*socket));
			if (session->sock.recv() > 0) { // data received
				const Jupiter::ReadableString &sock_buffer = session->sock.getBuffer();
				if (sock_buffer.size() < m_data->max_request_size) { // accept
					session->request = session->sock.getBuffer();
					if (sock_buffer.find(HTTP_REQUEST_ENDING) != Jupiter::INVALID_INDEX) { // completed request
						m_data->process_request(*session);
						if (session->keep_alive) { // session will live for 30 seconds.
							m_data->m_sessions.push_back(std::move(session));
						}
						// else // session completed
					}
					else { // store for more processing
						m_data->m_sessions.push_back(std::move(session));
					}
				}
				else if (sock_buffer.size() == m_data->max_request_size) {
					if (sock_buffer.find(HTTP_REQUEST_ENDING) == Jupiter::INVALID_INDEX) { // reject (too large)
						continue;
					}

					// accept (max size, completed request)
					session->request = session->sock.getBuffer();
					m_data->process_request(*session);
					if (session->keep_alive) { // session will live for 30 seconds.
						m_data->m_sessions.push_back(std::move(session));
					}
					// else // session completed
				}
				// else // reject (too large)
			}
			else if (session->sock.getLastError() == JUPITER_SOCK_EWOULDBLOCK) { // store for more processing
				m_data->m_sessions.push_back(std::move(session));
			}
		}
	}
	return 0;
}

const Jupiter::ReadableString &Jupiter::HTTP::Server::global_namespace = Jupiter::ReferenceString::empty;
const Jupiter::ReadableString &Jupiter::HTTP::Server::server_string = "Jupiter"_jrs;