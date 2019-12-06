// nut_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "nut/signal.h"
#include <iostream>
#include "signal_af.h"

using namespace std;
using namespace guru;

// signal test
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

