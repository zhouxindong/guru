// grocery_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "grocery/path.h"
#include <iostream>
#include "grocery/digest.h"
#include "grocery/ostream_ex.h"
#include <vector>

using namespace std;
using namespace guru;

class foo
{};

// path
//int main()
//{
//	cout << path::app_full_name() << endl;
//	cout << path::app_name() << endl;
//	cout << path::app_base_name() << endl;
//	cout << unique_name_by_date_for_class<int>("demo1", ".log") << endl;
//	cout << unique_name_by_date_for_class<double>("demo1", ".log") << endl;
//	cout << unique_name_by_date_for_class<foo>("demo1", ".log") << endl;
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
int main()
{
	vector<uint8_t> v1;
	for (int i = 0; i < 64; ++i)
	{
		v1.push_back(i);
	}

	cout << v1;
	system("pause");
	return 0;
}