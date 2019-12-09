// template_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "template/friend.h"
#include "template/variadic.h"
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include "template/raw_array.h"
#include "template/variable.h"

using namespace std;

// friend
//int main()
//{
//	Friend_Demo<int> fdi;
//	cout << fdi;
//	show_me(fdi);
//	show_me2(fdi);
//
//	system("pause");
//    return 0;
//}

// variadic
//int main()
//{
//	std::string s("world");
//	print(7.5, "hello", s);
//
//	print2(7.5, "hello", s);
//
//	print3(7.5, "hello", s);
//
//	print4(7.5, "hello", s);
//
//	//std::cout << foldSum(1, 2, 3, 4, 5) << '\n';
//
//	printDouble(1, 2, 3, 4, 5);
//
//	addOne(1, 2, 3, 4, 5);
//
//	std::vector<std::string> coll = { "good", "times","say", "bye" };
//	printElems(coll, 2, 0, 3);
//	printIdx<2, 0, 3>(coll);
//
//	std::array<std::string, 5> arr = { "Hello", "my", "new","!","World" };
//	printByIdx(arr, Indices<0, 4, 3>());
//
//	auto t = std::make_tuple(12, "monkeys", 2.0);
//	printByIdx(t, Indices<0, 1, 2>());
//
//	system("pause");
//	return 0;
//}

// raw_array and string literals
//int main()
//{
//	int x[] = { 1,2,3 };
//	int y[] = { 1,2,3,4,5 };
//	std::cout << less2(x, y) << '\n';
//
//	std::cout << less2("ab", "abc") << '\n';
//
//	system("pause");
//	return 0;
//}

// variable
int main()
{
	std::cout << pi<double> << '\n';
	std::cout << pi2<> << '\n';

	//std::cout << dval<'c'> << '\n';
	arr<10>[0] = 42;
	for (std::size_t i = 0; i < arr<10>.size(); ++i)
		std::cout << arr<10>[i] << '\n';

	arr<20>[2] = 22;

	system("pause");
	return 0;
}

