#pragma once
#ifndef _GURU_PATH_H_
#define _GURU_PATH_H_

#include "../gvals.h"
#include <string>
#include <windows.h>
#include "chrono_ex.h"
#include "digest.h"

_GURU_BEGIN

class path
{
public:
	/*
	** get current application full path name
	*/
	static
	std::string 
	app_full_name() noexcept
	{
		char buf[MAX_PATH];
		::GetModuleFileNameA(NULL, buf, MAX_PATH);
		return std::string(buf);
	}

	static 
	std::string
	app_path() noexcept
	{
		std::string f = app_full_name();
		size_t pos = f.find_last_of('\\', f.size());
		return f.substr(0, pos);
	}

	/*
	** get current application name(include .exe)
	*/
	static
	std::string 
	app_name(const char* s = "\\") noexcept
	{
		char buf[MAX_PATH];
		::GetModuleFileNameA(NULL, buf, MAX_PATH);
		char szExe[MAX_PATH] = "";
		char *pbuf = NULL;
		char* szLine = ::strtok_s(buf, s, &pbuf);
		while (NULL != szLine)
		{
			::strcpy_s(szExe, szLine);
			szLine = ::strtok_s(NULL, s, &pbuf);
		}
		return std::string(szExe);
	}

	/*
	** get current application nakin name(exclude .exe)
	*/
	static
	std::string 
	app_base_name(const char* s = "\\") noexcept
	{
		std::string name = std::move(app_name(s));
		char buf[MAX_PATH];
		::strncpy_s(buf, name.data(), (size_t)(name.size() - 4));
		return std::string(buf);
	}

	static
	std::string
	file_ext_name(std::string name) noexcept
	{
		size_t idx = name.find_last_of('.');
		return (idx == std::string::npos) ? std::string() : name.substr(idx + 1);
	}

	static
	std::string
	file_name_no_ext(std::string name) noexcept
	{
		size_t idx = name.find_last_of('.');
		return (idx == std::string::npos) ? name : name.substr(0, idx);
	}

	static
	std::string
	cwd() noexcept
	{
		char dir[MAX_PATH];
		::GetCurrentDirectoryA(MAX_PATH, dir);
		return std::string(dir);
	}

	static 
	void
	cwd(std::string new_path) noexcept
	{
		::SetCurrentDirectoryA(new_path.c_str());
	}
};

/*
** create a name 
** name+[digest for _Tx]+year+month+day+ext
*/
template <typename _Tx>
inline 
std::string 
unique_name_by_date_for_class(std::string const& name, std::string const& ext) noexcept
{
	auto t = tokenize_time_point(to_local(std::chrono::system_clock::now()));

	auto digest_name = digest_md5().str_md5(typeid(_Tx).name()).get_md5();
	std::ostringstream oss;
	oss << name << "[" << digest_name << "]"
		<< std::get<0>(t) << std::setw(2) << std::setfill('0')
		<< std::get<1>(t) << std::setw(2) << std::setfill('0') << std::get<2>(t)
		<< ext;
	return oss.str();
}

_GURU_END

#endif /* _GURU_PATH_H_ */