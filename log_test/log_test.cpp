// log_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zlog/log_item.h"
#include "zlog/log_formatter.h"
#include "base_chain/non_instantable.h"

#include <iostream>

using namespace std;
using namespace guru;

// log_item
/*
int main()
{
	log_item log1(_LOG_FATAL, __FILE__, __func__, __LINE__);
	log1 << "some information";
	log1 << " " << 3456;
	cout << log1 << endl;

	log1 = std::move((LOG(TRACE) << "a log item: trace"));
	cout << log1 << endl;

	log1 = LOG(DEBUG) << "a log item: debug";
	cout << log1 << endl;

	log1 = LOG(INFO) << "a log item: info";
	cout << log1 << endl;

	log1 = std::move(LOG(WARN) << "a log item: warn");
	cout << log1 << endl;

	log1 = LOG(ERROR) << "a log item: error";
	cout << log1 << endl;

	log1 = LOG(FATAL) << "a log item: fatal";
	cout << log1 << endl;

	system("pause");
    return 0;
}
*/

// log_formatter
int main()
{
	log_item log1(_LOG_FATAL, __FILE__, __func__, __LINE__);
	log1 << "some information";
	log1 << " " << 3456;
	cout << std_formatter<>::format(log1) << endl;
	cout << semicolon_formatter<>::format(log1) << endl;

	log1 = std::move((LOG(TRACE) << "a log item: trace"));
	cout << std_formatter<log_item>::format(log1) << endl;
	cout << semicolon_formatter<>::format(log1) << endl;

	log1 = LOG(DEBUG) << "a log item: debug";
	cout << std_formatter<log_item>::format(log1) << endl;
	cout << semicolon_formatter<>::format(log1) << endl;

	log1 = LOG(INFO) << "a log item: info";
	cout << std_formatter<log_item>::format(log1) << endl;
	cout << semicolon_formatter<>::format(log1) << endl;

	log1 = std::move(LOG(WARN) << "a log item: warn");
	cout << std_formatter<log_item>::format(log1) << endl;
	cout << semicolon_formatter<>::format(log1) << endl;

	log1 = LOG(ERROR) << "a log item: error";
	cout << std_formatter<log_item>::format(log1) << endl;
	cout << semicolon_formatter<>::format(log1) << endl;

	log1 = LOG(FATAL) << "a log item: fatal";
	cout << std_formatter<log_item>::format(log1) << endl;
	cout << semicolon_formatter<>::format(log1) << endl;

	system("pause");
	return 0;
}

// non_instantable
/*
class B {
public:
	B(int i = 0) {}
	int show(int a) {
		return a + 1;
	}

	static int foo() {
		return 15;
	}
};

int main()
{
	B b1;
	B b2(b1);
	B b3 = b2;
	cout << non_instantable<B>::foo() << endl;
	system("pause");
	return 0;
}
*/


