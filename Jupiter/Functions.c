/**
 * Copyright (C) 2013-2016 Jessica James.
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

#include <stdint.h> // uintxx_t typedefs.
#include <time.h> // Used by getTime()
#include <ctype.h> // isspace
#include <math.h> // pow
#include "Functions.h"

uint32_t getPowerTwo32(uint32_t v)
{
	--v;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	return ++v;

}

uint64_t getPowerTwo64(uint64_t v)
{
	--v;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	return ++v;
}

bool containsSymbol(const char *str, char c)
{
	while (*str != 0)
	{
		if (*str == c)
			return true;
		++str;
	}
	return false;
}

char *getTime()
{
	return getTimeFormat("%a %b %d %H:%M:%S %Y %Z");
}

char *getTimeFormat(const char *format)
{
	time_t rawtime = time(0);
	static char rtime[256];
	strftime(rtime, sizeof(rtime), format, localtime(&rawtime));
	return rtime;
}

/** Character to integer conversion tables */

const unsigned char base_table[] =
{
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 127, 127, 127, 127, 127, 127,
	127, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 127, 127, 127, 127, 127,
	127, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 127, 127, 127, 127, 127,

	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127
};

const signed char hexadecimal_table[] =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, 16, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, 16, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

const signed char decimal_table[] =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

const signed char octal_table[] =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

/** Integer to string (hexadecimal) conversion tables */

const char hexadecimal_rep_table_upper[][3] =
{
	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
	"10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
	"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
	"30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
	"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
	"50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
	"60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
	"70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
	"80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
	"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
	"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
	"B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
	"C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
	"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
	"E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
	"F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
};

const char hexadecimal_rep_table_lower[][3] =
{
	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f",
	"10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f",
	"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f",
	"30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f",
	"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4a", "4b", "4c", "4d", "4e", "4f",
	"50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5a", "5b", "5c", "5d", "5e", "5f",
	"60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b", "6c", "6d", "6e", "6f",
	"70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d", "7e", "7f",
	"80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
	"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f",
	"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af",
	"b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf",
	"c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf",
	"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "da", "db", "dc", "dd", "de", "df",
	"e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef",
	"f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb", "fc", "fd", "fe", "ff"
};

/** Single character functions */

bool Jupiter_isBase(unsigned char chr, int base)
{
	return base_table[chr] < base;
}

bool Jupiter_isHex(unsigned char chr)
{
	return hexadecimal_table[chr] != -1;
}

bool Jupiter_isDecimal(unsigned char chr)
{
	return decimal_table[chr] != -1;
}

bool Jupiter_isOctal(unsigned char chr)
{
	return octal_table[chr] != -1;
}

int Jupiter_getBase(unsigned char chr, int base)
{
	chr = base_table[chr];
	if (chr < base)
		return chr;
	return -1;
}

int Jupiter_getHex(unsigned char chr)
{
	return hexadecimal_table[chr];
}

int Jupiter_getDecimal(unsigned char chr)
{
	return decimal_table[chr];
}

int Jupiter_getOctal(unsigned char chr)
{
	return octal_table[chr];
}

const char *Jupiter_asHex_upper(uint8_t oct)
{
	return hexadecimal_rep_table_upper[oct];
}

const char *Jupiter_asHex_lower(uint8_t oct)
{
	return hexadecimal_rep_table_lower[oct];
}

/** Unsafe string converters */

int Jupiter_strtoi(const char *str, int base)
{
	while (isspace(*str))
		++str;
	return Jupiter_strtoi_nospace(str, base);
}

long long Jupiter_strtoll(const char *str, int base)
{
	while (isspace(*str))
		++str;
	return Jupiter_strtoll_nospace(str, base);
}

unsigned int Jupiter_strtoui(const char *str, int base)
{
	while (isspace(*str))
		++str;
	return Jupiter_strtoui_nospace(str, base);
}

unsigned long long Jupiter_strtoull(const char *str, int base)
{
	while (isspace(*str))
		++str;
	return Jupiter_strtoull_nospace(str, base);
}

double Jupiter_strtod(const char *str)
{
	while (isspace(*str))
		++str;
	return Jupiter_strtod_nospace(str);
}

int Jupiter_strtoi_nospace(const char *str, int base)
{
	if (*str != '-') return Jupiter_strtoui_nospace(str, base);
	return -1 * Jupiter_strtoui_nospace(++str, base);
}

long long Jupiter_strtoll_nospace(const char *str, int base)
{
	if (*str != '-') return Jupiter_strtoull_nospace(str, base);
	return -1 * Jupiter_strtoull_nospace(++str, base);
}

unsigned int Jupiter_strtoui_nospace(const char *str, int base)
{
	unsigned int total = 0;
	int tval;

	if (*str == '-')
		return Jupiter_strtoi_nospace(str, base);

	if (*str == '+')
		++str;

	if (base == 0) // Guess a base.
	{
		if (*str == '0')
		{
			switch (*++str)
			{
			case 'X':
			case 'x':
				++str;
				base = 16;
				break;
			case 'B':
			case 'b':
				++str;
				base = 2;
				break;
			default:
				base = 8;
				break;
			}
		}
		else base = 10;
	}
	else if (base == 16) // check for optional preceeding hexadecimal prefix.
	{
		if (*str == '0')
		{
			++str;
			if (*str == 'x' || *str == 'X')
				++str;
		}
	}

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		++str;
	}

	return total;
}

