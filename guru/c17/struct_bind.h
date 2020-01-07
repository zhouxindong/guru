#pragma once

#include <tuple>
#include <utility>

struct MaybeInt 
{
	bool valid;
	int value;
};

MaybeInt g() {
	return MaybeInt{ false, 100 };
}

void fooo()
{
	// 1. simple data class
	auto const&& [b, n] = g();

	// 2. arrays
	double pt[3];
	auto&[x, y, z] = pt;
	x = 3.0; y = 4.0; z = 0.0;

	// 3. tuple-like, simply get<>()
	std::tuple<bool, int> bi{ true, 42 };
	auto[b, i] = bi;
	int r = i;
}

enum M {};
template<> class std::tuple_size<M>
{
public:
	static unsigned const value = 2;	// map M to a pair of values
};

template<> class std::tuple_element<0, M>
{
public:
	using type = int;
};

template<> class std::tuple_element<1, M>
{
public:
	using type = double;
};

template<int> auto get(M);
template<> auto get<0>(M) { return 42; }
template<> auto get<1>(M) { return 7.0; }

auto[i, d] = M(); // as if: int&& i = 42, double&& d = 7.0
