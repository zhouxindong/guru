#include "stdafx.h"
#include "CppUnitTest.h"
#include "stream/null_stream.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace guru;
using namespace std;

namespace guru_unittest
{
	TEST_CLASS(null_stream_unittest)
	{
	public:

		TEST_METHOD(null_stream_test)
		{
			null_stream ns;
			ns << 33;
			ns << "char";
			ns << std::string("abcdefg");
		}

	};
}