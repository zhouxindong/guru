#pragma once
#include <algorithm>

template <class T>
T* NewCopy(const T* src, size_t srcsize, size_t destsize)
{
	assert(destsize >= srcsize);
	T* dest = new T[destsize];
	try
	{
		copy(src, src + srcsize, dest);
	}
	catch (...)
	{
		delete[] dest;
		throw;
	}
	return dest;
}



template <class T> 
class Stack
{
public:
	Stack() noexcept;
	~Stack() noexcept;
	Stack(const Stack<T>&);
	Stack<T>& operator = (const Stack<T>&);

public:
	size_t Count() const;
	void Push(const T&);
	T& Top();
	T Pop();

private:
	T* v_;
	size_t vsize_;
	size_t vused_;
};

template<class T>
inline Stack<T>::Stack() noexcept
	: v_{ new T[10] }, vsize_{ 10 }, vused_{ 0 }
{
}

template<class T>
inline Stack<T>::~Stack() noexcept
{
	delete[] v_;
}

template<class T>
inline Stack<T>::Stack(const Stack<T> & other)
	:v_{ NewCopy(other.v_, other.vsize_, other.vsize_) },
	vsize_{ other.vsize_ },
	vused_{ other.vused_ }
{
}

template<class T>
inline Stack<T>& Stack<T>::operator=(const Stack<T>& other)
{
	if (this != &other)
	{
		T* v_new = NewCopy(other.v_, other.vsize_, other.vsize_);
		delete[] v_;
		v_ = v_new;
		vsize_ = other.vsize_;
		vused_ = other.vused_;
	}
	return *this;
}

template<class T>
inline size_t Stack<T>::Count() const
{
	return vused_;
}

template<class T>
inline void Stack<T>::Push(const T &)
{
	if (vused_ == vsize_)
	{
		size_t vsize_new = vsize_ * 2 + 1;
		T* v_new = NewCopy(v_, vsize_, vsize_new);
		delete[] v_;
		v_ = v_new;
		vsize_ = vsize_new;
	}

	v_[vused_] = t;
	++vused_;
}

template<class T>
inline T & Stack<T>::Top()
{
	if (vused_ == 0)
	{
		throw "empty stack";
	}
	return v_[vused_ - 1];
}

template<class T>
inline T Stack<T>::Pop()
{
	if (vused_ == 0)
	{
		throw "pop from empty stack";
	}
	else
	{
		--vused_;y
	}
}
