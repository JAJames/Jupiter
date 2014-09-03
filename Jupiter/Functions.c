/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include <stdlib.h> // malloc
#include <stdint.h> // uintxx_t typedefs.
#include <stdio.h> // fprintf and stderr
#include <time.h> // Used by getTime()
#include <ctype.h> // toupper
#include <wctype.h> // towupper
#include <math.h> // pow
#include "Functions.h"

// Little Endian
unsigned int getZeroByte(uint32_t *v)
{
	if ((*v & 0x000000FF) == 0) return 0;
	if ((*v & 0x0000FF00) == 0) return 1;
	if ((*v & 0x00FF0000) == 0) return 2;
	if ((*v & 0xFF000000) == 0) return 3;
	return 4;
}

unsigned int getZeroByte2(uint64_t v)
{
	if ((v & 0x00000000000000FF) == 0) return 0;
	if ((v & 0x000000000000FF00) == 0) return 1;
	if ((v & 0x0000000000FF0000) == 0) return 2;
	if ((v & 0x00000000FF000000) == 0) return 3;
	if ((v & 0x000000FF00000000) == 0) return 4;
	if ((v & 0x0000FF0000000000) == 0) return 5;
	if ((v & 0x00FF000000000000) == 0) return 6;
	if ((v & 0xFF00000000000000) == 0) return 7;
	return 8;
}

/*
// Big Endian
unsigned int getZeroByte(uint32_t v)
{
	if ((v & 0xFF000000) == 0) return 0;
	if ((v & 0x00FF0000) == 0) return 1;
	if ((v & 0x0000FF00) == 0) return 2;
	if ((v & 0x000000FF) == 0) return 3;
	return 4;
}

unsigned int getZeroByte64(uint64_t v)
{
	if ((v & 0xFF00000000000000) == 0) return 0;
	if ((v & 0x00FF000000000000) == 0) return 1;
	if ((v & 0x0000FF0000000000) == 0) return 2;
	if ((v & 0x000000FF00000000) == 0) return 3;
	if ((v & 0x00000000FF000000) == 0) return 4;
	if ((v & 0x0000000000FF0000) == 0) return 5;
	if ((v & 0x000000000000FF00) == 0) return 6;
	if ((v & 0x00000000000000FF) == 0) return 7;
	return 8;
}
*/

uint32_t getPowerTwo32(uint32_t v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;

}

uint64_t getPowerTwo64(uint64_t v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v++;
	return v;
}

#define STRLEN_WRAPPER(TYPE) \
	register const TYPE *s = str; \
	while (*s != 0) s++; \
	return s - str;

size_t Jupiter_strlen(const char *str)
{
	STRLEN_WRAPPER(char)
}

size_t Jupiter_wstrlen(const wchar_t *str)
{
	STRLEN_WRAPPER(wchar_t)
}

size_t Jupiter_strlen8(const uint8_t *str)
{
	STRLEN_WRAPPER(uint8_t)
}

size_t Jupiter_strlen16(const uint16_t *str)
{
	STRLEN_WRAPPER(uint16_t)
}

size_t Jupiter_strlen32(const uint32_t *str)
{
	STRLEN_WRAPPER(uint32_t)
}

size_t Jupiter_strlen64(const uint64_t *str)
{
	STRLEN_WRAPPER(uint64_t)
}

size_t Jupiter_vstrlen(const void *str, size_t size)
{
	const char *s;
	const char *s2;
	switch (size)
	{
	case 0:
		return 0;
	case 1:
		return Jupiter_strlen((const char *)str);
	case 2:
		return Jupiter_strlen16((const uint16_t *)str);
	case 4:
		return Jupiter_strlen32((const uint32_t *)str);
	case 8:
		return Jupiter_strlen64((const uint64_t *)str);
	default:
		s = (const char *)str;
		s2 = s;
		while ((unsigned)(s2 - s) != size)
		{
			s2 = s;
			while ((unsigned)(s2 - s) > size)
			{
				if (*s2 != 0) break;
				s2++;
			}
			s += size;
		}
		return (s - (const char *)str) / size;
	}
}

