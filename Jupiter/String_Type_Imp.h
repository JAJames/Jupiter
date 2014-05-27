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

#if !defined _STRING_TYPE_IMP_H_HEADER
#define _STRING_TYPE_IMP_H_HEADER

/**
 * @file String_Type_Imp.h
 * @brief Provides the implementations for String_Type functions.
 * Note: Modification of this file is not supported in any way.
 */

#include "String_Type.h"
#include "Functions.h"

/**
* IMPLEMENTATION:
*	String_Type
*/

template<typename T> T &Jupiter::String_Type<T>::get(size_t index) const
{
	return Jupiter::String_Type<T>::str[index];
}

template<typename T> size_t Jupiter::String_Type<T>::size() const
{
	return Jupiter::String_Type<T>::length;
}

template<typename T> const T *Jupiter::String_Type<T>::ptr() const
{
	return Jupiter::String_Type<T>::str;
}

template<typename T> bool Jupiter::String_Type<T>::contains(const T &value) const
{
	for (unsigned int i = 0; i < Jupiter::String_Type<T>::length; i++) if (Jupiter::String_Type<T>::str[i] == value) return true;
	return false;
}

// compare()

template<typename T> int Jupiter::String_Type<T>::compare(const Jupiter::String_Type<T> &in) const
{
	// rewrite to compare multiple bytes at a time.
	size_t index = 0;
	while (Jupiter::String_Type<T>::str[index] == in.get(index))
	{
		index++;
		if (index == in.size())
		{
			if (index == Jupiter::String_Type<T>::length) return 0;
			return 1;
		}
		if (index == Jupiter::String_Type<T>::length) return 0 - in.get(index);
	}
	return Jupiter::String_Type<T>::str[index] - in.get(index);
}

template<typename T> int Jupiter::String_Type<T>::compare(const std::basic_string<T> &in) const
{
	return Jupiter::String_Type<T>::compare(in.c_str());
}

template<typename T> int Jupiter::String_Type<T>::compare(const T *s2) const
{
	if (Jupiter::String_Type<T>::length == 0) return 0 - *s2;
	size_t index = 0;
	while (Jupiter::String_Type<T>::str[index] == *s2)
	{
		index++;
		if (*s2 == 0)
		{
			if (index == Jupiter::String_Type<T>::length) return 0;
			return 1;
		}
		s2++;
		if (index == Jupiter::String_Type<T>::length) return 0 - *s2;
	}
	return Jupiter::String_Type<T>::str[index] - *s2;
}

template<typename T> int Jupiter::String_Type<T>::compare(const T &in) const
{
	if (Jupiter::String_Type<T>::length == 0) return 0 - in;
	return *Jupiter::String_Type<T>::str - in;
}

template<typename T> int Jupiter::String_Type<T>::compare(const std::nullptr_t) const
{
	if (Jupiter::String_Type<T>::length == 0) return true;
	return *Jupiter::String_Type<T>::str;
}

// equals()

template<typename T> bool Jupiter::String_Type<T>::equals(const Jupiter::String_Type<T> &in) const
{
	if (Jupiter::String_Type<T>::length != in.size()) return false;

	// rewrite to compare multiple bytes at a time.
	size_t index = 0;
	while (index != Jupiter::String_Type<T>::length)
	{
		if (Jupiter::String_Type<T>::str[index] != in.str[index]) return false;
		index++;
	}
	return true;
}

template<typename T> bool Jupiter::String_Type<T>::equals(const std::basic_string<T> &in) const
{
	if (Jupiter::String_Type<T>::length != in.size()) return false;
	return Jupiter::String_Type<T>::equals(in.c_str());
}

template<typename T> bool Jupiter::String_Type<T>::equals(const T *in) const
{
	if (in == nullptr) return Jupiter::String_Type<T>::length == 0;
	for (size_t index = 0; index != Jupiter::String_Type<T>::length; index++)
	{
		if (*in == 0 || Jupiter::String_Type<T>::str[index] != *in) return false;
		in++;
	}
	return *in == 0;
}

template<typename T> bool Jupiter::String_Type<T>::equals(const T &in) const
{
	return Jupiter::String_Type<T>::length == 1 && *Jupiter::String_Type<T>::str == in;
}

template<typename T> bool Jupiter::String_Type<T>::equals(std::nullptr_t) const
{
	return Jupiter::String_Type<T>::length == 0;
}

// equalsi()

template<typename T> bool Jupiter::String_Type<T>::equalsi(const Jupiter::String_Type<T> &in) const
{
	return false; // Concept of "case" not supported for type.
}

