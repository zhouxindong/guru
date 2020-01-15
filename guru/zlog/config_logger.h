#pragma once
#ifndef _GURU_ZLOG_LOGGER_CONFIG_H_
#define _GURU_ZLOG_LOGGER_CONFIG_H_

#include "../gvals.h"

#include <string>
#include "log_filter.h"
#include <memory>
#include "../grocery/string_ex.h"
#include <algorithm>
#include "../grocery/tinyxml2_wrap.h"
#include "../dump/uncatcher.h"

_GURU_BEGIN

static std::string Channel_Attr		= "CHANNEL";
static std::string Name_Attr		= "NAME";
static std::string RIP_Attr			= "RIP";
static std::string RPORT_Attr		= "RPORT";
static std::string LIP_Attr			= "LIP";
static std::string LPORT_Attr		= "LPORT";
static std::string Type_Attr		= "TYPE";
static std::string Value_Attr		= "VALUE";

static std::string Std_Formatter	= "STD";
static std::string Tbl_Formatter	= "TBL";

static std::string Level_Filter		= "LEVEL";
static std::string Set_Filter		= "SET";
static std::string Trace_Level		= "TRACE";
static std::string Debug_Level		= "DEBUG";
static std::string Info_Level		= "INFO";
static std::string Warn_Level		= "WARN";
static std::string Error_Level		= "ERROR";
static std::string Fatal_Level		= "FATAL";

static std::string Console_Channel	= "CONSOLE";
static std::string File_Channel		= "FILE";
static std::string Socket_Channel	= "SOCKET";

static std::string Log_Element		= "LOG";
static std::string Logger_Element	= "LOGGER";
static std::string Socket_Element	= "SOCKET";
static std::string Format_Element	= "FORMAT";
static std::string Filter_Element	= "FILTER";

inline std::pair<bool, _LOG_LEVEL> log_level_from_text(std::string const& t)
{
	if (t == "TRACE" || t == "trace" || t == "Trace")
		return std::make_pair(true, _LOG_LEVEL::_LOG_TRACE);
	if (t == "DEBUG" || t == "debug" || t == "Debug")
		return std::make_pair(true, _LOG_LEVEL::_LOG_DEBUG);
	if (t == "INFO" || t == "info" || t == "Info")
		return std::make_pair(true, _LOG_LEVEL::_LOG_INFO);
	if (t == "WARN" || t == "warn" || t == "Warn")
		return std::make_pair(true, _LOG_LEVEL::_LOG_WARN);
	if (t == "ERROR" || t == "error" || t == "Error")
		return std::make_pair(true, _LOG_LEVEL::_LOG_ERROR);
	if (t == "FATAL" || t == "fatal" || t == "Fatal")
		return std::make_pair(true, _LOG_LEVEL::_LOG_FATAL);
	return std::make_pair(false, _LOG_LEVEL::_LOG_DEBUG);
}

inline LogFormatter log_fmt_from_text(std::string const& t)
{
	if (t == "TBL" || t == "tbl" || t == "Tbl")
		return LogFormatter::TBL_FORMATTER;
	return LogFormatter::STD_FORMATTER;
}

inline std::shared_ptr<filter_base> filter_from_text(std::string const& t, std::string const& v)
{
	if (t == "LEVEL" || t == "level" || t == "Level")
	{
		auto p = log_level_from_text(v);
		if (p.first)
		{
			return std::shared_ptr<filter_base>{new level_filter_nt{ p.second }};
		}
	}
	if (t == "SET" || t == "set" || t == "Set")
	{
		std::vector<int> vec;
		auto tokens = split(v, ',');
		for (auto s : tokens)
		{
			auto p = log_level_from_text(s);
			if (p.first) vec.push_back(p.second);
		}
		return std::shared_ptr<filter_base>{new set_filter_nt{ vec }};
	}
	return std::shared_ptr<filter_base>{new level_filter_nt{ _LOG_TRACE }};
}

