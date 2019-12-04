#include "stdafx.h"
#include "CppUnitTest.h"
#include "zlog/log_item.h"
#include "zlog/log_formatter.h"
#include "zlog/log_channel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace guru;

namespace guru_unittest
{
	TEST_CLASS(channel_unittest)
	{
	public:

		TEST_METHOD(null_channel_test)
		{			
			log_item log1 = LOG(ERROR);
			log1 << "some information";
			log1 << " " << 3456;
			std::string s = std_formatter<>::format(log1);
			Dummy_Channel.stream() << s;
		}

	};
}