#pragma once
#include <iostream>
#include <tuple>

void print()
{
}

template <typename T, typename... Types>
void print(T firstArg, Types... args)  
{	// Types... is a pack expansion
	// that produces a sequence of template arguments
	// within the argument pack subsituted for Types
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


template <typename... Ts>
struct ArgList
{
	template<Ts... vals>
	struct Vals
	{
	};
};

static ArgList<int, char, char>::Vals<3, 'x', 'y'> tada;

template <typename... Types>
class PtrTuple : public std::tuple<Types*...> // pack expansion Types*... for each type in pack
{
};
PtrTuple<int, float> t3; // inherits from tuple<int*, float*>

						 // pack expansions
						 // 1. in the list of base classes
template <typename... Mixins>
class Point : public Mixins... // base class pack expansion
{
	double x, y, z;
public:
	Point() : Mixins()... {} // base class initializer pack expansion

	Point(Mixins... mixins) // mixins is a function parameter pack
		: Mixins(mixins)... // initialize each base with the supplied mixin value
	{
	}

	template <typename Visitor>
	void visitMixins(Visitor visitor)
	{
		visitor(static_cast<Mixins&>(*this)...); // call argument pack expansion
	}
};

struct Color
{
	char red, green, blue;
};

struct Label
{
	std::string name;
};

Point<Color, Label> p; // inherits from both Color and Label(default ctor)
Point<Color, Label> p({ 0x7F, 0, 0x7F }, { "center" });


// 2. to create a nontype or template parameter pack
template <typename... Ts>
struct Values
{
	template <Ts... Vs>
	struct Holder
	{
	};
};

int i;
Values<char, int, int*>::Holder<'a', 17, &i> valueHolder;

template <typename F, typename... Types>
void forwardCopy(F f, Types const&... values)
{
	f(Types(values)...); // two parameters packs, Types and values
}

// fold expression
// a right fold of fn over a sequence x[1], x[2]... x[n]
// fn(x[1], fn(x[2], fn(..., fn(x[n-1], x[n])...)))

bool and_all()
{
	return true;
}

template <typename T>
bool and_all(T cond)
{
	return cond;
}

template <typename T, typename... Ts>
bool and_all(T cond, Ts... conds)
{
	return cond && and_all(conds...);
}

// fold expressions applies to all binary operators
// right fold: (pack op ... op value)
// left fold:  (value op ... op pack)