const char *stristr(const char *str1, const char *str2)
{
	size_t i;
	size_t a;
	for (i = 0; str1[i] != 0; i++)
	{
		if (toupper(str1[i]) == toupper(str2[0]))
		{
			a = 1;
			while (str2[a] != 0)
			{
				if (toupper(str1[i + a]) != toupper(str2[a])) break;
				a++;
			}
			if (str2[a] == 0)
			{
				return str1 + i;
			}
		}
	}
	return NULL;
}

bool streql(const char *s1, const char *s2)
{
	if (s1 == s2) return true;
	while (*s1 != 0 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 == *s2);
}

bool streqli(const char *s1, const char *s2)
{
	if (s1 == s2) return true;
	while (*s1 != 0 && toupper(*s1) == toupper(*s2))
	{
		s1++;
		s2++;
	}
	if (*s1 == *s2) return true;
	return false;
}

bool wstreql(const wchar_t *s1, const wchar_t *s2)
{
	if (s1 == s2) return true;
	while (*s1 != 0 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 == *s2);
}

bool wstreqli(const wchar_t *s1, const wchar_t *s2)
{
	if (s1 == s2) return true;
	while (*s1 != 0 && towupper(*s1) == towupper(*s2))
	{
		s1++;
		s2++;
	}
	if (*s1 == *s2) return true;
	return false;
}

bool strmatch(const char *f, const char *s)
{
	while (*f != 0)
	{
		if (*f == '*')
		{
			f++;
			while (*f == '?')
			{
				if (*s == 0) return false;
				f++;
				s++;
			}
			if (*f == 0) return true;
			if (*f == '*') continue;
			while (*f != *s)
			{
				if (*s == 0) return false;
				s++;
			}
		}
		else if (*f != '?' && *f != *s) return false;
		f++;
		s++;
	}
	return *s == 0;
}

bool strmatchi(const char *f, const char *s)
{
	int fUpper;
	while (*f != 0)
	{
		if (*f == '*')
		{
			f++;
			while (*f == '?')
			{
				if (*s == 0) return false;
				f++;
				s++;
			}
			if (*f == 0) return true;
			if (*f == '*') continue;
			fUpper = toupper(*f);
			while (fUpper != toupper(*s))
			{
				if (*s == 0) return false;
				s++;
			}
		}
		else if (*f != '?' && toupper(*f) != toupper(*s)) return false;
		f++;
		s++;
	}
	return *s == 0;
}

bool wstrmatch(const wchar_t *f, const wchar_t *s)
{
	while (*f != 0)
	{
		if (*f == L'*')
		{
			f++;
			while (*f == L'?')
			{
				if (*s == 0) return false;
				f++;
				s++;
			}
			if (*f == 0) return true;
			if (*f == L'*') continue;
			while (*f != *s)
			{
				if (*s == 0) return false;
				s++;
			}
		}
		else if (*f != L'?' && *f != *s) return false;
		f++;
		s++;
	}
	return *s == 0;
}

bool wstrmatchi(const wchar_t *f, const wchar_t *s)
{
	wint_t fUpper;
	while (*f != 0)
	{
		if (*f == L'*')
		{
			f++;
			while (*f == L'?')
			{
				if (*s == 0) return false;
				f++;
				s++;
			}
			if (*f == 0) return true;
			if (*f == L'*') continue;
			fUpper = towupper(*f);
			while (fUpper != towupper(*s))
			{
				if (*s == 0) return false;
				s++;
			}
		}
		else if (*f != L'?' && towupper(*f) != towupper(*s)) return false;
		f++;
		s++;
	}
	return *s == 0;
}

