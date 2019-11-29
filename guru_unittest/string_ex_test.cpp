#include "stdafx.h"
#include "CppUnitTest.h"
#include "grocery/string_ex.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace guru;

namespace guru_unittest
{
	TEST_CLASS(string_ex_unittest)
	{
	public:

		TEST_METHOD(split_test)
		{
			auto v = split("", ',');
			Assert::IsTrue(v.empty());

			v = split("abcd", ',');
			Assert::IsTrue(1 == v.size());
			Assert::IsTrue("abcd" == v[0]);

			v = split("ab,cd", ',');
			Assert::IsTrue(2 == v.size());
			Assert::IsTrue("ab" == v[0]);
			Assert::IsTrue("cd" == v[1]);

			v = split("a,bcd,", ',');
			Assert::IsTrue(2 == v.size());
			Assert::IsTrue("a" == v[0]);
			Assert::IsTrue("bcd" == v[1]);
		}
	};
}