unsigned long long Jupiter_strtoull_nospace(const char *str, int base)
{
	unsigned long long total = 0;
	int tval;

	if (*str == '-')
		return Jupiter_strtoi_nospace(str, base);

	if (*str == '+')
		++str;

	if (base == 0) // Guess a base.
	{
		if (*str == '0')
		{
			switch (*++str)
			{
			case 'X':
			case 'x':
				++str;
				base = 16;
				break;
			case 'B':
			case 'b':
				++str;
				base = 2;
				break;
			default:
				base = 8;
				break;
			}
		}
		else base = 10;
	}
	else if (base == 16) // check for optional preceeding hexadecimal prefix.
	{
		if (*str == '0')
		{
			++str;
			if (*str == 'x' || *str == 'X')
				++str;
		}
	}

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		++str;
	}

	return total;
}

double Jupiter_strtod_nospace(const char *str)
{
	const int base = 10;
	double total = 0.0;
	double decimal = 0.0;
	int tval;

	if (*str == '-')
		return -1 * Jupiter_strtod_nospace(++str);

	if (*str == '+')
		++str;

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		++str;
	}

	if (*str == '.')
	{
		++str;
		tval = base;
		while ((decimal = Jupiter_getBase(*str, base)) != -1)
		{
			total = total + (decimal / tval);
			++str;
			tval = tval * base;
		}
		if (*str == 'e' || *str == 'E')
			total = total * pow(base, Jupiter_strtoi_nospace(++str, 10));
	}

	return total;
}

/** Safe string converters */

int Jupiter_strtoi_s(const char *str, size_t length, int base)
{
	while (length != 0 && isspace(*str))
		++str, --length;

	return Jupiter_strtoi_nospace_s(str, length, base);
}

long long Jupiter_strtoll_s(const char *str, size_t length, int base)
{
	while (length != 0 && isspace(*str))
		++str, --length;

	return Jupiter_strtoll_nospace_s(str, length, base);
}

unsigned int Jupiter_strtoui_s(const char *str, size_t length, int base)
{
	while (length != 0 && isspace(*str))
		++str, --length;

	return Jupiter_strtoui_nospace_s(str, length, base);
}

unsigned long long Jupiter_strtoull_s(const char *str, size_t length, int base)
{
	while (length != 0 && isspace(*str))
		++str, --length;

	return Jupiter_strtoull_nospace_s(str, length, base);
}

double Jupiter_strtod_s(const char *str, size_t length)
{
	while (length != 0 && isspace(*str))
		++str, --length;

	return Jupiter_strtod_nospace_s(str, length);
}

int Jupiter_strtoi_nospace_s(const char *str, size_t length, int base)
{
	if (length == 0)
		return 0;

	if (*str != '-')
		return Jupiter_strtoui_nospace_s(str, length, base);

	return -1 * Jupiter_strtoui_nospace_s(++str, --length, base);
}

unsigned int Jupiter_strtoui_nospace_s(const char *str, size_t length, int base)
{
	unsigned int total = 0;
	int tval;

	if (length == 0)
		return total;

	if (*str == '-')
		return Jupiter_strtoi_nospace(str, base);

	if (*str == '+')
	{
		if (--length == 0)
			return total;
		++str;
	}

	if (base == 0) // Guess a base.
	{
		if (*str == '0')
		{
			if (--length == 0)
				return total;

			++str;
			switch (*str)
			{
			case 'X':
			case 'x':
				if (--length == 0)
					return total;

				++str;
				base = 16;
				break;

			case 'B':
			case 'b':
				if (--length == 0)
					return total;

				++str;
				base = 2;
				break;

			default:
				base = 8;
				break;
			}
		}
		else base = 10;
	}
	else if (base == 16) // check for optional preceeding hexadecimal prefix.
	{
		if (*str == '0')
		{
			if (--length == 0)
				return total;
			++str;
			if (*str == 'x' || *str == 'X')
			{
				if (--length == 0)
					return total;
				++str;
			}
		}
	}

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		if (--length == 0)
			return total;
		++str;
	}

	return total;
}

long long Jupiter_strtoll_nospace_s(const char *str, size_t length, int base)
{
	if (length == 0) return 0;
	if (*str != '-') return Jupiter_strtoull_nospace_s(str, length, base);
	return -1 * Jupiter_strtoull_nospace_s(++str, --length, base);
}

