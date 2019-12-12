#pragma once
#ifndef _GURU_TRANSPARENT_STREAM_H_
#define _GURU_TRANSPARENT_STREAM_H_

#include "../gvals.h"

_GURU_BEGIN

/*
** transparent data stream
*/
template <size_t N = 65535>
class transparent_stream : public trait_stream<N>
{
public:
	typedef transparent_stream<N> self_class;
	typedef transparent_stream<N> type;
	typedef trait_stream<N> base_class;
	typedef trait_stream<N> base_type;

protected:
	void 
		_process() noexcept override
	{
		std::lock_guard<std::mutex> locker(base_type::_mutex);

		while (!base_type::_unmatched.empty())
		{
			base_type::_matched.push_back(base_type::_unmatched.front());
			base_type::_unmatched.pop_front();
		}
		base_type::_matched_signal(base_type::_matched);
		base_type::_matched.clear();
	}
};

_GURU_END

#endif /* _GURU_TRANSPARENT_STREAM_H_ */