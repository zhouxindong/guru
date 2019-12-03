#pragma once
#ifndef _GURU_LOOP_H_
#define _GURU_LOOP_H_

#include "../gvals.h"

_GURU_BEGIN

template <size_t DIM, typename _Tx>
class dot_product
{
public:
	static _Tx value(_Tx* a, _Tx* b)
	{
		return *a * * b + dot_product<DIM - 1, _Tx>::value(a + 1, b + 1);
	}
};

template <typename _Tx>
class dot_product<1, _Tx>
{
public:
	static _Tx value(_Tx* a, _Tx* b)
	{
		return *a * *b;
	}
};

_GURU_END

#endif /* _GURU_LOOP_H_ */