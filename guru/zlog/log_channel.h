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
#include "log_item.h"
#include "log_formatter.h"
#include <type_traits>
#include <iomanip>

_GURU_BEGIN

inline std::string to_string(log_item const&l, LogFormatter f = LogFormatter::STD_FORMATTER)
{
	std::ostringstream oss;
	oss << (f == LogFormatter::STD_FORMATTER ? std_formatter<log_item>::format(l) :
		f == LogFormatter::TBL_FORMATTER ? semicolon_formatter<log_item>::format(l) : "");
	oss << std::endl;
	return oss.str();
}

class channel_base
{
public:
	virtual ~channel_base() noexcept {}

public:
	virtual std::ostream& stream() noexcept = 0;
	virtual std::ostream& flush() noexcept = 0;
	virtual bool ready() noexcept = 0;
	virtual void log(log_item const&, LogFormatter = LogFormatter::STD_FORMATTER) noexcept = 0;
};

#pragma region null_channel

/*
** null_channel
** used for those log items which be filtered
*/
class null_channel : public channel_base
{
	_PROPERTY_READONLY(null_stream, null)

public:
	explicit
	null_channel(const std::string& = "") noexcept
	{}
	
	std::ostream& 
	stream() noexcept override
	{ 
		return _null; 
	}

	std::ostream&
	flush() noexcept override
	{
		return _null.flush(); 
	}

	bool 
	ready() noexcept override
	{ 
		return true; 
	}

	void log(log_item const& lg, LogFormatter = LogFormatter::STD_FORMATTER) noexcept override
	{
	}
};

static null_channel Dummy_Channel;
static null_stream NullStream;

#pragma endregion

#pragma region console_channel

/*
** console_channel
*/
class console_channel : public channel_base
{
public:
	explicit 
	console_channel(const std::string& = "") noexcept
	{
	}
	
	std::ostream& 
	stream() noexcept override
	{ 
		return std::cerr; 
	}

	std::ostream& 
	flush() noexcept override
	{
		return std::cerr.flush(); 
	}
	
	bool
	ready() noexcept override
	{ 
		return true; 
	}

	void log(log_item const& l, LogFormatter f = LogFormatter::STD_FORMATTER) noexcept override 
	{
		stream() << guru::to_string(l, f);
	}
};

static console_channel ConsoleChannel;

#pragma endregion

#pragma region file_channel

static constexpr int64_t LogFileMaxSize = 10 * 1000 * 1024;
static std::string LogPath = path::combine(path::cwd(), "log");

/*
** file_channel
*/
class file_channel : public channel_base
{
	//class log_path
	//{
	//public:
	//	log_path()
	//	{
	//		path::create_directory(LogPath);
	//	}
	//};

	template <typename _Log_item = log_item, typename Fmt = std_formatter<_Log_item>>
	friend file_channel& operator << (file_channel& out, _Log_item const& item);

	//_PROPERTY_READONLY(log_path, lp)
	//_PROPERTY_READONLY(std::string, name)
	//_PROPERTY_READONLY(std::string, file_name)
	//_PROPERTY_READONLY(std::ofstream, file)
	_PROPERTY_READONLY(roll_file<LogFileMaxSize>, roll)

public:
	explicit 
	file_channel(const std::string& file_name) noexcept :
		_roll{LogPath, file_name}
	{
	}

	file_channel(const file_channel&) = delete;
	file_channel(file_channel&&) = delete;
	file_channel& operator = (const file_channel&) = delete;
	file_channel& operator = (file_channel&&) = delete;

	//~file_channel() noexcept
	//{
	//	//if (ready())
	//	//	_file.close();
	//}

	std::ostream&
	stream() noexcept
	{ 
		//return _file; 
		return _roll.stream();
	}

	std::ostream& 
	flush() noexcept
	{
		//return _file.flush(); 
		return _roll.flush();
	}

	bool
	ready() noexcept 
	{ 
		return _roll.ready();
	}

	void log(log_item const& l, LogFormatter f = LogFormatter::STD_FORMATTER) noexcept override
	{
		assert(ready());
		//int64_t size = file_size(_file_name.c_str());
		////std::cout << size << endl;
		//if (size >= LogFileMaxSize)
		//{
		//	_file.close();
		//	_file_name = path::combine(LogPath, get_log_name(_name, LogPath, LogFileMaxSize));
		//	_file = std::ofstream(_file_name, std::ios::app | std::ios::out);
		//	assert(ready());
		//}

		////out.stream() << /*std_formatter<_Log_item>::format(item)*/Fmt::format(item);
		////out.stream() << std::endl;
		stream() << guru::to_string(l, f);
		flush();
	}
};

/**
 * socket channel used UDP
 */
class udp_channel : public channel_base
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

	void log(log_item const& l, LogFormatter f = LogFormatter::STD_FORMATTER) noexcept override
	{
		assert(ready());
		stream() << guru::to_string(l, f);
		std::string s = std::move(get_stream().str());
		_sender.send(s.c_str(), (int)s.size());
		_stream.str("");
	}
};

#pragma  endregion

//channel_base*
//operator << (channel_base* out, log_item const& item)
//{
//	if (std::is_same_v<null_channel, decltype(*out)>)
//	{
//		out->stream() << Fmt::format(item);
//		out->stream() << std::endl;
//		out->flush();
//	}
//	return out;
//}

template <
	typename _Log_item = log_item,
	typename Fmt = std_formatter<_Log_item>>
null_channel&
operator << (null_channel& out, _Log_item const& item)
{
	//out.stream() << /*std_formatter<_Log_item>::format(item)*/Fmt::format(item);
	//out.stream() << std::endl;
	//out.flush();
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
	//int64_t size = file_size(out._file_name.c_str());
	////std::cout << size << endl;
	//if (size >= LogFileMaxSize)
	//{
	//	out._file.close();
	//	out._file_name = path::combine(LogPath, get_log_name(out._name, LogPath, LogFileMaxSize));
	//	out._file = std::ofstream(out._file_name, std::ios::app | std::ios::out);
	//	assert(out.ready());
	//}

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