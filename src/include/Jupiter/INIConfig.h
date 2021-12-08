/**
 * Copyright (C) 2016-2021 Jessica James.
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

#if !defined JUPITER_INICONFIG_H_HEADER
#define JUPITER_INICONFIG_H_HEADER

/**
 * @file INIConfig.h
 * @brief Provides an INI config implementation for the Config interface.
 */

#include "Config.h"

/** DLL Linkage Nagging */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace Jupiter
{
	/**
	* @brief INI-based Config class
	* @see Jupiter::Config
	*/
	class JUPITER_API INIConfig : public Jupiter::Config
	{
	private:
		bool read_internal(const char *in_filename) override;
		bool write_internal(const char *in_filename) override;
		void write_helper(FILE *in_file, const Jupiter::Config *in_section, size_t in_depth);
	};
}

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // JUPITER_INICONFIG_H_HEADER