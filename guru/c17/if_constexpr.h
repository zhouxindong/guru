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