#pragma once
#include <iostream>
#include <typeinfo>
#include <type_traits>

template <typename T>
void f(T)
// pass by value lead to decay
// array types into pointers
// function types into pointer-to-function types
// delete any top-level const, volatile, reference qualifier
{
}

template <typename A>
void printParameterType(void(*)(A))
{
	std::cout << "Parameter type: " << typeid(A).name() << '\n';
	std::cout << "- is int: " << std::is_same<A, int>::value << '\n';
	std::cout << "- is const: " << std::is_const<A>::value << '\n';
	std::cout << "- is pointer: " << std::is_pointer<A>::value << '\n';
}

void test()
{
	//Parameter type : int
	//	- is int : 1
	//	- is const : 0
	//	- is pointer : 0
	printParameterType(&f<int>);

	//Parameter type : int
	//	- is int : 1
	//	- is const : 0 
	//	- is pointer : 0
	printParameterType(&f<int const>);

	//Parameter type : int * __ptr64
	//	- is int : 0
	//	- is const : 0
	//	- is pointer : 1
	printParameterType(&f<int[7]>);

	//Parameter type : int(__cdecl*)(int)
	//	- is int : 0
	//	- is const : 0
	//	- is pointer : 1
	printParameterType(&f<int(int)>);
}

// argument passed by value can decay type,
// also can implement a trait just same as value passed
#include  "type_traits.h"

template <typename T>
struct DecayT : RemoveCVT<T>
{
};

template <typename T>
struct DecayT<T&>
{
	using Type = T;
};

template <typename T>
struct DecayT<T[]>
{
	using Type = T*;
};

template <typename T, std::size_t N>
struct DecayT<T[N]>
{
	using Type = T*;
};

template <typename R, typename... Args>
struct DecayT<R(Args...)>
{
	using Type = R(*)(Args...);
};

template <typename R, typename... Args>
struct DecayT<R(Args..., ...)>
{
	using Type = R(*)(Args..., ...);
};