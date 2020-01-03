#pragma once
#include <iostream>

// perfect forwarding
// 1. modifyable objects should be forwarded so that they still can be modified
// 2. constant objects should be forwarded as read-only objects
// 3. movable objects should be forwarded as movable objects
// only applied 1. function template, 2. T&&

class X {};
void g(X&)	// 1.
{
	std::cout << "g() for variable\n";
}

void g(X const&)	// 2.
{
	std::cout << "g() for constant\n";
}

void g(X&&)	// 3.
{
	std::cout << "g() for movable object\n";
}

// let f() forward argument val to g()
void f(X& val)
{
	g(val);
}

void f(X const& val)
{
	g(val);
}

void f(X&& val)
{
	g(std::move(val)); // move semantics is not passed through automatically(without move(), X&)
}

// perfect forwarding
template <typename T>
void f(T&& val) // T&& behaves is diff X&&(X&& means rvalue ref, a temp obj or move())
{				// T&& means universal ref, can be bound a mutable, immutable, or movable object
	g(std::forward<T>(val));
}

// perfect forwarding temporaries
template <typename T>
void foo(T x)
{
	auto&& val = get(x);
	//
	//...
	//foo2(std::forward<decltype(val)>(val);

}

// deduce in forwarding reference
// 1. argument is an lvalue, the type deduction is an lvalue references to argument type
// 2. otherwise, is simply the argument type(not a reference type)
template <typename T>
void f(T&& p);	// p is a forwarding reference

void g()
{
	int i;
	int const j = 0;
	f(i);	// i is a lvalue, T = int&, p = int&
	f(j);	// j is a lvalue, T = int const&, p = int const&
	f(2);	// 2 is a rvalue, T = int, p = int&&
}

template <typename T>
void forwardToG(T&& x)
{
	// static_cast<T&&> casts x to its original type and lvalue or rvalue
	// g(std::forward<T>(x)); // same as below
	g(static_cast<T&&>(x));	// forward x to g()
}