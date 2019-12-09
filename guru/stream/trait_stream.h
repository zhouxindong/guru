#pragma once

#ifndef _GURU_TRAIT_STREAM_H_
#define _GURU_TRAIT_STREAM_H_

#include "../gvals.h"
#include "../nut/signal.h"

#include <deque>
#include <vector>
#include <mutex>
#include <algorithm>

_GURU_BEGIN

enum class MatchState
{
	Unknown = 0,

	ToFindHead = 1,
	ToFindTail = 3,
};

/*
** process the byte stream which split with trait bytes
** N is the max size of mathced data block
*/
template <size_t N = 4096>
class trait_stream
{
public:
	typedef trait_stream<N> self_class;
	typedef trait_stream<N> type;

	signal<void(std::vector<uint8_t>)>& matched_signal();
	void set_src_signal(signal<void(std::vector<uint8_t>)>& src_signal);

protected:
	trait_stream() noexcept {}
	~trait_stream() noexcept;

protected:
	virtual void _process() = 0;

protected:
	std::deque<uint8_t> _unmatched;
	std::vector<uint8_t> _matched;
	signal<void(std::vector<uint8_t>)> _arrived_signal;
	signal<void(std::vector<uint8_t>)> _matched_signal;

	mutable std::mutex _mutex;
	MatchState _state{ MatchState::ToFindHead };
};

template<size_t N>
inline
signal<void(std::vector<uint8_t>)>& 
trait_stream<N>::matched_signal() noexcept
{
	return _matched_signal;
}

template<size_t N>
inline 
trait_stream<N>::~trait_stream() noexcept
{
	_arrived_signal.disconnect_all();
	_matched_signal.disconnect_all();
}

/*
** set data source
*/
template<size_t N>
inline 
void 
trait_stream<N>::set_src_signal(signal<void(std::vector<uint8_t>)>& src_signal)
{
	_arrived_signal.disconnect_all();
	src_signal.connect(&_arrived_signal);
	_arrived_signal.connect([&](auto v)
	{
		{
			std::lock_guard<std::mutex> locker(_mutex);
			std::copy(v.cbegin(), v.cend(), std::back_inserter<deque<uint8_t>>(_unmatched));
		}
		if (!_unmatched.empty()) _process();
	});
}

_GURU_END

#endif /* _GURU_TRAIT_STREAM_H_ */