#pragma once
#ifndef _GURU_PATH_H_
#define _GURU_PATH_H_

#include "../gvals.h"
#include <string>
#include <windows.h>

_GURU_BEGIN

class path
{
public:
	/*
	** get current application full path name
	*/
	static std::string app_full_name()
	{
		char buf[MAX_PATH];
		::GetModuleFileNameA(NULL, buf, MAX_PATH);
		return std::string(buf);
	}

	/*
	** get current application name(include .exe)
	*/
	static std::string app_name(const char* s = "\\")
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
	static std::string app_base_name(const char* s = "\\")
	{
		std::string name = std::move(app_name(s));
		char buf[MAX_PATH];
		::strncpy_s(buf, name.data(), (size_t)(name.size() - 4));
		return std::string(buf);
	}
};

_GURU_END

#endif /* _GURU_PATH_H_ */