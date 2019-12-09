#pragma once
#include <iostream>

void print()
{
}

template <typename T, typename... Types>
void print(T firstArg, Types... args)
{
	std::cout << firstArg << '\n';
	print(args...);
}

template <typename T>
void print2(T arg)
{
	std::cout << arg << "\n";
}

template <typename T, typename... Types>
void print2(T firstArg, Types... args)
{
	print2(firstArg);
	print2(args...);
}

template <typename T, typename... Types>
void print3(T firstArg, Types... args)
{
	std::cout << sizeof...(Types) << '\n';
	std::cout << sizeof...(args) << '\n';
}

template <typename T, typename... Types>
void print4(T firstArg, Types... args)
{
	std::cout << firstArg << '\n';
	if (sizeof...(args) > 0)
	{
		print(args...);
	}
}

// Fold Expression
// C++ 17
// VS2017 not support
// (... op pack) -> (((pack1 op pack2) op pack3) ... op packN)
// (pack op ...) -> (pack1 op (...(packN-1 op packN)))
// (init op ... op pack) -> (((init op pack1) op pack2) ... op packN)
// (pack op ... op init) -> (pack1 op (...(packN op init)))
template <typename... T>
auto foldSum(T... s)
{
	return (... + s); // ((s1+s2)+s3)...
}

template <typename... T>
void printDouble(T const&... args)
{
	print(args + args...);
}

template <typename... T>
void addOne(T const&... args)
{
	print((args + 1)...);
}

template <typename C, typename... Idx>
void printElems(C const& coll, Idx... idx)
{
	print(coll[idx]...);
}

template <std::size_t... Idx, typename C>
void printIdx(C const& coll)
{
	print(coll[Idx]...);
}

// type for arbitrary number of indices:
template <std::size_t...>
struct Indices
{
};

template <typename T, std::size_t... Idx>
void printByIdx(T t, Indices<Idx...>)
{
	print(std::get<Idx>(t)...);
}