char *charToChar(const char *a, int b, int c)
{
	char *r = (char *) malloc(sizeof(char) * (c-b+1));
	if (r != NULL)
	{
		int i = 0;
		while (b < c)
		{
			r[i] = a[b];
			i++;
			b++;
		}
		r[i] = 0;
	}
	else fprintf(stderr, "ERROR: UNABLE TO ALLOCATE IN %s" ENDL, __FUNCTION__);
	return r;
}

wchar_t *wcharToChar(const wchar_t *a, int b, int c)
{
	wchar_t *r = (wchar_t *)malloc(sizeof(wchar_t)* (c - b + 1));
	if (r != NULL)
	{
		int i = 0;
		while (b < c)
		{
			r[i] = a[b];
			i++;
			b++;
		}
		r[i] = 0;
	}
	else fprintf(stderr, "ERROR: UNABLE TO ALLOCATE IN %s" ENDL, __FUNCTION__);
	return r;
}

void trim(char *str)
{
	int p = 0;
	int x = 0;
	int i;
	while (str[p] != 0)
	{
		if (str[p] == 10)
		{
			str[p] = 0;
		}
		else if (str[p] == 13)
		{
			str[p] = 0;
		}
		p++;
	}
	for (i = 0; i < p; i++)
	{
		if (str[i] != 0)
		{
			str[x] = str[i];
			x++;
		}
	}
	str[x] = 0;
}

char *getWord(const char *str, int w)
{
	char *result;
	int x = 0;
	int y;
	int i;
	for (i = 0; i < w; x++)
	{
		if (str[x] == 0) return NULL;
		if (str[x] == ' ') i++;
	}
	for (y = x; str[y] != ' ' && str[y] != 0; y++);
	result = (char *) malloc(sizeof(char) * (y-x+1));
	if (result != NULL)
	{
		for (i = 0; x < y; i++)
		{
			result[i] = str[x];
			x++;
		}
		result[i] = 0;
	}
	else fprintf(stderr, "ERROR: UNABLE TO ALLOCATE IN %s" ENDL, __FUNCTION__);
	return result;
}

unsigned int countSymbol(const char *str, char c)
{
	unsigned int result = 0;
	while(*str != 0)
	{
		if (*str == c) result++;
		str++;
	}
	return result;
}

unsigned int wordCount(const char *str)
{
	unsigned int result = 0;
	int i;
	for (i = 0; str[i] != 0; i++)
	{
		if (str[i] == ' ')
		{
			if (i > 0)
			{
				if (str[i-1] > ' ') result++;
			}
		}
		else if (str[i+1] < ' ') result++;
	}
	return result;
}

unsigned int countLines(const char *str)
{
	unsigned int r = 0;
	while (*str != 0)
	{
		if ((*str == '\r' && *(str + 1) != '\n') || *str == '\n') r++;
		str++;
	}
	return r;
}

char *getLine(const char *str, unsigned int l)
{
	char *result;
	unsigned int x = 0;
	unsigned int y;
	unsigned int i;
	for (i = 0; i < l; x++)
	{
		if (str[x] == 0) break;
		if (str[x] == '\n' || (str[x] == '\r' && str[x+1] != '\n')) i++;
	}
	for (y = x; str[y] != '\r' && str[y] != '\n' && str[y] != 0; y++);// if (str[y] == 0) break;
	result = (char *) malloc(sizeof(char) * (y-x+1));
	if (result != NULL)
	{
		for (i = 0; x < y; i++)
		{
			result[i] = str[x];
			x++;
		}
		result[i] = 0;
	}
	else fprintf(stderr, "ERROR: UNABLE TO ALLOCATE IN %s" ENDL, __FUNCTION__);
	return result;
}

int findSymbol(const char *str, char c, int pos)
{
	int r = 0;
	int a = 0;
	while (str[r] != 0)
	{
		if (str[r] == c)
		{
			if (a == pos) return r;
			a++;
		}
		r++;
	}
	return -1;
}

