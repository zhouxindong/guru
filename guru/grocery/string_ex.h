#pragma once
#ifndef _GURU_STRING_EX_H_
#define _GURU_STRING_EX_H_

#include "../gvals.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <objbase.h>
#include <type_traits>
#include <cassert>

#define STRING_EX_BUF_SIZE 100

_GURU_BEGIN

static const char* BoolTrue = "true";
static const char* BoolFalse = "false";

/*
** tokenize a string by delim
*/
inline 
std::vector<std::string> 
split(std::string const& s, char delim) noexcept
{
	std::vector<std::string> elems;
	std::ostringstream oss;
	for_each(s.cbegin(), s.cend(), [&](char const c)
	{
		if (c == delim)
		{
			if (!oss.str().empty())
				elems.push_back(oss.str());
			oss.str("");
		}
		else
		{
			oss << c;
		}
	});
	if (!oss.str().empty())
		elems.push_back(oss.str());

	return elems;
}

// case ignore char traits
struct ci_char_traits : public std::char_traits<char>
{
	static bool __CLRCALL_OR_CDECL eq(const _Elem& _Left, const _Elem& _Right) _NOEXCEPT
	{	// test for element equality
		return (::toupper(_Left) == ::toupper(_Right));
	}

	static bool __CLRCALL_OR_CDECL lt(const _Elem& _Left, const _Elem& _Right) _NOEXCEPT
	{	// test if _Left precedes _Right
		return (::toupper((unsigned char)_Left) < ::toupper((unsigned char)_Right));
	}

	//static int __CLRCALL_OR_CDECL compare(_In_reads_(_Count) const _Elem * const _First1,
	//	_In_reads_(_Count) const _Elem * const _First2, const size_t _Count) _NOEXCEPT // strengthened
	//{	// compare [_First1, _First1 + _Count) with [_First2, ...)
	//	return (_CSTD memicmp(_First1, _First2, _Count));
	//}

	static const _Elem * __CLRCALL_OR_CDECL find(_In_reads_(_Count) const _Elem *  _First,
		size_t _Count, const _Elem& _Ch) _NOEXCEPT // strengthened
	{	// look for _Ch in [_First, _First + _Count)
		while (_Count-- > 0 && toupper(*_First) != toupper(_Ch))
		{
			++_First;
		}
		return _Count >= 0 ? _First : 0;
	}

};

typedef std::basic_string<char, ci_char_traits> ci_string;

inline
std::string
guid() noexcept
{

	GUID guid;
	CoCreateGuid(&guid);
	char cBuffer[64] = { 0 };
	sprintf_s(cBuffer, sizeof(cBuffer),
		"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1, guid.Data2,
		guid.Data3, guid.Data4[0],
		guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4],
		guid.Data4[5], guid.Data4[6],
		guid.Data4[7]);
	return std::string(cBuffer);
}

inline
bool
end_of(std::string const& str, std::initializer_list<char> chars) noexcept
{
	for (char c : chars)
	{
		if (str.back() == c)
			return true;
	}
	return false;
}

inline
bool
begin_of(std::string const& str, std::initializer_list<char> chars) noexcept
{
	for (char c : chars)
	{
		if (str.front() == c)
			return true;
	}
	return false;
}

#pragma region to_string

inline
int 
snprintf(char* buffer, size_t size, const char* format, ...)
{
	va_list va;
	va_start(va, format);
	const int result = vsnprintf_s(buffer, size, (size_t)-1, format, va);
	va_end(va);
	return result;
}

template <typename T>
inline
std::string
to_string(T v) noexcept
{
	char buf[STRING_EX_BUF_SIZE];
	if (std::is_same_v<T, int32_t> ||
		std::is_same_v<T, int8_t> ||
		std::is_same_v<T, uint8_t> ||
		std::is_same_v<T, int16_t> ||
		std::is_same_v<T, uint16_t>)
	{
		snprintf(buf, STRING_EX_BUF_SIZE, "%d", v);
		return buf;
	}
	if (std::is_same_v<T, uint32_t>)
	{
		snprintf(buf, STRING_EX_BUF_SIZE, "%u", v);
		return buf;
	}
	if (std::is_same_v<T, bool>)
	{
		sprintf(buf, STRING_EX_BUF_SIZE, "%s", v ? BoolTrue : BoolFalse);
		return buf;
	}
	if (std::is_same_v<T, float>)
	{
		snprintf(buf, STRING_EX_BUF_SIZE, "%.8g", v);
		return buf;
	}
	if (std::is_same_v<T, double>)
	{
		snprintf(buf, STRING_EX_BUF_SIZE, "%.17g", v);
		return buf;
	}
	if (std::is_same_v<T, int64_t>)
	{
		snprintf(buf, STRING_EX_BUF_SIZE, "%lld", static_cast<long long>(v));
		return buf;
	}
	if (std::is_same_v<T, uint64_t>)
	{
		snprintf(buf, STRING_EX_BUF_SIZE, "%llu", (long long)v);
		return buf;
	}

	return "";
}

#pragma endregion

#pragma region to_scalr

inline
bool 
string_literal_equal(const char* p, const char* q, int nChar = INT_MAX)
{
	if (p == q) return true;
	assert(p);
	assert(q);
	assert(nChar >= 0);
	return strncmp(p, q, nChar) == 0;
}

inline
bool
to_int(std::string const& s, int* value) noexcept
{
	return sscanf_s(s.c_str(), "%d", value) == 1;
}

inline
bool
to_uint(std::string const& s, unsigned* value) noexcept
{
	return sscanf_s(s.c_str(), "%u", value) == 1;
}

inline
bool 
to_bool(std::string const& s, bool* value) noexcept
{
	int ival = 0;
	if (to_int(s, &ival)) {
		*value = (ival == 0) ? false : true;
		return true;
	}
	static const char* TRUE_TEXT[] = { "true", "True", "TRUE", 0 };
	static const char* FALSE_TEXT[] = { "false", "False", "FALSE", 0 };

	for (int i = 0; TRUE_TEXT[i]; ++i) {
		if (string_literal_equal(s.c_str(), TRUE_TEXT[i])) {
			*value = true;
			return true;
		}
	}
	for (int i = 0; FALSE_TEXT[i]; ++i) {
		if (string_literal_equal(s.c_str(), FALSE_TEXT[i])) {
			*value = false;
			return true;
		}
	}
	return false;
}

inline
bool
to_float(std::string const& s, float* value) noexcept
{
	return sscanf_s(s.c_str(), "%f", value) == 1;
}

inline
bool
to_double(std::string const& s, double* value) noexcept
{
	return sscanf_s(s.c_str(), "%lf", value) == 1;
}

inline
bool
to_int64(std::string const& s, int64_t* value) noexcept
{
	long long v = 0;	
	if (sscanf_s(s.c_str(), "%lld", &v) == 1) {
		*value = static_cast<int64_t>(v);
		return true;
	}
	return false;
}

inline
bool
to_uint64(std::string const& s, uint64_t* value) noexcept
{
	unsigned long long v = 0;	
	if (sscanf_s(s.c_str(), "%llu", &v) == 1) {
		*value = (uint64_t)v;
		return true;
	}
	return false;
}

#pragma endregion

_GURU_END

#endif /* _GURU_STRING_EX_H_ */