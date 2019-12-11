#pragma once
#ifndef _GURU_OSTREAM_EX_H_
#define _GURU_OSTREAM_EX_H_

#include "../gvals.h"
#include <ostream>
#include <iomanip>
#include <algorithm>
#include <type_traits>
#include <vector>

_GURU_BEGIN

_GURU_END

_STD_BEGIN

template <typename _Tx>
std::ostream& operator << (std::ostream& out, std::vector<_Tx> const& v)
{
	typedef std::common_type<_Tx, int>::type out_type;

	size_t line = 0;
	size_t count = 0;

	std::cout << "size:\t" << v.size() << std::endl;
	for_each(v.cbegin(), v.cend(), [&](auto& val)
	{
		if (count % 10 == 0)
		{
			if (count != 0)
			{
				std::cout << std::endl;
			}
			std::cout << std::setw(4) << std::setfill('0') << line++;
		}
		std::cout << "\t" << (out_type)val;
		count++;
	});
	std::cout << std::endl;
	return out;
}

_STD_END

#endif /* _GURU_OSTREAM_EX_H_ */