// log_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zlog/log_channel.h"
#include "zlog/log_item.h"
#include "zlog/log_formatter.h"
#include "base_chain/non_instantable.h"
#include "zlog/log_item_generator.h"
#include "zlog/log_filter.h"
#include "zlog/logger.h"
#include <future>
#include <memory>
#include "zlog/config_logger.h"
#include "zlog/dumper.h"
#include "dump/struct_exception.h"

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
	//system("pause");
	//return 0;

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
	//system("pause");

	// 3. file_channel
	file_channel fc("file_channel1.out");

	for (int i = 0; i < 100000; ++i)
	{
		if (i % 100 == 0)
			std::cout << i << endl;

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

		Sleep(1);
	}
	system("pause");
	return 0;

	// 4. udp_channel
	//udp_channel uc("udptest#10.16.2.55#8005#10.16.2.55");
	//while (true)
	//{
	//	static int count = 1;
	//	log_item log1 = LOG(INFO) << ++count;
	//	uc << log1;
	//	std::cout << count << "\n";
	//	Sleep(100);
	//}

	// 5. channel_base
	//std::shared_ptr<channel_base> channel{ new udp_channel("udptest#10.16.2.55#8005#10.16.2.55") };
	//std::shared_ptr<channel_base> channel{ new console_channel() };

	//log_item li = LOG(TRACE) << "this is a trace";
	//channel->log(li, LogFormatter::STD_FORMATTER);
	//li = LOG(DEBUG) << "this is a debug";
	//channel->log(li);
	//li = LOG(INFO) << "this is a info";
	//channel->log(li, LogFormatter::TBL_FORMATTER);
	//li = LOG(WARN) << "this is a warn";
	//channel->log(li);
	//li = LOG(ERROR) << "this is a error";
	//channel->log(li);
	//li = LOG(FATAL) << "this is a fatal";
	//channel->log(li, LogFormatter::TBL_FORMATTER);

	//system("pause");
	//return 0;
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
	// 1. console_logger
	//auto& logger = console_logger::get("console1");
	//auto v = std::move(generate_log_item(2));
	//for (auto& item : v)
	//{
	//	logger.log(item);
	//	Sleep(100);
	//}

	//auto& logger2 = console_info_logger::get("console2");
	//v = std::move(generate_log_item(200, " new log"));
	//for (auto& item : v)
	//{
	//	logger2.log(item);
	//	Sleep(100);
	//}

	//system("pause");


	// 2. file_logger
	//auto& logger = file_logger::get("testlog");
	//auto v = std::move(generate_log_item(2));
	//for (auto& item : v)
	//{
	//	logger.log(item);
	//}

	//auto& logger2 = file_logger::get("testlog");
	//v = std::move(generate_log_item(2, " new log"));
	//for (auto& item : v)
	//{
	//	logger2.log(item);
	//}

	//system("pause");
	//return 0;

	// 3. udp logger
	//auto& logger = udp_info_logger::get("udptest#10.16.2.55#28583#10.16.2.55");
	//auto v = std::move(generate_log_item(200000));
	//for (auto& item : v)
	//{
	//	logger.log(item);
	//}

	// 4. custom logger
	//typedef basic_logger<
	//	set_filter<_LOG_LEVEL::_LOG_DEBUG, _LOG_LEVEL::_LOG_FATAL>,
	//	table_formatter<log_item>,
	//	console_channel> my_logger;
	//auto& logger = my_logger::get("cosle");
	//auto v = std::move(generate_log_item(200000));
	//for (auto& item : v)
	//{
	//	logger.log(item);
	//}

}
*/

 //different logger class with the same name, it would create different log file
///*
bool thread_proc() {
	auto v = generate_log_item(5000, std::string("单独的异步线程..."));
	for (auto& item : v) {
		file_info_logger::get("max4kfile").log(item);

		Sleep(1);
	}
	return true;
}

int main()
{
	auto v = generate_log_item(5000, std::string("主线程..."));
	std::future<bool> rs(std::async(thread_proc));

	for (auto& item : v) {
		file_logger::get("max4kfile").log(item);
		Sleep(1);
	}
	rs.wait();
	system("pause");
	return 0;
}
//*/