unsigned long long Jupiter_strtoull_nospace_s(const char *str, size_t length, int base)
{
	unsigned long long total = 0;
	int tval;

	if (length == 0)
		return total;

	if (*str == '-')
		return Jupiter_strtoi_nospace(str, base);

	if (*str == '+')
	{
		if (--length == 0)
			return total;
		++str;
	}

	if (base == 0) // Guess a base.
	{
		if (*str == '0')
		{
			if (--length == 0)
				return total;

			++str;
			switch (*str)
			{
			case 'X':
			case 'x':
				if (--length == 0)
					return total;

				++str;
				base = 16;
				break;
			case 'B':
			case 'b':
				if (--length == 0)
					return total;

				++str;
				base = 2;
				break;
			default:
				base = 8;
				break;
			}
		}
		else base = 10;
	}
	else if (base == 16) // check for optional preceeding hexadecimal prefix.
	{
		if (*str == '0')
		{
			if (--length == 0)
				return total;

			++str;
			if (*str == 'x' || *str == 'X')
			{
				if (--length == 0)
					return total;

				++str;
			}
		}
	}

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		if (--length == 0)
			return total;
		++str;
	}

	return total;
}

double Jupiter_strtod_nospace_s(const char *str, size_t length)
{
	const int base = 10;
	double total = 0.0;
	double decimal = 0.0;
	int tval;

	if (length == 0)
		return 0;

	if (*str == '-')
		return -1 * Jupiter_strtod_nospace_s(++str, --length);

	if (*str == '+')
		++str, --length;

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		if (--length == 0)
			return total;
		++str;
	}

	if (*str == '.')
	{
		++str;
		--length;

		tval = base;
		while (length != 0 && (decimal = Jupiter_getBase(*str, base)) != -1)
		{
			total = total + (decimal / tval);
			if (--length == 0)
				return total;
			++str;
			tval = tval * base;
		}
		if (*str == 'e' || *str == 'E')
			total = total * pow(base, Jupiter_strtoi_nospace_s(++str, --length, 10));
	}

	return total;
}

/** Prefix length to bitmask conversion tables */

const uint32_t bitmask_table[] =
{
	0x00000000U, 0x80000000U, 0xC0000000U, 0xE0000000U,
	0xF0000000U, 0xF8000000U, 0xFC000000U, 0xFE000000U,
	0xFF000000U, 0xFF800000U, 0xFFC00000U, 0xFFE00000U,
	0xFFF00000U, 0xFFF80000U, 0xFFFC0000U, 0xFFFE0000U,
	0xFFFF0000U, 0xFFFF8000U, 0xFFFFC000U, 0xFFFFE000U,
	0xFFFFF000U, 0xFFFFF800U, 0xFFFFFC00U, 0xFFFFFE00U,
	0xFFFFFF00U, 0xFFFFFF80U, 0xFFFFFFC0U, 0xFFFFFFE0U,
	0xFFFFFFF0U, 0xFFFFFFF8U, 0xFFFFFFFCU, 0xFFFFFFFEU,
	0xFFFFFFFFU
};

const uint8_t netmask_table[][4] =
{
	{ 0x00, 0x00, 0x00, 0x00 }, { 0x80, 0x00, 0x00, 0x00 }, { 0xC0, 0x00, 0x00, 0x00 }, { 0xE0, 0x00, 0x00, 0x00 },
	{ 0xF0, 0x00, 0x00, 0x00 }, { 0xF8, 0x00, 0x00, 0x00 }, { 0xFC, 0x00, 0x00, 0x00 }, { 0xFE, 0x00, 0x00, 0x00 },
	{ 0xFF, 0x00, 0x00, 0x00 }, { 0xFF, 0x80, 0x00, 0x00 }, { 0xFF, 0xC0, 0x00, 0x00 }, { 0xFF, 0xE0, 0x00, 0x00 },
	{ 0xFF, 0xF0, 0x00, 0x00 }, { 0xFF, 0xF8, 0x00, 0x00 }, { 0xFF, 0xFC, 0x00, 0x00 }, { 0xFF, 0xFE, 0x00, 0x00 },
	{ 0xFF, 0xFF, 0x00, 0x00 }, { 0xFF, 0xFF, 0x80, 0x00 }, { 0xFF, 0xFF, 0xC0, 0x00 }, { 0xFF, 0xFF, 0xE0, 0x00 },
	{ 0xFF, 0xFF, 0xF0, 0x00 }, { 0xFF, 0xFF, 0xF8, 0x00 }, { 0xFF, 0xFF, 0xFC, 0x00 }, { 0xFF, 0xFF, 0xFE, 0x00 },
	{ 0xFF, 0xFF, 0xFF, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x80 }, { 0xFF, 0xFF, 0xFF, 0xC0 }, { 0xFF, 0xFF, 0xFF, 0xE0 },
	{ 0xFF, 0xFF, 0xFF, 0xF0 }, { 0xFF, 0xFF, 0xFF, 0xF8 }, { 0xFF, 0xFF, 0xFF, 0xFC }, { 0xFF, 0xFF, 0xFF, 0xFE },
	{ 0xFF, 0xFF, 0xFF, 0xFF }
};

uint32_t Jupiter_prefix_length_to_bitmask(uint8_t prefix_length)
{
	return bitmask_table[prefix_length];
}

uint32_t Jupiter_prefix_length_to_netmask(uint8_t prefix_length)
{
	return *(uint32_t *)netmask_table[prefix_length];
}
