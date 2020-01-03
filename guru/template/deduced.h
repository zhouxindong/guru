#pragma once
template <typename T>
void f(T) {}
template <typename T>
void g(T&) {}

double arr[20];
int const seven = 8;

f(arr);		// T = double*
g(arr);		// T = double[20]
f(seven);	// T = int
g(seven);	// T = int const
f(8);		// T = int
g(8);		// error!

template <typename T>
void f1(T*);

template <typename E, int N>
void f2(E(&)[N]);

template <typename T1, typename T2, typename T3>
void f3(T1(T2::*)(T3*));

class S
{
public:
	void f(double*);
};

void g(int*** ppp)
{
	bool b[42];
	f1(ppp);	// T = int**
	f2(b);		// E = bool, N = 42;
	f3(&S::f);	// T1 = void, T2 = S, T3 = double
}

// complex type declarations are built from more elementary constructs:
// pointer, reference, array, function
// pointer-to-member, template-id