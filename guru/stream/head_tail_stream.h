#pragma once
#ifndef _GURU_HEAD_TAIL_STREAM_H_
#define _GURU_HEAD_TAIL_STREAM_H_

#include "../gvals.h"
#include "trait_stream.h"
#include <iterator>

_GURU_BEGIN

/*
** stream that framed by head and tail trait bytes
*/
template <size_t N = 4096>
class head_tail_stream : public trait_stream<N>
{
public:
	typedef head_tail_stream<N> self_class;
	typedef head_tail_stream<N> type;

	typedef trait_stream<N> base_class;
	typedef trait_stream<N> base_type;

	head_tail_stream(std::initializer_list<uint8_t> head, std::initializer_list<uint8_t> tail) noexcept
		: _head(head), _tail(tail)
	{
	}

protected:
	void _process() noexcept override;

private:
	std::vector<uint8_t> _head;
	std::vector<uint8_t> _tail;
	bool _need_more;

private:
	void _find_head() noexcept;
	void _find_tail() noexcept;
};

template<size_t N>
inline 
void 
head_tail_stream<N>::_process() noexcept
{
	std::lock_guard<std::mutex> locker(base_type::_mutex);

	while (!base_type::_unmatched.empty())
	{
		switch (base_type::_state)
		{
		case MatchState::ToFindHead:
			_find_head();
			break;

		case MatchState::ToFindTail:
			_find_tail();
			break;
		}
		if (_need_more) break;
	}
}

template<size_t N>
inline
void 
head_tail_stream<N>::_find_head() noexcept
{
	_need_more = false;
	if (base_type::_unmatched.size() < _head.size())
	{
		_need_more = true;
		return;
	}

	if (equal(_head.cbegin(), _head.cend(), base_type::_unmatched.cbegin())) // find head
	{
		std::back_insert_iterator<std::vector<uint8_t>> bi(base_type::_matched);
		copy(_head.cbegin(), _head.cend(), bi);
		for (int i = 0; i < _head.size(); ++i)
		{
			base_type::_unmatched.pop_front();
		}
		base_type::_state = MatchState::ToFindTail;
	}
	else
	{
	 	base_type::_unmatched.pop_front();
	}
}

template<size_t N>
inline
void 
head_tail_stream<N>::_find_tail() noexcept
{
	_need_more = false;
	if (base_type::_unmatched.size() < _tail.size())
	{
		_need_more = true;
		return;
	}

	if (equal(_tail.cbegin(), _tail.cend(), base_type::_unmatched.cbegin())) // find tail
	{
		std::back_insert_iterator<std::vector<uint8_t>> bi(base_type::_matched);
		copy(_tail.cbegin(), _tail.cend(), bi);
		for (int i = 0; i < _tail.size(); ++i)
		{
			base_type::_unmatched.pop_front();
		}
		base_type::_matched_signal(base_type::_matched);
		base_type::_matched.clear();
	 	base_type::_state = MatchState::ToFindHead;
	}
	else
	{
		if (base_type::_unmatched.size() >= _head.size()) // check if new head occur
		{
			if (equal(_head.cbegin(), _head.cend(), base_type::_unmatched.cbegin())) // find head
			{
				base_type::_matched.clear();
				std::back_insert_iterator<std::vector<uint8_t>> bi(base_type::_matched);
				copy(_head.cbegin(), _head.cend(), bi);
				for (int i = 0; i < _head.size(); ++i)
				{
				 	base_type::_unmatched.pop_front();
				}
				base_type::_state = MatchState::ToFindTail;
				return;
			}
		}

		if (base_type::_matched.size() > N) // if error
		{
			base_type::_matched.clear();
			base_type::_state = MatchState::ToFindHead;
		}
		else
		{
			base_type::_matched.push_back(base_type::_unmatched.front());
			base_type::_unmatched.pop_front();
		}
	}
}

_GURU_END

#endif /* _GURU_HEAD_TAIL_STREAM_H_ */