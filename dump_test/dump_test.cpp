// dump_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "dump/stack_tracer.h"
#include <future>
#include <thread>
#include <iostream>
#include "struct_exception_af.h"
#include "dump/uncatcher.h"
#include "dump/mem_tracer.h"
#include "dump/proc_info.h"

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
/*
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
*/

// mem_tracer
//#if defined(_WIN32) || defined(_WIN64)
//#include <crtdbg.h>
//#endif
/*
class MyWork
{
public:
	MyWork() :
		_a{ new int[1000000] }, _b{ new int[1000000] }
	{
		//std::cout << "MyWork::MyWork()\n";
	}
	MyWork(int a, int b) 
		: _a{ new int[1000000] }, _b{ new int[1000000] }
	{
		//std::cout << "MyWork::MyWork(int, int)\n";
		int c = a + b;
		c++;
	}
	~MyWork()
	{
		//std::cout << "MyWork::~MyWork()\n";
		delete[] _a;
		delete[] _b;
	}

private:
	int* _a;
	int* _b;

};

int main()
{
	// 1. pointer
	//int* pi = new int;
	//double* pda = new double[10];
	//std::string* ps1 = new std::string("hello");
	//std::string* psa2 = new std::string[10];
	//MyWork* pmw1 = new MyWork(2, 3);
	//MyWork* pmw2 = new MyWork[10];

	//cout << dump_mem_tracer() << "\n";

	//delete pi;
	//delete[] pda;
	//delete ps1;
	//delete[] psa2;
	//delete pmw1;
	//delete[] pmw2;

	//cout << "\n--------------after deleted----------------\n";
	//cout << dump_mem_tracer() << "\n";

	// 2. smart pointer
	//uint64_t count = 0;
	//while (true)
	//{
	//	std::shared_ptr<MyWork> ps(new MyWork(1, 2));
	//	if (++count % 100 == 0)
	//		cout << dump_mem_tracer() << '\n';
	//	//Sleep(1);
	//}

	// 3. malloc
	void* pm = malloc(1000);
	cout << dump_mem_tracer() << '\n';
	free(pm);
	cout << dump_mem_tracer() << '\n';
	system("pause");
	return 0;
}
*/

int main()
{
	proc_mem<> pm;
	pm.fresh();
	cout << pm.working_size() << '\n';
	cout << pm.peak_working_size() << '\n';
	cout << pm.page_file() << '\n';
	cout << pm.peak_page_file() << '\n';

	pm.gc();
	pm.fresh();
	cout << pm.working_size() << '\n';
	cout << pm.peak_working_size() << '\n';
	cout << pm.page_file() << '\n';
	cout << pm.peak_page_file() << '\n';

	system("pause");
	return 0;
}