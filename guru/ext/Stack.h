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

// placement new in special memory
template <class T1, class T2>
void construct(T1* p, const T2& value)
{
	new (p) T1(value);
}

// placement new can't be call delete, must explict call dtor
template <class T>
void destroy(T* p)
{
	p->~T();
}

template <class FwdIter>
void destroy(FwdIter first, FwdIter last)
{
	while (first != last)
	{
		destroy(&*first);
		++first;
	}
}

template <class T>
void swap(T& a, T& b)
{
	T temp(a);
	a = b;
	b = temp;
}

#pragma region Stack

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
	T const& Top() const;
	T Pop();
	bool Empty() const;

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
inline T const & Stack<T>::Top() const
{
	if (vused_ == 0)
	{
		throw "empty stack";
	}
	else
	{
		return v_[vused_ - 1];
	}
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
		--vused_;
	}
}

template<class T>
inline bool Stack<T>::Empty() const
{
	return (vused_ == 0);
}

#pragma endregion

#pragma StackImpl

template <class T> 
class StackImpl
{
protected:
	StackImpl(size_t size = 0);
	~StackImpl();
	void Swap(StackImpl<T>& other) noexcept;

	T* v_;
	size_t vsize_;
	size_t vused_;

private:
	StackImpl(StackImpl<T> const&);
	StackImpl<T>& operator = (StackImpl<T> const&);
};

template <class T>
StackImpl<T>::StackImpl(size_t size /*= 0*/)
	: v_{ static_cast<T*>(size == 0 ? nullptr : operator new(sizeof(T)*size)) },
	vsize_{ size },
	vused_{ 0 }
{
}

template<class T>
inline StackImpl<T>::~StackImpl()
{
	destroy(v_, v_ + vused_);
	operator delete(v_);
}

template<class T>
inline void StackImpl<T>::Swap(StackImpl<T>& other) noexcept
{
	swap(v_, other.v_);
	swap(vsize_, other.vsize_);
	swap(vused_, other.vused_);
}

template <class T>
class Stack2 : private StackImpl<T>
{
public:
	Stack2(size_t size = 0);
	~Stack2();
	Stack2(const Stack2<T>&);
	Stack2<T>& operator = (const Stack2<T>&);
	size_t Count() const;
	void Push(const T&);
	T& Top();
	void Pop();
};

template<class T>
inline Stack2<T>::Stack2(size_t size)
	: StackImpl<T>{size}
{
}

template<class T>
inline Stack2<T>::~Stack2()
{
}

template<class T>
inline Stack2<T>::Stack2(const Stack2<T>& other)
	: StackImpl<T>(other.vused_)
{
	while (vused_ < other.vused_)
	{
		construct(v_ + vused_, other.v_[vused_]);
		++vused_;
	}
}

template<class T>
inline Stack2<T>& Stack2<T>::operator=(const Stack2<T>& other)
{
	Stack2<T> temp(other);	// temp take over old resource, and will be cleaned out scope
	Swap(temp);
	return *this;
}

template<class T>
inline size_t Stack2<T>::Count() const
{
	return vused_;
}

template<class T>
inline void Stack2<T>::Push(const T& t)
{
	if (vused_ == vsize_)
	{
		Stack2<T> temp(vsize_ * 2 + 1);
		while (temp.Count() < vused_)
		{
			temp.Push(v_[temp.Count()]);
		}
		temp.Push(t);
		Swap(temp);
	}
	else
	{
		construct(v_ + vused_, t);
		++vused_;
	}
}

template<class T>
inline T & Stack2<T>::Top()
{
	if (vused_ == 0)
	{
		throw "empty stack";
	}
	return v_[vused_ - 1];
}

template<class T>
inline void Stack2<T>::Pop()
{
	if (vused_ == 0)
	{
		throw "pop from empty stack";
	}
	else
	{
		--vused_;
		destroy(v_ + vused_);
	}
}

#pragma endregion

template <class T>
class StackImpl3
{
public:
	StackImpl3(size_t size = 0);
	~StackImpl3();
	void Swap(StackImpl3<T>& other) noexcept;

	T* v_;
	size_t vsize_;
	size_t vused_;

private:
	StackImpl3(StackImpl3<T> const&);
	StackImpl3<T>& operator = (StackImpl3<T> const&);
};

template <class T>
StackImpl3<T>::StackImpl3(size_t size /*= 0*/)
	: v_{ static_cast<T*>(size == 0 ? nullptr : operator new(sizeof(T)*size)) },
	vsize_{ size },
	vused_{ 0 }
{
}

template<class T>
inline StackImpl3<T>::~StackImpl3()
{
	destroy(v_, v_ + vused_);
	operator delete(v_);
}

template<class T>
inline void StackImpl3<T>::Swap(StackImpl3<T>& other) noexcept
{
	swap(v_, other.v_);
	swap(vsize_, other.vsize_);
	swap(vused_, other.vused_);
}

template <class T>
class Stack3
{
public:
	Stack3(size_t size = 0);
	~Stack3() {}
	Stack3(const Stack3&);
	Stack3& operator = (const Stack3&);
	size_t Count() const;
	void Push(const T&);
	T& Top();
	void Pop();

private:
	StackImpl3<T> impl_;
};

template<class T>
inline Stack3<T>::Stack3(size_t size)
	:impl_(size)
{
}

template<class T>
inline Stack3<T>::Stack3(const Stack3 & other)
	:impl_(other.impl_.vused_)
{
	while (impl_.vused_ < other.impl_.vused_)
	{
		construct(impl_.v_ + impl_.vused_, other.impl_.v_[impl_.vused_]);
		++impl_.vused_;
	}
}

template<class T>
inline Stack3 & Stack3<T>::operator=(const Stack3 &)
{
	Stack3 temp(other);
	impl_.Swap(temp.impl_);
	return *this;
}

template<class T>
inline size_t Stack3<T>::Count() const
{
	return impl_.vused_;
}

template<class T>
inline void Stack3<T>::Push(const T &)
{
	if (impl_.vused_ == impl_.vsize_)
	{
		Stack3 temp(impl_.vsize_ * 2 + 1);
		while (temp.Count() < impl_.vused_)
		{
			temp.Push(impl_.v_[temp.Count()]);
		}
		temp.Push(t);
		impl_.Swap(temp.impl_);
	}
	else
	{
		construct(impl_.v_ + impl_.vused_, t);
		++impl_.vused_;
	}
}

template<class T>
inline T & Stack3<T>::Top()
{
	if (impl_.vused_ == 0)
	{
		throw "empty stack";
	}
	return impl_.v_[impl_.vused_ - 1];
}

template<class T>
inline void Stack3<T>::Pop()
{
	if (impl_.vused_ == 0)
	{
		throw "pop from empty stack";
	}
	else
	{
		--impl_.vused_;
		destroy(impl_.v_ + impl_.vused_);
	}
}

