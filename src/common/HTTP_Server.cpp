/**
 * Copyright (C) 2015-2016 Jessica James.
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
#include "CString.h"
#include "Reference_String.h"
#include "TCPSocket.h"
#include "ArrayList.h"
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

Jupiter::HTTP::Server::Content::Content(const Jupiter::ReadableString &in_name, Jupiter::HTTP::Server::HTTPFunction in_function) : name(in_name)
{
	Jupiter::HTTP::Server::Content::function = in_function;
	Jupiter::HTTP::Server::Content::name_checksum = Jupiter::HTTP::Server::Content::name.calcChecksum(); // switch to calcChecksumi to make case-insensitive
}

Jupiter::ReadableString *Jupiter::HTTP::Server::Content::execute(const Jupiter::ReadableString &query_string)
{
	return Jupiter::HTTP::Server::Content::function(query_string);
}

// HTTP::Server::Directory

Jupiter::HTTP::Server::Directory::Directory(const Jupiter::ReadableString &in_name) : name(in_name)
{
	name_checksum = Jupiter::HTTP::Server::Directory::name.calcChecksum();
}

Jupiter::HTTP::Server::Directory::~Directory()
{
	Jupiter::HTTP::Server::Directory::directories.emptyAndDelete();
	Jupiter::HTTP::Server::Directory::content.emptyAndDelete();
}

// host/dir/content
// .hook("dir/subdir/", content)

void Jupiter::HTTP::Server::Directory::hook(const Jupiter::ReadableString &in_name, Content *in_content)
{
	Jupiter::ReferenceString in_name_ref = in_name;
	in_name_ref.shiftRight(in_name_ref.span('/'));

	if (in_name_ref.isEmpty()) // Hook content
		Jupiter::HTTP::Server::Directory::content.add(in_content);
	else
	{
		size_t index = in_name_ref.find('/');
		Jupiter::ReferenceString dir_name;
		if (index == Jupiter::INVALID_INDEX)
			dir_name = in_name_ref;
		else
			dir_name = in_name_ref.substring(size_t{ 0 }, index);

		in_name_ref.shiftRight(dir_name.size());
		Jupiter::HTTP::Server::Directory *directory;
		unsigned int dir_name_checksum = dir_name.calcChecksum();
		index = Jupiter::HTTP::Server::Directory::directories.size();
		while (index != 0)
		{
			directory = Jupiter::HTTP::Server::Directory::directories.get(--index);
			if (directory->name_checksum == dir_name_checksum && directory->name.equals(dir_name))
			{
				directory->hook(in_name_ref, in_content);
				return;
			}
		}

		// create directories
		directory = new Jupiter::HTTP::Server::Directory(dir_name);
		Jupiter::HTTP::Server::Directory::directories.add(directory);

	directory_add_loop:
		in_name_ref.shiftRight(in_name_ref.span('/'));
		if (in_name_ref.isNotEmpty())
		{
			// add directory
			index = in_name_ref.find('/');
			if (index != Jupiter::INVALID_INDEX)
			{
				directory->directories.add(new Jupiter::HTTP::Server::Directory(in_name_ref.substring(size_t{ 0 }, index)));
				directory = directory->directories.get(directories.size() - 1);
				in_name_ref.shiftRight(index + 1);
				goto directory_add_loop;
			}
			directory->directories.add(new Jupiter::HTTP::Server::Directory(in_name_ref));
			directory = directory->directories.get(directories.size() - 1);
		}

		// add content
		directory->content.add(in_content);
	}
}

bool Jupiter::HTTP::Server::Directory::remove(const Jupiter::ReadableString &path, const Jupiter::ReadableString &content_name)
{
	size_t index;
	Jupiter::ReferenceString in_name_ref = path;
	in_name_ref.shiftRight(in_name_ref.span('/'));
	unsigned int checksum;

	if (in_name_ref.isEmpty()) // Remove content
	{
		Jupiter::HTTP::Server::Content *content_node;
		checksum = content_name.calcChecksum();
		index = Jupiter::HTTP::Server::Directory::content.size();
		while (index != 0)
		{
			content_node = Jupiter::HTTP::Server::Directory::content.get(--index);
			if (content_node->name_checksum == checksum && content_node->name.equals(content_name))
			{
				delete Jupiter::HTTP::Server::Directory::content.remove(index);
				return true;
			}
		}

		return false;
	}
	else // Call remove() on next directory in path
	{
		index = in_name_ref.find('/');
		Jupiter::ReferenceString dir_name;
		if (index == Jupiter::INVALID_INDEX)
			dir_name = in_name_ref;
		else
			dir_name = in_name_ref.substring(size_t{ 0 }, index);

		in_name_ref.shiftRight(dir_name.size());
		Jupiter::HTTP::Server::Directory *directory;
		checksum = dir_name.calcChecksum();
		index = Jupiter::HTTP::Server::Directory::directories.size();
		while (index != 0)
		{
			directory = Jupiter::HTTP::Server::Directory::directories.get(--index);
			if (directory->name_checksum == checksum && directory->name.equals(dir_name))
				return directory->remove(in_name_ref, content_name);
		}
	}
	return false;
}

bool Jupiter::HTTP::Server::Directory::has(const Jupiter::ReadableString &in_name)
{
	return this->find(in_name) != nullptr;
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::Directory::find(const Jupiter::ReadableString &in_name)
{
	Jupiter::ReferenceString in_name_ref = in_name;
	in_name_ref.shiftRight(in_name_ref.span("/"_jrs));

	size_t index = in_name_ref.find('/');
	if (index == Jupiter::INVALID_INDEX) // Search content
	{
		unsigned int content_name_checksum = in_name_ref.calcChecksum();
		index = Jupiter::HTTP::Server::Directory::content.size();
		Jupiter::HTTP::Server::Content *content_itr;
		while (index != 0)
		{
			content_itr = Jupiter::HTTP::Server::Directory::content.get(--index);
			if (content_itr->name_checksum == content_name_checksum && content_itr->name.equals(in_name_ref))
				return content_itr;
		}
		return nullptr; // No such content
	}

	Jupiter::ReferenceString dir_name(in_name_ref.ptr(), index);
	in_name_ref.shiftRight(dir_name.size() + 1);
	Jupiter::HTTP::Server::Directory *directory;
	unsigned int dir_name_checksum = dir_name.calcChecksum();
	index = Jupiter::HTTP::Server::Directory::directories.size();

	while (index != 0)
	{
		directory = Jupiter::HTTP::Server::Directory::directories.get(--index);
		if (directory->name_checksum == dir_name_checksum && directory->name.equals(dir_name))
			return directory->find(in_name_ref);
	}

	return nullptr; // No such directory
}

Jupiter::ReadableString *Jupiter::HTTP::Server::Directory::execute(const Jupiter::ReadableString &in_name, const Jupiter::ReadableString &query_string)
{
	Jupiter::HTTP::Server::Content *content_ptr = Jupiter::HTTP::Server::Directory::find(in_name);
	if (content_ptr == nullptr)
		return nullptr;
	return content_ptr->execute(query_string);
}

// HTTP::Server::Host

Jupiter::HTTP::Server::Host::Host(const Jupiter::ReadableString &in_name) : Directory(in_name)
{
	name_checksum = Jupiter::HTTP::Server::Host::name.calcChecksumi();
}

// HTTPSession struct

struct HTTPSession
{
	Jupiter::Socket sock;
	Jupiter::String request;
	bool keep_alive = false;
	Jupiter::HTTP::Server::Host *host = nullptr;
	HTTPVersion version = HTTPVersion::HTTP_1_0;
	std::chrono::steady_clock::time_point last_active = std::chrono::steady_clock::now();
	HTTPSession(Jupiter::Socket &&in_sock);
	~HTTPSession();
};

HTTPSession::HTTPSession(Jupiter::Socket &&in_sock) : sock(std::move(in_sock))
{
}

HTTPSession::~HTTPSession()
{
}

// Server::Data struct

struct Jupiter::HTTP::Server::Data
{
	/** Data */
	Jupiter::ArrayList<Jupiter::HTTP::Server::Host> hosts;
	Jupiter::ArrayList<Socket> ports;
	Jupiter::ArrayList<HTTPSession> sessions;
	std::chrono::milliseconds session_timeout = std::chrono::milliseconds(2000); // TODO: Config variable
	std::chrono::milliseconds keep_alive_session_timeout = std::chrono::milliseconds(5000); // TODO: Config variable
	size_t max_request_size = 1024; // TODO: Config variable
	bool permit_keept_alive = true; // TODO: Config variable

	/** Foward functions */
	void hook(const Jupiter::ReadableString &host, const Jupiter::ReadableString &path, Content *in_content);
	bool remove(const Jupiter::ReadableString &hostname);
	//bool remove(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &path);
	bool remove(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &path, const Jupiter::ReadableString &name);
	bool has(const Jupiter::ReadableString &hostname);
	bool has(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name);
	Jupiter::HTTP::Server::Host *find_host(const Jupiter::ReadableString &name);
	Content *find(const Jupiter::ReadableString &name);
	Content *find(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name);
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

