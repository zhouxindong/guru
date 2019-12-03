#pragma once
#ifndef _GURU_POW_H_
#define _GURU_POW_H_

#include "../gvals.h"

_GURU_BEGIN

template <int M, size_t N>
class pow
{
public:
	enum { value = M*pow<M, N - 1>::value };
};

template <int M>
class pow<M, 0>
{
public:
	enum { value = 1 };
};

_GURU_END

#endif /* _GURU_POW_H_ */