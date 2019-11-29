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

_GURU_END

#endif /* _GURU_STRING_EX_H_ */