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
#include <numeric>
#include "jessilib/split.hpp"
#include "jessilib/unicode.hpp"
#include "String.hpp"
#include "Reference_String.h"
#include "TCPSocket.h"
#include "HTTP.h"
#include "HTTP_Server.h"

using namespace Jupiter::literals;
using namespace std::literals;

static const std::string_view HTTP_REQUEST_ENDING = "\r\n\r\n"sv;

template<typename ResultT = unsigned int, typename InT>
ResultT calc_checksum(const InT& in_container) {
	ResultT result{};
	result = std::accumulate(in_container.begin(), in_container.end(), result);
	return result;
}

template<typename ResultT = unsigned int, typename InT>
ResultT calc_checksumi(const InT& in_container) {
	ResultT result{};
	result = std::accumulate(in_container.begin(), in_container.end(), result, [](ResultT current_sum, typename InT::value_type current_element) {
		return current_sum + jessilib::fold(current_element);
	});
	return result;
}

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

Jupiter::HTTP::Server::Content::Content(std::string in_name, Jupiter::HTTP::Server::HTTPFunction in_function)
	: name(std::move(in_name)) {
	Jupiter::HTTP::Server::Content::function = in_function;
	Jupiter::HTTP::Server::Content::name_checksum = calc_checksum(name); // switch to calcChecksumi to make case-insensitive
}

std::string* Jupiter::HTTP::Server::Content::execute(std::string_view query_string) {
	return Jupiter::HTTP::Server::Content::function(query_string);
}

// HTTP::Server::Directory

Jupiter::HTTP::Server::Directory::Directory(std::string in_name)
	: name(std::move(in_name)) {
	name_checksum = calc_checksum(name);
}

Jupiter::HTTP::Server::Directory::~Directory() {
}

// host/dir/content
// .hook("dir/subdir/", content)

void Jupiter::HTTP::Server::Directory::hook(std::string_view in_name, std::unique_ptr<Content> in_content) {
	std::string_view in_name_ref = in_name;
	size_t in_name_start = in_name_ref.find_first_not_of('/');

	if (in_name_start == std::string_view::npos) { // Hook content
		content.push_back(std::move(in_content));
		return;
	}
	in_name_ref.remove_prefix(in_name_start);

	size_t in_name_end = in_name_ref.find('/');
	std::string_view dir_name;
	if (in_name_end == std::string_view::npos) {
		dir_name = in_name_ref;
	}
	else {
		dir_name = in_name_ref.substr(size_t{ 0 }, in_name_end);
	}

	in_name_ref.remove_prefix(dir_name.size());
	unsigned int dir_name_checksum = calc_checksum(dir_name);
	for (auto& directory : directories) {
		if (directory->name_checksum == dir_name_checksum && directory->name == dir_name) {
			directory->hook(in_name_ref, std::move(in_content));
			return;
		}
	}

	// create directories
	Directory* directory = directories.emplace_back(std::make_unique<Directory>(static_cast<std::string>(dir_name))).get();

directory_add_loop: // TODO: for the love of god, why why why
	in_name_start = in_name_ref.find_first_not_of('/');
	if (in_name_start != std::string_view::npos) {
		in_name_ref.remove_prefix(in_name_start);

		// add directory
		size_t index = in_name_ref.find('/');
		if (index != Jupiter::INVALID_INDEX) {
			directory->directories.push_back(std::make_unique<Directory>(static_cast<std::string>(in_name_ref.substr(size_t{ 0 }, index))));
			directory = directory->directories[directories.size() - 1].get();
			in_name_ref.remove_prefix(index + 1);
			goto directory_add_loop;
		}
		directory->directories.push_back(std::make_unique<Directory>(static_cast<std::string>(in_name_ref)));
		directory = directory->directories[directories.size() - 1].get();
	}

	// add content
	directory->content.push_back(std::move(in_content));
}

bool Jupiter::HTTP::Server::Directory::remove(std::string_view path, std::string_view content_name) {
	std::string_view in_name_ref = path;
	size_t in_name_start = in_name_ref.find_first_not_of('/');

	if (in_name_start == std::string_view::npos) { // Remove content
		unsigned int checksum = calc_checksum(content_name);
		for (auto itr = content.begin(); itr != content.end(); ++itr) {
			auto& content_node = *itr;
			if (content_node->name_checksum == checksum && content_node->name == content_name) {
				content.erase(itr);
				return true;
			}
		}

		return false;
	}
	in_name_ref.remove_prefix(in_name_start);

	// Call remove() on next directory in path
	size_t index = in_name_ref.find('/');
	std::string_view dir_name;
	if (index == std::string_view::npos)
		dir_name = in_name_ref;
	else
		dir_name = in_name_ref.substr(size_t{ 0 }, index);

	in_name_ref.remove_prefix(dir_name.size());
	unsigned int checksum = calc_checksum(dir_name);
	for (auto& directory : directories) {
		if (directory->name_checksum == checksum && directory->name == dir_name) {
			return directory->remove(in_name_ref, content_name);
		}
	}
	return false;
}

