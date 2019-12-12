#pragma once

#include <type_traits>
#include <iostream>

// 1. expression yields true, type -> second template argument, default is void
// 2. expression yields false, function template definition ignored
template <typename T>
typename std::enable_if<(sizeof(T) > 4)>::type
foo()
{
	std::cout << "foo()\n";
}

// common way to use
template <
	typename T,
	typename = std::enable_if_t<(sizeof(T) > 4)>>
	void foo()
{
}

// demo
template <
	typename STR,
	typename = std::enable_if_t<
	std::is_convertible_v<STR, std::string>>>
	Person(STR&& n) // only used for STR can converted to std::string
{
}

class thread2
{
public:
	template <typename F, typename... Args,
		typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, thread2>>>
		explicit thread2(F&& f, Args&&... args) {} // disable template copy ctor
};
