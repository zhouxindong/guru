#pragma once
#ifndef _GURU_LOGGER_H_
#define _GURU_LOGGER_H_

#include "../gvals.h"
#include "log_formatter.h"
#include "log_channel.h"
#include "log_filter.h"
#include <map>
#include <memory>
#include <mutex>
#include "../grocery/path.h"

_GURU_BEGIN

/*
** basic_logger
** thread safe
*/
template <
	typename _Filter = level_filter<_LOG_LEVEL::_LOG_TRACE>,
	typename _Formatter = std_formatter<log_item>,
	typename _Channel = file_channel>

	class basic_logger
{

	_PROPERTY_READONLY(std::string, name)

public:
	typedef basic_logger<_Filter, _Formatter, _Channel> type;
	typedef _Formatter	formatter_type;
	typedef _Filter		filter_type;
	typedef _Channel	channel_type;

	basic_logger(const std::string& name) noexcept:
		_name(name),
		_channel(unique_name_by_date_for_class<decltype(this)>(name, ".log"))
	{
	}

	/*
	** get logger by name(create if nonexist)
	*/
	static
	type&
	get(const std::string& name) noexcept
	{
		std::lock_guard<std::mutex> lock(_s_mutex);
		if (_loggers.find(name) != _loggers.end())
			return *(_loggers[name]);

		std::shared_ptr<type> l = std::make_shared<type>(name);
		_loggers[name] = l;
		return *(_loggers[name]);
	}

	/*
	** out log
	*/
	type& 
	log(typename formatter_type::log_item_type const& item) noexcept
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_filter(item, _channel) << formatter_type::format(item) << std::endl;
		return *this;
	}

private:
	static std::map<std::string, std::shared_ptr<type>> _loggers;
	static std::mutex _s_mutex;

private:
	filter_type _filter;
	channel_type _channel;
	std::mutex _mutex;
};

// definition of _loggers
template <typename _Filter, typename _Formatter, typename _Channel>
std::map<std::string, std::shared_ptr<basic_logger<_Filter, _Formatter, _Channel>>>
basic_logger<_Filter, _Formatter, _Channel>::_loggers;

// definition of _mutex
template <typename _Filter, typename _Formatter, typename _Channel>
std::mutex basic_logger<_Filter, _Formatter, _Channel>::_s_mutex;

// logger(all level, std_formatter<log_item>, file_channel)
typedef basic_logger<> file_logger;

// logger(all level, std_formatter<log_item>, console_channel)
typedef basic_logger<
	level_filter<_LOG_LEVEL::_LOG_TRACE>,
	std_formatter<log_item>,
	console_channel> console_logger;

// logger(INFO above, std_formatter<log_item>, file_channel)
typedef basic_logger<
	level_filter<_LOG_LEVEL::_LOG_INFO>,
	std_formatter<log_item>,
	file_channel> file_info_logger;

// logger(INFO above, std_formatter<log_item>, console_channel)
typedef basic_logger<
	level_filter<_LOG_LEVEL::_LOG_INFO>,
	std_formatter<log_item>,
	console_channel> console_info_logger;

_GURU_END

#endif /* _GURU_LOGGER_H_ */