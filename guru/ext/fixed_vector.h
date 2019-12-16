#pragma once

#include <algorithm>

template <typename T, size_t size>
class fixed_vector
{
public:
	typedef T*		iterator;
	typedef T const* const_iterator;
	iterator		begin()			{ return v_; }
	iterator		end()			{ return v_ + size; }
	const_iterator	cbegin() const	{ return v_; }
	const_iterator	cend() const	{ return v_ + size; }

public:
	fixed_vector() : v_(new T[size]) {}
	~fixed_vector() { delete[] v_; }

public:
	template <typename O, size_t osize>
	fixed_vector(const fixed_vector<O, osize>& other)
		: v_{ new T[size] }
	{
		try
		{
			std::copy(other.begin(), other.begin() + std::min(size, osize), begin());
		}
		catch (...)
		{
			delete[] v_;
			throw;
		}
	}

	fixed_vector(const fixed_vector<T, size>& other)
		: v_{ new T[size] }
	{
		try
		{
			std::copy(other.begin(), other.end(), begin());
		}
		catch (...)
		{
			delete[] v_;
			throw;
		}
	}

	void swap(fixed_vector<T, size>& other) noexcept
	{
		std::swap(v_, other.v_);
	}

	template <typename O, size_t osize>
	fixed_vector<T, size>& operator = (const fixed_vector<O, osize>& other)
	{
		fixed_vector<T, size> temp(other); // complete all works
		swap(temp);
		return *this;
	}

	fixed_vector<T, size>& operator = (const fixed_vector<T, size>& other)
	{
		fixed_vector<T, size> temp(other);
		swap(temp);
		return *this;
	}

private:
	T* v_;
};