bool Jupiter::HTTP::Server::Directory::has(std::string_view in_name) {
	return find(in_name) != nullptr;
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::Directory::find(std::string_view in_name) {
	std::string_view in_name_ref = in_name;
	while (!in_name_ref.empty() && in_name_ref.front() == '/') {
		in_name.remove_prefix(1);
	}

	size_t index = in_name_ref.find('/');
	if (index == Jupiter::INVALID_INDEX) { // Search content
		unsigned int content_name_checksum = calc_checksum(in_name_ref);
		index = content.size();
		for (auto& content_item : content) {
			if (content_item->name_checksum == content_name_checksum && content_item->name == in_name_ref) {
				return content_item.get();
			}
		}
		return nullptr; // No such content
	}

	// Just use split_once?
	std::string_view dir_name(in_name_ref.data(), index);
	in_name_ref.remove_prefix(dir_name.size() + 1);
	unsigned int dir_name_checksum = calc_checksum(dir_name);

	for (auto& directory : directories) {
		if (directory->name_checksum == dir_name_checksum && directory->name == dir_name) {
			return directory->find(in_name_ref);
		}
	}

	return nullptr; // No such directory
}

std::string* Jupiter::HTTP::Server::Directory::execute(std::string_view in_name, std::string_view query_string) {
	Jupiter::HTTP::Server::Content *content_ptr = find(in_name);
	if (content_ptr == nullptr)
		return nullptr;

	return content_ptr->execute(query_string);
}

// HTTP::Server::Host

Jupiter::HTTP::Server::Host::Host(std::string in_name)
	: Directory(std::move(in_name)) {
	name_checksum = calc_checksumi(name);
}

// HTTPSession struct

struct HTTPSession {
	Jupiter::Socket sock;
	std::string request;
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
	void hook(std::string_view host, std::string_view path, std::unique_ptr<Content> in_content);
	bool remove(std::string_view hostname);
	//bool remove(std::string_view hostname, std::string_view path);
	bool remove(std::string_view hostname, std::string_view path, std::string_view name);
	bool has(std::string_view hostname);
	bool has(std::string_view hostname, std::string_view name);
	Jupiter::HTTP::Server::Host *find_host(std::string_view name);
	Content* find(std::string_view name);
	Content* find(std::string_view hostname, std::string_view name);
	std::string* execute(std::string_view name, std::string_view query_string);
	std::string* execute(std::string_view hostname, std::string_view name, std::string_view query_string);

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
	m_hosts.push_back(std::make_unique<Host>(static_cast<std::string>(Jupiter::HTTP::Server::global_namespace)));
}

// Data destructor

Jupiter::HTTP::Server::Data::~Data() {
}

// Data functions

void Jupiter::HTTP::Server::Data::hook(std::string_view hostname, std::string_view in_path, std::unique_ptr<Content> in_content) {
	Jupiter::ReferenceString dir_name;
	Jupiter::HTTP::Server::Host* host = find_host(hostname);

	if (host == nullptr) {
		//host = new Jupiter::HTTP::Server::Host(hostname);
		host = m_hosts.emplace_back(std::make_unique<Host>(static_cast<std::string>(hostname))).get();
		// OPTIMIZE: create directory tree and return.
	}

	size_t in_path_start = in_path.find_first_not_of('/');
	if (in_path_start == std::string_view::npos) {
		host->content.push_back(std::move(in_content));
	}
	else {
		in_path.remove_prefix(in_path_start);
		host->hook(in_path, std::move(in_content));
	}
}

bool Jupiter::HTTP::Server::Data::remove(std::string_view hostname) {
	unsigned int name_checksum = calc_checksumi(hostname);
	for (auto host_itr = m_hosts.begin(); host_itr != m_hosts.end(); ++host_itr) {
		if (name_checksum == (*host_itr)->name_checksum && jessilib::equalsi((*host_itr)->name, hostname)) {
			m_hosts.erase(host_itr);
			return true;
		}
	}
	return false;
}

// name: path/to/resource OR path/
bool Jupiter::HTTP::Server::Data::remove(std::string_view hostname, std::string_view path, std::string_view name) {
	Jupiter::HTTP::Server::Host *host = find_host(hostname);
	if (host == nullptr) {
		return false;
	}
	return host->remove(path, name);
}

bool Jupiter::HTTP::Server::Data::has(std::string_view hostname) {
	unsigned int name_checksum = calc_checksumi(hostname);
	for (const auto& host : m_hosts) {
		if (name_checksum == host->name_checksum && jessilib::equalsi(host->name, hostname)) {
			return true;
		}
	}

	return false;
}

bool Jupiter::HTTP::Server::Data::has(std::string_view hostname, std::string_view name) {
	unsigned int name_checksum = calc_checksumi(hostname);
	for (const auto& host : m_hosts) {
		if (name_checksum == host->name_checksum && jessilib::equalsi(host->name, hostname)) {
			name_checksum = calc_checksum(name); // switch to equalsi to make case-insensitive
			for (const auto& content : host->content) {
				if (name_checksum == content->name_checksum && content->name == name) { // switch to equalsi to make case-insensitive
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

Jupiter::HTTP::Server::Host *Jupiter::HTTP::Server::Data::find_host(std::string_view name) {
	unsigned int name_checksum = calc_checksumi(name);
	for (const auto& host : m_hosts) {
		if (name_checksum == host->name_checksum && host->name == name) {
			return host.get();
		}
	}

	return nullptr;
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::Data::find(std::string_view name) {
	return m_hosts[0]->find(name);
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::Data::find(std::string_view hostname, std::string_view name) {
	Jupiter::HTTP::Server::Host *host = find_host(hostname);
	if (host == nullptr)
		return nullptr;

	return host->find(name);
}

std::string* Jupiter::HTTP::Server::Data::execute(std::string_view name, std::string_view query_string) {
	Jupiter::HTTP::Server::Content *content = find(name);
	if (content == nullptr)
		return nullptr;

	return content->execute(query_string);
}

std::string* Jupiter::HTTP::Server::Data::execute(std::string_view hostname, std::string_view name, std::string_view query_string) {
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
	auto lines = jessilib::split_view(session.request, "\r\n"sv);
	HTTPCommand command = HTTPCommand::NONE_SPECIFIED;
	Content *content = nullptr;
	std::string_view query_string;
	std::string_view first_token;
	size_t index = 0;
	size_t span;

	auto get_line_offset = [&session, &lines](size_t index)
	{
		size_t offset = 0;
		while (index != 0)
			offset += lines[--index].size() + 2;
		return offset;
	};

	while (index != lines.size())
	{
		std::string_view line = lines[index++];

		// trim front-end spaces.
		while (!line.empty() && line.front() == ' ') {
			line.remove_prefix(1);
		}

		if (!line.empty()) // end of http request
		{
			std::string result;
			result.reserve(256);
			switch (command)
			{
			case HTTPCommand::GET:
			case HTTPCommand::HEAD:
				if (content != nullptr)
				{
					// 200 (success)
					// TODO: remove referencestring warpper
					std::string* content_result = content->execute(query_string);

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

					result += string_printf("Content-Length: %u" ENDL, content_result->size());

					if (session.keep_alive)
						result += "Connection: keep-alive"_jrs ENDL;
					else
						result += "Connection: close"_jrs ENDL;

					result += "Content-Type: "_jrs;
					if (content->type.empty()) {
						result += Jupiter::HTTP::Content::Type::Text::PLAIN;
					}
					else {
						result += content->type;
					}

					if (!content->charset.empty()) {
						result += "; charset="_jrs;
						result += content->charset;
					}
					result += ENDL;

					if (!content->language.empty()) {
						result += "Content-Language: "_jrs;
						result += content->language;
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

			if (session.keep_alive == false) {// not keep-alive -- will be destroyed on return
				break;
			}

			if (index == lines.size()) { // end of packet
				session.request.erase();
			}
			else { // end of request -- another request is following
				// Maybe there's still some value in a shifty string, to avoid substr calls & heap allocations...
				session.request = session.request.substr(get_line_offset(index));
			}

			if (session.request.find(HTTP_REQUEST_ENDING) != Jupiter::INVALID_INDEX) { // there's another full request already received
				return process_request(session);
			}
			break;
		}

		// Not empty
		auto first_split = jessilib::split_once_view(line, ' ');
		first_token = first_split.first;

		if (first_token.back() == ':') { // header field
			first_token.remove_suffix(1); // trim trailing ':'
			if (jessilib::equalsi(first_token, "HOST"sv)) {
				session.host = find_host(first_split.second);
			}
			else if (jessilib::equalsi(first_token, "CONNECTION"sv)) {
				std::string_view connection_type = first_split.second;
				if (jessilib::equalsi(connection_type, "keep-alive"sv)) {
					session.keep_alive = permit_keept_alive;
				}
			}
		}
		else { // command
			if (first_token == "GET"sv) {
				command = HTTPCommand::GET;

				auto second_split = jessilib::split_once_view(first_split.second, ' ');
				query_string = second_split.first;
				span = query_string.find('?'); // repurposing 'span'
				if (span == Jupiter::INVALID_INDEX) {
					if (session.host == nullptr) {
						content = find(query_string);
					}
					else {
						content = session.host->find(query_string);
					}
					query_string = std::string_view{};
				}
				else {
					if (session.host == nullptr) {
						content = find(query_string.substr(size_t{ 0 }, span));
					}
					else {
						content = session.host->find(query_string.substr(size_t{ 0 }, span));
					}
					query_string.remove_prefix(span + 1);
					// decode query_string here
				}

				std::string_view protocol_str = second_split.second;
				if (jessilib::equalsi(protocol_str, "http/1.0"sv)) {
					session.version = HTTPVersion::HTTP_1_0;
				}
				else if (jessilib::equalsi(protocol_str, "http/1.1"sv)) {
					session.version = HTTPVersion::HTTP_1_1;
					session.keep_alive = permit_keept_alive;
				}
			}
			else if (first_token == "HEAD"sv) {
				command = HTTPCommand::HEAD;

				auto second_split = jessilib::split_once_view(first_split.second, ' ');
				query_string = second_split.first;
				span = query_string.find('?'); // repurposing 'span'
				if (span == Jupiter::INVALID_INDEX) {
					if (session.host == nullptr) {
						content = find(query_string);
					}
					else {
						content = session.host->find(query_string);
					}
					query_string = std::string_view{};
				}
				else {
					if (session.host == nullptr) {
						content = find(query_string.substr(size_t{ 0 }, span));
					}
					else {
						content = session.host->find(query_string.substr(size_t{ 0 }, span));
					}
					query_string.remove_prefix(span + 1);
					// decode query_string here
				}

				std::string_view protocol_str = second_split.second;
				if (jessilib::equalsi(protocol_str, "http/1.0"sv)) {
					session.version = HTTPVersion::HTTP_1_0;
				}
				else if (jessilib::equalsi(protocol_str, "http/1.1"sv)) {
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

void Jupiter::HTTP::Server::hook(std::string_view host, std::string_view name, std::unique_ptr<Content> content) {
	return m_data->hook(host, name, std::move(content));
}

bool Jupiter::HTTP::Server::remove(std::string_view host) {
	return m_data->remove(host);
}

bool Jupiter::HTTP::Server::remove(std::string_view host, std::string_view path, std::string_view name) {
	return m_data->remove(host, path, name);
}

bool Jupiter::HTTP::Server::has(std::string_view host) {
	return m_data->has(host);
}

bool Jupiter::HTTP::Server::has(std::string_view host, std::string_view name) {
	return m_data->has(host, name);
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::find(std::string_view name) {
	return m_data->find(name);
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::find(std::string_view host, std::string_view name) {
	return m_data->find(host, name);
}

std::string* Jupiter::HTTP::Server::execute(std::string_view name, std::string_view query_string) {
	return m_data->execute(name, query_string);
}

std::string* Jupiter::HTTP::Server::execute(std::string_view host, std::string_view name, std::string_view query_string) {
	return m_data->execute(host, name, query_string);
}

bool Jupiter::HTTP::Server::bind(std::string_view hostname, uint16_t port) {
	auto socket = std::make_unique<Jupiter::TCPSocket>();
	if (socket->bind(static_cast<std::string>(hostname).c_str(), port, true)) {
		socket->setBlocking(false);
		m_data->m_ports.push_back(std::move(socket));
		return true;
	}

	return false;
}

bool Jupiter::HTTP::Server::tls_bind(std::string_view hostname, uint16_t port) {
	auto socket = std::make_unique<Jupiter::SecureTCPSocket>();
	if (socket->bind(static_cast<std::string>(hostname).c_str(), port, true)) {
		m_data->m_ports.push_back(std::move(socket));
		return true;
	}

	return false;
}

int Jupiter::HTTP::Server::think() {
	// Process existing clients
	for (auto itr = m_data->m_sessions.begin(); itr != m_data->m_sessions.end();) {
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
			std::string_view sock_buffer = session->sock.getBuffer();
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
		socket.reset(port->accept());
		if (socket != nullptr) {
			socket->setBlocking(false);
			auto session = std::make_unique<HTTPSession>(std::move(*socket));
			if (session->sock.recv() > 0) { // data received
				std::string_view sock_buffer = session->sock.getBuffer();
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

std::string_view Jupiter::HTTP::Server::global_namespace = ""_jrs;
std::string_view Jupiter::HTTP::Server::server_string = "Jupiter"_jrs;