template<> bool inline Jupiter::String_Type<char>::equalsi(const Jupiter::String_Type<char> &in) const
{
	if (Jupiter::String_Type<char>::length != in.size()) return false;
	for (size_t index = 0; index != Jupiter::String_Type<char>::length; index++)
	{
		if (toupper(Jupiter::String_Type<char>::str[index]) != toupper(in.str[index])) return false;
	}
	return true;
}

template<> bool inline Jupiter::String_Type<wchar_t>::equalsi(const Jupiter::String_Type<wchar_t> &in) const
{
	if (Jupiter::String_Type<wchar_t>::length != in.size()) return false;
	for (size_t index = 0; index != Jupiter::String_Type<wchar_t>::length; index++)
	{
		if (toupper(Jupiter::String_Type<wchar_t>::str[index]) != toupper(in.str[index])) return false;
	}
	return true;
}

template<typename T> bool Jupiter::String_Type<T>::equalsi(const std::basic_string<T> &in) const
{
	if (Jupiter::String_Type<T>::length != in.size()) return false;
	return Jupiter::String_Type<T>::equalsi(in.c_str());
}

template<> bool inline Jupiter::String_Type<char>::equalsi(const char *in) const
{
	if (in == nullptr) return Jupiter::String_Type<char>::length == 0;
	for (size_t index = 0; index != Jupiter::String_Type<char>::length; index++)
	{
		if (*in == 0 || toupper(Jupiter::String_Type<char>::str[index]) != toupper(*in)) return false;
		in++;
	}
	return *in == 0;
}

template<> bool inline Jupiter::String_Type<wchar_t>::equalsi(const wchar_t *in) const
{
	if (in == nullptr) return Jupiter::String_Type<wchar_t>::length == 0;
	for (size_t index = 0; index != Jupiter::String_Type<wchar_t>::length; index++)
	{
		if (*in == 0 || towupper(Jupiter::String_Type<wchar_t>::str[index]) != towupper(*in)) return false;
		in++;
	}
	return *in == 0;
}

template<typename T> bool Jupiter::String_Type<T>::equalsi(const T *in) const
{
	return false; // Concept of "case" not supported for type.
}

template<> bool inline Jupiter::String_Type<char>::equalsi(const char &in) const
{
	return Jupiter::String_Type<char>::length == 1 && toupper(*Jupiter::String_Type<char>::str) == toupper(in);
}

template<> bool inline Jupiter::String_Type<wchar_t>::equalsi(const wchar_t &in) const
{
	return Jupiter::String_Type<wchar_t>::length == 1 && towupper(*Jupiter::String_Type<wchar_t>::str) == towupper(in);
}

template<typename T> bool Jupiter::String_Type<T>::equalsi(const T &in) const
{
	return false; // Concept of "case" not supported for type.
}

template<typename T> bool Jupiter::String_Type<T>::equalsi(const std::nullptr_t) const
{
	return Jupiter::String_Type<T>::length == 0;
}

// match()

template<typename T> bool Jupiter::String_Type<T>::match(const Jupiter::String_Type<T> &format) const
{
	return false; // Wildcard matching not supported for type.
}

template<> bool inline Jupiter::String_Type<char>::match(const Jupiter::String_Type<char> &format) const
{
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != format.size())
	{
		if (format.str[formatIndex] == '*')
		{
			formatIndex++;
			while (format.str[formatIndex] == '?')
			{
				if (Jupiter::String_Type<char>::str[index] == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.str[formatIndex] == 0) return true;
			if (format.str[formatIndex] == '*') continue;
			while (format.str[formatIndex] != Jupiter::String_Type<char>::str[index])
			{
				if (Jupiter::String_Type<char>::str[index] == 0) return false;
				index++;
			}
		}
		else if (format.str[formatIndex] != '?' && format.str[formatIndex] != Jupiter::String_Type<char>::str[index]) return false;
		formatIndex++;
		index++;
	}
	return index == Jupiter::String_Type<char>::length;
}

template<> bool inline Jupiter::String_Type<wchar_t>::match(const Jupiter::String_Type<wchar_t> &format) const
{
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != format.size())
	{
		if (format.str[formatIndex] == '*')
		{
			formatIndex++;
			while (format.str[formatIndex] == '?')
			{
				if (Jupiter::String_Type<wchar_t>::str[index] == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.str[formatIndex] == 0) return true;
			if (format.str[formatIndex] == '*') continue;
			while (format.str[formatIndex] != Jupiter::String_Type<wchar_t>::str[index])
			{
				if (Jupiter::String_Type<wchar_t>::str[index] == 0) return false;
				index++;
			}
		}
		else if (format.str[formatIndex] != '?' && format.str[formatIndex] != Jupiter::String_Type<wchar_t>::str[index]) return false;
		formatIndex++;
		index++;
	}
	return index == Jupiter::String_Type<wchar_t>::length;
}

