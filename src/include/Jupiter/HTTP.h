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

#if !defined _HTTP_H_HEADER
#define _HTTP_H_HEADER

/**
 * @file HTTP.h
 * @brief Defines constants related to HTTP
 */

namespace Jupiter
{
	namespace HTTP
	{
		/**
		* @brief HTTP status codes
		*/
		namespace Status
		{
			/** Informational (1xx) */
			static const int CONTINUE = 100;
			static const int SWITCHING_PROTOCOLS = 101;
			static const int PROCESSING = 102; // RFC 2518

			/** Success (2xx) */
			static const int OK = 200;
			static const int CREATED = 201;
			static const int ACCEPTED = 202;
			static const int NONAUTH_INFO = 203; // HTTP/1.1
			static const int NO_CONTENT = 204;
			static const int RESET_CONTENT = 205;
			static const int PARTIAL_CONTENT = 206; // RFC 7233
			static const int MULTI_STATUS = 207; // RFC 4918
			static const int ALREADY_REPORTED = 208; // RFC 5842
			static const int IM_USED = 226; // RFC 3229
			
			/** Redirection (3xx) */
			static const int MULTIPLE_CHOICES = 300;
			static const int MOVED_PERMANENTLY = 301;
			static const int FOUND = 302;
			static const int SEE_OTHER = 303; // HTTP/1.1
			static const int NOT_MODIFIED = 304; // RFC 7232
			static const int USE_PROXY = 305; // HTTP/1.1
			static const int SWITCH_PROXY = 306; // No longer used
			static const int TEMPORARY_REDIRECT = 307; // HTTP/1.1
			static const int PERMANENT_REDIRECT = 308; // RFC 7538
			static const int RESUME_INCOMPLETE = 308; // Google

			/** Client Error (4xx) */
			static const int BAD_REQUEST = 400;
			static const int UNAUTHORIZED = 401; // RFC 7235
			static const int PAYMENT_REQUIRED = 402;
			static const int FORBIDDEN = 403;
			static const int NOT_FOUND = 404;
			static const int METHOD_NOT_ALLOWED = 405;
			static const int NOT_ACCEPTABLE = 406;
			static const int PROXY_AUTHENTICATION_REQUIRED = 407; // RFC 7235
			static const int REQUEST_TIMEOUT = 408;
			static const int CONFLICT = 409;
			static const int GONE = 410;
			static const int LENGTH_REQUIRED = 411;
			static const int PRECONDITION_FAILED = 412; // RFC 7232
			static const int PAYLOAD_TOO_LARGE = 413; // RFC 7231
			static const int REQUEST_URI_TOO_LONG = 414;
			static const int UNSUPPORTED_MEDIA_TYPE = 415;
			static const int REQUESTED_RANGE_NOT_SATISFIABLE = 416; // RFC 7233
			static const int EXPECTATION_FAILED = 417;
			static const int IM_A_TEAPOT = 418; // RFC 2324
			static const int AUTHENTICATION_TIMEOUT = 419; // Non-RFC
			static const int METHOD_FAILURE = 420; // Spring Framework
			static const int ENHANCE_YOUR_CALM = 420; // Twitter
			static const int MISDIRECTED_REQUEST = 421; // RFC 7540
			static const int UNPROCESSABLE_ENTITY = 422; // RFC 4918
			static const int LOCKED = 423; // RFC 4918
			static const int FAILED_DEPENDENCY = 424; // RFC 4918
			static const int UPGRADE_REQUIRED = 426;
			static const int PRECONDITION_REQUIRED = 428; // RFC 6585
			static const int TOO_MANY_REQUESTS = 429; // RFC 6585
			static const int REQUEST_HEADER_FIELDS_TOO_LARGE = 431; // RFC 6585
			static const int LOGIN_TIMEOUT = 440; // Microsoft
			static const int NO_RESPONSE = 444; // Nginx
			static const int RETRY_WITH = 449; // Microsoft
			static const int BLOCKED_BY_WINDOWS_PARENTAL_CONTROLS = 450; // Microsoft
			static const int UNAVAILABLE_FOR_LEGAL_REASONS = 451; // "A New HTTP Status Code for Legally-restricted Resources"
			static const int REDIRECT = 451; // Microsoft
			static const int REQUEST_HEADER_TOO_LARGE = 494; // Nginx
			static const int CERT_ERROR = 495; // Nginx
			static const int NO_CERT = 496; // Nginx
			static const int HTTP_TO_HTTPS = 497; // Nginx
			static const int TOKEN_INVALID = 498; // Esri
			static const int CLIENT_CLOSED_REQUEST = 499; // Nginx
			static const int TOKEN_REQUIRED = 499; // Esri

			/** Server Error (5xx) */
			static const int INTERNAL_SERVER_ERROR = 500;
			static const int NOT_IMPLEMENTED = 501;
			static const int BAD_GATEWAY = 502;
			static const int SERVICE_UNAVAIABLE = 503;
			static const int GATEWAY_TIMEOUT = 504;
			static const int HTTP_VERSION_NOT_SUPPORTED = 505;
			static const int VARIANT_ALSO_NEGOTIATES = 506; // RFC 2295
			static const int INSUFFICIENT_STORAGE = 507; // RFC 4918
			static const int LOOP_DETECTED = 508; // RFC 5842
			static const int BANDWIDTH_LIMIT_EXCEEDED = 509; // Apache
			static const int NOT_EXTENDED = 510; // RFC 2774
			static const int NETWORK_AUTHENTICATION_REQUIRED = 511; // RFC 6585
			static const int UNKNOWN_ERROR = 520; // Non-RFC
			static const int ORIGIN_CONNECTION_TIMEOUT = 522; // CloudFlare
			static const int NETWORK_READ_TIMEOUT = 598; // Microsoft
			static const int NETWORK_CONNECTION_TIMEOUT  = 599; // Microsoft
		} // Jupiter::HTTP::Status namespace

		/**
		* @brief Content-type field values
		*/
		namespace Content
		{
			/**
			* @brief Content-Language values
			* TODO: Implement all ISO 639-1 translations
			*/
			namespace Language {
				static constexpr std::string_view ENGLISH = "en";
				static constexpr std::string_view FRENCH = "fr";
				static constexpr std::string_view GERMAN = "gr";
				static constexpr std::string_view RUSSIAN = "ru";
				static constexpr std::string_view JAPANESE = "ja";
				static constexpr std::string_view CHINESE = "zh";
			}

			/**
			* @brief Content-Type values
			* Note: This is not an exhaustive/exclusive list of values
			*/
			namespace Type
			{
				namespace Text
				{
					namespace Charset
					{
						static constexpr std::string_view ASCII = "ascii";
						static constexpr std::string_view UTF8 = "utf-8";
					}

					static constexpr std::string_view HTML = "text/html";
					static constexpr std::string_view PLAIN = "text/plain";
				}
				namespace Application
				{
					static constexpr std::string_view OCTET_STREAM = "application/octet-stream";
				}
			}
		}
	} // Jupiter::HTTP namespace
} // Jupiter namespace

#endif // _HTTP_H_HEADER