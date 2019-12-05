#pragma once
#ifndef _GURU_LOG_FILTER_H_
#define _GURU_LOG_FILTER_H_

#include "../gvals.h"
#include "log_item.h"
#include "log_channel.h"
#include <unordered_set>
#include <initializer_list>
#include <tuple>

_GURU_BEGIN

/*
** level filter
*/
template <int _Level>
struct level_filter 
{
	static constexpr int value = _Level;

	template <typename _Log_item, typename _Channel>
	std::ostream& 
	operator () (_Log_item& item, _Channel& channel) noexcept
	{
		if (item.get_level() >= value)
			return channel.stream();
		else
			return Dummy_Channel.stream();
	}
};

/*
** level set used for filter
*/
template <int... levels>
struct set_filter
{
	set_filter() noexcept
	{
		std::initializer_list<int> l{ levels... };
		for (int f : l)
		{
			_set.insert(f);
		}
	}

	bool 
	has(int level) noexcept
	{
		return _set.find(level) != _set.end();
	}

	template <typename _Log_item, typename _Channel>
	std::ostream& 
	operator () (_Log_item& item, _Channel& channel) noexcept
	{
		if (has(item.get_level()))
			return channel.stream();
		else
			return Dummy_Channel.stream();
	}

private:
	std::unordered_set<int> _set;
};

//template <typename _Channel, typename _Log_item = log_item>
//set_filter&
//operator << (set_filter& out, std::tuple<std::ref<_Channel>, std::ref<_Log_item>> const& t)
//{
//	//out(item, fc...) << std_formatter<log_item>::format(item) << std::endl;
//
//	//out.stream() << std_formatter<_Log_item>::format(item);
//	//out.stream() << std::endl;
//	//out.flush();
//	out(std::get<1>(t), std::get<0>(t)) << std_formatter<_Log_item>::format(t.second) << std::endl;
//	return out;
//}

_GURU_END

#endif /* _GURU_LOG_FILTER_H_ */