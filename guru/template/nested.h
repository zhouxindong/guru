#pragma once

template <typename T>
class Shell
{
public:
	template <int N>
	class In
	{
	public:
		template <int M>
		class Deep
		{
		public:
			virtual void f();
		};
	};
};

template <typename T, int N>
class Weird
{
public:
	void case1(typename Shell<T>::template In<N>::template Deep<N>* p)
	{
		p->template Deep<N>::f(); // inhibit virtual call
	}

	void case2(typename Shell<T>::template In<N>::template Deep<N>& p)
	{
		p.template Deep<N>::f(); // inhibit virtual call
	}
};