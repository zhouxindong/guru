#include "stdafx.h"
#include "CppUnitTest.h"
#include "mp/pow.h"
#include "mp/sqrt.h"
#include "mp/loop.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace guru;

namespace guru_unittest
{
	TEST_CLASS(pow_unittest)
	{
	public:

		TEST_METHOD(pow_test)
		{
			Assert::IsTrue(27 == guru::pow<3, 3>::value);
			Assert::IsTrue(1 == guru::pow<0, 0>::value);
			Assert::IsTrue(1 == guru::pow<1, 10>::value);
		}

		TEST_METHOD(sqrt_test)
		{
			Assert::IsTrue(4 == guru::sqrt<16>::value);
			Assert::IsTrue(5 == guru::sqrt<25>::value);
			Assert::IsTrue(6 == guru::sqrt<42>::value);
			Assert::IsTrue(1 == guru::sqrt<1>::value);

			Assert::IsTrue(4 == guru::sqrt2<16>::value);
			Assert::IsTrue(5 == guru::sqrt2<25>::value);
			Assert::IsTrue(6 == guru::sqrt2<42>::value);
			Assert::IsTrue(1 == guru::sqrt2<1>::value);

			Assert::IsTrue(4 == guru::sqrt3<16>::value);
			Assert::IsTrue(5 == guru::sqrt3<25>::value);
			Assert::IsTrue(7 == guru::sqrt3<42>::value);
			Assert::IsTrue(1 == guru::sqrt3<1>::value);
		}

		TEST_METHOD(loop_test)
		{
			int a[3] = { 1,2,3 };
			int b[3] = { 5,6,7 };
			Assert::IsTrue(5 + 12 + 21 == guru::dot_product<3, int>::value(a, b));
		}
	};
}