Jupiter::HTTP::Server::Data::Data()
{
	// hosts[0] is always the "global" namespace.
	Jupiter::HTTP::Server::Data::hosts.add(new Jupiter::HTTP::Server::Host(Jupiter::HTTP::Server::global_namespace));
}

// Data destructor

Jupiter::HTTP::Server::Data::~Data()
{
	Jupiter::HTTP::Server::Data::hosts.emptyAndDelete();
	Jupiter::HTTP::Server::Data::sessions.emptyAndDelete();
	Jupiter::HTTP::Server::Data::ports.emptyAndDelete();
}

// Data functions

void Jupiter::HTTP::Server::Data::hook(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &in_path, Content *in_content)
{
	Jupiter::ReferenceString path = in_path;
	Jupiter::ReferenceString dir_name;
	Jupiter::HTTP::Server::Host *host = Jupiter::HTTP::Server::Data::find_host(hostname);

	if (host == nullptr)
	{
		host = new Jupiter::HTTP::Server::Host(hostname);
		Jupiter::HTTP::Server::Data::hosts.add(host);
		// OPTIMIZE: create directory tree and return.
	}

	path.shiftRight(path.span('/'));
	if (path.isEmpty())
		host->content.add(in_content);
	else
		host->hook(path, in_content);
}

