#pragma once
template <typename T, T nontypeParam>
class C;

C<int, 33>* c1;

int a;
C<int*, &a>* c2;

void f();
void f(int);
C<void(*)(int), f>* c3;

template <typename T> 
void templ_func();
C<void(), &templ_func<double>>* c4;

struct X
{
	static bool b;
	int n;
	constexpr operator int() const
	{
		return 42;
	}
};
C<bool&, X::b>* c5;
C<int X::*, &X::n>* c6;
C<long, X{} > * c7;