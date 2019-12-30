// log_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zlog/log_item.h"
#include "zlog/log_formatter.h"
#include "base_chain/non_instantable.h"
#include "zlog/log_channel.h"
#include "zlog/log_item_generator.h"
#include "zlog/log_filter.h"
#include "zlog/logger.h"
#include <future>

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
/*
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
*/

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

// channel
/*
int main()
{
	// 1. null_channel
	//log_item log1 = LOG(FATAL);
	//log1 << "some information";
	//log1 << " " << 3456;
	//Dummy_Channel << log1;

	//log1 = std::move((LOG(TRACE) << "a log item: trace"));
	//Dummy_Channel << log1;

	//log1 = LOG(DEBUG) << "a log item: debug";
	//Dummy_Channel << log1;

	//log1 = LOG(INFO) << "a log item: info";
	//Dummy_Channel << log1;

	//log1 = std::move(LOG(WARN) << "a log item: warn");
	//Dummy_Channel << log1;

	//log1 = LOG(ERROR) << "a log item: error";
	//Dummy_Channel << log1;

	//log1 = LOG(FATAL) << "a log item: fatal";
	//Dummy_Channel << log1;

	// 2. console_channel
	//log_item log1 = LOG(FATAL);
	//log1 << "some information";
	//log1 << " " << 3456;
	//Console_Channel << log1;

	//log1 = std::move((LOG(TRACE) << "a log item: trace"));
	//Console_Channel << log1;

	//log1 = LOG(DEBUG) << "a log item: debug";
	//Console_Channel << log1;

	//log1 = LOG(INFO) << "a log item: info";
	//Console_Channel << log1;

	//log1 = std::move(LOG(WARN) << "a log item: warn");
	//Console_Channel << log1;

	//log1 = LOG(ERROR) << "a log item: error";
	//Console_Channel << log1;

	//log1 = LOG(FATAL) << "a log item: fatal";
	//Console_Channel << log1;

	// 3. file_channel
	file_channel fc("file_channel1.out");
	log_item log1 = LOG(FATAL);
	log1 << "some information";
	log1 << " " << 3456;
	fc << log1;

	log1 = std::move((LOG(TRACE) << "a log item: trace"));
	fc << log1;

	log1 = LOG(DEBUG) << "a log item: debug";
	fc << log1;

	log1 = LOG(INFO) << "a log item: info";
	fc << log1;

	log1 = std::move(LOG(WARN) << "a log item: warn");
	fc << log1;

	log1 = LOG(ERROR) << "a log item: error";
	fc << log1;

	log1 = LOG(FATAL) << "a log item: fatal";
	fc << log1;

	system("pause");
	return 0;
}
*/

// filter
/*
int main()
{
	// level filter + console channel
	//console_channel cc;
	//auto v = generate_log_item(100);
	//level_filter<_LOG_FATAL> filter;
	//for (auto& item : v) 
	//{
	//	filter(item, cc) << std_formatter<log_item>::format(item) << std::endl;
	//	Sleep(100);
	//}

	// set_filter + file channel
	file_channel fc("set_filter_demo.out");
	auto v = generate_log_item(100);
	set_filter<_LOG_TRACE, _LOG_FATAL> filter;
	for (auto& item : v)
	{
		filter(item, fc) << std_formatter<log_item>::format(item) << std::endl;
		//filter << std::tuple(std::ref<file_channel>(fc), std::ref<log_item>(item));
		Sleep(100);
	}

	system("pause");
	return 0;
}
*/

// basic_logger
/*
int main()
{
	// console_logger
	//auto& logger = console_logger::get("console1");
	//auto v = std::move(generate_log_item(200));
	//for (auto& item : v)
	//{
	//	logger.log(item);
	//	Sleep(100);
	//}

	//auto& logger2 = console_logger::get("console2");
	//v = std::move(generate_log_item(200, " new log"));
	//for (auto& item : v)
	//{
	//	logger2.log(item);
	//	Sleep(100);
	//}

	// file_logger
	auto& logger = file_logger::get("log_test");
	auto v = std::move(generate_log_item(200));
	for (auto& item : v)
	{
		logger.log(item);
	}

	auto& logger2 = file_logger::get("log_test");
	v = std::move(generate_log_item(200, " new log"));
	for (auto& item : v)
	{
		logger2.log(item);
	}

	system("pause");
	return 0;
}
*/

 //different logger class with the same name, it would create different log file
/*
bool thread_proc() {
	auto v = generate_log_item(1000, std::string("单独的异步线程..."));
	for (auto& item : v) {
		file_info_logger::get("thread_logger84").log(item);

		Sleep(1);
	}
	return true;
}

int main()
{
	auto v = generate_log_item(1000, std::string("主线程..."));
	std::future<bool> rs(std::async(thread_proc));

	for (auto& item : v) {
		file_logger::get("thread_logger84").log(item);
		Sleep(1);
	}
	rs.wait();
	system("pause");
	return 0;
}
*/

// multi channel
int main()
{
	system("pause");
	return 0;
}