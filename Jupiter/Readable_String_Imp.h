/**
 * Copyright (C) 2014 Justin James.
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

#if !defined _READABLE_STRING_IMP_H_HEADER
#define _READABLE_STRING_IMP_H_HEADER

/**
 * @file Readable_String_Imp.h
 * @brief Provides the implementations for Readable_String functions.
 * Note: Modification of this file is not supported in any way.
 */

#include "Readable_String.h"
#include "Functions.h"

/**
* IMPLEMENTATION:
*	Readable_String
*/

// contains

template<typename T> bool Jupiter::Readable_String<T>::contains(const T &value) const
{
	for (size_t i = 0; i != this->size(); i++) if (this->get(i) == value) return true;
	return false;
}

// compare()

template<typename T> int Jupiter::Readable_String<T>::compare(const Jupiter::Readable_String<T> &in) const
{
	// rewrite to compare multiple bytes at a time.
	size_t index = 0;
	while (this->get(index) == in.get(index))
	{
		index++;
		if (index == in.size())
		{
			if (index == this->size()) return 0;
			return 1;
		}
		if (index == this->size()) return 0 - in.get(index);
	}
	return this->get(index) - in.get(index);
}

template<typename T> int Jupiter::Readable_String<T>::compare(const std::basic_string<T> &in) const
{
	// rewrite to compare multiple bytes at a time.
	size_t index = 0;
	while (this->get(index) == in.at(index))
	{
		index++;
		if (index == in.size())
		{
			if (index == this->size()) return 0;
			return 1;
		}
		if (index == this->size()) return 0 - in.at(index);
	}
	return this->get(index) - in.at(index);
}

template<typename T> int Jupiter::Readable_String<T>::compare(const T *s2) const
{
	if (this->size() == 0) return 0 - *s2;
	size_t index = 0;
	while (this->get(index) == *s2)
	{
		index++;
		if (*s2 == 0)
		{
			if (index == this->size()) return 0;
			return 1;
		}
		s2++;
		if (index == this->size()) return 0 - *s2;
	}
	return this->get(index) - *s2;
}

template<typename T> int Jupiter::Readable_String<T>::compare(const T &in) const
{
	if (this->size() == 0) return 0 - in;
	return this->get(0) - in;
}

template<typename T> int Jupiter::Readable_String<T>::compare(const std::nullptr_t) const
{
	if (this->size() == 0) return true;
	return this->get(0);
}

// equals()

template<typename T> bool Jupiter::Readable_String<T>::equals(const Jupiter::Readable_String<T> &in) const
{
	if (this->size() != in.size()) return false;

	// rewrite to compare multiple bytes at a time.
	size_t index = 0;
	while (index != this->size())
	{
		if (this->get(index) != in.get(index)) return false;
		index++;
	}
	return true;
}

template<typename T> bool Jupiter::Readable_String<T>::equals(const std::basic_string<T> &in) const
{
	if (this->size() != in.size()) return false;
	// rewrite to compare multiple bytes at a time.
	size_t index = 0;
	while (index != this->size())
	{
		if (this->get(index) != in.at(index)) return false;
		index++;
	}
	return true;
}

template<typename T> bool Jupiter::Readable_String<T>::equals(const T *in) const
{
	if (in == nullptr) return this->size() == 0;
	for (size_t index = 0; index != this->size(); index++)
	{
		if (*in == 0 || this->get(index) != *in) return false;
		in++;
	}
	return *in == 0;
}

template<typename T> bool Jupiter::Readable_String<T>::equals(const T &in) const
{
	return this->size() == 1 && this->get(0) == in;
}

template<typename T> bool Jupiter::Readable_String<T>::equals(std::nullptr_t) const
{
	return this->size() == 0;
}

// equalsi()

template<> bool inline Jupiter::Readable_String<char>::equalsi(const Jupiter::Readable_String<char> &in) const
{
	if (this->size() != in.size()) return false;
	for (size_t index = 0; index != this->size(); index++)
	{
		if (toupper(this->get(index)) != toupper(in.get(index))) return false;
	}
	return true;
}

