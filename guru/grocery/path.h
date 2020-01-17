#pragma once
#ifndef _GURU_PATH_H_
#define _GURU_PATH_H_

#include "../gvals.h"
#include <string>
#include <windows.h>
#include "chrono_ex.h"
#include "digest.h"
#include "file.h"
#include "string_ex.h"

#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

#include <stdint.h>
#include <string>

#define MAX_PATH_LEN 256

#ifdef WIN32
#define ACCESS(fileName, accessMode) _access(fileName, accessMode)
#define MKDIR(path) _mkdir(path)
#define PATH_DELIMITER '\\'
#else
#define ACCESS(fileName, accessMode) access(fileName, accessMode)
#define MKDIR(path) mkdir(path, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)
#define PATH_DELIMITER '/'
#endif

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

	/**
	 * 从左到右依次判断文件夹是否存在,不存在就创建
	 */
	static
	int32_t
		create_directory(std::string path)
	{

		if (!end_of(path, { '\\', '/' }))
			path += PATH_DELIMITER;

		size_t len = path.length();
		if (len > MAX_PATH_LEN)
		{
			return -1;
		}
		char tmp[MAX_PATH_LEN] = { 0 };
		for (size_t i = 0; i < len; ++i)
		{
			tmp[i] = path[i];
			if (tmp[i] == '\\' || tmp[i] == '/')
			{
				if (ACCESS(tmp, 0) != 0)
				{
					int32_t ret = MKDIR(tmp);
					if (ret != 0)
					{
						return ret;
					}
				}
			}
		}
		return 0;
	}

	static
		std::string
		combine(std::string const& p1, std::string const& p2)
	{
		std::initializer_list<char> ps{ '/', '\\' };
		if (!end_of(p1, ps) && !begin_of(p2, ps))
			return p1 + PATH_DELIMITER + p2;
		if (end_of(p1, ps) && begin_of(p2, ps))
			return p1.substr(0, p1.size() - 1) + p2;
		return p1 + p2;
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

inline
std::string
get_log_name(std::string const& name, std::string const& log_path, int64_t max_size, int surfix = 1)
{
	auto t = tokenize_time_point(to_local(std::chrono::system_clock::now()));
	std::ostringstream oss;
	oss << name << "_" << std::get<0>(t)
		<< std::setw(2) << std::setfill('0') << std::get<1>(t)
		<< std::setw(2) << std::setfill('0') << std::get<2>(t);
	if (surfix != 1) oss << "_" << surfix;
	oss << ".log";
	std::string file_name = /*oss.str()*/path::combine(log_path, oss.str());	

	if (file_exists(file_name) && file_size(file_name.c_str()) >= max_size)
	{
		return get_log_name(name, log_path, max_size, ++surfix);
	}
	return oss.str();
}

template <size_t FileMaxSize = 10 * 1024 * 1024>
class roll_file
{
	class ensure_directory_exist
	{
	public:
		ensure_directory_exist(std::string const& p)
		{
			path::create_directory(p);
		}
	};

	_PROPERTY_READONLY(ensure_directory_exist, dir_exist)
	_PROPERTY_READONLY(unsigned, today)
	_PROPERTY_READONLY(std::string, name)
	_PROPERTY_READONLY(std::string, log_path)
	_PROPERTY_READONLY(std::string, file_name)
	_PROPERTY_READONLY(std::ofstream, file)

public:
	explicit roll_file(std::string const& log_path, std::string const& name) noexcept :
		_dir_exist{ log_path },
		_today{ std::get<2>(tokenize_time_point(to_local(std::chrono::system_clock::now()))) },
		_name{ name },
		_log_path{ log_path },
		_file_name{ path::combine(log_path, get_log_name(name, log_path, FileMaxSize)) },
		_file {	_file_name, std::ios::app | std::ios::out }
	{
	}

	roll_file(roll_file const&) = delete;
	roll_file(roll_file&&) = delete;
	roll_file& operator = (roll_file const&) = delete;
	roll_file& operator = (roll_file&&) = delete;

	~roll_file() noexcept
	{
		if (_file)
			_file.close();
	}

	std::ostream& stream() noexcept
	{
		if (_diff_day() || file_size(_file_name.c_str()) >= FileMaxSize)
		{
			_file.close();
			_today = std::get<2>(tokenize_time_point(to_local(std::chrono::system_clock::now())));
			_file_name = path::combine(_log_path, get_log_name(_name, _log_path, FileMaxSize));
			_file = ofstream(_file_name, std::ios::app | std::ios::out);
		}
		return _file;
	}

	std::ostream& flush() noexcept
	{
		return _file.flush(); 
	}

	bool ready() noexcept
	{
		return (bool)_file;
	}

private:
	bool _diff_day() noexcept
	{
		unsigned day = std::get<2>(tokenize_time_point(to_local(std::chrono::system_clock::now())));
		return day != _today;
	}
};

_GURU_END

#endif /* _GURU_PATH_H_ */