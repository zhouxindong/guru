#pragma once

// decltype: allows to express the precise type of an expression or declaration
// decltype procude: depending on whether the passed argument is a declared entity or an expression
// 1. if e is the name of an entity, decltype(e) yields the declared type of that entity
// 2. if e is any other expression, decltype(e) produces a type that reflects the type and value category of that expression
// 2.1 if e is an lvalue of type T, decltype(e) -> T&
// 2.2 if e is an xvalue of type T, decltype(e) -> T&& (将亡值)
// 2.3 if e is an prvalue of type T, decltype(e) -> T (中间值)

#include <string>
#include <vector>

void g(std::string&& s)
{
	// 1. check the type of s
	std::is_lvalue_reference<decltype(s)>::value;		// false
	std::is_rvalue_reference<decltype(s)>::value;		// true
	std::is_same<decltype(s), std::string&>::value;		// false
	std::is_same<decltype(s), std::string&&>::value;	// true

	// 2. check the value category of s used as expression
	std::is_lvalue_reference<decltype((s))>::value;		// true
	std::is_rvalue_reference<decltype((s))>::value;		// false
	std::is_same<decltype((s)), std::string&>::value;	// true
	std::is_same<decltype((s)), std::string&&>::value;	// false
}

template <typename Iterator>
void f()
{
	Iterator pos;
	auto element = *pos;	// copy of the element
	auto& element = *pos;	// failed if operator* return a value
	decltype(*pos) element = *pos;	// both avalibel
}

int i = 42;
int const& ref = i;
auto x = ref;	// x -> int, copy the value
decltype(auto) y = ref; // y -> int const&, refers to i;

std::vector<int> v = { 42 };
auto x = v[0];	// x denotes a new object of type int
decltype(auto) y = v[0]; // y is a reference int&
