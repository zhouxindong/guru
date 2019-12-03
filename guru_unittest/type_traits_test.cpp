#include "stdafx.h"
#include "CppUnitTest.h"
#include "trick/type_traits.h"
#include <typeinfo>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace guru;

namespace guru_unittest
{
	class foo 
	{
	public:
		double number;
		int get_me(double me) {}
	};

	TEST_CLASS(type_traits_unittest)
	{
	public:

		TEST_METHOD(is_funda_type_test)
		{
			Assert::IsTrue(is_funda_type<int>::value);
			Assert::IsFalse(is_funda_type<foo>::value);
		}

		TEST_METHOD(compound_type_test)
		{
			Assert::IsFalse(is_reference<compound_type<int>::base_type>::value);
			Assert::IsFalse(is_reference<compound_type<int>::bottom_type>::value);
			Assert::IsTrue(is_reference<compound_type<int&>::base_type>::value);
			Assert::IsFalse(is_reference < compound_type<int&>::bottom_type>::value);
			Assert::IsTrue(is_pointer<compound_type<int*>::base_type>::value);
			Assert::IsFalse(is_pointer < compound_type<int*>::bottom_type>::value);
			Assert::IsTrue(is_array<compound_type<int[6]>::base_type>::value);
			Assert::IsFalse(is_array<compound_type<int[6]>::bottom_type>::value);

			double foo::*pnum;
			Assert::IsTrue(is_member_pointer<decltype(pnum)>::value);
			Assert::IsTrue(is_member_pointer<double foo::*>::value);
			Assert::IsTrue(is_member_pointer<compound_type<double foo::*>::base_type>::value);

			Assert::IsTrue(is_same<decltype(pnum), compound_type<double foo::*>::base_type>::value);
			Assert::IsTrue(is_same<double, compound_type<double foo::*>::bottom_type>::value);

			int (foo::*pf)(double);
			Assert::IsTrue(is_member_pointer<decltype(pf)>::value);
			Assert::IsTrue(is_member_pointer<int (foo::*)(double)>::value);
			Assert::IsTrue(is_member_pointer<compound_type<int (foo::*)(double)>::base_type>::value);

			Assert::IsTrue(is_same<decltype(pf), compound_type<int (foo::*)(double)>::base_type>::value);
			Assert::IsTrue(is_same<int(double), compound_type<int (foo::*)(double)>::bottom_type>::value);
		}
	};
}