bool Jupiter::HTTP::Server::Data::remove(const Jupiter::ReadableString &hostname)
{
	unsigned int name_checksum = hostname.calcChecksumi();
	size_t index = Jupiter::HTTP::Server::Data::hosts.size();
	Jupiter::HTTP::Server::Host *host;
	while (index != 0)
	{
		host = Jupiter::HTTP::Server::Data::hosts.get(--index);
		if (name_checksum == host->name_checksum && host->name.equalsi(hostname))
		{
			delete Jupiter::HTTP::Server::Data::hosts.remove(index);
			return true;
		}
	}
	return false;
}

// name: path/to/resource OR path/
bool Jupiter::HTTP::Server::Data::remove(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &path, const Jupiter::ReadableString &name)
{
	Jupiter::HTTP::Server::Host *host = Jupiter::HTTP::Server::Data::find_host(hostname);
	if (host == nullptr)
		return false;
	return host->remove(path, name);
}

bool Jupiter::HTTP::Server::Data::has(const Jupiter::ReadableString &hostname)
{
	unsigned int name_checksum = hostname.calcChecksumi();
	size_t index = Jupiter::HTTP::Server::Data::hosts.size();
	Jupiter::HTTP::Server::Host *host;
	while (index != 0)
	{
		host = Jupiter::HTTP::Server::Data::hosts.get(--index);
		if (name_checksum == host->name_checksum && host->name.equalsi(hostname))
			return true;
	}
	return false;
}

