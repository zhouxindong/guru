#pragma once
#ifndef _GURU_LOG_CHANNEL_H_
#define _GURU_LOG_CHANNEL_H_

#include "../gvals.h"
#include "../socket/udp_sender.h"
#include "../stream/null_stream.h"
#include <iostream>
#include <fstream>
#include "../base_feature/lock_fixable.h"
#include "../grocery/string_ex.h"
#include "../grocery/path.h"
#include "../grocery/file.h"

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

static constexpr int64_t LogFileMaxSize = /*10 * 1024 * 1024*/ 4 * 1024;

/*
** file_channel
*/
class file_channel 
{
	template <typename _Log_item = log_item, typename Fmt = std_formatter<_Log_item>>
	friend file_channel& operator << (file_channel& out, _Log_item const& item);

	_PROPERTY_READONLY(std::string, name)
	_PROPERTY_READONLY(std::string, file_name)
	_PROPERTY_READONLY(std::ofstream, file)

public:
	explicit 
	file_channel(const std::string& file_name) noexcept :
		_name(file_name),
		_file_name(get_log_name(file_name)),
		_file(_file_name, std::ios::app | std::ios::out) 
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

/**
 * socket channel used UDP
 */
class udp_channel
{
	template <typename _Log_item = log_item, typename Fmt = std_formatter<_Log_item>>
	friend udp_channel&	operator << (udp_channel& out, _Log_item const& item);

	_PROPERTY_READONLY(std::ostringstream, stream)
	_PROPERTY_READONLY(std::string, name)
	_PROPERTY_READONLY(std::string, dip)
	_PROPERTY_READONLY(uint16_t, dport)
	_PROPERTY_READONLY(std::string, sip)
	_PROPERTY_READONLY(uint16_t, sport)

private:
	udp_sender<lock_fixable> _sender;

public:
	explicit udp_channel(const std::string& str) noexcept
	{
		auto splits = split(str, '#');
		if (splits.size() < 3) return; // name&dip&dport must

		_name = splits[0];
		_sender.dst_ip(splits[1]);
		int value;
		to_int(splits[2], &value);
		_sender.dst_port((uint16_t)value);
		_sender.src_ip(splits.size() >= 4 ? splits[3] : "127.0.0.1");
		if (splits.size() >= 5)
		{
			if (to_int(splits[4], &value))
				_sender.src_port((uint16_t)value);
			else
				_sender.src_port(0);
		}
		else
			_sender.src_port(0);
		_sender.init();
	}

	udp_channel(const udp_channel&) = delete;
	udp_channel(udp_channel&&) = delete;
	udp_channel& operator = (const udp_channel&) = delete;
	udp_channel& operator = (udp_channel&&) = delete;

	std::ostream&
	stream() noexcept
	{
		return _stream;
	}

	std::ostream&
	flush() noexcept
	{
		return _stream.flush();
	}

	bool
	ready() noexcept
	{
		return _sender.get_ready();
	}
};

#pragma  endregion

template <
	typename _Log_item = log_item,
	typename Fmt = std_formatter<_Log_item>>
null_channel&
operator << (null_channel& out, _Log_item const& item)
{
	out.stream() << /*std_formatter<_Log_item>::format(item)*/Fmt::format(item);
	out.stream() << std::endl;
	out.flush();
	return out;
}

template <
	typename _Log_item = log_item,
	typename Fmt = std_formatter<_Log_item>>
console_channel&
operator << (console_channel& out, _Log_item const& item)
{
	out.stream() << /*std_formatter<_Log_item>::format(item)*/Fmt::format(item);
	out.stream() << std::endl;
	out.flush();
	return out;
}

template <typename _Log_item, typename Fmt>
file_channel&
operator << (file_channel& out, _Log_item const& item)
{
	assert(out.ready());
	int64_t size = file_size(out._file_name.c_str());
	//std::cout << size << endl;
	if (size >= LogFileMaxSize)
	{
		out._file.close();
		out._file_name = get_log_name(out._name);
		out._file = std::ofstream(out._file_name, std::ios::app | std::ios::out);
		assert(out.ready());
	}

	out.stream() << /*std_formatter<_Log_item>::format(item)*/Fmt::format(item);
	out.stream() << std::endl;
	out.flush();
	return out;
}

template <typename _Log_item, typename Fmt>
udp_channel&
operator << (udp_channel& out, _Log_item const& item)
{
	assert(out.ready());
	out.stream() << /*std_formatter<_Log_item>::format(item)*/Fmt::format(item) << std::endl;
	std::string s = std::move(out.get_stream().str());
	out._sender.send(s.c_str(), (int)s.size());
	out._stream.str("");
	return out;
}

_GURU_END

#endif /* _GURU_LOG_CHANNEL_H_ */