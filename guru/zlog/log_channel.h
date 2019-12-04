#pragma once
#ifndef _GURU_LOG_CHANNEL_H_
#define _GURU_LOG_CHANNEL_H_

#include "../gvals.h"
#include "../stream/null_stream.h"
#include <iostream>
#include <fstream>

_GURU_BEGIN

#pragma region null_channel

/*
** null_channel
** used for those log items which be filtered
*/
class null_channel 
{
	_PROPERTY_READONLY(null_stream, null)

public:
	explicit
	null_channel(const std::string& = "") noexcept
	{}
	
	std::ostream& 
	stream() noexcept
	{ 
		return _null; 
	}

	std::ostream&
	flush() noexcept
	{
		return _null.flush(); 
	}

	bool 
	ready() noexcept
	{ 
		return true; 
	}
};

static null_channel Dummy_Channel;

#pragma endregion

#pragma region console_channel

/*
** console_channel
*/
class console_channel 
{
public:
	explicit 
	console_channel(const std::string& = "") noexcept
	{
	}
	
	std::ostream& 
	stream() noexcept
	{ 
		return std::cerr; 
	}

	std::ostream& 
	flush() noexcept
	{
		return std::cerr.flush(); 
	}
	
	bool
	ready() noexcept
	{ 
		return true; 
	}
};

static console_channel Console_Channel;

#pragma endregion

#pragma region file_channel

/*
** file_channel
*/
class file_channel 
{
	_PROPERTY_READONLY(std::ofstream, file)

public:
	explicit 
	file_channel(const std::string& file_name) noexcept :
		_file(file_name, std::ios::app | std::ios::out) 
	{
	}

	file_channel(const file_channel&) = delete;
	file_channel(file_channel&&) = delete;
	file_channel& operator = (const file_channel&) = delete;
	file_channel& operator = (file_channel&&) = delete;

	~file_channel() noexcept
	{
		if (ready())
			_file.close();
	}

	std::ostream&
	stream() noexcept
	{ 
		return _file; 
	}

	std::ostream& 
	flush() noexcept
	{
		return _file.flush(); 
	}

	bool
	ready() noexcept 
	{ 
		return (bool)_file; 
	}
};

#pragma  endregion

template <typename _Log_item = log_item>
null_channel&
operator << (null_channel& out, _Log_item const& item)
{
	out.stream() << std_formatter<_Log_item>::format(item);
	out.stream() << std::endl;
	out.flush();
	return out;
}

template <typename _Log_item = log_item>
console_channel&
operator << (console_channel& out, _Log_item const& item)
{
	out.stream() << std_formatter<_Log_item>::format(item);
	out.stream() << std::endl;
	out.flush();
	return out;
}

template <typename _Log_item = log_item>
file_channel&
operator << (file_channel& out, _Log_item const& item)
{
	out.stream() << std_formatter<_Log_item>::format(item);
	out.stream() << std::endl;
	out.flush();
	return out;
}

_GURU_END

#endif /* _GURU_LOG_CHANNEL_H_ */