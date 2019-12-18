#pragma once
#include <utility>
#include <functional>

template <typename Iter, typename Callable>
void foreach(Iter current, Iter end, Callable op)
{
	while (current != end)
	{
		op(*current);
		++current;
	}
}

// func: decays to pointers
// &func: pointers
// functor: usually as a constant member function
// lambda

// C++17
template <typename Iter, typename Callable, typename ...Args>
void foreach(Iter current, Iter end, Callable op, Args const& ...args)
{
	while (current != end)
	{
		std::invoke(op, args..., *current);
		++current;
	}
}

// a common application of std::invoke() is to wrap single function calls