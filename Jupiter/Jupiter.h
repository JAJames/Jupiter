/**
 * Copyright (C) 2013-2015 Justin James.
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

#if !defined JUPITER_API

/**
 * @file Jupiter.h
 * @brief Defines JUPITER_API and provides metadata about the program.
 * On anything other than Windows, JUPITER_API is defined as nothing, to prevent compiler errors.
 */

#if defined _WIN32

#if defined JUPITER_EXPORTS
#define JUPITER_API __declspec(dllexport) 
#else // JUPITER_EXPORTS
#define JUPITER_API __declspec(dllimport) 
#endif // JUPITER_EXPORTS

#else // _WIN32
#define JUPITER_API
#endif // _WIN32

#define JUPITER_VERSION "Jupiter 0.15" /** Version of this program at compile time. */

#if defined __cplusplus
extern "C"
{
#endif // __cplusplus

	JUPITER_API extern const char *Jupiter_version; /** Jupiter version number at runtime */
	JUPITER_API extern const char *Jupiter_copyright; /** Jupiter copyright notice */

#if defined __cplusplus
}

namespace Jupiter
{
	JUPITER_API extern const char *version; /** @see Jupiter_version */
	JUPITER_API extern const char *copyright; /** @see Jupiter_copyright */
}

#endif // __cplusplus

#endif // JUPITER_API