template<typename T> bool Jupiter::String_Type<T>::match(const std::basic_string<T> &format) const
{
	return Jupiter::String_Type<T>::match(format.c_str());
}

template<> inline bool Jupiter::String_Type<char>::match(const char *format) const
{
	size_t index = 0;
	while (*format != 0)
	{
		if (*format == '*')
		{
			format++;
			while (*format == '?')
			{
				if (Jupiter::String_Type<char>::str[index] == 0) return false;
				format++;
				index++;
			}
			if (*format == 0) return true;
			if (*format == '*') continue;
			while (*format != Jupiter::String_Type<char>::str[index])
			{
				if (Jupiter::String_Type<char>::str[index] == 0) return false;
				index++;
			}
		}
		else if (*format != '?' && *format != Jupiter::String_Type<char>::str[index]) return false;
		format++;
		index++;
	}
	return index == Jupiter::String_Type<char>::length;
}

template<> inline bool Jupiter::String_Type<wchar_t>::match(const wchar_t *format) const
{
	size_t index = 0;
	while (*format != 0)
	{
		if (*format == L'*')
		{
			format++;
			while (*format == L'?')
			{
				if (Jupiter::String_Type<wchar_t>::str[index] == 0) return false;
				format++;
				index++;
			}
			if (*format == 0) return true;
			if (*format == L'*') continue;
			while (*format != Jupiter::String_Type<wchar_t>::str[index])
			{
				if (Jupiter::String_Type<wchar_t>::str[index] == 0) return false;
				index++;
			}
		}
		else if (*format != L'?' && *format != Jupiter::String_Type<wchar_t>::str[index]) return false;
		format++;
		index++;
	}
	return index == Jupiter::String_Type<wchar_t>::length;
}

template<typename T> bool Jupiter::String_Type<T>::match(const T *format) const
{
	return false; // Type is not comparable to wildcards.
}

// matchi()

template<> bool inline Jupiter::String_Type<char>::matchi(const Jupiter::String_Type<char> &format) const
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
				if (Jupiter::String_Type<char>::str[index] == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.get(formatIndex) == 0) return true;
			if (format.get(formatIndex) == '*') continue;
			fUpper = toupper(format.get(formatIndex));
			while (fUpper != toupper(Jupiter::String_Type<char>::str[index]))
			{
				if (Jupiter::String_Type<char>::str[index] == 0) return false;
				index++;
			}
		}
		else if (format.get(formatIndex) != L'?' && toupper(format.get(formatIndex)) != toupper(Jupiter::String_Type<char>::str[index])) return false;
		formatIndex++;
		index++;
	}
	return index == Jupiter::String_Type<char>::length;
}

template<> bool inline Jupiter::String_Type<wchar_t>::matchi(const Jupiter::String_Type<wchar_t> &format) const
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
				if (Jupiter::String_Type<wchar_t>::str[index] == 0) return false;
				formatIndex++;
				index++;
			}
			if (format.get(formatIndex) == 0) return true;
			if (format.get(formatIndex) == '*') continue;
			fUpper = towupper(format.get(formatIndex));
			while (fUpper != towupper(Jupiter::String_Type<wchar_t>::str[index]))
			{
				if (Jupiter::String_Type<wchar_t>::str[index] == 0) return false;
				index++;
			}
		}
		else if (format.get(formatIndex) != L'?' && towupper(format.get(formatIndex)) != towupper(Jupiter::String_Type<wchar_t>::str[index])) return false;
		formatIndex++;
		index++;
	}
	return index == Jupiter::String_Type<wchar_t>::length;
}

template<typename T> bool Jupiter::String_Type<T>::matchi(const Jupiter::String_Type<T> &format) const
{
	return false;
}

template<typename T> bool Jupiter::String_Type<T>::matchi(const std::basic_string<T> &format) const
{
	return Jupiter::String_Type<T>::matchi(format.c_str());
}

template<> bool inline Jupiter::String_Type<char>::matchi(const char *format) const
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
				if (Jupiter::String_Type<char>::str[index] == 0) return false;
				format++;
				index++;
			}
			if (*format == 0) return true;
			if (*format == '*') continue;
			fUpper = toupper(*format);
			while (fUpper != toupper(Jupiter::String_Type<char>::str[index]))
			{
				if (Jupiter::String_Type<char>::str[index] == 0) return false;
				index++;
			}
		}
		else if (*format != '?' && toupper(*format) != toupper(Jupiter::String_Type<char>::str[index])) return false;
		format++;
		index++;
	}
	return index == Jupiter::String_Type<char>::length;
}

