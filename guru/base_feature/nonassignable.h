#pragma once
#ifndef _GURU_NONASSIGNABLE_H_
#define _GURU_NONASSIGNABLE_H_

#include "../gvals.h"

_GURU_BEGIN

class nonassignable
{
protected:
	nonassignable() = default;
	nonassignable& operator = (const nonassignable&) = delete;
	nonassignable& operator = (nonassignable&&) = delete;
};

_GURU_END

#endif /* _GURU_NONASSIGNABLE_H_ */