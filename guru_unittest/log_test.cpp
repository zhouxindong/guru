#include "stdafx.h"
#include "CppUnitTest.h"
#include "zlog/log_level.h"
#include "zlog/log_item.h"

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
	};
}