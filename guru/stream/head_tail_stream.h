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
	void _process() override;

private:
	std::vector<uint8_t> _head;
	std::vector<uint8_t> _tail;
	bool _need_more;

private:
	void _find_head();
	void _find_tail();
};

template<size_t N>
inline void head_tail_stream<N>::init_trait(std::initializer_list<uint8_t> l1, std::initializer_list<uint8_t> l2)
{
	_head = l1;
	_tail = l2;

	trait_stream<N>::init_trait(l1, l2);
}

template<size_t N>
inline head_tail_stream<N>::head_tail_stream(std::initializer_list<uint8_t> head, std::initializer_list<uint8_t> tail)
{
}

template<size_t N>
inline void head_tail_stream<N>::_process()
{
	trait_stream<N>::_process();

	std::lock_guard<std::mutex> locker(_mutex);

	while (!_unmatched.empty())
	{
		switch (_state)
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
inline void head_tail_stream<N>::_find_head()
{
	_need_more = false;
	if (_unmatched.size() < _head.size())
	{
		_need_more = true;
		return;
	}

	if (equal(_head.cbegin(), _head.cend(), _unmatched.cbegin())) // find head
	{
		std::back_insert_iterator<std::vector<uint8_t>> bi(_matched);
		copy(_head.cbegin(), _head.cend(), bi);
		for (int i = 0; i < _head.size(); ++i)
		{
			_unmatched.pop_front();
		}
		_state = MatchState::ToFindTail;
	}
	else
	{
		_unmatched.pop_front();
	}
}

template<size_t N>
inline void head_tail_stream<N>::_find_tail()
{
	_need_more = false;
	if (_unmatched.size() < _tail.size())
	{
		_need_more = true;
		return;
	}

	if (equal(_tail.cbegin(), _tail.cend(), _unmatched.cbegin())) // find tail
	{
		std::back_insert_iterator<std::vector<uint8_t>> bi(_matched);
		copy(_tail.cbegin(), _tail.cend(), bi);
		for (int i = 0; i < _tail.size(); ++i)
		{
			_unmatched.pop_front();
		}
		_matched_signal(_matched);
		_matched.clear();
		_state = MatchState::ToFindHead;
	}
	else
	{
		if (_unmatched.size() >= _head.size()) // check if new head occur
		{
			if (equal(_head.cbegin(), _head.cend(), _unmatched.cbegin())) // find head
			{
				_matched.clear();
				std::back_insert_iterator<std::vector<uint8_t>> bi(_matched);
				copy(_head.cbegin(), _head.cend(), bi);
				for (int i = 0; i < _head.size(); ++i)
				{
					_unmatched.pop_front();
				}
				_state = MatchState::ToFindTail;
				return;
			}
		}

		if (_matched.size() > N) // if error
		{
			_matched.clear();
			_state = MatchState::ToFindHead;
		}
		else
		{
			_matched.push_back(_unmatched.front());
			_unmatched.pop_front();
		}
	}
}

_GURU_END

#endif /* _GURU_HEAD_TAIL_STREAM_H_ */