bool containsSymbol(const char *str, char c)
{
	int i;
	for (i = 0; str[i] != 0; i++) if (str[i] == c) return true;
	return false;
}

char *makestr(const char *str)
{
	char *r;
	size_t len;
	if (str == NULL) return NULL;
	len = Jupiter_strlen(str);
	r = (char *) malloc(sizeof(char) * (len + 1));
	if (r != NULL)
	{
		r[len] = 0;
		while (len != 0)
		{
			len--;
			r[len] = str[len];
		}
		return r;
	}
	return NULL;
}

char *getTime()
{
	time_t rawtime = time(0);
	static char rtime[256];
	strftime(rtime, sizeof(rtime), "%a %b %d %H:%M:%S %Y %Z", localtime(&rawtime));
	return rtime;
}



/** Character to integer conversion table */

const unsigned char baseTable[] =
{
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 127, 127, 127, 127, 127, 127,
	127, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 127, 127, 127, 127, 127,
	127, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 127, 127, 127, 127, 127,

	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
};

/** Single character functions */

bool Jupiter_isBase(unsigned char c, int base)
{
	return baseTable[c] < base;
}

bool Jupiter_isHex(unsigned char c)
{
	return Jupiter_isBase(c, 16);
}

bool Jupiter_isDecimal(unsigned char c)
{
	return Jupiter_isBase(c, 10);
}

int Jupiter_getBase(unsigned char c, int base)
{
	c = baseTable[c];
	if (c < base) return c;
	return -1;
}

/** Unsafe string converters */

int Jupiter_strtoi(const char *str, int base)
{
	while (isspace(*str)) str++;
	return Jupiter_strtoi_nospace(str, base);
}

long long Jupiter_strtoll(const char *str, int base)
{
	while (isspace(*str)) str++;
	return Jupiter_strtoll_nospace(str, base);
}

unsigned int Jupiter_strtoui(const char *str, int base)
{
	while (isspace(*str)) str++;
	return Jupiter_strtoui_nospace(str, base);
}

unsigned long long Jupiter_strtoull(const char *str, int base)
{
	while (isspace(*str)) str++;
	return Jupiter_strtoull_nospace(str, base);
}

double Jupiter_strtod(const char *str)
{
	while (isspace(*str)) str++;
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
	if (*str == '-') return Jupiter_strtoi_nospace(str, base);

	if (*str == '+') str++;

	if (base == 0) // Guess a base.
	{
		if (*str == '0')
		{
			str++;
			switch (*str)
			{
			case 'X':
			case 'x':
				str++;
				base = 16;
				break;
			case 'B':
			case 'b':
				str++;
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
			str++;
			if (*str == 'x' || *str == 'X') str++;
		}
	}

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		str++;
	}

	return total;
}

unsigned long long Jupiter_strtoull_nospace(const char *str, int base)
{
	unsigned long long total = 0;
	int tval;
	if (*str == '-') return Jupiter_strtoi_nospace(str, base);

	if (*str == '+') str++;

	if (base == 0) // Guess a base.
	{
		if (*str == '0')
		{
			str++;
			switch (*str)
			{
			case 'X':
			case 'x':
				str++;
				base = 16;
				break;
			case 'B':
			case 'b':
				str++;
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
			str++;
			if (*str == 'x' || *str == 'X') str++;
		}
	}

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		str++;
	}

	return total;
}

