#include "stdafx.h"
#include "CppUnitTest.h"
#include "grocery/low_chrono.h"
#include "grocery/chrono_ex.h"
#include <tuple>
#include <chrono>
#include <sstream>
#include <iomanip>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace guru;
using namespace std;

namespace guru_unittest
{
	TEST_CLASS(low_chrono_unittest)
	{
	public:

		TEST_METHOD(days_from_civil_test)
		{
			Assert::AreEqual(0, days_from_civil(1970, 1, 1));
			Assert::AreEqual(-1, days_from_civil(1969, 12, 31));
			Assert::AreEqual(1, days_from_civil(1970, 1, 2));

			Assert::AreEqual(-719528, days_from_civil(0, 1, 1));
		}

		TEST_METHOD(civil_from_days_test)
		{
			auto epoch = civil_from_days(0);
			Assert::AreEqual(1970, get<0>(epoch));
			Assert::AreEqual((uint32_t)1, get<1>(epoch));
			Assert::AreEqual((uint32_t)1, get<2>(epoch));

			epoch = civil_from_days(-1);
			Assert::AreEqual(1969, get<0>(epoch));
			Assert::AreEqual((uint32_t)12, get<1>(epoch));
			Assert::AreEqual((uint32_t)31, get<2>(epoch));

			epoch = civil_from_days(1);
			Assert::AreEqual(1970, get<0>(epoch));
			Assert::AreEqual((uint32_t)1, get<1>(epoch));
			Assert::AreEqual((uint32_t)2, get<2>(epoch));

			epoch = civil_from_days(-719528);
			Assert::AreEqual(0, get<0>(epoch));
			Assert::AreEqual((uint32_t)1, get<1>(epoch));
			Assert::AreEqual((uint32_t)1, get<2>(epoch));
		}

		TEST_METHOD(weekday_difference_test)
		{
			constexpr unsigned a[7][7] =
			{// -       Sun Mon Tue Wed Thu Fri Sat
				/*Sun*/{ 0,  6,  5,  4,  3,  2,  1 },
				/*Mon*/{ 1,  0,  6,  5,  4,  3,  2 },
				/*Tue*/{ 2,  1,  0,  6,  5,  4,  3 },
				/*Wed*/{ 3,  2,  1,  0,  6,  5,  4 },
				/*Thu*/{ 4,  3,  2,  1,  0,  6,  5 },
				/*Fri*/{ 5,  4,  3,  2,  1,  0,  6 },
				/*Sat*/{ 6,  5,  4,  3,  2,  1,  0 }
			};
			for (unsigned x = 0; x < 7; ++x)
				for (unsigned y = 0; y < 7; ++y)
					Assert::IsTrue(weekday_difference(x, y) == a[x][y]);
		}

		TEST_METHOD(low_chrono_test)
		{
			static_assert(days_from_civil(1970, 1, 1) == 0, "1970-01-01 is day 0");
			Assert::IsTrue(civil_from_days(0) == make_tuple(1970, 1, 1));
			static_assert(weekday_from_days(days_from_civil(1970, 1, 1)) == 4, "1970-01-01 is a Thursday");

			int ystart = -1000;
			int prev_z = days_from_civil(ystart, 1, 1) - 1;
			Assert::IsTrue(prev_z < 0);
			int prev_wd = weekday_from_days(prev_z);
			Assert::IsTrue(0 <= prev_wd && prev_wd <= 6);
			auto t0 = std::chrono::system_clock::now();
			for (int y = ystart; y <= -ystart; ++y)
			{
				for (unsigned m = 1; m <= 12; ++m)
				{
					unsigned e = last_day_of_month(y, m);
					for (unsigned d = 1; d <= e; ++d)
					{
						int z = days_from_civil(y, m, d);
						Assert::IsTrue(prev_z < z);
						Assert::IsTrue(z == prev_z + 1);
						int yp;
						unsigned mp, dp;
						std::tie(yp, mp, dp) = civil_from_days(z);
						Assert::IsTrue(y == yp);
						Assert::IsTrue(m == mp);
						Assert::IsTrue(d == dp);
						unsigned wd = weekday_from_days(z);
						Assert::IsTrue(0 <= wd && wd <= 6);
						Assert::IsTrue(wd == next_weekday(prev_wd));
						Assert::IsTrue(prev_wd == prev_weekday(wd));
						prev_z = z;
						prev_wd = wd;
					}
				}
			}
			auto t1 = std::chrono::system_clock::now();
			typedef std::chrono::duration<float> sec;

			auto diff1 = sec(t1 - t0).count();
			auto range = days_from_civil(1000000, 12, 31) - days_from_civil(-1000000, 1, 1);
		}

		TEST_METHOD(low_with_chrono_test)
		{
			using namespace std;
			using namespace std::chrono;
			typedef duration<int, ratio_multiply<hours::period, ratio<24>>> days;
			auto utc_offset = hours(8);  // my current UTC offset
										  // Get duration in local units
			auto now = system_clock::now().time_since_epoch() + utc_offset;
			// Get duration in days
			auto today = duration_cast<days>(now);
			int year;
			unsigned month;
			unsigned day;
			// Convert days into year/month/day
			std::tie(year, month, day) = civil_from_days(today.count());
			// Subtract off days, leaving now containing time since local midnight
			now -= today;
			auto h = duration_cast<hours>(now);
			now -= h;
			auto m = duration_cast<minutes>(now);
			now -= m;
			auto s = duration_cast<seconds>(now);
			now -= s;
			auto us = duration_cast<microseconds>(now);

			ostringstream oss;
			oss.fill('0');
			oss << "Today is "
				<< year << '-' << setw(2) << month << '-' << setw(2) << day
				<< " at " << setw(2) << h.count() << ':'
				<< setw(2) << m.count() << ':'
				<< setw(2) << s.count() << '.' << setw(6) << us.count() << '\n';

			std::string out = oss.str();
		}

		TEST_METHOD(low_with_chrono_test2)
		{
			using namespace std;
			using namespace std::chrono;
			typedef duration<int, ratio_multiply<hours::period, ratio<24>>> days;
			auto utc_offset = hours(8);
			// Build a time point in local days::hours::minutes and then convert to UTC
			auto wakeup_at = system_clock::time_point(days(days_from_civil(1962, 8, 4))
				+ hours(10) + minutes(12) + seconds(0)/* - utc_offset*/);

			auto tim = wakeup_at.time_since_epoch();
			auto dayy = duration_cast<days>(tim);
			int year;
			unsigned month;
			unsigned day;
			std::tie(year, month, day) = civil_from_days(dayy.count());

			tim -= dayy;
			auto h = duration_cast<hours>(tim);
			tim -= h;
			auto m = duration_cast<minutes>(tim);
			tim -= m;
			auto s = duration_cast<seconds>(tim);
			tim -= s;
			auto us = duration_cast<microseconds>(tim);

			ostringstream oss;
			oss.fill('0');
			oss << "Today is "
				<< year << '-' << setw(2) << month << '-' << setw(2) << day
				<< " at " << setw(2) << h.count() << ':'
				<< setw(2) << m.count() << ':'
				<< setw(2) << s.count() << '.' << setw(6) << us.count() << '\n';

			std::string out = oss.str();
		}

		TEST_METHOD(low_with_chrono_test3)
		{
			using namespace std;
			using namespace std::chrono;
			typedef duration<int, ratio_multiply<hours::period, ratio<24>>> days;

			constexpr days epoch = days(days_from_civil(0, 1, 1));  // 0000-01-01
			//auto delta = std::chrono::system_clock::now().time_since_epoch() - epoch;

			auto wakeup_at = system_clock::time_point(days(days_from_civil(1962, 8, 4))
				+ hours(20) + minutes(12) + seconds(33)/* - utc_offset*/);
			auto delta = wakeup_at.time_since_epoch() - epoch;

			auto tim = wakeup_at.time_since_epoch();
			auto dayy = duration_cast<days>(tim);
			int year;
			unsigned month;
			unsigned day;
			std::tie(year, month, day) = civil_from_days(dayy.count());

			days d = std::chrono::duration_cast<days>(delta);
			ostringstream oss;
			oss << "It has been " << d.count() << " days, ";
			delta -= d;
			auto h = std::chrono::duration_cast<std::chrono::hours>(delta);
			oss << h.count() << " hours, ";
			delta -= h;
			auto m = std::chrono::duration_cast<std::chrono::minutes>(delta);
			oss << m.count() << " minutes, ";
			delta -= m;
			auto s = std::chrono::duration_cast<std::chrono::seconds>(delta);
			oss << s.count() << " seconds ";
			oss << " since 0000-01-01\n";

			std::string out = oss.str();
		}
	};
}