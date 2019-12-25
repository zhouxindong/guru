// nut_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "nut/signal.h"
#include <iostream>
#include "signal_af.h"
#include "nut/keyboard_eye.h"
#include "nut/thread_guard.h"
#include "nut/process_eye.h"
#include "nut/single_proc.h"
#include "nut/service_base.h"
#include "nut/service_installer.h"
#include "test_service.h"

using namespace std;
using namespace guru;

// signal test
/*
int main()
{
	// 1.
	//cout << "#1------------\n";
	//guru::signal<void(int, int)> data;
	//data.connect(print_sum);
	//data.connect(print_mul);
	//data(3, 4);

	// 2.
	//cout << "\n#2-------------\n";
	//guru::signal<int(int)> worker;
	//worker.connect(pow2);
	//worker.connect(pow3);
	//// worker(2) result is the last connect slot return
	//std::cout << "last slot = " << worker(2) << "\n";
	//// trigger signal pass second
	//std::cout << "agg value = " << worker(2, sum_agg) << "\n";

	// 3.
	//cout << "\n#3----------------\n";
	//guru::signal<void()> news;
	//guru::connection conn1 = news.connect([]() {std::cout << "news #1\n"; });
	//guru::connection conn2 = news.connect([]() {std::cout << "news #2\n"; });
	//news.connect([]() {std::cout << "news #3\n"; });
	//cout << "(all connections)\n";
	//news();

	//cout << "(lock connection 1#)\n";
	//conn1.set_lock(true);
	//news();

	//cout << "(disconnect connection 2#)\n";
	//news.disconnect(conn2);
	////conn2.disconnect(); // not work
	//news();

	// 4.
	//cout << "\n#4-------------------\n";
	//guru::signal<void()> dummy;
	//auto foo = []() -> void {std::cout << "lambda: foo\n"; };
	//dummy.connect(foo); // connect to lambda
	//dummy.connect(bar); // connect to function
	//baz b;
	//qux q;
	//dummy.connect(b); // connect to function object
	//dummy.connect(&q, &qux::print); // connect to member function
	//dummy();

	// 5. 
	//cout << "\n#5----------------------\n";
	//signal<void()> printer;
	//{
	//	demo_slot slot(42);
	//	auto print_value = [&]() {cout << "value = " << slot.value << "\n"; };
	//	printer.connect(std::move(print_value), &slot);
	//	printer();
	//}
	//printer();

	// 6. signal connect to signal
	//signal1->signal2->slot, emit signal1
	//	cout << "\n#6---------------------------\n";
	//signal<void(int)> signal1;
	//signal<void(int)> signal2;
	//signal1.connect(&signal2);
	//signal2.connect([](int x) {
	//	std::cout << "x = " << x << "\n";
	//});
	//signal1(10);

	// 7.
	//cout << "\n#7------------------------\n";
	//signal<void()> sig;
	//{
	//	slot s;
	//	sig.connect((signal<void()>::callback_type)bar, &s);
	//	auto foo = []() {std::cout << "sig7\n"; };
	//	sig.connect(foo, &s);
	//	baz b;
	//	qux q;
	//	sig.connect(b, &s);
	//	sig.connect(&q, &qux::print, &s);
	//	sig();
	//}
	//sig();

	// 8.
	//cout << "\n#8---------------------------\n";
	//signal<void()> sig8;
	//sig8.connect(bar);
	//sig8.connect(bar);
	//sig8.connect(bar);
	//sig8();
	//sig8.disconnect_all();
	//sig8();

	// 9. cross thread

	system("pause");
    return 0;
}
*/

// keyboard eye test
/*
void key_eyer(std::string m)
{
	cout << m << endl;
}

int main()
{
	keyboard_eye key_eye;
	key_eye.sig().connect(key_eyer);
	thread key_eye_thread(key_eye);
	thread_guard guard(key_eye_thread);

	return 0;
}
*/

// process eye test
//void process_eyer(std::string m)
//{
//	cout << m << endl;
//}
//int main()
//{
//	process_eye proc_eye;
//	proc_eye.sig().connect(process_eyer);
//	thread proc_eye_thread(proc_eye);
//	proc_eye_thread.detach();
//	thread_guard guard(proc_eye_thread);
//	system("pause");
//	return 0;
//}

// single proc test
//class Fooab {};
//int main()
//{
//	single_proc<Fooab> si;
//	cout << si.owned() << endl;
//	system("pause");
//	return 0;
//}

// service test
/*
 main()
 1. install service
 2. unstall service
 3. start service (loop in run())
 4. stop service (continue exec after run() call)

 1. install service
	1.1 main()
	1.2 ServiceBase::ServiceBase()
	1.3 Installing user tracker service
	1.4 install user tracker service successful

 2. start
	2.1 main()
	2.2 ServiceBase::ServiceBase()
	2.3 ServiceBase::RunInternal
	2.4 ServiceBase::SvcMain
	2.5 ServiceBase::Start
	2.6 UserTrackerService::OnStart

 3. stop
	3.1 ServiceBase::stop
	3.2 UserTrackerService::OnStop
	3.3 end of main

 4. unstall service
	4.1 main()
	4.2 ServiceBase::ServiceBase()
	4.3 uninstall user tracker service
	4.4 ServiceBase::stop
	4.5 UserTrackerService::OnStop
	4.6 end of main
	4.7 uninstall user tracker service successful
*/
int main(int argc, char* argv[])
{
	test_service service;
	OutputDebugString(_T("main()"));

	if (argc > 1) {
		if (strcmp(argv[1], ("install")) == 0) {
			_tprintf(_T("Installing service\n"));
			OutputDebugString(_T("Installing test service"));
			if (!guru::install_service(service)) {
				_tprintf(_T("Couldn't install service: %d\n"), ::GetLastError());
				OutputDebugString(_T("Install test service error"));
				return -1;
			}

			_tprintf(_T("Service installed\n"));
			OutputDebugString(_T("install test service successful"));
			return 0;
		}

		if (strcmp(argv[1], ("uninstall")) == 0) {
			OutputDebugString(_T("uninstall test service"));
			_tprintf(_T("Uninstalling service\n"));
			if (!guru::unstall_service(service)) {
				_tprintf(_T("Couldn't uninstall service: %d\n"), ::GetLastError());
				OutputDebugString(_T("Can't uninstall test service"));
				return -1;
			}

			_tprintf(_T("Service uninstalled\n"));
			OutputDebugString(_T("uninstall test service successful"));
			return 0;
		}

		_tprintf(_T("Invalid argument\n"));
		return -1;
	}

	service.run();
	OutputDebugString(_T("end of main"));
	return 0;
}