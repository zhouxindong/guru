#pragma once
#ifndef _GURU_SINGLE_PROC_H_
#define _GURU_SINGLE_PROC_H_

#include "../gvals.h"
#include <typeinfo>

_GURU_BEGIN

/*
** only one process can be run
*/
template <typename _Tx>
class single_proc
{
public:
	single_proc() noexcept
	{
		_mutex = ::CreateMutexA(NULL, NULL, typeid(_Tx).name());
		_owned = ::GetLastError() != ERROR_ALREADY_EXISTS;
	}

	~single_proc() noexcept
	{
		if (_mutex)
			::ReleaseMutex(_mutex);
	}

public:
	bool const owned() const noexcept
	{
		return _owned;
	}

private:
	HANDLE _mutex;
	bool _owned{ false };
};

_GURU_END


#endif /* _GURU_SINGLE_PROC_H_ */