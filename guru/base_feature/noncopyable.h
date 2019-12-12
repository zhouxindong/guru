#pragma once
#ifndef _GURU_NONCOPYABLE_H_
#define _GURU_NONCOPYABLE_H_

#include "../gvals.h"

_GURU_BEGIN

class noncopyable
{
protected:
	noncopyable() = default;
	noncopyable(const noncopyable&) = delete;
	noncopyable(noncopyable&&) = delete;
};

_GURU_END

#endif /* _GURU_NONCOPYABLE_H_ */