template<> bool inline Jupiter::Readable_String<wchar_t>::equalsi(const Jupiter::Readable_String<wchar_t> &in) const
{
	if (this->size() != in.size()) return false;
	for (size_t index = 0; index != this->size(); index++)
	{
		if (towupper(this->get(index)) != towupper(in.get(index))) return false;
	}
	return true;
}

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const Jupiter::Readable_String<T> &in) const
{
	return this->equals(in); // Concept of "case" not supported for type.
}

template<> bool inline Jupiter::Readable_String<char>::equalsi(const std::basic_string<char> &in) const
{
	if (this->size() != in.size()) return false;
	for (size_t index = 0; index != this->size(); index++)
	{
		if (toupper(this->get(index)) != toupper(in.at(index))) return false;
	}
	return true;
}

template<> bool inline Jupiter::Readable_String<wchar_t>::equalsi(const std::basic_string<wchar_t> &in) const
{
	if (this->size() != in.size()) return false;
	for (size_t index = 0; index != this->size(); index++)
	{
		if (towupper(this->get(index)) != towupper(in.at(index))) return false;
	}
	return true;
}

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const std::basic_string<T> &in) const
{
	return this->equals(in); // Concept of "case" not supported for type.
}

template<> bool inline Jupiter::Readable_String<char>::equalsi(const char *in) const
{
	if (in == nullptr) return this->size() == 0;
	for (size_t index = 0; index != this->size(); index++)
	{
		if (*in == 0 || toupper(this->get(index)) != toupper(*in)) return false;
		in++;
	}
	return *in == 0;
}

template<> bool inline Jupiter::Readable_String<wchar_t>::equalsi(const wchar_t *in) const
{
	if (in == nullptr) return this->size() == 0;
	for (size_t index = 0; index != this->size(); index++)
	{
		if (*in == 0 || towupper(this->get(index)) != towupper(*in)) return false;
		in++;
	}
	return *in == 0;
}

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const T *in) const
{
	return this->equals(in); // Concept of "case" not supported for type.
}

template<> bool inline Jupiter::Readable_String<char>::equalsi(const char &in) const
{
	return this->size() == 1 && toupper(this->get(0)) == toupper(in);
}

template<> bool inline Jupiter::Readable_String<wchar_t>::equalsi(const wchar_t &in) const
{
	return this->size() == 1 && towupper(this->get(0)) == towupper(in);
}

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const T &in) const
{
	return this->equals(in); // Concept of "case" not supported for type.
}

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const std::nullptr_t) const
{
	return this->size() == 0;
}

// match()

