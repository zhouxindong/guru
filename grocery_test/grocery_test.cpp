// grocery_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "grocery/path.h"
#include <iostream>
#include "grocery/chrono_ex.h"

using namespace std;
using namespace guru;

//int main()
//{
//	cout << path::app_full_name() << endl;
//	cout << path::app_name() << endl;
//	cout << path::app_base_name() << endl;
//
//	system("pause");
//    return 0;
//}

int main()
{
	auto t = from_time_point(chrono::system_clock::now());
	auto t2 = from_time_point(chrono::system_clock::now(), true);
	auto t3 = from_time_point(chrono::system_clock::now() - days(20000)); // before 1970-1-1

	cout << time_point_to_string(chrono::system_clock::now()) << endl;
	cout << time_point_to_string(chrono::system_clock::now(), true) << endl;
	cout << time_point_to_string(chrono::system_clock::now() - days(20000)) << endl;

	auto t4 = to_time_point(1970, 1, 1);
	auto t5 = to_time_point(1970, 1, 1, 0, 0, 0, 0, true);

	cout << time_point_to_string(t4) << endl;
	cout << time_point_to_string(t5) << endl;

	auto t6 = to_time_point(2000, 3, 18, 15, 33, 38, 135);
	auto t7 = to_time_point(1960, 7, 28, 4, 32, 1, 95, true);
	cout << time_point_to_string(t6) << endl;
	cout << time_point_to_string(t7) << endl;

	system("pause");
	return 0;
}