double Jupiter_strtod_nospace(const char *str)
{
	const int base = 10;
	double total = 0.0;
	double decimal = 0.0;
	int tval;

	if (*str == '-') return -1 * Jupiter_strtod_nospace(++str);
	if (*str == '+') str++;

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		str++;
	}

	if (*str == '.')
	{
		str++;
		tval = base;
		while ((decimal = Jupiter_getBase(*str, base)) != -1)
		{
			total = total + (decimal / tval);
			str++;
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
	{
		str++;
		length--;
	}
	return Jupiter_strtoi_nospace_s(str, length, base);
}

long long Jupiter_strtoll_s(const char *str, size_t length, int base)
{
	while (length != 0 && isspace(*str))
	{
		str++;
		length--;
	}
	return Jupiter_strtoll_nospace_s(str, length, base);
}

unsigned int Jupiter_strtoui_s(const char *str, size_t length, int base)
{
	while (length != 0 && isspace(*str))
	{
		str++;
		length--;
	}
	return Jupiter_strtoui_nospace_s(str, length, base);
}

unsigned long long Jupiter_strtoull_s(const char *str, size_t length, int base)
{
	while (length != 0 && isspace(*str))
	{
		str++;
		length--;
	}
	return Jupiter_strtoull_nospace_s(str, length, base);
}

double Jupiter_strtod_s(const char *str, size_t length)
{
	while (length != 0 && isspace(*str))
	{
		str++;
		length--;
	}
	return Jupiter_strtod_nospace_s(str, length);
}

int Jupiter_strtoi_nospace_s(const char *str, size_t length, int base)
{
	if (length == 0) return 0;
	if (*str != '-') return Jupiter_strtoui_nospace_s(str, length, base);
	return -1 * Jupiter_strtoui_nospace_s(++str, --length, base);
}

unsigned int Jupiter_strtoui_nospace_s(const char *str, size_t length, int base)
{
	unsigned int total = 0;
	int tval;
	if (length == 0) return total;
	if (*str == '-') return Jupiter_strtoi_nospace(str, base);

	if (*str == '+')
	{
		if (--length == 0) return total;
		str++;
	}

	if (base == 0) // Guess a base.
	{
		if (*str == '0')
		{
			if (--length == 0) return total;
			str++;
			switch (*str)
			{
			case 'X':
			case 'x':
				if (--length == 0) return total;
				str++;
				base = 16;
				break;
			case 'B':
			case 'b':
				if (--length == 0) return total;
				str++;
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
			if (--length == 0) return total;
			str++;
			if (*str == 'x' || *str == 'X')
			{
				if (--length == 0) return total;
				str++;
			}
		}
	}

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		if (--length == 0) return total;
		str++;
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
	if (length == 0) return total;
	if (*str == '-') return Jupiter_strtoi_nospace(str, base);

	if (*str == '+')
	{
		if (--length == 0) return total;
		str++;
	}

	if (base == 0) // Guess a base.
	{
		if (*str == '0')
		{
			if (--length == 0) return total;
			str++;
			switch (*str)
			{
			case 'X':
			case 'x':
				if (--length == 0) return total;
				str++;
				base = 16;
				break;
			case 'B':
			case 'b':
				if (--length == 0) return total;
				str++;
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
			if (--length == 0) return total;
			str++;
			if (*str == 'x' || *str == 'X')
			{
				if (--length == 0) return total;
				str++;
			}
		}
	}

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		if (--length == 0) return total;
		str++;
	}

	return total;
}

double Jupiter_strtod_nospace_s(const char *str, size_t length)
{
	const int base = 10;
	double total = 0.0;
	double decimal = 0.0;
	int tval;

	if (length == 0) return 0;
	if (*str == '-') return -1 * Jupiter_strtod_nospace_s(++str, --length);
	if (*str == '+')
	{
		str++;
		length--;
	}

	while ((tval = Jupiter_getBase(*str, base)) != -1)
	{
		total = base * total + tval;
		if (--length == 0) return total;
		str++;
	}

	if (*str == '.')
	{
		str++;
		length--;
		tval = base;
		while (length != 0 && (decimal = Jupiter_getBase(*str, base)) != -1)
		{
			total = total + (decimal / tval);
			if (--length == 0) return total;
			str++;
			tval = tval * base;
		}
		if (*str == 'e' || *str == 'E')
			total = total * pow(base, Jupiter_strtoi_nospace_s(++str, --length, 10));
	}

	return total;
}