template<> bool inline Jupiter::String_Type<wchar_t>::matchi(const wchar_t *format) const
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
				if (Jupiter::String_Type<wchar_t>::str[index] == 0) return false;
				format++;
				index++;
			}
			if (*format == 0) return true;
			if (*format == '*') continue;
			fUpper = towupper(*format);
			while (fUpper != towupper(Jupiter::String_Type<wchar_t>::str[index]))
			{
				if (Jupiter::String_Type<wchar_t>::str[index] == 0) return false;
				index++;
			}
		}
		else if (*format != L'?' && towupper(*format) != towupper(Jupiter::String_Type<wchar_t>::str[index])) return false;
		format++;
		index++;
	}
	return index == Jupiter::String_Type<wchar_t>::length;
}

template<typename T> bool Jupiter::String_Type<T>::matchi(const T *format) const
{
	return false; // Type is not comparable to wildcards. Concept of "case" not supported for type.
}

// wordCount()

template<typename T> unsigned int Jupiter::String_Type<T>::wordCount(const T *whitespace) const
{
	unsigned int result = 0;
	T *p = Jupiter::String_Type<T>::str;
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

template<> unsigned int inline Jupiter::String_Type<char>::asUnsignedInt(int base) const
{
	return strtoui_s(Jupiter::String_Type<char>::str, Jupiter::String_Type<char>::length, base);
}

template<typename T> unsigned int Jupiter::String_Type<T>::asUnsignedInt(int base) const
{
	return 0;
}

template<> int inline Jupiter::String_Type<char>::asInt(int base) const
{
	return strtoi_s(Jupiter::String_Type<char>::str, Jupiter::String_Type<char>::length, base);
}

template<typename T> int Jupiter::String_Type<T>::asInt(int base) const
{
	return 0;
}

// Stream output

template<typename T> size_t Jupiter::String_Type<T>::print(FILE *out) const
{
	return fwrite(Jupiter::String_Type<T>::str, sizeof(T), Jupiter::String_Type<T>::length, out);
}

template<> size_t inline Jupiter::String_Type<wchar_t>::print(FILE *out) const
{
	size_t index = 0;
	while (index != Jupiter::String_Type<wchar_t>::length && fputwc(Jupiter::String_Type<wchar_t>::str[index], out) != WEOF) index++;
	return index;
}

template<typename T> size_t Jupiter::String_Type<T>::print(std::basic_ostream<T> &out) const
{
	size_t index = 0;
	while (index != Jupiter::String_Type<T>::length)
	{
		out << Jupiter::String_Type<T>::str[index];
		index++;
	}
	return index;
}

template<typename T> size_t Jupiter::String_Type<T>::println(FILE *out) const
{
	size_t r = Jupiter::String_Type<T>::print(out);
	if (r != Jupiter::String_Type<T>::length) return r;
	// This may change to simply '\n' at a later date.
	if (fputs("\r\n", out) != EOF) r += 2;
	return r;
}

template<typename T> size_t Jupiter::String_Type<T>::println(std::basic_ostream<T> &out) const
{
	size_t r = Jupiter::String_Type<T>::print(out);
	if (r != Jupiter::String_Type<T>::length) return r;
	out << std::endl;
	return r;
}

// format forwards

template<typename T> size_t Jupiter::String_Type<T>::format(const String_Type<T> &format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->vformat(format.c_str(), args);
	va_end(args);
	return r;
}

template<typename T> size_t Jupiter::String_Type<T>::format(const std::basic_string<T> &format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->vformat(format.c_str(), args);
	va_end(args);
	return r;
}

template<typename T> size_t Jupiter::String_Type<T>::format(const T *format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->vformat(format, args);
	va_end(args);
	return r;
}

// aformat forwards

template<typename T> size_t Jupiter::String_Type<T>::aformat(const String_Type<T> &format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->avformat(format.c_str(), args);
	va_end(args);
	return r;
}

template<typename T> size_t Jupiter::String_Type<T>::aformat(const std::basic_string<T> &format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->avformat(format.c_str(), args);
	va_end(args);
	return r;
}

template<typename T> size_t Jupiter::String_Type<T>::aformat(const T *format, ...)
{
	size_t r;
	va_list args;
	va_start(args, format);
	r = this->avformat(format, args);
	va_end(args);
	return r;
}

#endif // _STRING_TYPE_IMP_H_HEADER