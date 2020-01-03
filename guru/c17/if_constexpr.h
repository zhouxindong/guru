#pragma once

#include <iostream>
#include <type_traits>

template <typename T, typename... Types>
void print5(T const& firstArg, Types const&... args)
{
	std::cout << firstArg << '\n';
	if constexpr(sizeof...(args) > 0)
	{
		print5(args...);
	}
}

template <typename T>
void foo(T t)
{
	if constexpr(std::is_integral_v<T>)
	{
		if (t > 0)
		{
			foo(t - 1);
		}
	}
	else
	{
		undeclared(t);
		undeclared();
		static_assert(false, "no integral");
		static_assert(!std::is_integral_v<T>, "no integral");
	}
}

template <typename T>
bool f(T p)
{
	if constexpr (sizeof(T) <= sizeof(long long))
	{
		return p > 0;
	}
	else
	{
		return p.compare(0) > 0;
	}
}

bool g(int n) {
	return f(n); // only if statement instantiated
}

// in C++14
template <bool b, typename T>
struct Dispatch
{
	static bool f(T p)
	{
		return p.compare(0) > 0;
	}
};

template <typename T>
struct Dispatch<true>
{
	static bool f(T p)
	{
		return p > 0;
	}
};

template <typename T>
bool f(T p)
{
	return Dispatch<sizeof(T) <= sizeof(long long), T>::f(p);
}

template <typename Head, typename... Remainder>
void f(Head&& h, Remainder&&... r)
{
	doSomething(std::forward<Head>(h));
	if constexpr (sizeof...(r) != 0)
	{
		f(std::forward<Remainder>(r)...);
	}
}