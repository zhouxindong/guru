#include "stdafx.h"
#include "CppUnitTest.h"
#include "log/log_level.h"
#include "log/log_item.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace guru;

namespace guru_unittest
{
	TEST_CLASS(log_unittest)
	{
	public:

		TEST_METHOD(log_level_test)
		{
			Assert::IsTrue(string("TRACE") == string(log_level(_LOG_TRACE)));
			Assert::IsTrue(string("DEBUG") == string(log_level(_LOG_DEBUG)));
			Assert::IsTrue(string("INFO") == string(log_level(_LOG_INFO)));
			Assert::IsTrue(string("WARN") == string(log_level(_LOG_WARN)));
			Assert::IsTrue(string("ERROR") == string(log_level(_LOG_ERROR)));
			Assert::IsTrue(string("FATAL") == string(log_level(_LOG_FATAL)));
		}

		TEST_METHOD(log_item_test)
		{
			log_item item(_LOG_FATAL, , __LINE__);
			item << "some test";
			int i = 100;
			item << i;
			auto s = item.message();

			log_item item2(item);

			log_item item3(std::move(item2));

			Assert::IsTrue(item.get_level() == item3.get_level());
			Assert::IsTrue(item.get_caller() == item3.get_caller());
			Assert::AreEqual(item.get_line(), item3.get_line());
			Assert::AreEqual(item.get_pid(), item3.get_pid());
			Assert::IsTrue(item.message() == item3.message());
		}
	};
}