template <typename Xmler = tinyxml2_wrap>
class config_logger
{
	using xml_parser = Xmler;
	using element = typename Xmler::element_type;

public:
	config_logger(element ele) noexcept
		: _ele{ ele },
		_uncatcher([&](std::string s) {
		log_item li = LOG(FATAL) << s;
		log(li);
	})
	{
		xml_parser parser{};
		auto p = parser.text_attribute(_ele, Channel_Attr.c_str());
		if (!p.first) return;
		if (p.second == Socket_Channel)
			_socket_channel(parser);
		else if (p.second == Console_Channel)
			_console_channel(parser);
		else if (p.second == File_Channel)
			_file_channel(parser);
	}

	config_logger(config_logger const&) = delete;
	config_logger(config_logger&&) = delete;

	void log(log_item const& item) noexcept
	{
		if (!_ready) return;
		std::lock_guard<std::mutex> lock(_mutex);
		if (&(_filter->filte(_channel, item)) != &NullStream)
		{
			_channel->log(item, _fmt);
		}
	}

private:
	void _socket_channel(xml_parser& parser)
	{
		auto p = parser.text_attribute(_ele, Name_Attr.c_str());
		if (!p.first) return;
		
		std::ostringstream oss;
		oss << p.second << '#';

		auto v = parser.childs(_ele, Socket_Element.c_str());
		if (v.size() != 1) return;
		p = parser.text_attribute(v[0], RIP_Attr.c_str());
		if (!p.first) return;
		oss << p.second << '#';
		p = parser.text_attribute(v[0], RPORT_Attr.c_str());
		if (!p.first) return;
		oss << p.second << '#';
		p = parser.text_attribute(v[0], LIP_Attr.c_str());
		if (!p.first) return;
		oss << p.second;
		p = parser.text_attribute(v[0], LPORT_Attr.c_str());
		if (p.first) oss << '#' << p.second;
		_channel = std::shared_ptr<channel_base>{ new udp_channel{oss.str()} };

		_parse_fmt(parser);
		_parse_filter(parser);
		_ready = true;
	}

	void _console_channel(xml_parser& parser)
	{
		_channel = std::shared_ptr<channel_base>{ new console_channel };
		_parse_filter(parser);

		_ready = true;
	}

	void _file_channel(xml_parser& parser)
	{
		auto p = parser.text_attribute(_ele, Name_Attr.c_str());
		if (!p.first) return;

		_channel = std::shared_ptr<channel_base>{ new file_channel{p.second} };
		_parse_fmt(parser);
		_parse_filter(parser);
		_ready = true;
	}
private:
	element _ele;
	LogFormatter _fmt{ LogFormatter::STD_FORMATTER };
	std::shared_ptr<channel_base> _channel;
	std::shared_ptr<filter_base> _filter;
	bool _ready{ false };
	std::mutex _mutex;
	uncatcher _uncatcher;


private:
	void _parse_filter(xml_parser& parser)
	{
		auto v = parser.childs(_ele, Filter_Element.c_str());
		if (v.size() == 1)
		{
			auto p = parser.text_attribute(v[0], Type_Attr.c_str());
			auto p2 = parser.text_attribute(v[0], Value_Attr.c_str());
			_filter = filter_from_text(p.second, p2.second);
		}
		else
			_filter = std::shared_ptr<filter_base>{ new level_filter_nt{ _LOG_TRACE } };
	}

	void _parse_fmt(xml_parser& parser)
	{
		auto v = parser.childs(_ele, Format_Element.c_str());
		if (v.size() == 1)
		{
			auto p = parser.text(v[0]);
			_fmt = log_fmt_from_text(p);
		}
	}
};

class config_log
{
public:
	config_log(char const* file)
	{
		tinyxml2_wrap xml(file);
		auto l = xml.all(Logger_Element.c_str(), Log_Element.c_str());
		for (auto g : l)
		{
			_logs.push_back(std::shared_ptr<config_logger<>>{new
				config_logger<>{ g }});
		}
	}

	void log(log_item const& item) noexcept
	{
		for_each(_logs.cbegin(), _logs.cend(), [&](auto const& l) {
			l->log(item);
		});
	}

private:
	std::vector<std::shared_ptr<config_logger<>>> _logs;
};

_GURU_END

#endif // _GURU_ZLOG_LOGGER_CONFIG_H_