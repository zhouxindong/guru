#pragma once
#ifndef _GURU_SQRT_H_
#define _GURU_SQRT_H_

#include "../gvals.h"
#include <type_traits>

_GURU_BEGIN

template <size_t N, size_t Lo = 0, size_t Hi = N>
class sqrt
{
public:
	enum { mid = (Lo + Hi + 1) / 2 };
	enum { value = (N < mid*mid) ? sqrt<N, Lo, mid - 1>::value : sqrt<N, mid, Hi>::value };
};

template <size_t N, size_t M>
class sqrt<N, M, M>
{
public:
	enum { value = M };
};

template <size_t N, size_t Lo = 0, size_t Hi = N>
class sqrt2
{
public:
	enum { mid = (Lo + Hi + 1) / 2 };

	typedef typename std::conditional<(N < mid*mid), sqrt2<N, Lo, mid - 1>, sqrt2<N, mid, Hi>>::type SubT;
	enum { value = SubT::value };
};

template <size_t N, size_t S>
class sqrt2<N, S, S>
{
public:
	enum { value = S };
};

template <size_t N, size_t I = 0>
class sqrt3
{
public:
	typedef typename std::conditional<(I*I < N), sqrt3<N, I + 1>,
		std::integral_constant<size_t, I>>::type SubT;
	enum { value = SubT::value };
};
_GURU_END

#endif /* _GURU_SQRT_H_ */