/**
 * Copyright (C) 2015 Jessica James.
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

#include "Jupiter.h"
#include "Thinker.h"
#include "Readable_String.h"
#include "ArrayList.h"

namespace Jupiter
{
	namespace HTTP
	{
		class JUPITER_API Server : public Thinker
		{
		public: // Jupiter::Thinker
			virtual int think();

		public: // Server
			typedef Jupiter::ReadableString *HTTPFunction(const Jupiter::ReadableString &parameters);
			static const Jupiter::ReadableString &global_namespace;
			static const Jupiter::ReadableString &server_string;

			struct JUPITER_API Content
			{
				Jupiter::HTTP::Server::HTTPFunction *function; // function to generate content data
				Jupiter::StringS name; // name of the content
				unsigned int name_checksum; // name.calcChecksum()
				const Jupiter::ReadableString *language = nullptr; // Pointer to a constant (or otherwise managed) string
				const Jupiter::ReadableString *type = nullptr; // Pointer to a constant (or otherwise managed) string
				const Jupiter::ReadableString *charset = nullptr; // Pointer to a constant (or otherwise managed) string

				virtual Jupiter::ReadableString *execute(const Jupiter::ReadableString &parameters);

				Content(const Jupiter::ReadableString &in_name, Jupiter::HTTP::Server::HTTPFunction in_function);
			};

			class JUPITER_API Directory
			{
			public:
				Jupiter::ArrayList<Jupiter::HTTP::Server::Directory> directories;
				Jupiter::ArrayList<Jupiter::HTTP::Server::Content> content;
				Jupiter::StringS name;
				unsigned int name_checksum;

				virtual void hook(const Jupiter::ReadableString &path, Content *in_content);
				virtual bool remove(const Jupiter::ReadableString &name);
				virtual bool has(const Jupiter::ReadableString &name);
				virtual Jupiter::HTTP::Server::Content *find(const Jupiter::ReadableString &name);
				virtual Jupiter::ReadableString *execute(const Jupiter::ReadableString &name, const Jupiter::ReadableString &parameters);

				Directory(const Jupiter::ReadableString &in_name);
				virtual ~Directory();
			};

			class JUPITER_API Host : public Directory
			{
			public:
				/*virtual bool remove(const Jupiter::ReadableString &name);
				virtual bool has(const Jupiter::ReadableString &name);
				virtual Jupiter::HTTP::Server::Content *find(const Jupiter::ReadableString &name);
				//virtual Jupiter::HTTP::Server::Directory *find_directory(const Jupiter::ReadableString &name);
				virtual Jupiter::ReadableString *execute(const Jupiter::ReadableString &name, const Jupiter::ReadableString &parameters);*/

				Host(const Jupiter::ReadableString &in_name);
			};

			void hook(const Jupiter::ReadableString &host, const Jupiter::ReadableString &path, Content *in_content);
			bool remove(const Jupiter::ReadableString &host);
			bool remove(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name);
			bool has(const Jupiter::ReadableString &host);
			bool has(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name);
			Content *find(const Jupiter::ReadableString &name);
			Content *find(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name);
			Jupiter::ReadableString *execute(const Jupiter::ReadableString &name, const Jupiter::ReadableString &parameters);
			Jupiter::ReadableString *execute(const Jupiter::ReadableString &host, const Jupiter::ReadableString &name, const Jupiter::ReadableString &parameters);

			bool bind(const Jupiter::ReadableString &hostname, uint16_t port = 80);
			bool tls_bind(const Jupiter::ReadableString &hostname, uint16_t port = 443);

			Server();
			Server(Jupiter::HTTP::Server &&source);
			~Server();
		/** Private members */
		private:
			struct Data;
			Data *data_;
		}; // Jupiter::HTTP::Server class
	} // Jupiter::HTTP namespace
} // Jupiter namespace

#endif // _HTTP_SERVER_H_HEADER