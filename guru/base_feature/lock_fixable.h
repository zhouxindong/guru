#pragma once
#ifndef _GURU_LOCK_FIXABLE_H_
#define _GURU_LOCK_FIXABLE_H_

#include "../gvals.h"
#include <mutex>

_GURU_BEGIN

/*
** intrusive pre|suffix
** lock operation
*/
struct lock_fixable
{
	void prefix() noexcept
	{
		_mtx.lock();
		_locked = true;
	}

	void suffix() noexcept
	{
		_mtx.unlock();
		_locked = false;
	}

	~lock_fixable() noexcept
	{
		if (_locked)
		{
			_mtx.unlock();
		}
	}
private:
	std::mutex _mtx;
	bool _locked{ false };
};

_GURU_END

#endif /* _GURU_LOCK_FIXABLE_H_ */