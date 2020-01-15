#pragma once
#include <iostream>

template <typename T>
class Outer
{
public:
	template <typename U>
	class Inner
	{
	private:
		static int count;
	};

	static int code;
	void print() const {
		std::cout << "generic";
	}
};

template <typename T>
int Outer<T>::code = 6;

template <typename T>
template <typename U>
int Outer<T>::Inner<U>::count = 7;

template<>
class Outer<bool>
{
public:
	template <typename U>
	class Inner {
	private:
		static int count;
	};
	void print() const {

	}
};

template<>
int Outer<void>::code = 12;

template<>
void Outer<void>::print() const
{
	std::cout << "Outer<void>";
}

// EnableIf
template <typename Key, typename Value, typename = void>
class Directory
{
	// vector implementation here
};

template <typename Key, typename Value>
class Directory<Key, Value, EnableIf<HasLess<Key>>>
{
	// parial specializate
	// map implementation here
};