template<> bool inline Jupiter::Readable_String<char>::match(const Jupiter::Readable_String<char> &format) const
{
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != format.size())
	{
		if (format.get(formatIndex) == '*')
		{
			formatIndex++;
			while (format.get(formatIndex) == '?')
			{
				if (this->get(index) == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.get(formatIndex) == 0) return true;
			if (format.get(formatIndex) == '*') continue;
			while (format.get(formatIndex) != this->get(index))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (format.get(formatIndex) != '?' && format.get(formatIndex) != this->get(index)) return false;
		formatIndex++;
		index++;
	}
	return index == this->size();
}

template<> bool inline Jupiter::Readable_String<wchar_t>::match(const Jupiter::Readable_String<wchar_t> &format) const
{
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != format.size())
	{
		if (format.get(formatIndex) == '*')
		{
			formatIndex++;
			while (format.get(formatIndex) == '?')
			{
				if (this->get(index) == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.get(formatIndex) == 0) return true;
			if (format.get(formatIndex) == '*') continue;
			while (format.get(formatIndex) != this->get(index))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (format.get(formatIndex) != '?' && format.get(formatIndex) != this->get(index)) return false;
		formatIndex++;
		index++;
	}
	return index == this->size();
}

template<typename T> bool Jupiter::Readable_String<T>::match(const Jupiter::Readable_String<T> &format) const
{
	return false; // Wildcard matching not supported for type.
}

template<> bool inline Jupiter::Readable_String<char>::match(const std::basic_string<char> &format) const
{
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != format.size())
	{
		if (format.at(formatIndex) == '*')
		{
			formatIndex++;
			while (format.at(formatIndex) == '?')
			{
				if (this->get(index) == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.at(formatIndex) == 0) return true;
			if (format.at(formatIndex) == '*') continue;
			while (format.at(formatIndex) != this->get(index))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (format.at(formatIndex) != '?' && format.at(formatIndex) != this->get(index)) return false;
		formatIndex++;
		index++;
	}
	return index == this->size();
}

template<> bool inline Jupiter::Readable_String<wchar_t>::match(const std::basic_string<wchar_t> &format) const
{
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != format.size())
	{
		if (format.at(formatIndex) == '*')
		{
			formatIndex++;
			while (format.at(formatIndex) == '?')
			{
				if (this->get(index) == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.at(formatIndex) == 0) return true;
			if (format.at(formatIndex) == '*') continue;
			while (format.at(formatIndex) != this->get(index))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (format.at(formatIndex) != '?' && format.at(formatIndex) != this->get(index)) return false;
		formatIndex++;
		index++;
	}
	return index == this->size();
}

template<typename T> bool Jupiter::Readable_String<T>::match(const std::basic_string<T> &format) const
{
	return false; // Wildcard matching not supported for type.
}

template<> inline bool Jupiter::Readable_String<char>::match(const char *format) const
{
	size_t index = 0;
	while (*format != 0)
	{
		if (*format == '*')
		{
			format++;
			while (*format == '?')
			{
				if (this->get(index) == 0) return false;
				format++;
				index++;
			}
			if (*format == 0) return true;
			if (*format == '*') continue;
			while (*format != this->get(index))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (*format != '?' && *format != this->get(index)) return false;
		format++;
		index++;
	}
	return index == this->size();
}

template<> inline bool Jupiter::Readable_String<wchar_t>::match(const wchar_t *format) const
{
	size_t index = 0;
	while (*format != 0)
	{
		if (*format == L'*')
		{
			format++;
			while (*format == L'?')
			{
				if (this->get(index) == 0) return false;
				format++;
				index++;
			}
			if (*format == 0) return true;
			if (*format == L'*') continue;
			while (*format != this->get(index))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (*format != L'?' && *format != this->get(index)) return false;
		format++;
		index++;
	}
	return index == this->size();
}

template<typename T> bool Jupiter::Readable_String<T>::match(const T *format) const
{
	return false; // Wildcard matching not supported for type.
}

// matchi()

template<> bool inline Jupiter::Readable_String<char>::matchi(const Jupiter::Readable_String<char> &format) const
{
	int fUpper;
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != format.size())
	{
		if (format.get(formatIndex) == L'*')
		{
			formatIndex++;
			while (format.get(formatIndex) == L'?')
			{
				if (this->get(index) == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.get(formatIndex) == 0) return true;
			if (format.get(formatIndex) == '*') continue;
			fUpper = toupper(format.get(formatIndex));
			while (fUpper != toupper(this->get(index)))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (format.get(formatIndex) != L'?' && toupper(format.get(formatIndex)) != toupper(this->get(index))) return false;
		formatIndex++;
		index++;
	}
	return index == this->size();
}

template<> bool inline Jupiter::Readable_String<wchar_t>::matchi(const Jupiter::Readable_String<wchar_t> &format) const
{
	wint_t fUpper;
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != format.size())
	{
		if (format.get(formatIndex) == L'*')
		{
			formatIndex++;
			while (format.get(formatIndex) == L'?')
			{
				if (this->get(index) == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.get(formatIndex) == 0) return true;
			if (format.get(formatIndex) == '*') continue;
			fUpper = towupper(format.get(formatIndex));
			while (fUpper != towupper(this->get(index)))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (format.get(formatIndex) != L'?' && towupper(format.get(formatIndex)) != towupper(this->get(index))) return false;
		formatIndex++;
		index++;
	}
	return index == this->size();
}

template<typename T> bool Jupiter::Readable_String<T>::matchi(const Jupiter::Readable_String<T> &format) const
{
	return false; // Wildcard matching not supported for type. Concept of "case" not supported for type.
}

template<> bool inline Jupiter::Readable_String<char>::matchi(const std::basic_string<char> &format) const
{
	int fUpper;
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != format.size())
	{
		if (format.at(formatIndex) == L'*')
		{
			formatIndex++;
			while (format.at(formatIndex) == L'?')
			{
				if (this->get(index) == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.at(formatIndex) == 0) return true;
			if (format.at(formatIndex) == '*') continue;
			fUpper = toupper(format.at(formatIndex));
			while (fUpper != toupper(this->get(index)))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (format.at(formatIndex) != L'?' && toupper(format.at(formatIndex)) != toupper(this->get(index))) return false;
		formatIndex++;
		index++;
	}
	return index == this->size();
}

template<> bool inline Jupiter::Readable_String<wchar_t>::matchi(const std::basic_string<wchar_t> &format) const
{
	wint_t fUpper;
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != format.size())
	{
		if (format.at(formatIndex) == L'*')
		{
			formatIndex++;
			while (format.at(formatIndex) == L'?')
			{
				if (this->get(index) == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.at(formatIndex) == 0) return true;
			if (format.at(formatIndex) == '*') continue;
			fUpper = towupper(format.at(formatIndex));
			while (fUpper != towupper(this->get(index)))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (format.at(formatIndex) != L'?' && towupper(format.at(formatIndex)) != towupper(this->get(index))) return false;
		formatIndex++;
		index++;
	}
	return index == this->size();
}

template<typename T> bool Jupiter::Readable_String<T>::matchi(const std::basic_string<T> &format) const
{
	return false; // Wildcard matching not supported for type. Concept of "case" not supported for type.
}

template<> bool inline Jupiter::Readable_String<char>::matchi(const char *format) const
{
	int fUpper;
	size_t index = 0;
	while (*format != 0)
	{
		if (*format == '*')
		{
			format++;
			while (*format == '?')
			{
				if (this->get(index) == 0) return false;
				format++;
				index++;
			}
			if (*format == 0) return true;
			if (*format == '*') continue;
			fUpper = toupper(*format);
			while (fUpper != toupper(this->get(index)))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (*format != '?' && toupper(*format) != toupper(this->get(index))) return false;
		format++;
		index++;
	}
	return index == this->size();
}

template<> bool inline Jupiter::Readable_String<wchar_t>::matchi(const wchar_t *format) const
{
	wint_t fUpper;
	size_t index = 0;
	while (*format != 0)
	{
		if (*format == L'*')
		{
			format++;
			while (*format == L'?')
			{
				if (this->get(index) == 0) return false;
				format++;
				index++;
			}
			if (*format == 0) return true;
			if (*format == '*') continue;
			fUpper = towupper(*format);
			while (fUpper != towupper(this->get(index)))
			{
				if (this->get(index) == 0) return false;
				index++;
			}
		}
		else if (*format != L'?' && towupper(*format) != towupper(this->get(index))) return false;
		format++;
		index++;
	}
	return index == this->size();
}

template<typename T> bool Jupiter::Readable_String<T>::matchi(const T *format) const
{
	return false; // Wildcard matching not supported for type. Concept of "case" not supported for type.
}

// wordCount()

template<typename T> unsigned int Jupiter::Readable_String<T>::wordCount(const T *whitespace) const
{
	unsigned int result = 0;
	const T *p = this->ptr();
	bool prev = true;
	while (*p != 0)
	{
		if (Jupiter::strpbrk<T>(whitespace, *p) == nullptr) // This isn't whitespace!
		{
			if (prev == true) // We just left whitespace!
			{
				prev = false;
				result++;
			}
		}
		else prev = true; // This is whitespace!
		p++;
	}
	return result;
}

// as<type>

template<> unsigned int inline Jupiter::Readable_String<char>::asUnsignedInt(int base) const
{
	return strtoui_s(this->ptr(), this->size(), base);
}

template<typename T> unsigned int Jupiter::Readable_String<T>::asUnsignedInt(int base) const
{
	return 0;
}

template<> int inline Jupiter::Readable_String<char>::asInt(int base) const
{
	return strtoi_s(this->ptr(), this->size(), base);
}

template<typename T> int Jupiter::Readable_String<T>::asInt(int base) const
{
	return 0;
}

// Stream output

template<typename T> size_t Jupiter::Readable_String<T>::print(FILE *out) const
{
	return fwrite(this->ptr(), sizeof(T), this->size(), out);
}

template<> size_t inline Jupiter::Readable_String<wchar_t>::print(FILE *out) const
{
	size_t index = 0;
	while (index != this->size() && fputwc(this->get(index), out) != WEOF) index++;
	return index;
}

template<typename T> size_t Jupiter::Readable_String<T>::print(std::basic_ostream<T> &out) const
{
	size_t index = 0;
	while (index != this->size())
	{
		out << this->get(index);
		index++;
	}
	return index;
}

template<typename T> size_t Jupiter::Readable_String<T>::println(FILE *out) const
{
	size_t r = this->print(out);
	if (r != this->size()) return r;
	if (fputs("\r\n", out) != EOF) r += 2;
	return r;
}

template<typename T> size_t Jupiter::Readable_String<T>::println(std::basic_ostream<T> &out) const
{
	size_t r = this->print(out);
	if (r != this->size()) return r;
	out << std::endl;
	return r;
}

// getWord

template<typename T> template<template<typename> class R> R<T> Jupiter::Readable_String<T>::getWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace)
{
	unsigned int x = 0;
	unsigned int y = 1;
	for (unsigned int i = 0; i < pos || y == 1; x++)
	{
		if (x == in.size()) return R<T>();
		if (Jupiter::strpbrk<T>(whitespace, in.get(x)) != nullptr)
		{
			if (y != 1)
			{
				y = 1;
				i++;
			}
		}
		else
		{
			if (i >= pos) break;
			y = 0;
		}
	}
	for (y = x; y != in.size() && Jupiter::strpbrk<T>(whitespace, in.get(y)) == nullptr; y++);
	return R<T>::substring(in, x, y - x);
}

template<typename T> template<template<typename> class R> R<T> Jupiter::Readable_String<T>::getWord(const T *in, size_t pos, const T *whitespace)
{
	unsigned int x = 0;
	unsigned int y = 1;
	for (unsigned int i = 0; i < pos || y == 1; x++)
	{
		if (in[x] == 0) return R<T>();
		if (Jupiter::strpbrk<T>(whitespace, in[x]) != nullptr)
		{
			if (y != 1)
			{
				y = 1;
				i++;
			}
		}
		else
		{
			if (i >= pos) break;
			y = 0;
		}
	}
	for (y = x; in[y] != 0 && Jupiter::strpbrk<T>(whitespace, in[y]) == nullptr; y++);
	return R<T>::substring(in, x, y - x);
}

// gotoWord

template<typename T> template<template<typename> class R> R<T> Jupiter::Readable_String<T>::gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace)
{
	unsigned int x = 0;
	bool y = true;
	for (unsigned int i = 0; i < pos || y == true; x++)
	{
		if (x == in.size()) return R<T>();
		if (Jupiter::strpbrk<T>(whitespace, in.get(x)) != nullptr)
		{
			if (y != true)
			{
				y = true;
				i++;
			}
		}
		else
		{
			if (i >= pos) break;
			y = false;
		}
	}
	return R<T>::substring(in, x);
}

template<typename T> template<template<typename> class R> R<T> Jupiter::Readable_String<T>::gotoWord(const T *in, size_t pos, const T *whitespace)
{
	unsigned int x = 0;
	bool y = true;
	for (unsigned int i = 0; i < pos || y == true; x++)
	{
		if (in[x] == 0) return R<T>();
		if (Jupiter::strpbrk<T>(whitespace, in[x]) != nullptr)
		{
			if (y != true)
			{
				y = true;
				i++;
			}
		}
		else
		{
			if (i >= pos) break;
			y = false;
		}
	}
	return R<T>::substring(in, x);
}

#endif // _READABLE_STRING_IMP_H_HEADER