bool Jupiter::HTTP::Server::Data::has(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name)
{
	unsigned int name_checksum = hostname.calcChecksumi();
	size_t index = Jupiter::HTTP::Server::Data::hosts.size();
	Jupiter::HTTP::Server::Host *host;
	while (index != 0)
	{
		host = Jupiter::HTTP::Server::Data::hosts.get(--index);
		if (name_checksum == host->name_checksum && host->name.equalsi(hostname))
		{
			name_checksum = name.calcChecksum(); // switch to equalsi to make case-insensitive
			index = host->content.size();
			Jupiter::HTTP::Server::Content *content;
			while (index != 0)
			{
				content = host->content.get(--index);
				if (name_checksum == content->name_checksum && content->name.equals(name)) // switch to equalsi to make case-insensitive
					return true;
			}
			return false;
		}
	}
	return false;
}

Jupiter::HTTP::Server::Host *Jupiter::HTTP::Server::Data::find_host(const Jupiter::ReadableString &name)
{
	unsigned int name_checksum = name.calcChecksumi();
	size_t index = Jupiter::HTTP::Server::Data::hosts.size();
	Jupiter::HTTP::Server::Host *host;
	while (index != 0)
	{
		host = Jupiter::HTTP::Server::Data::hosts.get(--index);
		if (name_checksum == host->name_checksum && host->name.equalsi(name))
			return host;
	}
	return nullptr;
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::Data::find(const Jupiter::ReadableString &name)
{
	return Jupiter::HTTP::Server::Data::hosts.get(0)->find(name);
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::Data::find(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name)
{
	Jupiter::HTTP::Server::Host *host = Jupiter::HTTP::Server::Data::find_host(hostname);
	if (host == nullptr)
		return nullptr;

	return host->find(name);
}

Jupiter::ReadableString *Jupiter::HTTP::Server::Data::execute(const Jupiter::ReadableString &name, const Jupiter::ReadableString &query_string)
{
	Jupiter::HTTP::Server::Content *content = Jupiter::HTTP::Server::Data::find(name);
	if (content == nullptr)
		return nullptr;
	return content->execute(query_string);
}

Jupiter::ReadableString *Jupiter::HTTP::Server::Data::execute(const Jupiter::ReadableString &hostname, const Jupiter::ReadableString &name, const Jupiter::ReadableString &query_string)
{
	Jupiter::HTTP::Server::Content *content = Jupiter::HTTP::Server::Data::find(hostname, name);
	if (content == nullptr)
		return nullptr;
	return content->execute(query_string);
}

char *html_time()
{
	time_t rawtime = time(0);
	char *rtime = new char[64];
	strftime(rtime, 64, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&rawtime));
	return rtime;
}

int Jupiter::HTTP::Server::Data::process_request(HTTPSession &session)
{
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
				return Jupiter::HTTP::Server::Data::process_request(session);
			break;
		}

		// Not empty
		first_token = line.getToken(0, ' ');

		if (first_token.get(first_token.size() - 1) == ':') // header field
		{
			first_token.truncate(1); // trim trailing ':'
			if (first_token.equalsi("HOST"_jrs))
				session.host = Jupiter::HTTP::Server::Data::find_host(line.getWord(1, " "));
			else if (first_token.equalsi("CONNECTION"_jrs))
			{
				Jupiter::ReferenceString connection_type = line.getWord(1, " ");
				if (connection_type.equalsi("keep-alive"_jrs))
					session.keep_alive = Jupiter::HTTP::Server::Data::permit_keept_alive;
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
						content = Jupiter::HTTP::Server::Data::find(query_string);
					else
						content = session.host->find(query_string);
					query_string.erase();
				}
				else
				{
					if (session.host == nullptr)
						content = Jupiter::HTTP::Server::Data::find(query_string.substring(size_t{ 0 }, span));
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
					session.keep_alive = Jupiter::HTTP::Server::Data::permit_keept_alive;
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
						content = Jupiter::HTTP::Server::Data::find(query_string);
					else
						content = session.host->find(query_string);
					query_string.erase();
				}
				else
				{
					if (session.host == nullptr)
						content = Jupiter::HTTP::Server::Data::find(query_string.substring(size_t{ 0 }, span));
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
					session.keep_alive = Jupiter::HTTP::Server::Data::permit_keept_alive;
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

Jupiter::HTTP::Server::Server()
{
	Jupiter::HTTP::Server::data_ = new Jupiter::HTTP::Server::Data();
}

Jupiter::HTTP::Server::Server(Jupiter::HTTP::Server &&source)
{
	Jupiter::HTTP::Server::data_ = source.data_;
	source.data_ = new Jupiter::HTTP::Server::Data();
}

Jupiter::HTTP::Server::~Server()
{
	delete Jupiter::HTTP::Server::data_;
}

// Server functions

void Jupiter::HTTP::Server::hook(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name, Content *content)
{
	return Jupiter::HTTP::Server::data_->hook(host, name, content);
}

bool Jupiter::HTTP::Server::remove(const Jupiter::ReadableString &host)
{
	return Jupiter::HTTP::Server::data_->remove(host);
}

/*bool Jupiter::HTTP::Server::remove(const Jupiter::ReadableString &host, const Jupiter::ReadableString &path)
{
	return Jupiter::HTTP::Server::data_->remove(host, path);
}*/

bool Jupiter::HTTP::Server::remove(const Jupiter::ReadableString &host, const Jupiter::ReadableString &path, const Jupiter::ReadableString &name)
{
	return Jupiter::HTTP::Server::data_->remove(host, path, name);
}

bool Jupiter::HTTP::Server::has(const Jupiter::ReadableString &host)
{
	return Jupiter::HTTP::Server::data_->has(host);
}

bool Jupiter::HTTP::Server::has(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name)
{
	return Jupiter::HTTP::Server::data_->has(host, name);
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::find(const Jupiter::ReadableString &name)
{
	return Jupiter::HTTP::Server::data_->find(name);
}

Jupiter::HTTP::Server::Content *Jupiter::HTTP::Server::find(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name)
{
	return Jupiter::HTTP::Server::data_->find(host, name);
}

Jupiter::ReadableString *Jupiter::HTTP::Server::execute(const Jupiter::ReadableString &name, const Jupiter::ReadableString &query_string)
{
	return Jupiter::HTTP::Server::data_->execute(name, query_string);
}

Jupiter::ReadableString *Jupiter::HTTP::Server::execute(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name, const Jupiter::ReadableString &query_string)
{
	return Jupiter::HTTP::Server::data_->execute(host, name, query_string);
}

bool Jupiter::HTTP::Server::bind(const Jupiter::ReadableString &hostname, uint16_t port)
{
	Jupiter::TCPSocket *socket = new Jupiter::TCPSocket();
	if (socket->bind(Jupiter::CStringS(hostname).c_str(), port, true))
	{
		socket->setBlocking(false);
		Jupiter::HTTP::Server::data_->ports.add(socket);
		return true;
	}
	delete socket;
	return false;
}

bool Jupiter::HTTP::Server::tls_bind(const Jupiter::ReadableString &hostname, uint16_t port)
{
	Jupiter::SecureTCPSocket *socket = new Jupiter::SecureTCPSocket();
	if (socket->bind(Jupiter::CStringS(hostname).c_str(), port, true))
	{
		Jupiter::HTTP::Server::data_->ports.add(socket);
		return true;
	}
	delete socket;
	return false;
}

int Jupiter::HTTP::Server::think()
{
	Jupiter::Socket *socket;
	HTTPSession *session;

	// Process existing clients
	size_t index = Jupiter::HTTP::Server::data_->sessions.size();
	while (index != 0)
	{
		session = Jupiter::HTTP::Server::data_->sessions.get(--index);
		std::chrono::steady_clock::now();
		if (session->sock.isShutdown())
		{
			if (session->sock.recv() == 0)
				delete Jupiter::HTTP::Server::data_->sessions.remove(index);
		}
		else if ((std::chrono::steady_clock::now() > session->last_active + Jupiter::HTTP::Server::data_->keep_alive_session_timeout)
			|| (session->keep_alive == false && std::chrono::steady_clock::now() > session->last_active + Jupiter::HTTP::Server::data_->session_timeout))
			delete Jupiter::HTTP::Server::data_->sessions.remove(index);
			//session->sock.shutdown();
		else if (session->sock.recv() > 0)
		{
			const Jupiter::ReadableString &sock_buffer = session->sock.getBuffer();
			if (session->request.size() + sock_buffer.size() <= Jupiter::HTTP::Server::data_->max_request_size) // accept
			{
				session->request += sock_buffer;
				if (session->request.find(HTTP_REQUEST_ENDING) != Jupiter::INVALID_INDEX) // completed request
				{
					session->last_active = std::chrono::steady_clock::now();
					Jupiter::HTTP::Server::data_->process_request(*session);
					if (session->keep_alive == false) // remove completed session
						delete Jupiter::HTTP::Server::data_->sessions.remove(index);
						//session->sock.shutdown();
				}
				else if (session->request.size() == Jupiter::HTTP::Server::data_->max_request_size) // reject (full buffer)
					delete Jupiter::HTTP::Server::data_->sessions.remove(index);
			}
			else // reject
				delete Jupiter::HTTP::Server::data_->sessions.remove(index);
		}
		else if (session->sock.getLastError() != 10035)
			delete Jupiter::HTTP::Server::data_->sessions.remove(index);
	}

	// Process incoming clients
	index = Jupiter::HTTP::Server::data_->ports.size();
	while (index != 0)
	{
		socket = Jupiter::HTTP::Server::data_->ports.get(--index);
		socket = socket->accept();
		if (socket != nullptr)
		{
			socket->setBlocking(false);
			session = new HTTPSession(std::move(*socket));
			if (session->sock.recv() > 0) // data received
			{
				const Jupiter::ReadableString &sock_buffer = session->sock.getBuffer();
				if (sock_buffer.size() < Jupiter::HTTP::Server::data_->max_request_size) // accept
				{
					session->request = session->sock.getBuffer();
					if (sock_buffer.find(HTTP_REQUEST_ENDING) != Jupiter::INVALID_INDEX) // completed request
					{
						Jupiter::HTTP::Server::data_->process_request(*session);
						if (session->keep_alive) // session will live for 30 seconds.
							Jupiter::HTTP::Server::data_->sessions.add(session);
						else // session completed
							delete session;
					}
					else // store for more processing
						Jupiter::HTTP::Server::data_->sessions.add(session);
				}
				else if (sock_buffer.size() == Jupiter::HTTP::Server::data_->max_request_size)
				{
					if (sock_buffer.find(HTTP_REQUEST_ENDING) == Jupiter::INVALID_INDEX) // reject (too large)
						delete session;
					else // accept (max size, completed request)
					{
						session->request = session->sock.getBuffer();
						Jupiter::HTTP::Server::data_->process_request(*session);
						if (session->keep_alive) // session will live for 30 seconds.
							Jupiter::HTTP::Server::data_->sessions.add(session);
						else // session completed
							delete session;
					}
				}
				else // reject (too large)
					delete session;
			}
			else if (session->sock.getLastError() == 10035) // store for more processing
				Jupiter::HTTP::Server::data_->sessions.add(session);
		}
	}
	return 0;
}

const Jupiter::ReadableString &Jupiter::HTTP::Server::global_namespace = Jupiter::ReferenceString::empty;
const Jupiter::ReadableString &Jupiter::HTTP::Server::server_string = "Jupiter"_jrs;