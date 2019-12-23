#pragma once
#ifndef _GURU_EVENT_BASE_H_
#define _GURU_EVENT_BASE_H_

#include "../gvals.h"
#include <string>
#include <sstream>
#include <windows.h>
#include <iomanip>

_GURU_BEGIN

enum class EVENT_TYPE
{
	EVENT_NONE,
	EVENT_KEYBOARD,
	EVENT_PROCESS 
};

class event_base
{
public:
	event_base() noexcept :
		_type(EVENT_TYPE::EVENT_NONE)
	{
		::GetSystemTime(&_sys_time);
		::GetLocalTime(&_loc_time);
	}

	event_base(const event_base& other) noexcept
		:_type{ other._type }
	{
		_sys_time = other._sys_time;
		_loc_time = other._loc_time;
	}

	virtual ~event_base() noexcept
	{
	}

	EVENT_TYPE event_type() const noexcept
	{
		return _type;
	}

	const SYSTEMTIME& sys_time() const noexcept
	{
		return _sys_time;
	}

	const SYSTEMTIME& loc_time() const noexcept
	{
		return _loc_time;
	}

public:
	virtual std::string to_string() const noexcept
	{
		std::ostringstream stream;
		stream
			<< "[LocalTime: " << _loc_time.wYear << "/"
			<< std::setw(2) << std::setfill('0') << _loc_time.wMonth << "/"
			<< std::setw(2) << std::setfill('0') << _loc_time.wDay << " "
			<< std::setw(2) << std::setfill('0') << _loc_time.wHour << ":"
			<< std::setw(2) << std::setfill('0') << _loc_time.wMinute << ":"
			<< std::setw(2) << std::setfill('0') << _loc_time.wSecond << ":"
			<< std::setw(3) << std::setfill('0') << _loc_time.wMilliseconds << "] ";
		return stream.str();
	}

protected:
	EVENT_TYPE _type;
	SYSTEMTIME _sys_time;
	SYSTEMTIME _loc_time;
};

_GURU_END

#endif /* _GURU_EVENT_BASE_H_ */