#pragma once

// for raw array or string literal
// 1. reference argument don't decay "hello" -> char const[6], this cause diff len means diff types
// 2. value argument lead decay "hello" -> char const*

template <typename T, int N, int M>
bool less2(T(&a)[N], T(&b)[M])
{
	for (int i = 0; i < N && i < M; ++i)
	{
		if (a[i] < b[i]) return true;
		if (b[i] < a[i]) return true;
	}
	return N < M;
}

template <typename T>
struct MyClass;		// primary template

template <typename T, std::size_t SZ>
struct MyClass<T[SZ]>	// value argument known bounds
{
	static void print()
	{
		std::cout << "print() for T[" << SZ << "]\n";
	}
};

template <typename T, std::size_t SZ>
struct MyClass<T(&)[SZ]>	// references argument
{
	static void print()
	{
		std::cout << "print() for T(&)[" << SZ << "]\n";
	}
};

template <typename T>
struct MyClass<T[]>	// value argument unknown bounds
{
	static void print()
	{
		std::cout << "print() for T[]\n";
	}
};

template <typename T>
struct MyClass<T(&)[]>	// references argument
{
	static void print()
	{
		std::cout << "print() for T(&)[]\n";
	}
};

template <typename T>
struct MyClass<T*>
{
	static void print()
	{
		std::cout << "print() for T*\n";
	}
};

template <typename T1, typename T2, typename T3>
void foo(
	int a1[7], int a2[],	// pointers by language rules
	int(&a3)[42],			// reference to array of known bound
	int(&x0)[],				// reference to array of unknown bound
	T1 x1,					// passing by value decays
	T2& x2, T3&& x3)		// passing by reference
{
	MyClass<decltype(a1)>::print();		// MyClass<T*>
	MyClass<decltype(a2)>::print();		// MyClass<T*>
	MyClass<decltype(a3)>::print();		// MyClass<T(&)[SZ]>
	MyClass<decltype(x0)>::print();		// MyClass<T(&)[]>
	MyClass<decltype(x1)>::print();		// MyClass<T*>
	MyClass<decltype(x2)>::print();		// MyClass<T(&)[]>
	MyClass<decltype(x3)>::print();		// MyClass<T(&)[]>
}

