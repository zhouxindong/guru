#pragma once

// a variable of type auto will never be a reference type, just like 
// auto reference: auto&
// rvalue reference: auto&&, but behave like a forwarding reference
int x;
auto&& rr = 42;	// ok, rvalue reference binds to an rvalue
// to bind the result of a function or operator invocation whose value is lvalue or rvalue
// without having to make a copy of that result
auto&& lr = x;	// ok, auto = int&, lr is a lvalue reference

template <typename Container> 
void g(Container c)
{
	for (auto&& x : c)
	{

	}
}
