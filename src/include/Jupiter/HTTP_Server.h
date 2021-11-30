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

#if !defined _HTTP_SERVER_H_HEADER
#define _HTTP_SERVER_H_HEADER

/**
 * @file HTTP_Server.h
 * @brief Provides an interface to distribute data using HTTP.
 */

#include <memory>
#include "Jupiter.h"
#include "Thinker.h"
#include "Readable_String.h"

/** DLL Linkage Nagging */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace Jupiter
{
	namespace HTTP
	{
		class JUPITER_API Server : public Thinker
		{
		public: // Jupiter::Thinker
			virtual int think();

		public: // Server
			typedef std::string* HTTPFunction(std::string_view query_string);
			static const Jupiter::ReadableString &global_namespace;
			static const Jupiter::ReadableString &server_string;

			struct JUPITER_API Content
			{
				bool free_result = true;
				Jupiter::HTTP::Server::HTTPFunction *function; // function to generate content data
				std::string name; // name of the content
				unsigned int name_checksum; // name.calcChecksum()
				const Jupiter::ReadableString *language = nullptr; // Pointer to a constant (or otherwise managed) string
				const Jupiter::ReadableString *type = nullptr; // Pointer to a constant (or otherwise managed) string
				const Jupiter::ReadableString *charset = nullptr; // Pointer to a constant (or otherwise managed) string

				virtual std::string* execute(std::string_view query_string);

				Content(std::string in_name, Jupiter::HTTP::Server::HTTPFunction in_function);
			};

			class JUPITER_API Directory
			{
			public:
				std::vector<std::unique_ptr<Server::Directory>> directories;
				std::vector<std::unique_ptr<Server::Content>> content;
				std::string name;
				unsigned int name_checksum;

				virtual void hook(std::string_view path, std::unique_ptr<Content> in_content);
				virtual bool remove(std::string_view path, std::string_view name);
				virtual bool has(std::string_view name);
				virtual Jupiter::HTTP::Server::Content* find(std::string_view name);
				virtual std::string* execute(std::string_view name, std::string_view query_string);

				Directory(const Directory&) = delete;
				Directory& operator=(const Directory&) = delete;
				Directory(std::string in_name);
				virtual ~Directory();
			};

			class JUPITER_API Host : public Directory
			{
			public:
				Host(std::string in_name);
			};

			void hook(std::string_view host, std::string_view path, std::unique_ptr<Content> in_content);
			bool remove(std::string_view host);
			bool remove(std::string_view host, std::string_view path, std::string_view name);
			bool has(std::string_view host);
			bool has(std::string_view host, std::string_view name);
			Content *find(std::string_view name);
			Content *find(std::string_view host, std::string_view name);
			std::string* execute(std::string_view name, std::string_view query_string);
			std::string* execute(std::string_view host, std::string_view name, std::string_view query_string);

			bool bind(std::string_view hostname, uint16_t port = 80);
			bool tls_bind(std::string_view hostname, uint16_t port = 443);

			Server();
			Server(Jupiter::HTTP::Server &&source);
			~Server();
		/** Private members */
		private:
			struct Data;
			Data *m_data;
		}; // Jupiter::HTTP::Server class
	} // Jupiter::HTTP namespace
} // Jupiter namespace

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // _HTTP_SERVER_H_HEADER