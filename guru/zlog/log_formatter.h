#pragma once
#ifndef _GURU_LOG_FORMATTER_H_
#define _GURU_LOG_FORMATTER_H_

#include "../gvals.h"
#include <sstream>
#include "../grocery/chrono_ex.h"
#include "../base_chain/non_instantable.h"

_GURU_BEGIN

/*
** log item formatter
** define log output text
*/
template <typename _Log_item = log_item>
class line_formatter {
public:
	typedef _Log_item log_item_type;
	typedef line_formatter<_Log_item> type;

	static 
	std::string 
	format(log_item_type const& item) noexcept 
	{
		std::ostringstream stream;

		auto t = (tokenize_time_point(to_local(item.get_time_stamp())));
		stream << std::get<0>(t) << "-";
		stream << std::setw(2) << std::setfill('0') << std::get<1>(t) << '-';
		stream << std::setw(2) << std::setfill('0') << std::get<2>(t) << ' ';
		stream << std::setw(2) << std::setfill('0') << std::get<3>(t) << ':';
		stream << std::setw(2) << std::setfill('0') << std::get<4>(t) << ':';
		stream << std::setw(2) << std::setfill('0') << std::get<5>(t) << '.';
		stream << std::setw(3) << std::setfill('0') << std::get<6>(t) << ' ';

		stream << std::setw(5) << std::setfill(' ') << log_level(item.get_level()) << ' ';
		stream << '[' << item.get_file() << ']' << ' ';
		stream << '[' << item.get_caller() << '@' << item.get_line() << ']' << ' ';
		stream << item.message();

		return stream.str();
	}
};

template <typename _Log_item = log_item>
class table_formatter {
public:
	typedef _Log_item log_item_type;
	typedef table_formatter<_Log_item> type;

	static 
	std::string 
	format(log_item_type const& item) noexcept
	{
		std::ostringstream stream;

		auto t = (tokenize_time_point(to_local(item.get_time_stamp())));
		stream << std::get<0>(t) << '/';
		stream << std::setw(2) << std::setfill('0') << std::get<1>(t) << '/';
		stream << std::setw(2) << std::setfill('0') << std::get<2>(t) << ';';
		stream << std::setw(2) << std::setfill('0') << std::get<3>(t) << ':';
		stream << std::setw(2) << std::setfill('0') << std::get<4>(t) << ':';
		stream << std::setw(2) << std::setfill('0') << std::get<5>(t) << '.';
		stream << std::setw(3) << std::setfill('0') << std::get<6>(t) << ';';

		stream << log_level(item.get_level()) << ';';
		stream << item.get_file() << ';';
		stream << item.get_caller() << '@' << item.get_line() << ';';
		stream << item.message();

		return stream.str();
	}
};

template <typename _Log_item = log_item>
using std_formatter = non_instantable<line_formatter<_Log_item>>;

template <typename _Log_item = log_item>
using semicolon_formatter = non_instantable<table_formatter<_Log_item>>;

_GURU_END

#endif /* _GURU_LOG_FORMATTER_H_ */