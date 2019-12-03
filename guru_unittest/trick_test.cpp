#include "stdafx.h"
#include "CppUnitTest.h"
#include "trick/countable_CRTP.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace guru;

namespace guru_unittest
{
	TEST_CLASS(trick_unittest)
	{
	public:

		TEST_METHOD(countable_test)
		{
			nontempl_class n1;
			Assert::IsTrue(nontempl_class::live() == 1);
			nontempl_class n2;
			Assert::IsTrue(nontempl_class::live() == 2);
			nontempl_class* p3 = new nontempl_class;
			Assert::IsTrue(nontempl_class::live() == 3);
			delete p3;
			Assert::IsTrue(nontempl_class::live() == 2);

			templ_class<int> t1, t2, t3;
			Assert::IsTrue(templ_class<int>::live() == 3);
			templ_class<int> t4 = t3;
			Assert::IsTrue(templ_class<int>::live() == 4);
			t2 = t4;
			Assert::IsTrue(templ_class<int>::live() == 4);
			templ_class<int>* p5 = new templ_class<int>;
			Assert::IsTrue(templ_class<int>::live() == 5);
			delete p5;
			Assert::IsTrue(templ_class<int>::live() == 4);

			templ_class<double> d1;
			Assert::IsTrue(templ_class<double>::live() == 1);
		}

	};
}