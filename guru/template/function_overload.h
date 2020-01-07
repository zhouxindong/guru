#pragma once
#include <utility>

class C {
public:
	C() = default;
	C(C const&)	// #1
	{
	}
	C(C&&) {}	// #2
	template <typename T>
	C(T&&) {}	// #3
};

void foo()
{
	C x;
	C x2{ x }; // #3
	C x3{ std::move(x) };	// #2
	C const c;
	C x4{ c };	// #1
	C x5{ std::move(c) };	// #3
}