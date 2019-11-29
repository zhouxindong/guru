// log_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "log/log_item.h"

#include <iostream>

using namespace std;
using namespace guru;

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

