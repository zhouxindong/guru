#include "stdafx.h"
#include "CppUnitTest.h"
#include "base_chain/non_instantable.h"
#include "base_chain/non_copyable.h"
#include "base_chain/prefixable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace guru;

namespace guru_unittest
{
	TEST_CLASS(base_chain_unittest)
	{
		class B {
		public:
			B(int i = 0) {}
			int show(int a) {
				return a + 1;
			}

			static int foo() {
				return 15;
			}
		};

	public:

		TEST_METHOD(non_instantable_test)
		{
			B b1;
			B b2(b1);
			B b3 = b2;
			Assert::IsTrue(15 == non_instantable<B>::foo());
			//non_instantable<B> b2(33); // can't be instantized
		}

		TEST_METHOD(non_copyable_test)
		{
			non_copyable<B> b1;
			//non_copyable<B> b2(b1); // throw non_copyable_exception
			//non_copyable<B> b3 = b1; // compile error
			non_copyable<B> b4(33);
		}

		template <typename _T>
		struct prefix
		{
			void operator() ()
			{
				++_T::_id;
			}
		};

		class C
		{
		public:
			static int _id;

			int id()
			{
				return _id;
			}
		};


		TEST_METHOD(prefixable_test)
		{
			prefix<C> pre;
			prefixable<C, prefix<C>> x(&pre);
			Assert::IsTrue(16 == x->id());
			Assert::IsTrue(17 == x->id());
		}
	};

	int base_chain_unittest::C::_id = 15;

}