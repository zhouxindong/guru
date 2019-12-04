#pragma once
#ifndef _GURU_LOG_ITEM_H_
#define _GURU_LOG_ITEM_H_

#include "../gvals.h"
#include "log_level.h"
#include "../grocery/chrono_ex.h"
#include <ostream>
#include <iomanip>

_GURU_BEGIN

/*
** a log item
*/
class log_item 
{
	_PROPERTY_READONLY(_LOG_LEVEL, level)
	_PROPERTY_READONLY(std::string, file)
	_PROPERTY_READONLY(std::string, caller)
	_PROPERTY_READONLY(size_t, line)
	_PROPERTY_READONLY(unsigned int, pid)
	_PROPERTY_READONLY(std::chrono::system_clock::time_point, time_stamp)

public:

	log_item(_LOG_LEVEL level, std::string file, std::string caller, size_t line) noexcept :
		_level(level),
		_file(file),
		_caller(caller),
		_line(line),
		_pid(GetCurrentProcessId()),
		_time_stamp(std::chrono::system_clock::now()) 
	{
	}

	log_item(const log_item& rhs) noexcept :
		_level(rhs._level),
		_file(rhs._file),
		_caller(rhs._caller),
		_line(rhs._line),
		_pid(rhs._pid),
		_time_stamp(rhs._time_stamp),
		_stream(rhs._stream.str()) 
	{
	}

	log_item(log_item&& rhs) noexcept :
		_level(rhs._level),
		_file(std::move(rhs._file)),
		_caller(std::move(rhs._caller)),
		_line(rhs._line),
		_pid(rhs._pid),
		_time_stamp(std::move(rhs._time_stamp)),
		_stream(std::move(rhs._stream)) 
	{
	}

	log_item& 
	operator = (const log_item& rhs) noexcept
	{
		_level = rhs._level;
		_file = rhs._file;
		_caller = rhs._caller;
		_line = rhs._line;
		_pid = rhs._pid;
		_time_stamp = rhs._time_stamp;
		_stream = std::ostringstream();
		_stream << rhs._stream.str();
		return *this;
	}

	log_item& 
	operator = (log_item&& rhs) noexcept
	{
		_level = rhs._level;
		_file = std::move(rhs._file);
		_caller = std::move(rhs._caller);
		_line = rhs._line;
		_pid = rhs._pid;
		_time_stamp = std::move(rhs._time_stamp);
		_stream = std::move(rhs._stream);
		return *this;
	}

	~log_item() = default;

	std::string 
	message() const noexcept
	{
		return _stream.str();
	}

	/*
	** write message to log item
	*/
	template <typename _Tx>
	log_item& 
	operator << (const _Tx& data) noexcept
	{
		_stream << data;
		return *this;
	}

	//log_item&
	//new_line() noexcept
	//{
	//	_stream << std::endl;
	//	return *this;
	//}

	//friend log_item& _cdecl endl(log_item& out) noexcept;

private:
	std::ostringstream _stream;
};

inline
std::ostream&
operator << (std::ostream& out, log_item const& log) noexcept
{
	using std::endl;
	out << "level:\t" << log_level(log.get_level()) << endl;
	out << "file:\t" << log.get_file() << endl;
	out << "caller:\t" << log.get_caller() << endl;
	out << "line:\t" << log.get_line() << endl;
	out << "pid:\t" << log.get_pid() << endl;
	out << "time:\t" << to_local(log.get_time_stamp()) << endl;
	out << "msg:\t" << log.message() << endl;
	return out;
}

//inline
//log_item&
//_cdecl
//endl(log_item& out) noexcept
//{
//	out._stream.put(out._stream.widen('\n'));
//	out._stream.flush();
//	return out;
//}

#define LOG_TRACE log_item(_LOG_LEVEL::_LOG_TRACE, __FILE__, __func__, __LINE__)
#define LOG_DEBUG log_item(_LOG_LEVEL::_LOG_DEBUG, __FILE__, __func__, __LINE__)
#define LOG_INFO  log_item(_LOG_LEVEL::_LOG_INFO,  __FILE__, __func__, __LINE__)
#define LOG_WARN  log_item(_LOG_LEVEL::_LOG_WARN,  __FILE__, __func__, __LINE__)
#define LOG_ERROR log_item(_LOG_LEVEL::_LOG_ERROR, __FILE__, __func__, __LINE__)
#define LOG_FATAL log_item(_LOG_LEVEL::_LOG_FATAL, __FILE__, __func__, __LINE__)

#define LOG(level) LOG_##level

_GURU_END

#endif /* _GURU_LOG_ITEM_H_ */