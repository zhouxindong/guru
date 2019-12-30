// grocery_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "grocery/path.h"
#include <iostream>
#include "grocery/digest.h"
#include "grocery/ostream_ex.h"
#include <vector>
#include "grocery/string_ex.h"
#include "grocery/file.h"
#include <cassert>

using namespace std;
using namespace guru;

class foo
{};

// path
//int main()
//{
//	cout << path::app_full_name() << endl;
//	cout << path::app_path() << endl;
//	cout << path::app_name() << endl;
//	cout << path::app_base_name() << endl;
//	cout << unique_name_by_date_for_class<int>("demo1", ".log") << endl;
//	cout << unique_name_by_date_for_class<double>("demo1", ".log") << endl;
//	cout << unique_name_by_date_for_class<foo>("demo1", ".log") << endl;
//
//	cout << path::cwd() << endl;
//	path::cwd("C:/Ñ¸À×ÏÂÔØ");
//	cout << path::cwd() << endl;
//
//	cout << path::file_ext_name("1.config") << endl;
//	cout << path::file_ext_name("12345") << endl;
//	cout << path::file_name_no_ext("1.config") << endl;
//	cout << path::file_name_no_ext("12345") << endl;
//
//	system("pause");
//	return 0;
//}

// digest
//int main()
//{
//	char const* str = "abcdefghijklmnopqrstuvwxyz1023456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//	digest_md5 md5;
//	cout << md5.str_md5(str).get_md5() << endl;
//	cout << md5.file_md5(path::app_full_name().c_str()).get_md5() << endl;
//	system("pause");
//	return 0;
//}

// ostream_ex
//int main()
//{
//	vector<uint8_t> v1;
//	for (int i = 0; i < 64; ++i)
//	{
//		v1.push_back(i);
//	}
//
//	cout << v1;
//	system("pause");
//	return 0;
//}

// guid
//int main()
//{
//	std::cout << guid() << endl;
//	system("pause");
//	return 0;
//}

// file
//int main()
//{
//	//assert(file_exists("exist.txt"));
//	//assert(!file_exists("nonexist.dat"));
//
//	//assert(!remove_file("nonexist.dat"));
//	//assert(remove_file("exist.txt"));
//
//
//
//	system("pause");
//	return 0;
//}
