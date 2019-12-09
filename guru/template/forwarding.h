#pragma once
#include <iostream>

// perfect forwarding
// 1. modifyable objects should be forwarded so that they still can be modified
// 2. constant objects should be forwarded as read-only objects
// 3. movable objects should be forwarded as movable objects

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