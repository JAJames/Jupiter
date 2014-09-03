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

// isEmpty

template<typename T> bool Jupiter::Readable_String<T>::isEmpty() const
{
	return this->size() == 0;
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
	if (in.size() > this->size()) return Jupiter::INVALID_INDEX;
	if (in.size() == this->size()) return this->equals(in) ? 0 : Jupiter::INVALID_INDEX;
	if (in.size() == 0) return 0;

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
	if (in.size() > this->size()) return Jupiter::INVALID_INDEX;
	if (in.size() == this->size()) return this->equalsi(in) ? 0 : Jupiter::INVALID_INDEX;
	if (in.size() == 0) return 0;

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
	if (in.size() > this->size()) return Jupiter::INVALID_INDEX;
	if (in.size() == this->size()) return this->equalsi(in) ? 0 : Jupiter::INVALID_INDEX;
	if (in.size() == 0) return 0;

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
	while (in.contains(this->get(index))) index++;
	return index;
}

template<typename T> size_t Jupiter::Readable_String<T>::span(const T *in) const
{
	size_t index = 0;
	while (containsSymbol(in, this->get(index))) index++;
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
	if (token.size() == 0)
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
	if (pos == 0 || token.size() == 0)
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
				if (pos-- == 0)
					return R<T>::substring(in, i);
				break;
			}
		}
	}

	return R<T>();
}

#endif // _READABLE_STRING_IMP_H_HEADER