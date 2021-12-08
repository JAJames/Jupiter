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

#if !defined _HTTP_H_HEADER
#define _HTTP_H_HEADER

/**
 * @file HTTP.h
 * @brief Defines constants related to HTTP
 */

namespace Jupiter {
namespace HTTP {

/**
* @brief HTTP status codes
*/
namespace Status {

/** Informational (1xx) */
static constexpr int CONTINUE = 100;
static constexpr int SWITCHING_PROTOCOLS = 101;
static constexpr int PROCESSING = 102; // RFC 2518

/** Success (2xx) */
static constexpr int OK = 200;
static constexpr int CREATED = 201;
static constexpr int ACCEPTED = 202;
static constexpr int NONAUTH_INFO = 203; // HTTP/1.1
static constexpr int NO_CONTENT = 204;
static constexpr int RESET_CONTENT = 205;
static constexpr int PARTIAL_CONTENT = 206; // RFC 7233
static constexpr int MULTI_STATUS = 207; // RFC 4918
static constexpr int ALREADY_REPORTED = 208; // RFC 5842
static constexpr int IM_USED = 226; // RFC 3229
			
/** Redirection (3xx) */
static constexpr int MULTIPLE_CHOICES = 300;
static constexpr int MOVED_PERMANENTLY = 301;
static constexpr int FOUND = 302;
static constexpr int SEE_OTHER = 303; // HTTP/1.1
static constexpr int NOT_MODIFIED = 304; // RFC 7232
static constexpr int USE_PROXY = 305; // HTTP/1.1
static constexpr int SWITCH_PROXY = 306; // No longer used
static constexpr int TEMPORARY_REDIRECT = 307; // HTTP/1.1
static constexpr int PERMANENT_REDIRECT = 308; // RFC 7538
static constexpr int RESUME_INCOMPLETE = 308; // Google

/** Client Error (4xx) */
static constexpr int BAD_REQUEST = 400;
static constexpr int UNAUTHORIZED = 401; // RFC 7235
static constexpr int PAYMENT_REQUIRED = 402;
static constexpr int FORBIDDEN = 403;
static constexpr int NOT_FOUND = 404;
static constexpr int METHOD_NOT_ALLOWED = 405;
static constexpr int NOT_ACCEPTABLE = 406;
static constexpr int PROXY_AUTHENTICATION_REQUIRED = 407; // RFC 7235
static constexpr int REQUEST_TIMEOUT = 408;
static constexpr int CONFLICT = 409;
static constexpr int GONE = 410;
static constexpr int LENGTH_REQUIRED = 411;
static constexpr int PRECONDITION_FAILED = 412; // RFC 7232
static constexpr int PAYLOAD_TOO_LARGE = 413; // RFC 7231
static constexpr int REQUEST_URI_TOO_LONG = 414;
static constexpr int UNSUPPORTED_MEDIA_TYPE = 415;
static constexpr int REQUESTED_RANGE_NOT_SATISFIABLE = 416; // RFC 7233
static constexpr int EXPECTATION_FAILED = 417;
static constexpr int IM_A_TEAPOT = 418; // RFC 2324
static constexpr int AUTHENTICATION_TIMEOUT = 419; // Non-RFC
static constexpr int METHOD_FAILURE = 420; // Spring Framework
static constexpr int ENHANCE_YOUR_CALM = 420; // Twitter
static constexpr int MISDIRECTED_REQUEST = 421; // RFC 7540
static constexpr int UNPROCESSABLE_ENTITY = 422; // RFC 4918
static constexpr int LOCKED = 423; // RFC 4918
static constexpr int FAILED_DEPENDENCY = 424; // RFC 4918
static constexpr int UPGRADE_REQUIRED = 426;
static constexpr int PRECONDITION_REQUIRED = 428; // RFC 6585
static constexpr int TOO_MANY_REQUESTS = 429; // RFC 6585
static constexpr int REQUEST_HEADER_FIELDS_TOO_LARGE = 431; // RFC 6585
static constexpr int LOGIN_TIMEOUT = 440; // Microsoft
static constexpr int NO_RESPONSE = 444; // Nginx
static constexpr int RETRY_WITH = 449; // Microsoft
static constexpr int BLOCKED_BY_WINDOWS_PARENTAL_CONTROLS = 450; // Microsoft
static constexpr int UNAVAILABLE_FOR_LEGAL_REASONS = 451; // "A New HTTP Status Code for Legally-restricted Resources"
static constexpr int REDIRECT = 451; // Microsoft
static constexpr int REQUEST_HEADER_TOO_LARGE = 494; // Nginx
static constexpr int CERT_ERROR = 495; // Nginx
static constexpr int NO_CERT = 496; // Nginx
static constexpr int HTTP_TO_HTTPS = 497; // Nginx
static constexpr int TOKEN_INVALID = 498; // Esri
static constexpr int CLIENT_CLOSED_REQUEST = 499; // Nginx
static constexpr int TOKEN_REQUIRED = 499; // Esri

/** Server Error (5xx) */
static constexpr int INTERNAL_SERVER_ERROR = 500;
static constexpr int NOT_IMPLEMENTED = 501;
static constexpr int BAD_GATEWAY = 502;
static constexpr int SERVICE_UNAVAIABLE = 503;
static constexpr int GATEWAY_TIMEOUT = 504;
static constexpr int HTTP_VERSION_NOT_SUPPORTED = 505;
static constexpr int VARIANT_ALSO_NEGOTIATES = 506; // RFC 2295
static constexpr int INSUFFICIENT_STORAGE = 507; // RFC 4918
static constexpr int LOOP_DETECTED = 508; // RFC 5842
static constexpr int BANDWIDTH_LIMIT_EXCEEDED = 509; // Apache
static constexpr int NOT_EXTENDED = 510; // RFC 2774
static constexpr int NETWORK_AUTHENTICATION_REQUIRED = 511; // RFC 6585
static constexpr int UNKNOWN_ERROR = 520; // Non-RFC
static constexpr int ORIGIN_CONNECTION_TIMEOUT = 522; // CloudFlare
static constexpr int NETWORK_READ_TIMEOUT = 598; // Microsoft
static constexpr int NETWORK_CONNECTION_TIMEOUT  = 599; // Microsoft

} // namespace Status

/**
* @brief Content-type field values
*/
namespace Content {

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

} // namespace Language

/**
* @brief Content-Type values
* Note: This is not an exhaustive/exclusive list of values
*/
namespace Type {

namespace Text {

static constexpr std::string_view HTML = "text/html";
static constexpr std::string_view PLAIN = "text/plain";

namespace Charset {

static constexpr std::string_view ASCII = "ascii";
static constexpr std::string_view UTF8 = "utf-8";

} // namespace Charset

} // namespace Text

namespace Application {
static constexpr std::string_view OCTET_STREAM = "application/octet-stream";
} // namespace Application

} // namespace Type
} // namespace Content
} // namespace HTTP
} // namespace Jupiter

#endif // _HTTP_H_HEADER