// config logger from file
//int main()
//{
//	config_log logger("log.config.xml");
//
//	for (int i = 0; i < 10000; ++i)
//	{
//		log_item li = LOG(TRACE) << "this is a trace";
//		logger.log(li);
//		li = LOG(DEBUG) << "this is a debug";
//		logger.log(li);
//		li = LOG(INFO) << "this is a info";
//		logger.log(li);
//		li = LOG(WARN) << "this is a warn";
//		logger.log(li);
//		li = LOG(ERROR) << "this is a error";
//		logger.log(li);
//		li = LOG(FATAL) << "this is a fatal";
//		logger.log(li);
//	}
//
//	system("pause");
//	return 0;
//}

// dump_stack_trace
//int main()
//{
//	// 1. config_log
//	//config_log logger("log.config.xml");
//	//dump_stack_trace(logger);
//
//	// 2. basic_log
//	//auto& logger = console_logger::get("console");
//	//dump_stack_trace(logger);
//
//	//auto& logger2 = file_logger::get("dump_file");
//	//dump_stack_trace(logger2);
//
//	//auto& logger3 = udp_logger::get("udptest#10.16.2.55#8808#10.16.2.55");
//	//dump_stack_trace(logger3);
//
//	system("pause");
//	return 0;
//}

void foo3()
{
	throw guru::struct_exception("test struct_exception");
}

void foo2()
{
	foo3();
}

void foo1()
{
	foo2();
}

// dump_exception_msg
//int main()
//{
//	// 1. config_log
//	config_log logger("log.config.xml");
//	try
//	{
//		foo1();
//	}
//	catch (std::exception& e)
//	{
//		dump_exception_msg(logger, e);
//	}
//
//	// 2. basic_log
//	//auto& logger = console_logger::get("console");
//	//auto& logger2 = file_logger::get("dump_file");
//	//auto& logger3 = udp_logger::get("udptest#10.16.2.55#8808#10.16.2.55");
//	//try
//	//{
//	//	foo1();
//	//}
//	//catch (std::exception& e)
//	//{
//	//	dump_exception_msg(logger, e);
//	//	dump_exception_msg(logger2, e);
//	//	dump_exception_msg(logger3, e);
//	//}
//
//	system("pause");
//	return 0;
//}

// uncatcher
// must be runned without IDE
// caution: xml file must be the right path(in the Debug dir)
//int main()
//{
//	// 1. basic_log
//	//auto& logger = console_logger::get("demo");
//
//	////foo1(); // uncatch
//
//	////int* p = 0; 
//	////*p = 33;  // SEH
//
//	//int a = 42;
//	//volatile int b = 0;
//	//int c = a / b; // SEH
//
//
//	// 2. config_log
//	config_log logger("log.config.xml");
//
//	//foo1();
//	//int* p = 0; 
//	//*p = 33;  // SEH
//
//	int a = 42;
//	volatile int b = 0;
//	int c = a / b; // SEH
//
//	system("pause");
//	return 0;
//}

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

	void show()
	{
		std::cout << "MyWork::show()\n";
	}

private:
	int* _a;
	int* _b;

};

// memory track
/*
int main()
{
	// 1. pointer		
	////auto& logger = console_logger::get("demo");
	//config_log logger("log.config.xml");

	//int* pi = new int;
	//double* pda = new double[10];
	//std::string* ps1 = new std::string("hello");
	//std::string* psa2 = new std::string[10];
	//MyWork* pmw1 = new MyWork(2, 3);
	//MyWork* pmw2 = new MyWork[10];

	//dump_memory(logger);

	//delete pi;
	//delete[] pda;
	//delete ps1;
	//delete[] psa2;
	//delete pmw1;
	//delete[] pmw2;

	//dump_memory(logger);

	// 2. smart pointer
	////auto& logger = console_logger::get("demo");
	//config_log logger("log.config.xml");

	//uint64_t count = 0;
	//while (count < 1000)
	//{
	//	std::shared_ptr<MyWork> ps(new MyWork(1, 2));
	//	if (++count % 5 == 0)
	//	{
	//		dump_memory(logger);
	//	}
	//	Sleep(100);
	//}

	// 3. malloc
	//auto& logger = console_logger::get("demo");
	config_log logger("log.config.xml");

	void* pm = malloc(1000);

	logger.log(LOG(INFO) << "before free()");
	dump_memory(logger);

	free(pm);
	logger.log(LOG(INFO) << "after free()");
	dump_memory(logger);

	system("pause");
	return 0;
}
*/