#pragma once
#ifndef _GURU_FILE_H_
#define _GURU_FILE_H_

#include "../gvals.h"
#include <fstream>
#include <string>

_GURU_BEGIN

enum class FILE_OP_RESULT
{
	UNDEF,
	SUCCEEDED,
	FAILED,
	EXISTS,
	NONEXISTS,
};

inline
bool 
file_exists(std::string const& name) noexcept
{
	std::ifstream f(name);
	return f.good();
}

inline
bool
remove_file(std::string const& name) noexcept
{
	return ::remove(name.c_str()) == 0;
}

inline
FILE_OP_RESULT
copy_file_b(std::string s, std::string d) noexcept
{
	if (!file_exists(s)) return FILE_OP_RESULT::NONEXISTS;
	if (file_exists(d)) return FILE_OP_RESULT::EXISTS;

	std::ifstream in(s, std::ios::binary | std::ios::in);
	if (!in) return FILE_OP_RESULT::FAILED;

	std::ofstream out(d, std::ios::binary | std::ios::out);
	if (!out) return FILE_OP_RESULT::FAILED;

	char c;
	while (in.get(c))
		out.put(c);
	out.close();
	in.close();

	return FILE_OP_RESULT::SUCCEEDED;
}

_GURU_END

#endif /* _GURU_FILE_H_ */