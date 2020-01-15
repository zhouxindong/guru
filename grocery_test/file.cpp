#include "stdafx.h"
#include "grocery/file.h"
#include <chrono>
#include <fstream>
#include <iostream>
using namespace std;
using namespace guru;

//int main()
//{
//	const char* file = "InstallationLog.txt";
//	ofstream f(file, std::ios::app | std::ios::out);
//
//	chrono::system_clock::time_point tp1 = chrono::system_clock::now();
//	for (int i=0; i<100; ++i)
//	{
//		int64_t size = file_size(file);
//	}
//	chrono::system_clock::time_point tp2 = chrono::system_clock::now();
//	chrono::system_clock::duration d = tp2 - tp1;
//	cout << "file: " << d.count() << endl;
//
//	f << "add by zhouxindong!!!" << endl;
//	f.close();
//
//	system("pause");
//	return 0;
//}