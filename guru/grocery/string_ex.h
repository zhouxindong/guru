#pragma once
#ifndef _GURU_STRING_EX_H_
#define _GURU_STRING_EX_H_

#include "../gvals.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

_GURU_BEGIN

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

_GURU_END

#endif /* _GURU_STRING_EX_H_ */