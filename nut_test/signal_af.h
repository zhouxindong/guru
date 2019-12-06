#pragma once
#include <iostream>
#include <vector>
#include <numeric>
#include "nut/signal.h"

inline
void print_sum(int x, int y)
{
	std::cout << "sum(" << x << ", " << y << ") = " << (x + y) << "\n";
}

inline
void print_mul(int x, int y)
{
	std::cout << "mul(" << x << ", " << y << ") = " << (x * y) << "\n";
}

inline
int pow2(int x)
{
	return x * x;
}

inline
int pow3(int x)
{
	return x * x * x;
}

inline
int sum_agg(const std::vector<int>& v)
{
	return std::accumulate(v.cbegin(), v.cend(), 0);
}

inline
void bar()
{
	std::cout << "function: bar\n";
}

struct baz
{
	void operator() ()
	{
		std::cout << "functor: baz\n";
	}
};

struct qux
{
	void print()
	{
		std::cout << "class member: qux\n";
	}
};

struct demo_slot
	: public guru::slot
{
	int value;

	demo_slot(int v) : value(v)
	{
	}
};