#pragma once
// 1. nondependent base class
template <typename X>
class Base
{
public:
	int basefield;
	using T = int;
};

class D1 : public Base<Base<void>>
{	// not a template case really
public:
	void f()
	{
		basefield = 3;	// usual access to inherited member
	}
};

template <typename T>
class D2 : public Base<double>	// nondependent base
{
public:
	void f()
	{
		basefield = 7;
	}
	T strange;	// T is Base<double>::T, not the template parameter T! 
};

template <typename T>
class DD : public Base<T>
{
public:
	void f()
	{
		basefield = 0;
	}
};

template <>
class Base<bool>
{
public:
	enum { basefield = 42 };
};

void g(DD<bool>& d)
{
	d.f();
}

template <typename T>
class DD1 : public Base<T>
{
public:
	void f()
	{
		this->basefield = 0;
	}
};

template <typename T>
class DD2 : public Base<T>
{
public:
	void f()
	{
		Base<T>::basefield = 0;
	}
};