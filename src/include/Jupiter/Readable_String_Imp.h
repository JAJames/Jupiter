/**
 * Copyright (C) 2014-2017 Jessica James.
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

// isEmpty

template<typename T> bool Jupiter::Readable_String<T>::isEmpty() const
{
	return this->size() == 0;
}

template<typename T> bool Jupiter::Readable_String<T>::isNotEmpty() const
{
	return this->size() != 0;
}

// contains

template<typename T> bool Jupiter::Readable_String<T>::contains(const T &value) const
{
	for (size_t i = 0; i != this->size(); i++) if (this->get(i) == value) return true;
	return false;
}

// find

template<typename T> size_t Jupiter::Readable_String<T>::find(const T &value, size_t index) const
{
	for (size_t i = 0; i != this->size(); i++)
	{
		if (this->get(i) == value)
		{
			if (index == 0) return i;
			else index--;
		}
	}
	return Jupiter::INVALID_INDEX;
}

template<typename T> size_t Jupiter::Readable_String<T>::find(const Jupiter::Readable_String<T> &in) const
{
	if (in.size() > this->size())
		return Jupiter::INVALID_INDEX;
	if (in.size() == this->size())
		return this->equals(in) ? 0 : Jupiter::INVALID_INDEX;
	if (in.isEmpty())
		return 0;

	for (size_t i = 0, j; i != this->size() - in.size() + 1; i++)
	{
		j = 0;
		while (this->get(i + j) == in.get(j))
			if (++j == in.size()) return i;
	}
	return Jupiter::INVALID_INDEX;
}

// findi

template<> size_t inline Jupiter::Readable_String<char>::findi(const char &value, size_t index) const
{
	const char upperValue = (const char) toupper(value);
	for (size_t i = 0; i != this->size(); i++)
	{
		if (toupper(this->get(i)) == upperValue)
		{
			if (index == 0) return i;
			else index--;
		}
	}
	return Jupiter::INVALID_INDEX;
}

template<> size_t inline Jupiter::Readable_String<wchar_t>::findi(const wchar_t &value, size_t index) const
{
	const wchar_t upperValue = towupper(value);
	for (size_t i = 0; i != this->size(); i++)
	{
		if (towupper(this->get(i)) == upperValue)
		{
			if (index == 0) return i;
			else index--;
		}
	}
	return Jupiter::INVALID_INDEX;
}

template<typename T> size_t Jupiter::Readable_String<T>::findi(const T &value, size_t index) const
{
	return Jupiter::INVALID_INDEX;
}

template<> size_t inline Jupiter::Readable_String<char>::findi(const Jupiter::Readable_String<char> &in) const
{
	if (in.size() > this->size())
		return Jupiter::INVALID_INDEX;
	if (in.size() == this->size())
		return this->equalsi(in) ? 0 : Jupiter::INVALID_INDEX;
	if (in.isEmpty())
		return 0;

	for (size_t i = 0, j; i != this->size() - in.size() + 1; i++)
	{
		j = 0;
		while (toupper(this->get(i + j)) == toupper(in.get(j)))
			if (++j == in.size()) return i;
	}
	return Jupiter::INVALID_INDEX;
}

template<> size_t inline Jupiter::Readable_String<wchar_t>::findi(const Jupiter::Readable_String<wchar_t> &in) const
{
	if (in.size() > this->size())
		return Jupiter::INVALID_INDEX;
	if (in.size() == this->size())
		return this->equalsi(in) ? 0 : Jupiter::INVALID_INDEX;
	if (in.isEmpty())
		return 0;

	for (size_t i = 0, j; i != this->size() - in.size() + 1; i++)
	{
		j = 0;
		while (towupper(this->get(i + j)) == towupper(in.get(j)))
			if (++j == in.size()) return i;
	}
	return Jupiter::INVALID_INDEX;
}

template<typename T> size_t Jupiter::Readable_String<T>::findi(const Jupiter::Readable_String<T> &) const
{
	return Jupiter::INVALID_INDEX;
}

// span()

template<typename T> size_t Jupiter::Readable_String<T>::span(const Jupiter::Readable_String<T> &in) const
{
	size_t index = 0;
	while (index != this->size() && in.contains(this->get(index)))
		++index;
	return index;
}

template<typename T> size_t Jupiter::Readable_String<T>::span(const T *in) const
{
	size_t index = 0;
	while (index != this->size() && containsSymbol(in, this->get(index)))
		++index;
	return index;
}

template<typename T> size_t Jupiter::Readable_String<T>::span(const T &in) const
{
	size_t index = 0;
	while (index != this->size() && this->get(index) == in)
		++index;
	return index;
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
	return this->equals(in.ptr(), in.size());
}

template<typename T> bool Jupiter::Readable_String<T>::equals(const std::basic_string<T> &in) const
{
	return this->equals(in.data(), in.size());
}

template<typename T> bool Jupiter::Readable_String<T>::equals(const T *in, size_t len) const
{
	if (this->size() != len)
		return false;
	// rewrite to compare multiple bytes at a time.
	in += len;
	while (len != 0)
		if (this->get(--len) != *(--in))
			return false;

	return true;
}

template<typename T> bool Jupiter::Readable_String<T>::equals(const T *in) const
{
	if (in == nullptr) return this->isEmpty();
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

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const Jupiter::Readable_String<T> &in) const
{
	return this->equalsi(in.ptr(), in.size());
}

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const std::basic_string<T> &in) const
{
	return this->equalsi(in.data(), in.size());
}

template<> bool inline Jupiter::Readable_String<char>::equalsi(const char *in, size_t len) const
{
	if (this->size() != len)
		return false;

	in += len;
	while (len != 0)
		if (toupper(this->get(--len)) != toupper(*(--in)))
			return false;

	return true;
}

template<> bool inline Jupiter::Readable_String<wchar_t>::equalsi(const wchar_t *in, size_t len) const
{
	if (this->size() != len)
		return false;

	while (len != 0)
		if (towupper(this->get(--len)) != towupper(*(--in)))
			return false;

	return true;
}

template<typename T> bool Jupiter::Readable_String<T>::equalsi(const T *in, size_t len) const
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

template<typename T> bool Jupiter::Readable_String<T>::match(const Jupiter::Readable_String<T> &format) const
{
	return this->match(format.ptr(), format.size());
}

template<typename T> bool Jupiter::Readable_String<T>::match(const std::basic_string<T> &format) const
{
	return this->match(format.data(), format.size());
}

template<> bool inline Jupiter::Readable_String<char>::match(const char *format, size_t formatSize) const
{
	if (this->size() == 0)
	{
		if (formatSize == 0)
			return true;
		while (format[--formatSize] == '*')
			if (formatSize == 0)
				return *format == '*';
		return false;
	}
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != formatSize)
	{
		if (format[formatIndex] == '*')
		{
			if (++formatIndex == formatSize)
				return true;

			while (format[formatIndex] == '?')
			{
				if (++formatIndex == formatSize)
					return true;
				if (++index == this->size())
				{
					while (format[formatIndex++] == '*')
						if (formatIndex == formatSize)
							return true;
					return false;
				}
			}
			if (format[formatIndex] == '*') continue;

			while (format[formatIndex] != this->get(index))
			{
				if (++index == this->size())
				{
					while (format[formatIndex] == '*')
						if (++formatIndex == formatSize)
							return true;
					return false;
				}
			}
		}
		else if (format[formatIndex] != '?' && format[formatIndex] != this->get(index)) return false;
		formatIndex++;
		if (++index == this->size())
		{
			if (formatIndex == formatSize)
				return true;
			while (format[formatIndex] == '*')
				if (++formatIndex == formatSize)
					return true;
			return false;
		}
	}
	return false;
}

template<> bool inline Jupiter::Readable_String<wchar_t>::match(const wchar_t *format, size_t formatSize) const
{
	if (this->size() == 0)
	{
		if (formatSize == 0)
			return true;
		while (format[--formatSize] == L'*')
			if (formatSize == 0)
				return *format == L'*';
		return false;
	}
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != formatSize)
	{
		if (format[formatIndex] == L'*')
		{
			if (++formatIndex == formatSize)
				return true;

			while (format[formatIndex] == L'?')
			{
				if (++formatIndex == formatSize)
					return index + 1 == this->size();
				if (++index == this->size())
				{
					while (format[formatIndex++] == L'*')
						if (formatIndex == formatSize)
							return true;
					return false;
				}
			}
			if (format[formatIndex] == L'*') continue;

			while (format[formatIndex] != this->get(index))
			{
				if (++index == this->size())
				{
					while (format[formatIndex] == L'*')
						if (++formatIndex == formatSize)
							return true;
					return false;
				}
			}
		}
		else if (format[formatIndex] != L'?' && format[formatIndex] != this->get(index)) return false;
		formatIndex++;
		if (++index == this->size())
		{
			while (format[formatIndex] == L'*')
				if (++formatIndex == formatSize)
					return true;
			return false;
		}
	}
	return false;
}

template<typename T> bool Jupiter::Readable_String<T>::match(const T *, size_t) const
{
	return false; // Wildcard matching not supported for type.
}

template<> bool inline Jupiter::Readable_String<char>::match(const char *format) const
{
	if (this->size() == 0)
	{
		while (*format == '*')
			format++;
		return *format == 0;
	}
	size_t index = 0;
	while (*format != 0)
	{
		if (*format == '*')
		{
			format++;
			while (*format == '?')
			{
				format++;
				if (++index == this->size())
				{
					while (*format == '*')
						format++;
					return *format == 0;
				}
			}
			if (*format == 0) return true;
			if (*format == '*') continue;

			while (*format != this->get(index))
			{
				if (++index == this->size())
				{
					while (*format == '*')
						format++;
					return *format == 0;
				}
			}
		}
		else if (*format != '?' && *format != this->get(index)) return false;
		format++;
		if (++index == this->size())
		{
			while (*format == '*')
				format++;
			return *format == 0;
		}
	}
	return false;
}

template<> bool inline Jupiter::Readable_String<wchar_t>::match(const wchar_t *format) const
{
	if (this->size() == 0)
	{
		while (*format == L'*')
			format++;
		return *format == 0;
	}
	size_t index = 0;
	while (*format != 0)
	{
		if (*format == L'*')
		{
			format++;
			while (*format == L'?')
			{
				format++;
				if (this->size() == 0)
				{
					while (*format == L'*')
						format++;
					return *format == 0;
				}
			}
			if (*format == 0) return true;
			if (*format == '*') continue;

			while (*format != this->get(index))
			{
				if (this->size() == 0)
				{
					while (*format == L'*')
						format++;
					return *format == 0;
				}
			}
		}
		else if (*format != L'?' && *format != this->get(index)) return false;
		format++;
		if (this->size() == 0)
		{
			while (*format == L'*')
				format++;
			return *format == 0;
		}
	}
	return false;
}

template<typename T> bool Jupiter::Readable_String<T>::match(const T *format) const
{
	return false; // Wildcard matching not supported for type.
}

// matchi()

template<typename T> bool Jupiter::Readable_String<T>::matchi(const Jupiter::Readable_String<T> &format) const
{
	return this->matchi(format.ptr(), format.size());
}

template<typename T> bool Jupiter::Readable_String<T>::matchi(const std::basic_string<T> &format) const
{
	return this->matchi(format.data(), format.size());
}

template<> bool inline Jupiter::Readable_String<char>::matchi(const char *format, size_t formatSize) const
{
	if (this->size() == 0)
	{
		if (formatSize == 0)
			return true;
		while (format[--formatSize] == '*')
			if (formatSize == 0)
				return *format == '*';
		return false;
	}
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != formatSize)
	{
		if (format[formatIndex] == '*')
		{
			if (++formatIndex == formatSize)
				return true;

			while (format[formatIndex] == '?')
			{
				if (++formatIndex == formatSize)
					return true;
				if (++index == this->size())
				{
					while (format[formatIndex++] == '*')
						if (formatIndex == formatSize)
							return true;
					return false;
				}
			}
			if (format[formatIndex] == '*') continue;

			while (toupper(format[formatIndex]) != toupper(this->get(index)))
			{
				if (++index == this->size())
				{
					while (format[formatIndex] == '*')
						if (++formatIndex == formatSize)
							return true;
					return false;
				}
			}
		}
		else if (format[formatIndex] != '?' && toupper(format[formatIndex]) != toupper(this->get(index))) return false;
		formatIndex++;
		if (++index == this->size())
		{
			if (formatIndex == formatSize)
				return true;
			while (format[formatIndex] == '*')
				if (++formatIndex == formatSize)
					return true;
			return false;
		}
	}
	return false;
}

template<> bool inline Jupiter::Readable_String<wchar_t>::matchi(const wchar_t *format, size_t formatSize) const
{
	if (this->size() == 0)
	{
		if (formatSize == 0)
			return true;
		while (format[--formatSize] == L'*')
			if (formatSize == 0)
				return *format == L'*';
		return false;
	}
	size_t index = 0;
	size_t formatIndex = 0;
	while (formatIndex != formatSize)
	{
		if (format[formatIndex] == L'*')
		{
			if (++formatIndex == formatSize)
				return true;

			while (format[formatIndex] == L'?')
			{
				if (++formatIndex == formatSize)
					return index + 1 == this->size();
				if (++index == this->size())
				{
					while (format[formatIndex++] == L'*')
						if (formatIndex == formatSize)
							return true;
					return false;
				}
			}
			if (format[formatIndex] == L'*') continue;

			while (towupper(format[formatIndex]) != towupper(this->get(index)))
			{
				if (++index == this->size())
				{
					while (format[formatIndex] == L'*')
						if (++formatIndex == formatSize)
							return true;
					return false;
				}
			}
		}
		else if (format[formatIndex] != L'?' && towupper(format[formatIndex]) != towupper(this->get(index))) return false;
		formatIndex++;
		if (++index == this->size())
		{
			while (format[formatIndex] == L'*')
				if (++formatIndex == formatSize)
					return true;
			return false;
		}
	}
	return false;
}

template<typename T> bool Jupiter::Readable_String<T>::matchi(const T *, size_t) const
{
	return false; // Wildcard matching not supported for type. Concept of "case" not supported for type.
}

template<> bool inline Jupiter::Readable_String<char>::matchi(const char *format) const
{
	if (this->size() == 0)
	{
		while (*format == '*')
			format++;
		return *format == 0;
	}
	size_t index = 0;
	while (*format != 0)
	{
		if (*format == '*')
		{
			format++;
			while (*format == '?')
			{
				format++;
				if (++index == this->size())
				{
					while (*format == '*')
						format++;
					return *format == 0;
				}
			}
			if (*format == 0) return true;
			if (*format == '*') continue;

			while (toupper(*format) != toupper(this->get(index)))
			{
				if (++index == this->size())
				{
					while (*format == '*')
						format++;
					return *format == 0;
				}
			}
		}
		else if (*format != '?' && toupper(*format) != toupper(this->get(index))) return false;
		format++;
		if (++index == this->size())
		{
			while (*format == '*')
				format++;
			return *format == 0;
		}
	}
	return false;
}

template<> bool inline Jupiter::Readable_String<wchar_t>::matchi(const wchar_t *format) const
{
	if (this->size() == 0)
	{
		while (*format == L'*')
			format++;
		return *format == 0;
	}
	size_t index = 0;
	while (*format != 0)
	{
		if (*format == L'*')
		{
			format++;
			while (*format == L'?')
			{
				format++;
				if (this->size() == 0)
				{
					while (*format == L'*')
						format++;
					return *format == 0;
				}
			}
			if (*format == 0) return true;
			if (*format == '*') continue;

			while (towupper(*format) != towupper(this->get(index)))
			{
				if (this->size() == 0)
				{
					while (*format == L'*')
						format++;
					return *format == 0;
				}
			}
		}
		else if (*format != L'?' && towupper(*format) != towupper(this->get(index))) return false;
		format++;
		if (this->size() == 0)
		{
			while (*format == L'*')
				format++;
			return *format == 0;
		}
	}
	return false;
}

template<typename T> bool Jupiter::Readable_String<T>::matchi(const T *format) const
{
	return false; // Wildcard matching not supported for type. Concept of "case" not supported for type.
}

// wordCount()

template<typename T> unsigned int Jupiter::Readable_String<T>::wordCount(const T *whitespace) const
{
	unsigned int result = 0;
	size_t i = 0;
	bool prev = true;
	while (i != this->size())
	{
		if (Jupiter::strpbrk<T>(whitespace, this->get(i)) == nullptr) // This isn't whitespace!
		{
			if (prev == true) // We just left whitespace!
			{
				prev = false;
				result++;
			}
		}
		else prev = true; // This is whitespace!
		i++;
	}
	return result;
}

// tokenCount()

template<typename T> size_t Jupiter::Readable_String<T>::tokenCount(const T &token) const
{
	if (this->size() == 0)
		return 0;

	size_t total = 1;
	for (size_t i = 0; i != this->size(); i++)
		if (this->get(i) == token)
			total++;
	return total;
}

template<typename T> size_t Jupiter::Readable_String<T>::tokenCount(const Jupiter::Readable_String<T> &token) const
{
	return this->tokenCount(token.ptr(), token.size());
}

template<typename T> size_t Jupiter::Readable_String<T>::tokenCount(const std::basic_string<T> &token) const
{
	return this->tokenCount(token.data(), token.size());
}

template<typename T> size_t Jupiter::Readable_String<T>::tokenCount(const T *token, size_t tokenLength) const
{
	if (tokenLength == 1)
		return this->tokenCount(*token);
	if (tokenLength == 0 || tokenLength > this->size())
		return 0;
	if (tokenLength == this->size())
		return this->equals(token, tokenLength) ? 1 : 0;

	size_t total = 1;
	for (size_t i = 0, j; i != this->size() - tokenLength + 1; i++)
	{
		Jupiter_Readable_String_tokenCount_skip_increment:
		j = 0;
		while (this->get(i + j) == token[j])
		{
			if (++j == tokenLength)
			{
				i += j;
				total++;
				if (i >= this->size() - tokenLength + 1)
					return total;

				goto Jupiter_Readable_String_tokenCount_skip_increment;
			}
		}
	}
	return total;
}

// as<type>

template<> bool inline Jupiter::Readable_String<char>::asBool() const
{
	if (this->equalsi("FALSE")) return false;
	if (this->equalsi('0')) return false;
	if (this->equalsi("OFF")) return false;
	if (this->equalsi('-')) return false;
	return true;
}

template<> bool inline Jupiter::Readable_String<wchar_t>::asBool() const
{
	if (this->equalsi(L"FALSE")) return false;
	if (this->equalsi(L'0')) return false;
	if (this->equalsi(L"OFF")) return false;
	if (this->equalsi(L'-')) return false;
	return true;
}

template<typename T> bool Jupiter::Readable_String<T>::asBool() const
{
	return false;
}

template<> int inline Jupiter::Readable_String<char>::asInt(int base) const
{
	return Jupiter_strtoi_s(this->ptr(), this->size(), base);
}

template<typename T> int Jupiter::Readable_String<T>::asInt(int base) const
{
	return 0;
}

template<> long long inline Jupiter::Readable_String<char>::asLongLong(int base) const
{
	return Jupiter_strtoll_s(this->ptr(), this->size(), base);
}

template<typename T> long long Jupiter::Readable_String<T>::asLongLong(int base) const
{
	return 0;
}

template<> unsigned int inline Jupiter::Readable_String<char>::asUnsignedInt(int base) const
{
	return Jupiter_strtoui_s(this->ptr(), this->size(), base);
}

template<typename T> unsigned int Jupiter::Readable_String<T>::asUnsignedInt(int base) const
{
	return 0;
}

template<> unsigned long long inline Jupiter::Readable_String<char>::asUnsignedLongLong(int base) const
{
	return Jupiter_strtoull_s(this->ptr(), this->size(), base);
}

template<typename T> unsigned long long Jupiter::Readable_String<T>::asUnsignedLongLong(int base) const
{
	return 0;
}

template<> double inline Jupiter::Readable_String<char>::asDouble() const
{
	return Jupiter_strtod_s(this->ptr(), this->size());
}

template<typename T> double Jupiter::Readable_String<T>::asDouble() const
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
	if (*in == 0)
		return R<T>();

	while (Jupiter::strpbrk<T>(whitespace, *in) != nullptr)
		if (*++in == 0)
			return R<T>();

	if (pos == 0)
	{
		do
			++in, ++pos;
		while (*in != 0 && Jupiter::strpbrk<T>(whitespace, *in) == nullptr);

		in -= pos;
		return R<T>::substring(in, 0, pos);
	}

	loop_start:
	{
		if (Jupiter::strpbrk<T>(whitespace, *in) != nullptr)
		{
			do
				if (*++in == 0)
					return R<T>();
			while (Jupiter::strpbrk<T>(whitespace, *in) != nullptr);

			if (--pos == 0)
				goto loop_end;
		}

		if (*++in == 0)
			return R<T>();
		goto loop_start;
	}
	loop_end:

	do
		++in, ++pos;
	while (*in != 0 && Jupiter::strpbrk<T>(whitespace, *in) == nullptr);

	in -= pos;
	return R<T>::substring(in, 0, pos);
}

// getToken

template<typename T> template<template<typename> class R> R<T> Jupiter::Readable_String<T>::getToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token)
{
	size_t x, y;
	for (x = 0; x != in.size() && pos != 0; x++)
		if (in.get(x) == token)
			pos--;
	for (y = x; y != in.size() && in.get(y) != token; y++);
	return R<T>::substring(in, x, y - x);
}

template<typename T> template<template<typename> class R> R<T> Jupiter::Readable_String<T>::getToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token)
{
	if (token.isEmpty())
		return R<T>(in);
	if (token.size() == 1)
		return Jupiter::Readable_String<T>::getToken<R>(in, pos, token.get(0));
	if (pos == 0)
		return R<T>::substring(in, 0, in.find(token));

	size_t i, j;
	for (i = 0; i != in.size(); i++)
	{
		Jupiter_Readable_String_getToken_skip_increment:
		j = 0;
		while (in.get(i + j) == token.get(j))
		{
			if (++j == token.size())
			{
				i += j;
				if (--pos == 0)
				{
					size_t k;
					for (j = i; j != in.size(); j++)
					{
						k = 0;
						while (in.get(j + k) == token.get(k))
							if (++k == token.size())
								return R<T>::substring(in, i, j - i);
					}
					return R<T>::substring(in, i);
				}
				else
					goto Jupiter_Readable_String_getToken_skip_increment;
				break;
			}
		}
	}

	return R<T>();
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

// gotoToken

template<typename T> template<template<typename> class R> R<T> Jupiter::Readable_String<T>::gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token)
{
	size_t i;
	for (i = 0; i != in.size() && pos != 0; i++)
		if (in.get(i) == token)
			pos--;

	return R<T>::substring(in, i);
}

template<typename T> template<template<typename> class R> R<T> Jupiter::Readable_String<T>::gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token)
{
	if (pos == 0 || token.isEmpty())
		return R<T>(in);
	if (token.size() == 1)
		return Jupiter::Readable_String<T>::gotoToken<R>(in, pos, token.get(0));

	size_t i, j;
	for (i = 0; i != in.size(); i++)
	{
		j = 0;
		while (in.get(i + j) == token.get(j))
		{
			if (++j == token.size())
			{
				if (--pos == 0)
					return R<T>::substring(in, i + token.size());
				break;
			}
		}
	}

	return R<T>();
}

// Jupiter::DataBuffer specialization

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::Readable_String>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::Readable_String<Y> *data)
	{
		buffer->secure(sizeof(size_t) + data->size() * sizeof(Y));
		buffer->push<size_t>(data->size());
		buffer->push(reinterpret_cast<const uint8_t *>(data->ptr()), data->size() * sizeof(Y));
	};
};

#endif // _READABLE_STRING_IMP_H_HEADER