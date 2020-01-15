#pragma once
#include <string>

// Mixins 提供了一个可选方案来定制化类型的某些行为，而无需从该类型继承
// 一个支持Mixins的类模板通常接受任意数量的附加类作为其基类
template <typename... Mixins>
class Point : public Mixins...
{
public:
	double x, y;
	Point() : Mixins()..., x(0.0), y(0.0){}
	Point(double x, double y) : Mixins()..., x(x), y(y){}
};

// mix in a base class containing a label
class Label
{
public:
	std::string label;
	Label() : label("") {}
};

using LabeledPoint = Point<Label>;

// more mixins
class Color
{
public:
	unsigned char red = 0, green = 0, blue = 0;
};

using MyPoint = Point<Label, Color>;

// Parameterized Virtuality
class NotVirtual {
};

class Virtual {
public:
	virtual void foo() {

	}
};

template <typename... Mixins>
class Base : public Mixins...
{
public:
	// the virtuality of foo() depends on its declaration
	// if(any) in the base classes Mixins...
	void foo() {
		std::cout << "Base::foo()" << '\n';
	}
};

template <typename... Mixins>
class Derived : public Base<Mixins...>
{
public:
	void foo() {
		std::cout << "Derived::foo()" << '\n';
	}
};

void foooo()
{
	Base<NotVirtual>* p1 = new Derived<NotVirtual>();
	p1->foo(); // Base::foo()

	Base<Virtual>* p2 = new Derived<Virtual>();
	p2->foo(); // Derived::foo()
}