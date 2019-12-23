#pragma once
#ifndef _GURU_THREAD_GUARD_H_
#define _GURU_THREAD_GUARD_H_

#include "../gvals.h"
#include <thread>

_GURU_BEGIN

class thread_guard
{
public:
	thread_guard(std::thread& t) noexcept :
		_thread{ t }
	{
	}

	~thread_guard() noexcept
	{
		if (_thread.joinable())
			_thread.join();
	}

private:
	std::thread& _thread;
};

_GURU_END

#endif /* _GURU_THREAD_GUARD_H_ */