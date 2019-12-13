// socket_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "socket/niceer.h"
#include <iostream>
#include "socket/udp_sender.h"
#include "base_feature/lock_fixable.h"
#include <future>
#include "socket/udp_recver.h"
#include "grocery/ostream_ex.h"
#include "socket/multi_sender.h"
#include "socket/multi_recver.h"
#include "socket/broad_sender.h"
#include "socket/broad_recver.h"
#include "socket/tcp_conn.h"
#include <initializer_list>

using namespace std;
using namespace guru;

// niceer
/*
int main()
{
	niceer local_machine;
	auto nics = local_machine.nic_list();

	for_each(nics.begin(), nics.end(),
		[](niceer::NIC nic) {
		cout << nic.name() << endl;
		cout << nic.desc() << endl;
		cout << nic.type() << endl;
		cout << nic.mac() << endl;

		auto ips = nic.ip_list();
		for_each(ips.begin(), ips.end(),
			[](niceer::IP ip) {
			cout << "ip:" << ip.ip() << endl;
			cout << "mask: " << ip.mask() << endl;
			cout << "gate: " << ip.gate() << endl;
		});

		cout << "------------------------------------------------" << endl;
	});

	std::string ip = "10.16.2.25";
	cout << local_machine.broad_ip(ip) << endl;

	system("pause");
    return 0;
}
*/

// udp_sender
/*
udp_sender<lock_fixable> us("10.16.2.25", 8005, "10.16.2.25");
bool sending = false;
void thread_proc()
{
	std::string s("aaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	while (sending)
	{
		us.send(s.c_str(), (int)s.size());
		Sleep(200);
	}
}

bool sending2 = false;
void thread_proc2()
{
	std::string s("hello ccccccccccccccccccc");
	while (sending2)
	{
		us.send(s.c_str(), (int)s.size());
		Sleep(200);
	}
}

int main()
{
	sending = true;
	sending2 = true;
	std::future<void> rs(std::async(std::launch::async, thread_proc));
	std::future<void> rs2(std::async(std::launch::async, thread_proc2));
	system("pause");
	sending = false;
	system("pause");
	sending2 = false;
	return 0;
}
*/

// udp_recver
/*
udp_recver<4096> ur("10.16.2.25", 60000);
void recv_proc()
{
	ur.start();
}

void received_data(vector<char> v)
{
	cout << v;
}

int main()
{
	ur.add_listen(received_data);
	std::future<void> f(std::async(std::launch::async, recv_proc));
	system("pause");
	ur.stop();
	return 0;
}
*/

// multi_sender
/*
multi_sender<lock_fixable> ms("234.1.2.3", 8058, "10.16.2.25", 12345);
bool sending = false;
void thread_proc()
{
	sending = true;
	char buf[128];
	int i = 0;
	while (sending)
	{
		sprintf(buf, "#:%d", ++i);
		ms.send(buf, sizeof(buf));
		cout << buf << endl;
		Sleep(200);
	}
}
int main()
{
	std::future<void> f(std::async(std::launch::async, thread_proc));
	system("pause");
	sending = false;
	return 0;
}
*/

// multi_recver
/*
multi_recver<> mr("239.255.0.1", 8058, "10.16.2.25");
void recv_proc()
{
	mr.start();
}

void received_data(vector<char> v)
{
	cout << v;
}

int main()
{
	mr.add_listen(received_data);
	std::future<void> f(std::async(std::launch::async, recv_proc));
	system("pause");
	mr.stop();
	return 0;
}
*/

// broad_sender
/*
broad_sender<lock_fixable> bs(3779, "10.16.2.25");
bool sending = false;
void thread_proc()
{
	sending = true;
	char buf[4];
	int i = 0;
	while (sending)
	{
		sprintf(buf, "%d", ++i);
		bs.send(buf, sizeof(buf));
		Sleep(300);
	}
}
int main()
{
	std::future<void> f(std::async(std::launch::async, thread_proc));
	system("pause");
	sending = false;
	return 0;
}
*/

// borad_recver
/*
broad_recver<> br(3779, "10.16.2.25");
void recv_proc()
{
	br.start();
}
void received_data(vector<char> v)
{
	cout << v;
}
int main()
{
	br.add_listen(received_data);
	std::future<void> f(std::async(std::launch::async, recv_proc));
	system("pause");
	br.stop();
	return 0;
}
*/

// tcp_conn
int main()
{
	SOCKET s;
	SOCKADDR_IN a;

	std::initializer_list<uint8_t> head{ 1,2,3,4 };
	std::initializer_list<uint8_t> tail{ 4,3,2,1 };

	tcp_conn<head_tail_stream<>> con1(s, a, head, tail);

	system("pause");
	return 0;
}