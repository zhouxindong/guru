// dump_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "dump/stack_tracer.h"
#include <future>
#include <thread>
#include <iostream>
#include "struct_exception_af.h"
#include "dump/uncatcher.h"

using namespace std;
using namespace guru;

// stack_tracer::stack_trace()
//int main()
//{
//	cout << "-------------stack_trace()----------------" << endl;
//	cout << stack_tracer::stack_trace("prefix", "surfix") << endl;
//	cout << "-------------stack_trace2()---------------" << endl;;
//	cout << stack_tracer::stack_trace2("prefix2", "surfix2") << endl;
//	system("pause");
//    return 0;
//}

// struct_exception
//int main()
//{
//	try
//	{
//		foo1();
//	}
//	catch (std::exception& e)
//	{
//		std::cout << e.what() << std::endl;
//	}
//
//	system("pause");
//	return 0;
//}

// uncatched exception
// must be runed without IDE
//void uncatch_cb(std::string const& msg)
//{
//	std::cout << "uncatched begin:\n";
//	std::cout << msg;
//	std::cout << "uncatched end!\n";
//}
//
//int main()
//{
//	uncatcher uncat(uncatch_cb);
//	foo1();
//	//throw std::exception("asdfasdf");
//
//	cout << "after uncaught!!!\n";
//	system("pause");
//	return 0;
//}

// SEH
// C++ /EHa
void uncatch_cb(std::string const& msg)
{
	std::cout << msg;
}

int main()
{
	uncatcher uncat(uncatch_cb);

	//try {
	//	int* p = 0;
	//	std::cout << *p;
	//}
	//catch (const std::exception& e) {
	//	std::cerr << e.what() << "\n";
	//}

	//try
	//{
	//	throw "adfasdf";
	//}
	//catch (...)
	//{
	//	std::cout << "adfasdf\n";
	//}

	//try {
	//	int* p = 0;
	//	*p = 0;
	//	std::cout << *p;
	//}
	//catch (const std::exception& e) {
	//	std::cerr << e.what() << "\n";
	//}

	//try {
	int a = 42;
	volatile int b = 0;
	std::cout << a / b;
	//}
	//catch (const std::exception& e) {
	//	std::cerr << e.what() << "\n";
	//}


	system("pause");
	return 0;
}

