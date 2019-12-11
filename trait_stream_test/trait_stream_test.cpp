// trait_stream_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "stream/head_tail_stream.h"
#include "stream/stream_generator.h"
#include <iostream>
#include "grocery/ostream_ex.h"
#include <future>
#include "stream/transparent_stream.h"

using namespace std;
using namespace guru;

// head_tail_stream normal test
/*
stream_generator<32> gen1;
void matched(vector<uint8_t> v)
{
	std::cout << v;
}

void start()
{
	gen1.start();
}

int main()
{
	head_tail_stream<> stream{ {1,2,3,4}, {4,3,2,1} };
	gen1.set_head({ 1,2,3,4 });
	gen1.set_tail({ 4,3,2,1 });
	stream.set_src_signal(gen1.signal());
	stream.matched_signal().connect(matched);
	std::future<void> f(std::async(std::launch::async, start));

	system("pause");
	gen1.stop();

	return 0;
}
*/

// head_tail_stream over MAX size body
/*
stream_generator<4096, 200> gen1;
void matched(vector<uint8_t> v)
{
	std::cout << v;
}

void start()
{
	gen1.start();
}

int main()
{
	head_tail_stream<5010> stream{ {11,22,33,44}, {44,33,22,11} };
	gen1.set_head({ 11,22,33,44 });
	gen1.set_tail({ 44,33,22,11 });
	stream.set_src_signal(gen1.signal());
	stream.matched_signal().connect(matched);
	std::future<void> f(std::async(std::launch::async, start));

	system("pause");
	gen1.stop();
	return 0;
}
*/

// customed stream_generater#1
/*
template <size_t _Volumn, size_t _Interval = 1000>
class double_stream : public stream_generator<_Volumn, _Interval>
{
protected:
	bool 
		_before_emit() noexcept override
	{ 
		std::vector<uint8_t> v;
		v.resize(_buf.size() * 2);
		auto it = v.begin();
		copy(_buf.cbegin(), _buf.cend(), it);
		advance(it, _buf.size());
		copy(_buf.cbegin(), _buf.cend(), it);
		stream_generator<_Volumn, _Interval>::_signal(v);
		Sleep(100);
		return true;
	}
};

double_stream<16, 1000> gen1;
void matched(vector<uint8_t> v)
{
	std::cout << v;
}

void start()
{
	gen1.start();
}
int main()
{
	head_tail_stream<> stream{ {11,22}, {22,11} };
	gen1.set_head({ 11,22 });
	gen1.set_tail({ 22,11 });
	stream.set_src_signal(gen1.signal());
	stream.matched_signal().connect(matched);
	std::future<void> f(std::async(std::launch::async, start));

	system("pause");
	gen1.stop();
	return 0;
}
*/

// customed stream_generater#2
/*
template <size_t _Volumn, size_t _Interval = 200>
class ball_stream : public stream_generator<_Volumn, _Interval>
{
protected:
	bool
		_before_emit() noexcept override
	{
		std::vector<uint8_t> v;
		for_each(_buf.cbegin(), _buf.cend(), [&](auto val) {
			v.resize(1);
			v[0] = val;
			stream_generator<_Volumn, _Interval>::_signal(v);
			Sleep(1);
		});
		return false;
	}
};

ball_stream<128> gen1;
void matched(vector<uint8_t> v)
{
	std::cout << v;
}

void start()
{
	gen1.start();
}
int main()
{
	head_tail_stream<> stream{ {0xEB, 0x90}, {0xEE,0XFF} };
	gen1.set_head({ 0xEB, 0x90 });
	gen1.set_tail({ 0xEE,0XFF });
	stream.set_src_signal(gen1.signal());
	stream.matched_signal().connect(matched);
	std::future<void> f(std::async(std::launch::async, start));

	system("pause");
	gen1.stop();
	return 0;
}
*/

// transparent_stream
///*
stream_generator<32> gen1;
void matched(vector<uint8_t> v)
{
	std::cout << v;
}

void start()
{
	gen1.start();
}

int main()
{
	transparent_stream<> stream{};
	stream.set_src_signal(gen1.signal());
	stream.matched_signal().connect(matched);
	std::future<void> f(std::async(std::launch::async, start));

	system("pause");
	gen1.stop();

	return 0;
}
//*/

