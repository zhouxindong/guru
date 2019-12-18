// operator_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "operator/newdel.h"
#include <iostream>

int main()
{
	std::string fill("<garbage>");
	std::string* string1 = new std::string("hello"); // standard version
	std::string* string2 = new(fill)std::string("world!"); // overloaded version
	std::cout << *string1 << '\n';
	std::cout << *string2 << '\n';

	delete string1;	// standard version;
	//delete string2; // 1. standard version;

	string2->~basic_string();
	operator delete(string2, fill);	// 2. overloaded version

	std::cout << fill << '\n';

	system("pause");
    return 0;
}

