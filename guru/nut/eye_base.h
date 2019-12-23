#pragma once
#ifndef _GURU_EYE_BASE_H
#define _GURU_EYE_BASE_H

#include "../gvals.h"
#include "signal.h"
#include <string>

_GURU_BEGIN

class eye_base
{
public:
	virtual ~eye_base() noexcept
	{
		_signal.disconnect_all();
	}

	virtual void operator()() noexcept = 0;

	signal<void(std::string)>& sig() noexcept
	{
		return _signal;
	}

protected:
	signal<void(std::string)> _signal;
};

_GURU_END

#endif /* _GURU_EYE_BASE_H */