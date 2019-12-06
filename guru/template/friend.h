#pragma once

#include <ostream>
#include <iostream>

// 3 way to defined friend for template class
// 1. operator <<
// 2. show_me
// 3. show_me2 (warning C4396)
#pragma warning(disable:4396)

template <typename _Tx>
class Friend_Demo;

template <typename _Tx>
void show_me2(Friend_Demo<_Tx> const&);

template <typename _Tx>
class Friend_Demo
{
	friend std::ostream& operator << (std::ostream& strm, Friend_Demo<_Tx> const& s)
	{
		std::cout << "friend operator << definied in class" << endl;
		return strm;
	}

	template <typename _U>
	friend void show_me(Friend_Demo<_U> const& s);

	friend void show_me2 <_Tx> (Friend_Demo<_Tx> const& s);
};

template<typename _U>
inline void show_me(Friend_Demo<_U> const & s)
{
	std::cout << "friend show_me definied outside" << endl;
}

template <typename _Tx>
inline void show_me2(Friend_Demo<_Tx> const & s)
{
	std::cout << "friend show_me2 definied outside" << endl;
}
