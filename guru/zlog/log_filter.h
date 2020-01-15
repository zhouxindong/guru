#pragma once
#ifndef _GURU_LOG_FILTER_H_
#define _GURU_LOG_FILTER_H_

#include "../gvals.h"
#include "log_item.h"
#include "log_channel.h"
#include <unordered_set>
#include <initializer_list>
#include <tuple>
#include "log_level.h"

_GURU_BEGIN

class filter_base
{
public:
	virtual std::ostream& filte(std::shared_ptr<channel_base>, log_item const&) noexcept = 0;
};

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
			//return Dummy_Channel.stream();
			return NullStream;
	}
};

class level_filter_nt : public filter_base
{
public:
	level_filter_nt(int level = _LOG_TRACE) noexcept
		: _level{ level }
	{}

	template <typename _Log_item, typename _Channel>
	std::ostream&
		operator () (_Log_item& item, _Channel& channel) noexcept
	{
		if (item.get_level() >= value)
			return channel.stream();
		else
			//return Dummy_Channel.stream();
			return NullStream;
	}

	std::ostream& filte(std::shared_ptr<channel_base> c, log_item const& l) noexcept override
	{
		if (l.get_level() >= _level)
			return c->stream();
		else
			//return Dummy_Channel.stream();
			return NullStream;
	}

private:
	int _level;
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
			//return Dummy_Channel.stream();
			return NullStream;
	}

private:
	std::unordered_set<int> _set;
};

class set_filter_nt : public filter_base
{
public:
	set_filter_nt(std::/*initializer_list*/vector<int> set) noexcept
		//: _set{ set }
	{
		for (auto i : set)
		{
			_set.insert(i);
		}
	}

	template <typename _Log_item, typename _Channel>
	std::ostream&
		operator () (_Log_item& item, _Channel& channel) noexcept
	{
		if (_set.find(item.get_level()) != _set.end())
			return channel.stream();
		else
			//return Dummy_Channel.stream();
			return NullStream;
	}

	std::ostream& filte(std::shared_ptr<channel_base> c, log_item const& l) noexcept override
	{
		if (_set.find(l.get_level()) != _set.end())
			return c->stream();
		else
			//return Dummy_Channel.stream();
			return NullStream;
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